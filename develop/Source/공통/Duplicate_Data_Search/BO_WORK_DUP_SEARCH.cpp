// BO_WORK_DUP_SEARCH.cpp : implementation file
//

#include "stdafx.h"
#include "BO_WORK_DUP_SEARCH.h"
#include "BO_WORK_DUP_DETAIL_INFO.h"
#include "DUP_KEY_SETTING.h"
#include "../BO_ACQ_API/BO_ACQ_Business_Api.h"
//#include _T("../K2UP_Common/BO_DELETE_Mgr.h")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_WORK_DUP_SEARCH dialog


CBO_WORK_DUP_SEARCH::CBO_WORK_DUP_SEARCH(INT DuplicateSearchType, CESL_Mgr* pParent/*=NULL*/)
	: CESL_Mgr(CBO_WORK_DUP_SEARCH::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_WORK_DUP_SEARCH)
	m_chkAuthor = FALSE;
	m_bIncludeDiscardedBook = FALSE;
	m_chkISBN = FALSE;
	m_chkMedClass = FALSE;
	m_chkPubYear = FALSE;
	m_chkPublisher = FALSE;
	m_chkRegCode = FALSE;
	m_chkShelfCode = FALSE;
	m_chkTwice = FALSE;
	m_chkTitle = FALSE;
	m_chkVolTitle = FALSE;
	m_chkVol = FALSE;
	m_radAuthor = -1;
	m_radISBN = -1;
	m_radMedCode = -1;
	m_radPubYear = -1;
	m_radPublisher = -1;
	m_radRegCode = -1;
	m_radShelfCode = -1;
	m_radTitle = -1;
	m_radVol = -1;
	m_radVolTitle = -1;
	//}}AFX_DATA_INIT
	m_isDeleteOperation = FALSE;
	m_DuplicateSearchType = DuplicateSearchType;
	m_pArrAcqKey = NULL;
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pDupMgr = NULL;
	m_pMenu = NULL;
	m_nSortDirection = ESL_DATAMGR_SORT_ASC;
	//==============================
	//2008.08.06 ADD BY PJW : ������ ���������� �н��� �����Ѵ�.
	m_FileName.Format(_T(".\\duplicate_search_key.txt"));
	m_SearchKey.SetSearchKeys(m_FileName);
	//m_FileName = filename;
	m_bInit = FALSE;
	//==============================

	m_pGroup = NULL;
	m_pGroupCollection = NULL;
}

CBO_WORK_DUP_SEARCH::~CBO_WORK_DUP_SEARCH()
{
	if(m_pDupMgr)
	{
		delete m_pDupMgr;
		m_pDupMgr = NULL;
	}
	
	FreeMenu();
}

VOID CBO_WORK_DUP_SEARCH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_WORK_DUP_SEARCH)
	DDX_Check(pDX, IDC_chkAUTHOR2, m_chkAuthor);
	DDX_Check(pDX, IDC_chkDiscardBook2, m_bIncludeDiscardedBook);
	DDX_Check(pDX, IDC_chkISBN2, m_chkISBN);
	DDX_Check(pDX, IDC_chkMED_CLASS2, m_chkMedClass);
	DDX_Check(pDX, IDC_chkPUBLICATE_YEAR2, m_chkPubYear);
	DDX_Check(pDX, IDC_chkPUBLISHER2, m_chkPublisher);
	DDX_Check(pDX, IDC_chkREG_CODE2, m_chkRegCode);
	DDX_Check(pDX, IDC_chkSEPARATE_SHELF_CODE2, m_chkShelfCode);
	DDX_Check(pDX, IDC_chkTITLE_TWICE_SEARCH2, m_chkTwice);
	DDX_Check(pDX, IDC_chkTITLE2, m_chkTitle);
	DDX_Check(pDX, IDC_chkVOL_TITLE2, m_chkVolTitle);
	DDX_Check(pDX, IDC_chkVOL2, m_chkVol);
	DDX_Radio(pDX, IDC_radAUTHOR_RIGHTTRUNCATION2, m_radAuthor);
	DDX_Radio(pDX, IDC_radISBN_EQUIVALENCE2, m_radISBN);
	DDX_Radio(pDX, IDC_radMEDIACODE_EQUIVALENCE2, m_radMedCode);
	DDX_Radio(pDX, IDC_radPUBLICATE_YEAR_EQUIVALENCE2, m_radPubYear);
	DDX_Radio(pDX, IDC_radPUBLISHER_RIGHTTRUNCATION2, m_radPublisher);
	DDX_Radio(pDX, IDC_radREGCODE_EQUIVALENCE2, m_radRegCode);
	DDX_Radio(pDX, IDC_radSEPARATECODE_EQUIVALENCE2, m_radShelfCode);
	DDX_Radio(pDX, IDC_radTITLE_RIGHTTRUNCATION2, m_radTitle);
	DDX_Radio(pDX, IDC_radVOL_EQUIVALENCE2, m_radVol);
	DDX_Radio(pDX, IDC_radVOL_TITLE_RIGHTTRUNCATION2, m_radVolTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_WORK_DUP_SEARCH, CDialog)
	//{{AFX_MSG_MAP(CBO_WORK_DUP_SEARCH)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnDETAIL, OnbDETAIL)
	ON_BN_CLICKED(IDC_btnDUPDEL, OnbOVERLAPDEL)
	ON_BN_CLICKED(IDC_btnPRINT, OnbPRINT)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnSELSOURCEDATA, OnbtnSELSOURCEDATA)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_chkAUTHOR2, OnchkAUTHOR2)
	ON_BN_CLICKED(IDC_chkDiscardBook2, OnchkDiscardBook2)
	ON_BN_CLICKED(IDC_chkISBN2, OnchkISBN2)
	ON_BN_CLICKED(IDC_chkMED_CLASS2, OnchkMEDCLASS2)
	ON_BN_CLICKED(IDC_chkPUBLICATE_YEAR2, OnchkPUBLICATEYEAR2)
	ON_BN_CLICKED(IDC_chkPUBLISHER2, OnchkPUBLISHER2)
	ON_BN_CLICKED(IDC_chkREG_CODE2, OnchkREGCODE2)
	ON_BN_CLICKED(IDC_chkSEPARATE_SHELF_CODE2, OnchkSEPARATESHELFCODE2)
	ON_BN_CLICKED(IDC_chkTITLE_TWICE_SEARCH2, OnchkTITLETWICESEARCH2)
	ON_BN_CLICKED(IDC_chkTITLE2, OnchkTITLE2)
	ON_BN_CLICKED(IDC_chkVOL_TITLE2, OnchkVOLTITLE2)
	ON_BN_CLICKED(IDC_chkVOL2, OnchkVOL2)
	ON_BN_CLICKED(IDC_radALLWORDSEARCH2, OnradALLWORDSEARCH2)
	ON_BN_CLICKED(IDC_radAUTHOR_EQUIVALENCE2, OnradAUTHOREQUIVALENCE2)
	ON_BN_CLICKED(IDC_radAUTHOR_RIGHTTRUNCATION2, OnradAUTHORRIGHTTRUNCATION2)
	ON_BN_CLICKED(IDC_radISBN_EQUIVALENCE2, OnradISBNEQUIVALENCE2)
	ON_BN_CLICKED(IDC_radMEDIACODE_EQUIVALENCE2, OnradMEDIACODEEQUIVALENCE2)
	ON_BN_CLICKED(IDC_radPUBLICATE_YEAR_EQUIVALENCE2, OnradPUBLICATEYEAREQUIVALENCE2)
	ON_BN_CLICKED(IDC_radPUBLISHER_EQUIVALENCE2, OnradPUBLISHEREQUIVALENCE2)
	ON_BN_CLICKED(IDC_radPUBLISHER_RIGHTTRUNCATION2, OnradPUBLISHERRIGHTTRUNCATION2)
	ON_BN_CLICKED(IDC_radREGCODE_EQUIVALENCE2, OnradREGCODEEQUIVALENCE2)
	ON_BN_CLICKED(IDC_radSEPARATECODE_EQUIVALENCE2, OnradSEPARATECODEEQUIVALENCE2)
	ON_BN_CLICKED(IDC_radTITLE_CUTSEARCH2, OnradTITLECUTSEARCH2)
	ON_BN_CLICKED(IDC_radTITLE_EQUIVALENCE2, OnradTITLEEQUIVALENCE2)
	ON_BN_CLICKED(IDC_radTITLE_RIGHTTRUNCATION2, OnradTITLERIGHTTRUNCATION2)
	ON_BN_CLICKED(IDC_radVOL_EQUIVALENCE2, OnradVOLEQUIVALENCE2)
	ON_BN_CLICKED(IDC_radVOL_TITLE_EQUIVALENCE2, OnradVOLTITLEEQUIVALENCE2)
	ON_BN_CLICKED(IDC_radVOL_TITLE_RIGHTTRUNCATION2, OnradVOLTITLERIGHTTRUNCATION2)
	ON_BN_CLICKED(IDC_btnDUP_FIND_START2, OnbtnDUPFINDSTART2)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(5000,5003,OnMSGBOKBONSEARCH)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_WORK_DUP_SEARCH message handlers

