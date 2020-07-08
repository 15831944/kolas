// KOLISNetWebSearchMgr.cpp : implementation file
//

#include "stdafx.h"
#include "KOLISNetWebSearchMgr.h"
#include "..\..\include\�������\RS_Acc.h"

// 2017/09/22 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
// KOLIS-NET OpenAPI
/*//NEW------------------------------------------------------------------------*/
#include "KolisNetService.h"
/*//END------------------------------------------------------------------------*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKOLISNetWebSearchMgr dialog


CKOLISNetWebSearchMgr::CKOLISNetWebSearchMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CKOLISNetWebSearchMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bISMoveWindow = FALSE;
	m_pBOSpeciesApi = NULL;
	m_pComboMgr = NULL;
	m_pComboMgr2 = NULL;
	m_ctlSlider = NULL;
}

CKOLISNetWebSearchMgr::~CKOLISNetWebSearchMgr()
{	
	if ( m_pComboMgr != NULL ) delete m_pComboMgr;
	if ( m_pComboMgr2 != NULL ) delete m_pComboMgr2;
	if ( m_pBOSpeciesApi != NULL ) delete m_pBOSpeciesApi;
	if ( m_ctlSlider != NULL ) delete m_ctlSlider;

}


void CKOLISNetWebSearchMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKOLISNetWebSearchMgr)		
	DDX_Control(pDX, IDC_PROGRESS1, m_ctlProgress);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_btnImport	, m_btnImport);
	DDX_Control(pDX, IDC_btnHide	, m_btnHIDE);	
	//DDX_Control(pDX, IDC_btnExport	, m_btnExport);
	//DDX_Control(pDX, IDC_bClear		, m_btnClear);
}


BEGIN_MESSAGE_MAP(CKOLISNetWebSearchMgr, CDialog)
	//{{AFX_MSG_MAP(CKOLISNetWebSearchMgr)
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_bClear, OnbClear)
	ON_BN_CLICKED(IDC_btnImport, OnbtnImport)
	ON_BN_CLICKED(IDC_btnHide, OnbtnHide)
	ON_WM_SETCURSOR()
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_USER + 862, OnMyMethod)
	ON_CBN_SELCHANGE(IDC_comboWorkCode, OnEditchangecWORKCODE)
	ON_EN_KILLFOCUS(IDC_eYear, OnKillfocusedtYear)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_THREAD_END, OnResultDisplay)	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKOLISNetWebSearchMgr message handlers

BOOL CKOLISNetWebSearchMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	if (InitESL_Mgr(_T("SM_KOLISNET_WEB_SEARCH")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	
	CESL_Grid *pGrid; 
	pGrid = ( CESL_Grid * )FindControl( _T("CM_KOLISNET_WEB_SEARCH"), _T("MainGrid") );	
	pGrid->SetColWidth(0,0,250);	

	m_bmpTop.LoadBMP(_T("..\\bmp\\kolisnetwebsearch.bmp"));
	m_bmpBottom.LoadBMP(_T("..\\bmp\\kolisnetwebsearchbottom.bmp"));
	m_bmpMiddle.LoadBMP(_T("..\\bmp\\kolisnetwebsearchmiddle.bmp"));
	m_btnImport.LoadBitmaps("..\\BMP\\kolisnetwebsearch_import.bmp", RGB(0,0,0));
	m_btnHIDE.LoadBitmaps("..\\BMP\\x.bmp", RGB(0,0,0));
	//m_btnExport.LoadBitmaps("..\\BMP\\IeSearch.bmp", RGB(0,0,0));
	//m_btnClear.LoadBitmaps("..\\BMP\\IeSearch.bmp", RGB(0,0,0));
	m_strISBNList = _T("");

	CString strYear;
	COleDateTime datetime = COleDateTime::GetCurrentTime();
	strYear.Format(_T("%04d") , datetime.GetYear());

	( (CEdit*) GetDlgItem (IDC_eYear) )->SetWindowText(strYear);

	m_ctlSlider = new CSlider;
	m_ctlSlider->Create(this, 165,17, _T("KolisnetWebSearch") );		

	InitControl();

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CKOLISNetWebSearchMgr::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	m_pParent = pParentWnd;
	
	return CDialog::Create(IDD, pParentWnd);
}


INT CKOLISNetWebSearchMgr::Search(CStringArray &pISBNList)
{		
	//m_pParent->SendMessage(WM_USER+1024, NULL, NULL);

	// ISBN�� ���� ��� �˻�â�� �ʱ�ȭ ��Ų��.

	if ( pISBNList.GetSize() == 0 )
	{
		CESL_ControlMgr *pCM= FindCM(_T("CM_KOLISNET_WEB_SEARCH"));
		CESL_DataMgr* pDM_Result = FindDM(_T("DM_KOLISNET_WEB_SEARCH"));
		if ( pDM_Result->GetRowCount() > 0 )
		{
			pDM_Result->FreeData();
			pCM->AllControlDisplay();			
		}

		m_arrayOldISBNList.RemoveAll();
		return 0;
	}
	
	// ������ �˻����� ����
	// ������ ���ٸ� ��˻� �� �ʿ� ����
	if ( pISBNList.GetSize() == m_arrayOldISBNList.GetSize() )
	{
		BOOL bSame;

		bSame = TRUE;
		for ( int i=0 ; i<pISBNList.GetSize() ; i++ )
		{
			if ( pISBNList.GetAt(i) != m_arrayOldISBNList.GetAt(i) )
			{
				bSame = FALSE;
				break;
			}
		}
		if ( bSame == TRUE) return 0;
	}

	m_strISBNList = GetISBNCondition(pISBNList, 2);

	CKolisNetSearchParam* pSearchParam = m_BoSearchFish.GetKolisNetSearchParam();
	pSearchParam->ReleaseParam();
	pSearchParam->SetSORT_PUB_YEAR(CKolisNetSearchParam::SORT_TYPE::SORT_TYPE_DESC);
	pSearchParam->SetISBN(m_strISBNList);

	CKolisNetDataMapper* pMapper = m_BoSearchFish.GetKolisNetDataMapper();
	pMapper->ReleaseData();
	pMapper->AddData(KOLISNET_PARAM_AUTHOR_INFO, _T("������"));
	pMapper->AddData(KOLISNET_PARAM_CONTROL_NO, _T("�����ȣ"));
	pMapper->AddData(KOLISNET_PARAM_FORM_CODE, _T("���ı���"));
	pMapper->AddData(KOLISNET_PARAM_MAT_CODE, _T("�ڷᱸ��"));
	pMapper->AddData(KOLISNET_PARAM_MEDIA_CODE, _T("��ü����"));	
	pMapper->AddData(KOLISNET_PARAM_PUB_FREQ, _T("�����"));
	pMapper->AddData(KOLISNET_PARAM_PUB_INFO, _T("������"));
	pMapper->AddData(KOLISNET_PARAM_PUB_YEAR_INFO, _T("�����"));
	pMapper->AddData(KOLISNET_PARAM_TITLE_INFO, _T("ǥ��"));
	pMapper->AddData(KOLISNET_PARAM_ID, _T("����������Ű"));
	pMapper->AddData(KOLISNET_PARAM_TOC_YN, _T("��������"));
	pMapper->AddData(KOLISNET_PARAM_ABS_YN, _T("�ʷ�����"));
	pMapper->AddData(KOLISNET_PARAM_ISBN, _T("ISBN"));

	TCHAR   szDanCollectionName[128];  
	_tcscpy(szDanCollectionName , _T("dan-gm,dan-limit,dan-etc,dan-rb,dan-ca,dan-dmdp,dan-ha,dan-hr"));
	TCHAR   szBoQuery[1024];
	_stprintf(szBoQuery, _T("(%s) <in> zst_code"), m_strISBNList );	

	INT ids = m_BoSearchFish.Initialize (this , &m_ctlProgress , _T("����������˻�") , _T("DM_�����ڷ�_DB") , _T("CM_KOLISNET_WEB_SEARCH") 
		, _T("MainGrid") , szDanCollectionName, szBoQuery, m_pBoResultCtl 
		, 2000 , 2000 , FALSE , pSearchParam , pMapper);	
	if ( ids != 0 ) return ids;

	m_BoSearchFish.InitDiaplay( );
	m_BoSearchFish.SetAppendDisplay( FALSE );
	m_pBoThread = m_BoSearchFish.SearchDiaplay( );

	m_pISBNList.RemoveAll();
	for ( int i=0 ; i<pISBNList.GetSize() ; i++ )
	{
		m_pISBNList.Add(pISBNList.GetAt(i));
	}
	
	return 0;
}

INT CKOLISNetWebSearchMgr::SearchPos()
{
	CString strWhere;
	CString strMsg;
	// �˻� ����� ���� ��� �������� ����� POS�����͸� �˻��Ѵ�.
	CESL_DataMgr* pKNDM = FindDM(_T("DM_KOLISNET_WEB_SEARCH"));

	CESL_DataMgr* pPosDM = FindDM(_T("DM_KOLISNET_POS"));
	pPosDM->FreeData();

	m_ctlProgress.SetWindowText(_T("�������� POS �˻���..."));

	CKolisNetDataMapper Mapper;
	Mapper.AddData(KOLISNET_PARAM_BSP_KEY, "REC_KEY");
	Mapper.AddData(KOLISNET_PARAM_BOOK_ID, "BOOK_ID");
	Mapper.AddData(KOLISNET_PARAM_TITLE, "TITLE");
	Mapper.AddData(KOLISNET_PARAM_PUBLISHER, "PUBLISHER");
	Mapper.AddData(KOLISNET_PARAM_AUTHOR, "AUTHOR");
	Mapper.AddData(KOLISNET_PARAM_ISBN, "ISBN");
	Mapper.AddData(KOLISNET_PARAM_EAN13, "EAN13");
	Mapper.AddData(KOLISNET_PARAM_CLASS_CODE, "CLASS");
	Mapper.AddData(KOLISNET_PARAM_PRICE, "PRICE");
	Mapper.AddData(KOLISNET_PARAM_PUBLISH_DATE, "PUBLISH_DATE");
	Mapper.AddData(KOLISNET_PARAM_EDITION, "EDITION");
	Mapper.AddData(KOLISNET_PARAM_CREATE_DATE, "CREATE_DATE");

	CKolisNetService::GetBtpSpeciesPosInfo(this, GetISBNCondition(m_pISBNList, 2), pPosDM, &Mapper);

	
	if ( pPosDM->GetRowCount() > 0 )
	{
		strMsg.Format(_T("�˻���� %d��"), pPosDM->GetRowCount() );		
		m_ctlProgress.SetWindowText(strMsg);
	}
	else
	{
		m_ctlProgress.SetWindowText(_T("�˻���� ����"));
	}

	
	if ( m_pISBNList.GetSize() > 0 )
	{		
		m_arrayOldISBNList.RemoveAll();
		// �˻����� ISBN����
		for ( int i=0 ; i<m_pISBNList.GetSize() ; i++ )
		{
			m_arrayOldISBNList.Add(m_pISBNList.GetAt(i));			
		}
	}	
	else
	{
		// �˻����� ISBN����
		m_arrayOldISBNList.RemoveAll();
	}
	

	return 0;
}

VOID CKOLISNetWebSearchMgr::OnResultDisplay()
{
	CESL_ControlMgr *pCM= FindCM(_T("CM_KOLISNET_WEB_SEARCH"));
	
	CESL_DataMgr* pDM = FindDM(_T("DM_�����ڷ�_DB"));
	CESL_DataMgr* pPosDM = FindDM(_T("DM_KOLISNET_POS"));	
	CESL_DataMgr* pDM_Result = FindDM(_T("DM_KOLISNET_WEB_SEARCH"));

	CString strTitleInfo;
	CString strAuthor;
	CString strPublisher;
	CString strPubYear;
	CString strBookInfo;
	CString strISBN;
	CString strHOLDISBN;
	CString strHOLD_YN;

	pDM_Result->FreeData();

	CESL_DataMgr *pDM_Hold= FindDM(_T("DM_KOLISNET_HOLD_SEARCH"));			
	
	CString strWhere;		
	strISBN = GetISBNCondition(m_pISBNList, 1);
	
	if ( strISBN.GetLength() > 0 )
	{
		strWhere.Format(_T("ST_CODE in (%s)"), strISBN );
		pDM_Hold->RefreshDataManager(strWhere);
	}

	INT i;
	for ( i=0 ; i<pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData(_T("ǥ��")		, i, strTitleInfo);
		pDM->GetCellData(_T("������")	, i, strAuthor);
		pDM->GetCellData(_T("������")	, i, strPublisher);
		pDM->GetCellData(_T("�����")	, i, strPubYear);		
		pDM->GetCellData(_T("ISBN")		, i, strISBN);		

		strBookInfo = strTitleInfo + _T(" / ") +
					strAuthor + _T(" / ") +	strPublisher + _T(", ") + strPubYear;
		pDM_Result->InsertRow(-1);
		pDM_Result->SetCellData(_T("å����")	, strBookInfo, i);
		pDM_Result->SetCellData(_T("ISBN")		, strISBN, i);

		
		strHOLD_YN = _T("");
		for ( int y=0 ; y<pDM_Hold->GetRowCount() ; y++ )
		{
			pDM_Hold->GetCellData(_T("ST_CODE")	, y, strHOLDISBN);			
			strHOLDISBN.Replace(_T("-"), _T(""));
			strISBN.Replace(_T("-"), _T(""));

			if ( strHOLDISBN.Left(3) == _T("978") )
			{
				if ( strHOLDISBN.Mid(3) == strISBN || strHOLDISBN == strISBN )
				{
					strHOLD_YN = _T("O");
					break;
				}
			}					
			else if ( _T("978")+strHOLDISBN == strISBN || strHOLDISBN == strISBN ) 
			{
				strHOLD_YN = _T("O");
				break;
			}
		}
		
		pDM_Result->SetCellData(_T("���忩��")	, strHOLD_YN, i);
		
		pDM_Result->SetCellData(_T("�˻���ġ")	, _T("KOLISNET"), i);		
	}
	
	SearchPos();
	INT nIdx;
	nIdx = pDM->GetRowCount();

	for ( i=0 ; i<pPosDM->GetRowCount() ; i++ )
	{
		pPosDM->GetCellData(_T("TITLE")			, i, strTitleInfo);
		pPosDM->GetCellData(_T("AUTHOR")		, i, strAuthor);
		pPosDM->GetCellData(_T("PUBLISHER")		, i, strPublisher);
		pPosDM->GetCellData(_T("PUBLISH_DATE")	, i, strPubYear);			

		strBookInfo = strTitleInfo + _T(" / ") +
					strAuthor + _T(" / ") +	strPublisher + _T(", ") + strPubYear;
		pDM_Result->InsertRow(-1);
		pDM_Result->SetCellData(_T("å����")		, strBookInfo, nIdx+i);
		pDM_Result->SetCellData(_T("���忩��")		, strHOLD_YN, nIdx+i);	
		pDM_Result->SetCellData(_T("�˻���ġ")		, _T("�������������"), nIdx+i);
	}
	pCM->AllControlDisplay();

	CESL_Grid *pGrid; 
	pGrid = ( CESL_Grid * )FindControl( _T("CM_KOLISNET_WEB_SEARCH"), _T("MainGrid") );	
	pGrid->SetRowHeight(0,450);		 
	for ( i=0 ; i<pDM->GetRowCount() ; i++ )
	{
		pGrid->SetRowHeight(i+1,800);		
	}	
	for ( i=0 ; i<pPosDM->GetRowCount() ; i++ )
	{
		pGrid->SetRowHeight(i+1,800);		
	}	

	CString strProgressText;
	m_ctlProgress.GetWindowText(strProgressText);
	if ( strProgressText == _T("�˻��� �Դϴ�    ") )
	{
		m_ctlProgress.SetWindowText(_T("�˻���� ����"));
	}	

	CString strMsg;
	strMsg.Format(_T("�˻���� %d��"), pDM->GetRowCount() + pPosDM->GetRowCount() );		
	m_ctlProgress.SetWindowText(strMsg);

	if ( m_pParent->GetSafeHwnd() ) m_pParent->SendMessage(WM_USER+1025, NULL, NULL);
	m_ctlProgress.ShowWindow(SW_SHOW);
}

CString CKOLISNetWebSearchMgr::GetISBNCondition(CStringArray &pISBNList, INT nType)
{
	CString strCondition;
	CString strISBN;
	CString strConvertISBN;
	CString strOutput;
	INT		ids;

	strCondition = _T("");
	for ( int i=0 ; i<pISBNList.GetSize() ; i++ )
	{
		strISBN = pISBNList.GetAt(i);
		ids = CheckISBN(strISBN, strOutput);
		if ( ids < 0 ) continue;

		strConvertISBN.Format(_T("%s"), strOutput);
		strConvertISBN.Replace(_T("-"), _T(""));

		if ( strCondition.Find(strConvertISBN) < 0 )
		{
		
			if (nType == 2)
			{
				if (!strCondition.IsEmpty()) strCondition += _T("|");
				strCondition += strConvertISBN;
			}
			else
			{
				if (!strCondition.IsEmpty()) strCondition += _T(",");
				if (nType == 0) 
				{
					strCondition += strConvertISBN;
				}
				else
				{
					strCondition += _T("'")+strConvertISBN+_T("'");
				}
			}
		}

	}

	return strCondition;
}


INT CKOLISNetWebSearchMgr::CheckISBN(CString strISBNFrom, CString &strISBNTo)
{
	INT nLength, iResult, nFind, ids = -1;
	CString strSet, strTemp, strTemp2, strMsg, strOutISBN, strCheckDigit;
	CISBNChecker isbnChecker;
	

	strISBNTo.Empty();

	// () ����..
	nFind = strISBNFrom.Find('(');
	if( nFind >= 0 )
	{
		strSet = strISBNFrom = strISBNFrom.Left(nFind);
	}
	if( strISBNFrom.IsEmpty() == TRUE )		return -1;

	// '-' ����
	strTemp = strISBNFrom;
	strISBNFrom.Empty();
	isbnChecker.RemoveDashFromISBN(strTemp.GetBuffer(0), strISBNFrom.GetBuffer(MAX_PATH));
	strISBNFrom.ReleaseBuffer();

	if( strISBNFrom.IsEmpty() == TRUE )		return -1;	

	// ���̰� 10, 9�� ����
	iResult = -1;
	nLength = strISBNFrom.GetLength();
	switch( nLength )
	{
	case 9:		
		isbnChecker.ReturnISBNCheckDigit( strISBNFrom.GetBuffer(0), strOutISBN.GetBuffer(MAX_PATH), strCheckDigit.GetBuffer(10), FALSE );
		strOutISBN.ReleaseBuffer();
		strCheckDigit.ReleaseBuffer();
		strTemp = strISBNFrom.Left(2);
		strISBNTo.Format(_T("%s%s%s%s"), strTemp, strOutISBN, strCheckDigit, strSet);		
		break;

	case 10:		
		ids = isbnChecker.ConvertISBN(strISBNFrom.GetBuffer(0), strISBNTo.GetBuffer(MAX_PATH), &iResult, FALSE);
		if( ids < 0 )	return -1;
		
		if( iResult == 0 )
		{				
			strISBNTo += strSet;
		}
		else if (iResult == -2)
		{				
			strISBNTo += strSet;			
		}
		else
		{
			return -1;
		}		
		break;

	case 12:
		{			
			isbnChecker.ReturnISBNCheckDigit( strISBNFrom.GetBuffer(0), strOutISBN.GetBuffer(MAX_PATH), strCheckDigit.GetBuffer(10), FALSE );
			strOutISBN.ReleaseBuffer();
			strCheckDigit.ReleaseBuffer();
			strTemp = strISBNFrom.Left(3);
			strTemp2 = strISBNFrom.Mid(3,2);
			strISBNTo.Format(_T("%s%s%s%s%s"), strTemp, strTemp2, strOutISBN, strCheckDigit, strSet);
		}
		break;
	case 13:		
		ids = isbnChecker.ConvertISBN(strISBNFrom.GetBuffer(0), strISBNTo.GetBuffer(MAX_PATH), &iResult, FALSE);
		if( ids < 0 )	return -1;
		
		if( iResult == 0 )
		{				
			strISBNTo += strSet;
		}
		else if (iResult == -2)
		{				
			strISBNTo += strSet;
		}
		else
		{
			return -1;
		}
		break;

	default:
		return -1;		
		break;
	}

	return 0;
}

void CKOLISNetWebSearchMgr::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcWnd;
	GetClientRect(rcWnd);

	m_bmpTop.DrawDIB(&dc, 0, 0);
	m_bmpMiddle.DrawDIB(&dc, 0, 35);

	
	m_bmpBottom.DrawDIB(&dc, 0, rcWnd.bottom - 36);
}

void CKOLISNetWebSearchMgr::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);	
}

void CKOLISNetWebSearchMgr::OnLButtonDown(UINT nFlags, CPoint point) 
{
EFS_BEGIN
	CRect rcTitle;
	GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);
	ScreenToClient(rcTitle);
	if (rcTitle.PtInRect(point))
	{
		m_bISMoveWindow = TRUE;
		SetCapture();
		GetCursorPos(&m_CusorPoint);
		GetWindowRect(m_rcWnd);
	}
	
	CDialog::OnLButtonDown(nFlags, point);
EFS_END
}

void CKOLISNetWebSearchMgr::OnMouseMove(UINT nFlags, CPoint point) 
{
	EFS_BEGIN

	if (m_bISMoveWindow)
	{
		CPoint point;
		GetCursorPos(&point);

		INT x = point.x - m_CusorPoint.x;
		INT y = point.y - m_CusorPoint.y;


		SetWindowPos(NULL, m_rcWnd.left + x, m_rcWnd.top + y, 1, 1, SWP_NOSIZE | SWP_NOZORDER);

		GetWindowRect(m_rcWnd);
		m_CusorPoint = point;

		return;
	}
	
	CDialog::OnMouseMove(nFlags, point);

	EFS_END
}

void CKOLISNetWebSearchMgr::OnLButtonUp(UINT nFlags, CPoint point) 
{
	EFS_BEGIN

	if (m_bISMoveWindow)
	{
		m_bISMoveWindow = FALSE;
		ReleaseCapture();
	}
	
	CDialog::OnLButtonUp(nFlags, point);

	EFS_END
}

CString CKOLISNetWebSearchMgr::GetResultCount()
{
	CString strText;	

	if ( m_strISBNList.GetLength() > 60 )
	{
		m_strISBNList = m_strISBNList.Left(57) + _T("...");
	}
	
	CESL_DataMgr* pDM_Result = FindDM(_T("DM_KOLISNET_WEB_SEARCH"));
	//strText.Format(_T("%s : %d �� �˻�"), m_strISBNList, pDM_Result->GetRowCount() );
	strText.Format(_T("%d��"), pDM_Result->GetRowCount() );
	
	return strText;
}

void CKOLISNetWebSearchMgr::OnbClear() 
{
	CESL_ControlMgr *pCM= FindCM(_T("CM_KOLISNET_WEB_SEARCH"));
	CESL_DataMgr* pDM_Result = FindDM(_T("DM_KOLISNET_WEB_SEARCH"));
	pDM_Result->FreeData();

	pCM->AllControlDisplay();
}

void CKOLISNetWebSearchMgr::OnbtnImport() 
{
	CESL_ControlMgr		*pCM		= FindCM(_T("CM_KOLISNET_WEB_SEARCH"));
	CESL_DataMgr		*pDM_Result	= FindDM(_T("DM_�����ڷ�_DB"));
	CESL_DataMgr		*pPosDM		= FindDM(_T("DM_KOLISNET_POS"));
	CESL_DataMgr		*pDataMgr2	= FindDM(_T("DM_����������_����"));
	CESL_DataMgr		*pDM_Result2= FindDM(_T("DM_KOLISNET_WEB_SEARCH"));	
	
	CESL_Grid* pGrid = (CESL_Grid*)pCM->FindControl (_T("MainGrid") );
	if(pCM == NULL)
	{
		AfxMessageBox(_T("FindCM() Error~!")) ;
		return;
	}

	const INT	nFieldCnt = 10;
	CString strPosData[10];
	CString strPosDataItemList[nFieldCnt] =
	{
		_T("TITLE"),	_T("PUBLISHER"), _T("AUTHOR"),		_T("ISBN")		,_T("EAN13"),
		_T("CLASS"),	_T("PRICE"),     _T("PUBLISH_DATE"),_T("EDITION")	,_T("CREATE_DATE")
	};	

	CString vdkvgwkey;
	CString strKeyList;
	INT		ids;
	INT		nCount;
	INT		nIdx = -1;

    ids = pGrid->SelectMakeList () ;
	pGrid->SelectGetHeadPosition();
	nCount = pGrid->SelectGetCount();
	if ( nCount <= 0 ) return;
	
	m_ctlProgress.SetPos(0);
	m_ctlProgress.SetRange(0, nCount );
	m_ctlProgress.SetStep(1);
	m_ctlProgress.SetWindowText(_T("�����ڷ� ������..."));

	strKeyList = _T("");
	CString strSearchHold;
	CString strStreamMarc;
	CString strTmp;
	pDataMgr2->FreeData();

	CMarcMgr	*pMarcMgr;
	CMarc		*pMarc;
	INT			i;
	INT			nInsetIdx;
	pMarcMgr = m_pInfo->pMarcMgr;
	pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
	

	pGrid->SelectGetHeadPosition();
	// KOLISNET �˻� ��� ����
	CString strWhere;

	for ( i=0 ; i<nCount ; i++ )
	{
		nIdx = pGrid->SelectGetIdx();
		pDM_Result2->GetCellData(_T("�˻���ġ"), nIdx, strSearchHold );
		if ( strSearchHold == _T("KOLISNET") )
		{
			m_ctlProgress.StepIt();
			nIdx = pGrid->SelectGetIdx();
			pDM_Result->GetCellData(_T("����������Ű"), nIdx, vdkvgwkey );
			if ( strKeyList.GetLength() > 0 ) strKeyList += _T("|");
			strKeyList += vdkvgwkey;
		}
		else
		{
			// �̴������ʹ� �� POS�������̹Ƿ�.
			break;
		}
		pGrid->SelectGetNext();
	}
	if ( strKeyList.GetLength() > 0 )
	{
		CKolisNetDataMapper Mapper;
		Mapper.AddData(KOLISNET_PARAM_ABSTRACTS_YN, "�ʷ�����");
		Mapper.AddData(KOLISNET_PARAM_CONTENTS_YN, "��������");
		Mapper.AddData(KOLISNET_PARAM_CONTROL_NO, "�����ȣ");
		Mapper.AddData(KOLISNET_PARAM_CREATE_DATE, "��������");
		Mapper.AddData(KOLISNET_PARAM_CREATE_LIB_CODE, "����������");
		Mapper.AddData(KOLISNET_PARAM_DDC_CLASS_NO, "DDC�з���ȣ");
		Mapper.AddData(KOLISNET_PARAM_DDC_CLASS_NO_VER, "DDC�з���ȣ����");
		Mapper.AddData(KOLISNET_PARAM_ETC_CLASS_NO, "ETC�з���ȣ");
		Mapper.AddData(KOLISNET_PARAM_ETC_CLASS_NO_TYPE, "ETC�з���ȣŸ��");
		Mapper.AddData(KOLISNET_PARAM_ETC_CLASS_NO_VER, "ETC�з���ȣ����");
		Mapper.AddData(KOLISNET_PARAM_FORM_CODE, "���ı���");
		Mapper.AddData(KOLISNET_PARAM_KDC_CLASS_NO, "KDC�з���ȣ");
		Mapper.AddData(KOLISNET_PARAM_KDC_CLASS_NO_VER, "KDC�з���ȣ����");
		Mapper.AddData(KOLISNET_PARAM_BIB_MARC, "MARC");
		Mapper.AddData(KOLISNET_PARAM_MAT_CODE, "�ڷᱸ��");
		Mapper.AddData(KOLISNET_PARAM_MEDIA_CODE, "��ü����");
		Mapper.AddData(KOLISNET_PARAM_NOTE, "�޸���");
		Mapper.AddData(KOLISNET_PARAM_BIB_KEY, "����������Ű");
		Mapper.AddData(KOLISNET_PARAM_UNDO_FLAG, "UNDO_FLAG");
		Mapper.AddData(KOLISNET_PARAM_UPDATE_DATE, "������������");
		Mapper.AddData(KOLISNET_PARAM_UPDATE_LIB, "��������������");
		Mapper.AddData(KOLISNET_PARAM_USE_LIMIT_CODE, "�̿����ѱ���");

		CKolisNetService::GetBibInfo(this, strKeyList, pDataMgr2, &Mapper);
	}

	// POS ������ ����
	pMarc = new CMarc();		
	pGrid->SelectGetHeadPosition();

	INT offset = pDM_Result->GetRowCount();

	for ( i=0 ; i<nCount ; i++ )
	{
		nIdx = pGrid->SelectGetIdx();
		pDM_Result2->GetCellData(_T("�˻���ġ"), nIdx, strSearchHold );
		if ( strSearchHold == _T("�������������") )
		{
			m_ctlProgress.StepIt();		
			pPosDM->GetCellData(strPosDataItemList, 10, strPosData, nIdx-offset );
			pMarcMgr->SetItem(pMarc, _T("245$a"),  strPosData[0] );			
			pMarcMgr->SetItem(pMarc, _T("260$b"),  strPosData[1] );
			pMarcMgr->SetItem(pMarc, _T("245$d"),  strPosData[2] );
			pMarcMgr->SetItem(pMarc, _T("020$a"),  strPosData[3] );

			// 2009.01.12 UPDATE BY PDJ : ��� 13�ڸ��̸�
			if ( strPosData[3] != strPosData[4] ) 
			{
				pMarcMgr->SetItem(pMarc, _T("020$a"),  strPosData[4] );
			}
			// 2009.01.12 REM BY PDJ : �з���ȣ�� �ѱ۷� �Ǿ� �־ ������.
			//pMarcMgr->SetItem(pMarc, _T("056$a"),  strPosData[5] );
			pMarcMgr->SetItem(pMarc, _T("950$b"),  strPosData[6] );
			// 2009.01.12 UPDATE BY PDJ : ����⿡ ������� ���ԵǾ� ����
			if ( strPosData[7].GetLength() >= 4 )
			{
				pMarcMgr->SetItem(pMarc, _T("260$c"),  strPosData[7].Left(4) );
			}
			pMarcMgr->SetItem(pMarc, _T("250$a"),  strPosData[8] );

			pMarcMgr->Encoding(pMarc, strStreamMarc);
			pDataMgr2->InsertRow(-1);
			nInsetIdx = pDataMgr2->GetRowCount()-1;
			pDataMgr2->SetCellData(_T("MARC"), strStreamMarc, nInsetIdx );
		}
		pGrid->SelectGetNext();		
	}	
	delete pMarc;

	
	m_ctlProgress.SetPos(0);
	m_ctlProgress.SetRange(0, nCount );
	m_ctlProgress.SetStep(1);
	m_ctlProgress.SetWindowText(_T("������.."));

	ImportData();

	m_ctlProgress.SetWindowText(_T("���ԿϷ�"));
}

void CKOLISNetWebSearchMgr::OnbtnHide() 
{
	ShowWindow(SW_HIDE);	
}

void CKOLISNetWebSearchMgr::SetParentInfo(CESL_Information	*m_pParentInfo)
{
	m_pInfo = m_pParentInfo;
}

INT CKOLISNetWebSearchMgr::InitControl()
{	
	
	CString			strQuery;
	CString			strAcqYear;
	CString			strWorkCode;
	CString			strSeqNo;
	CString			strWorkNo;
	CStringArray	pArray;
	INT				ids;
	INT				nSeqNo;

	CESL_ControlMgr *pCM = FindCM(_T("CM_KOLISNET_WEB_SEARCH"));
	CESL_DataMgr	*pDM = FindDM(_T("DM_������������"));

	pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear, -1);
	pCM->GetControlMgrData(_T("��������"), strWorkCode, -1);
	
	m_pComboMgr = new CMulitComboBoxManager(this);

	if(strWorkCode.IsEmpty())
	{
		strQuery.Format(_T("ACQ_YEAR='%s' AND WORK_CODE IS NULL"), strAcqYear);		
	}
	else
	{
		strQuery.Format(_T("ACQ_YEAR='%s' AND WORK_CODE='%s'"), strAcqYear, strWorkCode);		
	}
	ids = pDM->RefreshDataManager(strQuery);
	if( ids < 0 )	return -1;	

	nSeqNo = -1;
	pArray.RemoveAll();
	for ( int i=0 ; i<pDM->GetRowCount() ; i++ )
	{
		strSeqNo = pDM->GetCellData(_T("������ȣ"), i);
		if ( _ttoi(strSeqNo) > nSeqNo )
		{
			nSeqNo = _ttoi(strSeqNo);
		}		
		pArray.Add(strSeqNo);
	}
	
	m_pComboMgr->SetSingleComboBoxStrData(_T("CM_KOLISNET_WEB_SEARCH"), nSeqNo, &pArray, _T("������ȣ"));			
		
	m_pComboMgr2 = new CMulitComboBoxManager(this);
	CESL_DataMgr tmpDM;	
	tmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);	
	if(strWorkCode.IsEmpty())
		strQuery.Format(_T("SELECT WORK_NO FROM BO_PURCHASE_WORK_NO_TBL WHERE PURCHASE_SEQ_NO_KEY IN (SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '%s' AND SEQ_NO = %d AND WORK_CODE IS NULL AND MANAGE_CODE=UDF_MANAGE_CODE) ORDER BY WORK_NO"), strAcqYear, nSeqNo);
	else
		strQuery.Format(_T("SELECT WORK_NO FROM BO_PURCHASE_WORK_NO_TBL WHERE PURCHASE_SEQ_NO_KEY IN (SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '%s' AND SEQ_NO = %d AND WORK_CODE='%s' AND MANAGE_CODE=UDF_MANAGE_CODE) ORDER BY WORK_NO"), strAcqYear, nSeqNo, strWorkCode);
	ids = tmpDM.RestructDataManager(strQuery);

	nSeqNo = -1;
	pArray.RemoveAll();
	for ( i=0 ; i<tmpDM.GetRowCount() ; i++ )
	{
		strSeqNo = tmpDM.GetCellData(i, 0);
		if ( _ttoi(strSeqNo) > nSeqNo )
		{
			nSeqNo = _ttoi(strSeqNo);
		}		
		pArray.Add(strSeqNo);
	}
	
	m_pComboMgr2->SetSingleComboBoxStrData(_T("CM_KOLISNET_WEB_SEARCH"), nSeqNo, &pArray, _T("�۾���ȣ"));	

	return 0;
}


INT CKOLISNetWebSearchMgr::insert_data( VOID *pAppend_data_search, INT RowCount ,CString DMAlias ,INT ColCount)
{   
	DB_DATA_ARRAY_TYPE_SEARCH *pAppend_data = (DB_DATA_ARRAY_TYPE_SEARCH*)pAppend_data_search;

    CString temp_m_szDataMgrAlias;
    temp_m_szDataMgrAlias.Format (_T("%s"),DMAlias);

	CESL_DataMgr *pDM = FindDM(temp_m_szDataMgrAlias);
	if (pDM == NULL) return -1;

	pDM->FreeData();

	INT ColumnCount = pDM->RefList.GetCount();
	INT row, col;
	TCHAR *cell;
	CString strData;
	
	INT nColIndex = 0;

	CString sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType;
	
	for (row = 0; row < RowCount; row++) {
		pDM->InsertRow(-1);
		nColIndex = 0;
		for (col = 0; col < ColCount; col++) {
			cell = NULL;
			cell = pAppend_data[col].db_data[row].stringData;
			if (cell) {
				
				while(TRUE)
				{
					pDM->GetColumnRefInfo( nColIndex , sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType );
					//============================================
					//20080611 ADD BY PJW : �ҹ��ڴ빮�ڵ��� �����Ƿ� �ҹ��ڷ� �ٲپ ���Ѵ�.
					sDataType.MakeLower();
					//============================================
					if( sDataType.Compare(_T("udf"))!=0 )
						 break;
					nColIndex++;
				}
				strData.Format(_T("%s"), cell);
				pDM->SetCellData(row, nColIndex , strData);
			}
			nColIndex++;
		}
	}
	return 0;
}


VOID CKOLISNetWebSearchMgr::InitSpeciesApi()
{
	// �� API �ʱ�ȭ
	if (m_pBOSpeciesApi) 
	{
		delete m_pBOSpeciesApi;
		m_pBOSpeciesApi = NULL;
	}
	m_pBOSpeciesApi = new CBO_SPECIES(this);
	m_pBOSpeciesApi->InitDataMgr();
	if (m_pBOSpeciesApi == NULL)
	{
		m_strLastErrMsg = _T("�� API �ʱ�ȭ ����");
		return ;
	}

}

INT CKOLISNetWebSearchMgr::ImportData()
{
	CESL_DataMgr *pSejiDataMgr = FindDM(_T("DM_����������_����"));
	CESL_DataMgr *pDataMgr = FindDM(_T("DM_������������"));
	
	CESL_ControlMgr *pCM= FindCM(_T("CM_KOLISNET_WEB_SEARCH"));
	CMarc	marc;

	
	CString strSpeciesKey;	
	CString strBookKey;
	CString strAcqKey;
	CString strQuery;
	
	CString strAcqYear, strWorkCode;	
	CString strSeqNo, strWorkNo;
	CString strInputDate;
	CString strManageCode;
	CString strSerialNo;	
	CString strStreamMarc;
	CString strMarcData1;
	CString strCurrencyCode, strPrice;
	CString strISBN;
	CString strKDCClassNo;
	CString strSubjectCode;
	INT		nSerialNo;
	INT		ids;
	
	pCM->GetControlMgrData(_T("���Գ⵵"), strAcqYear, -1);
	pCM->GetControlMgrData(_T("��������"), strWorkCode, -1);
	pCM->GetControlMgrData(_T("������ȣ"), strSeqNo, -1);
	pCM->GetControlMgrData(_T("�۾���ȣ"), strWorkNo, -1);

	strQuery.Format(_T("SELECT LAST_SERIAL_NO FROM BO_PURCHASE_WORK_NO_TBL WHERE WORK_NO = 1 AND PURCHASE_SEQ_NO_KEY = (SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '%s' AND SEQ_NO = %d AND WORK_CODE = %d AND MANAGE_CODE=UDF_MANAGE_CODE)"), strAcqYear, _ttoi(strSeqNo), _ttoi(strWorkCode));
	ids = pDataMgr->GetOneValueQuery(strQuery,  strSerialNo);
	if( ids < 0 )	return -1;	

	//++2008.12.04 ADD BY LKS {{++
	//�з���ȣ���� ���� �����´�.
	CString strClassNoType;
	m_pInfo->pCodeMgr->GetDefaultCode(_T("�з�ǥ����"),strClassNoType);
	//--2008.12.04 ADD BY LKS --}}

	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));

	strQuery.Format(_T("SELECT MANAGE_CODE FROM MN_USER_TBL WHERE ID='%s'"), m_pInfo->USER_ID );
	pDataMgr->GetOneValueQuery(strQuery, strManageCode);
	
	COleDateTime datetime = COleDateTime::GetCurrentTime();
	strInputDate.Format(_T("%04d/%02d/%02d") , datetime.GetYear(), datetime.GetMonth(), datetime.GetDay() );
	

	nSerialNo = _ttoi(strSerialNo);
	pDataMgr->StartFrame();	
	CString strTmp;
	INT i,j;
	for ( i=0 ; i < pSejiDataMgr->GetRowCount() ; i++ )
	{
		// ������ ����
		pDataMgr->MakeRecKey(strSpeciesKey);
		pDataMgr->MakeRecKey(strBookKey);
		pDataMgr->MakeRecKey(strAcqKey);
		
		strSerialNo.Format(_T("%d"), nSerialNo + i + 1 );

		pSejiDataMgr->GetCellData(_T("MARC"), i, strStreamMarc );

		// =============
		// ������ ����
		// =============
		pDataMgr->InitDBFieldData();	
		pDataMgr->AddDBFieldData(_T("REC_KEY")				, _T("NUMERIC"), strSpeciesKey);	
		pDataMgr->AddDBFieldData(_T("ACQ_YEAR")				, _T("STRING"), strAcqYear);		
		pDataMgr->AddDBFieldData(_T("CLASS_NO_TYPE")		, _T("STRING"), _T("1") );
		//=============================================
	

		strKDCClassNo = pSejiDataMgr->GetCellData(_T("KDC�з���ȣ")		, i );
		
		pDataMgr->AddDBFieldData(_T("CONTROL_MAT_CODE")		, _T("STRING"), _T("KMO") );				                                               
		pDataMgr->AddDBFieldData(_T("KDC_CLASS_NO")			, _T("STRING"), strKDCClassNo );	
		pDataMgr->AddDBFieldData(_T("KDC_CLASS_NO_VER")		, _T("STRING"), pSejiDataMgr->GetCellData(_T("KDC�з���ȣ����")	, i ) );	
		pDataMgr->AddDBFieldData(_T("DDC_CLASS_NO")			, _T("STRING"), pSejiDataMgr->GetCellData(_T("DDC�з���ȣ")		, i ) );
		pDataMgr->AddDBFieldData(_T("DDC_CLASS_NO_VER")		, _T("STRING"), pSejiDataMgr->GetCellData(_T("KDC�з���ȣ����")	, i ) );		

		//=====================================================
		//2009.02.20 ADD BY PJW : ���������̺�  �������� �߰��Ѵ�.
		if ( strKDCClassNo.GetLength() > 1 )
		{
			strSubjectCode = strKDCClassNo.Left(1);
			if ( strSubjectCode >= _T("0") && strSubjectCode <= _T("9") )
			{
				pDataMgr->AddDBFieldData(_T("SUBJECT_CODE")		, _T("STRING"), strSubjectCode );
			}
		}
		//=====================================================
		strTmp = pSejiDataMgr->GetCellData(_T("���ı���")		, i );
		if ( strTmp == _T("") ) strTmp = _T("BK");		
		pDataMgr->AddDBFieldData(_T("FORM_CODE")			, _T("STRING"), strTmp );	
		pDataMgr->AddDBFieldData(_T("INPUT_DATE")			, _T("STRING"), strInputDate);							
		pDataMgr->AddDBFieldData(_T("MANAGE_CODE")			, _T("STRING"), strManageCode);	
		pDataMgr->AddDBFieldData(_T("MARC")					, _T("STRING"), strStreamMarc );	
		strTmp = pSejiDataMgr->GetCellData(_T("�ڷᱸ��")	, i );
		if ( strTmp == _T("") ) strTmp = _T("GM");				
		pDataMgr->AddDBFieldData(_T("MAT_CODE")				, _T("STRING"), strTmp		, i );	
		strTmp = pSejiDataMgr->GetCellData(_T("��ü����")	, i );
		if ( strTmp == _T("") ) strTmp = _T("PR");				
		pDataMgr->AddDBFieldData(_T("MEDIA_CODE")			, _T("STRING"), strTmp		, i );
		pDataMgr->AddDBFieldData(_T("REMARK")				, _T("STRING"), _T("Internet Web�˻� ������� �����ڷ�"));		
		pDataMgr->AddDBFieldData(_T("SUB_REG_CODE")			, _T("STRING"), _T("EL") );					
		strTmp = pSejiDataMgr->GetCellData(_T("�̿����ѱ���")	, i );
		if ( strTmp == _T("") ) strTmp = _T("GM");				
		pDataMgr->AddDBFieldData(_T("USE_LIMIT_CODE")		, _T("STRING"),  strTmp);
		pDataMgr->AddDBFieldData(_T("USE_OBJ_CODE")			, _T("STRING"), _T("PU") );
		pDataMgr->AddDBFieldData(_T("WORKER")				, _T("STRING"), m_pInfo->USER_ID );
		pDataMgr->AddDBFieldData(_T("WORK_CODE")			, _T("STRING"), strWorkCode );

		ids = pDataMgr->MakeInsertFrame(_T("BO_SPECIES_TBL"));
		if ( ids < 0 ) 
		{			
			ids = pDataMgr->EndFrame();
			return ids;
		}
			
		// =============
		// �������� ����
		// =============
		pDataMgr->InitDBFieldData();
		pDataMgr->AddDBFieldData(_T("REC_KEY")				, _T("NUMERIC"), strAcqKey);
		pDataMgr->AddDBFieldData(_T("SPECIES_KEY")			, _T("NUMERIC"), strSpeciesKey);
		pDataMgr->AddDBFieldData(_T("CURRENCY_CODE")		, _T("STRING"), _T("\\") );
		pDataMgr->AddDBFieldData(_T("EXCHANGE_RATE")		, _T("STRING"), _T("1") );
		pDataMgr->AddDBFieldData(_T("INPUT_DATE")			, _T("STRING"), strInputDate );
		pDataMgr->AddDBFieldData(_T("CHECKIN_DATE")			, _T("STRING"), strInputDate);					
		pDataMgr->AddDBFieldData(_T("INPUT_TYPE")			, _T("STRING"), _T("0"));
		pDataMgr->AddDBFieldData(_T("ORDER_BOOK_CNT")		, _T("STRING"), _T("1") );
		pDataMgr->AddDBFieldData(_T("ORDER_DISCOUNT_TYPE")	, _T("STRING"), _T("������") );
		pDataMgr->AddDBFieldData(_T("ORDER_SEQ_NO")			, _T("STRING"), strSeqNo );
		pDataMgr->AddDBFieldData(_T("ORDER_SERIAL_NO")		, _T("STRING"), strSerialNo );
		pDataMgr->AddDBFieldData(_T("ORDER_WORK_NO")		, _T("STRING"), strWorkNo );
		pDataMgr->AddDBFieldData(_T("ORDER_YEAR")			, _T("STRING"), strAcqYear );
		pDataMgr->AddDBFieldData(_T("TOTAL_VOL_CNT")		, _T("STRING"), _T("1") );		
		pDataMgr->AddDBFieldData(_T("DELIVERY_BOOK_CNT")		, _T("NUMERIC"), _T("1") );
		pDataMgr->AddDBFieldData(_T("MISSING_BOOK_CNT")		, _T("NUMERIC"), _T("0") );
		//===================================================
		//2009.10.05 ADD BY PJW : ������������ ��� �־��ش�
		pDataMgr->AddDBFieldData(_T("REMARK")				, _T("STRING"), _T("Internet Web�˻� ������� �����ڷ�"));		
		//===================================================
// 		//=====================================================
// 		//2008.11.20 ADD BY PJW : �������� �־��ش�.
 		pDataMgr->AddDBFieldData(_T("PRICE_DISCOUNT_RATE")	, _T("STRING"), _T("0"));
// 		//=====================================================

		if ( strKDCClassNo.GetLength() > 1 )
		{
			strSubjectCode = strKDCClassNo.Left(1);
			//=====================================================
			//2009.02.20 UPDATE BY PJW : 0 ~ 9������ üũ�Ѵ�.
// 			if ( strSubjectCode >= _T("0") && strSubjectCode <= _T("0") )
			if ( strSubjectCode >= _T("0") && strSubjectCode <= _T("9") )
			//=====================================================
			{
				pDataMgr->AddDBFieldData(_T("SUBJECT_CODE")		, _T("STRING"), strSubjectCode );
			}
		}
		//=============================================
		//2008.12.04 ADD BY LKS : ������ �־��ش�.
		m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
		m_pInfo->pMarcMgr->GetItem(&marc, _T("950$b"), strMarcData1);
		CString sBP_Price;
		if( strMarcData1.IsEmpty() == FALSE )	
		{
			GetPriceInfo(strMarcData1, strCurrencyCode, sBP_Price);
		}				
		pDataMgr->AddDBFieldData(_T("PRICE")			, _T("NUMERIC"), sBP_Price );
		pDataMgr->AddDBFieldData(_T("DELIVERY_PRICE")	, _T("NUMERIC"), sBP_Price );
		pDataMgr->AddDBFieldData(_T("ORDER_PRICE")	, _T("NUMERIC"), sBP_Price );
		m_pInfo->pMarcMgr->Encoding(&marc, strStreamMarc);
		//=============================================

		ids = pDataMgr->MakeInsertFrame(_T("BO_PURCHASEINFO_TBL"));
		if ( ids < 0 ) 
		{			
			ids = pDataMgr->EndFrame();
			return ids;
		}

		// =============
		// �������� ����
		// =============
		CBO_SPECIES API(this);
		CIndexTool indexTool;		
		
		CArray<CString, CString&> pArray;
		
		CString TITLE, AUTHOR, PUBLISHER;
		CString sTitle;
		CString sTitleTagX;
		CString sTitleTagB;
		CString sAuthor;
		CString sAuthorTagE;
		CString sPublisher;
		CString strItemData;
		CString strISBN;
		CString sQuery;
		CString sPubYear;
		
		CString strQuery, strReturnValue;
		CString sWhere;
		
		INT	chk	= -1;
		INT iCh = -1;
		INT	nSpeciesCount = 1;
		
		//=======================================================================
		// 1. ���� ��ũ�� �����Ѵ�.
		//=======================================================================			
		CMarc pmarc;		
		m_pInfo->pMarcMgr->Decoding(strStreamMarc, &pmarc);
		
		//=======================================================================
		// 3. ��ũ�� ���� ������ �����Ѵ�.
		//=======================================================================
		indexTool.Init(m_pInfo->pMarcMgr, &pmarc);
		
		//MARC���� ��������
		m_pInfo->pMarcMgr->GetItem(&pmarc, _T("����"), TITLE);
		m_pInfo->pMarcMgr->GetItem(&pmarc, _T("����"), AUTHOR);
		m_pInfo->pMarcMgr->GetItem(&pmarc, _T("������"), PUBLISHER);
		
		//����
		m_pInfo->pMarcMgr->GetItem(&pmarc, _T("245$a"), sTitle);	
		m_pInfo->pMarcMgr->GetItem(&pmarc, _T("245$x"), sTitleTagX);
		if ( sTitleTagX != _T("") ) {
			sTitle = sTitle + _T(" = ") + sTitleTagX;
		}
		m_pInfo->pMarcMgr->GetItem(&pmarc, _T("245$b"), sTitleTagB);
		if ( sTitleTagB != _T("") ) {
			sTitle = sTitle + _T(" : ") + sTitleTagB;
		}
		//����
		m_pInfo->pMarcMgr->GetItem( &pmarc, _T("245$d"), sAuthor );
		m_pInfo->pMarcMgr->GetItem( &pmarc, _T("245$e"), sAuthorTagE );
		if ( sAuthorTagE != _T("") ) {
			sAuthor = sAuthor + _T(" ; ") + sAuthorTagE;
		}
		//������
		m_pInfo->pMarcMgr->GetItem( &pmarc, _T("260$b"), strItemData, &pArray );
		for ( j = 0 ; j < pArray.GetSize() ; j++ ) 
		{
			if ( j == 0  ) {
				sPublisher = pArray.GetAt(j);
			}
			else {
				sPublisher = sPublisher + _T(" : ") + pArray.GetAt(j);
			}
		}
		
		//=======================================================================
		// 4. �����ʵ���� DB��  UPDATE �Ѵ�.
		//=======================================================================
		pDataMgr->InitDBFieldData();		
		pDataMgr->AddDBFieldData(_T("IDX_ITITLE"), _T("STRING"), TITLE);
		
		CString sIdxAllItem;	// AllItem
		CString strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("��ǥ��") , strIndexItem );	
		pDataMgr->AddDBFieldData(_T("IDX_TITLE"), _T("STRING"), strIndexItem);
		pDataMgr->AddDBFieldData(_T("TITLE"), _T("STRING"), sTitle);
		sIdxAllItem = strIndexItem;
		
		strIndexItem = _T("");
		//chk = indexTool.GetIndex( _T("Ű����") , strIndexItem );
		chk = indexTool.GetIndex( _T("Ű��������") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("IDX_KEYWORD"), _T("STRING"), strIndexItem);	
		if ( strIndexItem != _T("") ) {
			sIdxAllItem = sIdxAllItem + _T(" ") + strIndexItem;
		}
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("������") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("IDX_AUTHOR"), _T("STRING"), strIndexItem);
		pDataMgr->AddDBFieldData(_T("AUTHOR"), _T("STRING"), sAuthor);
		if ( strIndexItem != _T("") ) {
			sIdxAllItem = sIdxAllItem + _T(" ") + strIndexItem;
		}
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("������") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("IDX_PUBLISHER"), _T("STRING"), strIndexItem);
		pDataMgr->AddDBFieldData(_T("PUBLISHER"), _T("STRING"), sPublisher);
		if ( strIndexItem != _T("") ) {
			sIdxAllItem = sIdxAllItem + _T(" ") + strIndexItem;
		}
		
		//IDX_ALL_ITEM �����(�ߺ�����)
		CArray<CString, CString&> pArrayIdxAllItem;
		INT iPos = sIdxAllItem.Find( _T(" ") );
		while ( iPos != -1 ) {
			CString sItem = sIdxAllItem.Left( iPos );
			INT iFind = 0;
			for ( INT j = 0 ; j < pArrayIdxAllItem.GetSize() ; j++ ) {
				if ( sItem == pArrayIdxAllItem.GetAt(j) ) {
					iFind = 1;
					break;
				}
			}
			if ( iFind == 0 ) {
				pArrayIdxAllItem.Add( sItem );
			}
			sIdxAllItem = sIdxAllItem.Mid( iPos+1 );
			iPos = sIdxAllItem.Find( _T(" ") );
		}
		sIdxAllItem = _T("");
		for ( j = 0 ; j < pArrayIdxAllItem.GetSize() ; j++ ) {
			sIdxAllItem = sIdxAllItem + _T(" ") + pArrayIdxAllItem.GetAt(j);
		}
		sIdxAllItem = sIdxAllItem.Mid(1);
		pDataMgr->AddDBFieldData(_T("IDX_ALL_ITEM"), _T("STRING"), sIdxAllItem);		
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("�����˻��������") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("IDX_IPUBLISHER"), _T("STRING"), strIndexItem);
		
		strISBN = _T("");
		chk = indexTool.GetIndex( _T("ǥ�غ�ȣ_ISBN") , strISBN );
		pDataMgr->AddDBFieldData(_T("ST_CODE"), _T("STRING"), strISBN);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("ǥ�غ�ȣ_ISSN") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("ST_ISSN"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("ǥ�غ�ȣ_STRN") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("ST_STRN"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("ǥ�غ�ȣ_RNSTRN") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("ST_RNSTRN"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("ǥ�غ�ȣ_����������ȣ") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("ST_CBN"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("ǥ�غ�ȣ_���۱ǵ�Ϲ�ȣ") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("ST_CAN"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("��Ÿ��ȣ_û����ȣ_��ġ��ȣ") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("SE_SHELF_CODE"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("��Ÿ��ȣ_û����ȣ_�з���ȣ") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("CLASS_NO"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("��Ÿ��ȣ_û����ȣ_������ȣ") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("BOOK_CODE"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("��Ÿ��ȣ_û����ȣ_��å��ȣ") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("VOL_CODE_DISP"), _T("STRING"), strIndexItem);	//_T("��Ÿ��ȣ_û����ȣ_��å��ȣ_DISP")
		pDataMgr->AddDBFieldData(_T("VOL_CODE"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("��Ÿ��ȣ_�����ȣ") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("CONTROL_NO"), _T("STRING"), strIndexItem);
		
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("KDCP") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("KDCP_CLASS"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("KDC") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("KDC_CLASS"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("DDC") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("DDC_CLASS"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("CEC") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("CEC_CLASS"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("CWC") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("CWC_CLASS"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("COC") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("COC_CLASS"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("UDC") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("UDC_CLASS"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("NDC") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("NDC_CLASS"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("LC") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("LC_CLASS"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("�ѱ����к�ȣ") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("UNI_CODE"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("���α����ȣ") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("GOV_CODE"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("�������а�") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("DEG_CODE"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("����") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("EDIT"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("�����˻������⵵") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("IDX_IPUB_YEAR"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		m_pInfo->pMarcMgr->GetItem( &pmarc, _T("260$c"), strIndexItem);
		sPubYear = strIndexItem;
		//chk = indexTool.GetIndex( _T("����⵵") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("PUB_YEAR"), _T("STRING"), strIndexItem);
		//pDataMgr->SetCellData( _T("IBS_����⵵"), strIndexItem, m_idx );
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("�������") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("TEXT_LANG"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("��๮���") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("SUM_LANG"), _T("STRING"), strIndexItem);
		
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("ǥ������INFO") , strIndexItem );
		//pDataMgr->AddDBFieldData(_T("TITLE_INFO"), _T("STRING"), strIndexItem);
		pDataMgr->AddDBFieldData(_T("TITLE_INFO"), _T("STRING"), sTitle);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("�����ڻ���") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("AUTHOR_INFO"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("�����ڻ���") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("PUB_INFO"), _T("STRING"), strIndexItem);
		
		strIndexItem = _T("");
		chk = indexTool.GetIndex( _T("����⵵����") , strIndexItem );
		pDataMgr->AddDBFieldData(_T("PUB_YEAR_INFO"), _T("STRING"), strIndexItem);	
		pDataMgr->AddDBFieldData(_T("SPECIES_CLASS"), _T("STRING"), _T("0") );			
		pDataMgr->AddDBFieldData(_T("WORK_CODE"), _T("STRING"), strWorkCode );
		pDataMgr->AddDBFieldData(_T("REC_KEY"), _T("STRING"), strSpeciesKey);
		
		// �������а�
		// ��������, �ڷᱸ��, ���ı���, ��ü����, ������ϱ���, �̿��󱸺�, �̿����ѱ���,�����ڷ�

		strTmp = pSejiDataMgr->GetCellData(_T("���ı���")		, i );
		if ( strTmp == _T("") ) strTmp = _T("BK");		
		pDataMgr->AddDBFieldData(_T("FORM_CODE")			, _T("STRING"), strTmp );	
		strTmp = pSejiDataMgr->GetCellData(_T("�ڷᱸ��")	, i );
		if ( strTmp == _T("") ) strTmp = _T("GM");				
		pDataMgr->AddDBFieldData(_T("MAT_CODE")				, _T("STRING"), strTmp		, i );	
		strTmp = pSejiDataMgr->GetCellData(_T("��ü����")	, i );
		if ( strTmp == _T("") ) strTmp = _T("PR");				
		pDataMgr->AddDBFieldData(_T("MEDIA_CODE")			, _T("STRING"), strTmp		, i );
		pDataMgr->AddDBFieldData(_T("SUB_REG_CODE")			, _T("STRING"), _T("EL") );					
		strTmp = pSejiDataMgr->GetCellData(_T("�̿����ѱ���")	, i );
		if ( strTmp == _T("") ) strTmp = _T("GM");				
		pDataMgr->AddDBFieldData(_T("USE_LIMIT_CODE")		, _T("STRING"),  strTmp);
		pDataMgr->AddDBFieldData(_T("USE_OBJ_CODE")			, _T("STRING"), _T("PU") );
		//=====================================================
		//2008.12.11 ADD BY PJW : IDX_BO_TBL���� ��ϱ��� �־��ش�.
		
		//===================================================
		//2009.09.16 ADD BY LKS : ��ϱ����� �⺻���� ���� ������ ����
		//DEL	pDataMgr->AddDBFieldData(_T("REG_CODE")				, _T("STRING"), _T("EM"));
		CString strDefaultRegCode;
		m_pInfo->pCodeMgr->GetDefaultCode(_T("��ϱ���"), strDefaultRegCode);
		if( strDefaultRegCode.IsEmpty() == TRUE )	strDefaultRegCode = _T("EM");
		pDataMgr->AddDBFieldData(_T("REG_CODE")				, _T("STRING"), strDefaultRegCode);
		//===================================================
		
		//=====================================================
		
		//=============================================
		//2008.12.04 ADD BY LKS : �з�ǥ�����߰� 
//DEL		pDataMgr->AddDBFieldData(_T("CLASS_NO_TYPE")			, _T("STRING"), strClassNoType );
		pDataMgr->AddDBFieldData(_T("CLASS_NO_TYPE")			, _T("STRING"), "1" );
		//=============================================

		//=============================================
		//2009.06.01 ADD BY LKS : �з�ǥ�����߰� 
		pDataMgr->AddDBFieldData(_T("WORKING_STATUS")			, _T("STRING"), "BOA111N" );
		//=============================================
		
		
		chk = pDataMgr->MakeInsertFrame( _T("IDX_BO_TBL") );
		if ( 0 > chk ) return ids;

		// =============
		// å���� ����
		// =============
		pDataMgr->InitDBFieldData();
		pDataMgr->AddDBFieldData(_T("REC_KEY")				, _T("NUMERIC"), strBookKey);
		pDataMgr->AddDBFieldData(_T("SPECIES_KEY")			, _T("STRING"), strSpeciesKey);
		pDataMgr->AddDBFieldData(_T("ACQ_KEY")				, _T("STRING"), strAcqKey);
		pDataMgr->AddDBFieldData(_T("ACQ_CODE")				, _T("STRING"), _T("1") );
		pDataMgr->AddDBFieldData(_T("ACQ_YEAR")				, _T("STRING"), strAcqYear );
		pDataMgr->AddDBFieldData(_T("BOOK_APPENDIX_FLAG")	, _T("STRING"), _T("B") );
		
		//=============================================
		//2008.12.04 UPDATE BY LKS : ����Ʈ�� �޾ƿͼ� ����
		//2008.12.04 UPDATE BY LKS : ���������� ���� -> ������ ���泻�� ����
//DEL		pDataMgr->AddDBFieldData(_T("CLASS_NO_TYPE")		, _T("STRING"), strClassNoType );
		pDataMgr->AddDBFieldData(_T("CLASS_NO_TYPE")		, _T("STRING"), _T("1") );
		//=============================================
		pDataMgr->AddDBFieldData(_T("INPUT_DATE")			, _T("STRING"), strInputDate );
		pDataMgr->AddDBFieldData(_T("MANAGE_CODE")			, _T("STRING"), strManageCode );
		pDataMgr->AddDBFieldData(_T("MEDIA_CODE")			, _T("STRING"), pSejiDataMgr->GetCellData(_T("��ü����")		, i ));
		
		//===================================================
		//2009.09.16 ADD BY LKS : ��ϱ����� �⺻���� ���� ������ ����
		//DEL	pDataMgr->AddDBFieldData(_T("REG_CODE")				, _T("STRING"), _T("EM"));
		pDataMgr->AddDBFieldData(_T("REG_CODE")				, _T("STRING"), strDefaultRegCode);
		//===================================================
		
		
		
		
		pDataMgr->AddDBFieldData(_T("REPRESENT_BOOK_YN")	, _T("STRING"), _T("Y"));
		pDataMgr->AddDBFieldData(_T("SEQ_NO")				, _T("STRING"), strSeqNo);
		pDataMgr->AddDBFieldData(_T("SERIAL_NO")			, _T("STRING"), _T("1"));
		
		pDataMgr->AddDBFieldData(_T("USE_LIMIT_CODE")		, _T("STRING"), pSejiDataMgr->GetCellData(_T("�̿����ѱ���")	, i ) );
		pDataMgr->AddDBFieldData(_T("USE_OBJECT_CODE")		, _T("STRING"), _T("PU") );
		pDataMgr->AddDBFieldData(_T("VOL_SORT_NO")			, _T("STRING"), _T("10") );		
		pDataMgr->AddDBFieldData(_T("WORKING_STATUS")		, _T("STRING"), _T("BOA111N"));
		pDataMgr->AddDBFieldData(_T("EA_ISBN")				, _T("STRING"), strISBN);

		pDataMgr->AddDBFieldData(_T("AUTHOR")		, _T("STRING"), sAuthor);			

		// �������� å�� �ݿ�
		// =====================
		// VOL_TITLE // ����� // ũ�� // ������ // ������ // ����		
		// PAGE, PHYSICAL_PROPERTY, BOOK_SIZE, PRICE, PRICE_CHARACTER, EA_ISBN
		m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
		
		// 2009.01.12 REM BY PDJ : 245 $a,$p�� �����ϰ� ����Ƿ�
		//m_pInfo->pMarcMgr->GetItem(&marc, _T("245$a"), strMarcData1);		
		//pDataMgr->AddDBFieldData(_T("VOL_TITLE")		, _T("STRING"), strMarcData1);	
		
		m_pInfo->pMarcMgr->GetItem(&marc, _T("300$a"), strMarcData1);
		pDataMgr->AddDBFieldData(_T("PAGE")		, _T("STRING"), strMarcData1);	

		m_pInfo->pMarcMgr->GetItem(&marc, _T("300$b"), strMarcData1);
		pDataMgr->AddDBFieldData(_T("PHYSICAL_PROPERTY")		, _T("STRING"), strMarcData1);	

		m_pInfo->pMarcMgr->GetItem(&marc, _T("300$c"), strMarcData1);
		pDataMgr->AddDBFieldData(_T("BOOK_SIZE")		, _T("STRING"), strMarcData1);			


		m_pInfo->pMarcMgr->GetItem(&marc, _T("950$b"), strMarcData1);
		if( strMarcData1.IsEmpty() == FALSE )	
		{
			GetPriceInfo(strMarcData1, strCurrencyCode, strPrice);
		}		
		pDataMgr->AddDBFieldData(_T("PRICE")		, _T("STRING"), strPrice);			
		pDataMgr->AddDBFieldData(_T("DELIVERY_YN")	, _T("STRING"), _T("Y"));

		//=============================================
		//2008.12.04 ADD BY LKS : ������������ ����� ���̴� ���� ����
		pDataMgr->AddDBFieldData(_T("PUBLISH_YEAR")				, _T("STRING"), sPubYear);
		//=============================================


		ids = pDataMgr->MakeInsertFrame(_T("BO_BOOK_TBL"));

		if ( ids < 0 ) 
		{			
			ids = pDataMgr->EndFrame();
			return ids;

		}	
		
	
	}

	strQuery.Format(_T("UPDATE BO_PURCHASE_WORK_NO_TBL SET LAST_SERIAL_NO = '%s' WHERE WORK_NO = 1 AND PURCHASE_SEQ_NO_KEY IN (SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '%s' AND SEQ_NO = %d AND WORK_CODE = %d AND MANAGE_CODE=UDF_MANAGE_CODE);"), strSerialNo, strAcqYear, _ttoi(strSeqNo), _ttoi(strWorkCode));
	pDataMgr->AddFrame(strQuery);

	pDataMgr->SendFrame(TRUE);	
	ids = pDataMgr->EndFrame();
 


	return 0;
}

BOOL CKOLISNetWebSearchMgr::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CKOLISNetWebSearchMgr::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	if ( bShow == TRUE)
	{
		InitControl();		
	}
}

INT CKOLISNetWebSearchMgr::GetPriceInfo(CString strSrcData, CString &strCurrencyCode, CString &strPrice)
{
	TCHAR * strData = strSrcData.GetBuffer(0);
	INT cnt = strSrcData.GetLength();
	for(INT i=0 ; i<cnt ; i++)
	{
		if(strData[i] >= '0' && strData[i] <= '9')
			break;
	}
	
	strCurrencyCode = strSrcData.Mid(0, i);
	strPrice = strSrcData.Mid(i, cnt);
	return 0;
}

HBRUSH CKOLISNetWebSearchMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}

void CKOLISNetWebSearchMgr::SetTransparency(int percent)
{
	SLWA pSetLayeredWindowAttributes = NULL;  // �Լ������� ����, �ʱ�ȭ.
	HINSTANCE hmodUSER32 = LoadLibrary(_T("USER32.DLL")); // �ν��Ͻ� ����.
	pSetLayeredWindowAttributes=(SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes");
    //�Լ������� ����.
	HWND hwnd = this->m_hWnd; //���̾�α��� �ڵ� ����.
	SetWindowLong(hwnd, GWL_EXSTYLE,GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	pSetLayeredWindowAttributes(hwnd, 0, (255 * percent) / 100, LWA_ALPHA);
}

LONG CKOLISNetWebSearchMgr::OnMyMethod(WPARAM wParam,LPARAM lParam)
{
	int nPercent = (int)lParam;
	SetTransparency(nPercent);

	return 0;
}


VOID CKOLISNetWebSearchMgr::FindISBN(CString strISBN )
{
	// Link�������� ISBN�� �߰ߵǸ� �˻�������� ã�Ƽ� 
	// �ش� Row�� �̵��Ѵ�.
	CESL_DataMgr* pDM_Result = FindDM(_T("DM_KOLISNET_WEB_SEARCH"));
	INT nRowCount = pDM_Result->GetRowCount();
	if ( nRowCount <= 0 ) return;

	CESL_Grid *pGrid; 
	pGrid = ( CESL_Grid * )FindControl( _T("CM_KOLISNET_WEB_SEARCH"), _T("MainGrid") );	

	CString strTargetISBN;
	for ( int i=0 ; i<nRowCount ; i++ )
	{
		//if ( pDM_Result->GetRowCount() >= i ) break;
		strTargetISBN = pDM_Result->GetCellData(_T("ISBN"), i);
		if ( strTargetISBN.Find(strISBN) >= 0 )
		{
			// �ش� �ο�� scroll
			pGrid->SetTopRow(i+1);
			pGrid->SetReverse(i);
			break;
		}
	}

	// ��ġ��� ������ �ִ��� �˻��Ѵ�.
	CString strQuery, strValue;
	strQuery.Format(_T("SELECT APPLICANT_DATE FROM BO_FURNISH_TBL WHERE EA_ISBN LIKE '%%%s%%' AND FURNISH_STATUS='1' AND MANAGE_CODE=UDF_MANAGE_CODE"), strISBN);
	pDM_Result->GetOneValueQuery(strQuery,strValue);

	if ( strValue != _T("") )
	{
		if ( m_pParent->GetSafeHwnd() ) m_pParent->SendMessage(WM_USER+1026, (WPARAM)&strValue, NULL);
	}
}

VOID CKOLISNetWebSearchMgr::OnEditchangecWORKCODE() 
{
EFS_BEGIN
	InitControl();
EFS_END
}

void CKOLISNetWebSearchMgr::OnKillfocusedtYear() 
{
	// TODO: Add your control notification handler code here
EFS_BEGIN
	InitControl();
EFS_END		
}