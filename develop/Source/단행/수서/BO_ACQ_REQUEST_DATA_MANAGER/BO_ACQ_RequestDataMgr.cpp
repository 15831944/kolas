// BO_ACQ_RequestDataMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_RequestDataMgr.h"
#include "Request_PrintMgr.h"
#include "Request_DataMgr.h"
#include "../../../����/BO_ACQ_API/BO_ACQ_Business_Api.h"
#include "../../../����/BO_ACQ_GROUP_MANAGER/DivisionNo_Select_Dlg.h"
#include "../../../����/BO_ACQ_GROUP_MANAGER/RequisitionNo_Select_Dlg.h"
#include "../../../����/BO_SPECIES_API/GroupInfo.h"
#include "../../../����/BO_ACQ_COMMON/RequestMailMgr.h"
#include "../../../����/Duplicate_Data_Search/DUP_KEY_SETTING.h"
#include "../../../����/BO_ACQ_PRINT_MGR/MakePrintDMMgr.h"
#include "../BO_ACQ_FILE_IMPORT_EXPORT/BO_ACQ_FileExportMgr.h"
#include "../BO_ACQ_FILE_IMPORT_EXPORT/BO_ACQ_FileImportMgr.h"
#include "SBL.H"
#include "BO_LOC_3000Api.h"
#include "DLG_BATCH_MODIFY.h"
#include "Request_StatisticDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_RequestDataMgr dialog
CBO_ACQ_RequestDataMgr::CBO_ACQ_RequestDataMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_RequestDataMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_RequestDataMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	AfxInitRichEdit();
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pSearchDlg = NULL;
	m_pDuplicateSearchMgr = NULL;
	m_pPrintDlg = NULL;
	m_pDataProcessingDlg = NULL;
	m_pSortItemSet = NULL;
	m_strPosNumber.Empty();
}

CBO_ACQ_RequestDataMgr::~CBO_ACQ_RequestDataMgr()
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

VOID CBO_ACQ_RequestDataMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_RequestDataMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_RequestDataMgr, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_RequestDataMgr)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_MESSAGE(SHOW_SEARCH_RESULT, OnShowSearchResult)
	ON_MESSAGE(MAKE_PRINT_DM, OnMakePrintDM)
	ON_MESSAGE(DUPLICATE_DATA_DELETE, OnDuplicateDataDelete)
	ON_MESSAGE(SET_PRINT_DATA_ORDER_CONDITION, OnSetOrderCondition)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_RequestDataMgr message handlers

VOID CBO_ACQ_RequestDataMgr::OnClose() 
{
	DestroyWindow();
}

BOOL CBO_ACQ_RequestDataMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strMsg;
	
	if( InitESL_Mgr(_T("K2UPGRADE_��ġ�������")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_��ġ����ڷ����"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	if( m_pCM == NULL || m_pGrid == NULL )
	{
		ESLAfxMessageBox( _T("[CM_��ġ����ڷ����]�� ������ �� �����ϴ�.") );
		return FALSE;
	}

	if(!m_pInfo->m_bUserInfoExportYN)
	{
		m_pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		m_pGrid->m_bPrivacyPrintLogRecord = TRUE;
		m_pGrid->m_strPrintInfo = _T("��ġ����ڷ� ����");
	}

	m_pDM		= FindDM(_T("DM_��ġ����ڷ����"));
	m_pPrintDM	= FindDM(_T("DM_��ġ����ڷ����_��¿�"));
	if( m_pDM == NULL || m_pPrintDM == NULL )
	{
		ESLAfxMessageBox( _T("[DM_��ġ����ڷ����/��¿�]�� ������ �� �����ϴ�.") );
		return FALSE;
	}

	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	GetManageValue(_T("����"), _T("����"), _T("���������������"), _T(""), m_strPosNumber);

	m_pSearchDlg = new CSearchMgr(this, BO_ACQ_REQUEST, ACQ_TYPE_INVALID, BO_REQUEST, m_pDM->CONNECTION_INFO);
	m_pPrintDlg = new CBO_ACQ_PrintDlg(BO_REQUEST, ACQ_TYPE_INVALID, this);
	
	if( InitSortItem() == FALSE )		return FALSE;
	
	strMsg = _T("��ġ��� �ڷḦ �����մϴ�.");
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));

	InitPopupMenuItem();

	if( m_pInfo->GetSMSUseMode() < 1 )
	{
		CRequestManager mgr(this);
		mgr.SetUDFCheck(m_pDM, _T("SMS���ſ���"));
	}

	GetManageValue(_T("��Ÿ"), _T("����"), _T("���յ�������"), _T("���յ�������"), m_sIsUnityLoanService);
	m_sIsUnityLoanService.TrimLeft();	m_sIsUnityLoanService.TrimRight();
	m_sIsUnityLoanService.MakeUpper();
	if(_T("Y")!=m_sIsUnityLoanService) m_sIsUnityLoanService = _T("N");
	if(TRUE == m_pInfo->m_bCertifyUse) m_strCivilNoViewYN = _T("N");
	else GetManageValue(_T("����"), _T("����"), _T("�̿��ڰ���"), _T("����ȭ���ֹι�ȣǥ������"), m_strCivilNoViewYN);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  
}

