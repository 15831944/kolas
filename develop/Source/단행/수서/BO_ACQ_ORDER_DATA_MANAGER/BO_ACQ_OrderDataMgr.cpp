// BO_ACQ_OrderDataMgr.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_OrderDataMgr.h"
#include "BO_ACQ_CheckInDlg.h"
#include "OrderDataCancelDlg.h"
#include "OrderCancelDivnoSelectDlg.h"
#include "OrderDataPrintDlg.h"
#include "DataExportDlg.h"
#include "../BO_ACQ_STATISTICS/BO_Purchase_Statistics.h"
#include "../../../esl/kolas3_main/kolas3_mainapi.h"
#include "../../../����/BO_ACQ_GROUP_MANAGER/DivisionNo_Select_Dlg.h"
#include "../../../����/BO_ACQ_COMMON/OrderMailMgr.h"
#include "../../../����/BO_ACQ_COMMON/ViewTransferDataDlg.h"
#include "../../../����/BO_ACQ_PRINT_MGR/MakePrintDMMgr.h"
// 2004.10.29 
#include "../BO_ACQ_FILE_IMPORT_EXPORT/BO_ACQ_FileImportMgr.h"
#include "../BO_ACQ_FILE_IMPORT_EXPORT/BO_ACQ_FileExportMgr.h"

#include "../../../����/CMarcChanger/MARC_CHANGER_01.h"
#include "sbl.h"

//==============================================
//2008.07.11 ADD BY PJW : �����ϴܿ� ���¹�LOG���
#include "SBL.H"
//==============================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_OrderDataMgr dialog

CBO_ACQ_OrderDataMgr::CBO_ACQ_OrderDataMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_OrderDataMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_OrderDataMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nRow		= -1;
	m_nEditCol	= -1;	
	m_bCellEditing	= TRUE;
	m_strDivNo		= _T("");
	m_strAcqYear	= _T("");
	m_strPosNumber	= _T("");
	m_strIs5163		= _T("");
	m_strIs64		= _T("");
	m_strOrderPrice_Unit = _T("");

	m_pCM				 = NULL;
	m_pDM = NULL;
	m_pActiveDM = NULL;
	m_pPrintDM_Species = NULL;
	m_pPrintDM_Volume = NULL;
	m_pSpeciesPrintDM	 = NULL;	
	m_pGrid_Species = NULL;
	m_pGrid_Volume = NULL;
	m_pSearchDlg		 = NULL;
	m_pResultSearchDlg	 = NULL;
	m_pPrintDlg = NULL;
	m_pDataProcessingDlg = NULL;
	m_pSortItemSet = NULL;

}
CBO_ACQ_OrderDataMgr::~CBO_ACQ_OrderDataMgr()
{
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	if(m_pResultSearchDlg)
	{
		delete m_pResultSearchDlg;
		m_pResultSearchDlg = NULL;
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
	if(m_pSortItemSet)
	{
		delete m_pSortItemSet;
		m_pSortItemSet = NULL;
	}
}

VOID CBO_ACQ_OrderDataMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_OrderDataMgr)
	DDX_Control(pDX, IDC_tabORDER_DATA, m_tabOrderData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_OrderDataMgr, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_OrderDataMgr)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnRESULT_SEARCH, OnbtnRESULTSEARCH)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabORDER_DATA, OnSelchangetabORDERDATA)
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_MESSAGE(MAKE_PRINT_DM, OnMakePrintDM)	
	ON_MESSAGE(SET_PRINT_DATA_ORDER_CONDITION, OnSetOrderCondition)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_OrderDataMgr message handlers

VOID CBO_ACQ_OrderDataMgr::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

/////////////////////////////////////////////////////////////////////////////
// Ŭ���� �ʱ�ȭ
//
// [ �̴Ͻ��� ]
// 1. SM �ʱ�ȭ 
// 2. DM �ʱ�ȭ
// 3. CM �ʱ�ȭ
// 4. �켱�����ڷ� ��뿩�� ���� ���� ���� �޾ƿ´�.
// 5. ���� ������� �������� ������ ��������� �����Ѵ�.
// 6. ���̾˷α� ���� ���ҽ����� �ʱ�ȭ �Ѵ�.
// 7. CM���� ���� ������/������ �׸��� ��Ʈ�� �����͸� �����´�.
// 8. ������ �׸��带 ùȭ�鿡 Ȱ��ȭ��Ű��, ������ �׸���� �����.
// 9. �˻�â �� ��¹�â�� ��������� ����Ѵ�.
// 10. ������ �����׸��� �˻��� ���ĵǵ��� �˻�Ŭ������ �����Ѵ�.
// 11. ��ũ��ȯ���� Ŭ������ ��ũ���� Ŭ������ ����Ѵ�.
// 12. ��,��,å���� ���� �޽����� �˷��ְ�
//	   [CM_�ֹ��ڷ����]�� �׷��ȣ�� ���Գ⵵�� �����Ѵ�.
// 15. ��/������ �׸����� �˾��޴� ���� 
// 16. ���ٹ�ư�� �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////
BOOL CBO_ACQ_OrderDataMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//=======================================================================
	// ���� ����	
	//=======================================================================
	CString strPriorityCatalogYN, strValue;

	CString strErrMsg = _T("");
	INT ids = 0 ;
		

	//=======================================================================
	// 1. SM �ʱ�ȭ 
	//=======================================================================
	if (InitESL_Mgr(_T("K2UPGRADE_�ֹ��ڷ����")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	//=======================================================================
	// 2. DM �ʱ�ȭ
	//		- �ʱ�ȭ �� SM�� ���� DM�� ã�´�.
	//=======================================================================
	m_pDM				= FindDM(_T("DM_�ֹ��ڷ����")			);
	m_pPrintDM_Species	= FindDM(_T("DM_�ֹ��ڷ����_��")		);
	m_pPrintDM_Volume	= FindDM(_T("DM_�ֹ��ڷ����_��")		);
	m_pSpeciesPrintDM = FindDM(_T("DM_�ֹ��ڷ����_��_��¿�"));
	
	if( m_pDM == NULL || m_pPrintDM_Species == NULL || m_pPrintDM_Volume == NULL || m_pSpeciesPrintDM == NULL)
	{
		ESLAfxMessageBox(_T("DM �ʱ�ȭ ����\n [ DM_�ֹ��ڷ���� ]\n [ DM_�ֹ��ڷ����_�� ]\n [ DM_�ֹ��ڷ����_�� ]\n [ DM_�ֹ��ڷ����_��_��¿� ]\n [ SM=K2UPGRADE_�ֹ��ڷ���� ]"));
		return FALSE;
	}

	//=======================================================================		
	// 3. CM �ʱ�ȭ
	//=======================================================================		
	m_pCM = FindCM(_T("CM_�ֹ��ڷ����"));
	if( m_pCM == NULL ) 
	{
		ESLAfxMessageBox( _T("CM_�ֹ��ڷ���� error") );
		return FALSE;
	}
	
	
	//=======================================================================
	// 4. �켱�����ڷ� ��뿩�� ���� ���� ���� �޾ƿ´�.
	//
	// 2004.11.16 �켱�����ڷ� ��뿩�ο� ���� CM�� �ٸ� �ɷ� �����Ͽ� ����Ѵ�.
	//=======================================================================	
	ids = m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'PRIORITYCATALOG'"), 	strPriorityCatalogYN);
	if( ids < 0 )
	{
		switch (ids)
		{
		case -4004:	strErrMsg.Format(_T("[ %d ] : �����ͺ��̽��� ���������� �ʽ��ϴ�.\nSELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'PRIORITYCATALOG'"), ids );
			break;
		case -4005:	strErrMsg.Format(_T("[ %d ] : ���ǹ�(Query)�� ���� ����� ������ �ֽ��ϴ�.\nSELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'PRIORITYCATALOG'"), ids );
			break;
		case -4006:	strErrMsg.Format(_T("[ %d ] : ���ǹ�(Query)�� ���� ����� �ʹ� �����ϴ�.\nSELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'PRIORITYCATALOG'"), ids );
			break;
		case -4007:	strErrMsg.Format(_T("[ %d ] : ���ǹ�(Query)�� ���� ����� �����ϴ�.\nSELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'PRIORITYCATALOG'"), ids );
			break;
		case -4008:	strErrMsg.Format(_T("[ %d ] : �˻��ʵ尡 ����ʵ庸�� �����ϴ�.\nSELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'PRIORITYCATALOG'"), ids );
			break;
		case -4999:	strErrMsg.Format(_T("[ %d ] : Oracle�� ���������� Ȯ���Ͻʽÿ�.\nSELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'PRIORITYCATALOG'"), ids );
			break;
		default:	strErrMsg.Format(_T("[ %d ] : Unknown Error"), ids);
			break;
		}
		ESLAfxMessageBox(strErrMsg);
	}
	
	//=======================================================================
	// 5. ���� ������� �������� ������ ��������� �����Ѵ�.
	//		- 0 : ������ ����
	//		- 1 : �Ҽ��� 1�ڸ� ���� ����
	//		- 2 : �Ҽ��� 2�ڸ� ���� ���� [�⺻��]
	//
	// 2005-01-13 ���� �ڸ��� �������� �����´�.
	//=======================================================================		
	ids = GetManageValue(_T("����"), _T("����"), _T("���������������"), _T(""), m_strPosNumber);
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("������������������� �ҷ����� ���߽��ϴ�."));
	}
	
	// ���ְ��ݴ��������� �����´�. - ����Ʈ 0������
	ids = GetManageValue(_T("����"), _T("����"), _T("���԰��ݴ�������"), _T(""), m_strOrderPrice_Unit);
	if( ids < 0 )		m_strOrderPrice_Unit = _T("0");
	

	//=======================================================================
	// 6. 5163, �������� �Ǻ����� ��������� �����Ѵ�.
	//=======================================================================
	GetManageValue(_T("��Ÿ"), _T("����"), _T("5163"), _T(""), m_strIs5163);
	GetManageValue(_T("��Ÿ"), _T("����"), _T("����"), _T(""), m_strIs64);
	
	
	//=======================================================================
	// 7. ���̾˷α� ���� ���ҽ����� �ʱ�ȭ �Ѵ�.
	//		- ����Ʈ�ڽ� �� ��ư�� �����.
	//		- ����Ʈ���� �ʱ�ȭ�Ѵ�.
	//=======================================================================
	GetDlgItem(IDC_stcORDER_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtORDER_DIVNO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_stcORDER_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtORDER_GROUP_CREATE_YEAR)->ShowWindow(SW_HIDE);;	
	GetDlgItem(IDC_btnRESULT_SEARCH)->ShowWindow(SW_HIDE);	
	m_tabOrderData.InsertItem(0, _T("������"));
	m_tabOrderData.InsertItem(1, _T("������"));
	m_tabOrderData.SetCurSel(1);

	//=======================================================================
	// 8. CM���� ���� ������/������ �׸��� ��Ʈ�� �����͸� �����´�.
	//		- 3,4 ***
	//=======================================================================
	m_pGrid_Species = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid_Volume = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid_Volume"));
	m_pGrid_Species->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	m_pGrid_Volume->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;


	//=======================================================================
	// 9. ������ �׸��带 ùȭ�鿡 Ȱ��ȭ��Ű��, ������ �׸���� �����.
	//		- �ʱ� �⺻���� �������� Ȱ��ȭ DM�̴�.
	// 2004.10.15 ȭ��� ActiveDM �ٲ�.
	//=======================================================================
	m_pGrid_Species->ShowWindow(SW_HIDE);
	m_pGrid_Volume->ShowWindow(SW_SHOW);
	m_pActiveDM = m_pPrintDM_Volume;


	//=======================================================================
	// 10. �˻�â �� ��¹�â�� ��������� ����Ѵ�.
	//		- GroupCollection ***
	//=======================================================================
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	m_pSearchDlg = new CSearchMgr(this, BO_ACQ_ORDER, PURCHASE, BO_ORDER, m_pDM->CONNECTION_INFO);
	m_pPrintDlg = new CBO_ACQ_PrintDlg(BO_ORDER, PURCHASE, this);
	m_pPrintDlg->SetGroupCollection(m_pSearchDlg->GetGroupCollection());


	//=======================================================================
	// 11. ������ �����׸��� �˻��� ���ĵǵ��� �˻�Ŭ������ �����Ѵ�.
	//		- ����Ŭ������ ���� ���ǵ� ���� ��������� ���´�.
	//=======================================================================
	InitSortItem(m_pGrid_Volume, _T("MainGrid_Volume"));
	
	
	//=======================================================================
	// 12. ��ũ��ȯ���� Ŭ������ ��ũ���� Ŭ������ ����Ѵ�.
	//=======================================================================
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);


	//=======================================================================
	// 13. ��,��,å���� ���� �޽����� �˷��ְ�
	//	   [CM_�ֹ��ڷ����]�� �׷��ȣ�� ���Գ⵵�� �����Ѵ�.
	//=======================================================================
	SetOrderDataInfo(m_tabOrderData.GetCurSel());
	
	//=======================================================================	
	// 16. ��/������ �׸����� �˾��޴� ���� 
	//     �������׸����� _T("��ǰå��") ������ �׸���󿡼� �����ϵ��� ����
	//=======================================================================	
	InitPopupMenuItem();


	//=======================================================================	
	// 17. ���ٹ�ư�� �����Ѵ�.
	//=======================================================================	
	SetToolBarBtnByTabSelect();

	return TRUE;  
}

VOID CBO_ACQ_OrderDataMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_tabORDER_DATA);
	if(pTab->GetSafeHwnd() == NULL) return;
	pTab->MoveWindow(0, 0, cx+4, cy);

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridORDER_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	
	//==========================================
	//20080604 UPDATE BY PJW : ��ũ�ѹ� ������ �ϱ����켭 ���α��� ����
	//pGrid->MoveWindow(-2, 60, cx+2, cy);
	pGrid->MoveWindow(-2, 60, cx+2, cy-60);		
	//==========================================
	

	pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridORDER_VOLUME_DATA);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//==========================================
	//20080604 UPDATE BY PJW : ��ũ�ѹ� ������ �ϱ����켭 ���α��� ����
	//pGrid->MoveWindow(-2, 60, cx+2, cy);
	pGrid->MoveWindow(-2, 60, cx+2, cy-60);		
	//==========================================

	CRect rect;
	CButton* pBtn = (CButton*)GetDlgItem(IDC_stcORDER_GROUP_CREATE_YEAR);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(20, 35, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_edtORDER_GROUP_CREATE_YEAR);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(80, 32, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);
	
	pBtn = (CButton*)GetDlgItem(IDC_stcORDER_DIVNO);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(165, 35, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);

	pEdt = (CEdit*)GetDlgItem(IDC_edtORDER_DIVNO);
	pEdt->GetWindowRect(&rect);
	pEdt->MoveWindow(225, 32, rect.Width(), rect.Height());
	pEdt->ShowWindow(SW_SHOW);

	pBtn = (CButton*)GetDlgItem(IDC_btnRESULT_SEARCH);
	pBtn->GetWindowRect(&rect);
	pBtn->MoveWindow(400, 32, rect.Width(), rect.Height());
	pBtn->ShowWindow(SW_SHOW);
}

