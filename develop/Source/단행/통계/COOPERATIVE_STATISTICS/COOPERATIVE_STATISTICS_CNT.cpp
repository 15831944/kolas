// COOPERATIVE_STATISTICS_CNT.cpp : implementation file
//

#include "stdafx.h"
#include "COOPERATIVE_STATISTICS_CNT.h"
#include "..\..\����\�弭����\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCOOPERATIVE_STATISTICS_CNT dialog


CCOOPERATIVE_STATISTICS_CNT::CCOOPERATIVE_STATISTICS_CNT(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CCOOPERATIVE_STATISTICS_CNT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCOOPERATIVE_STATISTICS_CNT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParentGrid = NULL;
	pParentMgr = pParent;
}
CCOOPERATIVE_STATISTICS_CNT::~CCOOPERATIVE_STATISTICS_CNT()
{
}


void CCOOPERATIVE_STATISTICS_CNT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCOOPERATIVE_STATISTICS_CNT)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCOOPERATIVE_STATISTICS_CNT, CDialog)
	//{{AFX_MSG_MAP(CCOOPERATIVE_STATISTICS_CNT)
	ON_BN_CLICKED(IDC_btnSTATICS_START, OnbtnSTATICSSTART)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCOOPERATIVE_STATISTICS_CNT message handlers

BOOL CCOOPERATIVE_STATISTICS_CNT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	INT ids = InitESL_Mgr( _T("COOPERATIVE_STATISTICS_CNT") );
	if( ids < 0 ) {
		AfxMessageBox ( _T("OnInitDialog Error") );
		return FALSE;
	}

	CString sStaticsClass;
	GetControlData( _T("CM_COOPERATIVE_STATISTICS_MAIN"), _T("�������"), sStaticsClass, -1, 0 );
	// ���̾�α� ����
	SetWindowText( sStaticsClass );
	// ��¥ ��Ʈ��
	if(_T("����Ƚ�����") == sStaticsClass)
	{
		GetDlgItem(IDC_STC_CLASS_DATE)->SetWindowText( _T("��������") );
		GetDlgItem(IDC_staCooperativeStatisticsCnt)->SetWindowText( _T("����Ƚ��") );
		m_pParentGrid->SetTextMatrix(0, 8, _T("����Ƚ��"));
	}
	else
	{
		GetDlgItem(IDC_STC_CLASS_DATE)->SetWindowText( _T("��û����") );
		GetDlgItem(IDC_staCooperativeStatisticsCnt)->SetWindowText( _T("��ûȽ��") );
		m_pParentGrid->SetTextMatrix(0, 8, _T("��ûȽ��"));
	}
	// ������� (Default:0009(�Ϸ���ڷ�))
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbCNT_STATE);
	pCombo->SetCurSel(4);

	return TRUE;
}

void CCOOPERATIVE_STATISTICS_CNT::OnbtnSTATICSSTART() 
{	
	// ������
	RunStatics();
}

void CCOOPERATIVE_STATISTICS_CNT::OnbtnCLOSE() 
{
	OnCancel();	
}