VOID CBO_ACQ_RequestDataMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridREQUEST_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 0, cx, cy);	
}

BOOL CBO_ACQ_RequestDataMgr::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_ACQ_RequestDataMgr::ShowPrintDlg()
{
	if( m_pPrintDlg->GetCreate() == FALSE )		 m_pPrintDlg->Create(this);

	m_pPrintDlg->ShowWindow(SW_SHOW);
	m_pPrintDlg->UpdateWindow();
	m_pPrintDlg->CenterWindow();
	m_pPrintDlg->PrivacyPrintLogYN = TRUE;
}

VOID CBO_ACQ_RequestDataMgr::ShowRequestDataDlg(OPERATION_TYPE nOpType)
{
	if( nOpType != SINSERT )
	{
		if( m_pDM->GetRowCount() == 0 )
		{
			ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
			return ;
		}
	}

	m_pGrid->SelectMakeList();

	if( m_pGrid->SelectGetCount() > 0 && nOpType != SINSERT)
	{
		m_pGrid->SetAt( m_pGrid->GetIdx() , 0 , _T("V") );
		m_pGrid->SelectMakeList();
	}

	INT nRowCnt = m_pDM->GetRowCount();
	CRequest_DataMgr dlg(this);
	dlg.SetOpType(nOpType);
	INT nIdx;
	nIdx = m_pGrid->GetIdx();
	dlg.SetIndex(nIdx);

	if(_T("N") == m_strCivilNoViewYN)
	{
		dlg.SetUserNoChange(TRUE);
	}
	
	dlg.DoModal();
	SetRequestDataInfo();
	if(nOpType == SINSERT)
	{
		INT nCurCnt = m_pDM->GetRowCount();
		if(nCurCnt > nRowCnt)
		{
			m_pGrid->DisplayNumber();
			m_pGrid->SetTopRow(nRowCnt+1);
		}
	}
}

VOID CBO_ACQ_RequestDataMgr::DeleteRequestData()
{
	INT ids = CheckSelectDataValid();
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}

	if(IDCANCEL == ESLAfxMessageBox(_T("�����Ͻ� �ڷḦ �����Ͻðڽ��ϱ�?\n�ػ����� DB���� �ٷ� �����˴ϴ�."), MB_OKCANCEL | MB_ICONQUESTION))		return;

	CRequestManager pMgr(this);
	pMgr.InitTempData();
	pMgr.SetTempData(_T("DMAlias"), m_pDM->DB_MGR_ALIAS.GetBuffer(0));
	pMgr.SetTempData(_T("CMAlias"), m_pCM->CM_ALIAS.GetBuffer(0));

	ids = pMgr.SPFExecutePath(_T("��ġ�������"));
	CString errstr;
	if( ids < 0 )
	{
		switch(ids)
		{
		case -1000:			errstr = _T("��ġ����ڷ� ������ ���� DM�� ã�� �� �����ϴ�.");
			break;
		case -1001:			errstr = _T("���� ����� ������ �� �����ϴ�.");
			break;
		case -1002:			errstr = _T("������ �ڷḦ DB���� �����ϴµ� ������ �߻��Ͽ����ϴ�.");
			break;
		}
		ESLAfxMessageBox(errstr.GetBuffer(0));
		return;
	}
	errstr = _T("������ ��ġ����ڷḦ ���� �Ͽ����ϴ�.");
	ESLAfxMessageBox(errstr.GetBuffer(0));
					
	DisplayGrid();
	SetRequestDataInfo();
}

VOID CBO_ACQ_RequestDataMgr::PurchaseRequestData()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}
	CDivisionNo_Select_Dlg dlg(this);
	dlg.SetWorkStatus(_T("���Դ��"));
	if(m_pInfo->GetKIIUPClientVersion() == 'L') dlg.SetAllowNULLGroup(TRUE);
	if(IDCANCEL == dlg.DoModal()) return ;
	
	CGroupInfo * groupInfo = dlg.GetSelGroup();
	CStringArray ArrayFurnishKey;
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_REQUEST);
	api.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("BOOKDM"), (DWORD*)NULL);
	api.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
	api.SetTempPtr(_T("GroupInfo"), (DWORD*)groupInfo);
	api.SetTempPtr(_T("FurnishKey"), (DWORD*)&ArrayFurnishKey);
	ids = api.SPFExecutePath(_T("����ó��"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ ����ó�� �� �� �����ϴ�."));
		return;
	}
	ESLAfxMessageBox(_T("������ �ڷḦ ����ó�� �Ͽ����ϴ�."));

	DisplayGrid();
}

VOID CBO_ACQ_RequestDataMgr::DonateRequestData()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}

	CRequisitionNo_Select_Dlg dlg(this);
	if(IDCANCEL == dlg.DoModal()) return ;
	
	CGroupInfo * groupInfo = dlg.GetSelGroup();
	CStringArray ArrayFurnishKey;
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_REQUEST);
	api.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("GRID"), (DWORD*)m_pGrid);
	api.SetTempPtr(_T("GroupInfo"), (DWORD*)groupInfo);
	api.SetTempPtr(_T("FurnishKey"), (DWORD*)&ArrayFurnishKey);
	ids = api.SPFExecutePath(_T("����ó��"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ ����ó�� �� �� �����ϴ�."));
		return;
	}
	ESLAfxMessageBox(_T("������ �ڷḦ ����ó�� �Ͽ����ϴ�."));

	DisplayGrid();
}