INT CBO_ACQ_OrderDataMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	INT nGroupNo, nSerialNo, nWorkNo, nOrderSerialNo, ids;	
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strTmpData, strWorkCode, strOrderSerialNo;

	TCHAR* pEndPtr = NULL;

	if(m_nRow != nRow)
	{
		CMarc marc;
		strTmpData = m_pActiveDM->GetCellData(_T("BS_MARC"), nRow);
		ids = m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
		if(ids < 0) return -1;
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
		if(ids >= 0) 
		{
			m_pActiveDM->SetCellData(_T("UDF_�Ѽ�����"), strTmpData, nRow);
			m_pDM->SetCellData(_T("UDF_�Ѽ�����"), strTmpData, nRow);//lks
		}
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetEditionStatement(&marc, strTmpData);
		if(ids >= 0) 
		{
			m_pActiveDM->SetCellData(_T("IBS_�ǻ���"), strTmpData, nRow);
		}
		strTmpData.Empty();	 strTmpData = m_pActiveDM->GetCellData(_T("IBS_��ǥ��"), nRow);
		ids = m_MarcConvertMgr.GetTitleStatement(&marc, strTmpData);
		if(ids >= 0) 
		{
			m_pActiveDM->SetCellData(_T("UDF_ǥ������"), strTmpData, nRow);
			m_pDM->SetCellData(_T("UDF_ǥ������"), strTmpData, nRow);//lks
		}
		ids = m_MarcConvertMgr.GetPublishPlace(&marc, strTmpData);
		if(ids >= 0) 
		{
			m_pActiveDM->SetCellData(_T("UDF_������"), strTmpData, nRow);
			m_pDM->SetCellData(_T("UDF_������"), strTmpData, nRow);//lks
		}
		m_nRow = nRow;
		if(m_nRow == m_pActiveDM->GetRowCount()-1) m_nRow = -1;
	}

	if( strDMFieldAlias == _T("UDF_��������") )
	{	
		strAcqYear = m_pActiveDM->GetCellData(_T("BP_���Գ⵵"), nRow);
		strWorkCode = m_pActiveDM->GetCellData(_T("IBS_��������"), nRow);
		strGroupNo = m_pActiveDM->GetCellData(_T("BP_�׷��ȣ"), nRow);
		nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
		strWorkNo = m_pActiveDM->GetCellData(_T("BP_�۾���ȣ"), nRow);
		nWorkNo = _ttoi(strWorkNo.GetBuffer(0));
		strSerialNo = m_pActiveDM->GetCellData(_T("BP_�Ϸù�ȣ"), nRow);
		nSerialNo = _ttoi(strSerialNo.GetBuffer(0));
		strOrderSerialNo = m_pActiveDM->GetCellData(_T("BP_�����Ϸù�ȣ"), nRow);
		nOrderSerialNo = _ttoi(strOrderSerialNo.GetBuffer(0));
		str.Format(_T("%s-%s-%04d-%03d-%05d"), strAcqYear, strWorkCode, nGroupNo, nWorkNo, nSerialNo);// = strAcqYear+_T("-")+strGroupNo+_T("-")+strWorkNo+_T("-")+strSerialNo;
		m_pActiveDM->SetCellData(strDMFieldAlias, str, nRow);
		strTmpData.Format(_T("%04d-%05d"), nGroupNo, nOrderSerialNo);
		m_pActiveDM->SetCellData(_T("UDF_�����������"), strTmpData, nRow);
		m_pDM->SetCellData(_T("UDF_��������"), strTmpData, nRow);//lks
	}
	else if( strDMFieldAlias == _T("UDF_�Է±��м���") )
	{
		strTmpData = m_pActiveDM->GetCellData(_T("BP_�Է±���"), nRow);
		if(strTmpData == _T("0")) str = _T("ȭ���Է�");
		else if(strTmpData == _T("1")) str = _T("��Ȱ�����");
		else if(strTmpData == _T("2")) str = _T("����ڷ����");
		else if(strTmpData == _T("3")) str = _T("���Ϲ���");
		m_pActiveDM->SetCellData(strDMFieldAlias, str, nRow);
		m_pDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if( strDMFieldAlias == _T("BP_��������") )
	{
		strTmpData = m_pActiveDM->GetCellData(_T("IBS_�з�ǥ����"), nRow);
		
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
		else //if(strTmpData.IsEmpty()) 
		{
			str = _T("");
		}

		m_pActiveDM->SetCellData(_T("UDF_�������м���"), str, nRow);
		m_pDM->SetCellData(_T("UDF_�������м���"), str, nRow);//lks
	}
	else if( strDMFieldAlias == _T("UDF_�������м���") )
	{
		str = m_pActiveDM->GetCellData(_T("BP_��������"), nRow);
		strTmpData = m_pActiveDM->GetCellData(_T("IBS_�з�ǥ����"), nRow);
		
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
		else //if(strTmpData.IsEmpty()) 
		{
			str = _T("");
		}

		m_pActiveDM->SetCellData(_T("UDF_�������м���"), str, nRow);
		m_pDM->SetCellData(_T("UDF_�������м���"), str, nRow);//lks
	}
	else if( strDMFieldAlias == _T("UDF_����ó��ȣ����") )
	{
		strTmpData = m_pActiveDM->GetCellData(_T("BP_����ó��ȣ"), nRow);
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("����ó����"), strTmpData, str);
		m_pActiveDM->SetCellData(strDMFieldAlias, str, nRow);
		m_pDM->SetCellData(strDMFieldAlias, str, nRow);//lks
	}
	// �ǹ��ְ��ݼ���
	else if( strDMFieldAlias == _T("UDF_���ΰ�") )
	{
		//20070830 ADD PJW : �������� ���濡 ���� ���ְ���, ���ΰ��� DM�� ���Ѵ�.��
		CString strBookPrice;
		CString strPriceDiscountRate;
		CString strExchangeRate;
		DOUBLE dBookPrice;
		DOUBLE dExchangeRate;
		DOUBLE dPriceDiscountRate;
		DOUBLE dOrderPRICE;

		strBookPrice = m_pActiveDM->GetCellData(_T("UDF_��ȯ����"), nRow);
		strBookPrice.Replace(_T(","), _T(""));

		strPriceDiscountRate = m_pActiveDM->GetCellData(_T("BP_������"), nRow);
		strExchangeRate = m_pActiveDM->GetCellData(_T("BP_ȯ��"), nRow);
		//dBookPrice = atof(strBookPrice);
		dBookPrice = _tcstod(strBookPrice.GetBuffer(0), &pEndPtr);

		//dExchangeRate = atof(strExchangeRate);
		dExchangeRate = _tcstod(strExchangeRate.GetBuffer(0), &pEndPtr);

		//dPriceDiscountRate = atof(strPriceDiscountRate);
		dPriceDiscountRate = _tcstod(strPriceDiscountRate.GetBuffer(0), &pEndPtr);

		dOrderPRICE = dBookPrice * dExchangeRate * ( ( 100.0-dPriceDiscountRate )/100.0 );
		strTmpData.Format(_T("%.2f"), dOrderPRICE);
		m_pActiveDM->SetCellData(_T("UDF_���ְ���_��"), strTmpData, nRow);
		m_pActiveDM->SetCellData(_T("UDF_���ΰ�"), strTmpData, nRow);
		//

		DOUBLE dPrice;
		INT	ids;
		ids = m_pActiveDM->GetCellData(_T("UDF_���ְ���_��"), nRow, strTmpData);
		if ( strTmpData == _T("") ) ids = m_pActiveDM->GetCellData(_T("UDF_���ΰ�"), nRow, strTmpData);
		strTmpData.Replace(_T(","), _T(""));
		//dPrice = (DOUBLE)atof(strTmpData);
		dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		//2007.05.04 PJW ���� ��ȭ�ϰ�� �Ҽ��� ����
		if( _T("\\") ==  m_pActiveDM->GetCellData(_T("BP_ȭ�󱸺�"), nRow) )
		{
			strTmpData.Format(_T("%d"), (INT)dPrice);
		}
		else
		{
			if( dPrice == 0 )	strTmpData = _T("0");
			else
			{
				if		( m_strOrderPrice_Unit == _T("1") )	strTmpData.Format(_T("%.1f"), dPrice);
				else if	( m_strOrderPrice_Unit == _T("2") )	strTmpData.Format(_T("%.2f"), dPrice);
				else										strTmpData.Format(_T("%.0f"), dPrice);
			}
		}
		/*
		if( dPrice == 0 )	strTmpData = _T("0");
		else
		{
			if		( m_strOrderPrice_Unit == _T("1") )	strTmpData.Format(_T("%.1f"), dPrice);
			else if	( m_strOrderPrice_Unit == _T("2") )	strTmpData.Format(_T("%.2f"), dPrice);
			else										strTmpData.Format(_T("%.0f"), dPrice);
		}
		*/
		

		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, str);
		m_pActiveDM->SetCellData(strDMFieldAlias, strTmpData, nRow);
		m_pActiveDM->SetCellData(_T("UDF_��ȯ���ΰ���"), str, nRow);
	}
	else if( strDMFieldAlias == _T("UDF_����") )
	{
		INT nBookCnt = 0;
		DOUBLE dPrice = 0.0;
		strTmpData = m_pActiveDM->GetCellData(_T("BB_����"), nRow);
		//dPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
		dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		strTmpData = m_pActiveDM->GetCellData(_T("UDF_å��"), nRow);
		nBookCnt = _ttoi(strTmpData.GetBuffer(0));
		//2007.05.04 PJW ���� ��ȭ�ϰ�� �Ҽ��� ����
		if( _T("\\") ==  m_pActiveDM->GetCellData(_T("BP_ȭ�󱸺�"), nRow) )
		{
			dPrice = dPrice*nBookCnt;
			strTmpData.Format(_T("%d"), (INT)dPrice);
		}
		else
		{
			strTmpData = TruncPrice(m_pActiveDM, dPrice*nBookCnt);
		}
		// 2004.12.03
		//strTmpData = TruncPrice(m_pActiveDM, dPrice*nBookCnt);

		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, str);
		m_pActiveDM->SetCellData(_T("UDF_��ȯ����"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_������"))
	{
		// 2004.09.14  ȭ�󱸺а��� �Ҽ��� 2�ڸ����� ��Ÿ����.
		str.Replace(_T(","), _T(""));
		//2007.05.04 PJW ���� ��ȭ�ϰ�� �Ҽ��� ����
		if( _T("\\") ==  m_pActiveDM->GetCellData(_T("BP_ȭ�󱸺�"), nRow) )
		{
			str.Format(_T("%d"), (INT)_ttoi(str));
			m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
			//str.Replace(_T(","), _T(""));
		}
		else
		{
			//str = TruncPrice(m_pActiveDM, (DOUBLE)atof(str));
			str = TruncPrice(m_pActiveDM, _tcstod(str.GetBuffer(0), &pEndPtr));

			m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
			str.Replace(_T(",."), _T("."));
		}

		//str = TruncPrice(m_pActiveDM, (DOUBLE)atof(str));
		//m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		// 2004.09.14
		//str.Replace(_T(",."), _T("."));
		
		m_pActiveDM->SetCellData(_T("UDF_��ȯ������"), str, nRow);
	}
	// �����ְ��� ����
	else if( strDMFieldAlias == _T("UDF_����������") )
	{
		//20070830 ADD PJW : �������� ���濡 ���� ���ְ���, ������������ DM�� ���Ѵ�.
		CString strSpeciesPrice;
		CString strPriceDiscountRate;
		CString strExchangeRate;
		CString strMoney;
		CString strVolRowCnt;
		CString strSpeciesKey;

		DOUBLE dSpeciesPrice;
		DOUBLE dExchangeRate;
		DOUBLE dPriceDiscountRate;
		DOUBLE dOrderPRICE;
		INT iBookRowCnt;

		iBookRowCnt = m_pDM->GetRowCount(); 
		strSpeciesPrice = m_pActiveDM->GetCellData( _T("UDF_������"), nRow );
		strPriceDiscountRate = m_pActiveDM->GetCellData( _T("BP_������"), nRow );
		strExchangeRate = m_pActiveDM->GetCellData( _T("BP_ȯ��"), nRow );
		strMoney = m_pActiveDM->GetCellData( _T("BP_ȭ�󱸺�"), nRow );
		strSpeciesKey = m_pActiveDM->GetCellData( _T("IBS_��KEY"), nRow );

		for( INT i = 0 ; i < iBookRowCnt ; i++ )
		{
			if( strSpeciesKey == m_pDM->GetCellData( _T("IBS_��KEY"), i ) )
			{
				m_pDM->SetCellData(_T("BP_ȯ��"), strExchangeRate , i);
				m_pDM->SetCellData(_T("BP_������"), strPriceDiscountRate , i);
				m_pDM->SetCellData(_T("BP_ȭ�󱸺�"), strMoney, i);
			}
		}

		//dSpeciesPrice = atof(strSpeciesPrice);
		dSpeciesPrice = _tcstod(strSpeciesPrice.GetBuffer(0), &pEndPtr);

		//dExchangeRate = atof(strExchangeRate);
		dExchangeRate = _tcstod(strExchangeRate.GetBuffer(0), &pEndPtr);

		//dPriceDiscountRate = atof(strPriceDiscountRate);
		dPriceDiscountRate = _tcstod(strPriceDiscountRate.GetBuffer(0), &pEndPtr);

		dOrderPRICE = dSpeciesPrice * dExchangeRate * ( ( 100.0-dPriceDiscountRate )/100.0 );
		strTmpData.Format(_T("%.2f"), dOrderPRICE);
		m_pActiveDM->SetCellData(_T("BP_���ְ���"), strTmpData, nRow);
		m_pActiveDM->SetCellData(_T("UDF_����������"), strTmpData, nRow);
		//

		DOUBLE dPrice;
		strTmpData = m_pActiveDM->GetCellData(_T("BP_���ְ���"), nRow);
		//dPrice = (DOUBLE)atof(strTmpData);
		dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		//2007.05.04 PJW ���� ��ȭ�ϰ�� �Ҽ��� ����
		if( _T("\\") ==  strMoney )
		{
			strTmpData.Format(_T("%d"), (INT)dPrice);
		}
		else
		{
			if( dPrice == 0 )	strTmpData = _T("0");
			else
			{
				if		( m_strOrderPrice_Unit == _T("1") )	strTmpData.Format(_T("%.1f"), dPrice);
				else if	( m_strOrderPrice_Unit == _T("2") )	strTmpData.Format(_T("%.2f"), dPrice);
				else										strTmpData.Format(_T("%.0f"), dPrice);
			}
		}
		/*
		if( dPrice == 0 )	strTmpData = _T("0");
		else
		{
			if( m_strOrderPrice_Unit == _T("1") )		strTmpData.Format(_T("%.1f"), dPrice);
			else if( m_strOrderPrice_Unit == _T("2") )	strTmpData.Format(_T("%.2f"), dPrice);
			else										strTmpData.Format(_T("%.0f"), dPrice);
		}*/
		
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData, str);
		m_pActiveDM->SetCellData(strDMFieldAlias, strTmpData, nRow);
		m_pActiveDM->SetCellData(_T("UDF_��ȯ����������"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_�ܰ�"))
	{
		// 2004.09.14  ȭ�󱸺а��� �Ҽ��� 2�ڸ����� ��Ÿ����.
		str.Replace(_T(","), _T(""));
		if( _T("\\") ==  m_pActiveDM->GetCellData(_T("BP_ȭ�󱸺�"), nRow) )
		{
			str.Format(_T("%d"), (INT)_ttoi(str));
		}
		else
		{
			//str = TruncPrice(m_pActiveDM, (DOUBLE)atof(str));	
			str = TruncPrice(m_pActiveDM, _tcstod(str.GetBuffer(0), &pEndPtr));	
		}
		//str = TruncPrice(m_pActiveDM, (DOUBLE)atof(str));		
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		m_pActiveDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	return 0;
}

VOID CBO_ACQ_OrderDataMgr::DisplayGrid()
{
	INT nCurSel;
	BOOL bSetTopRow = TRUE;
	nCurSel = GetActiveGrid()->GetTopRow();

	if( nCurSel < 0 )								bSetTopRow = FALSE;
	if( GetActiveDM()->GetRowCount() == 0 )			bSetTopRow = FALSE;
	if( nCurSel > GetActiveDM()->GetRowCount() )	bSetTopRow = FALSE;

	GetActiveGrid()->Display();

	if( bSetTopRow == TRUE )	GetActiveGrid()->SetTopRow(nCurSel);
}


//###########################################################################
//
// [ �Լ����� ]
// - ��,å,�Ǽ��� ���� �޽����� �����ְ�, 
//	 [CM_�ֹ��ڷ����]�� �׷��ȣ�� ���Գ⵵�� �����Ѵ�.
//
// [ �̴Ͻ��� ] 
// 1. å���� 0���� ũ��, ������ 0�̶�� å����ŭ ������ �����Ѵ�.
// 2. å���� 0���� ũ��, �Ǽ��� 0�̶�� å����ŭ �Ǽ��� �����Ѵ�.
// 3. �˻��� �ڷῡ ���� �޽����� �ۼ��Ͽ� ������. (�޽������� == 1,2�� ����)
// 4. �˻�Ŭ������ ����� �׷���� Ŭ�������� ���ε� �׷��ȣ ���� �����´�.
// 5. ������ �׷��ȣ�� CM�� �����Ѵ�.
// 6. ���Գ⵵ ���� �˻�Ŭ������ ����� �׷���� Ŭ�������� �������Ŀ�
//	 CM�� �����Ѵ�.
//
//###########################################################################
VOID CBO_ACQ_OrderDataMgr::SetOrderDataInfo(INT nSelect)
{
	//=======================================================================
	// ��������
	//=======================================================================
	CString strMsg;
	INT nRowCnt = m_pPrintDM_Species->GetRowCount();
	INT nVolCnt = m_pPrintDM_Volume->GetRowCount();
	INT nBookCnt = m_pDM->GetRowCount();
	INT ids = 0;

	//=======================================================================
	// 1. å���� 0���� ũ��, ������ 0�̶�� å����ŭ ������ �����Ѵ�.
	//		- ��������Key, _T("") �� �����Ѵ�.
	//=======================================================================
	if(nBookCnt > 0 && nRowCnt == 0) 
		nRowCnt = GetSpeciesCnt(m_pDM);


	//=======================================================================
	// 2. å���� 0���� ũ��, �Ǽ��� 0�̶�� å����ŭ �Ǽ��� �����Ѵ�.
	//		- ��������Key-���Ϸù�ȣ, _T("") �� �����Ѵ�.
	//=======================================================================
	if(nBookCnt > 0 && nVolCnt == 0)
		nVolCnt = GetVolumeCnt(m_pDM);
	
	//=======================================================================
	// 3. �˻��� �ڷῡ ���� �޽����� �ۼ��Ͽ� ������.
	//		- 1,2�� ������ �޽��� ���뿡 ����.
	//=======================================================================
	strMsg.Format(_T("%d�� %d�� %då�� �˼� �ڷᰡ �˻��Ǿ����ϴ�."), nRowCnt, nVolCnt, nBookCnt);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	

	//=======================================================================
	// 4. �˻�Ŭ������ ����� �׷���� Ŭ�������� ���ε� �׷��ȣ ���� �����´�.
	//=======================================================================
	ids = m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strMsg);
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("�׷��ȣ ���� ������ �� �����ϴ�."));
		return;
	}


	//=======================================================================
	// 5. ������ �׷��ȣ�� CM�� �����Ѵ�.
	//=======================================================================
	ids = m_pCM->SetControlMgrData(_T("�׷��ȣ"), strMsg);
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("�׷��ȣ ���� [CM_�ֹ��ڷ����]�� ������ �� �����ϴ�."));
		return;
	}

	//=======================================================================
	// 6. ���Գ⵵ ���� �˻�Ŭ������ ����� �׷���� Ŭ�������� �������Ŀ�
	//	 CM�� �����Ѵ�.
	//=======================================================================
	ids = m_pCM->SetControlMgrData(_T("���Գ⵵"), m_pSearchDlg->GetGroupCollection()->GetAcqYear());
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("���Գ⵵ ���� [CM_�ֹ��ڷ����]�� ������ �� �����ϴ�."));
		return;
	}
}

