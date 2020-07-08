// TRANSFER.cpp : implementation file
//

#include "stdafx.h"
#include "TRANSFER.h"
#include "..\SE_API\SeApi.h"
#include "TransferDetail.h"
#include "TransferMgr.h"
#include "../../../esl/kolas3_main/kolas3_mainapi.h"
#include "TransferRegNo.h"
#include "TransferLabelPrint.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTRANSFER dialog


CTRANSFER::CTRANSFER(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CTRANSFER)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nACQ_TYPE = 1; //default
	m_pDlg = NULL;
	m_pDM = NULL;
	m_pDM_PRINT = NULL;
	m_pGrid = NULL;
	m_nCurrentIdx = -1;
	m_nSearchType = 0; //default
	m_bIsLightVersion = FALSE;
}

CTRANSFER::~CTRANSFER()
{
	if(m_pDlg)
	{
		delete m_pDlg;
		m_pDlg = NULL;
	}
}


VOID CTRANSFER::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTRANSFER)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTRANSFER, CDialog)
	//{{AFX_MSG_MAP(CTRANSFER)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(SENDQUERY, OnSendQuery)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTRANSFER message handlers
BOOL CTRANSFER::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CTRANSFER::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridTRANSFER);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 0, cx, cy);	

}

VOID CTRANSFER::DoSearch()
{
	if(m_pDlg)
	{
		m_pDlg->ShowWindow(TRUE);
		m_pDlg->UpdateWindow();
		m_pDlg->CenterWindow();
		m_pDlg->SetFocusOnTitle();
	}
}

VOID CTRANSFER::OnSendQuery(WPARAM w,LPARAM l)
{
	m_pDM->RefreshDataManager(m_pDlg->m_strQuery);
	INT nRowCnt = m_pDM->GetRowCount();
	
	m_nSearchType = m_pDlg->m_nSearchType;

	//�ڷ��, �ڷ����, �����
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("�ڷ�Ǳ���"),_T("UDF_�ڷ��"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("������ڵ�"),_T("UDF_�����"));


	CString strWorkingStatus;
	CString strDesc;
	CString strRFIDSerial;
	CSeApi api(this);

	for(INT i=0;i<nRowCnt;i++)
	{
		strWorkingStatus = _T("");
		strDesc = _T("");
		strRFIDSerial = _T("");

		strWorkingStatus = m_pDM->GetCellData(_T("SB_�ڷ����"),i);
		
		
		if(_T("SEL111O") == strWorkingStatus)
		{
			if(0 == m_nSearchType)
			{
				strDesc = _T("�ǽð��ΰ��ڷ�(����)"); 
			}
			else if(1 == m_nSearchType)
			{
				strDesc = _T("�ǽð��ΰ��ڷ�");
			}		

		}
		else
		{
			strDesc = api.GetWorkingStatusDesc(strWorkingStatus);
		}
		
		m_pDM->SetCellData(_T("UDF_�ڷ����"),strDesc,i);

		//RFID���� 
		strRFIDSerial = m_pDM->GetCellData(_T("SB_RFID"),i);
		strRFIDSerial.TrimLeft();
		strRFIDSerial.TrimRight();
		
		if(strRFIDSerial.IsEmpty())
		{
			m_pDM->SetCellData(_T("UDF_RFID"),_T("N"),i);
		}
		else
		{
			m_pDM->SetCellData(_T("UDF_RFID"),_T("Y"),i);
		}
	}

	
	m_pGrid->Display();
	m_nCurrentIdx = -1;



	if(0 == nRowCnt)
	{
		CString str;
		str.Format(_T("�˻� ����� �����ϴ�."),nRowCnt);
		static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)str.GetBuffer(0));
		AfxMessageBox(str);
	}
	else
	{
		CString str;
		str.Format(_T(" %d ���� �ڷᰡ �˻��Ǿ����ϴ�."),nRowCnt);
		static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)str.GetBuffer(0));
	}

	if( nRowCnt > 0 )
		m_pDlg->ShowWindow(SW_HIDE);
}