VOID CBO_ACQ_RequestDataMgr::ShowRequestStatusDlg()
{
	
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}

	CRequest_StatusMgr dlg(this);
	if( IDOK == dlg.DoModal() )
	{
		INT nCurIdx;
		nCurIdx = m_pGrid->GetIdx();
		m_pGrid->SetProtectSelectColumn(TRUE);
		DisplayGrid();
		m_pGrid->SetProtectSelectColumn(FALSE);
		m_pGrid->SetReverse(nCurIdx);
	}
}

VOID CBO_ACQ_RequestDataMgr::ShowRequestMailMgrDlg()
{
	INT nCurIdx;
	nCurIdx = m_pGrid->GetIdx();
	m_pGrid->SetProtectSelectColumn(TRUE);
	DisplayGrid();
	m_pGrid->SetProtectSelectColumn(FALSE);
	m_pGrid->SetReverse(nCurIdx);
}

UINT CBO_ACQ_RequestDataMgr::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	BEGIN_SBL();

	CString strMsg;
	CString strQuery;
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("��ġ����ڷḦ �������Դϴ�."));
	m_pSearchDlg->GetWhereStmt(strQuery);

	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	INT nIndex = m_pDM->FindColumn(_T("��ġ���_�ֹε�Ϲ�ȣ")); 
	if(-1 < nIndex)
	{
		m_pDM->GetColumnRefInfo(nIndex, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
		m_pDM->SetDataMgrRef(nIndex, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, _T("UDF"));
	}
	m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0)
	{
		DeleteDataProcessingDlg();
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		SetRequestDataInfo();
		DisplayGrid();
		return 0;
	}

	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < m_pDM->GetRowCount())
	{
		CString strGetData;
		CStringArray asDes1,asDes2,asDes3,asDes4,asDes5;
		for(INT i = 0; i < m_pDM->GetRowCount(); i++)
		{
			m_pDM->GetCellData(_T("�ڵ���"), i, strGetData);
			asDes1.Add(strGetData);
			m_pDM->GetCellData(_T("EMAIL"), i, strGetData);
			asDes2.Add(strGetData);
			m_pDM->GetCellData(_T("�ּ�"), i, strGetData);
			asDes3.Add(strGetData);
			m_pDM->GetCellData(_T("��ȭ��ȣ"), i, strGetData);
			asDes4.Add(strGetData);
			m_pDM->GetCellData(_T("��ġ���_�ֹε�Ϲ�ȣ"), i, strGetData);
			asDes5.Add(strGetData);
		}
		CBO_LOC_3000Api *pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("HANDPHONE"), asDes1);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("EMAIL"), asDes2);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("ADDRESS"), asDes3);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("TEL"), asDes4);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("CIVIL_NO"), asDes5);
					pApi->CloseSocket();	
				}
				for(i = 0; i < m_pDM->GetRowCount(); i++)
				{
					m_pDM->SetCellData(_T("�ڵ���"), asDes1.GetAt(i), i);
					m_pDM->SetCellData(_T("EMAIL"), asDes2.GetAt(i), i);
					m_pDM->SetCellData(_T("�ּ�"), asDes3.GetAt(i), i);
					m_pDM->SetCellData(_T("��ȭ��ȣ"), asDes4.GetAt(i), i);
					m_pDM->SetCellData(_T("��ġ���_�ֹε�Ϲ�ȣ"), asDes5.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
		asDes4.RemoveAll(); asDes4.FreeExtra();
		asDes5.RemoveAll(); asDes5.FreeExtra();
	}

	CString strValue1;
	ChangeSecuriyCivilNo(m_pDM,-1,_T("��ġ���_�ֹε�Ϲ�ȣ"));

	if(_T("N") == m_strCivilNoViewYN)
	{
		ChangeUserNo(m_pDM,_T("��ġ���_�����ڹ�ȣ"));
	}
	else
	{
		CString strTemp;
		for( INT i = 0 ; i < m_pDM->GetRowCount() ; i++ )
		{
			m_pDM->GetCellData( _T("��ġ���_�����ڹ�ȣ"), i ,strTemp);
			m_pDM->SetCellData( _T("�����ڹ�ȣ") , strTemp , i );
		}
	}

	SetRequestDataInfo();
	m_pSortItemSet->DataSort(m_pDM, !m_pSearchDlg->GetDataSortYN());
	DeleteDataProcessingDlg();
	DisplayGrid();

	INT nRowCount = m_pDM->GetRowCount();
	END_SBL(nRowCount);
	
	return 0;
}

VOID CBO_ACQ_RequestDataMgr::ShowSearchDlg()
{
	m_pSearchDlg->ShowSearchDlg();
}

