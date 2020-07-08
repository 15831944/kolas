// FileImportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MarcImportDlg_SE.h"
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
// CMarcImportDlg_SE dialog

CMarcImportDlg_SE::CMarcImportDlg_SE(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcImportDlg_SE)
	m_nFileType = 1;
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
	
	m_hThread= ::CreateEvent(NULL, FALSE, FALSE, _T("ThreadEvent"));
}

CMarcImportDlg_SE::~CMarcImportDlg_SE()
{
}

BOOL CMarcImportDlg_SE::Create(CWnd* pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	return FALSE;


	EFS_END
	return FALSE;
}

VOID CMarcImportDlg_SE::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcImportDlg_SE)
	DDX_Radio(pDX, IDC_rEXCEL, m_nFileType);
	DDX_Text(pDX, IDC_eFILE_PATH, m_strFilePath);
	//}}AFX_DATA_MAP


	EFS_END
}


BEGIN_MESSAGE_MAP(CMarcImportDlg_SE, CDialog)
	//{{AFX_MSG_MAP(CMarcImportDlg_SE)
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
	ON_CBN_SELCHANGE(IDC_cmbMEDIA_CLASS, OnSelchangecmbMEDIACLASS)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_END_MARC_ADJUST, OnEndMarcAdjust)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////
// THREAD

