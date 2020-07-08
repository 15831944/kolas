// MNG_LIBINFO_MGR_1000.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_LIBINFO_MGR_1000.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "MNG_LIBINFO_MGR_2000.h"
#include "MNG_LIBINFO_MGR_3000.h"
#include "..\\..\\..\\����\\CAT_UNION_FIXED_FIELD\\DLG_008_BK.h"

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIBINFO_MGR_1000 dialog


CMNG_LIBINFO_MGR_1000::CMNG_LIBINFO_MGR_1000(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_LIBINFO_MGR_1000)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_cUseMANAGE_CODE = 'N';
}

CMNG_LIBINFO_MGR_1000::~CMNG_LIBINFO_MGR_1000()
{	
	
}

BOOL CMNG_LIBINFO_MGR_1000::Create(CWnd* pParentWnd)
{	
	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_LIBINFO_MGR_1000::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_LIBINFO_MGR_1000)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_LIBINFO_MGR_1000, CDialog)
	//{{AFX_MSG_MAP(CMNG_LIBINFO_MGR_1000)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR() 
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIBINFO_MGR_1000 message handlers

BOOL CMNG_LIBINFO_MGR_1000::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( 0 > InitESL_Mgr( _T("MNG_LIBINFO_MGR_1000") ))
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_LIBINFO_MGR_1000") );
	}
	
	InitMANAGE_CODE();
	SetTimer(0, 100, NULL);

	const INT gridCnt = 1;
	CString gridAlias[ gridCnt ] = { _T("���") };
	CArray< CString, CString >removeListArray;
	CESL_Grid *pGrid = NULL;
	for( INT i = 0; i < gridCnt; i++ )
	{
		pGrid = ( CESL_Grid * )FindControl( _T("CM_MNG_LIBINFO_MGR_1000"), gridAlias[ i ] );
		if( !pGrid )
			return -1;

		pGrid->InitPopupMenuItem();
		pGrid->AddPopupMenuItem( ( CView * )pMain, this->VIEW_STYLE, &removeListArray );
	}

	// 18.09.27 JMJ KOL.RED.2018.008
	// å�η�����(å�η� ����Ҷ� ���� ����(I:����, D:�л�))
	INT ids;
	CString strValue, strBookCooperativeMode;
	ids = GetManageValue(_T("����"), _T("����"), _T("å�η��ý���"), _T("å�η���뿩��"), strValue);
	if(0 > ids) 
	{
		return -1;
	}
	if(strValue.IsEmpty()) 
	{
		strBookCooperativeMode = _T("I");
	}
	else
	{
		if(strValue == _T("Y"))
		{ // �����
			ids = GetManageValue(_T("����"), _T("����"), _T("å�η��ý���"), _T("å�η�����"), strValue);
			if(0 > ids) 
			{
				return -1;
			}
			if(strValue.IsEmpty()) 
			{
				strBookCooperativeMode = _T("I");
			}
			else
			{
				strBookCooperativeMode = strValue;
			}
		}
		else
		{ // ������
			strBookCooperativeMode = _T("I");
		}
	}
	// �л�ý��� �������� ���� HIDE
	if(_T("D") == strBookCooperativeMode)
	{
		CString strButtonAlias;
		strButtonAlias = _T("����");
		pMain->SendMessage( USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE );
		strButtonAlias = _T("����");
		pMain->SendMessage( USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE );
		strButtonAlias = _T("�Է�");
		pMain->SendMessage( USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE );
	}

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_LIBINFO_MGR_1000::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	ResizeControl();
}

VOID CMNG_LIBINFO_MGR_1000::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	ResizeControl();
}

VOID CMNG_LIBINFO_MGR_1000::ResizeControl(VOID)
{
	if (GetSafeHwnd() == NULL) return;
	CWnd *pWnd = NULL;

	CRect rcDlg, rcControl;
	GetClientRect(rcDlg);

	pWnd = GetDlgItem(IDC_mngGRID_1);
	if (pWnd) {
		rcControl = rcDlg;
		rcControl.top = rcControl.top + 0;
		
		pWnd->MoveWindow(rcControl);
	}
	return;
}

