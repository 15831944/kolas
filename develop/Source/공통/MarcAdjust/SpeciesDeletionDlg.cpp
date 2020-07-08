// SpeciesDeletionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpeciesDeletionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpeciesDeletionDlg dialog


CSpeciesDeletionDlg::CSpeciesDeletionDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpeciesDeletionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM_Parent = NULL;
	m_pDM_Book = NULL;
	m_pCatApi = NULL;

	m_nParentGridSelectRow = -1;

	m_pGrid_Parent = NULL;
	m_pGrid_Species = NULL;
	m_pGrid_Book = NULL;

}

CSpeciesDeletionDlg::~CSpeciesDeletionDlg()
{
	
}

VOID CSpeciesDeletionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpeciesDeletionDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpeciesDeletionDlg, CDialog)
	//{{AFX_MSG_MAP(CSpeciesDeletionDlg)
	ON_BN_CLICKED(IDC_bDELETE_SPECIES, OnbDELETESPECIES)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpeciesDeletionDlg Operations

VOID CSpeciesDeletionDlg::SetParentInfo(CESL_DataMgr *pDM_Parent, CESL_Grid *pGrid_Parent, LPCTSTR lpszSpeciesKeyAlias, LPCTSTR lpszTitleAlias)
{
	m_pDM_Parent = pDM_Parent;
	m_pGrid_Parent = pGrid_Parent;
	m_strSpeciesKeyAlias = lpszSpeciesKeyAlias;
	m_strTitleAlias = lpszTitleAlias;
}

INT CSpeciesDeletionDlg::Init()
{
	if (!m_pDM_Parent || !m_pGrid_Parent || m_strSpeciesKeyAlias.IsEmpty()) return -1;

	m_pGrid_Species = (CESL_Grid*)FindControl(_T("CM_����_����_������"), _T("���׸���"));
	m_pGrid_Book = (CESL_Grid*)FindControl(_T("CM_����_����_������"), _T("å�׸���"));
	if (!m_pGrid_Species || !m_pGrid_Book) return -1;

	m_pGrid_Book->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	INT ids = -1;

	m_lpszDMAlias = _T("DM_����_����DM_å����");

	m_pCatApi = new CCatApi_BO(this);

	m_arrStrErrorWorkingStatus.Add(_T("BOL211O"));
	m_arrStrErrorWorkingStatus.Add(_T("BOL212O"));
	m_arrStrErrorWorkingStatus.Add(_T("BOL213O"));
	m_arrStrErrorWorkingStatus.Add(_T("BOL214O"));
	m_arrStrErrorWorkingStatus.Add(_T("BOL215O"));
	m_arrStrErrorWorkingStatus.Add(_T("BOL218O"));
	m_arrStrErrorWorkingStatus.Add(_T("BOL219O"));
	m_arrStrErrorWorkingStatus.Add(_T("BOL411O"));
	m_arrStrErrorWorkingStatus.Add(_T("BOL511O"));
	m_arrStrErrorWorkingStatus.Add(_T("BOL611O"));

	if (m_pDM_Parent->GetRowCount() <= 0) return -1;
	ids = m_pGrid_Parent->SelectMakeList();
	INT nSelectedRowCount = m_pGrid_Parent->SelectGetCount();
	if (nSelectedRowCount < 1)
	{
		m_strLastErrMsg = _T("������ �ڷᰡ �����ϴ�.");
		return -1;
	}
	else
		m_nParentGridSelectRow = m_pGrid_Parent->SelectGetHeadPosition();

	MakeBookDM();
	
	DisplaySpeciesGrid();

	DisplayBookGrid(0);

	return 0;
}