UINT ThreadFunc_Import(LPVOID pParam)
{
	CMarcImportDlg_SE *pDlg = (CMarcImportDlg_SE*)pParam;
	// KOL.UDF.022
	if (&pDlg == NULL)
	{
		pDlg->m_bIsThreadRunning = FALSE;
		return -1;
	}

	pDlg->m_bIsThreadRunning = TRUE;
	pDlg->ButtonStatus(0);

	// Initialization Variable
	CESL_ControlMgr *pCM = NULL;
	CESL_Grid *pGrid_OK = NULL;
	CESL_Grid *pGrid_ERR = NULL;
	CESL_Grid *pGrid_SUCCESS = NULL;

	// �ű� �׸���
	CESL_Grid *pGrid_TO = NULL;
	// ����������ų ID
	UINT nID_TO = -1;
	// ����������ų String
	CString strTO;

	// DataMgr
	CString strDMPK;

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
	
	INT ids = -1;
	INT idxDM = -1;
	INT nCount = -1;

	INT nColCount = -1;

	INT nRowTo = 0;
	INT nRowsTo = 0;

	CString strData;
	CString strErrorMsg;
    CString nTotalCnt;
    CString strTotalMsg;
    CString nFalseCnt;
	CString nTrueCnt;

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

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = pDlg->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	
	while(TRUE)
	{
		if (!pDlg->m_bIsThreadRunning) break;
		strErrorMsg.Empty();
		pGrid_OK->SelectMakeList();
		INT idx = pGrid_OK->SelectGetHeadPosition() + 1;
		if (idx <= 0) break;

		strData = pGrid_OK->GetTextMatrix(idx, 0);
		if (strData.IsEmpty()) continue;

		idxDM = _ttoi(strData);

		//�ڵ��ο��� üũ�Ǿ� ������� �ڵ��ο� DM ����
		CButton * pChkGiveAutoRegno;
		pChkGiveAutoRegno = (CButton *)pDlg->GetDlgItem(IDC_ChkGiveAutoRegno);
		if ( pChkGiveAutoRegno->GetCheck() == 1 ) {	

			///////////////////////////////////////////////////////////////////////////////////////////

			INT ids = -1;
			// ��ϱ��� ��������
			CString sRegCode;
			pDlg->GetControlData( _T("CM_����_��ũ����_�����ڵ�"),  _T("SB_��ϱ���"), sRegCode );
			//pDlg->GetControlData( _T("CM_����_��ũ����_�����ڵ�"), _T("SB_��ϱ���"), sRegCode );

			// ������ ��ȣ ��������
			CString sQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'REG_NO' AND MANAGE_CODE = UDF_MANAGE_CODE AND DIVISION_VALUE = '") + sRegCode + _T("'");
			CString sLastNum;	
			DM_Species.GetOneValueQuery( sQuery , sLastNum );
			INT iLastNum = _ttoi ( sLastNum );

			//��ϱ����� ������ �����߰�
			if ( sLastNum == _T("") ) {
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
				sQuery_reg.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ( REC_KEY , PUBLISH_FORM_CODE , KIND_CODE , DIVISION_VALUE , MAKE_YEAR , LAST_NUMBER,MANAGE_CODE) ")
								  _T("VALUES ( '%s' , '%s' , '%s' , '%s' , '%s' , '%s',UDF_MANAGE_CODE );") 
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

			CESL_DataMgr *pDmLastNum = pDlg->FindDM( _T("DM_����_��������ȣ") );
			if (pDmLastNum == NULL) {
				AfxMessageBox( _T("DM_��������ȣ Ȯ��") );
				goto ERR;
			}
			pDmLastNum->FreeData();
			pDmLastNum->InsertRow(-1);
			pDmLastNum->SetCellData( _T("SB_��ϱ���_�ڵ�"), sRegCode, 0 );
			
			//����
			CMarc marc;
			CString sMarc = DM_Species.GetCellData( _T("SS_MARC"), 0 );
			pDlg->m_pInfo->pMarcMgr->Decoding(sMarc, &marc);
			CArray<CString, CString&> pArr049l;
			CString strItemData;
			pDlg->m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &pArr049l);


			//������ �ڷ��� å���� �ϵ��� ����
			//ȫ��

			INT iBookCount = DM_Book.GetRowCount();

			for ( INT j = 0 ; j < iBookCount ; j++ ) {
				iLastNum++;
				CString sRegNo;

				//=====================================================
				//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 				sRegNo.Format( _T("%s%010d"), sRegCode, iLastNum );
// 				sLastNum.Format( _T("%d"), iLastNum );

				sLastNum.Format( _T("%d"), iLastNum );
				sRegNo = pDlg->m_pInfo->MakeRegNo(sRegCode, sLastNum );				
				//=====================================================
				
				pDmLastNum->SetCellData( _T("��������ȣ"), sLastNum, 0 );
				
				CString sItem;
				if ( pArr049l.GetSize() == iBookCount ) {
					sItem = pArr049l.GetAt(j);
					ids = DM_Book.SetCellData(_T("SB_��Ϲ�ȣ"), sRegNo, j);
					//=====================================================
					//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 					ids = DM_Book.SetCellData(_T("SB_��ϱ���_�ڵ�"), sRegNo.Left(2), j);
					
					ids = DM_Book.SetCellData(_T("SB_��ϱ���_�ڵ�"), sRegNo.Left(nRegCodeLength), j);
					//=====================================================
					
					pDlg->m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo, pArr049l.GetAt(j) );
				}
				else {
					ids = DM_Book.SetCellData(_T("SB_��Ϲ�ȣ"), sRegNo, j);
					//=====================================================
					//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 					ids = DM_Book.SetCellData(_T("SB_��ϱ���_�ڵ�"), sRegNo.Left(2), j);
					
					ids = DM_Book.SetCellData(_T("SB_��ϱ���_�ڵ�"), sRegNo.Left(nRegCodeLength), j);
					//=====================================================
					
					pDlg->m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo );
				}
				pDlg->m_pInfo->pMarcMgr->Encoding( &marc, sMarc );
				DM_Species.SetCellData( _T("SS_MARC"), sMarc, 0 );
			}
		///////////////////////////////////////////////////////////////////////////////////////////	
		}


		///////////////////////////////////////////////////////////////////////////////////////////	
		//�����߹���, ������� �������� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		///////////////////////////////////////////////////////////////////////////////////////////	

		CString sAcqCode;		
		pDlg->GetControlData( _T("CM_����_��ũ����_�����ڵ�"), _T("ISS_���Ա���_�ڵ�"), sAcqCode );
		INT iBookCount = DM_Book.GetRowCount();
		for ( INT i = 0 ; i < iBookCount ; i++ ) {
			//���Ա��� ����
			DM_Book.SetCellData( _T("SB_���Ա���_�ڵ�"), sAcqCode , i );
			
			//����� ����
			CString result;
			CTime t = CTime::GetCurrentTime();
			result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
			ids = DM_Book.SetCellData( _T("SB_�����"), result, i);

			//��Ϲ�ȣ 12�ڸ� �ǵ��
			CString strRegNo;
			ids = DM_Book.GetCellData( _T("SB_��Ϲ�ȣ"), i, strRegNo);
			if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
				//=====================================================
				//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 				if ( strRegNo.GetLength() > 2 ) {
// 					CString sRight;
// 					sRight.Format( _T("%010s") , strRegNo.Mid(2) );
// 					strRegNo = strRegNo.Left(2) + sRight;
// 				}
				
				if ( strRegNo.GetLength() > nRegCodeLength ) {
					strRegNo = pDlg->m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength),strRegNo.Mid(nRegCodeLength) );
				}
				//=====================================================
			}
			ids = DM_Book.SetCellData( _T("SB_��Ϲ�ȣ"), strRegNo, i);
			
		}

		for ( i = 0 ; i < DM_Species.GetRowCount() ; i++ ) {
			DM_Species.SetCellData( _T("SS_��������"), _T("1") , i );
		}
		///////////////////////////////////////////////////////////////////////////////////////////	
		//�����߹���, ������� �������� -----------------------------------------------------------
		///////////////////////////////////////////////////////////////////////////////////////////	

		CButton * pChkWorkStatus;	
		pChkWorkStatus = (CButton *)pDlg->GetDlgItem(IDC_ChkWorkStatus);
		if ( pChkWorkStatus->GetCheck() == 0 ) {

			//////////////////////////////////////////////////////////////////////////////
			//�������
			//////////////////////////////////////////////////////////////////////////////

			for ( i = 0 ; i < DM_Species.GetRowCount() ; i++ ) { DM_Species.SetCellData( _T("SS_�۾����±���"), _T("0") , i ); }
			for ( i = 0 ; i < DM_Index.GetRowCount() ; i++ ) { DM_Index.SetCellData( _T("ISS_�۾�����INFO"), _T("BOL112N") , i ); }
			ids = pDlg->ImportSpecies(&DM_Species, &DM_Index, &DM_Book, &DM_Volume);
		}
		else {

			//////////////////////////////////////////////////////////////////////////////
			//�����߹���
			//////////////////////////////////////////////////////////////////////////////

			for ( INT i = 0 ; i < DM_Book.GetRowCount() ; i++ ) {

				DM_Book.SetCellData( _T("SB_�ڷ����"), _T("BOC113N") , i );

				CString strRegNo;
				ids = DM_Book.GetCellData( _T("SB_��Ϲ�ȣ"), i, strRegNo);
				//��Ϲ�ȣ 12�ڸ� �����
				if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
					//=====================================================
					//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 					if ( strRegNo.GetLength() > 2 ) {
// 						CString sRight;
// 						sRight.Format( _T("%010s") , strRegNo.Mid(2) );
// 						strRegNo = strRegNo.Left(2) + sRight;
// 					}
					//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
					if ( strRegNo.GetLength() > nRegCodeLength ) {
						strRegNo = pDlg->m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength),strRegNo.Mid(nRegCodeLength) );
					}
					//=====================================================
					
				}
				ids = DM_Book.SetCellData( _T("SB_��Ϲ�ȣ"), strRegNo, i);
			}

			for ( i = 0 ; i < DM_Species.GetRowCount() ; i++ ) { DM_Species.SetCellData( _T("SS_�۾����±���"), _T("1") , i ); }
			for ( i = 0 ; i < DM_Index.GetRowCount() ; i++ ) { DM_Index.SetCellData( _T("ISS_�۾�����INFO"), _T("BOC113N") , i ); }


			//��ǥå���� �谡�� �ο�
			INT nIdx;
			pDlg->GetRepresentationBook(&DM_Book, _T("SB"), nIdx);
			
			CString sTmpStreamMarc;
			CMarc marc;
			
			DM_Species.GetCellData( _T("SS_MARC"), nIdx, sTmpStreamMarc);
			pDlg->m_pInfo->pMarcMgr->Decoding(sTmpStreamMarc, &marc );

			ids = pDlg->m_pCatApi->ApplyShelfCodeData(&DM_Book, &marc, FALSE);

			CStringArray m_arrStrAddFrame;

		}								
		
		if (ids < 0)
		{
			pGrid_TO = pGrid_ERR;
			nID_TO = 2;
			strTO = _T("���� : ");
			strErrorMsg = pDlg->GetLastError();
		}
		else
		{

			pDlg->m_pDM_Main->InsertRow(-1);
			INT nRowIdx = pDlg->m_pDM_Main->GetRowCount() - 1;
			
			// ���� ���� ī�� 
			pDlg->CopyDM(&DM_Index, 0, pDlg->m_pDM_Main, nRowIdx);
			
			// �� ���� ī��
			pDlg->CopyDM(&DM_Species, 0, pDlg->m_pDM_Main, nRowIdx);
			
			// ���� ��Ϲ�ȣ, å�� �־��ֱ�.
			INT nIdx = - 1;
			ids = pDlg->GetRepresentationBook(&DM_Book, _T("SB"), nIdx);

			if (ids < 0 || nIdx < 0)
				nIdx = pDlg->SetRepresentationBook(&DM_Book, _T("SB"), nIdx);

			pDlg->m_pDM_Main->SetCellData(_T("���۵�Ϲ�ȣ"), DM_Book.GetCellData( _T("SB_��Ϲ�ȣ"), nIdx), nRowIdx);
			
			CString strItemData;
			strItemData.Format(_T("%d"), pDlg->GetBookCount(&DM_Book));
			pDlg->m_pDM_Main->SetCellData(_T("å��"), strItemData, nRowIdx);

			strItemData = _T("");

			pDlg->m_pDM_Main->GetCellData( _T("SS_��������"), nRowIdx, strItemData);
			INT nMainGridRowIdx = pDlg->m_pDM_Main->GetRowCount() -1;

			if (strItemData == _T("1") ) {
				pDlg->m_pDM_Main->SetCellData( _T("SS_��������"), _T("������"), nMainGridRowIdx );
				INT ii = pDlg->m_pDM_Main->GetRowCount();
			}
			else if (strItemData == _T("0") ) {
				pDlg->m_pDM_Main->SetCellData( _T("SS_��������"), _T("�����ȵ�"), nMainGridRowIdx );
			}
			

			pGrid_TO = pGrid_SUCCESS;
			nID_TO = 3;
			strTO = _T("���� : ");
		}
        
         //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	   // 2003�� 10�� 14�� ������ ���� 
	    if(!(ids<0)) 
		{ 
		    CString strRegNo ;
			CString strGetAccRegNo ;
			CString strQuery ;
			CString strRegCode ;
			INT iRegNumber ;
			INT nCount ;
			CString strCount =_T("") ;
			
			nCount = DM_Book.GetRowCount () ;
			DM_Book.GetCellData ( _T("SB_��Ϲ�ȣ"),idxDM,strRegNo);
			if(!strRegNo.IsEmpty ())
			{   
				//=====================================================
				//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 				strRegCode = strRegNo.Left(2) ;
// 				iRegNumber = _ttoi(strRegNo.Mid (2)) ;
				//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
				strRegCode = strRegNo.Left(nRegCodeLength) ;
				iRegNumber = _ttoi(strRegNo.Mid (nRegCodeLength)) ;
				//=====================================================
				
				
                strQuery.Format (_T("delete from CO_DROP_ACCESSION_NO_TBL  where reg_code = '%s' and  REG_NO = %d ;"), strRegCode,iRegNumber );
				DM_Book.StartFrame () ;
				DM_Book.InitDBFieldData () ;
				DM_Book.AddFrame (strQuery);
				DM_Book.SendFrame ();
				DM_Book.EndFrame () ;
			}
		
		}
       //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


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
			pDlg->m_mapGridAlias.Lookup( _T("SS_���Ϲ��Կ���"), col);
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
	AfxMessageBox(strTotalMsg);
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