INT CBO_ACQ_RequestDataMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	if(strDMFieldAlias == _T("��ġ����"))
	{
		if(str.GetLength() == 0) str = _T("�������");
		else if(str == _T("1")) str = _T("��û��");
		else if(str == _T("2")) str = _T("ó����");
		else if(str == _T("3")) str = _T("������");
		else if(str == _T("4")) str = _T("��ҵ�");
		m_pDM->SetCellData(_T("��ġ���¼���"), str, nRow);
	}
	else if(strDMFieldAlias == _T("����"))
	{
		DOUBLE dTemp;
		CString strTemp;
		strTemp = m_pDM->GetCellData(_T("����"), nRow);
		strTemp.Replace(_T(","), _T(""));
		TCHAR* pEndPtr = NULL;
		dTemp = _tcstod(strTemp.GetBuffer(0), &pEndPtr);

		str = TruncPrice(dTemp);		
		
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
	}
	else if(strDMFieldAlias == _T("UDF_��һ���"))
	{
		m_pDM->GetCellData(_T("��һ���"), nRow, str);
		str.TrimLeft();	str.TrimRight();
		str.Replace(_T("\r"), _T(" "));
		str.Replace(_T("\n"), _T(" "));
		m_pDM->SetCellData(_T("UDF_��һ���"), str, nRow);
	}
	else if(strDMFieldAlias == _T("�Ѱ���"))
	{
		DOUBLE dTemp;
		CString strTemp;
		strTemp = m_pDM->GetCellData(_T("�Ѱ���"), nRow);
		strTemp.Replace(_T(","), _T(""));
		TCHAR* pEndPtr = NULL;
		dTemp = _tcstod(strTemp.GetBuffer(0), &pEndPtr);
		
		str = TruncPrice(dTemp);		
		
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
	}
	return 0;
}

VOID CBO_ACQ_RequestDataMgr::DisplayGrid()
{
	BOOL bSetTopRow = TRUE;
	INT nCurSel = m_pGrid->GetTopRow();
	if( nCurSel < 0 )						bSetTopRow = FALSE;
	if( m_pDM->GetRowCount() == 0 )			bSetTopRow = FALSE;
	if( nCurSel > m_pDM->GetRowCount() )	bSetTopRow = FALSE;

	m_pGrid->Display();
	if( bSetTopRow == TRUE )	m_pGrid->SetTopRow(nCurSel);
}

BEGIN_EVENTSINK_MAP(CBO_ACQ_RequestDataMgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_ACQ_RequestDataMgr)
	ON_EVENT(CBO_ACQ_RequestDataMgr, IDC_gridREQUEST_DATA, -601 /* DblClick */, OnDblClickgridREQUESTDATA, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_ACQ_RequestDataMgr::OnDblClickgridREQUESTDATA() 
{
	// TODO: Add your control notification handler code here
	if(m_pGrid->GetMouseRow() == 0) return;
	if(m_pDM->GetRowCount() == 0) return ;
	ShowRequestDataDlg(GUPDATE);
}

VOID CBO_ACQ_RequestDataMgr::SetRequestDataInfo()
{
	CString strMsg;
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0)
		strMsg.Format(_T("�˻��� ��ġ����ڷᰡ �����ϴ�."));
	else
		strMsg.Format(_T("%d���� ��ġ����ڷᰡ �˻��Ǿ����ϴ�."), nRowCnt);
	
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
}

INT CBO_ACQ_RequestDataMgr::CheckSelectDataValid()
{
	if(m_pDM->GetRowCount() == 0) return -1;

	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0) return -1;
	ids = m_pGrid->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}

VOID CBO_ACQ_RequestDataMgr::InitPopupMenuItem()
{
	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();
	m_pGrid->InitPopupMenuItem();
	m_pGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
}

VOID CBO_ACQ_RequestDataMgr::DupRequestDataSearch()
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
	m_pDuplicateSearchMgr->ShowHoldDuplicateSearch(_T("��ġ����ڷẹ������"), m_pDM->DB_MGR_ALIAS, m_pCM->CM_ALIAS, _T("MainGrid"));
}

UINT CBO_ACQ_RequestDataMgr::OnShowSearchResult(LPARAM lParam, WPARAM wParam)
{
	CString strTmpData;
	if(m_pDuplicateSearchMgr)
		m_pDuplicateSearchMgr->ShowDuplicateSearchDlg();
	return 0;
}

