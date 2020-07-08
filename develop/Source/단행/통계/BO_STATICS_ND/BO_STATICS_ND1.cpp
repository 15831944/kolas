// BO_STATICS_ND1.cpp : implementation file
//

#include "stdafx.h"
#include "BO_STATICS_ND1.h"
#include "..\..\����\�弭����\LOC_CommonAPI\LocCommonAPI.h"
#include "..\..\..\ESL\SIReportMgr\SIReportManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_STATICS_ND dialog


CBO_STATICS_ND::CBO_STATICS_ND(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_STATICS_ND)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_sExportFileName = _T("");
}
CBO_STATICS_ND::~CBO_STATICS_ND()
{
	
}

VOID CBO_STATICS_ND::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_STATICS_ND)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_STATICS_ND, CDialog)
	//{{AFX_MSG_MAP(CBO_STATICS_ND)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_STATICS_ND message handlers

BOOL CBO_STATICS_ND::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_STATICS_ND::OnOK() 
{
	INT ids;
	
	CString sWhere;
	ids = MakeQuery( &sWhere );
	if ( ids != 0 ) return;
	
	if ( m_sStaticsName == _T("���Ա��к����(�����)") || m_sStaticsName == _T("�������к����"))
	{
		ids = MakeStatics_ACQ( sWhere);
		if ( ids < 0 ) return;
	}
	else if ( m_sStaticsName == _T("�������") || m_sStaticsName == _T("�����߰��ڷ����") || 
			  m_sStaticsName == _T("�񵵼��ڷ����") )
	{
		ids = MakeStatics_ND( sWhere);
		if ( ids < 0 ) return;
	}
	else if ( m_sStaticsName == _T("�ڷ�������") )
	{
		ids = MakeStatics_Class( sWhere);
		if ( ids < 0 ) return;
	}
	else if ( m_sStaticsName == _T("�弭������Ȳ") )
	{
		ids = MakeStatics_ALL( sWhere);
		if ( ids < 0 ) return;
	}
	
	CButton* pbtnSaveFile;
	pbtnSaveFile = ( CButton* )GetDlgItem(IDC_chkExpFile);
	if ( pbtnSaveFile->GetCheck() )
	{
		SaveExcelFile();
	}

	ids = RunSIPreView();
	if ( ids < 0 ) return;
//	CDialog::OnOK();
}

VOID CBO_STATICS_ND::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-16
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   �Լ�����   ] 
// ��������� �����Ѵ�.
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::SetStaticsName(CString sStaticsName)
{
	m_sStaticsName = sStaticsName;
	return 0;
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-16
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   �Լ�����   ] 
// �Է°��� ���� �������� �����.
//
// [  PARAMETERS  ]
//    psGetWhere  : ����������     
//
// [ RETURN VALUE ]
//    0    : ���� 
//    ���� : ����
//
// [   �̴Ͻ���   ]
// 1. ��Ʈ���� ���� ���Ѵ�.
// 2. �Է°����� �������� �����.
// 3. ����������.
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::MakeQuery(CString *psGetWhere)
{
	INT ids;
	CString sErrMsg;

	//-----------------------------------------------------------------------------
	// 1. ��Ʈ���� ���� ���Ѵ�. 
	//-----------------------------------------------------------------------------
	CString sCMAlias = _T("CM_STATICS_ND");
	CESL_ControlMgr* pCM = FindCM( sCMAlias );
	if ( pCM == NULL )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeQuery - 0 ERROR !!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	const INT nDataCnt = 5;
	CString sDataAlias[nDataCnt] = { _T("�谡�μ�"), _T("�谡��1") , _T("�谡��2") , _T("������1"), _T("������2") };
	
	CESL_Control_Element *pCE;
	CString sShelfCode;							// �谡�μ�
	CString sStartRegDate;						// ���۹谡��
	CString sEndRegDate;						// ���谡��
	CString sStartChangeDate;					// ���ۺ�����
	CString sEndChangeDate;						// ��������
	
	
	// �谡�μ�
	ids = pCM->GetControlMgrData( sDataAlias[0], sShelfCode, -1, 0 );
	if ( ids < 0 )	
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeQuery - 1 ERROR !!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	// ���۹谡��
	pCE = (CESL_Control_Element*)pCM->FindControlElement( sDataAlias[1] );
	if ( pCE == NULL )	
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeQuery - 2 ERROR !!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	sStartRegDate = CLocCommonAPI::GetDate(this,pCE->CTRL_ID);
	sStartRegDate.TrimLeft(); sStartRegDate.TrimRight();

	// ���谡��
	pCE = (CESL_Control_Element*)pCM->FindControlElement( sDataAlias[2] );
	if ( pCE == NULL )	
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeQuery - 3 ERROR !!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	sEndRegDate = CLocCommonAPI::GetDate(this,pCE->CTRL_ID);
	sEndRegDate.TrimLeft(); sEndRegDate.TrimRight();
    
	// ���ۺ�����
	pCE = (CESL_Control_Element*)pCM->FindControlElement( sDataAlias[3] );
	if ( pCE == NULL )	
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeQuery - 4 ERROR !!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	sStartChangeDate = CLocCommonAPI::GetDate(this,pCE->CTRL_ID);
	sStartChangeDate.TrimLeft(); sStartChangeDate.TrimRight();

	// ��������
	pCE = (CESL_Control_Element*)pCM->FindControlElement( sDataAlias[4] );
	if ( pCE == NULL )	
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeQuery - 5 ERROR !!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	sStartChangeDate = CLocCommonAPI::GetDate(this,pCE->CTRL_ID);
	sStartChangeDate.TrimLeft(); sStartChangeDate.TrimRight();

	// 2005-12-15 REM BY PDW(Jr) ==================================================
	// ��û����
	/*
	// �谡�� ������ �ԷµǾ���Ѵ�.
	// 2005-11- 19 WORK UPD ��� ������ ���� üũ����
	if ( m_sStaticsName == _T("���Ա��к����(�����)") || m_sStaticsName == _T("�������к����") )
	{
		if ( sStartRegDate == _T("") || sEndRegDate == _T("") ) 
		{
			AfxMessageBox( _T("�谡���� �Էµ��� �ʾҽ��ϴ�.") );
			return 1 ;
		}
	}
	*/
	// ============================================================================

	//-----------------------------------------------------------------------------
	// 2. �Է°����� �������� �����.
	//-----------------------------------------------------------------------------
	CString sWhere;                      // ������
	CString sTemp;                       // �ӽú��� 
	sWhere.Empty();

	// �谡�μ� ���� 
	if ( !sShelfCode.IsEmpty() )
	{
		if ( !sWhere.IsEmpty() )
			sWhere += _T(" AND ");

		sTemp.Format( _T(" B.SHELF_LOC_CODE = %s ") , sShelfCode );
		sWhere += sTemp;
	}

	// 2005-12-15 UDP BY PDW(Jr) ==================================================
	// üũ�� ������ ���Ѵ�.
	/* ������ 
	// �谡�� ���� 
	// �谡�������� ������ Ȯ���Ѵ�.
	// 2005-11-21 WORK UPD
	// ������ ���� �߰�
	if ( m_sStaticsName == _T("���Ա��к����(�����)") || m_sStaticsName == _T("�������к����") )
	{
		if ( !sWhere.IsEmpty() )
			sWhere += _T(" AND ");

		sTemp.Format( _T(" B.SHELF_DATE BETWEEN '%s' AND '%s'") ,sStartRegDate ,sEndRegDate  );
		sWhere += sTemp;

		if ( !sStartChangeDate.IsEmpty() )
		{
			sTemp.Format( _T(" AND B.SHELF_CHANGE_DATE BETWEEN '%s' AND '%s'") ,sStartChangeDate ,sEndChangeDate  );
			sWhere += sTemp;
		}

	}
	*/
	if ( m_sStaticsName == _T("���Ա��к����(�����)") || m_sStaticsName == _T("�������к����") )
	{
		if ( !sStartRegDate.IsEmpty() && sEndRegDate.IsEmpty() )
			sTemp.Format( _T(" B.SHELF_DATE BETWEEN '%s' AND '%s'") , sStartRegDate , sStartRegDate  );
		else if ( !sStartRegDate.IsEmpty() && !sEndRegDate.IsEmpty() )
			sTemp.Format( _T(" B.SHELF_DATE BETWEEN '%s' AND '%s'") ,sStartRegDate ,sEndRegDate  );
		else if ( sStartRegDate.IsEmpty() && !sEndRegDate.IsEmpty() )
		{
			AfxMessageBox( _T("�谡�� �Է��� �߸��Ǿ� �ֽ��ϴ�.") );
			return 1 ;
		}

		if ( !sWhere.IsEmpty() )
			sWhere += _T(" AND ");
		
		sWhere += sTemp;


		if ( !sStartChangeDate.IsEmpty() && sEndChangeDate.IsEmpty() )
		{
			sTemp.Format( _T(" AND B.SHELF_CHANGE_DATE BETWEEN '%s' AND '%s'") ,sStartChangeDate ,sStartChangeDate  );
			sWhere += sTemp;
		}
		else if ( !sStartChangeDate.IsEmpty() && !sEndChangeDate.IsEmpty() )
		{
			sTemp.Format( _T(" AND B.SHELF_CHANGE_DATE BETWEEN '%s' AND '%s'") ,sStartChangeDate ,sEndChangeDate  );
			sWhere += sTemp;
		}

	}
	// ============================================================================

	//-----------------------------------------------------------------------------
	// 3. ���� ������
	//-----------------------------------------------------------------------------
	if ( !sWhere.IsEmpty() )
		sWhere = _T(" AND ") + sWhere;

	*psGetWhere = sWhere;
		
	return 0;
}

