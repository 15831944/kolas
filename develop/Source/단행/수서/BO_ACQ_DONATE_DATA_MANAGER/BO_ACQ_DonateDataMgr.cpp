// BO_ACQ_NonDonateDataMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_DonateDataMgr.h"
#include "../../../����/BO_ACQ_PRINT_MGR/MakePrintDMMgr.h"
#include "../../../����/BO_ACQ_GROUP_MANAGER/RequisitionNo_List_Dlg.h"
#include "../../../����/BO_DATA_CHANGE_MANAGER/DonateInfoChange.h"
#include "../../../����/BO_ACQ_COMMON/BO_ACQ_ImportMgr.h"
#include "../../../esl/kolas3_main/kolas3_mainapi.h"
#include "..\..\..\����\Duplicate_Data_Search\DUP_KEY_SETTING.h"

#include "../BO_ACQ_FILE_IMPORT_EXPORT/BO_ACQ_FileImportMgr.h"
#include "../BO_ACQ_FILE_IMPORT_EXPORT/BO_ACQ_FileExportMgr.h"

#include "SBL.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_NonDonateDataMgr dialog


CBO_ACQ_DonateDataMgr::CBO_ACQ_DonateDataMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_DonateDataMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_NonDonateDataMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pPrintDM = NULL;
	m_pPrintObjDM = NULL;
	m_pSearchDlg = NULL;
	m_strDivNo = _T("");
	m_pPrintDlg = NULL;
	m_pDataProcessingDlg = NULL;
	m_pDuplicateSearchMgr = NULL;
	m_pSortItemSet = NULL;
	m_nRow = -1;
}

CBO_ACQ_DonateDataMgr::~CBO_ACQ_DonateDataMgr()
{
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}	
	if(m_pPrintDlg)
	{
		delete m_pPrintDlg;
		m_pPrintDlg = NULL;
	}
	if(m_pDataProcessingDlg)
	{
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
	if(m_pDuplicateSearchMgr)
	{
		delete m_pDuplicateSearchMgr;
		m_pDuplicateSearchMgr = NULL;
	}
	if(m_pSortItemSet)
	{
		delete m_pSortItemSet;
		m_pSortItemSet = NULL;
	}

}

VOID CBO_ACQ_DonateDataMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_NonDonateDataMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_DonateDataMgr, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_NonDonateDataMgr)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_MESSAGE(MAKE_PRINT_DM, OnMakePrintDM)
	ON_MESSAGE(SHOW_SEARCH_RESULT, OnShowSearchResult)
	ON_MESSAGE(DUPLICATE_DATA_DELETE, OnDuplicateDataDelete)
	ON_MESSAGE(SET_PRINT_DATA_ORDER_CONDITION, OnSetOrderCondition)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_NonDonateDataMgr message handlers

VOID CBO_ACQ_DonateDataMgr::OnClose() 
{
	CDialog::OnClose();
}

BOOL CBO_ACQ_DonateDataMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("K2UPGRADE_���������ڷ����")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pDM			= FindDM(_T("DM_���������ڷ����"));
	m_pPrintDM		= FindDM(_T("DM_���������ڷ����_��"));
	m_pPrintObjDM	= FindDM(_T("DM_���������ڷ����_��_��¿�"));
	m_pCM			= FindCM(_T("CM_���������ڷ����"));
	m_pGrid			= (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	GetManageValue(_T("����"), _T("����"), _T("���������������"), _T(""), m_strPosNumber);

	if(!m_pInfo->m_bUserInfoExportYN)
	{
		m_pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		m_pGrid->m_bPrivacyPrintLogRecord = TRUE;
		m_pGrid->m_strPrintInfo = _T("�����ڷ� ����");
	}
	
	GetDlgItem(IDC_stcDONATE_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtDONATE_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcDONATE_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtDONATE_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);

	if( m_pSearchDlg )
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	m_pSearchDlg = new CSearchMgr(this, BO_ACQ_DONATE, DONATE, BO_DONATE, m_pDM->CONNECTION_INFO);
	m_pPrintDlg = new CBO_ACQ_PrintDlg(BO_DONATE, DONATE, this);
	m_pPrintDlg->SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	InitSortItem();
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);
	CString strMsg = _T("���� �����ڷḦ �����մϴ�.");
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	SetDonateDataInfo();
	
	InitPopupMenuItem();

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;
}

