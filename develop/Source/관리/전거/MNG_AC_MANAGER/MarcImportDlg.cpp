// FileImportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MarcImportDlg.h"
#include "..\..\..\ESL\SIReportMgr\SIReportManager.h"

#include "efs.h"

#ifndef OBM_SIZE
#define	OBM_SIZE		32766
// taken from WinresRc.h
// if not used for any reason
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcImportDlg dialog

CMarcImportDlg::CMarcImportDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcImportDlg)
	m_strFilePath = _T("");
	//}}AFX_DATA_INIT

	m_pThreadImport		= NULL;
	m_pThreadDisplay	= NULL;
	m_pParent			= pParent;
	m_bIsThreadRunning	= FALSE;

	m_pDM_TOTAL			= NULL;
	m_pDM_OK			= NULL;
	m_pDM_ERR			= NULL;
	m_pDM_SUCCESS		= NULL;

	m_pGrid_OK			= NULL;
	m_pGrid_ERR			= NULL;
	m_pGrid_SUCCESS		= NULL;

	m_lpszSMAlias		= _T("SM_����_���Ÿ�ũ����");
	m_lpszCMAlias		= _T("CM_����_���Ÿ�ũ����");
	
	m_hThread= ::CreateEvent(NULL, FALSE, FALSE, _T("ThreadEvent"));
}

CMarcImportDlg::~CMarcImportDlg()
{
}

BOOL CMarcImportDlg::Create(CWnd* pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;
}

VOID CMarcImportDlg::CopyToMain()
{
	INT ids = m_pParent->SendMessage(IDS_MAKEDM,0,0);
	if(ids < 0)
		return;
}
INT CMarcImportDlg::CopyDM(CESL_DataMgr *pDM_From, INT nFromIdx, CESL_DataMgr *pDM_To, INT nToIdx, CString strToAliasHeader /*= _T("")*/, CStringArray *pArrIgnoreAlias /*= NULL*/)
{
	EFS_BEGIN

	if (pDM_From == NULL || pDM_To == NULL) return -1;

	INT ids = -1;
	INT nFind = -1;

	INT nRowCount = pDM_From->GetRowCount();
	if (nRowCount - 1 < nFromIdx || nFromIdx < 0) return -1;

	nRowCount = pDM_To->GetRowCount();
	if (nRowCount - 1 < nToIdx ) return -1;

	if (nToIdx < 0)
	{
		ids = pDM_To->InsertRow(-1);
		if (ids < 0) return -1;
		nToIdx = pDM_To->GetRowCount() - 1;
	}	

	BOOL bSwitchHeader = FALSE;
	
	if (!strToAliasHeader.IsEmpty())
		bSwitchHeader = TRUE;
	
	CString strAlias;
	CString strItemData;

	CESL_DataMgr::reference *pRef = NULL;
	
	POSITION pos = pDM_From->RefList.GetHeadPosition();
	while (pos)
	{
		pRef = (CESL_DataMgr::reference*)pDM_From->RefList.GetNext(pos);
		if (pRef == NULL) continue;

		strAlias = pRef->FIELD_ALIAS;

		// �ź� ����Ʈ�� �ִ��� Ȯ��
		// Default Ignore List
//		if (IsStringInArray(pArrIgnoreAlias, strAlias)) continue;
//		if ( strAlias == _T("IBS_�谡��ġ��ȣ") ) continue;
		// User Ignore List
//		if (pArrIgnoreAlias != NULL)
//			if (IsStringInArray(pArrIgnoreAlias, strAlias)) continue;
		
		ids = pDM_From->GetCellData(strAlias, nFromIdx, strItemData);
		if (ids < 0) continue;

		// ��� ����
		if (bSwitchHeader)
		{
			nFind = strAlias.Find(_T("_"));
			if (nFind >= 0)
				strAlias = strAlias.Mid(nFind + 1);
			strAlias = strToAliasHeader + _T("_") + strAlias;
		}
		/*
		if ( strAlias == _T("BB_��å��ȣ") || strAlias == _T("BB_�����") ) continue;
		*/
		if ( strAlias == _T("BB_��ϱ���_�ڵ�") ) continue;
		ids = pDM_To->SetCellData(strAlias, strItemData, nToIdx);
		if (ids < 0) continue;
	}

	return nToIdx;

	EFS_END
	return -1;
}

VOID CMarcImportDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcImportDlg)
	DDX_Text(pDX, IDC_eFILE_PATH, m_strFilePath);
	//}}AFX_DATA_MAP


	EFS_END
}


BEGIN_MESSAGE_MAP(CMarcImportDlg, CDialog)
	//{{AFX_MSG_MAP(CMarcImportDlg)
	ON_BN_CLICKED(IDC_bFILE_OPEN, OnbFILEOPEN)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_bIMPORT, OnbIMPORT)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_IMPORT, OnSelchangeTabImport)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_BN_CLICKED(IDC_bEXPORT_EDITMARC, OnbEXPORTEDITMARC)
	ON_BN_CLICKED(IDC_bPrintRevision, OnbPrintRevision)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	//ON_MESSAGE(WM_END_MARC_ADJUST, OnEndMarcAdjust)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////
// THREAD
// ȫ��
UINT ThreadFunc_Import(LPVOID pParam)
{
	CMarcImportDlg *pDlg = (CMarcImportDlg*)pParam;
	if (pDlg == NULL)
	{
		pDlg->m_bIsThreadRunning = FALSE;
		return -1;
	}

	pDlg->m_bIsThreadRunning = TRUE;
	pDlg->ButtonStatus(0);

	// Initialization Variable
	CESL_ControlMgr *pCM 	 = NULL;
	CESL_Grid *pGrid_OK		 = NULL;
	CESL_Grid *pGrid_ERR	 = NULL;
	CESL_Grid *pGrid_SUCCESS = NULL;

	// �ű� �׸���
	CESL_Grid *pGrid_TO = NULL;
	// ����������ų ID
	UINT nID_TO = -1;
	// ����������ų String
	CString strTO;


	INT ids = -1;
	//INT idxDM = -1;
	INT nCount = -1;
	INT nColCount = -1;

	INT nRowTo = 0;
	INT nRowsTo = 0;

	CString strData;
	CString strErrorMsg;

	//2003.10.20 ������ ���� +++++++++++++++++++++++++++++++++
    CString nTotalCnt ;
    CString strTotalMsg ;
    CString nFalseCnt ;
	CString nTrueCnt  ;
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	// �׸��� ����
	pGrid_OK = pDlg->m_pGrid_OK;
	pGrid_ERR = pDlg->m_pGrid_ERR;	
	pGrid_SUCCESS = pDlg->m_pGrid_SUCCESS;
	
	if (pGrid_OK == NULL || pGrid_ERR == NULL || pGrid_SUCCESS == NULL)
		goto ERR;

	// ������ �ڷḸ �����Ѵ�.
	
	// GRID OK Count
	nCount = pGrid_OK->SelectGetCount();
	if (nCount <= 0)
	{
		goto ERR;
	}

	pDlg->m_wndProgressBar.SetRange(0, nCount);
	pDlg->m_wndProgressBar.SetPos(0);
	pDlg->m_wndProgressBar.SetWindowText(_T(""));

	nColCount = pGrid_OK->GetCols(0);
	
	while(TRUE)
	{
		if (!pDlg->m_bIsThreadRunning) break;
		strErrorMsg.Empty();
		pGrid_OK->SelectMakeList();
		INT idx = pGrid_OK->SelectGetHeadPosition() + 1;
		if (idx <= 0) break;

		INT iIdxDM = idx -1;
		CString sStreamMarc;
		pDlg->m_pDM_OK->GetCellData(_T("MARC"), iIdxDM, sStreamMarc);

	
		//���Ե� MARC�� ����
		ids = pDlg->InsertToDB(sStreamMarc);

		if (ids < 0)
		{
			pGrid_TO = pGrid_ERR;
			nID_TO = 2;
			strTO = _T("���� : ");
		}
		else
		{
			pGrid_TO = pGrid_SUCCESS;
			nID_TO = 3;
			strTO = _T("���� : ");

			pDlg->m_pDM_SUCCESS->InsertRow(-1);
			INT nRowIdx = pDlg->m_pDM_SUCCESS->GetRowCount() - 1;			
			pDlg->CopyDM(pDlg->m_pDM_OK, iIdxDM, pDlg->m_pDM_SUCCESS, nRowIdx);
			pDlg->m_pDM_OK->DeleteRow(iIdxDM);
			//���α׸��忡 �ѷ��ش�.
			pDlg->CopyToMain();
		}
	
		// ������ �����Ѵ�.
		strData = pDlg->m_wndStatusBarCtrl.GetText(1);
		strData = strData.Mid(strData.ReverseFind(':') + 1);
		strData.Format(_T("%d"), _ttoi(strData) - 1);
		pDlg->m_wndStatusBarCtrl.SetText(_T("���� : ") + strData, 1, 0);
		
	

		strData = pDlg->m_wndStatusBarCtrl.GetText(nID_TO);
		strData = strData.Mid(strData.ReverseFind(':') + 1);
		strData.Format(_T("%d"), _ttoi(strData) + 1);
		pDlg->m_wndStatusBarCtrl.SetText(strTO + strData, nID_TO, 0);


		//2003.10.15 ������ ����
        nTotalCnt =  strTO+strData ; 
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++

		// �׸����� �����͸� �ű��.
		nRowsTo = pGrid_TO->GetRows();
		if (pGrid_TO->GetTextMatrix(nRowsTo - 1, 0).IsEmpty())
		{
			nRowTo = nRowsTo - 1;
		}
		else
		{
			pGrid_TO->SetRows(nRowsTo + 1);
			nRowTo = nRowsTo;
		}

		for (INT col = 0; col < nColCount; col++)
		{
			pGrid_TO->SetTextMatrix(nRowTo, col, pGrid_OK->GetTextMatrix(idx, col));
		}

		// ���� �� ������ ���� ����
		if (!strErrorMsg.IsEmpty())
		{
			pDlg->m_mapGridAlias.Lookup(_T("BS_���Ϲ��Կ���"), col);
			pGrid_TO->SetTextMatrix(nRowTo, col, strErrorMsg);
		}

		// ���α׷����� �÷��ֱ�...
		pDlg->m_wndProgressBar.StepIt();

		// Remove Row
		if (pGrid_OK->GetRows() == 2)
		{
			pGrid_OK->Clear();
			pGrid_OK->SetColumnInfo();
			break;
		}
		else
		{
			pGrid_OK->RemoveItem(idx);
		}
	}
	
	pDlg->m_wndProgressBar.SetWindowText(_T("�����Ͽ����ϴ�."));
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//2003.10.20 ������ ���� 

    nTotalCnt = pDlg->m_wndStatusBarCtrl.GetText(0) ;
    nFalseCnt = pDlg->m_wndStatusBarCtrl.GetText(2) ;
	nTrueCnt  =  pDlg->m_wndStatusBarCtrl.GetText(3) ;

	strTotalMsg.Format( _T("�����Ͽ����ϴ�. \r\n\r\n %s�� \r\n %s�� \r\n %s��"),nTotalCnt,nFalseCnt,nTrueCnt )  ; 	

	pGrid_OK->DisplayNumber();
	pGrid_ERR->DisplayNumber();
	pGrid_SUCCESS->DisplayNumber();

	//++++++++++++++++++++++++++++++++++++++

	pDlg->m_bIsThreadRunning = FALSE;
	pDlg->ButtonStatus(1);
	SetEvent(pDlg->m_hThread);

	return 0;

ERR:
	pDlg->m_wndProgressBar.SetWindowText(_T("���� ����"));
	AfxMessageBox(_T("���� ����"));
	pDlg->m_bIsThreadRunning = FALSE;
	pDlg->ButtonStatus(1);
	return -1;

return -1;
}


