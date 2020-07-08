// DonateDataView.cpp : implementation file
//

#include "stdafx.h"
#include "DonateDataView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDonateDataView dialog


CDonateDataView::CDonateDataView(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDonateDataView::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDonateDataView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pBO_DM = NULL;
	m_pBO_PrintDM = NULL;
	m_pSE_DM = NULL;
	m_pCM = NULL;
	m_pBOGrid = NULL;
	m_pSEGrid = NULL;
	m_pMainGrid = NULL;
	m_pMainDM = NULL;
	m_nIdx = -1;
	m_nFirstIdx = -1;
	m_nLastIdx = -1;
	m_bBO_Select = FALSE;
	m_bSE_Select = FALSE;
	m_pPrintMgr = NULL;
	m_pPrintDM = NULL;
	m_pDonateDataSearchDlg = NULL;
	m_nRow = -1;
}

CDonateDataView::~CDonateDataView()
{

	if(m_pPrintMgr)
	{
		delete m_pPrintMgr;
		m_pPrintMgr = NULL;
	}
	if(m_pDonateDataSearchDlg)
	{
		delete m_pDonateDataSearchDlg;
		m_pDonateDataSearchDlg = NULL;
	}
}

VOID CDonateDataView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDonateDataView)
	DDX_Control(pDX, IDC_tabDVIEW_DONATE_DATA, m_tabDonateData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDonateDataView, CDialog)
	//{{AFX_MSG_MAP(CDonateDataView)
	ON_BN_CLICKED(IDC_btnDVIEW_PRINT, OnbtnDVIEWPRINT)
	ON_BN_CLICKED(IDC_btnDVIEW_PREV, OnbtnDVIEWPREV)
	ON_BN_CLICKED(IDC_btnDVIEW_NEXT, OnbtnDVIEWNEXT)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabDVIEW_DONATE_DATA, OnSelchangetabDVIEWDONATEDATA)
	ON_BN_CLICKED(IDC_btnDONATE_DATE_SEARCH, OnbtnDONATEDATESEARCH)
	ON_WM_SIZE()
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDonateDataView message handlers