/*
@Create
 - 2009.02.10 BY PWR
@Description
 - ������
@Parameter
 - 
@Return Value
 - 0, ��� : ����
 - ���� : ����
@Mini Spec
 1. ���� ����
 2. ���� ���� ����� �����´�.
 3. ��� dm�� �Է��Ѵ�.
 4. �׸��� �ݿ�
 5. ��¹� ���� �ǵ��� 
*/
INT CCOOPERATIVE_STATISTICS_CNT::RunStatics()
{
EFS_BEGIN

	INT ids;
	CString strClassDate, strShelfDate, strClassCnt, strRank, strManageCode, strState;
	
	// ���� ��������
	if( 0 != GetControlSQL(strClassDate, strShelfDate, strClassCnt, strRank, strManageCode, strState) )
		return 0;

	CESL_DataMgr pTmpDM;
	pTmpDM.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );
	CESL_DataMgr* pDM = FindDM(_T("DM_COOPERATIVE_STATISTICS_CNT"));
	if( !pDM ) ERROR_MSG_RETURN_ERROR_CODE2(-1, _T("AllControlDisplayGrid"));
	pDM->FreeData();
	CESL_ControlMgr* pCM = FindCM(_T("CM_COOPERATIVE_STATISTICS_MAIN"));
	if( !pCM ) ERROR_MSG_RETURN_ERROR_CODE2(-1, _T("AllControlDisplayGrid"));

	CString sStaticsClass;
	GetControlData( _T("CM_COOPERATIVE_STATISTICS_MAIN"), _T("�������"), sStaticsClass, -1, 0 );

	// 1. ���� ����
	//    ���� / ���� �ѹ濡 ������
	CString strQuery;	
	strQuery.Format(
			_T("SELECT ROWNUM, ")
					_T("REG_NO, ")
					_T("TITLE, ")
					_T("AUTHOR, ")
					_T("PUBLISHER, ")
					_T("APL_MANAGE_CODE, ")
					_T("GIVE_MANAGE_CODE, ")
					_T("CNT ")
			_T("FROM (SELECT BB.REG_NO AS REG_NO, ")
							_T("IB.TITLE AS TITLE, ")
							_T("IB.AUTHOR AS AUTHOR, ")
							_T("IB.PUBLISHER AS PUBLISHER, ")
							_T("VV.APL_MANAGE_CODE AS APL_MANAGE_CODE, ")
							_T("VV.GIVE_MANAGE_CODE AS GIVE_MANAGE_CODE, ")
							_T("VV.CNT AS CNT ")
					_T("FROM BO_BOOK_TBL BB, ")
						_T("IDX_BO_TBL IB, ")
						_T("(SELECT TV.BOOK_KEY AS BOOK_KEY, ")
								_T("TV.APL_MANAGE_CODE AS APL_MANAGE_CODE, ")
								_T("TV.GIVE_MANAGE_CODE AS GIVE_MANAGE_CODE, ")
								_T("COUNT(1) AS CNT ")
						_T("FROM CO_LOAN_USER_TBL CLU, ")
								_T("ILL_TRANS_TBL IT, ")
								_T("BO_BOOK_TBL B, ")
								_T("TRANS_VIEW TV ")
						_T("WHERE IT.TRANS_NO=TV.TRANS_NO ")
							_T("AND CLU.REC_KEY=TV.USER_KEY ")
							_T("AND B.REC_KEY=TV.BOOK_KEY ")
							_T("AND IT.PUBLISH_FORM_CODE='MO' ")
							_T("%s") // STATE
							_T("%s") // GIVE_MANAGE_CODE
							_T("%s") // ��������
							_T("%s") // �谡����
						_T("GROUP BY TV.BOOK_KEY, ")
							_T("TV.APL_MANAGE_CODE, ")
							_T("TV.GIVE_MANAGE_CODE ")
						_T("ORDER BY CNT DESC) VV ")
					_T("WHERE BB.REC_KEY=VV.BOOK_KEY ")
						_T("AND BB.SPECIES_KEY=IB.REC_KEY ")
					_T("UNION ALL ")
					_T("SELECT BB.REG_NO AS REG_NO, ")
							_T("IB.TITLE AS TITLE, ")
							_T("IB.AUTHOR AS AUTHOR, ")
							_T("IB.PUBLISHER AS PUBLISHER, ")
							_T("VV.APL_MANAGE_CODE AS APL_MANAGE_CODE, ")
							_T("VV.GIVE_MANAGE_CODE AS GIVE_MANAGE_CODE, ")
							_T("VV.CNT AS CNT ")
					_T("FROM SE_BOOK_TBL BB, ")
						_T("IDX_SE_TBL IB, ")
						_T("(SELECT TV.BOOK_KEY AS BOOK_KEY, ")
							_T("TV.APL_MANAGE_CODE AS APL_MANAGE_CODE, ")
							_T("TV.GIVE_MANAGE_CODE AS GIVE_MANAGE_CODE, ")
							_T("COUNT(1) AS CNT ")
						_T("FROM CO_LOAN_USER_TBL CLU, ")
							   _T("ILL_TRANS_TBL IT, ")
							   _T("SE_BOOK_TBL B, ")
							   _T("TRANS_VIEW TV ")
						_T("WHERE IT.TRANS_NO=TV.TRANS_NO ")
							_T("AND CLU.REC_KEY=TV.USER_KEY ")
							_T("AND B.REC_KEY=TV.BOOK_KEY ")
							_T("AND IT.PUBLISH_FORM_CODE='SE' ")
							_T("%s") // STATE
							_T("%s") // GIVE_MANAGE_CODE
							_T("%s") // ��������
							_T("%s") // �谡����
						_T("GROUP BY TV.BOOK_KEY, ")
							_T("TV.APL_MANAGE_CODE, ")
							_T("TV.GIVE_MANAGE_CODE ")
						_T("ORDER BY CNT DESC) VV ")
					_T("WHERE BB.REC_KEY=VV.BOOK_KEY ")
						_T("AND BB.SPECIES_KEY=IB.REC_KEY) ")
			_T("WHERE  1=1 ")
				_T("%s") // Ƚ��
				_T("%s") // ����
			, strState, strManageCode, strClassDate, strShelfDate
			, strState, strManageCode, strClassDate, strShelfDate
			, strClassCnt, strRank );

	ids = pTmpDM.RestructDataManager( strQuery );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2(-1, _T("AllControlDisplayGrid"));

	
	// pTmpDM -> pDM ����
	for(INT i = 0; i < pTmpDM.GetRowCount(); i++)
	{
		pDM->InsertRow(-1);
		CString sData, sDesc;
		// ����
		ids = pTmpDM.GetCellData(i, 0, sData);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-1, _T("AllControlDisplayGrid"));
		ids = pDM->SetCellData(_T("����"), sData, i);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-2, _T("AllControlDisplayGrid"));

		// ��Ϲ�ȣ
		ids = pTmpDM.GetCellData(i, 1, sData);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-3, _T("AllControlDisplayGrid"));
		ids = pDM->SetCellData(_T("��Ϲ�ȣ"), sData, i);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-4, _T("AllControlDisplayGrid"));

		// ����
		ids = pTmpDM.GetCellData(i, 2, sData);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-5, _T("AllControlDisplayGrid"));
		ids = pDM->SetCellData(_T("����"), sData, i);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-6, _T("AllControlDisplayGrid"));

		// ����
		ids = pTmpDM.GetCellData(i, 3, sData);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-7, _T("AllControlDisplayGrid"));
		ids = pDM->SetCellData(_T("����"), sData, i);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-8, _T("AllControlDisplayGrid"));

		// ������
		ids = pTmpDM.GetCellData(i, 4, sData);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-9, _T("AllControlDisplayGrid"));
		ids = pDM->SetCellData(_T("������"), sData, i);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-10, _T("AllControlDisplayGrid"));

		// ��û������
		ids = pTmpDM.GetCellData(i, 5, sData);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-11, _T("AllControlDisplayGrid"));
		ids = pDM->SetCellData(_T("��û��������"), sData, i);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-12, _T("AllControlDisplayGrid"));
		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("��������"), sData, sDesc );
		ids = pDM->SetCellData( _T("��û�������м���"), sDesc, i );
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-13, _T("AllControlDisplayGrid"));

		// ����������
		sDesc.Empty();
		ids = pTmpDM.GetCellData(i, 6, sData);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-14, _T("AllControlDisplayGrid"));
		ids = pDM->SetCellData(_T("������������"), sData, i);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-15, _T("AllControlDisplayGrid"));
		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("��������"), sData, sDesc );
		ids = pDM->SetCellData( _T("�����������м���"), sDesc, i );
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-16, _T("AllControlDisplayGrid"));

		// Ƚ��
		ids = pTmpDM.GetCellData(i, 7, sData);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-17, _T("AllControlDisplayGrid"));
		ids = pDM->SetCellData(_T("Ƚ��"), sData, i);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2(-18, _T("AllControlDisplayGrid"));
	}

	pCM->ControlDisplay(_T("CntGrid"));
	
	INT nRowCnt = pDM->GetRowCount();
	if( !nRowCnt )
		AfxMessageBox(_T("������� �����ϴ�."));
	else
		AfxMessageBox(_T("�Ϸ�Ǿ����ϴ�"));

	return 0;

