// BO_LOC_38201.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_38201.h"
#include "BO_LOC_3824.h"
// 2011.08.26 ADD : 3000Api
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_GRID_CNT	3000
/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3820 dialog


CBO_LOC_3820::CBO_LOC_3820(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3820)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pSendDlg = NULL;
	// KOL.UDF.022
	sDirectQuery = _T("");
	nDirectCount = 0;
	m_nGridMode = 0;
	m_sMailClass = _T("");
	m_nMailClass = 0;
	m_nMailKey = 0;
	m_sIsUnityLoanService = _T("");
}

CBO_LOC_3820::~CBO_LOC_3820()
{
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]
	delete m_pSendDlg;
}

VOID CBO_LOC_3820::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3820)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3820, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3820)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_tBOOK, OnSelchangetBOOK)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3820 message handlers

BOOL CBO_LOC_3820::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_LOC_3820::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	INT ids;

	ids = InitESL_Mgr(_T("SM_BO_LOC_3820"));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitESL_Mgr") );
	
	m_nGridMode = 0;
	ids = DisplayGrid();

	InitTabCtrl();

	// 2010.09.03 ADD BY KSJ : ���յ������� ��뿩��
	GetManageValue(_T("��Ÿ"), _T("����"), _T("���յ�������"), _T("���յ�������"), m_sIsUnityLoanService );
	m_sIsUnityLoanService.TrimLeft();	m_sIsUnityLoanService.TrimRight();
	m_sIsUnityLoanService.MakeUpper();
	if(_T("Y")!=m_sIsUnityLoanService) m_sIsUnityLoanService = _T("N");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_LOC_3820::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if(m_nGridMode==0){//LoanGrid
		ReSize0( cx, cy );
	}else if(m_nGridMode==1){//UserGrid
		ReSize1( cx, cy );
	}else if(m_nGridMode==2){//UserGrid + BoBookGrid + SeBookGrid
		ReSize2( cx, cy );
	}else if(m_nGridMode==3){//FurnishGrid
		ReSize3( cx, cy );
	}else if(m_nGridMode==4){//UserGrid + PollGrid
		ReSize4( cx, cy );
	}else if(m_nGridMode==5){//NonUserGrid
		ReSize5( cx, cy );
	}
}

INT CBO_LOC_3820::ViewInitGrid()
{
	EFS_BEGIN

	if(m_nGridMode==0){//LoanGrid
		GetDlgItem(IDC_tBOOK)->ShowWindow(FALSE);
		GetDlgItem(IDC_sUSER_LIST)->ShowWindow(FALSE);
		GetDlgItem(IDC_sBOOK_LIST)->ShowWindow(FALSE);
		GetDlgItem(IDC_sPOLL_LIST)->ShowWindow(FALSE);

		GetDlgItem(IDC_gMAIN_GRID_LOAN)->ShowWindow(TRUE);
		GetDlgItem(IDC_gMAIN_GRID_USER)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_BO_BOOK)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_SE_BOOK)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_FURNISH)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_POLL)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_NONUSER)->ShowWindow(FALSE);
	}else if(m_nGridMode==1){//UserGrid
		GetDlgItem(IDC_tBOOK)->ShowWindow(FALSE);
		GetDlgItem(IDC_sUSER_LIST)->ShowWindow(FALSE);
		GetDlgItem(IDC_sBOOK_LIST)->ShowWindow(FALSE);
		GetDlgItem(IDC_sPOLL_LIST)->ShowWindow(FALSE);

		GetDlgItem(IDC_gMAIN_GRID_LOAN)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_USER)->ShowWindow(TRUE);
		GetDlgItem(IDC_gMAIN_GRID_BO_BOOK)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_SE_BOOK)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_FURNISH)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_POLL)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_NONUSER)->ShowWindow(FALSE);
	}else if(m_nGridMode==2){//UserGrid + BoBookGrid + SeBookGrid
		GetDlgItem(IDC_tBOOK)->ShowWindow(TRUE);
		GetDlgItem(IDC_sUSER_LIST)->ShowWindow(TRUE);
		GetDlgItem(IDC_sBOOK_LIST)->ShowWindow(TRUE);
		GetDlgItem(IDC_sPOLL_LIST)->ShowWindow(FALSE);

		GetDlgItem(IDC_gMAIN_GRID_LOAN)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_USER)->ShowWindow(TRUE);
		GetDlgItem(IDC_gMAIN_GRID_BO_BOOK)->ShowWindow(TRUE);
		GetDlgItem(IDC_gMAIN_GRID_SE_BOOK)->ShowWindow(TRUE);
		GetDlgItem(IDC_gMAIN_GRID_FURNISH)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_POLL)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_NONUSER)->ShowWindow(FALSE);
	}else if(m_nGridMode==3){//FurnishGrid
		GetDlgItem(IDC_tBOOK)->ShowWindow(FALSE);
		GetDlgItem(IDC_sUSER_LIST)->ShowWindow(FALSE);
		GetDlgItem(IDC_sBOOK_LIST)->ShowWindow(FALSE);
		GetDlgItem(IDC_sPOLL_LIST)->ShowWindow(FALSE);

		GetDlgItem(IDC_gMAIN_GRID_LOAN)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_USER)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_BO_BOOK)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_SE_BOOK)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_FURNISH)->ShowWindow(TRUE);
		GetDlgItem(IDC_gMAIN_GRID_POLL)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_NONUSER)->ShowWindow(FALSE);
	}else if(m_nGridMode==4){//UserGrid + PollGrid
		GetDlgItem(IDC_tBOOK)->ShowWindow(FALSE);
		GetDlgItem(IDC_sUSER_LIST)->ShowWindow(TRUE);
		GetDlgItem(IDC_sBOOK_LIST)->ShowWindow(FALSE);
		GetDlgItem(IDC_sPOLL_LIST)->ShowWindow(TRUE);

		GetDlgItem(IDC_gMAIN_GRID_LOAN)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_USER)->ShowWindow(TRUE);
		GetDlgItem(IDC_gMAIN_GRID_BO_BOOK)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_SE_BOOK)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_FURNISH)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_POLL)->ShowWindow(TRUE);
		GetDlgItem(IDC_gMAIN_GRID_NONUSER)->ShowWindow(FALSE);
	}else if(m_nGridMode==5){//NonUserGrid
		GetDlgItem(IDC_tBOOK)->ShowWindow(FALSE);
		GetDlgItem(IDC_sUSER_LIST)->ShowWindow(FALSE);
		GetDlgItem(IDC_sBOOK_LIST)->ShowWindow(FALSE);
		GetDlgItem(IDC_sPOLL_LIST)->ShowWindow(FALSE);

		GetDlgItem(IDC_gMAIN_GRID_LOAN)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_USER)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_BO_BOOK)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_SE_BOOK)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_FURNISH)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_POLL)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID_NONUSER)->ShowWindow(TRUE);
	}

	CRect WndPos;
	GetWindowRect(&WndPos);
	
	if(m_nGridMode == 0 ){
		ReSize0(WndPos.Width(), WndPos.Height());
	}else if(m_nGridMode == 1 ){
		ReSize1(WndPos.Width(), WndPos.Height());
	}else if(m_nGridMode == 2 ){
		ReSize2(WndPos.Width(), WndPos.Height());
	}else if(m_nGridMode == 3 ){
		ReSize3(WndPos.Width(), WndPos.Height());
	}else if(m_nGridMode == 4 ){
		ReSize4(WndPos.Width(), WndPos.Height());
	}else if(m_nGridMode == 5 ){
		ReSize5(WndPos.Width(), WndPos.Height());
	}

	return 0;
	EFS_END
	return -1;
}