VOID CBO_ACQ_OrderDataMgr::ShowSearchDlg()
{
	m_pSearchDlg->ShowSearchDlg();
}

VOID CBO_ACQ_OrderDataMgr::ShowPrintDlg()
{
	if(!m_pPrintDlg->GetCreate()) m_pPrintDlg->Create(this);
	m_pPrintDlg->ShowWindow(SW_SHOW);
	m_pPrintDlg->UpdateWindow();
	m_pPrintDlg->CenterWindow();
}

VOID CBO_ACQ_OrderDataMgr::ShowOrderDataDlg(OPERATION_TYPE nOpType)
{
	INT nSelect, nRowCnt;
	CESL_Grid* pGrid	= NULL;
	CESL_DataMgr* pDM	= NULL;
	PARENT_TYPE nParentType = SPECIES;

	pGrid = GetActiveGrid();
	pDM = GetActiveDM();
	nSelect = m_tabOrderData.GetCurSel();
	nRowCnt = pDM->GetRowCount();
	
	if		( nSelect == 0 ) 	nParentType = SPECIES;
	else if	( nSelect == 1 ) 	nParentType = VOLUME;
	
	// 2005-01-06 ������ �׸����� ��� �ٱǺ� ������ ȭ�� �������� ���� �ϱ� ���� �ϰ� display�ϹǷ� 
	// ���� �÷� ��ȣ ����� �ϸ� ����ó���� ��� �׸����� �� �ؿ� �߰��ǹǷ� ���� �����Ͱ� �ٲ�� ������.
	// if(pGrid) pGrid->m_bProtectSelectColumn = TRUE;
	CSpecies_Form dlg(this);
	dlg.SetAcqType(PURCHASE);
	dlg.SetSpeciesType(BO_ORDER);
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	dlg.SetOpType(nOpType);
	if( nOpType == SINSERT && nParentType == VOLUME )
	{
		ESLAfxMessageBox(_T("������ �׸���ȭ�鿡�� �Է��� �ּ���."));
		return ;
	}
	if( nOpType == SUPDATE && nParentType == VOLUME )
	{
		ESLAfxMessageBox(_T("������ �׸���ȭ�鿡�� ������ �ּ���."));
		return ;
	}
	if( nOpType != SINSERT )
	{
		if( 0 == nRowCnt )
		{
			ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
			return ;
		}
	}

	//===================================================
	//2009.04.08 ADD BY PJW : �׸��� ������ �Ұ�� �������ڷᰡ ������� ����Ŀ���� ������ �߰��Ѵ�.
	pGrid->SelectMakeList();
//DEL	if(nOpType == GUPDATE)
//DEL	{
		if( pGrid->SelectGetCount() > 0 )
		{
			pGrid->SetAt( pGrid->GetIdx() , 0 , _T("V") );
			pGrid->SelectMakeList();
		}
//DEL	}	
	//===================================================

	dlg.SetSpeciesKeyAlias(_T("IBS_��KEY"));
	dlg.SetAccuKeyAlias(_T("BP_��������KEY"));
	dlg.SetParentType(nParentType);
	dlg.SetParentDM(pDM);
	dlg.SetParentBookDM(m_pDM);
	dlg.SetParentGrid(pGrid);
	dlg.DoModal();
	SetOrderDataInfo(nSelect);
	if( nOpType == SINSERT )
	{
		INT nCurCnt = pDM->GetRowCount();
		if(nCurCnt > nRowCnt)
		{
			pGrid->DisplayNumber();
			pGrid->SetTopRow(nRowCnt+1);
		}
	}

	// 2005-01-06
	// ���� �÷� ��ȣ ����� ���� �Ѵ�. 
	// if(pGrid) pGrid->m_bProtectSelectColumn = FALSE;


}

VOID CBO_ACQ_OrderDataMgr::DeleteOrderData()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	INT nCurSel = m_tabOrderData.GetCurSel();
	if(nCurSel == 0)
	{
		ids = DeleteOrderSpeciesData();
	}
	else if(nCurSel == 1)
	{
		ids = DeleteOrderVolumeData();
	}
	if(ids == 0)
	{
		SetOrderDataInfo(nCurSel);
		DisplayGrid();	
	}
/*
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ ������ �� �����ϴ�."));
		return;
	}
	if(ids == 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ �����Ͽ����ϴ�."));
		SetOrderDataInfo(nCurSel);
		DisplayGrid();
	}
*/
}

VOID CBO_ACQ_OrderDataMgr::OrderCancel()
{
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0)
	{
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		return ;
	}
	if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		return ;
	}

	COrderDataCancelDlg dlg(this);
	if(IDCANCEL == dlg.DoModal()) return;
	
	if(dlg.m_nOrderCancelType == 0)
	{
		OrderCancelByDivno();	
	}
	else if(dlg.m_nOrderCancelType == 1)
	{
		OrderCancelBySelData();
	}
}

VOID CBO_ACQ_OrderDataMgr::CheckInOrderData()
{
	if(m_pPrintDM_Volume->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("�˼�ó���� �ڷᰡ �����ϴ�."));
		return;
	}
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid_Volume"));
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("��DM"), (DWORD*)m_pPrintDM_Volume);
	api.SetTempPtr(_T("åDM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("�׸���"), (DWORD*)pGrid);
	api.SetTempData(_T("�׸����÷���ȣ"), m_nEditCol);
	//=====================================================
	//2009.02.24 UPDATE BY PJW : K2UP���� FALSE�� ���õǾ� ���� -> �� TRUE�� �Ǿ� �մ��� �𸣰���
// 	api.SetTempData(_T("���º��濩��"), TRUE);
 	api.SetTempData(_T("���º��濩��"), FALSE);
	//=====================================================
	
	api.SetTempData(_T("�ڷ����"), _T("BOA112N"));
	INT ids = api.SPFExecutePath(_T("�˼�ó��"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�۾��Ͻ� �˼��۾� ������ ������ �� �����ϴ�."));
		return;
	}
	if(ids > 0) return ;
	ESLAfxMessageBox(_T("�۾��Ͻ� �˼��۾� ������ �����Ͽ����ϴ�."));
	pGrid->Display();
}

VOID CBO_ACQ_OrderDataMgr::ShowChangeCodeDlg()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}

	CESL_DataMgr * pDM = GetActiveDM();
	CESL_Grid * pGrid = GetActiveGrid();

	BOOL bSpeciesYN;
	if(m_tabOrderData.GetCurSel() == 0) bSpeciesYN = TRUE;
	else bSpeciesYN = FALSE;

	CCodeChange dlg(this);
	dlg.SetDM(pDM);
	dlg.SetBookDM(m_pDM);
	dlg.SetMainGrid(pGrid);
	dlg.SetAcqType(1);
	dlg.SetSpeciesYN(bSpeciesYN);
	if(IDOK == dlg.DoModal())
	{
		pGrid->SetProtectSelectColumn(TRUE);
		DisplayGrid();
		pGrid->SetProtectSelectColumn(FALSE);
	}
}

VOID CBO_ACQ_OrderDataMgr::ShowChangePurchaseInfoDlg()
{
	//20070830 ADD PJW : �������ǿ��� �������������� ���� �� �ϵ��� ����
	INT nSelect;
	PARENT_TYPE nParentType = SPECIES;
	nSelect = m_tabOrderData.GetCurSel();
	if		( nSelect == 0 ) 	nParentType = SPECIES;
	else if	( nSelect == 1 ) 	nParentType = VOLUME;
	
	if( nParentType == VOLUME )
	{
		ESLAfxMessageBox(_T("������ �׸���ȭ�鿡�� ���������� ������ �ּ���."));
		return ;
	}
	//

	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	CESL_DataMgr * pDM = GetActiveDM();
	CESL_Grid * pGrid = GetActiveGrid();
	CPurchaseInfoChange dlg(this);
	dlg.SetDM(pDM);
	dlg.SetBookDM(m_pDM);
	dlg.SetMainGrid(pGrid);
	dlg.SetSpeciesType(2);
	if(IDOK == dlg.DoModal())
	{
		pGrid->SetProtectSelectColumn(TRUE);
		DisplayGrid();
		pGrid->SetProtectSelectColumn(FALSE);
	}
}

VOID CBO_ACQ_OrderDataMgr::ShowDivNoMgrDlg()
{
	CDivisionNo_Mgr mgr(this);
	mgr.SetWorkStatus(_T("�ֹ�"));
	mgr.DoModal();

	return ;
}

VOID CBO_ACQ_OrderDataMgr::ShowPriceSumDlg()
{
	CPriceSumDlg pDlg(this);
	pDlg.SetOpenMode(K2UPGRADE);
	pDlg.SetSpeciesType(BO_ORDER);
	INT nSelect = m_tabOrderData.GetCurSel();
	CESL_Grid * pGrid = NULL;
	if(nSelect == 0)
	{
		pDlg.SetParentDM(m_pPrintDM_Species);
		pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
		pDlg.SetSpeciesYN(TRUE);
	}
	else
	{
		pDlg.SetParentDM(m_pPrintDM_Volume);
		pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid_Volume"));
		pDlg.SetSpeciesYN(FALSE);
		pDlg.SetParentBookDM(m_pDM);
	}
	pDlg.SetParentGroupCollection(m_pSearchDlg->GetGroupCollection());
	pDlg.SetParentGrid(pGrid);
	pDlg.DoModal();
}