VOID CBO_WORK_DUP_SEARCH::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	EndDialog(IDCANCEL);
}

BOOL CBO_WORK_DUP_SEARCH::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UP_�ߺ�������")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	GetWindowRect(&m_rcInitial);
 	m_pCM = FindCM(_T("CM_�ߺ�����"));
	m_pDM = FindDM(_T("DM_�ߺ�������"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid2"));
	
	//2007.06.27 ADD PJW : �ߺ������ VIEW 
	CString dupFUNC;
	CString tempDupFUNC;
	CString dupKEY;
	CString tempDupKEY;
	CString escapeDupKEY;
	CStdioFile file;		
	CString strLine;
	CString filename = _T(".\\duplicate_search_key.txt");
	CString filename_1 = _T(".\\duplicate_search_key2.txt");
	INT firstIdx, lastIdx;

	dupFUNC.Empty();
	tempDupFUNC.Empty();
	dupKEY.Empty();
	tempDupKEY.Empty();
	escapeDupKEY.Empty();

	if ( !file.Open(filename, CFile::modeRead | CFile::typeBinary )) 
	{
		AfxMessageBox(_T("�������� ����Ű�� �������� �ʽ��ϴ�."));
		return 0;
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	while( file.ReadString(strLine) )
	{
		lastIdx = 0;
		firstIdx = strLine.Find('[');
		lastIdx = strLine.Find(']');

		if (lastIdx > 1)
		{
			tempDupKEY = strLine.Mid(firstIdx +1, lastIdx-1);
			dupKEY.Format(_T("%s :"), tempDupKEY);
			escapeDupKEY = tempDupKEY;
		}

		if (dupKEY == _T("��ǥ�� :"))
		{
			dupKEY.Format(_T("���� :"));
		}

		if (dupKEY == _T("����ISBN :"))
		{
			dupKEY.Format(_T("ISBN :"));
		}

		if (dupKEY == _T("����� :"))
		{
			dupKEY.Format(_T("��/���� :"));
		}

		//======================================
		//20080630 UPDATE BY PJW : ==�� FIND�� �ٲ۴�.
		//if (strLine == _T("SEARCH_TYPE=0"))
		if( 0 <= strLine.Find( _T("SEARCH_TYPE=0") ) )
		//======================================
		{
			tempDupKEY = dupKEY;
			dupKEY.Format(_T("%s �����ܰ˻�"), tempDupKEY);				
		}
		//======================================
		//20080630 UPDATE BY PJW : ==�� FIND�� �ٲ۴�.
		//else if (strLine == _T("SEARCH_TYPE=1"))
		else if( 0 <= strLine.Find( _T("SEARCH_TYPE=1") ) )
		//======================================
		
		{
			tempDupKEY = dupKEY;
			dupKEY.Format(_T("%s ��ġ�˻�"), tempDupKEY);
					
		}
		//======================================
		//20080630 UPDATE BY PJW : ==�� FIND�� �ٲ۴�.
		//else if(strLine == _T("SEARCH_TYPE=2"))
		else if( 0 <= strLine.Find( _T("SEARCH_TYPE=2") ) )
		//======================================	
		{
			tempDupKEY = dupKEY;
			dupKEY.Format(_T("%s ����˻�"), tempDupKEY);					
		}

		//======================================
		//20080630 UPDATE BY PJW : ==�� FIND�� �ٲ۴�.
		//if(strLine == _T("ARTICLE_SEARCH_YN=1"))	
		if( 0 <= strLine.Find( _T("ARTICLE_SEARCH_YN=1") ) )
		//======================================
		{
			tempDupKEY = dupKEY;
			dupKEY.Format(_T("%s, ��Ī���"), tempDupKEY);				
		}

		if (escapeDupKEY != _T("��ġ��ȣ") && escapeDupKEY != _T("��ϱ���"))
		{
			//======================================
			//20080630 UPDATE BY PJW : ==�� FIND�� �ٲ۴�.
			//if(strLine == _T("USE_YN=1"))
			if( 0 <= strLine.Find( _T("USE_YN=1") ) )
			//======================================
			{
				if(tempDupFUNC == _T(""))
				{
					dupFUNC.Format(_T("(%s)"), dupKEY);						
				}else
				{
					dupFUNC.Format(_T("%s AND (%s)"), tempDupFUNC, dupKEY);		
				}
				tempDupFUNC.Format(_T("%s"), dupFUNC);				
			}
		}
		/*

		if (strLine == _T("USE_YN=1"))
		{
			if (tempDupFUNC == _T(""))
			{
				dupFUNC.Format(_T("(%s)"), dupKEY);						
			}else
			{
				dupFUNC.Format(_T("%s AND (%s)"), tempDupFUNC, dupKEY);		
			}
			tempDupFUNC.Format(_T("%s"), dupFUNC);				
		}*/
	}
	file.Close();

	//2007.07.05 ADD PJW : �����ڷ� ���� �����߰�
	if ( !file.Open(filename_1, CFile::modeRead | CFile::typeBinary )) 
	{
		AfxMessageBox(_T("�������� ����Ű�� �������� �ʽ��ϴ�."));
		return FALSE;
	}
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	while ( file.ReadString(strLine) )
	{
		lastIdx = 0;
		firstIdx = strLine.Find('[');
		lastIdx = strLine.Find(']');

		if (lastIdx > 1)
		{
			dupKEY = strLine.Mid(firstIdx +1, lastIdx-1);
		}

		if (dupKEY == _T("�����ڷ����Կ���") && strLine == _T("USE_YN=1"))
		{
			if (tempDupFUNC == _T(""))
			{
				dupFUNC.Format(_T("(�����ڷ�����)"),dupKEY);						
			}else
			{
				dupFUNC.Format(_T("%s AND (�����ڷ�����)"),tempDupFUNC);		
			}
			tempDupFUNC.Format(_T("%s"),dupFUNC);				
		}
	}
	file.Close();
	dupFUNC.Format(_T("%s"),tempDupFUNC);
	GetDlgItem(IDC_dupFUNC2)->SetWindowText(dupFUNC);

	//========================================================================
	//2008.08.26 ADD BY PJW : �������� �������� �ʱ�ȭ �Ѵ�.
	InitDupKeySetting();
	//========================================================================

	// 0.1 ���Ŀ� Grid display ����
	SetTimer(0, 100, NULL);

	m_pGrid->SetSort(FALSE);
	m_pGrid->m_bFroceReverse = FALSE;
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	m_pDupMgr = new CDuplicateSearchMgr(this);

	PostMessage(WM_SIZE);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_WORK_DUP_SEARCH::OnbDETAIL() 
{
	// TODO: Add your control notification handler code here
	ShowDetailInfo(SUPDATE);
}

INT CBO_WORK_DUP_SEARCH::CheckSelectDataValid()
{
	CESL_DataMgr * pDM = FindDM(_T("DM_�ߺ�������"));
	CESL_Grid * pGrid = (CESL_Grid*)FindControl(_T("CM_�ߺ�����"), _T("MainGrid2"));

	if(pDM->GetRowCount() == 0) return -1;

	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	ids = pGrid->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}

VOID CBO_WORK_DUP_SEARCH::OnbOVERLAPDEL() 
{
	// TODO: Add your control notification handler code here
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	CESL_DataMgr * pDM = FindDM(_T("DM_�ߺ�������"));
	CESL_Grid * pGrid = (CESL_Grid*)FindControl(_T("CM_�ߺ�����"), _T("MainGrid2"));
	
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();		api.InitTempPtr();
	api.SetTempPtr(_T("DM"), (DWORD*)pDM);
	api.SetTempPtr(_T("BookDM"), (DWORD*)NULL);
	api.SetTempPtr(_T("Grid"), (DWORD*)pGrid);
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_PURCHASE);
	api.SetTempData(_T("ACQ_TYPE"), (INT)PURCHASE);
	api.SetTempData(_T("����Ÿ��"), (INT)SPECIES_DELETE);
	api.SetTempData(_T("å���˸��ƽ�"), _T("UDF_�ڷ���¼���"));
	api.SetTempData(_T("���ݾ˸��ƽ�"), _T("������ȣ"));
	api.SetTempData(_T("��������"), _T("�ߺ��ڷ����"));
	api.SetTempPtr(_T("��Ű����Ʈ"), (DWORD*)m_pArrAcqKey);
	api.SetTempPtr(_T("��������Ű����Ʈ"), NULL);
	api.SetTempPtr(_T("���Ϸù�ȣ����Ʈ"), NULL);
	ids = api.SPFExecutePath(_T("����"));
	if(ids == 0)
	{
		m_isDeleteOperation = TRUE;
		DisplayGrid();
	}
/*
	api.SetTempPtr(_T("DM"), (DWORD*)pDM);
	api.SetTempPtr(_T("Grid"), (DWORD*)pGrid);
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_PURCHASE);
	api.SetTempData(_T("ACQ_TYPE"), (INT)PURCHASE);
	api.SetTempData(_T("��Ű�˸��ƽ�"), _T("IBS_��KEY"));
	api.SetTempData(_T("��������Ű�˸��ƽ�"), _T("BP_��������KEY"));
	api.SetTempData(_T("��������"), _T("�ߺ��ڷ����"));
	api.SetTempPtr(_T("��Ű����Ʈ"), (DWORD*)m_pArrAcqKey);
	api.SetTempPtr(_T("��������Ű����Ʈ"), NULL);
	ids = api.SPFExecutePath(_T("������"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ ������ �� �����ϴ�."));
		return;
	}
	if(ids == 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ �����Ͽ����ϴ�."));
		m_isDeleteOperation = TRUE;
		DisplayGrid();
	}
*/
}

VOID CBO_WORK_DUP_SEARCH::OnbPRINT() 
{
	CString strTmpData;
	CBasicPrintMgr printMgr(this);
	printMgr.SetPrintDM(FindDM(_T("DM_�ߺ�������")));
	printMgr.SetSIReportFilename(_T("K2UP_�ߺ�������"));
	printMgr.InitPrintData();
	if(m_DuplicateSearchType == WORK_DUPLICATE_SEARCH_DIVNO)
	{
		printMgr.AddPrintData(_T("����"), _T("���������ߺ�������"));
		if(!m_strAcqYear.IsEmpty())
		{
			strTmpData.Format(_T("���Գ⵵ : %s"), m_strAcqYear);
			printMgr.AddPrintData(_T("���Գ⵵"), strTmpData);
		}
		if(!m_strGroupInfo.IsEmpty())
		{
			strTmpData.Format(_T("������ȣ : %s"), m_strAcqYear);
			printMgr.AddPrintData(_T("������ȣ"), strTmpData);
		}
	}
	else 
		printMgr.AddPrintData(_T("����"), _T("�ߺ�������"));
	printMgr.Print();
}

INT CBO_WORK_DUP_SEARCH::RefreshDMandShowList(VOID)
{
	CESL_DataMgr * pDM = FindDM(_T("DM_�ߺ�������"));
	INT cnt = pDM->GetRowCount();

	SortDuplicateData(NULL);

	CESL_ControlMgr *pCM = FindCM(_T("CM_�ߺ�����"));
	if (pCM) {
		CESL_Grid *pGrid = (CESL_Grid *)pCM->FindControl(_T("MainGrid2"));
		if (pGrid) DisplayGrid();
	}
	return 0;
}

VOID CBO_WORK_DUP_SEARCH::OnTimer(UINT nIDEvent) 
{
	INT ids;
	if (nIDEvent == 0) {
		KillTimer(0);
		ids = RefreshDMandShowList();
	}
	CDialog::OnTimer(nIDEvent);
}

BEGIN_EVENTSINK_MAP(CBO_WORK_DUP_SEARCH, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_WORK_DUP_SEARCH)
	ON_EVENT(CBO_WORK_DUP_SEARCH, IDC_gridMAINGRID, -600 /* Click */, OnClickgridMAINGRID, VTS_NONE)
	ON_EVENT(CBO_WORK_DUP_SEARCH, IDC_gridMAINGRID, -605 /* MouseDown */, OnMouseDowngridMAINGRID, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CBO_WORK_DUP_SEARCH, IDC_gridMAINGRID, -601 /* DblClick */, OnDblClickgridMAINGRID, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_WORK_DUP_SEARCH::OnClickgridMAINGRID() 
{
	// TODO: Add your control notification handler code here
/*	m_pDupMgr->InitTempData();
	m_pDupMgr->SetTempData(_T("CMAlias"), _T("CM_�ߺ�����"));
	m_pDupMgr->SetTempData(_T("DMAlias"), _T("DM_�ߺ�������"));
	m_pDupMgr->SetTempData(_T("GridAlias"), _T("MainGrid2"));
	m_pDupMgr->SetTempData(_T("��������"), _T("MainGrid"));
	m_pDupMgr->SetTempData(_T("Index"), -1);
	INT ids = m_pDupMgr->SPFExecutePath(_T("�׸��弱��ó��"));

	if(ids < 0)
	{
		//����ó��
		return;
	}
*/
}

VOID CBO_WORK_DUP_SEARCH::OnbtnSELSOURCEDATA() 
{
	// TODO: Add your control notification handler code here
	m_pDupMgr->InitTempData();
	m_pDupMgr->SetTempData(_T("CMAlias"), _T("CM_�ߺ�����"));
	m_pDupMgr->SetTempData(_T("DMAlias"), _T("DM_�ߺ�������"));
	m_pDupMgr->SetTempData(_T("GridAlias"), _T("MainGrid2"));
	m_pDupMgr->SetTempData(_T("��������"), _T("All"));
	m_pDupMgr->SetTempData(_T("Index"), -1);
	INT ids = m_pDupMgr->SPFExecutePath(_T("�׸��弱��ó��"));
	if(ids < 0)
	{
		//����ó��
		return;
	}
}

BOOL CBO_WORK_DUP_SEARCH::GetIsDeleteOperation()
{
	return m_isDeleteOperation;
}

INT CBO_WORK_DUP_SEARCH::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strTmpData, strWorkCode;
	INT nGroupNo, nWorkNo, nSerialNo;
	
	strTmpData = m_pDM->GetCellData(_T("IBS_��������"), nRow);
	strTmpData.TrimLeft();	strTmpData.TrimRight();
	INT nPos = strTmpData.Find(_T(" "), 0);
	if(nPos >= 0)
	{
		strTmpData = strTmpData.Left(nPos);
		m_pDM->SetCellData(_T("IBS_��������"), strTmpData, nRow);
	}

	if(strDMFieldAlias == _T("BP_��������"))
	{
		GetPurchaseGroupInfo(str, strAcqYear, strGroupNo, strWorkNo, strSerialNo);
		strWorkCode = m_pDM->GetCellData(_T("IBS_��������"), nRow);
		nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
		nWorkNo = _ttoi(strWorkNo.GetBuffer(0));
		nSerialNo = _ttoi(strSerialNo.GetBuffer(0));
		str.Format(_T("%s-%s-%04d-%03d-%05d"), strAcqYear, strWorkCode, nGroupNo, nWorkNo, nSerialNo);

		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("��������"), strWorkCode, strWorkCode);
		m_pDM->SetCellData(_T("UDF_��������"), strWorkCode, nRow);
		//=====================================================
		//2009.01.07 ADD BY PJW : DM�� ������ȣ�� �����͸� �߰��Ѵ�.
		m_pDM->SetCellData(_T("������ȣ"), str, nRow);
		//=====================================================
	}	
	else if(strDMFieldAlias == _T("�ĺ���ȣ"))
	{
		if(str != _T("**"))
		{
			SetGridRowBackColor(m_pGrid, nRow);
		}
	}	
	
	return 0;
}

INT CBO_WORK_DUP_SEARCH::GetPurchaseGroupInfo(CString strSrc, CString &strAcqYear, CString &strGroupNo, CString &strWorkNo, CString &strSerialNo)
{
	CStringArray arrData;
	m_pDupMgr->ParsingString(_T("-"), strSrc, &arrData, FALSE);
	if(arrData.GetSize() > 3)
	{
		strAcqYear = arrData.GetAt(0);
		strGroupNo = arrData.GetAt(1);
		strWorkNo = arrData.GetAt(2);
		strSerialNo = arrData.GetAt(3);
	}
	return 0;	
}

VOID CBO_WORK_DUP_SEARCH::OnMouseDowngridMAINGRID(short Button, short Shift, long x, long y) 
{
	// TODO: Add your control notification handler code here
	INT row, col;
	row = m_pGrid->GetRow();
	col = m_pGrid->GetCol();

	// click�� �÷��� 2 �϶� ����Ʈ ���� üũ�Ѵ�. 
	if (Button == 1) {
		switch( col )
		{
			case 1 :
				CheckActiveValue( row, 1 );
				break;
			default :
				break;
		}		
	}
	if (Button == 2) {	// ������ ��ư�϶�
		if (col == 1) {
			FreeMenu();
			m_pMenu = new CMenu;
			if (m_pMenu == NULL) return;
			m_pMenu->CreatePopupMenu();

			m_pMenu->AppendMenu( MF_STRING , 5000 , _T("����") );
			m_pMenu->AppendMenu( MF_STRING , 5001 , _T("����") );
			m_pMenu->AppendMenu( MF_STRING , 5002 , _T("��ü����") );
			m_pMenu->AppendMenu( MF_STRING , 5003 , _T("��ü����") );
			CPoint pos;
			GetCursorPos(&pos);
			SetForegroundWindow();
			m_pMenu->TrackPopupMenu(TPM_LEFTALIGN,pos.x,pos.y,this);
		}
	}	
}

VOID CBO_WORK_DUP_SEARCH::CheckActiveValue( INT row, INT option )
{
	if( _T("V") == m_pGrid->GetText() )
	{
		m_pGrid->SetTextMatrix( row, 1, _T("") );
	}
	else
	{
		if(m_pDM->GetCellData(_T("�ĺ���ȣ"), row-1) != _T("**"))
			m_pGrid->SetTextMatrix( row, 1, _T("V") );
	}
}

VOID CBO_WORK_DUP_SEARCH::OnMSGBOKBONSEARCH( UINT  nIDC )
{
	INT nRows = m_pGrid->GetRows();
	INT row = m_pGrid->GetRow();
	INT col = 1;

	INT i;

	INT top, bottom;
	top = m_pGrid->GetRow();
	bottom = m_pGrid->GetRowSel();

	INT tmp;
	if ( top < bottom ) {
		tmp = bottom;
		bottom = top;
		top = tmp;
	}

	// ����
	if (nIDC == 5000) {
		for ( i = bottom ; i <= top ; i++ ) 
		{
			if(m_pDM->GetCellData(_T("�ĺ���ȣ"), i-1) != _T("**"))
				m_pGrid->SetTextMatrix(i, col, _T("V"));
		}
	}
	// ����
	if (nIDC == 5001) {
		for ( i = bottom ; i <= top ; i++ ) 
			m_pGrid->SetTextMatrix(i, col, _T(""));
	}
	// ��ü����
	if (nIDC == 5002) {
		for (i = 1; i < nRows; i++)
		{
			if(m_pDM->GetCellData(_T("�ĺ���ȣ"), i-1) != _T("**"))
				m_pGrid->SetTextMatrix(i, col, _T("V"));
		}
	}
	// ��ü����
	if (nIDC == 5003) {
		for (i = 1; i < nRows; i++) m_pGrid->SetTextMatrix(i, col, _T(""));
	}

	return;
}

VOID CBO_WORK_DUP_SEARCH::FreeMenu(VOID)
{
	if (m_pMenu)
	{
		m_pMenu->DestroyMenu();
		delete m_pMenu;
		m_pMenu = NULL;
	}

	return;
}

INT CBO_WORK_DUP_SEARCH::SetGridRowBackColor(CESL_Grid *pGrid, INT nRow)
{
	if (pGrid == NULL) return -1;

	COLORREF red, white, black;
	
	red = RGB(251,196,234);
	white = RGB(255,255,255);
	black = RGB(0,0,0);

	INT nCol = pGrid->GetCol();
	INT nCols = pGrid->GetCols(0);
	
	if (nRow >= 0)
	{
		pGrid->SetRow(nRow + 1);
		
		for (INT i = 1 ; i < nCols; i++)
		{
			pGrid->SetCol(i);
			pGrid->SetCellBackColor(red);
			pGrid->SetCellForeColor(black);
		}
	}
	
	pGrid->SetCol(nCol);

	return 0;
}

VOID CBO_WORK_DUP_SEARCH::OnDblClickgridMAINGRID() 
{
	// TODO: Add your control notification handler code here
	if(m_pGrid->GetMouseRow() == 0)
	{
		SortDuplicateData(m_pGrid);
		return;
	}	
	ShowDetailInfo(GUPDATE);
	return ;
}

INT CBO_WORK_DUP_SEARCH::SortDuplicateData(CESL_Grid *pGrid)
{
	CString strSortAlias;
	if(pGrid)
	{
		INT nCol = pGrid->GetCol();
		POSITION pos = pGrid->m_arrColumnInfo.FindIndex(nCol-1);
		CESL_GridColumnInfo *pColumnInfo = (CESL_GridColumnInfo*)pGrid->m_arrColumnInfo.GetAt(pos);
		if(pColumnInfo == NULL) return -1;
		strSortAlias = (CString)pColumnInfo->strColumnDMFieldAlias;
		if(m_pDupMgr == NULL) return 0;
	}
	else
	{
		strSortAlias = _T("IBS_�ڷ����");
		m_nSortDirection = ESL_DATAMGR_SORT_ASC;
	}
	m_pDupMgr->InitTempData();
	m_pDupMgr->InitTempPtr();
	m_pDupMgr->SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	m_pDupMgr->SetTempData(_T("SORT_FIELD_ALIAS"), strSortAlias.GetBuffer(0));
	m_pDupMgr->SetTempData(_T("SORT_DIRECTION"), m_nSortDirection);
	INT ids = m_pDupMgr->SPFExecutePath(_T("�ڷ�����"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�ڷḦ ������ �� �����ϴ�."));
		return -1;
	}
	if(m_nSortDirection == ESL_DATAMGR_SORT_ASC) m_nSortDirection = ESL_DATAMGR_SORT_DESC;
	else m_nSortDirection = ESL_DATAMGR_SORT_ASC;
	DisplayGrid();
	return 0;
}

VOID CBO_WORK_DUP_SEARCH::DisplayGrid()
{
	BOOL bSetTopRow = TRUE;
	INT nCurSel = m_pGrid->GetTopRow();
	if(nCurSel < 0) bSetTopRow = FALSE;
	if(m_pDM->GetRowCount() == 0) bSetTopRow = FALSE;
	if(nCurSel > m_pDM->GetRowCount()) bSetTopRow = FALSE;
	m_pGrid->Display();
	if(bSetTopRow) m_pGrid->SetTopRow(nCurSel);
}

INT CBO_WORK_DUP_SEARCH::ShowDetailInfo(OPERATION_TYPE nOpType)
{
	CBO_WORK_DUP_DETAIL_INFO DetailInfoDlg(this);
	DetailInfoDlg.SetMainDM(m_pDM);
	DetailInfoDlg.SetMainGrid(m_pGrid);
	DetailInfoDlg.SetOpenMode(nOpType);
	DetailInfoDlg.DoModal();
	return 0;
}

BOOL CBO_WORK_DUP_SEARCH::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if ( pMsg->message == WM_KEYDOWN ) 
	{
		if ( pMsg->wParam == VK_F12 ) 
		{
			return TRUE;
		}
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CBO_WORK_DUP_SEARCH::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_pGrid->GetSafeHwnd() == NULL) return;
	
	CRect rcWnd;
	GetWindowRect(rcWnd);
	
	if (rcWnd.Width() < m_rcInitial.Width())
	{
		rcWnd.right = rcWnd.left + m_rcInitial.Width();
		if (rcWnd.Height() < m_rcInitial.Height())
			rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	
		
		MoveWindow(rcWnd);
		return;
	}
	
	if (rcWnd.Height() < m_rcInitial.Height())
	{
		rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	
		if (rcWnd.Width() < m_rcInitial.Width())
			rcWnd.right = rcWnd.left + m_rcInitial.Width();			
		
		MoveWindow(rcWnd);
		return;
	}
	
	CRect rect;
	GetClientRect(rect);
	rect.top += 190;
	rect.bottom -= 4;
	rect.left += 4;
	rect.right -= 4;
	
	m_pGrid->MoveWindow(rect);			
}

HBRUSH CBO_WORK_DUP_SEARCH::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

//========================================================================
//2008.08.26 ADD BY PJW : �������缳���� �������� ���̾�α׿��� �����ϵ��� ����
INT CBO_WORK_DUP_SEARCH::InitDupKeySetting()
{
	this->SetDisplayData();
	m_bInit = TRUE;
	OnchkAUTHOR2();
	OnchkISBN2();
	OnchkMEDCLASS2();
	OnchkPUBLICATEYEAR2();
	OnchkPUBLISHER2();
	OnchkREGCODE2();
	OnchkSEPARATESHELFCODE2();
	OnchkTITLE2();
	OnchkTITLETWICESEARCH2();
	OnchkVOL2();
	OnchkVOLTITLE2();
	m_bInit = FALSE;

	// 2007.06.01 ADD BY PDJ
	// �������� �����ڷ� ���Կ��� ���� �߰���
	// ���� ��İ� ������ �޶� ������ ���Ϸ� �߰���.
	// ���� ��ġ�� ���� ������ overwrite ������ ������ ������ ��.
	LoadAppendCondition();
	return 0;
}

VOID CBO_WORK_DUP_SEARCH::SetDisplayData()
{
	INT titleStype;
	CButton * pRadTitleAll = (CButton *)GetDlgItem(IDC_radALLWORDSEARCH2);
	CButton * pRadRight = (CButton *)GetDlgItem(IDC_radTITLE_RIGHTTRUNCATION2);
	CButton * pRadEqual = (CButton *)GetDlgItem(IDC_radTITLE_EQUIVALENCE2);
	CButton * pRadTitleEng = (CButton *)GetDlgItem(IDC_radTITLE_CUTSEARCH2);
	CButton * pChkTwice = (CButton *)GetDlgItem(IDC_chkTITLE_TWICE_SEARCH2);
	INT ids = m_SearchKey.GetFieldKeySettingInfo(_T("��ǥ��"), &m_chkTitle, &titleStype, &m_chkTwice);
	if(ids < 0) 
	{
		m_SearchKey.SetDefaultSearchKeys();
	}

	if(titleStype == 2)
	{
		pRadTitleEng->SetCheck(1);
		this->m_radTitle = -1;
		pChkTwice->EnableWindow(FALSE);
	}
	else
	{
		pRadTitleAll->SetCheck(1);
		if(titleStype < 0)
		{
			m_SearchKey.GetFieldKeySettingInfo(_T("��ǥ��"), &m_chkTitle, &titleStype, &m_chkTwice);
		}
		this->m_radTitle = titleStype;		
	}
	

//	this->m_chkTwice = 0;

	ids = m_SearchKey.GetFieldKeySettingInfo(_T("������"), &this->m_chkAuthor, &this->m_radAuthor, NULL);
	if(ids < 0) return;
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("������"), &this->m_chkPublisher, &this->m_radPublisher, NULL);
	if(ids < 0) return ;
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("�����"), &this->m_chkPubYear, &this->m_radPubYear, NULL);
	if(ids < 0) return;
	this->m_radPubYear = this->m_radPubYear - 1;
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("����ISBN"), &this->m_chkISBN, &this->m_radISBN, NULL);
	if(ids < 0) return;
	this->m_radISBN = this->m_radISBN - 1;
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("��ü����"), &this->m_chkMedClass, &this->m_radMedCode, NULL);
	if(ids < 0) return;
	this->m_radMedCode = this->m_radMedCode - 1;
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("�����"), &this->m_chkVol, &this->m_radVol, NULL);
	if(ids < 0) return;
	this->m_radVol = this->m_radVol - 1;
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("�Ǽ���"), &this->m_chkVolTitle, &this->m_radVolTitle, NULL);
	if(ids < 0) return;
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("��ġ��ȣ"), &this->m_chkShelfCode, &this->m_radShelfCode, NULL);
	if(ids < 0) return;
	this->m_radShelfCode = this->m_radShelfCode - 1;
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("��ϱ���"), &this->m_chkRegCode, &this->m_radRegCode, NULL);
	if(ids < 0) return;
	this->m_radRegCode = this->m_radRegCode -1;	

	this->UpdateData(FALSE);

}
VOID CBO_WORK_DUP_SEARCH::GetDisplayData()
{
	this->UpdateData(TRUE);

	INT ids;
	CButton * pRadTitleAll = (CButton *)GetDlgItem(IDC_radALLWORDSEARCH2);
	if(pRadTitleAll->GetCheck() == 1)
	{
		ids = m_SearchKey.SetFieldKeySettingInfo(_T("��ǥ��"), this->m_chkTitle, this->m_radTitle, this->m_chkTwice);
	}
	else
	{
		ids = m_SearchKey.SetFieldKeySettingInfo(_T("��ǥ��"), this->m_chkTitle, 2, this->m_chkTwice);
	}
	if(ids < 0) return;
	
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("������"), this->m_chkAuthor, this->m_radAuthor, FALSE);
	if(ids < 0) return;
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("������"), this->m_chkPublisher, this->m_radPublisher, FALSE);
	if(ids < 0) return ;
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("�����"), this->m_chkPubYear, this->m_radPubYear+1, FALSE);
	if(ids < 0) return;
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("����ISBN"), this->m_chkISBN, this->m_radISBN + 1, FALSE);
	if(ids < 0) return;
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("��ü����"), this->m_chkMedClass, this->m_radMedCode + 1, FALSE);
	if(ids < 0) return;
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("�����"), this->m_chkVol, this->m_radVol + 1, FALSE);
	if(ids < 0) return;
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("�Ǽ���"), this->m_chkVolTitle, this->m_radVolTitle, FALSE);
	if(ids < 0) return;
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("��ġ��ȣ"), this->m_chkShelfCode, this->m_radShelfCode + 1, FALSE);
	if(ids < 0) return;
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("��ϱ���"), this->m_chkRegCode, this->m_radRegCode + 1, FALSE);
	if(ids < 0) return;	
}