UINT CBO_ACQ_RequestDataMgr::OnMakePrintDM(LPARAM lParam, WPARAM wParam)
{
	if(lParam == BO_ACQ_REQUEST_DATA)
	{
		INT ids ;
		CString strDivAlias = _T("��ġ���KEY");
		CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
		CESL_DataMgr* pDM = m_pDM;
		CMakePrintDMMgr PrintMgr(this);

		CESL_DataMgr pCopyDM;
		MakeCopyDM( this , m_pDM , &pCopyDM );
		pCopyDM.CopyDM(m_pDM);

		if( m_pPrintDlg->GetPrintDataType() == 2 )
		{
			ids = PrintMgr.GetGridSelectData(m_pGrid, &pCopyDM, strDivAlias, &MapDivValue);
			
			if(ids < 0)
			{
				ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
				return -1;
			}
		}

		pDM = m_pPrintDM;

		ShowDataProcessingDlg();

		PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
		PrintMgr.SetTempData(_T("����ƮŸ��"), lParam);
		PrintMgr.SetTempData(_T("���о˸��ƽ�"), strDivAlias.GetBuffer(0));
		PrintMgr.SetTempData(_T("����ڷ����Ŀ���"), m_pPrintDlg->GetDataSortYN());
		PrintMgr.SetTempData(_T("��µ�����Ÿ��"), m_pPrintDlg->GetPrintDataType());
		PrintMgr.SetTempPtr(_T("�׸��弱���ڷ�"), (DWORD*)&MapDivValue);
		PrintMgr.SetTempPtr(_T("���α׷�����Ʈ��"), (DWORD*)m_pDataProcessingDlg);		
		PrintMgr.SetTempPtr(_T("����DM"), (DWORD*)m_pDM);		
		PrintMgr.SetTempPtr(_T("�������DM"), (DWORD*)m_pPrintDM);
		PrintMgr.SetTempPtr(_T("���DM"), (DWORD*)NULL);
		PrintMgr.SetTempPtr(_T("��������"), (DWORD*)m_pSortItemSet);

		ids = PrintMgr.SPFExecutePath(_T("��ġ����ڷ����"));

		DeleteDataProcessingDlg();

		if(ids < 0)
		{
			ESLAfxMessageBox(_T("��¿� �����͸� ������ �� �����ϴ�."));
			return -1;
		}

		m_pPrintDlg->SetPrintDM(pDM);
		m_pPrintDlg->Print();

	}
	else if(lParam == BO_ACQ_REQUEST_MAIL_SEND_LIST)
	{

	}
	return 0;
}

VOID CBO_ACQ_RequestDataMgr::DupKeySetting()
{
	CDUP_KEY_SETTING Dlg(this, _T(".\\duplicate_search_key.txt"));
	Dlg.DoModal();
}

VOID CBO_ACQ_RequestDataMgr::ShowDataProcessingDlg()
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

VOID CBO_ACQ_RequestDataMgr::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

UINT CBO_ACQ_RequestDataMgr::OnDuplicateDataDelete(LPARAM lParam, WPARAM wParam)
{
	CString strMsg;
	CString strQuery;
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("��ġ����ڷḦ �������Դϴ�."));
	m_pSearchDlg->GetWhereStmt(strQuery);
	if( strQuery.IsEmpty() )
	{
		CString strKey;
		strQuery = _T(" REC_KEY IN ( ");
		for( INT i = 0 ; i < m_pDM->GetRowCount() ; i++ )
		{
			m_pDM->GetCellData(_T("��ġ���KEY"),i,strKey);
			if( !strKey.IsEmpty() )
				strQuery = strQuery +  strKey + _T(" ,");
		}
		strQuery = strQuery.Left(strQuery.GetLength() - 2);
		strQuery = strQuery + _T(" )");
	}
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	INT nIndex = m_pDM->FindColumn(_T("��ġ���_�ֹε�Ϲ�ȣ")); 
	if(-1 < nIndex)
	{
		m_pDM->GetColumnRefInfo(nIndex, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
		m_pDM->SetDataMgrRef(nIndex, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, _T("UDF"));
	}
	m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0)
	{
		DeleteDataProcessingDlg();
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		SetRequestDataInfo();
		DisplayGrid();
		return 0;
	}

	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < m_pDM->GetRowCount())
	{
		CString strGetData;
		CStringArray asDes1,asDes2,asDes3,asDes4,asDes5;
		for(INT i = 0; i < m_pDM->GetRowCount(); i++)
		{
			m_pDM->GetCellData(_T("�ڵ���"), i, strGetData);
			asDes1.Add(strGetData);
			m_pDM->GetCellData(_T("EMAIL"), i, strGetData);
			asDes2.Add(strGetData);
			m_pDM->GetCellData(_T("�ּ�"), i, strGetData);
			asDes3.Add(strGetData);
			m_pDM->GetCellData(_T("��ȭ��ȣ"), i, strGetData);
			asDes4.Add(strGetData);
			m_pDM->GetCellData(_T("��ġ���_�ֹε�Ϲ�ȣ"), i, strGetData);
			asDes5.Add(strGetData);
		}
		CBO_LOC_3000Api *pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("HANDPHONE"), asDes1);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("EMAIL"), asDes2);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("ADDRESS"), asDes3);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("TEL"), asDes4);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("CIVIL_NO"), asDes5);
					pApi->CloseSocket();	
				}
				for(i = 0; i < m_pDM->GetRowCount(); i++)
				{
					m_pDM->SetCellData(_T("�ڵ���"), asDes1.GetAt(i), i);
					m_pDM->SetCellData(_T("EMAIL"), asDes2.GetAt(i), i);
					m_pDM->SetCellData(_T("�ּ�"), asDes3.GetAt(i), i);
					m_pDM->SetCellData(_T("��ȭ��ȣ"), asDes4.GetAt(i), i);
					m_pDM->SetCellData(_T("��ġ���_�ֹε�Ϲ�ȣ"), asDes5.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
		asDes4.RemoveAll(); asDes4.FreeExtra();
		asDes5.RemoveAll(); asDes5.FreeExtra();
	}

	SetRequestDataInfo();
	DeleteDataProcessingDlg();
	DisplayGrid();
	
	return 0;
}