EFS_END
return -1;
}

/*
@Create
 - 2009.02.10 BY PWR
@Description
 - ��Ʈ���� �Է°����� ��������� �����Ѵ�.
@Parameter
 - strClassDate[OUT]
 - strShelfDate[OUT]
 - strClassCnt[OUT]
 - strRank[OUT]
 - strManageCode[OUT]
 - strState[OUT]
@Return Value
 - 0, ��� : ����
 - ���� : ����
@Mini Spec
 1. ������ ���� ����
 2. �谡�� ���� ����
 3. ����Ƚ�� ���� ����
 4. ���� ���� ����
*/
INT CCOOPERATIVE_STATISTICS_CNT::GetControlSQL(CString &strClassDate, CString &strShelfDate, CString &strClassCnt, CString &strRank, CString &strManageCode, CString &strState)
{
	CESL_ControlMgr* pCM = FindCM(_T("CM_COOPERATIVE_STATISTICS_CNT"));
	if( !pCM ){
		AfxMessageBox( _T("FindCM( CM_COOPERATIVE_STATISTICS_CNT ) ERROR ") );
		return -1;
	}

	CString sStaticsClass;
	GetControlData( _T("CM_COOPERATIVE_STATISTICS_MAIN"), _T("�������"), sStaticsClass, -1, 0 );

	const INT nControlCnt = 9;
	CString strCtrlAlias[nControlCnt] =
	{
		_T("���Ǻ�����1"),	_T("���Ǻ�����2"), 
		_T("�谡����1"),	_T("�谡����2"),
		_T("���Ǻ�Ƚ��1"),	_T("���Ǻ�Ƚ��2"), 
		_T("����"),			_T("��������"),
		_T("�������")
	};

	CString strCtrlData[nControlCnt];
	CESL_Control_Element *pCE = NULL;

	// ��¥�� �����´�.
	for( INT i=0; i<4; i++ )
	{		
		pCE = (CESL_Control_Element*)pCM->FindControlElement( strCtrlAlias[i] );
		strCtrlData[i] = CLocCommonAPI::GetDate(this,pCE->CTRL_ID);
		strCtrlData[i].TrimLeft(); strCtrlData[i].TrimRight();
	}

	if( strCtrlData[0].IsEmpty() && strCtrlData[2].IsEmpty() )
	{
		AfxMessageBox( _T("��¥�� �Է��ؾ��մϴ�") );
		return 1;
	}

	// ��Ÿ
	for( i=4; i<nControlCnt; i++ )
	{
		pCM->GetControlMgrData( strCtrlAlias[i], strCtrlData[i] );
		strCtrlData[i].TrimLeft(); strCtrlData[i].TrimRight();
	}	

	// ������ ����
	if( !strCtrlData[0].IsEmpty() )
	{
		if( strCtrlData[1].IsEmpty() )
		{
			strCtrlData[1] = strCtrlData[0];
		}

		if(_T("����Ƚ�����") == sStaticsClass)
		{
			strClassDate.Format( _T("AND IT.GIVE_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') ")
								   _T("AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') "),
									 strCtrlData[0], strCtrlData[1] );
		}
		else
		{
			strClassDate.Format( _T("AND IT.APL_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') ")
								   _T("AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') "),
									 strCtrlData[0], strCtrlData[1] );
		}
	}

	// �谡�� ����
	if( !strCtrlData[2].IsEmpty() )
	{
		if( strCtrlData[3].IsEmpty() )
		{
			strCtrlData[3] = strCtrlData[2];
		}

		strShelfDate.Format(_T("AND B.SHELF_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') ")
							_T("AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') "),
							strCtrlData[2], strCtrlData[3] );
	}

	// ����Ƚ�� 
	if( !strCtrlData[4].IsEmpty() )
	{
		if( strCtrlData[5].IsEmpty() )
		{
			strClassCnt.Format( _T("AND CNT>=%s "), strCtrlData[4] );
		}
		else
		{
			strClassCnt.Format( _T("AND CNT BETWEEN %s AND %s "), strCtrlData[4], strCtrlData[5] );
		}
	}
	
	// ����
	if( !strCtrlData[6].IsEmpty() )
	{
		strRank.Format( _T("AND ROWNUM<=%s "), strCtrlData[6] );
	}

	// ��������
	if( !strCtrlData[7].IsEmpty() )
	{
		if(_T("����Ƚ�����") == sStaticsClass)
			strManageCode.Format( _T("AND TV.GIVE_MANAGE_CODE='%s' "), strCtrlData[7]);
		else
			strManageCode.Format( _T("AND TV.APL_MANAGE_CODE='%s' "), strCtrlData[7]);
	}

	// �������
	if( !strCtrlData[8].IsEmpty() && strCtrlData[8]!=_T("----") )
	{
		strState.Format( _T("AND IT.STATE='%s' "), strCtrlData[8]);
	}

	return 0;
}