VOID CBO_ACQ_DonateDataMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);

	CRect rect;
	CButton *pBtn = (CButton*)GetDlgItem(IDC_stcDONATE_GROUP_CREATE_YEAR);
	if(pBtn->GetSafeHwnd() == NULL) return;
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(20, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_edtDONATE_GROUP_CREATE_YEAR);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(80, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	pBtn = (CButton*)GetDlgItem(IDC_stcDONATE_DIVNO);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(160, 13, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	pEdt = (CEdit*)GetDlgItem(IDC_edtDONATE_DIVNO);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(220, 10, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridDONATE_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;

	pGrid->MoveWindow(-2, 35, cx+2, cy-33);		
}

BOOL CBO_ACQ_DonateDataMgr::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_ACQ_DonateDataMgr::ShowSearchDlg()
{
	m_pSearchDlg->ShowSearchDlg();
}

VOID CBO_ACQ_DonateDataMgr::ShowPrintDlg()
{
	if( !m_pPrintDlg->GetCreate() )
	{
		m_pPrintDlg->Create(this);
	}

	m_pPrintDlg->PrivacyPrintLogYN = TRUE;

	m_pPrintDlg->ShowWindow(SW_SHOW);
	m_pPrintDlg->UpdateWindow();
	m_pPrintDlg->CenterWindow();	
}

VOID CBO_ACQ_DonateDataMgr::ShowDonateDataDlg(OPERATION_TYPE nOpType)
{
	INT nRowCnt = m_pPrintDM->GetRowCount();
	CArray<INT, INT> arrDelIdx;	arrDelIdx.RemoveAll();
	CSpecies_Form dlg(this);
	dlg.SetAcqType(DONATE);
	dlg.SetSpeciesType(BO_DONATE);
	dlg.SetOpType(nOpType);
	if(nOpType != SINSERT)
	{
		if(0 == m_pPrintDM->GetRowCount()) 
		{
			ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
			return ;
		}
		dlg.SetDonateRegYNArray(&arrDelIdx);
	}

	m_pGrid->SelectMakeList();
	if( m_pGrid->SelectGetCount() > 0 )
	{
		m_pGrid->SetAt( m_pGrid->GetIdx() , 0 , _T("V") );
		m_pGrid->SelectMakeList();
	}

	dlg.SetSpeciesKeyAlias(_T("IBS_��KEY"));
	dlg.SetAccuKeyAlias(_T("BP_��������KEY"));
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	dlg.SetParentType(SPECIES);
	dlg.SetParentDM(m_pPrintDM);
	dlg.SetParentBookDM(m_pDM);
	dlg.SetParentGrid(m_pGrid);
	dlg.DoModal();
	
	if(nOpType != SINSERT)
	{
		INT cnt = arrDelIdx.GetSize();
		if(cnt > 0)
		{
			CBO_ACQ_Business_Api api(this);
			api.DeleteSelectRow(m_pPrintDM, &arrDelIdx, _T(""));
			SetDonateDataInfo();
			DisplayGrid();
			return ;
		}
	}

	SetDonateDataInfo();
	if(nOpType == SINSERT)
	{
		INT nCurCnt = m_pPrintDM->GetRowCount();
		if(nCurCnt > nRowCnt)
		{
			m_pGrid->DisplayNumber();
			m_pGrid->SetTopRow(nRowCnt+1);
		}
	}	
}

INT CBO_ACQ_DonateDataMgr::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;

EFS_END
return -1;

}

VOID CBO_ACQ_DonateDataMgr::DeleteDonateData()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();		api.InitTempPtr();
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("BookDM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_DONATE);
	api.SetTempData(_T("ACQ_TYPE"), (INT)DONATE);
	api.SetTempData(_T("����Ÿ��"), (INT)SPECIES_DELETE);
	api.SetTempData(_T("å���˸��ƽ�"), _T("BP_��ǰå��"));
	api.SetTempData(_T("���ݾ˸��ƽ�"), _T("UDF_��ȯ����"));
	api.SetTempData(_T("��������"), _T(""));
	api.SetTempPtr(_T("��Ű����Ʈ"), NULL);
	api.SetTempPtr(_T("��������Ű����Ʈ"), NULL);
	api.SetTempPtr(_T("���Ϸù�ȣ����Ʈ"), NULL);
	ids = api.SPFExecutePath(_T("����"));
	if(ids == 0)
	{
		SetDonateDataInfo();
		DisplayGrid();	
	}
}

VOID CBO_ACQ_DonateDataMgr::ShowChangeCodeDlg()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}

	CCodeChange dlg(this);
	dlg.SetDM(m_pPrintDM);
	dlg.SetBookDM(m_pDM);
	dlg.SetMainGrid(m_pGrid);
	dlg.SetAcqType(2);
	if(IDOK == dlg.DoModal())
	{
		m_pGrid->SetProtectSelectColumn(TRUE);
		DisplayGrid();
		m_pGrid->SetProtectSelectColumn(FALSE);
	}
}

VOID CBO_ACQ_DonateDataMgr::ShowChangeDonateInfoDlg()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	INT nPreCnt;
	INT nNextCnt;
	nPreCnt = m_pPrintDM->GetRowCount();

	CDonateInfoChange dlg(this);
	dlg.SetDM(m_pPrintDM);
	dlg.SetBookDM(m_pDM);
	dlg.SetMainGrid(m_pGrid);
	if(IDOK == dlg.DoModal())
	{
		nNextCnt = m_pPrintDM->GetRowCount();
		if( nPreCnt ==  nNextCnt )
		{
			m_pGrid->SetProtectSelectColumn(TRUE);
			DisplayGrid();
			m_pGrid->SetProtectSelectColumn(FALSE);
		}
		else
		{
			DisplayGrid();
		}				
	}
}

