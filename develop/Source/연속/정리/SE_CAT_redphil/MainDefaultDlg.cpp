// MainDefaultDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDefaultDlg.h"

// �������� Ű����
//#include _T("../../../����/Duplicate_Data_Search/DUP_KEY_SETTING.h")

// ���� ����
//#include _T("..\BO_CAT_DUP_FREE\BO_CAT_DUP_FREE2.h")

// ī�����
#include "..\..\..\����\����\BO_CAT_PROC\BO_CAT_PROC_0202.h"

// ������
//#include _T("..\..\..\����\����\BO_CAT_PROC\BO_CAT_PROC_0203.h")
#include "..\..\..\����\CAT_PRINT\EquipPrintDlg.h"

// ���η��� ��� ���
#include "..\..\..\����\����\BO_CAT_PROC\BO_CAT_PROC_0204.h"

// ������ ���
#include "..\..\..\����\����\BO_CAT_PROC\BO_CAT_PROC_0201.h"

// �谡���� ���
//#include _T("..\..\..\����\����\BO_CAT_PROC\BO_CAT_PROC_1200.h")
#include "..\..\..\����\MarcAdjust\LocRecordDlg.h"

// ���Ӽ���api
#include "..\..\����\SE_API\SeApi.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainDefaultDlg dialog


CMainDefaultDlg::CMainDefaultDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDefaultDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM = NULL;
	m_pGrid = NULL;
	m_pDuplicateSearchMgr = NULL;

	m_bCatHold = FALSE;
}

BOOL CMainDefaultDlg::Create(CWnd* pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;
}

VOID CMainDefaultDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDefaultDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CMainDefaultDlg, CDialog)
	//{{AFX_MSG_MAP(CMainDefaultDlg)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(SHOW_SEARCH_RESULT, OnDUP_ShowSearchResult)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDefaultDlg Operations

VOID CMainDefaultDlg::MarcAdjust_Insert()
{
	EFS_BEGIN

	CString strBookWhere;
	CESL_DataMgr *pDM_Where = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
	if (pDM_Where)
	pDM_Where->GetCellData(_T("å������"), 0, strBookWhere);
	
	m_MarcAdjLaunCher.SetParentInfo(m_pDM, m_pGrid, _T("SS_��KEY"), strBookWhere, m_pGrid->GetRow() - 1);
	m_MarcAdjLaunCher.LaunchMarcAdjust(GetParentFrame(), this, _T("SEC111N"), MNG_INSERT, MODE_SERIAL);

	EFS_END
}

VOID CMainDefaultDlg::MarcAdjust_Modify()
{
	EFS_BEGIN

	if (m_pDM->GetRowCount() <= 0) return;

	CString strBookWhere;
	CESL_DataMgr *pDM_Where = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
	if (pDM_Where)
		pDM_Where->GetCellData(_T("å������"), 0, strBookWhere);
	
	m_MarcAdjLaunCher.SetParentInfo(m_pDM, m_pGrid, _T("SS_��KEY"), strBookWhere, m_pGrid->GetRow() - 1, m_bCatHold);
	m_MarcAdjLaunCher.LaunchMarcAdjust(GetParentFrame(), this, _T("SEC111N"), MNG_MODIFY, MODE_SERIAL);

	EFS_END
}

INT CMainDefaultDlg::DUP_Search()
{
	EFS_BEGIN

	m_pDuplicateSearchMgr->ShowWorkDuplicateSearch(_T("������������"), m_strMainDMAlias, m_strMainCMAlias, m_strMainGridAlias);
	
	return 0;

	EFS_END
	return -1;
}

INT CMainDefaultDlg::DUP_Free()
{
	EFS_BEGIN

//	CBO_CAT_DUP_FREE dlg(this);
//	dlg.SetParentInfo(m_strMainDMAlias, m_strMainCMAlias, m_strMainGridAlias);
//	dlg.DoModal();

	return 0;

	EFS_END
	return -1;
}

