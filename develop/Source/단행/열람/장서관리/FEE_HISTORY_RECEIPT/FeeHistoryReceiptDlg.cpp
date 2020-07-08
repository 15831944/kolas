// FeeHistoryReceiptDlg.cpp : implementation file
// MAKE BY PWR

#include "stdafx.h"
#include "FeeEditHistory.h"
#include "FeeEditArrear.h"
#include "FeeEditLoanStop.h"
#include "FeeEditReCard.h"
#include "FeeEditReCard2.h"
#include "FeeHistoryReceiptDlg.h"
#include "FeeInsert.h"
#include "FeeInsertLogDlg.h"

// SBL
#include "SBL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFeeHistoryReceiptDlg dialog

CFeeHistoryReceiptDlg::CFeeHistoryReceiptDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CFeeHistoryReceiptDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeeHistoryReceiptDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pGrid = NULL;
}
CFeeHistoryReceiptDlg::~CFeeHistoryReceiptDlg()
{
}

VOID CFeeHistoryReceiptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeeHistoryReceiptDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFeeHistoryReceiptDlg, CDialog)
	//{{AFX_MSG_MAP(CFeeHistoryReceiptDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CFeeHistoryReceiptDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CFeeHistoryReceiptDlg)
	ON_EVENT(CFeeHistoryReceiptDlg, IDC_gridMAIN, -601 /* DblClick */, OnDblClickgridMAIN, VTS_NONE)
	ON_EVENT(CFeeHistoryReceiptDlg, IDC_gridMAIN, -600 /* Click */, OnClickgridMAIN, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeeHistoryReceiptDlg message handlers

BOOL CFeeHistoryReceiptDlg::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CFeeHistoryReceiptDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("FEE_HISTORY") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : FEE_HISTORY") );
		return FALSE;
	}

	m_pGrid = (CESL_Grid*)FindControl( _T("CM_FEE_HISTORY") , _T("MainGrid") );
	if( m_pGrid == NULL ) 
	{
		AfxMessageBox(	_T("CFeeHistoryReceiptDlg::OnInitDialog()\r\n")
						_T("-Reference Error::CM_FEE_HISTORY"), MB_ICONSTOP );
		return FALSE;
	}

	//JOB.2019.0017 ADD BY KYJ
	//�������� ���� ����,�α� ������ ����
	//-------------------------------------------------------------------------
	if(!m_pInfo->m_bUserInfoExportYN)
	{
		m_pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		m_pGrid->m_bPrivacyPrintLogRecord = TRUE;
		m_pGrid->m_strPrintInfo = _T("������ �����̷�");
	}
	//-------------------------------------------------------------------------

	m_pDM = FindDM( _T("DM_FEE_HISTORY") );
	if( !m_pDM ) 
	{
		AfxMessageBox(	_T("CFeeHistoryReceiptDlg::OnInitDialog()\r\n")
						_T("-Reference Error::DM_FEE_HISTORY"), MB_ICONSTOP );
		return FALSE;
	}

	CString sValue;
	GetManageValue( _T("����"), _T("����"), _T("��ü����"), _T("���������ް�����������"), m_sIsHolyDay );
	GetManageValue( _T("����"), _T("����"), _T("��ü����"), _T("��ü��"), sValue );
	m_nBasicArrear = _ttoi( sValue );
	GetManageValue( _T("����"), _T("����"), _T("�ηϰ���"), _T("�ηϿ�ü���뿩��"), m_sSupplement );

	// 2010.09.08 ADD BY KSJ : ���յ������� ��뿩��
	GetManageValue(_T("��Ÿ"), _T("����"), _T("���յ�������"), _T("���յ�������"), m_sIsUnityLoanService );
	m_sIsUnityLoanService.TrimLeft();	m_sIsUnityLoanService.TrimRight();
	m_sIsUnityLoanService.MakeUpper();
	if(_T("Y")!=m_sIsUnityLoanService) m_sIsUnityLoanService = _T("N");

	EnableThemeDialogTexture(GetSafeHwnd()); 

	return TRUE;
}

VOID CFeeHistoryReceiptDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	const INT term = 0;	
	CESL_Grid *pGrid = ( CESL_Grid * )GetDlgItem( IDC_gridMAIN );
	if( pGrid->GetSafeHwnd() )
		pGrid->MoveWindow( term, term, cx - ( term * 2 ), cy - ( term * 2 ) );	
}

// 2008.03.04 ADD BY PWR
// ** �̷��� �˻��Ѵ�. ( sQuery�� �ϼ��� ������ ���´�. )
VOID CFeeHistoryReceiptDlg::SearchData( CString sQuery )
{
	INT ids;
	// 1. �ɹ� ������ ����
	m_sQuery = sQuery;

	// 2. ���� ������ �˻��� �ϰ� DM���� ����
	InsertDM( sQuery );

	// 3. �׸��忡 ǥ��
	ids = ControlDisplay( _T("CM_FEE_HISTORY"), _T("MainGrid") );	
	if( ids < 0 )
	{
		CString sErrorMsg;
		sErrorMsg = _T("ControlDisplay Error::CM_FEE_HISTORY-MainGrid");
		if( ids == -1 )
			sErrorMsg = _T("ControlDisplay Error::'MainGrid' Grid Control�� ã�� �� ����");
		else if( ids == -1006 )
			sErrorMsg = _T("ControlDisplay Error::'CM_FEE_HISTORY' CM�� ã�� �� ����");
		AfxMessageBox( sErrorMsg, MB_ICONSTOP );
		return;
	}
}

// 2008.03.05 ADD BY PWR
// ** DM�� ����Ÿ�� �ִ´�. ( �˻����->TempDM->pDM )
VOID CFeeHistoryReceiptDlg::InsertDM( CString sQuery )
{
// 2008.04.22 UPDATE BY PWR
// ID�߰� ( �α�����ID�� �����ֵ��� DM�� �߰� )
	INT ids;

	BEGIN_SBL();

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 1. TEMP�� �˻���� �־��ش�.
	CESL_DataMgr *pTmpDM = FindDM( _T("DM_FEE_HISTORY_TMP") );
	if( !pTmpDM ) 
	{
		AfxMessageBox(	_T("CFeeHistoryReceiptDlg::InsertDM()\r\n")
						_T("-Reference Error::DM_FEE_HISTORY_TMP"), MB_ICONSTOP );
		return;
	}
	CESL_DataMgr *pDM = FindDM( _T("DM_FEE_HISTORY") );
	if( !pDM ) 
	{
		AfxMessageBox(	_T("CFeeHistoryReceiptDlg::InsertDM()\r\n")
						_T("-Reference Error::DM_FEE_HISTORY"), MB_ICONSTOP );
		return;
	}
	pDM->FreeData();

	ids = pTmpDM->RestructDataManager( sQuery );
	if( ids < 0 )
	{
		CString sErrorMsg;
		sErrorMsg = _T("RestructDataManager Error::�˻��̷�");
		if( ids == -4004 )
			sErrorMsg = _T("RestructDataManager Error::�˻��̷�\r\n-���ӵ�������");
		else if( ids == -4005 )
			sErrorMsg = _T("RestructDataManager Error::�˻��̷�\r\n-��������� �����߻�");
		else if( ids == -4006 )
			sErrorMsg = _T("RestructDataManager Error::�˻��̷�\r\n-������ �ʹ�����");
		AfxMessageBox( sErrorMsg, MB_ICONSTOP );
		return;
	}

	END_SBL(pTmpDM->GetRowCount());

	if( pTmpDM->GetRowCount() <= 0 ) return;

////////////////////////////////////////////////////////////////////////////////////////////////////	
/// 2. TEMP���� ���� DM���� �˻��� ����� �����Ѵ�.
	CString sData[24];
	CString sAlias[] = { _T("�����ڹ�ȣ") , _T("�����ڸ�")		, _T("ȸ������")	, _T("�Ҽ�")		,
						 _T("����")		  , _T("�������������"), _T("����������")	, _T("���αݾ�")	,
						 _T("��������")	  , _T("��������")		, _T("�ݳ�������")	, _T("�ݳ�����")	, 
						 _T("��Ϲ�ȣ")	  , _T("����")			, _T("�ڷ��")		, _T("RECEIPT_KEY")	, 
						 _T("ȸ��������") , _T("�α���ID")		, _T("PAYMENT_KEY")	, _T("LOAN_INFO_KEY")	, 
						 _T("����������") , _T("�ұ��Է�����")	, _T("��������") };

	for(INT i = 0; i< pTmpDM->GetRowCount(); i++)
	{
		pDM->InsertRow(-1);
		for(INT j = 0; j < pTmpDM->GetColCount(); j++ )
		{
			pTmpDM->GetCellData(i, j, sData[j] );
			pDM->SetCellData( sAlias[j], sData[j], i );
		}
	}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 3. �ڵ� �������� ����
	//this->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM, _T("����Ҽ�����"), _T("�Ҽ�") );
	//this->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM, _T("������������"), _T("����") );
	//// 2008.04.04 ADD BY PWR
	//// �ڷ�� �ڵ带 �ڷ�� �������� ����
	//// �ڷ���� ��ü�Ḹ �����ֵ��� �Ѵ�.
	//this->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM, _T("�ڷ�Ǳ���"), _T("�ڷ��") );
	
	for( i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		// 2009.03.04 ADD BY PWR : �ڵ� -> ���� ���� FOR������ �� ó���ϵ��� 
		CString sCode, sDesc;
		// �Ҽ�
		pDM->GetCellData( _T("�Ҽ�"), i, sCode);
		this->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("����Ҽ�����"), sCode, sDesc);
		pDM->SetCellData( _T("�Ҽ�"), sDesc, i);
		// ����
		sCode.Empty();	sDesc.Empty();
		pDM->GetCellData( _T("����"), i, sCode);
		this->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("������������"), sCode, sDesc);
		pDM->SetCellData( _T("����"), sDesc, i);
		// �ڷ��
		sCode.Empty();	sDesc.Empty();
		pDM->GetCellData( _T("�ڷ��"), i, sCode);
		this->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("�ڷ�Ǳ���"), sCode, sDesc);
		pDM->SetCellData( _T("�ڷ��"), sDesc, i);
		// ��������
		sCode.Empty();	sDesc.Empty();
		pDM->GetCellData( _T("��������"), i, sCode);
		this->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("��������"), sCode, sDesc);
		pDM->SetCellData( _T("�������м���"), sDesc, i);
		// �����ڹ�ȣ
		sCode.Empty(); sDesc.Empty();
		pDM->GetCellData( _T("�����ڹ�ȣ"), i, sCode );
		// 2010.09.08 ADD BY KSJ : ���յ������� ��� �� ��� ǥ�ù�� ����
//DEL 		CString strBuf = sCode;
//DEL 		if( 13 <= sCode.GetLength() )
//DEL 		{
//DEL 			strBuf = sCode.Mid(0,6) + _T("-") + sCode.Mid(6);
//DEL 		}
//DEL 		ids = CLocCommonAPI::CivilNoCheck( strBuf );
//DEL 		if( 0 == ids )
//DEL 		{
//DEL 			sDesc.Format(_T("%s**%s******"),strBuf.Left(4),strBuf.Mid(7,1));
//DEL 		}
//DEL 		else
//DEL 		{
//DEL 			sDesc = sCode;
//DEL 		}
		CLocCommonAPI::ChangeSecuriyUserNo( this , sCode , m_sIsUnityLoanService );
		pDM->SetCellData( _T("�����ڹ�ȣ����"), sCode, i );

		CString sInferiorClass;
		CString sFeeClass;
		CString sPayment;
		CString sCardClass;
		CString sShelf;
		CString sRetroYN;

		pDM->GetCellData( _T("ȸ������")	, i, sInferiorClass );
		pDM->GetCellData( _T("����������")	, i, sFeeClass );
		pDM->GetCellData( _T("���αݾ�")	, i, sPayment );
		pDM->GetCellData( _T("ȸ��������")	, i, sCardClass );
		pDM->GetCellData( _T("�ڷ��")		, i, sShelf ); 
		pDM->GetCellData( _T("�ұ��Է�����"), i, sRetroYN ); 

		// 3.1 ȸ������ ( 0:����, 1:�ҷ�, 2:����, 3:�������н� )
		if( _ttoi(sInferiorClass) == 0 )
			sInferiorClass = _T("����ȸ��");
		else if( _ttoi(sInferiorClass) == 1 )
			sInferiorClass = _T("��������");
		else if( _ttoi(sInferiorClass) == 2 )
			sInferiorClass = _T("����ȸ��");
		else if( _ttoi(sInferiorClass) == 3 )
			sInferiorClass = _T("�������н�");

		// 3.2 ���������� ( 0:��ü��, 1:ȸ������߱�, 2:�������������� )
		if( _ttoi(sFeeClass) == 0 )
		{
			sFeeClass = _T("��ü��");
		}
		else if( _ttoi(sFeeClass) == 1 )
		{
			if( !sCardClass.IsEmpty() )
			{
				sFeeClass.Format( _T("ȸ������߱�\n(%s)"), sCardClass );
			}
			else
			{
				sFeeClass = _T("ȸ������߱�");
			}
			pDM->SetCellData( _T("�ڷ��")	, _T(""), i );
		}
		else if( _ttoi(sFeeClass) == 2 )
		{
			sFeeClass = _T("��������������");
			pDM->SetCellData( _T("�ڷ��")	, _T(""), i );
		}

		// 3.3 ���αݾ� õ������ ǥ��
		SetMoneyFormat( sPayment );

		pDM->SetCellData( _T("ȸ������")	, sInferiorClass, i );
		pDM->SetCellData( _T("����������")	, sFeeClass		, i );
		pDM->SetCellData( _T("���αݾ�")	, sPayment		, i );

		// 2009.03.06 ADD BY PWR : RETRO_YN�� Y���ڷ�� �ұ��Է��ڷ��̴�.
		// �ұ��Է��� �̿��� Ű�� ������ �ڷ���� �˼������Ƿ�
		if(sRetroYN == _T("Y"))
		{
			pDM->SetCellData( _T("�ڷ��")	, _T(""), i );
		}
	}
}