BOOL CTRANSFER::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("����_����_�ڷ��ΰ�") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_�ڷ��ΰ�") );
		return false;
	}

	CESL_ControlMgr* pCM = FindCM(_T("CM_����_����_�ڷ��ΰ�"));
	m_pGrid = (CESL_Grid*)pCM->FindControl(_T("�׸���"));
	m_pDM = FindDM(_T("DM_����_����_�ڷ��ΰ�"));
	m_pDM_PRINT = FindDM(_T("DM_����_����_�ڷ��ΰ�_���"));
	
	if(!m_pDM || !m_pDM_PRINT || !m_pGrid) {
		AfxMessageBox( _T("DM,CM,Grid Error : ����_����_�ڷ��ΰ�") );
		return false;
	}
	
	//����Ʈ ���� ���� 
	CSeApi api(this);
	m_bIsLightVersion = api.IsLightVersion(m_pDM);

	//����Ʈ ���� setting 
	if(m_bIsLightVersion)
	{
		SetLightVersion();
	}

	//search dialog
	if(!m_pDlg)
	{
		m_pDlg = new CTransferSearch(this);
		m_pDlg->m_nACQ_TYPE = m_nACQ_TYPE;
		m_pDlg->pMain = this->pMain;
		m_pDlg->pParentMgr = (CESL_Mgr*)this;
		m_pDlg->m_pInfo = this->m_pInfo;
		m_pDlg->Create(this);
		m_pDlg->ShowWindow(FALSE);
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CTRANSFER::SetLightVersion()
{
	//����Ʈ ���������� ���� �ΰ� ��ư�� ��� �ΰ� ��ư�� �Ⱥ��̰� �Ѵ�
	CString strRemoveButton;
	strRemoveButton = _T("��ϴ��ó��");
	pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strRemoveButton , TRUE);
	
	strRemoveButton = _T("�������ó��");
	pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strRemoveButton , TRUE);

	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();
	m_pGrid->InitPopupMenuItem();
	RemoveListArray.Add(_T("��ϴ��ó��"));
	RemoveListArray.Add(_T("�������ó��"));
	m_pGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	

}

VOID CTRANSFER::TransferToRealTime()
{
	if(1 == m_nSearchType)
	{
		AfxMessageBox(_T("�̹� �ΰ�� �ڷ��Դϴ�!"));
		return;
	}

	CTransferMgr mgr(this);
	mgr.SPFExecutePath(_T("�ǽð��ΰ�"));
}

VOID CTRANSFER::TransferToReg()
{
	if(1 == m_nSearchType)
	{
		AfxMessageBox(_T("�̹� �ΰ�� �ڷ��Դϴ�!"));
		return;
	}

	
	CTransferMgr mgr(this);
	mgr.SPFExecutePath(_T("��ϴ��ó��"));
}

VOID CTRANSFER::TransferToBinding()
{
	if(1 == m_nSearchType)
	{
		AfxMessageBox(_T("�̹� �ΰ�� �ڷ��Դϴ�!"));
		return;
	}
	
	CTransferMgr mgr(this);
	mgr.SPFExecutePath(_T("�������ó��"));
	
}

VOID CTRANSFER::TransferToShelf()
{
	if(1 == m_nSearchType)
	{
		AfxMessageBox(_T("�̹� �ΰ�� �ڷ��Դϴ�!"));
		return;
	}

	CTransferMgr mgr(this);
	mgr.SPFExecutePath(_T("�ǽð��谡"));
}

VOID CTRANSFER::ShowSpecificData()
{
	if(m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("���õ� �׸��� �����ϴ�!"));
		return;
	}

	CTransferDetail dlg(this);
	dlg.m_pDM = m_pDM;
	dlg.m_nIdx = m_nCurrentIdx;

	dlg.DoModal();

}