INT CMainDefaultDlg::DUP_KeySetting()
{
	EFS_BEGIN

//	CDUP_KEY_SETTING dlg(this, _T(".\\duplicate_search_key.txt"));
//	dlg.DoModal();

	return 0;

	EFS_END
	return -1;
}

INT CMainDefaultDlg::PRINT(CString strAlias)
{
	EFS_BEGIN

	if (strAlias == _T("ī����"))
	{
		CBO_CAT_PROC_0202 dlg(this);
		dlg.m_nModeMONO_SERIAL = MODE_SERIAL;
		dlg.DoModal();
	}
	else if (strAlias == _T("���η�"))
	{
		CBO_CAT_PROC_0204 dlg(this);
		dlg.DoModal();
	}
	else if (strAlias == _T("���"))
	{
		CEquipPrintDlg dlg(this);
		CStringArray aSpeciesKeyList;
		//dlg.SetParentInfo(m_pDM, m_pGrid, _T("SS_��KEY"));
		dlg.SetParentInfo(m_pDM, m_pGrid, _T("SS_��KEY"), &aSpeciesKeyList);
		dlg.m_nModeMONO_SERIAL = MODE_SERIAL;
		dlg.DoModal();
	}
	else if (strAlias == _T("������"))
	{
		CBO_CAT_PROC_0201 dlg(this);	//������ ȭ��
		dlg.DoModal();
	}
	else if (strAlias == _T("�����ΰ���"))
	{
		// DM ����
		CCatApi_SE CatApi(this);
		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		dm.InitDMPKFromFile(_T("DM_����_���_�����ΰ�_����"));
		dm.SetMarcMgr(m_pInfo->pMarcMgr);

		CString strBookWhere;
		CESL_DataMgr *pDM_Where = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
		if (pDM_Where)
			strBookWhere = pDM_Where->GetCellData(_T("å������"), 0);

		// From TBL �� �ٲ۴�.
		// ������ ���� ���̺��� �ֳ� �˻��Ѵ�.
		if (strBookWhere.Find(_T("a.")) >= 0)
		{
			// CO_ACCESSION_REC_TBL �� ����
			dm.TBL_NAME += _T(" , CO_ACCESSION_REC_TBL a ");
		}
		if (strBookWhere.Find(_T("p.")) >= 0)
		{
			// BO_PURCHASE_SEQ_NO_TBL �� ����
			dm.TBL_NAME += _T(" , BO_PURCHASE_SEQ_NO_TBL p ");
		}

		// ������ ���Ѵ�.
		CStringArray arrStrSpeciesKey;

		m_pGrid->SelectMakeList();
		if (m_pGrid->SelectGetCount() <= 0) 
		{
			AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
			return -1;
		}

		CString strSpeciesKey;
		
		INT idx = m_pGrid->SelectGetHeadPosition();
		while (idx >= 0)
		{
			// ������ �ο��� ��Ű�� �����´�.
			strSpeciesKey = m_pDM->GetCellData(_T("SS_��KEY"), idx);
			idx = m_pGrid->SelectGetNext();

			arrStrSpeciesKey.Add(strSpeciesKey);
		}

		// ������ �������� å�� �����ͼ� ����Ѵ�.
		CString strOption;
		strOption.Format(_T("b.SPECIES_KEY IN ( "));
		INT nRowCount = arrStrSpeciesKey.GetSize();
		for (idx = 0; idx < nRowCount; idx++)
		{
			strSpeciesKey = arrStrSpeciesKey.GetAt(idx);
			if (strSpeciesKey.IsEmpty()) continue;

			if (idx != nRowCount - 1)
				strOption += strSpeciesKey + _T(", ");
			else
				strOption += strSpeciesKey;
		}

		strOption += _T(") AND ") + strBookWhere;

		INT ids = dm.RefreshDataManager(strOption);
		if (ids < 0) return -1;

		// ���鼭 ��ȣ�� �־��ش�.
		CString strNum;
		nRowCount = dm.GetRowCount();
		for (idx = 0; idx < nRowCount; idx++)
		{
			strNum.Format(_T("%d"), idx + 1);
			dm.SetCellData(_T("��ȣ"), strNum, idx);
		}

		CSIReportManager SIReportManager(this);
		SIReportManager.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
		ids = SIReportManager.GetSIReportFileFromDB(_T("����_����_�����ΰ���"));
		if (ids < 0) {
			AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
			return -1;
		}

		CString strCount;
		strCount.Format(_T("%d"), arrStrSpeciesKey.GetSize());
		SIReportManager.SetTempVariable(_T("����"), strCount);
		strCount.Format(_T("%d"), dm.GetRowCount());
		SIReportManager.SetTempVariable(_T("å��"), strCount);

		SIReportManager.SetDataMgr(0, &dm, 0);
		
		ids = SIReportManager.Print();
	}

	return 0;

	EFS_END
	return -1;
}