UINT ThreadFunc_Display(LPVOID pParam)
{
	CMarcImportDlg_SE *pDlg = (CMarcImportDlg_SE*)pParam;
	// KOL.UDF.022
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

	INT stepQuota = 20;
//	INT stepCount = nTotalCount / stepQuota;

	// Start...

	// ������ DM
	pDM_Species = pDlg->m_pDM_Species;
	if (pDM_Species == NULL)
	{
		goto ERR;
	}
	
	nTotalCount = pDM_Species->GetRowCount();
	
	pCM = pDlg->FindCM(pDlg->m_lpszCMAlias);
	if (pCM == NULL)
	{
		goto ERR;
	}

	// TOTAL_COUNT
	strCount.Format(_T("��ü : %d"), nTotalCount);
	pDlg->m_wndStatusBarCtrl.SetText(strCount, 0, 0);

	
	// ��������
	if ( nTotalCount/stepQuota == 0 )
		pDlg->m_wndProgressBar.SetRange(0, 1);
	else
		pDlg->m_wndProgressBar.SetRange(0, nTotalCount);
	pDlg->m_wndProgressBar.SetPos(0);
	pDlg->m_wndProgressBar.SetWindowText(_T(""));
		
	// ERR_COUNT
	for (i = 0; i < nTotalCount; i++)
	{
		if (!pDM_Species->GetCellData( _T("SS_���Ϲ��Կ���"), i).IsEmpty())
		{
			nErrCount++;
		}
	}
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

	// Init Grid
	pGrid_OK->Clear();
	pGrid_OK->SetRows(2);
	pGrid_OK->SetColumnInfo();

	pGrid_ERR->Clear();
	pGrid_ERR->SetRows(2);
	pGrid_ERR->SetColumnInfo();

	pGrid_SUCCESS->Clear();
	pGrid_SUCCESS->SetRows(2);
	pGrid_SUCCESS->SetColumnInfo();


	// Set Grid Rows
	if (nErrCount > 0)
		pGrid_ERR->SetRows(nErrCount + 1);
	if (nOKCount > 0)
		pGrid_OK->SetRows(nOKCount + 1);

	// Fetch Start

	
	//BOOL bChkGiveAutoRegno = (pChkGiveAutoRegno->GetCheck() == 1);

	for (row = 0; row < nTotalCount; row++)
	{
		if (0==row % stepQuota)
			pDlg->m_wndProgressBar.StepIt();

		if (pDM_Species->GetCellData( _T("SS_���Ϲ��Կ���"), row).IsEmpty())
		{
			pGrid = pGrid_OK;
			nGridRow = ++nOKGridRow;
		}
		else
		{
			pGrid = pGrid_ERR;
			nGridRow = ++nErrGridRow;
		}

		strCount.Format(_T("%d"), row+1);
		pGrid->SetTextMatrix(nGridRow, 0, strCount);

		if (!pDlg->m_bIsThreadRunning) break;
		
		// �׸��忡 ����.
		pos = pDlg->m_mapGridAlias.GetStartPosition();
//		CButton * pChkGiveAutoRegno;
//		pChkGiveAutoRegno = (CButton *)pDlg->GetDlgItem(IDC_ChkGiveAutoRegno);
		
		while (pos)
		{
			pDlg->m_mapGridAlias.GetNextAssoc(pos, strGridAlias, nGridCol);
			//CString sTmp = pDM_Species->GetCellData(strGridAlias, row);

			//�ڵ��ο� üũ�� ��� �׸��忡 �ڵ��ο��� �ѷ��ֱ�
			CButton * pChkGiveAutoRegno = (CButton *)pDlg->GetDlgItem(IDC_ChkGiveAutoRegno);
			if ( (pChkGiveAutoRegno->GetCheck() == 1) && strGridAlias ==  _T("SS_MARC.049$l") ) 
				pGrid->SetTextMatrix(nGridRow, nGridCol, _T("�ڵ��ο�") );
			else
				pGrid->SetTextMatrix(nGridRow, nGridCol, pDM_Species->GetCellData(strGridAlias, row));
		}
	}

	// �׸��� ���� ����
	pGrid_ERR->SetRowHeight(-1, pGrid_ERR->m_nHeight * 15);
	pGrid_OK->SetRowHeight(-1, pGrid_ERR->m_nHeight * 15);
	pGrid_SUCCESS->SetRowHeight(-1, pGrid_ERR->m_nHeight * 15);
	
	//pDlg->m_wndProgressBar.StepIt();
	pDlg->m_wndProgressBar.SetWindowText(_T("������ġ �Ϸ�"));
	pDlg->m_bIsThreadRunning = FALSE;
	pDlg->ButtonStatus(1);

	SetEvent(pDlg->m_hThread);
	
	return 0;
ERR:
	pDlg->m_bIsThreadRunning = FALSE;
	pDlg->ButtonStatus(1);
	return -1;

	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CMarcImportDlg_SE Operations

INT CMarcImportDlg_SE::Init()
{
	EFS_BEGIN

	// �� API
	m_pCatApi = new CCatApi_SE(this);

	m_pCatApi->InitMemberData();


	// ����
	m_lpszSpeciesDMAlias = _T("DM_����_����DM_������");
	m_lpszIndexDMAlias = _T("DM_����_����DM_��������");
	m_lpszVolumeDMAlias = _T("DM_����_����DM_������");
	m_lpszBookDMAlias = _T("DM_����_����DM_å����");
	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));

	SetWindowText(_T("���� ��ũ ����"));

	// ��DM�� �����Ѵ�.
	m_pDM_Species = new CESL_DataMgr;
	if (m_pDM_Species == NULL) return -1;

	m_pDM_Species->SetCONNECTION_INFO(CONNECTION_INFO);
	m_pDM_Species->SetMarcMgr(m_pInfo->pMarcMgr);

	m_pDM_Species->InitDMPKFromFile(m_lpszSpeciesDMAlias);

	// ���� DM�� �����Ѵ�.
	m_pDM_Index = new CESL_DataMgr;
	if (m_pDM_Index == NULL) return -1;
	m_pDM_Index->SetCONNECTION_INFO(CONNECTION_INFO);
	m_pDM_Index->SetMarcMgr(m_pInfo->pMarcMgr);

	m_pDM_Index->InitDMPKFromFile(m_lpszIndexDMAlias);

	// å DM�� �����Ѵ�.
	m_pDM_Book = new CESL_DataMgr;
	if (m_pDM_Book == NULL) return -1;
	m_pDM_Book->SetCONNECTION_INFO(CONNECTION_INFO);
	m_pDM_Book->InitDMPKFromFile(m_lpszBookDMAlias);


	m_pDM_Book->SetMarcMgr(m_pInfo->pMarcMgr);

	m_pDM_Main = FindDM(_T("DM_����_����_�˻����"));

	// CM
	CESL_ControlMgr *pCM = FindCM(m_lpszCMAlias);
	if (pCM == NULL) return -1;

	// Grid
	m_pGrid_OK = (CESL_Grid*)pCM->FindControl(_T("Grid_OK"));
	// ERROR GRID
	m_pGrid_ERR = (CESL_Grid*)pCM->FindControl(_T("Grid_ERR"));
	// SUCCESS GRID
	m_pGrid_SUCCESS = (CESL_Grid*)pCM->FindControl(_T("Grid_SUCCESS"));

	if (m_pGrid_OK == NULL || m_pGrid_ERR == NULL || m_pGrid_SUCCESS == NULL) return -1;

	// Move Windows
	CRect rcGrid;
	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);
	
	m_pGrid_OK->MoveWindow(rcGrid);
	
	m_pGrid_ERR->MoveWindow(rcGrid);
	
	m_pGrid_SUCCESS->MoveWindow(rcGrid);

	m_pGrid_OK->ELEMENT->INIT_VALUE.Replace(_T("ISBN"), _T("ISSN"));
	m_pGrid_OK->InitColumnInfo();
	m_pGrid_OK->SetColumnInfo();

	m_pGrid_ERR->ELEMENT->INIT_VALUE.Replace(_T("ISBN"), _T("ISSN"));
	m_pGrid_ERR->InitColumnInfo();
	m_pGrid_ERR->SetColumnInfo();

	m_pGrid_SUCCESS->ELEMENT->INIT_VALUE.Replace(_T("ISBN"), _T("ISSN"));
	m_pGrid_SUCCESS->InitColumnInfo();
	m_pGrid_SUCCESS->SetColumnInfo();
		
	// �׸��� Alias ����
	// ����
	// ���۵�Ϲ�ȣ
	// å��
	// ��ǥ��
	// ������
	// ������
	// �Ѽ�ǥ��
	// �����
	// �����
	// ISBN
	// ����
	m_mapGridAlias.SetAt(_T(""), 1);
	m_mapGridAlias.SetAt( _T("SS_MARC.049$l"), 2);
	m_mapGridAlias.SetAt( _T("SS_���Ϲ���å��"), 3);
	m_mapGridAlias.SetAt( _T("SS_MARC.����"), 4);
	m_mapGridAlias.SetAt( _T("SS_MARC.����"), 5);
	m_mapGridAlias.SetAt( _T("SS_MARC.������"), 6);
	m_mapGridAlias.SetAt( _T("SS_MARC.�Ѽ�ǥ��"), 7);
	m_mapGridAlias.SetAt( _T("SS_MARC.�����"), 8);
	m_mapGridAlias.SetAt( _T("SS_MARC.�����"), 9);
	m_mapGridAlias.SetAt( _T("SS_MARC.020$a"), 10);
	m_mapGridAlias.SetAt( _T("SS_���Ϲ��Կ���"), 11);

	// ���ڵ� ���¿� ���ı���
	m_mapRecord_FormCodeMatCode.SetAt(_T("a"), _T("TX=PR|OF"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("c"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("d"), _T("MU=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("e"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("f"), _T("MP=NA"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("g"), _T("VM=VD|VT|VU"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("h"), _T("VM=MC"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("i"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("j"), _T("MU=SB|SD|SG"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("k"), _T("VM=LP|LQ|LY|LZ"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("m"), _T("CF=OB|OA|OD|OH"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("o"), _T("VM=LE"));
	m_mapRecord_FormCodeMatCode.SetAt(_T("r"), _T("VM=LF"));

	// Ű���� ���� ���̾�α�
	CMarc marc;
	m_pDlgKeyWord = new CBO_CAT_PROC_KEYWORD_0000(this, marc);
	m_pDlgKeyWord->Create(this);
	m_pDlgKeyWord->ShowWindow(SW_HIDE);

	return 0;

	return -1;


	EFS_END
	return -1;
}

// Mode : 0 �۾�������
// Mode : 1 �۾��Ϸ�
VOID CMarcImportDlg_SE::ButtonStatus(INT nMode)
{
	EFS_BEGIN

	const INT nIDCount = 2;
	INT nArrID[nIDCount] = {
		IDC_bMODIFY,
//		IDC_bIMPORT, 
		IDC_bEXPORT
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

INT CMarcImportDlg_SE::ImportMarcFromFile()
{
	EFS_BEGIN

	BeginWaitCursor();
	INT ids;

	if (m_strFilePath.IsEmpty()) return -1;

	// ��ũ �׷� ����
	CString sFormCode, sMarcGroup;
	GetControlData( _T("CM_����_��ũ����_�����ڵ�"), _T("ISS_���ı���_�ڵ�"), sFormCode );

	//��ũ����	
	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
	

	// MarcMgr, FileType, FilePath ����
	// File Type : XLS -- EXCEL ����, TXT -- TEXT ����
	m_marcImpExpMgr.Init(this, m_pInfo->pMarcMgr, m_nFileType, m_strFilePath);

	// Call Back Func ����
	//m_marcImpExpMgr.SetCallBackMarcCheck((INT (CWnd::*)(CMarc *pMarc, CString &str))CallBackCheckMarc);
	m_marcImpExpMgr.SetCallBackMarcCheck((INT (CWnd::*)(CMarc *pMarc, CString &str))CallBackCheckMarc);

	// DM ����
	m_marcImpExpMgr.InitDMList();
	m_marcImpExpMgr.AddDMPtr(m_pDM_Species);
	m_marcImpExpMgr.AddDMPtr(m_pDM_Index);

	// ���� üũ ���� DM Alias ����
	m_marcImpExpMgr.SetErrChkAlias( _T("SS_���Ϲ��Կ���"));

	// å�� ���� DM Alias ����
	m_marcImpExpMgr.SetBookCountAlias( _T("SS_���Ϲ���å��"));

	// Stream Marc ���� DM Alias ����
	m_marcImpExpMgr.SetMarcStreamAlias( _T("SS_MARC"));

	// Import ����
	ids = m_marcImpExpMgr.ImportFile();	
	
	// Display Thread Start
	if (ids >= 0)
	{
		// ���� ����
		// ��Ϲ�ȣ �ڵ��ο� -> 049Tag ����
//		CButton * pChkRegNoAutoGive;
//		pChkRegNoAutoGive = (CButton *)GetDlgItem(IDC_ChkGiveAutoRegno);
//		if ( pChkRegNoAutoGive->GetCheck() == 1 )
//		{
			ids = Delete049TagFromMarc();
			if ( ids < 0 ) return -1;
//
//		}

		m_pThreadDisplay = AfxBeginThread(ThreadFunc_Display, this);
	}
	
	EndWaitCursor();

	return ids;


	return -1;

	EFS_END
	return -1;
}

INT CMarcImportDlg_SE::ExportToTxtFile(CString strFilePath)
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
		INT nTmp = _ttoi(pGrid->GetTextMatrix(i + 1, 0)) -1;
		strErrMarc = m_pDM_Species->GetCellData( _T("SS_MARC"), nTmp );
		if (strErrMarc.IsEmpty()) continue;

		strErrMarc += _T("\r\n");

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

VOID CMarcImportDlg_SE::CodeApply()
{
	EFS_BEGIN

	BeginWaitCursor();
	
	INT nRowCount = m_pDM_Index->GetRowCount();
	for (INT row = 0; row < nRowCount; row++)
		CodeApply(row);
	
	EndWaitCursor();


	EFS_END
}

VOID CMarcImportDlg_SE::CodeApply(INT nIdxDM)
{
	EFS_BEGIN

	CESL_ControlMgr *pCM = FindCM(m_lpszCMAlias_Code);
	if (pCM == NULL) return;

	// TODO : ��ũ ���к�~ �ڵ� ����
	INT ids = -1;
	
	CStringArray arrIgnorAlias;
	arrIgnorAlias.Add( _T("ISS_���ı���_�ڵ�"));
	arrIgnorAlias.Add( _T("ISS_��ü����_�ڵ�"));

	// ���� ����, ��ü������ �˸°� �־��ش�.

	CMarc marc;
	CString strStreamMarc;
	CString strFormCode;
	CString strMatCode;

	ids = m_pDM_Species->GetCellData( _T("SS_MARC"), nIdxDM, strStreamMarc);
	if (ids >= 0 && !strStreamMarc.IsEmpty())
	{
		// ���� ����, ��ü������ �˸°� �־��ش�.
		ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
		if (ids >= 0)
		{
			ids = GetFormCodeMatCode(&marc, strFormCode, strMatCode);
			if (ids >= 0)
			{
				m_pDM_Index->SetCellData( _T("ISS_���ı���_�ڵ�"), strFormCode, nIdxDM);
				m_pDM_Index->SetCellData( _T("ISS_��ü����_�ڵ�"), strMatCode, nIdxDM);
			}
		}
	}
	
	
	m_pCatApi->MakeDMFromCM(pCM, m_pDM_Species, MNG_INSERT, nIdxDM, _T("SS"), &arrIgnorAlias);
	m_pCatApi->MakeDMFromCM(pCM, m_pDM_Index, MNG_INSERT, nIdxDM, _T("ISS"), &arrIgnorAlias);

	EFS_END
}

INT CMarcImportDlg_SE::GetFormCodeMatCode(CMarc *pMarc, CString &strFormCode, CString &strMatCode)
{
	EFS_BEGIN

	if (!pMarc) return -1;
	strFormCode.Empty();
	strMatCode.Empty();

	CString strLeader;
	pMarc->GetLeader(strLeader);
	if (strLeader.GetLength() < 7) return -1;

	CString strRecord = strLeader.GetAt(6);

	CString strFormCodeMatCode;
	BOOL bFind = m_mapRecord_FormCodeMatCode.Lookup(strRecord, strFormCodeMatCode);
	if (bFind < 0) return -1;

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
		pCM->GetControlMgrData( _T("ISS_���ı���_�ڵ�"), strTemp);
		if (strTemp == strGetFormCode)
		{
			pCM->GetControlMgrData( _T("ISS_��ü����_�ڵ�"), strTemp);
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

INT CMarcImportDlg_SE::CallBackCheckMarc(CMarc *pMarc, CString &strCheckErrMsg)
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
	arrStrEssentialTag.Add(_T("245"));

	//�ڵ��ο� �ο� ��ư
//	CButton * pChkRegNoAutoGive;
//	pChkRegNoAutoGive = (CButton *)GetDlgItem(IDC_ChkGiveAutoRegno);
//	if ( pChkRegNoAutoGive->GetCheck() == 0 ) {
//		arrStrEssentialTag.Add(_T("049"));
//	}

	CButton * pChkWorkStatus;	
	pChkWorkStatus = (CButton *)GetDlgItem(IDC_ChkWorkStatus);

	//������ ���� ��ư
	if ( pChkWorkStatus->GetCheck() == 0 ) {
		arrStrEssentialTag.Add(_T("008"));
		arrStrEssentialTag.Add(_T("090"));
		arrStrEssentialTag.Add(_T("056"));
		arrStrEssentialTag.Add(_T("260"));
		arrStrEssentialTag.Add(_T("300"));
	}

	// ���� �˻�
	CString strLeader;
	pMarc->GetLeader(strLeader);
	TCHAR ch07 = strLeader.GetAt(7);
	if (ch07 == ' ') 
	{
			strLeader.SetAt(7, 's');
	}
	else 
	{
		if (!(ch07 == 'b' || ch07 == 's'))
		{
			strErrMsg = _T("���� ���������� �ƴմϴ�.. �������ּ���");
			arrStrErrMsg.Add(strErrMsg);
		}		
	}

	if ( pChkWorkStatus->GetCheck() == 0 ) {
		// ��ġ��ȣ ����˻�
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

		// û����ȣ ���� �˻�
		ids = m_pInfo->pMarcMgr->GetItem(pMarc, _T("090$a"), strItemData);
		if (ids < 0)
		{
			strErrMsg = _T("û����ȣ�� �����ϴ�. �������ּ���");
			arrStrErrMsg.Add(strErrMsg);
		}
	}

	// 653���� �˻�
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

	// �ʼ� �� �˻�
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

	// 008 �Է����ڰ� ������ �־��ش�.
	ids = m_pInfo->pMarcMgr->GetItem(pMarc, _T("008@0-5"), strItemData);
	CIndex::TrimAll(strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		strItemData = CTime::GetCurrentTime().Format(_T("%y%m%d"));
		m_pInfo->pMarcMgr->SetItem(pMarc, _T("008@0-5"), strItemData);
	}

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

	if (nCount > 0)
		return -1;
	else
		return 0;

	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CMarcImportDlg_SE message handlers

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	100,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

BOOL CMarcImportDlg_SE::OnInitDialog() 
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

	// ��ϱ��� SE�� Set
	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_cmbREGCODE_CLASS2);
	if ( pCombo )
	{
		pCombo->SetCurSel(28);
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

	GetDlgItem(IDC_rEXCEL)->EnableWindow(FALSE);
	GetDlgItem(IDC_ChkGiveAutoRegno)->EnableWindow(FALSE);	// ��Ϲ�ȣ �ڵ��ο�
	GetDlgItem(IDC_bIMPORT)->EnableWindow(FALSE);	// �����ڷ� ����
	

	PostMessage(WM_SIZE);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	return FALSE;


	EFS_END
	return FALSE;
}

VOID CMarcImportDlg_SE::OnbCLOSE() 
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

VOID CMarcImportDlg_SE::OnSelchangeTabImport(NMHDR* pNMHDR, LRESULT* pResult) 
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

VOID CMarcImportDlg_SE::OnSize(UINT nType, INT cx, INT cy) 
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

BOOL CMarcImportDlg_SE::PreTranslateMessage(MSG* pMsg) 
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

VOID CMarcImportDlg_SE::OnDestroy() 
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

	CDialog::OnDestroy();	


	EFS_END
}

VOID CMarcImportDlg_SE::OnbFILEOPEN() 
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
		OFN.lpstrFilter = _T("XLS����(*.xls)\0*.xls\0�������(*.*)\0*.*\0");
		OFN.lpstrTitle = _T("EXCEL ���� ����");
	}
	else if(m_nFileType == 1)
	{
		OFN.lpstrFilter = _T("TXT����(*.txt)\0*.txt\0�������(*.*)\0*.*\0");
		OFN.lpstrTitle = _T("TEXT ���� ����");
	}


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


	//������ ��ư(�����߹���, ���Ͽ���, ���ı���)�� Disable
	GetDlgItem(IDC_bFILE_OPEN)->EnableWindow(FALSE);
	GetDlgItem(IDC_ChkWorkStatus)->EnableWindow(FALSE);
//	GetDlgItem(IDC_ChkGiveAutoRegno)->EnableWindow(FALSE);
	GetDlgItem(IDC_cmbFORM_CLASS)->EnableWindow(FALSE);

	EFS_END
}

VOID CMarcImportDlg_SE::OnbMODIFY() 
{
	EFS_BEGIN

	// ��ũ���� ȭ���� ����.
	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl == NULL) return;
	
	INT nCurSel = pTabCtrl->GetCurSel();

	CESL_Grid *pGrid = NULL;
	
	if (nCurSel == 0)
	{
		pGrid = m_pGrid_OK;
	}
	else if (nCurSel == 1)
	{
		pGrid = m_pGrid_ERR;
	}

	if (pGrid == NULL) return;

	CString strData = pGrid->GetTextMatrix(pGrid->GetRow(), 0);
	if (strData.IsEmpty()) return;

	UpdateData();
	// Code Apply
	//CodeApply(_ttoi(strData)-1);
	CodeApply();

	m_MarcAdjLaunCher.SetParentInfo(m_pDM_Species, m_pDM_Index, pGrid, pGrid->GetRow() - 1);
//	m_MarcAdjLaunCher.SetParentInfo(m_pDM_Species, m_pDM_Index, pGrid, _ttoi(strData)-1);
	//�����߹��� üũ�Ǿ� ���� ��� ó��
	//����ȫ��
	CButton * pChkWorkStatus;
	pChkWorkStatus = (CButton *)GetDlgItem(IDC_ChkWorkStatus);
	if ( pChkWorkStatus->GetCheck() == 0 ) {
		//����
		m_MarcAdjLaunCher.LaunchMarcAdjust(this, this, _T(""), MNG_IMPORT, MODE_SERIAL);	
	}
	else
	{	
		//����
		m_MarcAdjLaunCher.LaunchMarcAdjust(this, this, _T(""), MNG_IMPORT_WORKING, MODE_SERIAL);		
	}

	return;
	EFS_END
}

VOID CMarcImportDlg_SE::OnbIMPORT() 
{
	EFS_BEGIN

	m_pGrid_OK->SelectMakeList();

	INT nCount = m_pGrid_OK->SelectGetCount();
	if (nCount < 1) 
	{
		AfxMessageBox(_T("������ �ڷḦ ������ �ּ���"));
		return;
	}
	else if (m_pGrid_OK->GetTextMatrix(m_pGrid_OK->SelectGetHeadPosition() + 1, 0).IsEmpty())
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return;
	}
	
	UpdateData();
	// Code Apply
	CodeApply();

	// Import Thread Start
	m_pThreadImport = AfxBeginThread(ThreadFunc_Import, this);


	EFS_END
}

VOID CMarcImportDlg_SE::OnbEXPORT() 
{
	EFS_BEGIN

	// Deterime What Grid is Selected
	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl == NULL) return;

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
	
	if (pGrid == NULL) return;


	if (pGrid->GetRows() == 2)
	{
		if (pGrid->GetTextMatrix(1, 0).IsEmpty())
		{
			AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
			return;
		}
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

	INT ids = GetSaveFileName(&OFN);
	SetCurrentDirectory(szBuffer);

	if (ids != 0)
	{
		strFilePath= OFN.lpstrFile;
		ExportToTxtFile(strFilePath);
	}

	EFS_END
}

VOID CMarcImportDlg_SE::OnbEXPORTEDITMARC() 
{
	EFS_BEGIN

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
	INT nCount = m_pDM_Species->GetRowCount();
	for (INT i = 0; i < nCount; i++)
	{
		strStreamMarc = m_pDM_Species->GetCellData( _T("SS_MARC"), i);
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

UINT CMarcImportDlg_SE::OnEndMarcAdjust(WPARAM wParam, LPARAM lParam)
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
		if (m_pDM_Species->GetCellData( _T("SS_���Ϲ��Լ���"), _ttoi(pGrid->GetTextMatrix(idx + 1, 0))-1) == _T("Y"))
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

BEGIN_EVENTSINK_MAP(CMarcImportDlg_SE, CDialog)
    //{{AFX_EVENTSINK_MAP(CMarcImportDlg_SE)
	ON_EVENT(CMarcImportDlg_SE, IDC_MSHFLEXGRID_ERR, -605 /* MouseDown */, OnMouseDownMshflexgridErr, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMarcImportDlg_SE, IDC_MSHFLEXGRID_OK, -605 /* MouseDown */, OnMouseDownMshflexgridOk, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMarcImportDlg_SE::OnMouseDownMshflexgridOk(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	INT i = m_pGrid_OK->GetMouseRow();
	if (i == 0) return;
	i = m_pGrid_OK->GetFixedRows();
	i = m_pGrid_OK->GetRow();
	//m_pGrid_OK->SetReverse(m_pGrid_OK->GetRow() - 1);


	EFS_END
}

VOID CMarcImportDlg_SE::OnMouseDownMshflexgridErr(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	//m_pGrid_ERR->SetReverse(m_pGrid_ERR->GetRow() - 1);


	EFS_END
}


VOID CMarcImportDlg_SE::OnChkGiveAutoRegno() 
{
	// TODO: Add your control notification handler code here
	CButton * pChkGiveAutoRegno;
	pChkGiveAutoRegno = (CButton *)GetDlgItem(IDC_ChkGiveAutoRegno);
	if ( pChkGiveAutoRegno->GetCheck() == 1 ) {
		GetDlgItem(IDC_cmbREGCODE_CLASS2)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_cmbREGCODE_CLASS2)->EnableWindow(FALSE);
	}
}

VOID CMarcImportDlg_SE::OnSelchangecmbMEDIACLASS() 
{
	//��ü���� �����������

	CESL_ControlMgr *pCM = FindCM(m_lpszCMAlias_Code);
	
	CString sMatCode;
	pCM->GetControlMgrData( _T("ISS_��ü����_�ڵ�") , sMatCode );

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

BOOL CMarcImportDlg_SE::SetConfigAutoRegNo( CESL_DataMgr *DM_Book )
{
EFS_BEGIN
	//�ڵ���ȣ �ο��� ������ �׸� ����
	INT idx = 1;
	if ( idx < 0 ) return FALSE;
	
	// ��dm ��������
	CMarcImportDlg_SE *pDlg = (CMarcImportDlg_SE*)this;
	// KOL.UDF.022
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

	INT ids = -1;
	// ��ϱ��� ��������
	CString sRegCode;
	GetControlData( _T("CM_����_��ũ����_�����ڵ�"), _T("SB_��ϱ���"), sRegCode );

	// ������ ��ȣ ��������
	CString sQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'REG_NO' AND MANAGE_CODE = UDF_MANAGE_CODE AND DIVISION_VALUE = '") + sRegCode + _T("'");
	CString sLastNum;	
	pDlg->m_pDM_Species->GetOneValueQuery( sQuery , sLastNum );
	INT iLastNum = _ttoi ( sLastNum );

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

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
		sQuery_reg.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ( REC_KEY , PUBLISH_FORM_CODE , KIND_CODE , DIVISION_VALUE , MAKE_YEAR , LAST_NUMBER,MANAGE_CODE) ")
						  _T("VALUES ( '%s' , '%s' , '%s' , '%s' , '%s' , '%s',UDF_MANAGE_CODE );") 
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
	pDmLastNum->SetCellData( _T("SB_��ϱ���_�ڵ�"), sRegCode, 0 );


	//����

	CMarc marc;
	CString sMarc = pDlg->m_pDM_Species->GetCellData( _T("SS_MARC"), idx );
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
// 		sLastNum.Format( _T("%d"), iLastNum );
		//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
		sLastNum.Format( _T("%d"), iLastNum );
		sRegNo = pParentMgr->m_pInfo->MakeRegNo(sRegCode, sLastNum );
		//=====================================================
		
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
		pDlg->m_pDM_Species->SetCellData( _T("SS_MARC"), sMarc, idx );
	}

	return TRUE;


EFS_END
return FALSE;
}

INT CMarcImportDlg_SE::MakeMarcAdjustDM(	CESL_DataMgr *pDM_From_Species, 
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
	

	// �� ī��
	CopyDM(pDM_From_Species, nFromSpeciesRow, pDM_To_Species, 0);
	// ���� ī��
	CopyDM(pDM_From_Index, nFromIndexRow, pDM_To_Index, 0);

	// ��ũ��ũ�� ���߱� ���� 049�� �ش��ϴ� ������ å DM�� �ִ´�...
	CMarc marc;
	CString strStreamMarc = pDM_To_Species->GetCellData( _T("SS_MARC"), 0);
	m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);

	CString strRegNo;
	CString strItemData;
	CArray<CString, CString&> arrItem;
	m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &arrItem);
	INT nRowCount = arrItem.GetSize();

	// ��Ϲ�ȣ �ڵ��ο� üũ��ư
	CButton * pChkRegNoAutoGive;
	pChkRegNoAutoGive = (CButton *)GetDlgItem(IDC_ChkGiveAutoRegno);

	// 049 Tag�� ������ �ٽ� �����.
	if (nRowCount >= 0) 
	{	
		pDM_To_Book->FreeData();

		// 049 Tag��� �ڵ��ο��̸� 1å�� �����.(���� ����)
		if ( 0 == nRowCount && pChkRegNoAutoGive->GetCheck() == 1) 
		{
			nRowCount = 1;	
		}
	}
	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	// 049 Tag�� ������ŭ å�� �����.
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		pDM_To_Book->InsertRow(-1);
		
		// ��Ϲ�ȣ ����(�ڵ��ο� �ƴϸ�)
		if ( pChkRegNoAutoGive->GetCheck() == 0 )
		{
			strRegNo = arrItem.GetAt(idx);

			// ��Ϲ�ȣ ����
			pDM_To_Book->SetCellData(_T("SB_��Ϲ�ȣ"), strRegNo, idx);

			//��ϱ��� ����
			CString sRegCode;
			//=====================================================
			//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 			if ( strRegNo.GetLength() >= 2 ) {
// 				sRegCode = strRegNo.Left(2);
// 				pDM_To_Book->SetCellData(_T("SB_��ϱ���_�ڵ�"), sRegCode, idx);
// 			}
			
			if ( strRegNo.GetLength() >= nRegCodeLength ) {
				sRegCode = strRegNo.Left(nRegCodeLength);
				pDM_To_Book->SetCellData(_T("SB_��ϱ���_�ڵ�"), sRegCode, idx);
			}

			//=====================================================
			
		}

		// å�η� �÷���
		pDM_To_Book->SetCellData(_T("SB_å�η��÷���"), _T("B"), idx);

		// DB FLAG ����
		pDM_To_Book->SetCellData(_T("SB_DB_FLAG"), _T("I"), idx);

		// �� -> å
		CopyDM(pDM_To_Species, 0, pDM_To_Book, idx, _T("SB"));
		// ���� -> å
		CopyDM(pDM_To_Index, 0, pDM_To_Book, idx, _T("SB"));
	}

	// ��ǥå ����
	INT nRepBookIdx = SetRepresentationBook(pDM_To_Book, _T("SB"));

	// ��ũ�� ��ũ����
	m_pCatApi->ConvertMarcToCatDM(&marc, pDM_To_Species, pDM_To_Index, pDM_To_Book, pDM_To_Volume);

	return 0;

	EFS_END
	return -1;

}