/// ��ũ�� �о �׸��忡 �ѷ��ִ� ������
UINT ThreadFunc_Display(LPVOID pParam)
{
	EFS_BEGIN

	//���� ���̾�α� �����͸� �޴´�. 
	CMarcImportDlg *pDlg = (CMarcImportDlg*)pParam;
	if (pDlg == NULL)
	{
		pDlg->m_bIsThreadRunning = FALSE;
		return -1;
	}

	//������ ���������� ����
	pDlg->m_bIsThreadRunning = TRUE;
	//������ �������� ��ư���·� ����
	pDlg->ButtonStatus(0);

	// Initialization Variable
	//CESL_DataMgr *pDM_ACMarc= NULL;
	
	CESL_ControlMgr *pCM = NULL;

	CESL_Grid *pGrid_OK			= NULL;
	CESL_Grid *pGrid_ERR		= NULL;
	CESL_Grid *pGrid_SUCCESS	= NULL;

	// For Count
	CString strCount;
	INT i = 0;
	INT nTotalCount = -1;
	INT nErrCount = 0;
	INT nOKCount = 0;

	// For Fetch
	INT row = 0;
	INT nGridRow = 0;
	INT nErrGridRow = 0;
	INT nOKGridRow = 0;
	CESL_Grid *pGrid = NULL;

	POSITION pos = NULL;
	CString strGridAlias;
	INT nGridCol = -1;

	// Start...

	//���Ϸκ��� �о� ����� DM�� �ο찳��, ��ü��
	nTotalCount = pDlg->m_pDM_TOTAL->GetRowCount();
	nOKCount = pDlg->m_pDM_OK->GetRowCount();
	nErrCount = pDlg->m_pDM_ERR->GetRowCount();
	
	pCM = pDlg->FindCM(pDlg->m_lpszCMAlias);
	if (pCM == NULL)
	{
		goto ERR;
	}

	// ��ü ���Ե� ����
	strCount.Format(_T("��ü : %d"), nTotalCount);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 0, 0);

	pDlg->m_wndProgressBar.SetRange(0, nTotalCount);
	pDlg->m_wndProgressBar.SetPos(0);
	pDlg->m_wndProgressBar.SetWindowText(_T(""));
		
/*	
	for (i = 0; i < nTotalCount; i++)
	{
		// ERR_COUNT
		INT iCheckSyntax = -1;
		CString sTmpStreamMarc;
		CMarc *pMarc = NULL ;
		pMarc = new CMarc;
		
		pDlg->m_pDM_TOTAL->GetCellData(_T("MARC"), i , sTmpStreamMarc);	

		pDlg->m_pInfo->pMarcMgr->Decoding(sTmpStreamMarc, pMarc);

		//��ũ �����˻�
		iCheckSyntax = pDlg->m_pInfo->pMarcMgr->CheckSimpleSyntax(pMarc);
		if ( iCheckSyntax < 0 ) nErrCount++;

		delete pMarc;		
	}
*/
	strCount.Format(_T("���� : %d"), nErrCount);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 2, 0);
		
	// OK_COUNT
	nOKCount = nTotalCount - nErrCount;
	strCount.Format(_T("���� : %d"), nOKCount);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 1, 0);
	
	// SUCCESS_COUNT
	pDlg->m_wndStatusBarCtrl.SetText(_T("���� : 0"), 3, 0);


	pGrid_OK		= pDlg->m_pGrid_OK;
	pGrid_ERR		= pDlg->m_pGrid_ERR;
	pGrid_SUCCESS	= pDlg->m_pGrid_SUCCESS;
	if (pGrid_OK == NULL || pGrid_ERR == NULL || pGrid_SUCCESS == NULL)
	{
		goto ERR;
	}

	// �׸��带 �⺻������ �����Ѵ�. 
	pGrid_OK->Clear();
	pGrid_OK->SetRows(2);
	pGrid_OK->SetColumnInfo();

	pGrid_ERR->Clear();
	pGrid_ERR->SetRows(2);
	pGrid_ERR->SetColumnInfo();

	pGrid_SUCCESS->Clear();
	pGrid_SUCCESS->SetRows(2);
	pGrid_SUCCESS->SetColumnInfo();


	// ����/�����׸����� �ο���� �����Ѵ�. 
	if (nErrCount > 0)
		pGrid_ERR->SetRows(nErrCount + 1);
	if (nOKCount > 0)
		pGrid_OK->SetRows(nOKCount + 1);

	// OK �׸����� ���� �����Ѵ�. 
	for (row = 0; row < nOKCount; row++)
	{
		pDlg->m_wndProgressBar.StepIt();

		pGrid = pGrid_OK;
		
		strCount.Format(_T("%d"), row);
	
		CString sTmpData = _T("") ;
		pDlg->m_pDM_OK->GetCellData(_T("����ǥ��"), nGridRow, sTmpData );
		pGrid->SetAt(nGridRow, 1, sTmpData );

		sTmpData = _T("") ;
		pDlg->m_pDM_OK->GetCellData(_T("������"), nGridRow, sTmpData );
		pGrid->SetAt(nGridRow, 2, sTmpData );

		sTmpData = _T("") ;
		pDlg->m_pDM_OK->GetCellData(_T("�ֱ�"), nGridRow, sTmpData );
		pGrid->SetAt(nGridRow, 3, sTmpData );	

		sTmpData = _T("") ;
		pDlg->m_pDM_OK->GetCellData(_T("��������"), nGridRow, sTmpData );
		pGrid->SetAt(nGridRow, 4, sTmpData );	

		nGridRow = ++nOKGridRow;
		
		if (!pDlg->m_bIsThreadRunning) break;
	}

	// ERROR �׸����� ���� �����Ѵ�. 
	for (row = 0; row < nErrCount; row++)
	{
		pDlg->m_wndProgressBar.StepIt();

		pGrid = pGrid_ERR;
				
		CString sTmpData = _T("") ;
		pDlg->m_pDM_ERR->GetCellData(_T("����ǥ��"), nGridRow, sTmpData );
		pGrid->SetAt(nGridRow, 1, sTmpData );

		sTmpData = _T("") ;
		pDlg->m_pDM_ERR->GetCellData(_T("������"), nGridRow, sTmpData );
		pGrid->SetAt(nGridRow, 2, sTmpData );

		sTmpData = _T("") ;
		pDlg->m_pDM_ERR->GetCellData(_T("�ֱ�"), nGridRow, sTmpData );
		pGrid->SetAt(nGridRow, 3, sTmpData );	

		sTmpData = _T("") ;
		pDlg->m_pDM_ERR->GetCellData(_T("��������"), nGridRow, sTmpData );
		pGrid->SetAt(nGridRow, 4, sTmpData );	

		nGridRow = ++nErrGridRow;
		
		if (!pDlg->m_bIsThreadRunning) break;
	}

	pDlg->m_wndProgressBar.SetWindowText(_T("������ġ �Ϸ�"));

	pDlg->m_bIsThreadRunning = FALSE;
	pDlg->ButtonStatus(1);

	SetEvent(pDlg->m_hThread);
	
	return 0;

