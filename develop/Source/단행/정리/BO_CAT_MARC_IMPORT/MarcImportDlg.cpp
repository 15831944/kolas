// FileImportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MarcImportDlg.h"
#include "MarcEditDlg.h"
#include "efs.h"

#include "..\..\..\����\BO_SPECIES_API\BO_SPECIES.h"

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

	m_nOpenMode = 1000;
	m_pThreadImport = NULL;
	m_pThreadDisplay = NULL;

	m_bIsThreadRunning = FALSE;
	m_pCatApi = NULL;

	m_pDM_Index = NULL;
	m_pDM_Species = NULL;
	m_pDM_Book = NULL;
	m_pDM_Main = NULL;

	m_pGrid_OK = NULL;
	m_pGrid_ERR = NULL;
	m_pGrid_SUCCESS = NULL;

	m_pDlgKeyWord = NULL;

	m_lpszSMAlias = _T("SM_����_��ũ����");
	m_lpszCMAlias = _T("CM_����_��ũ����");
	m_lpszCMAlias_Code = _T("CM_����_��ũ����_�����ڵ�");	
	
	// 16/03/29 ������ : �Ѽ� �����ϱ�
//*/ ADD -------------------------------------------------------------------------------------
	m_bIsExcel = TRUE;
	CLSID clsExcelApp;
	if (!FAILED(CLSIDFromProgID(_T("HCell.Application"), &clsExcelApp))) 
	{
		m_bIsExcel = FALSE;	// 16/04/05 ������ : ��ȣ���� ������ ���� �ӽ��ּ�ó�� -> // 16/04/19 ������ : �ּ�Ǯ��
	}	
//*/ END -------------------------------------------------------------------------------------

	// KOL.RED.015 JMC 20160928 ��ũ���� ���Խ� ��������ȣ �ڵ� ������Ʈ
	m_bAutoReg = FALSE;
	m_bAutoAcq = FALSE;

	//2020.02.13 JOB.2020.0013 ADD BY KYJ : ��ġ��� ISBN ��Ī �����ؼ� �� �� �ֵ��� ��ɰ��� 
	m_bMatchISBN = FALSE;

	m_hThread= ::CreateEvent(NULL, FALSE, FALSE, _T("ThreadEvent"));
}

CMarcImportDlg::~CMarcImportDlg()
{
}

BOOL CMarcImportDlg::Create(CWnd* pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	return FALSE;


	EFS_END
	return FALSE;
}

VOID CMarcImportDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcImportDlg)	
	DDX_Text(pDX, IDC_eFILE_PATH, m_strFilePath);
	// 16/03/29 ������ : �Ѽ� �����ϱ�
//*/ ADD -------------------------------------------------------------------------------------
	if(m_bIsExcel != TRUE)
	{
		DDX_Text(pDX, IDC_rEXCEL, CString(_T("HCELL")));	// 16/04/05 ������ : ��ȣ���� ������ ���� �ӽ��ּ�ó�� -> // 16/04/19 ������ : �ּ�Ǯ��
	}
//*/ END -------------------------------------------------------------------------------------
	//2020.02.13 JOB.2020.0013 ADD BY KYJ : ��ġ��� ISBN ��Ī �����ؼ� �� �� �ֵ��� ��ɰ��� 
	DDX_Check(pDX, IDC_ChkMatchISBN, m_bMatchISBN);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CMarcImportDlg, CDialog)
	//{{AFX_MSG_MAP(CMarcImportDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bFILE_OPEN, OnbFILEOPEN)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_bIMPORT, OnbIMPORT)
	ON_BN_CLICKED(IDC_bEXPORT, OnbEXPORT)
	ON_BN_CLICKED(IDC_bMODIFY, OnbMODIFY)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_IMPORT, OnSelchangeTabImport)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_BN_CLICKED(IDC_bEXPORT_EDITMARC, OnbEXPORTEDITMARC)
	ON_BN_CLICKED(IDC_ChkGiveAutoRegno, OnChkGiveAutoRegno)
	ON_BN_CLICKED(IDC_ChkLastNoUp_REG, OnChkLastNoUp_REG)
	ON_BN_CLICKED(IDC_ChkLastNoUp_Acq, OnChkLastNoUp_Acq)
	ON_CBN_SELCHANGE(IDC_cmbMEDIA_CLASS, OnSelchangecmbMEDIACLASS)
	ON_BN_CLICKED(IDC_bInit, OnbInit)
	ON_BN_CLICKED(IDC_rTXT, OnrTXT)
	ON_BN_CLICKED(IDC_rEXCEL, OnrEXCEL)
	ON_BN_CLICKED(IDC_rMODS, OnrMODS)
	ON_BN_CLICKED(IDC_rCSV, OnrCSV)
	ON_CBN_SELCHANGE(IDC_cmbIMPORTTYPE, OnSelchangecmbIMPORTTYPE)
	ON_BN_CLICKED(IDC_ChkAccessionRec, OnChkAccessionRec)
	ON_BN_CLICKED(IDC_btnAccessionRecNo, OnbtnAccessionRecNo)
	ON_EN_KILLFOCUS(IDC_eAccessionRecNo, OnKillfocuseAccessionRecNo)
	//JOB.2019.0003 ADD BY KYJ : ���Գ⵵����� ���ι�ȣ�� ����ȵǴ� ���� ���� 
	ON_EN_KILLFOCUS(IDC_eACQUISIT_YEAR, OnKillfocuseAccessionYear)
	ON_BN_CLICKED(IDC_bNORMALIZATION, OnbNORMALIZATION)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_END_MARC_ADJUST, OnEndMarcAdjust)
	ON_MESSAGE(WM_GRID_DISPLAY, MsgDisplayGrid)
	ON_MESSAGE(WM_SORT_GRID, MsgSortGrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] ���� ������
// [Min Spec] 
// 1.Initialization Variable
// 2. ���� ���� �ð� üũ
// 3. ���õ� ������ŭ ����ٸ� �ʱ�ȭ�Ѵ�.
// 4. ������ ������� ��ũ���� DM���� �����Ѵ�.
// 5. �ڵ��ο��� üũ�Ǿ� ������� �ڵ��ο� DM ���� �� ��ũ�� �����Ѵ�.
// 6. åDM�� ���Ա���/�����/��Ϲ�ȣ ����
// 7. MARC�� 049$l�� ��� 12�ڸ��� �����
// 8. ���� �������¸� 1�� ����
// 9. ��������̸� �۾����¸� �������� DB�� INSERT�Ѵ�.
// 10. ������ �����̸� �۾����¸� ��������/��ǥå �谡�� �ο�/ DB�� INSERT�Ѵ�.
// 11. ������ �Ϸ�Ǹ� �����ǿ� �ű�� ���� �׸��忡 �ݿ��Ѵ�.
// 12. ���¹��� ������ �����Ѵ�.
// 13. ������ ���� ������ �����͸� �ű��.
// 14. ����ٸ� �������ش�.
/////////////////////////////////////////////////////////////////////////////
//===================================================
//2009.03.10 UPDATE BY PJW : ����
// UINT ThreadFunc_Import(LPVOID pParam)
// {
// 
// 	//=======================================================================
// 	// 1.Initialization Variable
// 	//=======================================================================
// 	CESL_ControlMgr *pCM = NULL;
// 	CESL_Grid *pGrid_OK = NULL;			// ������ �׸���
// 	CESL_Grid *pGrid_ERR = NULL;		// ������ �׸���
// 	CESL_Grid *pGrid_SUCCESS = NULL;	// ������ �׸���
// 	CESL_Grid *pGrid_TO = NULL;			// �ű� �׸���
// 	// ����������ų ID
// 	UINT nID_TO = -1;
// 	// ����������ų String
// 	CString strTO;
// 	// DataMgr
// 	CString strDMPK;
// 	
// 	CMarcImportDlg *pDlg = (CMarcImportDlg*)pParam;
// 	if (pDlg == NULL)
// 	{
// 		pDlg->m_bIsThreadRunning = FALSE;
// 		return -1;
// 	}
// 
// 	pDlg->m_bIsThreadRunning = TRUE;
// 	pDlg->ButtonStatus(0);
// 
// 	// ��DM
// 	CESL_DataMgr DM_Species;
// 	DM_Species.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
// 	DM_Species.InitDMPKFromFile(pDlg->m_lpszSpeciesDMAlias);
// 	DM_Species.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
// 
// 	// ����DM
// 	CESL_DataMgr DM_Index;
// 	DM_Index.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
// 	DM_Index.InitDMPKFromFile(pDlg->m_lpszIndexDMAlias);
// 	DM_Index.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
// 	
// 	// åDM
// 	CESL_DataMgr DM_Book;
// 	DM_Book.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
// 	DM_Book.InitDMPKFromFile(pDlg->m_lpszBookDMAlias);
// 	DM_Book.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
// 
// 	// ��DM
// 	CESL_DataMgr DM_Volume;
// 	DM_Volume.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
// 	DM_Volume.InitDMPKFromFile(pDlg->m_lpszVolumeDMAlias);
// 	DM_Volume.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
// 	
// 	INT ids = -1;
// 	INT idxDM = -1;
// 	INT nCount = -1;
// 
// 	INT nColCount = -1;
// 
// 	INT nRowTo = 0;
// 	INT nRowsTo = 0;
// 	
// 	CString strData;
// 	CString strErrorMsg;
//     CString nTotalCnt;
//     CString strTotalMsg;
//     CString nFalseCnt;
// 	CString nTrueCnt;
// 
// 	DWORD start_app, end_app;
// 	CString strMsg;		
// 	INT		nProcCount = 0;
// 
// 	// HSS_040524 �������
// 	CString strHD_Book, strHD_Species, strHD_Index;
// 	strHD_Species = _T("BS");
// 	strHD_Book = _T("BB");
// 	strHD_Index = _T("IBS");
// 
// 
// 	// �׸��� ����
// 	pGrid_OK = pDlg->m_pGrid_OK;
// 	pGrid_ERR = pDlg->m_pGrid_ERR;	
// 	pGrid_SUCCESS = pDlg->m_pGrid_SUCCESS;
// 	
// 	if (pGrid_OK == NULL || pGrid_ERR == NULL || pGrid_SUCCESS == NULL)
// 		goto ERR;
// 
// 	//=====================================================
// 	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	INT nRegCodeLength;
// 	nRegCodeLength = pDlg->m_pInfo->nREG_CODE_LENGTH;
// 	//=====================================================
// 
// 	//=======================================================================
// 	// 2. ���� ���� �ð� üũ (üũ�� ���ϴ��� �𸣰���.)
// 	//=======================================================================
// 	FILE *fpTime;
// 	fpTime = _tfopen( _T("..\\tunning\\���Լ���ð�.txt"), _T("a+b") );	
// 	
// 		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
// 		// KOLAS 2 UP UNICODE �����۾�
// 		// ���� �����ÿ��� UNICODE ����� �Է��ؾ��Ѵ�.
// 		fseek(fpTime, 0, SEEK_END);		
// 		if( 0 == ftell(fpTime) )
// 		{
// 			/*UNCHANGE*/fputc(0xFF, fpTime);
// 			/*UNCHANGE*/fputc(0xFE, fpTime);
// 		}
// 		// ------------------------------------------------------------------------------
// 		
// 	DWORD start_total, end_total;
// 	start_total = GetTickCount();
// 		
// 	//=======================================================================
// 	// 3. ���õ� ������ŭ ����ٸ� �ʱ�ȭ�Ѵ�.
// 	//=======================================================================
// 	nCount = pGrid_OK->SelectGetCount();
// 	if (nCount <= 0)
// 	{
// 		goto ERR;
// 	}
// 	
// 	pDlg->m_wndProgressBar.SetRange(0, nCount);
// 	pDlg->m_wndProgressBar.SetPos(0);
// 	pDlg->m_wndProgressBar.SetWindowText(_T(""));
// 
// 	nColCount = pGrid_OK->GetCols(0);
// 
// 	// ������ ����
// 	BOOL	bError;	
// 	while(TRUE)
// 	{
// 		bError = FALSE;
// 		start_app = GetTickCount();
// 		
// 		if (!pDlg->m_bIsThreadRunning) break;
// 		strErrorMsg.Empty();
// 		pGrid_OK->SelectMakeList();
// 		INT idx = pGrid_OK->SelectGetHeadPosition() + 1;
// 		if (idx <= 0) break;
// 
// 		strData = pGrid_OK->GetTextMatrix(idx, 0);
// 		if (strData.IsEmpty()) continue;
// 
// 		idxDM = _ttoi(strData) - 1 ;
// 		
// 		//===================================================================
// 		// 4. ������ ������� ��ũ���� DM���� �����Ѵ�.
// 		//===================================================================
// 		ids = pDlg->MakeMarcAdjustDM(pDlg->m_pDM_Species, idxDM, pDlg->m_pDM_Index, idxDM, &DM_Species, &DM_Index, &DM_Book, &DM_Volume);
// //		CString MngCode = DM_Book.GetCellData(_T("BB_��������_�ڵ�"), 0);
// 		CMarc marc;
// 		
// 		//===================================================================
// 		// 5. �ڵ��ο��� üũ�Ǿ� ������� �ڵ��ο� DM ���� �� ��ũ�� �����Ѵ�.
// 		//===================================================================
// 		CButton * pChkGiveAutoRegno;
// 		pChkGiveAutoRegno = (CButton *)pDlg->GetDlgItem(IDC_ChkGiveAutoRegno);
// 		if ( pChkGiveAutoRegno->GetCheck() == 1 ) 
// 		{	
// 			INT ids = -1;
// 			// ��ϱ��� ��������
// 			CString sRegCode;
// 			pDlg->GetControlData( _T("CM_����_��ũ����_�����ڵ�"), strHD_Book + _T("_��ϱ���"), sRegCode );
// 
// 			// ������ ��ȣ ��������
// 			CString sQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'REG_NO' AND MANAGE_CODE = UDF_MANAGE_CODE AND DIVISION_VALUE = '") + sRegCode + _T("'");
// 			CString sLastNum;	
// 			DM_Species.GetOneValueQuery( sQuery , sLastNum );
// 			INT iLastNum = _ttoi ( sLastNum );
// 
// 			//��ϱ����� ������ �����߰�
// 			if ( sLastNum == _T("") ) {
// 				iLastNum = 0;
// 
// 				ids = DM_Species.StartFrame();
// 				if ( 0 > ids ) goto ERR;
// 				CString sRecKey;
// 				ids = DM_Species.MakeRecKey( sRecKey );
// 				if ( 0 > ids ) goto ERR;
// 				CString sQuery_reg;
// 				CTime time = CTime::GetCurrentTime();
// 				CString sYear;
// 				sYear.Format( _T("%04d"), time.GetYear() );
// 				sQuery_reg.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ( REC_KEY , PUBLISH_FORM_CODE , KIND_CODE , DIVISION_VALUE , MAKE_YEAR , LAST_NUMBER, MANAGE_CODE) ")
// 								  _T("VALUES ( '%s' , '%s' , '%s' , '%s' , '%s' , '%s', UDF_MANAGE_CODE );") 
// //DEL 				sQuery_reg.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ( REC_KEY , PUBLISH_FORM_CODE , KIND_CODE , DIVISION_VALUE , MAKE_YEAR , LAST_NUMBER) ")
// //DEL 								  _T("VALUES ( '%s' , '%s' , '%s' , '%s' , '%s' , '%s' );") 
// 								  , sRecKey , _T("CO") , _T("REG_NO") , sRegCode , sYear , _T("0"));
// 				ids = DM_Species.AddFrame( sQuery_reg );
// 				if ( 0 > ids ) goto ERR;
// 
// 				ids = DM_Species.SendFrame();			
// 				if (ids < 0) {
// 					AfxMessageBox(_T("�Է»����� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�(DM_REG)."));
// 					goto ERR;
// 				}
// 				ids = DM_Species.EndFrame();
// 				if ( 0 > ids ) goto ERR;
// 			}
// 
// 			CESL_DataMgr *pDmLastNum = pDlg->FindDM( _T("DM_��������ȣ") );
// 			if (pDmLastNum == NULL) {
// 				AfxMessageBox( _T("DM_��������ȣ Ȯ��") );
// 				goto ERR;
// 			}
// 			pDmLastNum->FreeData();
// 			pDmLastNum->InsertRow(-1);
// 			pDmLastNum->SetCellData( strHD_Book + _T("_��ϱ���_�ڵ�"), sRegCode, 0 );
// 			
// 			//����
// 			CString sMarc = DM_Species.GetCellData( strHD_Species + _T("_MARC"), 0 );
// 			pDlg->m_pInfo->pMarcMgr->Decoding(sMarc, &marc);
// 			CArray<CString, CString&> pArr049l;
// 			CString strItemData;
// 			pDlg->m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &pArr049l);
// 
// 
// 			//������ �ڷ��� å���� �ϵ��� ����
// 			//ȫ��
// 			INT iBookCount = DM_Book.GetRowCount();
// 
// 			for ( INT j = 0 ; j < iBookCount ; j++ ) {
// 				iLastNum++;
// 				CString sRegNo;
// 				//=====================================================
// 				//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// // 				sRegNo.Format( _T("%s%010d"), sRegCode, iLastNum );
// // 				sLastNum.Format( _T("%d"), iLastNum );
// 				sLastNum.Format( _T("%d"), iLastNum );
// 				sRegNo = pDlg->m_pInfo->MakeRegNo( sRegCode, sLastNum );
// 				//=====================================================
// 				
// 
// 				
// 				pDmLastNum->SetCellData( _T("��������ȣ"), sLastNum, 0 );
// 				
// 				CString sItem;
// 				if ( pArr049l.GetSize() == iBookCount ) {
// 					sItem = pArr049l.GetAt(j);
// 					ids = DM_Book.SetCellData(strHD_Book + _T("_��Ϲ�ȣ"), sRegNo, j);
// 					//=====================================================
// 					//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 					//ids = DM_Book.SetCellData(strHD_Book + _T("_��ϱ���_�ڵ�"), sRegNo.Left(2), j);
// 					ids = DM_Book.SetCellData(strHD_Book + _T("_��ϱ���_�ڵ�"), sRegNo.Left(nRegCodeLength), j);
// 					//=====================================================
// 					
// 					pDlg->m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo, pArr049l.GetAt(j) );
// 				}
// 				else {
// 					ids = DM_Book.SetCellData(strHD_Book + _T("_��Ϲ�ȣ"), sRegNo, j);
// 					//=====================================================
// 					//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 					//ids = DM_Book.SetCellData(strHD_Book + _T("_��ϱ���_�ڵ�"), sRegNo.Left(2), j);
// 					ids = DM_Book.SetCellData(strHD_Book + _T("_��ϱ���_�ڵ�"), sRegNo.Left(nRegCodeLength), j);
// 					//=====================================================
// 					
// 					pDlg->m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo );
// 				}
// 				pDlg->m_pInfo->pMarcMgr->Encoding( &marc, sMarc );
// 				DM_Species.SetCellData( strHD_Species + _T("_MARC"), sMarc, 0 );
// 			}
// 		}
// 
// 		//================== �����߹���, ������� �������� ==================
// 		
// 		//===================================================================
// 		// 6. åDM�� ���Ա���/�����/��Ϲ�ȣ ����
// 		//===================================================================
// 		CString sAcqCode;		
// 		pDlg->GetControlData( _T("CM_����_��ũ����_�����ڵ�"), strHD_Index + _T("_���Ա���_�ڵ�"), sAcqCode );
// 		INT iBookCount = DM_Book.GetRowCount();
// 
// 		
// 
// 		for ( INT i = 0 ; i < iBookCount ; i++ ) {
// 			//���Ա��� ����
// 			DM_Book.SetCellData( strHD_Book + _T("_���Ա���_�ڵ�"), sAcqCode , i );
// 			
// 			//����� ����
// 			CString result;
// 			CTime t = CTime::GetCurrentTime();
// 			result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
// 			ids = DM_Book.SetCellData( strHD_Book + _T("_�����"), result, i);
// 
// 			//��Ϲ�ȣ 12�ڸ� �ǵ��
// 			//=====================================================
// 			//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// // 			CString strRegNo;
// // 			ids = DM_Book.GetCellData( strHD_Book + _T("_��Ϲ�ȣ"), i, strRegNo);
// // 			if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
// // 				if ( strRegNo.GetLength() > 2 ) {
// // 					CString sRight;
// // 					sRight.Format( _T("%010s") , strRegNo.Mid(2) );
// // 					strRegNo = strRegNo.Left(2) + sRight;
// // 				}
// // 			}
// 			CString strRegNo;
// 			ids = DM_Book.GetCellData( strHD_Book + _T("_��Ϲ�ȣ"), i, strRegNo);
// 			if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
// 				if ( strRegNo.GetLength() > nRegCodeLength ) {
// 					strRegNo = pDlg->m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength), strRegNo.Mid(nRegCodeLength));
// 				}
// 			}
// 			//=====================================================
// 			
// 			ids = DM_Book.SetCellData( strHD_Book + _T("_��Ϲ�ȣ"), strRegNo, i);
// 			// ����Ű ����
// 			if ( pDlg->m_strAccessionRecKey != _T("") )
// 			{
// 				ids = DM_Book.SetCellData( strHD_Book + _T("_����KEY"), pDlg->m_strAccessionRecKey, i);
// 			}
// 						
// 		}
// 
// 		//===================================================================
// 		// 7. MARC�� 049$l�� ��� 12�ڸ��� �����
// 		//===================================================================
// 		CArray<CString, CString&> arr049Item;
// 		CString strItemData;
// 		ids = pDlg->m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &arr049Item);
// 		for ( INT n049Cnt = 0 ; n049Cnt < arr049Item.GetSize(); n049Cnt++ )
// 		{
// 			CString str049Item;
// 			str049Item = arr049Item.GetAt( n049Cnt );
// 			//=====================================================
// 			//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// // 			if ( str049Item != _T("") && str049Item.GetLength() < 12 ) {
// // 				if ( str049Item.GetLength() > 2 ) {
// // 					CString sRight;
// // 					sRight.Format( _T("%010s") , str049Item.Mid(2) );
// // 					str049Item = str049Item.Left(2) + sRight;
// // 				}
// // 			}
// 			
// 			if ( str049Item != _T("") && str049Item.GetLength() < 12 ) {
// 				if ( str049Item.GetLength() > nRegCodeLength ) {
// 					str049Item = pDlg->m_pInfo->MakeRegNo(str049Item.Left(nRegCodeLength), str049Item.Mid(nRegCodeLength));
// 				}
// 			}
// 			//=====================================================
// 
// 			
// 
// 			pDlg->m_pInfo->pMarcMgr->SetItem(&marc, _T("049$l"), strItemData, arr049Item.GetAt( n049Cnt ) );
// 		}
// 		
// 		//===================================================================
// 		// 8. ���� �������¸� 1�� ����
// 		//===================================================================
// 		for ( i = 0 ; i < DM_Species.GetRowCount() ; i++ ) {
// 			DM_Species.SetCellData( strHD_Species + _T("_��������"), _T("1") , i );
// 		}
// 		//		MngCode = DM_Book.GetCellData(_T("BB_��������_�ڵ�"), 0);
// 		
// 		//===================================================================
// 		// 9. ��������̸� �۾����¸� �������� DB�� INSERT�Ѵ�. 
// 		//===================================================================
// 		CButton * pChkWorkStatus;	
// 		pChkWorkStatus = (CButton *)pDlg->GetDlgItem(IDC_ChkWorkStatus);
// 		if ( pChkWorkStatus->GetCheck() == 0 ) 
// 		{
// 			for ( i = 0 ; i < DM_Species.GetRowCount() ; i++ ) 
// 			{ 
// 				DM_Species.SetCellData( strHD_Species + _T("_�۾����±���"), _T("0") , i ); 
// 			}
// 			for ( i = 0 ; i < DM_Index.GetRowCount() ; i++ ) 
// 			{ 
// 				DM_Index.SetCellData( strHD_Index + _T("_�۾�����INFO"), _T("BOL112N") , i ); 
// 			}
// 			
// 			ids = pDlg->ImportSpecies(&DM_Species, &DM_Index, &DM_Book, &DM_Volume);
// 		}
// 		//===================================================================
// 		// 10. ������ �����̸� �۾����¸� ��������/��ǥå �谡�� �ο�/ DB�� INSERT�Ѵ�.
// 		//===================================================================
// 		else 
// 		{
// 			CString strRegNo;
// 			CString strQuery;
// 			CString strResult;			
// 			bError = FALSE;
// 			for ( int i = 0 ; i < DM_Book.GetRowCount() ; i++ ) 
// 			{
// 				// 2008.04.23 ADD BY PDJ
// 				// ������ ������ ��쵵 ��Ϲ�ȣ �ߺ�����
// 				// ��Ϲ�ȣ �ֱ�
// 				DM_Book.GetCellData( _T("BB_��Ϲ�ȣ"), i, strRegNo );
// 				
// 				strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE REG_NO = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), DM_Book.TBL_NAME, strRegNo);
// 				ids = DM_Species.GetOneValueQuery(strQuery, strResult);				
// 				if (ids >= 0 && !strResult.IsEmpty())
// 				{
// 					//��Ϲ�ȣ �ߺ�����
// 					strErrorMsg.Format(_T("��Ϲ�ȣ�� �ߺ��˴ϴ�. [%s]"), strRegNo);
// 					bError = TRUE;
// 					break;					
// 				}
// 				DM_Book.SetCellData( strHD_Book + _T("_�ڷ����"), "BOC113N" , i );
// 				// �ռ� �߱⶧���� ����
// /*				CString strRegNo;
// 				ids = DM_Book.GetCellData( strHD_Book + _T("_��Ϲ�ȣ"), i, strRegNo);
// 				//��Ϲ�ȣ 12�ڸ� �����
// 				if ( strRegNo != "" && strRegNo.GetLength() < 12 ) {
// 					if ( strRegNo.GetLength() > 2 ) {
// 						CString sRight;
// 						sRight.Format( "%010s" , strRegNo.Mid(2) );
// 						strRegNo = strRegNo.Left(2) + sRight;
// 					}
// 				}
// 				ids = DM_Book.SetCellData( strHD_Book + _T("_��Ϲ�ȣ"), strRegNo, i);
// */
// 			}
// 
// 			if ( bError == FALSE )
// 			{			
// 				for ( i = 0 ; i < DM_Species.GetRowCount() ; i++ ) 
// 				{ 
// 					DM_Species.SetCellData( strHD_Species + _T("_�۾����±���"), "1" , i ); 
// 				}
// 				for ( i = 0 ; i < DM_Index.GetRowCount() ; i++ ) 
// 				{ 
// 					DM_Index.SetCellData( strHD_Index + _T("_�۾�����INFO"), "BOC113N" , i ); 
// 				}
// 				
// 				
// 				//��ǥå���� �谡�� �ο�
// 				INT nIdx;
// 				pDlg->GetRepresentationBook(&DM_Book, strHD_Book, nIdx);
// 				
// 				CString sTmpStreamMarc;
// 				CMarc marc;
// 				
// 				DM_Species.GetCellData( strHD_Species + _T("_MARC"), nIdx, sTmpStreamMarc);
// 				pDlg->m_pInfo->pMarcMgr->Decoding(sTmpStreamMarc, &marc );
// 				
// 				ids = ((CCatApi_BO*)pDlg->m_pCatApi)->ApplyShelfCodeData(&DM_Book, &marc, FALSE);
// 				
// 				
// 				CStringArray m_arrStrAddFrame;
// 				
// 				// ���� ����
// 				INT nids = -1;
// 				nids = DM_Species.StartFrame();
// 				if ( 0 > nids ) goto ERR;
// 				
// 				ids = pDlg->InsertSpecies(&DM_Species, &DM_Index, &DM_Book, &DM_Volume, &DM_Volume, &m_arrStrAddFrame, FALSE);
// 				
// 				nids = DM_Species.SendFrame();
// 				if (nids < 0) {
// 					AfxMessageBox(_T("�Է»����� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�(DM_REG)."));
// 					goto ERR;
// 				}
// 				
// 				nids = DM_Species.EndFrame();
// 				if ( 0 > nids ) goto ERR;
// 			}
// 		}		
// 		
// 		if (ids < 0 || bError == TRUE)
// 		{
// 			if ( bError == TRUE )
// 			{
// 				pGrid_TO = pGrid_ERR;
// 				nID_TO = 2;
// 				strTO = _T("���� : ");				
// 			}
// 			else
// 			{
// 				pGrid_TO = pGrid_ERR;
// 				nID_TO = 2;
// 				strTO = _T("���� : ");
// 				strErrorMsg = pDlg->GetLastError();
// 			}
// 		}
// 		//===================================================================
// 		// 11. ������ �Ϸ�Ǹ� �����ǿ� �ű�� ���� �׸��忡 �ݿ��Ѵ�.
// 		//===================================================================
// 		else
// 		{
// 			CString strItemData;
// 			CString strRecKey;
// 
// 			pDlg->m_pDM_Main->InsertRow(-1);
// 			INT nRowIdx = pDlg->m_pDM_Main->GetRowCount() - 1;
// 			
// 			// ���� ���� ī�� 
// 			pDlg->CopyDM(&DM_Index, 0, pDlg->m_pDM_Main, nRowIdx);
// 			
// 			// �� ���� ī��
// 			pDlg->CopyDM(&DM_Species, 0, pDlg->m_pDM_Main, nRowIdx);
// 
// 			// 2005.11.1 ADD BY PDJ
// 			strRecKey = pDlg->m_pDM_Main->GetCellData(_T("BS_��KEY"), nRowIdx );
// 			pDlg->m_pDM_Main->SetCellData(_T("IBS_��KEY"), strRecKey, nRowIdx );			
// 			
// 			// ���� ��Ϲ�ȣ, å�� �־��ֱ�.
// 			INT nIdx = - 1;
// 			ids = pDlg->GetRepresentationBook(&DM_Book, strHD_Book, nIdx);
// 
// 			if (ids < 0 || nIdx < 0)
// 				nIdx = pDlg->SetRepresentationBook(&DM_Book, strHD_Book, nIdx);
// 
// 			pDlg->m_pDM_Main->SetCellData(_T("���۵�Ϲ�ȣ"), DM_Book.GetCellData( strHD_Book + _T("_��Ϲ�ȣ"), nIdx), nRowIdx);
// 			
// 			strItemData.Format(_T("%d (%d)"), pDlg->GetBookCount(&DM_Book), pDlg->GetBookCount(&DM_Book));
// 			pDlg->m_pDM_Main->SetCellData(_T("å��"), strItemData, nRowIdx);
// 
// 			strItemData = _T("");
// 
// 			pDlg->m_pDM_Main->GetCellData( strHD_Species + _T("_��������"), nRowIdx, strItemData);
// 			INT nMainGridRowIdx = pDlg->m_pDM_Main->GetRowCount() -1;
// 
// 			if (strItemData == _T("1") ) {
// 				pDlg->m_pDM_Main->SetCellData( strHD_Species + _T("_��������"), _T("������"), nMainGridRowIdx );
// 				INT ii = pDlg->m_pDM_Main->GetRowCount();
// 			}
// 			else if (strItemData == _T("0") ) {
// 				pDlg->m_pDM_Main->SetCellData( strHD_Species + _T("_��������"), _T("�����ȵ�"), nMainGridRowIdx );
// 			}
// 			
// 			pGrid_TO = pGrid_SUCCESS;
// 			nID_TO = 3;
// 			strTO = _T("���� : ");
// 
// 			// ������Ϲ�ȣ�� �ִ� ��Ϲ�ȣ��� �������ش�.
// 			CString strRegNo ;
// 			CString strGetAccRegNo ;
// 			CString strQuery ;
// 			CString strRegCode ;
// 			INT iRegNumber ;
// 			INT nCount ;
// 			CString strCount =_T("") ;
// 			
// 			nCount = DM_Book.GetRowCount () ;
// 			DM_Book.GetCellData ( strHD_Book + _T("_��Ϲ�ȣ"),idxDM,strRegNo);
// 			if(!strRegNo.IsEmpty ())
// 			{   
// 				//=====================================================
// 				//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// // 				strRegCode = strRegNo.Left(2) ;
// // 				iRegNumber = _ttoi(strRegNo.Mid (2)) ;
// 
// 				strRegCode = strRegNo.Left(nRegCodeLength) ;
// 				iRegNumber = _ttoi(strRegNo.Mid (nRegCodeLength)) ;
// 				//=====================================================
// 				
// 				
//                 strQuery.Format (_T("delete from CO_DROP_ACCESSION_NO_TBL  where reg_code = '%s' and  REG_NO = %d ;"), strRegCode,iRegNumber );
// 				DM_Book.StartFrame () ;
// 				DM_Book.InitDBFieldData () ;
// 				DM_Book.AddFrame (strQuery);
// 				DM_Book.SendFrame ();
// 				DM_Book.EndFrame () ;
// 			}
// 		}
//         
// /*        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 	   // 2003�� 10�� 14�� ������ ���� 
// 	    if(!(ids<0)) 
// 		{ 
// 		    CString strRegNo ;
// 			CString strGetAccRegNo ;
// 			CString strQuery ;
// 			CString strRegCode ;
// 			INT iRegNumber ;
// 			INT nCount ;
// 			CString strCount =_T("") ;
// 			
// 			nCount = DM_Book.GetRowCount () ;
// 			DM_Book.GetCellData ( strHD_Book + _T("_��Ϲ�ȣ"),idxDM,strRegNo);
// 			if(!strRegNo.IsEmpty ())
// 			{   
// 				strRegCode = strRegNo.Left(2) ;
// 				iRegNumber = _ttoi(strRegNo.Mid (2)) ;
// 				
//                 strQuery.Format (_T("delete from CO_DROP_ACCESSION_NO_TBL  where reg_code = '%s' and  REG_NO = %d ;"), strRegCode,iRegNumber );
// 				DM_Book.StartFrame () ;
// 				DM_Book.InitDBFieldData () ;
// 				DM_Book.AddFrame (strQuery);
// 				DM_Book.SendFrame ();
// 				DM_Book.EndFrame () ;
// 			}
// 		
// 		}
//        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// */
// 
// 		//===================================================================
// 		// 12. ���¹��� ������ �����Ѵ�.
// 		//===================================================================
// 		strData = pDlg->m_wndStatusBarCtrl.GetText(1);
// 		strData = strData.Mid(strData.ReverseFind(':') + 1);
// 		strData.Format(_T("%d"), _ttoi(strData) - 1);
// 		pDlg->m_wndStatusBarCtrl.SetText(_T("���� : ") + strData, 1, 0);
// 		
// 	
// 
// 		strData = pDlg->m_wndStatusBarCtrl.GetText(nID_TO);
// 		strData = strData.Mid(strData.ReverseFind(':') + 1);
// 		strData.Format(_T("%d"), _ttoi(strData) + 1);
// 		pDlg->m_wndStatusBarCtrl.SetText(strTO + strData, nID_TO, 0);
// 
// 
// 		//2003.10.15 ������ ����
//         nTotalCnt =  strTO+strData ; 
// 		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 
// 		//===================================================================
// 		// 13. ������ ���� ������ �����͸� �ű��.
// 		//===================================================================
// 		nRowsTo = pGrid_TO->GetRows();
// 		if (pGrid_TO->GetTextMatrix(nRowsTo - 1, 0).IsEmpty())
// 		{
// 			nRowTo = nRowsTo - 1;
// 		}
// 		else
// 		{
// 			pGrid_TO->SetRows(nRowsTo + 1);
// 			nRowTo = nRowsTo;
// 		}
// 
// 		for (INT col = 0; col < nColCount; col++)
// 		{
// 			pGrid_TO->SetTextMatrix(nRowTo, col, pGrid_OK->GetTextMatrix(idx, col));
// 		}
// 
// 		// ���� �� ������ ���� ����
// 		if (!strErrorMsg.IsEmpty())
// 		{
// 			pDlg->m_mapGridAlias.Lookup( strHD_Species + _T("_���Ϲ��Կ���"), col);
// 			pGrid_TO->SetTextMatrix(nRowTo, col, strErrorMsg);
// 		}
// 
// 		//===================================================================
// 		// 14. ����ٸ� �������ش�.
// 		//===================================================================
// 		pDlg->m_wndProgressBar.StepIt();
// 
// 		// Remove Row
// 		if (pGrid_OK->GetRows() == 2)
// 		{
// 			pGrid_OK->Clear();
// 			pGrid_OK->SetColumnInfo();
// 			break;
// 		}
// 		else
// 		{
// 			pGrid_OK->RemoveItem(idx);
// 		}
// 
// 		end_app = GetTickCount();
// 		nProcCount++;
// 		strMsg.Format(_T("ROW %d \t %d.%03d\n"), nProcCount, (end_app-start_app)/1000, (end_app-start_app)%1000);	
// 		_ftprintf( fpTime, strMsg );		
// 	}
// 	//=======================================================================
// 	// ��ü ���Խð�
// 	end_total = GetTickCount();
// 	strMsg.Format(_T("��ü���Խð� \t %d.%03d\n======================================\n"), (end_total-start_total)/1000, (end_total-start_total)%1000);	
// 	_ftprintf( fpTime, strMsg );		
// 	fclose(fpTime);
// 		
// 	pDlg->m_wndProgressBar.SetWindowText(_T("���� �Ϸ�."));
// 
// 	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 	//2003.10.20 ������ ���� 
//     nTotalCnt = pDlg->m_wndStatusBarCtrl.GetText(0) ;
//     nFalseCnt = pDlg->m_wndStatusBarCtrl.GetText(2) ;
// 	nTrueCnt  =  pDlg->m_wndStatusBarCtrl.GetText(3) ;
// 
// 	strTotalMsg.Format( _T("���� �Ϸ�. \r\n\r\n %s�� \r\n %s�� \r\n %s��"),nTotalCnt,nFalseCnt,nTrueCnt ); 
// 	AfxMessageBox(strTotalMsg);
// 	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 
// 	pDlg->m_bIsThreadRunning = FALSE;
// 	pDlg->ButtonStatus(1);
// 	SetEvent(pDlg->m_hThread);
// 
// 	return 0;
// 
// ERR:
// 	pDlg->m_wndProgressBar.SetWindowText(_T("���� ����"));
// 	AfxMessageBox(_T("���� ����"));
// 	pDlg->m_bIsThreadRunning = FALSE;
// 	pDlg->ButtonStatus(1);
// 	return -1;
// 
// 	return -1;
// }
//===================================================
//===================================================
//2009.03.10 UPDATE BY PJW : TEMP DM �̿� ����
//UINT ThreadFunc_Import(LPVOID pParam)
//{
//	//=======================================================================
// 	// 1.Initialization Variable
// 	//=======================================================================
// 	CESL_ControlMgr *pCM = NULL;
// 	CESL_Grid *pGrid_OK = NULL;			// ������ �׸���
// 	CESL_Grid *pGrid_ERR = NULL;		// ������ �׸���
// 	CESL_Grid *pGrid_SUCCESS = NULL;	// ������ �׸���
// 	CESL_Grid *pGrid_TO = NULL;			// �ű� �׸���
// 	// ����������ų ID
// 	UINT nID_TO = -1;
// 	// ����������ų String
// 	CString strTO;
// 	// DataMgr
// 	CString strDMPK;
//
//	INT nStopIdx;
// 	
// 	CMarcImportDlg *pDlg = (CMarcImportDlg*)pParam;
// 	if (pDlg == NULL)
// 	{
// 		pDlg->m_bIsThreadRunning = FALSE;
// 		return -1;
// 	}
// 
// 	pDlg->m_bIsThreadRunning = TRUE;
// 	pDlg->ButtonStatus(0);
// 
// 	// ��DM
// 	CESL_DataMgr DM_Species;
// 	DM_Species.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
// 	DM_Species.InitDMPKFromFile(pDlg->m_lpszSpeciesDMAlias);
// 	DM_Species.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
// 
// 	// ����DM
// 	CESL_DataMgr DM_Index;
// 	DM_Index.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
// 	DM_Index.InitDMPKFromFile(pDlg->m_lpszIndexDMAlias);
// 	DM_Index.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
// 	
// 	// åDM
// 	CESL_DataMgr DM_Book;
// 	DM_Book.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
// 	DM_Book.InitDMPKFromFile(pDlg->m_lpszBookDMAlias);
// 	DM_Book.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
// 
// 	// ��DM
// 	CESL_DataMgr DM_Volume;
// 	DM_Volume.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
// 	DM_Volume.InitDMPKFromFile(pDlg->m_lpszVolumeDMAlias);
// 	DM_Volume.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
// 	
// 	INT ids = -1;
// 	INT idxDM = -1;
// 	INT nCount = -1;
// 
// 	INT nColCount = -1;
// 
// 	INT nRowTo = 0;
// 	INT nRowsTo = 0;
// 	
// 	CString strData;
// 	CString strErrorMsg;
//     CString nTotalCnt;
//     CString strTotalMsg;
//     CString nFalseCnt;
// 	CString nTrueCnt;
// 
// 	DWORD start_app, end_app;
// 	CString strMsg;		
// 	INT		nProcCount = 0;
// 
// 	// HSS_040524 �������
// 	CString strHD_Book, strHD_Species, strHD_Index;
// 	strHD_Species = _T("BS");
// 	strHD_Book = _T("BB");
// 	strHD_Index = _T("IBS");
// 
// 
// 	// �׸��� ����
// 	pGrid_OK = pDlg->m_pGrid_OK;
// 	pGrid_ERR = pDlg->m_pGrid_ERR;	
// 	pGrid_SUCCESS = pDlg->m_pGrid_SUCCESS;
// 	
// 	if (pGrid_OK == NULL || pGrid_ERR == NULL || pGrid_SUCCESS == NULL)
// 		goto ERR;
// 
// 	//=====================================================
// 	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	INT nRegCodeLength;
// 	nRegCodeLength = pDlg->m_pInfo->nREG_CODE_LENGTH;
// 	//=====================================================
// 
// 	//=======================================================================
// 	// 2. ���� ���� �ð� üũ (üũ�� ���ϴ��� �𸣰���.)
// 	//=======================================================================
// 	FILE *fpTime;
// 	fpTime = _tfopen( _T("..\\tunning\\���Լ���ð�.txt"), _T("a+b") );	
// 	
// 		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
// 		// KOLAS 2 UP UNICODE �����۾�
// 		// ���� �����ÿ��� UNICODE ����� �Է��ؾ��Ѵ�.
// 		fseek(fpTime, 0, SEEK_END);		
// 		if( 0 == ftell(fpTime) )
// 		{
// 			/*UNCHANGE*/fputc(0xFF, fpTime);
// 			/*UNCHANGE*/fputc(0xFE, fpTime);
// 		}
// 		// ------------------------------------------------------------------------------
// 		
// 	DWORD start_total, end_total;
// 	start_total = GetTickCount();
// 		
// 	//=======================================================================
// 	// 3. ���õ� ������ŭ ����ٸ� �ʱ�ȭ�Ѵ�.
// 	//=======================================================================
// 	nCount = pGrid_OK->SelectGetCount();
// 	if (nCount <= 0)
// 	{
// 		goto ERR;
// 	}
// 	
// 	pDlg->m_wndProgressBar.SetRange(0, nCount);
// 	pDlg->m_wndProgressBar.SetPos(0);
// 	pDlg->m_wndProgressBar.SetWindowText(_T(""));
// 
// 	nColCount = pGrid_OK->GetCols(0);
// 
// 	// ������ ����
// 	BOOL	bError;	
//
//	//=====================================================
//	//2008.12.10 ADD BY PJW : ó������ �ƴ����� üũ�Ѵ�.
//	BOOL bChkFirstWork;
//	bChkFirstWork = TRUE;
//// 	pGrid_OK->SelectMakeList();
//	//=====================================================
//
// 	while(TRUE)
// 	{
// 		bError = FALSE;
// 		start_app = GetTickCount();
// 		
// 		if (!pDlg->m_bIsThreadRunning) break;
// 		strErrorMsg.Empty();
//		//=====================================================
//		//2008.12.10 UPDATE BY PJW : ���ԿϷ��� �����ڷ� �׸��忡�� �����ϱ� ���� ����
////  		pGrid_OK->SelectMakeList();
////  		INT idx = pGrid_OK->SelectGetHeadPosition() + 1;
//		INT idx;
//		if(bChkFirstWork == TRUE)
//		{
//			idx = pGrid_OK->SelectGetHeadPosition() + 1;
//			bChkFirstWork = FALSE;
//		}
//		else
//		{
//			idx = pGrid_OK->SelectGetNext() + 1;
//		}
//		if (idx <= 0) break;
//		//=====================================================
// 
// 		strData = pGrid_OK->GetTextMatrix(idx, 0);
// 		if (strData.IsEmpty()) continue;
// 
// 		idxDM = _ttoi(strData) - 1 ;
// 		
// 		//===================================================================
// 		// 4. ������ ������� ��ũ���� DM���� �����Ѵ�.
// 		//===================================================================
// 		ids = pDlg->MakeMarcAdjustDM(pDlg->m_pDM_Species, idxDM, pDlg->m_pDM_Index, idxDM, &DM_Species, &DM_Index, &DM_Book, &DM_Volume);
// //		CString MngCode = DM_Book.GetCellData(_T("BB_��������_�ڵ�"), 0);
// 		CMarc marc;
// 		
// 		//===================================================================
// 		// 5. �ڵ��ο��� üũ�Ǿ� ������� �ڵ��ο� DM ���� �� ��ũ�� �����Ѵ�.
// 		//===================================================================
// 		CButton * pChkGiveAutoRegno;
// 		pChkGiveAutoRegno = (CButton *)pDlg->GetDlgItem(IDC_ChkGiveAutoRegno);
// 		if ( pChkGiveAutoRegno->GetCheck() == 1 ) 
// 		{	
// 			INT ids = -1;
// 			// ��ϱ��� ��������
// 			CString sRegCode;
// 			pDlg->GetControlData( _T("CM_����_��ũ����_�����ڵ�"), strHD_Book + _T("_��ϱ���"), sRegCode );
// 
// 			// ������ ��ȣ ��������
// 			CString sQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'REG_NO' AND MANAGE_CODE = UDF_MANAGE_CODE AND DIVISION_VALUE = '") + sRegCode + _T("'");
// 			CString sLastNum;	
// 			DM_Species.GetOneValueQuery( sQuery , sLastNum );
// 			INT iLastNum = _ttoi ( sLastNum );
// 
// 			//��ϱ����� ������ �����߰�
// 			if ( sLastNum == _T("") ) {
// 				iLastNum = 0;
// 
// 				ids = DM_Species.StartFrame();
// 				if ( 0 > ids ) goto ERR;
// 				CString sRecKey;
// 				ids = DM_Species.MakeRecKey( sRecKey );
// 				if ( 0 > ids ) goto ERR;
// 				CString sQuery_reg;
// 				CTime time = CTime::GetCurrentTime();
// 				CString sYear;
// 				sYear.Format( _T("%04d"), time.GetYear() );
// 				sQuery_reg.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ( REC_KEY , PUBLISH_FORM_CODE , KIND_CODE , DIVISION_VALUE , MAKE_YEAR , LAST_NUMBER, MANAGE_CODE) ")
// 								  _T("VALUES ( '%s' , '%s' , '%s' , '%s' , '%s' , '%s', UDF_MANAGE_CODE );") 
// //DEL 				sQuery_reg.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ( REC_KEY , PUBLISH_FORM_CODE , KIND_CODE , DIVISION_VALUE , MAKE_YEAR , LAST_NUMBER) ")
// //DEL 								  _T("VALUES ( '%s' , '%s' , '%s' , '%s' , '%s' , '%s' );") 
// 								  , sRecKey , _T("CO") , _T("REG_NO") , sRegCode , sYear , _T("0"));
// 				ids = DM_Species.AddFrame( sQuery_reg );
// 				if ( 0 > ids ) goto ERR;
// 
// 				ids = DM_Species.SendFrame();			
// 				if (ids < 0) {
// 					AfxMessageBox(_T("�Է»����� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�(DM_REG)."));
// 					goto ERR;
// 				}
// 				ids = DM_Species.EndFrame();
// 				if ( 0 > ids ) goto ERR;
// 			}
// 
// 			CESL_DataMgr *pDmLastNum = pDlg->FindDM( _T("DM_��������ȣ") );
// 			if (pDmLastNum == NULL) {
// 				AfxMessageBox( _T("DM_��������ȣ Ȯ��") );
// 				goto ERR;
// 			}
// 			pDmLastNum->FreeData();
// 			pDmLastNum->InsertRow(-1);
// 			pDmLastNum->SetCellData( strHD_Book + _T("_��ϱ���_�ڵ�"), sRegCode, 0 );
// 			
// 			//����
// 			CString sMarc = DM_Species.GetCellData( strHD_Species + _T("_MARC"), 0 );
// 			pDlg->m_pInfo->pMarcMgr->Decoding(sMarc, &marc);
// 			CArray<CString, CString&> pArr049l;
// 			CString strItemData;
// 			pDlg->m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &pArr049l);
// 
// 
// 			//������ �ڷ��� å���� �ϵ��� ����
// 			//ȫ��
// 			INT iBookCount = DM_Book.GetRowCount();
// 
// 			for ( INT j = 0 ; j < iBookCount ; j++ ) {
// 				iLastNum++;
// 				CString sRegNo;
// 				//=====================================================
// 				//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// // 				sRegNo.Format( _T("%s%010d"), sRegCode, iLastNum );
// // 				sLastNum.Format( _T("%d"), iLastNum );
// 				sLastNum.Format( _T("%d"), iLastNum );
// 				sRegNo = pDlg->m_pInfo->MakeRegNo( sRegCode, sLastNum );
// 				//=====================================================
// 				
// 
// 				
// 				pDmLastNum->SetCellData( _T("��������ȣ"), sLastNum, 0 );
// 				
// 				CString sItem;
// 				if ( pArr049l.GetSize() == iBookCount ) {
// 					sItem = pArr049l.GetAt(j);
// 					ids = DM_Book.SetCellData(strHD_Book + _T("_��Ϲ�ȣ"), sRegNo, j);
// 					//=====================================================
// 					//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 					//ids = DM_Book.SetCellData(strHD_Book + _T("_��ϱ���_�ڵ�"), sRegNo.Left(2), j);
// 					ids = DM_Book.SetCellData(strHD_Book + _T("_��ϱ���_�ڵ�"), sRegNo.Left(nRegCodeLength), j);
// 					//=====================================================
// 					
// 					pDlg->m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo, pArr049l.GetAt(j) );
// 				}
// 				else {
// 					ids = DM_Book.SetCellData(strHD_Book + _T("_��Ϲ�ȣ"), sRegNo, j);
// 					//=====================================================
// 					//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 					//ids = DM_Book.SetCellData(strHD_Book + _T("_��ϱ���_�ڵ�"), sRegNo.Left(2), j);
// 					ids = DM_Book.SetCellData(strHD_Book + _T("_��ϱ���_�ڵ�"), sRegNo.Left(nRegCodeLength), j);
// 					//=====================================================
// 					
// 					pDlg->m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo );
// 				}
// 				pDlg->m_pInfo->pMarcMgr->Encoding( &marc, sMarc );
// 				DM_Species.SetCellData( strHD_Species + _T("_MARC"), sMarc, 0 );
// 			}
// 		}
// 
// 		//================== �����߹���, ������� �������� ==================
// 		
// 		//===================================================================
// 		// 6. åDM�� ���Ա���/�����/��Ϲ�ȣ ����
// 		//===================================================================
// 		CString sAcqCode;		
// 		pDlg->GetControlData( _T("CM_����_��ũ����_�����ڵ�"), strHD_Index + _T("_���Ա���_�ڵ�"), sAcqCode );
// 		INT iBookCount = DM_Book.GetRowCount();
// 
// 		
// 
// 		for ( INT i = 0 ; i < iBookCount ; i++ ) {
// 			//���Ա��� ����
// 			DM_Book.SetCellData( strHD_Book + _T("_���Ա���_�ڵ�"), sAcqCode , i );
// 			
// 			//����� ����
// 			CString result;
// 			CTime t = CTime::GetCurrentTime();
// 			result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
// 			ids = DM_Book.SetCellData( strHD_Book + _T("_�����"), result, i);
// 
// 			//��Ϲ�ȣ 12�ڸ� �ǵ��
// 			//=====================================================
// 			//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// // 			CString strRegNo;
// // 			ids = DM_Book.GetCellData( strHD_Book + _T("_��Ϲ�ȣ"), i, strRegNo);
// // 			if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
// // 				if ( strRegNo.GetLength() > 2 ) {
// // 					CString sRight;
// // 					sRight.Format( _T("%010s") , strRegNo.Mid(2) );
// // 					strRegNo = strRegNo.Left(2) + sRight;
// // 				}
// // 			}
// 			CString strRegNo;
// 			ids = DM_Book.GetCellData( strHD_Book + _T("_��Ϲ�ȣ"), i, strRegNo);
// 			if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
// 				if ( strRegNo.GetLength() > nRegCodeLength ) {
// 					strRegNo = pDlg->m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength), strRegNo.Mid(nRegCodeLength));
// 				}
// 			}
// 			//=====================================================
// 			
// 			ids = DM_Book.SetCellData( strHD_Book + _T("_��Ϲ�ȣ"), strRegNo, i);
// 			// ����Ű ����
// 			if ( pDlg->m_strAccessionRecKey != _T("") )
// 			{
// 				ids = DM_Book.SetCellData( strHD_Book + _T("_����KEY"), pDlg->m_strAccessionRecKey, i);
// 			}
// 						
// 		}
// 
// 		//===================================================================
// 		// 7. MARC�� 049$l�� ��� 12�ڸ��� �����
// 		//===================================================================
// 		CArray<CString, CString&> arr049Item;
// 		CString strItemData;
// 		ids = pDlg->m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &arr049Item);
// 		for ( INT n049Cnt = 0 ; n049Cnt < arr049Item.GetSize(); n049Cnt++ )
// 		{
// 			CString str049Item;
// 			str049Item = arr049Item.GetAt( n049Cnt );
// 			//=====================================================
// 			//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// // 			if ( str049Item != _T("") && str049Item.GetLength() < 12 ) {
// // 				if ( str049Item.GetLength() > 2 ) {
// // 					CString sRight;
// // 					sRight.Format( _T("%010s") , str049Item.Mid(2) );
// // 					str049Item = str049Item.Left(2) + sRight;
// // 				}
// // 			}
// 			
// 			if ( str049Item != _T("") && str049Item.GetLength() < 12 ) {
// 				if ( str049Item.GetLength() > nRegCodeLength ) {
// 					str049Item = pDlg->m_pInfo->MakeRegNo(str049Item.Left(nRegCodeLength), str049Item.Mid(nRegCodeLength));
// 				}
// 			}
// 			//=====================================================
// 
// 			
// 
// 			pDlg->m_pInfo->pMarcMgr->SetItem(&marc, _T("049$l"), strItemData, arr049Item.GetAt( n049Cnt ) );
// 		}
// 		
// 		//===================================================================
// 		// 8. ���� �������¸� 1�� ����
// 		//===================================================================
// 		for ( i = 0 ; i < DM_Species.GetRowCount() ; i++ ) {
// 			DM_Species.SetCellData( strHD_Species + _T("_��������"), _T("1") , i );
// 		}
// 		//		MngCode = DM_Book.GetCellData(_T("BB_��������_�ڵ�"), 0);
// 		
// 		//===================================================================
// 		// 9. ��������̸� �۾����¸� �������� DB�� INSERT�Ѵ�. 
// 		//===================================================================
// 		CButton * pChkWorkStatus;	
// 		pChkWorkStatus = (CButton *)pDlg->GetDlgItem(IDC_ChkWorkStatus);
// 		if ( pChkWorkStatus->GetCheck() == 0 ) 
// 		{
// 			for ( i = 0 ; i < DM_Species.GetRowCount() ; i++ ) 
// 			{ 
// 				DM_Species.SetCellData( strHD_Species + _T("_�۾����±���"), _T("0") , i ); 
// 			}
// 			for ( i = 0 ; i < DM_Index.GetRowCount() ; i++ ) 
// 			{ 
// 				DM_Index.SetCellData( strHD_Index + _T("_�۾�����INFO"), _T("BOL112N") , i ); 
// 			}
// 			
// 			ids = pDlg->ImportSpecies(&DM_Species, &DM_Index, &DM_Book, &DM_Volume);
// 		}
// 		//===================================================================
// 		// 10. ������ �����̸� �۾����¸� ��������/��ǥå �谡�� �ο�/ DB�� INSERT�Ѵ�.
// 		//===================================================================
// 		else 
// 		{
// 			CString strRegNo;
// 			CString strQuery;
// 			CString strResult;			
// 			bError = FALSE;
// 			for ( int i = 0 ; i < DM_Book.GetRowCount() ; i++ ) 
// 			{
// 				// 2008.04.23 ADD BY PDJ
// 				// ������ ������ ��쵵 ��Ϲ�ȣ �ߺ�����
// 				// ��Ϲ�ȣ �ֱ�
// 				DM_Book.GetCellData( _T("BB_��Ϲ�ȣ"), i, strRegNo );
// 				
// 				strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE REG_NO = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), DM_Book.TBL_NAME, strRegNo);
// 				ids = DM_Species.GetOneValueQuery(strQuery, strResult);				
// 				if (ids >= 0 && !strResult.IsEmpty())
// 				{
// 					//��Ϲ�ȣ �ߺ�����
// 					strErrorMsg.Format(_T("��Ϲ�ȣ�� �ߺ��˴ϴ�. [%s]"), strRegNo);
// 					bError = TRUE;
// 					break;					
// 				}
// 				DM_Book.SetCellData( strHD_Book + _T("_�ڷ����"), "BOC113N" , i );
// 				// �ռ� �߱⶧���� ����
// /*				CString strRegNo;
// 				ids = DM_Book.GetCellData( strHD_Book + _T("_��Ϲ�ȣ"), i, strRegNo);
// 				//��Ϲ�ȣ 12�ڸ� �����
// 				if ( strRegNo != "" && strRegNo.GetLength() < 12 ) {
// 					if ( strRegNo.GetLength() > 2 ) {
// 						CString sRight;
// 						sRight.Format( "%010s" , strRegNo.Mid(2) );
// 						strRegNo = strRegNo.Left(2) + sRight;
// 					}
// 				}
// 				ids = DM_Book.SetCellData( strHD_Book + _T("_��Ϲ�ȣ"), strRegNo, i);
// */
// 			}
// 
// 			if ( bError == FALSE )
// 			{			
// 				for ( i = 0 ; i < DM_Species.GetRowCount() ; i++ ) 
// 				{ 
// 					DM_Species.SetCellData( strHD_Species + _T("_�۾����±���"), "1" , i ); 
// 				}
// 				for ( i = 0 ; i < DM_Index.GetRowCount() ; i++ ) 
// 				{ 
// 					DM_Index.SetCellData( strHD_Index + _T("_�۾�����INFO"), "BOC113N" , i ); 
// 				}
// 				
// 				
// 				//��ǥå���� �谡�� �ο�
// 				INT nIdx;
// 				pDlg->GetRepresentationBook(&DM_Book, strHD_Book, nIdx);
// 				
// 				CString sTmpStreamMarc;
// 				CMarc marc;
// 				
// 				DM_Species.GetCellData( strHD_Species + _T("_MARC"), nIdx, sTmpStreamMarc);
// 				pDlg->m_pInfo->pMarcMgr->Decoding(sTmpStreamMarc, &marc );
// 				
// 				ids = ((CCatApi_BO*)pDlg->m_pCatApi)->ApplyShelfCodeData(&DM_Book, &marc, FALSE);
// 				
// 				
// 				CStringArray m_arrStrAddFrame;
// 				
// 				// ���� ����
// 				INT nids = -1;
// 				nids = DM_Species.StartFrame();
// 				if ( 0 > nids ) goto ERR;
// 				
// 				ids = pDlg->InsertSpecies(&DM_Species, &DM_Index, &DM_Book, &DM_Volume, &DM_Volume, &m_arrStrAddFrame, FALSE);
//				//=====================================================
//				//2008.12.29 ADD BY PJW : Ȩü���� IDX_BO_TBL�� �߰��� ������ SENDFRAME�ٷ� ������ ����.
//				CString strSpeciesKey;
//				DM_Species.GetCellData(_T("BS_��KEY"),0,strSpeciesKey);
//			 	pDlg->m_pCatApi->ApplyExtraQuery(&DM_Species, &DM_Index, &DM_Book, strSpeciesKey, &m_arrStrAddFrame);
//				//=====================================================
// 				
// 				nids = DM_Species.SendFrame();
// 				if (nids < 0) {
// 					AfxMessageBox(_T("�Է»����� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�(DM_REG)."));
// 					goto ERR;
// 				}
// 				
// 				nids = DM_Species.EndFrame();
// 				if ( 0 > nids ) goto ERR;
// 			}
// 		}		
// 		
// 		if (ids < 0 || bError == TRUE)
// 		{
// 			if ( bError == TRUE )
// 			{
// 				pGrid_TO = pGrid_ERR;
// 				nID_TO = 2;
// 				strTO = _T("���� : ");				
// 			}
// 			else
// 			{
// 				pGrid_TO = pGrid_ERR;
// 				nID_TO = 2;
// 				strTO = _T("���� : ");
// 				strErrorMsg = pDlg->GetLastError();
// 			}
// 		}
// 		//===================================================================
// 		// 11. ������ �Ϸ�Ǹ� �����ǿ� �ű�� ���� �׸��忡 �ݿ��Ѵ�.
// 		//===================================================================
// 		else
// 		{
// 			CString strItemData;
// 			CString strRecKey;
// 
// 			pDlg->m_pDM_Main->InsertRow(-1);
// 			INT nRowIdx = pDlg->m_pDM_Main->GetRowCount() - 1;
// 			
// 			// ���� ���� ī�� 
// 			pDlg->CopyDM(&DM_Index, 0, pDlg->m_pDM_Main, nRowIdx);
// 			
// 			// �� ���� ī��
// 			pDlg->CopyDM(&DM_Species, 0, pDlg->m_pDM_Main, nRowIdx);
// 
// 			// 2005.11.1 ADD BY PDJ
// 			strRecKey = pDlg->m_pDM_Main->GetCellData(_T("BS_��KEY"), nRowIdx );
// 			pDlg->m_pDM_Main->SetCellData(_T("IBS_��KEY"), strRecKey, nRowIdx );			
// 			
// 			// ���� ��Ϲ�ȣ, å�� �־��ֱ�.
// 			INT nIdx = - 1;
// 			ids = pDlg->GetRepresentationBook(&DM_Book, strHD_Book, nIdx);
// 
// 			if (ids < 0 || nIdx < 0)
// 				nIdx = pDlg->SetRepresentationBook(&DM_Book, strHD_Book, nIdx);
// 
// 			pDlg->m_pDM_Main->SetCellData(_T("���۵�Ϲ�ȣ"), DM_Book.GetCellData( strHD_Book + _T("_��Ϲ�ȣ"), nIdx), nRowIdx);
// 			
// 			strItemData.Format(_T("%d (%d)"), pDlg->GetBookCount(&DM_Book), pDlg->GetBookCount(&DM_Book));
// 			pDlg->m_pDM_Main->SetCellData(_T("å��"), strItemData, nRowIdx);
// 
// 			strItemData = _T("");
// 
// 			pDlg->m_pDM_Main->GetCellData( strHD_Species + _T("_��������"), nRowIdx, strItemData);
// 			INT nMainGridRowIdx = pDlg->m_pDM_Main->GetRowCount() -1;
// 
// 			if (strItemData == _T("1") ) {
// 				pDlg->m_pDM_Main->SetCellData( strHD_Species + _T("_��������"), _T("������"), nMainGridRowIdx );
// 				INT ii = pDlg->m_pDM_Main->GetRowCount();
// 			}
// 			else if (strItemData == _T("0") ) {
// 				pDlg->m_pDM_Main->SetCellData( strHD_Species + _T("_��������"), _T("�����ȵ�"), nMainGridRowIdx );
// 			}
// 			
// 			pGrid_TO = pGrid_SUCCESS;
// 			nID_TO = 3;
// 			strTO = _T("���� : ");
// 
// 			// ������Ϲ�ȣ�� �ִ� ��Ϲ�ȣ��� �������ش�.
// 			CString strRegNo ;
// 			CString strGetAccRegNo ;
// 			CString strQuery ;
// 			CString strRegCode ;
// 			INT iRegNumber ;
// 			INT nCount ;
// 			CString strCount =_T("") ;
// 			
// 			nCount = DM_Book.GetRowCount () ;
// 			DM_Book.GetCellData ( strHD_Book + _T("_��Ϲ�ȣ"),idxDM,strRegNo);
// 			if(!strRegNo.IsEmpty ())
// 			{   
// 				//=====================================================
// 				//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// // 				strRegCode = strRegNo.Left(2) ;
// // 				iRegNumber = _ttoi(strRegNo.Mid (2)) ;
// 
// 				strRegCode = strRegNo.Left(nRegCodeLength) ;
// 				iRegNumber = _ttoi(strRegNo.Mid (nRegCodeLength)) ;
// 				//=====================================================
// 				
// 				
//                 strQuery.Format (_T("delete from CO_DROP_ACCESSION_NO_TBL  where reg_code = '%s' and  REG_NO = %d ;"), strRegCode,iRegNumber );
// 				DM_Book.StartFrame () ;
// 				DM_Book.InitDBFieldData () ;
// 				DM_Book.AddFrame (strQuery);
// 				DM_Book.SendFrame ();
// 				DM_Book.EndFrame () ;
// 			}
// 		}
//         
// /*        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 	   // 2003�� 10�� 14�� ������ ���� 
// 	    if(!(ids<0)) 
// 		{ 
// 		    CString strRegNo ;
// 			CString strGetAccRegNo ;
// 			CString strQuery ;
// 			CString strRegCode ;
// 			INT iRegNumber ;
// 			INT nCount ;
// 			CString strCount =_T("") ;
// 			
// 			nCount = DM_Book.GetRowCount () ;
// 			DM_Book.GetCellData ( strHD_Book + _T("_��Ϲ�ȣ"),idxDM,strRegNo);
// 			if(!strRegNo.IsEmpty ())
// 			{   
// 				strRegCode = strRegNo.Left(2) ;
// 				iRegNumber = _ttoi(strRegNo.Mid (2)) ;
// 				
//                 strQuery.Format (_T("delete from CO_DROP_ACCESSION_NO_TBL  where reg_code = '%s' and  REG_NO = %d ;"), strRegCode,iRegNumber );
// 				DM_Book.StartFrame () ;
// 				DM_Book.InitDBFieldData () ;
// 				DM_Book.AddFrame (strQuery);
// 				DM_Book.SendFrame ();
// 				DM_Book.EndFrame () ;
// 			}
// 		
// 		}
//        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// */
// 
// 		//===================================================================
// 		// 12. ���¹��� ������ �����Ѵ�.
// 		//===================================================================
// 		strData = pDlg->m_wndStatusBarCtrl.GetText(1);
// 		strData = strData.Mid(strData.ReverseFind(':') + 1);
// 		strData.Format(_T("%d"), _ttoi(strData) - 1);
// 		pDlg->m_wndStatusBarCtrl.SetText(_T("���� : ") + strData, 1, 0);
// 		
// 	
// 
// 		strData = pDlg->m_wndStatusBarCtrl.GetText(nID_TO);
// 		strData = strData.Mid(strData.ReverseFind(':') + 1);
// 		strData.Format(_T("%d"), _ttoi(strData) + 1);
// 		pDlg->m_wndStatusBarCtrl.SetText(strTO + strData, nID_TO, 0);
// 
// 
// 		//2003.10.15 ������ ����
//         nTotalCnt =  strTO+strData ; 
// 		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 
// 		//===================================================================
// 		// 13. ������ ���� ������ �����͸� �ű��.
// 		//===================================================================
// 		nRowsTo = pGrid_TO->GetRows();
// 		if (pGrid_TO->GetTextMatrix(nRowsTo - 1, 0).IsEmpty())
// 		{
// 			nRowTo = nRowsTo - 1;
// 		}
// 		else
// 		{
// 			pGrid_TO->SetRows(nRowsTo + 1);
// 			nRowTo = nRowsTo;
// 		}
// 
// 		for (INT col = 0; col < nColCount; col++)
// 		{
// 			pGrid_TO->SetTextMatrix(nRowTo, col, pGrid_OK->GetTextMatrix(idx, col));
// 		}
// 
// 		// ���� �� ������ ���� ����
// 		if (!strErrorMsg.IsEmpty())
// 		{
// 			pDlg->m_mapGridAlias.Lookup( strHD_Species + _T("_���Ϲ��Կ���"), col);
// 			pGrid_TO->SetTextMatrix(nRowTo, col, strErrorMsg);
// 		}
//		//=====================================================
//		//2008.12.10 ADD BY PJW : �����Ͽ��� ���� ROWINDEX�� �����Ѵ�.
//		nStopIdx = idx;
//		//=====================================================
// 
// 		//===================================================================
// 		// 14. ����ٸ� �������ش�.
// 		//===================================================================
// 		pDlg->m_wndProgressBar.StepIt();
// 
//		//=====================================================
//		//2008.12.10 REM BY PJW : ���ԿϷ��� DISPLAY ���� ����
// 		// Remove Row
////  		if (pGrid_OK->GetRows() == 2)
////  		{
////  			pGrid_OK->Clear();
////  			pGrid_OK->SetColumnInfo();
////  			break;
////  		}
////  		else
////  		{
////  			pGrid_OK->RemoveItem(idx);
////  		}
//		//=====================================================
// 
// 		end_app = GetTickCount();
// 		nProcCount++;
// 		strMsg.Format(_T("ROW %d \t %d.%03d\n"), nProcCount, (end_app-start_app)/1000, (end_app-start_app)%1000);	
// 		_ftprintf( fpTime, strMsg );		
// 	}
// 	//=======================================================================
//	//=====================================================
//	//2008.12.10 ADD BY PJW : ���Կ��� ������ �����͸� �����ϰ� �׸��� DISPLAY�Ѵ�.
//	{
//		CESL_DataMgr * pGridTempDM;
//		CString strSelectChk;
//		pGridTempDM = NULL;
//		INT nGridRowCnt;
//		INT nColCnt;
//		INT nSelectChkIdx;
//		INT nDmRowIdx;
//		INT nDmRowCnt;
//		INT i ;
//		INT j ;
//		pGridTempDM = pDlg->FindDM( _T("DM_����_��ũ����_GRID_TEMP") );
//		nGridRowCnt = pGrid_OK->GetRows();
//		nColCnt = pGrid_OK->GetColCount();
//		nDmRowIdx = 0;
//		for( i = 0 ; i < nColCnt ; i++ )
//		{
//			if( pGrid_OK->GetTextMatrix( 0 , i) == _T("����") )
//			{
//				nSelectChkIdx = i;
//				break;
//			}
//		}
//	
//		for( i = 0 ; i < nGridRowCnt-1 ; i++ )
//		{
//			
//			strSelectChk = pGrid_OK->GetTextMatrix( i+1 , nSelectChkIdx);
//			strSelectChk.TrimLeft(); strSelectChk.TrimRight();
//			CString strData;
//			if( i < nStopIdx )
//			{
//				if( strSelectChk == _T("") )
//				{
//					
//					pGridTempDM->InsertRow(-1);				
//					for( j = 0 ; j < nColCnt ; j++ )
//					{
//						strData = pGrid_OK->GetTextMatrix(i+1,j);
//						pGridTempDM->SetCellData(nDmRowIdx,j,strData);
//					}
//					++nDmRowIdx;
//				}
//			}
//			else
//			{
//				pGridTempDM->InsertRow(-1);				
//				for( j = 0 ; j < nColCnt ; j++ )
//				{
//					strData = pGrid_OK->GetTextMatrix(i+1,j);
//					pGridTempDM->SetCellData(nDmRowIdx,j,strData);
//				}
//				++nDmRowIdx;				
//			}
//		}
//
//		nDmRowCnt = pGridTempDM->GetRowCount();
//		pGrid_OK->Clear();
//		pGrid_OK->SetRows(2);
//		pGrid_OK->SetColumnInfo();
//
//		if(nDmRowCnt > 0)
//		{
//			pGrid_OK->SetRows(nDmRowCnt+1);
//			CString strData;
//			for( i = 0 ; i < nDmRowCnt ; i++ )
//			{
//				for( j = 0 ; j < nColCnt ; j++ )
//				{
//					strData = pGridTempDM->GetCellData(i,j);
//					pGrid_OK->SetTextMatrix(i+1,j,strData);
//				}
//			}
//			for( i = 0 ; i < nDmRowCnt ; i++ )
//			{
//				pGridTempDM->FreeData();
//			}
//		}		
//	}
//	//=====================================================
//
//
// 	// ��ü ���Խð�
// 	end_total = GetTickCount();
// 	strMsg.Format(_T("��ü���Խð� \t %d.%03d\n======================================\n"), (end_total-start_total)/1000, (end_total-start_total)%1000);	
// 	_ftprintf( fpTime, strMsg );		
// 	fclose(fpTime);
// 		
// 	pDlg->m_wndProgressBar.SetWindowText(_T("���� �Ϸ�."));
// 
// 	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 	//2003.10.20 ������ ���� 
//     nTotalCnt = pDlg->m_wndStatusBarCtrl.GetText(0) ;
//     nFalseCnt = pDlg->m_wndStatusBarCtrl.GetText(2) ;
// 	nTrueCnt  =  pDlg->m_wndStatusBarCtrl.GetText(3) ;
// 
// 	strTotalMsg.Format( _T("���� �Ϸ�. \r\n\r\n %s�� \r\n %s�� \r\n %s��"),nTotalCnt,nFalseCnt,nTrueCnt ); 
// 	AfxMessageBox(strTotalMsg);
// 	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 
// 	pDlg->m_bIsThreadRunning = FALSE;
// 	pDlg->ButtonStatus(1);
// 	SetEvent(pDlg->m_hThread);
// 
// 	return 0;
// 
// ERR:
// 	pDlg->m_wndProgressBar.SetWindowText(_T("���� ����"));
// 	AfxMessageBox(_T("���� ����"));
// 	pDlg->m_bIsThreadRunning = FALSE;
// 	pDlg->ButtonStatus(1);
// 	return -1;
// 
// 	return -1;
// }
//===================================================
//===================================================
//2009.03.10 UPDATE BY PJW : ���׸��忡 DM ����
//UINT ThreadFunc_Import(LPVOID pParam)
//{
//	//=======================================================================
// 	// 1.Initialization Variable
// 	//=======================================================================
// 	CESL_ControlMgr *pCM = NULL;
// 	CESL_Grid *pGrid_OK = NULL;			// ������ �׸���
//
//	CESL_DataMgr *pDM_OK		= NULL; // ������ DM
//	CESL_DataMgr *pDM_ERR		= NULL; // ������ DM
//	CESL_DataMgr *pDM_SUCCESS	= NULL; // ������ DM
// 	CESL_DataMgr *pDM_TO		= NULL;	// �ű�	  DM
//
// 	// ����������ų ID
// 	UINT nID_TO = -1;
// 	// ����������ų String
// 	CString strTO;
// 	// DataMgr
// 	CString strDMPK;
//
//// 	INT nStopIdx;
// 	
// 	CMarcImportDlg *pDlg = (CMarcImportDlg*)pParam;
// 	if (pDlg == NULL)
// 	{
// 		pDlg->m_bIsThreadRunning = FALSE;
// 		return -1;
// 	}
// 
// 	pDlg->m_bIsThreadRunning = TRUE;
// 	pDlg->ButtonStatus(0);
// 
// 	// ��DM
// 	CESL_DataMgr DM_Species;
// 	DM_Species.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
// 	DM_Species.InitDMPKFromFile(pDlg->m_lpszSpeciesDMAlias);
// 	DM_Species.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
// 
// 	// ����DM
// 	CESL_DataMgr DM_Index;
// 	DM_Index.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
// 	DM_Index.InitDMPKFromFile(pDlg->m_lpszIndexDMAlias);
// 	DM_Index.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
// 	
// 	// åDM
// 	CESL_DataMgr DM_Book;
// 	DM_Book.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
// 	DM_Book.InitDMPKFromFile(pDlg->m_lpszBookDMAlias);
// 	DM_Book.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
// 
// 	// ��DM
// 	CESL_DataMgr DM_Volume;
// 	DM_Volume.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
// 	DM_Volume.InitDMPKFromFile(pDlg->m_lpszVolumeDMAlias);
// 	DM_Volume.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
// 	
// 	INT ids = -1;
// 	INT idxDM = -1;
// 	INT nCount = -1;
// 
// 	INT nColCnt = -1;
// 
//// 	INT nRowTo = 0;
//// 	INT nRowsTo = 0;
// 	
// 	CString strData;
// 	CString strErrorMsg;
//	CString nTotalCnt;
//	CString strTotalMsg;
//	CString nFalseCnt;
// 	CString nTrueCnt;
// 
// 	DWORD start_app, end_app;
// 	CString strMsg;		
// 	INT		nProcCount = 0;
//	INT		nSelectCnt = 0;
// 
// 	// HSS_040524 �������
// 	CString strHD_Book, strHD_Species, strHD_Index;
// 	strHD_Species = _T("BS");
// 	strHD_Book = _T("BB");
// 	strHD_Index = _T("IBS");
// 
// 
// 	// �׸��� ����
// 	pGrid_OK = pDlg->m_pGrid_OK;
//
//	pDM_OK		= pDlg->m_pDmGridOk;
//	pDM_ERR		= pDlg->m_pDmGridError;
//	pDM_SUCCESS	= pDlg->m_pDmGridSuccess;
// 	
// 	if ( pGrid_OK == NULL )
// 		goto ERR;
// 
// 	//=====================================================
// 	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	INT nRegCodeLength;
// 	nRegCodeLength = pDlg->m_pInfo->nREG_CODE_LENGTH;
// 	//=====================================================
// 
// 	//=======================================================================
// 	// 2. ���� ���� �ð� üũ (üũ�� ���ϴ��� �𸣰���.)
// 	//=======================================================================
// 	FILE *fpTime;
// 	fpTime = _tfopen( _T("..\\tunning\\���Լ���ð�.txt"), _T("a+b") );	
// 	
// 	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
// 	// KOLAS 2 UP UNICODE �����۾�
// 	// ���� �����ÿ��� UNICODE ����� �Է��ؾ��Ѵ�.
// 	fseek(fpTime, 0, SEEK_END);		
// 	if( 0 == ftell(fpTime) )
// 	{
// 		/*UNCHANGE*/fputc(0xFF, fpTime);
// 		/*UNCHANGE*/fputc(0xFE, fpTime);
// 	}
// 	// ------------------------------------------------------------------------------
// 		
// 	DWORD start_total, end_total;
// 	start_total = GetTickCount();
// 		
// 	//=======================================================================
// 	// 3. ���õ� ������ŭ ����ٸ� �ʱ�ȭ�Ѵ�.
// 	//=======================================================================
//	pGrid_OK->SelectMakeList();
// 	nCount = pGrid_OK->SelectGetCount();
// 	if (nCount <= 0)
// 	{
// 		goto ERR;
// 	}
// 	
// 	pDlg->m_wndProgressBar.SetRange(0, nCount);
// 	pDlg->m_wndProgressBar.SetPos(0);
// 	pDlg->m_wndProgressBar.SetWindowText(_T(""));
// 
// 	nColCnt = pDM_OK->GetRefColumnCount();
// 
// 	// ������ ����
// 	BOOL	bError;	
//
//	INT idx;
//	idx = pGrid_OK->SelectGetHeadPosition();
//	
//	
// 	while( TRUE )
// 	{
//		if( idx < 0 )
//		{
//			break;
//		}
//
// 		bError = FALSE;
// 		start_app = GetTickCount();
// 		
// 		if ( !pDlg->m_bIsThreadRunning ) 
//		{
//			break;
//		}
// 		strErrorMsg.Empty();
//
//		strData = pDM_OK->GetCellData(_T("BS_���Ϲ��Կ���"), idx); 
//		if( !strData.IsEmpty() )
//		{
//			idx = pGrid_OK->SelectGetNext();
//			pDlg->m_wndProgressBar.StepIt();
//			continue;
//		}
//  		strData = pDM_OK->GetCellData(_T("����"), idx); 
//// 		strData = pGrid_OK->GetTextMatrix(idx+1, 0);
//		idxDM = _ttoi(strData) - 1 ;
// 		
// 		//===================================================================
// 		// 4. ������ ������� ��ũ���� DM���� �����Ѵ�.
// 		//===================================================================
// 		ids = pDlg->MakeMarcAdjustDM(pDlg->m_pDM_Species, idxDM, pDlg->m_pDM_Index, idxDM, &DM_Species, &DM_Index, &DM_Book, &DM_Volume);
// 		CMarc marc;
// 		
// 		//===================================================================
// 		// 5. �ڵ��ο��� üũ�Ǿ� ������� �ڵ��ο� DM ���� �� ��ũ�� �����Ѵ�.
// 		//===================================================================
// 		CButton * pChkGiveAutoRegno;
// 		pChkGiveAutoRegno = (CButton *)pDlg->GetDlgItem(IDC_ChkGiveAutoRegno);
// 		if ( pChkGiveAutoRegno->GetCheck() == 1 ) 
// 		{	
// 			INT ids = -1;
// 			// ��ϱ��� ��������
// 			CString sRegCode;
// 			pDlg->GetControlData( _T("CM_����_��ũ����_�����ڵ�"), strHD_Book + _T("_��ϱ���"), sRegCode );
// 
// 			// ������ ��ȣ ��������
// 			CString sQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'REG_NO' AND MANAGE_CODE = UDF_MANAGE_CODE AND DIVISION_VALUE = '") + sRegCode + _T("'");
// 			CString sLastNum;	
// 			DM_Species.GetOneValueQuery( sQuery , sLastNum );
// 			INT iLastNum = _ttoi ( sLastNum );
// 
// 			//��ϱ����� ������ �����߰�
// 			if ( sLastNum == _T("") ) 
//			{
// 				iLastNum = 0;
// 
// 				ids = DM_Species.StartFrame();
// 				if ( 0 > ids ) goto ERR;
// 				CString sRecKey;
// 				ids = DM_Species.MakeRecKey( sRecKey );
// 				if ( 0 > ids ) goto ERR;
// 				CString sQuery_reg;
// 				CTime time = CTime::GetCurrentTime();
// 				CString sYear;
// 				sYear.Format( _T("%04d"), time.GetYear() );
// 				sQuery_reg.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ( REC_KEY , PUBLISH_FORM_CODE , KIND_CODE , DIVISION_VALUE , MAKE_YEAR , LAST_NUMBER, MANAGE_CODE) ")
// 								  _T("VALUES ( '%s' , '%s' , '%s' , '%s' , '%s' , '%s', UDF_MANAGE_CODE );") 
// 								  , sRecKey , _T("CO") , _T("REG_NO") , sRegCode , sYear , _T("0"));
// 				ids = DM_Species.AddFrame( sQuery_reg );
// 				if ( 0 > ids ) goto ERR;
// 
// 				ids = DM_Species.SendFrame();			
// 				if (ids < 0) {
// 					AfxMessageBox(_T("�Է»����� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�(DM_REG)."));
// 					goto ERR;
// 				}
// 				ids = DM_Species.EndFrame();
// 				if ( 0 > ids ) goto ERR;
// 			}
// 
// 			CESL_DataMgr *pDmLastNum = pDlg->FindDM( _T("DM_��������ȣ") );
// 			if (pDmLastNum == NULL) {
// 				AfxMessageBox( _T("DM_��������ȣ Ȯ��") );
// 				goto ERR;
// 			}
// 			pDmLastNum->FreeData();
// 			pDmLastNum->InsertRow(-1);
// 			pDmLastNum->SetCellData( strHD_Book + _T("_��ϱ���_�ڵ�"), sRegCode, 0 );
// 			
// 			//����
// 			CString sMarc = DM_Species.GetCellData( strHD_Species + _T("_MARC"), 0 );
// 			pDlg->m_pInfo->pMarcMgr->Decoding(sMarc, &marc);
// 			CArray<CString, CString&> pArr049l;
// 			CString strItemData;
// 			pDlg->m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &pArr049l);
// 
// 
// 			//������ �ڷ��� å���� �ϵ��� ����
// 			//ȫ��
// 			INT iBookCount = DM_Book.GetRowCount();
// 
// 			for ( INT j = 0 ; j < iBookCount ; j++ ) 
//			{
// 				iLastNum++;
// 				CString sRegNo;
// 				sLastNum.Format( _T("%d"), iLastNum );
// 				sRegNo = pDlg->m_pInfo->MakeRegNo( sRegCode, sLastNum );
//
//				pDmLastNum->SetCellData( _T("��������ȣ"), sLastNum, 0 );
// 				
// 				CString sItem;
// 				if ( pArr049l.GetSize() == iBookCount ) 
//				{
// 					sItem = pArr049l.GetAt(j);
// 					ids = DM_Book.SetCellData(strHD_Book + _T("_��Ϲ�ȣ"), sRegNo, j);
// 					ids = DM_Book.SetCellData(strHD_Book + _T("_��ϱ���_�ڵ�"), sRegNo.Left(nRegCodeLength), j); 					
// 					
//					pDlg->m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo, pArr049l.GetAt(j) );
// 				}
// 				else 
//				{
// 					ids = DM_Book.SetCellData(strHD_Book + _T("_��Ϲ�ȣ"), sRegNo, j);
// 					ids = DM_Book.SetCellData(strHD_Book + _T("_��ϱ���_�ڵ�"), sRegNo.Left(nRegCodeLength), j);
// 					
// 					pDlg->m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo );
// 				}
// 				pDlg->m_pInfo->pMarcMgr->Encoding( &marc, sMarc );
// 				DM_Species.SetCellData( strHD_Species + _T("_MARC"), sMarc, 0 );
// 			}
// 		}
// 
// 		//================== �����߹���, ������� �������� ==================
// 		
// 		//===================================================================
// 		// 6. åDM�� ���Ա���/�����/��Ϲ�ȣ ����
// 		//===================================================================
// 		CString sAcqCode;		
// 		pDlg->GetControlData( _T("CM_����_��ũ����_�����ڵ�"), strHD_Index + _T("_���Ա���_�ڵ�"), sAcqCode );
// 		INT iBookCount = DM_Book.GetRowCount();
// 
// 		for ( INT i = 0 ; i < iBookCount ; i++ ) 
//		{
// 			//���Ա��� ����
// 			DM_Book.SetCellData( strHD_Book + _T("_���Ա���_�ڵ�"), sAcqCode , i );
// 			
// 			//����� ����
// 			CString result;
// 			CTime t = CTime::GetCurrentTime();
// 			result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
// 			ids = DM_Book.SetCellData( strHD_Book + _T("_�����"), result, i);
// 
// 			//��Ϲ�ȣ 12�ڸ� �ǵ�� 
// 			CString strRegNo;
// 			ids = DM_Book.GetCellData( strHD_Book + _T("_��Ϲ�ȣ"), i, strRegNo);
// 			if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
// 				if ( strRegNo.GetLength() > nRegCodeLength ) {
// 					strRegNo = pDlg->m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength), strRegNo.Mid(nRegCodeLength));
// 				}
// 			}
//
// 			ids = DM_Book.SetCellData( strHD_Book + _T("_��Ϲ�ȣ"), strRegNo, i);
// 			// ����Ű ����
// 			if ( pDlg->m_strAccessionRecKey != _T("") )
// 			{
// 				ids = DM_Book.SetCellData( strHD_Book + _T("_����KEY"), pDlg->m_strAccessionRecKey, i);
// 			}
// 						
// 		}
// 
// 		//===================================================================
// 		// 7. MARC�� 049$l�� ��� 12�ڸ��� �����
// 		//===================================================================
// 		CArray<CString, CString&> arr049Item;
// 		CString strItemData;
// 		ids = pDlg->m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &arr049Item);
// 		for ( INT n049Cnt = 0 ; n049Cnt < arr049Item.GetSize(); n049Cnt++ )
// 		{
// 			CString str049Item;
// 			str049Item = arr049Item.GetAt( n049Cnt );
//
// 			if ( str049Item != _T("") && str049Item.GetLength() < 12 ) 
//			{
// 				if ( str049Item.GetLength() > nRegCodeLength ) {
// 					str049Item = pDlg->m_pInfo->MakeRegNo(str049Item.Left(nRegCodeLength), str049Item.Mid(nRegCodeLength));
// 				}
// 			}
//
// 			pDlg->m_pInfo->pMarcMgr->SetItem(&marc, _T("049$l"), strItemData, arr049Item.GetAt( n049Cnt ) );
// 		}
// 		
// 		//===================================================================
// 		// 8. ���� �������¸� 1�� ����
// 		//===================================================================
//// 		for ( i = 0 ; i < DM_Species.GetRowCount() ; i++ ) 
////		{
//// 			DM_Species.SetCellData( strHD_Species + _T("_��������"), _T("1") , i );
//// 		}
// 		
// 		//===================================================================
// 		// 9. ��������̸� �۾����¸� �������� DB�� INSERT�Ѵ�. 
// 		//===================================================================
// 		CButton * pChkWorkStatus;	
// 		pChkWorkStatus = (CButton *)pDlg->GetDlgItem(IDC_ChkWorkStatus);
// 		if ( pChkWorkStatus->GetCheck() == 0 ) 
// 		{
//			INT nSpeciesRow;
//			INT nIdxRow;
//			nSpeciesRow = DM_Species.GetRowCount();
//			nIdxRow = DM_Index.GetRowCount();
//			if( nSpeciesRow == nIdxRow )
//			{
//				for ( i = 0 ; i < nSpeciesRow ; i++ ) 
// 				{ 
// 					DM_Species.SetCellData( strHD_Species + _T("_�۾����±���"), _T("0") , i ); 
//					DM_Index.SetCellData( strHD_Index + _T("_�۾�����INFO"), _T("BOL112N") , i ); 
//					//===================================================
//					//2009.03.12 UPDATE BY PJW : ��ũ������ û����ȣ�� ���������� �������� �������°� '0'�� ����.
//					//							 0 - �����ȵ�, 1 - ������
//// 					DM_Species.SetCellData( strHD_Species + _T("_��������"), _T("1") , i );
//					DM_Species.SetCellData( strHD_Species + _T("_��������"), _T("0") , i );
//					//===================================================
//					
// 				}
//			}
//			else
//			{
// 				for ( i = 0 ; i < nSpeciesRow ; i++ ) 
// 				{ 
// 					DM_Species.SetCellData( strHD_Species + _T("_�۾����±���"), _T("0") , i ); 
//					//===================================================
//					//2009.03.12 UPDATE BY PJW : ��ũ������ û����ȣ�� ���������� �������� �������°� '0'�� ����.
//					//							 0 - �����ȵ�, 1 - ������
//// 					DM_Species.SetCellData( strHD_Species + _T("_��������"), _T("1") , i );
//					DM_Species.SetCellData( strHD_Species + _T("_��������"), _T("0") , i );
//					//===================================================
//					
// 				}
// 				for ( i = 0 ; i < nIdxRow ; i++ ) 
// 				{ 
// 					DM_Index.SetCellData( strHD_Index + _T("_�۾�����INFO"), _T("BOL112N") , i ); 
// 				}
//			}
// 			
// 			ids = pDlg->ImportSpecies(&DM_Species, &DM_Index, &DM_Book, &DM_Volume);
// 		}
// 		//===================================================================
// 		// 10. ������ �����̸� �۾����¸� ��������/��ǥå �谡�� �ο�/ DB�� INSERT�Ѵ�.
// 		//===================================================================
// 		else 
// 		{
// 			CString strRegNo;
// 			CString strQuery;
// 			CString strResult;			
// 			bError = FALSE;
// 			for ( int i = 0 ; i < DM_Book.GetRowCount() ; i++ ) 
// 			{
// 				// 2008.04.23 ADD BY PDJ
// 				// ������ ������ ��쵵 ��Ϲ�ȣ �ߺ�����
// 				// ��Ϲ�ȣ �ֱ�
// 				DM_Book.GetCellData( _T("BB_��Ϲ�ȣ"), i, strRegNo );
// 				
// 				strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE REG_NO = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), DM_Book.TBL_NAME, strRegNo);
// 				ids = DM_Species.GetOneValueQuery(strQuery, strResult);				
// 				if (ids >= 0 && !strResult.IsEmpty())
// 				{
// 					//��Ϲ�ȣ �ߺ�����
// 					strErrorMsg.Format(_T("��Ϲ�ȣ�� �ߺ��˴ϴ�. [%s]"), strRegNo);
// 					bError = TRUE;
// 					break;					
// 				}
// 				DM_Book.SetCellData( strHD_Book + _T("_�ڷ����"), "BOC113N" , i );
// 			}
// 
// 			if ( bError == FALSE )
// 			{		
//				INT nSpeciesRow;
//				INT nIdxRow;
//				nSpeciesRow = DM_Species.GetRowCount();
//				nIdxRow = DM_Index.GetRowCount();
//				if( nSpeciesRow == nIdxRow )
//				{
//					for ( i = 0 ; i < nSpeciesRow ; i++ ) 
// 					{ 
// 						DM_Species.SetCellData( strHD_Species + _T("_�۾����±���"), "1" , i );
//						DM_Index.SetCellData( strHD_Index + _T("_�۾�����INFO"), "BOC113N" , i ); 
//						//===================================================
//						//2009.03.12 UPDATE BY PJW : ��ũ������ û����ȣ�� ���������� �������� �������°� '0'�� ����.
//						//							 0 - �����ȵ�, 1 - ������
//// 	 					DM_Species.SetCellData( strHD_Species + _T("_��������"), _T("1") , i );
//						DM_Species.SetCellData( strHD_Species + _T("_��������"), _T("0") , i );
//						//===================================================
//						
// 					}
//				}
//				else
//				{
// 					for ( i = 0 ; i < DM_Species.GetRowCount() ; i++ ) 
// 					{ 
// 						DM_Species.SetCellData( strHD_Species + _T("_�۾����±���"), "1" , i ); 
//						//===================================================
//						//2009.03.12 UPDATE BY PJW : ��ũ������ û����ȣ�� ���������� �������� �������°� '0'�� ����.
//						//							 0 - �����ȵ�, 1 - ������
//// 	 					DM_Species.SetCellData( strHD_Species + _T("_��������"), _T("1") , i );
//						DM_Species.SetCellData( strHD_Species + _T("_��������"), _T("0") , i );
//						//===================================================
// 					}
// 					for ( i = 0 ; i < DM_Index.GetRowCount() ; i++ ) 
// 					{ 
// 						DM_Index.SetCellData( strHD_Index + _T("_�۾�����INFO"), "BOC113N" , i ); 
// 					}
//				}
// 				
// 				
// 				//��ǥå���� �谡�� �ο�
// 				INT nIdx;
// 				pDlg->GetRepresentationBook(&DM_Book, strHD_Book, nIdx);
// 				
// 				CString sTmpStreamMarc;
// 				CMarc marc;
// 				
// 				DM_Species.GetCellData( strHD_Species + _T("_MARC"), nIdx, sTmpStreamMarc);
// 				pDlg->m_pInfo->pMarcMgr->Decoding(sTmpStreamMarc, &marc );
// 				
// 				ids = ((CCatApi_BO*)pDlg->m_pCatApi)->ApplyShelfCodeData(&DM_Book, &marc, FALSE);
// 				
// 				
// 				CStringArray m_arrStrAddFrame;
// 				
// 				// ���� ����
// 				INT nids = -1;
// 				nids = DM_Species.StartFrame();
// 				if ( 0 > nids ) 
//				{
//					goto ERR;
//				}
// 				
// 				ids = pDlg->InsertSpecies(&DM_Species, &DM_Index, &DM_Book, &DM_Volume, &DM_Volume, &m_arrStrAddFrame, FALSE);
//				if (ids < 0 )
// 				{
//					strErrorMsg = pDlg->GetLastError(); 
//					if( strErrorMsg.IsEmpty() )
//					{
//						pDM_OK->SetCellData(_T("BS_���Ϲ��Կ���"), strErrorMsg, idx);				
//					}
//					else
//					{				
//						pDM_OK->SetCellData(_T("BS_���Ϲ��Կ���"), _T("��ũ���� ����"), idx);
//					}
//
//					nids = DM_Species.EndFrame();
//
// 					if ( 0 > nids ) 
//					{
//						goto ERR;
//					}
//
//					idx = pGrid_OK->SelectGetNext();
//					pDlg->m_wndProgressBar.StepIt();
//					continue;
// 				}
//				//=====================================================
//				//2008.12.29 ADD BY PJW : Ȩü���� IDX_BO_TBL�� �߰��� ������ SENDFRAME�ٷ� ������ ����.
//				CString strSpeciesKey;
//				DM_Species.GetCellData(_T("BS_��KEY"),0,strSpeciesKey);
//			 	pDlg->m_pCatApi->ApplyExtraQuery(&DM_Species, &DM_Index, &DM_Book, strSpeciesKey, &m_arrStrAddFrame);
//				//=====================================================
// 				
// 				nids = DM_Species.SendFrame();
// 				if (nids < 0) {
// 					AfxMessageBox(_T("�Է»����� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�(DM_REG)."));
// 					goto ERR;
// 				}
// 				
// 				nids = DM_Species.EndFrame();
// 				if ( 0 > nids ) goto ERR;
// 			}
// 		}		
// 		
// 		if (bError == TRUE)
// 		{
//// 			pDM_TO = pDM_ERR;
//// 			nID_TO = 2;
//// 			strTO = _T("���� : ");				
// 			strErrorMsg = pDlg->GetLastError();
//			if (!strErrorMsg.IsEmpty())
// 			{
// 				pDM_OK->SetCellData( _T("BS_���Ϲ��Կ���"), strErrorMsg, idx);
// 			}
//			else
//			{				
//				pDM_OK->SetCellData(_T("BS_���Ϲ��Կ���"), _T("��ũ���� ����"), idx);
//			}
//
// 		}
// 		//===================================================================
// 		// 11. ������ �Ϸ�Ǹ� �����ǿ� �ű�� ���� �׸��忡 �ݿ��Ѵ�.
// 		//===================================================================
// 		else
// 		{
// 			CString strItemData;
// 			CString strRecKey;
// 
// 			pDlg->m_pDM_Main->InsertRow(-1);
// 			INT nRowIdx = pDlg->m_pDM_Main->GetRowCount() - 1;
// 			
// 			// ���� ���� ī�� 
// 			pDlg->CopyDM(&DM_Index, 0, pDlg->m_pDM_Main, nRowIdx);
// 			
// 			// �� ���� ī��
// 			pDlg->CopyDM(&DM_Species, 0, pDlg->m_pDM_Main, nRowIdx);
// 
// 			// 2005.11.1 ADD BY PDJ
// 			strRecKey = pDlg->m_pDM_Main->GetCellData(_T("BS_��KEY"), nRowIdx );
// 			pDlg->m_pDM_Main->SetCellData(_T("IBS_��KEY"), strRecKey, nRowIdx );			
// 			
// 			// ���� ��Ϲ�ȣ, å�� �־��ֱ�.
// 			INT nIdx = - 1;
// 			ids = pDlg->GetRepresentationBook(&DM_Book, strHD_Book, nIdx);
// 
// 			if (ids < 0 || nIdx < 0)
// 				nIdx = pDlg->SetRepresentationBook(&DM_Book, strHD_Book, nIdx);
// 
// 			pDlg->m_pDM_Main->SetCellData(_T("���۵�Ϲ�ȣ"), DM_Book.GetCellData( strHD_Book + _T("_��Ϲ�ȣ"), nIdx), nRowIdx);
// 			
// 			strItemData.Format(_T("%d (%d)"), pDlg->GetBookCount(&DM_Book), pDlg->GetBookCount(&DM_Book));
// 			pDlg->m_pDM_Main->SetCellData(_T("å��"), strItemData, nRowIdx);
// 
// 			strItemData = _T("");
// 
// 			pDlg->m_pDM_Main->GetCellData( strHD_Species + _T("_��������"), nRowIdx, strItemData);
// 			INT nMainGridRowIdx = pDlg->m_pDM_Main->GetRowCount() -1;
// 
// 			if (strItemData == _T("1") ) {
// 				pDlg->m_pDM_Main->SetCellData( strHD_Species + _T("_��������"), _T("������"), nMainGridRowIdx );
// 				INT ii = pDlg->m_pDM_Main->GetRowCount();
// 			}
// 			else if (strItemData == _T("0") ) {
// 				pDlg->m_pDM_Main->SetCellData( strHD_Species + _T("_��������"), _T("�����ȵ�"), nMainGridRowIdx );
// 			}
// 			
//
//// 			pDM_TO = pDM_SUCCESS;
//// 			nID_TO = 3;
//// 			strTO = _T("���� : ");
//
// 			// ������Ϲ�ȣ�� �ִ� ��Ϲ�ȣ��� �������ش�.
// 			CString strRegNo ;
// 			CString strGetAccRegNo ;
// 			CString strQuery ;
// 			CString strRegCode ;
// 			INT iRegNumber ;
// 			INT nCount ;
// 			CString strCount =_T("") ;
// 			
// 			nCount = DM_Book.GetRowCount () ;
// 			DM_Book.GetCellData ( strHD_Book + _T("_��Ϲ�ȣ"),idxDM,strRegNo);
// 			if(!strRegNo.IsEmpty ())
// 			{   
// 				strRegCode = strRegNo.Left(nRegCodeLength) ;
// 				iRegNumber = _ttoi(strRegNo.Mid (nRegCodeLength)) ;
// 			
//				strQuery.Format (_T("DELETE FROM CO_DROP_ACCESSION_NO_TBL  WHERE REG_CODE = '%s' and  REG_NO = %d ;"), strRegCode,iRegNumber );
// 				DM_Book.StartFrame () ;
// 				DM_Book.InitDBFieldData () ;
// 				DM_Book.AddFrame (strQuery);
// 				DM_Book.SendFrame ();
// 				DM_Book.EndFrame () ;
// 			}
// 		}
// 
// 		//===================================================================
// 		// 12. ���¹��� ������ �����Ѵ�.
// 		//===================================================================
//// 		strData = pDlg->m_wndStatusBarCtrl.GetText(1);
//// 		strData = strData.Mid(strData.ReverseFind(':') + 1);
//// 		strData.Format(_T("%d"), _ttoi(strData) - 1);
//// 		pDlg->m_wndStatusBarCtrl.SetText(_T("���� : ") + strData, 1, 0);
//// 		
//// 		strData = pDlg->m_wndStatusBarCtrl.GetText(nID_TO);
//// 		strData = strData.Mid(strData.ReverseFind(':') + 1);
//// 		strData.Format(_T("%d"), _ttoi(strData) + 1);
//// 		pDlg->m_wndStatusBarCtrl.SetText(strTO + strData, nID_TO, 0);
// 
// 
// 		//2003.10.15 ������ ����
////          nTotalCnt =  strTO+strData ; 
// 		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 
// 		//===================================================================
// 		// 13. ������ ���� ������ �����͸� �ű��.
// 		//===================================================================
// 
// 		// ���� �� ������ ���� ����
//// 		if (!strErrorMsg.IsEmpty())
//// 		{
//// 			pDM_OK->SetCellData( _T("BS_���Ϲ��Կ���"), strErrorMsg, idx);
//// 		}
//
// 		//===================================================================
// 		// 14. ����ٸ� �������ش�.
// 		//===================================================================
// 		pDlg->m_wndProgressBar.StepIt();
// 
//
// 		end_app = GetTickCount();
// 		nProcCount++;
// 		strMsg.Format(_T("ROW %d \t %d.%03d\n"), nProcCount, (end_app-start_app)/1000, (end_app-start_app)%1000);	
// 		_ftprintf( fpTime, strMsg );
//		
//		idx = pGrid_OK->SelectGetNext();
// 	}
//
// 	// ��ü ���Խð�
// 	end_total = GetTickCount();
// 	strMsg.Format(_T("��ü���Խð� \t %d.%03d\n======================================\n"), (end_total-start_total)/1000, (end_total-start_total)%1000);	
// 	_ftprintf( fpTime, strMsg );		
// 	fclose(fpTime);
// 		
// 	pDlg->m_wndProgressBar.SetWindowText(_T("���� �Ϸ�."));
//
//	
//	idx = pGrid_OK->SelectGetTailPosition();
//	while( TRUE )
//	{
//		INT nRowsTo;
//		if( idx < 0 )
//		{
//			break;
//		}
//		
//		strData = pDM_OK->GetCellData( _T("BS_���Ϲ��Կ���"), idx);
//		if( strData.IsEmpty() )
//		{
//			nRowsTo = pDM_SUCCESS->GetRowCount();
//			pDM_SUCCESS->InsertRow(-1);
// 			for (INT col = 0; col < nColCnt; col++)
// 			{	
//// 				CString strData;
//				strData = pDM_OK->GetCellData( idx ,col);
// 				pDM_SUCCESS->SetCellData( nRowsTo ,col,  strData);
//
// 			}						
//		}
//		else
//		{
//			nRowsTo = pDM_ERR->GetRowCount();
//			pDM_ERR->InsertRow(-1);
// 			for (INT col = 0; col < nColCnt; col++)
// 			{	
//// 				CString strData;
//				strData = pDM_OK->GetCellData( idx ,col);
// 				pDM_ERR->SetCellData( nRowsTo ,col,  strData);
//
// 			}	
//		}
//		pDM_OK->DeleteRow( idx);
//		idx = pGrid_OK->SelectGetPrev();
//	}
//
//	{
//	INT nRowCnt;
//	CString strCount;
//
//	nRowCnt = pDM_OK->GetRowCount();
//	strCount.Format(_T("���� : %d"), nRowCnt);
//	pDlg->m_wndStatusBarCtrl.SetText(strCount, 1, 0);	
//	
//	nRowCnt = pDM_ERR->GetRowCount();
//
//	strCount.Format(_T("���� : %d"), nRowCnt);
//	pDlg->m_wndStatusBarCtrl.SetText(strCount, 2, 0);
//
//	nRowCnt = pDM_SUCCESS->GetRowCount();
//	strCount.Format(_T("���� : %d"), nRowCnt);
//	pDlg->m_wndStatusBarCtrl.SetText(strCount, 3, 0);	
//	}
// 
// 	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 	//2003.10.20 ������ ���� 
//	nTotalCnt = pDlg->m_wndStatusBarCtrl.GetText(0) ;
//    nFalseCnt = pDlg->m_wndStatusBarCtrl.GetText(2) ;
// 	nTrueCnt  = pDlg->m_wndStatusBarCtrl.GetText(3) ;
// 
// 	strTotalMsg.Format( _T("���� �Ϸ�. \r\n\r\n %s�� \r\n %s�� \r\n %s��"),nTotalCnt,nFalseCnt,nTrueCnt ); 
// 	AfxMessageBox(strTotalMsg);
// 	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 
// 	pDlg->m_bIsThreadRunning = FALSE;
// 	pDlg->ButtonStatus(1);
//
//	::PostMessage(pDlg->GetSafeHwnd(), WM_SORT_GRID, 0, 0);
//
// 	SetEvent(pDlg->m_hThread);
// 
// 	return 0;
// 
// ERR:
// 	pDlg->m_wndProgressBar.SetWindowText(_T("���� ����"));
// 	AfxMessageBox(_T("���� ����"));
// 	pDlg->m_bIsThreadRunning = FALSE;
// 	pDlg->ButtonStatus(1);
// 	return -1;
// 
// 	return -1;
// }
UINT ThreadFunc_Import(LPVOID pParam)
{
	//=======================================================================
 	// 1.Initialization Variable
 	//=======================================================================
 	CESL_ControlMgr *pCM = NULL;
 	CESL_Grid *pGrid_OK = NULL;			// ������ �׸���

	CESL_DataMgr *pDM_OK		= NULL; // ������ DM
	CESL_DataMgr *pDM_ERR		= NULL; // ������ DM
	CESL_DataMgr *pDM_SUCCESS	= NULL; // ������ DM
 	CESL_DataMgr *pDM_TO		= NULL;	// �ű�	  DM

 	// ����������ų ID
 	UINT nID_TO = -1;
 	// ����������ų String
 	CString strTO;
 	// DataMgr
 	CString strDMPK;

// 	INT nStopIdx;
 	
	// KOL.UDF.022
 	CMarcImportDlg *pDlg = (CMarcImportDlg*)pParam;

 	if (&pDlg == NULL)
 	{
 		pDlg->m_bIsThreadRunning = FALSE;
 		return -1;
 	}
 
 	pDlg->m_bIsThreadRunning = TRUE;
 	pDlg->ButtonStatus(0);
 
 	// ��DM
 	CESL_DataMgr DM_Species;
 	DM_Species.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
 	DM_Species.InitDMPKFromFile(pDlg->m_lpszSpeciesDMAlias);
 	DM_Species.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
 
 	// ����DM
 	CESL_DataMgr DM_Index;
 	DM_Index.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
 	DM_Index.InitDMPKFromFile(pDlg->m_lpszIndexDMAlias);
 	DM_Index.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
 	
 	// åDM
 	CESL_DataMgr DM_Book;
 	DM_Book.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
 	DM_Book.InitDMPKFromFile(pDlg->m_lpszBookDMAlias);
 	DM_Book.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
 
 	// ��DM
 	CESL_DataMgr DM_Volume;
 	DM_Volume.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
 	DM_Volume.InitDMPKFromFile(pDlg->m_lpszVolumeDMAlias);
 	DM_Volume.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);

	// KOL.RED.015 JMC 20160928 ��ũ���� ���Խ� ��������ȣ �ڵ� ������Ʈ
	// ���Ե� �ڷ��߿� ���� ū ��Ϲ�ȣ ����� DM
	CESL_DataMgr DM_MaxRegBo;
	DM_MaxRegBo.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
	DM_MaxRegBo.InitDMPKFromFile(pDlg->m_lpszMaxRegNoDMAlias);
	DM_MaxRegBo.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
	DM_MaxRegBo.FreeData();	
	// ���� ū ���Լ���ȣ �����DM
	CESL_DataMgr DM_MaxAcqNo;
	DM_MaxAcqNo.SetCONNECTION_INFO(pDlg->CONNECTION_INFO);
	DM_MaxAcqNo.InitDMPKFromFile(pDlg->m_lpszMaxAcqNoDMAlias);
	DM_MaxAcqNo.SetMarcMgr(pDlg->m_pInfo->pMarcMgr);
	DM_MaxAcqNo.FreeData();		
 	
 	INT ids = -1;
 	INT idxDM = -1;
	INT nStopCnt = 0;