// 040401_HSS �η��ڷ��� ���µ� �ٲپ� �־�� �Ѵ�.
INT CMainDefaultDlg::UpdateWorkingStatus(LPCTSTR lpszFromStatus, LPCTSTR lpszToStatus, CString strUpdateFieldName /*= _T("")*/, CString strUpdateFieldData /*= _T("")*/)
{
	EFS_BEGIN

	INT ids = -1;
	CString strQuery;
	CString strSpeciesKey;
	CString strBookKey;
	CCatApi_SE CatApi(this);

	m_pDM->StartFrame();

	m_pGrid->SelectMakeList();
	if (m_pGrid->SelectGetCount() <= 0) 
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return -1;
	}

	// å DM ����
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	dm.InitDMPKFromFile(_T("DM_����_����DM_å����"));
	dm.SetMarcMgr(m_pInfo->pMarcMgr);

	// 040401_HSS �η� DM ����
	CESL_DataMgr dmApp;
	dmApp.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	dmApp.InitDMPKFromFile(_T("DM_����_����DM_�η�����"));
	dmApp.SetMarcMgr(m_pInfo->pMarcMgr);

	CString strBookWhere;
	CESL_DataMgr *pDM_Where = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
	if (pDM_Where)
		strBookWhere = pDM_Where->GetCellData(_T("å������"), 0);

	// From TBL �� �ٲ۴�.
	// ������ ���� ���̺��� �ֳ� �˻��Ѵ�.
	if (strBookWhere.Find(_T("a.")) >= 0)
	{
		// CO_ACCESSION_REC_TBL �� ����
		dm.TBL_NAME += _T(" , CO_ACCESSION_REC_TBL a ");
		dmApp.TBL_NAME += _T(" , CO_ACCESSION_REC_TBL a ");
	}
	if (strBookWhere.Find(_T("p.")) >= 0)
	{
		// BO_PURCHASE_SEQ_NO_TBL �� ����
		dm.TBL_NAME += _T(" , BO_PURCHASE_SEQ_NO_TBL p ");
		dmApp.TBL_NAME += _T(" , BO_PURCHASE_SEQ_NO_TBL p ");
	}
	
	INT nRowCount = 0;
	INT i = -1;
	INT idx = m_pGrid->SelectGetHeadPosition();
	while (idx >= 0)
	{
		strSpeciesKey.Empty();
		
		// ��Ű�� �����´�.
		strSpeciesKey = m_pDM->GetCellData(_T("ISS_��KEY"), idx);
		idx = m_pGrid->SelectGetNext();

		// ���� Book Where���� �ش��ϴ� å Ű���� �����´�.
		strQuery.Format(_T("SPECIES_KEY = %s AND %s"), strSpeciesKey, strBookWhere);
		ids = dm.RefreshDataManager(strQuery);
		if (ids < 0) continue;
		ids = dmApp.RefreshDataManager(strQuery);
		if (ids < 0) continue;


		// å�� ���鼭 working status�� Update�Ѵ�.
		nRowCount = dm.GetRowCount();
		for (i = 0; i < nRowCount; i++)
		{
			strBookKey = dm.GetCellData(_T("SB_åKEY"), i);
			if (!strUpdateFieldName.IsEmpty())
				strQuery.Format(_T("UPDATE SE_BOOK_TBL SET %s = '%s', WORKING_STATUS = '%s', PREV_WORKING_STATUS = '%s', LAST_WORK='%s' "
								   _T("WHERE (REC_KEY = %s OR BINDING_BOOK_KEY = %s) AND WORKING_STATUS = '%s';")), 
							       strUpdateFieldName, strUpdateFieldData, lpszToStatus, lpszFromStatus, 
								   GetWorkLogMsg(_T("���ӻ��º���"),WORK_LOG), strBookKey, strBookKey, lpszFromStatus);
			else 
				strQuery.Format(_T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = '%s', PREV_WORKING_STATUS = '%s', LAST_WORK='%s' "
								   _T("WHERE (REC_KEY = %s OR BINDING_BOOK_KEY = %s) AND WORKING_STATUS = '%s';")), 
								   lpszToStatus, lpszFromStatus, GetWorkLogMsg(_T("���ӻ��º���"),WORK_LOG), strBookKey, strBookKey, lpszFromStatus);
			m_pDM->AddFrame(strQuery);
		}
		// 040401_HSS �η��� ���鼭 working status�� Update�Ѵ�.
		nRowCount = dmApp.GetRowCount();
		for (i = 0; i < nRowCount; i++)
		{
			strBookKey = dmApp.GetCellData(_T("SB_åKEY"), i);
			if (!strUpdateFieldName.IsEmpty())
				strQuery.Format(_T("UPDATE SE_BOOK_TBL SET %s = '%s', WORKING_STATUS = '%s', PREV_WORKING_STATUS = '%s', LAST_WORK='%s' "
								   _T("WHERE (REC_KEY = %s OR BINDING_BOOK_KEY = %s) AND WORKING_STATUS = '%s';")), 
							       strUpdateFieldName, strUpdateFieldData, lpszToStatus, lpszFromStatus, 
								   GetWorkLogMsg(_T("���ӻ��º���"),WORK_LOG), strBookKey, strBookKey, lpszFromStatus);
			else 
				strQuery.Format(_T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = '%s', PREV_WORKING_STATUS = '%s', LAST_WORK='%s' "
								   _T("WHERE (REC_KEY = %s OR BINDING_BOOK_KEY = %s) AND WORKING_STATUS = '%s';")), 
								   lpszToStatus, lpszFromStatus, GetWorkLogMsg(_T("���ӻ��º���"),WORK_LOG), strBookKey, strBookKey, lpszFromStatus);
			m_pDM->AddFrame(strQuery);
		}
	}

	// SendFrame
	ids = m_pDM->SendFrame();
	if (ids < 0) return ids;
	ids = m_pDM->EndFrame();
	if (ids < 0) return ids;

	return 0;

	EFS_END
	return -1;
}