BOOL CBO_STATICS_ND::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids;
	CString sErrMsg;

	ids = InitESL_Mgr( _T("BO_STATICS_ND") );
	if ( ids < 0 ) 
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::OnInitDialog Error") );
		AfxMessageBox ( sErrMsg );
		return FALSE;
	}

	GetDlgItem(IDC_eStaticsNDTitle)->SetWindowText(m_sStaticsName);
	 
	GetDlgItem(IDC_datREG_DATE1_ND)->EnableWindow( FALSE );
	GetDlgItem(IDC_datREG_DATE2_ND)->EnableWindow( FALSE );
	GetDlgItem(IDC_datCHANGE_DATE1_ND)->EnableWindow( FALSE );
	GetDlgItem(IDC_datCHANGE_DATE2_ND)->EnableWindow( FALSE );

	if ( m_sStaticsName == _T("���Ա��к����(�����)") || m_sStaticsName == _T("�������к����") )
	{
		GetDlgItem(IDC_datREG_DATE1_ND)->EnableWindow();
		GetDlgItem(IDC_datREG_DATE2_ND)->EnableWindow();
		GetDlgItem(IDC_datCHANGE_DATE1_ND)->EnableWindow();
		GetDlgItem(IDC_datCHANGE_DATE2_ND)->EnableWindow();
	}

	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-18
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   �Լ�����   ] 
// ���Ա��к� ���, �������к� ���
//
// [  PARAMETERS  ]
//    sWhere  : ������     
//
// [ RETURN VALUE ]
//    0    : ���� 
//    ���� : ����
//
// [   �̴Ͻ���   ]
// 1. ���DM �ʱ�ȭ
// 2. �⺻��� ���� ����
// 3. ���� ��� ����
// 4. ���� ��� ����
// 5. �հ� ����
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::MakeStatics_ACQ(CString sWhere)
{
    INT ids;
	CString sErrMsg;
	
	//-----------------------------------------------------------------------------
	// 1. ���DM �ʱ�ȭ
	//-----------------------------------------------------------------------------
	CString sDMAlias = _T("DM_���԰������к����_�����");
	CESL_DataMgr* pDM_Dst = FindDM( sDMAlias );
	if ( pDM_Dst == NULL )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ACQ - 1 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	pDM_Dst->FreeData();

	// ��¿� DM�� ������ �����Ǿ��ִ�.
	const INT nResultRow = 18;
	const INT nResultCol = 4;
	for ( INT i = 0;i < nResultRow;i++ )
	{
		pDM_Dst->InsertRow( -1 );
		for ( INT j = 0;j < nResultCol;j++ )
			pDM_Dst->SetCellData( i, j, _T("0 / 0"));		
	}

	// ��� ������ DM
	CESL_DataMgr DM;
	DM.SetCONNECTION_INFO ( pDM_Dst->CONNECTION_INFO );	
	
	//-----------------------------------------------------------------------------
	// 2. �⺻ ���� ���� ����
	//-----------------------------------------------------------------------------
	CString sSelect;
	CString sTableName;
	CString sBaseWhere;
	CString sGroupBy;
	CString sBoWorkingStatus;
	CString sSeWorkinStatus;
	CString sQuery;
	
	// ������� �������� ��谡 �޶�����.
	if ( m_sStaticsName == _T("���Ա��к����(�����)") )
	{
		sSelect.Format ( _T("SELECT B.REG_CODE, B.ACQ_CODE ") );
		sBaseWhere.Format ( _T(" B.SPECIES_KEY=S.REC_KEY ") );
		sGroupBy.Format( _T("Group By B.REG_CODE , B.ACQ_CODE") );
	}
	else if ( m_sStaticsName == _T("�������к����") )
	{
		sSelect.Format ( _T("SELECT B.REG_CODE, B.MANAGE_CODE ") );
		sBaseWhere.Format ( _T(" B.SPECIES_KEY=S.REC_KEY ") );
		sGroupBy.Format( _T("Group By B.REG_CODE , B.MANAGE_CODE") );
	}


	// 17/07/20 ������ : å���������ڷ� ���� �߰� 
/*//BEFORE-----------------------------------------------------------------------------
	//=====================================================
	//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
// 	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
// 													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
// 		                                               _T(" 'BOL214O' , 'BOL215O' ) ") );
// 	sSeWorkinStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'SEL112N' , 'SEL212N' , 'SEL213O' , 'SEL214O' , ")
// 										              _T(" 'SEL215O' , 'SEL216O' , 'SEL311O' , 'SEL312O' , ")
// 		                                              _T(" 'SEL313O' , 'SEL314O' , 'SEL315O' ) ") );
	// 2013.12.01 UPDATE BY BJS : 2013���������ռ���Ȯ�뱸��װ��� (���յ���Ÿ������ ���� �߰�)
	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
		                                               _T(" 'BOL214O' , 'BOL215O' , 'BOL218O' , 'BOL511O' , 'BOL611O') ") );
//	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
//													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
//		                                               _T(" 'BOL214O' , 'BOL215O' , 'BOL218O' , 'BOL511O' ) ") );
	//=====================================================
*///AFTER------------------------------------------------------------------------------
	// 18.09.27 JMJ KOL.RED.2018.008
	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
		                                               _T(" 'BOL214O' , 'BOL215O' , 'BOL218O' , 'BOL219O' , ")
													   _T(" 'BOL411O' , 'BOL511O' , 'BOL611O') ") );
/*//END-------------------------------------------------------------------------------*/	
	// 18.09.27 JMJ KOL.RED.2018.008
	sSeWorkinStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'SEL112N' , 'SEL212N' , 'SEL213O' , 'SEL214O' , ")
										              _T(" 'SEL215O' , 'SEL216O' , 'SEL311O' , 'SEL312O' , ")
		                                              _T(" 'SEL313O' , 'SEL314O' , 'SEL315O' , 'SEL411O', 'SEL511O' ) ") );


	//-----------------------------------------------------------------------------
	// 3. ���� ��� ����
	//-----------------------------------------------------------------------------	
	sTableName = _T("BO_SPECIES_TBL S , BO_BOOK_TBL B ");
	sQuery.Format(_T("%s , COUNT(DISTINCT S.REC_KEY), COUNT(DISTINCT B.REC_KEY) FROM %s WHERE %s ")
				  		_T(" AND B.BOOK_APPENDIX_FLAG = 'B' %s ")
				  , sSelect, sTableName, sBaseWhere, sWhere);
	sQuery += sBoWorkingStatus + sGroupBy;
	
	ids = DM.RestructDataManager( sQuery );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ACQ - 2 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	ids = CopySrcToDst1( &DM, pDM_Dst, 0 );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ACQ - 3 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	//-----------------------------------------------------------------------------
	// 4. ���� ��� ����
	//-----------------------------------------------------------------------------	
	DM.FreeData();
	sTableName = _T("SE_SPECIES_TBL S , SE_BOOK_TBL B ");
	sQuery.Format(_T("%s , COUNT(DISTINCT S.REC_KEY) , COUNT(DISTINCT B.REC_KEY) FROM %s WHERE %s %s ")				  
				  , sSelect, sTableName, sBaseWhere, sWhere );
	sQuery += sSeWorkinStatus + sGroupBy;
	
	ids = DM.RestructDataManager( sQuery );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ACQ - 4 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	ids = CopySrcToDst1( &DM, pDM_Dst, 1 );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ACQ - 5 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	//-----------------------------------------------------------------------------
	// 5. �հ� ����
	//-----------------------------------------------------------------------------	
	// ���� ��� ���ϱ� 
	UpdateRowSum( pDM_Dst, 0, 2, 3 );
	// ���� ��� ���ϱ� 
	UpdateColSum( pDM_Dst, 0, 2, 3 );
	UpdateColSum( pDM_Dst, 4, 10, 11 );
	UpdateColSum( pDM_Dst, 12, 15, 16 );
	// �Ѱ� ���ϱ� 
	// ���� �ο� 3, 11, 16 -> 17
	UpdateColSum( pDM_Dst, 3, 3, 17 );
	UpdateColSum( pDM_Dst, 11, 11, 17 );
	UpdateColSum( pDM_Dst, 16, 16, 17 );
 

	return 0;
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-18
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   �Լ�����   ] 
// ScrDM���� �ش� ��ϱ��п� ���ؼ� pDstDM�� Ư�� Cell�� �ִ´�.
// ���Ա��к����(�����), �������к� , �ڷ���� ��迡�� ���ȴ�.
//
// [  PARAMETERS  ]
//    pSrcDM  : ������ ���� ��� DM     
//    pDstDM  : ����� DM
//    nMode   : ���� ���� ���� �ڵ� ( 0 : ���� , 1 : ���� )
//
// [ RETURN VALUE ]
//    0    : ���� 
//    ���� : ����
//
// [   �̴Ͻ���   ]
// 1. idx = 0�� ����( ��ϱ��� )�� �Էµ� ��ġ�� �ٲ��.
// 2. idx = 1�� ����( ���Ա��� )�� �Էµ� ��ġ�� �ٲ��.
// 3. idx = 2,3�� ������ �Էµ� ��� ���̴�.
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::CopySrcToDst1(CESL_DataMgr *pSrcDM, CESL_DataMgr *pDstDM, INT nMode)
{
	INT ids;
	CString sErrMsg;

	CString sRegCode;								// ��ϱ���
	CString sData;								    // ���� ����
    CString sInsertData;                            // �Է°� 
	INT nInsertRowIdx;                              // �Է½�ų �ο��� ��ġ
	INT nInsertColIdx;								// �Է½�ų �÷��� ��ġ
	INT nRowCount = pSrcDM->GetRowCount();
	for ( INT i = 0; i < nRowCount;i++ )
	{
		ids = pSrcDM->GetCellData( i, 0, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst1 - 1 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}

		//-----------------------------------------------------------------------------
		// 1. inx = 0�� ����ġ�� ����( ��ϱ��� )�� �Էµ� ��ġ�� �ٲ��.
		//-----------------------------------------------------------------------------	
		if ( sData == _T("EM") )		nInsertRowIdx = 0;
		else if ( sData == _T("JM") )   nInsertRowIdx = 1;		
		else if ( sData == _T("WM") )   nInsertRowIdx = 2;
		else if ( sData == _T("KN") )   nInsertRowIdx = 4;
		else if ( sData == _T("DJ") || sData == _T("DM") || sData == _T("DS") ) nInsertRowIdx = 5;
		else if ( sData == _T("MI") )   nInsertRowIdx = 6;
		else if ( sData == _T("MM") )   nInsertRowIdx = 7;
		else if ( sData == _T("AA") )   nInsertRowIdx = 8;
		else if ( sData == _T("EE") )   nInsertRowIdx = 9;
		else if ( sData == _T("ES") )   nInsertRowIdx = 12;
		else if ( sData == _T("JS") )   nInsertRowIdx = 13;
		else if ( sData == _T("WS") )	nInsertRowIdx = 14;
		else
		{
			if ( nMode == 0 )
				nInsertRowIdx = 10;
			else if ( nMode == 1 )
				nInsertRowIdx = 15;
		}

		//-----------------------------------------------------------------------------
		// 2. inx = 1�� ����ġ�� ����( ����,����,�з���ȣ ���� )�� �Էµ� ��ġ�� �ٲ��.
		//-----------------------------------------------------------------------------	
		ids = pSrcDM->GetCellData( i, 1, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst1 - 2 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}
		
		if ( m_sStaticsName == _T("���Ա��к����(�����)") )
		{
			if ( sData == _T("1") )
				nInsertColIdx = 0;
			else if ( sData == _T("2") )
				nInsertColIdx = 1;
			else
				nInsertColIdx = 2;
		}
		else if ( m_sStaticsName == _T("�������к����") )
		{
			if ( sData == _T("MA") )
				nInsertColIdx = 0;
			else if ( sData == _T("BR") )
				nInsertColIdx = 1;
			else
				nInsertColIdx = 2;			
		}
		else if ( m_sStaticsName == _T("�ڷ�������") )
		{
			if ( !sData.IsEmpty() )
				nInsertColIdx = _ttoi( sData );
			else
				nInsertColIdx = 10;
		}	

		//-----------------------------------------------------------------------------
		// 3. idx = 2,3�� ������ �Էµ� ������
		//-----------------------------------------------------------------------------	
		ids = pSrcDM->GetCellData( i, 2, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst1 - 3 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}
		
		sInsertData = sData;

		ids = pSrcDM->GetCellData( i, 3, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst1 - 4 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}

		// ���� �����Ϳ� ���� ����� ��ģ��.
		CString sSpecies;
		CString sBook;
		CString sPreData;

		ids = pDstDM->GetCellData( nInsertRowIdx, nInsertColIdx, sPreData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst1 - 4 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}		

		GetDivValue( sPreData, &sSpecies, &sBook );

		sInsertData.Format( _T("%d"), _ttoi( sSpecies ) + _ttoi( sInsertData ) );
		sData.Format( _T("%d"), _ttoi( sBook ) + _ttoi( sData ) );
		sInsertData = sInsertData + _T(" / ") + sData;		
	
		// �Է�
		ids = pDstDM->SetCellData( nInsertRowIdx, nInsertColIdx, sInsertData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst1 - 5 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}
	}

	return 0;
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-18
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   �Լ�����   ] 
// _T(" �� / �� ")�� ������ �и��Ѵ�.
//
// [  PARAMETERS  ]
//    sPreData  : ��� ������
//    psSpecies : ������
//    psBook    : å����
//
// [ RETURN VALUE ]
//    0    : ���� 
//    ���� : ����
//
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::GetDivValue(CString sPreData, CString *psSpecies, CString *psBook)
{
	sPreData.Replace( _T(" "), _T("") );

	INT nPos = sPreData.FindOneOf( _T("/") );

	if ( nPos < 0 )
	{
		psSpecies->Format( _T("%s"), sPreData );
		psBook->Format( _T("%s"), _T("0") );
	}
	else
	{
		psSpecies->Format( _T("%s"), sPreData.Mid(0, nPos) );
		psBook->Format( _T("%s"), sPreData.Mid(nPos + 1) );
	}

	return 0;
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-18
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   �Լ�����   ] 
// ��� ����� SIReport�� �����ش�.
//
// [ RETURN VALUE ]
//    0    : ���� 
//    ���� : ����
//
// [   �̴Ͻ���   ]
// 1. �ش� DM ã�� , �ش� ��¹����� ����
// 2. SIReport �ʱ�ȭ 
// 3. SIReport ���� �Է�
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::RunSIPreView()
{
	INT ids;
	CString sErrMsg;

	//-----------------------------------------------------------------------------
	//1. �ش� DM ã�� , �ش� ��¹����� ����
	//-----------------------------------------------------------------------------
	CESL_DataMgr *pDM_Dst;
	CString sSIReportAlias;

	if ( m_sStaticsName == _T("���Ա��к����(�����)") || m_sStaticsName == _T("�������к����") )
	{
		sSIReportAlias = _T("���԰������к����_�����");
		pDM_Dst = FindDM ( _T("DM_���԰������к����_�����") );	
	}	
	else if ( m_sStaticsName == _T("�弭������Ȳ") )
	{
		sSIReportAlias = _T("�弭������Ȳ_�����");
		pDM_Dst = FindDM ( _T("DM_�弭������Ȳ_�����") );	
	}
	else if ( m_sStaticsName == _T("�����߰��ڷ����") )
	{
		sSIReportAlias = _T("�����߰��ڷ����_�����");
		pDM_Dst = FindDM ( _T("DM_�����񵵼��߰��ڷ����_�����") );
	}
	else if ( m_sStaticsName == _T("�������") )
	{
		sSIReportAlias = _T("�������_�����");
		pDM_Dst = FindDM ( _T("DM_�����񵵼��߰��ڷ����_�����") );
	}
	else if ( m_sStaticsName == _T("�񵵼��ڷ����") )
	{
		sSIReportAlias = _T("�񵵼��ڷ����_�����");
		pDM_Dst = FindDM ( _T("DM_�����񵵼��߰��ڷ����_�����") );
	}
	else if ( m_sStaticsName == _T("�ڷ�������") )
	{
		sSIReportAlias = _T("�ڷ�������_�����");
		pDM_Dst = FindDM ( _T("DM_�ڷ�������_�����") );
	}

	if ( pDM_Dst == NULL )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::RunSIPreVeiw - 1 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	//-----------------------------------------------------------------------------
	//2. SIReport �ʱ�ȭ 
	//-----------------------------------------------------------------------------
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
//	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) 
	{
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return -1;
	}

	pSIReportManager->SetCONNECTION_INFO(pDM_Dst->CONNECTION_INFO);
	pSIReportManager->DeleteTempPageFiles();
	
	ids = pSIReportManager->GetSIReportFileFromDB( sSIReportAlias );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::RunSIPreVeiw - 2 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	//-----------------------------------------------------------------------------
	//3. SIReport �����Է�
	//-----------------------------------------------------------------------------
	// ����
	CString strStaticsTitle;
	GetDlgItem(IDC_eStaticsNDTitle)->GetWindowText(strStaticsTitle);
	pSIReportManager->SetTempVariable(_T("�������"), strStaticsTitle);

	CString sCMAlias = _T("CM_STATICS_ND");
	CESL_ControlMgr* pCM = FindCM( sCMAlias );
	if ( pCM == NULL )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::RunSIPreVeiw - 3 ERROR !!") );
		AfxMessageBox( sErrMsg );
		delete pSIReportManager;
		return -1;
	}

	
	CESL_Control_Element *pCE;
	CString sData;
	CString sStartRegDate;
	CString sEndRegDate;

	// �谡��1
	pCE = (CESL_Control_Element*)pCM->FindControlElement( _T("�谡��1") );
	if ( pCE == NULL )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::RunSIPreVeiw - 4 ERROR !!") );
		AfxMessageBox( sErrMsg );
		delete pSIReportManager;
		return -1;
	}

	sStartRegDate = CLocCommonAPI::GetDate(this,pCE->CTRL_ID);
	sStartRegDate.TrimLeft(); sStartRegDate.TrimRight();

	// �谡��2
	pCE = (CESL_Control_Element*)pCM->FindControlElement( _T("�谡��2") );
	if ( pCE == NULL )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::RunSIPreVeiw - 5 ERROR !!") );
		AfxMessageBox( sErrMsg );
		delete pSIReportManager;
		return -1;
	}
	
	sEndRegDate = CLocCommonAPI::GetDate(this,pCE->CTRL_ID);
	sEndRegDate.TrimLeft(); sEndRegDate.TrimRight();

	sData.Format( _T("%s ~ %s"), sStartRegDate, sEndRegDate );
	pSIReportManager->SetTempVariable(_T("�谡������"), sData);
	
	// ��豸��
	CString sAlias1;
	CString sAlias2;
	if ( m_sStaticsName == _T("���Ա��к����(�����)") )
	{
		sAlias1 = _T("����");
		sAlias2 = _T("����");
	}
	else if ( m_sStaticsName == _T("�������к����") )
	{
		sAlias1 = _T("����");
		sAlias2 = _T("�а�");
	}
	else
	{
		sAlias1 = _T("1");
		sAlias2 = _T("2");
	}

	pSIReportManager->SetTempVariable(_T("��豸��1"), sAlias1);
	pSIReportManager->SetTempVariable(_T("��豸��2"), sAlias2);

	// �����
	CTime tCur = CTime::GetCurrentTime();
	CString sCurTime = tCur.Format( _T("%Y��%m�� ����") );
	pSIReportManager->SetTempVariable(_T("�����"), sCurTime);
	pSIReportManager->SetDataMgr(0, pDM_Dst );

	pSIReportManager->MakeXMLPageFiles();