BOOL CBO_ACQ_RequestDataMgr::InitSortItem()
{
	INT ids ;
	CStringArray * pTmpData = NULL;	
	
	m_pSortItemSet = new CSortItemSet(this);
	m_pSortItemSet->SetGrid(m_pGrid);

	ids = m_pSortItemSet->InitDataManager(m_pDM->CONNECTION_INFO, _T("DM_�����׸���"));
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("[DM_�����׸���]�� �ʱ�ȭ �� �� �����ϴ�."));
		return FALSE;
	}

	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(), _T("MainGrid"));

	pTmpData = m_pSortItemSet->GetHeadSortList();

	CMapStringToString * pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	pMap->SetAt(_T("��ġ���¼���"), _T("��ġ����"));
	
	m_pSortItemSet->ReadSortItem();

	m_pSearchDlg->SetDataSortItem(m_pSortItemSet);

	return TRUE;
}

VOID CBO_ACQ_RequestDataMgr::ShowSortDlg(INT nType/*=1*/)
{
	if(m_pSortItemSet == NULL || m_pDM == NULL) 
	{
		ESLAfxMessageBox(_T("����ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		return;
	}
	if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	
	CDataSortDlg dlg(this);
	dlg.SetMainDM(m_pDM);
	dlg.SetSortItemSet(m_pSortItemSet);
	dlg.SetOpenMode(nType);
	if(IDOK == dlg.DoModal() && nType > 0)
		DisplayGrid();
}

UINT CBO_ACQ_RequestDataMgr::OnSetOrderCondition(LPARAM lParam, WPARAM wParam)
{
	ShowSortDlg(-1);
	return 0;
}

VOID CBO_ACQ_RequestDataMgr::ShowDataRequestExport()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}

	CBO_ACQ_FileExportMgr FileExportMgr;

	INT nSelectedRowIdx;
	CString strRowIdx;
	CStringArray SelectedRowIdxArray;	
	nSelectedRowIdx = m_pGrid->SelectGetHeadPosition();
	if(nSelectedRowIdx < 0) 
	{
		ESLAfxMessageBox(_T("������ �׸��� ��ġ�� ã�� �� �����ϴ�."));
		return ;
	}
	while(nSelectedRowIdx >= 0)
	{
		strRowIdx = m_pDM->GetCellData(_T("��ġ���KEY"), nSelectedRowIdx);
		SelectedRowIdxArray.Add(strRowIdx);
		nSelectedRowIdx = m_pGrid->SelectGetNext();
	}

	ids = FileExportMgr.SetExportColumn();
	if(ids < 0) return ;

	FileExportMgr.SetSelectList(&SelectedRowIdxArray);

	FileExportMgr.SetSpeciesType(BO_REQUEST);

	ids = FileExportMgr.FileExportProc();
	if(ids < 0) return ;

	AfxMessageBox(_T("�ڷ������ �Ϸ��Ͽ����ϴ�."));
}

VOID CBO_ACQ_RequestDataMgr::ShowDataRequestImport()
{
	CBO_ACQ_FileImportMgr FileImportMgr(this);

	FileImportMgr.SetSpeciesType(BO_REQUEST);

	INT ids;
	ids = FileImportMgr.FileImportProc();
	if(ids < 0) return ;

	AfxMessageBox(_T("�ڷ������ �Ϸ��Ͽ����ϴ�."));

}

CString CBO_ACQ_RequestDataMgr::TruncPrice(DOUBLE dPrice)
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

HBRUSH CBO_ACQ_RequestDataMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}

INT CBO_ACQ_RequestDataMgr::MakeCopyDM( CESL_Mgr *pEslMgr , CESL_DataMgr *pSourceDM , CESL_DataMgr *pCopyDM )
{
	INT ids;

 	pCopyDM->SetCONNECTION_INFO(pSourceDM->CONNECTION_INFO);
	pCopyDM->TBL_NAME = pSourceDM->TBL_NAME;

	pCopyDM->EXTRA_CONDITION = pSourceDM->EXTRA_CONDITION;
	pCopyDM->CONDITION = pSourceDM->CONDITION;
	pCopyDM->DB_MGR_ALIAS = pSourceDM->DB_MGR_ALIAS;
	

	pCopyDM->InitDataMgrRef( pSourceDM->RefList.GetCount() );
	pCopyDM->m_nCols = pSourceDM->RefList.GetCount();

	CString sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType;
	for( INT k = 0 ; k < pSourceDM->RefList.GetCount() ; k++ )
	{
		sFieldAlias = sFieldName = sFieldType = sInitValue = sDataType = _T("");
		ids = pSourceDM->GetColumnRefInfo( k , sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType );
		if( ids < 0 ) return -1;

		ids = pCopyDM->SetDataMgrRef( k , sFieldAlias , sFieldName , sFieldType  , sInitValue  , sDataType );			
		if( ids < 0 ) return -1;
	}		
	
	return 0;
}