// 2008.03.05 ADD BY PWR
// ** õ������ ǥ��
VOID CFeeHistoryReceiptDlg::SetMoneyFormat(CString &sMoney)
{
	sMoney.Replace(_T(","), _T(""));

	CString strBuf = sMoney;
	strBuf.Replace(_T(","), _T(""));

	INT nSize = (strBuf.GetLength() * 3) + 100;
	_TCHAR *pszFormattedNumber = new _TCHAR [nSize];

	NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 0 };

	::GetNumberFormat(NULL, NULL, strBuf, &nFmt, pszFormattedNumber, nSize-1);

	sMoney = pszFormattedNumber;

	if (pszFormattedNumber)
		delete [] pszFormattedNumber;
}

// 2008.03.06 ADD BY PWR
// ** ������
INT CFeeHistoryReceiptDlg::PrintList()
{
EFS_BEGIN

	INT ids;
////////////////////////////////////////////////////////////////////////////////////////////////////
/// 1. ��� DM �� ��� DM �ʱ�ȭ 
	CESL_DataMgr *pSrcDM = FindDM( _T("DM_FEE_HISTORY") );
	if( pSrcDM == NULL ) 
	{
		AfxMessageBox(	_T("CFeeHistoryReceiptDlg::PrintList()\r\n")
						_T("-Reference Error::DM_FEE_HISTORY"), MB_ICONSTOP );
		return -1;
	}
		
	if( pSrcDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("����� ����� �������� �ʽ��ϴ�.") );
		return -2;
	}
	
	CESL_DataMgr *pPrintDM = FindDM( _T("DM_FEE_HISTORY_PRINT") );
	if( pPrintDM == NULL )
	{
		AfxMessageBox(	_T("CFeeHistoryReceiptDlg::PrintList()\r\n")
						_T("-Reference Error::DM_FEE_HISTORY_PRINT"), MB_ICONSTOP );
		return -3;
	}
	pPrintDM->FreeData();

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 2. ��� �ʵ� ����
	const INT nCopyCnt = 15;
	TCHAR *sField[nCopyCnt][2] =
	{
		{ _T("�����ڹ�ȣ����")	, _T("�����ڹ�ȣ")		},
		{ _T("�����ڸ�")		, _T("�����ڸ�")		},
		{ _T("ȸ������")		, _T("ȸ������")		},
		{ _T("�Ҽ�")			, _T("�Ҽ�")			},
		{ _T("����")			, _T("����")			},
		{ _T("�������������")	, _T("�������������")	},
		{ _T("����������")		, _T("����������")		},
		{ _T("���αݾ�")		, _T("���αݾ�")		},
		{ _T("��������")		, _T("��������")		},
		{ _T("��������")		, _T("��������")		},
		{ _T("�ݳ�������")		, _T("�ݳ�������")		},
		{ _T("�ݳ�����")		, _T("�ݳ�����")		},
		{ _T("��Ϲ�ȣ")		, _T("��Ϲ�ȣ")		}, 
		{ _T("����")			, _T("����")			},
		{ _T("�ڷ��")			, _T("�ڷ��")			}
	};
	