VOID CBO_LOC_3820::ReSize0(INT cx, INT cy)
{
	CMSHFlexGrid *pGrid = NULL;
	pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID_LOAN );
	if( pGrid->GetSafeHwnd() )
		pGrid->MoveWindow( 0, 0, cx, cy );
}

VOID CBO_LOC_3820::ReSize1(INT cx, INT cy)
{
	CMSHFlexGrid *pGrid = NULL;
	pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID_USER );
	if( pGrid->GetSafeHwnd() )
		pGrid->MoveWindow( 0, 0, cx, cy );
}

VOID CBO_LOC_3820::ReSize2(INT cx, INT cy)
{
	const UINT staticWidth  = 90;
	const UINT staticHeight = 12;
	const UINT space        = 5;
	INT gridWidth  = cx - 10;
	INT gridHeight = cy / 3 - 25;
	
	CStatic *pStatic = NULL;
	pStatic = ( CStatic * )GetDlgItem( IDC_sUSER_LIST );
	if( pStatic->GetSafeHwnd() )
		pStatic->MoveWindow( space, space, staticWidth, staticHeight );
	pStatic = ( CStatic * )GetDlgItem( IDC_sBOOK_LIST );
	if( pStatic->GetSafeHwnd() )
		pStatic->MoveWindow( space, space * 4 + staticHeight + gridHeight, staticWidth, staticHeight );
	
	CMSHFlexGrid *pGrid = NULL;
	pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID_USER );
	if( pGrid->GetSafeHwnd() )
		pGrid->MoveWindow( space, space * 2 + staticHeight, gridWidth, gridHeight );
	
	CTabCtrl *pTab = ( CTabCtrl * )GetDlgItem( IDC_tBOOK );
	INT tabY = ( space * 5 ) + ( staticHeight * 2 ) + gridHeight;
	INT tabHeight = cy - gridHeight - ( staticHeight * 2 ) - ( space * 5 );
	if( pTab->GetSafeHwnd() )
		pTab->MoveWindow( 0, tabY, cx, tabHeight );

	INT bookGridWidth = cx;//gridWidth - ( space * 2 );
	pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID_BO_BOOK );
	if( pGrid->GetSafeHwnd() )
		pGrid->MoveWindow( space, tabY + 30, gridWidth, tabHeight - 35 );
	pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID_SE_BOOK );
	if( pGrid->GetSafeHwnd() )
		pGrid->MoveWindow( space, tabY + 30, gridWidth, tabHeight - 35 );
}

VOID CBO_LOC_3820::ReSize3(INT cx, INT cy)
{
	CMSHFlexGrid *pGrid = NULL;
	pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID_FURNISH );
	if( pGrid->GetSafeHwnd() )
		pGrid->MoveWindow( 0, 0, cx, cy );
}