//	Print_SetLastPageLine();

	ids = pSIReportManager->Print2();
	if (pSIReportManager) 
	{
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	

	return 0;
}

INT CBO_STATICS_ND::Print_SetLastPageLine()
{
	INT ids;

	// XML �� WorkForm��ü�� �����Ѵ�.
	CSIReportXMLIOMgr *pXMLIOMgr = new CSIReportXMLIOMgr(MASTER_IS_MANAGER);
	CSIReportWorkForm *pPageWorkForm = new CSIReportWorkForm(MASTER_IS_MANAGER);
	
	if (pXMLIOMgr == NULL) {
		AfxMessageBox(_T("XML ó���� ������ �����Ͽ����ϴ�."));
		return -1;
	}
	if (pPageWorkForm == NULL) {
		AfxMessageBox(_T("�߰� XML ó���� ������ �����Ͽ����ϴ�."));
		return -1;
	}

	// ���� ������ ������ ��ȣ�� �����´�.
	INT nMaxTmpPageNum = pXMLIOMgr->GetMaxTmpPageNum();
	if (nMaxTmpPageNum <= 0) {
		AfxMessageBox(_T("������ ������ ã�� �� �����ϴ�."));
		return -1;
	}
	CString strFileName;
	strFileName.Format(_T("rpt_tmp\\page%04d.xml"), nMaxTmpPageNum);
	// ���� ������ ������ ������ �ҷ��´�.
	ids = pPageWorkForm->LoadWorkForm(strFileName);
	if (ids != 0) {
		AfxMessageBox(_T("�������� �ε� ���� ���Ͽ����ϴ�."));
		return -1;
	}
	// �ҷ��� WorkForm�߿��� First Sheet�� �����´�.(�߰� ������ ���Ͽ��� Sheet�� �Ѱ����̴�.)
	CSIReportWorkSheet *pSheet = pPageWorkForm->FindSheet(0);
	if (pSheet == NULL) {
		AfxMessageBox(_T("�������� ã�� �� �����ϴ�."));
		return -1;
	}
	// ������ ������ ó���� �̹� �Ǿ� �ִ��� ����. �̹� ó���Ǿ� �ִٸ� �ι� �� �ʿ� ����.
	CSIReportObject * pObject = pSheet->FindObject(_T("�߰��ȸ�����������"));
	if (pObject) return -1;
	// �ܺ����̺� �� �ݺ� ���̺��� �����´�.(���̺� �̸��� ��Ȳ�� ���� ������ ��)
	CSIReportObject_Table * pIn = (CSIReportObject_Table*)pSheet->FindObject(_T("���̺�_7"));
	CSIReportObject_Table * pOut = (CSIReportObject_Table*)pSheet->FindObject(_T("���̺�_37"));
	if (pIn == NULL) return -1;
	if (pOut == NULL) return -1;

	CRect InLeft, InRight, OutLeft, OutRight, NewRect;

	CSIReportObject_Table_Cell *pCell;

	// �ݺ����̺��� ������ ������ ���� ó���� ������ ���������� Rect�� ���ϰ�
	pCell = NULL;
	pCell = pIn->GetCell(pIn->row_count-1, 0);
	if (pCell) InLeft = pCell->GetRect();
	
	pCell = NULL;
	pCell = pIn->GetCell(pIn->row_count-1, pIn->col_count-1);
	if (pCell) InRight = pCell->GetRect();

	// �ܺ����̺��� ������ ������ ���� ó�� ���� Rect�� ���Ѵ�.
	pCell = NULL;
	pCell = pOut->GetCell(pOut->row_count-1, 0);
	if (pCell) OutLeft = pCell->GetRect();

	// �ܺ����̺��� �ݺ����̺��� ������ ������ �������� �߰����ش�.
	if (InLeft.bottom < OutLeft.bottom) {
		// ������ �������� Rect�� ����Ѵ�.
		NewRect.top = InLeft.bottom;
		NewRect.bottom = InLeft.bottom;
		NewRect.left = InLeft.left;
		NewRect.right = InRight.right;

		// ������ �������� �߰� ������ ���Ͽ� �߰��Ѵ�.
		CSIReportObject_Line *pLine = (CSIReportObject_Line *)pSheet->AddNewObject(OBJECT_LINE);
		if (pLine) {
			pLine->SetName(_T("�߰��ȸ�����������"));
			pLine->SetRect(NewRect);
		}

		// ����� �߰������� ������ �����Ѵ�.
		ids = pXMLIOMgr->SaveToXML(pPageWorkForm, strFileName);
	}

	// ������ Object�� ����
	if (pXMLIOMgr) {
		delete pXMLIOMgr;
		pXMLIOMgr = NULL;
	}
	if (pPageWorkForm) {
		delete pPageWorkForm;
		pPageWorkForm = NULL;
	}
	return 0 ;
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-18
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   �Լ�����   ] 
// �񵵼��ڷ����, �����߰��ڷ� ���, �������
//
// [  PARAMETERS  ]
//    sWhere  : ������     
//
// [ RETURN VALUE ]
//    0    : ���� 
//    ���� : ����
//
// [   �̴Ͻ���   ]
// 1. ���DM �ʱ�ȭ
// 2. �⺻��� ���� ����
// 3. ���� ��� ����
// 4. ���� ��� ����
// 5. �հ� ����
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::MakeStatics_ND(CString sWhere)
{
	INT ids;
	CString sErrMsg;
	
	//-----------------------------------------------------------------------------
	// 1. ���DM �ʱ�ȭ
	//-----------------------------------------------------------------------------
	CString sDMAlias = _T("DM_�����񵵼��߰��ڷ����_�����");
	CESL_DataMgr* pDM_Dst = FindDM( sDMAlias );
	if ( pDM_Dst == NULL )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ND - 1 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	pDM_Dst->FreeData();

	// ��¿� DM�� ������ �����Ǿ��ִ�.
	INT nRow = 0;
	if ( m_sStaticsName == _T("�������") )
		nRow = 15;
	else if ( m_sStaticsName == _T("�����߰��ڷ����") )
		nRow = 12;
	else if ( m_sStaticsName == _T("�񵵼��ڷ����") )
		nRow = 12;

	const INT nResultRow = nRow;
	const INT nResultCol = 9;

	
	for ( INT i = 0;i < nResultRow;i++ )
	{
		pDM_Dst->InsertRow( -1 );
		for ( INT j = 0;j < nResultCol;j++ )
			pDM_Dst->SetCellData( i, j, _T("0 / 0"));		
	}

	// ��� ������ DM
	CESL_DataMgr DM;
	DM.SetCONNECTION_INFO ( pDM_Dst->CONNECTION_INFO );	

	//-----------------------------------------------------------------------------
	// 2. �⺻��� ���� ����
	//-----------------------------------------------------------------------------	
	CString sSelect;
	CString sTableName;
	CString sBaseWhere;
	CString sGroupBy;
	CString sBoWorkingStatus;
	CString sSeWorkinStatus;
	CString sQuery;
	
	if ( m_sStaticsName == _T("�������") )
	{
		sSelect.Format ( _T("SELECT S.MAT_CODE , B.MANAGE_CODE , B.WORKING_STATUS ") );
		sBaseWhere.Format ( _T(" B.SPECIES_KEY=S.REC_KEY AND B.REG_CODE = 'MM' ")  );
		sGroupBy.Format( _T("Group By S.MAT_CODE , B.MANAGE_CODE , B.WORKING_STATUS") );
	}
	else if ( m_sStaticsName == _T("�����߰��ڷ����") )
	{
		sSelect.Format ( _T("SELECT S.MAT_CODE , B.MANAGE_CODE , B.WORKING_STATUS ") );
		sBaseWhere.Format ( _T(" B.SPECIES_KEY=S.REC_KEY AND B.REG_CODE = 'KN' ") );
		sGroupBy.Format( _T("Group By S.MAT_CODE , B.MANAGE_CODE , B.WORKING_STATUS") );
	}
	else if ( m_sStaticsName == _T("�񵵼��ڷ����") )
	{
		sSelect.Format ( _T("SELECT B.MEDIA_CODE , B.MANAGE_CODE , B.WORKING_STATUS ") );
		sBaseWhere.Format ( _T(" B.SPECIES_KEY=S.REC_KEY AND B.REG_CODE = 'AA' ") );
		sGroupBy.Format( _T("Group By B.MEDIA_CODE , B.MANAGE_CODE , B.WORKING_STATUS") );
	}

	// 17/07/20 ������ : å���������ڷ� ���� �߰�
/*//BEFORE-----------------------------------------------------------------------------
	//=====================================================
	//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
// 	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
// 													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
// 		                                               _T(" 'BOL214O' , 'BOL215O' ) ") );
// 	sSeWorkinStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'SEL112N' , 'SEL212N' , 'SEL213O' , 'SEL214O' , ")
// 										              _T(" 'SEL215O' , 'SEL216O' , 'SEL311O' , 'SEL312O' , ")
// 		                                              _T(" 'SEL313O' , 'SEL314O' , 'SEL315O' ) ") );
	// 2013.12.01 UPDATE BY BJS : 2013���������ռ���Ȯ�뱸��װ��� (���յ���Ÿ������ ���� �߰�)
	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
		                                               _T(" 'BOL214O' , 'BOL215O' , 'BOL218O' , 'BOL511O' , 'BOL611O') ") );	
//	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
//													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
//		                                               _T(" 'BOL214O' , 'BOL215O' , 'BOL218O' , 'BOL511O' ) ") );
	//=====================================================
*///AFTER------------------------------------------------------------------------------
// 18.09.27 JMJ KOL.RED.2018.008
	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
		                                               _T(" 'BOL214O' , 'BOL215O' , 'BOL218O' , 'BOL219O' , ")
													   _T(" 'BOL411O' , 'BOL511O' , 'BOL611O') ") );	