INT CMNG_LIBINFO_MGR_1000::OnRefresh(VOID)
{
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_LIBINFO_MGR_1000"));
	if (pDM == NULL) return -1;
	CESL_Grid *pGrid = (CESL_Grid *)FindControl(_T("CM_MNG_LIBINFO_MGR_1000"), _T("���"));
	if (pGrid == NULL) return -2;

	CString strOption = _T("");
	if (m_cUseMANAGE_CODE == 'Y') {		// ��� �����ش�.(�ٸ� ���������� _T("�������")�� �� ����)
		strOption = _T("MANAGE_CODE IS NOT NULL");
	} else {							// MANAGE_CODE�� NULL �ΰ͸� �����ش�.
		strOption.Format(_T("MANAGE_CODE IS NULL"));
	}
	pDM->RefreshDataManager(strOption);
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("��������"), _T("��������_����"));
	pGrid->Display();

	return 0;
}

INT CMNG_LIBINFO_MGR_1000::OnInsert(VOID)
{
	CMNG_LIBINFO_MGR_2000 dlg(this);
	dlg.m_cUseMANAGE_CODE = m_cUseMANAGE_CODE;
	if (dlg.DoModal() != IDOK) return 0;

	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_LIBINFO_MGR_1000"));
	if (pDM == NULL) return -1;

	CString strMANAGE_CODE, strLIB_CODE, strTmp;
	strMANAGE_CODE = dlg.m_strMANAGE_CODE;
	strLIB_CODE = _T("");
	CString strQuery;
	if (strMANAGE_CODE.GetLength() == 0) {
		strQuery.Format(_T("SELECT COUNT(*) FROM	%s WHERE MANAGE_CODE IS NULL"), pDM->TBL_NAME);
	} else {
		strQuery.Format(_T("SELECT COUNT(*) FROM	%s WHERE MANAGE_CODE='%s'"), pDM->TBL_NAME, strMANAGE_CODE);
	}
	pDM->GetOneValueQuery(strQuery, strTmp);
	strTmp.TrimLeft();			strTmp.TrimRight();
	if (strTmp != _T("0")) {
		AfxMessageBox(_T("�̹� �ش� ���������� ����ϴ� �����Ͱ� �ֽ��ϴ�. �Է��� ��ҵ˴ϴ�."));
		return 0;
	}

	CString strPK;
	pDM->MakeRecKey(strPK);

	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("REC_KEY"),			_T("NUMERIC"), strPK);
	pDM->AddDBFieldData(_T("MANAGE_CODE"),		_T("STRING"),  dlg.m_strMANAGE_CODE);
	pDM->AddDBFieldData(_T("LIB_NAME"),			_T("STRING"),  dlg.m_strLIB_NAME);
	pDM->AddDBFieldData(_T("LIB_DIRECTOR_NAME"),_T("STRING"),  dlg.m_strLIB_DIRECTOR_NAME);
	pDM->AddDBFieldData(_T("LIB_ZIP_CODE"),		_T("STRING"),  dlg.m_strLIB_ZIP_CODE);
	pDM->AddDBFieldData(_T("LIB_ADDRESS"),		_T("STRING"),  dlg.m_strLIB_ADDRESS);
	pDM->AddDBFieldData(_T("LIB_TEL"),			_T("STRING"),  dlg.m_strLIB_TEL);
	pDM->AddDBFieldData(_T("LIB_FAX"),			_T("STRING"),  dlg.m_strLIB_FAX);
	pDM->AddDBFieldData(_T("LIB_URL"),			_T("STRING"),  dlg.m_strLIB_URL);
	pDM->AddDBFieldData(_T("LIB_EMAIL"),		_T("STRING"),  dlg.m_strLIB_EMAIL);
	pDM->AddDBFieldData(_T("FIRST_WORK"),		_T("STRING"),  GetWorkLogMsg( _T("���������Ͱ���"), __WFILE__, __LINE__ ), TRUE);
	pDM->AddDBFieldData(_T("LAST_WORK"),		_T("STRING"),  GetWorkLogMsg( _T("���������Ͱ���"), __WFILE__, __LINE__ ), TRUE);
	pDM->MakeInsertFrame(pDM->TBL_NAME);
	INT ids = pDM->SendFrame();
	pDM->EndFrame();

	if (ids < 0) {
		AfxMessageBox(_T("�Էµ��� ������ �߻��Ͽ����ϴ�."));
	} else {
		OnRefresh();
		AfxMessageBox(_T("�ԷµǾ����ϴ�."));
	}

	return 0;
}