VOID CBO_LOC_3820::ReSize4(INT cx, INT cy)
{
	const UINT staticWidth  = 90;
	const UINT staticHeight = 12;
	const UINT space        = 5;
	INT gridWidth  = cx - 10;
	INT gridHeight = cy / 3 - 25;
	
	CStatic *pStatic = NULL;
	pStatic = ( CStatic * )GetDlgItem( IDC_sUSER_LIST );
	if( pStatic->GetSafeHwnd() )
		pStatic->MoveWindow( space, space, staticWidth, staticHeight );
	pStatic = ( CStatic * )GetDlgItem( IDC_sPOLL_LIST );
	if( pStatic->GetSafeHwnd() )
		pStatic->MoveWindow( space, space * 4 + staticHeight + gridHeight, staticWidth, staticHeight );
	
	CMSHFlexGrid *pGrid = NULL;
	pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID_USER );
	if( pGrid->GetSafeHwnd() )
		pGrid->MoveWindow( space, space * 2 + staticHeight, gridWidth, gridHeight );
	
	CTabCtrl *pTab = ( CTabCtrl * )GetDlgItem( IDC_tBOOK );
	INT tabY = ( space * 5 ) + ( staticHeight * 2 ) + gridHeight;
	INT tabHeight = cy - gridHeight - ( staticHeight * 2 ) - ( space * 5 );
	//if( pTab->GetSafeHwnd() )
	//	pTab->MoveWindow( 0, tabY, cx, tabHeight );

	INT bookGridWidth = cx;//gridWidth - ( space * 2 );
	pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID_POLL );
	if( pGrid->GetSafeHwnd() )
		pGrid->MoveWindow( space, tabY + 0, gridWidth, tabHeight - 5 );
}

VOID CBO_LOC_3820::ReSize5(INT cx, INT cy)
{
	CMSHFlexGrid *pGrid = NULL;
	pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gMAIN_GRID_NONUSER );
	if( pGrid->GetSafeHwnd() )
		pGrid->MoveWindow( 0, 0, cx, cy );
}