VOID CBO_ACQ_DonateDataMgr::ShowDivNoMgrDlg()
{
	CRequisitionNo_Mgr dlg(this);
	dlg.SetOpenMode(0);
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	dlg.DoModal();
}

UINT CBO_ACQ_DonateDataMgr::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	BEGIN_SBL();

	CString strQuery;
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("���������ڷḦ �������Դϴ�."));
	m_pSearchDlg->GetWhereStmt(strQuery);
	m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	m_pPrintDM->FreeData();
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0)
	{
		DeleteDataProcessingDlg();
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		DisplayGrid();
		SetDonateDataInfo();
		return 0;
	}
	CBO_ACQ_Business_Api mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("åDM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("��DM"), (DWORD*)m_pPrintDM);
	mgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
	INT ids = mgr.SPFExecutePath(_T("åTo��������"));
	DeleteDataProcessingDlg();
	if(ids < 0) return ids;
	m_pSortItemSet->DataSort(m_pPrintDM, !m_pSearchDlg->GetDataSortYN());
	SetDonateDataInfo();

	DisplayGrid();

	INT nRowCount = m_pPrintDM->GetRowCount();
	END_SBL(nRowCount);

	return 0;
}

UINT CBO_ACQ_DonateDataMgr::OnMakePrintDM(LPARAM lParam, WPARAM wParam)
{
	INT ids;
	BOOL bExtraInfo = FALSE;
	CString strDivAlias = _T("BP_��������KEY");
	CMapStringToString MapDivValue;		MapDivValue.RemoveAll();
	CMakePrintDMMgr PrintMgr(this);	

	switch( lParam )
	{
	case BO_ACQ_DONATE_DATA:
		{			
			if( m_pPrintDlg->GetPrintDataType() == 2 )
			{
				ids = PrintMgr.GetGridSelectData(m_pGrid, m_pPrintDM, strDivAlias, &MapDivValue);
				if( ids < 0 )
				{
					ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
					return -1;
				}
			}
			
			ShowDataProcessingDlg();
			
			PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
			PrintMgr.SetTempData(_T("����ƮŸ��"), lParam);
			PrintMgr.SetTempData(_T("���о˸��ƽ�"), _T("BP_��������KEY"));
			PrintMgr.SetTempData(_T("å���˸��ƽ�"), _T("UDF_å��"));
			PrintMgr.SetTempData(_T("���ݾ˸��ƽ�"), _T("BP_����"));
			PrintMgr.SetTempData(_T("���ΰ��˸��ƽ�"), _T(""));
			PrintMgr.SetTempData(_T("����ڷ����Ŀ���"), m_pPrintDlg->GetDataSortYN());
			PrintMgr.SetTempData(_T("��µ�����Ÿ��"), m_pPrintDlg->GetPrintDataType());
			PrintMgr.SetTempPtr(_T("�׸��弱���ڷ�"), (DWORD*)&MapDivValue);
			PrintMgr.SetTempPtr(_T("���α׷�����Ʈ��"), (DWORD*)m_pDataProcessingDlg);
			PrintMgr.SetTempPtr(_T("����åDM"), (DWORD*)m_pDM);
			PrintMgr.SetTempPtr(_T("�������DM"), (DWORD*)m_pPrintDM);
			PrintMgr.SetTempPtr(_T("���DM"), (DWORD*)m_pPrintObjDM);
			PrintMgr.SetTempPtr(_T("��������"), (DWORD*)m_pSortItemSet);
			
			ids = PrintMgr.SPFExecutePath(_T("�Ϲݸ���Ʈ���"));
			DeleteDataProcessingDlg();

			if( ids < 0 )
			{
				ESLAfxMessageBox(_T("��¿� �����͸� ������ �� �����ϴ�."));
				return -1;
			}
			m_pPrintDlg->SetPrintDM(m_pPrintObjDM);
			m_pPrintDlg->Print();
		}
		break;
	case DONATOR_LABEL:
		{
			if( m_pPrintDlg->GetPrintDataType() == 2 )
			{
				ids = PrintMgr.GetGridSelectData(m_pGrid, m_pPrintDM, strDivAlias, &MapDivValue);
				if( ids < 0 )
				{
					ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
					return -1;
				}
			}

			ShowDataProcessingDlg();
			
			PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
			PrintMgr.SetTempData(_T("����ƮŸ��"), lParam);
			PrintMgr.SetTempData(_T("���о˸��ƽ�"), _T("BP_��������KEY"));
			PrintMgr.SetTempData(_T("å���˸��ƽ�"), _T("UDF_å��"));
			PrintMgr.SetTempData(_T("���ݾ˸��ƽ�"), _T("BP_����"));
			PrintMgr.SetTempData(_T("���ΰ��˸��ƽ�"), _T(""));
			PrintMgr.SetTempData(_T("����ڷ����Ŀ���"), m_pPrintDlg->GetDataSortYN());
			PrintMgr.SetTempData(_T("��µ�����Ÿ��"), m_pPrintDlg->GetPrintDataType());
			PrintMgr.SetTempPtr(_T("�׸��弱���ڷ�"), (DWORD*)&MapDivValue);
			PrintMgr.SetTempPtr(_T("���α׷�����Ʈ��"), (DWORD*)m_pDataProcessingDlg);
			PrintMgr.SetTempPtr(_T("����åDM"), (DWORD*)m_pDM);
			PrintMgr.SetTempPtr(_T("�������DM"), (DWORD*)m_pPrintDM);
			PrintMgr.SetTempPtr(_T("���DM"), (DWORD*)m_pPrintObjDM);
			PrintMgr.SetTempPtr(_T("��������"), (DWORD*)m_pSortItemSet);
			
			ids = PrintMgr.SPFExecutePath(_T("�Ϲݸ���Ʈ���"));
			DeleteDataProcessingDlg();

			if( ids < 0 )
			{
				ESLAfxMessageBox(_T("��¿� �����͸� ������ �� �����ϴ�."));
				return -1;
			}
						
			CSIReportManager* pSIReportManager = new CSIReportManager(this);;
			pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
			if( !pSIReportManager )
			{
				AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
				return -1;
			}
			
			pSIReportManager->SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
							
			ids = pSIReportManager->GetSIReportFileFromDB(_T("K2UP_�����ڶ����"));
			if( ids < 0 )
			{
				AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
				return -1;
			}
			pSIReportManager->m_nDefaultPaperType = PAPER_A4_PORTRAIT;
			ids = pSIReportManager->DeleteTempPageFiles();
			
			BOOL IsPrint;
			const INT nTmpAliasCnt = 3;
			CString strDonator, sDonatorName[10], sDonatorPosition[10], sDonateDate[10];
			CString sTmpAlias[nTmpAliasCnt] =
			{
				_T("BP_�����ڸ�") , _T("DO_�Ҽ�") , _T("BP_�Է���") 
			};

			INT nRows = m_pPrintObjDM->GetRowCount();
			if( nRows < 2 )
			{
				AfxMessageBox(_T(" ����� �����Ͱ� �����ϴ�."));
				return -1;
			}
			
			for(INT i = 0; i < nRows-1 ; i++)
			{
				IsPrint = FALSE;
				// ����
				if( i == nRows-2) IsPrint = TRUE;
				if( (i+1)%10==0 && (i!=0) ) IsPrint = TRUE;
				if( 2 == nRows ) IsPrint = TRUE;
				
				ids = m_pPrintObjDM->GetCellData( sTmpAlias[0] , i , sDonatorName[(i)%10] );
				if( ids < 0 ) return -1;
				ids = m_pPrintObjDM->GetCellData( sTmpAlias[1] , i , sDonatorPosition[(i)%10] );
				if( ids < 0 ) return -1;
				ids = m_pPrintObjDM->GetCellData( sTmpAlias[2] , i , sDonateDate[(i)%10] );
				if( ids < 0 ) return -1;
			

				if( !IsPrint ) continue;

				CString sVariableName;
				for( INT j = 0 ; j < 10 ; j++ )
				{
					sVariableName.Format( _T("������%d") , j+1 );
					pSIReportManager->SetTempVariable( sVariableName , sDonatorName[j] );
					sVariableName.Format( _T("�Ҽ�%d") , j+1 );
					pSIReportManager->SetTempVariable( sVariableName , sDonatorPosition[j] );
					sVariableName.Format( _T("������%d") , j+1 );
					pSIReportManager->SetTempVariable( sVariableName , sDonateDate[j] );

				}
				
				ids = pSIReportManager->MAKEPAGE_DrawWorkForm(-1);

				for( j = 0 ; j < 10 ; j++ )
				{
					sDonatorName[j].Empty();
					sDonatorPosition[j].Empty();
					sDonateDate[j].Empty();
				}
			}
			
			ids = pSIReportManager->Print2();
			
			if (pSIReportManager) 
			{
				delete pSIReportManager;
				pSIReportManager = NULL;
			}
		}
		break;

	case BO_ACQ_DONATE_CHECKING_DATA_DIVNO:
	case BO_ACQ_DONATE_CHECKING_DATA_PROVIDER:
	case BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE:
	case BO_ACQ_DONATE_CHECKED_DATA_DIVNO:
	case BO_ACQ_DONATE_CHECKED_DATA_PROVIDER:
	case BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE:
		{
			return PrintDonateSubTotalData(lParam, wParam);
		}
		break;

	default:
		break;
	}

	return 0;
}