VOID CTRANSFER::PrintTransferedList()
{
	INT nRowCnt = m_pDM->GetRowCount();
	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("����� �ڷᰡ �����ϴ�!"));
		return;
	}
	
	
	m_pGrid->SelectMakeList();
	INT nCount = m_pGrid->SelectGetCount();
	if(0 == nCount) {
		AfxMessageBox(_T("���õ� �׸��� �����ϴ�!"));
		return;
	}

	INT nIdx = m_pGrid->SelectGetHeadPosition();
	
	CString class_name = _T("�ڷ�Ǳ���");
	INT nCode = 256;
	INT nDefaultValue = 0;
	CString strCode[256], strDesc[256];

	m_pInfo->pCodeMgr->GetCode(class_name, strCode, strDesc, nCode, nDefaultValue);	
	INT nCodeCnt = m_pInfo->pCodeMgr->GetCodeCount(class_name);
	
	//print
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	

	INT ids;
	pSIReportManager->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("�����ΰ��ڷ���"));
	
	if (ids < 0) {
		ESLAfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return ;
	}
	
	pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;
	pSIReportManager->SetDataMgr(0, m_pDM_PRINT, 0);


	CString strSHELF_LOC_CODE;
	INT nPrintIdx = -1;
	INT nRowCount;

	pSIReportManager->DeleteTempPageFiles();

	
	for(INT i=0;i<nCodeCnt;i++)
	{
		m_pDM_PRINT->FreeData();
		
		nIdx = m_pGrid->SelectGetHeadPosition();
		
		while(nIdx>=0)
		{
			strSHELF_LOC_CODE = _T("");
			strSHELF_LOC_CODE = m_pDM->GetCellData(_T("SB_�谡��ġ_�ڵ�"),nIdx);
			
			if(strCode[i] == strSHELF_LOC_CODE)
			{
				m_pDM_PRINT->InsertRow(-1);
				nPrintIdx = m_pDM_PRINT->GetRowCount()-1;
				CopyDMToDMByAlias(m_pDM,nIdx,m_pDM_PRINT,nPrintIdx);
			}

			nIdx = m_pGrid->SelectGetNext();
		}
		
 		nRowCount = m_pDM_PRINT->GetRowCount();
		
		if(nRowCount > 0)
		{
			pSIReportManager->MAKEPAGE_DrawWorkForm(-1);
	
		}
	}

	ids = pSIReportManager->Print2();
	

	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}

}

BEGIN_EVENTSINK_MAP(CTRANSFER, CDialog)
    //{{AFX_EVENTSINK_MAP(CTRANSFER)
	ON_EVENT(CTRANSFER, IDC_gridTRANSFER, -600 /* Click */, OnClickgridTRANSFER, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CTRANSFER::OnClickgridTRANSFER() 
{
	// TODO: Add your control notification handler code here
	if(0 == m_pDM->GetRowCount()) return;

	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		m_nCurrentIdx = row-1;
	}
}

INT CTRANSFER::CopyDMToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
{
	if (!pDM_SOURCE || !pDM_TARGET) return -1;

	INT nSourceColumnCount = pDM_SOURCE->GetRefColumnCount();
	CString field_alias, field_name, field_type, data_type, strData;
	INT ids;
	for (INT col = 0; col < nSourceColumnCount; col++) {
		strData = _T("");
		ids = pDM_SOURCE->FindColumnFieldName(col, field_alias, field_name, field_type, data_type, SourceRowIdx, strData);
		ids = pDM_TARGET->SetCellData(field_alias, strData, TargetRowIdx);
	}
	
	return 0;
}

// ����Ϲ�ȣ �ο�
/// ������ �ڷῡ ���� �ϰ� ó�� 
/// 1. ����Ϲ�ȣ�� ����Ҽ� �ִ� �ڷ����� �˻�
/// 2. å���� UPDATE 
/// 3. DM ����
/// 4. �׸��� ����
/// �ϰ� �۾��̱� ������ ���α׷����� �ʿ�

VOID CTRANSFER::RecordTmpRegNo()
{
	
	INT ids;

	CString sSpeciesCnt,sBookCnt;
	ids = GetDMEqualRemoveCnt( this  , m_pDM , _T("SB_��KEY") , m_pGrid , sSpeciesCnt );
	if( ids < 0 ) return;

	m_pGrid->SelectMakeList();
	m_pGrid->SelectGetHeadPosition();
	INT nSelectCnt = m_pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("�ڷḦ �������ֽʽÿ�.") );
		return;
	}

	sBookCnt.Format(_T("%d"),nSelectCnt);


	CTransferRegNo dlg(this);
	dlg.m_strBookCnt = sBookCnt;
	dlg.m_strSpeciesCnt = sSpeciesCnt;
	if(dlg.DoModal() != IDOK) return;


	INT nSuccessCnt = 0;	// �۾��� ������ �ڷ� �� 
	CString sWorkingStatus; // �ڷ����

	// ���α׷����� ����
	CTransferProgress* pProgressBar = NULL;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);


	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// �����Ȳ �����ֱ�
		pProgressBar->ChangeProgressBar(i);

		nIndex = m_pGrid->SelectGetIdx();

		CESL_DataMgr TmpDM;

		sWorkingStatus = m_pDM->GetCellData( _T("SB_�ڷ����") , nIndex);

		if(_T("SEL111O") != sWorkingStatus)
		{
			m_pGrid->SelectGetNext();
			continue;
		}

		// �谡�� ��� DB�۾�
		ids = RecordRegNoDBProc( nIndex , &TmpDM );
		if( ids < 0 ) {
			AfxMessageBox(_T("DB�۾� ������ ������ �־ �۾��� �ߴ��մϴ�"));
			DeleteProgressBarDlg( pProgressBar );
			return;
		}
	

		// DM �۾�
		ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
		if( ids < 0 ) {
			AfxMessageBox(_T("������ �۾� ������ ������ �־ �۾��� �ߴ��մϴ�"));
			DeleteProgressBarDlg( pProgressBar );
			return;
		}
	
		// ȭ�� �۾�
		ids = DBInsertUpdateGridProc(nIndex);
		if( ids < 0 ) {
			AfxMessageBox(_T("Grid ������ ������ �־ �۾��� �ߴ��մϴ�"));
			DeleteProgressBarDlg( pProgressBar );
			return;
		}
	
		nSuccessCnt++;
		m_pGrid->SelectGetNext();
	}
	
	// ���α׷����� ����
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// ��� �޽���
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷῡ ��Ϲ�ȣ�� �ο��Ͽ����ϴ�.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷᰡ ��Ϲ�ȣ�� �ο� �� �� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.") , nSelectCnt );
	else
		sResultMsg.Format( _T("���� �ڷ� : '%d'\r\n���� �ڷ� : '%d'\r\n")
						   _T("���� �ڷ� : '%d'(�ڷ���°� ��Ϲ�ȣ�� �ο� �� �� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.)") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );
}