INT CMainDefaultDlg::Complete_Dup()
{
	EFS_BEGIN

	INT ids = -1;
	ids = UpdateWorkingStatus(_T("BOC1110"), _T("BOC113N")) ;
	if (ids < 0)
	{
		//AfxMessageBox(_T("�����Ͽ����ϴ�."));
		return -1;
	}
	AfxMessageBox(_T("�������� �Ϸ� ó���Ͽ����ϴ�."));

	return 0;

	EFS_END
	return -1;
}

INT CMainDefaultDlg::Complete_Dup2()
{
	EFS_BEGIN

	INT ids = -1;
	ids = UpdateWorkingStatus(_T("BOC111O"), _T("BOC112O"));
	if (ids < 0)
	{
		//AfxMessageBox(_T("�����Ͽ����ϴ�."));
		return -1;
	}
	AfxMessageBox(_T("�������� �Ϸ� ó���Ͽ����ϴ�."));
	return 0;

	EFS_END
	return -1;
}

INT CMainDefaultDlg::Complete_Classification()
{
	EFS_BEGIN

	INT ids = -1;
	ids = UpdateWorkingStatus(_T("BOC1120"), _T("BOC113N"));
	if (ids < 0)
	{
		//AfxMessageBox(_T("�����Ͽ����ϴ�."));
		return -1;
	}
	AfxMessageBox(_T("�з� �Ϸ� ó���Ͽ����ϴ�."));
	return 0;

	EFS_END
	return -1;
}