INT CBO_ACQ_DonateDataMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strTmpData;
	INT nGroupNo;
	if(m_nRow != nRow)
	{
		CMarc marc;
		INT ids;
		strTmpData = m_pPrintDM->GetCellData(_T("BS_MARC"), nRow);
		ids = m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
		if(ids < 0) return -1;
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_�Ѽ�����"), strTmpData, nRow);
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetEditionStatement(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("IBS_�ǻ���"), strTmpData, nRow);
		strTmpData.Empty();	 strTmpData = m_pPrintDM->GetCellData(_T("IBS_��ǥ��"), nRow);
		ids = m_MarcConvertMgr.GetTitleStatement(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_ǥ������"), strTmpData, nRow);
		ids = m_MarcConvertMgr.GetPublishPlace(&marc, strTmpData);
		if(ids >= 0) m_pPrintDM->SetCellData(_T("UDF_������"), strTmpData, nRow);
		m_nRow = nRow;
		if(m_nRow == m_pPrintDM->GetRowCount()-1) m_nRow = -1;
	}

	if(strDMFieldAlias == _T("UDF_������ȣ����"))
	{	
		strAcqYear = m_pPrintDM->GetCellData(_T("BP_���Գ⵵"), nRow);
		strGroupNo = m_pPrintDM->GetCellData(_T("BP_�׷��ȣ"), nRow);
		nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
		str.Format(_T("%s-%04d"), strAcqYear, nGroupNo);// = strAcqYear+_T("-")+strGroupNo;
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
		strGroupNo.Format(_T("%04d"), nGroupNo);
		m_pPrintDM->SetCellData(_T("UDF_���������ȣ����"), strGroupNo, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_�Է±��м���"))
	{
		strTmpData = m_pPrintDM->GetCellData(_T("BP_�Է±���"), nRow);
		if(strTmpData == _T("0")) str = _T("ȭ���Է�");
		else if(strTmpData == _T("1")) str = _T("��Ȱ�����");
		else if(strTmpData == _T("2")) str = _T("����ڷ����");
		else if(strTmpData == _T("3")) str = _T("���Ϲ���");
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if(strDMFieldAlias == _T("BP_��������"))
	{
		strTmpData = m_pPrintDM->GetCellData(_T("IBS_�з�ǥ����"), nRow);
		
		if		( strTmpData == _T("1") )
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("KDC��������ȣ����"), str, str);
		}
		else if	( strTmpData == _T("2") )
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("DDC��������ȣ����"), str, str);
		}
		else if	( strTmpData == _T("3") || strTmpData == _T("7") )
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("LC������ȣ"), str, str);
		}
		else
		{
			str = _T("");
		}

		m_pPrintDM->SetCellData(_T("UDF_�������м���"), str, nRow);
	}
	else if(strDMFieldAlias == _T("BP_����"))
	{
		DOUBLE strTemp;
		TCHAR* pEndPtr = NULL;
		strTemp = _tcstod(str.GetBuffer(0), &pEndPtr);

		CString strCurrencyCode;
		CString strTempPrice;
		INT nPos;

		m_pPrintDM->GetCellData(_T("BP_ȭ�󱸺�"), nRow, strCurrencyCode);
		if( 0 <= strCurrencyCode.Find(_T("\\")))
		{
			strTempPrice.Format(_T("%.2f"), strTemp);
			nPos = strTempPrice.Find(_T("."));
			str = strTempPrice.Left(nPos);
		}
		else
		{
			str = TruncPrice(m_pPrintDM, strTemp);
		}

		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pPrintDM->SetCellData(_T("UDF_��ȯ����"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_å��"))		m_pPrintDM->SetCellData(_T("BP_��ǰå��"), str, nRow);
	else if(strDMFieldAlias == _T("UDF_�ܰ�"))
	{
		DOUBLE strTemp;

		TCHAR* pEndPtr = NULL;
		strTemp = _tcstod(str.GetBuffer(0), &pEndPtr);

		CString strCurrencyCode;
		CString strTempPrice;
		INT nPos;
		m_pPrintDM->GetCellData(_T("BP_ȭ�󱸺�"), nRow, strCurrencyCode);
		if( 0 <= strCurrencyCode.Find(_T("\\")))
		{
			strTempPrice.Format(_T("%.2f"), strTemp);
			nPos = strTempPrice.Find(_T("."));
			str = strTempPrice.Left(nPos);
		}
		else
		{
			str = TruncPrice(m_pPrintDM, strTemp);
		}		

		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pPrintDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	return 0;
}

VOID CBO_ACQ_DonateDataMgr::DisplayGrid()
{
	BOOL bSetTopRow = TRUE;
	INT nCurSel = m_pGrid->GetTopRow();
	if(nCurSel < 0) bSetTopRow = FALSE;
	if(m_pPrintDM->GetRowCount() == 0) bSetTopRow = FALSE;
	if(nCurSel > m_pPrintDM->GetRowCount()) bSetTopRow = FALSE;
	m_pGrid->Display();
	if(bSetTopRow) m_pGrid->SetTopRow(nCurSel);
}

VOID CBO_ACQ_DonateDataMgr::SetDonateDataInfo()
{
	CString strMsg;
	INT nRowCnt = m_pPrintDM->GetRowCount();
	INT nBookCnt = m_pDM->GetRowCount();
	strMsg.Format(_T("%d�� %då�� ���� �����ڷᰡ �˻��Ǿ����ϴ�."), nRowCnt, nBookCnt);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strMsg);
	m_pCM->SetControlMgrData(_T("�׷��ȣ"), strMsg);
	m_pCM->SetControlMgrData(_T("���Գ⵵"), m_pSearchDlg->GetGroupCollection()->GetAcqYear());
}

INT CBO_ACQ_DonateDataMgr::ShowDataDonateImport()
{
	INT nRowCnt = m_pPrintDM->GetRowCount();
	m_pCM->GetControlMgrData(_T("�׷��ȣ"), m_strDivNo);
	CBO_ACQ_ImportMgr importDlg(this);
	importDlg.SetAcqType(DONATE);
	CGroupCollection * pGroups = m_pSearchDlg->GetGroupCollection();
	if(pGroups->GetGroupCount() == 0)
	{
		if(IDYES == ESLAfxMessageBox(_T("������ ������ȣ������ �����ϴ�. ������ȣ�� �����Ͻðڽ��ϱ�?"), MB_YESNO))
		{
			CRequisitionNo_List_Dlg dlg(this);
			dlg.SetSpeciesType(BO_DONATE);
			dlg.SetAcqYear(pGroups->GetAcqYear());
			dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
			dlg.DoModal();
		}
		else return 0;
	}

	if(pGroups->IsEmpty()) return 0;
	importDlg.SetGroups(m_pSearchDlg->GetGroupCollection());
	importDlg.SetParentDM(m_pPrintDM);
	importDlg.SetParentBookDM(m_pDM);
	importDlg.SetParentGrid(m_pGrid);
	importDlg.DoModal();
	
	SetDonateDataInfo();
	INT nCurCnt = m_pPrintDM->GetRowCount();
	if(nRowCnt < nCurCnt)
	{
		m_pGrid->DisplayNumber();
		m_pGrid->SetTopRow(nRowCnt+1);
	}
	return 0;
}

INT CBO_ACQ_DonateDataMgr::CheckSelectDataValid()
{
	if(m_pPrintDM->GetRowCount() == 0) return -1;

	if(!m_pGrid) return -1;
	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0) return -1;
	ids = m_pGrid->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}

VOID CBO_ACQ_DonateDataMgr::InitPopupMenuItem()
{
	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();
	m_pGrid->InitPopupMenuItem();
	m_pGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
}

BEGIN_EVENTSINK_MAP(CBO_ACQ_DonateDataMgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_ACQ_NonDonateDataMgr)
	ON_EVENT(CBO_ACQ_DonateDataMgr, IDC_gridDONATE_DATA, -601 /* DblClick */, OnDblClickgridDONATEDATA, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_ACQ_DonateDataMgr::OnDblClickgridDONATEDATA() 
{
	// TODO: Add your control notification handler code here
	if(m_pGrid->GetMouseRow() == 0 ) return ;
	ShowDonateDataDlg(GUPDATE);
}

VOID CBO_ACQ_DonateDataMgr::ShowDataProcessingDlg()
{
	if(m_pDataProcessingDlg == NULL) m_pDataProcessingDlg = new CDataProcessingDlg(this);

	if(!m_pDataProcessingDlg->GetCreate())
	{
		if(!m_pDataProcessingDlg->Create(this))
		{
			delete m_pDataProcessingDlg;
			m_pDataProcessingDlg = NULL;
			return;
		}
	}
	m_pDataProcessingDlg->CenterWindow();
	m_pDataProcessingDlg->ShowWindow(SW_SHOW);
	m_pDataProcessingDlg->UpdateWindow();

	m_pDataProcessingDlg->ShowHideCancelButton(FALSE);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
}

VOID CBO_ACQ_DonateDataMgr::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

VOID CBO_ACQ_DonateDataMgr::DupPurchaseDataSearch()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}

	if(!m_pDuplicateSearchMgr)
	{
		m_pDuplicateSearchMgr = new CDuplicateSearch_Interface(this);
		m_pDuplicateSearchMgr->Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), this, 1004, NULL);
	}

	if(m_pDuplicateSearchMgr->GetRunning())		
	{
		ESLAfxMessageBox(_T("�̹� �������簡 �������Դϴ�."));
		return ;
	}		
	m_pDuplicateSearchMgr->ShowHoldDuplicateSearch(_T("��������"), m_pPrintDM->DB_MGR_ALIAS, m_pCM->CM_ALIAS, _T("MainGrid"));
}