//  	INT nCount = -1;
 
	INT nSelectRowCnt;
 	INT nColCnt = -1;
 
// 	INT nRowTo = 0;
// 	INT nRowsTo = 0;
 	
 	CString strData;
 	CString strErrorMsg;
	CString nTotalCnt;
	CString strTotalMsg;
	CString nFalseCnt;
 	CString nTrueCnt;
 
 	DWORD start_app, end_app;
 	CString strMsg;		
 	INT		nProcCount = 0;
	INT		nSelectCnt = 0;
 
 	// HSS_040524 �������
 	CString strHD_Book, strHD_Species, strHD_Index;
 	strHD_Species = _T("BS");
 	strHD_Book = _T("BB");
 	strHD_Index = _T("IBS");
 
 
 	// �׸��� ����
 	pGrid_OK = pDlg->m_pGrid_OK;

	pDM_OK		= pDlg->m_pDmGridOk;
	pDM_ERR		= pDlg->m_pDmGridError;
	pDM_SUCCESS	= pDlg->m_pDmGridSuccess;
 	
 	if ( pGrid_OK == NULL )
 		goto ERR;
 
 	//=====================================================
 	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
 	INT nRegCodeLength;
 	nRegCodeLength = pDlg->m_pInfo->nREG_CODE_LENGTH;
 	//=====================================================
 
 	//=======================================================================
 	// 2. ���� ���� �ð� üũ (üũ�� ���ϴ��� �𸣰���.)
 	//=======================================================================
 	FILE *fpTime;
 	fpTime = _tfopen( _T("..\\tunning\\���Լ���ð�.txt"), _T("a+b") );	
 	
 	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
 	// KOLAS 2 UP UNICODE �����۾�
 	// ���� �����ÿ��� UNICODE ����� �Է��ؾ��Ѵ�.
 	fseek(fpTime, 0, SEEK_END);		
 	if( 0 == ftell(fpTime) )
 	{
 		/*UNCHANGE*/fputc(0xFF, fpTime);
 		/*UNCHANGE*/fputc(0xFE, fpTime);
 	}
 	// ------------------------------------------------------------------------------
 		
 	DWORD start_total, end_total;
 	start_total = GetTickCount();
 		
 	//=======================================================================
 	// 3. ���õ� ������ŭ ����ٸ� �ʱ�ȭ�Ѵ�.
 	//=======================================================================
	pGrid_OK->SelectMakeList();
 	nSelectRowCnt = pGrid_OK->SelectGetCount();
 	
 	pDlg->m_wndProgressBar.SetRange(0, nSelectRowCnt);
 	pDlg->m_wndProgressBar.SetPos(0);
 	pDlg->m_wndProgressBar.SetWindowText(_T(""));
 
 	nColCnt = pDM_OK->GetRefColumnCount();
 
 	// ������ ����
 	BOOL	bError;	

	INT nSelectIdx;
	INT nChkCnt;
	
	
	nSelectIdx = pGrid_OK->SelectGetHeadPosition();

	for( nChkCnt = 0 ; nChkCnt < nSelectRowCnt ; ++nChkCnt )
	{
		bError = FALSE;
 		start_app = GetTickCount();
		if ( !pDlg->m_bIsThreadRunning ) 
		{
			nStopCnt = nChkCnt;
			break;
		}

		strErrorMsg.Empty();

		strData = pDM_OK->GetCellData(_T("BS_���Ϲ��Կ���"), nSelectIdx); 
		if( !strData.IsEmpty() )
		{
			nSelectIdx = pGrid_OK->SelectGetNext();
			pDlg->m_wndProgressBar.StepIt();
			continue;
		}

		strData = pDM_OK->GetCellData(_T("����"), nSelectIdx); 
		idxDM = _ttoi(strData) - 1 ;

		//===================================================================
 		// 4. ������ ������� ��ũ���� DM���� �����Ѵ�.
 		//===================================================================
 		ids = pDlg->MakeMarcAdjustDM(pDlg->m_pDM_Species, idxDM, pDlg->m_pDM_Index, idxDM, &DM_Species, &DM_Index, &DM_Book, &DM_Volume);
 		CMarc marc;
	
		//===================================================================
 		// 5. �ڵ��ο��� üũ�Ǿ� ������� �ڵ��ο� DM ���� �� ��ũ�� �����Ѵ�.
 		//===================================================================
 		CButton * pChkGiveAutoRegno;
 		pChkGiveAutoRegno = (CButton *)pDlg->GetDlgItem(IDC_ChkGiveAutoRegno);
 		if ( pChkGiveAutoRegno->GetCheck() == 1 ) 
 		{	
 			INT ids = -1;
 			// ��ϱ��� ��������
 			CString sRegCode;
 			pDlg->GetControlData( _T("CM_����_��ũ����_�����ڵ�"), strHD_Book + _T("_��ϱ���"), sRegCode );
 
 			// ������ ��ȣ ��������
 			CString sQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'REG_NO' AND MANAGE_CODE = UDF_MANAGE_CODE AND DIVISION_VALUE = '") + sRegCode + _T("'");
 			CString sLastNum;	
 			DM_Species.GetOneValueQuery( sQuery , sLastNum );
 			INT iLastNum = _ttoi ( sLastNum );
 
 			//��ϱ����� ������ �����߰�
 			if ( sLastNum == _T("") ) 
			{
 				iLastNum = 0;
 
 				ids = DM_Species.StartFrame();
 				if ( 0 > ids ) goto ERR;
 				CString sRecKey;
 				ids = DM_Species.MakeRecKey( sRecKey );
 				if ( 0 > ids ) goto ERR;
 				CString sQuery_reg;
 				CTime time = CTime::GetCurrentTime();
 				CString sYear;
 				sYear.Format( _T("%04d"), time.GetYear() );
 				sQuery_reg.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ( REC_KEY , PUBLISH_FORM_CODE , KIND_CODE , DIVISION_VALUE , MAKE_YEAR , LAST_NUMBER, MANAGE_CODE) ")
 								  _T("VALUES ( '%s' , '%s' , '%s' , '%s' , '%s' , '%s', UDF_MANAGE_CODE );") 
 								  , sRecKey , _T("CO") , _T("REG_NO") , sRegCode , sYear , _T("0"));
 				ids = DM_Species.AddFrame( sQuery_reg );
 				if ( 0 > ids ) goto ERR;
 
 				ids = DM_Species.SendFrame();			
 				if (ids < 0) {
 					AfxMessageBox(_T("�Է»����� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�(DM_REG)."));
 					goto ERR;
 				}
 				ids = DM_Species.EndFrame();
 				if ( 0 > ids ) goto ERR;
 			}
 
 			CESL_DataMgr *pDmLastNum = pDlg->FindDM( _T("DM_��������ȣ") );
 			if (pDmLastNum == NULL) 
			{
 				AfxMessageBox( _T("DM_��������ȣ Ȯ��") );
 				goto ERR;
 			}
 			pDmLastNum->FreeData();
 			pDmLastNum->InsertRow(-1);
 			pDmLastNum->SetCellData( strHD_Book + _T("_��ϱ���_�ڵ�"), sRegCode, 0 );
 			
 			//����
 			CString sMarc = DM_Species.GetCellData( strHD_Species + _T("_MARC"), 0 );
 			pDlg->m_pInfo->pMarcMgr->Decoding(sMarc, &marc);
 			CArray<CString, CString&> pArr049l;
 			CString strItemData;
 			pDlg->m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &pArr049l);
 
 
 			//������ �ڷ��� å���� �ϵ��� ����
 			//ȫ��
 			INT iBookCount = DM_Book.GetRowCount();
 
 			for ( INT j = 0 ; j < iBookCount ; j++ ) 
			{
 				iLastNum++;
 				CString sRegNo;
 				sLastNum.Format( _T("%d"), iLastNum );
 				sRegNo = pDlg->m_pInfo->MakeRegNo( sRegCode, sLastNum );

				pDmLastNum->SetCellData( _T("��������ȣ"), sLastNum, 0 );
 				
 				CString sItem;
 				if ( pArr049l.GetSize() == iBookCount ) 
				{
 					sItem = pArr049l.GetAt(j);
 					ids = DM_Book.SetCellData(strHD_Book + _T("_��Ϲ�ȣ"), sRegNo, j);
 					ids = DM_Book.SetCellData(strHD_Book + _T("_��ϱ���_�ڵ�"), sRegNo.Left(nRegCodeLength), j); 					
 					
					pDlg->m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo, pArr049l.GetAt(j) );
 				}
 				else 
				{
 					ids = DM_Book.SetCellData(strHD_Book + _T("_��Ϲ�ȣ"), sRegNo, j);
 					ids = DM_Book.SetCellData(strHD_Book + _T("_��ϱ���_�ڵ�"), sRegNo.Left(nRegCodeLength), j);
 					
 					pDlg->m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo );
 				}
 				pDlg->m_pInfo->pMarcMgr->Encoding( &marc, sMarc );
 				DM_Species.SetCellData( strHD_Species + _T("_MARC"), sMarc, 0 );
 			}
 		}
 
 		//================== �����߹���, ������� �������� ==================
 		
 		//===================================================================
 		// 6. åDM�� ���Ա���/�����/��Ϲ�ȣ ����
 		//===================================================================
 		CString sAcqCode;		
 		pDlg->GetControlData( _T("CM_����_��ũ����_�����ڵ�"), strHD_Index + _T("_���Ա���_�ڵ�"), sAcqCode );
 		INT iBookCount = DM_Book.GetRowCount();
 
 		for ( INT i = 0 ; i < iBookCount ; i++ ) 
		{
 			//���Ա��� ����
 			DM_Book.SetCellData( strHD_Book + _T("_���Ա���_�ڵ�"), sAcqCode , i );
 			
 			//����� ����
 			CString result;
 			CTime t = CTime::GetCurrentTime();
 			result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
 			ids = DM_Book.SetCellData( strHD_Book + _T("_�����"), result, i);
 
 			//��Ϲ�ȣ 12�ڸ� �ǵ�� 
 			CString strRegNo;
 			ids = DM_Book.GetCellData( strHD_Book + _T("_��Ϲ�ȣ"), i, strRegNo);
 			if ( strRegNo.GetLength() > nRegCodeLength && strRegNo.GetLength() < 12 ) 
			{
 				strRegNo = pDlg->m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength), strRegNo.Mid(nRegCodeLength));
 			}

 			ids = DM_Book.SetCellData( strHD_Book + _T("_��Ϲ�ȣ"), strRegNo, i);
 			// ����Ű ����
 			if ( pDlg->m_strAccessionRecKey != _T("") )
 			{
 				ids = DM_Book.SetCellData( strHD_Book + _T("_����KEY"), pDlg->m_strAccessionRecKey, i);
 			}
 						
 		}
 
 		//===================================================================
 		// 7. MARC�� 049$l�� ��� 12�ڸ��� �����
 		//===================================================================
 		CArray<CString, CString&> arr049Item;
 		CString strItemData;
 		ids = pDlg->m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &arr049Item);
 		for ( INT n049Cnt = 0 ; n049Cnt < arr049Item.GetSize(); n049Cnt++ )
 		{
 			CString str049Item;
 			str049Item = arr049Item.GetAt( n049Cnt );

 			if ( str049Item.GetLength() > nRegCodeLength && str049Item.GetLength() < 12 ) 
			{
 				str049Item = pDlg->m_pInfo->MakeRegNo(str049Item.Left(nRegCodeLength), str049Item.Mid(nRegCodeLength));
 			}

 			pDlg->m_pInfo->pMarcMgr->SetItem(&marc, _T("049$l"), strItemData, arr049Item.GetAt( n049Cnt ) );
 		}
 		
 		//===================================================================
 		// 8. ���� �������¸� 1�� ����
 		//===================================================================
		//===================================================
		//2009.03.17 UPDATE BY PJW : 1. �۾����� ���� ���¸� �������ش�.