/// ����Ϲ�ȣ ���
/// ������ �ڷῡ ���� �ϰ� ó�� 
/// 1. ����Ϲ�ȣ ����Ҽ� �ִ� �ڷ����� �˻�
/// 2. å���� UPDATE 
/// 3. DM ����
/// 4. �׸��� ����
/// �ϰ� �۾��̱� ������ ���α׷����� �ʿ�
VOID CTRANSFER::CancelTmpRegNo()
{
	
	INT ids;

	m_pGrid->SelectMakeList();
	m_pGrid->SelectGetHeadPosition();
	INT nSelectCnt = m_pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("�ڷḦ �������ֽʽÿ�.") );
		return;
	}

	INT nSuccessCnt = 0;	// �۾��� ������ �ڷ� �� 
	CString sWorkingStatus; // �ڷ����

	// ���α׷����� ����
	CTransferProgress* pProgressBar = NULL;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// �����Ȳ �����ֱ�
		pProgressBar->ChangeProgressBar(i);

		nIndex = m_pGrid->SelectGetIdx();

		CESL_DataMgr TmpDM;

		sWorkingStatus = m_pDM->GetCellData( _T("SB_�ڷ����") , nIndex);

		if(_T("SEL111O") != sWorkingStatus)
		{
			m_pGrid->SelectGetNext();
			continue;
		}


		// ����Ϲ�ȣ ��� DB�۾�
		ids = CancelRegNoDBProc( nIndex , &TmpDM );
		if( ids < 0 ) {
			AfxMessageBox(_T("DB�۾� ������ ������ �־ �۾��� �ߴ��մϴ�"));
			DeleteProgressBarDlg( pProgressBar );
			return;
		}
	

		// DM �۾�
		ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
		if( ids < 0 ) {
			AfxMessageBox(_T("������ �۾� ������ ������ �־ �۾��� �ߴ��մϴ�"));
			DeleteProgressBarDlg( pProgressBar );
			return;
		}
	

		// ȭ�� �۾�
		ids = DBInsertUpdateGridProc(nIndex);
		if( ids < 0 ) {
			AfxMessageBox(_T("Grid ������ ������ �־ �۾��� �ߴ��մϴ�"));
			DeleteProgressBarDlg( pProgressBar );
			return;
		}
	

		nSuccessCnt++;
		m_pGrid->SelectGetNext();
	}
	
	// ���α׷����� ����
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );


	CString sResultMsg;	// ��� �޽���
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷῡ ��Ϲ�ȣ�� �����Ͽ����ϴ�.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("�����Ͻ� �� '%d'���� �ڷᰡ ��Ϲ�ȣ�� ���� �� �� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.") , nSelectCnt );
	else
		sResultMsg.Format( _T("���� �ڷ� : '%d'\r\n\r\n���� �ڷ� : '%d'\r\n")
						   _T("���� �ڷ� : '%d'(�ڷ���°� ��Ϲ�ȣ�� ���� �� �� ���� �����̱� ������ �۾��� �����Ͽ����ϴ�.)") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );
	

}

VOID CTRANSFER::PrintLabel()
{
	CTransferLabelPrint Dlg(this);
	Dlg.SetParentInfo(m_pDM,m_pGrid);
	Dlg.DoModal();

}

// ���α׷����� ����
CTransferProgress* CTRANSFER::CreateProgressBarDlg( INT nUpper )
{
	CTransferProgress* pProgressBar = new CTransferProgress(this);
	pProgressBar->Create( this );
	pProgressBar->BringWindowToTop();
	pProgressBar->CenterWindow();
	pProgressBar->ShowWindow( SW_SHOW );
	pProgressBar->InitRange(nUpper);

	return pProgressBar;
}

// ���α׷����� ����
INT CTRANSFER::DeleteProgressBarDlg( CTransferProgress *pProgressBarDlg )
{
	if( pProgressBarDlg != NULL ) 
		delete pProgressBarDlg;
	pProgressBarDlg = NULL;
	
	return 0;
}

/// ����Ϲ�ȣ �ο� DB �۾�
/// 1. å���� UPDATE
/// 3. TmpDM�� ����� ���� ��� - �� �ҽ��� ������±�.. �и��ұ�.. �Ѥ�a
INT CTRANSFER::RecordRegNoDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	
	INT ids;

	CString sRegNo;

	// 1. Column���� pTmpDM�� ����
	ids = MakeCopyDM( this , m_pDM , pTmpDM );
	if( ids < 0 ) return -2; 

	// 2. nDMIndex�� ������ pTmpDM�� ����
	ids = AddDM( m_pDM , pTmpDM , nDMIndex , -1 , this );
	if( ids < 0 ) return -3; 

	m_pDM->StartFrame();

	CString sBookRecKey ;
	ids = pTmpDM->GetCellData( _T("SB_åKEY") , 0 , sBookRecKey );
	if( ids < 0 ) return -4; 

	ids = MakeTmpRegNo(sRegNo);
	if( ids < 0 ) return -5; 

	// ������ ����
	// 1. å���� UPDATE
	m_pDM->InitDBFieldData();
	m_pDM->AddDBFieldData( _T("REG_NO") , _T("STRING") , sRegNo );

	m_pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	m_pDM->InitDBFieldData();

	// 1.1. TmpDM�� ���
	ids = pTmpDM->SetCellData( _T("SB_��Ϲ�ȣ") , sRegNo , 0 );
	if( ids < 0 ) return -6; 

	// 3. ������ DB�� Send~
	ids = m_pDM->SendFrame();
	if( ids < 0 ) return -7;
	

	return 0;
}