VOID CBO_ACQ_OrderDataMgr::CheckInComplete()
{
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0)
	{
		ESLAfxMessageBox(_T("�˻��� ���������� �����ϴ�."));
		return ;
	}
	CString strGroupInfo;
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strGroupInfo);
	if(strGroupInfo == _T("��ü�ڷ�"))
	{
		ESLAfxMessageBox(_T("���� ������ ������ �����ϴ�. �˻��� ���Ͽ� Ư�� ������ ������ �ֽʽÿ�."));
		return;
	}
	INT ids = this->CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}

	CBO_ACQ_CheckInDlg dlg(this);
	dlg.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	if(IDCANCEL == dlg.DoModal()) return ;

	CString strGroupNo, strSortDesc;
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();	
	api.SetTempPtr(_T("BOOK_DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("�׷�����"), (DWORD*)m_pSearchDlg->GetGroupCollection());
	ids = api.SPFExecutePath(_T("�˼��Ϸ��ĵ�ϴ��ó��"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ �˼��Ϸ�ó�� �� �� �����ϴ�."));
		return;
	}
	ESLAfxMessageBox(_T("������ �ڷḦ �˼��Ϸ�ó�� �Ͽ����ϴ�."));
	//==========================================================
	//20080605 UPDATE BY PJW : �˼�ó���� DM�� �ʱ�ȭ���� �ʴ´�.->�׸��� ������ ������°� ����
	//20080605 REM BY PJW : CBO_ACQ_Business_Api���� DM�� �������ڷ�� Refresh�ϱ⶧���� �����ϱ� �����
	//m_pDM->FreeData();
	//m_pPrintDM_Species->FreeData();
	//m_pPrintDM_Volume->FreeData();
	//INT nSelect = m_tabOrderData.GetCurSel();
	//INT nSelect = m_tabOrderData.GetCurSel();
	//DeleteDmRow( nSelect);
	//m_pGrid->Display();
	//==========================================================
	m_pDM->FreeData();
	m_pPrintDM_Species->FreeData();
	m_pPrintDM_Volume->FreeData();
	INT nSelect = m_tabOrderData.GetCurSel();
	SetTabSelect(nSelect);
}

BOOL CBO_ACQ_OrderDataMgr::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_ACQ_OrderDataMgr::OnbtnRESULTSEARCH() 
{
	// TODO: Add your control notification handler code here

	INT nCurSel = m_tabOrderData.GetCurSel();
	CESL_DataMgr * pDM = NULL;
	CESL_Grid * pGrid = NULL;
	if(nCurSel == 0) 
	{
		pDM = m_pPrintDM_Species;
		pGrid = m_pGrid_Species;
	}
	else if(nCurSel == 1) 
	{
		pDM = m_pPrintDM_Volume;
		pGrid = m_pGrid_Volume;
	}
	
	if(m_pResultSearchDlg != NULL)
	{
		delete m_pResultSearchDlg;
		m_pResultSearchDlg = NULL;
	}
	m_pResultSearchDlg = new CSearchMgr(this, BO_ACQ_RESEARCH, pDM, pGrid);
	m_pResultSearchDlg->ShowSearchDlg();
}


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ������/������ �׸��� �� ����
//
// [ �̴Ͻ��� ]
// 1. ���ٹ�ư�� �׸��� �ǿ� �°� �����Ѵ�.
// 2. �ǹ�ȣ�� ������ �ǹ�ȣ�� �ش��ϴ� DM�� Grid�� �����Ѵ�.
// 3. �����׸����� ���� ������ �о� ����Ŭ������ �����ϰ� ��������� ����Ѵ�.
//	- �����׸��� ������ �� ����Ŭ������ �˻�Ŭ������ �Ѱ� �˻��� ���ĵǵ��� �Ѵ�.	
// 4. DM�� �����ϰ� �����ϸ�, CM�� �����Ѵ�.
//
/////////////////////////////////////////////////////////////////////////////
VOID CBO_ACQ_OrderDataMgr::OnSelchangetabORDERDATA(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//==============================================
	//2008.07.11 ADD BY PJW : �����ϴܿ� ���¹�LOG���
	BEGIN_SBL();
	//==============================================
	//=======================================================================
	// ��������
	//=======================================================================
	INT nSelect, ids ;
	CString strGridAlias;


	//=======================================================================
	// 1. ���ٹ�ư�� �׸��� �ǿ� �°� �����Ѵ�.
	//=======================================================================
	SetToolBarBtnByTabSelect();
	
	//=======================================================================
	// 2. �ǹ�ȣ�� ������ �ǹ�ȣ�� �ش��ϴ� DM�� Grid�� �����Ѵ�.
	//=======================================================================
	nSelect = m_tabOrderData.GetCurSel();
	if(nSelect == 0) 
	{
		m_pActiveDM = m_pPrintDM_Species;
		strGridAlias = _T("MainGrid");
		//========================================================================
		//20080605 ADD BY PJW �׸��� ���� ����ɶ����� DM�� �������� ���� �Ѵ�.
		SetDmSelectToCancel( m_pPrintDM_Volume );
		SetDmSelectToCancel( m_pDM );
		//========================================================================

	}
	else if(nSelect == 1)
	{
		m_pActiveDM = m_pPrintDM_Volume;
		strGridAlias = _T("MainGrid_Volume");
		//========================================================================
		//20080605 ADD BY PJW �׸��� ���� ����ɶ����� DM�� �������� ���� �Ѵ�.
		SetDmSelectToCancel( m_pPrintDM_Species );
		SetDmSelectToCancel( m_pDM );
		//========================================================================			
	}

	//=======================================================================
	// 3. �����׸����� ���� ������ �о� �����׸���Ŭ������ �����ϰ� ��������� ����Ѵ�.
	//	- �����׸��� ������ �� �����׸���Ŭ������ �˻�Ŭ������ �Ѱ� �˻��� ���ĵǵ��� �Ѵ�.	
	//=======================================================================
	InitSortItem(GetActiveGrid(), strGridAlias);


	//=======================================================================
	// 4. DM�� �����ϰ� �����ϸ�, CM�� �����Ѵ�.
	//=======================================================================
	ids = MakeOrderDM(nSelect);

	if(m_pResultSearchDlg != NULL)
	{
		delete m_pResultSearchDlg;
		m_pResultSearchDlg = NULL;	
	}

	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �ǹ�ȣ������ �׸��带 �ٽ� �׸���, CM�� �����Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. ����(�ǹ�ȣ)�� ���� �׸��带 ���������� �����ش�. 
// 2. �׸��带 �ٽ� �׸���.
// 3. �ֹ�����(���Գ⵵, ����(�׷�)��ȣ)�� CM�� �����Ѵ�.
//
/////////////////////////////////////////////////////////////////////////////
INT CBO_ACQ_OrderDataMgr::SetTabSelect(INT nSelect)
{
	//=======================================================================
	// ���� ����
	//=======================================================================
	INT nRowCnt = -1;


	//=======================================================================
	// 1. ����(�ǹ�ȣ)�� ���� �׸��带 ���������� �����ش�. 
	//=======================================================================
	switch( nSelect )
	{
	case 0:
	{
		m_pGrid_Species->ShowWindow(SW_SHOW);
		m_pGrid_Volume->ShowWindow(SW_HIDE);
	}
		break;
	case 1:
	{
		m_pGrid_Species->ShowWindow(SW_HIDE);
		m_pGrid_Volume->ShowWindow(SW_SHOW);
	}
		break;
	default:	 return -1;
		break;
	}

	
	//=======================================================================
	// 2. �׸��带 �ٽ� �׸���.
	//=======================================================================
		DisplayGrid();


	//=======================================================================
	// 3. �ֹ�����(���Գ⵵, ����(�׷�)��ȣ)�� CM�� �����Ѵ�.
	//=======================================================================
		SetOrderDataInfo(nSelect);

	return 0;
}

UINT CBO_ACQ_OrderDataMgr::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	//==============================================
	//2008.07.11 ADD BY PJW : �����ϴܿ� ���¹�LOG���
	BEGIN_SBL();
	//==============================================
	INT nSelect;
	CString strQuery;
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("�ֹ��ڷḦ �������Դϴ�."));
	m_pSearchDlg->GetWhereStmt(strQuery);
	m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	m_pPrintDM_Species->FreeData();
	m_pPrintDM_Volume->FreeData();
	DeleteDataProcessingDlg();

	nSelect = m_tabOrderData.GetCurSel();

	return MakeOrderDM(nSelect);
}


////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - 
//
// [ �̴Ͻ��� ]
////////////////////////////////////////////////////////////////////////////
UINT CBO_ACQ_OrderDataMgr::OnMakePrintDM(LPARAM lParam, WPARAM wParam)
{
	//=====================================================================
	// ��������
	//=====================================================================
	INT ids = -1;
	INT nCurSel ;	
	CString strDivAlias, strExtraAlias ;
	CMapStringToString MapDivValue ;
	CMakePrintDMMgr PrintMgr(this) ;

	//=====================================================================
	// 1. DM�� �����Ѵ�.
	//=====================================================================
	CESL_DataMgr* pDM = FindDM(_T("DM_�ֹ��ڷ����_���"));
	if( pDM == NULL )
	{
		ESLAfxMessageBox( _T("[DM_�ֹ��ڷ����_���]�� ������ �� �����ϴ�.") );
		return 1;
	}


	//=====================================================================
	// 2. ���ڰ�(lParam)�� ���� �ش��ڷ����� ����� �Ѵ�.
	//=====================================================================
	switch( lParam )
	{
	// �˼�����ڷ���
	//=====================================================================
	// 2-1. �˼�����ڷ���
	//		- i. ���� �׸��尡 ���������� ���������� �ľ��Ѵ�.
	//=====================================================================
	case BO_ACQ_CHECKINING_DATA:
		{
			nCurSel = m_tabOrderData.GetCurSel();
			MapDivValue.RemoveAll() ;
			strDivAlias = _T("BP_��������KEY");

			if(nCurSel == 0)	// ������ �׸���
		{
				if(m_pPrintDlg->GetPrintDataType() == 2)	// �׸��� �����ڷ�
			{
					// ������ �ڷ��� _T("BP_��������KEY")���� �����Ѵ�.
				ids = PrintMgr.GetGridSelectData(GetActiveGrid(), GetActiveDM(), strDivAlias, &MapDivValue);
				if(ids < 0)
				{
					ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
					return -1;
				}
			}
				// ���α׷��� ��Ʈ�� â�� ���� �� �ʱ�ȭ �Ͽ� ��������� �����Ѵ�.
			ShowDataProcessingDlg();

			PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
			PrintMgr.SetTempData(_T("����ƮŸ��"), (INT)lParam);
			PrintMgr.SetTempData(_T("���о˸��ƽ�"), _T("BP_��������KEY"));
			PrintMgr.SetTempData(_T("å���˸��ƽ�"), _T("UDF_å��"));
			PrintMgr.SetTempData(_T("���ݾ˸��ƽ�"), _T("UDF_������"));
			PrintMgr.SetTempData(_T("���ΰ��˸��ƽ�"), _T("UDF_����������"));
			PrintMgr.SetTempData(_T("����ڷ����Ŀ���"), m_pPrintDlg->GetDataSortYN());
			PrintMgr.SetTempData(_T("��µ�����Ÿ��"), m_pPrintDlg->GetPrintDataType());

			PrintMgr.SetTempPtr(_T("�׸��弱���ڷ�"), (DWORD*)&MapDivValue);
			PrintMgr.SetTempPtr(_T("���α׷�����Ʈ��"), (DWORD*)m_pDataProcessingDlg);
			PrintMgr.SetTempPtr(_T("����åDM"), (DWORD*)m_pDM);
				PrintMgr.SetTempPtr(_T("�������DM"), (DWORD*)m_pPrintDM_Species);
			PrintMgr.SetTempPtr(_T("���DM"), (DWORD*)m_pSpeciesPrintDM);
			PrintMgr.SetTempPtr(_T("��������"), (DWORD*)m_pSortItemSet);

			ids = PrintMgr.SPFExecutePath(_T("�Ϲݸ���Ʈ���"));

				// ���α׷��� ��Ʈ�� â�� �Ҹ��Ѵ�.
			DeleteDataProcessingDlg();

			//===================================================
			//2009.10.21 ADD BY LKS : �ּ�����/���������� ����� ������ �̵��� �������� �ν��Ͽ� �ּ� ����
			//�ּ� ó�� ���� �𸣰���.
			m_pPrintDlg->SetIsSpecies(TRUE);
			//===================================================
			
			m_pPrintDlg->SetPrintDM(m_pSpeciesPrintDM);
		}
		else if(nCurSel == 1)
		{	
				strExtraAlias = _T("BB_���Ϸù�ȣ");
				
			if(m_pPrintDlg->GetPrintDataType() == 2)
			{
				ids = PrintMgr.GetGridSelectData(GetActiveGrid(), GetActiveDM(), strDivAlias, &MapDivValue, strExtraAlias);
				if(ids < 0)
				{
					ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
					return -1;
				}
			}
			ShowDataProcessingDlg();

			PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
			PrintMgr.SetTempData(_T("����ƮŸ��"), lParam);
			PrintMgr.SetTempData(_T("���о˸��ƽ�"), _T("BP_��������KEY"));
			PrintMgr.SetTempData(_T("����ڷ����Ŀ���"), m_pPrintDlg->GetDataSortYN());
			PrintMgr.SetTempData(_T("��µ�����Ÿ��"), m_pPrintDlg->GetPrintDataType());
			PrintMgr.SetTempPtr(_T("�׸��弱���ڷ�"), (DWORD*)&MapDivValue);
			PrintMgr.SetTempPtr(_T("���α׷�����Ʈ��"), (DWORD*)m_pDataProcessingDlg);
				PrintMgr.SetTempPtr(_T("����åDM"), (DWORD*)m_pPrintDM_Volume);
			PrintMgr.SetTempPtr(_T("���DM"), (DWORD*)pDM);
			PrintMgr.SetTempPtr(_T("��������"), (DWORD*)m_pSortItemSet);
			ids = PrintMgr.SPFExecutePath(_T("�Ǹ���Ʈ���"));

			DeleteDataProcessingDlg();
			if(ids < 0) 
			{
				ESLAfxMessageBox(_T("��µ����Ϳ� ������ �ֽ��ϴ�."));
				return -1;
			}
			m_pPrintDlg->SetPrintDM(pDM);
			m_pPrintDlg->SetIsSpecies(FALSE);
		}
		
		m_pPrintDlg->Print();
		return 0;
	}
		break;

	case BO_ACQ_CHECKIN_REQUEST_DATA:
	{
		//��ġ�����û�ڷḦ ���� ��DM�� �����.
		CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
		CString strDivAlias = _T("BP_��������KEY");
		CMakePrintDMMgr PrintMgr(this);
		if(m_pPrintDlg->GetPrintDataType() == 2)
		{
			ids = PrintMgr.GetGridSelectData(GetActiveGrid(), GetActiveDM(), strDivAlias, &MapDivValue);
			if(ids < 0)
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
		PrintMgr.SetTempData(_T("���ݾ˸��ƽ�"), _T("UDF_������"));
		PrintMgr.SetTempData(_T("���ΰ��˸��ƽ�"), _T("UDF_����������"));
		PrintMgr.SetTempData(_T("����ڷ����Ŀ���"), m_pPrintDlg->GetDataSortYN());
		PrintMgr.SetTempData(_T("��µ�����Ÿ��"), m_pPrintDlg->GetPrintDataType());
		PrintMgr.SetTempPtr(_T("�׸��弱���ڷ�"), (DWORD*)&MapDivValue);
		PrintMgr.SetTempPtr(_T("���α׷�����Ʈ��"), (DWORD*)m_pDataProcessingDlg);
		PrintMgr.SetTempPtr(_T("����åDM"), (DWORD*)m_pDM);
		PrintMgr.SetTempPtr(_T("�������DM"), (DWORD*)pDM);
		PrintMgr.SetTempPtr(_T("���DM"), (DWORD*)m_pSpeciesPrintDM);
		PrintMgr.SetTempPtr(_T("��������"), (DWORD*)m_pSortItemSet);
		ids = PrintMgr.SPFExecutePath(_T("��û�ڷᵵ��������"));
		DeleteDataProcessingDlg();
		m_pPrintDlg->SetIsSpecies(TRUE);
		m_pPrintDlg->SetPrintDM(m_pSpeciesPrintDM);	
		m_pPrintDlg->Print();
		return 0;
	}
		break;

	case BO_ACQ_CHECKIN_DATA:
	case BO_ACQ_NONCHECKIN_DATA:
	{
		CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
		CString strDivAlias = _T("BP_��������KEY");
		CString strExtraAlias = _T("BB_���Ϸù�ȣ");
		CMakePrintDMMgr PrintMgr(this);
		if(m_pPrintDlg->GetPrintDataType() == 2)
		{
			ids = PrintMgr.GetGridSelectData(GetActiveGrid(), GetActiveDM(), strDivAlias, &MapDivValue, strExtraAlias);
			if(ids < 0)
			{
				ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
				return -1;
			}
		}
		ShowDataProcessingDlg();
		PrintMgr.InitTempData();	PrintMgr.InitTempPtr();
		PrintMgr.SetTempData(_T("����ƮŸ��"), lParam);
		PrintMgr.SetTempData(_T("���о˸��ƽ�"), _T("BP_��������KEY"));
		PrintMgr.SetTempData(_T("����ڷ����Ŀ���"), m_pPrintDlg->GetDataSortYN());
		PrintMgr.SetTempData(_T("��µ�����Ÿ��"), m_pPrintDlg->GetPrintDataType());
		PrintMgr.SetTempPtr(_T("�׸��弱���ڷ�"), (DWORD*)&MapDivValue);
		PrintMgr.SetTempPtr(_T("���α׷�����Ʈ��"), (DWORD*)m_pDataProcessingDlg);
		PrintMgr.SetTempPtr(_T("����åDM"), (DWORD*)m_pDM);
		PrintMgr.SetTempPtr(_T("���DM"), (DWORD*)pDM);
		PrintMgr.SetTempPtr(_T("��������"), (DWORD*)m_pSortItemSet);
		ids = PrintMgr.SPFExecutePath(_T("�Ǹ���Ʈ���"));
		DeleteDataProcessingDlg();
	}
		break;

	case BO_ACQ_CHECKIN_STATISTICS:
	{
		CBO_Purchase_Statistics dlg(this);
		dlg.SetOpenType(1000);
		dlg.SetParentGroupCollection(m_pSearchDlg->GetGroupCollection());
		dlg.SetDivNoWorkStatus(_T("�ֹ�"));
		dlg.SetSpeciesType(BO_ORDER);
		dlg.DoModal();
	}
		break;

	case BO_ACQ_NONCHECKIN_STATISTICS:
	{
		CBO_Purchase_Statistics dlg(this);
		dlg.SetOpenType(-1000);
		dlg.SetParentGroupCollection(m_pSearchDlg->GetGroupCollection());
		dlg.SetDivNoWorkStatus(_T("�ֹ�"));
		dlg.SetSpeciesType(BO_ORDER);
		dlg.DoModal();
	}
		break;
	
	case BO_ACQ_ORDER_DATA_ORDER:
	{	
		BOOL bExtraInfo = FALSE;
		if(wParam == 1000) bExtraInfo = TRUE;
		CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
		CString strDivAlias = _T("BP_��������KEY");
		CMakePrintDMMgr PrintMgr(this);
		if(m_pPrintDlg->GetPrintDataType() == 2)
		{
			ids = PrintMgr.GetGridSelectData(GetActiveGrid(), GetActiveDM(), strDivAlias, &MapDivValue);
			if(ids < 0)
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
		PrintMgr.SetTempData(_T("���ݾ˸��ƽ�"), _T("UDF_����"));
		PrintMgr.SetTempData(_T("���ΰ��˸��ƽ�"), _T("UDF_���ΰ�"));
		PrintMgr.SetTempData(_T("åDM�˸��ƽ�"), m_pDM->DB_MGR_ALIAS.GetBuffer(0));
		PrintMgr.SetTempData(_T("KOLAS2����"), bExtraInfo);
		PrintMgr.SetTempData(_T("����ڷ����Ŀ���"), m_pPrintDlg->GetDataSortYN());		
		PrintMgr.SetTempData(_T("��µ�����Ÿ��"), m_pPrintDlg->GetPrintDataType());
		PrintMgr.SetTempPtr(_T("�׸��弱���ڷ�"), (DWORD*)&MapDivValue);
		PrintMgr.SetTempPtr(_T("���α׷�����Ʈ��"), (DWORD*)m_pDataProcessingDlg);
		PrintMgr.SetTempPtr(_T("�׷��÷���"), (DWORD*)m_pSearchDlg->GetGroupCollection());
		PrintMgr.SetTempPtr(_T("���DM"), (DWORD*)pDM);
		PrintMgr.SetTempPtr(_T("��������"), (DWORD*)m_pSortItemSet);
		PrintMgr.SetTempData(_T("�����ʵ�˸��ƽ�"), _T(""));
				
		ids = PrintMgr.SPFExecutePath(_T("�Ұ��ڷ����"));
		DeleteDataProcessingDlg();
		
	}
		break;

	default:
	{
		BOOL bExtraInfo = FALSE;
		if(wParam == 1000) bExtraInfo = TRUE;
		CMapStringToString MapDivValue;	MapDivValue.RemoveAll();
		CString strDivAlias = _T("BP_��������KEY");
		CMakePrintDMMgr PrintMgr(this);
		if(m_pPrintDlg->GetPrintDataType() == 2)
		{
			ids = PrintMgr.GetGridSelectData(GetActiveGrid(), GetActiveDM(), strDivAlias, &MapDivValue);
			if(ids < 0)
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
		PrintMgr.SetTempData(_T("���ݾ˸��ƽ�"), _T("UDF_����"));
		PrintMgr.SetTempData(_T("���ΰ��˸��ƽ�"), _T("UDF_���ΰ�"));
		PrintMgr.SetTempData(_T("åDM�˸��ƽ�"), m_pDM->DB_MGR_ALIAS.GetBuffer(0));
		PrintMgr.SetTempData(_T("KOLAS2����"), bExtraInfo);
		PrintMgr.SetTempData(_T("����ڷ����Ŀ���"), m_pPrintDlg->GetDataSortYN());		
		PrintMgr.SetTempData(_T("��µ�����Ÿ��"), m_pPrintDlg->GetPrintDataType());
		PrintMgr.SetTempPtr(_T("�׸��弱���ڷ�"), (DWORD*)&MapDivValue);
		PrintMgr.SetTempPtr(_T("���α׷�����Ʈ��"), (DWORD*)m_pDataProcessingDlg);
		PrintMgr.SetTempPtr(_T("�׷��÷���"), (DWORD*)m_pSearchDlg->GetGroupCollection());
		PrintMgr.SetTempPtr(_T("���DM"), (DWORD*)pDM);
		PrintMgr.SetTempPtr(_T("��������"), (DWORD*)m_pSortItemSet);

		switch( lParam )
		{
		case BO_ACQ_ORDER_DATA_DIVNO:		PrintMgr.SetTempData(_T("�����ʵ�˸��ƽ�"), _T(""));
			break;
		case BO_ACQ_ORDER_DATA_PROVIDER:	PrintMgr.SetTempData(_T("�����ʵ�˸��ƽ�"), _T("BP_����ó��ȣ"));
			break;
		case BO_ACQ_ORDER_DATA_SUBJECTCODE:	PrintMgr.SetTempData(_T("�����ʵ�˸��ƽ�"), _T("BP_��������"));
			break;
		}
		
		ids = PrintMgr.SPFExecutePath(_T("�Ұ��ڷ����"));
		DeleteDataProcessingDlg();
	}
		break;
	}

	if(ids < 0) return 0;
	m_pPrintDlg->SetPrintDM(pDM);
	m_pPrintDlg->Print();

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - DM�� �����ϰ� �����ϸ�, CM�� �����Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. DM�� �˻��� ������ ���ٸ� �����Ѵ�.
// 2. Progress Ctrl Dialog â ���� �� �ʱ�ȭ 
// 3. Business API�� �ʱ�ȭ �Ѵ�.
// 4. �ǹ�ȣ�� �ش��ϴ� DM �� ������ �Լ��� Business Api�� �����Ѵ�.
// 5. Progress Ctrl â�� �Ҹ��Ѵ�.
// 6. ����Ŭ������ �̿��Ͽ� ���� DM�� �����Ѵ�.
// 7. �ǹ�ȣ�� ���� �׸��带 �ٽ� �׸���, CM�� �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////
INT CBO_ACQ_OrderDataMgr::MakeOrderDM(INT nSelect)
{
	//============================================================================
	//2009.03.17 UPDATE BY LKS : ���� �� �� �̵��� ������ ���� �ݿ��ȵǴ� ���� ����
	CString strQuery;
	m_pSearchDlg->GetWhereStmt(strQuery);
	if(!strQuery.IsEmpty())
	{
		m_pDM->RefreshDataManager(strQuery);
		m_pPrintDM_Species->FreeData();
		m_pPrintDM_Volume->FreeData();
	}
	//============================================================================

	//=======================================================================
	// ��������
	//=======================================================================
	INT ids = -1;
	CBO_ACQ_Business_Api mgr(this);


	//=======================================================================
	// 1. DM�� �˻��� ������ ���ٸ� �����Ѵ�.
	//=======================================================================
	if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		SetTabSelect(nSelect);
		return 0;
	}

	
	//=======================================================================
	// 2. Progress Ctrl Dialog â ���� �� �ʱ�ȭ 
	//=======================================================================
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("�ֹ��ڷḦ �������Դϴ�."));
	

	//=======================================================================
	// 3. Business API�� �ʱ�ȭ �Ѵ�.
	//=======================================================================
	mgr.InitTempData();
	
	//=======================================================================
	// 4. �ǹ�ȣ�� �ش��ϴ� DM �� ������ �Լ��� Business Api�� �����Ѵ�.
	//=======================================================================
	switch( nSelect )
	{
	case 0:
	{
		m_pPrintDM_Species->FreeData();
		mgr.SetTempPtr(_T("åDM"), (DWORD*)m_pDM);
		mgr.SetTempPtr(_T("��DM"), (DWORD*)m_pPrintDM_Species);
		mgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
		ids = mgr.SPFExecutePath(_T("åTo��������"));
	}
		break;

	case 1:
	{
		m_pPrintDM_Volume->FreeData();
		mgr.SetTempPtr(_T("åDM"), (DWORD*)m_pDM);
		mgr.SetTempPtr(_T("��DM"), (DWORD*)m_pPrintDM_Volume);
		mgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
		ids = mgr.SPFExecutePath(_T("åTo��"));
	}
		break;

	default:
		{
			ESLAfxMessageBox( _T("���� �ǿ� �ش��ϴ� �۾��� �� �� �����ϴ�.") );
			return -1;
		}
		break;
	}


	//=======================================================================
	// 5. Progress Ctrl â�� �Ҹ��Ѵ�.
	//=======================================================================
	DeleteDataProcessingDlg();

	if(ids < 0) return ids;
	


	//=======================================================================
	// 6. ����Ŭ������ �̿��Ͽ� ���� DM�� �����Ѵ�.
	//=======================================================================
	ids = m_pSortItemSet->DataSort(GetActiveDM(), !m_pSearchDlg->GetDataSortYN());
	if( ids < 0 ) return ids;
	//SortByGroupInfo();



	//=======================================================================
	// 7. �ǹ�ȣ�� ���� �׸��带 �ٽ� �׸���, CM�� �����Ѵ�.
	//=======================================================================
	ids = SetTabSelect(nSelect);

	//==============================================
	//2008.07.11 ADD BY PJW : �����ϴܿ� ���¹�LOG���
	INT nRowCount = m_pActiveDM->GetRowCount();
	END_SBL(nRowCount);
	//==============================================

	if( ids < 0 ) return -1;

	return 0;
}

BEGIN_EVENTSINK_MAP(CBO_ACQ_OrderDataMgr, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_ACQ_OrderDataMgr)
	ON_EVENT(CBO_ACQ_OrderDataMgr, IDC_gridORDER_VOLUME_DATA, -601 /* DblClick */, OnDblClickgridORDERVOLUMEDATA, VTS_NONE)
	ON_EVENT(CBO_ACQ_OrderDataMgr, IDC_gridORDER_DATA, -601 /* DblClick */, OnDblClickgridORDERDATA, VTS_NONE)
	ON_EVENT(CBO_ACQ_OrderDataMgr, IDC_gridORDER_VOLUME_DATA, -600 /* Click */, OnClickgridORDERVOLUMEDATA, VTS_NONE)
	ON_EVENT(CBO_ACQ_OrderDataMgr, IDC_gridORDER_VOLUME_DATA, -607 /* MouseUp */, OnMouseUpgridORDERVOLUMEDATA, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CBO_ACQ_OrderDataMgr, IDC_gridORDER_DATA, -607 /* MouseUp */, OnMouseUpgridORDERDATA, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_ACQ_OrderDataMgr::OnDblClickgridORDERVOLUMEDATA() 
{
	// TODO: Add your control notification handler code here
	//===================================================
	
	//===================================================
	//2009.05.20 ADD BY LKS : ��ǰå�� �÷��� ����Ŭ������ ���� ���� ó��
	CESL_Grid * pGrid = GetActiveGrid();
	int nCol;
	nCol = pGrid->GetMouseCol();
	CString sColumnName;
	sColumnName = pGrid->GetTextMatrix(0,nCol);
	if(sColumnName == _T("��ǰå��"))
		return;
	//===================================================
	
	//2009.04.07 ADD BY PJW : �׸��� ����Ŭ��������� ������ TAB�̸� �޼��� �����ش�.
	INT nSelect;
	INT nParentType;
	nSelect = m_tabOrderData.GetCurSel();
	if		( nSelect == 0 ) 	nParentType = SPECIES;
	else if	( nSelect == 1 ) 	nParentType = VOLUME;
	if( nParentType == VOLUME )
	{
		ESLAfxMessageBox(_T("������ �׸���ȭ�鿡�� ������ �ּ���."));
		return ;
	}
	//===================================================
	

	if(m_pGrid_Volume->GetMouseRow() == 0 ) return ;
	if(m_pGrid_Volume->GetCol() == m_nEditCol+1) return ;
	CESL_DataMgr * pDM = GetActiveDM();
	if(pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		return;
	}
	//CESL_Grid * pGrid = GetActiveGrid();
	if(pGrid->GetMouseRow() == 0) return ;

	ShowOrderDataDlg(GUPDATE);
}

VOID CBO_ACQ_OrderDataMgr::OnDblClickgridORDERDATA() 
{
	// TODO: Add your control notification handler code here
	CESL_DataMgr * pDM = GetActiveDM();
	if(pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("�˻��� �ڷᰡ �����ϴ�."));
		return;
	}
	CESL_Grid * pGrid = GetActiveGrid();
	if(pGrid->GetMouseRow() == 0) return ;
	
	ShowOrderDataDlg(GUPDATE);
}

VOID CBO_ACQ_OrderDataMgr::OnClickgridORDERVOLUMEDATA() 
{
	// TODO: Add your control notification handler code here
}

BOOL CBO_ACQ_OrderDataMgr::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN ) 
	{
		if(pMsg->wParam == VK_F11) 
		{
			return FALSE;
		}
	}
	
	if (ESL_GRID_MODIFY == pMsg->message)
	{
		// ��Ǳå�� �÷��� �����Ǿ��ٸ�.
		if (pMsg->lParam== m_nEditCol) 
		{	
			if (0>ReCalcBookCount(pMsg->wParam + 1))	///���� ���ȣ
			{
				ESLAfxMessageBox(_T("�̳�å���� �����ϴµ� �����Ͽ����ϴ�."));
			}	
		}
	}
	
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

BOOL CBO_ACQ_OrderDataMgr::CheckIsDigit(CString str)
{
	if(str.IsEmpty()) return FALSE;

	TCHAR * tmp = str.GetBuffer(0);
	INT size = str.GetLength();
	for(INT i=0 ; i<size ; i++)
	{
		if(tmp[i] < '0' || tmp[i] > '9') 
			return FALSE;
	}
	return TRUE;
}


INT CBO_ACQ_OrderDataMgr::ReCalcBookCount(INT nRow/*def:-1*/)
{
	CESL_Grid *pGrid = (CESL_Grid *)m_pCM->FindControl(_T("MainGrid_Volume"));

	if (!pGrid) return -2;
	
	INT nCurRow = -1;
	if (-1==nRow)	nCurRow = pGrid->GetRow();
	else			nCurRow = nRow;
	///�Էµ� å �� ����
	CString sTemp, strDeliveredCount;
	BOOL bInvalidCount = FALSE;
	INT nOrderCount;
	
	nOrderCount	= _ttoi((pGrid->GetTextMatrix(nCurRow, m_nEditCol)).GetBuffer(0));
	strDeliveredCount = pGrid->GetTextMatrix(nCurRow, m_nEditCol+1);
	
	if(!CheckIsDigit(strDeliveredCount))
	{
		sTemp.Format(_T("��ǰå���� ���� �ƴѰ�( %s )�� �ԷµǾ����ϴ�."), strDeliveredCount);
		bInvalidCount = TRUE;
	}
	INT nDeliveredCount = _ttoi((strDeliveredCount).GetBuffer(0));
	INT nNondeliveredCount = nOrderCount-nDeliveredCount;

	///��ǰå���� �ֹ�å������ ũ�ٸ�
	if (nOrderCount<nDeliveredCount) 
	{
		sTemp.Format(_T("��ǰå��(%d)�� �ֹ�å��(%d)���� Ů�ϴ�."), nDeliveredCount, nOrderCount);
		bInvalidCount = TRUE;
	}
	///��ǰå���� 0���� �۴ٸ�
	else if (0>nDeliveredCount)
	{
		sTemp.Format(_T("��ǰå��(%d)�� 0 ���� �۽��ϴ�."), nDeliveredCount, nOrderCount);
		bInvalidCount = TRUE;
	}
	
	///�߸��� å�� �Էµ�
	if (bInvalidCount) 
	{
		ESLAfxMessageBox(sTemp);
		///���� ��ǰå��/�ֹ�å�� ������ ����
		pGrid->SetTextMatrix(nCurRow, m_nEditCol+1, GetActiveDM()->GetCellData(_T("UDF_��ǰå��"), nCurRow-1));
		nDeliveredCount = _ttoi(GetActiveDM()->GetCellData(_T("UDF_��ǰå��"), nCurRow-1).GetBuffer(0));
		sTemp.Format(_T("%d"), nOrderCount-nDeliveredCount);
		pGrid->SetTextMatrix(nCurRow, m_nEditCol+2, sTemp);
		return 1;
	}
	
	sTemp.Format(_T("%d"), nOrderCount-nDeliveredCount);
	pGrid->SetTextMatrix(nCurRow, m_nEditCol+2, sTemp);

	return 0;
}

VOID CBO_ACQ_OrderDataMgr::SelDataDeliveryProc()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("Grid"), (DWORD*)GetActiveGrid());
	api.SetTempData(_T("GET_COLUMN"), _T("����å��"));
	api.SetTempData(_T("SET_COLUMN"), _T("��ǰå��"));
	api.SetTempData(_T("CLEAR_COLUMN"), _T("�̳�å��"));
	ids = api.SPFExecutePath(_T("��ǰó��"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ ��ǰó���� �� �����ϴ�."));
		return ;
	}
}

VOID CBO_ACQ_OrderDataMgr::SelDataDeliveryCancelProc()
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("Grid"), (DWORD*)GetActiveGrid());
	api.SetTempData(_T("GET_COLUMN"), _T("����å��"));
	api.SetTempData(_T("SET_COLUMN"), _T("�̳�å��"));
	api.SetTempData(_T("CLEAR_COLUMN"), _T("��ǰå��"));
	ids = api.SPFExecutePath(_T("��ǰ���"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ ��ǰ����� �� �����ϴ�."));
		return ;
	}
}