INT CSpeciesDeletionDlg::MakeBookDM()
{
	CString strOption;
	CString strSpeciesKey;
	CESL_DataMgr *pDM = NULL;
	INT ids;
	INT idx = m_pGrid_Parent->SelectGetHeadPosition();
	while (idx >= 0)
	{
		strSpeciesKey = m_pDM_Parent->GetCellData(m_strSpeciesKeyAlias, idx);
		if (strSpeciesKey.IsEmpty()) continue;
		
		pDM = new CESL_DataMgr;
		m_mapSpeciesKey_BookDM.SetAt(strSpeciesKey, pDM);

		pDM->SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);		
		pDM->InitDMPKFromFile(m_lpszDMAlias);
		pDM->SetMarcMgr(m_pInfo->pMarcMgr);		

		strOption.Format(_T("b.SPECIES_KEY = %s ORDER BY VOL_SORT_NO, REG_NO"), strSpeciesKey);
		ids = pDM->RefreshDataManager(strOption);
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("RefreshDataManager �� Ȯ�����ּ���"));
			return -1;
		}



		CheckDeletionError(strSpeciesKey, pDM);

		idx = m_pGrid_Parent->SelectGetNext();
	}

	POSITION pos = m_mapSpeciesKey_BookDM.GetStartPosition();
	if (pos == NULL) return -1;
	m_mapSpeciesKey_BookDM.GetNextAssoc(pos, strSpeciesKey, (VOID*&)m_pDM_Book);
	DMList.AddTail((CObject*)m_pDM_Book);

	return 0;
}

INT CSpeciesDeletionDlg::CheckDeletionError(LPCTSTR lpszSpeciesKey, CESL_DataMgr *pDM)
{
	if (!pDM) return -1;

	INT nRowCount = pDM->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		CString strWorkingStatus = pDM->GetCellData(_T("BB_�ڷ����"), idx);
		if (m_pCatApi->IsStringInArray(&m_arrStrErrorWorkingStatus, strWorkingStatus))
		{
			m_arrStrErrorSpeciesKey.Add(lpszSpeciesKey);
			return 0;
		}
	}
	
	return 0;
}

INT CSpeciesDeletionDlg::DisplaySpeciesGrid()
{
	INT nSelectedRowCount = m_pGrid_Parent->SelectGetCount();
	m_pGrid_Species->Clear();
	m_pGrid_Species->SetColumnInfo();
	m_pGrid_Species->SetRows(nSelectedRowCount + 1);

	INT nRow = 1;
	CString strRow;
	CString strSpeciesKey;
	INT idx = m_pGrid_Parent->SelectGetHeadPosition();
	while (idx >= 0)
	{
		strSpeciesKey = m_pDM_Parent->GetCellData(m_strSpeciesKeyAlias, idx);
		
		strRow.Format(_T("%d"), nRow);
		m_pGrid_Species->SetTextMatrix(nRow, 0, strRow);
		m_pGrid_Species->SetTextMatrix(nRow, 2, strSpeciesKey);
		m_pGrid_Species->SetTextMatrix(nRow, 3, m_pDM_Parent->GetCellData(m_strTitleAlias, idx));

		if (m_pCatApi->IsStringInArray(&m_arrStrErrorSpeciesKey, strSpeciesKey))
			m_pGrid_Species->SetTextMatrix(nRow, 1, _T("V"));
		
		nRow++;
		idx = m_pGrid_Parent->SelectGetNext();
	}

	return 0;
}

INT CSpeciesDeletionDlg::DisplayBookGrid(INT nRowIdx)
{
	CString strSpeciesKey = m_pGrid_Species->GetTextMatrix(nRowIdx + 1, 2);
	CESL_DataMgr *pDM = NULL;
	if (!m_mapSpeciesKey_BookDM.Lookup(strSpeciesKey, (VOID*&)pDM)) return -1;

	POSITION pos = NULL;
	POSITION posOld = NULL;
	pos = DMList.GetTailPosition();
	while (pos)
	{
		posOld = pos;
		if (m_pDM_Book == (CESL_DataMgr*)DMList.GetPrev(pos))
		{
			DMList.RemoveAt(posOld);
			break;
		}
	}

	m_pDM_Book = pDM;
	DMList.AddTail((CObject*)m_pDM_Book);

	m_pGrid_Book->Display();

	return 0;
}

INT CSpeciesDeletionDlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	strGridAlias.TrimLeft();		strGridAlias.TrimRight();
	
	if (strDMFieldAlias.Find(_T("�ڷ����")) >= 0)
		str = m_pCatApi->GetWorkingStatusDesc(str);
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CSpeciesDeletionDlg message handlers