// 040401_HSS �η��ڷᵵ �����Ѵ�.
INT CMainDefaultDlg::Complete_Cataloging(BOOL bChangeWorkingStatus /*= TRUE*/)
{
	EFS_BEGIN

	CCatApi_SE CatApi(this);
	CatApi.InitMemberData();

	// DM ����
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	dm.InitDMPKFromFile(_T("DM_����_����DM_å����"));
	dm.SetMarcMgr(m_pInfo->pMarcMgr);

	// 040401_HSS �η� DM ����
	CESL_DataMgr dmApp;
	dmApp.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	dmApp.InitDMPKFromFile(_T("DM_����_����DM_�η�����"));
	dmApp.SetMarcMgr(m_pInfo->pMarcMgr);

	CString strBookWhere;
	CESL_DataMgr *pDM_Where = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
	if (pDM_Where)
		strBookWhere = pDM_Where->GetCellData(_T("å������"), 0);

	// From TBL �� �ٲ۴�.
	// ������ ���� ���̺��� �ֳ� �˻��Ѵ�.
	if (strBookWhere.Find(_T("a.")) >= 0)
	{
		// CO_ACCESSION_REC_TBL �� ����
		dm.TBL_NAME += _T(" , CO_ACCESSION_REC_TBL a ");
		dmApp.TBL_NAME += _T(" , CO_ACCESSION_REC_TBL a ");
	}
	
	// ������ �ڷ� üũ
	INT ids = -1;
	ids = m_pGrid->SelectMakeList();
	INT nSelectCount = m_pGrid->SelectGetCount();
	if (nSelectCount <= 0)
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return -1;
	}

	// ������ ���� ���鼭 üũ�� �Ѵ�.
	CString strSpeciesKey;
	CString strQuery;
	CString strItem;

	INT nIdx = -1;
	INT nRowCount, nRowCount_App;
	INT nSelectIndex = m_pGrid->SelectGetHeadPosition();
	while (nSelectIndex >= 0)
	{
		nIdx = nSelectIndex;
		nSelectIndex = m_pGrid->SelectGetNext();
		ids = m_pDM->GetCellData(_T("SS_��KEY"), nIdx, strSpeciesKey);
		if (ids < 0 || strSpeciesKey.IsEmpty()) continue;

		// ���� �׸��� - ����� �ڵ� Ȯ��
		ids = m_pDM->GetCellData(_T("SS_�����ڷᱸ��_�ڵ�"), nIdx, strItem);
		if (ids < 0 || strItem.IsEmpty())
		{
			AfxMessageBox(_T("�����ڷᱸ�� �ڵ尡 �����ϴ�."));
			return -1;
		}
		
		// å Select
		strQuery.Format(_T("SPECIES_KEY = %s AND %s"), strSpeciesKey, strBookWhere);
		ids = dm.RefreshDataManager(strQuery);
		if (ids < 0) 
		{
			AfxMessageBox(_T("��� å �˻� ����!"));
			return -1;
		}
		// 040401_HSS �η� Select
		strQuery.Format(_T("SPECIES_KEY = %s AND %s"), strSpeciesKey, strBookWhere);
		ids = dmApp.RefreshDataManager(strQuery);
		if (ids < 0) 
		{
			AfxMessageBox(_T("��� �η� �˻� ����!"));
			return -1;
		}

		// �ڷ� �� Ȯ��
		nRowCount = dm.GetRowCount();
		nRowCount_App = dmApp.GetRowCount();
		if (nRowCount <= 0 && nRowCount_App <= 0)
		{
			AfxMessageBox(_T("����� �Ǵ� �ڷᰡ �����ϴ�."));
			return -1;
		}	
		
		// �����Ϸ� ��� �ڷ����� üũ - å
		INT idx;
		for (idx = 0; idx < nRowCount; idx++)
		{
			// 1. ���å�鿡�� ��Ϲ�ȣ�� �ο������ üũ
			ids = dm.GetCellData(_T("SB_��Ϲ�ȣ"), idx, strItem);
			if (ids < 0 || strItem.IsEmpty()) 
			{
				AfxMessageBox(_T("��Ϲ�ȣ�� �ο����� ���� å�� �ֽ��ϴ�."));
				return -1;
			}			

			// 2. �谡�� �ڵ� Ȯ��
			ids = dm.GetCellData(_T("SB_�谡��ġ��ȣ"), idx, strItem);
			if (ids < 0 || strItem.IsEmpty())
			{
				AfxMessageBox(_T("�谡�� �ڵ尡 �ο����� ���� å�� �ֽ��ϴ�."));
				return -1;
			}
		}

		// 040401_HSS �����Ϸ� ��� �ڷ����� üũ - �η�
		for (idx = 0; idx < nRowCount_App; idx++)
		{
			// 1. ���ηϵ鿡�� ��Ϲ�ȣ�� �ο������ üũ
			ids = dmApp.GetCellData(_T("SB_��Ϲ�ȣ"), idx, strItem);
			if (ids < 0 || strItem.IsEmpty()) 
			{
				AfxMessageBox(_T("��Ϲ�ȣ�� �ο����� ���� å�� �ֽ��ϴ�."));
				return -1;
			}			

			// 2. �谡�� �ڵ� Ȯ��
			ids = dmApp.GetCellData(_T("SB_�谡��ġ��ȣ"), idx, strItem);
			if (ids < 0 || strItem.IsEmpty())
			{
				AfxMessageBox(_T("�谡�� �ڵ尡 �ο����� ���� å�� �ֽ��ϴ�."));
				return -1;
			}
		}

	}

	m_pDM->StartFrame();

	// �����Ϸ� �۾��� �����Ѵ�.
	nSelectIndex = m_pGrid->SelectGetHeadPosition();
	while (nSelectIndex >= 0)
	{
		nIdx = nSelectIndex;
		nSelectIndex = m_pGrid->SelectGetNext();

		// ���� �ϷḦ �θ���.
		ids = CatApi.CompleteCataloging(m_pDM, m_pDM, nIdx, NULL, FALSE);
	}

	ids = m_pDM->SendFrame();
	if (ids < 0)
	{
		AfxMessageBox(_T("DB���� �۾��� �����߽��ϴ�."));
		return -1;
	}
	ids = m_pDM->EndFrame();
	
	if (bChangeWorkingStatus)
		ids = UpdateWorkingStatus(_T("SEC111N"), _T("SEC112O"), _T("CAT_COMPLETE_DATE"), CTime::GetCurrentTime().Format(_T("%Y/%m/%d")));
	else
		ids = UpdateWorkingStatus(_T("SEC111N"), _T("SEC111N"), _T("CAT_COMPLETE_DATE"), CTime::GetCurrentTime().Format(_T("%Y/%m/%d")));

	
	if (ids < 0)
		return -1;
	
	return 0;

	EFS_END
	return -1;
}