VOID CBO_ACQ_OrderDataMgr::RegTransfer()
{
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0)
	{
		ESLAfxMessageBox(_T("�˻��� ���������� �����ϴ�."));
		return ;
	}

	CString strGroupInfo;
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strGroupInfo);
	if(strGroupInfo == _T("��ü�ڷ�"))
	{
		ESLAfxMessageBox(_T("���� ������ ������ �����ϴ�. �˻��� ���Ͽ� Ư�� ������ ������ �ֽʽÿ�."));
		return ;
	}

	INT ids = this->CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}

	CString strSpeciesKey;
	CString strBookSpeciesKey;
	CString strVolSortNo;
	CString strBookVolStroNo;
	INT nIdx, y;
	for ( y=0 ; y<m_pDM->GetRowCount() ; y++ )
	{	
		m_pDM->SetCellData(_T("����"), _T(""), y );	
	}			

	CESL_DataMgr*	m_pPrintDM;		
	CESL_Grid*	pGrid;	
	if ( m_tabOrderData.GetCurSel() == 0 ) 
	{
		m_pPrintDM = m_pPrintDM_Species;
		pGrid = m_pGrid_Species;		
	}
	else 
	{
		m_pPrintDM = m_pPrintDM_Volume;
		pGrid = m_pGrid_Volume;
	}

	for (nIdx = 0 ; nIdx < m_pPrintDM->GetRowCount() ; nIdx++ )
	{
		if ( pGrid->GetTextMatrix(nIdx+1,1) == _T("V") )
		{			
			m_pPrintDM->SetCellData( _T("����"), _T("V"), nIdx  );
			// 2008.03.12
			// �������� ���� ���¸� å������ �ݿ��Ѵ�.
			// DM_������������_�� -> DM_������������
			if ( m_tabOrderData.GetCurSel() == 0 ) 
			{
				strSpeciesKey = m_pPrintDM->GetCellData(_T("IBS_��KEY"), nIdx );
				for ( y=0 ; y<m_pDM->GetRowCount() ; y++ )
				{
					strBookSpeciesKey = m_pDM->GetCellData(_T("IBS_��KEY"), y );
					if ( strSpeciesKey == strBookSpeciesKey )
					{
						m_pDM->SetCellData(_T("����"), _T("V"), y );
					}
				}
			}
			else
			{
				// �Ǵ��� �ϰ�� ����� ���� �Ǵ��Ѵ�.
				strSpeciesKey = m_pPrintDM->GetCellData(_T("IBS_��KEY"), nIdx );
				strVolSortNo = m_pPrintDM->GetCellData(_T("BB_���Ϸù�ȣ"), nIdx );
				for ( y=0 ; y<m_pDM->GetRowCount() ; y++ )
				{
					strBookSpeciesKey = m_pDM->GetCellData(_T("IBS_��KEY"), y );
					strBookVolStroNo = m_pDM->GetCellData(_T("BB_���Ϸù�ȣ"), y );
					if ( strSpeciesKey == strBookSpeciesKey && strVolSortNo == strBookVolStroNo )
					{
						m_pDM->SetCellData(_T("����"), _T("V"), y );
					}
				}
			}
		}
		else
		{
			m_pPrintDM->SetCellData( _T("����"), _T(""), nIdx  );
		}
	}

	CBO_ACQ_CheckInDlg dlg(this);
	dlg.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	if(IDCANCEL == dlg.DoModal()) return ;

	CString strGroupNo, strSortDesc;
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();	
	api.SetTempPtr(_T("BOOK_DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("�׷�����"), (DWORD*)m_pSearchDlg->GetGroupCollection());
	ids = api.SPFExecutePath(_T("�˼��Ϸ��ĵ���ΰ�"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ ����ΰ�ó�� �� �� �����ϴ�."));
		return;
	}
	ESLAfxMessageBox(_T("������ �ڷḦ ����ΰ�ó�� �Ͽ����ϴ�."));
	m_pDM->FreeData();
	m_pPrintDM_Species->FreeData();
	m_pPrintDM_Volume->FreeData();
	INT nSelect = m_tabOrderData.GetCurSel();
	SetTabSelect(nSelect);
}

INT CBO_ACQ_OrderDataMgr::CheckSelectDataValid()
{
	CESL_DataMgr * pDM = GetActiveDM();
	CESL_Grid * pGrid = GetActiveGrid();
	
	if(pDM->GetRowCount() == 0) return -1;

	if(!pGrid) return -1;
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	ids = pGrid->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}

CESL_DataMgr * CBO_ACQ_OrderDataMgr::GetActiveDM()
{
	INT nCurSel = m_tabOrderData.GetCurSel();
	if(nCurSel == 0) return m_pPrintDM_Species;
	return m_pPrintDM_Volume;
}

CESL_Grid * CBO_ACQ_OrderDataMgr::GetActiveGrid()
{
	INT nCurSel = m_tabOrderData.GetCurSel();
	if(nCurSel == 0) return m_pGrid_Species;
	return m_pGrid_Volume;
}


//##########################################################################
// [ �Լ����� ]
// - ��/������ �׸����� �˾��޴��� �����ϰ�, 
//   ������ �׸��忡�� _T("��ǰå��")�� �׸���󿡼� ���� �����ϰ� �Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. ��ȭ�� �׸��忡 �߰��� �˾��޴��� �Է��Ѵ�.
// 2. ��ȭ�� �׸��忡 �˾��޴��� �߰��Ѵ�.
// 3. ��ȭ�� �׸��忡 �߰��� �˾��޴��� �Է��Ѵ�.
// 4. ��ȭ�� �׸��忡 �˾��޴��� �߰��Ѵ�.
// 5. ��ȭ�� �׸��忡 ������ �� �ִ� į���� _T("��ǰå��") �ϳ������� �����Ѵ�.
//
//##########################################################################
VOID CBO_ACQ_OrderDataMgr::InitPopupMenuItem()
{
	INT ids = 0;
	CBO_ACQ_Business_Api api(this);
	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();
	
	//=======================================================================	
	// 1. ��ȭ�� �׸��忡 �߰��� �˾��޴��� �Է��Ѵ�.
	//		- ��ǰó��, �̳�ó��, ���泻������
	//=======================================================================	
	RemoveListArray.Add(_T("��ǰó��"));
	RemoveListArray.Add(_T("�̳�ó��"));
	RemoveListArray.Add(_T("���泻������"));

	//=======================================================================	
	// 2. ��ȭ�� �׸��忡 �˾��޴��� �߰��Ѵ�.
	//=======================================================================	
	ids = m_pGrid_Species->InitPopupMenuItem();
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("��-�׸���ȭ���� �˾��޴��� �ʱ�ȭ �� �� �����ϴ�."));
		return;
	}
	ids = m_pGrid_Species->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("��-�׸���ȭ���� �˾��޴��� ����� �� �����ϴ�."));
		return ;
	}

	//=======================================================================	
	// 3. ��ȭ�� �׸��忡 �߰��� �˾��޴��� �Է��Ѵ�.
	//		- ������ ����Ʈ�� ��� �����Ŀ�
	//		- �ֹ���Ҹ� �߰��Ѵ�.
	//=======================================================================	
	RemoveListArray.RemoveAll();
	RemoveListArray.Add(_T("�ֹ����"));

	//=======================================================================	
	// 4. ��ȭ�� �׸��忡 �˾��޴��� �߰��Ѵ�.
	//=======================================================================	
	m_pGrid_Volume->InitPopupMenuItem();
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("��-�׸���ȭ���� �˾��޴��� �ʱ�ȭ �� �� �����ϴ�."));
		return;
	}

	m_pGrid_Volume->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("��-�׸���ȭ���� �˾��޴��� ����� �� �����ϴ�."));
		return ;
	}

	//=======================================================================	
	// 5. ��ȭ�� �׸��忡 ������ �� �ִ� į���� _T("��ǰå��") �ϳ������� �����Ѵ�.
	//=======================================================================	
	m_pGrid_Volume->m_bGridEdit = TRUE;
	m_pGrid_Volume->SELEDITOR_RemoveAllColumns();
	m_nEditCol = api.GetGridColumnIdx(m_pGrid_Volume, _T("��ǰå��"));
	m_pGrid_Volume->SELEDITOR_AddColumn(m_nEditCol);
}
//###########################################################################
// [ �Լ����� ]
// - ���ٹ�ư�� �����Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. ���� �׸��尡 ������ ������ �˱� ���� �ǹ�ȣ�� ���´�.
// 2. �������϶��� ������ ��ư�̸��� �Է��Ѵ�.
// 3. �������϶��� ������ ��ư�̸��� �Է��Ѵ�.
// 4. ��/������ ��� ������ �Ǵ��Ͽ� ���ٹ�ư�� �����϶�� �޽����� ������.
//###########################################################################
VOID CBO_ACQ_OrderDataMgr::SetToolBarBtnByTabSelect()
{
	INT i, nCurSel;
	CStringArray arrSpeciseBtnAlias, arrVolumeBtnAlias;

	//=======================================================================	
	// 1. ���� �׸��尡 ������ ������ �˱� ���� �ǹ�ȣ�� ���´�.
	//=======================================================================	
	nCurSel = m_tabOrderData.GetCurSel();
	

	//=======================================================================	
	// 2. �������϶��� ������ ��ư�̸��� �Է��Ѵ�.
	//=======================================================================	
	arrSpeciseBtnAlias.Add(_T("��ǰó��"));		arrSpeciseBtnAlias.Add(_T("��ǰ���"));
	arrSpeciseBtnAlias.Add(_T("�˼�ó��"));

	//=======================================================================	
	// 3. �������϶��� ������ ��ư�̸��� �Է��Ѵ�.
	//=======================================================================	
	arrVolumeBtnAlias.Add(_T("�ֹ����"));
	

	//=======================================================================	
	// 4. ��/������ ��� ������ �Ǵ��Ͽ� ���ٹ�ư�� �����϶�� �޽����� ������.
	//=======================================================================	
	// 13/03/27 �ڴ�� : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2�� �����۾�
	//                   ������ �ǿ��� ��ǰó��/��ǰ���/�˼�ó�� ���ٰ� ������ �ʴ� ���� ����
/*//BEFORE-----------------------------------------------------------------------
	if(nCurSel == 0)
	{
		for(i=0 ; i<arrSpeciseBtnAlias.GetSize() ; i++)
			pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&arrSpeciseBtnAlias[i], TRUE);
		for(i=0 ; i<arrVolumeBtnAlias.GetSize() ; i++)
			pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&arrVolumeBtnAlias[i], FALSE);
	}
	else if(nCurSel == 1)
	{
		for(i=0 ; i<arrVolumeBtnAlias.GetSize() ; i++)
			pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&arrVolumeBtnAlias[i], TRUE);
	}
*///AFTER------------------------------------------------------------------------
	// ������ ���� ��쿡�� ǥ���Ѵ�
	for (i=0; i<arrSpeciseBtnAlias.GetSize(); i++)
	{
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&arrSpeciseBtnAlias[i], (nCurSel==0));
	}
	// ������ ���� ��쿡�� ǥ���Ѵ�
	for (i=0; i<arrVolumeBtnAlias.GetSize(); i++)
	{
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&arrVolumeBtnAlias[i], (nCurSel==1));
	}