BOOL CDonateDataView::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(m_pMainDM == NULL || m_pMainGrid == NULL)
	{
		ESLAfxMessageBox(_T("�����ڰ��� ������ �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	if (InitESL_Mgr(_T("K2UPGRADE_�����ڷẸ��")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	GetWindowRect(m_rcInitial);

	m_pCM = FindCM(_T("CM_�����ڷẸ��"));
	m_pBO_DM = FindDM(_T("DM_���������ڷ����"));
	m_pBO_PrintDM = FindDM(_T("DM_���������ڷ����_��"));	
	m_pSE_DM = FindDM(_T("DM_����_�����ڷ�"));
	m_pPrintDM = FindDM(_T("DM_����_�����ڷ�"));
	
	m_pDonateDataSearchDlg = new CDonateDataSearchDlg(this);
	
	m_pBOGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid_BO"));
	m_pSEGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid_SE"));
	m_pBOGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	m_pSEGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	//JOB.2019.0017 ADD BY KYJ
	//�������� ���� ����,�α� ������ ����
	//-------------------------------------------------------------------------
	if(!m_pInfo->m_bUserInfoExportYN)
	{
		m_pBOGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
		m_pSEGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		m_pBOGrid->m_bPrivacyPrintLogRecord = TRUE;
		m_pBOGrid->m_strPrintInfo = _T("�����ڷ�(����) ����");
		m_pSEGrid->m_bPrivacyPrintLogRecord = TRUE;
		m_pSEGrid->m_strPrintInfo = _T("�����ڷ�(����) ����");
	}
	//-------------------------------------------------------------------------
		
	m_tabDonateData.InsertItem(0, _T("����"));
	m_tabDonateData.InsertItem(1, _T("����"));
	m_tabDonateData.SetCurSel(0);
	m_pBOGrid->ShowWindow(SW_SHOW);
	m_pSEGrid->ShowWindow(SW_HIDE);
	
	m_pPrintMgr = new CBasicPrintMgr(this);
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);

	PostMessage(WM_SIZE);
	
	CLibInfoMgr mgr(this);
	mgr.GetLibInfo(GetUserID(), &m_cLibInfo);
	Display(0);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CDonateDataView::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strTmpData;
	INT nGroupNo;
	if(m_nRow != nRow)
	{
		CMarc marc;
		INT ids;
		strTmpData = GetActiveDM()->GetCellData(_T("BS_MARC"), nRow);
		ids = m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
		if(ids < 0) return -1;
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
		if(ids >= 0) GetActiveDM()->SetCellData(_T("UDF_�Ѽ�����"), strTmpData, nRow);
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetEditionStatement(&marc, strTmpData);
		if(ids >= 0) GetActiveDM()->SetCellData(_T("IBS_�ǻ���"), strTmpData, nRow);
		strTmpData.Empty();	 strTmpData = GetActiveDM()->GetCellData(_T("IBS_��ǥ��"), nRow);
		ids = m_MarcConvertMgr.GetTitleStatement(&marc, strTmpData);
		if(ids >= 0) GetActiveDM()->SetCellData(_T("UDF_ǥ������"), strTmpData, nRow);
		m_nRow = nRow;
		if(m_nRow == GetActiveDM()->GetRowCount()-1) m_nRow = -1;
	}

	if(strDMFieldAlias == _T("UDF_������ȣ����"))
	{	
		strAcqYear = GetActiveDM()->GetCellData(_T("BP_���Գ⵵"), nRow);
		strGroupNo = GetActiveDM()->GetCellData(_T("BP_�׷��ȣ"), nRow);
		nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
		str.Format(_T("%s-%03d"), strAcqYear, nGroupNo);// = strAcqYear+_T("-")+strGroupNo;
		GetActiveDM()->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if(strDMFieldAlias == _T("BP_��������"))
	{
		strTmpData = GetActiveDM()->GetCellData(_T("IBS_�з�ǥ����"), nRow);
		if(strTmpData.IsEmpty()) str = _T("");
		else if(strTmpData == _T("1"))
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("KDC��������ȣ����"), str, str);
		}
		else if(strTmpData == _T("2"))
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("DDC��������ȣ����"), str, str);
		}
		GetActiveDM()->SetCellData(_T("UDF_�������м���"), str, nRow);
	}
	else if(strDMFieldAlias == _T("BP_����"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		GetActiveDM()->SetCellData(_T("UDF_��ȯ����"), str, nRow);
	}
	else if(strDMFieldAlias == _T("SV_����"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
		GetActiveDM()->SetCellData(_T("UDF_��ȯ����"), str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_å��"))
		GetActiveDM()->SetCellData(_T("BP_��ǰå��"), str, nRow);
	return 0;
}

VOID CDonateDataView::OnbtnDVIEWPRINT() 
{
	// TODO: Add your control notification handler code here
	INT ids = 0;
	if(!m_bBO_Select) ids = MakeBO_DonateData(m_pMainDM, m_pBO_DM, m_pBO_PrintDM, m_nIdx);
	if(ids < 0) return ;
	if(!m_bSE_Select) ids = MakeSE_DonateData(m_pMainDM, m_pSE_DM, m_nIdx);
	if(ids < 0) return ;
	ids = MakeMergeDonateData(m_pBO_PrintDM, m_pSE_DM, m_pPrintDM);
	if(ids < 0) return ;
	if(m_pPrintDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("����� �����ڷᰡ �����ϴ�."));
		return ;
	}

	ids = SetPrintDonateConfirmData(m_pMainDM, m_pBO_DM, m_pBO_PrintDM, m_pSE_DM, m_pPrintDM, m_pPrintMgr, &m_cLibInfo, m_nIdx);
	if(ids < 0) return;
	m_pPrintMgr->Print();	
}

VOID CDonateDataView::OnbtnDVIEWPREV() 
{
 	Display(-1);
}

VOID CDonateDataView::OnbtnDVIEWNEXT() 
{
	// TODO: Add your control notification handler code here
	Display(1);
}

INT CDonateDataView::Display(INT nIdx)
{
	switch(nIdx)
	{
	case 0:
		InitFirstLastIndex();
		m_nIdx = m_nFirstIdx;
		if(m_nIdx < 0) m_nIdx = 0;
		ShowNextPrevButton(m_nIdx);
		break;
	case -1:
		m_nIdx = m_pMainGrid->SelectGetPrev();
		ShowNextPrevButton(m_nIdx);
		break;
	case 1:
		m_nIdx = m_pMainGrid->SelectGetNext();
		ShowNextPrevButton(m_nIdx);
		break;
	case 1000:
		break;
	default:
		return 0;
	}

	m_pCM->AllControlDisplay(m_nIdx);
	if(m_pDonateDataSearchDlg)
	{
		m_pCM->SetControlMgrData(_T("��������1"), m_pDonateDataSearchDlg->GetDonateDate1());
		m_pCM->SetControlMgrData(_T("��������2"), m_pDonateDataSearchDlg->GetDonateDate2());
	}
	m_bBO_Select = FALSE;
	m_bSE_Select = FALSE;
	
	INT ids = MakeDonateData(m_nIdx);
	if(ids < 0) return -1;
	GetActiveGrid()->Display();
	m_pMainGrid->SetReverse(m_nIdx);
	
	GetDlgItem(IDC_edtDONATE_DATE1)->EnableWindow(FALSE);
	GetDlgItem(IDC_edtDONATE_DATE2)->EnableWindow(FALSE);

	return 0;	
}

VOID CDonateDataView::InitFirstLastIndex()
{
	INT ids = m_pMainGrid->SelectMakeList();
	if(ids < 0) return;
	m_nLastIdx = m_pMainGrid->SelectGetTailPosition();
	m_nFirstIdx = m_pMainGrid->SelectGetHeadPosition();
	
/*
	INT cnt = m_pMainDM->GetRowCount();
	m_nFirstIdx = 0;
	m_nLastIdx = cnt-1;
*/
}

VOID CDonateDataView::ShowNextPrevButton(INT idx)
{
	GetDlgItem(IDC_btnDVIEW_PREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnDVIEW_NEXT)->EnableWindow(TRUE);

	if(m_nFirstIdx== idx || idx < 0)
		GetDlgItem(IDC_btnDVIEW_PREV)->EnableWindow(FALSE);
	if(m_nLastIdx == idx || idx < 0)
		GetDlgItem(IDC_btnDVIEW_NEXT)->EnableWindow(FALSE);
}

INT CDonateDataView::MakeDonateData(INT nIdx)
{
	INT nCurSel = m_tabDonateData.GetCurSel();
	INT ids = 0;
	if(nCurSel == 0)
	{
		if(!m_bBO_Select)
		{
			ids = MakeBO_DonateData(m_pMainDM, m_pBO_DM, m_pBO_PrintDM, nIdx);
			m_bBO_Select = TRUE;
		}
	}
	else if(nCurSel == 1)
	{
		if(!m_bSE_Select)
		{
			ids = MakeSE_DonateData(m_pMainDM, m_pSE_DM, nIdx);
			m_bSE_Select = TRUE;
		}
	}

	return ids;
}

CESL_DataMgr * CDonateDataView::GetActiveDM()
{
	CESL_DataMgr * pDM = NULL;
	INT nCurSel = m_tabDonateData.GetCurSel();
	if(nCurSel == 0)
		pDM = m_pBO_PrintDM;
	else if(nCurSel == 1)
		pDM = m_pSE_DM;
	return pDM;
}

CESL_Grid * CDonateDataView::GetActiveGrid()
{
	CESL_Grid * pGrid = NULL;
	INT nCurSel = m_tabDonateData.GetCurSel();
	if(nCurSel == 0)
		pGrid = m_pBOGrid;
	else if(nCurSel == 1)
		pGrid = m_pSEGrid;
	return pGrid;
}

INT CDonateDataView::MakeSE_DonateData(CESL_DataMgr * pDonatorDM, CESL_DataMgr * pSE_DM, INT nIdx)
{
	if(m_pDonateDataSearchDlg == NULL) return -1;
	CString strDonateDate1, strDonateDate2, strTmpQuery, strDateQuery;
	strDonateDate1 = m_pDonateDataSearchDlg->GetDonateDate1();
	strDonateDate2 = m_pDonateDataSearchDlg->GetDonateDate2();
	
	strTmpQuery = MakeDateRangeQuery(_T("ACQ_DATE"), strDonateDate1, strDonateDate2);
	if(!strTmpQuery.IsEmpty())
	{
		strDateQuery.Format(_T(" AND A.REC_KEY IN (SELECT ACQ_TYPE_VOL_KEY FROM SE_BOOK_TBL WHERE REC_KEY IS NOT NULL %s)"), strTmpQuery);
	}

	CString strQuery, strDonatorKey;
	strDonatorKey = pDonatorDM->GetCellData(_T("����������KEY"), nIdx);
	if(strDonatorKey.IsEmpty()) return -1;

	strQuery.Format(_T("I.REC_KEY=D.SPECIES_KEY AND D.REC_KEY=A.ACQ_KEY AND A.VOL_KEY=V.REC_KEY AND D.DONATOR_KEY=%s"), strDonatorKey);
	strQuery += strDateQuery;
	pSE_DM->FreeData();
	INT ids = pSE_DM->RefreshDataManager(strQuery);
	if(ids < 0) return ids;

	pSE_DM->SORT(_T("UDF_�Լ���"));

	return 0;
}

INT CDonateDataView::MakeBO_DonateData(CESL_DataMgr * pDonatorDM, CESL_DataMgr * pBO_DM, CESL_DataMgr * pBO_PrintDM, INT nIdx)
{
	//�������� ��������
	if(m_pDonateDataSearchDlg == NULL) return -1;
	CString strDonateDate1, strDonateDate2, strDateQuery;
	strDonateDate1 = m_pDonateDataSearchDlg->GetDonateDate1();
	strDonateDate2 = m_pDonateDataSearchDlg->GetDonateDate2();
	
	strDateQuery = MakeDateRangeQuery(_T("P.INPUT_DATE"), strDonateDate1, strDonateDate2);

	CString strQuery, strDonatorKey;
	strDonatorKey = pDonatorDM->GetCellData(_T("����������KEY"), nIdx);
	if(strDonatorKey.IsEmpty()) return -1;
	
	strQuery.Format(_T("I.REC_KEY=S.REC_KEY AND I.REC_KEY=P.SPECIES_KEY AND P.REC_KEY=B.ACQ_KEY AND P.DONATOR_KEY = C.REC_KEY(+) AND P.DONATOR_KEY=%s"), strDonatorKey);
	strQuery += strDateQuery;
	
	pBO_DM->FreeData();
	INT ids = pBO_DM->RefreshDataManager(strQuery);
	if(ids < 0) return ids;
	pBO_PrintDM->FreeData();
	CBO_ACQ_Business_Api mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("åDM"), (DWORD*)pBO_DM);
	mgr.SetTempPtr(_T("��DM"), (DWORD*)pBO_PrintDM);
	ids = mgr.SPFExecutePath(_T("åTo��"));
	if(ids < 0) return ids;

	pBO_PrintDM->SORT(_T("BP_�Է���"));

	INT nRowCnt = pBO_PrintDM->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		pBO_PrintDM->SetCellData(_T("BP_��ǰå��"), pBO_PrintDM->GetCellData(_T("UDF_å��"), i), i);
	}
	return 0;
}