////////////////////////////////////////////////////////////////////////////////////////////////////
/// 3. ��¸�� ����
	// ������ �ο찡 ���ٸ� ��ü ���
	m_pGrid->SelectMakeList();
	m_pGrid->SelectGetHeadPosition();
	
	INT nMode = 0;
	INT nSelectCnt = m_pGrid->SelectGetCount();
	
	if( nSelectCnt > 0 )
		nMode = 1;
	else
		nSelectCnt = pSrcDM->GetRowCount();
	
////////////////////////////////////////////////////////////////////////////////////////////////////
// 4. pSrcDM -> pPrintDM���� ����
	CString sData;
	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 0 )
			nIndex = i;
		else
			nIndex = m_pGrid->SelectGetIdx();		

		pPrintDM->InsertRow(-1);
		
		sData.Format( _T("%d") , i+1 );
		ids = pPrintDM->SetCellData( _T("��ȣ") , sData , i);
		if( ids < 0 ) AfxMessageBox( _T("PrintList") );
		
		for( INT j = 0 ; j < nCopyCnt ; j++ )
		{
			ids = pSrcDM->GetCellData( sField[j][0] , nIndex , sData );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , nIndex , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) AfxMessageBox( _T("PrintList") );
			
			ids = pPrintDM->SetCellData( sField[j][1] , sData , i );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , i , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) AfxMessageBox( _T("PrintList") );
		}
		if( nMode == 1)
			m_pGrid->SelectGetNext();
	}
	