/// ����Ϲ�ȣ ���� DB �۾�
/// 1. å���� UPDATE
/// 3. TmpDM�� ����� ���� ��� - �� �ҽ��� ������±�.. �и��ұ�.. �Ѥ�a
INT CTRANSFER::CancelRegNoDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	INT ids;

	CString sRegNo;

	// 1. Column���� pTmpDM�� ����
	ids = MakeCopyDM( this , m_pDM , pTmpDM );
	if( ids < 0 ) return -1; 

	// 2. nDMIndex�� ������ pTmpDM�� ����
	ids = AddDM( m_pDM , pTmpDM , nDMIndex , -1 , this );
	if( ids < 0 ) return -2;

	m_pDM->StartFrame();

	CString sBookRecKey ;
	ids = pTmpDM->GetCellData( _T("SB_åKEY") , 0 , sBookRecKey );
	if( ids < 0 ) return -3;


	// ������ ����
	// 1. å���� UPDATE
	m_pDM->InitDBFieldData();
	m_pDM->AddDBFieldData( _T("REG_NO") , _T("STRING") , _T("") );

	m_pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	m_pDM->InitDBFieldData();

	// 1.1. TmpDM�� ���
	ids = pTmpDM->SetCellData( _T("SB_��Ϲ�ȣ") , _T("") , 0 );
	if( ids < 0 ) return -4;

	// 3. ������ DB�� Send~
	ids = m_pDM->SendFrame();
	if( ids < 0 ) return -5;

	return 0;

}


/// �ڷ� ���濡 ������ DM�� ������ ���� ���Ǵ� DM�� ����Ѵ�.
INT CTRANSFER::DBInsertUpdateDMProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	INT ids;

	ids = AddDM( pTmpDM , m_pDM , 0 , nDMIndex , this );
	if( ids < 0 ) return -1;


	ids = m_pDM->SetCellData( _T("SB_��Ϲ�ȣ"), pTmpDM->GetCellData( _T("SB_��Ϲ�ȣ"), 0 ), nDMIndex );
	if( ids < 0 ) return -2;	
	
	return 0;
}

/// 1. INDEX�κ��� �׸����� ���� ����
INT CTRANSFER::DBInsertUpdateGridProc( INT nDMIndex )
{
	INT ids;

	ids = InsertGridRow( this , m_pGrid , nDMIndex , nDMIndex );
	if( ids < 0 ) return -1; 

	return 0;
}

/// ����Ϲ�ȣ ���� & ���� DB�۾�
INT CTRANSFER::MakeTmpRegNo( CString &sRegNo )
{
	INT ids;

	CString sYear;
	CString sSql;
	CString sTailRegNo;

	CSeApi api(this);
	sYear = api.GetCurrentYear();


	// ������ ��Ϲ�ȣ ����
	//++2008.10.17 UPDATE BY PWR {{++
	// ���սý��� ����
	sSql.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
				 _T("WHERE KIND_CODE='TRG_NO' AND MAKE_YEAR='%s' AND MANAGE_CODE=UDF_MANAGE_CODE"), sYear );
	// sSql.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE='TRG_NO' AND MAKE_YEAR='%s'"),sYear);
	//--2008.10.17 UPDATE BY PWR --}}
	m_pDM_PRINT->GetOneValueQuery( sSql , sTailRegNo );

	// ������ ��Ϲ�ȣ�� �������� ���� ��� ����
	if( sTailRegNo.IsEmpty() )
	{
		sSql.Format( _T("INSERT INTO CO_LAST_NUMBER_TBL(REC_KEY,KIND_CODE,PUBLISH_FORM_CODE,MAKE_YEAR,LAST_NUMBER,CREATE_DATE,MANAGE_CODE) ")
											_T("VALUES(esl_seq.NextVal,'TRG_NO','CO','%s',1,SYSDATE,UDF_MANAGE_CODE);") , sYear );
		ids = m_pDM_PRINT->ExecuteQuery( sSql , 1 );
		if( ids < 0 ) return -2;

		sTailRegNo = _T("1");
	}

	sRegNo.Format( _T("%s%08s"),sYear,sTailRegNo);

	// ������ ��Ϲ�ȣ ++
	//++2008.10.17 UPDATE BY PWR {{++
	// ���սý��� ����
	sSql.Format(	_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%d ")
					_T("WHERE KIND_CODE='TRG_NO' AND MAKE_YEAR='%s' AND MANAGE_CODE=UDF_MANAGE_CODE;")
					, _ttoi(sTailRegNo)+1, sYear );