//									 2. ��ũ������ û����ȣ�� ���������� �������� �������°� '0'�� ����.
//										0 - �����ȵ�, 1 - ������
		//2009.03.25 UPDATE BY PJW : 1. ���������� ������ �������¸� 1�� �����Ѵ�. -> �ڹΰ澾 �䱸����
// 		for ( i = 0 ; i < DM_Species.GetRowCount() ; i++ ) 
//		{
// 			DM_Species.SetCellData( strHD_Species + _T("_��������"), _T("1") , i );
// 		}
		DM_Species.SetCellData( strHD_Species + _T("_��������"), _T("1") , 0 );
		//===================================================
 		
 		//===================================================================
 		// 9. ��������̸� �۾����¸� �������� DB�� INSERT�Ѵ�. 
 		//===================================================================
 		CButton * pChkWorkStatus;	
 		pChkWorkStatus = (CButton *)pDlg->GetDlgItem(IDC_ChkWorkStatus);
 		if ( pChkWorkStatus->GetCheck() == 0 ) 
 		{
			DM_Species.SetCellData( strHD_Species + _T("_�۾����±���"), "0" , 0 );
			DM_Index.SetCellData( strHD_Index + _T("_�۾�����INFO"), "BOL112N" , 0 ); 
			
 			ids = pDlg->ImportSpecies(&DM_Species, &DM_Index, &DM_Book, &DM_Volume);
 		}
 		//===================================================================
 		// 10. ������ �����̸� �۾����¸� ��������/��ǥå �谡�� �ο�/ DB�� INSERT�Ѵ�.
 		//===================================================================
 		else 
 		{
 			CString strRegNo;
 			CString strQuery;
 			CString strResult;			
 			bError = FALSE;
			strResult = _T("");
 			for ( INT i = 0 ; i < DM_Book.GetRowCount() ; i++ ) 
 			{
 				// 2008.04.23 ADD BY PDJ
 				// ������ ������ ��쵵 ��Ϲ�ȣ �ߺ�����
 				// ��Ϲ�ȣ �ֱ�
 				DM_Book.GetCellData( _T("BB_��Ϲ�ȣ"), i, strRegNo );
 				
 				strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE REG_NO = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), DM_Book.TBL_NAME, strRegNo);
 				ids = DM_Species.GetOneValueQuery(strQuery, strResult);	

//				if (ids >= 0 && !strResult.IsEmpty())
// 				{
// 					//��Ϲ�ȣ �ߺ�����
// 					strErrorMsg.Format(_T("��Ϲ�ȣ�� �ߺ��˴ϴ�. [%s]"), strRegNo);
// 					bError = TRUE;
// 					break;					
// 				}
				if ( !strResult.IsEmpty() )
 				{
 					//��Ϲ�ȣ �ߺ�����
 					strErrorMsg.Format(_T("��Ϲ�ȣ�� �ߺ��˴ϴ�. [%s]"), strRegNo);
 					bError = TRUE;
 					break;					
 				}

 				DM_Book.SetCellData( strHD_Book + _T("_�ڷ����"), "BOC113N" , i );
 			}
 
 			if ( bError == FALSE )
 			{		

				DM_Species.SetCellData( strHD_Species + _T("_�۾����±���"), "1" , 0 );
				DM_Index.SetCellData( strHD_Index + _T("_�۾�����INFO"), "BOC113N" , 0 );
 				
 				
 				//��ǥå���� �谡�� �ο�
 				INT nIdx;
 				pDlg->GetRepresentationBook(&DM_Book, strHD_Book, nIdx);
 				
 				CString sTmpStreamMarc;
 				CMarc marc;
 				
 				DM_Species.GetCellData( strHD_Species + _T("_MARC"), nIdx, sTmpStreamMarc);
 				pDlg->m_pInfo->pMarcMgr->Decoding(sTmpStreamMarc, &marc );
 				
 				ids = ((CCatApi_BO*)pDlg->m_pCatApi)->ApplyShelfCodeData(&DM_Book, &marc, FALSE);
 				
 				
 				CStringArray m_arrStrAddFrame;
 				
 				// ���� ����
 				INT nids = -1;
 				nids = DM_Species.StartFrame();
 				if ( 0 > nids ) 
				{
					goto ERR;
				}
 				
 				ids = pDlg->InsertSpecies(&DM_Species, &DM_Index, &DM_Book, &DM_Volume, &DM_Volume, &m_arrStrAddFrame, FALSE);

				//=====================================================
				//2008.12.29 ADD BY PJW : Ȩü���� IDX_BO_TBL�� �߰��� ������ SENDFRAME�ٷ� ������ ����.
				CString strSpeciesKey;
				DM_Species.GetCellData(_T("BS_��KEY"),0,strSpeciesKey);
			 	pDlg->m_pCatApi->ApplyExtraQuery(&DM_Species, &DM_Index, &DM_Book, strSpeciesKey, &m_arrStrAddFrame);
				//=====================================================
 				
 				nids = DM_Species.SendFrame();
 				if (nids < 0) {
 					AfxMessageBox(_T("�Է»����� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�(DM_REG)."));
 					goto ERR;
 				}
 				
 				nids = DM_Species.EndFrame();
 				if ( 0 > nids ) goto ERR;
 			}
 		}		
 		
 		if ( bError == TRUE  || 0 > ids )
 		{
 			strErrorMsg = pDlg->GetLastError();
			if (!strErrorMsg.IsEmpty())
 			{
 				pDM_OK->SetCellData( _T("BS_���Ϲ��Կ���"), strErrorMsg, nSelectIdx);
 			}
			else
			{				
				pDM_OK->SetCellData(_T("BS_���Ϲ��Կ���"), _T("��ũ���� ����"), nSelectIdx);
			}

 		}
 		//===================================================================
 		// 11. ������ �Ϸ�Ǹ� �����ǿ� �ű�� ���� �׸��忡 �ݿ��Ѵ�.
 		//===================================================================
 		else
 		{
 			CString strItemData;
 			CString strRecKey;
 
 			pDlg->m_pDM_Main->InsertRow(-1);
 			INT nRowIdx = pDlg->m_pDM_Main->GetRowCount() - 1;
 			
 			// ���� ���� ī�� 
 			pDlg->CopyDM(&DM_Index, 0, pDlg->m_pDM_Main, nRowIdx);
 			
 			// �� ���� ī��
 			pDlg->CopyDM(&DM_Species, 0, pDlg->m_pDM_Main, nRowIdx);
 
 			// 2005.11.1 ADD BY PDJ
 			strRecKey = pDlg->m_pDM_Main->GetCellData(_T("BS_��KEY"), nRowIdx );
 			pDlg->m_pDM_Main->SetCellData(_T("IBS_��KEY"), strRecKey, nRowIdx );			
 			
 			// ���� ��Ϲ�ȣ, å�� �־��ֱ�.
 			INT nIdx = - 1;
 			ids = pDlg->GetRepresentationBook(&DM_Book, strHD_Book, nIdx);
 
 			if (ids < 0 || nIdx < 0)
			{
 				nIdx = pDlg->SetRepresentationBook(&DM_Book, strHD_Book, nIdx);
			}
 
 			pDlg->m_pDM_Main->SetCellData(_T("���۵�Ϲ�ȣ"), DM_Book.GetCellData( strHD_Book + _T("_��Ϲ�ȣ"), nIdx), nRowIdx);
 			
 			strItemData.Format(_T("%d (%d)"), pDlg->GetBookCount(&DM_Book), pDlg->GetBookCount(&DM_Book));
 			pDlg->m_pDM_Main->SetCellData(_T("å��"), strItemData, nRowIdx);
 
 			strItemData = _T("");
 
 			
 
			//===================================================
			//2009.03.17 UPDATE BY PJW : û����ȣ ������ ��ũ�����ϸ� ������ �����ȵ����� �ִ´�.
			//2009.03.25 UPDATE BY PJW : 1. ���������� ������ �������¸� ���������� �����Ѵ�. -> �ڹΰ澾 �䱸����
//			pDlg->m_pDM_Main->GetCellData( strHD_Species + _T("_��������"), nRowIdx, strItemData);
// 			INT nMainGridRowIdx = pDlg->m_pDM_Main->GetRowCount() -1;
//			if (strItemData == _T("1") ) {
// 				pDlg->m_pDM_Main->SetCellData( strHD_Species + _T("_��������"), _T("������"), nMainGridRowIdx );
// 				INT ii = pDlg->m_pDM_Main->GetRowCount();
// 			}
// 			else if (strItemData == _T("0") ) {
// 				pDlg->m_pDM_Main->SetCellData( strHD_Species + _T("_��������"), _T("�����ȵ�"), nMainGridRowIdx );
// 			}
			INT nMainGridRowIdx = pDlg->m_pDM_Main->GetRowCount() -1;
			pDlg->m_pDM_Main->SetCellData( strHD_Species + _T("_��������"), _T("������"), nMainGridRowIdx );
			//===================================================
			//===================================================
			//2009.10.13 ADD BY PJW : û����ȣ�� �׸��忡 �ݿ��Ѵ�.
			CString strCallNoQuery;
			CString strTempData;
			
			pDlg->m_pDM_Main->GetCellData(_T("IBS_��KEY"), nMainGridRowIdx, strTempData);
			strCallNoQuery.Format( _T("SELECT ECO$F_MAKE_CALLNO$I( SE_SHELF_CODE,  CLASS_NO,  BOOK_CODE,  VOL_CODE) FROM IDX_BO_TBL WHERE REC_KEY = %s"), strTempData );
			pDlg->m_pDM_Main->GetOneValueQuery( strCallNoQuery, strTempData);
			pDlg->m_pDM_Main->SetCellData(_T("��û����ȣ"), strTempData, nMainGridRowIdx);
			//===================================================
 			
 			

// 			pDM_TO = pDM_SUCCESS;
// 			nID_TO = 3;
// 			strTO = _T("���� : ");

 			// ������Ϲ�ȣ�� �ִ� ��Ϲ�ȣ��� �������ش�.
 			CString strRegNo ;
 			CString strGetAccRegNo ;
 			CString strQuery ;
 			CString strRegCode ;
 			INT iRegNumber ;
 			INT nCount ;
 			CString strCount =_T("") ;
 			
 			nCount = DM_Book.GetRowCount () ;
 			DM_Book.GetCellData ( strHD_Book + _T("_��Ϲ�ȣ"),idxDM,strRegNo);
 			if(!strRegNo.IsEmpty ())
 			{   
 				strRegCode = strRegNo.Left(nRegCodeLength) ;
 				iRegNumber = _ttoi(strRegNo.Mid (nRegCodeLength)) ;
 			
				strQuery.Format (_T("DELETE FROM CO_DROP_ACCESSION_NO_TBL  WHERE REG_CODE = '%s' and  REG_NO = %d ;"), strRegCode,iRegNumber );
 				DM_Book.StartFrame () ;
 				DM_Book.InitDBFieldData () ;
 				DM_Book.AddFrame (strQuery);
 				DM_Book.SendFrame ();
 				DM_Book.EndFrame () ;
 			}

			// KOL.RED.015 JMC 20160928 ��ũ���� ���Խ� ��������ȣ �ڵ� ������Ʈ
			// �Ϸ�� å�� ��Ϲ�ȣ�� ����.
			ids = pDlg->SaveSuccRegNo(&DM_Book, &DM_MaxRegBo);

			// KOL.RED.015 JMC 20160928 ��ũ���� ���Խ� ��������ȣ �ڵ� ������Ʈ
			// ���Լ���ȣ ����
			ids = pDlg->SaveSuccAcqNo(&DM_Species, &DM_MaxAcqNo);			
 		}
 
 		//===================================================================
 		// 12. ���¹��� ������ �����Ѵ�.
 		//===================================================================
// 		strData = pDlg->m_wndStatusBarCtrl.GetText(1);
// 		strData = strData.Mid(strData.ReverseFind(':') + 1);
// 		strData.Format(_T("%d"), _ttoi(strData) - 1);
// 		pDlg->m_wndStatusBarCtrl.SetText(_T("���� : ") + strData, 1, 0);
// 		
// 		strData = pDlg->m_wndStatusBarCtrl.GetText(nID_TO);
// 		strData = strData.Mid(strData.ReverseFind(':') + 1);
// 		strData.Format(_T("%d"), _ttoi(strData) + 1);
// 		pDlg->m_wndStatusBarCtrl.SetText(strTO + strData, nID_TO, 0);
 
 
 		//2003.10.15 ������ ����
//          nTotalCnt =  strTO+strData ; 
 		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 
 		//===================================================================
 		// 13. ������ ���� ������ �����͸� �ű��.
 		//===================================================================
 
 		// ���� �� ������ ���� ����
// 		if (!strErrorMsg.IsEmpty())
// 		{
// 			pDM_OK->SetCellData( _T("BS_���Ϲ��Կ���"), strErrorMsg, idx);
// 		}

 		//===================================================================
 		// 14. ����ٸ� �������ش�.
 		//===================================================================
 		pDlg->m_wndProgressBar.StepIt(); 

 		end_app = GetTickCount();
 		nProcCount++;
 		strMsg.Format(_T("ROW %d \t %d.%03d\n"), nProcCount, (end_app-start_app)/1000, (end_app-start_app)%1000);	
 		_ftprintf( fpTime, strMsg );
		
		nSelectIdx = pGrid_OK->SelectGetNext();
 	}

	// KOL.RED.015 JMC 20160928 ��ũ���� ���Խ� ��������ȣ �ڵ� ������Ʈ
	// ���� ������ ���������� ���Ե� ��Ϲ�ȣ�� ��������ȣ ������Ʈ
	ids = pDlg->UpdateLastNo(&DM_MaxRegBo);
	if ( ids < 0 ) {
		AfxMessageBox(_T("UpdateLastNo() Error!"));
	}
	// ���Լ���ȣ ������Ʈ
	ids = pDlg->UpdateAcqNo(&DM_MaxAcqNo);
	if ( ids < 0 ) {
		AfxMessageBox(_T("UpdateAcqNo() Error!"));
	}

 	// ��ü ���Խð�
 	end_total = GetTickCount();
 	strMsg.Format(_T("��ü���Խð� \t %d.%03d\n======================================\n"), (end_total-start_total)/1000, (end_total-start_total)%1000);	
 	_ftprintf( fpTime, strMsg );		
 	fclose(fpTime);
 		
 	pDlg->m_wndProgressBar.SetWindowText(_T("���� �Ϸ�."));

	nSelectIdx = pGrid_OK->SelectGetTailPosition();
	if ( !pDlg->m_bIsThreadRunning ) 
	{
		for( INT i = 0 ; i < nSelectRowCnt - nStopCnt ; i++ )
		{
			nSelectIdx = pGrid_OK->SelectGetPrev();
		}

		nSelectRowCnt = nStopCnt;
	}

	for( nChkCnt = 0 ; nChkCnt < nSelectRowCnt ; ++nChkCnt )
	{
		INT nRowsTo;

		strData = pDM_OK->GetCellData( _T("BS_���Ϲ��Կ���"), nSelectIdx);

		if( strData.IsEmpty() )
		{
			pDM_TO = pDM_SUCCESS;
		}
		else
		{
			pDM_TO = pDM_ERR;
		}

		nRowsTo = pDM_TO->GetRowCount();
		pDM_TO->InsertRow(-1);
		
		for (INT col = 0; col < nColCnt; col++)
 		{	
			strData = pDM_OK->GetCellData( nSelectIdx ,col);
 			pDM_TO->SetCellData( nRowsTo ,col,  strData);

 		}

		pDM_OK->DeleteRow( nSelectIdx);
		nSelectIdx = pGrid_OK->SelectGetPrev();
	}

	{
	//===================================================================
 	// ���¹��� ������ �����Ѵ�.
 	
	INT nRowCnt;
	CString strCount;

	nRowCnt = pDM_OK->GetRowCount();
	strCount.Format(_T("���� : %d"), nRowCnt);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 1, 0);	
	
	nRowCnt = pDM_ERR->GetRowCount();

	strCount.Format(_T("���� : %d"), nRowCnt);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 2, 0);

	nRowCnt = pDM_SUCCESS->GetRowCount();
	strCount.Format(_T("���� : %d"), nRowCnt);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 3, 0);	
	}
	//===================================================================
 
 	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 	//2003.10.20 ������ ���� 
	nTotalCnt = pDlg->m_wndStatusBarCtrl.GetText(0) ;
    nFalseCnt = pDlg->m_wndStatusBarCtrl.GetText(2) ;
 	nTrueCnt  = pDlg->m_wndStatusBarCtrl.GetText(3) ;
 
 	strTotalMsg.Format( _T("���� �Ϸ�. \r\n\r\n %s�� \r\n %s�� \r\n %s��"),nTotalCnt,nFalseCnt,nTrueCnt ); 
 	AfxMessageBox(strTotalMsg);
 	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 
 	pDlg->m_bIsThreadRunning = FALSE;
 	pDlg->ButtonStatus(1);

	::PostMessage(pDlg->GetSafeHwnd(), WM_SORT_GRID, 0, 0);

 	SetEvent(pDlg->m_hThread);
 
 	return 0;
 
 ERR:
	// KOL.RED.015 JMC 20160928 ��ũ���� ���Խ� ��������ȣ �ڵ� ������Ʈ
	// ������ �߻��ϱ� ������ ���Ե� ��Ϲ�ȣ�� ��������ȣ ������Ʈ
	ids = pDlg->UpdateLastNo(&DM_MaxRegBo);
	if ( ids < 0 ) {
		AfxMessageBox(_T("UpdateLastNo() Error!"));
	}
	// ���Լ���ȣ ������Ʈ
	ids = pDlg->UpdateAcqNo(&DM_MaxAcqNo);
	if ( ids < 0 ) {
		AfxMessageBox(_T("UpdateAcqNo() Error!"));
	}

 	pDlg->m_wndProgressBar.SetWindowText(_T("���� ����"));
 	AfxMessageBox(_T("���� ����"));
 	pDlg->m_bIsThreadRunning = FALSE;
 	pDlg->ButtonStatus(1);
 	return -1;
 
 	return -1;
 }

 // KOL.RED.015 JMC 20160928 ��ũ���� ���Խ� ��������ȣ �ڵ� ������Ʈ
VOID CMarcImportDlg::OnChkLastNoUp_REG()
{
	CButton * pStatus;	
	pStatus = (CButton *)GetDlgItem(IDC_ChkLastNoUp_REG);
	if ( pStatus->GetCheck() == 1 )  {
		m_bAutoReg = TRUE;
		CString strMsg = _T("");
		strMsg.Format(_T("��Ϲ�ȣ�� �ο��� ��ũ(�ڷ�)�� �����Ͽ� �״�� �ڰ��� ��Ϲ�ȣ�� ����� �� ����˴ϴ�.\n")
			          _T("�����ڷ��� ��ϱ����� [���� > ����ȯ�漳�� > ��������ȣ����] ���� ���� �� �̾�� ���ŵ˴ϴ�.")
					  _T("����ִ�(����)��ȣ�� [���� > ����ȯ�漳�� > ��Ϲ�ȣ����]���� �����Ͻʽÿ�."));					  
		AfxMessageBox(strMsg);
	}
	else {
		m_bAutoReg = FALSE;
	}
}

// KOL.RED.015 JMC 20160928 ��ũ���� ���Խ� ��������ȣ �ڵ� ������Ʈ
VOID CMarcImportDlg::OnChkLastNoUp_Acq()
{
	CButton * pStatus;	
	pStatus = (CButton *)GetDlgItem(IDC_ChkLastNoUp_Acq);
	if ( pStatus->GetCheck() == 1 )  {
		m_bAutoAcq = TRUE;
		CString strMsg = _T("");
		strMsg.Format(_T("���Լ� û����ȣ�� �ο��� ��ũ(�ڷ�)�� ���� �� �� ����˴ϴ�.\n")
			          _T("090$a , 090$b �� �ش簪�� [���� > �ý���ȯ�漳�� > ���Լ���ȣ����] ���� ���� �� �̾�� ���ŵ˴ϴ�.\n")
					  _T("�з���ȣ�� KDC�̸�, ��ġ��ȣ�� ������� �ʽ��ϴ�."));

		AfxMessageBox(strMsg);
	}
	else {
		m_bAutoAcq = FALSE;
	}
}	

// KOL.RED.015 JMC 20160928 ��ũ���� ���Խ� ��������ȣ �ڵ� ������Ʈ
// ���ε� ������ ��Ϲ�ȣ�� ������Ʈ�뵵�� �����Ѵ�.
INT CMarcImportDlg::SaveSuccRegNo(CESL_DataMgr* pDM_Book, CESL_DataMgr* pDM_MaxRegBo)
{
	if ( m_bAutoReg == FALSE ) return 0;

	INT ids = -1, nRowCnt = 0, i = -1, j = -1, nRegCodeLength = -1, nSerialNo = 0, nSerialLng = 0;
	BOOL bNumber = TRUE;
	CString strRegCode = _T(""), strSerialNo = _T(""), strMsg = _T(""), strRegNo = _T(""), strCmpRegCode = _T("");	

	nRowCnt = pDM_Book->GetRowCount();
	if ( nRowCnt <= 0 ) return 0;
	
	// ��ϱ��� �ڸ��� Ȯ��.
 	nRegCodeLength = this->m_pInfo->nREG_CODE_LENGTH;

	// �Ǻ�ó��
	for( i = 0; i < nRowCnt; i++ ) {
		// å���� üũ
		if (!IsBook(pDM_Book, i, _T("BB") ) ) continue;

		// ��ϱ��� / �Ϸù�ȣ ���ϱ�
		ids = pDM_Book->GetCellData(_T("BB_��ϱ���_�ڵ�"), i, strRegCode);
		if ( ids < 0 ) return -5;
		ids = pDM_Book->GetCellData(_T("BB_��Ϲ�ȣ"), i, strRegNo);
		if ( ids < 0 ) return -10;

		if ( strRegNo.GetLength() < 12) {
			strMsg.Format(_T("��Ϲ�ȣ�� 12�ڸ����� �� : [%s] �� ó���Ұ�."), strRegNo);
			pDM_Book->QueryLog(strMsg.GetBuffer(0));
			continue;
		}

		strCmpRegCode = strRegNo.Left(nRegCodeLength);
		if ( strRegCode != strCmpRegCode ) {
			strMsg.Format(_T("��ϱ��а� ��ȣ���� ������ ��ϱ��� �ٸ� : [%s-%s] �� ó���Ұ�."), strRegCode, strCmpRegCode);
			pDM_Book->QueryLog(strMsg.GetBuffer(0));
			continue;
		}

		// �̾Ƴ� �Ϸù�ȣ�� ��� �������� üũ
		strSerialNo = strRegNo.Mid(nRegCodeLength);		
		nSerialLng = strSerialNo.GetLength();
		for ( j = 0; j < nSerialLng; j++ ) {
			if ( '0' > strSerialNo.GetAt(j) || '9' < strSerialNo.GetAt(j) ) bNumber = FALSE;
		}
		if ( bNumber == FALSE ) {
			strMsg.Format(_T("��Ϲ�ȣ���� ������ �Ϸù�ȣ�� ���ڰ� �ƴ� : [%s]"), strSerialNo);
			pDM_Book->QueryLog(strMsg.GetBuffer(0));
			continue;
		}
		
		// ��ϱ��к��� ���� ū ������ Ȯ���ؼ� �ִ´�.
		nSerialNo = _ttoi(strSerialNo);		
		ids = CheckNPutSerialNo(pDM_MaxRegBo, strRegCode, nSerialNo);
		if ( ids < 0 ) return -15;
	}	

	return 0;
}
// KOL.RED.015 JMC 20160928 ��ũ���� ���Խ� ��������ȣ �ڵ� ������Ʈ
INT CMarcImportDlg::CheckNPutSerialNo(CESL_DataMgr* pDM_MaxRegBo, CString strRegCode, INT nSerialNo)
{
	INT ids = -1, nRowCnt = 0, i = 0, nCmpNo = 0;
	BOOL bExist = FALSE;
	CString strCmpRegCode = _T(""), strCmpSerialNo = _T(""), strSerialNo = _T("");

	nRowCnt = pDM_MaxRegBo->GetRowCount();
	if ( nRowCnt == 0 ) {
		pDM_MaxRegBo->InsertRow(-1);
		pDM_MaxRegBo->SetCellData(_T("BB_��ϱ���_�ڵ�"), strRegCode, 0);
		strSerialNo.Format(_T("%d"), nSerialNo);
		pDM_MaxRegBo->SetCellData(_T("����ū�Ϸù�ȣ"), strSerialNo, 0);
		return 0;
	}

	// �߰��� ��ϱ����߿� ���ٸ� �߰�.
	for ( i = 0 ; i < nRowCnt; i++) {
		ids = pDM_MaxRegBo->GetCellData(_T("BB_��ϱ���_�ڵ�"), i, strCmpRegCode);
		if ( strCmpRegCode == strRegCode ) bExist = TRUE;		
	}

	if ( bExist == FALSE ) {
		pDM_MaxRegBo->InsertRow(-1);
		nRowCnt = pDM_MaxRegBo->GetRowCount();
		pDM_MaxRegBo->SetCellData(_T("BB_��ϱ���_�ڵ�"), strRegCode, nRowCnt - 1);
		strSerialNo.Format(_T("%d"), nSerialNo);
		pDM_MaxRegBo->SetCellData(_T("����ū�Ϸù�ȣ"), strSerialNo, nRowCnt - 1);
		return 0;
	}

	// ���� ū�� Ȯ���ؼ� �ٲ�ġ���Ѵ�.
	for ( i = 0; i < nRowCnt; i++ ) {
		ids = pDM_MaxRegBo->GetCellData(_T("BB_��ϱ���_�ڵ�"), i, strCmpRegCode);
		if ( strCmpRegCode != strRegCode ) continue;

		ids = pDM_MaxRegBo->GetCellData(_T("����ū�Ϸù�ȣ"), i, strCmpSerialNo);
		nCmpNo = _ttoi(strCmpSerialNo);

		if ( nSerialNo > nCmpNo ) {
			strSerialNo.Format(_T("%d"), nSerialNo);
			ids = pDM_MaxRegBo->SetCellData(_T("����ū�Ϸù�ȣ"), strSerialNo, i );
			break;
		}
	}

	return 0;
}

// KOL.RED.015 JMC 20160928 ��ũ���� ���Խ� ��������ȣ �ڵ� ������Ʈ
INT CMarcImportDlg::UpdateLastNo(CESL_DataMgr* pDM_MaxRegBo)
{
	if ( m_bAutoReg == FALSE ) return 0;

	INT ids = -1, nRowCnt = 0, i = -1, nProcCnt = 0;
	CString strQuery = _T(""), strRegCode = _T(""), strSerialNo = _T(""), strReckey = _T(""), strYear = _T(""), strOneMsg = _T(""), strMsg = _T("");

	strMsg = _T("��������ȣ�� ������Ʈ �Ǿ����ϴ�.");
	nRowCnt = pDM_MaxRegBo->GetRowCount();
	if (nRowCnt <= 0 ) return 0;	

	// �Ǻ�ó��
	for( i = 0; i < nRowCnt; i++ ) {
		ids = pDM_MaxRegBo->GetCellData(_T("BB_��ϱ���_�ڵ�"), i, strRegCode);
		if ( ids < 0 ) return -5;
		ids = pDM_MaxRegBo->GetCellData(_T("����ū�Ϸù�ȣ"), i, strSerialNo);
		if ( ids < 0 ) return -10;

		// ������Ʈ ��� ��ȸ
		strQuery.Format(_T("SELECT REC_KEY FROM CO_LAST_NUMBER_TBL WHERE ")
			            _T("MANAGE_CODE=UDF_MANAGE_CODE AND ")
						_T("KIND_CODE='REG_NO' AND ")						
						_T("PUBLISH_FORM_CODE = 'CO' AND ")
						_T("DIVISION_VALUE='%s' AND ")
						_T("LAST_NUMBER < %s"), strRegCode, strSerialNo);
		ids = pDM_MaxRegBo->GetOneValueQuery( strQuery , strReckey );
		if ( ids < 0 && ids != -4007 ) return -15;

		if ( strReckey.GetLength() <= 0 ) {
			strQuery.Format(_T("����� REC_KEY : [NULL]���� ������Ʈ ��� ����."));
			pDM_MaxRegBo->QueryLog(strQuery.GetBuffer(0));
			continue;
		}
		
		// ������Ʈ
		strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = %s, LAST_WORK='%s' WHERE REC_KEY=%s; "), strSerialNo, this->GetWorkLogMsg(_T("��ũ�����ڵ�������Ʈ"), __WFILE__, __LINE__), strReckey);
		ids = pDM_MaxRegBo->StartFrame();
		if ( ids < 0 ) return -20;
		pDM_MaxRegBo->InitDBFieldData();		
		ids = pDM_MaxRegBo->AddFrame(strQuery);
		if ( ids < 0 ) return -30;
		ids = pDM_MaxRegBo->SendFrame();
		if ( ids < 0 ) return -35;
		ids = pDM_MaxRegBo->EndFrame();
		if ( ids < 0 ) return -40;

		// �޽��� �ۼ�.
		nProcCnt++;
		strOneMsg.Format(_T("\n��ϱ��� %s : %s"), strRegCode, strSerialNo);
		strMsg += strOneMsg;
	}

	// �Ϸ�޽��� ���.
	if ( nProcCnt <= 0 ) return 0;

	strMsg += _T(".");
	AfxMessageBox(strMsg);
	
	return 0;
}

// KOL.RED.015 JMC 20160928 ��ũ���� ���Խ� ��������ȣ �ڵ� ������Ʈ
// ���ε� ������ ���Լ���ȣ�� ������Ʈ�뵵�� �����Ѵ�.
INT CMarcImportDlg::SaveSuccAcqNo(CESL_DataMgr* pDM_Species, CESL_DataMgr* pDM_MaxAcqNo)
{
	if ( m_bAutoAcq == FALSE ) return 0;

	INT ids = -1, nRowCnt = 0, i = -1, j = -1, nAcqNo = 0, nLength = 0, n090aCnt = 0, n090bCnt = 0;
	BOOL bNumber = TRUE;
	CString strClassNo = _T(""), strAcqNo = _T(""), strMsg = _T(""), strCmpClassNo = _T("");	
	
	CMarc marc;
	CString strMarc = _T(""), strItemData = _T(""), str090a = _T(""), str090b = _T(""), strControlNo = _T("");
	CArray<CString, CString&> pArr090aItem;
	CArray<CString, CString&> pArr090bItem;
	CArray<CString, CString&> pArr001Item;
	
	strMarc = pDM_Species->GetCellData( _T("BS_MARC"), 0 );
	ids = this->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	ids = this->m_pInfo->pMarcMgr->GetItem(&marc, _T("090$a"), strItemData, &pArr090aItem);
	ids = this->m_pInfo->pMarcMgr->GetItem(&marc, _T("090$b"), strItemData, &pArr090bItem);
	ids = this->m_pInfo->pMarcMgr->GetItem(&marc, _T("001*"), strControlNo, &pArr001Item);	

	n090aCnt = pArr090aItem.GetSize();
	n090bCnt = pArr090bItem.GetSize();
	if ( n090aCnt != n090bCnt) {
		strMsg.Format(_T("�����ȣ %s �� 090$a�� $b�� ������ ��ġ���� ����. [090$a : %d, 090$b : %d]"), strControlNo, n090aCnt, n090bCnt);
		pDM_MaxAcqNo->QueryLog(strMsg.GetBuffer(0));
		return 0;
	}

	for( i = 0; i < n090aCnt; i++ ) {
		str090a = pArr090aItem.GetAt( i );
		str090b = pArr090bItem.GetAt( i );

		// �з���ȣ�� ���� . �� �־�� �ϰ�
		bNumber = TRUE;
		nLength = str090a.GetLength();
		for ( j = 0; j < nLength; j++ ) {
			if ( !(('0' <= str090a.GetAt(j) && '9' >= str090a.GetAt(j) ) || str090a.GetAt(j) == '.' )) bNumber = FALSE;
		}
		if ( bNumber == FALSE ) {
			strMsg.Format(_T("�����ȣ %s �� �з���ȣ �Ǵ� ����. [090$a : %s]"), strControlNo, str090a);
			pDM_MaxAcqNo->QueryLog(strMsg.GetBuffer(0));
			pArr090aItem.RemoveAll();
			pArr090bItem.RemoveAll();
			pArr001Item.RemoveAll();			
			return 0;
		}

		// ���Լ���ȣ�� ���ڸ� �־�� �Ѵ�.
		bNumber = TRUE;
		nLength = str090b.GetLength();
		for ( j = 0; j < nLength; j++ ) {
			if ( '0' > str090b.GetAt(j) || '9' < str090b.GetAt(j) ) bNumber = FALSE;
		}
		if ( bNumber == FALSE ) {
			strMsg.Format(_T("�����ȣ %s �� ������ȣ�� ���ڰ� �ƴ�. [090$b : %s]"), strControlNo, str090b);
			pDM_MaxAcqNo->QueryLog(strMsg.GetBuffer(0));
			pArr090aItem.RemoveAll();
			pArr090bItem.RemoveAll();
			pArr001Item.RemoveAll();			
			return 0;
		}

		nAcqNo = _ttoi(str090b);
		ids = CheckNPutAcqNo(pDM_MaxAcqNo, str090a, nAcqNo);
		if ( ids < 0 ) {
			pArr090aItem.RemoveAll();
			pArr090bItem.RemoveAll();
			pArr001Item.RemoveAll();			
			return -15;
		}
	}

	pArr090aItem.RemoveAll();
	pArr090bItem.RemoveAll();
	pArr001Item.RemoveAll();	
	
	return 0;
}

// KOL.RED.015 JMC 20160928 ��ũ���� ���Խ� ��������ȣ �ڵ� ������Ʈ
INT CMarcImportDlg::CheckNPutAcqNo(CESL_DataMgr* pDM_MaxAcqNo, CString strClassNo, INT nAcqNo)
{
	INT ids = -1, nRowCnt = 0, i = 0, nCmpNo = 0;
	BOOL bExist = FALSE;
	CString strCmpClassNo = _T(""), strCmpAcqNo = _T(""), strSerialNo = _T("");

	nRowCnt = pDM_MaxAcqNo->GetRowCount();
	if ( nRowCnt == 0 ) {
		pDM_MaxAcqNo->InsertRow(-1);
		pDM_MaxAcqNo->SetCellData(_T("�з���ȣ"), strClassNo, 0);
		strSerialNo.Format(_T("%d"), nAcqNo);
		pDM_MaxAcqNo->SetCellData(_T("����ū�Ϸù�ȣ"), strSerialNo, 0);
		return 0;
	}

	// �߰��� ��ϱ����߿� ���ٸ� �߰�.
	for ( i = 0 ; i < nRowCnt; i++) {
		ids = pDM_MaxAcqNo->GetCellData(_T("�з���ȣ"), i, strCmpClassNo);
		if ( strCmpClassNo == strClassNo ) bExist = TRUE;	
	}

	if ( bExist == FALSE ) {
		pDM_MaxAcqNo->InsertRow(-1);
		nRowCnt = pDM_MaxAcqNo->GetRowCount();
		pDM_MaxAcqNo->SetCellData(_T("�з���ȣ"), strClassNo, nRowCnt - 1);
		strSerialNo.Format(_T("%d"), nAcqNo);
		pDM_MaxAcqNo->SetCellData(_T("����ū�Ϸù�ȣ"), strSerialNo, nRowCnt - 1);
		return 0;
	}

	// ���� ū�� Ȯ���ؼ� �ٲ�ġ���Ѵ�.
	for ( i = 0; i < nRowCnt; i++ ) {
		ids = pDM_MaxAcqNo->GetCellData(_T("�з���ȣ"), i, strCmpClassNo);
		if ( strCmpClassNo != strClassNo ) continue;

		ids = pDM_MaxAcqNo->GetCellData(_T("����ū�Ϸù�ȣ"), i, strCmpAcqNo);
		nCmpNo = _ttoi(strCmpAcqNo);

		if ( nAcqNo > nCmpNo ) {
			strSerialNo.Format(_T("%d"), nAcqNo);
			ids = pDM_MaxAcqNo->SetCellData(_T("����ū�Ϸù�ȣ"), strSerialNo, i );
			break;
		}
	}

	return 0;
}