VOID CBO_ACQ_DonateDataMgr::DupKeySetting()
{
	CDUP_KEY_SETTING Dlg(this, _T(".\\duplicate_search_key.txt"));
	Dlg.DoModal();
}

UINT CBO_ACQ_DonateDataMgr::OnShowSearchResult(LPARAM lParam, WPARAM wParam)
{
	if(m_pDuplicateSearchMgr)
		m_pDuplicateSearchMgr->ShowDuplicateSearchDlg();
	return 0;
}

UINT CBO_ACQ_DonateDataMgr::OnDuplicateDataDelete(LPARAM lParam, WPARAM wParam)
{
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();		api.InitTempPtr();
	api.SetTempPtr(_T("��DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("��DM"), NULL);
	api.SetTempPtr(_T("åDM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("����������"), (DWORD*)m_pDuplicateSearchMgr->GetAcqKeyArray());
	api.SetTempData(_T("��Alias"), _T("IBS_��KEY"));
	INT ids = api.SPFExecutePath(_T("DM�ڷ����"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ DM���� ������ �� �����ϴ�."));
		return 0;
	}
	DisplayGrid();
	SetDonateDataInfo();
	return 0;
}

CString CBO_ACQ_DonateDataMgr::GetCommonManageValue(CString strGroup2, CString strGroup3, CString strClassAlias)
{
	CString strValue, strQuery;
	strQuery.Format(_T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE GROUP_2='%s' AND GROUP_3='%s' AND CLASS_ALIAS='%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), 
		strGroup2, strGroup3, strClassAlias);
	m_pDM->GetOneValueQuery(strQuery, strValue);
	return strValue;
}

VOID CBO_ACQ_DonateDataMgr::DonateDataOrder(INT nType/*=1*/)
{
	if(m_pSortItemSet == NULL || m_pPrintDM == NULL) 
	{
		ESLAfxMessageBox(_T("����ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		return;
	}
	if(m_pPrintDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	
	CDataSortDlg dlg(this);
	dlg.SetMainDM(m_pPrintDM);
	dlg.SetSortItemSet(m_pSortItemSet);
	dlg.SetOpenMode(nType);
	if(IDOK == dlg.DoModal() && nType > 0)
		DisplayGrid();
}

BOOL CBO_ACQ_DonateDataMgr::InitSortItem()
{
	CStringArray * pTmpData = NULL;	
	
	m_pSortItemSet = new CSortItemSet(this);
	m_pSortItemSet->SetGrid(m_pGrid);
	m_pSortItemSet->InitDataManager(m_pDM->CONNECTION_INFO, _T("DM_�����׸���"));
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(), _T("MainGrid"));
	pTmpData = m_pSortItemSet->GetHeadSortList();
	pTmpData->Add(_T("BP_���Գ⵵"));
	pTmpData->Add(_T("BP_�׷��ȣ"));
	CMapStringToString * pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	pMap->SetAt(_T("UDF_�������м���"), _T("BP_��������"));
	pMap->SetAt(_T("UDF_�Է±��м���"), _T("BP_�Է±���"));
	pTmpData = m_pSortItemSet->GetTailSortList();
	pTmpData->Add(_T("UDF_�Ϸù�ȣ"));
	m_pSortItemSet->ReadSortItem();
	m_pSearchDlg->SetDataSortItem(m_pSortItemSet);
	return TRUE;
}

UINT CBO_ACQ_DonateDataMgr::OnSetOrderCondition(LPARAM lParam, WPARAM wParam)
{
	DonateDataOrder(-1);
	return 0;
}

CString CBO_ACQ_DonateDataMgr::TruncPrice(CESL_DataMgr *pDM, DOUBLE dPrice)
{
	CString strTmpData, strTmp, strValue;
	INT nPos = -1;

	strTmp.Format(_T("%.2f"), dPrice);
	nPos = strTmp.Find(_T("."));

	if(m_strPosNumber == _T("1"))  
		strTmpData = strTmp.Left(nPos+2);
	else if(m_strPosNumber == _T("2"))  
		strTmpData = strTmp.Left(nPos+3);
	else
		strTmpData = strTmp.Left(nPos);

	return strTmpData;
}

INT CBO_ACQ_DonateDataMgr::PrintDonateSubTotalData(INT nPrintType, INT nExtraInfo)
{
	INT ids = -1;
	BOOL bExtraInfo = FALSE;
	CString strDivAlias = _T("BP_��������KEY");
	CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
	CMakePrintDMMgr PrintMgr(this);	

	if( nExtraInfo == 1000 )	bExtraInfo = TRUE;

	if( m_pPrintDlg->GetPrintDataType() == 2 )
	{
		ids = PrintMgr.GetGridSelectData(m_pGrid, m_pPrintDM, strDivAlias, &MapDivValue);
		if( ids < 0 )
		{
			ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
			return -1;
		}
	}

	ShowDataProcessingDlg();
	
	PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
	PrintMgr.SetTempData(_T("����ƮŸ��"),		nPrintType		);
	PrintMgr.SetTempData(_T("���о˸��ƽ�"),	_T("BP_��������KEY"));
	PrintMgr.SetTempData(_T("å���˸��ƽ�"),	_T("UDF_å��")	);
	PrintMgr.SetTempData(_T("���ݾ˸��ƽ�"),	_T("UDF_����")	);
	PrintMgr.SetTempData(_T("���ΰ��˸��ƽ�"),	_T("UDF_���ΰ�"));
	
	PrintMgr.SetTempData(_T("åDM�˸��ƽ�"),	m_pDM->DB_MGR_ALIAS.GetBuffer(0));
	PrintMgr.SetTempData(_T("KOLAS2����"),		bExtraInfo);
	PrintMgr.SetTempPtr(_T("�׷��÷���"),		(DWORD*)m_pSearchDlg->GetGroupCollection());
	
	PrintMgr.SetTempData(_T("����ڷ����Ŀ���"),	m_pPrintDlg->GetDataSortYN());
	PrintMgr.SetTempData(_T("��µ�����Ÿ��"),		m_pPrintDlg->GetPrintDataType());
	PrintMgr.SetTempPtr(_T("�׸��弱���ڷ�"),		(DWORD*)&MapDivValue);
	PrintMgr.SetTempPtr(_T("���α׷�����Ʈ��"),		(DWORD*)m_pDataProcessingDlg);
	PrintMgr.SetTempPtr(_T("���DM"),				(DWORD*)m_pPrintObjDM);
	PrintMgr.SetTempPtr(_T("��������"),				(DWORD*)m_pSortItemSet);
	
	switch( nPrintType )
	{
	case BO_ACQ_DONATE_CHECKING_DATA_DIVNO:
	case BO_ACQ_DONATE_CHECKED_DATA_DIVNO:
		{
			PrintMgr.SetTempData(_T("�����ʵ�˸��ƽ�"), _T("BP_�׷��ȣ"));
		}
		break;

	case BO_ACQ_DONATE_CHECKING_DATA_PROVIDER:
	case BO_ACQ_DONATE_CHECKED_DATA_PROVIDER:
		{
			PrintMgr.SetTempData(_T("�����ʵ�˸��ƽ�"), _T("BP_�����ڸ�"));
		}
		break;

	case BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE:
	case BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE:
		{
			PrintMgr.SetTempData(_T("�����ʵ�˸��ƽ�"), _T("BP_��������"));
		}
		break;

	default:
		{
			ESLAfxMessageBox(_T("��¹� ������ ������ �� �����ϴ�."));
			DeleteDataProcessingDlg();
			return -1;
		}
		break;
	}
	
	ids = PrintMgr.SPFExecutePath(_T("�Ұ��ڷ����"));

	DeleteDataProcessingDlg();

	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("��¿� �����͸� ������ �� �����ϴ�."));
		return -1;
	}
	else if( ids == 1000 )		return 0;
	
	m_pPrintDlg->SetPrintDM(m_pPrintObjDM);
	m_pPrintDlg->Print();

	return 0;
}

VOID CBO_ACQ_DonateDataMgr::ShowFileImport()
{
	INT nRowCnt = m_pPrintDM->GetRowCount();
	CFileImportDlg dlg(this);
	dlg.SetBeginImportStatus(_T("��������"), _T("BOA211O"));
	dlg.SetOpenMode(4000);
	dlg.SetAcqYear(m_pSearchDlg->GetGroupCollection()->GetAcqYear());
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	dlg.SetParentDM(m_pPrintDM);
	dlg.SetParentBookDM(m_pDM);
	dlg.SetParentGrid(m_pGrid);
	dlg.DoModal();

	INT nIdx;
	
	CString strImportAcqNo;
	CString strImportAcqYear;	

	CString strWhere;
	CString strSpeciesKey;
	CString strSpeciesKeys;
	CString strAcqNo;
	INT nCount;
	INT nPos;
	
	strImportAcqNo = dlg.m_strExtGruopInfo;
	strImportAcqYear = dlg.m_strExtAcqYear;

	CEdit* pEdit = (CEdit*) GetDlgItem(IDC_edtDONATE_DIVNO);
	pEdit->GetWindowText(strAcqNo);
	nPos = strAcqNo.Find(strImportAcqNo);
	if( nPos < 0 )
	{
		m_pSearchDlg->GetGroupCollection()->AddGroupInfo(strImportAcqYear, strImportAcqNo, _T("2"));
	}

	SetDonateDataInfo();
	INT nCurCnt = m_pPrintDM->GetRowCount();
	if(nRowCnt < nCurCnt)
	{
		m_pGrid->DisplayNumber();
		m_pGrid->SetTopRow(nRowCnt + 1);
	}
}

VOID CBO_ACQ_DonateDataMgr::RegTransfer(CString strPath)
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}

	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("BOOK_DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("DM"), (DWORD*)m_pPrintDM);
	api.SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);	
	api.SetTempData(_T("ó������"), _T("��"));
	api.SetTempData(_T("��Ű�˸��ƽ�"), _T("IBS_��KEY"));
	api.SetTempData(_T("��������Ű�˸��ƽ�"), _T("BP_��������KEY"));
	api.SetTempData(_T("ACQ_TYPE"), (INT)DONATE);
	ids = api.SPFExecutePath(strPath);
	CString strMsg;
	if(ids < 0)
	{
		strMsg.Format(_T("������ �ڷḦ %s ó���� �� �����ϴ�."), strPath);
		ESLAfxMessageBox(strMsg);
		return;
	}
	strMsg.Format(_T("������ �ڷḦ %s ó�� �Ͽ����ϴ�."), strPath);
	ESLAfxMessageBox(strMsg);
	SetRegObjDataInfo();
	DisplayGrid();	
}

VOID CBO_ACQ_DonateDataMgr::SetRegObjDataInfo()
{
	CString strMsg;
	INT nRowCnt = m_pPrintDM->GetRowCount();
	INT nBookCnt = m_pDM->GetRowCount();
	strMsg.Format(_T("%d�� %då�� ���� ��� ����ڷᰡ �˻��Ǿ����ϴ�."), nRowCnt, nBookCnt);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strMsg);
	m_pCM->SetControlMgrData(_T("�׷��ȣ"), strMsg);
	m_pCM->SetControlMgrData(_T("���Գ⵵"), m_pSearchDlg->GetGroupCollection()->GetAcqYear());
}

HBRUSH CBO_ACQ_DonateDataMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