INT CMainDefaultDlg::Complete_Loc()
{
	EFS_BEGIN

	INT ids = -1;
	ids = UpdateWorkingStatus(_T("SEC112O"), _T("SEL211O"), _T("LOC_TRANSFER_DATE"), CTime::GetCurrentTime().Format(_T("%Y/%m/%d")));
	if (ids < 0)
		return -1;
	return 0;

	EFS_END
	return -1;
}

INT CMainDefaultDlg::Complete_Loc2()
{
	EFS_BEGIN

	INT ids = -1;
	/*// �����ΰ��ڷ�� ����
	ids = UpdateWorkingStatus(_T("SEL211O"), _T("SEL212N"));
	if (ids < 0)
	{
		//AfxMessageBox(_T("�����Ͽ����ϴ�."));
		return -1;
	}
	*/

	// �谡���� ��� & �谡�ڷ�� ����
	ids = Loc_Record();
	if (ids < 0)
	{
		return -1;
	}

	//AfxMessageBox(_T("�����ΰ� ó���Ͽ����ϴ�."));
	return 0;

	EFS_END
	return -1;
}

INT CMainDefaultDlg::Loc_Record()
{
	EFS_BEGIN

	/*
	INT ids = -1;
	CBO_CAT_PROC_1200 dlg(this);
	dlg.SetParentInfo(m_strMainDMAlias, m_strMainCMAlias, m_strMainGridAlias);
	if (dlg.DoModal() != IDOK) return -1;
	
	// �谡�ڷ�� ����
	ids = UpdateWorkingStatus(_T("SEL211O"), _T("SEL212N"));
	*/

	// �谡���� ����ϴ°� �����
	CLocRecordDlg dlg(this);
	dlg.m_nModeMONO_SERIAL = MODE_SERIAL;
	dlg.SetParentInfo(m_pDM, m_pGrid, _T("SS_��KEY"));
	if (dlg.DoModal() != IDOK) return -1;

	return 0;

	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CMainDefaultDlg message handlers

BOOL CMainDefaultDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_����_����")) < 0)
		goto ERR;

	m_strMainGridAlias = _T("���α⺻�׸���");
	m_strMainDMAlias = _T("DM_����_����_�˻����");
	m_strMainCMAlias = _T("CM_����_����");

	m_pGrid = static_cast<CESL_Grid*>(FindControl(m_strMainCMAlias, m_strMainGridAlias));
	if (m_pGrid == NULL) goto ERR;
	m_pGrid->InitPopupMenuItem();
	m_pGrid->AddPopupMenuItem(static_cast<CView*>(pMain), this->VIEW_STYLE);

	m_pDM = FindDM(m_strMainDMAlias);
	if (m_pDM == NULL) goto ERR;

	// �������� �޴���
	m_pDuplicateSearchMgr = new CDuplicateSearch_Interface(this);
	m_pDuplicateSearchMgr->Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), this, 1004, NULL);
	m_pDuplicateSearchMgr->ShowWindow(SW_HIDE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
ERR:
	MessageBox(_T("���̾�α� �ʱ�ȭ ����"));
	EndDialog(IDCANCEL);
	return FALSE;

	EFS_END
	return FALSE;
}