// KOL.RED.015 JMC 20160928 ��ũ���� ���Խ� ��������ȣ �ڵ� ������Ʈ
INT CMarcImportDlg::UpdateAcqNo(CESL_DataMgr* pDM_MaxAcqNo)
{
	if ( m_bAutoAcq == FALSE ) return 0;

	INT ids = -1, nRowCnt = 0, i = -1, nProcCnt = 0;
	CString strQuery = _T(""), strClassNo = _T(""), strSerialNo = _T(""), strReckey = _T(""), strOneMsg = _T(""), strMsg = _T("");

	strMsg = _T("���Լ���ȣ�� ������Ʈ �Ǿ����ϴ�.");
	nRowCnt = pDM_MaxAcqNo->GetRowCount();
	if (nRowCnt <= 0 ) return 0;

	// �Ǻ�ó��
	for( i = 0; i < nRowCnt; i++ ) {
		ids = pDM_MaxAcqNo->GetCellData(_T("�з���ȣ"), i, strClassNo);
		if ( ids < 0 ) return -5;
		ids = pDM_MaxAcqNo->GetCellData(_T("����ū�Ϸù�ȣ"), i, strSerialNo);
		if ( ids < 0 ) return -10;

		// ������Ʈ ��� ��ȸ
		strQuery.Format(_T("SELECT REC_KEY FROM CO_LAST_BOOK_ACQUISITNO_TBL WHERE ")
			            _T("MANAGE_CODE=UDF_MANAGE_CODE AND ")
						_T("CLASS_NO_TYPE='1' AND ")
						_T("CLASS_NO = '%s' AND ")
						_T("SEPARATE_SHELF_CODE IS NULL AND ")						
						_T("ACQUISIT_NO < %s"), strClassNo, strSerialNo);
		ids = pDM_MaxAcqNo->GetOneValueQuery( strQuery , strReckey );
		if ( ids < 0 && ids != -4007 ) return -15;

		if ( strReckey.GetLength() <= 0 ) {
			strQuery.Format(_T("����� REC_KEY : [NULL]���� ������Ʈ ��� ����."));
			pDM_MaxAcqNo->QueryLog(strQuery.GetBuffer(0));
			continue;
		}
		
		// ������Ʈ
		strQuery.Format(_T("UPDATE CO_LAST_BOOK_ACQUISITNO_TBL SET ACQUISIT_NO = %s, LAST_WORK='%s' WHERE REC_KEY=%s; "), strSerialNo, this->GetWorkLogMsg(_T("��ũ�����ڵ�������Ʈ"), __WFILE__, __LINE__), strReckey);
		ids = pDM_MaxAcqNo->StartFrame();
		if ( ids < 0 ) return -20;
		pDM_MaxAcqNo->InitDBFieldData();		
		ids = pDM_MaxAcqNo->AddFrame(strQuery);
		if ( ids < 0 ) return -30;
		ids = pDM_MaxAcqNo->SendFrame();
		if ( ids < 0 ) return -35;
		ids = pDM_MaxAcqNo->EndFrame();
		if ( ids < 0 ) return -40;

		// �޽��� �ۼ�.
		nProcCnt++;
		strOneMsg.Format(_T("\n�з���ȣ %s : %s"), strClassNo, strSerialNo);
		strMsg += strOneMsg;
	}

	// �Ϸ�޽��� ���.
	if ( nProcCnt <= 0 ) return 0;

	strMsg += _T(".");
	AfxMessageBox(strMsg);
	
	return 0;
}

//===================================================

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] ������ġ ������
// [Min Spec] 
// 1. ���¹�/�����/�׸��带 �ʱ�ȭ�Ѵ�.
// 2. ������ ��DM���� ����/���� ������ ī��Ʈ �Ѵ�.
// 3. å���� �����ϰ� ��Ϲ�ȣ�� GRID�� �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////
//===================================================
//2009.03.10 UPDATE BY PJW : ���Ϲ��� �׸��忡 DM�̿��ϴ� ������ ����
//UINT ThreadFunc_Display(LPVOID pParam)
//{
//	CMarcImportDlg *pDlg = (CMarcImportDlg*)pParam;
//	if (pDlg == NULL)
//	{
//		pDlg->m_bIsThreadRunning = FALSE;
//		return -1;
//	}
//
//	pDlg->m_bIsThreadRunning = TRUE;
//	pDlg->ButtonStatus(0);
//
//	// Initialization Variable
//	CESL_DataMgr *pDM_Species = NULL;
//	CESL_ControlMgr *pCM = NULL;
//
//	CESL_Grid *pGrid_OK			= NULL;
//	CESL_Grid *pGrid_ERR		= NULL;
//	CESL_Grid *pGrid_SUCCESS	= NULL;
//
//	CESL_Grid *pGrid = NULL;
//
//	CString strHD_Species;
//
//	// For Count
//	CString strCount;
//	
//	INT i = 0;
//	INT nTotalCount = -1;
//	INT nErrCount = 0;
//	INT nOKCount = 0;
//
//	// For Fetch
//	CString strGridAlias;
//
//	INT row = 0;
//	INT nGridRow = 0;
//	INT nErrGridRow = 0;
//	INT nOKGridRow = 0;
//	
//	POSITION pos = NULL;
//	INT nGridCol = -1;
//	INT nGridInxCount = 1;
//	INT stepQuota = 10;
//
//	//===================================================================
//	// 1. ���¹�/�����/�׸��带 �ʱ�ȭ�Ѵ�.
//	//===================================================================
//
//	// ������ DM
//	pDM_Species = pDlg->m_pDM_Species;
//	if (pDM_Species == NULL)
//	{
//		goto ERR;
//	}
//	
//	nTotalCount = pDM_Species->GetRowCount() ;
//	
//	pCM = pDlg->FindCM(pDlg->m_lpszCMAlias);
//	if (pCM == NULL)
//	{
//		goto ERR;
//	}
//
//	// TOTAL_COUNT
//	strCount.Format(_T("��ü : %d"), nTotalCount);
//	pDlg->m_wndStatusBarCtrl.SetText(strCount, 0, 0);
//	
//	// ��������
//	//if ( nTotalCount/stepQuota == 0 )
//	//	pDlg->m_wndProgressBar.SetRange(0, 1);
//	//else 
//	pDlg->m_wndProgressBar.SetRange(0, nTotalCount);
//	
//	pDlg->m_wndProgressBar.SetStep(1);
//	pDlg->m_wndProgressBar.SetPos(0);
//	pDlg->m_wndProgressBar.SetWindowText(_T(""));
//		
//	// HSS_040524 �������
//	strHD_Species = _T("BS");
//
//	// ERR_COUNT
//	for (i = 0; i < nTotalCount; i++)
//	{
//		if (!pDM_Species->GetCellData( strHD_Species + _T("_���Ϲ��Կ���"), i).IsEmpty())
//		{
//			nErrCount++;
//		}
//	}
//
//	strCount.Format(_T("���� : %d"), nErrCount);
//	pDlg->m_wndStatusBarCtrl.SetText(strCount, 2, 0);
//		
//	// OK_COUNT
//	nOKCount = nTotalCount - nErrCount;
//	strCount.Format(_T("���� : %d"), nOKCount);
//	pDlg->m_wndStatusBarCtrl.SetText(strCount, 1, 0);
//	
//	// SUCCESS_COUNT
//	pDlg->m_wndStatusBarCtrl.SetText(_T("���� : 0"), 3, 0);
//
//	pGrid_OK		= pDlg->m_pGrid_OK;
//	pGrid_ERR		= pDlg->m_pGrid_ERR;
//	pGrid_SUCCESS	= pDlg->m_pGrid_SUCCESS;
//	if (pGrid_OK == NULL || pGrid_ERR == NULL || pGrid_SUCCESS == NULL)
//	{
//		goto ERR;
//	}
//
//	// Init Grid
//	pGrid_OK->Clear();
//	pGrid_OK->SetRows(2);
//	pGrid_OK->SetColumnInfo();
//
//	pGrid_ERR->Clear();
//	pGrid_ERR->SetRows(2);
//	pGrid_ERR->SetColumnInfo();
//
//	pGrid_SUCCESS->Clear();
//	pGrid_SUCCESS->SetRows(2);
//	pGrid_SUCCESS->SetColumnInfo();
//
//
//	// Set Grid Rows
//	if (nErrCount > 0)
//		pGrid_ERR->SetRows(nErrCount + 1);
//	if (nOKCount > 0)
//		pGrid_OK->SetRows(nOKCount + 1);
//
//	//========================   Fetch Start  ===========================
//	
//	for (row = 0; row < nTotalCount; row++)
//	{
//		CString sBookCnt;
//
//		//===============================================================
//		// 2. ������ ��DM���� ����/���� ������ ī��Ʈ �Ѵ�.
//		//===============================================================
//		//if (0==row % stepQuota)
//		pDlg->m_wndProgressBar.StepIt();
//
//		if (pDM_Species->GetCellData( strHD_Species + _T("_���Ϲ��Կ���"), row).IsEmpty())
//		{
//			pGrid = pGrid_OK;
//			nGridRow = ++nOKGridRow;
//		}
//		else
//		{
//			pGrid = pGrid_ERR;
//			nGridRow = ++nErrGridRow;
//		}
//
//		strCount.Format(_T("%d"), row+1);
//		pGrid->SetTextMatrix(nGridRow, 0, strCount);
//		
//
//		if (!pDlg->m_bIsThreadRunning) 
//			break;
//		
//		//===============================================================
//		// 3. å���� �����ϰ� ��Ϲ�ȣ�� GRID�� �����Ѵ�.
//		//===============================================================
//		pos = pDlg->m_mapGridAlias.GetStartPosition();
//
//		//=====================================================
//		//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
//		INT nRegCodeLength;
//		nRegCodeLength = pDlg->m_pInfo->nREG_CODE_LENGTH;
//		//=====================================================
//		
//		while (pos)
//		{
//			pDlg->m_mapGridAlias.GetNextAssoc(pos, strGridAlias, nGridCol);
//			CString sTmp = pDM_Species->GetCellData(strGridAlias, row);
//
//			//�ڵ��ο� üũ�� ��� �׸��忡 �ڵ��ο��� �ѷ��ֱ�
//			CButton * pChkGiveAutoRegno = (CButton *)pDlg->GetDlgItem(IDC_ChkGiveAutoRegno);
//			CButton * pChkMultiBook = (CButton *)pDlg->GetDlgItem(IDC_ChkMULTIBOOK);
//
//			if ( (pChkGiveAutoRegno->GetCheck() == 1) && strGridAlias ==  strHD_Species + _T("_MARC.049$l") ) 
//			{
//				pGrid->SetTextMatrix(nGridRow, nGridCol, _T("�ڵ��ο�") );
//			
//				if( pChkMultiBook->GetCheck() == 1)
//				{
//					sBookCnt = pDM_Species->GetCellData(_T("UDF_å��"),row);
//					pGrid->SetTextMatrix(nGridRow, nGridCol+1,sBookCnt);
//				}
//				else
//					pGrid->SetTextMatrix(nGridRow, nGridCol+1, _T("1") );
//			}
//			else
//			{
//				//12�ڸ�(���� ���� 2004.09.21)+++++++++++++++++++++++++++
//				if ( strGridAlias == _T("BS_MARC.049$l") )
//				{
//					CString stmpRegNo = pDM_Species->GetCellData(strGridAlias, row);
//					if ( stmpRegNo != _T("") && stmpRegNo.GetLength() < 12 ) {
//						//=====================================================
//						//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
//						//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
//// 						if ( stmpRegNo.GetLength() > 2 ) {
//// 							CString sRight;
//// 							sRight.Format( _T("%010s") , stmpRegNo.Mid(2) );
//// 							stmpRegNo = stmpRegNo.Left(2) + sRight;
//// 						}
//						if ( stmpRegNo.GetLength() > nRegCodeLength ) {
//							stmpRegNo =  pDlg->m_pInfo->MakeRegNo( stmpRegNo.Left(nRegCodeLength) , stmpRegNo.Mid(nRegCodeLength) );
//						}
//						//=====================================================
//						
//					}
//					pGrid->SetTextMatrix(nGridRow, nGridCol, stmpRegNo);
//				}
//				// ++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				else pGrid->SetTextMatrix(nGridRow, nGridCol, pDM_Species->GetCellData(strGridAlias, row));
//			}
//		}
//	}
//
//	// �׸��� ���� ����
//	pGrid_ERR->SetRowHeight(-1, pGrid_ERR->m_nHeight * 15);
//	pGrid_OK->SetRowHeight(-1, pGrid_ERR->m_nHeight * 15);
//	pGrid_SUCCESS->SetRowHeight(-1, pGrid_ERR->m_nHeight * 15);
//	
//	//pDlg->m_wndProgressBar.StepIt();
//	pDlg->m_wndProgressBar.SetWindowText(_T("������ġ �Ϸ�"));
//	pDlg->m_bIsThreadRunning = FALSE;
//	pDlg->ButtonStatus(1);
//
//	SetEvent(pDlg->m_hThread);
//	
//	return 0;
//ERR:
//	pDlg->m_bIsThreadRunning = FALSE;
//	pDlg->ButtonStatus(1);
//	return -1;
//
//	return -1;
//}
UINT ThreadFunc_Display(LPVOID pParam)
{
	// KOL.UDF.022
	CMarcImportDlg *pDlg = (CMarcImportDlg*)pParam;
	if (&pDlg == NULL)
	{
		pDlg->m_bIsThreadRunning = FALSE;
		return -1;
	}

	pDlg->m_bIsThreadRunning = TRUE;
	pDlg->ButtonStatus(0);

	// Initialization Variable
	CESL_DataMgr *pDM_Species = NULL;
	CESL_ControlMgr *pCM = NULL;

	CESL_DataMgr *pDM_OK		= NULL;
	CESL_DataMgr *pDM_ERR		= NULL;
	CESL_DataMgr *pDM_SUCCESS	= NULL;

	CESL_DataMgr *pTempDM = NULL;

	CString strHD_Species;

	// For Count
	CString strCount;
	
	INT i = 0;
	INT nTotalCount = -1;
	INT nErrCount = 0;
	INT nOKCount = 0;

	INT row = 0;

	CString strAlias;
	CStringArray astrAlias;
	CString strFieldName;
	CString strFieldType;
	CString strDataType;
	CString strData;
	INT nColCnt;

	CString strSortNo;
	strSortNo.Empty();

	//===================================================================
	// 1. ���¹�/�����/�׸��带 �ʱ�ȭ�Ѵ�.
	//===================================================================

	// ������ DM
	pDM_Species = pDlg->m_pDM_Species;
	if (pDM_Species == NULL)
	{
		goto ERR;
	}
	
	nTotalCount = pDM_Species->GetRowCount() ;
	
	pCM = pDlg->FindCM(pDlg->m_lpszCMAlias);
	if (pCM == NULL)
	{
		goto ERR;
	}

	// TOTAL_COUNT
	strCount.Format(_T("��ü : %d"), nTotalCount);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 0, 0);
	
	// ��������
	//if ( nTotalCount/stepQuota == 0 )
	//	pDlg->m_wndProgressBar.SetRange(0, 1);
	//else 
	pDlg->m_wndProgressBar.SetRange(0, nTotalCount);
	
	pDlg->m_wndProgressBar.SetStep(1);
	pDlg->m_wndProgressBar.SetPos(0);
	pDlg->m_wndProgressBar.SetWindowText(_T(""));
		
	pDM_OK		= pDlg->m_pDmGridOk;
	pDM_ERR		= pDlg->m_pDmGridError;
	pDM_SUCCESS	= pDlg->m_pDmGridSuccess;

	
// 	INT i;
	nColCnt = pDM_OK->GetRefColumnCount();
	for( i = 0 ; i < nColCnt ; i++ )
	{
		pDM_OK->FindColumnFieldName( i , strAlias , strFieldName ,strFieldType,strDataType,0,strData);
		astrAlias.Add(strAlias);
	}


	//========================   Fetch Start  ===========================
	INT nDmRowIdx;
	
	for (row = 0; row < nTotalCount; row++)
	{
		CString sBookCnt;

		//===============================================================
		// 2. ������ ��DM���� ����/���� ������ ī��Ʈ �Ѵ�.
		//===============================================================
		pDlg->m_wndProgressBar.StepIt();

		if (pDM_Species->GetCellData( _T("BS_���Ϲ��Կ���"), row).IsEmpty())
		{
			pTempDM = pDM_OK;
		}
		else
		{
			pTempDM = pDM_ERR;
		}
		nDmRowIdx = pTempDM->GetRowCount();
		pTempDM->InsertRow(-1);
		
		if (!pDlg->m_bIsThreadRunning) 
		{
			break;
		}
		
		//===============================================================
		// 3. å���� �����ϰ� ��Ϲ�ȣ�� GRID�� �����Ѵ�.
		//===============================================================
		INT nRegCodeLength;
		nRegCodeLength = pDlg->m_pInfo->nREG_CODE_LENGTH;
		for( i = 0 ; i < nColCnt ; i++ )
		{
			strAlias = astrAlias.GetAt(i);
			if( _T("����") == strAlias ) 
			{
				continue;
			}
			if( _T("����") == strAlias )
			{
				strData.Format(_T("%d"), row+1);
			}
			else if(  _T("���۵�Ϲ�ȣ") == strAlias )
			{
				//�ڵ��ο� üũ�� ��� �׸��忡 �ڵ��ο��� �ѷ��ֱ�
				CButton * pChkGiveAutoRegno = (CButton *)pDlg->GetDlgItem(IDC_ChkGiveAutoRegno);
				
				if ( (pChkGiveAutoRegno->GetCheck() == 1) ) 
				{
					strData = _T("�ڵ��ο�");
				}
				else
				{
					strData = pDM_Species->GetCellData(_T("BS_MARC.049$l"), row);
					if ( strData.GetLength() > nRegCodeLength && strData.GetLength() < 12 ) 
					{
						strData =  pDlg->m_pInfo->MakeRegNo( strData.Left(nRegCodeLength) , strData.Mid(nRegCodeLength) );						
					}
				}
			}
			else if( _T("BS_���Ϲ���å��") == strAlias )
			{
				CButton * pChkGiveAutoRegno = (CButton *)pDlg->GetDlgItem(IDC_ChkGiveAutoRegno);
				if ( (pChkGiveAutoRegno->GetCheck() == 1) ) 
				{
					CButton * pChkMultiBook = (CButton *)pDlg->GetDlgItem(IDC_ChkMULTIBOOK);
					if( pChkMultiBook->GetCheck() == 1)
					{
						strData = pDM_Species->GetCellData(_T("UDF_å��"),row);
					}
					else
					{
						strData = _T("1");
					}
				}
				else
				{
					strData = pDM_Species->GetCellData(_T("BS_���Ϲ���å��"),row);					
				}

				
			}
			else if( _T("BS_���Ϲ��Կ���") == strAlias )
			{
				strData = pDM_Species->GetCellData(_T("BS_���Ϲ��Կ���"),row);
			}
			else if( _T("����ISBN") == strAlias )
			{
				strData = pDM_Species->GetCellData(_T("BS_MARC.020$a"),row);
			}
			else
			{
				pDM_Species->GetCellData(("BS_MARC.") + strAlias, row, strData);
			}

			pTempDM->SetCellData( nDmRowIdx, i, strData);
		}
	}

	INT nRowCnt;

	nRowCnt = pDM_OK->GetRowCount();

	strCount.Format(_T("���� : %d"), nRowCnt);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 1, 0);	
	
	nRowCnt = pDM_ERR->GetRowCount();

	strCount.Format(_T("���� : %d"), nRowCnt);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 2, 0);

	pDlg->m_wndStatusBarCtrl.SetText(_T("���� : 0"), 3, 0);

	pDlg->m_wndProgressBar.SetWindowText(_T("������ġ �Ϸ�"));

	pDlg->m_bIsThreadRunning = FALSE;
	pDlg->ButtonStatus(1);

	SetEvent(pDlg->m_hThread);

	::PostMessage(pDlg->GetSafeHwnd(), WM_GRID_DISPLAY, 0, 0);
	
	return 0;