INT CBO_LOC_3820::SearchLoanUser(CString &sWhere, CString &sMailClass)
{
	EFS_BEGIN

	INT ids=0;
	CString sWhereTemp;
	CString sTemp;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3820") );
	if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3820::SearchLoanUser() := FindDM()") );
	
	if(sMailClass.Compare(_T("����ȳ�(�ݳ���)"))==0){
		sTemp = _T(" AND L.STATUS='3' AND ( SELECT COUNT(*) FROM LH_STORAGE_V01 L1 WHERE L1.BOOK_KEY=L.BOOK_KEY AND L1.STATUS = '1') > 0 AND ( SELECT COUNT(*) FROM LS_WORK_T01 L3 WHERE L3.BOOK_KEY=L.BOOK_KEY AND L3.STATUS IN ('0','2') ) = 0 AND L.RESERVATION_DATE = (SELECT MIN(L2.RESERVATION_DATE) FROM LS_WORK_T01 L2 WHERE L2.BOOK_KEY = L.BOOK_KEY AND L2.STATUS='3')");
		sWhereTemp = sWhere + sTemp;
	}else if(sMailClass.Compare(_T("����ȳ�(��ġ�ڷΌ��)"))==0){
		sTemp = _T(" AND L.STATUS='3' AND ( L.BOOK_KEY IN (select rec_key from bo_book_tbl where working_status = 'BOL112N') OR L.BOOK_KEY IN (select rec_key from se_book_tbl where working_status IN ('SEL112N','SEL212N')) ) AND ( SELECT COUNT(*) FROM LS_WORK_T01 L3 WHERE L3.BOOK_KEY=L.BOOK_KEY AND L3.STATUS IN ('0','2') ) = 0 AND L.RESERVATION_DATE = (SELECT MIN(L2.RESERVATION_DATE) FROM LS_WORK_T01 L2 WHERE L2.BOOK_KEY = L.BOOK_KEY AND L2.STATUS='3')");
		sWhereTemp = sWhere + sTemp;
	}else if(sMailClass.Compare(_T("�ݳ�����"))==0){
		sTemp = _T(" AND L.STATUS IN ('0','2') AND TO_CHAR(RETURN_PLAN_DATE,'YYYY/MM/DD') <= TO_CHAR(SYSDATE-1,'YYYY/MM/DD')");
		sWhereTemp = sWhere + sTemp;
	}else if(sMailClass.Compare(_T("����ȳ�"))==0){
		sTemp = _T(" AND L.STATUS IN ('0','2')");
		sWhereTemp = sWhere + sTemp;
	}else if(sMailClass.Compare(_T("�ݳ�Ȯ��(��ü�ȵ��ڷ�)"))==0){
		sTemp = _T(" AND L.STATUS = '1' AND TO_CHAR(RETURN_DATE,'YYYY/MM/DD') <= TO_CHAR(RETURN_PLAN_DATE,'YYYY/MM/DD')");
		sWhereTemp = sWhere + sTemp;
	}else if(sMailClass.Compare(_T("�ݳ�Ȯ��(��ü���ڷ�)"))==0){
		sTemp = _T(" AND L.STATUS = '1' AND TO_CHAR(RETURN_DATE,'YYYY/MM/DD') > TO_CHAR(RETURN_PLAN_DATE,'YYYY/MM/DD')");
		sWhereTemp = sWhere + sTemp;
	}else if(sMailClass.Compare(_T("�������"))==0){
		sTemp = _T(" AND L.STATUS = '4'");
		sWhereTemp = sWhere + sTemp;
	}else if(sMailClass.Compare(_T("�ݳ�����"))==0){
		sTemp = _T(" AND L.STATUS = '2'");
		sWhereTemp = sWhere + sTemp;
	}else if(sMailClass.Compare(_T("�ݳ������ȳ�"))==0){
		sTemp = _T(" AND L.STATUS IN ('0','2') AND TO_CHAR(RETURN_PLAN_DATE,'YYYY/MM/DD') >= TO_CHAR(SYSDATE,'YYYY/MM/DD')");
		sWhereTemp = sWhere + sTemp;
	}else if(sMailClass.Compare(_T("����ݳ�����"))==0){
		sTemp = _T(" AND L.STATUS IN ('0','2') AND TO_CHAR(RETURN_PLAN_DATE,'YYYY/MM/DD') <= TO_CHAR(SYSDATE-1,'YYYY/MM/DD')");
		sWhereTemp = sWhere + sTemp;
	}else{
		sWhereTemp = sTemp;
	}

	//MessageBox(sWhereTemp);
	ids = FreeAllDM();
	pDM->RefreshDataManager(_T(" 1>0 ") + sWhereTemp);

	if(pDM->GetRowCount() < 1){
		AfxMessageBox(_T("�˻� ����� �����ϴ�."));
		return 0;
	}
	
	// 2011.09.07 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse)
	{
		CStringArray asDes1,asDes2;
		CString strGetData;
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->GetCellData(_T("�̸���"), i, strGetData);
			asDes1.Add(strGetData);
			pDM->GetCellData(_T("�ڵ���"), i, strGetData);
			asDes2.Add(strGetData);
		}
		CBO_LOC_3000Api *pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes1);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes2);
					pApi->CloseSocket();
				}
				for(i = 0; i < pDM->GetRowCount(); i++)
				{
					pDM->SetCellData(_T("�̸���"), asDes1.GetAt(i), i);
					pDM->SetCellData(_T("�ڵ���"), asDes2.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
	}

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM, _T("������������"), _T("����") );
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM, _T("����Ҽ�����"), _T("�Ҽ�") );

	
	ids = DisplayGrid();

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3820::SearchUser(CString &sWhere, CString &sMailClass)
{
	EFS_BEGIN

	INT ids=0;
	CString sWhereTemp;
	CString sTemp;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3820_USER") );
	if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3820::SearchUser() := FindDM()") );
	
	if(sMailClass.Compare(_T("�ű�ȸ���ȳ�"))==0){
		sWhereTemp = sWhere + sTemp;
	}else if(sMailClass.Compare(_T("�������ϸ���"))==0){
		sWhereTemp = sWhere + sTemp;
	}else if(sMailClass.Compare(_T("��������"))==0){
		sWhereTemp = sWhere + sTemp;
	}else{
		sWhereTemp = sWhere + sTemp;
	}

	//MessageBox(sWhereTemp);
	ids = FreeAllDM();
	nDirectCount = 0;
	sDirectQuery.Empty();
	sTemp.Format(_T("select count(*) from co_loan_user_tbl U where 1>0 %s"), sWhereTemp);
	CString sCnt;
	pDM->GetOneValueQuery(sTemp , sCnt);
	if(_ttoi(sCnt) > MAX_GRID_CNT){
		sTemp.Format(_T("�˻� ����� %d���Դϴ�.\n����� �ʹ� ���� ȭ��� �̿��������� ��� ������ �� �����ϴ�.\n%d���� ������ ��� �����ðڽ��ϱ�?"), _ttoi(sCnt) , _ttoi(sCnt));
		if(MessageBox(sTemp, _T("Ȯ��"), MB_YESNO|MB_ICONQUESTION ) == IDNO ){
			return 0;
		}else{
			nDirectCount = _ttoi(sCnt);
			sDirectQuery = sWhereTemp;
		}
	}


	if(nDirectCount==0){
		pDM->RefreshDataManager(_T(" 1>0 ") + sWhereTemp);
		if(pDM->GetRowCount() < 1){
			AfxMessageBox(_T("�˻� ����� �����ϴ�."));
			return 0;
		}
	}

	// 2011.09.07 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse)
	{
		CStringArray asDes1,asDes2;
		CString strGetData;
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->GetCellData(_T("�̸���"), i, strGetData);
			asDes1.Add(strGetData);
			pDM->GetCellData(_T("�ڵ���"), i, strGetData);
			asDes2.Add(strGetData);
		}
		CBO_LOC_3000Api *pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes1);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes2);
					pApi->CloseSocket();
				}
				for(i = 0; i < pDM->GetRowCount(); i++)
				{
					pDM->SetCellData(_T("�̸���"), asDes1.GetAt(i), i);
					pDM->SetCellData(_T("�ڵ���"), asDes2.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
	}

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM, _T("������������"), _T("����") );
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM, _T("����Ҽ�����"), _T("�Ҽ�") );

	ids = DisplayGrid();

	if(nDirectCount > 0){
		if( m_pSendDlg == NULL )
		{
			m_pSendDlg = new CBO_LOC_3823( this );
			m_pSendDlg->pMain = this->pMain;
			
			m_pSendDlg->m_pInfo = this->m_pInfo;

			if( m_pSendDlg->Create( this ) )
			{
				m_pSendDlg->ShowWindow( SW_SHOW );
				m_pSendDlg->UpdateWindow();
				m_pSendDlg->CenterWindow();
			}		
		}
		else
			m_pSendDlg->ShowWindow(SW_SHOW);
	}

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3820::SearchUserNPoll(CString &sWhere, CString &sMailClass)
{
	EFS_BEGIN

	INT ids=0;
	CString sWhereTemp;
	CString sTemp;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3820_USER") );
	if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3820::SearchUserNPoll() := FindDM()") );

	CESL_DataMgr *pDM_POLL = FindDM( _T("DM_BO_LOC_3820_POLL") );
	if(pDM_POLL==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3820::SearchUserNPoll() := FindDM()") );

	
	if(sMailClass.Compare(_T("��������"))==0){
		sWhereTemp = sWhere + sTemp;
	}else{
		sWhereTemp = sTemp;
	}

	ids = FreeAllDM();

	//1. User
	sWhereTemp.Empty();
	sTemp.Empty();
	sWhereTemp = sWhere + sTemp;

	nDirectCount = 0;
	sDirectQuery.Empty();
	sTemp.Format(_T("select count(*) from co_loan_user_tbl U where 1>0 %s"), sWhereTemp);
	CString sCnt;
	pDM->GetOneValueQuery(sTemp , sCnt);
	if(_ttoi(sCnt) > MAX_GRID_CNT){
		sTemp.Format(_T("�˻� ����� %d���Դϴ�.\n����� �ʹ� ���� ȭ��� �̿��������� ��� ������ �� �����ϴ�.\n%d���� ������ ��� �����ðڽ��ϱ�?"), _ttoi(sCnt) , _ttoi(sCnt));
		if(MessageBox(sTemp, _T("Ȯ��"), MB_YESNO|MB_ICONQUESTION ) == IDNO ){
			return 0;
		}else{
			nDirectCount = _ttoi(sCnt);
			sDirectQuery = sWhereTemp;
		}
	}

	if(nDirectCount==0){
		pDM->RefreshDataManager(_T(" 1>0 ") + sWhereTemp);
		if(pDM->GetRowCount() < 1){
			AfxMessageBox(_T("�̿��� �˻� ����� �����ϴ�."));
			return 0;
		}
	}

	// 2011.09.07 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse)
	{
		CStringArray asDes1,asDes2;
		CString strGetData;
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->GetCellData(_T("�̸���"), i, strGetData);
			asDes1.Add(strGetData);
			pDM->GetCellData(_T("�ڵ���"), i, strGetData);
			asDes2.Add(strGetData);
		}
		CBO_LOC_3000Api *pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes1);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes2);
					pApi->CloseSocket();
				}
				for(i = 0; i < pDM->GetRowCount(); i++)
				{
					pDM->SetCellData(_T("�̸���"), asDes1.GetAt(i), i);
					pDM->SetCellData(_T("�ڵ���"), asDes2.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
	}

	sTemp.Empty();

	//2. Poll
	pDM_POLL->RefreshDataManager(_T(""));
	if( pDM_POLL->GetRowCount() < 1){
		AfxMessageBox(_T("�˻��� �������� �����ϴ�."));
		return 0;
	}

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM, _T("������������"), _T("����") );
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM, _T("����Ҽ�����"), _T("�Ҽ�") );
	
	
	ids = DisplayGrid();

	if(nDirectCount > 0){
		if( m_pSendDlg == NULL )
		{
			m_pSendDlg = new CBO_LOC_3823( this );
			m_pSendDlg->pMain = this->pMain;
			
			m_pSendDlg->m_pInfo = this->m_pInfo;

			if( m_pSendDlg->Create( this ) )
			{
				m_pSendDlg->ShowWindow( SW_SHOW );
				m_pSendDlg->UpdateWindow();
				m_pSendDlg->CenterWindow();
			}		
		}
		else
			m_pSendDlg->ShowWindow(SW_SHOW);
	}

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3820::SearchUserNBook(CString &sWhere, CString &sWhereBook, CString &sMailClass)
{
	EFS_BEGIN

	INT ids=0;
	CString sWhereTemp;
	CString sTemp;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3820_USER") );
	if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3820::SearchLoanUser() := FindDM()") );

	CESL_DataMgr *pDM_BO = FindDM( _T("DM_BO_LOC_3820_BO_BOOK") );
	if(pDM_BO==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3820::SearchLoanUser() := FindDM()") );

	CESL_DataMgr *pDM_SE = FindDM( _T("DM_BO_LOC_3820_SE_BOOK") );
	if(pDM_SE==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3820::SearchLoanUser() := FindDM()") );
	
	if(sMailClass.Compare(_T("�����ڷ�ȳ�"))==0){
		sWhereTemp = sWhere + sTemp;
	}else{
		sWhereTemp = sTemp;
	}

	ids = FreeAllDM();

	//1. User
	sWhereTemp.Empty();
	sTemp.Empty();
	sWhereTemp = sWhere + sTemp;
	//MessageBox(sWhereTemp);

	nDirectCount = 0;
	sDirectQuery.Empty();
	sTemp.Format(_T("select count(*) from co_loan_user_tbl U where 1>0 %s"), sWhereTemp);
	CString sCnt;
	pDM->GetOneValueQuery(sTemp , sCnt);
	if(_ttoi(sCnt) > MAX_GRID_CNT){
		sTemp.Format(_T("�˻� ����� %d���Դϴ�.\n����� �ʹ� ���� ȭ��� �̿��������� ��� ������ �� �����ϴ�.\n%d���� ������ ��� �����ðڽ��ϱ�?"), _ttoi(sCnt) , _ttoi(sCnt));
		if(MessageBox(sTemp, _T("Ȯ��"), MB_YESNO|MB_ICONQUESTION ) == IDNO ){
			return 0;
		}else{
			nDirectCount = _ttoi(sCnt);
			sDirectQuery = sWhereTemp;
		}
	}

	if(nDirectCount==0){
		pDM->RefreshDataManager(_T(" 1>0 ") + sWhereTemp);
		if(pDM->GetRowCount() < 1){
			AfxMessageBox(_T("�̿��� �˻� ����� �����ϴ�."));
			return 0;
		}
	}

	// 2011.09.07 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse)
	{
		CStringArray asDes1,asDes2;
		CString strGetData;
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->GetCellData(_T("�̸���"), i, strGetData);
			asDes1.Add(strGetData);
			pDM->GetCellData(_T("�ڵ���"), i, strGetData);
			asDes2.Add(strGetData);
		}
		CBO_LOC_3000Api *pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes1);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes2);
					pApi->CloseSocket();
				}
				for(i = 0; i < pDM->GetRowCount(); i++)
				{
					pDM->SetCellData(_T("�̸���"), asDes1.GetAt(i), i);
					pDM->SetCellData(_T("�ڵ���"), asDes2.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
	}

	//2. Book
	sWhereTemp = sWhereBook;

	//����
	sTemp.Empty();
	sTemp.Format(_T("select count(*) from BO_BOOK_TBL B, IDX_BO_TBL I where %s and I.MANAGE_CODE = UDF_MANAGE_CODE AND 1>0 %s "), pDM_BO->CONDITION.GetBuffer(0) , sWhereTemp);
	pDM_BO->GetOneValueQuery(sTemp, sCnt);
	if(_ttoi(sCnt) > 1000){
		sTemp.Format(_T("�����ڷ� �˻������ %s���Դϴ�. �˻������� �����Ͽ� �ֽʽÿ�."), sCnt);
		AfxMessageBox(sTemp);
		return 0;
	}

	pDM_BO->RefreshDataManager(_T(" 1>0 ") + sWhereTemp);

	//����
	sWhereTemp.Replace(_T("B.MEDIA_CODE"), _T("I.MEDIA_CODE"));
	sWhereTemp.Replace(_T("BO_IDX_TITLE"), _T("SE_IDX_TITLE"));
	sWhereTemp.Replace(_T("BO_IDX_AUTHOR"), _T("SE_IDX_AUTHOR"));
	
	sTemp.Empty();
	sTemp.Format(_T("select count(*) from SE_BOOK_TBL B, IDX_SE_TBL I where %s and I.MANAGE_CODE = UDF_MANAGE_CODE AND 1>0 %s "), pDM_BO->CONDITION.GetBuffer(0) , sWhereTemp);
	pDM_SE->GetOneValueQuery(sTemp, sCnt);
	if(_ttoi(sCnt) > 1000){
		sTemp.Format(_T("�����ڷ� �˻������ %s���Դϴ�. �˻������� �����Ͽ� �ֽʽÿ�."), sCnt);
		AfxMessageBox(sTemp);
		return 0;
	}

	pDM_SE->RefreshDataManager(_T(" 1>0 ") + sWhereTemp);

	if( ( pDM_BO->GetRowCount() + pDM_SE->GetRowCount() ) < 1){
		AfxMessageBox(_T("å���� �˻� ����� �����ϴ�."));
		return 0;
	}

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM, _T("������������"), _T("����") );
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM, _T("����Ҽ�����"), _T("�Ҽ�") );

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM_BO, _T("��ϱ���"), _T("��ϱ���") );
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM_BO, _T("��������"), _T("��������") );
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM_BO, _T("��ü����"), _T("��ü����") );
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM_SE, _T("��ϱ���"), _T("��ϱ���") );
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM_SE, _T("��������"), _T("��������") );
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM_SE, _T("��ü����"), _T("��ü����") );
	
	
	ids = DisplayGrid();

	if(nDirectCount > 0){
		if( m_pSendDlg == NULL )
		{
			m_pSendDlg = new CBO_LOC_3823( this );
			m_pSendDlg->pMain = this->pMain;
			
			m_pSendDlg->m_pInfo = this->m_pInfo;

			if( m_pSendDlg->Create( this ) )
			{
				m_pSendDlg->ShowWindow( SW_SHOW );
				m_pSendDlg->UpdateWindow();
				m_pSendDlg->CenterWindow();
			}		
		}
		else
			m_pSendDlg->ShowWindow(SW_SHOW);
	}

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3820::SearchFurnish(CString &sWhere, CString &sMailClass)
{
	EFS_BEGIN

	INT ids=0;
	CString sWhereTemp;
	CString sTemp;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3820_FURNISH") );
	if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3820::SearchFurnish() := FindDM()") );
	
	if(sMailClass.Compare(_T("��ġ����ȳ�"))==0){
		sWhereTemp = sWhere + sTemp;
	}else{
		sWhereTemp = sWhere + sTemp;
	}

	//MessageBox(sWhereTemp);
	ids = FreeAllDM();
	pDM->RefreshDataManager(_T(" 1>0 ") + sWhereTemp);

	if(pDM->GetRowCount() < 1){
		AfxMessageBox(_T("�˻� ����� �����ϴ�."));
		return 0;
	}

	// 2011.09.07 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse)
	{
		CStringArray asDes1,asDes2,asDes3;
		CString strGetData;
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->GetCellData(_T("�ֹε�Ϲ�ȣ"), i, strGetData);
			asDes1.Add(strGetData);
			pDM->GetCellData(_T("�̸���"), i, strGetData);
			asDes2.Add(strGetData);
			pDM->GetCellData(_T("�ڵ���"), i, strGetData);
			asDes3.Add(strGetData);
		}
		CBO_LOC_3000Api *pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("CIVIL_NO"), asDes1);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("EMAIL"), asDes2);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("HANDPHONE"), asDes3);
					pApi->CloseSocket();
				}
				for(i = 0; i < pDM->GetRowCount(); i++)
				{
					pDM->SetCellData(_T("�ֹε�Ϲ�ȣ"), asDes1.GetAt(i), i);
					pDM->SetCellData(_T("�̸���"), asDes2.GetAt(i), i);
					pDM->SetCellData(_T("�ڵ���"), asDes3.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
	}

	ids = DisplayGrid();

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3820::SearchNonUser(CString &sWhere, CString &sMailClass)
{
	EFS_BEGIN

	INT ids=0;
	CString sWhereTemp;
	CString sTemp;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3820_NONUSER") );
	if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3820::SearchNonUser() := FindDM()") );
	
	sWhereTemp = sWhere + sTemp;

	ids = FreeAllDM();
	pDM->RefreshDataManager(_T(" 1>0 ") + sWhereTemp);

	if(pDM->GetRowCount() < 1){
		AfxMessageBox(_T("�˻� ����� �����ϴ�."));
		return 0;
	}

	// 2011.09.07 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse)
	{
		CStringArray asDes1,asDes2,asDes3;
		CString strGetData;
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->GetCellData(_T("�ֹε�Ϲ�ȣ"), i, strGetData);
			asDes1.Add(strGetData);
			pDM->GetCellData(_T("�̸���"), i, strGetData);
			asDes2.Add(strGetData);
			pDM->GetCellData(_T("�ڵ���"), i, strGetData);
			asDes3.Add(strGetData);
		}
		CBO_LOC_3000Api *pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes1);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes2);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes3);
					pApi->CloseSocket();
				}
				for(i = 0; i < pDM->GetRowCount(); i++)
				{
					pDM->SetCellData(_T("�ֹε�Ϲ�ȣ"), asDes1.GetAt(i), i);
					pDM->SetCellData(_T("�̸���"), asDes2.GetAt(i), i);
					pDM->SetCellData(_T("�ڵ���"), asDes3.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
	}
	
	ids = DisplayGrid();

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3820::ChangeGridMode(CString &sMailClass, INT nMailClass, INT nMailKey)
{
	EFS_BEGIN

	m_sMailClass = sMailClass;
	m_nMailClass = nMailClass;
	m_nMailKey = nMailKey;

	if(sMailClass.Compare(_T("����ȳ�(�ݳ���)"))==0 || sMailClass.Compare(_T("����ȳ�(��ġ�ڷΌ��)"))==0){
		m_nGridMode = 0;
	}else if(sMailClass.Compare(_T("�ݳ�����"))==0 || sMailClass.Compare(_T("�ݳ�����"))==0 || sMailClass.Compare(_T("�ݳ������ȳ�"))==0 || sMailClass.Compare(_T("����ݳ�����"))==0){
		m_nGridMode = 0;
	}else if(sMailClass.Compare(_T("����ȳ�"))==0 || sMailClass.Compare(_T("�������"))==0){
		m_nGridMode = 0;
	}else if(sMailClass.Compare(_T("�ݳ�Ȯ��(��ü�ȵ��ڷ�)"))==0 || sMailClass.Compare(_T("�ݳ�Ȯ��(��ü���ڷ�)"))==0){
		m_nGridMode = 0;
	}else if(sMailClass.Compare(_T("�ű�ȸ���ȳ�"))==0){
		m_nGridMode = 1;
	}else if(sMailClass.Compare(_T("�����ڷ�ȳ�"))==0){
		m_nGridMode = 2;
		// 2009.10.21 UPDATE BY KSJ : Alias �� ����
	//DEL }else if(sMailClass.Compare(_T("��ġ����ȳ�"))==0){
	}else if(	sMailClass.Compare(_T("��ġ�������ȳ�"))==0 
			||	m_sMailClass.Compare(_T("��ġ����ȳ�(�谡��)"))==0 ){
		m_nGridMode = 3;
	}else if(sMailClass.Compare(_T("�������ϸ���"))==0){
		m_nGridMode = 1;
	}else if(sMailClass.Compare(_T("��������"))==0){
		m_nGridMode = 1;
	}else if(sMailClass.Compare(_T("��������"))==0){
		m_nGridMode = 4;
	}else if(sMailClass.Compare(_T("��ȸ������"))==0){
		m_nGridMode = 5;
	}else{
		m_nGridMode = 1;
	}

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3820::InitTabCtrl()
{
	EFS_BEGIN

	CTabCtrl *pTab = ( CTabCtrl * )GetDlgItem( IDC_tBOOK );
	pTab->InsertItem( 0, _T(" �� �� ") );
	pTab->InsertItem( 1, _T(" �� �� ") );
	pTab->SetCurSel( 0 );

	return 0;
	EFS_END
	return -1;
}

VOID CBO_LOC_3820::OnSelchangetBOOK(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	ViewBookGrid();

	*pResult = 0;
}

VOID CBO_LOC_3820::ViewBookGrid()
{
	const INT cnt = 2;
	UINT id[ cnt ] = {
		IDC_gMAIN_GRID_BO_BOOK, IDC_gMAIN_GRID_SE_BOOK };
	
	CTabCtrl *pTab = ( CTabCtrl * )GetDlgItem( IDC_tBOOK );
	bool flag[ cnt ] = { false, false };
	flag[ pTab->GetCurSel() ] = true;

	CMSHFlexGrid *pGrid;
	for( INT i = 0; i < cnt; i++ )
	{
		pGrid = ( CMSHFlexGrid * )GetDlgItem( id[ i ] );
		pGrid->ShowWindow( flag[ i ] );
	}
}


INT CBO_LOC_3820::FreeAllDM()
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3820") );
	if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3820::FreeAllDM() := FindDM()") );

	CESL_DataMgr *pDM_USER = FindDM( _T("DM_BO_LOC_3820_USER") );
	if(pDM_USER==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3820::FreeAllDM() := FindDM()") );

	CESL_DataMgr *pDM_BO = FindDM( _T("DM_BO_LOC_3820_BO_BOOK") );
	if(pDM_BO==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3820::FreeAllDM() := FindDM()") );

	CESL_DataMgr *pDM_SE = FindDM( _T("DM_BO_LOC_3820_SE_BOOK") );
	if(pDM_SE==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CBO_LOC_3820::FreeAllDM() := FindDM()") );

	CESL_DataMgr *pDM_FURNISH = FindDM( _T("DM_BO_LOC_3820_FURNISH") );
	if(pDM_FURNISH==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBO_LOC_3820::FreeAllDM() := FindDM()") );

	CESL_DataMgr *pDM_POLL = FindDM( _T("DM_BO_LOC_3820_POLL") );
	if(pDM_POLL==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CBO_LOC_3820::FreeAllDM() := FindDM()") );

	CESL_DataMgr *pDM_NONUSER = FindDM( _T("DM_BO_LOC_3820_NONUSER") );
	if(pDM_NONUSER==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CBO_LOC_3820::FreeAllDM() := FindDM()") );

	pDM->FreeData();
	pDM_USER->FreeData();
	pDM_BO->FreeData();
	pDM_SE->FreeData();
	pDM_FURNISH->FreeData();
	pDM_POLL->FreeData();
	pDM_NONUSER->FreeData();

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3820::DisplayGrid()
{
	EFS_BEGIN
		
	ViewInitGrid();

	if(m_nGridMode==0){//LoanGrid
		ControlDisplay( _T("CM_BO_LOC_3820"), _T("LoanGrid") );
	}else if(m_nGridMode==1){//UserGrid
		ControlDisplay( _T("CM_BO_LOC_3820"), _T("UserGrid") );
	}else if(m_nGridMode==2){//UserGrid + BoBookGrid + SeBookGrid
		ControlDisplay( _T("CM_BO_LOC_3820"), _T("UserGrid") );
		ControlDisplay( _T("CM_BO_LOC_3820"), _T("BoBookGrid") );
		ControlDisplay( _T("CM_BO_LOC_3820"), _T("SeBookGrid") );
	}else if(m_nGridMode==3){//FurnishGrid
		ControlDisplay( _T("CM_BO_LOC_3820"), _T("FurnishGrid") );
	}else if(m_nGridMode==4){//UserGrid + PollGrid
		ControlDisplay( _T("CM_BO_LOC_3820"), _T("UserGrid") );
		ControlDisplay( _T("CM_BO_LOC_3820"), _T("PollGrid") );
	}else if(m_nGridMode==5){//NonUserGrid
		ControlDisplay( _T("CM_BO_LOC_3820"), _T("NonUserGrid") );
	}

	return 0;
	EFS_END
	return -1;
}

BEGIN_EVENTSINK_MAP(CBO_LOC_3820, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_LOC_3820)
	ON_EVENT(CBO_LOC_3820, IDC_gMAIN_GRID_POLL, -601 /* DblClick */, OnDblClickgMAINGRIDPOLL, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_LOC_3820::OnDblClickgMAINGRIDPOLL() 
{
EFS_BEGIN

	CESL_Grid *pGrid = ( CESL_Grid * )FindControl( _T("CM_BO_LOC_3820"), _T("PollGrid") );

	if( pGrid->GetMouseRow() < 1 ) return;

	CBO_LOC_3824 Dlg(this);
	
	Dlg.DoModal();

EFS_END
}

HBRUSH CBO_LOC_3820::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 ��� [2008-05-15]
	���̾�α� & ����ƽ ������
	 ************************************/
	// ���⼭���� 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
