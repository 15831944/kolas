// BO_REG_EPC_SEARCH.cpp : implementation file
//

#include "stdafx.h"
#include "BO_REG_EPC_SEARCH.h"


#include "../../../����/BO_ACQ_GROUP_MANAGER/DivisionNo_List_Dlg.h"
#include "../../../����/BO_ACQ_GROUP_MANAGER/RequisitionNo_List_Dlg.h"
#include "../../../����/BO_ACQ_GROUP_MANAGER/GroupCollection.h"

//==============================================
//2008.07.11 ADD BY PJW : �����ϴܿ� ���¹�LOG���
#include "SBL.H"
//==============================================

#include "Imm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_REG_EPC_SEARCH dialog

CBO_REG_EPC_SEARCH::CBO_REG_EPC_SEARCH(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_REG_EPC_SEARCH)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_REG_EPC_SEARCH::~CBO_REG_EPC_SEARCH()
{
}

VOID CBO_REG_EPC_SEARCH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_REG_EPC_SEARCH)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_REG_EPC_SEARCH, CDialog)
	//{{AFX_MSG_MAP(CBO_REG_EPC_SEARCH)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
	ON_BN_CLICKED(IDC_BTN_SEQ_INFO, OnBtnSeqInfo)
	ON_CBN_SELCHANGE(IDC_CB_EPCDATA_ACQ_CODE, OnSelchangeCbEpcdataAcqCode)
	ON_BN_CLICKED(IDC_BTN_CLEAR, OnBtnClear)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDT_EPCDATA_REG_NO1, OnSetfocusEdtEpcdataRegNo1)
	ON_EN_SETFOCUS(IDC_EDT_EPCDATA_REG_NO2, OnSetfocusEdtEpcdataRegNo2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_REG_EPC_SEARCH message handlers

BOOL CBO_REG_EPC_SEARCH::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( 0 > InitESL_Mgr(_T("BO_BOOK_TRADE_REG_SEARCH")) )
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	this->SetCONNECTION_INFO( this->m_pInfo->CONNECTION_INFO );

	ChangeScreenForAcqCode();

	// KOL.RED.2018.220 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//��Ϲ�ȣ_����|E|1124|||||||
	//��Ϲ�ȣ_����|E|1325|||||||
	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_BOOK_TRADE_SEARCH"), _T("��Ϲ�ȣ_����"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_BOOK_TRADE_SEARCH"), _T("��Ϲ�ȣ_����"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}
	// KOL.RED.2018.220 ---------------------------------------------------------------------------	

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_REG_EPC_SEARCH::OnBtnClose() 
{
	this->OnCancel();
}

VOID CBO_REG_EPC_SEARCH::OnBtnSearch() 
{
EFS_BEGIN

	SearchRegEpcList();

EFS_END
}

VOID CBO_REG_EPC_SEARCH::OnBtnSeqInfo() 
{
EFS_BEGIN

	ShowSeqInfo();

EFS_END
}

VOID CBO_REG_EPC_SEARCH::OnSelchangeCbEpcdataAcqCode() 
{
EFS_BEGIN

	ChangeScreenForAcqCode();

EFS_END
}