INT CBO_ACQ_RequestDataMgr::SearchRefresh()
{
	BEGIN_SBL();

	CString strMsg;
	CString strQuery;

	m_pSearchDlg->GetWhereStmt(strQuery);
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	INT nIndex = m_pDM->FindColumn(_T("��ġ���_�ֹε�Ϲ�ȣ")); 
	if(-1 < nIndex)
	{
		m_pDM->GetColumnRefInfo(nIndex, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
		m_pDM->SetDataMgrRef(nIndex, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, _T("UDF"));
	}
	m_pDM->RefreshDataManager(strQuery);
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0)
	{
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		SetRequestDataInfo();
		DisplayGrid();
		return 0;
	}

	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < m_pDM->GetRowCount())
	{
		CString strGetData;
		CStringArray asDes1,asDes2,asDes3,asDes4,asDes5;
		for(INT i = 0; i < m_pDM->GetRowCount(); i++)
		{
			m_pDM->GetCellData(_T("�ڵ���"), i, strGetData);
			asDes1.Add(strGetData);
			m_pDM->GetCellData(_T("EMAIL"), i, strGetData);
			asDes2.Add(strGetData);
			m_pDM->GetCellData(_T("�ּ�"), i, strGetData);
			asDes3.Add(strGetData);
			m_pDM->GetCellData(_T("��ȭ��ȣ"), i, strGetData);
			asDes4.Add(strGetData);
			m_pDM->GetCellData(_T("��ġ���_�ֹε�Ϲ�ȣ"), i, strGetData);
			asDes5.Add(strGetData);
		}
		CBO_LOC_3000Api *pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("HANDPHONE"), asDes1);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("EMAIL"), asDes2);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("ADDRESS"), asDes3);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("TEL"), asDes4);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("CIVIL_NO"), asDes5);
					pApi->CloseSocket();	
				}
				for(i = 0; i < m_pDM->GetRowCount(); i++)
				{
					m_pDM->SetCellData(_T("�ڵ���"), asDes1.GetAt(i), i);
					m_pDM->SetCellData(_T("EMAIL"), asDes2.GetAt(i), i);
					m_pDM->SetCellData(_T("�ּ�"), asDes3.GetAt(i), i);
					m_pDM->SetCellData(_T("��ȭ��ȣ"), asDes4.GetAt(i), i);
					m_pDM->SetCellData(_T("��ġ���_�ֹε�Ϲ�ȣ"), asDes5.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
		asDes4.RemoveAll(); asDes4.FreeExtra();
		asDes5.RemoveAll(); asDes5.FreeExtra();
	}

	CString strValue1;
	ChangeSecuriyCivilNo(m_pDM,-1,_T("��ġ���_�ֹε�Ϲ�ȣ"));

	if(_T("N") == m_strCivilNoViewYN)
	{
		ChangeUserNo(m_pDM,_T("��ġ���_�����ڹ�ȣ"));
	}
	else
	{
		CString strTemp;
		for( INT i = 0 ; i < m_pDM->GetRowCount() ; i++ )
		{
			m_pDM->GetCellData( _T("��ġ���_�����ڹ�ȣ"), i ,strTemp);
			m_pDM->SetCellData( _T("�����ڹ�ȣ") , strTemp , i );
		}
	}

	SetRequestDataInfo();
	m_pSortItemSet->DataSort(m_pDM, !m_pSearchDlg->GetDataSortYN());
	DisplayGrid();

	INT nRowCount = m_pDM->GetRowCount();
	END_SBL(nRowCount);
	
	return 0;
}

INT CBO_ACQ_RequestDataMgr::CivilNoCheck( CString sCivilNo )
{
	EFS_BEGIN

	INT ids;
	INT nDashIndex;
	CString sCivilNo1,sCivilNo2;

	if( sCivilNo.GetLength() != 14 )
		return 1;
	
	nDashIndex = sCivilNo.Find(_T("-"));

	if( nDashIndex != 6 ) 
		return 2;

	sCivilNo1 = sCivilNo.Mid(0,nDashIndex);
	sCivilNo2 = sCivilNo.Mid(nDashIndex+1);

	ids = CivilNoCheck( sCivilNo1 , sCivilNo2 );
	if( ids != 0 ) return 3;

	return 0;


	EFS_END
	return -1;

}