BOOL CBO_WORK_DUP_SEARCH::CheckDupSearchElements()
{
	if(!m_chkVolTitle && !m_chkTitle && !m_chkAuthor && !m_chkPublisher && !m_chkPubYear 
		&& !m_chkISBN && !this->m_chkMedClass && !this->m_chkRegCode && !this->m_chkShelfCode
		&& !this->m_chkVol)
	{
		ESLAfxMessageBox(_T("��� �� �� �̻��� �����׸��� �����ϼž� �մϴ�."));
		return FALSE;
	}

	// 17/05/10 ������ : �����˻� ���� ('����˻�', 'ISBN�˻�'���� ����)
/*//BEFORE----------------------------------------------------------------------------------------
	if(!m_chkTitle && !m_chkAuthor && !m_chkPublisher )
	{
		ESLAfxMessageBox(_T("[����]�� �����׸����� �������� ���� ���\n[������]�� [������] �׸��� �����׸����� �����Ǿ�� �մϴ�."));
		PostMessage(WM_COMMAND, IDC_chkAUTHOR2, BN_CLICKED);
		PostMessage(WM_COMMAND, IDC_chkPUBLISHER2, BN_CLICKED);
		return FALSE;
	}
*///AFTER-----------------------------------------------------------------------------------------
	if(!m_chkTitle && !m_chkISBN && !m_chkAuthor && !m_chkPublisher )
	{
		ESLAfxMessageBox(_T("[����] �Ǵ� [ISBN]�� �����׸����� �������� ���� ���\n[������]�� [������] �׸��� �����׸����� �����Ǿ�� �մϴ�."));
		PostMessage(WM_COMMAND, IDC_chkAUTHOR1, BN_CLICKED);
		PostMessage(WM_COMMAND, IDC_chkPUBLISHER1, BN_CLICKED);
		return FALSE;
	}
/*//END-------------------------------------------------------------------------------------------*/

	return TRUE;
}