INT CMNG_LIBINFO_MGR_1000::OnModify(INT bMakeSelectFlag/* = TRUE*/)
{
	CESL_Grid *pGrid = (CESL_Grid *)FindControl(_T("CM_MNG_LIBINFO_MGR_1000"), _T("���"));
	if (pGrid == NULL) return -2;
	if (bMakeSelectFlag == TRUE) {
		pGrid->SelectMakeList();
	}
	if (pGrid->SelectGetCount() == 0) {
		AfxMessageBox(_T("���� ������ �Ͽ� �ֽʽÿ�."));
		return 0;
	}
	
	CMNG_LIBINFO_MGR_3000 dlg(this);
	dlg.m_pGrid = pGrid;
	if (bMakeSelectFlag == FALSE) dlg.m_nStartRowIdx = m_nStartRowIdx;
	dlg.m_cUseMANAGE_CODE = m_cUseMANAGE_CODE;
	dlg.DoModal();
	if (dlg.m_bModifyFlag == TRUE) {
		/*
		CESL_DataMgr *pDM = FindDM(_T("DM_MNG_LIBINFO_MGR_1000"));
		if (pDM == NULL) return -1;
		m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("��������"), _T("��������_����"));
		pGrid->Display();
		*/
		OnRefresh();
	}
	return 0;
}

INT CMNG_LIBINFO_MGR_1000::OnDelete(VOID)
{
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_LIBINFO_MGR_1000"));
	if (pDM == NULL) return -1;
	if (pDM->GetRowCount() == 0) {
		AfxMessageBox(_T("�ڷᰡ �����ϴ�."));
		return 0;
	}
	CESL_Grid *pGrid = (CESL_Grid *)FindControl(_T("CM_MNG_LIBINFO_MGR_1000"), _T("���"));
	if (pGrid == NULL) return -2;
	pGrid->SelectMakeList();
	if (pGrid->SelectGetCount() == 0) {
		AfxMessageBox(_T("���� ������ �ڷḦ ������ �Ͽ� �ֽʽÿ�."));
		return 0;
	}
	if (AfxMessageBox(_T("�����Ͻ� �ڷḦ ���� �����Ͻðڽ��ϱ�?"), MB_YESNO) != IDYES) return 0;
	
	CString strRecKey, strTmp;
	INT nRows = pDM->GetRowCount();
	pDM->StartFrame();
	for (INT i = nRows-1; i >= 0; i--) {
		strTmp = pGrid->GetTextMatrix(i+1, 1);			strTmp.TrimLeft();			strTmp.TrimRight();
		if (strTmp.GetLength() == 0) continue;
		strRecKey = pDM->GetCellData(_T("REC_KEY"), i);
		pDM->InitDBFieldData();
		pDM->MakeDeleteFrame(pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strRecKey);
		pDM->DeleteRow(i);
	}
	INT ids = pDM->SendFrame();
	pDM->EndFrame();
	pGrid->Display();
	if (ids < 0) {
		AfxMessageBox(_T("�������� ������ �߻��Ͽ����ϴ�."));
	} else {
		OnRefresh();
		AfxMessageBox(_T("�����Ǿ����ϴ�."));
	}
	return 0;
}