VOID CBO_REG_EPC_SEARCH::OnBtnClear() 
{
EFS_BEGIN

	ClearForAllControl();

EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// CBO_REG_EPC_SEARCH func

/////////////////////////////////////////////////////////////////////////////
// [Func Desc]
//  ��Ʈ�ѿ� �Էµ� ������ Where���� �����Ѵ�.
// [Mini Spec]
//  1. ��Ʈ�ѿ� �Էµ� ���� ����
//  2. ���� ����
/////////////////////////////////////////////////////////////////////////////
INT CBO_REG_EPC_SEARCH::MakeWhereFromControl( CString &strGetWhere )
{
EFS_BEGIN

	/////////////////////////////////////////////////////////////////////////
	// Declare Variable
	/////////////////////////////////////////////////////////////////////////
	INT		ids;
	CString strCMAlias;
	const INT nControlCnt = 10;
	CString strControlAlias[nControlCnt] = {
		_T("�ڷ����"),
		_T("�Լ�����"),
		_T("���Գ⵵"),
		_T("��������"),
		_T("������ȣ"),
		_T("�۾���ȣ"),
		_T("���γ⵵"),
		_T("���ι�ȣ"),
		_T("��Ϲ�ȣ_����"),
		_T("��Ϲ�ȣ_����")
	};
	CString strControlData[nControlCnt];
	CString strBuf;
	CString strError;

	/////////////////////////////////////////////////////////////////////////
	// 1. ��Ʈ�ѿ� �Էµ� ���� ����
	/////////////////////////////////////////////////////////////////////////
	strCMAlias = _T("CM_BO_BOOK_TRADE_SEARCH");
	
	for( INT i=0;i<nControlCnt;i++ )
	{
		ids = GetControlData( strCMAlias, strControlAlias[i], strControlData[i] );
		if( 0 > ids )
		{			
			strError.Format( _T("CBO_REG_EPC_SEARCH::MakeWhereFromControl() ERROR!![%s:%d]"), strControlAlias[i] );
			AfxMessageBox( strError );
			return ids;
		}

		strControlData[i].TrimLeft();
		strControlData[i].TrimRight();
	}
	
	/////////////////////////////////////////////////////////////////////////
	// 2. ���� ����
	/////////////////////////////////////////////////////////////////////////
	strGetWhere.Empty();

	/// �ڷ����
	if( 0 < strControlData[0].GetLength() )
	{
		if( _T("��ü") == strControlData[0] )
		{
		}
		else if( _T("�������ڷ�") == strControlData[0] )
		{
			strBuf = _T(" BB.WORKING_STATUS < 'BOL112N' ");
		}
		else if( _T("�����ڷ�") == strControlData[0] )
		{
			strBuf = _T(" BB.WORKING_STATUS > 'BOL111O' ");
		}

		if( 0 < strGetWhere.GetLength() )
		{
			strGetWhere += _T(" AND ");
		}

		strGetWhere += strBuf;
	}

	/// �Լ�����
	if( 0 < strControlData[1].GetLength() )
	{
		strBuf = _T(" BB.ACQ_CODE='") + strControlData[1] + _T("' ");

		if( 0 < strGetWhere.GetLength() )
		{
			strGetWhere += _T(" AND ");
		}

		strGetWhere += strBuf;
	}

	/// ���Գ⵵
	if( 0 < strControlData[2].GetLength() )
	{
		strBuf = _T(" BB.ACQ_YEAR='") + strControlData[2] + _T("' ");

		if( 0 < strGetWhere.GetLength() )
		{
			strGetWhere += _T(" AND ");
		}

		strGetWhere += strBuf;
	}

	/// ��������
	if( 0 < strControlData[3].GetLength() )
	{
		strBuf = _T(" IB.WORK_CODE='") + strControlData[3] + _T("' ");

		if( 0 < strGetWhere.GetLength() )
		{
			strGetWhere += _T(" AND ");
		}

		strGetWhere += strBuf;
	}

	if( 0 < strGetWhere.GetLength() )
	{
		strGetWhere += _T(" AND ");
	}
	strGetWhere += _T("IB.MANAGE_CODE = UDF_MANAGE_CODE");


	/// ������ȣ && �۾���ȣ
	if( 0 < strControlData[4].GetLength() )
	{
		if( 0 < strControlData[5].GetLength() && _T("�������") != strControlData[5] )
		{
			strBuf = _T(" BB.ACQ_KEY IN ( SELECT REC_KEY FROM BO_PURCHASEINFO_TBL WHERE ORDER_SEQ_NO IN (") + strControlData[4] + _T(") AND ORDER_WORK_NO = ") + strControlData[5] + _T(") ");
		}
		else
		{
			strBuf = _T(" BB.ACQ_KEY IN ( SELECT REC_KEY FROM BO_PURCHASEINFO_TBL WHERE ORDER_SEQ_NO IN (") + strControlData[4] + _T(") ) ");
		}

		if( 0 < strGetWhere.GetLength() )
		{
			strGetWhere += _T(" AND ");
		}

		strGetWhere += strBuf;
	}

	/// ���γ⵵ && ���ι�ȣ	
	if( 0 < strControlData[6].GetLength() && 0 < strControlData[7].GetLength() )
	{
		strBuf = _T(" CA.ACCESSION_REC_MAKE_YEAR ='") + strControlData[6] + _T("' AND CA.ACCESSION_REC_NO =") + strControlData[7] + _T(" ");

		if( 0 < strGetWhere.GetLength() )
		{
			strGetWhere += _T(" AND ");
		}

		strGetWhere += strBuf;
	}

	/// ��Ϲ�ȣ ���� - ����
	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================


	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	if( 2 < strControlData[8].GetLength() )
// 	{
// 		CString strRegCode,strRegNo,strBuf;
// 		if( 12 > strControlData[8].GetLength() )
// 		{
// 			strRegCode = strControlData[8].Mid( 0, 2 );
// 			strRegNo   = strControlData[8].Mid( 2 );
// 			
// 			for( INT i=0;i<(10-strRegNo.GetLength());i++ )
// 			{
// 				strBuf = _T("0") + strBuf;
// 			}
// 
// 			strControlData[8] = strRegCode + strBuf + strRegNo;
// 		}
// 
// 		if( 2 < strControlData[9].GetLength() )
// 		{
// 			if( 12 > strControlData[9].GetLength() )
// 			{
// 				strRegCode = strControlData[9].Mid( 0, 2 );
// 				strRegNo   = strControlData[9].Mid( 2 );
// 				strBuf     = _T("");
// 
// 				for( INT i=0;i<(10-strRegNo.GetLength());i++ )
// 				{
// 					strBuf = _T("0") + strBuf;
// 				}
// 
// 				strControlData[9] = strRegCode + strBuf + strRegNo;
// 			}
// 
// 			strBuf = _T(" BB.REG_NO BETWEEN '") + strControlData[8] + _T("' AND '") + strControlData[9] + _T("' ");
// 		}
// 		else
// 		{
// 			strBuf = _T(" BB.REG_NO = '") + strControlData[8] + _T("' ");
// 		}

	if( nRegCodeLength < strControlData[8].GetLength() )
	{
		CString strRegCode,strRegNo,strBuf;
		if( 12 > strControlData[8].GetLength() )
		{
			strRegCode = strControlData[8].Mid( 0, nRegCodeLength );
			strRegNo   = strControlData[8].Mid( nRegCodeLength );
			strControlData[8] = m_pInfo->MakeRegNo(strRegCode, strRegNo);			
		}

		if( nRegCodeLength < strControlData[9].GetLength() )
		{
			if( 12 > strControlData[9].GetLength() )
			{
				strRegCode = strControlData[9].Mid( 0, nRegCodeLength );
				strRegNo   = strControlData[9].Mid( nRegCodeLength );
				strBuf     = _T("");

				strControlData[9] = m_pInfo->MakeRegNo(strRegCode, strRegNo);
			}

			strBuf = _T(" BB.REG_NO BETWEEN '") + strControlData[8] + _T("' AND '") + strControlData[9] + _T("' ");
		}
		else
		{
			strBuf = _T(" BB.REG_NO = '") + strControlData[8] + _T("' ");
		}
	//=====================================================
	

		if( 0 < strGetWhere.GetLength() )
		{
			strGetWhere += _T(" AND ");
		}

		strGetWhere += strBuf;
	}

	return 0;

EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [Func Desc]
//  ���� ���� �ڵ尪�� ���� ȭ�� ����
/////////////////////////////////////////////////////////////////////////////
INT CBO_REG_EPC_SEARCH::ChangeScreenForAcqCode()
{
EFS_BEGIN
	
	/////////////////////////////////////////////////////////////////////////
	// Declare Variable
	/////////////////////////////////////////////////////////////////////////
	INT		ids;
	CString strAcqCode;

	/////////////////////////////////////////////////////////////////////////
	//
	/////////////////////////////////////////////////////////////////////////
	ids = GetControlData( _T("CM_BO_BOOK_TRADE_SEARCH"), _T("�Լ�����"), strAcqCode );
	if( 0 > ids )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::ChangeScreenForAcqCode() ERROR!!") );
		return ids;
	}
	
	if( _T("1") == strAcqCode )
	{		
		( GetDlgItem(IDC_STA_EPCDATA_SEQ_NO) )->SetWindowText( _T("������ȣ") );
		( GetDlgItem(IDC_EDT_EPCDATA_SEQ_NO) )->EnableWindow( TRUE );
		( GetDlgItem(IDC_STA_EPCDATA_WORK_NO) )->SetWindowText( _T("�۾���ȣ") );
		( GetDlgItem(IDC_STA_EPCDATA_WORK_NO) )->ShowWindow( SW_SHOW );
		( GetDlgItem(IDC_CB_EPCDATA_WORK_NO) )->ShowWindow( SW_SHOW );
		( GetDlgItem(IDC_BTN_SEQ_INFO) )->ShowWindow( SW_SHOW );
		( GetDlgItem(IDC_BTN_SEQ_INFO) )->SetWindowText( _T("������ȣ����") );
	}
	else if( _T("2") == strAcqCode )
	{
		( GetDlgItem(IDC_STA_EPCDATA_SEQ_NO) )->SetWindowText( _T("�۾���ȣ") );
		( GetDlgItem(IDC_EDT_EPCDATA_SEQ_NO) )->EnableWindow( TRUE );
		( GetDlgItem(IDC_STA_EPCDATA_WORK_NO) )->ShowWindow( SW_HIDE );
		( GetDlgItem(IDC_CB_EPCDATA_WORK_NO) )->ShowWindow( SW_HIDE );
		( GetDlgItem(IDC_BTN_SEQ_INFO) )->ShowWindow( SW_SHOW );
		( GetDlgItem(IDC_BTN_SEQ_INFO) )->SetWindowText( _T("������ȣ����") );
	}
	else
	{
		( GetDlgItem(IDC_STA_EPCDATA_SEQ_NO) )->SetWindowText( _T("������ȣ") );
		( GetDlgItem(IDC_EDT_EPCDATA_SEQ_NO) )->EnableWindow( FALSE );
		( GetDlgItem(IDC_STA_EPCDATA_WORK_NO) )->SetWindowText( _T("�۾���ȣ") );
		( GetDlgItem(IDC_STA_EPCDATA_WORK_NO) )->ShowWindow( SW_HIDE );
		( GetDlgItem(IDC_CB_EPCDATA_WORK_NO) )->ShowWindow( SW_HIDE );
		( GetDlgItem(IDC_BTN_SEQ_INFO) )->ShowWindow( SW_HIDE );		
	}

	return 0;

EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [Func Desc]
//  ������ȣ/������ȣ ���� ���̾�α� �˾�
/////////////////////////////////////////////////////////////////////////////
INT CBO_REG_EPC_SEARCH::ShowSeqInfo()
{
EFS_BEGIN
	
	/////////////////////////////////////////////////////////////////////////
	// Declare Variable
	/////////////////////////////////////////////////////////////////////////
	INT		ids;
	CString strAcqCode;
	CString strAcqYear;
	CString strWorkCode;
	CGroupCollection Groups(this);
	/////////////////////////////////////////////////////////////////////////
	//
	/////////////////////////////////////////////////////////////////////////
	ids = GetControlData( _T("CM_BO_BOOK_TRADE_SEARCH"), _T("�Լ�����"), strAcqCode );
	if( 0 > ids )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::ShowSeqInfo() ERROR!! -1") );
		return ids;
	}
	
	ids = GetControlData( _T("CM_BO_BOOK_TRADE_SEARCH"), _T("���Գ⵵"), strAcqYear);
	if( 0 > ids )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::ShowSeqInfo() ERROR!! -2") );
		return ids;
	}
	ids = GetControlData( _T("CM_BO_BOOK_TRADE_SEARCH"), _T("��������"), strWorkCode);
	if( 0 > ids )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::ShowSeqInfo() ERROR!! -3") );
		return ids;
	}

	if( _T("1") == strAcqCode )
	{
		Groups.SetAcqType( (ACQ_TYPE)(_ttoi(strAcqCode)) );
		CDivisionNo_List_Dlg dlg(this);
		dlg.SetWorkStatus(_T("�˼��Ϸ�"));
		dlg.SetSpeciesType(BO_REG_PURCHASE);
		dlg.SetAcquisitYear(strAcqYear);
		dlg.SetWorkCode(strWorkCode);
		dlg.SetGroupCollection(&Groups);
		if(IDOK == dlg.DoModal())
		{	
			SetControlData( _T("CM_BO_BOOK_TRADE_SEARCH"), _T("������ȣ"), dlg.GetSelDivNo());
			SetControlData( _T("CM_BO_BOOK_TRADE_SEARCH"), _T("���Գ⵵"), Groups.GetAcqYear());
			SetControlData( _T("CM_BO_BOOK_TRADE_SEARCH"), _T("��������"), Groups.GetWorkCode());			
		}

		MakeWorkNoInfo();
	}
	else if( _T("2") == strAcqCode )
	{
		Groups.SetAcqType( (ACQ_TYPE)(_ttoi(strAcqCode)) );

		CRequisitionNo_List_Dlg dlg(this);
		dlg.SetSpeciesType(BO_REG_DONATE);
		dlg.SetAcqYear(strAcqYear);
		dlg.SetGroupCollection(&Groups);
		if(IDOK == dlg.DoModal())
		{
			SetControlData( _T("CM_BO_BOOK_TRADE_SEARCH"), _T("������ȣ"), dlg.GetSelInfo());
			SetControlData( _T("CM_BO_BOOK_TRADE_SEARCH"), _T("���Գ⵵"), Groups.GetAcqYear());			
		}
	}

	return 0;

EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [Func Desc]
//  �۾���ȣ ������ �����Ѵ�.
/////////////////////////////////////////////////////////////////////////////
INT CBO_REG_EPC_SEARCH::MakeWorkNoInfo()
{
EFS_BEGIN

	/////////////////////////////////////////////////////////////////////////
	// Declare Variable
	/////////////////////////////////////////////////////////////////////////
	INT		  ids;
	CString   strSeqNo;
	CString   strAcqYear;
	CString   strWorkCode;
	CString   strBuf;
	CGroupMgr GroupMgr(this);

	/////////////////////////////////////////////////////////////////////////
	//
	/////////////////////////////////////////////////////////////////////////
	ids = GetControlData( _T("CM_BO_BOOK_TRADE_SEARCH"), _T("������ȣ"), strSeqNo );
	if( 0 > ids )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::MakeWorkNoInfo() ERROR!! -1") );
		return ids;
	}
	
	ids = GetControlData( _T("CM_BO_BOOK_TRADE_SEARCH"), _T("���Գ⵵"), strAcqYear);
	if( 0 > ids )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::MakeWorkNoInfo() ERROR!! -2") );
		return ids;
	}

	ids = GetControlData( _T("CM_BO_BOOK_TRADE_SEARCH"), _T("��������"), strWorkCode);
	if( 0 > ids )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::MakeWorkNoInfo() ERROR!! -3") );
		return ids;
	}

	GroupMgr.InitTempData();
	GroupMgr.SetTempData(_T("AcqYear"),		strAcqYear.GetBuffer(0)	);
	GroupMgr.SetTempData(_T("DivNo"),		strSeqNo.GetBuffer(0)	);
	GroupMgr.SetTempData(_T("WorkCode"),	strWorkCode.GetBuffer(0));

	ids = GroupMgr.SPFExecutePath(_T("�������۾���ȣ����"));
	if( 0 > ids )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::MakeWorkNoInfo() ERROR!! -4") );
		return ids;
	}
	
	/////////////////////////////////////////////////////////////////////////
	// 2. ��Ʈ�ѿ� �Է�
	/////////////////////////////////////////////////////////////////////////
	CESL_ControlMgr* pCM = FindCM( _T("CM_BO_BOOK_TRADE_SEARCH") );
	if( !pCM )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::MakeWorkNoInfo() ERROR!! -5") );
		return -1;
	}

	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox *)pCM->FindControl( _T("�۾���ȣ") );
	if( !pCMB )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::MakeWorkNoInfo() ERROR!! -6") );
		return -1;
	}

	pCMB->InitMultiComboBox();

	pCMB->SetData( &CString(_T("�������")), 1 );
	
	for( INT i=1;i<=ids;i++ )
	{
		strBuf.Format( _T("%d"), i );
		pCMB->SetData( &strBuf, 1 );
	}
	
	pCMB->SetCurSel( 0 );

	return 0;