ERR:
	pDlg->m_bIsThreadRunning = FALSE;
	pDlg->ButtonStatus(1);
	return -1;

	return -1;
}
//===================================================

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] �ʱ�ȭ
// [Return Values] ���� 1, ���� -1
// [Mini Spec] 
// 1. API/ API ��� �ʱ�ȭ
// 2. DM �ʱ�ȭ
// 3. �׸��� Alias ����
// ����/���۵�Ϲ�ȣ/å��/��ǥ��/������/������/�Ѽ�ǥ��/�����/�����/ISBN/����
// 4. ���ڵ� ���¿� ���ı��� ����
/////////////////////////////////////////////////////////////////////////////
INT CMarcImportDlg::Init()
{
	EFS_BEGIN

	INT ids = -1;

	//=======================================================================
	// 1. API/ API ��� �ʱ�ȭ
	//=======================================================================
	m_pCatApi = new CCatApi_BO(this);
	if( m_pCatApi == NULL )
	{
		AfxMessageBox(_T("CCatApi_BO �ʱ�ȭ ����"));
		return -1;
	}

	ids = m_pCatApi->InitMemberData();
	if( ids < 0 )
	{
		AfxMessageBox(_T("InitMemberData() Fail"));
		return -1;
	}

	//=======================================================================
	// 2. DM �ʱ�ȭ
	//=======================================================================
	m_lpszSpeciesDMAlias = _T("DM_����_����DM_������");
	m_lpszIndexDMAlias = _T("DM_����_����DM_��������");
	m_lpszVolumeDMAlias = _T("DM_����_����DM_������");
	m_lpszBookDMAlias = _T("DM_����_����DM_å����");
	// KOL.RED.015 JMC 20160928 ��ũ���� ���Խ� ��������ȣ �ڵ� ������Ʈ
	m_lpszMaxRegNoDMAlias = _T("DM_����_����DM_����ū��Ϲ�ȣ");
	m_lpszMaxAcqNoDMAlias = _T("DM_����_����DM_����ū���Լ���ȣ");

	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));

	SetWindowText(_T("���� ��ũ ����"));

	CString strHD_Species;
	strHD_Species = _T("BS");

	
	// ��DM�� �����Ѵ�.
	m_pDM_Species = new CESL_DataMgr;
	if (m_pDM_Species == NULL) 
	{
		AfxMessageBox(_T("m_pDM_Species is null"));
		return -1;
	}

	m_pDM_Species->SetCONNECTION_INFO(CONNECTION_INFO);
	m_pDM_Species->SetMarcMgr(m_pInfo->pMarcMgr);
	m_pDM_Species->InitDMPKFromFile(m_lpszSpeciesDMAlias);

	// ���� DM�� �����Ѵ�.
	m_pDM_Index = new CESL_DataMgr;
	if (m_pDM_Index == NULL) 
	{
		AfxMessageBox(_T("m_pDM_Index is null"));
		return -1;
	}

	m_pDM_Index->SetCONNECTION_INFO(CONNECTION_INFO);
	m_pDM_Index->SetMarcMgr(m_pInfo->pMarcMgr);
	m_pDM_Index->InitDMPKFromFile(m_lpszIndexDMAlias);

	// å DM�� �����Ѵ�.
	m_pDM_Book = new CESL_DataMgr;
	if (m_pDM_Book == NULL) 
	{
		AfxMessageBox(_T("m_pDM_Book is null"));
		return -1;
	}
	m_pDM_Book->SetCONNECTION_INFO(CONNECTION_INFO);
	m_pDM_Book->InitDMPKFromFile(m_lpszBookDMAlias);
	m_pDM_Book->SetMarcMgr(m_pInfo->pMarcMgr);

	m_pDM_Main = FindDM(_T("DM_����_����_�˻����"));
	if( m_pDM_Main == NULL )
	{
		AfxMessageBox(_T("m_pDM_Main is null"));
		return -1;
	}

	
	//===================================================
	//2009.03.10 ADD BY PJW : ���Թ��� ������ ���� DM
	// �����ڷ� DM
	m_pDmGridOk = FindDM(_T("DM_����_��ũ����_GRID_OK"));
	if( m_pDmGridOk == NULL )
	{
		AfxMessageBox(_T("m_pDmOk is null"));
		return -1;
	}

	// �����ڷ� DM
	m_pDmGridError = FindDM(_T("DM_����_��ũ����_GRID_ERROR"));
	if( m_pDmGridError == NULL )
	{
		AfxMessageBox(_T("m_pDmError is null"));
		return -1;
	}

	// �����ڷ� DM
	m_pDmGridSuccess = FindDM(_T("DM_����_��ũ����_GRID_SUCCESS"));
	if( m_pDmGridSuccess == NULL )
	{
		AfxMessageBox(_T("m_pDmSuccess is null"));
		return -1;
	}
	//===================================================

	// CM
	CESL_ControlMgr *pCM = FindCM(m_lpszCMAlias);
	if (pCM == NULL)
	{
		AfxMessageBox(_T("pCM is null"));
		return -1;
	}

	// Grid
	m_pGrid_OK = (CESL_Grid*)pCM->FindControl(_T("Grid_OK"));
	// ERROR GRID
	m_pGrid_ERR = (CESL_Grid*)pCM->FindControl(_T("Grid_ERR"));
	// SUCCESS GRID
	m_pGrid_SUCCESS = (CESL_Grid*)pCM->FindControl(_T("Grid_SUCCESS"));

	if (m_pGrid_OK == NULL || m_pGrid_ERR == NULL || m_pGrid_SUCCESS == NULL)
	{
		AfxMessageBox(_T("Grids is null"));
		return -1;
	}

	// Move Windows
	CRect rcGrid;
	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);
	
	m_pGrid_OK->MoveWindow(rcGrid);
	
	m_pGrid_ERR->MoveWindow(rcGrid);
	
	m_pGrid_SUCCESS->MoveWindow(rcGrid);

	
	//=======================================================================
	// 3. �׸��� Alias ����
	// ����/���۵�Ϲ�ȣ/å��/��ǥ��/������/������/�Ѽ�ǥ��/�����/�����/ISBN/����
	//=======================================================================
	m_mapGridAlias.SetAt(_T(""), 1);
	m_mapGridAlias.SetAt( strHD_Species + _T("_MARC.049$l"), 2);
	m_mapGridAlias.SetAt( strHD_Species + _T("_���Ϲ���å��"), 3);
	m_mapGridAlias.SetAt( strHD_Species + _T("_MARC.����"), 4);
	m_mapGridAlias.SetAt( strHD_Species + _T("_MARC.����"), 5);
	m_mapGridAlias.SetAt( strHD_Species + _T("_MARC.������"), 6);
	m_mapGridAlias.SetAt( strHD_Species + _T("_MARC.�Ѽ�ǥ��"), 7);
	m_mapGridAlias.SetAt( strHD_Species + _T("_MARC.�����"), 8);
	m_mapGridAlias.SetAt( strHD_Species + _T("_MARC.�����"), 9);
	m_mapGridAlias.SetAt( strHD_Species + _T("_MARC.020$a"), 10);
	m_mapGridAlias.SetAt( strHD_Species + _T("_���Ϲ��Կ���"), 11);

	//=======================================================================
	// 4. ���ڵ� ������ �������� ���� ���ı���,��ü����(��ǥ��)����
	//=======================================================================
	// BK
	m_mapRecord_FormCodeMatCode.SetAt(_T("aa"), _T("BK=PR|OF"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ac"), _T("BK=PR|OF"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ad"), _T("BK=PR|OF"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("am"), _T("BK=PR|OF"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ta"), _T("BK=PR|OF"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("tc"), _T("BK=PR|OF"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("td"), _T("BK=PR|OF"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("tm"), _T("BK=PR|OF"));

	// ER
	m_mapRecord_FormCodeMatCode.SetAt(_T("mi"), _T("ER=OB|OA|OD|OH"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("mm"), _T("ER=OB|OA|OD|OH"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ms"), _T("ER=OB|OA|OD|OH"));

	// MP
	m_mapRecord_FormCodeMatCode.SetAt(_T("ea"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("eb"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ec"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ed"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("em"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("es"), _T("MP=NA"));

	m_mapRecord_FormCodeMatCode.SetAt(_T("fa"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("fc"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("fd"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("fm"), _T("MP=NA"));

	// MU
	m_mapRecord_FormCodeMatCode.SetAt(_T("ca"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("cb"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("cc"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("cd"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("cm"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("cs"), _T("MU=NA"));

	m_mapRecord_FormCodeMatCode.SetAt(_T("da"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("dc"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("dd"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("dm"), _T("MU=NA"));

	m_mapRecord_FormCodeMatCode.SetAt(_T("ia"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ib"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ic"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("id"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("im"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("is"), _T("MU=SB|SD|SG"));

	m_mapRecord_FormCodeMatCode.SetAt(_T("ja"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("jb"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("jc"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("jd"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("jm"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("js"), _T("MU=SB|SD|SG"));

	// CR
	m_mapRecord_FormCodeMatCode.SetAt(_T("ab"), _T("CR=PR"));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("ai"), _T("CR=PR"));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("as"), _T("CR=PR"));	

	// VM
	m_mapRecord_FormCodeMatCode.SetAt(_T("ga"), _T("VM=VD|VT|VU"));		
	m_mapRecord_FormCodeMatCode.SetAt(_T("gb"), _T("VM=VD|VT|VU"));		
	m_mapRecord_FormCodeMatCode.SetAt(_T("gc"), _T("VM=VD|VT|VU"));		
	m_mapRecord_FormCodeMatCode.SetAt(_T("gd"), _T("VM=VD|VT|VU"));		
	m_mapRecord_FormCodeMatCode.SetAt(_T("gi"), _T("VM=VD|VT|VU"));		
	m_mapRecord_FormCodeMatCode.SetAt(_T("gm"), _T("VM=VD|VT|VU"));		
	m_mapRecord_FormCodeMatCode.SetAt(_T("gs"), _T("VM=VD|VT|VU"));		

	m_mapRecord_FormCodeMatCode.SetAt(_T("ka"), _T("VM=LP|LQ|LY|LZ"));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("kb"), _T("VM=LP|LQ|LY|LZ"));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("kc"), _T("VM=LP|LQ|LY|LZ"));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("kd"), _T("VM=LP|LQ|LY|LZ"));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("ki"), _T("VM=LP|LQ|LY|LZ"));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("km"), _T("VM=LP|LQ|LY|LZ"));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("ks"), _T("VM=LP|LQ|LY|LZ"));	

	m_mapRecord_FormCodeMatCode.SetAt(_T("ra"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("rb"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("rc"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("rd"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ri"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("rm"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("rs"), _T("VM=VD|VT|VU"));

	m_mapRecord_FormCodeMatCode.SetAt(_T("ya"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("yb"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("yc"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("yd"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("yi"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ym"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ys"), _T("VM=VD|VT|VU"));
	
	// RB
	m_mapRecord_FormCodeMatCode.SetAt(_T("oa"), _T("RB=PR"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("ob"), _T("RB=PR"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("oc"), _T("RB=PR"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("od"), _T("RB=PR"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("oi"), _T("RB=PR"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("om"), _T("RB=PR"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("os"), _T("RB=PR"));

	// �����ڷ�
	m_mapRecord_FormCodeMatCode.SetAt(_T("pa"), _T("MP="));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("pb"), _T("MP="));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("pc"), _T("MP="));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("pd"), _T("MP="));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("pi"), _T("MP="));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("pm"), _T("MP="));	
	m_mapRecord_FormCodeMatCode.SetAt(_T("ps"), _T("MP="));
	

	// Ű���� ���� ���̾�α�
	CMarc marc;
	m_pDlgKeyWord = new CBO_CAT_PROC_KEYWORD_0000(this, marc);
	m_pDlgKeyWord->Create(this);
	m_pDlgKeyWord->ShowWindow(SW_HIDE);

	// 2005.07.01 ADD BY PDJ
	// USMARC���Խ� �����׸� �߰�
	( (CButton*) GetDlgItem (IDC_rKORMARC) )->SetCheck(TRUE);

	// ���� Unicode ����
	( (CButton*) GetDlgItem (IDC_rUNICODE) )->SetCheck(TRUE);

	// ���� ����
	OnChkAccessionRec();

	

	return 0;

	EFS_END
	return -1;
}


/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] ��ư ���� ����
// [Return Values] 
// [Mini Spec] 
// Mode : 0 �۾������� -> ����
// Mode : 1 �۾��Ϸ� -> �ݱ�
/////////////////////////////////////////////////////////////////////////////
VOID CMarcImportDlg::ButtonStatus(INT nMode)
{
	EFS_BEGIN

	const INT nIDCount = 3;
	INT nArrID[nIDCount] = {
		IDC_bMODIFY,
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

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] ���Ϸκ��� ��ũ�� �о�´�.
// [Return Values] ������ ��
// [Mini Spec] 
// 1. DM�˸��ƽ� �������
// 2. ��ũ ���к� ��ũ �׷� ����
// 3. IMPORT/EXPORT MGR �ʱ�ȭ
// 4. Display Thread ���� ��Ϲ�ȣ �ڵ��ο��̸� ��Ϲ�ȣ ����
/////////////////////////////////////////////////////////////////////////////
INT CMarcImportDlg::ImportMarcFromFile()
{
	EFS_BEGIN

	BeginWaitCursor();
	INT ids;

	//=======================================================================
	// 1. DM�˸��ƽ� �������
	//=======================================================================
	CString strHD_Species, strHD_Index, strHD_Book;
	strHD_Species = _T("BS");
	strHD_Book = _T("BB");
	strHD_Index = _T("IBS");

	if (m_strFilePath.IsEmpty()) return -1;

	//=======================================================================
	// 2. ��ũ ���к� ��ũ �׷� ����
	//=======================================================================
	CString sFormCode, sMarcGroup;
	GetControlData( _T("CM_����_��ũ����_�����ڵ�"), strHD_Index + _T("_���ı���_�ڵ�"), sFormCode );
	
	//��ũ����
	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));

	//=======================================================================
	// 3. IMPORT/EXPORT MGR �ʱ�ȭ
	//=======================================================================
	// MarcMgr, FileType, FilePath ����
	// File Type : XLS -- EXCEL ����, TXT -- TEXT ����
	CButton * pChkRegNoAutoGive;
	pChkRegNoAutoGive = (CButton *)GetDlgItem(IDC_ChkGiveAutoRegno);

	m_marcImpExpMgr.Init(this, m_pInfo->pMarcMgr, m_nFileType, m_strFilePath);	
	// 2007. 08.18 ADD BY PDJ
	// ��Ϲ�ȣ �ڵ��ο��� ��� 049 �����ϰ� ���ý� üũ
	if ( pChkRegNoAutoGive->GetCheck() == 1 )
	{
		m_marcImpExpMgr.bRegNoAutoGrant = TRUE;
	}

	// Call Back Func ����
	m_marcImpExpMgr.SetCallBackMarcCheck((INT (CWnd::*)(CMarc *pMarc, CString &str))CallBackCheckMarc);

	// DM ����
	m_marcImpExpMgr.InitDMList();
	m_marcImpExpMgr.AddDMPtr(m_pDM_Species);
	m_marcImpExpMgr.AddDMPtr(m_pDM_Index);

	// ���� üũ ���� DM Alias ����
	m_marcImpExpMgr.SetErrChkAlias( strHD_Species + _T("_���Ϲ��Կ���"));

	// å�� ���� DM Alias ����
	m_marcImpExpMgr.SetBookCountAlias( strHD_Species +_T("_���Ϲ���å��"));

	// Stream Marc ���� DM Alias ����
	m_marcImpExpMgr.SetMarcStreamAlias( strHD_Species +_T("_MARC"));

	// Import ����
	// 2005.07.01 ADD BY 
	// USMARC�� ��� �Ķ���� default 0;
	INT nCheck;
	nCheck = ( (CButton*) GetDlgItem (IDC_rUSMARC) )->GetCheck();		
	ids = m_marcImpExpMgr.ImportFile(_T(""),nCheck);	
	//=======================================================================
	// 4. Display Thread ���� ��Ϲ�ȣ �ڵ��ο��̸� ��Ϲ�ȣ ����
	//=======================================================================
	if (ids >= 0)
	{
		// ���� ����
		// ��Ϲ�ȣ �ڵ��ο� -> 049Tag ����		
		if ( pChkRegNoAutoGive->GetCheck() == 1 )
		{
			ids = Delete049TagFromMarc();
			if ( ids < 0 ) return -1;
		}
		m_pThreadDisplay = AfxBeginThread(ThreadFunc_Display, this);
	}
	else
	{
		// ������ �ʱ�ȭ
		OnbInit();
		return ids;
	}
	
	EndWaitCursor();

	return ids;

	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] ���Ͽ� ��ũ�� ����.
// [Return Values] ������ ��
// [Mini Spec] 
// 1. �׸��� ���� �� �ڷ� �����˻�
// 2. ������ ����(����) stream marc�� �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////
INT CMarcImportDlg::ExportToTxtFile(CString strFilePath)
{
	EFS_BEGIN

	BeginWaitCursor();
	
	// HSS_040524 �������
	CString strHD_Species;
	strHD_Species = _T("BS");

	// Deterime What Grid is Selected
	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl == NULL)
	{
		AfxMessageBox(_T("���� ����Ʈ�� �� �����ϴ�."));
		return -1;
	}
	
	//=======================================================================
	// 1. �׸��� ���� �� �ڷ� �����˻�
	//=======================================================================
	INT nSel = pTabCtrl->GetCurSel();
	// 	CESL_Grid *pGrid = NULL;
	CESL_DataMgr *pDM = NULL;

	// OK GRID
	if (nSel == 0)
	{
// 		pGrid = m_pGrid_OK;
		pDM = m_pDmGridOk;
	}
	// ERROR GRID
	else if (nSel == 1)
	{
// 		pGrid = m_pGrid_ERR;
		pDM = m_pDmGridError;
	}
	
	// SUCCESS GRID
	else if (nSel == 2)
	{
// 		pGrid = m_pGrid_SUCCESS;
		pDM = m_pDmGridSuccess;
	}
	
	
// 	if (pGrid == NULL) return;
	if (pDM == NULL) 
	{
		return 0;
	}
	


//	if (pGrid->GetRows() == 2)
//	{
//		if (pGrid->GetTextMatrix(1, 0).IsEmpty())
//		{
//			AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
//			return;
//		}
//	}
	if (pDM->GetRowCount() < 1)
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return 0;
	}

	//=======================================================================
	// 2. ������ ����(����) stream marc�� �����Ѵ�.
	//=======================================================================
	CStdioFile file;

	//20071115 ADD PJW : FILE OPEN ��� üũ
	INT nConvertMode;
	//nConvertMode = m_MarcConvertApi->m_nConvertModeChk;
	nConvertMode = m_MarcConvertApi.m_nConvertModeChk;
	//20071115 UPDATE PJW :��庰 File Open
	if(nConvertMode == _UTF8_FILE)
	{
		if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		return -1;
		if( 0 == file.GetLength() )
		{			
			//TCHAR cUni = 0xEFBB;//
			CHAR  cMul1;
			CHAR  cMul2;
			CHAR  cMul3;
			BYTE byteMul1 = 0xEF;
			BYTE byteMul2 = 0xBB;
			BYTE byteMul3 = 0xBF;
			
			cMul1 = (CHAR)byteMul1;
			cMul2 = (CHAR)byteMul2;
			cMul3 = (CHAR)byteMul3;
			
			file.Write( &cMul1, sizeof(CHAR));
			file.Write( &cMul2, sizeof(CHAR));
			file.Write( &cMul3, sizeof(CHAR));
		}
	}
	else if(nConvertMode == _UNICODE_FILE)
	{
		if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		return -1;
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}
	}
	else if(nConvertMode == _ANSI_FILE)
	{
		if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeText))
		return -1;
	}

	CString strErrMarc;
	CMarc pMarc;
	CString strIndex;
	INT nNeedByte;
// 	INT nCount = pGrid->GetCount();
	INT nCount = pDM->GetRowCount();
	for (INT i = 0; i < nCount; i++)
	{
		// 041013_HSS �׸��忡 1���� �ѷ��ִ� ��� �ٽ� �ǵ����� ��
		//=========================================================
		//2008.09.11 UPDATE BY PJW : �׸��忡 �÷��� �߰��Ǿ� �ε����� �и�
// 		// INT nTmp = _ttoi(pGrid->GetTextMatrix(i + 1, 0) -1)
//		INT nTmp = _ttoi(pGrid->GetTextMatrix(i + 1, 0));
//		strErrMarc = m_pDM_Species->GetCellData( strHD_Species + _T("_MARC"), nTmp-1 );

		strIndex = pDM->GetCellData(_T("����"), i);
		strErrMarc = m_pDM_Species->GetCellData( strHD_Species + _T("_MARC"), _ttoi(strIndex) -1 );
		//=========================================================
		
		if (strErrMarc.IsEmpty()) continue;
		
		m_MarcConvertApi.GetMarcStruct(strErrMarc);
		strErrMarc.Format(m_MarcConvertApi.m_strUniConvertMarc);
		if(nConvertMode == _UTF8_FILE)
		{				
			// 2008.11.4 ADD BY PDJ : UTF8 Marc Convert
			m_pInfo->pMarcMgr->Decoding(strErrMarc, &pMarc);
			m_pInfo->pMarcMgr->m_nEncodingCharset = CP_UTF8;
			m_pInfo->pMarcMgr->Encoding(&pMarc, strErrMarc);			

			nNeedByte = m_MarcConvertApi.GetLengthUnicodeToUTF8(strErrMarc);
			m_MarcConvertApi.GetStrUniCodeToUtf8(strErrMarc);	
			CHAR cTemp;
			for( INT i = 0; i < nNeedByte-1 ; i++ )
			{
				cTemp = *(m_MarcConvertApi.m_cpConvertData+i);
				file.Write(&cTemp, sizeof(CHAR));
			}
			CString strTemp;
			cTemp = '\r';
			file.Write(&cTemp, sizeof(CHAR));
			cTemp = '\n';
			file.Write(&cTemp, sizeof(CHAR));
		}
		else if(nConvertMode == _UNICODE_FILE)
		{
			// 2008.11.4 ADD BY PDJ : UTF8 Marc Convert
			m_pInfo->pMarcMgr->Decoding(strErrMarc, &pMarc);
			m_pInfo->pMarcMgr->m_nEncodingCharset = -1;
			m_pInfo->pMarcMgr->Encoding(&pMarc, strErrMarc);
			strErrMarc += _T("\r\n");
			file.WriteString(strErrMarc);			
		}
		else if(nConvertMode == _ANSI_FILE)
		{
			// 2008.11.4 ADD BY PDJ : ANSI Marc Convert			
			m_pInfo->pMarcMgr->Decoding(strErrMarc, &pMarc);
			m_pInfo->pMarcMgr->m_nEncodingCharset = CP_ACP;
			m_pInfo->pMarcMgr->Encoding(&pMarc, strErrMarc);

			nNeedByte = m_MarcConvertApi.GetLengthUnicodeToAnsi(strErrMarc);
			m_MarcConvertApi.GetStrUniCodeToAnsi(strErrMarc);
			CHAR cTemp;
			for( INT i = 0; i < nNeedByte-1 ; i++ )
			{
				cTemp = *(m_MarcConvertApi.m_cpConvertData+i);
				file.Write(&cTemp, sizeof(CHAR));
			}
			CString strTemp;
			cTemp = '\r';
			file.Write(&cTemp, sizeof(CHAR));
			cTemp = '\n';
			file.Write(&cTemp, sizeof(CHAR));
		}
	}

	file.Close();

	m_pInfo->pMarcMgr->m_nEncodingCharset = CP_UTF8;

	EndWaitCursor();

	AfxMessageBox(_T("���� �Ϸ�"));

	return 0;

	return -1;

	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] �ڵ�����
// [Return Values] 
// [Mini Spec] 
// �Ѱ��� �ο캰�� �ڵ� ����
/////////////////////////////////////////////////////////////////////////////
VOID CMarcImportDlg::CodeApply()
{
	EFS_BEGIN

	BeginWaitCursor();
	
	INT nRowCount = m_pDM_Index->GetRowCount();
	for (INT row = 0; row < nRowCount; row++)
		CodeApply(row);
	
	EndWaitCursor();


	EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] �ڵ�����
// [Return Values] 
// [Mini Spec] 
// ���ı��� ��ü���п� �˸°� �ڵ带 �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////
VOID CMarcImportDlg::CodeApply(INT nIdxDM)
{
	EFS_BEGIN

	CStringArray arrIgnorAlias;
	CString strHD_Species, strHD_Index;
	CMarc marc;
	CString strStreamMarc;
	CString strFormCode;
	CString strMatCode;
	
	// HSS_040524 �������
	strHD_Species = _T("BS");
	strHD_Index = _T("IBS");

	CESL_ControlMgr *pCM = FindCM(m_lpszCMAlias_Code);
	if (pCM == NULL) return;

	// TODO : ��ũ ���к�~ �ڵ� ����
	INT ids = -1;
	
	arrIgnorAlias.Add( strHD_Index + _T("_���ı���_�ڵ�"));
	arrIgnorAlias.Add( strHD_Index + _T("_��ü����_�ڵ�"));	

	// ���� ����, ��ü������ �˸°� �־��ش�.
	ids = m_pDM_Species->GetCellData( strHD_Species + _T("_MARC"), nIdxDM, strStreamMarc);
	if (ids >= 0 && !strStreamMarc.IsEmpty())
	{
		// ���� ����, ��ü������ �˸°� �־��ش�.
		ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
		if (ids >= 0)
		{
			ids = GetFormCodeMatCode(&marc, strFormCode, strMatCode);
			// 2007.06.13 ADD BY PDJ
			if ( strFormCode == _T("") ) strFormCode = _T("BK");
			if ( strMatCode == _T("") ) strMatCode = _T("PR");
			
			m_pDM_Index->SetCellData( strHD_Index + _T("_���ı���_�ڵ�"), strFormCode, nIdxDM);
			m_pDM_Index->SetCellData( strHD_Index + _T("_��ü����_�ڵ�"), strMatCode, nIdxDM);			
		}
	}
	
	m_pCatApi->MakeDMFromCM(pCM, m_pDM_Species, MNG_INSERT, nIdxDM, strHD_Species, &arrIgnorAlias);
	m_pCatApi->MakeDMFromCM(pCM, m_pDM_Index, MNG_INSERT, nIdxDM, strHD_Index, &arrIgnorAlias);


	//job.2018.0053
	//��Ͽϼ��� ��ũ���Խ� ���������� ���� �ȵǴ� ������� 
	//----------------------------------------------------------------------------------
	CString strWorkCode = "";
	GetControlData(m_lpszCMAlias_Code,_T("BB_��������"),strWorkCode,-1,0);
	m_pDM_Index->SetCellData( strHD_Index + _T("_��������_�ڵ�"), strWorkCode, nIdxDM);	
	//----------------------------------------------------------------------------------
	EFS_END
}


/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] ��ũ�κ��� ���ı���, ��ü���� ����
// [Return Values] ������ ����
// [Mini Spec] 
// 1. �����κ��� ���ı����� ���Ѵ�.
/////////////////////////////////////////////////////////////////////////////
INT CMarcImportDlg::GetFormCodeMatCode(CMarc *pMarc, CString &strFormCode, CString &strMatCode)
{
	EFS_BEGIN

	if (!pMarc) return -1;
	strFormCode.Empty();
	strMatCode.Empty();

	CString strLeader;
	CString strFormCodeMatCode;
	CString strHD_Index;
	strHD_Index = _T("IBS");

	pMarc->GetLeader(strLeader);
	if (strLeader.GetLength() < 7)
	{
		AfxMessageBox(_T("������ ���̰� 7���� �۽��ϴ�."));
		return -1;
	}
	
	//=======================================================================
	// 1. �����κ��� ���ı����� ���Ѵ�.
	//=======================================================================
	CString strRecord = strLeader.Mid(6,1) + strLeader.Mid(7,1);
	// 2007.06.12 ADD BY PDJ
	// �������� ���Խ� �Ǵ� ���ڵ� ���°� NULL�� ��� Default 'a'
	if ( strRecord == _T("  ") ) strRecord = _T("am");

	BOOL bFind = m_mapRecord_FormCodeMatCode.Lookup(strRecord, strFormCodeMatCode);
	if (bFind < 0)
	{
		AfxMessageBox(_T(""));
		return -1;
	}

	// ���ı���, ��ü���� ��������
	INT nFind = strFormCodeMatCode.Find(_T("="));
	if (nFind < 0) return -1;

	CString strGetFormCode = strFormCodeMatCode.Mid(0, nFind);
	CString strGetMatCode = strFormCodeMatCode.Mid(nFind + 1);

	strFormCode = strGetFormCode;
	
	// ������ �� ���� Ÿ���� ���
	nFind = strGetMatCode.Find(_T("|"));
	if (nFind > 0)
	{
		strGetMatCode = strGetMatCode.Mid(0, nFind);

		// ȭ���� ������ ���� �����´�.
		CESL_ControlMgr *pCM = FindCM(m_lpszCMAlias_Code);
		if (pCM == NULL) return -1;

		CString strTemp;
		pCM->GetControlMgrData( strHD_Index + _T("_���ı���_�ڵ�"), strTemp);
		if (strTemp == strGetFormCode)
		{
			pCM->GetControlMgrData( strHD_Index + _T("_��ü����_�ڵ�"), strTemp);
			strMatCode = strTemp;
			return 0;
		}
	}
	
	// ������ ���� �ƴϰ� �����Ҽ� �ִ� Ÿ���� ���
	strMatCode = strGetMatCode;
	
	return 0;

	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] ��ũ �ݹ� �Լ� (��ȿ���˻�)
// [Return Values] ������ ����
// [Mini Spec] 
// 1. �ʼ� �±� ����
// 2. �����˻�
// 3. ��������̸� ��ġ/����(950) ��ȿ���� �˻��Ѵ�. 
// 4. 653 Tag �����˻� : ������ ����
// 5. �ʼ��±� �˻�
// 6. 008 �����˻� : ������ ����
// 7. 005 ���������� ����
/////////////////////////////////////////////////////////////////////////////
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
	
	CStringArray arrStrEssentialTag;

	CButton * pChkWorkStatus;	
	CButton * pChkRegNoAutoGive;

	strCheckErrMsg.Empty();

	//=======================================================================
	// 1. �ʼ� �±� ����
	//=======================================================================
	arrStrEssentialTag.Add(_T("245"));

	//�ڵ��ο� �ο� ��ư
	pChkRegNoAutoGive = (CButton *)GetDlgItem(IDC_ChkGiveAutoRegno);
	if ( pChkRegNoAutoGive->GetCheck() == 0 ) {
		arrStrEssentialTag.Add(_T("049"));
	}

	//������ ���� ��ư
	pChkWorkStatus = (CButton *)GetDlgItem(IDC_ChkWorkStatus);
	if ( pChkWorkStatus->GetCheck() == 0 ) {
		arrStrEssentialTag.Add(_T("008"));

		//arrStrEssentialTag.Add(_T("090"));

		//2004.12.13 ���Թ��� �� �з���ȣ �ݵ�� üũ ���� �ʾƵ� ��
		//�з���ȣ�� 
		//arrStrEssentialTag.Add(_T("056"));
		arrStrEssentialTag.Add(_T("260"));
		arrStrEssentialTag.Add(_T("300"));
	}

	//=======================================================================
	// 2. �����˻�
	//=======================================================================
	CString strLeader;
	pMarc->GetLeader(strLeader);
	TCHAR ch07 = strLeader.GetAt(7);
	if (ch07 == ' ') 
	{
		strLeader.SetAt(7, 'm');
	}
	else 
	{
		if (!(ch07 == 'a' || ch07 == 'c' || ch07 == 'm'))
		{
			strErrMsg = _T("���� ���������� �ƴմϴ�.. �������ּ���");
			arrStrErrMsg.Add(strErrMsg);
		}
	}

	//=======================================================================
	// 3. ��������̸� ��ġ/����(950) ��ȿ���� �˻��Ѵ�. 
	//=======================================================================
	if ( pChkWorkStatus->GetCheck() == 0 ) {
		// ��ġ��ȣ ����˻�
		if ( pChkRegNoAutoGive->GetCheck() == 0 )
		{
			ids = m_pInfo->pMarcMgr->GetItem(pMarc, _T("049$f"), strItemData, &arrItem);
			if (ids >= 0)
			{
				ids = m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("��ġ��ȣ"), strItemData, strItemData);
				if (ids < 0) 
				{
					strErrMsg = _T("��ġ��ȣ ���� �߸�����ϴ�. �������ּ���");
					arrStrErrMsg.Add(strErrMsg);
				}
			}
		}

		// 950$b �� ȭ�󱸺� �ִ��� ���� �˻� & ȭ�󱸺� ����˻�
		ids = m_pInfo->pMarcMgr->GetItem(pMarc, _T("950$b"), strItemData);
		if (ids >= 0)
		{
			// ȭ�󱸺��� �ִ��� �˻�
			strItemData = GetPriceCode(strItemData, strTemp);
			if (strItemData.IsEmpty()) 
			{
				strErrMsg = _T("[950$b] �� ȭ�󱸺��� �����ϴ�. �������ּ���");
				arrStrErrMsg.Add(strErrMsg);
			}

			// ȭ�󱸺��� ��ȿ���� �˻�
			ids = m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("ȭ���ȣ����"), strItemData, strItemData);
			if (ids < 0)
			{
				strErrMsg = _T("[950$b] �� ȭ�󱸺� ���� �߸�����ϴ�. �������ּ���");
				arrStrErrMsg.Add(strErrMsg);
			}
		}

		/*-----------------------------------------------------------------------
		// û����ȣ ���� �˻�
		ids = m_pInfo->pMarcMgr->GetItem(pMarc, _T("090$a"), strItemData);
		if (ids < 0)
		{
			strErrMsg = _T("û����ȣ�� �����ϴ�. �������ּ���");
			arrStrErrMsg.Add(strErrMsg);
		}
		*/
	}

	//=======================================================================
	// 4. 653 Tag �����˻� : ������ ����
	//=======================================================================
	CButton * pChk653Tag;
	pChk653Tag = (CButton *)GetDlgItem(IDC_Create653Tag);
	if ( pChk653Tag->GetCheck() == 1 ) {	
		ids = m_pInfo->pMarcMgr->GetItem(pMarc, _T("653$a"), strItemData);	
		if (ids < 0)
		{
			// TODO : CREATE KEY WORD			
			CString strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc);
			m_pDlgKeyWord->Init(strStreamMarc);
			m_pDlgKeyWord->MakeKeyWord();
			*pMarc = m_pDlgKeyWord->m_marc;		
			
			//arrStrErrMsg.Add(_T("[653] Tag�� �����ϴ�."));	
		}
	}

	//=======================================================================
	// 5. �ʼ��±� �˻�
	//=======================================================================
	nCount = arrStrEssentialTag.GetSize();
	for (idx = 0; idx < nCount; idx++)
	{
		strItem = arrStrEssentialTag.GetAt(idx);
		if (pMarc->FindTag(strItem) == NULL)
		{
			strErrMsg.Format(_T("[%s] Tag�� �Է����ּ���"), strItem);
			arrStrErrMsg.Add(strErrMsg);
		}
	}

	//=======================================================================
	// 6. 008 �����˻� : ������ ����
	//=======================================================================
	ids = m_pInfo->pMarcMgr->GetItem(pMarc, _T("008@0-5"), strItemData);
	CIndex::TrimAll(strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		strItemData = CTime::GetCurrentTime().Format(_T("%y%m%d"));
		m_pInfo->pMarcMgr->SetItem(pMarc, _T("008@0-5"), strItemData);
	}

	//=======================================================================
	// 7. 005 ���������� ����
	//=======================================================================
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

	if (nCount > 0)
		return -1;
	else
		return 0;

	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CMarcImportDlg message handlers

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	100,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

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

		pTabCtrl->SetCurFocus(1);
		pTabCtrl->SetCurFocus(0);
	}

	//��ϱ��� ��Ȱ��ȭ
	GetDlgItem(IDC_cmbREGCODE_CLASS2)->EnableWindow(FALSE);

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

	PostMessage(WM_SIZE);

	( (CButton*) GetDlgItem (IDC_rTXT) )->SetCheck(TRUE);
	m_nFileType = 1;

	DefaultAccessionRecNo();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	return FALSE;


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

		CDialog::OnOK();
	}
	else if (strText == _T("����"))
	{
		m_bIsThreadRunning = FALSE;
	}

	AllControlDisplay(_T("CM_����_����"));

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
	
	// Resize Grapper
	/*if (::IsWindow(m_wndSizeGrapper.GetSafeHwnd()))
	{
		CRect rcClient;
		m_wndSizeGrapper.GetWindowRect(rcClient);
		ScreenToClient(rcClient);

		rcClient.left = rcDlg.right - rcClient.Width();
		rcClient.right = rcDlg.right;
		rcClient.top = rcDlg.bottom - rcClient.Height();
		rcClient.bottom = rcDlg.bottom;

		m_wndSizeGrapper.MoveWindow(rcClient);
	}
	*/


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

	delete m_pDM_Index;
	m_pDM_Index = NULL;

	delete m_pDM_Species;
	m_pDM_Species = NULL;

	delete m_pDM_Book;
	m_pDM_Book = NULL;

	delete m_pDlgKeyWord;
	m_pDlgKeyWord = NULL;

	// 16/05/30 ������ : ��Ͽϼ�>��ũ���� ������ ���Ŀ� �޸𸮸��� �߻��Ͽ� ����
//*/ ADD ------------------------------------------------------------------------
	delete m_pCatApi;
	m_pCatApi = NULL;
//*/ END ------------------------------------------------------------------------

	CDialog::OnDestroy();	


	EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] ���Ͽ��� ��ư �̺�Ʈ
// [Return Values] 
// [Mini Spec] 
// 1. ����Ÿ�� ���� ������ ����  ������ ��ġ�Ѵ�. 
/////////////////////////////////////////////////////////////////////////////
VOID CMarcImportDlg::OnbFILEOPEN() 
{
	EFS_BEGIN

	UpdateData();

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

	if (m_nFileType == 0)
	{
		// 16/03/29 ������ : �Ѽ� �����ϱ�
//*/ ADD -------------------------------------------------------------------------------------
		// 16/04/05 ������ : ��ȣ���� ������ ���� �ӽ��ּ�ó�� -> // 16/04/19 ������ : �ּ�Ǯ��
		if (m_bIsExcel != TRUE) 
		{
			OFN.lpstrFilter = _T("�Ѽ�����(*.cell)\0*.cell\0�������(*.*)\0*.*\0");
			OFN.lpstrTitle = _T("�Ѽ� ���� ����");
		}	
		else
//*/ END -------------------------------------------------------------------------------------
		{
			OFN.lpstrFilter = _T("XLS����(*.xls)\0*.xls\0�������(*.*)\0*.*\0");
			OFN.lpstrTitle = _T("EXCEL ���� ����");
		}
	}
	else if(m_nFileType == 1)
	{
		OFN.lpstrFilter = _T("TXT����(*.txt)\0*.txt\0�������(*.*)\0*.*\0");
		OFN.lpstrTitle = _T("TEXT ���� ����");
	}
	else if( m_nFileType == 5) 
	{
		OFN.lpstrFilter = _T("TXT���� (������ �и��� �ؽ�Ʈ) (*.txt)\0*.txt\0CSV���� (������ �и��� �ؽ�Ʈ) (*.csv)\0*.csv\0������� (������ �и��� �ؽ�Ʈ)(*.*)\0*.*\0");
		OFN.lpstrTitle = _T("CSV ���� ����");
	}

	BOOL bSuccess = GetOpenFileName(&OFN);
	SetCurrentDirectory(szBuffer);
	if (bSuccess)
	{
		m_strFilePath = OFN.lpstrFile;
		UpdateData(FALSE);

		m_wndProgressBar.SetWindowText(_T("���� ��ġ��..."));
		m_wndProgressBar.UpdateWindow();

		INT ids;
		// ���� ���� ����
		ids = ImportMarcFromFile();
		if ( ids ) return;
	}
	else
	{
		return;
	}
	
	if ( _T("") != m_strFilePath )
	{
		//������ ��ư(�����߹���, ���Ͽ���, ��Ϲ�ȣ �ڵ��ο�, ���ı���)�� Disable
		GetDlgItem(IDC_bFILE_OPEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_ChkWorkStatus)->EnableWindow(FALSE);
		GetDlgItem(IDC_ChkGiveAutoRegno)->EnableWindow(FALSE);
		GetDlgItem(IDC_cmbFORM_CLASS)->EnableWindow(FALSE);
		GetDlgItem(IDC_ChkMULTIBOOK)->EnableWindow(FALSE);

		GetDlgItem(IDC_bMODIFY)->EnableWindow(TRUE);
		GetDlgItem(IDC_bEXPORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_bIMPORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_bEXPORT_EDITMARC)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_ChkAccessionRec)->EnableWindow(FALSE);
		GetDlgItem(IDC_eAccessionRecNo)->EnableWindow(FALSE);
	}	

	CheckAccessionRecNo();
	
EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] ��ũ ������ư �̺�Ʈ
// [Return Values] 
// [Mini Spec] 
// 1. �ڵ带 �����Ѵ�. 
// 2. ���º� ��ũ����â�� ����.
/////////////////////////////////////////////////////////////////////////////
//===================================================
//2009.03.11 UPDATE BY PJW : ����ȭ���� �ƴ� ��ũ ����â�� ����.
//VOID CMarcImportDlg::OnbMODIFY() 
//{
//	EFS_BEGIN
//
//	// ��ũ���� ȭ���� ����.
//	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
//	if (pTabCtrl == NULL) return;
//	
//	INT nCurSel = pTabCtrl->GetCurSel();
//
//	CESL_Grid *pGrid = NULL;
//	
//	if (nCurSel == 0)
//	{
//		pGrid = m_pGrid_OK;
//	}
//	else if (nCurSel == 1)
//	{
//		pGrid = m_pGrid_ERR;
//	}
//
//	if (pGrid == NULL) return;
//
//	CString strData = pGrid->GetTextMatrix(pGrid->GetRow(), 0) ;
//	if (strData.IsEmpty()) return;
//
//	UpdateData();
//	
//	//===========================================================================
//	// 1. �ڵ带 �����Ѵ�. 
//	//===========================================================================
//	CodeApply();
//
//	m_MarcAdjLaunCher.SetParentInfo(m_pDM_Species, m_pDM_Index, pGrid, pGrid->GetRow() - 1);
//
//	//===================================================
//	//20080121 ADD PJW : ��ũ���� ȭ���� ��ﶧ �θ���̾�α��� ���±��� �޺��ڽ��� dm�� ���±����� �����.
//	CString strFormCode;
//	INT nDmRowCnt = 0;
//	GetControlData( _T("CM_����_��ũ����_�����ڵ�"), _T("IBS_���±���_�ڵ�"), strFormCode );
//	nDmRowCnt = m_pDM_Species->GetRowCount();
//	for ( INT i = 0 ; i < nDmRowCnt ; i++)
//	{
//		m_pDM_Species->SetCellData( _T("BS_���±���_�ڵ�"), strFormCode, i);
//	}
//	nDmRowCnt = m_pDM_Index->GetRowCount();
//	for (  i = 0 ; i < nDmRowCnt ; i++)
//	{
//		m_pDM_Index->SetCellData( _T("IBS_���±���_�ڵ�"), strFormCode, i);
//	}
//	//===================================================
//	
//
//	//===========================================================================
//	// 2. ���º� ��ũ����â�� ����.
//	//===========================================================================
//	CButton * pChkWorkStatus;
//	pChkWorkStatus = (CButton *)GetDlgItem(IDC_ChkWorkStatus);
//	
//	if ( pChkWorkStatus->GetCheck() == 0 ) //�������
//	{
//		m_MarcAdjLaunCher.LaunchMarcAdjust(this, this, _T(""), MNG_IMPORT, MODE_MONO);	
//	}
//	else //������ ����
//	{	
//		m_MarcAdjLaunCher.LaunchMarcAdjust(this, this, _T(""), MNG_IMPORT_WORKING, MODE_MONO);		
//	}
//
//	//===================================================
//	//2009.03.11 UPDATE BY PJW : �׸��尡 ����Ǹ� DM�� �߰��ϰ� �׸��带 �����Ѵ�.
////	INT nSuccessDmRowCnt;
////	INT nSuccessGridRowCnt;
////	INT nSuccessDmColCnt;
////	INT nChkDm;
////
////	CString strGridData;
////	CString strDmData;
////
////	nSuccessGridRowCnt = m_pGrid_SUCCESS->GetRows();
////	nSuccessDmRowCnt = m_pDmGridSuccess->GetRowCount();
////
////	nSuccessDmColCnt = m_pDmGridSuccess->GetRefColumnCount();
////	
////	nChkDm = nSuccessGridRowCnt - nSuccessDmRowCnt;
////	if( 0 < nChkDm )
////	{
////		for( INT i = 0 ; i < nChkDm ; i++ )
////		{
////			strGridData = m_pGrid_SUCCESS->GetTextMatrix( nSuccessDmRowCnt + i + 1, 0 );
////			
////			for( INT j = 0 ; j < m_pDmGridError->GetRowCount() ; j++ ) 
////			{
////				strDmData = m_pDmGridError->GetCellData(_T("����"), j);
////				if( strGridData == strDmData )
////				{
////					m_pDmGridSuccess->InsertRow(-1);
////					for( INT n = 0 ; n < nSuccessDmColCnt ; n++ )
////					{
////						strDmData = m_pDmGridError->GetCellData( j, n );
////						m_pDmGridSuccess->SetCellData(  nSuccessDmRowCnt + i , n,  strDmData);
////
////					}
////					m_pDmGridError->DeleteRow(j);
////					
////					break;
////				}
////			}
////		}
////
////	}
//	//===================================================
//
//	return;
//	EFS_END
//}
VOID CMarcImportDlg::OnbMODIFY() 
{
	EFS_BEGIN

	// ��ũ���� ȭ���� ����.
	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl == NULL) return;
	
	INT nCurSel = pTabCtrl->GetCurSel();

	CESL_Grid *pGrid = NULL;
	CESL_DataMgr *pDm = NULL;
	
	if (nCurSel == 0)
	{
		pGrid = m_pGrid_OK;
		pDm   = m_pDmGridOk;
	}
	else if (nCurSel == 1)
	{
		pGrid = m_pGrid_ERR;
		pDm   = m_pDmGridError;
	}

	if (pGrid == NULL || pDm == NULL)
	{
		return;
	}

	pGrid->SelectMakeList();
	INT nSelectCnt;
	nSelectCnt = pGrid->SelectGetCount();
	if( 1 > nSelectCnt )
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}

//	CString strData = pGrid->GetTextMatrix(pGrid->GetRow(), 0) ;
//	if (strData.IsEmpty()) return;

	UpdateData();

	// ��ũ ���� ȭ���� �����ش�.
	CMarcEditDlg dlg(this);
	dlg.SetMarcImpExpMgr(&m_marcImpExpMgr);
	INT nFileType = 0;
	INT nChkMultiBook;
	INT nChkGiveAutoRegno;
	INT nChkWorkStatus;

	CButton * pChkGiveAutoRegno = (CButton *)GetDlgItem(IDC_ChkGiveAutoRegno);
	nChkGiveAutoRegno = pChkGiveAutoRegno->GetCheck();

	CButton * pChkMultiBook = (CButton *)GetDlgItem(IDC_ChkMULTIBOOK);
	nChkMultiBook = pChkMultiBook->GetCheck();

	CButton * pChkWorkStatus = (CButton *)GetDlgItem(IDC_ChkMULTIBOOK);
	nChkWorkStatus = pChkWorkStatus->GetCheck();

	 

	dlg.SetParentInfo(m_pDM_Species, pDm, pGrid, _T("BS_MARC"), _T("BS_���Ϲ��Կ���")
		              ,0, nChkMultiBook, nChkGiveAutoRegno, nChkWorkStatus);

	pGrid->SetProtectSelectColumn(TRUE);
	dlg.DoModal();
	pGrid->Display();
	INT nIdx;
	CString strData;
	nIdx = pGrid->SelectGetHeadPosition();
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		strData = pDm->GetCellData(_T("����"), nIdx);
		pGrid->SetTextMatrix(nIdx+1, 0,strData);
		nIdx = pGrid->SelectGetNext();		
	}
	pGrid->SetProtectSelectColumn(FALSE);

	return;
	EFS_END
}
//===================================================



/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] �����ڷ� ���� ��ư �̺�Ʈ 
// [Return Values] 
// [Mini Spec] 
// 1. �ڵ带 �����Ѵ�. 
// 2. ���� �����带 ������.
/////////////////////////////////////////////////////////////////////////////
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

	//===================================================
	//2009.03.17 REM BY PJW : DM�� �̿������� �ʿ������ �ڵ�
//	if (m_pGrid_OK->GetTextMatrix(m_pGrid_OK->SelectGetHeadPosition() + 1, 0).IsEmpty())
//	{   
//		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
//		return;
//	}
	//===================================================
	
	UpdateData();
	//=======================================================================
	// 1. �ڵ带 �����Ѵ�. 
	//=======================================================================
	CodeApply();

	//=======================================================================
	// 2. ���� ����
	//=======================================================================
	//=====================================================
	//2009.01.05 UPDATE BY PJW : üũ�Ǿ�������츸 ���θ� �����Ѵ�.
	//SetAccessionRecNoApply();
	CButton* pBtn = (CButton*) GetDlgItem(IDC_ChkAccessionRec);
	if( pBtn->GetCheck() )
	{
		SetAccessionRecNoApply();
	}
	//=====================================================

	//=======================================================================
	// 3. ���� �����带 ������.
	//=======================================================================
	m_pThreadImport = AfxBeginThread(ThreadFunc_Import, this);

	EFS_END
}


/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] �ڷ���� �̺�Ʈ
// [Return Values] 
// [Mini Spec] 
// Stream Marc �� TXT���Ϸ� �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////
VOID CMarcImportDlg::OnbEXPORT() 
{
	EFS_BEGIN

	INT nExportFileType;
	BOOL bCheck;
	INT ids;
	//20071208 ADD PJW : ������ ���� ��� ����
	// =============================================================
	// 1. ��ũ ���� Ÿ�� ����
	// =============================================================
	// 1.1 ansi ����
	bCheck = ( (CButton*) GetDlgItem (IDC_rANSI) )->GetCheck();
	if ( bCheck ) nExportFileType = _ANSI_FILE;

	// 1.2 utf-8 ����
	bCheck = ( (CButton*) GetDlgItem (IDC_rUTF8) )->GetCheck();
	if ( bCheck ) nExportFileType = _UTF8_FILE;

	// 1.3 unicode ����
	bCheck = ( (CButton*) GetDlgItem (IDC_rUNICODE) )->GetCheck();
	if ( bCheck ) nExportFileType = _UNICODE_FILE;
	
	// 1.3 MarcConvert���� ��� ���� 
	ids = m_MarcConvertApi.SetConvertMode(nExportFileType);

	// Deterime What Grid is Selected
	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl == NULL) return;

	INT nSel = pTabCtrl->GetCurSel();
	// 	CESL_Grid *pGrid = NULL;
	CESL_DataMgr *pDM = NULL;

	// OK GRID
	if (nSel == 0)
	{
// 		pGrid = m_pGrid_OK;
		pDM = m_pDmGridOk;
	}
	// ERROR GRID
	else if (nSel == 1)
	{
// 		pGrid = m_pGrid_ERR;
		pDM = m_pDmGridError;
	}
	
	// SUCCESS GRID
	else if (nSel == 2)
	{
// 		pGrid = m_pGrid_SUCCESS;
		pDM = m_pDmGridSuccess;
	}
	
	
// 	if (pGrid == NULL) return;
	if (pDM == NULL) 
	{
		return;
	}
	


//	if (pGrid->GetRows() == 2)
//	{
//		if (pGrid->GetTextMatrix(1, 0).IsEmpty())
//		{
//			AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
//			return;
//		}
//	}
	if (pDM->GetRowCount() < 1)
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
			return;
	}

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

	ids = GetSaveFileName(&OFN);
	SetCurrentDirectory(szBuffer);

	if (ids != 0)
	{
		strFilePath= OFN.lpstrFile;
		ExportToTxtFile(strFilePath);
	}

	EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] ��ũ����Ʈ���� �̺�Ʈ
// [Return Values] 
// [Mini Spec] 
// ��ũ�� ����Ʈ��ũ �������� �����Ѵ�. 
/////////////////////////////////////////////////////////////////////////////
VOID CMarcImportDlg::OnbEXPORTEDITMARC() 
{
	EFS_BEGIN

	// HSS_040524 �������
	CString strHD_Species;
	strHD_Species = _T("BS");

	// Marc Editor�� �־��ٰ� ����.
	CMarcEditCtrl MarcEditCtrl;
	MarcEditCtrl.Create(_T(""), _T(""), WS_CHILD | ES_MULTILINE, CRect(0,0,0,0), this, 10202);

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

	// =============================================================
	// 1. ��ũ ���� Ÿ�� ����
	// =============================================================
	// 1.1 ansi ����
	BOOL bCheck;
	INT	nExportFileType;
	bCheck = ( (CButton*) GetDlgItem (IDC_rANSI) )->GetCheck();
	if ( bCheck ) nExportFileType = _ANSI_FILE;

	// 1.2 utf-8 ����
	bCheck = ( (CButton*) GetDlgItem (IDC_rUTF8) )->GetCheck();
	if ( bCheck ) nExportFileType = _UTF8_FILE;

	// 1.3 unicode ����
	bCheck = ( (CButton*) GetDlgItem (IDC_rUNICODE) )->GetCheck();
	if ( bCheck ) nExportFileType = _UNICODE_FILE;
	
	// 1.3 MarcConvert���� ��� ���� 
	ids = m_MarcConvertApi.SetConvertMode(nExportFileType);

	//20071115 ADD PJW : FILE OPEN ��� üũ
	INT nConvertMode;
	//nConvertMode = m_MarcConvertApi->m_nConvertModeChk;
	nConvertMode = m_MarcConvertApi.m_nConvertModeChk;
	//20071115 UPDATE PJW :��庰 File Open	
	if(nConvertMode == _ANSI_FILE)
	{
		if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeText))
		{
			AfxMessageBox(_T("���� ���� ����"));
			return ;
		}
	}
	else if(nConvertMode == _UTF8_FILE)
	{
		if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		{
			AfxMessageBox(_T("���� ���� ����"));
			return ;
		}
		if( 0 == file.GetLength() )
		{			
			//TCHAR cUni = 0xEFBB;//
			CHAR  cMul1;
			CHAR  cMul2;
			CHAR  cMul3;
			BYTE byteMul1 = 0xEF;
			BYTE byteMul2 = 0xBB;
			BYTE byteMul3 = 0xBF;
			
			cMul1 = (CHAR)byteMul1;
			cMul2 = (CHAR)byteMul2;
			cMul3 = (CHAR)byteMul3;
			
			file.Write( &cMul1, sizeof(CHAR));
			file.Write( &cMul2, sizeof(CHAR));
			file.Write( &cMul3, sizeof(CHAR));
		}
	}
	else //if(nConvertMode == _UNICODE_FILE)
	{
		if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		{
			AfxMessageBox(_T("���� ���� ����"));
			return ;
		}
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}
	}
	
		
	CMarc marc;
	CString strStreamMarc;
	CString strEditMarc;
	CString strTmp;
	INT nCount = m_pDM_Species->GetRowCount();
	INT nLen;
	CHAR cTemp;
	for (INT i = 0; i < nCount; i++)
	{
		strStreamMarc = m_pDM_Species->GetCellData( strHD_Species + _T("_MARC"), i);
		if (strStreamMarc.IsEmpty()) continue;

		ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
		if (ids < 0) continue;

		// MARC�� �����ͼ� Edit Ctrl�� �������� Text�� �����´�.
		MarcEditCtrl.Init(m_pInfo->pMarcMgr, &marc);
		MarcEditCtrl.Display();
		
		MarcEditCtrl.GetWindowText(strEditMarc);

		if(nConvertMode == _UTF8_FILE)
		{
			CHAR	*szpEditMarc;			
			m_cvt.UnicodeToUTF8(strEditMarc.GetBuffer(0), &szpEditMarc, &nLen);			
			file.Write(szpEditMarc, nLen);
			cTemp = '\r';
			file.Write(&cTemp, sizeof(CHAR));
			cTemp = '\n';
			file.Write(&cTemp, sizeof(CHAR));
			free(szpEditMarc);
		}
		else if(nConvertMode == _ANSI_FILE)
		{
			CHAR	*szpEditMarc;			
			m_cvt.UnicodeToMultiByte(strEditMarc.GetBuffer(0), &szpEditMarc, &nLen);			
			file.Write(szpEditMarc, nLen);
			cTemp = '\r';
			file.Write(&cTemp, sizeof(CHAR));
			cTemp = '\n';
			file.Write(&cTemp, sizeof(CHAR));
			free(szpEditMarc);
		}
		else // if(nConvertMode == _UNICODE_FILE)
		{
			strEditMarc += _T("\r\n");
			file.WriteString(strEditMarc);		
		}		
	}

	file.Close();

	EndWaitCursor();

	AfxMessageBox(_T("���� �Ϸ�"));

	EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] ��ũ������ ������ �� �̺�Ʈ �޽���
// [Return Values] ������ ����
// [Mini Spec] 
// ������ �ڷ���� ���������� �ڷḦ �Ѱ��ش�. 
/////////////////////////////////////////////////////////////////////////////
UINT CMarcImportDlg::OnEndMarcAdjust(WPARAM wParam, LPARAM lParam)
{
	EFS_BEGIN

	// HSS_040524 �������
	CString strHD_Species;
	strHD_Species = _T("BS");

	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl == NULL) return -1;
	
	CESL_Grid *pGrid = NULL;

	INT nID_FROM = -1;
	CString strFROM;
	CString strData;

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
	
	INT nRowCount = pGrid->GetCount();
	INT nColCount = pGrid->GetCols(0);

	for (INT idx = nRowCount - 1; idx >= 0; idx--)
	{
		// ���� ����
		if (m_pDM_Species->GetCellData( strHD_Species + _T("_���Ϲ��Լ���"), _ttoi(pGrid->GetTextMatrix(idx + 1, 0))-1) == _T("Y"))
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
	ON_EVENT(CMarcImportDlg, IDC_MSHFLEXGRID_ERR, -605 /* MouseDown */, OnMouseDownMshflexgridErr, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMarcImportDlg, IDC_MSHFLEXGRID_OK, -605 /* MouseDown */, OnMouseDownMshflexgridOk, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMarcImportDlg::OnMouseDownMshflexgridOk(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	INT i = m_pGrid_OK->GetMouseRow();
	if (i == 0) return;
	i = m_pGrid_OK->GetFixedRows();
	i = m_pGrid_OK->GetRow();
	//m_pGrid_OK->SetReverse(m_pGrid_OK->GetRow() - 1);


	EFS_END
}

VOID CMarcImportDlg::OnMouseDownMshflexgridErr(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	//m_pGrid_ERR->SetReverse(m_pGrid_ERR->GetRow() - 1);


	EFS_END
}

VOID CMarcImportDlg::OnChkGiveAutoRegno() 
{
	// TODO: Add your control notification handler code here
	CButton * pChkGiveAutoRegno;
	pChkGiveAutoRegno = (CButton *)GetDlgItem(IDC_ChkGiveAutoRegno);
	if ( pChkGiveAutoRegno->GetCheck() == 1 ) {
		GetDlgItem(IDC_cmbREGCODE_CLASS2)->EnableWindow(TRUE);
		GetDlgItem(IDC_ChkMULTIBOOK)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_cmbREGCODE_CLASS2)->EnableWindow(FALSE);
		GetDlgItem(IDC_ChkMULTIBOOK)->EnableWindow(FALSE);
	}
}

VOID CMarcImportDlg::OnSelchangecmbMEDIACLASS() 
{
	//��ü���� �����������

	CString strHD_Index;
	strHD_Index = _T("IBS");
	

	CESL_ControlMgr *pCM = FindCM(m_lpszCMAlias_Code);
	
	CString sMatCode;
	pCM->GetControlMgrData( strHD_Index + _T("_��ü����_�ڵ�") , sMatCode );

	CButton * pChkWorkStatus;	
	pChkWorkStatus = (CButton *)GetDlgItem(IDC_ChkWorkStatus);

	if ( sMatCode == _T("OF") )
	{
		if ( pChkWorkStatus->GetCheck() == 1 )
		{
			AfxMessageBox(_T("��ü������ OnlineBook(OF)�� ��쿡�� ������ ������ �� �� �����ϴ�.\n�������� ���Ե˴ϴ�."));

			CButton * pChkWorkStatus;	
			pChkWorkStatus = (CButton *)GetDlgItem(IDC_ChkWorkStatus);
			pChkWorkStatus->SetCheck(0);
		}
		GetDlgItem(IDC_ChkWorkStatus)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_ChkWorkStatus)->EnableWindow(TRUE);
	}

}

BOOL CMarcImportDlg::SetConfigAutoRegNo( CESL_DataMgr *DM_Book )
{
EFS_BEGIN
	//�ڵ���ȣ �ο��� ������ �׸� ����
	INT idx = 1;
	if ( idx < 0 ) return FALSE;
	
	// ��dm ��������
	// KOL.UDF.022
	CMarcImportDlg *pDlg = (CMarcImportDlg*)this;
	if (&pDlg == NULL)
	{
		pDlg->m_bIsThreadRunning = FALSE;
		return FALSE;
	}
	CESL_DataMgr *pDM_Species = NULL;
	pDM_Species = pDlg->m_pDM_Species;
	if (pDM_Species == NULL)
	{
		return FALSE;
	}	

	CESL_DataMgr *pDM_Book = NULL;
	pDM_Book = pDlg->m_pDM_Book;
	if (pDM_Book == NULL)
	{
		return FALSE;
	}	

	// HSS_040524 �������
	CString strHD_Species, strHD_Book;
	strHD_Species = _T("BS");
	strHD_Book = _T("BB");


	INT ids = -1;
	// ��ϱ��� ��������
	CString sRegCode;
	GetControlData( _T("CM_����_��ũ����_�����ڵ�"), strHD_Book + _T("_��ϱ���"), sRegCode );

	// ������ ��ȣ ��������
	CString sQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'REG_NO' AND MANAGE_CODE = UDF_MANAGE_CODE AND DIVISION_VALUE = '") + sRegCode + _T("'");
	CString sLastNum;	
	pDlg->m_pDM_Species->GetOneValueQuery( sQuery , sLastNum );
	INT iLastNum = _ttoi ( sLastNum );


	//��ϱ����� ������ �����߰�
	if ( sLastNum == _T("") ) {
		iLastNum = 0;

		ids = pDM_Species->StartFrame();
		if ( 0 > ids ) return FALSE;
		CString sRecKey;
		ids = pDM_Species->MakeRecKey( sRecKey );
		if ( 0 > ids ) return FALSE;
		CString sQuery_reg;
		CTime time = CTime::GetCurrentTime();
		CString sYear;
		sYear.Format( _T("%04d"), time.GetYear() );
		sQuery_reg.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ( REC_KEY , PUBLISH_FORM_CODE , KIND_CODE , DIVISION_VALUE , MAKE_YEAR , LAST_NUMBER, MANAGE_CODE) ")
						  _T("VALUES ( '%s' , '%s' , '%s' , '%s' , '%s' , '%s', UDF_MANAGE_CODE );") 
//DEL 		sQuery_reg.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ( REC_KEY , PUBLISH_FORM_CODE , KIND_CODE , DIVISION_VALUE , MAKE_YEAR , LAST_NUMBER) ")
//DEL 						  _T("VALUES ( '%s' , '%s' , '%s' , '%s' , '%s' , '%s' );") 
						  , sRecKey , _T("CO") , _T("REG_NO") , sRegCode , sYear , _T("0"));
		ids = pDM_Species->AddFrame( sQuery_reg );
		if ( 0 > ids ) return FALSE;

		ids = pDM_Species->SendFrame();			
		if (ids < 0) {
			AfxMessageBox(_T("�Է»����� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�(DM_REG)."));
			return FALSE;
		}
		ids = pDM_Species->EndFrame();
		if ( 0 > ids ) return FALSE;
	}



	CESL_DataMgr *pDmLastNum = FindDM( _T("DM_��������ȣ") );
	if (pDmLastNum == NULL) {
		AfxMessageBox( _T("DM_��������ȣ Ȯ��") );
		return FALSE;
	}
	pDmLastNum->FreeData();
	pDmLastNum->InsertRow(-1);
	pDmLastNum->SetCellData( strHD_Book + _T("_��ϱ���_�ڵ�"), sRegCode, 0 );


	//����

	CMarc marc;
	CString sMarc = pDlg->m_pDM_Species->GetCellData( strHD_Species + _T("_MARC"), idx );
	m_pInfo->pMarcMgr->Decoding(sMarc, &marc);
	CArray<CString, CString&> pArr049l;
	CString strItemData;
	m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &pArr049l);


	//������ �ڷ��� å���� �ϵ��� ����
	//ȫ��

	INT iBookCount = pDM_Book->GetRowCount();

	for ( INT j = 0 ; j < iBookCount ; j++ ) {
		iLastNum++;
		CString sRegNo;
		//=====================================================
		//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 		sRegNo.Format( _T("%s%010d"), sRegCode, iLastNum );
		CString strRegNumTemp;
		strRegNumTemp.Format(_T("%d"),iLastNum);
		sRegNo = pDlg->m_pInfo->MakeRegNo(  sRegCode, strRegNumTemp );
		//=====================================================
		

		sLastNum.Format( _T("%d"), iLastNum );
		pDmLastNum->SetCellData( _T("��������ȣ"), sLastNum, 0 );
		
		CString sItem;
		if ( pArr049l.GetSize() == iBookCount ) {
			sItem = pArr049l.GetAt(j);
			m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo, pArr049l.GetAt(j) );
		}
		else {
			m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo );
		}
		m_pInfo->pMarcMgr->Encoding( &marc, sMarc );
		pDlg->m_pDM_Species->SetCellData( strHD_Species + _T("_MARC"), sMarc, idx );
	}

	return TRUE;