/*//END-------------------------------------------------------------------------------*/	
// 18.09.27 JMJ KOL.RED.2018.008
	sSeWorkinStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'SEL112N' , 'SEL212N' , 'SEL213O' , 'SEL214O' , ")
										              _T(" 'SEL215O' , 'SEL216O' , 'SEL311O' , 'SEL312O' , ")
		                                              _T(" 'SEL313O' , 'SEL314O' , 'SEL315O' , 'SEL411O', 'SEL511O' ) ") );

	//-----------------------------------------------------------------------------
	// 3. ���� ��� ����
	//-----------------------------------------------------------------------------	
	sTableName = _T("BO_SPECIES_TBL S , BO_BOOK_TBL B ");
	sQuery.Format(_T("%s , COUNT(DISTINCT S.REC_KEY), COUNT(DISTINCT B.REC_KEY) FROM %s WHERE %s ")
				  		_T(" AND B.BOOK_APPENDIX_FLAG = 'B' ") 						
				  , sSelect, sTableName, sBaseWhere );

	sQuery += sWhere + sBoWorkingStatus + sGroupBy;
	
	ids = DM.RestructDataManager( sQuery );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ND - 2 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	ids = CopySrcToDst3( &DM, pDM_Dst );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ND - 3 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	//-----------------------------------------------------------------------------
	// 4. ���� ��� ����
	// 2005-11-21 UPD BY PDW
	// �񵵼� �ڷ�� �����Ѵ�.
	//-----------------------------------------------------------------------------	
	if ( m_sStaticsName != _T("�񵵼��ڷ����") )
	{
		DM.FreeData();
		sTableName = _T("SE_SPECIES_TBL S , SE_BOOK_TBL B ");
		sQuery.Format(_T("%s , COUNT(DISTINCT S.REC_KEY) , COUNT(DISTINCT B.REC_KEY) FROM %s WHERE %s ")				  		  
					  , sSelect, sTableName, sBaseWhere );
		sQuery += sWhere + sSeWorkinStatus + sGroupBy;
		
		ids = DM.RestructDataManager( sQuery );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ND - 4 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}

		ids = CopySrcToDst3( &DM, pDM_Dst );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ND - 5 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}
	}

	//-----------------------------------------------------------------------------
	// 5. �հ� ����
	//-----------------------------------------------------------------------------	
	// ���� �հ� ���ϱ� 
	UpdateRowSum( pDM_Dst, 0, 2, 3 );
	UpdateRowSum( pDM_Dst, 4, 6, 7 );
	UpdateRowSum( pDM_Dst, 3, 3, 8 );	
	UpdateRowSum( pDM_Dst, 7, 7, 8 );
	// ���� �հ� ���ϱ� 
	UpdateColSum( pDM_Dst, 0, nResultRow - 3, nResultRow - 2 );
	// ���� ���ϱ�
	CString sTemp;
	CString sSpeices;
	CString sBook;
	ids = pDM_Dst->GetCellData( nResultRow - 2, nResultCol - 1 , sTemp );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ND - 6 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	GetDivValue( sTemp, &sSpeices, &sBook );

	UpdatePercentage(pDM_Dst, nResultRow - 2, nResultRow - 1, _ttoi(sBook), 0 ); 


	return 0;
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-18
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   �Լ�����   ] 
// DM�� �����÷����� ���÷����� ���� �հ踦 Ÿ���÷��� �Է��Ѵ�.
//
// [  PARAMETERS  ]
//    pDM       : �հ� ���� DM
//    nStartCol : ���� �÷� idx
//    nEndCol   : �� �÷� idx
//    nTagetCol : ���� �÷� idx
//    nMode     : 0 -> 0/0    1 -> 0 ���·� 
//
// [ RETURN VALUE ]
//    0    : ���� 
//    ���� : ����
//
// [   �̴Ͻ���   ]
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::UpdateRowSum(CESL_DataMgr *pDM, INT nStartCol, INT nEndCol, INT nTagetCol, INT nMode)
{
	INT ids;
	CString sErrMsg;

	if ( pDM == NULL )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::UpdateRowSum - 1 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	CString sData;
	CString sPreData;
	CString sBookCnt;
	CString sSpeciesCnt;
	INT nBookCnt;
	INT nSpeciesCnt;
	INT nRowCnt = pDM->GetRowCount();

	for ( INT i = 0;i < nRowCnt;i++ )
	{
		nBookCnt = 0;
		nSpeciesCnt = 0;

		for ( INT j = nStartCol;j < nEndCol + 1;j++ )
		{
			ids = pDM->GetCellData( i, j, sData );
			if ( ids < 0 )
			{
				sErrMsg.Format( _T("CBO_STATICS_ND::UpdateRowSum - 2 ERROR!!") );
				AfxMessageBox( sErrMsg );
				return -1;
			}

			GetDivValue( sData, &sSpeciesCnt, &sBookCnt );

			nBookCnt += _ttoi( sBookCnt );
			nSpeciesCnt += _ttoi( sSpeciesCnt );
		}

		// ���� �����Ϳ� ���Ѵ�.
		ids = pDM->GetCellData( i, nTagetCol, sPreData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::UpdateRowSum - 3 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}

		GetDivValue( sPreData, &sSpeciesCnt, &sBookCnt );

		nBookCnt += _ttoi( sBookCnt );
		nSpeciesCnt += _ttoi( sSpeciesCnt );

		if ( nMode == 0 )
			sData.Format( _T("%d / %d"), nSpeciesCnt, nBookCnt );
		else if ( nMode == 1 )
			sData.Format( _T("%d"), nSpeciesCnt );
		ids = pDM->SetCellData( i, nTagetCol, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::UpdateRowSum - 4 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}
	}

	return 0;
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-18
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   �Լ�����   ] 
// DM�� ���۷ο���� ���ο���� ���� �հ踦 Ÿ�ٷο쿡 �Է��Ѵ�.
//
// [  PARAMETERS  ]
//    pDM       : �հ� ���� DM
//    nStartRow : ���� �ο� idx
//    nEndRow   : �� �ο� idx
//    nTagetRow : ���� �ο� idx
//    nMode     : 0 -> 0/0    1 -> 0 ���·� 
//
// [ RETURN VALUE ]
//    0    : ���� 
//    ���� : ����
//
// [   �̴Ͻ���   ]
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::UpdateColSum(CESL_DataMgr *pDM, INT nStartRow, INT nEndRow, INT nTagetRow, INT nMode)
{
	INT ids;
	CString sErrMsg;

	if ( pDM == NULL )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::UpdateColSum - 1 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	CString sData;
	CString sPreData;
	CString sBookCnt;
	CString sSpeciesCnt;
	INT nBookCnt;
	INT nSpeciesCnt;
	INT nColCnt = pDM->GetColCount();

	for ( INT i = 0;i < nColCnt;i++ )
	{
		nBookCnt = 0;
		nSpeciesCnt = 0;

		for ( INT j = nStartRow;j < nEndRow + 1;j++ )
		{
			ids = pDM->GetCellData( j, i, sData );
			if ( ids < 0 )
			{
				sErrMsg.Format( _T("CBO_STATICS_ND::UpdateColSum - 2 ERROR!!") );
				AfxMessageBox( sErrMsg );
				return -1;
			}

			GetDivValue( sData, &sSpeciesCnt, &sBookCnt );

			nBookCnt += _ttoi( sBookCnt );
			nSpeciesCnt += _ttoi( sSpeciesCnt );
		}

		// ���� �����Ϳ� ���Ѵ�.
		ids = pDM->GetCellData( nTagetRow, i, sPreData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::UpdateRowSum - 3 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}

		GetDivValue( sPreData, &sSpeciesCnt, &sBookCnt );

		nBookCnt += _ttoi( sBookCnt );
		nSpeciesCnt += _ttoi( sSpeciesCnt );

		if ( nMode == 0 )
			sData.Format( _T("%d / %d"), nSpeciesCnt, nBookCnt );
		else if ( nMode == 1 )
			sData.Format( _T("%d"), nSpeciesCnt );

		ids = pDM->SetCellData( nTagetRow, i, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::UpdateColSum - 4 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}
	}

	return 0;
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-18
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   �Լ�����   ] 
// �弭 ���� ��Ȳ ���
//
// [  PARAMETERS  ]
//    sWhere  : ������     
//
// [ RETURN VALUE ]
//    0    : ���� 
//    ���� : ����
//
// [   �̴Ͻ���   ]
// 1. ���DM �ʱ�ȭ
// 2. �⺻��� ���� ����
// 3. ���� ��� ����
// 4. ���� ��� ����
// 5. �հ� ����
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::MakeStatics_ALL(CString sWhere)
{
	INT ids;
	CString sErrMsg;

	//-----------------------------------------------------------------------------
	// 1. ���DM �ʱ�ȭ
	//-----------------------------------------------------------------------------
	CString sDMAlias = _T("DM_�弭������Ȳ_�����");
	CESL_DataMgr* pDM_Dst = FindDM( sDMAlias );
	if ( pDM_Dst == NULL )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ALL - 1 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	pDM_Dst->FreeData();

	// ��¿� DM�� ������ �����Ǿ��ִ�.
	const INT nResultRow = 16;
	const INT nResultCol = 4;
	for ( INT i = 0;i < nResultRow;i++ )
	{
		pDM_Dst->InsertRow( -1 );
		for ( INT j = 0;j < nResultCol;j++ )
			pDM_Dst->SetCellData( i, j, _T("0"));		
	}

	// ��� ������ DM
	CESL_DataMgr DM;
	DM.SetCONNECTION_INFO ( pDM_Dst->CONNECTION_INFO );	
	
	//-----------------------------------------------------------------------------
	// 2. �⺻ ���� ���� ����
	//-----------------------------------------------------------------------------
	CString sSelect;
	CString sQuery;
	CString sBoWorkingStatus;
	CString sSeWorkinStatus;

	// 17/07/20 ������ : å���������ڷ� ���� �߰� 
/*//BEFORE-----------------------------------------------------------------------------
	//=====================================================
	//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
// 	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
// 													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
// 		                                               _T(" 'BOL214O' , 'BOL215O' ) ") );
// 	sSeWorkinStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'SEL112N' , 'SEL212N' , 'SEL213O' , 'SEL214O' , ")
// 										              _T(" 'SEL215O' , 'SEL216O' , 'SEL311O' , 'SEL312O' , ")
// 		                                              _T(" 'SEL313O' , 'SEL314O' , 'SEL315O' ) ") );

	// 2013.12.01 UPDATE BY BJS : 2013���������ռ���Ȯ�뱸��װ��� (���յ���Ÿ������ ���� �߰�)
	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
		                                               _T(" 'BOL214O' , 'BOL215O' , 'BOL218O' , 'BOL511O' , 'BOL611O') ") );

//	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
//													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
//		                                               _T(" 'BOL214O' , 'BOL215O' , 'BOL218O' , 'BOL511O' ) ") );	
	//=====================================================
*///AFTER------------------------------------------------------------------------------
// 18.09.27 JMJ KOL.RED.2018.008
	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
		                                               _T(" 'BOL214O' , 'BOL215O' , 'BOL218O' , 'BOL219O' , ")
													   _T(" 'BOL411O' , 'BOL511O' , 'BOL611O') ") );