/*//END------------------------------------------------------------------------*/
}

INT CBO_ACQ_OrderDataMgr::OrderCancelByDivno()
{
	CGroupCollection* pGroupCollection = NULL;
	COrderCancelDivnoSelectDlg dlg(this);
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	dlg.DoModal();
	pGroupCollection = dlg.GetOrderCancelGroupCollection();
	if( pGroupCollection->GetGroupCount() == 0 )	return 0;
	
	//JOB.2019.0073 : �浹 License 
	//�ֹ���� ���� ��������
	//ShowOrderCancelMailMgr(CANCEL, AUTO, pGroupCollection);

	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("DM_BOOK"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("DM_SPECIES"), (DWORD*)m_pPrintDM_Species);
	api.SetTempPtr(_T("DM_VOLUME"), (DWORD*)m_pPrintDM_Volume);
	api.SetTempPtr(_T("GROUPINFO"), (DWORD*)pGroupCollection);
	INT ids = api.SPFExecutePath(_T("�׷��ڷ����"));
	
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�ֹ�����ڷḦ ������ �� �����ϴ�."));
		return -1;
	}
	INT nCurSel = m_tabOrderData.GetCurSel();
	DeleteDmRow( nCurSel );

	

	
	SetTabSelect(nCurSel);
	return 0;
}