INT CBO_ACQ_RequestDataMgr::CivilNoCheck( CString sCivilNo1, CString sCivilNo2 )
{
	EFS_BEGIN

	if( 6 != sCivilNo1.GetLength() || 7 != sCivilNo2.GetLength() )
	{
		return -4;
	}

	INT sum, i; 

	sum = 0;

    INT nMonth = _ttoi(sCivilNo1.Mid(2,2));
    if(nMonth < 1 || nMonth > 12)
	{
        return -1;
    }

    INT nDay = _ttoi(sCivilNo1.Mid(4,2));
    if(nMonth < 1 || nMonth > 31)
	{
        return -2;
    }
    
    if( (sCivilNo2.GetAt(0)!='1') && (sCivilNo2.GetAt(0)!= '2') && (sCivilNo2.GetAt(0)!='3') && (sCivilNo2.GetAt(0)!='4') 
		&& (sCivilNo2.GetAt(0)!= '5') && (sCivilNo2.GetAt(0)!= '6') && (sCivilNo2.GetAt(0)!= '7') && (sCivilNo2.GetAt(0)!= '8') )
	{
        return -3;
    }

	if( sCivilNo2.GetAt(0)=='5' || (sCivilNo2.GetAt(0) == '6') || (sCivilNo2.GetAt(0)== '7') || (sCivilNo2.GetAt(0)== '8'))
	{
		INT ids;
		ids = CheckForeignCivilNum( sCivilNo1 + sCivilNo2 );
		if( ids != 0 ) return -1;
	}
	else
	{
		for( i = 0 ; i < 12 ; i++) 
		{ 
			if(i < 6) 
				sum += (sCivilNo1.GetAt(i)-'0') * (i+2); 
			if( i > 5 && i < 8 ) 
				sum += (sCivilNo2.GetAt(i-6)-'0') * (i+2); 
			
			if( i > 7 ) 
				sum += (sCivilNo2.GetAt(i-6)-'0') * (i-6); 
		} 
		
		INT nCheckDigit = 11-(sum%11);
		nCheckDigit = nCheckDigit%10;
		if( nCheckDigit != (sCivilNo2.GetAt(6)-'0') ) 
			return -1;
	}
	
	return 0;

	EFS_END
	return -1;

}

INT CBO_ACQ_RequestDataMgr::CheckForeignCivilNum( CString civilNum )
{	
	const INT cnt = 13;
	INT num[ cnt ];
	INT sum = 0, odd = 0;
	for( INT i = 0; i < cnt; i++ )
		num[ i ] = _ttoi( civilNum.Mid( i, 1 ) );
	
	odd = num[ 7 ] * 10 + num[ 8 ];
	
	if( odd % 2 != 0 )
		return 1;
	
	if( 
		num[ 11 ] != 6 && 
		num[ 11 ] != 7 && 
		num[ 11 ] != 8 &&
		num[ 11 ] != 9 )
		return 2;
	
	INT checkNum[] = { 2, 3, 4, 5, 6, 7, 8, 9, 2, 3, 4, 5 };
	
	for( i = 0, sum = 0; i < 12; i++ )
		sum += ( num[ i ] *= checkNum[ i ] );
	sum = 11 - ( sum % 11 );
	
	if( sum >= 10 )
		sum -= 10;
	sum += 2;
	
	if( sum >= 10 )
		sum -= 10;
	
	if( sum != num[ 12 ] )
		return 3;
	
	return 0;
}

INT CBO_ACQ_RequestDataMgr::ChangeSecuriyCivilNo( CESL_DataMgr* pDM , INT nIndex , CString sAlias )
{
	EFS_BEGIN

	INT ids;

	CString sCivilNo;
	INT nStartIndex = 0;
	INT nFinishIndex = pDM->GetRowCount();

	CString strConvertAlias;
	strConvertAlias = sAlias;
	strConvertAlias.Replace(_T("��ġ���_"),_T(""));
	
	if( nIndex != -1 )
	{
		nStartIndex = nIndex;
		nFinishIndex = nIndex + 1;
	}

	for( INT i = nStartIndex ; i < nFinishIndex ; i++ )
	{
		ids = pDM->GetCellData( sAlias , i , sCivilNo );

		CLocCommonAPI::ChangeSecuriyCivilNo(this,sCivilNo,m_sIsUnityLoanService);

		pDM->SetCellData( strConvertAlias , sCivilNo , i );	
	}

	return 0;

	EFS_END

	return -1;
}

INT CBO_ACQ_RequestDataMgr::ChangeSecuriyCivilNo( CString &sCivilNo )
{
	EFS_BEGIN
	
	CLocCommonAPI::ChangeSecuriyCivilNo(this,sCivilNo,m_sIsUnityLoanService);

	return 0;

	EFS_END

	return -1;

}

INT CBO_ACQ_RequestDataMgr::ChangeUserNo( CESL_DataMgr* pDM,CString sAlias)
{

	EFS_BEGIN

	INT nCnt = pDM->GetRowCount();
	CString strUserNo;

	CString strConvertAlias;
	strConvertAlias = sAlias;
	strConvertAlias.Replace(_T("��ġ���_"),_T(""));

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		INT ids = pDM->GetCellData( sAlias , i , strUserNo );

		CLocCommonAPI::ChangeSecuriyUserNo( this , strUserNo , m_sIsUnityLoanService );
		pDM->SetCellData( strConvertAlias , strUserNo , i );
	}

	return 0;

	EFS_END

	return -1;
}

INT CBO_ACQ_RequestDataMgr::ChangeUserNo( CString& strUserNo )
{
	EFS_BEGIN

	CLocCommonAPI::ChangeSecuriyUserNo( this , strUserNo , m_sIsUnityLoanService );

	return 0;

	EFS_END

	return -1;
}

void CBO_ACQ_RequestDataMgr::ShowDlg_Batch_Modify()
{
	CDLG_BATCH_MODIFY	dlg(this);
	dlg.DoModal();
}

VOID CBO_ACQ_RequestDataMgr::RequestStatistic(){
	
	CRequest_StatisticDlg dlg;
		
	dlg.DoModal();
}