/*//END-------------------------------------------------------------------------------*/	
// 18.09.27 JMJ KOL.RED.2018.008
	sSeWorkinStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'SEL112N' , 'SEL212N' , 'SEL213O' , 'SEL214O' , ")
										              _T(" 'SEL215O' , 'SEL216O' , 'SEL311O' , 'SEL312O' , ")
		                                              _T(" 'SEL313O' , 'SEL314O' , 'SEL315O' , 'SEL411O' , 'SEL511O' ) ") );
	
	//-----------------------------------------------------------------------------
	// 3. ���� ��� ����
	//-----------------------------------------------------------------------------	
	sQuery.Format(_T("SELECT B.REG_CODE, B.MANAGE_CODE , COUNT(DISTINCT B.REC_KEY) ")
		          _T("FROM BO_SPECIES_TBL S , BO_BOOK_TBL B ")
				  _T("WHERE B.SPECIES_KEY=S.REC_KEY ")
				   _T(" AND B.BOOK_APPENDIX_FLAG = 'B' %s %s ") 
				   _T(" Group By B.REG_CODE , B.MANAGE_CODE "), sWhere , sBoWorkingStatus);
	
	ids = DM.RestructDataManager( sQuery );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ACQ - 2 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	ids = CopySrcToDst2( &DM, pDM_Dst);
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ACQ - 3 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	//-----------------------------------------------------------------------------
	// 4. ���� ��� ����
	//-----------------------------------------------------------------------------	
	DM.FreeData();
	sQuery.Format(_T("SELECT B.REG_CODE, B.MANAGE_CODE , COUNT(DISTINCT B.REC_KEY) ")
		          _T("FROM SE_SPECIES_TBL S , SE_BOOK_TBL B ")
				  _T("WHERE B.SPECIES_KEY=S.REC_KEY ")
				   _T(" %s %s ") 
				   _T(" Group By B.REG_CODE , B.MANAGE_CODE "), sWhere , sSeWorkinStatus);
	
	ids = DM.RestructDataManager( sQuery );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ACQ - 4 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	ids = CopySrcToDst2( &DM, pDM_Dst );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_ACQ - 5 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	//-----------------------------------------------------------------------------
	// 5. �հ� ����
	//-----------------------------------------------------------------------------
	// �������
	UpdateRowSum(pDM_Dst, 0, 2, 3, 1);
	// ������� 
	UpdateColSum(pDM_Dst, 0, 2, 3, 1);
	UpdateColSum(pDM_Dst, 4, 6, 7, 1);
	UpdateColSum(pDM_Dst, 10, 14, 15, 1);
	UpdateColSum(pDM_Dst, 3, 3, 15, 1);
	UpdateColSum(pDM_Dst, 7, 7, 15, 1);
	return 0;
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-18
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   �Լ�����   ] 
// ScrDM���� �ش� ��ϱ��п� ���ؼ� pDstDM�� Ư�� Cell�� �ִ´�.
// �弭���� ��Ȳ��迡�� ���ȴ�.
//
// [  PARAMETERS  ]
//    pSrcDM  : ������ ���� ��� DM     
//    pDstDM  : ����� DM
//
// [ RETURN VALUE ]
//    0    : ���� 
//    ���� : ����
//
// [   �̴Ͻ���   ]
// 1. idx = 0�� ����( ��ϱ��� )�� �Էµ� ��ġ�� �ٲ��.
// 2. idx = 1�� ����( ���Ա��� )�� �Էµ� ��ġ�� �ٲ��.
// 3. idx = 2�� ������ �Էµ� ��� ���̴�.
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::CopySrcToDst2(CESL_DataMgr *pSrcDM, CESL_DataMgr *pDstDM)
{
	INT ids;
	CString sErrMsg;

	CString sRegCode;								// ��ϱ���
	CString sData;								    // ���� ����
    CString sInsertData;                            // �Է°� 
	INT nInsertRowIdx;                              // �Է½�ų �ο��� ��ġ
	INT nInsertColIdx;								// �Է½�ų �÷��� ��ġ
	INT nRowCount = pSrcDM->GetRowCount();
	for ( INT i = 0; i < nRowCount;i++ )
	{
		ids = pSrcDM->GetCellData( i, 0, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst2 - 1 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}

		//-----------------------------------------------------------------------------
		// 1. inx = 0�� ����ġ�� ����( ��ϱ��� )�� �Էµ� ��ġ�� �ٲ��.
		//-----------------------------------------------------------------------------	
		if ( sData == _T("EM") )		nInsertRowIdx = 0;
		else if ( sData == _T("JM") )   nInsertRowIdx = 1;		
		else if ( sData == _T("WM") )   nInsertRowIdx = 2;
		else if ( sData == _T("KN") )   nInsertRowIdx = 10;
		else if ( sData == _T("DJ") || sData == _T("DM") || sData == _T("DS") ) nInsertRowIdx = 12;
		else if ( sData == _T("MI") )   nInsertRowIdx = 13;
		else if ( sData == _T("MM") )   nInsertRowIdx = 11;
		else if ( sData == _T("ES") )   nInsertRowIdx = 4;
		else if ( sData == _T("JS") )   nInsertRowIdx = 5;
		else if ( sData == _T("WS") )	nInsertRowIdx = 6;
		else						nInsertRowIdx = 14;
		
		//-----------------------------------------------------------------------------
		// 2. inx = 1�� ����ġ�� ����( ���� ���� )�� �Էµ� ��ġ�� �ٲ��.
		//-----------------------------------------------------------------------------	
		ids = pSrcDM->GetCellData( i, 1, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst2 - 2 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}
		
		if ( sData == _T("MA") )
			nInsertColIdx = 0;
		else if ( sData == _T("BR") )
			nInsertColIdx = 1;
		else
			nInsertColIdx = 2;			
		
		//-----------------------------------------------------------------------------
		// 3. idx = 2�� ������ �Էµ� ������
		//-----------------------------------------------------------------------------	
		CString sCurBook;
		CString sCurSpecies;

		ids = pSrcDM->GetCellData( i, 2, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst2 - 3 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}

		// 2005-12-26 ADD BY PDW(Jr) =================================================
		// ���� �����Ϳ� ���ϵ��� ���� 
		GetDivValue( sData, &sCurSpecies, &sCurBook );

		// �Է�
		CString sSpecies;
		CString sBook;
		CString sPreData;

		ids = pDstDM->GetCellData( nInsertRowIdx, nInsertColIdx, sPreData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst2 - 4 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}		

		GetDivValue( sPreData, &sSpecies, &sBook );
		
		// �弭������Ȳ�� å���� ��Ÿ����.
		sData.Format( _T("%d"), _ttoi( sSpecies ) + _ttoi( sCurSpecies ) );
		// ===========================================================================

		ids = pDstDM->SetCellData( nInsertRowIdx, nInsertColIdx, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst2 - 5 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}
	}

	return 0;
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-18
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   �Լ�����   ] 
// ScrDM���� �ش� �ڷᱸ�п� ���ؼ� pDstDM�� Ư�� Cell�� �ִ´�.
// �����߰��ڷ�, �������, �񵵼��ڷ���迡 ���ȴ�.
//
// [  PARAMETERS  ]
//    pSrcDM  : ������ ���� ��� DM     
//    pDstDM  : ����� DM
//
// [ RETURN VALUE ]
//    0    : ���� 
//    ���� : ����
//
// [   �̴Ͻ���   ]
// 1. idx = 0�� ����ġ�� ����( �ڷᱸ�� || ��ü���� )�� �Էµ� ��ġ�� �ٲ��.
// 2. idx = 1, idx = 2�� ����ġ�� ����( ���� ���� && �ڷ���� )�� �Էµ� ��ġ�� �ٲ��.
// 3. idx = 3,4�� ������ �Էµ� ������
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::CopySrcToDst3(CESL_DataMgr *pSrcDM, CESL_DataMgr *pDstDM)
{
	INT ids;
	CString sErrMsg;

	CString sMatCode;								// �ڷᱸ��
	CString sManageCode;							// ��������
	CString sData;
	CString sTemp;
	CString sPreSpecies;
	CString sPreBook;

    CString sInsertData;                            // �Է°� 
	INT nInsertRowIdx;                              // �Է½�ų �ο��� ��ġ
	INT nInsertColIdx;								// �Է½�ų �÷��� ��ġ
	INT nRowCount = pSrcDM->GetRowCount();
	for ( INT i = 0; i < nRowCount;i++ )
	{
		ids = pSrcDM->GetCellData( i, 0, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst3 - 1 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}

		//-----------------------------------------------------------------------------
		// 1. idx = 0�� ����ġ�� ����( �ڷᱸ�� || ��ü���� )�� �Էµ� ��ġ�� �ٲ��.
		//-----------------------------------------------------------------------------	
		if      ( sData == _T("QA") || sData == _T("MD") || sData == _T("SG") )	  nInsertRowIdx = 0;
		else if ( sData == _T("QB") || sData == _T("ME") || sData == _T("PC") )   nInsertRowIdx = 1;		
		else if ( sData == _T("QC") || sData == _T("MF") || sData == _T("OB") )   nInsertRowIdx = 2;
		else if ( sData == _T("QD") || sData == _T("MH") || sData == _T("XA") )   nInsertRowIdx = 3;
		else if ( sData == _T("QE") || sData == _T("MJ") || sData == _T("XU") )   nInsertRowIdx = 4;
		else if ( sData == _T("QF") || sData == _T("MK") || sData == _T("MA") )   nInsertRowIdx = 5;
		else if ( sData == _T("QG") || sData == _T("ML") || sData == _T("LF") )   nInsertRowIdx = 6;
		else if ( sData == _T("QH") || sData == _T("MN") || sData == _T("LQ") )   nInsertRowIdx = 7;
		else if ( sData == _T("QI") || sData == _T("MQ") || sData == _T("LP") )   nInsertRowIdx = 8;
		else if ( sData == _T("QJ") )					                  nInsertRowIdx = 9;
		else if ( sData == _T("QK") )					                  nInsertRowIdx = 10;
		else if ( sData == _T("QL") )					                  nInsertRowIdx = 11;
		else										 
		{
			if ( m_sStaticsName == _T("�������") )                       nInsertRowIdx = 12;
			else if ( m_sStaticsName == _T("�����߰��ڷ����") )          nInsertRowIdx = 9;
			else if ( m_sStaticsName == _T("�񵵼��ڷ����") )            nInsertRowIdx = 9;
		}
		
		//-----------------------------------------------------------------------------
		// 2. idx = 1, idx = 2�� ����ġ�� ����( ���� ���� && �ڷ���� )�� �Էµ� ��ġ�� �ٲ��.
		//-----------------------------------------------------------------------------	
		ids = pSrcDM->GetCellData( i, 1, sManageCode );			// ��������
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst3 - 2 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}
		
		ids = pSrcDM->GetCellData( i, 2, sData );				// �ڷ����
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst3 - 3 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}

		if ( sManageCode == _T("MA") )			nInsertColIdx = 0;
		else if ( sManageCode == _T("BR") )		nInsertColIdx = 1;
		else								nInsertColIdx = 2;

		// 17/07/20 ������ : å���������ڷ� ���� �߰� 