BOOL CSpeciesDeletionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_����_����_������")) < 0)
	{
		m_strLastErrMsg = _T("Init ESL Mgr Error");
		goto ERR;
	}
	
	if (Init() < 0) goto ERR;

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
ERR:
	if (!m_strLastErrMsg.IsEmpty())
		AfxMessageBox(m_strLastErrMsg);
	EndDialog(IDCANCEL);
	return FALSE;
}

VOID CSpeciesDeletionDlg::OnbDELETESPECIES() 
{
	if ( MessageBox(_T("������ ��� �������� �����Ͻðڽ��ϱ�?\n�˻��� å �Ӹ� �ƴ϶� �ش� ���� �����˴ϴ�.\nå ������ �Ǻ��������� å�� �����Ͻðų� �谡�ڷ������ ��������� �̿��Ͻʽÿ�."), _T("������"), MB_YESNO | MB_ICONQUESTION) != IDYES )
	{
		return;
	}

	CStringArray arrStrSpeciesKey;
	CString strSpeciesKey;
	
	INT iFlag = 0;

	INT idx = m_pGrid_Parent->SelectGetHeadPosition();
	while (idx >= 0)
	{
		strSpeciesKey = m_pDM_Parent->GetCellData(m_strSpeciesKeyAlias, idx);

		if (!m_pCatApi->IsStringInArray(&m_arrStrErrorSpeciesKey, strSpeciesKey))
			arrStrSpeciesKey.Add(strSpeciesKey);
		else 
			iFlag = 1;		

		if ( m_pDM_Parent->GetCellData(_T("BS_��������"), idx) == _T("V") )
			iFlag = 2;
		idx = m_pGrid_Parent->SelectGetNext();
	}
	INT ids;

	ids = -1;
	if ( iFlag == 0)
		ids = m_pCatApi->DeleteSpecies(&arrStrSpeciesKey);

	INT nResult;
	if (ids >= 0)
	{
		m_strLastErrMsg = _T("�����Ͽ����ϴ�.");
		nResult = IDOK;
	}
	else
	{
		if ( iFlag == 1 )
			m_strLastErrMsg = _T("�������� �ڷ�� �����Ͻ� �� �����ϴ�.");
		else if ( iFlag == 2 )
			m_strLastErrMsg = _T("����ó���� �ڷ�� �����Ͻ� �� �����ϴ�.");
		else
			m_strLastErrMsg = _T("������ �����Ͽ����ϴ�.");
		nResult = IDCANCEL;
	}

	AfxMessageBox(m_strLastErrMsg);
	EndDialog(nResult);

	return;
}

VOID CSpeciesDeletionDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	CESL_DataMgr *pDM = NULL;
	POSITION pos = NULL;
	POSITION posOld = NULL;

	if (m_pDM_Book)
	{
		pos = DMList.GetTailPosition();
		while (pos)
		{
			posOld = pos;
			pDM = (CESL_DataMgr*)DMList.GetPrev(pos);
			if (pDM != m_pDM_Book) continue;

			DMList.RemoveAt(posOld);
			break;
		}
	}

	CString strKey;
	pos = m_mapSpeciesKey_BookDM.GetStartPosition();
	while (pos)
	{
		pDM = NULL;
		
		m_mapSpeciesKey_BookDM.GetNextAssoc(pos, strKey, (VOID*&)pDM);
		
		delete pDM;
	}

	m_mapSpeciesKey_BookDM.RemoveAll();

	delete m_pCatApi;
}

BEGIN_EVENTSINK_MAP(CSpeciesDeletionDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSpeciesDeletionDlg)
	ON_EVENT(CSpeciesDeletionDlg, IDC_GRID_SPECIES_MA, 70 /* RowColChange */, OnRowColChangeGridSpeciesMa, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSpeciesDeletionDlg::OnRowColChangeGridSpeciesMa() 
{
	if (m_pGrid_Species->GetMouseRow() == 0) return;
	
	DisplayBookGrid(m_pGrid_Species->GetRow() - 1);	
}

HBRUSH CSpeciesDeletionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