VOID CMainDefaultDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	if (GetSafeHwnd() == NULL) return;

	CRect rcClient;
	GetClientRect(rcClient);
	
	if (m_pGrid && m_pGrid->GetSafeHwnd())
		m_pGrid->MoveWindow(rcClient);	
	

	EFS_END
}

VOID CMainDefaultDlg::OnDUP_ShowSearchResult(WPARAM wParam, LPARAM lParam)
{
	EFS_BEGIN

	if(m_pDuplicateSearchMgr)
		m_pDuplicateSearchMgr->ShowDuplicateSearchDlg();

	EFS_END
}

VOID CMainDefaultDlg::OnDestroy() 
{
	EFS_BEGIN

	CDialog::OnDestroy();
	
	if (m_pDuplicateSearchMgr)
	{
		delete m_pDuplicateSearchMgr;
		m_pDuplicateSearchMgr = NULL;
	}

	EFS_END
}

BEGIN_EVENTSINK_MAP(CMainDefaultDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMainDefaultDlg)
	ON_EVENT(CMainDefaultDlg, IDC_GRID_CAT, -601 /* DblClick */, OnDblClickGridCat, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

INT CMainDefaultDlg::SelectAgain()
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
	if (pDM == NULL) return -1;
	if (pDM->GetRowCount() <= 0) return -1;

	CString strWhere = pDM->GetCellData(_T("������"), 0);
	

	// Select
	m_pDM->RefreshDataManager(strWhere);
	//���� ��Ϲ�ȣ ��Ʈ
	m_pDM->SORT( _T("���۵�Ϲ�ȣ"));


	CString strMsg;

	INT nRowCount = m_pDM->GetRowCount();

	INT nBookCount = 0;
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		nBookCount += _ttoi(m_pDM->GetCellData(_T("å��"), idx));
	}

	strMsg.Format(_T("���� : %5d\tå�� : %5d"), nRowCount, nBookCount);

	GetParentFrame()->SendMessage(WM_USER + 2004, NULL, (LPARAM)strMsg.GetBuffer(0));


	m_pGrid->Display();

	return 0;

	EFS_END
	return -1;
}