VOID CBO_WORK_DUP_SEARCH::LoadAppendCondition()
{	
	CStdioFile file;		
	CString strLine;
	CString filename = _T("duplicate_search_key2.txt");	

	if ( !file.Open(filename, CFile::modeRead | CFile::typeBinary )) 
	{
		AfxMessageBox(_T("�������� ����Ű�� �������� �ʽ��ϴ�."));
		return;
	}	
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	while( file.ReadString(strLine) )
	{			
		if ( strLine.Find(_T("[�����ڷ����Կ���]") ) >= 0 )
		{
			file.ReadString(strLine);
			if ( strLine.Find(_T("USE_YN=1") )  >= 0 )
			{
				m_bIncludeDiscardedBook = TRUE; 
			}
			else 
			{
				m_bIncludeDiscardedBook = FALSE;				
			}
		}		
	}

	file.Close();

	UpdateData(FALSE);	
}

VOID CBO_WORK_DUP_SEARCH::SaveAppendCondition()
{
	UpdateData(TRUE);		
	
	CStdioFile file;		
	CString strLine;
	CString filename = _T("duplicate_search_key2.txt");	

	m_bIncludeDiscardedBook = ( (CButton*) GetDlgItem(IDC_chkDiscardBook2) )->GetCheck();

	if ( !file.Open(filename, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary )) 
	{
		AfxMessageBox(_T("�������� ����Ű�� �������� �ʽ��ϴ�."));
		return;
	}
		
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}

	if ( m_bIncludeDiscardedBook == TRUE) 
	{
		strLine = _T("[�����ڷ����Կ���]\nUSE_YN=1\n");
	}
	else
	{
		strLine = _T("[�����ڷ����Կ���]\nUSE_YN=0\n");
	}
	file.WriteString(strLine);
	file.Close();	
}