EFS_END
return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] ����� DM�� �������ش�. 
// [Return Values] ������ ����
// [Mini Spec] 
// 1. ����/�� DM�� �����Ѵ�.
// 2. ��ũ�� ���� å������ �����Ѵ�. 
/////////////////////////////////////////////////////////////////////////////
INT CMarcImportDlg::MakeMarcAdjustDM(	CESL_DataMgr *pDM_From_Species, 
										INT nFromSpeciesRow, 
										CESL_DataMgr *pDM_From_Index, 
										INT nFromIndexRow, 
										CESL_DataMgr *pDM_To_Species, 
										CESL_DataMgr *pDM_To_Index, 
										CESL_DataMgr *pDM_To_Book, 
										CESL_DataMgr *pDM_To_Volume)
{
	EFS_BEGIN

	if (!pDM_From_Species ||
		!pDM_From_Index ||
		!pDM_To_Index ||
		!pDM_To_Species ||
		!pDM_To_Book ||
		!pDM_To_Volume ||
		nFromSpeciesRow < 0 ||
		nFromIndexRow < 0) return -1;

	// ������ �ʱ�ȭ
	pDM_To_Species->FreeData();
	pDM_To_Index->FreeData();
	pDM_To_Book->FreeData();
	pDM_To_Volume->FreeData();

	pDM_To_Species->InsertRow(-1);
	pDM_To_Index->InsertRow(-1);
	pDM_To_Book->InsertRow(-1);
	pDM_To_Volume->InsertRow(-1);

	INT ids = -1;
	
	//=======================================================================
	// 1. ����/�� DM�� �����Ѵ�.
	//=======================================================================
	ids = CopyDM(pDM_From_Species, nFromSpeciesRow, pDM_To_Species, 0);
	if( ids < 0 ) 
	{
		AfxMessageBox(_T("��DM������ ����"));
		return -1;
	}
	
	ids = CopyDM(pDM_From_Index, nFromIndexRow, pDM_To_Index, 0);
	if( ids < 0 ) 
	{
		AfxMessageBox(_T("����DM������ ����"));
		return -1;
	}

	// ��ũ��ũ�� ���߱� ���� 049�� �ش��ϴ� ������ å DM�� �ִ´�...
	CMarc marc;
	CString strStreamMarc = pDM_To_Species->GetCellData( _T("BS_MARC"), 0);
	m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);

	CString strRegNo;
	CString strItemData;
	CArray<CString, CString&> arrItem;
	m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &arrItem);
	INT nRowCount = arrItem.GetSize();


	//=======================================================================
	// 2. ��ũ�� ���� å������ �����Ѵ�. 
	//=======================================================================
	// ��Ϲ�ȣ �ڵ��ο� üũ��ư , �ٱǹ��� üũ��ư
	CButton * pChkRegNoAutoGive;
	CButton * pChkMultiBook;
	pChkRegNoAutoGive = (CButton *)GetDlgItem(IDC_ChkGiveAutoRegno);
	pChkMultiBook = (CButton *)GetDlgItem(IDC_ChkMULTIBOOK);

	// 049 Tag�� ������ �ٽ� �����.
	if (nRowCount >= 0) 
	{	
		pDM_To_Book->FreeData();
		
		INT nBookCnt = 1;
		// 049 Tag��� �ڵ��ο��̸� 1å�� �����.(���� ����)
		if ( 0 == nRowCount && pChkRegNoAutoGive->GetCheck() == 1) 
		{
			//nRowCount = 1;
			if( pChkMultiBook->GetCheck() == 1 )
			{
				nBookCnt = _ttoi( pDM_To_Species->GetCellData(_T("UDF_å��"),0));
				if( nBookCnt > 0 )
					nRowCount = nBookCnt;
				else
					nRowCount = 1;
			}
			else
				nRowCount = 1;
		}
	}

	// 049 Tag�� ������ŭ å�� �����.
	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		pDM_To_Book->InsertRow(-1);

		// ��Ϲ�ȣ ����(�ڵ��ο� �ƴϸ�)
		if ( pChkRegNoAutoGive->GetCheck() == 0 )
		{
			strRegNo = arrItem.GetAt(idx);
			pDM_To_Book->SetCellData(_T("BB_��Ϲ�ȣ"), strRegNo, idx);

			//��ϱ��� ����
			CString sRegCode;
				//=====================================================
			//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 			if ( strRegNo.GetLength() >= 2 ) {
// 				sRegCode = strRegNo.Left(2);
// 				pDM_To_Book->SetCellData(_T("BB_��ϱ���_�ڵ�"), sRegCode, idx);
// 			}

			if ( strRegNo.GetLength() >= nRegCodeLength ) {
				sRegCode = strRegNo.Left(nRegCodeLength);
				pDM_To_Book->SetCellData(_T("BB_��ϱ���_�ڵ�"), sRegCode, idx);
			}
			//=====================================================

			pDM_To_Book->SetCellData(_T("BB_��ϱ���_�ڵ�"), sRegCode, idx);
			
		}

		// å�η� �÷���
		pDM_To_Book->SetCellData(_T("BB_å�η��÷���"), _T("B"), idx);

		// DB FLAG ����
		pDM_To_Book->SetCellData(_T("BB_DB_FLAG"), _T("I"), idx);

		// �� -> å
		ids = CopyDM(pDM_To_Species, 0, pDM_To_Book, idx, _T("BB"));
		if( ids < 0 )
		{
			AfxMessageBox(_T("åDM ���� ����"));
			return -1;
		}
		// ���� -> å
		ids = CopyDM(pDM_To_Index, 0, pDM_To_Book, idx, _T("BB"));
		if( ids < 0 )
		{
			AfxMessageBox(_T("åDM ���� ����"));
			return -1;
		}
	}

	// ��ǥå ����
	INT nRepBookIdx = SetRepresentationBook(pDM_To_Book, _T("BB"));

	// ��ũ�� ��ũ����
	m_pCatApi->ConvertMarcToCatDM(&marc, pDM_To_Species, pDM_To_Index, pDM_To_Book, pDM_To_Volume);

	return 0;

	EFS_END
	return -1;

}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc] ���� ������ DB ����
// [Return Values] ������ ����
// [Mini Spec] 
// 1. ��Ϲ�ȣ �ߺ� ����
// 2. �谡��ġ ����
// 3. ������ ��ȣ ��������
// 4. ������������ ���� 040
// 5. LOC_FLAG �ֱ�
// 6. DB�� �����Ѵ�
// 7. 001 TAG ����
// 8. ������ �谡���ڸ� ����Ѵ�.
/////////////////////////////////////////////////////////////////////////////
INT CMarcImportDlg::ImportSpecies( CESL_DataMgr *pDM_Species, 
								  CESL_DataMgr *pDM_Index, 
								  CESL_DataMgr *pDM_Book, 
								  CESL_DataMgr *pDM_Volume, 
								  CESL_DataMgr *pDM_Appendix, 
								  CStringArray *pArrStrAddFrame, 
								  LPCTSTR lpszShelfDate)
{
	EFS_BEGIN

	if (!pDM_Species ||
		!pDM_Index ||
		!pDM_Book ) return -1;

	INT ids = -1;
	INT idx = -1;
	INT nRowCount = -1;

	ids = pDM_Species->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
		return -1;
	}

	CString strRegNo;
	CString strQuery;
	CString strResult;
	CString strTemp;
	
	//��������ȣ üũ �ϱ� ����
	CESL_DataMgr *DM_LastNumber = NULL;

	//=======================================================================
	// 1. ��Ϲ�ȣ �ߺ� ����
	//=======================================================================
	DM_LastNumber = FindDM( _T("DM_��ũ����_��������ȣ") );
	
	CString sLastNumberQuery = _T("") ;
	sLastNumberQuery.Format(_T("SELECT DIVISION_VALUE, LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'REG_NO' AND MANAGE_CODE = UDF_MANAGE_CODE"));
	DM_LastNumber->RestructDataManager(sLastNumberQuery);

	nRowCount = pDM_Book->GetRowCount();


	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	for (idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("BB"))) continue;
		
		ids = pDM_Book->GetCellData( _T("BB_��Ϲ�ȣ"), idx, strRegNo);
		if (ids < 0 || strRegNo.IsEmpty()) continue;

		//��Ϲ�ȣ 12�ڸ� �����
		//=====================================================
		//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 		if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
// 			if ( strRegNo.GetLength() > 2 ) {
// 				CString sRight;
// 				sRight.Format( _T("%010s") , strRegNo.Mid(2) );
// 				strRegNo = strRegNo.Left(2) + sRight;
// 			}
// 		}

		if ( strRegNo.GetLength() > nRegCodeLength && strRegNo.GetLength() < 12 ) 
		{
			strRegNo = pParentMgr->m_pInfo->MakeRegNo( strRegNo.Left(nRegCodeLength) , strRegNo.Mid(nRegCodeLength) );
		}
		//=====================================================
		

		//��Ϲ�ȣ �ֱ�
		pDM_Book->SetCellData( _T("BB_��Ϲ�ȣ"), strRegNo, idx );

		strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE REG_NO = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), pDM_Book->TBL_NAME, strRegNo);
		ids = pDM_Species->GetOneValueQuery(strQuery, strResult);

		if (ids >= 0 && !strResult.IsEmpty())
		{
			//��Ϲ�ȣ �ߺ�����
			m_strLastError.Format(_T("��Ϲ�ȣ�� �ߺ��˴ϴ�. [%s]"), strRegNo);
			return -1;
		}
	}
	
	CMarc marc;
	CString strStreamMarc = pDM_Species->GetCellData( _T("BS_MARC"), 0);
	ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
	if (ids < 0) return -1;

	//=======================================================================
	// 2. �谡��ġ ����
	//=======================================================================
	ids = ((CCatApi_BO*)m_pCatApi)->ApplyShelfCodeData(pDM_Book, &marc, FALSE);
	if (ids < 0) return -1;


	
	//=======================================================================
	// 3. ������ ��ȣ ��������
	//=======================================================================
	CString strLibCode;
	m_pCatApi->GetLibInfo(&CString(_T("LIB_CODE")), &strLibCode, 1);
	if ( strLibCode.IsEmpty() )
	{
		m_strLastError.Format(_T("������ ��ȣ�� �����ϴ�. �������ּ���"));
		return -1;
	}

	//=======================================================================
	// 4. ������������ ���� 040
	//=======================================================================
	m_pCatApi->ApplyLastModifyData(&marc);

	// ��ũ �����Ȱ��� �����Ѵ�.
	ids = m_pInfo->pMarcMgr->Encoding(&marc, strStreamMarc);
	ids = pDM_Species->SetCellData( _T("BS_MARC"), strStreamMarc, 0);
	ids = pDM_Species->SetCellData( _T("BS_�������ڷ�"), _T("Y"), 0 );
	
	//=======================================================================
	// 5. LOC_FLAG �ֱ�
	//=======================================================================
	ids = pDM_Species->SetCellData( _T("BS_�۾����±���"), _T("1"), 0 );

	//=======================================================================
	// 6. DB�� �����Ѵ�. 
	//=======================================================================
	ids = InsertSpecies(pDM_Species, pDM_Index, pDM_Book, pDM_Volume, pDM_Appendix, pArrStrAddFrame, FALSE);

	if (ids < 0) return -1;

	//=======================================================================
	// 7. 001 TAG ����
	//=======================================================================
	ids = m_pCatApi->CompleteCataloging(pDM_Species, pDM_Species, 0, pDM_Book, FALSE);
	if (ids < 0) return -1;

	CString strShelfDate;
	
	//=======================================================================
	// 8. ������ �谡���ڸ� ����Ѵ�.
	//=======================================================================
	if (lpszShelfDate == NULL)
	{
		// ���� ȫ�� ����
		// strShelfDate = _T("1200/01/01");
		strShelfDate = CTime::GetCurrentTime().Format(_T("%Y/%m/%d"));
	}
	else
		strShelfDate = lpszShelfDate;		

	CStringArray arrSpeciesKey;
	CStringArray arrBookKey;
	CStringArray arrAppendixKey;

	arrSpeciesKey.Add(pDM_Species->GetCellData( _T("BS_��KEY"), 0));
	
	nRowCount = pDM_Book->GetRowCount();
	for (idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("BB"))) continue;

		arrBookKey.Add(pDM_Book->GetCellData( _T("BB_åKEY"), idx));
	}
	if(pDM_Appendix)
	{
		nRowCount = pDM_Appendix->GetRowCount();
		for (idx = 0; idx < nRowCount; idx++)
		{
			arrAppendixKey.Add(pDM_Appendix->GetCellData( _T("BB_åKEY"), idx));
		}
	}

	ids = m_pCatApi->RecordLocDate(pDM_Species, &arrSpeciesKey, &arrBookKey, &arrAppendixKey, strShelfDate, FALSE);
	if (ids < 0) return -1;

	//=====================================================
	//2008.12.29 ADD BY PJW : Ȩü���� IDX_BO_TBL�� �߰��� ������ SENDFRAME�ٷ� ������ ����.
	CString strSpeciesKey;
	pDM_Species->GetCellData(_T("BS_��KEY"),0,strSpeciesKey);
 	m_pCatApi->ApplyExtraQuery(pDM_Species, pDM_Index, pDM_Book, strSpeciesKey, pArrStrAddFrame);
	//=====================================================

	ids = pDM_Species->SendFrame();
	if (ids < 0) 
	{
		m_strLastError = _T("DB�� ������ ���� ����");
		return ids;
	}

	ids = pDM_Species->EndFrame();
	
	return 0;

	EFS_END
	return -1;
}


/////////////////////////////////////////////////////////////////////////////
// [Func. Desc]  DB�� ����
// [Return Values] ������ ����
// [Mini Spec] 
// 1. ������ ������ ���� �����Ų��.
// 2. �⺻ ������ �����Ų��.
// 3. ������ �����̸� �����ȣ ����
// 4. ���� ���� ����
// 5. ����/�������� ���� ����
// 6. å���� ���� ����
// 7. ebook ���� ���� �ϱ� 
// 8. ���������� ����
// 9. ������ ��ȣ Update
/////////////////////////////////////////////////////////////////////////////
INT CMarcImportDlg::InsertSpecies( CESL_DataMgr *pDM_Species, 
								  CESL_DataMgr *pDM_Index, 
								  CESL_DataMgr *pDM_Book, 
								  CESL_DataMgr *pDM_Volume, 
								  CESL_DataMgr *pDM_Appendix, 
								  CStringArray *pArrStrAddFrame, 
								  BOOL bCommit)
{
	EFS_BEGIN

	if (pDM_Species == NULL ||
		pDM_Index == NULL ||
		pDM_Book == NULL
		) return -1;

	INT ids = -1;

	// ��Ű
	CString strSpeciesKey;

	// ���� ���鶧 ������ Alias Array
	CArray<CString, CString> arrRemoveAliasList;
	
	//========================================================================
	// 1. ������ ������ ���� �����Ų��.
	//========================================================================
	CopyDM(pDM_Index, 0, pDM_Species, 0,  _T("BS"));

	//========================================================================
	// 2. �⺻ ������ �����Ų��.7
	//========================================================================
	ids = m_pCatApi->ApplyDefaultValue(pDM_Species, pDM_Index, pDM_Book);
	if (ids < 0) return ids;

	// ���� ����� ����

	// �� ������ �����.
	CMarc marc;
	CString strStreamMarc;
	
    ids = pDM_Species->GetCellData( _T("BS_MARC"), 0, strStreamMarc);
	ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);

	//===================================================
	//2010.05.03 ADD BY PJW : ������ ���Ե� 040tag ���� �߰��ϵ��� ��
	m_pCatApi->ApplyLastModifyData(&marc);
	//===================================================
	//===============================================
	//===============================================
	//2008.07.16 ADD BY PJW : ȭ�󱸺��� ��ũ���� �����´�.
	CString strCurrencyCode;
	CString strTemp;
	ids = m_pInfo->pMarcMgr->GetItem(&marc, _T("950$b"), strTemp);
	strCurrencyCode = GetPriceCode(strTemp, strCurrencyCode);
	//===============================================
	
	//========================================================================
	// 3. ������ �����̸� �����ȣ ����
	//========================================================================	
	ids = m_pInfo->pMarcMgr->DeleteField( &marc, _T("001") );
	ids = m_pInfo->pMarcMgr->Encoding(&marc, strStreamMarc);
	ids = pDM_Species->SetCellData( _T("BS_MARC"), strStreamMarc, 0 );

	ids = pDM_Species->MakeRecKey(strSpeciesKey);
	if (ids < 0) return ids;
	pDM_Species->SetCellData( _T("BS_��KEY"), strSpeciesKey, 0);
	
	pDM_Species->InitDBFieldData();

	ids = pDM_Species->MakeQueryByDM(arrRemoveAliasList, pDM_Species, pDM_Species->TBL_NAME, 0, pDM_Species);
	m_pCatApi->LogToFile(pDM_Species->m_lastmsg);

	//========================================================================
	// 4. ���� ���� ����
	//========================================================================	
	// ���ο� Insert�� Insert���� �����Ѵ�.
	pDM_Index->SetCellData( _T("IBS_REC_KEY"), strSpeciesKey, 0);
	pDM_Index->SetCellData( _T("IBS_�����а�"), _T("0"), 0);

	// ���� ������ ����
	arrRemoveAliasList.RemoveAll();
	ids = m_pCatApi->ApplyIndex(pDM_Species, pDM_Index, pDM_Book);

	// ST_CODE�� �����Ѵ�.
	m_pCatApi->ApplyIndex_ST_CODE(pDM_Index, pDM_Book);
	
	pDM_Species->InitDBFieldData();

	ids = pDM_Species->MakeQueryByDM(arrRemoveAliasList, pDM_Index, pDM_Index->TBL_NAME, 0, pDM_Species);
	m_pCatApi->LogToFile(pDM_Species->m_lastmsg);

	//�̿����ѱ���/��������/�ڷ��/��ϱ��� ���а��� ���а� concatenation
	CStringArray Index_classfycodes;
	Index_classfycodes.Add( _T("IBS_�̿����ѱ���_�ڵ�"));
	Index_classfycodes.Add( _T("IBS_��������_�ڵ�"));
	Index_classfycodes.Add( _T("IBS_�ڷ��ִ°�INFO_�ڵ�"));
	Index_classfycodes.Add( _T("IBS_��ϱ���_�ڵ�"));

	CStringArray Book_classfycodes;
	Book_classfycodes.Add( _T("BB_�̿����ѱ���_�ڵ�"));
	Book_classfycodes.Add( _T("BB_��������_�ڵ�"));
	Book_classfycodes.Add( _T("BB_�谡��ġ��ȣ"));
	Book_classfycodes.Add( _T("BB_��ϱ���_�ڵ�"));

	CString Index_Update_sql;
	m_pCatApi->MakeIndexClassifyCodeConcatenationSQL(pDM_Index, pDM_Book, &Index_classfycodes, &Book_classfycodes, 4, Index_Update_sql, strSpeciesKey, _T("BB"));

	ids = pDM_Species->AddFrame(Index_Update_sql);
	if (ids < 0) return ids;

	//========================================================================
	// 5. ����/�������� ���� ����
	//========================================================================
	CString sAcqCode = pDM_Book->GetCellData( _T("BB_���Ա���_�ڵ�"), 0 );
	
	CString sAcqKey;
	ids = pDM_Species->MakeRecKey(sAcqKey);
	if ( 0 > ids ) return -1;

	CString sProvider = pDM_Book->GetCellData( _T("BB_����ó"), 0 );
	//===============================================
	//20080630 ADD BY PJW : �Է����ڸ� �߰��Ѵ�.���Գ⵵�� �߰��Ѵ�.
	CTime t = CTime::GetCurrentTime();
	CString strInputDate;
	CString strAcqYear;
	strInputDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	GetDlgItem( IDC_eACQUISIT_YEAR )->GetWindowText(strAcqYear);
	//===============================================
	//===============================================
	//2008.07.08 ADD BY PJW : ȭ�󱸺�, ���������� �߰��Ѵ�.
	CString strPrice; 
	strPrice = pDM_Book->GetCellData( _T("BB_����"), 0 );
	//===============================================

	if ( sAcqCode == _T("1") ) {
		pDM_Species->InitDBFieldData();
		pDM_Species->AddDBFieldData( _T("REC_KEY"), _T("STRING"), sAcqKey);
		pDM_Species->AddDBFieldData( _T("PROVIDER_CODE"), _T("STRING"), sProvider);		
		pDM_Species->AddDBFieldData( _T("SPECIES_KEY"), _T("STRING"), strSpeciesKey);
		//===============================================
		//20080630 ADD BY PJW : �Է����ڸ� �߰��Ѵ�.���Գ⵵�� �߰��Ѵ�.
		pDM_Species->AddDBFieldData( _T("INPUT_DATE"), _T("STRING"), strInputDate);
		pDM_Species->AddDBFieldData( _T("ORDER_YEAR"), _T("STRING"), strAcqYear);
		//===============================================
		//2008.07.08 ADD BY PJW : ȭ�󱸺�, ���������� �߰��Ѵ�.
		pDM_Species->AddDBFieldData( _T("CURRENCY_CODE"), _T("STRING"), strCurrencyCode);
		strPrice.Replace(_T(","),_T(""));
		pDM_Species->AddDBFieldData( _T("PRICE"), _T("STRING"), strPrice);
		//===============================================
		pDM_Species->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE") );

		pDM_Species->MakeInsertFrame(_T("BO_PURCHASEINFO_TBL"));
	}
	else if ( sAcqCode == _T("2") ) {
		pDM_Species->InitDBFieldData();
		pDM_Species->AddDBFieldData( _T("REC_KEY"), _T("STRING"), sAcqKey);
		pDM_Species->AddDBFieldData( _T("DONATOR_NAME"), _T("STRING"), sProvider);		
		pDM_Species->AddDBFieldData( _T("SPECIES_KEY"), _T("STRING"), strSpeciesKey);
		//===============================================
		//20080630 ADD BY PJW : �Է����ڸ� �߰��Ѵ�.���Գ⵵�� �߰��Ѵ�.
		pDM_Species->AddDBFieldData( _T("INPUT_DATE"), _T("STRING"), strInputDate);
		pDM_Species->AddDBFieldData( _T("ACQ_YEAR"), _T("STRING"), strAcqYear);
		//===============================================
		//===============================================
		//2008.07.08 ADD BY PJW : ȭ�󱸺�, ���������� �߰��Ѵ�.
		pDM_Species->AddDBFieldData( _T("CURRENCY_CODE"), _T("STRING"), strCurrencyCode);
		strPrice.Replace(_T(","),_T(""));
		pDM_Species->AddDBFieldData( _T("PRICE"), _T("STRING"), strPrice);
		//===============================================
		pDM_Species->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE") );

		pDM_Species->MakeInsertFrame(_T("BO_DONINFO_TBL"));
		
	}

	//========================================================================
	// 6. å���� ���� ����
	//========================================================================

	CString strVolSortNo;
	for ( INT i = 0 ; i < pDM_Book->GetRowCount() ; i++ ) {

		pDM_Book->SetCellData( _T("BB_����KEY"), m_strAccessionRecKey, i );
		pDM_Book->SetCellData( _T("BB_��������KEY"), sAcqKey, i );

		strVolSortNo.Format(_T("%d"), (1)*10);
		pDM_Book->SetCellData( _T("BB_���Ϸù�ȣ"), strVolSortNo, i );
	}

	if ( sAcqKey != _T("") ) {
		ids = ((CCatApi_BO*)m_pCatApi)->MakeBookQueryFrame(pDM_Book, strSpeciesKey, sAcqKey, pDM_Species);
	}
	else {
		ids = ((CCatApi_BO*)m_pCatApi)->MakeBookQueryFrame(pDM_Book, strSpeciesKey, _T("-1"), pDM_Species);
	}

	if (ids < 0)
	{
		if ( m_strLastError == _T("") ) 
		{
			m_strLastError = _T("å���� ��������� ����");
		}
		return ids;
	}

	//========================================================================
	// 7. ebook ���� ���� �ϱ�
	//========================================================================
    CESL_DataMgr * pDM_EBook ; 
	pDM_EBook = new CESL_DataMgr;
	pDM_EBook->SetCONNECTION_INFO(pDM_Species->CONNECTION_INFO);
	pDM_EBook->InitDMPKFromFile(_T("DMUP_����_��_EBOOK"));
	pDM_EBook->SetMarcMgr(m_pInfo->pMarcMgr);

	CBO_SPECIES *pBO_Species = new CBO_SPECIES ( this );

    ids = pBO_Species->MakeEBookInfoAndQueryFrame (pDM_Book,pDM_EBook,pDM_Species) ;
    if (ids < 0) 
	{
		m_strLastError = _T("ebook���� ��������� ����");
		return ids;
	}

	//========================================================================
	// 8. ���������� ����
	//======================================================================== 
	//=====================================================
	//2008.12.29 REM BY PJW : Ȩü���� IDX_BO_TBL�� �߰��� ������ SENDFRAME�ٷ� ������ ����.