VOID CMainDefaultDlg::OnDblClickGridCat() 
{
	EFS_BEGIN

	if (m_pGrid->GetMouseRow() == 0) return;

	if (m_strMenuAlias == _T("����_����_��Ͽϼ�"))
		MarcAdjust_Modify();

	EFS_END
}

// 041319_HSS ������ �߰�
VOID CMainDefaultDlg::DeleteSpecies()
{
	m_pGrid->SelectMakeList();

	INT idx = m_pGrid->SelectGetHeadPosition();
	if(idx<0)
	{
		AfxMessageBox(_T("���õ� �׸��� �����ϴ�!"));
	    return;
	}
	
	CString msg;
	INT nCount = m_pGrid->SelectGetCount();
	msg.Format(_T("[ %d ] ���� �ڷḦ ���� �Ͻðڽ��ϱ�?\n������ �˻��� å �� �ƴ϶� �ش� ���� ��� �ڷᰡ �����˴ϴ�."),nCount);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return; 
	
	BeginWaitCursor();

	CSeApi api(this);
	CString strSPECIES_KEY;
	CESL_DataMgr* pDM_SPECIES = FindDM(_T("DM_����_������������_��"));
	CESL_DataMgr* pDM_SPECIES_DELETE = FindDM(_T("DM_����_������������_������"));
	CESL_DataMgr* pDM_VOL = FindDM(_T("DM_����_������������_��"));


	CString strTITLE;
	INT ids = 0;
	CArray<INT,INT> arrayDeletedRow;

	while(idx>=0)
	{
		strSPECIES_KEY = m_pDM->GetCellData(_T("ISS_��KEY"),idx);
		
		ids = api.SPECIES_CAT_DELETE(strSPECIES_KEY,
							     pDM_SPECIES,
							     pDM_SPECIES_DELETE,
							     pDM_VOL);

		if(ids<0)
		{
			EndWaitCursor();
			strTITLE = m_pDM->GetCellData(_T("ISS_ǥ������"),idx);

			if(-1001 == ids) //cancel
			{
				msg.Format(_T("[ %s ] �ڷ��� ������ ����ϼ̽��ϴ�!"),strTITLE);
				AfxMessageBox(msg);
			}
			else if(-1002 == ids)
			{
				// Ư���� ������ ���� ����
			}
			else
			{
				msg.Format(_T("[ %s ] �ڷḦ �����ϴ� �� ������ �߻��� �۾��� �ߴ��մϴ�!"),strTITLE);
				AfxMessageBox(msg);
				break;
			}
		}
		else 
		{
			arrayDeletedRow.Add(idx);
		}

		idx = m_pGrid->SelectGetNext();
	}
	
	INT nSize = arrayDeletedRow.GetSize();
	INT nDeletedRow;

	for(INT i=0;i<nSize;i++)
	{
		nDeletedRow = arrayDeletedRow.GetAt(nSize-i-1);
		m_pDM->DeleteRow(nDeletedRow);
	}
	
	m_pGrid->Display();
	EndWaitCursor();

	msg.Format(_T("[ %d ] ���� �ڷḦ �����Ͽ����ϴ�!"),nSize);
	AfxMessageBox(msg);

	return; 	

}