VOID CBO_WORK_DUP_SEARCH::DupKeySave() 
{
	// TODO: Add extra validation here
	if(!CheckDupSearchElements()) return;
	GetDisplayData();
	m_SearchKey.GetSearchKeys(m_FileName);
	SaveAppendCondition();
// 	ESLAfxMessageBox(_T("�����Ͻ� �ߺ�/�������� Ű���� �����Ͽ����ϴ�."));
}
//========================================================================

////========================================================================
//2008.08.26 ADD BY PJW : �������縦 �������� ���̾�α׿��� �����ϵ��� ����
VOID CBO_WORK_DUP_SEARCH::SearchDuplicateData()
{
	INT searchCount = -1;	
	m_pDupMgr->SetKeyFileName(_T(".\\duplicate_search_key.txt"));
	m_pDupMgr->InitTempData();
	m_pDupMgr->SetTempData(_T("CMAlias"), m_strParentCMAlias.GetBuffer(0));
	m_pDupMgr->SetTempData(_T("SourceDMAlias"), m_strParentDMAlias.GetBuffer(0));
	m_pDupMgr->SetTempData(_T("GridAlias"), m_strParentGridAlias.GetBuffer(0));
	m_pDupMgr->SetTempData(_T("CopyDMAlias"), _T("DM_�ߺ�������_���纻"));
	m_pDupMgr->SetTempData(_T("TargetDMAlias"), _T("DM_�ߺ�������"));
	m_pDupMgr->SetTempData(_T("KeyDMAlias"), _T("DM_�ߺ�������Ű"));
//	m_pDupMgr->SetTempPtr(_T("DataProcessingProgressCtrl"), (DWORD*)m_pPrgCtrl);
	if(m_pGroup != NULL) m_pDupMgr->SetTempPtr(_T("GroupInfo"), (DWORD*)m_pGroup);
	if(m_pGroupCollection != NULL) m_pDupMgr->SetTempPtr(_T("GroupCollection"), (DWORD*)m_pGroupCollection);

		
	INT ids = m_pDupMgr->SPFExecutePath(m_strSearchType);
	m_pDupMgr->GetTempData(_T("SearchCount"), searchCount);
	m_nSearchCount = searchCount;
	
	if( searchCount < 1 )
	{
		ESLAfxMessageBox(_T("�ߺ��ڷᰡ �����ϴ�."));

	}
}
////========================================================================