/*//BEFORE-----------------------------------------------------------------------------
		//=====================================================
		//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
// 		if ( sData == _T("BOL211O") || sData == _T("BOL212O") || sData == _T("BOL213O") || sData == _T("BOL214O") || sData == _T("BOL215O") ||
// 			 sData == _T("SEL3110") || sData == _T("SEL312O") || sData == _T("SEL313O") || sData == _T("SEL314O") || sData == _T("SEL315O") )
// 			 nInsertColIdx += 4;
		// 2013.12.01 UPDATE BY BJS : 2013���������ռ���Ȯ�뱸��װ��� (���յ���Ÿ������ ���� �߰�)
		if ( sData == _T("BOL211O") || sData == _T("BOL212O") || sData == _T("BOL213O") || sData == _T("BOL214O") || sData == _T("BOL215O") || _T("BOL218O") || sData == _T("BOL511O") ||
			 sData == _T("SEL3110") || sData == _T("SEL312O") || sData == _T("SEL313O") || sData == _T("SEL314O") || sData == _T("SEL315O") || _T("BOL5110"))
			 nInsertColIdx += 4;
//		if ( sData == _T("BOL211O") || sData == _T("BOL212O") || sData == _T("BOL213O") || sData == _T("BOL214O") || sData == _T("BOL215O") || _T('BOL218O') || sData == _T("BOL511O") ||
//			 sData == _T("SEL3110") || sData == _T("SEL312O") || sData == _T("SEL313O") || sData == _T("SEL314O") || sData == _T("SEL315O") )
//			 nInsertColIdx += 4;
		//=====================================================
*///AFTER------------------------------------------------------------------------------
// 18.09.27 JMJ KOL.RED.2018.008
		if ( sData == _T("BOL211O") || sData == _T("BOL212O") || sData == _T("BOL213O") || sData == _T("BOL214O") || sData == _T("BOL215O") || sData == _T("BOL218O") || sData == _T("BOL219O") || sData == _T("BOL411O") ||  sData == _T("BOL511O") || sData == _T("BOL611O") ||
			 sData == _T("SEL311O") || sData == _T("SEL312O") || sData == _T("SEL313O") || sData == _T("SEL314O") || sData == _T("SEL315O") || sData == _T("SEL411O") || sData == _T("SEL511O"))
			 nInsertColIdx += 4;
/*//END-------------------------------------------------------------------------------*/
		
		//-----------------------------------------------------------------------------
		// 3. idx = 3,4�� ������ �Էµ� ������
		//-----------------------------------------------------------------------------	
		ids = pSrcDM->GetCellData( i, 3, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst3 - 4 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}
		
		sInsertData = sData;

		ids = pSrcDM->GetCellData( i, 4, sData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst3 - 5 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}

		// �Է�
		// ���������� ���Ѵ�.
		ids = pDstDM->GetCellData( nInsertRowIdx, nInsertColIdx, sTemp );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst3 - 6 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}

		GetDivValue( sTemp, &sPreSpecies, &sPreBook );

		sInsertData.Format(_T("%d / %d") , _ttoi(sInsertData) + _ttoi(sPreSpecies) , _ttoi(sData) + _ttoi(sPreBook));

		ids = pDstDM->SetCellData( nInsertRowIdx, nInsertColIdx, sInsertData );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst3 - 7 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}
	}

	return 0;
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-18
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   �Լ�����   ] 
// DM�� Ÿ����ġ�� ���� ������� Ÿ�� �ʵ忡 �Է��Ѵ�.
// å�� ���� ������̴�.
//
// [  PARAMETERS  ]
//    pDstDM         : �ۼ�Ʈ ���� DM
//    nTotalValuePos : �հ� �÷� idx
//    nUpdateField   : Ÿ�� �ۼ�Ʈ �÷� idx
//    nTotalValue    : ���հ� 
//    nMode          : 0 -> �ο���� 1 -> �÷����� �� ����Ѵ�.
//
// [ RETURN VALUE ]
//    0    : ���� 
//    ���� : ����
//
// [   �̴Ͻ���   ]
// 1. nTotalValuePos ��ġ�� ���� nTotalValue ������ �ۼ�Ʈ�� ���Ѵ�.
// 2. nUpdateField�� �Է��Ѵ�.
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::UpdatePercentage(CESL_DataMgr *pDstDM, INT nTotalValuePos, INT nUpdateField, INT nTotalValue, INT nMode)
{
	INT ids;
	INT nLoofCnt;
	CString sErrMsg;

	// �ο� ���� �۾��� �Ѵ�.
	CString sTemp;
	CString sSpeices;
	CString sBook;
	float fPercent;
	if ( nMode == 0 )
	{
		nLoofCnt = pDstDM->GetColCount();
		
		for ( INT i = 0;i < nLoofCnt;i++ )
		{
			ids = pDstDM->GetCellData( nTotalValuePos, i, sTemp );
			if ( ids < 0 )
			{
				sErrMsg.Format( _T("CBO_STATICS_ND::UpdatePercentage - 1 ERROR!!") );
				AfxMessageBox( sErrMsg );
				return -1;
			}

			if ( nTotalValue > 0 )
			{
				GetDivValue( sTemp, &sSpeices, &sBook);
			
				fPercent = (float)_ttoi(sBook) / (float)nTotalValue;
				sTemp.Format( _T("%.02f"), fPercent * 100 );			
			}
			else
			{
				sTemp.Format( _T("0") );
			}

			ids = pDstDM->SetCellData( nUpdateField, i, sTemp );
			if ( ids < 0 )
			{
				sErrMsg.Format( _T("CBO_STATICS_ND::UpdatePercentage - 2 ERROR!!") );
				AfxMessageBox( sErrMsg );
				return -1;
			}
		}
		/*
		ids = pDstDM->GetCellData( nInsertRowIdx, nInsertColIdx, sTemp );
		if ( ids < 0 )
		{
			sErrMsg.Format( _T("CBO_STATICS_ND::CopySrcToDst3 - 6 ERROR!!") );
			AfxMessageBox( sErrMsg );
			return -1;
		}
		*/
	}
	// �÷� ���� �۾��� �Ѵ�.
	else if ( nMode == 1)
	{
		nLoofCnt = pDstDM->GetRowCount();
		for ( INT i = 0;i < nLoofCnt;i++ )
		{
			ids = pDstDM->GetCellData( i, nTotalValuePos, sTemp );
			if ( ids < 0 )
			{
				sErrMsg.Format( _T("CBO_STATICS_ND::UpdatePercentage - 3 ERROR!!") );
				AfxMessageBox( sErrMsg );
				return -1;
			}

			GetDivValue( sTemp, &sSpeices, &sBook);
			
			fPercent = (float)_ttoi(sBook) / (float)nTotalValue;
			sTemp.Format( _T("%.02f"), fPercent * 100 );	
			
			ids = pDstDM->SetCellData( i, nUpdateField, sTemp );
			if ( ids < 0 )
			{
				sErrMsg.Format( _T("CBO_STATICS_ND::UpdatePercentage - 4 ERROR!!") );
				AfxMessageBox( sErrMsg );
				return -1;
			}
		}
	}

	return 0;
}