//	sSql.Format(	_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%d ")
//					_T("WHERE KIND_CODE='TRG_NO' AND MAKE_YEAR='%s';")
//					, _ttoi(sTailRegNo)+1 , sYear 
//					);
	//--2008.10.17 UPDATE BY PWR --}}

	ids = m_pDM_PRINT->ExecuteQuery( sSql , 1 );
	if( ids < 0 ) return -2; 

	return 0;
}

/// DM�� ADD���ִ� �Լ�
INT CTRANSFER::AddDM(	CESL_DataMgr *pSrcDM , CESL_DataMgr *pDstDM , INT nSrcIndex , INT nDstIndex , CESL_Mgr *pEslMgr )
{
	
	INT ids;

	INT nAliasCnt;
	nAliasCnt = pDstDM->RefList.GetCount();

	CString *sColAlias;
	sColAlias = new CString[nAliasCnt];
	
	ids = pDstDM->GetAllAlias( sColAlias , nAliasCnt );
	if( pDstDM == NULL ) return -1;

	if( nDstIndex == -1 )
	{
		pDstDM->InsertRow(-1);
		nDstIndex = pDstDM->GetRowCount()-1;
	}

	CString sResult;
	for( INT i = 0 ; i < nAliasCnt ; i++ )
	{
		ids = pSrcDM->GetCellData( sColAlias[i] , nSrcIndex , sResult );
		if( ids < 0 && (ids != -4012) )
		{
			CString sEMsg;
			sEMsg.Format( _T("sColAlias[i] : %s ") , sColAlias[i] );
			AfxMessageBox( sColAlias[i]);
		}
		if( ids < 0 && (ids != -4012) ) return -2;
		if( ids == -4012 ) sResult.Empty();
		
		ids = pDstDM->SetCellData( sColAlias[i] , sResult , nDstIndex );
		if( ids < 0 ) return -3;
	}

	delete []sColAlias;

	return 0;
}

// �׸��忡 DM�� index�� ���� �μ�Ʈ�Ѵ�.
INT CTRANSFER::InsertGridRow(  CESL_Mgr *pEslMgr , CESL_Grid *pGrid , INT nDMIndex , INT nGridIndex , INT nRowHeight )
{

	INT ids;

	INT nColCount = pGrid->GetColCount();

	if( nGridIndex == -1 )
		nGridIndex = pGrid->GetCount();

	CString sData;
	CESL_DataMgr *pDM;
	for( INT k = 0 ; k < nColCount ; k++ )
	{
		POSITION pos = pGrid->m_arrColumnInfo.FindIndex(k);
		CESL_GridColumnInfo *pColumnInfo = (CESL_GridColumnInfo*)pGrid->m_arrColumnInfo.GetAt(pos);

		if( pColumnInfo->strColumnDMAlias.IsEmpty() ) continue;
		if( pColumnInfo->strColumnDMFieldAlias.IsEmpty()) continue;

		pDM = pEslMgr->FindDM( pColumnInfo->strColumnDMAlias );
		if( pDM == NULL ) return -1;

		ids = pEslMgr->GetDataMgrData( pColumnInfo->strColumnDMAlias , pColumnInfo->strColumnDMFieldAlias , sData , nDMIndex );
		if( ids < 0 ) return -2;

		pGrid->SetAt( nGridIndex , k , sData );

	}

	if( nRowHeight != -1 ) 
		pGrid->SetRowHeight( nGridIndex+1 , nRowHeight );

	CString sIndex;
	sIndex.Format(_T("%d"),nGridIndex+1);
	pGrid->SetTextMatrix( nGridIndex+1 , 0 , sIndex );

	pGrid->m_nRealRowCount = pDM->GetRowCount();
	
	return 0;

}