INT CMarcImportDlg_SE::ImportSpecies( CESL_DataMgr *pDM_Species, 
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

	// ��Ϲ�ȣ �ߺ����� �ϱ�..
	CString strRegNo;
	CString strQuery;
	CString strResult;
	CString strTemp;

	//��������ȣ üũ �ϱ� ����
	CESL_DataMgr *DM_LastNumber = NULL;

	
	DM_LastNumber = FindDM( _T("DM_��ũ����_��������ȣ") );
	
	CString sLastNumberQuery = _T("") ;
	sLastNumberQuery.Format(_T("SELECT DIVISION_VALUE, LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND KIND_CODE = 'REG_NO'"));
	DM_LastNumber->RestructDataManager(sLastNumberQuery);

	nRowCount = pDM_Book->GetRowCount();

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	for (idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("SB"))) continue;
		
		ids = pDM_Book->GetCellData( _T("SB_��Ϲ�ȣ"), idx, strRegNo);
		if (ids < 0 || strRegNo.IsEmpty()) continue;

		//��Ϲ�ȣ 12�ڸ� �����
		if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
			//=====================================================
			//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 			if ( strRegNo.GetLength() > 2 ) {
// 				CString sRight;
// 				sRight.Format( _T("%010s") , strRegNo.Mid(2) );
// 				strRegNo = strRegNo.Left(2) + sRight;
// 			}
			//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
			if ( strRegNo.GetLength() > nRegCodeLength ) {
				strRegNo = pParentMgr->m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength), strRegNo.Mid(nRegCodeLength) );
			}
			//=====================================================
			
		}

		//��Ϲ�ȣ �ֱ�
		pDM_Book->SetCellData( _T("SB_��Ϲ�ȣ"), strRegNo, idx );

		strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE MANAGE_CODE = UDF_MANAGE_CODE AND REG_NO = '%s'"), pDM_Book->TBL_NAME, strRegNo);
		ids = pDM_Species->GetOneValueQuery(strQuery, strResult);

		if (ids >= 0 && !strResult.IsEmpty())
		{
			//��Ϲ�ȣ �ߺ�����
			m_strLastError.Format(_T("��Ϲ�ȣ�� �ߺ��˴ϴ�. [%s]"), strRegNo);
			return -1;
		}

	}
	
	CMarc marc;
	CString strStreamMarc = pDM_Species->GetCellData( _T("SS_MARC"), 0);
	ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
	if (ids < 0) return -1;

	// �谡��ġ ����
	ids = m_pCatApi->ApplyShelfCodeData(pDM_Book, &marc, FALSE);
	if (ids < 0) return -1;

	// ��������
	// ������ ��ȣ ��������
	CString strLibCode;
	m_pCatApi->GetLibInfo(&CString(_T("LIB_CODE")), &strLibCode, 1);
	if ( strLibCode.IsEmpty() )
	{
		m_strLastError.Format(_T("������ ��ȣ�� �����ϴ�. �������ּ���"));
		return -1;
	}
	// ������������ ���� 040
	m_pCatApi->ApplyLastModifyData(&marc);

	// ��ũ �����Ȱ��� �����Ѵ�.
	ids = m_pInfo->pMarcMgr->Encoding(&marc, strStreamMarc);
	ids = pDM_Species->SetCellData( _T("SS_MARC"), strStreamMarc, 0);
	ids = pDM_Species->SetCellData( _T("SS_�������ڷ�"), _T("Y"), 0 );
	
	// LOC_FLAG �ֱ�
	ids = pDM_Species->SetCellData( _T("SS_�۾����±���"), _T("1"), 0 );

	// �Է��ϰ�...
	ids = InsertSpecies(pDM_Species, pDM_Index, pDM_Book, pDM_Volume, pDM_Appendix, pArrStrAddFrame, FALSE);

	if (ids < 0) return -1;

	// 001 TAG ����
	ids = m_pCatApi->CompleteCataloging(pDM_Species, pDM_Species, 0, pDM_Book, FALSE);
	if (ids < 0) return -1;

	CString strShelfDate;
	// ������ �谡���ڸ� ����Ѵ�.
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

	arrSpeciesKey.Add(pDM_Species->GetCellData( _T("SS_��KEY"), 0));
	
	nRowCount = pDM_Book->GetRowCount();
	for (idx = 0; idx < nRowCount; idx++)
	{
		if (!IsBook(pDM_Book, idx, _T("SB"))) continue;

		arrBookKey.Add(pDM_Book->GetCellData( _T("SB_åKEY"), idx));
	}
	if(pDM_Appendix)
	{
		nRowCount = pDM_Appendix->GetRowCount();
		for (idx = 0; idx < nRowCount; idx++)
		{
			arrAppendixKey.Add(pDM_Appendix->GetCellData( _T("SB_åKEY"), idx));
		}
	}

	ids = m_pCatApi->RecordLocDate(pDM_Species, &arrSpeciesKey, &arrBookKey, &arrAppendixKey, strShelfDate, FALSE);
	if (ids < 0) return -1;

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