//--------------------------------------------------------------------------------------------
// Work Day : 2005-11-22
// Author   : Park, Daewoo(Jr)
// Type     : ADD   
//
// [   �Լ�����   ] 
// �ڷ� ���� ���
//
// [  PARAMETERS  ]
//    sWhere  : ������     
//
// [ RETURN VALUE ]
//    0    : ���� 
//    ���� : ����
//
// [   �̴Ͻ���   ]
// 1. ���DM �ʱ�ȭ
// 2. �⺻��� ���� ����
// 3. ���� ��� ����
// 4. �հ� ����
//--------------------------------------------------------------------------------------------
INT CBO_STATICS_ND::MakeStatics_Class(CString sWhere)
{
	INT ids;
	CString sErrMsg;
	
	//-----------------------------------------------------------------------------
	// 1. ���DM �ʱ�ȭ
	//-----------------------------------------------------------------------------
	CString sDMAlias = _T("DM_�ڷ�������_�����");
	CESL_DataMgr* pDM_Dst = FindDM( sDMAlias );
	if ( pDM_Dst == NULL )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_Class - 1 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	pDM_Dst->FreeData();

	// ��¿� DM�� ������ �����Ǿ��ִ�.
	const INT nResultRow = 18;
	const INT nResultCol = 13;
	for ( INT i = 0;i < nResultRow;i++ )
	{
		pDM_Dst->InsertRow( -1 );
		for ( INT j = 0;j < nResultCol;j++ )
			pDM_Dst->SetCellData( i, j, _T("0 / 0"));		
	}

	// ��� ������ DM
	CESL_DataMgr DM;
	DM.SetCONNECTION_INFO ( pDM_Dst->CONNECTION_INFO );	

	//-----------------------------------------------------------------------------
	// 2. �⺻ ���� ���� ����
	//-----------------------------------------------------------------------------
	CString sSelect;
	CString sTableName;
	CString sBaseWhere;
	CString sGroupBy;
	CString sBoWorkingStatus;
	CString sSeWorkinStatus;
	CString sQuery;
	
	
	sSelect.Format ( _T("SELECT B.REG_CODE, ") 
		             _T(" DECODE( SIGN( ASCII( SUBSTR( LTRIM( B.CLASS_NO, ' ' ) , 0, 1 ) ) - '58' ), -1, DECODE( SIGN( ASCII( SUBSTR( LTRIM( B.CLASS_NO, ' ' ) , 0, 1 ) ) - '47' ) , 1 , SUBSTR( LTRIM( B.CLASS_NO, ' ' ), 0, 1 ), '' ), DECODE( SUBSTR( LTRIM( B.CLASS_NO, ' ' ) , 0, 1 ) , NULL, '' , 'A' , '0' , 'Z' , '0' , 'B' , DECODE( SIGN( ASCII( SUBSTR( LTRIM( B.CLASS_NO, ' ' ) , 1, 1 ) ) - '74' ) , 1, '2' , '1' ) , 'H' , '3' , 'J' , '3' , 'K' , '3' , 'L' , '3' , 'U' , '3' , 'V' , '3' , 'Q' , '4' , 'R' , '5' , 'S' , '5' , 'T' , '5' , 'M' , '6' , 'N' , '6' , 'P' , DECODE( SUBSTR( LTRIM( B.CLASS_NO, ' ' ) , 1, 1 ) , 'C' , '7' , 'E' , '7' , 'F' , '7' , 'K' , '7' , 'M' , '7' , '8' ) , 'C' , '9' , 'D' , '9' , 'E' , '9' , 'F' , '9' , 'G' , '9' , '' ) )") );
	sBaseWhere.Format ( _T(" B.SPECIES_KEY=S.REC_KEY ") );
	sGroupBy.Format( _T("Group By B.REG_CODE , ")
		             _T(" DECODE( SIGN( ASCII( SUBSTR( LTRIM( B.CLASS_NO, ' ' ) , 0, 1 ) ) - '58' ), -1, DECODE( SIGN( ASCII( SUBSTR( LTRIM( B.CLASS_NO, ' ' ) , 0, 1 ) ) - '47' ) , 1 , SUBSTR( LTRIM( B.CLASS_NO, ' ' ), 0, 1 ), '' ), DECODE( SUBSTR( LTRIM( B.CLASS_NO, ' ' ) , 0, 1 ) , NULL, '' , 'A' , '0' , 'Z' , '0' , 'B' , DECODE( SIGN( ASCII( SUBSTR( LTRIM( B.CLASS_NO, ' ' ) , 1, 1 ) ) - '74' ) , 1, '2' , '1' ) , 'H' , '3' , 'J' , '3' , 'K' , '3' , 'L' , '3' , 'U' , '3' , 'V' , '3' , 'Q' , '4' , 'R' , '5' , 'S' , '5' , 'T' , '5' , 'M' , '6' , 'N' , '6' , 'P' , DECODE( SUBSTR( LTRIM( B.CLASS_NO, ' ' ) , 1, 1 ) , 'C' , '7' , 'E' , '7' , 'F' , '7' , 'K' , '7' , 'M' , '7' , '8' ) , 'C' , '9' , 'D' , '9' , 'E' , '9' , 'F' , '9' , 'G' , '9' , '' ) )") );
	
	// 17/07/20 ������ : å���������ڷ� ���� �߰� 
/*//BEFORE-----------------------------------------------------------------------------
	//=====================================================
	//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
// 	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
// 													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
// 		                                               _T(" 'BOL214O' , 'BOL215O' ) ") );

// 	sSeWorkinStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'SEL112N' , 'SEL212N' , 'SEL213O' , 'SEL214O' , ")
// 										              _T(" 'SEL215O' , 'SEL216O' , 'SEL311O' , 'SEL312O' , ")
// 		                                              _T(" 'SEL313O' , 'SEL314O' , 'SEL315O' ) ") );

	// 2013.12.01 UPDATE BY BJS : 2013���������ռ���Ȯ�뱸��װ��� (���յ���Ÿ������ ���� �߰�)
	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
		                                               _T(" 'BOL214O' , 'BOL215O' , 'BOL218O' , 'BOL511O' , 'BOL511O') ") );

//	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
//													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
//		                                               _T(" 'BOL214O' , 'BOL215O' , 'BOL218O' , 'BOL511O' ) ") );
	//=====================================================
*///AFTER------------------------------------------------------------------------------
// 18.09.27 JMJ KOL.RED.2018.008
	sBoWorkingStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'BOL112N' , 'BOL113O' , 'BOL114O' , 'BOL115O' , ")
													   _T(" 'BOL116O' , 'BOL211O' , 'BOL212O' , 'BOL213O' , ")
		                                               _T(" 'BOL214O' , 'BOL215O' , 'BOL218O' , 'BOL219O' , ")
													   _T(" 'BOL411O' , 'BOL511O' , 'BOL611O') ") );
/*//END-------------------------------------------------------------------------------*/	
// 18.09.27 JMJ KOL.RED.2018.008
	sSeWorkinStatus.Format( _T(" AND B.WORKING_STATUS IN ( 'SEL112N' , 'SEL212N' , 'SEL213O' , 'SEL214O' , ")
										              _T(" 'SEL215O' , 'SEL216O' , 'SEL311O' , 'SEL312O' , ")
		                                              _T(" 'SEL313O' , 'SEL314O' , 'SEL315O' , 'SEL411O' , 'SEL511O' ) ") );
	

	//-----------------------------------------------------------------------------
	// 3. ���� ��� ����
	//-----------------------------------------------------------------------------	
	sTableName = _T("BO_SPECIES_TBL S , BO_BOOK_TBL B ");
	sQuery.Format(_T("%s , COUNT(DISTINCT S.REC_KEY), COUNT(DISTINCT B.REC_KEY) FROM %s WHERE %s ")
				  		_T(" AND B.BOOK_APPENDIX_FLAG = 'B' %s ")
				  , sSelect, sTableName, sBaseWhere, sWhere);
	sQuery += sBoWorkingStatus + sGroupBy;
	
	ids = DM.RestructDataManager( sQuery );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_Class - 2 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	ids = CopySrcToDst1( &DM, pDM_Dst, 0 );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_Class - 3 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	//-----------------------------------------------------------------------------
	// 4. ���� ��� ����
	//-----------------------------------------------------------------------------	
	DM.FreeData();
	sTableName = _T("SE_SPECIES_TBL S , SE_BOOK_TBL B ");
	sQuery.Format(_T("%s , COUNT(DISTINCT S.REC_KEY) , COUNT(DISTINCT B.REC_KEY) FROM %s WHERE %s %s ")				  	  
				  , sSelect, sTableName, sBaseWhere, sWhere );
	sQuery += sSeWorkinStatus + sGroupBy;
	
	ids = DM.RestructDataManager( sQuery );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_Class - 4 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	ids = CopySrcToDst1( &DM, pDM_Dst, 1 );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_Class - 5 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	//-----------------------------------------------------------------------------
	// 5. �հ� ����
	//-----------------------------------------------------------------------------	
	// ���� ��� ���ϱ� 
	UpdateRowSum( pDM_Dst, 0, 10, 11 );
	// ���� ��� ���ϱ� 
	UpdateColSum( pDM_Dst, 0, 2, 3 );
	UpdateColSum( pDM_Dst, 4, 10, 11 );
	UpdateColSum( pDM_Dst, 12, 15, 16 );
	// �Ѱ� ���ϱ� 
	// ���� �ο� 3, 11, 16 -> 17
	UpdateColSum( pDM_Dst, 3, 3, 17 );
	UpdateColSum( pDM_Dst, 11, 11, 17 );
	UpdateColSum( pDM_Dst, 16, 16, 17 );
	// ���� ���ϱ�
	CString sTemp;
	CString sSpeices;
	CString sBook;
	ids = pDM_Dst->GetCellData( nResultRow - 1, nResultCol - 2 , sTemp );
	if ( ids < 0 )
	{
		sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_Class - 6 ERROR!!") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	GetDivValue( sTemp, &sSpeices, &sBook );

	UpdatePercentage(pDM_Dst, nResultCol - 2, nResultCol - 1, _ttoi(sBook), 1 ); 

	// �μ� ���� ���� -- ĭ�� ����
	for (i = 0;i < nResultRow;i++ )
	{
		for ( INT j = 0;j < nResultCol;j++ )
		{
			ids = pDM_Dst->GetCellData( i, j, sTemp);	
			if ( ids < 0 )
			{
				sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_Class - 7 ERROR!!") );
				AfxMessageBox( sErrMsg );
				return -1;
			}

			sTemp.Replace(_T(" / "), _T("\n("));

			if ( j != nResultCol - 1 )
				sTemp += _T(")");

			ids = pDM_Dst->SetCellData( i, j, sTemp);	
			if ( ids < 0 )
			{
				sErrMsg.Format( _T("CBO_STATICS_ND::MakeStatics_Class - 8 ERROR!!") );
				AfxMessageBox( sErrMsg );
				return -1;
			}
		}
	}

	return 0;
}

/*--------------------------------------------------------------------------------------------
   Work Day : 2005-12-26
   Author   : Park, Daewoo(Jr)
   Type     : ADD   

   [   �Լ�����   ] 
      ��� DM ������ �����Ѵ� 
 
   [ RETURN VALUE ]
      0    : ���� 
      ���� : ����

   [   �̴Ͻ���   ]
      1. ���� ���ϸ��� �����Ѵ�.
      2. ����� ������ ���� ���� DM�� �޸��Ѵ�. 
--------------------------------------------------------------------------------------------*/
INT CBO_STATICS_ND::SaveExcelFile()
{
	INT ids = 0;

	//-----------------------------------------------------------------------------
	// 1. ���� ���ϸ��� �����Ѵ�.
	//-----------------------------------------------------------------------------
	TCHAR TmpDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,TmpDir);

	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFile = lpstrFile;
	OFN.hwndOwner = this->GetSafeHwnd();
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_OVERWRITEPROMPT;
	OFN.lpstrFilter = _T("XLS����(*.xls)\0*.xls\0�������(*.*)\0*.*\0");
	OFN.lpstrTitle = _T("EXCEL ���� ����");
	
	if (GetSaveFileName(&OFN) == 0)
	{
		SetCurrentDirectory(TmpDir);
		AfxMessageBox( _T("Excel ����� ��ҵǾ����ϴ�.") );
        return 0;
	}
	
	C_ExcelManControl m_ExcelCtrl;
	
	CRect rect;
	this->GetClientRect(&rect);

	// Excel Manager OCX��ü��		 �����Ѵ�.
	m_ExcelCtrl.Create(_T("Excel Control"), BS_DEFPUSHBUTTON, rect, this, 8805);
	if (m_ExcelCtrl.m_hWnd == NULL) {
		SetCurrentDirectory(TmpDir);
		AfxMessageBox(_T("Excelmanager.ocx�� ��ġ�Ǿ� ���� �ʽ��ϴ�. ��ġ�Ͽ� �ֽʽÿ�."), MB_ICONEXCLAMATION);
		return 0;
	}

    CExcelManager m_ExcelManager(&m_ExcelCtrl);
    if(m_ExcelManager.OpenManager() == FALSE) {
		SetCurrentDirectory(TmpDir);
        AfxMessageBox(_T("NOT Installed Excelmanager.ocx or No Excel!!!"), MB_ICONEXCLAMATION);
		return 0;
	}

	//-----------------------------------------------------------------------------
	// 2. ����� ������ ���� ���� DM�� �޸��Ѵ�. 
	//-----------------------------------------------------------------------------	
	CString sDMAlias;
	GetDMAlias( sDMAlias );

	CESL_DataMgr* pDM = FindDM( sDMAlias );
	
	INT nRowCnt = pDM->GetRowCount();
	INT nColCnt = pDM->GetColCount();
	
	INT nStartRow = 0;
	INT nStartCol = 0;

	TCHAR TmpDir2[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,TmpDir2);
	SetCurrentDirectory(TmpDir);
	MakeExcelShape( &m_ExcelManager, nStartRow, nStartCol );
	SetCurrentDirectory(TmpDir2);

	CString strData;
	for (INT i = 0; i < nRowCnt; i++) 
	{
		for (INT j = 0; j < nColCnt; j++) 
		{
			pDM->GetCellData( i , j , strData );
			strData = _T("'") + strData;
			m_ExcelManager.SetTextMatrix(i+nStartRow, j+nStartCol, strData);			
		}		
	}
	
	// �н��� �����̸��� �����Ͽ� �����Ѵ�.
    //m_ExcelManager.SaveAs(sPath + sTitle + _T(".xls"));
	CString strPath = OFN.lpstrFile;
	strPath.Replace(_T(".xls"), _T(""));
	m_ExcelManager.SaveAs(strPath + _T(".xls"));

	// ����� ���� ��ü�� ��ȯ�Ѵ�.
    m_ExcelManager.CloseManager();

	AfxMessageBox(_T("EXCEL�� ����Ǿ����ϴ�."), MB_ICONEXCLAMATION);
	SetCurrentDirectory(TmpDir);

	return 0;
}