// 	m_pCatApi->ApplyExtraQuery(pDM_Species, pDM_Index, pDM_Book, strSpeciesKey, pArrStrAddFrame);
	//=====================================================
	
	// ECOSEARCH�� ���� ����
	if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		CString strJJSQuery;
		strJJSQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strSpeciesKey);
		ids = pDM_Species->AddFrame(strJJSQuery);
		if (ids < 0) return ids;
	}

	//========================================================================
	// 9. ������ ��ȣ Update
	//========================================================================
    CESL_DataMgr *pDmLastNum = FindDM( _T("DM_��������ȣ") );
	if (pDmLastNum == NULL) {
		AfxMessageBox( _T("DM_��������ȣ Ȯ��") );
		return -1;
	}

	INT iBookCount = pDM_Book->GetRowCount();
	pDM_Book->SORT(_T("BB_��Ϲ�ȣ"));
	CString sRegno = pDM_Book->GetCellData( _T("BB_��Ϲ�ȣ"), iBookCount-1 );
	INT iRegno = 0;
	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	if ( sRegno != _T("") ) {
// 		iRegno = _ttoi(sRegno.Mid(2));
// 	}
	if ( sRegno != _T("") ) {
		iRegno = _ttoi(sRegno.Mid(nRegCodeLength));
	}
	//=====================================================
	
	
	for ( i = 0 ; i < pDmLastNum->GetRowCount() ; i++ ) {
		CString sRegCode = pDmLastNum->GetCellData( _T("BB_��ϱ���_�ڵ�"), i );
		CString sLastNum = pDmLastNum->GetCellData( _T("��������ȣ"), i );
		INT iLastNum = _ttoi ( sLastNum );
		if ( iRegno != 0 ) {
			iLastNum = iRegno;
		}
		CString sQuery;
		sQuery.Format( _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = '%d', LAST_WORK='%s' WHERE division_value = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE;") , iLastNum , _T("�ڵ���Ϲ�ȣ�ο�"), sRegCode );
		ids = pDM_Species->AddFrame( sQuery );
		if ( 0 > ids ) {
			AfxMessageBox( _T("��������ȣ AddFrame Ȯ��") );
			return -1;
		}
	}
	
	// KOL.RED.2018.223 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if ( !sAcqKey.IsEmpty() && m_bMatchISBN == TRUE ) // ��������KEY�� �ݵ�� �־�� �Ѵ�
	{
		//��ġ�̵�
		// ������ ��� ���� Ȯ�� 
		CString strReservationUseYN,strReserveSpecialCond;
		pDM_Species->GetOneValueQuery( _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='�������' AND VALUE_NAME='�����ɻ�뿩��' AND MANAGE_CODE = UDF_MANAGE_CODE"), strReservationUseYN );

		pDM_Species->GetOneValueQuery(_T("SELECT MAX(VALUE_1) FROM ESL_MANAGE_TBL WHERE GROUP_1='L' AND GROUP_2='����' AND GROUP_3='����' AND VALUE_NAME = '��ġ����켱����Ư������' AND MANAGE_CODE= UDF_MANAGE_CODE"), strReserveSpecialCond); 

		for (i = 0; i < pDM_Book->GetRowCount(); i++ ) 
		{
			// ISBN���� ����ڷ� ��û������ ��ȸ�Ѵ�
			CString strISBN = pDM_Book->GetCellData(_T("BB_����ISBN"), i);
			if (!strISBN.IsEmpty())
			{
				//���� �ڷᰡ ��û�� ��츦 ó���ϱ� ����
				CString strQuery;
				//2020.02-07 JOB.2020.0011 ADD BY KYJ : �ڰ� ��ġ����ڷḸ ������ ���� ����
				//strQuery.Format(_T("SELECT REC_KEY FROM (SELECT REC_KEY FROM BO_FURNISH_TBL WHERE FURNISH_STATUS='1' AND EA_ISBN='%s' ORDER BY APPLICANT_DATE)"), strISBN);
				strQuery.Format(_T("SELECT REC_KEY FROM (SELECT REC_KEY FROM BO_FURNISH_TBL WHERE FURNISH_STATUS='1' AND EA_ISBN='%s' AND MANAGE_CODE = UDF_MANAGE_CODE ORDER BY APPLICANT_DATE)"), strISBN);
				
				CString strRecKey;
				pDM_Species->GetOneValueQuery(strQuery, strRecKey);
				if (!strRecKey.IsEmpty())
				{
					// �ڷ���¿� ���� ����ó�� ������ �޶�����
					CString strWorkingStatus = pDM_Book->GetCellData(_T("BB_�ڷ����"), i);
										
					if (!strWorkingStatus.IsEmpty() && strWorkingStatus.CompareNoCase(_T("BOL112N")) == 0)				
					{
						// ������·� ���ԵǾ��ٸ�
						// 1. ����ڷ����� ���� (����, �����ڵ�, ����Ű, ��ġ����)
						// 2. ����ó��

						// �ڷ���� ó���� ��ǻ�� �ð����� ó���� -> ImportSpecies()
						CString strShelfDate = CTime::GetCurrentTime().Format(_T("%Y/%m/%d"));

						CString strFurnishQuery;
						strFurnishQuery.Format(_T("UPDATE BO_FURNISH_TBL SET FURNISH_STATUS='3', ACQ_CODE='%s', ACQ_KEY=%s, FURNISH_DATE='%s' WHERE REC_KEY='%s';"), sAcqCode, sAcqKey, strShelfDate, strRecKey);
						ids = pDM_Species->AddFrame(strFurnishQuery);
						if (0 > ids)
						{
							// �ڷ������ ����ó���� ���ؼ� ����׿����� ���
							TRACE(_T("CMarcImportDlg::InsertSpecies() FURNISH PROCESS ERROR -10 : %d"), ids);
						}
						
						//��ġ�̵� 
						// ������ ��� ���� Ȯ�� 
						//CString strReservationUseYN;
						//pDM_Species->GetOneValueQuery( _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='�������' AND VALUE_NAME='�����ɻ�뿩��' AND MANAGE_CODE = UDF_MANAGE_CODE"), strReservationUseYN );
						
						if (strReservationUseYN.CompareNoCase(_T("Y")) == 0) // ������� �ʴ´ٸ� Ȯ���� �ʿ䰡 ����
						{
							//��ġ�̵� 
							// ����ڷ� �켱�������� Ȯ��
							strQuery.Format(_T("SELECT RESERVATION_YN FROM BO_FURNISH_TBL WHERE REC_KEY=%s AND USER_NO IS NOT NULL"), strRecKey);

							CString strReservationYN;
							pDM_Species->GetOneValueQuery(strQuery, strReservationYN);

							if (strReservationYN.CompareNoCase(_T("Y")) == 0)
							{
								// IF���� �������� 
								BOOL bFurnishReservation = FALSE;

								// �̿��� DM
								CESL_DataMgr DM_USER;
 								DM_USER.SetCONNECTION_INFO(pDM_Species->CONNECTION_INFO);
 								DM_USER.InitDMPKFromFile(_T("DM_BO_LOC_3300"));

								CString strWhere;
								strWhere.Format(_T("CLU.USER_NO = (SELECT USER_NO FROM BO_FURNISH_TBL WHERE REC_KEY=%s)"), sAcqCode, strRecKey);
								ids = DM_USER.RefreshDataManager(strWhere);
								if (0 > ids)
								{
									// �ڷ������ ����ó���� ���ؼ� ����׿����� ���
									TRACE(_T("CMarcImportDlg::InsertSpecies() FURNISH PROCESS ERROR -11 : %d"), ids);
								}

								// �̿��ڰ� �˻��� ��쿡��
								bFurnishReservation = (DM_USER.GetRowCount() > 0);
								
								if (bFurnishReservation)
								{
									// ��ü��Ȳ Ȯ��
									bFurnishReservation = (_ttoi(DM_USER.GetCellData(_T("��üå��"), 0)) < 1);
								}
																				
								CString strUserClassCode = DM_USER.GetCellData(_T("������������"), 0);

								//if (bFurnishReservation)
								if (bFurnishReservation && strReserveSpecialCond != _T("Y") )
								{
									// ���޺� ���� ���� �Ǽ� Ȯ��
									strQuery.Format(_T("SELECT PREENGAGE_BOOK_CNT FROM CD_CODE_TBL WHERE CLASS=31 AND CODE = '%s'"), strUserClassCode); 

									CString strReservationApplicantCount;
									pDM_Species->GetOneValueQuery(strQuery, strReservationApplicantCount);

									CString strReservationCount = DM_USER.GetCellData(_T("�ѿ���å��"), 0);
									bFurnishReservation = (_ttoi(strReservationApplicantCount) > 0 && _ttoi(strReservationApplicantCount) > _ttoi(strReservationCount));
								}

								if (bFurnishReservation)
								{
									// �����ڷ�
									CString strObjectCode = pDM_Book->GetCellData(_T("BB_�̿��󱸺�_�ڵ�"), i);
									if (strObjectCode.CompareNoCase(_T("AD")) == 0)
									{
										strQuery.Format(_T("SELECT USEOBJCODE FROM CD_CODE_TBL WHERE CLASS=31 AND CODE = '%s'"), strUserClassCode); 

										CString strValue;
										pDM_Species->GetOneValueQuery(strQuery, strValue);

										bFurnishReservation = (strValue.IsEmpty() || strValue.CompareNoCase(_T("Y")) == 0);
									}
								}

								if (bFurnishReservation)
								{
									// ��ϱ��� ��������
									CString strRegCode = pDM_Book->GetCellData(_T("BB_��ϱ���_�ڵ�"), i);

									strQuery.Format(_T("SELECT LOAN_YN FROM CD_CODE_TBL WHERE CLASS = 4 AND CODE='%s'"), strRegCode); 

									CString strValue;
									pDM_Species->GetOneValueQuery(strQuery, strValue);

									bFurnishReservation = (strValue.CompareNoCase(_T("Y")) == 0);
								}

								if (bFurnishReservation)
								{
									// ��ġ���� ��������
									CString strSeperateCode = pDM_Book->GetCellData(_T("BB_��ġ��ȣ_�ڵ�"), i);
									if (!strSeperateCode.IsEmpty())
									{
										strQuery.Format(_T("SELECT LOAN_YN FROM CD_CODE_TBL WHERE CLASS = 11 AND CODE='%s'"), strSeperateCode); 

										CString strValue;
										pDM_Species->GetOneValueQuery(strQuery, strValue);

										bFurnishReservation = (strValue.CompareNoCase(_T("Y")) == 0);
									}
								}

								if (bFurnishReservation)
								{										
									// �ڷ�� ��������
									CString strLocCode = pDM_Book->GetCellData(_T("BB_�谡��ġ��ȣ"), i);

									strQuery.Format(_T("SELECT LOAN_YN FROM CD_CODE_TBL WHERE CLASS = 19 AND CODE='%s'"), strLocCode);
									
									CString strValue;
									pDM_Species->GetOneValueQuery(strQuery, strValue);

									bFurnishReservation = (strValue.CompareNoCase(_T("Y")) == 0);
								}

								if (bFurnishReservation)
								{
									// �������� ����
									CString strParamManageCode = pDM_Book->GetCellData(_T("BB_��������_�ڵ�"), i);

									// ���ุ���� ����
									strQuery.Format(_T("SELECT RESERVATION_EXPIRE_DAY FROM CD_CODE_TBL WHERE CLASS = 31 and CODE='%s'"), strUserClassCode);	

									CString strValue;
									pDM_Species->GetOneValueQuery(strQuery, strValue);

									CString strParamExpireDate;
									if (strParamManageCode.IsEmpty())
									{
										strParamExpireDate.Format(_T("ECO$F_G_ISADDHOLIDAY$I(TRIM(SYSDATE), %d)+(23/24)+(59/24/60)+(59/24/60/60)"), _ttoi(strValue));
									}
									else
									{
										strParamExpireDate.Format(_T("ECO$F_G_ISADDHOLIDAY$I(TRIM(SYSDATE), %d, '%s')+(23/24)+(59/24/60)+(59/24/60/60)"), _ttoi(strValue), strParamManageCode);
									}

									//û����ȣ|ECO$F_MAKE_CALLNO$I(B.SEPARATE_SHELF_CODE, B.CLASS_NO, B.BOOK_CODE, B.VOL_CODE, B.COPY_CODE)|str|||
									CString strParamCallNo;
									strParamCallNo.Format(_T("ECO$F_MAKE_CALLNO$I(%s, %s, %s, %s, %s)"), 
											              pDM_Book->GetCellData(_T("BB_��ġ��ȣ_�ڵ�"), i), pDM_Book->GetCellData(_T("BB_�з���ȣ"), i), 
														  pDM_Book->GetCellData(_T("BB_������ȣ"), i), pDM_Book->GetCellData(_T("BB_��å��ȣ"), i),
														  pDM_Book->GetCellData(_T("BB_������ȣ"), i));

									// ���ټ����� ���Ϸù�ȣ 
									strQuery.Format(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'ONELNERESERV'")); 
									pDM_Species->GetOneValueQuery(strQuery, strValue);

									CString strParamVolInfo;
									if (strValue.CompareNoCase(_T("Y")) == 0)
									{
										strParamVolInfo = pDM_Book->GetCellData(_T("BB_���Ϸù�ȣ"), i);
									}

									CString strReservationQuery;
									strReservationQuery.Format(_T("INSERT INTO LS_WORK_T01(REC_KEY, PUBLISH_FORM_CODE, STATUS, RESERVATION_DATE, ")
																		_T("BOOK_KEY, SPECIES_KEY, USER_KEY, CALL_NO, RESERVATION_EXPIRE_DATE, TITLE, PUBLISHER, AUTHOR, REG_CODE, REG_NO, SHELF_LOC_CODE, CLASS_NO_TYPE, CLASS_NO, MANAGE_CODE, SEPARATE_SHELF_CODE, VOL_INFO, MEMBER_CLASS, USER_POSITION_CODE, USER_CLASS_CODE) ")
											                   _T("VALUES (ESL_SEQ.NEXTVAL, 'MO', '3', SYSDATE, ")
																		_T("%s, %s, %s, %s, %s, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s');"),
															   pDM_Book->GetCellData(_T("BB_åKEY"), i),
															   pDM_Book->GetCellData(_T("BB_��KEY"), i),
															   DM_USER.GetCellData(_T("REC_KEY"), 0),
															   strParamCallNo,
															   strParamExpireDate, 
															   pDM_Index->GetCellData(_T("IBS_ǥ������"), 0),
															   pDM_Index->GetCellData(_T("IBS_�����ڻ���"), 0),
															   pDM_Book->GetCellData(_T("BB_������"), i),
															   pDM_Book->GetCellData(_T("BB_��ϱ���_�ڵ�"), i),
															   pDM_Book->GetCellData(_T("BB_��Ϲ�ȣ"), i),
															   pDM_Book->GetCellData(_T("BB_�谡��ġ��ȣ"), i),
															   pDM_Book->GetCellData(_T("BB_�з���ȣ����_�ڵ�"), i),
															   pDM_Book->GetCellData(_T("BB_�з���ȣ"), i),
															   strParamManageCode, 
															   pDM_Book->GetCellData(_T("BB_��ġ��ȣ_�ڵ�"), i),
															   strParamVolInfo,
															   DM_USER.GetCellData(_T("ȸ������"), 0),
															   DM_USER.GetCellData(_T("����Ҽ�����"), 0),
															   DM_USER.GetCellData(_T("������������"), 0)																   
															   );

									ids = pDM_Species->AddFrame(strReservationQuery);
									if (0 > ids) {
										// �ڷ������ ����ó���� ���ؼ� ����׿����� ���
										TRACE(_T("CMarcImportDlg::InsertSpecies() FURNISH PROCESS ERROR -12 : %d"), ids);
									}
									
									// �̿��� ����Ǽ� ����
									INT nReservationCount = _ttoi(DM_USER.GetCellData(_T("�ѿ���å��"), 0));
									nReservationCount++;

									CString strUserQuery;
									strUserQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET RESERVATION_COUNT=%d WHERE REC_KEY=%s;"), nReservationCount, DM_USER.GetCellData(_T("REC_KEY"), 0));
									ids = pDM_Species->AddFrame(strUserQuery);
									if (0 > ids) {
										// �ڷ������ ����ó���� ���ؼ� ����׿����� ���
										TRACE(_T("CMarcImportDlg::InsertSpecies() FURNISH PROCESS ERROR -13 : %d"), ids);
									}										
								}
							}
						}
					}
					else
					{
						// �۾����·� ���ԵǾ��ٸ�
						// 1. ����ڷ����� ���� (����, �����ڵ�, ����Ű)
						CString strFurnishQuery;
						strFurnishQuery.Format(_T("UPDATE BO_FURNISH_TBL SET FURNISH_STATUS='2', ACQ_CODE='%s', ACQ_KEY=%s WHERE REC_KEY='%s';"), sAcqCode, sAcqKey, strRecKey);
						ids = pDM_Species->AddFrame(strFurnishQuery);
						if (0 > ids) {
							// �ڷ������ ����ó���� ���ؼ� ����׿����� ���
							TRACE(_T("CMarcImportDlg::InsertSpecies() FURNISH PROCESS ERROR -20 : %d"), ids);
						}
					}					
				}
			}
		}
	}
	// KOL.RED.2018.223 ---------------------------------------------------------------------------	

	return 0;

	EFS_END
	return -1;
}


/////////////////////////////////////////////////////////////////////////////
// [Func. Desc]  ��ǥå ���Ѵ�.
// [Parameters] åDM/�˸��ƽ����/�����ε���
// [Return Values] ���� 0 ���� -1
// [Mini Spec] 
// ��ǥå�� ���Ѵ�.
/////////////////////////////////////////////////////////////////////////////
INT CMarcImportDlg::GetRepresentationBook(CESL_DataMgr *pDM_Book, 
								   CString strToAliasHeader, 
								   INT &nIdx)
{
	EFS_BEGIN

	if (pDM_Book == NULL) return -1;
	if (strToAliasHeader.GetLength()<=0) strToAliasHeader.Format(_T("BB"));
	nIdx = -1;
	
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, strToAliasHeader)) continue;

		if (pDM_Book->GetCellData( strToAliasHeader + _T("_��ǥå����"), idx) == _T("Y")) 
		{
			nIdx = idx;
			return 0;
		}
	}

	return -1;

	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc]  ��ǥå ���� 
// [Parameters] åDM/�˸��ƽ����/�ε���
// [Return Values] ���� 0 ���� -1
// [Mini Spec] 
// ��ǥå�� ���Ѵ�.
// ��ǥå�� ������ ù��° å�� ��ǥå
/////////////////////////////////////////////////////////////////////////////
INT CMarcImportDlg::SetRepresentationBook(CESL_DataMgr *pDM_Book, 
								   CString strToAliasHeader, 
								   INT nIdx /*= -1*/)
{
	EFS_BEGIN

	if (pDM_Book == NULL) return -1;
	if (strToAliasHeader.GetLength()<=0) strToAliasHeader.Format(_T("BB"));

	// nIdx �� -1�̰� ��ǥå�� ������� ù��° å�� ��ǥå���� �Ѵ�.

	INT ids = -1;

	INT nRowCount = pDM_Book->GetRowCount();
	
	if (nIdx == -1)
	{
		if (GetRepresentationBook(pDM_Book, strToAliasHeader, nIdx) < 0)
		{
			// ��ǥå�� �������
			for (INT idx = 0; idx < nRowCount; idx++)
				if (IsBook(pDM_Book, idx, strToAliasHeader)) break;
			nIdx = idx;
		}		
	}
	else
	{
		// nIdx �� �����Ͱ� å���� Ȯ���Ѵ�.
		if (!IsBook(pDM_Book, nIdx, strToAliasHeader)) return -1;
	}

	// ��ǥå IDX�� ���� �������� N���� ������ �ش�.
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (idx == nIdx)
			pDM_Book->SetCellData( strToAliasHeader + _T("_��ǥå����"), _T("Y"), idx);
		else
			pDM_Book->SetCellData( strToAliasHeader + _T("_��ǥå����"), _T("N"), idx);
	}
	
	return nIdx;

	EFS_END
	return -1;
}


/////////////////////////////////////////////////////////////////////////////
// [Func. Desc]  å���� ���Ѵ�. 
// [Parameters] åDM/�˸��ƽ����/��ϱ���/������
// [Return Values] ���� 0 ���� -1
// [Mini Spec] 
// å���� ���Ѵ�.
/////////////////////////////////////////////////////////////////////////////
INT CMarcImportDlg::GetBookCount(CESL_DataMgr *pDM_Book, 
						  CString strToAliasHeader /*= _T("")*/, 
						  CString strRegCode /*= _T("")*/, 
						  LPCTSTR lpszVol /*=NULL*/)
{
	EFS_BEGIN

	if (pDM_Book == NULL) return -1;

	if (strToAliasHeader.GetLength()<=0) strToAliasHeader.Format(_T("BB"));

	INT nReturnCnt = 0;
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// å�� ���� �ƴϸ� �Ѿ��.
		if (!IsBook(pDM_Book, idx, strToAliasHeader, lpszVol)) continue;

		// ��ϱ��� ��
		if (!strRegCode.IsEmpty())
			if (pDM_Book->GetCellData( strToAliasHeader + _T("_��ϱ���_�ڵ�"), idx) != strRegCode) continue;
		
		nReturnCnt++;
	}

	return nReturnCnt;

	EFS_END
	return -1;
}

BOOL CMarcImportDlg::IsBook(CESL_DataMgr *pDM_Book, 
					 INT idx, 
					 CString strToAliasHeader /*= _T("")*/, 
					 LPCTSTR lpszVol /*= NULL*/)
{
	if (pDM_Book == NULL) return FALSE;

	// ���ุ ����
	if (pDM_Book->GetCellData( strToAliasHeader + _T("_å�η��÷���"), idx) == _T("A")) return FALSE;

	if (pDM_Book->GetCellData( strToAliasHeader + _T("_DB_FLAG"), idx) == _T("D")) return FALSE;

	// ������� ������ üũ
	if (lpszVol != NULL)
		if (pDM_Book->GetCellData( strToAliasHeader + _T("_�����"), idx) != lpszVol) return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc]  DM ����  
// [Parameters] �ҽ�DM/�ҽ��ε���/Ÿ��DM/Ÿ���ε���/�˸��ƽ����/�źθ���Ʈ
// [Return Values] ���� 0 ���� -1
// [Mini Spec] 
// �źθ���Ʈ�� ������ �˸����� ���� DM�� �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////
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
		if (IsStringInArray(pArrIgnoreAlias, strAlias)) continue;
		if ( strAlias == _T("IBS_�谡��ġ��ȣ") ) continue;
		// User Ignore List
		if (pArrIgnoreAlias != NULL)
			if (IsStringInArray(pArrIgnoreAlias, strAlias)) continue;
		
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


BOOL CMarcImportDlg::IsStringInArray(CStringArray *pArr, CString str)
{
	EFS_BEGIN

	if (pArr == NULL || str.IsEmpty()) return FALSE;

	INT nRowCount = pArr->GetSize();
	if (nRowCount == 0) return FALSE;

	for (INT i = 0; i < nRowCount; i++)
	{
		if (pArr->GetAt(i) == str) return TRUE;
	}

	return FALSE;

	EFS_END
	return FALSE;
}

CString CMarcImportDlg::GetPriceCode(CString strPrice, 
									CString &strRealPrice)
{
	EFS_BEGIN

	CString strResult;

	INT nStartPrice = -1;

	TCHAR ch = 0;
	INT nLength = strPrice.GetLength();
	for (INT idx = 0; idx < nLength; idx++)
	{
		ch = strPrice.GetAt(idx);
		if (_istdigit(ch))
		{
			nStartPrice = idx;
			break;
		}
		
		strResult += ch;
	}

	if (nStartPrice >= 0)
		strRealPrice = strPrice.Mid(nStartPrice);

	return strResult;

	return _T("");

	EFS_END
	return _T("");
}

/////////////////////////////////////////////////////////////////////////////
// [Func. Desc]  049�±� ����
// [Parameters] 
// [Return Values] ���� 0 ���� -1
// [Mini Spec] 
// 049�±׸� �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////
INT CMarcImportDlg::Delete049TagFromMarc()
{
EFS_BEGIN

	CString strHD_Species = _T("BS");
	CMarc marc;
	CString strStreamMarc;
	CString strEditMarc;
	
	CArray<CString, CString&> pArr049l;
	CString strItemData, strBookCnt;
	
	INT nBookCnt;
	INT ids = -1;

	INT nCount = m_pDM_Species->GetRowCount();
	for (INT i=0; i < nCount; i++ )
	{
		strStreamMarc = m_pDM_Species->GetCellData( strHD_Species + _T("_MARC"), i);
		if (strStreamMarc.IsEmpty()) return -1;

		ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
		if (ids < 0) return -1;
		
		
		m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &pArr049l);
		nBookCnt = pArr049l.GetSize();		
		pArr049l.RemoveAll();				
		
		strBookCnt.Format(_T("%d"),nBookCnt);
		m_pDM_Species->SetCellData(_T("UDF_å��"),strBookCnt,i);		

		/*
		// 2007.08.18 ADD BY PDJ
		// ���Խ� 049�� �����µ� ������ �ʴ� ��ġ�� ���ý� üũ����.
		// UDF_å�� �����Ŀ� ������ ��
		m_pInfo->pMarcMgr->DeleteItem(&marc, _T("049$f") );
		*/

		ids = m_pInfo->pMarcMgr->DeleteField( &marc, _T("049") );
		if (ids < 0) return -1;

		ids = m_pInfo->pMarcMgr->Encoding(&marc, strStreamMarc );
		if (ids < 0) return -1;

		ids = m_pDM_Species->SetCellData( strHD_Species + _T("_MARC"), strStreamMarc, i);
		if (ids < 0) return -1;
		
	}

	return 0;
EFS_END
	return -1;
}


VOID CMarcImportDlg::OnbInit() 
{	
	//������ ��ư(�����߹���, ���Ͽ���, ��Ϲ�ȣ �ڵ��ο�, ���ı���)�� Disable
	GetDlgItem(IDC_bFILE_OPEN)->EnableWindow(TRUE);
	GetDlgItem(IDC_ChkWorkStatus)->EnableWindow(TRUE);
	GetDlgItem(IDC_ChkGiveAutoRegno)->EnableWindow(TRUE);
	GetDlgItem(IDC_cmbFORM_CLASS)->EnableWindow(TRUE);
	GetDlgItem(IDC_ChkMULTIBOOK)->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_ChkGiveAutoRegno))->SetCheck(FALSE);
	/***********************************************************
	 ***********************************************************/
	//20080515 ADD BY PJW : �����߹���, ���Ͽ���, ��Ϲ�ȣ �ڵ��ο�, ���±��� Default�� ���õǵ��� ����
	((CButton*)GetDlgItem(IDC_ChkWorkStatus))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_ChkMULTIBOOK))->SetCheck(FALSE);
	GetDlgItem(IDC_cmbFORM_CLASS)->EnableWindow(TRUE);
	CString strFormCode;
	pParentMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("���ı���"), strFormCode);
	CESL_ControlMultiComboBox * pCMB = (CESL_ControlMultiComboBox *)GetDlgItem(IDC_cmbFORM_CLASS);
	pCMB->SelectString(strFormCode);
	/***********************************************************
	 ***********************************************************/
	GetDlgItem(IDC_cmbREGCODE_CLASS2)->EnableWindow(FALSE);
	
	GetDlgItem(IDC_bMODIFY)->EnableWindow(FALSE);
	GetDlgItem(IDC_bEXPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_bIMPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_bEXPORT_EDITMARC)->EnableWindow(FALSE);
	
	( (CButton*) GetDlgItem(IDC_ChkAccessionRec) )->SetCheck(FALSE);
	//GetDlgItem(IDC_ChkAccessionRec)->EnableWindow(FALSE);
	GetDlgItem(IDC_ChkAccessionRec)->EnableWindow(TRUE);
	GetDlgItem(IDC_eAccessionRecNo)->EnableWindow(TRUE);
	
	//2020.02.13 JOB.2020.0013 ADD BY KYJ : ��ġ��� ISBN ��Ī �����ؼ� �� �� �ֵ��� ��ɰ���
	((CButton*)GetDlgItem(IDC_ChkMatchISBN))->SetCheck(FALSE);

	//===================================================
	//2009.03.10 ADD BY PJW : �׸���DM�� �ʱ�ȭ�Ѵ�.
	m_pDmGridError->FreeData();
	m_pDmGridOk->FreeData();
	m_pDmGridSuccess->FreeData();
	//===================================================
	

	AllControlDisplay(_T("CM_����_��ũ����"));

	DefaultAccessionRecNo();

	m_wndStatusBarCtrl.SetText(_T(""), 1, 0);
	m_wndStatusBarCtrl.SetText(_T(""), 2, 0);
	m_wndStatusBarCtrl.SetText(_T(""), 3, 0);
}

void CMarcImportDlg::OnrTXT() 
{
	m_nFileType = 1;
	
}

void CMarcImportDlg::OnrEXCEL() 
{
	m_nFileType = 0;
	
}

void CMarcImportDlg::OnrMODS() 
{
	m_nFileType = 3;	
}

void CMarcImportDlg::OnrCSV() 
{
	m_nFileType = 5;	
}

INT CMarcImportDlg::GetAcqType()
{
	CString sAcqCode;
	GetControlData( _T("CM_����_��ũ����_�����ڵ�"), _T("IBS_���Ա���_�ڵ�"), sAcqCode );

	return _ttoi(sAcqCode);
}

VOID CMarcImportDlg::SetCtrlByAcqCode()
{
	
}

void CMarcImportDlg::OnSelchangecmbIMPORTTYPE() 
{
	SetCtrlByAcqCode();	
}

void CMarcImportDlg::OnChkAccessionRec() 
{
	BOOL bCheck;
	
	bCheck = ( (CButton*) GetDlgItem (IDC_ChkAccessionRec) )->GetCheck();
	if ( bCheck )
	{
		if ( CheckAccessionRecNo() == FALSE ) 
		{
			( (CButton*) GetDlgItem (IDC_ChkAccessionRec) )->SetCheck(FALSE);
			return;
		}

		//JOB.2019.0003 ADD BY KYJ : ���Գ⵵����� ���ι�ȣ�� ����ȵǴ� ���� ���� 
		//=================================================================================		
		//DefaultAccessionRecNo();
		//==================================================================================

		( (CEdit*)	GetDlgItem	(IDC_eAccessionRecNo	) )->EnableWindow(TRUE);
		( (CButton*) GetDlgItem	(IDC_btnAccessionRecNo	) )->EnableWindow(TRUE);		
		
	}
	else
	{
		( (CEdit*) GetDlgItem	(IDC_eAccessionRecNo	) )->EnableWindow(FALSE);
		( (CButton*) GetDlgItem	(IDC_btnAccessionRecNo	) )->EnableWindow(FALSE);
	}
}

void CMarcImportDlg::OnbtnAccessionRecNo() 
{
	// ���ΰ���
	
}

void CMarcImportDlg::DefaultAccessionRecNo()
{
	CString strQuery;
	CString strAcqYear;
	CString strAccessionRecNo;
	INT		nAccNum;

	GetControlData( _T("CM_����_��ũ����_�����ڵ�"), _T("BS_���Գ⵵"), strAcqYear );

	strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'ACC_NO' AND MAKE_YEAR = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), strAcqYear );
	CString sResult;
	m_pDM_Main->GetOneValueQuery( strQuery , sResult );

	nAccNum = _ttoi(sResult.GetBuffer(0));
	m_nAccNum = ++nAccNum;	
	strAccessionRecNo.Format( _T("%d") , nAccNum );
	SetControlData( _T("CM_����_��ũ����_�����ڵ�") , _T("BB_���ι�ȣ") , strAccessionRecNo);

	( (CEdit*) GetDlgItem (IDC_eAccessionRecNo) )->EnableWindow(FALSE);


}

INT CMarcImportDlg::SetAccessionRecNoApply()
{
	// 0. ���� �������� üũ
	//    ���� ������ ��ȣ üũ. ������ ��ȣ ���� ũ�� ����� �� ����.
	// 1. ������ �ڷ��� ��Ϲ�ȣ�� �ο����� ���� �ڷᰡ �ִ��� Ȯ���Ѵ�.
	// 2. �ο����� ���� �ڷᰡ ������ ���������� �ڵ� ���� �ϸ� �޽����� �ȳ��Ѵ�.
	//  => ��Ϲ�ȣ�� �ο����� ���� �ڷᰡ �־� ���ι�ȣ�� ������ �� �����ϴ�.
	// 3. ���ǰ� �ִ� �������� �˷��ش�.
	// 4. ���ΰ� ������ Ű�� ������
	// 5. ���ΰ� ������ �����Ѵ�.	
	EFS_BEGIN
	CString strAccessionRecNo;
	m_strAccessionRecKey = _T("");

	CString strAcqCode;
	CString strAcqYear;
	CString sREGISTRANT;
	CString sREG_DATE;
	CString sQuery;
	
	GetControlData(_T("CM_����_��ũ����_�����ڵ�"), _T("IBS_���Ա���_�ڵ�")	, strAcqCode);
	GetControlData(_T("CM_����_��ũ����_�����ڵ�"), _T("BS_���Գ⵵")		, strAcqYear);
	GetControlData(_T("CM_����_��ũ����_�����ڵ�"), _T("BB_���ι�ȣ")		, strAccessionRecNo);

	//���ι�ȣ üũ		
	sQuery.Format(_T("SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_MAKE_YEAR='%s' AND ACCESSION_REC_NO='%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), strAcqYear, strAccessionRecNo );
	CString sResult;	//���� ��Ϲ�ȣ
	m_pDM_Species->GetOneValueQuery( sQuery , sResult );
	if ( sResult != _T("") ) {
		m_strAccessionRecKey = sResult;		
		return 0;
	}


	sREGISTRANT = this->GetUserID();

	CTime t = CTime::GetCurrentTime();
	sREG_DATE.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	
	//rec_key �����
	CString sRecKey;	
	INT ids = m_pDM_Species->MakeRecKey( sRecKey );
	if ( 0 > ids ) return ids;
	ids = m_pDM_Species->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
		return ids;
	}

	m_strAccessionRecKey = sRecKey;
	m_pDM_Species->InitDBFieldData();
	m_pDM_Species->AddDBFieldData(_T("REC_KEY")					, _T("STRING"), sRecKey);
	m_pDM_Species->AddDBFieldData(_T("ACCESSION_REC_MAKE_YEAR")	, _T("STRING"), strAcqYear);
	m_pDM_Species->AddDBFieldData(_T("ACCESSION_REC_NO")		, _T("STRING"), strAccessionRecNo);
	m_pDM_Species->AddDBFieldData(_T("PUBLISH_FORM_CODE")		, _T("STRING"), _T("MO"));
	m_pDM_Species->AddDBFieldData(_T("ACQUISIT_CODE")			, _T("STRING"), strAcqCode);
	m_pDM_Species->AddDBFieldData(_T("ACCESSION_REC_MAKER")		, _T("STRING"), sREGISTRANT);
	m_pDM_Species->AddDBFieldData(_T("REG_DATE")				, _T("STRING"), sREG_DATE);
	m_pDM_Species->AddDBFieldData(_T("MANAGE_CODE")				, _T("STRING"), _T("UDF_MANAGE_CODE") );

	ids = m_pDM_Species->MakeInsertFrame(_T("CO_ACCESSION_REC_TBL"));
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("MakeInsertFrame �� Ȯ���ϼ���"));
		return ids;
	}
	
	// ��������ȣ +1 �� �״�� ����ϸ� UPDATE
	// ������ ��ȣ+1���� ���� ��� �����Ѵٰ� ����.
	if (m_nAccNum == _ttoi(strAccessionRecNo) )
	{		
		sQuery.Format( _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = '%s' WHERE MAKE_YEAR = '%s' AND KIND_CODE = 'ACC_NO' AND MANAGE_CODE = UDF_MANAGE_CODE;") , strAccessionRecNo, strAcqYear );
		INT iCh = m_pDM_Species->AddFrame( sQuery );
		if ( 0 > iCh ) return -100;
	}

	ids = m_pDM_Species->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("�Է»����� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
		return -100;
	}
	ids = m_pDM_Species->EndFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("EndFrame �� Ȯ���ϼ���"));
		return -100;
	}	

	return 0;
	EFS_END
	return -1;
}

BOOL CMarcImportDlg::CheckAccessionRecNo()
{
	CString strAccessionRecNo;
	INT	nAccNum;
	BOOL bCheck;
	
	bCheck = ( (CButton*) GetDlgItem (IDC_ChkAccessionRec) )->GetCheck();
	if ( bCheck == FALSE ) return FALSE;

	GetControlData( _T("CM_����_��ũ����_�����ڵ�") , _T("BB_���ι�ȣ") , strAccessionRecNo);
	nAccNum = _ttoi(strAccessionRecNo.GetBuffer(0));
	if ( nAccNum > m_nAccNum )
	{		
		AfxMessageBox(_T("������ ���ι�ȣ +1 ���� ū ���ι�ȣ�� ����� �� �����ϴ�."));		
		return FALSE;
	} 
	else if ( nAccNum <= 0 ) 
	{	
		AfxMessageBox(_T("����� �� ���� ���ι�ȣ �Դϴ�."));		
		return FALSE;
	}	

	return TRUE;
}



void CMarcImportDlg::OnKillfocuseAccessionRecNo() 
{
	BOOL bCheck;
	
	bCheck = ( (CButton*) GetDlgItem (IDC_ChkAccessionRec) )->GetCheck();
	if ( bCheck == FALSE ) return;

	if ( CheckAccessionRecNo() == FALSE )
	{
		( (CButton*) GetDlgItem (IDC_ChkAccessionRec) )->SetCheck(FALSE);		
	}
}

//JOB.2019.0003 ADD BY KYJ : ���Գ⵵����� ���ι�ȣ�� ����ȵǴ� ���� ���� 
//====================================================================================================================================================
VOID CMarcImportDlg::OnKillfocuseAccessionYear()
{
	BOOL bCheck;
	
	bCheck = ( (CButton*) GetDlgItem (IDC_ChkAccessionRec) )->GetCheck();
	if ( bCheck == FALSE ) return;
	
	CString strQuery;
	CString strAcqYear;
	CString strAccessionRecNo;
	INT		nAccNum;
	
	GetControlData( _T("CM_����_��ũ����_�����ڵ�"), _T("BS_���Գ⵵"), strAcqYear );
	
	strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'ACC_NO' AND MAKE_YEAR = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), strAcqYear );
	CString sResult;
	m_pDM_Main->GetOneValueQuery( strQuery , sResult );
	
	nAccNum = _ttoi(sResult.GetBuffer(0));
	m_nAccNum = ++nAccNum;	
	strAccessionRecNo.Format( _T("%d") , nAccNum );
	SetControlData( _T("CM_����_��ũ����_�����ڵ�") , _T("BB_���ι�ȣ") , strAccessionRecNo);
	
}
//====================================================================================================================================================

HBRUSH CMarcImportDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}


//===================================================
//2009.03.10 ADD BY PJW : �׸��带 display���� ������ �����Ѵ�.
INT CMarcImportDlg::DisplayGrid()
{
	m_pGrid_OK->Display();
	m_pGrid_ERR->Display();	
	m_pGrid_SUCCESS->Display();	

	SetGridRank();
	
	return 0 ;
}
//===================================================

//===================================================
//2009.03.10 ADD BY PJW : �׸��忡 ������ �����Ѵ�.
INT CMarcImportDlg::SetGridRank()
{
	INT nRowCnt;
	INT i;
	CString strData;

 	nRowCnt = m_pDmGridOk->GetRowCount();

	for( i = 0 ; i < nRowCnt ; i++ )
	{
		strData = m_pDmGridOk->GetCellData(_T("����"),i);
		m_pGrid_OK->SetTextMatrix( i+1, 0, strData);
	}

	nRowCnt = m_pDmGridError->GetRowCount();

	for( i = 0 ; i < nRowCnt ; i++ )
	{
		strData = m_pDmGridError->GetCellData(_T("����"),i);
		m_pGrid_ERR->SetTextMatrix( i+1, 0, strData);
	}

	nRowCnt = m_pDmGridSuccess->GetRowCount();

	for( i = 0 ; i < nRowCnt ; i++ )
	{
		strData = m_pDmGridSuccess->GetCellData(_T("����"),i);
		m_pGrid_SUCCESS->SetTextMatrix( i+1, 0, strData);
	}

	return 0 ;
}

//===================================================

//===================================================
//2009.03.17 UPDATE BY PJW : �׸��带 ������ ���� �����Ѵ�.
INT CMarcImportDlg::SortGrid()
{
//	INT nRowCnt;
//	CString strData;
//
// 	nRowCnt = m_pDmGridOk->GetRowCount();

	m_pDmGridOk->SORT(_T("����"));
	m_pDmGridError->SORT(_T("����"));
	m_pDmGridSuccess->SORT(_T("����"));
//	SortDm( m_pDmGridOk );
//	SortDm( m_pDmGridError );
//	SortDm( m_pDmGridSuccess );

	return 0 ;
}
//===================================================

//===================================================
//2009.03.17 UPDATE BY PJW : �׸��带 ������ ���� �����Ѵ�.
INT CMarcImportDlg::SortDm( CESL_DataMgr * pSortDm )
{
	BOOL bChkSwap;

	INT nRowCnt;
	INT i;

	CString strData1;
	CString strData2;
	
	nRowCnt = pSortDm->GetRowCount();
	while(1)
	{
		bChkSwap = FALSE;
		for( i = 0 ; i < nRowCnt-1 ; i++ )
		{
			strData1 = pSortDm->GetCellData( _T("����"), i );		
			strData2 = pSortDm->GetCellData( _T("����"), i+1 );
			if( _ttoi(strData1) > _ttoi(strData2) )
			{
				pSortDm->SORT_SwapRow( i , i + 1 );
				bChkSwap = TRUE;
			}
		}

		if( bChkSwap == FALSE )
		{
			break;
		}
	}
	
	return 0 ;
}
//===================================================



//===================================================
//2009.03.10 ADD BY PJW : 

INT CMarcImportDlg::MsgSortGrid()
{
	SortGrid();
	DisplayGrid();

	return 0 ;
}

INT CMarcImportDlg::MsgDisplayGrid()
{
	DisplayGrid();
	
	return 0 ;
}

//===================================================

void CMarcImportDlg::OnbNORMALIZATION() 
{
	EFS_BEGIN

	INT nDmColCount		= 0;
	INT nIdx			= 0;
	INT nSelectCnt		= 0;
	INT nOkDmRowCnt		= 0;
	INT nErrDmRowCnt	= 0;
	INT nRecovers		= 0;

	CString strMsg;
	CString strData;

 	BeginWaitCursor();

	nDmColCount = m_pDmGridError->GetRefColumnCount();
	nErrDmRowCnt = m_pDmGridError->GetRowCount();
	m_pGrid_ERR->SelectMakeList();
	nSelectCnt = m_pGrid_ERR->SelectGetCount();
	
	if( 0 < nSelectCnt )
	{
		nIdx = m_pGrid_ERR->SelectGetTailPosition();

		for( INT i = 0 ; i < nSelectCnt ; i++ )
		{
			m_pDmGridError->GetCellData(_T("BS_���Ϲ��Կ���"), nIdx, strData);

			if( strData.IsEmpty() )
			{
				nOkDmRowCnt = m_pDmGridOk->GetRowCount();
				m_pDmGridOk->InsertRow(-1);
				
				for( INT j = 0 ; j < nDmColCount ; j++ )
				{
					strData = m_pDmGridError->GetCellData( nIdx, j );
					m_pDmGridOk->SetCellData( nOkDmRowCnt, j, strData );					
				}

				m_pDmGridError->DeleteRow( nIdx );

				// ���� ���� ����
//				strData = m_wndStatusBarCtrl.GetText(1);
//				strData = strData.Mid(strData.ReverseFind(':') + 1);
//				strData.Format(_T("%d"), _ttoi(strData) + 1);
//				m_wndStatusBarCtrl.SetText(_T("���� : ") + strData, 1, 0);
//				
//				strData = m_wndStatusBarCtrl.GetText(2);
//				strData = strData.Mid(strData.ReverseFind(':') + 1);
//				strData.Format(_T("%d"), _ttoi(strData) - 1);
//				m_wndStatusBarCtrl.SetText(_T("���� : ") + strData, 2, 0);

				nRecovers++;
			}

			nIdx = m_pGrid_ERR->SelectGetPrev();
		}
	}
	else
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return ;
	}

 	EndWaitCursor();

	if( nRecovers > 0 )
	{
		INT nRowCnt;
		nRowCnt = m_pDmGridOk->GetRowCount();
		strData.Format( _T("%d"), nRowCnt );
		m_wndStatusBarCtrl.SetText(_T("���� : ") + strData, 1, 0);
				
		nRowCnt = m_pDmGridError->GetRowCount();
		strData.Format( _T("%d"), nRowCnt );
		m_wndStatusBarCtrl.SetText(_T("���� : ") + strData, 2, 0);

		strMsg.Format(_T("[ %d ]�� �� [ %d ] ���� �����߽��ϴ�."), nErrDmRowCnt, nRecovers);
		AfxMessageBox(strMsg);
// 		m_pDmGridOk->SORT(_T("����"));
		SortDm(m_pDmGridOk);
		DisplayGrid();
	}
	else
	{
		strMsg.Format(_T("���� ������ �ڷᰡ �����ϴ�."));
		AfxMessageBox(strMsg);	
	}

	return ;
	EFS_END
	
}