INT CMarcImportDlg_SE::InsertSpecies( CESL_DataMgr *pDM_Species, 
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

	// ������ ������ ���� �����Ų��.
	CopyDM(pDM_Index, 0, pDM_Species, 0,  _T("SS"));

	// �⺻ ������ �����Ų��.
	ids = m_pCatApi->ApplyDefaultValue(pDM_Species, pDM_Index, pDM_Book);
	if (ids < 0) return ids;

	// ���� ����� ����

	// �� ������ �����.
	CMarc marc;
	CString strStreamMarc;
	
    ids = pDM_Species->GetCellData( _T("SS_MARC"), 0, strStreamMarc);
	ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
	
	// ������ ���� �϶� �����ȣ ����	
	ids = m_pInfo->pMarcMgr->DeleteField( &marc, _T("001") );
	ids = m_pInfo->pMarcMgr->Encoding(&marc, strStreamMarc);
	ids = pDM_Species->SetCellData( _T("SS_MARC"), strStreamMarc, 0 );

	ids = pDM_Species->MakeRecKey(strSpeciesKey);
	if (ids < 0) return ids;
	pDM_Species->SetCellData( _T("SS_��KEY"), strSpeciesKey, 0);
	
	pDM_Species->InitDBFieldData();

	ids = pDM_Species->MakeQueryByDM(arrRemoveAliasList, pDM_Species, pDM_Species->TBL_NAME, 0, pDM_Species);
	m_pCatApi->LogToFile(pDM_Species->m_lastmsg);

	// ���� ������ �����.
	
	// ���ο� Insert�� Insert���� �����Ѵ�.
	pDM_Index->SetCellData( _T("ISS_REC_KEY"), strSpeciesKey, 0);
	pDM_Index->SetCellData( _T("ISS_�����а�"), _T("0"), 0);

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
	Index_classfycodes.Add( _T("ISS_�̿����ѱ���_�ڵ�"));
	Index_classfycodes.Add( _T("ISS_��������_�ڵ�"));
	Index_classfycodes.Add( _T("ISS_�ڷ��ִ°�INFO_�ڵ�"));
	Index_classfycodes.Add( _T("ISS_��ϱ���_�ڵ�"));

	CStringArray Book_classfycodes;
	Book_classfycodes.Add( _T("SB_�̿����ѱ���_�ڵ�"));
	Book_classfycodes.Add( _T("SB_��������_�ڵ�"));
	Book_classfycodes.Add( _T("SB_�谡��ġ��ȣ"));
	Book_classfycodes.Add( _T("SB_��ϱ���_�ڵ�"));

	CString Index_Update_sql;
	m_pCatApi->MakeIndexClassifyCodeConcatenationSQL(pDM_Index, pDM_Book, &Index_classfycodes, &Book_classfycodes, 4, Index_Update_sql, strSpeciesKey, _T("SB"));

	ids = pDM_Species->AddFrame(Index_Update_sql);
	if (ids < 0) return ids;

	// å������ Insert�� Insert���� �����Ѵ�.

	// ����ó, ������ �ֱ�
	CString sAcqCode = pDM_Book->GetCellData( _T("SB_���Ա���_�ڵ�"), 0 );
	
	CString sAcqKey;
	ids = pDM_Species->MakeRecKey(sAcqKey);
	if ( 0 > ids ) return -1;

	CString sProvider = pDM_Book->GetCellData( _T("SB_����ó"), 0 );

	if ( sAcqCode == _T("1") ) {
		pDM_Species->InitDBFieldData();
		pDM_Species->AddDBFieldData( _T("REC_KEY"), _T("STRING"), sAcqKey);
		pDM_Species->AddDBFieldData( _T("PROVIDER_CODE"), _T("STRING"), sProvider);		
		pDM_Species->AddDBFieldData( _T("SPECIES_KEY"), _T("STRING"), strSpeciesKey);
		pDM_Species->AddDBFieldData( _T("MANAGE_CODE"), _T("STRING"), _T("MANAGE_CODE"));
		pDM_Species->MakeInsertFrame(_T("BO_PURCHASEINFO_TBL"));
	}
	else if ( sAcqCode == _T("2") ) {
		pDM_Species->InitDBFieldData();
		pDM_Species->AddDBFieldData( _T("REC_KEY"), _T("STRING"), sAcqKey);
		pDM_Species->AddDBFieldData( _T("DONATOR_NAME"), _T("STRING"), sProvider);		
		pDM_Species->AddDBFieldData( _T("SPECIES_KEY"), _T("STRING"), strSpeciesKey);
		pDM_Species->AddDBFieldData( _T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE"));
		pDM_Species->MakeInsertFrame(_T("BO_DONINFO_TBL"));
	}

	for ( INT i = 0 ; i < pDM_Book->GetRowCount() ; i++ ) {
		pDM_Book->SetCellData( _T("SB_��������KEY"), sAcqKey, i );
	}

	if ( sAcqKey != _T("") ) {
		ids = ((CCatApi_SE*)m_pCatApi)->MakeBookQueryFrame(pDM_Book, strSpeciesKey, sAcqKey, pDM_Species);
	}
	else {
		ids = ((CCatApi_SE*)m_pCatApi)->MakeBookQueryFrame(pDM_Book, strSpeciesKey, _T("-1"), pDM_Species);
	}

	if (ids < 0)
	{
		//m_strLastError = _T("å���� ��������� ����");
		if ( m_strLastError == _T("") ) {
			m_strLastError = _T("å���� ��������� ����");
		}
		return ids;
	}

    //ebook ���� ���� �ϱ� 
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

	// �η� ������ ������ 
	/* 041005_HSS ���Կ� �η��� �����??
	if (pDM_Appendix)
	{
		// �η� ��Ϲ�ȣ �����Ѵ�.
		ids = ((CCatApi_SE*)m_pCatApi)->GetAppendixInst().ApplyAppendixRegNo(pDM_Appendix, MNG_INSERT);
		if (ids < 0)
		{
			m_strLastError = _T("�η����� ��������� ����");
			return ids;
		}

		//�������� �����Ѵ�.
		ids = ((CCatApi_SE*)m_pCatApi)->MakeBookQueryFrame(pDM_Appendix, strSpeciesKey, _T("-1"), pDM_Species);
		if (ids < 0)
		{
			m_strLastError = _T("�η����� ��������� ����");
			return ids;
		}
	}
	*/

	// Extra Query
	m_pCatApi->ApplyExtraQuery(pDM_Species, pDM_Index, pDM_Book, strSpeciesKey, pArrStrAddFrame);
	
	// ECOSEARCH�� ���� ����
	if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		CString strJJSQuery;
		strJJSQuery.Format(_T("ECO$P_MAKE_SE$I(%s);"), strSpeciesKey);
		ids = pDM_Species->AddFrame(strJJSQuery);
		if (ids < 0) return ids;
	}

	// ��������ȣ ������Ʈ
	CESL_DataMgr *pDmLastNum = FindDM( _T("DM_����_��������ȣ") );
	if (pDmLastNum == NULL) {
		AfxMessageBox( _T("DM_��������ȣ Ȯ��") );
		return -1;
	}
	INT iBookCount = pDM_Book->GetRowCount();
	pDM_Book->SORT(_T("SB_��Ϲ�ȣ"));
	CString sRegno = pDM_Book->GetCellData( _T("SB_��Ϲ�ȣ"), iBookCount-1 );
	INT iRegno = 0;
	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	if ( sRegno != _T("") ) {
		//=====================================================
		//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 		iRegno = _ttoi(sRegno.Mid(2));
		//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
		iRegno = _ttoi(sRegno.Mid(nRegCodeLength));
		//=====================================================
		
	}
	for ( i = 0 ; i < pDmLastNum->GetRowCount() ; i++ ) {
		CString sRegCode = pDmLastNum->GetCellData( _T("SB_��ϱ���_�ڵ�"), i );
		CString sLastNum = pDmLastNum->GetCellData( _T("��������ȣ"), i );
		INT iLastNum = _ttoi ( sLastNum );
		if ( iRegno != 0 ) {
			iLastNum = iRegno;
		}
		CString sQuery;
		sQuery.Format( _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = '%d', LAST_WORK='%s' WHERE MANAGE_CODE = UDF_MANAGE_CODE AND division_value = '%s';") , iLastNum , _T("�ڵ���Ϲ�ȣ�ο�"), sRegCode );
		ids = pDM_Species->AddFrame( sQuery );
		if ( 0 > ids ) {
			AfxMessageBox( _T("��������ȣ AddFrame Ȯ��") );
			return -1;
		}
	}

	return 0;

	EFS_END
	return -1;
}

INT CMarcImportDlg_SE::GetRepresentationBook(CESL_DataMgr *pDM_Book, 
								   CString strToAliasHeader, 
								   INT &nIdx)
{
	EFS_BEGIN

	if (pDM_Book == NULL) return -1;
	if (strToAliasHeader.GetLength()<=0) strToAliasHeader.Format(_T("SB"));
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

INT CMarcImportDlg_SE::SetRepresentationBook(CESL_DataMgr *pDM_Book, 
								   CString strToAliasHeader, 
								   INT nIdx /*= -1*/)
{
	EFS_BEGIN

	if (pDM_Book == NULL) return -1;
	if (strToAliasHeader.GetLength()<=0) strToAliasHeader.Format(_T("SB"));

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

INT CMarcImportDlg_SE::GetBookCount(CESL_DataMgr *pDM_Book, 
						  CString strToAliasHeader /*= _T("")*/, 
						  CString strRegCode /*= _T("")*/, 
						  LPCTSTR lpszVol /*=NULL*/)
{
	EFS_BEGIN

	if (pDM_Book == NULL) return -1;

	if (strToAliasHeader.GetLength()<=0) strToAliasHeader.Format(_T("SB"));

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

BOOL CMarcImportDlg_SE::IsBook(CESL_DataMgr *pDM_Book, 
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

INT CMarcImportDlg_SE::CopyDM(CESL_DataMgr *pDM_From, INT nFromIdx, CESL_DataMgr *pDM_To, INT nToIdx, CString strToAliasHeader /*= _T("")*/, CStringArray *pArrIgnoreAlias /*= NULL*/)
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
		if ( strAlias == _T("ISS_�谡��ġ��ȣ") ) continue;
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
		if ( strAlias == _T("SB_��å��ȣ") || strAlias == _T("SB_�����") ) continue;
		*/
		if ( strAlias == _T("SB_��ϱ���_�ڵ�") ) continue;
		ids = pDM_To->SetCellData(strAlias, strItemData, nToIdx);
		if (ids < 0) continue;
	}

	return nToIdx;

	EFS_END
	return -1;
}
BOOL CMarcImportDlg_SE::IsStringInArray(CStringArray *pArr, CString str)
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
CString CMarcImportDlg_SE::GetPriceCode(CString strPrice, 
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

INT CMarcImportDlg_SE::Delete049TagFromMarc()
{
EFS_BEGIN

	CMarc marc;
	CString strStreamMarc;
	CString strEditMarc;
	INT ids = -1;

	INT nCount = m_pDM_Species->GetRowCount();
	for (INT i=0; i < nCount; i++ )
	{
		strStreamMarc = m_pDM_Species->GetCellData( _T("SS_MARC"), i);
		if (strStreamMarc.IsEmpty()) return -1;

		ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
		if (ids < 0) return -1;

		ids = m_pInfo->pMarcMgr->DeleteField( &marc, _T("049") );
		if (ids < 0) return -1;

		ids = m_pInfo->pMarcMgr->Encoding(&marc, strStreamMarc );
		if (ids < 0) return -1;

		ids = m_pDM_Species->SetCellData( _T("SS_MARC"), strStreamMarc, i);
		if (ids < 0) return -1;
		
	}

	return 0;
EFS_END
	return -1;
}
HBRUSH CMarcImportDlg_SE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