VOID CDonateDataView::OnSelchangetabDVIEWDONATEDATA(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SetTabSelect();
	*pResult = 0;
}

VOID CDonateDataView::SetTabSelect()
{
	INT nCurSel = m_tabDonateData.GetCurSel();
	if(nCurSel == 0)
	{
		m_pBOGrid->ShowWindow(SW_SHOW);
		m_pSEGrid->ShowWindow(SW_HIDE);
	}
	else if(nCurSel == 1)
	{
		m_pBOGrid->ShowWindow(SW_HIDE);
		m_pSEGrid->ShowWindow(SW_SHOW);
	}
	MakeDonateData(m_nIdx);
	GetActiveGrid()->Display();
}

INT CDonateDataView::MakeMergeDonateData(CESL_DataMgr *pBO_DM, CESL_DataMgr *pSE_DM, CESL_DataMgr *pMerge_DM)
{
	pMerge_DM->FreeData();
	INT nRowCnt = pBO_DM->GetRowCount();
	CString strTmpData;
	INT nCurRow;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		pMerge_DM->InsertRow(-1);
		nCurRow = pMerge_DM->GetRowCount()-1;
		strTmpData.Format(_T("����-%d"), i+1);
		pMerge_DM->SetCellData(_T("�Ϸù�ȣ"), strTmpData, nCurRow);
		pMerge_DM->SetCellData(_T("��������"), pBO_DM->GetCellData(_T("BP_�Է���"), i), nCurRow);
		pMerge_DM->SetCellData(_T("�ڷ��"), pBO_DM->GetCellData(_T("UDF_ǥ������"), i), nCurRow);
		pMerge_DM->SetCellData(_T("å��"), pBO_DM->GetCellData(_T("BP_��ǰå��"), i), nCurRow);
		pMerge_DM->SetCellData(_T("���ı���"), pBO_DM->GetCellData(_T("IBS_���ı���"), i), nCurRow);
		pMerge_DM->SetCellData(_T("��ü����"), pBO_DM->GetCellData(_T("IBS_��ü����"), i), nCurRow);
	}

	nRowCnt = pSE_DM->GetRowCount();
	for(i=0 ; i<nRowCnt ; i++)
	{
		pMerge_DM->InsertRow(-1);
		nCurRow = pMerge_DM->GetRowCount()-1;
		strTmpData.Format(_T("����-%d"), i+1);
		pMerge_DM->SetCellData(_T("�Ϸù�ȣ"), strTmpData, nCurRow);
		pMerge_DM->SetCellData(_T("��������"), pSE_DM->GetCellData(_T("SD_����������"), i), nCurRow);
		strTmpData.Format(_T("%s-%s"), pSE_DM->GetCellData(_T("ISS_ǥ������"), i), pSE_DM->GetCellData(_T("SV_��ȣ��"), i));
		pMerge_DM->SetCellData(_T("�ڷ��"), strTmpData, nCurRow);
		pMerge_DM->SetCellData(_T("å��"), pSE_DM->GetCellData(_T("UDF_å��"), i), nCurRow);
		pMerge_DM->SetCellData(_T("���ı���"), pSE_DM->GetCellData(_T("SV_���ı���_�ڵ�"), i), nCurRow);
		pMerge_DM->SetCellData(_T("��ü����"), pSE_DM->GetCellData(_T("SV_��ü����_�ڵ�"), i), nCurRow);
	}
	return 0;
}