ERR:
	pDlg->m_bIsThreadRunning = FALSE;
	pDlg->ButtonStatus(1);

	return -1;
	
	EFS_END
	return -1;
}


INT CMarcImportDlg::DataDisplay()
{
	EFS_BEGIN

	ButtonStatus(0);

	// For Count
	CString strCount;
	INT i = 0;
	INT nTotalCount = -1;
	INT nErrCount = 0;
	INT nOKCount = 0;

	// For Fetch
	INT row = 0;
	INT nGridRow = 0;
	INT nErrGridRow = 0;
	INT nOKGridRow = 0;
	CESL_Grid *pGrid = NULL;

	POSITION pos = NULL;
	CString strGridAlias;
	INT nGridCol = -1;

	// Start...

	//���Ϸκ��� �о� ����� DM�� �ο찳��, ��ü��
	nTotalCount = m_pDM_TOTAL->GetRowCount();
	nOKCount = m_pDM_OK->GetRowCount();
	nErrCount = m_pDM_ERR->GetRowCount();
	
	// ��ü ���Ե� ����
	strCount.Format(_T("��ü : %d"), nTotalCount);
	m_wndStatusBarCtrl.SetText(strCount, 0, 0);

	m_wndProgressBar.SetRange(0, nTotalCount);
	m_wndProgressBar.SetPos(0);
	m_wndProgressBar.SetWindowText(_T(""));
		
	strCount.Format(_T("���� : %d"), nErrCount);
	m_wndStatusBarCtrl.SetText(strCount, 2, 0);
		
	// OK_COUNT
	nOKCount = nTotalCount - nErrCount;
	strCount.Format(_T("���� : %d"), nOKCount);
	m_wndStatusBarCtrl.SetText(strCount, 1, 0);
	
	// SUCCESS_COUNT
	m_wndStatusBarCtrl.SetText(_T("���� : 0"), 3, 0);


	// �׸��带 �⺻������ �����Ѵ�. 
	m_pGrid_OK->Clear();
	m_pGrid_OK->SetRows(2);
	m_pGrid_OK->SetColumnInfo();

	m_pGrid_ERR->Clear();
	m_pGrid_ERR->SetRows(2);
	m_pGrid_ERR->SetColumnInfo();

	m_pGrid_SUCCESS->Clear();
	m_pGrid_SUCCESS->SetRows(2);
	m_pGrid_SUCCESS->SetColumnInfo();


	// ����/�����׸����� �ο���� �����Ѵ�. 
	if (nErrCount > 0)
		m_pGrid_ERR->SetRows(nErrCount + 1);
	if (nOKCount > 0)
		m_pGrid_OK->SetRows(nOKCount + 1);

	// OK �׸����� ���� �����Ѵ�. 
	for (row = 0; row < nOKCount; row++)
	{
		m_wndProgressBar.StepIt();
		
		strCount.Format(_T("%d"), row);
		
		CString sTmpData = _T("") ;
		m_pDM_OK->GetCellData(_T("����ǥ��"), nGridRow, sTmpData );
		m_pGrid_OK->SetAt(nGridRow, 1, sTmpData );

		sTmpData = _T("") ;
		m_pDM_OK->GetCellData(_T("������"), nGridRow, sTmpData );
		m_pGrid_OK->SetAt(nGridRow, 2, sTmpData );

		sTmpData = _T("") ;
		m_pDM_OK->GetCellData(_T("�ֱ�"), nGridRow, sTmpData );
		m_pGrid_OK->SetAt(nGridRow, 3, sTmpData );	

		sTmpData = _T("") ;
		m_pDM_OK->GetCellData(_T("�ʼ��±�"), nGridRow, sTmpData );
		m_pGrid_OK->SetAt(nGridRow, 4, sTmpData );

		sTmpData = _T("") ;
		m_pDM_OK->GetCellData(_T("��������"), nGridRow, sTmpData );
		m_pGrid_OK->SetAt(nGridRow, 5, sTmpData );	

		nGridRow = ++nOKGridRow;
	}

	// ERROR �׸����� ���� �����Ѵ�. 
	for (row = 0; row < nErrCount; row++)
	{
		m_wndProgressBar.StepIt();
		
		CString sTmpData = _T("") ;
		m_pDM_ERR->GetCellData(_T("����ǥ��"), nGridRow, sTmpData );
		m_pGrid_ERR->SetAt(nGridRow, 1, sTmpData );

		sTmpData = _T("") ;
		m_pDM_ERR->GetCellData(_T("������"), nGridRow, sTmpData );
		m_pGrid_ERR->SetAt(nGridRow, 2, sTmpData );

		sTmpData = _T("") ;
		m_pDM_ERR->GetCellData(_T("�ֱ�"), nGridRow, sTmpData );
		m_pGrid_ERR->SetAt(nGridRow, 3, sTmpData );	

		sTmpData = _T("") ;
		m_pDM_ERR->GetCellData(_T("�ʼ��±�"), nGridRow, sTmpData );
		m_pGrid_ERR->SetAt(nGridRow, 4, sTmpData );

		sTmpData = _T("") ;
		m_pDM_ERR->GetCellData(_T("��������"), nGridRow, sTmpData );
		m_pGrid_ERR->SetAt(nGridRow, 5, sTmpData );	

		nGridRow = ++nErrGridRow;
	}

	m_wndProgressBar.SetWindowText(_T("������ġ �Ϸ�"));

	ButtonStatus(1);

	m_pCM->AllControlDisplay();

	return 0;
	EFS_END
return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CMarcImportDlg Operations

INT CMarcImportDlg::Init()
{
	EFS_BEGIN

	// ���� MASTER DM Alias
	m_lpszACDMAlias = _T("DM_����_���Ÿ�ũ����");
	m_pInfo->pMarcMgr->SetMarcGroup(_T("����"));
	SetWindowText(_T("���� ��ũ ����"));

/*	m_pDM_TOTAL = new CESL_DataMgr;
	if (m_pDM_TOTAL == NULL) return -1;

	m_pDM_TOTAL->SetCONNECTION_INFO(CONNECTION_INFO);
	m_pDM_TOTAL->SetMarcMgr(m_pInfo->pMarcMgr);

	m_pDM_TOTAL->InitDMPKFromFile(m_lpszACDMAlias);
*/

	m_pDM_TOTAL = FindDM(_T("DM_����_���Ÿ�ũ����") );
	if( m_pDM_TOTAL == NULL )
	{
		AfxMessageBox(_T("DM_����_���Ÿ�ũ���� not found!!") );
		return -1;
	}

	m_pDM_OK = new CESL_DataMgr;
	if (m_pDM_OK == NULL) return -1;

	m_pDM_OK->SetCONNECTION_INFO(CONNECTION_INFO);
	m_pDM_OK->SetMarcMgr(m_pInfo->pMarcMgr);

	m_pDM_ERR = new CESL_DataMgr;
	if (m_pDM_ERR == NULL) return -1;

	m_pDM_ERR->SetCONNECTION_INFO(CONNECTION_INFO);
	m_pDM_ERR->SetMarcMgr(m_pInfo->pMarcMgr);

	m_pDM_SUCCESS = new CESL_DataMgr;
	if (m_pDM_SUCCESS == NULL) return -1;

	m_pDM_SUCCESS->SetCONNECTION_INFO(CONNECTION_INFO);
	m_pDM_SUCCESS->SetMarcMgr(m_pInfo->pMarcMgr);

	m_pDM_OK->CopyDM(m_pDM_TOTAL);
	m_pDM_ERR->CopyDM(m_pDM_TOTAL);
	m_pDM_SUCCESS->CopyDM(m_pDM_TOTAL);

	InitDMAlias(m_pDM_OK);
	InitDMAlias(m_pDM_ERR);

	// CM
	m_pCM = FindCM(m_lpszCMAlias);
	if (m_pCM == NULL)
	{
		AfxMessageBox(_T("CM_����_���Ÿ�ũ���� not found!!") );
		return -1;
	}

	// Grid
	m_pGrid_OK = (CESL_Grid*)m_pCM->FindControl(_T("Grid_OK"));
	// ERROR GRID
	m_pGrid_ERR = (CESL_Grid*)m_pCM->FindControl(_T("Grid_ERR"));
	// SUCCESS GRID
	m_pGrid_SUCCESS = (CESL_Grid*)m_pCM->FindControl(_T("Grid_SUCCESS"));

	if (m_pGrid_OK == NULL || m_pGrid_ERR == NULL || m_pGrid_SUCCESS == NULL) return -1;


	// Move Windows
	CRect rcGrid;
	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);
	
	m_pGrid_OK->MoveWindow(rcGrid);
	m_pGrid_ERR->MoveWindow(rcGrid);
	m_pGrid_SUCCESS->MoveWindow(rcGrid);

	return 0;


EFS_END
return -1;
}