void CBO_WORK_DUP_SEARCH::OnchkAUTHOR2() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkAuthor = !this->m_chkAuthor;
	CButton * pchkRight = (CButton*)GetDlgItem(IDC_radAUTHOR_RIGHTTRUNCATION2);
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radAUTHOR_EQUIVALENCE2);
	if(this->m_chkAuthor)
	{
		if(this->m_radAuthor == -1)
			this->m_radAuthor = 0;
		pchkRight->EnableWindow(TRUE);
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radAuthor = -1;
		pchkRight->EnableWindow(FALSE);
		pchkEqui->EnableWindow(FALSE);
	}
	this->UpdateData(FALSE);
	
}

void CBO_WORK_DUP_SEARCH::OnchkDiscardBook2() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(TRUE);
}

void CBO_WORK_DUP_SEARCH::OnchkISBN2() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkISBN = !this->m_chkISBN;
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radISBN_EQUIVALENCE2);
	if(this->m_chkISBN)
	{
		this->m_radISBN = 0;
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radISBN = -1;
		pchkEqui->EnableWindow(FALSE);
	}	
	this->UpdateData(FALSE);	
	
}

void CBO_WORK_DUP_SEARCH::OnchkMEDCLASS2() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkMedClass = !this->m_chkMedClass;
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radMEDIACODE_EQUIVALENCE2);
	if(this->m_chkMedClass)
	{
		this->m_radMedCode = 0;
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radMedCode = -1;
		pchkEqui->EnableWindow(FALSE);
	}	
	this->UpdateData(FALSE);	
	
}