EFS_END
	return -1;	
}

INT	CBO_REG_EPC_SEARCH::ClearForAllControl()
{
EFS_BEGIN

	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_BOOK_TRADE_SEARCH"));
	
	if (pCM)
	{
		pCM->AllControlClear();			
	}

	return 0;

EFS_END
	return -1;	
}

/////////////////////////////////////////////////////////////////////////////
// [Func Desc]
//  ��� �˻�
/////////////////////////////////////////////////////////////////////////////
INT CBO_REG_EPC_SEARCH::SearchRegEpcList()
{
EFS_BEGIN
	//==============================================
	//2008.07.11 ADD BY PJW : �����ϴܿ� ���¹�LOG���
	BEGIN_SBL();
	//==============================================

	/////////////////////////////////////////////////////////////////////////
	// Declare Variable
	/////////////////////////////////////////////////////////////////////////
	INT		ids;
	CString strWhere;
	
	/////////////////////////////////////////////////////////////////////////
	// 
	/////////////////////////////////////////////////////////////////////////
	ids = MakeWhereFromControl( strWhere ) ;
	if( 0 > ids )	return ids;

	/////////////////////////////////////////////////////////////////////////
	//
	/////////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pDM = FindDM( _T("DM_BO_BOOK_TRADE_MAIN") );
	if( !pDM )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::SearchRegEpcList() ERROR -1 !!!") );
		return -1;
	}

	ids = pDM->RefreshDataManager( strWhere );
	if( 0 > ids )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::SearchRegEpcList() ERROR -2 !!!") );
		return -2;
	}

	if( 0 == pDM->GetRowCount() )
	{
		AfxMessageBox( _T("�˻��� �ڷᰡ �����ϴ�.") );
		return 0;
	}
	
	CESL_ControlMgr* pCM = FindCM( _T("CM_BO_BOOK_TRADE_MAIN") );
	if( !pCM )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::SearchRegEpcList() ERROR -3 !!!") );
		return -1;
	}
		
	pCM->AllControlDisplay();

	//==============================================
	//2008.07.11 ADD BY PJW : �����ϴܿ� ���¹�LOG���
	INT nRowCount = pDM->GetRowCount();
	END_SBL(nRowCount);
	//==============================================

	AfxMessageBox( _T("�˻��� �Ϸ�Ǿ����ϴ�.") );

	return 0;