//==================================================================
// [�Լ�����]
//  ���� ������ ���Ѵ�.
//==================================================================
INT CDonateDataView::GetSESpeicesCount(CESL_DataMgr *pSE_DM)
{
	pSE_DM->SORT(_T("ISS_��KEY"));
	INT nRowCnt = pSE_DM->GetRowCount();
	INT nTotalCnt = 0;
	CString strKey, strPrevKey;
	
	//2005-11-03 Ȳ���� ����
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strKey = pSE_DM->GetCellData(_T("ISS_��KEY"), i);
		if(strKey != strPrevKey)
		{
			nTotalCnt++;
			strPrevKey = strKey;
		}
	}
	return nTotalCnt;
}

//==================================================================
// [�Լ�����]
//  ���� å���� ���Ѵ�.
//==================================================================
INT CDonateDataView::GetSEBookCount(CESL_DataMgr *pSE_DM)
{
	INT nRowCnt = pSE_DM->GetRowCount();
	INT nTotalCnt = 0;
	CString strTmpData;
	
	//2005-11-03 Ȳ���� ����
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pSE_DM->GetCellData(_T("UDF_å��"), i);
		nTotalCnt += _ttoi(strTmpData.GetBuffer(0));
	}
	return nTotalCnt;	
}

INT CDonateDataView::SetPrintDonateConfirmData(CESL_DataMgr *pDonatorDM,
											   CESL_DataMgr *pBO_BookDM, 
											   CESL_DataMgr *pBO_PrintDM, 
											   CESL_DataMgr *pSE_DM, 
											   CESL_DataMgr *pPrintDM, 
											   CBasicPrintMgr *pPrintMgr, 
											   CLibInfo * pLibInfo, 
											   INT nIdx)
{
	if(pDonatorDM == NULL || pBO_BookDM == NULL || pBO_PrintDM == NULL ||
		pSE_DM == NULL || pPrintDM == NULL || pPrintMgr == NULL || pLibInfo == NULL) 
		return -1;
	
	CString strBOSpeciesCnt, strBOBookCnt, strSESpeciesCnt, strSEBookCnt, strMsg, strDonator, strDonateData;
	pPrintMgr->SetPrintDM(pPrintDM);
	pPrintMgr->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);
	pPrintMgr->SetSIReportFilename(_T("K2UP_�����ڷ��μ���"));
	pPrintMgr->InitPrintData();
	strBOSpeciesCnt.Format(_T("%d"), pBO_PrintDM->GetRowCount());
	strBOBookCnt.Format(_T("%d"), pBO_BookDM->GetRowCount());
	strSESpeciesCnt.Format(_T("%d"), GetSESpeicesCount(pSE_DM));
	strSEBookCnt.Format(_T("%d"), GetSEBookCount(pSE_DM));
	strDonator = pDonatorDM->GetCellData(_T("������"), nIdx);
	strDonateData = pPrintDM->GetCellData(_T("�ڷ��"), 0);
	pPrintMgr->AddPrintData(_T("������"), pDonatorDM->GetCellData(_T("������"), nIdx));
	pPrintMgr->AddPrintData(_T("�ּ�"), pDonatorDM->GetCellData(_T("�ּ�"), nIdx));
	pPrintMgr->AddPrintData(_T("��������"), strBOSpeciesCnt);
	pPrintMgr->AddPrintData(_T("����å��"), strBOBookCnt);
	pPrintMgr->AddPrintData(_T("��������"), strSESpeciesCnt);
	pPrintMgr->AddPrintData(_T("����å��"), strSEBookCnt);
	pPrintMgr->AddPrintData(_T("��������"), pLibInfo->GetLibName());
	pPrintMgr->AddPrintData(_T("�����ڷ�"), strDonateData);
	return 0;
}