void CBO_WORK_DUP_SEARCH::OnchkPUBLICATEYEAR2() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkPubYear = !this->m_chkPubYear;
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radPUBLICATE_YEAR_EQUIVALENCE2);
	if(this->m_chkPubYear)
	{
		this->m_radPubYear = 0;
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radPubYear = -1;
		pchkEqui->EnableWindow(FALSE);
	}	
	this->UpdateData(FALSE);
	
}

void CBO_WORK_DUP_SEARCH::OnchkPUBLISHER2() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkPublisher = !this->m_chkPublisher;
	CButton * pchkRight = (CButton*)GetDlgItem(IDC_radPUBLISHER_RIGHTTRUNCATION2);
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radPUBLISHER_EQUIVALENCE2);
	if(this->m_chkPublisher)
	{
		if(this->m_radPublisher == -1)
			this->m_radPublisher = 0;
		pchkRight->EnableWindow(TRUE);
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radPublisher = -1;
		pchkRight->EnableWindow(FALSE);
		pchkEqui->EnableWindow(FALSE);
	}	
	this->UpdateData(FALSE);
	
}

void CBO_WORK_DUP_SEARCH::OnchkREGCODE2() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkRegCode = !this->m_chkRegCode;
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radREGCODE_EQUIVALENCE2);
	if(this->m_chkRegCode)
	{
		this->m_radRegCode = 0;
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radRegCode = -1;
		pchkEqui->EnableWindow(FALSE);
	}	
	this->UpdateData(FALSE);	
	
}

void CBO_WORK_DUP_SEARCH::OnchkSEPARATESHELFCODE2() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkShelfCode = !this->m_chkShelfCode;
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radSEPARATECODE_EQUIVALENCE2);
	if(this->m_chkShelfCode)
	{
		this->m_radShelfCode = 0;
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radShelfCode = -1;
		pchkEqui->EnableWindow(FALSE);
	}	
	this->UpdateData(FALSE);	
	
}

void CBO_WORK_DUP_SEARCH::OnchkTITLE2() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkTitle = !this->m_chkTitle;
	CButton * prad1 = (CButton*)GetDlgItem(IDC_radALLWORDSEARCH2);
	CButton * prad2 = (CButton*)GetDlgItem(IDC_radTITLE_RIGHTTRUNCATION2);
	CButton * prad3 = (CButton*)GetDlgItem(IDC_radTITLE_EQUIVALENCE2);
	CButton * prad4 = (CButton*)GetDlgItem(IDC_radTITLE_CUTSEARCH2);
	CButton * pchk = (CButton*)GetDlgItem(IDC_chkTITLE_TWICE_SEARCH2);
	if(this->m_chkTitle)
	{
		prad1->EnableWindow(TRUE);
		prad2->EnableWindow(TRUE);
		prad3->EnableWindow(TRUE);
		prad4->EnableWindow(TRUE);
		pchk->EnableWindow(TRUE);
	}
	else
	{
		if(!m_bInit)
			ESLAfxMessageBox(_T("���������� ������ ��� ����/�ߺ����� ����� ������ �� �ֽ��ϴ�.\n�� ������ ����/�ߺ����� �ӵ��� ���� ��ų�� �ֽ��ϴ�."));
		prad1->EnableWindow(FALSE);
		prad2->EnableWindow(FALSE);
		prad3->EnableWindow(FALSE);
		prad4->EnableWindow(FALSE);
		pchk->EnableWindow(FALSE);
	}
	this->UpdateData(FALSE);
	
}

void CBO_WORK_DUP_SEARCH::OnchkTITLETWICESEARCH2() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkTwice = !this->m_chkTwice;
	CButton * pradCut = (CButton*)GetDlgItem(IDC_radTITLE_CUTSEARCH2);
	CButton * pchkTwice = (CButton*)GetDlgItem(IDC_chkTITLE_TWICE_SEARCH2);
	if(pradCut->GetCheck() == 1)
	{
		pchkTwice->EnableWindow(FALSE);
	}
	else
	{
		pchkTwice->EnableWindow(TRUE);
	}

	// 17/05/10 ������ : �����˻� ���� ('����˻�', 'ISBN�˻�'���� ����)
//*/ ADD ---------------------------------------------------------------------------
	if ( m_chkTitle == 0 )
	{
		pchkTwice->EnableWindow(FALSE);
	}
//*/ END ---------------------------------------------------------------------------

	this->UpdateData(FALSE);
}

void CBO_WORK_DUP_SEARCH::OnchkVOL2() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkVol = !this->m_chkVol;
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radVOL_EQUIVALENCE2);
	if(this->m_chkVol)
	{
		this->m_radVol = 0;
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radVol = -1;
		pchkEqui->EnableWindow(FALSE);
	}	
	this->UpdateData(FALSE);	
}

void CBO_WORK_DUP_SEARCH::OnchkVOLTITLE2() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkVolTitle = !this->m_chkVolTitle;
	CButton * pchkRight = (CButton*)GetDlgItem(IDC_radVOL_TITLE_RIGHTTRUNCATION2);
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radVOL_TITLE_EQUIVALENCE2);
	if(this->m_chkVolTitle)
	{
		if(this->m_radVolTitle == -1)
			this->m_radVolTitle = 0;
		pchkRight->EnableWindow(TRUE);
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radPublisher = -1;
		pchkRight->EnableWindow(FALSE);
		pchkEqui->EnableWindow(FALSE);
	}	
	this->UpdateData(FALSE);	
}