VOID CMNG_LIBINFO_MGR_1000::InitMANAGE_CODE(VOID)
{
	m_cUseMANAGE_CODE = 'Y';
	m_strUSER_MANAGE_CODE = m_pInfo->MANAGE_CODE;

//	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIBINFO_MGR_1000") );
//	if (pDM == NULL) return;
//
//	CString strTmp, strQuery;
//	strTmp = _T("");
//	strQuery.Format(_T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE GROUP_1='L' AND GROUP_2='��Ÿ' AND GROUP_3='����' AND CLASS_ALIAS='�������к�����������'"));
//	pDM->GetOneValueQuery(strQuery, strTmp);
//	strTmp.TrimLeft();				strTmp.TrimRight();
//	strTmp.MakeUpper();
//	if (strTmp == _T("N")) m_cUseMANAGE_CODE = 'N';
//		else m_cUseMANAGE_CODE = 'Y';
//	
//	strQuery.Format(_T("SELECT MANAGE_CODE FROM MN_USER_TBL WHERE ID='%s'"), GetUserID());
//	pDM->GetOneValueQuery(strQuery, m_strUSER_MANAGE_CODE);
//	m_strUSER_MANAGE_CODE.TrimLeft();			m_strUSER_MANAGE_CODE.TrimRight();

	return;
}

VOID CMNG_LIBINFO_MGR_1000::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 0) {
		KillTimer(0);
		OnRefresh();
	}
	
	CDialog::OnTimer(nIDEvent);
}

BEGIN_EVENTSINK_MAP(CMNG_LIBINFO_MGR_1000, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_LIBINFO_MGR_1000)
	ON_EVENT(CMNG_LIBINFO_MGR_1000, IDC_mngGRID_1, -601 /* DblClick */, OnDblClickmngGRID1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_LIBINFO_MGR_1000::OnDblClickmngGRID1() 
{
	PreModify();
}

BOOL CMNG_LIBINFO_MGR_1000::IsInSelectedList(CESL_Grid *pGrid, INT nRow)
{
	if (pGrid == NULL) return FALSE;
	INT nCount = pGrid->SelectGetCount();
	POSITION pos;
	INT nData;
	for (INT i = 0; i < nCount; i++) {
		pos = pGrid->m_arraySelect.FindIndex(i);
		if (pos == NULL) continue;
		nData = pGrid->m_arraySelect.GetAt(pos);
		if (nData == nRow) return TRUE;
	}
	return FALSE;
}

VOID CMNG_LIBINFO_MGR_1000::PreModify()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIBINFO_MGR_1000") );
	if (pDM == NULL) return;
	INT nRows = pDM->GetRowCount();
	if (nRows == 0) return;
	CESL_Grid *pGrid = (CESL_Grid *)FindControl(_T("CM_MNG_LIBINFO_MGR_1000"), _T("���"));
	if (pGrid == NULL) return;
	INT i;
	pGrid->SelectMakeList();

	INT nRow = pGrid->GetRow() - 1;
	m_nStartRowIdx = nRow;
	BOOL bModify = FALSE;
	if (bModify == FALSE && pGrid->SelectGetCount() == 0) {
		pGrid->SelectRemoveAll();
		for (i = 0; i < nRows; i++) pGrid->m_arraySelect.AddTail(i);
		OnModify(FALSE);
		bModify = TRUE;
	}

	if (bModify == FALSE && pGrid->SelectGetCount() > 0 && IsInSelectedList(pGrid, nRow) == FALSE) {
		pGrid->m_arraySelect.AddTail(nRow);
		OnModify(FALSE);
		bModify = TRUE;
	}
	if (bModify == FALSE && pGrid->SelectGetCount() > 0 && IsInSelectedList(pGrid, nRow) == TRUE) {
		OnModify(FALSE);
		bModify = TRUE;
	}
}

HBRUSH CMNG_LIBINFO_MGR_1000::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}