INT CMarcImportDlg::InitDMAlias(CESL_DataMgr * pDM)
{
	pDM->InitDataMgrRef(17);
	pDM->SetDataMgrRef(0,	_T("AC_CONTROL_NO"),	_T("AC_CONTROL_NO"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(1,	_T("MARC"),				_T("MARC"),				_T("INT"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(2,	_T("REC_KEY"),	_T("REC_KEY"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(3,	_T("AC_TYPE"),	_T("AC_TYPE"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(4,	_T("AC_CLASS"),	_T("AC_CLASS"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(5,	_T("SEE_ALSO_REFERENCE_YN"),	_T("SEE_ALSO_REFERENCE_YN"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(6,	_T("INPUT_DATE"),	_T("INPUT_DATE"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(7,	_T("UPDATE_DATE"),	_T("UPDATE_DATE"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(8,	_T("FIRST_WORKER"),	_T("FIRST_WORKER"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(9,	_T("LAST_WORKER"),	_T("LAST_WORKER"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(10,	_T("FIRST_WORK"),	_T("FIRST_WORK"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(11,	_T("LAST_WORK"),	_T("LAST_WORK"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(12,	_T("����ǥ��"),		_T("����ǥ��"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(13,	_T("������"),		_T("������"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(14,	_T("�ֱ�"),			_T("�ֱ�"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(15,	_T("�ʼ��±�"),		_T("�ʼ��±�"),	_T("str"),	_T(""),	_T(""));
	pDM->SetDataMgrRef(16,	_T("��������"),		_T("��������"),	_T("str"),	_T(""),	_T(""));

	return 0;
}

// Mode : 0 �۾�������
// Mode : 1 �۾��Ϸ�
VOID CMarcImportDlg::ButtonStatus(INT nMode)
{
	EFS_BEGIN

	const INT nIDCount = 4;
	INT nArrID[nIDCount] = {
		IDC_bFILE_OPEN, IDC_bEXPORT,
		IDC_bIMPORT, IDC_bEXPORT
	};

	for (INT i = 0; i < nIDCount; i++)
	{
		if (nMode == 0)
		{
			GetDlgItem(nArrID[i])->EnableWindow(FALSE);
		}
		else if (nMode == 1)
		{
			GetDlgItem(nArrID[i])->EnableWindow(TRUE);
		}
	}

	if (nMode == 0)
	{
		GetDlgItem(IDC_bCLOSE)->SetWindowText(_T("����"));
	}
	else if (nMode == 1)
	{
		GetDlgItem(IDC_bCLOSE)->SetWindowText(_T("�ݱ�"));
	}


	EFS_END
}

//���Ϸκ��� ��ũ ������ �����Ѵ�. 
//
INT CMarcImportDlg::ImportMarcFromFile()
{
	EFS_BEGIN

	BeginWaitCursor();
	INT ids;
	m_pDM_TOTAL->FreeData();


	if (m_strFilePath.IsEmpty()) return -1;

	ids = ImportFile();

	// Display Thread Start
	if (ids >= 0)		m_pThreadDisplay = AfxBeginThread(ThreadFunc_Display, this);
//	ids = DataDisplay();

	EndWaitCursor();
	return ids;

EFS_END
return -1;
}

INT CMarcImportDlg::ExportToTxtFile(CString strFilePath)
{
	EFS_BEGIN

	BeginWaitCursor();

	// Deterime What Grid is Selected
	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl == NULL) return -1;

	INT nSel = pTabCtrl->GetCurSel();
	CESL_Grid *pGrid = NULL;

	// OK GRID
	if (nSel == 0)
		pGrid = m_pGrid_OK;
	// ERROR GRID
	else if (nSel == 1)
		pGrid = m_pGrid_ERR;
	// SUCCESS GRID
	else if (nSel == 2)
		pGrid = m_pGrid_SUCCESS;
	
	if (pGrid == NULL) return -1;


	if (pGrid->GetRows() == 2)
	{
		if (pGrid->GetTextMatrix(1, 0).IsEmpty())
		{
			AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
			return -1;
		}
	}

	// File
	CStdioFile file;
	if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		return -1;
		
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}	

	CString strErrMarc;
	INT nCount = pGrid->GetCount();
	for (INT i = 0; i < nCount; i++)
	{
		strErrMarc = m_pDM_TOTAL->GetCellData(_T("MARC"), _ttoi(pGrid->GetTextMatrix(i + 1, 0)));
		if (strErrMarc.IsEmpty()) continue;

		strErrMarc += _T("\n");

		file.WriteString(strErrMarc);
	}

	file.Close();

	EndWaitCursor();

	AfxMessageBox(_T("���� �Ϸ�"));

	return 0;

	return -1;

	EFS_END
	return -1;
}


INT CMarcImportDlg::CallBackCheckMarc(CMarc *pMarc, CString &strCheckErrMsg)
{
	EFS_BEGIN

	if (pMarc == NULL) return -1;

	INT ids = -1;
	INT idx = -1;
	INT nCount = -1;
	CString strItem;
	CString strTemp;
	CString strItemData;
	CString strErrMsg;
	CStringArray arrStrErrMsg;
	CArray<CString, CString&> arrItem;

	strCheckErrMsg.Empty();

	CStringArray arrStrEssentialTag;
	
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 008 �Է����ڰ� ������ �־��ش�.
	ids = m_pInfo->pMarcMgr->GetItem(pMarc, _T("008@0-5"), strItemData);
	CIndex::TrimAll(strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		strItemData = CTime::GetCurrentTime().Format(_T("%y%m%d"));
		m_pInfo->pMarcMgr->SetItem(pMarc, _T("008@0-5"), strItemData);
	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//���ſ��� �־�� ��
	// ������������ ���� 005 ������ ����..
	CTime time = CTime::GetCurrentTime();	
	strItem.Format(_T("%04d%02d%02d%02d%02d%02d"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	m_pInfo->pMarcMgr->DeleteField(pMarc, _T("005"));
	m_pInfo->pMarcMgr->InsertField(pMarc, _T("005") + strItem);
	
	nCount = arrStrErrMsg.GetSize();
	for (idx = 0; idx < nCount; idx ++)
	{
		strCheckErrMsg += arrStrErrMsg.GetAt(idx) + _T("\n");
	}

	strCheckErrMsg.TrimLeft();
	strCheckErrMsg.TrimRight();
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	if (nCount > 0)
		return -1;
	else
		return 0;

	EFS_END
	return -1;
}

BOOL CMarcImportDlg::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();


	if (InitESL_Mgr(m_lpszSMAlias) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	if (Init() < 0)
	{
		AfxMessageBox(_T("Initialization is Failed!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	GetWindowRect(m_rcInitial);
	
	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl)
	{
		pTabCtrl->InsertItem(0, _T("����"));
		pTabCtrl->InsertItem(1, _T("����"));
		pTabCtrl->InsertItem(2, _T("����"));

		pTabCtrl->SetCurFocus(0);
	}

	// Status Bar
	m_wndStatusBarCtrl.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_SIZEGRIP , CRect(0, 0, 0, 0), this, WM_USER + 1000);
	m_wndStatusBarCtrl.SetMinHeight(22);
	
	if (!m_wndProgressBar.Create(WS_CHILD | WS_VISIBLE | PBS_SMOOTH, CRect(0,0,100,100), &m_wndStatusBarCtrl, 17298))
		return -1;

	m_wndProgressBar.SetFont(GetFont());

	const INT nParts = 5;
	CRect rect;
	m_wndStatusBarCtrl.GetClientRect(&rect);
	INT widths[nParts] = { 100, 200, 300, 400, -1 };
	m_wndStatusBarCtrl.SetParts(nParts, widths);

	m_pGrid_ERR->ShowWindow(SW_HIDE);
	m_pGrid_SUCCESS->ShowWindow(SW_HIDE);
	m_pGrid_OK->ShowWindow(SW_SHOW);

	PostMessage(WM_SIZE);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;
}

VOID CMarcImportDlg::OnbCLOSE() 
{
	EFS_BEGIN

	CString strText;

	GetDlgItem(IDC_bCLOSE)->GetWindowText(strText);

	if (strText == _T("�ݱ�"))
	{

		// Thread Check
		if (m_bIsThreadRunning)
		{
			m_bIsThreadRunning = FALSE;

			MSG msg;
			while (TRUE)
			{
				if (::PeekMessage(&msg,GetSafeHwnd(), 1, 1, PM_NOREMOVE))
					AfxGetApp()->PumpMessage();

				if (WaitForSingleObject(m_hThread, 0) == WAIT_OBJECT_0)
					break;
			}
		}
		CDialog::OnCancel();
	}
	else if (strText == _T("����"))
	{
		m_bIsThreadRunning = FALSE;
	}

	EFS_END
}



VOID CMarcImportDlg::OnSelchangeTabImport(NMHDR* pNMHDR, LRESULT* pResult) 
{
	EFS_BEGIN

	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl == NULL) return;
	
	INT nCurSel = pTabCtrl->GetCurSel();

	CMSHFlexGrid *pGridNormal = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID_OK);
	CMSHFlexGrid *pGridError  = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID_ERR);
	CMSHFlexGrid *pGridSuccess  = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID_SUCCESS);

	if (pGridNormal == NULL || pGridError == NULL || pGridSuccess == NULL) return;
	
	if (nCurSel == 0)
	{
		pGridNormal->ShowWindow(SW_SHOW);
		pGridError->ShowWindow(SW_HIDE);
		pGridSuccess->ShowWindow(SW_HIDE);
	}
	else if (nCurSel == 1)
	{
		pGridError->ShowWindow(SW_SHOW);
		pGridNormal->ShowWindow(SW_HIDE);
		pGridSuccess->ShowWindow(SW_HIDE);
	}
	else if (nCurSel == 2)
	{
		pGridError->ShowWindow(SW_HIDE);
		pGridNormal->ShowWindow(SW_HIDE);
		pGridSuccess->ShowWindow(SW_SHOW);
	}
		
	*pResult = 0;

	EFS_END
}

VOID CMarcImportDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);

	if (GetSafeHwnd() == NULL) return;

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

	INT i = 0;
	CRect rcClient;
	CWnd *pWnd = NULL;

	CRect rcDlg;
	GetClientRect(rcDlg);

	// Tab Ctrl
	pWnd = GetDlgItem(IDC_TAB_IMPORT);
	if (pWnd)
	{
		pWnd->GetWindowRect(rcClient);
		ScreenToClient(rcClient);

		rcClient.right = rcDlg.right - 10;
		rcClient.bottom = rcDlg.bottom - 24;

		pWnd->MoveWindow(rcClient);
	}

	// Picture Ctrl
	pWnd = GetDlgItem(IDC_STATIC_GRID);
	if (pWnd)
	{
		rcClient.DeflateRect(5, 5);
		rcClient.top += 20;
		pWnd->MoveWindow(rcClient);
	}

		
	// Grids
	const INT nResizeControlCnt = 3;
	INT nArrayResizeControl[nResizeControlCnt] = {
		IDC_MSHFLEXGRID_OK, IDC_MSHFLEXGRID_ERR, 
		IDC_MSHFLEXGRID_SUCCESS, 
	};	
	
	for (i = 0; i < nResizeControlCnt; i++)
	{
		pWnd = GetDlgItem(nArrayResizeControl[i]);
		if (pWnd != NULL)
		{
			pWnd->MoveWindow(rcClient);
		}
	}

	// StatusBar Ctrl
	if (m_wndStatusBarCtrl.GetSafeHwnd() != NULL)
		m_wndStatusBarCtrl.SendMessage(WM_SIZE, 0, 0);

	// ProgressBar
	if(!::IsWindow(m_wndProgressBar.m_hWnd))
        return;

	CRect   rc;
    m_wndStatusBarCtrl.GetRect(4,rc);
	rc.top -=1;
	rc.bottom +=5;
    
    m_wndProgressBar.SetWindowPos(NULL,rc.left,
                                 rc.top-1,
                                 rc.Width(),
                                 rc.Height()+2,SWP_NOZORDER | SWP_NOACTIVATE);

	m_wndProgressBar.GetClientRect(rc);
	HRGN hrgn = CreateRectRgn(rc.left+2, rc.top+4, rc.right-2, rc.bottom-6);
	m_wndProgressBar.SetWindowRgn(hrgn, TRUE);
	DeleteObject(hrgn);
	
	EFS_END
}

BOOL CMarcImportDlg::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);

	return FALSE;

	EFS_END
	return FALSE;
}

VOID CMarcImportDlg::OnDestroy() 
{
	EFS_BEGIN
	
	CDialog::OnDestroy();

	EFS_END
}

VOID CMarcImportDlg::OnbFILEOPEN() 
{
	EFS_BEGIN

	//UpdateData();

	TCHAR szBuffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szBuffer);
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFile = lpstrFile;
	OFN.hwndOwner = m_hWnd;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_OVERWRITEPROMPT;

	OFN.lpstrFilter = _T("TXT����(*.txt)\0*.txt\0�������(*.*)\0*.*\0");
	OFN.lpstrTitle = _T("TEXT ���� ����");

	BOOL bSuccess = GetOpenFileName(&OFN);
	SetCurrentDirectory(szBuffer);
	if (bSuccess)
	{
		m_strFilePath = OFN.lpstrFile;
		UpdateData(FALSE);

		m_wndProgressBar.SetWindowText(_T("���� ��ġ��..."));
		m_wndProgressBar.UpdateWindow();

		// ���� ���� ����
		ImportMarcFromFile();		
	}


EFS_END
}


VOID CMarcImportDlg::OnbIMPORT() 
{
	EFS_BEGIN

	m_pGrid_OK->SelectMakeList();

	INT nCount = m_pGrid_OK->SelectGetCount();
	if (nCount < 1) 
	{
		AfxMessageBox(_T("������ �ڷḦ ������ �ּ���"));
		return;
	}

	UpdateData();

	m_pThreadImport = AfxBeginThread(ThreadFunc_Import, this);

	EFS_END
}

VOID CMarcImportDlg::OnbEXPORTEDITMARC() 
{
	EFS_BEGIN

	// Marc Editor�� �־��ٰ� ����.
	CMarcEditCtrl MarcEditCtrl;
	MarcEditCtrl.Create(_T("RichEdit20W"), NULL , WS_CHILD | ES_MULTILINE, CRect(0,0,0,0), this, 10202);

	// File Select Dialog
	CString strFilePath;
	
	TCHAR szBuffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szBuffer);
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFile = lpstrFile;
	OFN.hwndOwner = m_hWnd;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_OVERWRITEPROMPT;
	OFN.lpstrDefExt = _T("TXT");

	OFN.lpstrFilter = _T("TXT����(*.txt)\0*.txt\0�������(*.*)\0*.*\0");
	OFN.lpstrTitle = _T("TEXT ���� ����");

	INT ids = GetSaveFileName(&OFN);
	SetCurrentDirectory(szBuffer);

	if (ids == 0) return;

	strFilePath= OFN.lpstrFile;

	BeginWaitCursor();
	// File
	CStdioFile file;
	if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		return;
		
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}	
		
	CMarc marc;
	CString strStreamMarc;
	CString strEditMarc;
	INT nCount = m_pDM_TOTAL->GetRowCount();
	for (INT i = 0; i < nCount; i++)
	{
		strStreamMarc = m_pDM_TOTAL->GetCellData(_T("BS_MARC"), i);
		if (strStreamMarc.IsEmpty()) continue;

		ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
		if (ids < 0) continue;

		// MARC�� �����ͼ� Edit Ctrl�� �������� Text�� �����´�.
		MarcEditCtrl.Init(m_pInfo->pMarcMgr, &marc);
		MarcEditCtrl.Display();

		MarcEditCtrl.GetWindowText(strEditMarc);

		strEditMarc += _T("\r\n\r\n+++++++++++++++++++++++++++++++++++++++++++++++\r\n\r\n");

		file.WriteString(strEditMarc);
	}

	file.Close();

	EndWaitCursor();

	AfxMessageBox(_T("���� �Ϸ�"));

	EFS_END
}

UINT CMarcImportDlg::OnEndMarcAdjust(WPARAM wParam, LPARAM lParam)
{
	EFS_BEGIN

	// ��ũ ������ �Ϸ�� �ڷ�� �����ڷ�� ����.
	
	// Deterime What Grid is Selected
	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl == NULL) return -1;
	
	CESL_Grid *pGrid = NULL;

	INT nID_FROM = -1;
	CString strFROM;

	INT nSel = pTabCtrl->GetCurSel();
	// OK GRID
	if (nSel == 0)
	{
		pGrid = m_pGrid_OK;
		nID_FROM = 1;
		strFROM = _T("���� : ");
	}
	// ERROR GRID
	else if (nSel == 1)
	{
		pGrid = m_pGrid_ERR;
		nID_FROM = 2;
		strFROM = _T("���� : ");
	}
	else
		return -1;
	
	INT nRowsCUR = -1;
	INT nRowsOK = -1;

	INT nRowOK = -1;
	CString strData;

	INT nRowCount = pGrid->GetCount();
	INT nColCount = pGrid->GetCols(0);

	for (INT idx = nRowCount - 1; idx >= 0; idx--)
	{
		if (m_pDM_TOTAL->GetCellData(_T("BS_���Ϲ��Լ���"), _ttoi(pGrid->GetTextMatrix(idx + 1, 0))) == _T("Y"))
		{
			// ���� Grid�� �ű��.
			nRowsOK = m_pGrid_SUCCESS->GetRows();
			strData = m_pGrid_SUCCESS->GetTextMatrix(nRowsOK - 1, 0);
			if (strData.IsEmpty())
			{
				nRowOK = nRowsOK - 1;
			}
			else
			{
				m_pGrid_SUCCESS->SetRows(nRowsOK + 1);
				nRowOK = nRowsOK;
			}

			for (INT col = 0; col < nColCount; col++)
			{
				m_pGrid_SUCCESS->SetTextMatrix(nRowOK, col, pGrid->GetTextMatrix(idx + 1, col));
			}

			// RemoveItem
			INT nRowsCUR = pGrid->GetRows();
			if (nRowsCUR != 2)
			{
				pGrid->RemoveItem(idx + 1);
			}
			else if (nRowsCUR == 2)
			{
				pGrid->Clear();
				pGrid->SetColumnInfo();
			}

			// ������ �����Ѵ�.
			strData = m_wndStatusBarCtrl.GetText(nID_FROM);
			strData = strData.Mid(strData.ReverseFind(':') + 1);
			strData.Format(_T("%d"), _ttoi(strData) - 1);
			m_wndStatusBarCtrl.SetText(strFROM + strData, nID_FROM, 0);		

			strData = m_wndStatusBarCtrl.GetText(3);
			strData = strData.Mid(strData.ReverseFind(':') + 1);
			strData.Format(_T("%d"), _ttoi(strData) + 1);
			m_wndStatusBarCtrl.SetText(_T("���� : ") + strData, 3, 0);
		}
	}

	return 0;

	EFS_END
	return -1;
}

BEGIN_EVENTSINK_MAP(CMarcImportDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMarcImportDlg)
//	ON_EVENT(CMarcImportDlg, IDC_MSHFLEXGRID_ERR, -605 /* MouseDown */, OnMouseDownMshflexgridErr, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
//	ON_EVENT(CMarcImportDlg, IDC_MSHFLEXGRID_OK, -605 /* MouseDown */, OnMouseDownMshflexgridOk, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
/*
VOID CMarcImportDlg::OnMouseDownMshflexgridOk(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	INT i = m_pGrid_OK->GetMouseRow();
	if (i == 0) return;
	i = m_pGrid_OK->GetFixedRows();
	i = m_pGrid_OK->GetRow();

	EFS_END
}

VOID CMarcImportDlg::OnMouseDownMshflexgridErr(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	//m_pGrid_ERR->SetReverse(m_pGrid_ERR->GetRow() - 1);

	EFS_END
}
*/

VOID CMarcImportDlg::OnbPrintRevision() 
{
EFS_BEGIN

	m_pGrid_OK->SelectMakeList();

	INT nCount = m_pGrid_OK->SelectGetCount();
	if (nCount < 1) 
	{
		AfxMessageBox(_T("������ ����� �ڷḦ ������ �ּ���"));
		return;
	}

	//������ ���
	PrintRevision();

EFS_END	
}

INT CMarcImportDlg::ImportFile()
{
EFS_BEGIN

	CStdioFile file;
	if (!file.Open(m_strFilePath, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox( _T("File Open Failed!!") ) ;
		return -1;
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	//INT ids;
	INT idxDMMarc = 0 ;
	CString strLine;
	CMarc marc;

	file.ReadString(strLine);

	m_pDM_TOTAL->FreeData();
	m_pDM_OK->FreeData();
	m_pDM_ERR->FreeData();

	do 
	{
		marc.Init();
		strLine.TrimLeft();
		strLine.TrimRight();
		if (strLine.IsEmpty()) break;
		
		// ��Ʈ�� ��ũ�� ��ġ �Ѵ�.
		while (TRUE)
		{
			CString strStreamMarc = _T("");
			
			INT nFind = strLine.Find(RECORD_TERMINATOR);
			if (nFind < 0)
				break;
			else
			{
				if (nFind == strLine.GetLength() - 1)
				{
					strStreamMarc = strLine;
					strLine.Empty();
				}
				else
				{
					strStreamMarc = strLine.Mid(0, nFind + 1);
					strLine = strLine.Mid(nFind + 1);
				}
			}
			
			//ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
			//if (ids < 0) continue;
			
			MakeDM(idxDMMarc, strStreamMarc);
			idxDMMarc++;
		}
	} while (file.ReadString(strLine));

	//INT iLen = strLine.GetLength();
	file.Close();

	return 0;

EFS_END
return -1;
}

//��ũ�� ���� DM�� �����.
VOID CMarcImportDlg::MakeDM(INT idxDMMarc, CString strStreamMarc )
{
EFS_BEGIN

	if (strStreamMarc == _T("") ) return;

	//INT ids;
	CString sTmpMarcData = _T("");

	m_pDM_TOTAL->InsertRow(-1);

	//Stream Marc ����
	m_pDM_TOTAL->SetCellData(_T("MARC"), strStreamMarc, idxDMMarc );
	
	//Marc Decoding
	CMarc *pMarc = NULL;
	pMarc = new CMarc;

	m_pInfo->pMarcMgr->Decoding( strStreamMarc, pMarc );
	
	INT ids = m_pInfo->pMarcMgr->CheckSimpleSyntax(pMarc);
	ids = 0;
	CESL_DataMgr * pDM;

	if(ids < 0) // ���������� �Ǵ�
	{
		m_pDM_ERR->InsertRow(-1);
		pDM = (CESL_DataMgr *)m_pDM_ERR;
	}
	else // ����
	{
		m_pDM_OK->InsertRow(-1);
		pDM = (CESL_DataMgr *)m_pDM_OK;
	}


	pDM->SetCellData(_T("MARC"), strStreamMarc, idxDMMarc );

	m_pInfo->pMarcMgr->GetItem(pMarc, _T("100$a"), sTmpMarcData);
	if ( sTmpMarcData != _T(""))
	{
		pDM->SetCellData(_T("����ǥ��"), sTmpMarcData , idxDMMarc );
		pDM->SetCellData(_T("AC_TYPE"), _T("0") , idxDMMarc );

		//������
		m_pInfo->pMarcMgr->GetItem(pMarc, _T("100$d"), sTmpMarcData);
		if ( sTmpMarcData != _T(""))
			pDM->SetCellData(_T("������"), sTmpMarcData , idxDMMarc );
	}

	sTmpMarcData = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("110$a"), sTmpMarcData);
	if ( sTmpMarcData != _T(""))
	{
		pDM->SetCellData(_T("����ǥ��"), sTmpMarcData , idxDMMarc );
		pDM->SetCellData(_T("AC_TYPE"), _T("1") , idxDMMarc );

		//������
		m_pInfo->pMarcMgr->GetItem(pMarc, _T("110$d"), sTmpMarcData);
		if ( sTmpMarcData != _T(""))
			pDM->SetCellData(_T("������"), sTmpMarcData , idxDMMarc );
	}

	sTmpMarcData = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("111$a"), sTmpMarcData);
	if ( sTmpMarcData != _T(""))
	{
		pDM->SetCellData(_T("����ǥ��"), sTmpMarcData , idxDMMarc );
		pDM->SetCellData(_T("AC_TYPE"), _T("2") , idxDMMarc );

		//������
		m_pInfo->pMarcMgr->GetItem(pMarc, _T("111$d"), sTmpMarcData);
		if ( sTmpMarcData != _T(""))
			pDM->SetCellData(_T("������"), sTmpMarcData , idxDMMarc );
	}

	sTmpMarcData = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("130$a"), sTmpMarcData);
	if ( sTmpMarcData != _T(""))
	{
		pDM->SetCellData(_T("����ǥ��"), sTmpMarcData , idxDMMarc );
		pDM->SetCellData(_T("AC_TYPE"), _T("3") , idxDMMarc );
	}

	sTmpMarcData = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("150$a"), sTmpMarcData);
	if ( sTmpMarcData != _T(""))
	{
		pDM->SetCellData(_T("����ǥ��"), sTmpMarcData , idxDMMarc );
		pDM->SetCellData(_T("AC_TYPE"), _T("4") , idxDMMarc );
	}
	
	sTmpMarcData = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("678$a"), sTmpMarcData);
	if ( sTmpMarcData != _T(""))
	{
		pDM->SetCellData(_T("�ֱ�"), sTmpMarcData , idxDMMarc );
	}
	
	INT nIdx = pDM->GetRowCount()-1;

	// ���������� ��´�. 
	CString strType		= _T("");
	CString strRefItem  = _T("");
	CString strCopy		= _T("");
	CString strQuery	= _T("");
	
	pDM->GetCellData(_T("AC_TYPE"),nIdx,strType);
	if(strType.IsEmpty() == FALSE)
	{
		strRefItem = pDM->GetCellData(_T("����ǥ��"),nIdx);
		if(strRefItem.IsEmpty() == FALSE)
		{
			if(strType == _T("0") || strType == _T("1") || strType == _T("2") )
			{
				strQuery.Format(_T("SELECT COUNT(*) FROM AC_INDEX_AUTHOR_TBL WHERE KEYWORD = '%s' "), strRefItem);
				m_pDM_TOTAL->GetOneValueQuery(strQuery, strCopy);
				pDM->SetCellData(_T("��������"),strCopy,nIdx);
			}
			else if(strType == _T("3"))
			{
				strQuery.Format(_T("SELECT COUNT(*) FROM AC_INDEX_TITLE_TBL WHERE KEYWORD = '%s' "), strRefItem);
				m_pDM_TOTAL->GetOneValueQuery(strQuery,strCopy);
				pDM->SetCellData(_T("��������"),strCopy,nIdx);
			}
			else
			{
				strQuery.Format(_T("SELECT COUNT(*) FROM AC_INDEX_SUBJECT_TBL WHERE KEYWORD = '%s' "), strRefItem);
				m_pDM_TOTAL->GetOneValueQuery(strQuery, strCopy);
				pDM->SetCellData(_T("��������"),strCopy,nIdx);
			}
		}
	}

	if ( pMarc != NULL ) delete pMarc;

	return;

EFS_END
}

INT CMarcImportDlg::PrintRevision()
{
EFS_BEGIN

	// Grid ������ �� Key�� �����´�.
	INT nCount = m_pGrid_OK->SelectGetCount();
	if (nCount <= 0)
	{
		AfxMessageBox( _T("������ ����Ʈ�� �����ϴ�.") );
		return -1;
	}

	// Marc Editor�� �־��ٰ� ����.
	CString strEditMarc;
	CMarcEditCtrl MarcEditCtrl;
	MarcEditCtrl.Create(_T("RichEdit20W"), NULL, WS_CHILD | ES_MULTILINE, CRect(0,0,0,0), this, 10202);

	BeginWaitCursor();
	// DM ����
	CESL_DataMgr DM_ACMarc;
	DM_ACMarc.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	
	// Where ���� �����Ѵ�.
	CString strOption;
	CString strQuery;
	CStringArray arrStrSpeciesKey;

	
	CESL_DataMgr *pACPrintDM = NULL;
	pACPrintDM = FindDM(_T("DM_����_���ű�����"));

	pACPrintDM->FreeData();

	INT iCount = 0;
	INT iRec = 0;
	CString sSpeciesKey;
	CString strStreamMarc;

	CMarc *pMarc = NULL ;
	pMarc = new CMarc;

	INT nIdx = m_pGrid_OK->SelectGetHeadPosition();

	for( INT i = 0 ; i < nCount ; i++ ) {

		m_pDM_OK->GetCellData(_T("MARC"), nIdx, strStreamMarc);
		
		if (m_pInfo->pMarcMgr->Decoding( strStreamMarc, pMarc ) < 0) continue;

		MarcEditCtrl.Init(m_pInfo->pMarcMgr, pMarc);
		MarcEditCtrl.Display();
			
		MarcEditCtrl.GetWindowText( strEditMarc );

		CString str;
		str.Format(_T("%c"), SUBFIELD_CODE);
		strEditMarc.Replace(str, _T("��"));

		str.Format(_T("%c"), FIELD_TERMINATOR);
		strEditMarc.Replace(str, _T("��"));

		CString sLeader;
		pMarc->GetLeader(sLeader);
		strEditMarc = sLeader + _T("��") + strEditMarc;
		
		//��ũ ¥����
		INT iStart = 0;

		while ( TRUE ) {
			INT iPos = strEditMarc.Find(_T("��"));
			if ( iPos == 0 ) {
				iPos = strEditMarc.Find(_T("\r\n"));
			}
			CString sData = strEditMarc.Mid( iStart, iPos+2 );			
			strEditMarc = strEditMarc.Mid( iPos+2 );
			strEditMarc.TrimLeft();
			INT iLineWordCount = 1000;

			while ( sData.GetLength() > iLineWordCount ) {
				CString sDataSub = sData.Left( iLineWordCount );
				BOOL bCh = IsCanCut( sDataSub, --iLineWordCount );
				if ( bCh == FALSE ) {
					iLineWordCount;
					sDataSub = sData.Left( iLineWordCount );
				}
				pACPrintDM->InsertRow(-1);
				pACPrintDM->SetCellData( _T("MarcLine"), sDataSub, iCount );
				iCount++;
				sData = sData.Mid(iLineWordCount);
			}
			iStart = 0;
			pACPrintDM->InsertRow(-1);
			pACPrintDM->SetCellData( _T("MarcLine"), sData, iCount );
			iCount++;

			if ( iPos == -1 ) {
				break;
			}
		}

		pACPrintDM->InsertRow(-1);
		pACPrintDM->SetCellData( _T("MarcLine"), _T(""), iCount );
		iCount++;

		nIdx = m_pGrid_OK->SelectGetNext();
	}	

	CSIReportManager SIReportManager(this);
	SIReportManager.SetCONNECTION_INFO(pACPrintDM->CONNECTION_INFO);
	INT ids = SIReportManager.GetSIReportFileFromDB(_T("�Է±�����"));
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return -1;
	}
	
	SIReportManager.SetDataMgr(0, pACPrintDM, 0);
	
	EndWaitCursor();
	
	ids = SIReportManager.Print();
	return 0;

EFS_END
return -1;
}

BOOL CMarcImportDlg::IsCanCut(CString sStr, INT nPos)
{
	EFS_BEGIN
		
		INT i, nLastPos = -1;
	//���ڼ� �ʰ��� FALSE
	if(nPos > sStr.GetLength())
		return FALSE;
	TCHAR ch = sStr.GetAt(nPos);
	//������ ���� || �ѱ��� �ƴϸ� TRUE
	if(nPos + 1 == sStr.GetLength() || (ch & (0x80)) != 128)
		return TRUE;
	//ù���ڰ� 1byte���ڸ� TRUE �ƴϸ� FALSE
	if(nPos == 0)
		if((ch & (0x80)) != 128)
			return TRUE;
		else
			return FALSE;
		
		for(i = 0; i < nPos; i++) {
			ch = sStr.GetAt(i);
			//�ڸ��� �κп��� ���� �����(��������)������ ��ġ
			if((ch & (0x80)) != 128)
				nLastPos = i;
		}
		if (nLastPos == -1 && nPos % 2 == 1) return FALSE;
		//¦���� ���̳��� TRUE
		return !((nPos - nLastPos) % 2);
		
		EFS_END
			return FALSE;
		
}

INT CMarcImportDlg::InsertToDB(CString sStreamMarc)
{
EFS_BEGIN

	CString sACMasterReckey;
	m_pDM_TOTAL->MakeRecKey(sACMasterReckey);

	CMarc *pMarc = NULL;
	pMarc = new CMarc;

	m_pInfo->pMarcMgr->Decoding(sStreamMarc, pMarc );

	if ( InsertACMASTERTBL(sACMasterReckey, pMarc) < 0 ) return -1;
	
	if ( InsertACTBL(sACMasterReckey, pMarc) < 0 ) return -1;

	delete pMarc;

	return 0;

EFS_END
return -1;
}

INT CMarcImportDlg::InsertACMASTERTBL(CString sACMasterKey, CMarc *pMarc)
{
EFS_BEGIN

	INT ids = -1;

	m_pDM_TOTAL->FreeData();

//	CESL_DataMgr * pDM = new CESL_DataMgr;
//	pDM->SetCONNECTION_INFO(m_pDM_TOTAL->CONNECTION_INFO);

	ids = m_pDM_TOTAL->StartFrame();
	if ( ids < 0 ) return -1;

	m_pDM_TOTAL->InsertRow(-1);
	m_pDM_TOTAL->SetCellData(_T("REC_KEY"), sACMasterKey , 0);

	//���� �����ȣ ���ϱ� 
	CString sACConNo = GetAcControlNo(m_pDM_TOTAL);

	if ( sACConNo == _T("") ) return -1;
	m_pDM_TOTAL->SetCellData(_T("AC_CONTROL_NO"), sACConNo , 0);

	//MARC�� �����ȣ �Է�
	m_pInfo->pMarcMgr->DeleteField(pMarc, _T("001"));
	m_pInfo->pMarcMgr->InsertField(pMarc, _T("001") + sACConNo);

	// STREAM MARC �Է�
	CString sStreamMarc;
	m_pInfo->pMarcMgr->Encoding(pMarc, sStreamMarc );
	m_pDM_TOTAL->SetCellData(_T("MARC"), sStreamMarc , 0);

	//�������� ���ϱ�
	CString ACType = GetACType(pMarc);
	if ( ACType == _T("") ) return -1;

	m_pDM_TOTAL->SetCellData(_T("AC_TYPE"), ACType , 0);
	m_pDM_TOTAL->SetCellData(_T("AC_CLASS"), _T("") , 0);
	m_pDM_TOTAL->SetCellData(_T("SEE_ALSO_REFERENCE_YN"), _T("N") , 0);


	CTime t = CTime::GetCurrentTime();
	CString strMsg;
	strMsg.Format(_T("%04d%02d%02d %02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute());


	m_pDM_TOTAL->SetCellData(_T("INPUT_DATE"), strMsg , 0);
	m_pDM_TOTAL->SetCellData(_T("UPDATE_DATE"), strMsg , 0);

	m_pDM_TOTAL->SetCellData(_T("FIRST_WORKER"), m_pInfo->USER_ID , 0);
	m_pDM_TOTAL->SetCellData(_T("LAST_WORKER"), m_pInfo->USER_ID , 0);

	m_pDM_TOTAL->SetCellData(_T("FIRST_WORK"), GetWorkLogMsg(_T("���Ÿ�ũ����"),WORK_LOG) , 0);
	m_pDM_TOTAL->SetCellData(_T("LAST_WORK"), GetWorkLogMsg(_T("���Ÿ�ũ����"),WORK_LOG) , 0);

	CArray<CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();

	ids = m_pDM_TOTAL->MakeQueryByDM(RemoveAliasList, m_pDM_TOTAL, m_pDM_TOTAL->TBL_NAME, 0, m_pDM_TOTAL);
	if ( ids < 0) return -1;

	ids = m_pDM_TOTAL->SendFrame();
	if ( ids < 0) return -1;

	ids = m_pDM_TOTAL->EndFrame();
	if ( ids < 0 ) return -1;

	return 0;


EFS_END
return -1;
}

INT CMarcImportDlg::InsertACTBL(CString sACMasterKey, CMarc *pMarc)
{
EFS_BEGIN

	INT ids = -1;
	CESL_DataMgr *pDM = NULL;
	INT iACMode = -1;

	iACMode = GetACMode(pMarc);

	if ( iACMode == 0 )
		pDM = FindDM( _T("DM_����_���Ÿ�ũ����_AUTHOR") );
	else if ( iACMode == 1 )
		pDM = FindDM( _T("DM_����_���Ÿ�ũ����_TITLE") );
	else if ( iACMode == 2 )
		pDM = FindDM( _T("DM_����_���Ÿ�ũ����_SUBJECT") );

	if ( pDM == NULL ) return -1;

	ids = pDM->StartFrame();
	if ( ids < 0 ) return -1;
	
	CString sACReckey;
	
	// �����ε��� ���̺� �Է�
	// ä��ǥ���� �����Ϳ� �ε����� ����ǥ���� �ε����� �ִ´�.
	// ����ǥ���� ���Ѵ�.

	CArray <CString, CString &> sArrayRef;
	sArrayRef.RemoveAll();
	
	ids = GetRefItem(pMarc,GetACType(pMarc),&sArrayRef);
	if( ids < 0 ) return -1;

	INT nCount = sArrayRef.GetSize();
	if(nCount < 1)
		return 0;

	CString sACKeyWord = _T("");
	for( INT i = 0; i < nCount+1 ; i++ )
	{
		pDM->MakeRecKey(sACReckey);
		pDM->InsertRow(-1);
		pDM->SetCellData(_T("REC_KEY"), sACReckey , i);
		pDM->SetCellData(_T("AC_MASTER_MAT_KEY"), sACMasterKey , i);

		if(i == nCount)
		{
			sACKeyWord = GetACKeyword(pMarc);
			pDM->SetCellData(_T("KEYWORD"), sACKeyWord , i);
			pDM->SetCellData(_T("SELECT_YN"), _T("Y") , i);
		}
		else
		{
			pDM->SetCellData(_T("KEYWORD"), sArrayRef.GetAt(i) , i);
			pDM->SetCellData(_T("SELECT_YN"), _T("N") , i);
		}

		pDM->SetCellData(_T("FIRST_WORK"), GetWorkLogMsg(_T("���Ÿ�ũ����"),WORK_LOG) , i);
		pDM->SetCellData(_T("LAST_WORK"), GetWorkLogMsg(_T("���Ÿ�ũ����"),WORK_LOG) , i);

		CArray<CString, CString> RemoveAliasList;
		RemoveAliasList.RemoveAll();

		ids = pDM->MakeQueryByDM(RemoveAliasList, pDM, pDM->TBL_NAME, i, pDM);
		if ( ids < 0) return -1;
	}

	ids = pDM->SendFrame();
	if ( ids < 0) return -1;

	ids = pDM->EndFrame();
	if ( ids < 0 ) return -1;

	return 0;

EFS_END
return -1;
}

CString CMarcImportDlg::GetAcControlNo(CESL_DataMgr *pDM)
{
	EFS_BEGIN
		
	CTime t = CTime::GetCurrentTime();
	
	CString sYear;
	sYear.Format(_T("%04d"), t.GetYear());
	
	CString strQuery;
	strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE MAKE_YEAR = '%s' AND DIVISION_VALUE = 'KAC' AND KIND_CODE = 'CON_NO'"), sYear );
	
	//�����ȣ �˻�
	CString sACConNo = _T("");
	pDM->GetOneValueQuery(strQuery, sACConNo);	
	
	if ( sACConNo == _T("") )
	{
		strQuery.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL (REC_KEY, KIND_CODE, PUBLISH_FORM_CODE, MAKE_YEAR, DIVISION_VALUE, LAST_NUMBER, CREATE_DATE, CREATE_WORKER, FIRST_WORK, LAST_WORK) VALUES ( ESL_SEQ.NEXTVAL, 'CON_NO', 'CO', '%s', 'KAC', '1', '','','','');"), sYear );
		pDM->AddFrame(strQuery);
		sACConNo = _T("1");
	}
	else {
		INT iTmp = _ttoi(sACConNo);
		iTmp++;
		sACConNo.Format(_T("%d"), iTmp);
		
		strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = '%s' WHERE KIND_CODE = 'CON_NO' AND  PUBLISH_FORM_CODE = 'CO' AND MAKE_YEAR = '%s' AND DIVISION_VALUE = 'KAC' ;"), sACConNo, sYear );
		pDM->AddFrame(strQuery);
	}
	
	CString sTmp0 = _T("");
	for ( INT i = sACConNo.GetLength() ; i < 9 ; i ++)
	{
		sTmp0 += _T("0");
	}
	sACConNo = _T("KAC") + sTmp0 + sACConNo;
	
	return sACConNo;
	
	EFS_END
		return _T("");
}

CString CMarcImportDlg::GetACType(CMarc *pMarc)
{
	EFS_BEGIN
	if ( pMarc == NULL ) return _T("");
		
	CString sTmpItem = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("100$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return _T("0");
	
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("110$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return _T("1");
	
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("111$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return _T("2");
	
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("130$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return _T("3");
	
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("150$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return _T("4");
	
	
EFS_END
return _T("");
}

INT CMarcImportDlg::GetACMode(CMarc *pMarc)
{
EFS_BEGIN

	CString sTmpItem = _T("");
	
	sTmpItem = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("100$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return 0;

	sTmpItem = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("110$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return 0;

	sTmpItem = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("111$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return 0;

	sTmpItem = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("130$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return 1;

	sTmpItem = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("150$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return 2;


EFS_END
return -1;
}

CString CMarcImportDlg::GetACKeyword(CMarc *pMarc)
{
EFS_BEGIN

	if ( pMarc == NULL ) return _T("");

	CString sTmpItem = _T("");
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("100$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return sTmpItem;

	m_pInfo->pMarcMgr->GetItem(pMarc, _T("110$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return sTmpItem;

	m_pInfo->pMarcMgr->GetItem(pMarc, _T("111$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return sTmpItem;

	m_pInfo->pMarcMgr->GetItem(pMarc, _T("130$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return sTmpItem;

	m_pInfo->pMarcMgr->GetItem(pMarc, _T("150$a"), sTmpItem );
	if ( sTmpItem != _T("") ) return sTmpItem;

EFS_END
return _T("");
}

INT CMarcImportDlg::GetRefItem(CMarc * pMarc, CString sType, CArray <CString, CString&>* sArray)
{
	//CString <CString , CString &> sArray;
	CString str;
	if( sType == _T("0") )
	{
		m_pInfo->pMarcMgr->GetItem(pMarc, _T("400$a"), str, sArray);
	}
	else if( sType == _T("1") )
	{
		m_pInfo->pMarcMgr->GetItem(pMarc, _T("410$a"), str, sArray);
	}
	else if( sType == _T("2") )
	{
		m_pInfo->pMarcMgr->GetItem(pMarc, _T("411$a"), str, sArray);
	}
	else if( sType == _T("3") )
	{
		m_pInfo->pMarcMgr->GetItem(pMarc, _T("430$a"), str, sArray);
	}
	else if( sType == _T("4") )
	{
		m_pInfo->pMarcMgr->GetItem(pMarc, _T("450$a"), str, sArray);
	}
	else 
	{
		AfxMessageBox(_T("ACType Undefined!!"));
		return -1;
	}
	return 0;
}

HBRUSH CMarcImportDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