void CBO_WORK_DUP_SEARCH::OnradAUTHOREQUIVALENCE2() 
{
	// TODO: Add your control notification handler code here
	this->m_chkAuthor = TRUE;
	this->m_radAuthor = 1;
	this->UpdateData(FALSE);
	
}

void CBO_WORK_DUP_SEARCH::OnradAUTHORRIGHTTRUNCATION2() 
{
	// TODO: Add your control notification handler code here
	this->m_chkAuthor = TRUE;
	this->m_radAuthor = 0;
	this->UpdateData(FALSE);
	
}

void CBO_WORK_DUP_SEARCH::OnradMEDIACODEEQUIVALENCE2() 
{
	// TODO: Add your control notification handler code here
	this->m_chkMedClass = TRUE;
	this->m_radMedCode = 0;
	this->UpdateData(FALSE);
	
}

void CBO_WORK_DUP_SEARCH::OnradPUBLICATEYEAREQUIVALENCE2() 
{
	// TODO: Add your control notification handler code here
	this->m_chkPubYear = TRUE;
	this->m_radPubYear = 0;
	this->UpdateData(FALSE);	
	
}

void CBO_WORK_DUP_SEARCH::OnradPUBLISHEREQUIVALENCE2() 
{
	// TODO: Add your control notification handler code here
	this->m_chkPublisher = TRUE;
	this->m_radPublisher = 1;
	this->UpdateData(FALSE);
	
}

void CBO_WORK_DUP_SEARCH::OnradPUBLISHERRIGHTTRUNCATION2() 
{
	// TODO: Add your control notification handler code here
	this->m_chkPublisher = TRUE;
	this->m_radPublisher = 0;
	this->UpdateData(FALSE);
	
}

void CBO_WORK_DUP_SEARCH::OnradREGCODEEQUIVALENCE2() 
{
	// TODO: Add your control notification handler code here
	this->m_chkRegCode = TRUE;
	this->m_radRegCode = 0;
	this->UpdateData(FALSE);	
	
}

void CBO_WORK_DUP_SEARCH::OnradSEPARATECODEEQUIVALENCE2() 
{
	// TODO: Add your control notification handler code here
	this->m_chkShelfCode = TRUE;
	this->m_radShelfCode = 0;
	this->UpdateData(FALSE);
	
}

void CBO_WORK_DUP_SEARCH::OnradTITLECUTSEARCH2() 
{
	// TODO: Add your control notification handler code here
	CButton * pradCut = (CButton*)GetDlgItem(IDC_radTITLE_CUTSEARCH2);
	CButton * pchkTwice = (CButton*)GetDlgItem(IDC_chkTITLE_TWICE_SEARCH2);
	CButton * pchkRight = (CButton*)GetDlgItem(IDC_radTITLE_RIGHTTRUNCATION2);
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radTITLE_EQUIVALENCE2);
	if(pradCut->GetCheck() == 1)
	{
		//this->m_radTitle = -1;
		pchkRight->EnableWindow(FALSE);
		pchkEqui->EnableWindow(FALSE);
		pchkTwice->EnableWindow(FALSE);
	}
	else
	{
		//this->m_radTitle = -1;
		pchkRight->EnableWindow(TRUE);
		pchkEqui->EnableWindow(TRUE);
		pchkTwice->EnableWindow(TRUE);
	}
	this->UpdateData(FALSE);	
	
}

void CBO_WORK_DUP_SEARCH::OnradTITLEEQUIVALENCE2() 
{
	// TODO: Add your control notification handler code here
	CButton * pradCut = (CButton*)GetDlgItem(IDC_radTITLE_CUTSEARCH2);
	CButton * pchkTwice = (CButton*)GetDlgItem(IDC_chkTITLE_TWICE_SEARCH2);
	CButton * pradAllWord = (CButton*)GetDlgItem(IDC_radALLWORDSEARCH2);
	if(pradCut->GetCheck() == 1)
		this->m_radTitle = -1;
	else
	{
		this->m_radTitle = 1;
		pradAllWord->SetCheck(1);
	}
	
	this->UpdateData(FALSE);	
	
}

void CBO_WORK_DUP_SEARCH::OnradTITLERIGHTTRUNCATION2() 
{
	// TODO: Add your control notification handler code here
	CButton * pradCut = (CButton*)GetDlgItem(IDC_radTITLE_CUTSEARCH2);
	CButton * pchkTwice = (CButton*)GetDlgItem(IDC_chkTITLE_TWICE_SEARCH2);
	CButton * pradAllWord = (CButton*)GetDlgItem(IDC_radALLWORDSEARCH2);
	if(pradCut->GetCheck() == 1)
		this->m_radTitle = -1;
	else
	{
		this->m_radTitle = 0;
		pradAllWord->SetCheck(1);
	}
	
	this->UpdateData(FALSE);
	
}

void CBO_WORK_DUP_SEARCH::OnradVOLEQUIVALENCE2() 
{
	// TODO: Add your control notification handler code here
	this->m_chkVol = TRUE;
	this->m_radVol = 0;
	this->UpdateData(FALSE);	
}

void CBO_WORK_DUP_SEARCH::OnradVOLTITLEEQUIVALENCE2() 
{
	// TODO: Add your control notification handler code here
	this->m_chkVolTitle = TRUE;
	this->m_radVolTitle = 1;
	this->UpdateData(FALSE);
	
}

void CBO_WORK_DUP_SEARCH::OnradISBNEQUIVALENCE2() 
{
	// TODO: Add your control notification handler code here
	this->m_chkISBN = TRUE;
	this->m_radISBN = 0;
	this->UpdateData(FALSE);
	
}

void CBO_WORK_DUP_SEARCH::OnradVOLTITLERIGHTTRUNCATION2() 
{
	// TODO: Add your control notification handler code here
	this->m_chkVolTitle = TRUE;
	this->m_radVolTitle = 0;
	this->UpdateData(FALSE);	
	
}


//void CBO_HOLDING_ALL_DUP_SEARCH::OnbtnDUPFINDSTART() 
//{
//	DupKeySave();
//	SearchDuplicateData();	
//	DisplayGrid();
//	
//}




void CBO_WORK_DUP_SEARCH::OnradALLWORDSEARCH2() 
{
	// TODO: Add your control notification handler code here
	CButton * pradAllWord = (CButton*)GetDlgItem(IDC_radALLWORDSEARCH2);
	CButton * pchkTwice = (CButton*)GetDlgItem(IDC_chkTITLE_TWICE_SEARCH2);
	CButton * pchkRight = (CButton*)GetDlgItem(IDC_radTITLE_RIGHTTRUNCATION2);
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radTITLE_EQUIVALENCE2);
	if(pradAllWord->GetCheck() == 1)
	{
		if(this->m_radTitle == -1)
			this->m_radTitle = 0;
		pchkRight->EnableWindow(TRUE);
		pchkEqui->EnableWindow(TRUE);
		pchkTwice->EnableWindow(TRUE);
	}
	
	this->UpdateData(FALSE);
	
}

void CBO_WORK_DUP_SEARCH::OnbtnDUPFINDSTART2() 
{
	DupKeySave();
	SearchDuplicateData();	
	DisplayGrid();
	
}