/*--------------------------------------------------------------------------------------------
   Work Day : 2005-12-26
   Author   : Park, Daewoo(Jr)
   Type     : ADD   

   [   �Լ�����   ] 
      ���� ����� DM �˸��ƽ��� �����Ѵ�.
	
   [  PARAMETERS  ]
      sGetDMAlias : DM �˸��ƽ�

   [ RETURN VALUE ]
      0    : ���� 
      ���� : ����   
--------------------------------------------------------------------------------------------*/
INT CBO_STATICS_ND::GetDMAlias(CString &sGetDMAlias)
{
	if ( m_sStaticsName == _T("���Ա��к����(�����)") || m_sStaticsName == _T("�������к����"))
	{
		sGetDMAlias = _T("DM_���԰������к����_�����");
	}
	else if ( m_sStaticsName == _T("�������") || m_sStaticsName == _T("�����߰��ڷ����") || 
			  m_sStaticsName == _T("�񵵼��ڷ����") )
	{
		sGetDMAlias = _T("DM_�����񵵼��߰��ڷ����_�����");
	}
	else if ( m_sStaticsName == _T("�ڷ�������") )
	{
		sGetDMAlias = _T("DM_�ڷ�������_�����");
	}
	else if ( m_sStaticsName == _T("�弭������Ȳ") )
	{
		sGetDMAlias = _T("DM_�弭������Ȳ_�����");
	}

	return 0;
}

/*--------------------------------------------------------------------------------------------
   Work Day : 2005-12-26
   Author   : Park, Daewoo(Jr)
   Type     : ADD   

   [   �Լ�����   ] 
      �������¸� �����.
	
   [  PARAMETERS  ]
      pExcelMgr : [I N] �����޴���
	  nStartRow : [OUT] �����Ͱ� �Է½��۵� �ο��ε��� 
	  nStartCol : [OUT] �����Ͱ� �Է½��۵� �÷��ε��� 

   [ RETURN VALUE ]
      0    : ���� 
      ���� : ����   
--------------------------------------------------------------------------------------------*/
INT CBO_STATICS_ND::MakeExcelShape(CExcelManager *pExcelMgr, INT &nStartRow, INT &nStartCol)
{
	// 1. DM�� ã�Ƽ� ColCount�� ���Ѵ�.
	CString sDMAlias;
	GetDMAlias( sDMAlias );
	CESL_DataMgr* pDM = FindDM( sDMAlias );
	if ( !pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CBO_STATICS_ND::MakeExcelShape()") );
	
	INT nColCnt = pDM->GetColCount();
	
	INT nCol = 0;
	INT nRow = 0;
	CString sTmp = _T("");
	CString sCurrentDay = _T("");
	CTime time;	
	time = time.GetCurrentTime();
	sCurrentDay = _T("'") + time.Format(_T("%Y/%m/%d"));

/*	// 2. ù�ٿ� ������ �Է��Ѵ�.
	pExcelMgr->SetTextMatrix( ++nRow , 1 , m_sStaticsName);
	
	// 3. ��°�ٿ� ������������ �Է��Ѵ�.
	CTime time;	
	time = time.GetCurrentTime();
	sTmp = _T("'") + time.Format(_T("%Y/%m/%d"));
	pExcelMgr->SetTextMatrix( ++nRow , 1 , sTmp);
*/
	// 4. ����� �������� �÷��� Ʋ����
	//    ������ ���Ϸ� �����Ѵ�. ���ϸ��� �����ϰ� �����ϴ� 
	CString sFileName = _T("..\\Cfg\\���\\Excel����\\") + m_sStaticsName + _T(".cfg");
	CStdioFile FILE;
	if ( !FILE.Open( sFileName, CFile::modeRead | CFile::typeBinary ) )
	{
		CString sErrorMsg;
		sErrorMsg.Format( _T("FILE OPEN ERROR :: %s") , sFileName );
		AfxMessageBox( sErrorMsg );
		return -1;
	}
	
	TCHAR cUni;
	FILE.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		FILE.SeekToBegin();
	}

	INT nPos = 0;
	INT nMAXCol = 0;
	CString sBuf;

	while ( FILE.ReadString( sTmp ) )
	{
		// �ּ�ó��
		if ( sTmp.Mid(0, 1) == _T("#") )	continue;
		// ������ �Է� ���� ��ġ
		else if ( sTmp.Mid(0, 1) == _T("$") )
		{
			sTmp.Replace( _T("$"), _T("") );
			if ( sTmp.Replace( _T("StartRowIndex="), _T("") ) > 0 )
				nStartRow = _ttoi( sTmp );
			else if ( sTmp.Replace( _T("StartColIndex="), _T("") ) > 0 )
				nStartCol = _ttoi( sTmp );
		}
		// �߰� ����
		else if ( sTmp.Mid(0, 1) == _T("%") )
		{
			sTmp.Replace( _T("%"), _T("") );
			// ',' ������ �и� �Ǿ� �ִ�
			nCol = 0;
			nRow++;
			while ( TRUE ) 
			{
				nPos = sTmp.Find( _T(",") );
				
				if ( nPos < 0 )
				{
					sBuf = sTmp;
					sBuf.Replace( _T(",") , _T("") );
					sBuf.TrimLeft();sBuf.TrimRight();
					
					if ( sBuf.Compare( _T("����") ) == 0 )
						sBuf = m_sStaticsName;
					else if ( sBuf.Compare( _T("�����") ) == 0 )
                        sBuf = sCurrentDay;

					pExcelMgr->SetTextMatrix( nRow , ++nCol , sBuf);
					break;
				}
				else
				{
					sBuf = sTmp.Mid(0, nPos + 1);
					sBuf.Replace( _T(",") , _T("") );
					sBuf.TrimLeft();sBuf.TrimRight();
					
					if ( sBuf.Compare( _T("����") ) == 0 )
						sBuf = m_sStaticsName;
					else if ( sBuf.Compare( _T("�����") ) == 0 )
                        sBuf = sCurrentDay;
											
					pExcelMgr->SetTextMatrix( nRow , ++nCol , sBuf);

					sTmp = sTmp.Mid(nPos + 1);
				}
			}
			
		}
		// ����������
		else if ( sTmp.Mid(0, 1) == _T("@") )
		{
			sTmp.Replace( _T("@"), _T("") );
			// ������
			// ',' ������ ���еǾ� ������
			INT nSX = 0;
			INT nSY = 0;
			INT nEX = 0;
			INT nEY = 0;
	
			while ( TRUE ) 
			{
				nPos = sTmp.Find( _T(",") );
				
				if ( nPos < 0 )
				{
					sTmp.Replace( _T("EY"), _T("") );	// EndY
					nEY = _ttoi( sTmp );
					break;
				}
				else
				{
					sBuf = sTmp.Mid(0, nPos + 1);
					sBuf.Replace( _T(",") , _T("") );
					sBuf.TrimLeft();sBuf.TrimRight();

					if ( sBuf.Replace( _T("SX"), _T("") ) > 0 )	// StartX
						nSX = _ttoi( sBuf );					
					else if ( sBuf.Replace( _T("SY"), _T("") ) > 0 )	// StartY
						nSY = _ttoi( sBuf );					
					else if ( sBuf.Replace( _T("EX"), _T("") ) > 0 )	// EndX
						nEX = _ttoi( sBuf );					

					sTmp = sTmp.Mid(nPos + 1);
				}
			}

			pExcelMgr->Merge( CRect( nSX, nSY, nEX, nEY ) );
		}		
		// �÷���
		else
		{
			// ',' ������ �и� �Ǿ� �ִ� �÷����� �Է��Ѵ�

			nCol = 0;
			nRow++;
			while ( TRUE )
			{
				nPos = sTmp.Find( _T(",") );
				if ( nPos < 0 )
				{
					pExcelMgr->SetTextMatrix( nRow , ++nCol , sTmp);
					// �÷��� ���õ� �κ��� ���� ĥ����
					pExcelMgr->Cell_Color(nCol, nRow, nCol, nRow, 15);
					pExcelMgr->Font(nCol, nRow, nCol, nRow, TRUE, FALSE, 0, 9);
					if ( nCol > nMAXCol )	nMAXCol = nCol;
					break;
				}
				else
				{
					sBuf = sTmp.Mid(0, nPos + 1);
					sBuf.Replace( _T(",") , _T("") );
					sBuf.TrimLeft();sBuf.TrimRight();
					pExcelMgr->SetTextMatrix( nRow , ++nCol , sBuf);
					// �÷��� ���õ� �κ��� ���� ĥ����
					pExcelMgr->Cell_Color(nCol, nRow, nCol, nRow, 15);
					pExcelMgr->Font(nCol, nRow, nCol, nRow, TRUE, FALSE, 0, 9);
					sTmp = sTmp.Mid(nPos + 1);
					if ( nCol > nMAXCol )	nMAXCol = nCol;
				}			
			}
		}
	}
	
	FILE.Close();

	// ���� �׸��� 	
	pExcelMgr->DrawLine(1,3,nMAXCol,nRow, 1, 7, 2, 0);//����
	pExcelMgr->DrawLine(1,3,nMAXCol,nRow, 2, 7, 2, 0);//������
	pExcelMgr->DrawLine(1,3,nMAXCol,nRow, 3, 7, 2, 0);//����
	pExcelMgr->DrawLine(1,3,nMAXCol,nRow, 4, 7, 2, 0);//�Ʒ���		
	
	pExcelMgr->Vertical_Alignment(1,3,nMAXCol,nRow, ALIGN_VCENTER);
	pExcelMgr->Horizontal_Alignment(1,3,nMAXCol,nRow, ALIGN_HCENTER);

/*	// 5. ���� �ο�� �÷� �ε����� �����Ѵ�. 
	//    ����� �������� Ʋ����. 
	//    �ҽ��� FIX, ������ �ɰ����� �𸣰ڴ� �Ѥ�	
	if ( m_sStaticsName == _T("�弭������Ȳ") )
	{
		nStartRow = 4;nStartCol = 3;
	}
	else if ( m_sStaticsName == _T("�񵵼��ڷ����") || m_sStaticsName == _T("�������") || m_sStaticsName == _T("�����߰��ڷ����"))
	{
		nStartRow = 5;nStartCol = 2;
	}
	else if ( m_sStaticsName == _T("�ڷ�������") || m_sStaticsName == _T("�������к����") || m_sStaticsName == _T("���Ա��к����(�����)"))
	{
		nStartRow = 4;nStartCol = 4;
	}
*/
	return 0;
}

HBRUSH CBO_STATICS_ND::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