INT CBO_ACQ_OrderDataMgr::OrderCancelBySelData()
{
	INT ids = this->CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return -1;
	}

	CDivisionNo_Select_Dlg dlg(this);
	dlg.SetAcqYear(m_pSearchDlg->GetGroupCollection()->GetAcqYear());
	dlg.SetWorkStatus(_T("���Դ��"));
	dlg.SetSourceDM(GetActiveDM());
	dlg.SetSourceGrid(GetActiveGrid());
	if(m_pInfo->GetKIIUPClientVersion() == 'L') dlg.SetAllowNULLGroup(TRUE);
	if(IDCANCEL == dlg.DoModal()) return 0;

	CGroupInfo * pGroup = dlg.GetSelGroup();
	CESL_DataMgr * pDM = GetActiveDM();
	CESL_Grid * pGrid = GetActiveGrid();
	CStringArray arrAcqKey;
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("DM"), (DWORD*)pDM);
	api.SetTempPtr(_T("BOOK_DM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("GRID"), (DWORD*)pGrid);
	api.SetTempPtr(_T("GROUP"), (DWORD*)pGroup);
	api.SetTempPtr(_T("ACQKEYLIST"), (DWORD*)&arrAcqKey);
	api.SetTempData(_T("BOOK_CNT_ALIAS"), _T("BP_����å��"));
	
	ids = api.SPFExecutePath(_T("�����ڷ��ֹ����"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("������ �ڷḦ �ֹ���� �� �� �����ϴ�."));
		return -1;
	}

	//�ֹ���� ���� ��������
	CString strBookKeyList = GetBookKeyList(&arrAcqKey);
	if(!strBookKeyList.IsEmpty())
	{
		//JOB.2019.0073 : �浹 License
// 		COrderMailMgr mailMgr(this);
// 		mailMgr.SetRecKeyList(strBookKeyList);
// 		mailMgr.SetOperationType(CANCEL);
// 		mailMgr.SetOpenType(AUTO);
// 		mailMgr.SetAcqYear(m_pSearchDlg->GetGroupCollection()->GetAcqYear());
// 		mailMgr.DoModal();
	}
	
	INT nCurSel = m_tabOrderData.GetCurSel();
	SetTabSelect(nCurSel);
	return 0;
}

CString CBO_ACQ_OrderDataMgr::GetBookKeyList(CStringArray *pArrAcqKey)
{
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	CString strTmpData, strQuery;
	INT cnt = pArrAcqKey->GetSize();
	if(cnt == 0) return _T("");

	//=====================================================
	//2009.02.23 UPDATE BY PJW : 1000�� �̻��ϰ�� ����� ������ ������.
// 	for(INT i=0 ; i<cnt ; i++)
// 	{
// 		if(strTmpData.IsEmpty()) strTmpData = pArrAcqKey->GetAt(i);
// 		else strTmpData += _T(", ") + pArrAcqKey->GetAt(i);
// 	}
	CString strInQuery;

	for(INT i=0 ; i<cnt ; i++)
	{
		if(strTmpData.IsEmpty()) 
		{
			strTmpData = pArrAcqKey->GetAt(i);
		}
		else 
		{
			if( i%1000 == 999 )
			{				
				if( strInQuery.IsEmpty() )
				{
					strInQuery.Format(_T("ACQ_KEY IN (%s)"), strTmpData);
				}
				else
				{
					strInQuery = strInQuery + _T("OR ACQ_KEY IN ( "), strTmpData + _T(" )");
				}
				strTmpData = pArrAcqKey->GetAt(i);
			}
			else
			{
				strTmpData += _T(", ") + pArrAcqKey->GetAt(i);
			}
		}
	}

	if( !strTmpData.IsEmpty() )
	{
		if( strInQuery.IsEmpty() )
		{
			strInQuery.Format(_T("ACQ_KEY IN (%s)"), strTmpData);
		}
		else
		{
			strInQuery = strInQuery + _T("OR ACQ_KEY IN ( ") + strTmpData + _T(" )");
		}
	}
	
	strTmpData.Format(_T("SELECT ACQ_KEY FROM BO_BOOK_TBL WHERE %s"), strInQuery);
	//=====================================================

	strQuery.Format(_T("SELECT REC_KEY FROM BO_BOOK_TBL WHERE ACQ_KEY IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE"), strTmpData);
	tmpDM.RestructDataManager(strQuery);
	cnt = tmpDM.GetRowCount();
	if(cnt == 0) return _T("");
	strTmpData.Empty();
	for(i=0 ; i<cnt ; i++)
	{
		if(strTmpData.IsEmpty()) strTmpData = tmpDM.GetCellData(i, 0);
		else strTmpData += _T(", ") + tmpDM.GetCellData(i, 0);
	}
	return strTmpData;
}

VOID CBO_ACQ_OrderDataMgr::ViewCheckInCompleteData()
{
	CViewTransferDataDlg dlg(this);
	dlg.DoModal();
}

VOID CBO_ACQ_OrderDataMgr::ShowOrderCancelMailMgr(INT nOpType, INT nOpenType, CGroupCollection * pGroupCollection)
{
	//JOB.2019.0073 : �浹 License
// 	COrderMailMgr mailMgr(this);
// 	if(pGroupCollection == NULL) pGroupCollection = m_pSearchDlg->GetGroupCollection();
// 	
// 	mailMgr.SetParentGroupCollection(pGroupCollection);
// 	mailMgr.SetOperationType(nOpType);
// 	mailMgr.SetOpenType(nOpenType);
// 	mailMgr.SetAcqYear(pGroupCollection->GetAcqYear());
// 	mailMgr.DoModal();
}

INT CBO_ACQ_OrderDataMgr::MakePrintDM(CString strSortField)
{
	if(m_pSearchDlg->GetGroupCollection()->GetGroupCount() == 0) return 0;
	CString strGroupInfo;
	m_pSearchDlg->GetGroupCollection()->GetGroupInfo(strGroupInfo);
	if(strGroupInfo == _T("��ü�ڷ�"))
	{
		ESLAfxMessageBox(_T("���� ������ ������ �����ϴ�. �˻��� ���Ͽ� Ư�� ������ ������ �ֽʽÿ�."));
		return -1;
	}

	CESL_DataMgr * pDM = FindDM(_T("DM_�ֹ��ڷ����_���"));
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();
	api.SetTempPtr(_T("��¿�DM"), (DWORD*)pDM);
	api.SetTempPtr(_T("åDM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("GroupCollection"), (DWORD*)m_pSearchDlg->GetGroupCollection());
	api.SetTempData(_T("SORT_FIELD"), strSortField.GetBuffer(0));
	INT ids = api.SPFExecutePath(_T("�ֹ������¿�DM����"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("��¿� �����͸� ������ �� �����ϴ�."));
		return -1;
	}
	return 0;
}

INT CBO_ACQ_OrderDataMgr::DeleteOrderSpeciesData()
{
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();		api.InitTempPtr();
	api.SetTempPtr(_T("DM"), (DWORD*)GetActiveDM());
	api.SetTempPtr(_T("BookDM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("Grid"), (DWORD*)GetActiveGrid());
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_ORDER);
	api.SetTempData(_T("ACQ_TYPE"), (INT)PURCHASE);
	api.SetTempData(_T("����Ÿ��"), SPECIES_DELETE);
	api.SetTempData(_T("å���˸��ƽ�"), _T("UDF_å��"));
	api.SetTempData(_T("���ݾ˸��ƽ�"), _T("UDF_��ȯ����������"));
	api.SetTempData(_T("��������"), _T(""));
	api.SetTempPtr(_T("��Ű����Ʈ"), NULL);
	api.SetTempPtr(_T("��������Ű����Ʈ"), NULL);
	api.SetTempPtr(_T("���Ϸù�ȣ����Ʈ"), NULL);
	INT ids = api.SPFExecutePath(_T("����"));

	return ids;
}

INT CBO_ACQ_OrderDataMgr::DeleteOrderVolumeData()
{
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();		api.InitTempPtr();
	api.SetTempPtr(_T("DM"), (DWORD*)GetActiveDM());
	api.SetTempPtr(_T("BookDM"), (DWORD*)m_pDM);
	api.SetTempPtr(_T("Grid"), (DWORD*)GetActiveGrid());
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_ORDER);
	api.SetTempData(_T("ACQ_TYPE"), (INT)PURCHASE);
	api.SetTempData(_T("����Ÿ��"), (INT)VOLUME_DELETE);
	api.SetTempData(_T("å���˸��ƽ�"), _T("UDF_å��"));
	api.SetTempData(_T("���ݾ˸��ƽ�"), _T("UDF_��ȯ���ΰ���"));
	api.SetTempData(_T("��������"), _T(""));
	api.SetTempPtr(_T("��Ű����Ʈ"), NULL);
	api.SetTempPtr(_T("��������Ű����Ʈ"), NULL);
	api.SetTempPtr(_T("���Ϸù�ȣ����Ʈ"), NULL);
	INT ids = api.SPFExecutePath(_T("����"));

	return ids;
}

///////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - Progress Ctrl Dialog â ���� �� �ʱ�ȭ
///////////////////////////////////////////////////////////////////////////
VOID CBO_ACQ_OrderDataMgr::ShowDataProcessingDlg()
{
	if(m_pDataProcessingDlg == NULL) m_pDataProcessingDlg = new CDataProcessingDlg(this);

	if( m_pDataProcessingDlg->GetCreate() == FALSE )
	{
		if( m_pDataProcessingDlg->Create(this) ==FALSE )
		{
			delete m_pDataProcessingDlg;
			m_pDataProcessingDlg = NULL;
			return;
		}
	}
	m_pDataProcessingDlg->CenterWindow();
	m_pDataProcessingDlg->ShowWindow(SW_SHOW);
	m_pDataProcessingDlg->UpdateWindow();
	//���α׷��� ��Ʈ�� �ʱ�ȭ
	m_pDataProcessingDlg->ShowHideCancelButton(FALSE);
//	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, 0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
}

VOID CBO_ACQ_OrderDataMgr::DeleteDataProcessingDlg()
{
	if( m_pDataProcessingDlg )
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

//###########################################################################
// [ �Լ����� ]
// - SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE GROUP_2='%s' AND GROUP_3='%s' AND CLASS_ALIAS='%s'
// VAlUE_NAME ���� ���ͼ� �������ش�.
//
// [ �̴Ͻ��� ]
// 1. ������ �ۼ�
// 2. ���� ����� �޾ƿ� �������ش�.
//###########################################################################
CString CBO_ACQ_OrderDataMgr::GetCommonManageValue(CString strGroup2, CString strGroup3, CString strClassAlias)
{
	CString strValue, strQuery, strErrMsg= _T("");
	INT		ids = 0;
	
	//=======================================================================
	// 1. ������ �ۼ�
	//		- ESL_MANAGE_TBL���� VALUE_NAME�� ���´�.
	//=======================================================================
	strQuery.Format(_T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE GROUP_2='%s' AND GROUP_3='%s' AND CLASS_ALIAS='%s'  AND MANAGE_CODE = UDF_MANAGE_CODE"), 
		strGroup2, strGroup3, strClassAlias);
	
	//=======================================================================
	// 2. ���� ����� �޾ƿ� �������ش�.
	//=======================================================================
	ids = m_pDM->GetOneValueQuery(strQuery, strValue);
	if( ids < 0 )
	{
		switch (ids)
		{
		case -4004:	strErrMsg.Format(_T("[ %d ] : �����ͺ��̽��� ���������� �ʽ��ϴ�.\n%s"), ids, strQuery );
			break;
		case -4005:	strErrMsg.Format(_T("[ %d ] : ���ǹ�(Query)�� ���� ����� ������ �ֽ��ϴ�.\n%s"), ids, strQuery );
			break;
		case -4006:	strErrMsg.Format(_T("[ %d ] : ���ǹ�(Query)�� ���� ����� �ʹ� �����ϴ�.\n%s"), ids, strQuery );
			break;
		case -4007:	strErrMsg.Format(_T("[ %d ] : ���ǹ�(Query)�� ���� ����� �����ϴ�.\n%s"), ids, strQuery );
			break;
		case -4008:	strErrMsg.Format(_T("[ %d ] : �˻��ʵ尡 ����ʵ庸�� �����ϴ�.\n%s"), ids, strQuery );
			break;
		case -4999:	strErrMsg.Format(_T("[ %d ] : Oracle�� ���������� Ȯ���Ͻʽÿ�.\n%s"), ids, strQuery );
			break;
		default:	strErrMsg.Format(_T("[ %d ] : Unknown Error"), ids);
			break;
		}
		ESLAfxMessageBox(strErrMsg);
		return _T("");
	}

	return strValue;
}

INT CBO_ACQ_OrderDataMgr::GetSpeciesCnt(CESL_DataMgr *pDM)
{
	CMapStringToString tmpMap;
	tmpMap.RemoveAll();

	INT nRowCnt = pDM->GetRowCount();
	CString strTmpData;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pDM->GetCellData(_T("BP_��������KEY"), i);
		tmpMap.SetAt(strTmpData, _T(""));
	}

	return tmpMap.GetCount();
}

INT CBO_ACQ_OrderDataMgr::GetVolumeCnt(CESL_DataMgr *pDM)
{
	CMapStringToString tmpMap;
	tmpMap.RemoveAll();
	
	INT nRowCnt = pDM->GetRowCount();
	CString strTmpData;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pDM->GetCellData(_T("BP_��������KEY"), i);
		strTmpData += _T("-") + pDM->GetCellData(_T("BB_���Ϸù�ȣ"), i);
		tmpMap.SetAt(strTmpData, _T(""));
	}
	
	return tmpMap.GetCount();
}

VOID CBO_ACQ_OrderDataMgr::ShowSortDlg(INT nType/*=1*/)
{
	CDataSortDlg dlg(this);

	if( m_pSortItemSet == NULL || GetActiveDM() == NULL ) 
	{
		ESLAfxMessageBox(_T("����ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		return;
	}
	if( GetActiveDM()->GetRowCount() == 0 )
	{
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}
	
	dlg.SetMainDM(GetActiveDM());
	dlg.SetSortItemSet(m_pSortItemSet);
	dlg.SetOpenMode(nType);
	if(IDOK == dlg.DoModal() && nType > 0)		DisplayGrid();
}

UINT CBO_ACQ_OrderDataMgr::OnSetOrderCondition(LPARAM lParam, WPARAM wParam)
{
	ShowSortDlg(-1);
	return 0;
}

//###########################################################################
// [ �Լ����� ]
// - �����׸����� ���� ������ �о� ����Ŭ������ �����ϰ� ��������� ����Ѵ�.
// - �����׸��� ������ �� ����Ŭ������ �˻�Ŭ������ �Ѱ� �˻��� ���ĵǵ��� �Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. CSortItemSet Ŭ����(�����׸���Ŭ����)�� �����Ͽ� ��������� ����Ѵ�.
// 2. ������ ����Ŭ������ �����ϰ��� �ϴ� �ش�(����) �׸��带 �������ش�.
// 3. ����Ŭ������ DM�� ���Ϸ� ���� �ʱ�ȭ �Ѵ�.
// 4. ������ �����׸��� ����� ������ ���� Ŭ������ �����Ѵ�.
// 5. ����Ŭ������ m_HeadSortList�� ���Գ⵵, ��������, �׷��ȣ�� �߰��Ѵ�.
// 6. ����Ŭ������ m_TailSortList�� �����ηù�ȣ, ���Ϸù�ȣ�� �߰��Ѵ�.
// 7. UDF�� Alias�� ���ν�Ų��.
// 8. 4������ ������ ������ �о�鿩 �����׸���Ŭ���� DM�� �߰��Ѵ�.
// 9. ���ǵ� �����׸���Ŭ������ �˻�âŬ������ �����׸����� �����Ѵ�.
//###########################################################################
BOOL CBO_ACQ_OrderDataMgr::InitSortItem(CESL_Grid * pGrid, CString strGridAlias)
{
	//=======================================================================
	// ��������
	//=======================================================================
	INT		ids = 0;
	CStringArray* pTmpData = NULL;	
	
	
	//=======================================================================
	// 1. CSortItemSet Ŭ����(�����׸���Ŭ����)�� �����Ͽ� ��������� ����Ѵ�.
	//=======================================================================
	if( m_pSortItemSet )	// CSortItemSet
	{
		delete m_pSortItemSet;
		m_pSortItemSet = NULL;
	}
	m_pSortItemSet = new CSortItemSet(this);

	//=======================================================================
	// 2. ������ ����Ŭ������ �����ϰ��� �ϴ� �ش�(����) �׸��带 �������ش�.
	//=======================================================================
	m_pSortItemSet->SetGrid(pGrid);

	//=======================================================================
	// 3. �����׸���Ŭ������ DM�� ���Ϸ� ���� �ʱ�ȭ �Ѵ�.
	//		- DM������ �ι�° ������ DM_�����׸���.DM �����̴�.
	//		- �ش��Լ��� CESL_DataMgr::InitDMPKFromFile()�� ȣ���Ѵ�.
	//=======================================================================
	ids = m_pSortItemSet->InitDataManager(m_pDM->CONNECTION_INFO, _T("DM_�����׸���"));
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("[DM_�����׸���]�� �ʱ�ȭ �� �� �����ϴ�."));
		return FALSE;
	}

	//=======================================================================
	// 4. ������ �����׸��� ����� ������ ���� Ŭ������ �����Ѵ�.
	//		- Cfg/SortConfig/SMAlias_GridAlias.cfg ���Ͽ� ���ǵǾ� �ִ�.
	//=======================================================================
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(),strGridAlias);


	//=======================================================================
	// 5. �����׸���Ŭ������ m_HeadSortList�� ���Գ⵵, ��������, �׷��ȣ�� �߰��Ѵ�.
	//=======================================================================
	pTmpData = m_pSortItemSet->GetHeadSortList();
	pTmpData->Add(_T("BP_���Գ⵵"));
	pTmpData->Add(_T("IBS_��������"));
	pTmpData->Add(_T("BP_�׷��ȣ"));

	//=======================================================================
	// 6. �����׸���Ŭ������ m_TailSortList�� �����ηù�ȣ, ���Ϸù�ȣ�� �߰��Ѵ�.
	//=======================================================================
	pTmpData = m_pSortItemSet->GetTailSortList();
	pTmpData->Add(_T("BP_�����Ϸù�ȣ"));
	// BB_���Ϸù�ȣ�� ���������� �ش�Ǵ� �����̴�.
	// 2005-10-12 ������
	//
	if( strGridAlias == _T("MainGrid_Volume") )	pTmpData->Add(_T("BB_���Ϸù�ȣ"));


	//=======================================================================
	// 7. UDF�� Alias�� ���ν�Ų��.
	//		- �������м���, �Է±��м���, ����ó��ȣ����, ������, ����������
	//=======================================================================
	CMapStringToString * pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	
	//2009.05.14 UPDATE BY LKS : ������ �ƴ� �ڵ尪���� ������ �Ǿ� ���� �� �ּ�ó��
	//2010.04.26 UPDATE BY PJW : �������� ������ �ƴ� �ڵ尪���� ���ĵǴ°��� ���� , ������ �븮 �䱸����
//	pMap->SetAt(_T("BP_��������"), _T("UDF_�������м���"));
	pMap->SetAt(_T("UDF_�������м���"), _T("BP_��������"));
	//===================================================
	//pMap->SetAt(_T("UDF_����ó��ȣ����"), _T("BP_����ó��ȣ"));
	pMap->SetAt(_T("UDF_�Է±��м���"), _T("BP_�Է±���"));
	// 2005-04-06
	pMap->SetAt(_T("UDF_������"), _T("BP_����"));
	pMap->SetAt(_T("UDF_����������"), _T("BP_���ְ���"));
	

	//=======================================================================
	// 8. 4������ ������ ������ �о�鿩 �����׸�Ŭ���� DM�� �߰��Ѵ�.
	//		- �����׸��� �����Ǿ� ���� �ʴٰ� �ص� ������ �ƴϹǷ� �׳� �н��Ѵ�.
	//=======================================================================
	ids = m_pSortItemSet->ReadSortItem();	
	if( ids < 0 )
	{
	//	ESLAfxMessageBox(_T("[DM_�����׸���]�� ������ ���� �����ϴ�."));
	//	return FALSE;
	}

	//=======================================================================
	// 9. ���ǵ� ���� Ŭ������ �˻�âŬ������ �����׸����� �����Ѵ�.
	//=======================================================================
	m_pSearchDlg->SetDataSortItem(m_pSortItemSet);

	return TRUE;
}

VOID CBO_ACQ_OrderDataMgr::FileImport()
{
	CBO_ACQ_FileImportMgr  ImportMgr(this);

	ImportMgr.SetSpeciesType(BO_ORDER);		//��Ÿ��
	// ������ �����Ѵ�.
	INT ids = ImportMgr.FileImportProc();
	if(ids < 0) return ;

//	ids =m_pDM->RefreshDataManager(ImportMgr.m_strWhereStmt);

	if(ids == 0)
		AfxMessageBox(_T("���� ������ �Ϸ��Ͽ����ϴ�."));	
	else return ;

}

VOID CBO_ACQ_OrderDataMgr::FileExport()
{
	INT ids;
	CString strAcqYear, strGroupInfo, strWorkCode, strQuery, strResult;
	CDataExportDlg Dlg(this);
	ids = Dlg.DoModal();
	if( ids == IDCANCEL ) return ;

	strAcqYear = Dlg.GetAcqYear();
	strGroupInfo = Dlg.GetSeqNo();
	strWorkCode = Dlg.GetWorkCode();

	if(strAcqYear.IsEmpty() || strGroupInfo.IsEmpty() || strWorkCode.IsEmpty())
	{
		AfxMessageBox(_T("���� ������ ��� �Է��ϼž� �մϴ�."));
		return ;
	}
	
	strQuery.Format(_T("SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL ")
								_T("WHERE ACQ_YEAR = %s AND SEQ_NO = %s AND WORK_CODE = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"),
								strAcqYear, strGroupInfo, strWorkCode);
	strResult.Empty();
	ids = m_pDM->GetOneValueQuery(strQuery, strResult);

	if( ids <0 || strResult.IsEmpty()) 
	{
		AfxMessageBox(_T("�Է����ǿ� �´� ������ �������� �ʽ��ϴ�."));
		return ;
	}

	CBO_ACQ_FileExportMgr ExportMgr;
	
	ExportMgr.SetAcqYear(strAcqYear);
	ExportMgr.SetGroupInfo(strGroupInfo);
	ExportMgr.SetWorkCode(strWorkCode);
	ExportMgr.SetSpeciesType(BO_ORDER);		//��Ÿ��
	
	// ������ �����Ѵ�.
	ids = ExportMgr.FileExportProc();

	if(ids == 0)
		AfxMessageBox(_T("���� ������ �Ϸ��Ͽ����ϴ�."));

}

CString CBO_ACQ_OrderDataMgr::TruncPrice(CESL_DataMgr *pDM, DOUBLE dPrice)
{
	// 2004.12.01 ������å�� ���� ��������������� �о�ͼ� ������ �ڸ�����ŭ ǥ���ϵ��� �Ѵ�.
	CString strTmpData, strTmp, strValue;
	INT nPos = -1;

	strTmp.Format(_T("%.2f"), dPrice);  // 2005-01-13
	nPos = strTmp.Find(_T("."));

	// 2005-01-13
	if(m_strPosNumber == _T("1"))  
		strTmpData = strTmp.Left(nPos+2);
	else if(m_strPosNumber == _T("2"))  
		strTmpData = strTmp.Left(nPos+3);
	else  // m_strPosNumber == _T("0") || m_strPosNumber.Empty()
		strTmpData = strTmp.Left(nPos);

	return strTmpData;
}

VOID CBO_ACQ_OrderDataMgr::ChangeMarc()
{
	//===============================================
	//20080627 ADD BY PJW : ��ũ��ü�� �������׸��忡�� �ϵ��� �Ѵ�.
	PARENT_TYPE nParentType = SPECIES;
	INT nSelect;
	nSelect = m_tabOrderData.GetCurSel();
	if		( nSelect == 0 ) 	nParentType = SPECIES;
	else if	( nSelect == 1 ) 	nParentType = VOLUME;
	if( nParentType == VOLUME )
	{
		ESLAfxMessageBox(_T("������ �׸���ȭ�鿡�� ��ũ��ü�� �� �ּ���."));
		return ;
	}
	//===============================================
	// ������, �Ǵ��� ó�� �ʿ�
	CMARC_CHANGER_01 dlg(this);
	//===============================================
	//20080627 UPDATE BY PJW : ��ũ��ü�� �������׸��忡�� �ϵ��� �Ѵ�.
	//2008.07.02 UPDATE BY PJW : ��ǥå�������� �����Ѵ�.
	//dlg.SetParentMgr(_T("IBS_��KEY"), _T("CM_�ֹ��ڷ����"), _T("MainGrid_Volume"), _T("DM_�ֹ��ڷ����_��"), _T("BS_MARC"));
	dlg.SetParentMgr(_T("IBS_��KEY"), _T("CM_�ֹ��ڷ����"), _T("MainGrid"), _T("DM_�ֹ��ڷ����_��"), _T("BS_MARC"), _T("DM_�ֹ��ڷ����"));
	//===============================================

	//===============================================
	//2008.08.22 UPDATE BY PJW : �������������ϰ� �׸��带 ���÷����Ѵ�.
// 	if ( dlg.DoModal() == IDOK )
//	{
//		AllControlDisplay(_T("CM_�ֹ��ڷ����"));
//		m_pDM->RefreshPositionInfo();		
//	}
	m_pGrid_Species->SetProtectSelectColumn(TRUE);
	if ( dlg.DoModal() == IDOK )
	{
		AllControlDisplay(_T("CM_�ֹ��ڷ����"));
		m_pDM->RefreshPositionInfo();	
	}
	m_pGrid_Species->Display();
	m_pGrid_Species->SetProtectSelectColumn(FALSE);
	//===============================================
	
//	m_pSearchDlg->ShowSearchDlg();
	
	//===============================================
	//20080627 UPDATE BY PJW : ��ũ��ü���� �ֹ��ڷ����_��,�ֹ��ڷ����_��,�ֹ��ڷ���� DM�� Refresh�Ѵ�.
//	INT nRowCnt;
//	CString strQuery;
//	CString strRecKeys;
//	CString strTempRecKeys;
//	CString strRecKey;
//	nRowCnt = m_pDM->GetRowCount();
//	for( INT i = 0 ; i < nRowCnt ; i++ )
//	{
//		
//	}

//	m_pDM				= FindDM(_T("DM_�ֹ��ڷ����")			);
//	m_pPrintDM_Species	= FindDM(_T("DM_�ֹ��ڷ����_��")		);
//	m_pPrintDM_Volume	= FindDM(_T("DM_�ֹ��ڷ����_��")		);	
	
//	INT nRowCnt;
//	CString strQuery;
//	CString strRecKeys;
//	CString strTempRecKeys;
//	CString strRecKey;
//
//	nRowCnt = m_pPrintDM_Volume->GetRowCount();
//	for( INT i = 0 ; i < nRowCnt ; i++ )
//	{
//		m_pPrintDM_Volume->GetCellData(_T("BB_åKEY"), i, strRecKey);
//		if( strRecKeys.IsEmpty() )
//		{
//			strRecKeys = strRecKey;
//		}
//		else
//		{
//			strTempRecKeys = strRecKeys;
//			strRecKeys.Format(_T("%s, %s"), strTempRecKeys, strRecKey);
//		}
//	}
//
//	strQuery.Format(_T("B.REC_KEY IN( %s )"), strRecKeys );
//
//	m_pPrintDM_Volume->RefreshDataManager(strQuery);


//	//dlg.SetParentMgr(_T("IBS_��KEY"), _T("CM_�ֹ��ڷ����"), _T("MainGrid_Volume"), _T("DM_�ֹ��ڷ����_��"), _T("BS_MARC"));
//	CString strQuery;
//	//strQuery
//	m_pSearchDlg->GetWhereStmt(strQuery);
//	m_pDM->RefreshDataManager(strQuery);
//	m_pPrintDM_Species->RefreshDataManager(strQuery);
//	m_pPrintDM_Volume->RefreshDataManager(strQuery);
	//===============================================
}



//========================================================================
//20080605 ADD BY PJW : ��, �� �׸��忡 ���� DM�� ���� ���� �����Ѵ�.
INT CBO_ACQ_OrderDataMgr::SelectGridToDmChk( CESL_DataMgr* pDM		, 
											 CString strSpeciesKey	, 
											 CString strGrid		,
											 CString strChk			, 
											 CString strVolNo		)
{
	CString strTempKey;
	CString strTempVolNo;
	INT i;
	if( _T("���׸���") == strGrid )
	{
		for( i = 0 ; i < pDM->GetRowCount() ; i++ )
		{
			pDM->GetCellData( _T("IBS_��KEY"), i, strTempKey);
			if( strSpeciesKey == strTempKey )
			{
				pDM->SetCellData( _T("����"), strChk, i );
			}
		}
	}
	else
	{
		for( i = 0 ; i < pDM->GetRowCount() ; i++ )
		{
			pDM->GetCellData( _T("IBS_��KEY"), i, strTempKey);
			pDM->GetCellData( _T("BB_�����"), i, strTempVolNo);
			if( strSpeciesKey == strTempKey  &&  strVolNo == strTempVolNo)
			{
				pDM->SetCellData( _T("����"), strChk, i );
			}
		}
	}
	return 0;
}
//========================================================================

//========================================================================
//20080605 ADD BY PJW : ���콺 UP�� ���������� SelectGridToDmChk()ȣ��
void CBO_ACQ_OrderDataMgr::OnMouseUpgridORDERVOLUMEDATA(short Button, short Shift, long x, long y) 
{
	INT nSelectRow, nSelectCol;
	CString strChk;
	CString strSpeciesKey;
	CString strBookKey;
	CString strVolNo;
	CMSHFlexGrid *pGrid;	

	pGrid		= ( CMSHFlexGrid * )GetDlgItem( IDC_gridORDER_VOLUME_DATA );
	nSelectRow	= pGrid->GetRow();
	nSelectCol	= pGrid->GetCol();
	strChk		= pGrid->GetText();
	if(  0 >= nSelectRow )
	{
		return;
	}

	nSelectRow -= 1;

	if ( 1 == Button && 1 == nSelectCol ) 
	{
		if( _T("V") == strChk ) 
		{
			m_pActiveDM->SetCellData( _T("����"), strChk, nSelectRow );
			m_pActiveDM->GetCellData( _T("IBS_��KEY"), nSelectRow, strSpeciesKey);
			m_pActiveDM->GetCellData( _T("BB_�����"), nSelectRow, strVolNo);
			SelectGridToDmChk( m_pDM, strSpeciesKey, _T("�Ǳ׸���"), strChk,strVolNo);
		}
		else
		{
			m_pActiveDM->SetCellData( _T("����"), strChk, nSelectRow );
			m_pActiveDM->GetCellData( _T("IBS_��KEY"), nSelectRow, strSpeciesKey);
			m_pActiveDM->GetCellData( _T("BB_�����"), nSelectRow, strVolNo);
			SelectGridToDmChk( m_pDM, strSpeciesKey, _T("�Ǳ׸���"), strChk, strVolNo);
		
		}

	}
}
//========================================================================

//========================================================================
//20080605 ADD BY PJW : ���콺 UP�� ���������� SelectGridToDmChk()ȣ��
void CBO_ACQ_OrderDataMgr::OnMouseUpgridORDERDATA(short Button, short Shift, long x, long y) 
{
	INT nSelectRow, nSelectCol;
	CString strChk;
	CString strSpeciesKey;
	CString strBookKey;
	CMSHFlexGrid *pGrid;
	

	pGrid		= ( CMSHFlexGrid * )GetDlgItem( IDC_gridORDER_DATA );
	nSelectRow	= pGrid->GetRow();
	nSelectCol	= pGrid->GetCol();
	strChk		= pGrid->GetText();
	
	if(  0 >= nSelectRow )
	{
		return;
	}

	nSelectRow -= 1;

	if ( 1 == Button && 1 == nSelectCol ) 
	{
		if( _T("V") == strChk ) 
		{
			m_pActiveDM->SetCellData( _T("����"), strChk, nSelectRow );
			m_pActiveDM->GetCellData( _T("IBS_��KEY"), nSelectRow, strSpeciesKey);
			SelectGridToDmChk(m_pDM,strSpeciesKey,_T("���׸���"),strChk,_T(""));
		}
		else
		{
			m_pActiveDM->SetCellData( _T("����"), strChk, nSelectRow );
			m_pActiveDM->GetCellData( _T("IBS_��KEY"), nSelectRow, strSpeciesKey);
			SelectGridToDmChk(m_pDM,strSpeciesKey,_T("���׸���"),strChk,_T(""));
		
		}
	}
}
//========================================================================



//========================================================================
//20080605 ADD BY PJW : �׸��� ���� ����ɶ����� DM�� �������� ���� �Ѵ�.
void CBO_ACQ_OrderDataMgr::SetDmSelectToCancel( CESL_DataMgr* pCancelDM )
{
	INT nRow;
	nRow = pCancelDM->GetRowCount();
	for( INT i= 0 ; i<nRow ; i++ )
	{
		pCancelDM->SetCellData( _T("����"), _T(""), i );
	}
}
//========================================================================

//========================================================================
//20080605 ADD BY PJW : ó���� DM�� �����Ѵ�.
void CBO_ACQ_OrderDataMgr::DeleteDmRow( INT nSelectTab )
{
	CString strSelect;
	CString strActiveDMSpeciesKey;
	CString strNonActiveDMSpeciesKey;
	INT nActiveDMRowsCnt;
	INT nNonActiveDMRowsCnt;
	INT nChkCnt;
	INT i;
	INT j;

	if(nSelectTab == 0) 
	{
		//1. DM_�ֹ��ڷ����_���� ROW�� �����Ѵ�
		nActiveDMRowsCnt = m_pActiveDM->GetRowCount();
		for ( i=0 ; i < nActiveDMRowsCnt ; i++ )
		{	
			m_pActiveDM->GetCellData( _T("����"), i, strSelect );
			if( _T("V") == strSelect )
			{
				m_pActiveDM->GetCellData( _T("IBS_��KEY"), i, strActiveDMSpeciesKey );
				//2. DM_�ֹ��ڷ����_�� ROW�� �����Ѵ�
				nNonActiveDMRowsCnt = m_pPrintDM_Volume->GetRowCount();
				for( j = 0 ; j < nNonActiveDMRowsCnt ; j++)
				{
					m_pPrintDM_Volume->GetCellData( _T("IBS_��KEY"), j, strNonActiveDMSpeciesKey );
					if( strNonActiveDMSpeciesKey == strActiveDMSpeciesKey )
					{
						m_pPrintDM_Volume->DeleteRow(j);
						--nNonActiveDMRowsCnt;
						--j;
					}
				}
				m_pActiveDM->DeleteRow(i);
				--nActiveDMRowsCnt;
				--i;
			}
		}

		//3. DM_�ֹ��ڷ������ ROW�� �����Ѵ�
		//nActiveDMRowsCnt = m_pDM->GetRowCount();
		//for ( i=0 ; i < nActiveDMRowsCnt ; i++ )
		//{	
		//	m_pDM->GetCellData( _T("����"), i, strSelect );
		//	if( _T("V") == strSelect )
		//	{
		//		m_pDM->DeleteRow(i);
		//		--nActiveDMRowsCnt;
		//		--i;
		//	}
		//}			
	}
	else
	{
		//1. DM_�ֹ��ڷ����_�� ROW�� �����Ѵ�
		nActiveDMRowsCnt = m_pActiveDM->GetRowCount();
		for ( i=0 ; i < nActiveDMRowsCnt ; i++ )
		{	
			m_pActiveDM->GetCellData( _T("����"), i, strSelect );
			if( _T("V") == strSelect )
			{
				m_pActiveDM->DeleteRow(i);
				--nActiveDMRowsCnt;
				--i;
			}
		}

		//2. DM_�ֹ��ڷ����_���� ROW�� �����Ѵ�
		nNonActiveDMRowsCnt = m_pPrintDM_Species->GetRowCount();
		for( i = 0 ; i < nNonActiveDMRowsCnt ; i++)
		{
			nChkCnt = 0;
			m_pPrintDM_Volume->GetCellData( _T("IBS_��KEY"), i, strNonActiveDMSpeciesKey );
			for( j = 0 ; j < nActiveDMRowsCnt ; j++)
			{
				m_pActiveDM->GetCellData( _T("IBS_��KEY"), j, strActiveDMSpeciesKey );
				if( strNonActiveDMSpeciesKey == strActiveDMSpeciesKey )
				{
					++nChkCnt;
					break;
				}
			}

			if( nChkCnt == 0 )
			{
				m_pPrintDM_Species->DeleteRow(i);
				--nNonActiveDMRowsCnt;
				--i;
			}			
		}
		//3. DM_�ֹ��ڷ������ ROW�� �����Ѵ�
		//nActiveDMRowsCnt = m_pDM->GetRowCount();
		//for ( i=0 ; i < nActiveDMRowsCnt ; i++ )
		//{	
		//	m_pDM->GetCellData( _T("����"), i, strSelect );
		//	if( _T("V") == strSelect )
		//	{
		//		m_pDM->DeleteRow(i);
		//		--nActiveDMRowsCnt;
		//		--i;
		//	}
		//}	
	}
}
//========================================================================

VOID CBO_ACQ_OrderDataMgr::KSearch(CString WorkType, CString strAcqCode, CString strAcqYear, CString strWorkCode, CString strSeqNo)
{
	INT nSelect;
	CString strQuery;

	strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY AND P.REC_KEY = B.ACQ_KEY AND I.SPECIES_CLASS IN ('0', '1') AND B.WORKING_STATUS IN ('BOA112N','BOA113O') AND B.ACQ_CODE='%s'  AND P.ORDER_YEAR = '%s' AND I.WORK_CODE = '%s' AND P.ORDER_SEQ_NO IN (%s)")
			, strAcqCode, strAcqYear, strWorkCode, strSeqNo);
	
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("�ֹ��ڷḦ �������Դϴ�."));	
	BEGIN_SBL();
	m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	m_pPrintDM_Species->FreeData();
	m_pPrintDM_Volume->FreeData();
	
	DeleteDataProcessingDlg();	
	
	nSelect = m_tabOrderData.GetCurSel();

	MakeOrderDM(nSelect);

	END_SBL(m_pPrintDM_Volume->GetRowCount());

	m_pCM->SetControlMgrData(_T("�׷��ȣ"), strSeqNo);
	m_pCM->SetControlMgrData(_T("���Գ⵵"), strAcqYear);

	m_pSearchDlg->GetGroupCollection()->RemoveAll();
	m_pSearchDlg->GetGroupCollection()->SetAcqCode(strAcqCode);	
	m_pSearchDlg->GetGroupCollection()->AddGroupInfo(strAcqYear, strSeqNo, strWorkCode);	
	m_pSearchDlg->GetGroupCollection()->SetHaveGroupInfo(TRUE);
}