////////////////////////////////////////////////////////////////////////////////////////////////////
/// 5. SIreport ���
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	if (!pSIReportManager) {
		AfxMessageBox( _T("Report Manager�� �������� ���Ͽ����ϴ�.") );
		return -3;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB( _T("�����ᳳ���̷�") );
	if (ids < 0) {
		AfxMessageBox( _T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�.") );
		return -4;
	}
	//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰�
	pSIReportManager->m_bPrivacyPrintLogRecord = TRUE;
	pSIReportManager->SetDataMgr(0, pPrintDM , 0);
	
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	
	return 0;
	
EFS_END
return -1;
}

// 2008.06.02 ADD BY PWR
// ** �����̷º���
INT CFeeHistoryReceiptDlg::PaymentEditHistory()
{
EFS_BEGIN

	CFeeEditHistory dlg(this);
	// 2010.09.08 ADD BY KSJ : ���յ������� ��뿩��
	dlg.m_sIsUnityLoanService = m_sIsUnityLoanService;
	dlg.DoModal();

	return 0;

EFS_END
return -1;
}

// 2008.05.29 ADD BY PWR
// ** ���αݾ� ����
INT CFeeHistoryReceiptDlg::PaymentEdit()
{
EFS_BEGIN
	
	// �˻��� �����Ͱ� �ִ��� �˻��Ѵ�.
	if( m_pDM->GetRowCount() < 1 ) return 0;

	// ��������� �ڷ� �ϳ��� �����ߴ��� �˻��Ѵ�.
	m_pGrid->SelectMakeList();
	m_pGrid->SelectGetHeadPosition();
	INT nSelectCnt = m_pGrid->SelectGetCount();
	if( nSelectCnt < 1 )
	{
		AfxMessageBox( _T("������ �ڷḦ ������ �ֽʽÿ�."), MB_ICONINFORMATION );
		return 0;
	}
	else if( nSelectCnt > 1 )
	{
		AfxMessageBox( _T("������ �ڷ� �ϳ��� ������ �ֽʽÿ�."), MB_ICONINFORMATION );
		return 0;
	}

	// ������ ������ ����
	m_nIndex = m_pGrid->SelectGetIdx();
	CString sFeeClass, sCardClass;
	m_pDM->GetCellData( _T("����������"), m_nIndex, sFeeClass );
	m_pDM->GetCellData( _T("ȸ��������"), m_nIndex, sCardClass );

	// 1. ȸ������߱� ����
	if( !sCardClass.IsEmpty() )
	{
		EditReCard();
		return 0;
	}
	// 2. ��ü�� ����
	else if( sFeeClass == _T("��ü��") )
	{
		EditArrear();
		return 0;
	}
	// 3. �������������� ����
	else if( sFeeClass == _T("��������������") )
	{
		EditLoanStopDateCancel();
		return 0;
	}
	// 2009.03.04 ADD BY PWR : �ұ��Է��߰�
	else
	{
		EditReCard2();
		return 0;
	}
	return 0;

EFS_END
return -1;
}

// 2008.05.29 ADD BY PWR
// ** ��ü�� ����
INT CFeeHistoryReceiptDlg::EditArrear()
{
EFS_BEGIN

	INT ids;
	CString sErrorMsg;

	CFeeEditArrear dlg(this);
	dlg.m_nIndex = m_nIndex;
	dlg.DoModal();
	if( dlg.m_bSuccess == TRUE )
	{
		// ���� ������ �˻��� �ϰ� DM���� ����
		InsertDM( m_sQuery );
		// �׸��忡 ǥ��
		ids = ControlDisplay( _T("CM_FEE_HISTORY"), _T("MainGrid") );	
		if( ids < 0 )
		{
			sErrorMsg = _T("ControlDisplay Error::CM_FEE_HISTORY-MainGrid");
			if( ids == -1 )
				sErrorMsg = _T("ControlDisplay Error::'MainGrid' Grid Control�� ã�� �� ����");
			else if( ids == -1006 )
				sErrorMsg = _T("ControlDisplay Error::'CM_FEE_HISTORY' CM�� ã�� �� ����");
			AfxMessageBox( sErrorMsg, MB_ICONSTOP );
			return -1;
		}

		AfxMessageBox( _T("�����Ǿ����ϴ�.") );
	}

	return 0;

EFS_END
return -1;
}

// 2008.05.29 ADD BY PWR
// ** ���������� ���� ������ ����
INT CFeeHistoryReceiptDlg::EditLoanStopDateCancel()
{
EFS_BEGIN

	INT ids;
	CString sErrorMsg;

	CFeeEditLoanStop dlg(this);
	dlg.m_nIndex = m_nIndex;
	dlg.DoModal();
	if( dlg.m_bSuccess == TRUE )
	{
		// ���� ������ �˻��� �ϰ� DM���� ����
		InsertDM( m_sQuery );
		// �׸��忡 ǥ��
		ids = ControlDisplay( _T("CM_FEE_HISTORY"), _T("MainGrid") );	
		if( ids < 0 )
		{
			sErrorMsg = _T("ControlDisplay Error::CM_FEE_HISTORY-MainGrid");
			if( ids == -1 )
				sErrorMsg = _T("ControlDisplay Error::'MainGrid' Grid Control�� ã�� �� ����");
			else if( ids == -1006 )
				sErrorMsg = _T("ControlDisplay Error::'CM_FEE_HISTORY' CM�� ã�� �� ����");
			AfxMessageBox( sErrorMsg, MB_ICONSTOP );
			return -1;
		}

		AfxMessageBox( _T("�����Ǿ����ϴ�.") );
	}

	return 0;

EFS_END
return -1;
}

// 2008.05.29 ADD BY PWR
// ** ȸ������߱� ������ ����
INT CFeeHistoryReceiptDlg::EditReCard()
{
EFS_BEGIN

	INT ids;
	CString sErrorMsg;

	CFeeEditReCard dlg(this);
	dlg.m_nIndex = m_nIndex;
	dlg.DoModal();
	if( dlg.m_bSuccess == TRUE )
	{
		// ���� ������ �˻��� �ϰ� DM���� ����
		InsertDM( m_sQuery );
		// �׸��忡 ǥ��
		ids = ControlDisplay( _T("CM_FEE_HISTORY"), _T("MainGrid") );	
		if( ids < 0 )
		{
			sErrorMsg = _T("ControlDisplay Error::CM_FEE_HISTORY-MainGrid");
			if( ids == -1 )
				sErrorMsg = _T("ControlDisplay Error::'MainGrid' Grid Control�� ã�� �� ����");
			else if( ids == -1006 )
				sErrorMsg = _T("ControlDisplay Error::'CM_FEE_HISTORY' CM�� ã�� �� ����");
			AfxMessageBox( sErrorMsg, MB_ICONSTOP );
			return -1;
		}

		AfxMessageBox( _T("�����Ǿ����ϴ�.") );
	}

	return 0;

EFS_END
return -1;
}

// 2009.03.04 ADD BY PWR
// ** ȸ������߱� ������ ���� (�ұ��ԷµȰ�)
INT CFeeHistoryReceiptDlg::EditReCard2()
{
EFS_BEGIN

	INT ids;
	CString sErrorMsg;

	CFeeEditReCard2 dlg(this);
	dlg.m_nIndex = m_nIndex;
	dlg.DoModal();
	if( dlg.m_bSuccess == TRUE )
	{
		// ���� ������ �˻��� �ϰ� DM���� ����
		InsertDM( m_sQuery );
		// �׸��忡 ǥ��
		ids = ControlDisplay( _T("CM_FEE_HISTORY"), _T("MainGrid") );	
		if( ids < 0 )
		{
			sErrorMsg = _T("ControlDisplay Error::CM_FEE_HISTORY-MainGrid");
			if( ids == -1 )
				sErrorMsg = _T("ControlDisplay Error::'MainGrid' Grid Control�� ã�� �� ����");
			else if( ids == -1006 )
				sErrorMsg = _T("ControlDisplay Error::'CM_FEE_HISTORY' CM�� ã�� �� ����");
			AfxMessageBox( sErrorMsg, MB_ICONSTOP );
			return -1;
		}

		AfxMessageBox( _T("�����Ǿ����ϴ�.") );
	}

	return 0;

EFS_END
return -1;
}

/*
@Create
 - 2009.03.03 BY PWR
@Description
 - ������ �ұ��Է�
@Parameter
 - 
@Return Value
 - 
@Mini Spec
 1. 
*/
INT CFeeHistoryReceiptDlg::FeeInsertProc()
{
	CFeeInsert dlg(this);
	// 2010.09.08 ADD BY KSJ : ���յ������� ��뿩��
	dlg.m_sIsUnityLoanService = m_sIsUnityLoanService;
	dlg.pMain = this->pMain;
	dlg.pParentMgr = (CESL_Mgr*)this;
	dlg.m_pInfo = this->m_pInfo;
	if( IDOK == dlg.DoModal() )
	{
		CFeeInsertLogDlg logDlg(this);
		logDlg.pMain = this->pMain;
		logDlg.pParentMgr = (CESL_Mgr*)this;
		logDlg.m_pInfo = this->m_pInfo;
		logDlg.m_strFeeClass = dlg.m_strFeeClass;
		logDlg.m_strPaymentDate = dlg.m_strPaymentDate;
		logDlg.m_strMoneyPaid = dlg.m_strMoneyPaid;
		logDlg.m_strUserKey = dlg.m_strUserKey;
		logDlg.m_strUserName = dlg.m_strUserName;
		logDlg.m_strUserNo = dlg.m_strUserNo;
		logDlg.m_strUserNoDesc = dlg.m_strUserNoDesc;
		if( IDOK == logDlg.DoModal() )
		{
			AfxMessageBox(_T("�����Ḧ �Է��Ͽ����ϴ�."));
		}
	}

	return 0;
}

HBRUSH CFeeHistoryReceiptDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	
	return hbr; 
}

void CFeeHistoryReceiptDlg::OnDblClickgridMAIN() {}
void CFeeHistoryReceiptDlg::OnClickgridMAIN() {}