VOID CDonateDataView::OnbtnDONATEDATESEARCH() 
{
	// TODO: Add your control notification handler code here
	ShowDonateDataSearchDlg();
}

UINT CDonateDataView::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	Display(1000);
	return 0;
}

CString CDonateDataView::MakeDateRangeQuery(CString strFieldName, CString strDate1, CString strDate2)
{
	CString strQuery;
	if(!strDate1.IsEmpty() && strDate2.IsEmpty())
	{
		strQuery.Format(_T(" AND %s = '%s'"), strFieldName, strDate1);
	}
	else if(!strDate1.IsEmpty() && !strDate2.IsEmpty())
	{
		strQuery.Format(_T(" AND %s BETWEEN '%s' AND '%s'"), strFieldName, strDate1, strDate2);
	}
	return strQuery;
}

VOID CDonateDataView::ShowDonateDataSearchDlg()
{
	if(m_pDonateDataSearchDlg == NULL) 
		return;
	if(!m_pDonateDataSearchDlg->GetCreate()) 
		m_pDonateDataSearchDlg->Create(this);
	m_pDonateDataSearchDlg->ShowWindow(SW_SHOW);
	m_pDonateDataSearchDlg->UpdateWindow();
	m_pDonateDataSearchDlg->CenterWindow();
}

VOID CDonateDataView::ShowDonateDataSearchDlgByModal()
{
	if(m_pDonateDataSearchDlg == NULL) 
		m_pDonateDataSearchDlg = new CDonateDataSearchDlg(pParentMgr);
	m_pDonateDataSearchDlg->DoModal();	
}

VOID CDonateDataView::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
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
	
	CWnd * pTab = GetDlgItem(IDC_tabDVIEW_DONATE_DATA);
	
	CRect rect;
	GetClientRect(rect);
	rect.top += 87;
	rect.bottom -= 4;
	rect.left += 4;
	rect.right -= 4;
	
	if(pTab) pTab->MoveWindow(rect);		

	rect.top += 24;
	rect.bottom -= 6;
	rect.left += 6;
	rect.right -= 6;
	if(m_pBOGrid) m_pBOGrid->MoveWindow(rect);
	if(m_pSEGrid) m_pSEGrid->MoveWindow(rect);
}

HBRUSH CDonateDataView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;

}