INT CTRANSFER::MakeCopyDM( CESL_Mgr *pEslMgr , CESL_DataMgr *pSrcDM , CESL_DataMgr *pDstDM )
{
	INT ids;

	pDstDM->SetCONNECTION_INFO(GetDBConnection(pEslMgr->m_pInfo->MODE));
	pDstDM->TBL_NAME = pSrcDM->TBL_NAME;

	pDstDM->EXTRA_CONDITION = pSrcDM->EXTRA_CONDITION;
	pDstDM->CONDITION = pSrcDM->CONDITION;
	pDstDM->DB_MGR_ALIAS = pSrcDM->DB_MGR_ALIAS;
	

	pDstDM->InitDataMgrRef( pSrcDM->RefList.GetCount() );
	pDstDM->m_nCols = pSrcDM->RefList.GetCount();

	CString sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType;
	for( INT k = 0 ; k < pSrcDM->RefList.GetCount() ; k++ )
	{
		sFieldAlias = sFieldName = sFieldType = sInitValue = sDataType = _T("");
		ids = pSrcDM->GetColumnRefInfo( k , sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType );
		if( ids < 0 ) return -1;

		ids = pDstDM->SetDataMgrRef( k , sFieldAlias , sFieldName , sFieldType  , sInitValue  , sDataType );			
		if( ids < 0 ) return -1;
	}		
	
	return 0;
}

CString CTRANSFER::GetDBConnection(INT nMode)
{

	CString sPath;
	sPath = _T("");
	//++2008.09.05 DEL BY CJY {{++
//DEL 	if( nMode == 10000 || (nMode==30000) )
//DEL 	{
//DEL 		sPath = _T("..\\cfg\\Mobile\\MobileDB.cfg");
//DEL 	}
	//--2008.09.05 DEL BY CJY --}}
	CReadCfgFile ReadCfg;	
	ReadCfg.ReadCfgFile(sPath);

	return ReadCfg.m_sConnection;

}

/// pGrid�� �ڷ��� üũ�� �ڷ��� pDM�� sEqualFieldAlias�� ������ sCnt�� ���� 
INT CTRANSFER::GetDMEqualRemoveCnt( CESL_Mgr *pEslMgr , CESL_DataMgr *pDM , CString sEqualFieldAlias , CESL_Grid* pGrid , CString &sCnt )
{

	INT ids;

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();

	INT nSelectCnt = pGrid->SelectGetCount();

	CStringArray sUniqueValue;

	CString sValue;
	CString sData;
	bool IsEqual;
	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();

		IsEqual = FALSE;
		ids = pDM->GetCellData( sEqualFieldAlias , nIndex , sData );
		if( ids < 0 ) return -1;

		for( INT j = 0 ; j < sUniqueValue.GetSize() ; j++ )
		{
			sValue = sUniqueValue.GetAt(j);
			if( sValue.Compare(sData) == 0 )
				IsEqual = TRUE;

		}

		if( IsEqual )
		{
			pGrid->SelectGetNext();
			continue;
		}

		sUniqueValue.Add( sData );

		pGrid->SelectGetNext();
	}

	sCnt.Format( _T("%d") , sUniqueValue.GetSize() );

	return 0;
	
}

HBRUSH CTRANSFER::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

 