EFS_END
	return -1;
}

HBRUSH CBO_REG_EPC_SEARCH::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

BOOL CBO_REG_EPC_SEARCH::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_KEYDOWN == pMsg->message)
	{
 		if(VK_RETURN == pMsg->wParam)
		{
			OnBtnSearch();
			return TRUE;
		}
	};
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

void CBO_REG_EPC_SEARCH::OnSetfocusEdtEpcdataRegNo1() 
{
	// TODO: Add your control notification handler code here
	SetIMEStatus(_T("����"), IDC_EDT_EPCDATA_REG_NO1);
}

void CBO_REG_EPC_SEARCH::OnSetfocusEdtEpcdataRegNo2() 
{
	// TODO: Add your control notification handler code here
	SetIMEStatus(_T("����"), IDC_EDT_EPCDATA_REG_NO2);
}

int CBO_REG_EPC_SEARCH::SetIMEStatus(CString strMode, UINT uControlID)
{
	HIMC hIME;
    DWORD dwConversion, dwSentence;
	hIME = ::ImmGetContext(GetDlgItem(uControlID)->m_hWnd);
	if(NULL != hIME)
	{
		ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		if(_T("�ѱ�") == strMode)
		{
			dwConversion=IME_CMODE_HANGUL;
		}
		else if(_T("����") == strMode)
		{
			dwConversion = dwConversion & ~IME_CMODE_LANGUAGE;
			dwConversion |= IME_CMODE_ALPHANUMERIC;
		}
		else return -1;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );		
		ImmReleaseContext(GetDlgItem(uControlID)->m_hWnd,hIME); 
	}
	else return -2;

	return 0;
}
