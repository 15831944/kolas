// SE_CAT_SEARCH_MGR1.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_SEARCH_MGR_GROUP.h"
#include "CommonDefine.h"
#include "..\\..\\����\\�弭����\\LOC_CommonAPI\LocCommonAPI.h"

//==============================================
//2008.07.11 ADD BY PJW : �����ϴܿ� ���¹�LOG���
// #include "SBL.H"
//==============================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_SEARCH_MGR_GROUP_GROUP dialog
#include "..\..\..\����\K2UP_Common\MakeSearchData.h"

CBO_CAT_SEARCH_MGR_GROUP::CBO_CAT_SEARCH_MGR_GROUP(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_SEARCH_MGR_GROUP)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	pMakeData = new CMakeSearchData(this);
}


CBO_CAT_SEARCH_MGR_GROUP::~CBO_CAT_SEARCH_MGR_GROUP()
{
	if(pMakeData)
	{
		delete pMakeData;
		pMakeData = NULL;
	}
}

VOID CBO_CAT_SEARCH_MGR_GROUP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_SEARCH_MGR_GROUP)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_SEARCH_MGR_GROUP, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_SEARCH_MGR_GROUP)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bDetailNo, OnbDetailNo)
	ON_BN_CLICKED(IDC_bREG0100CLEAR, OnbREG0100CLEAR)
	ON_EN_SETFOCUS(IDC_REG_AUTHOR, OnSetfocusRegAuthor)
	ON_EN_SETFOCUS(IDC_REG_NO1, OnSetfocusRegNo1)
	ON_EN_SETFOCUS(IDC_REG_NO2, OnSetfocusRegNo2)
	ON_EN_SETFOCUS(IDC_REG_TITLE, OnSetfocusRegTitle)
	ON_WM_CTLCOLOR() 
	ON_EN_KILLFOCUS(IDC_preGROUP_NO1, OnKillfocuspreGROUPNO1)
	ON_EN_KILLFOCUS(IDC_preGROUP_NO2, OnKillfocuspreGROUPNO2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_SEARCH_MGR_GROUP message handlers

BOOL CBO_CAT_SEARCH_MGR_GROUP::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_����_����_���_�˻�_�Ŵ���_����")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}


	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_����_����_����"));

	CString m_Year;
	// 040304_HSS ���� LocalTime���� Select +++++++++++++++
	//CTime mTime = pDM->GetDBTime();
	CTime mTime = CTime::GetCurrentTime();
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++
	m_Year.Format(_T("%d"), mTime.GetYear());

	SetControlData(_T("CM_����_���_�˻�_����"), _T("BS_���Գ⵵"), m_Year);
	
	// KOL.RED.2018.220 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//BB_��Ϲ�ȣ_����|E|1124|||||||
	//BB_��Ϲ�ȣ_����|E|1325|||||||
	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_����_���_�˻�_����"), _T("BB_��Ϲ�ȣ_����"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_����_���_�˻�_����"), _T("BB_��Ϲ�ȣ_����"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}
	// KOL.RED.2018.220 ---------------------------------------------------------------------------	

	//���� ���� 
	OnbDetailNo(); 

	EnableThemeDialogTexture(GetSafeHwnd()); 

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CBO_CAT_SEARCH_MGR_GROUP::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);
}


VOID CBO_CAT_SEARCH_MGR_GROUP::OnbSEARCH() 
{

	// TODO: Add your control notification handler code here
	//==============================================
	//2008.07.11 ADD BY PJW : �����ϴܿ� ���¹�LOG���
//  	BEGIN_SBL();
	//==============================================
	CString strQry,strWorkNo1,strWorkNo2,strpreGroup1,strpreGroup2,strYear,sACQUISIT_TYPE;
	INT ids;

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_����_����_����_����"));

	// ���� ��ȣ �� ������ ��ȣ�� ���ο� ���� �ش�.
	GetControlData(_T("CM_����_���_�˻�_����"), _T("���ι�ȣ����"), strpreGroup1);
	GetControlData(_T("CM_����_���_�˻�_����"), _T("���ι�ȣ��"), strpreGroup2);		 

	GetControlData(_T("CM_����_���_�˻�_����"), _T("BS_���Գ⵵"), strYear);		 
	GetControlData(_T("CM_����_���_�˻�_����"), _T("BB_���Ա���_�ڵ�")	  , sACQUISIT_TYPE , -1 ,1);


	if( (strpreGroup1 != _T(""))&&(strpreGroup2 != _T("")) )
	{
		strpreGroup1 = strpreGroup1 + _T(" - ") +strpreGroup2;
	}

	SetControlData(_T("CM_����_����_����_����"), _T("���ι�ȣ"), strpreGroup1);
	SetControlData(_T("CM_����_����_����_����"), _T("�⵵"), strYear);
	SetControlData(_T("CM_����_����_����_����"), _T("�Լ�����"), sACQUISIT_TYPE);


	strQry = MakeSelectQry();
	if( strQry == _T("")) return;
	
	AfxGetApp()->DoWaitCursor( 1 );

	ids = pDM->RefreshDataManager(strQry);
	if (ids < 0 )
	{
		AfxMessageBox(_T("RefreshDataManager �� Ȯ���� �ּ���"));
		return;
	}

	// �������� 2004.09.15 ++++++++
	AfxGetApp()->DoWaitCursor( -1 );

	INT nCount = pDM->GetRowCount();
	if ( nCount <= 0 ) 
	{
		AfxMessageBox(_T("�˻� ����� �����ϴ�."));
	}
	// -----------------------------


	CESL_ControlMgr *pCM;
	CESL_Grid *pGrid_MainGrid_All;

	pCM = FindCM(_T("CM_����_����_����_����"));
	pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_����_����_����_����"), _T("MAIN_GRID"));
	pGrid_MainGrid_All->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
    


	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2003.11.21 ������ �߰� 
	// �˻���� ���¹ٿ� ��Ÿ���� 
    CString strMsg ;
	INT nRowCount = 0 ;
	INT nBookCount = 0;
	nBookCount = pDM->GetRowCount () ;
	CMap <CString ,LPCTSTR,CString,LPCTSTR> mapRecKey;
	CString strSpecies_key;
	for(INT i =0 ; i<nBookCount;i++)
	{
        strSpecies_key=_T("") ;
		pDM->GetCellData (_T("IBS_��KEY"),i,strSpecies_key);
		if(!strSpecies_key.IsEmpty ())
        { 
			mapRecKey.SetAt(strSpecies_key,strSpecies_key);
		}

		//===================================================
		//2009.05.20 ADD BY PJW : ������ ��ȯ�Ѵ�.
		CString sPrice;
		INT nPriceSum;
		CString sStreamMarc;
		CMarc marc;
		pDM->GetCellData( _T("BS_MARC"), i, sStreamMarc );
		m_pInfo->pMarcMgr->Decoding( sStreamMarc, &marc );

		nPriceSum += _ttoi(sPrice);
		CString sItem, sPriceCode;
		m_pInfo->pMarcMgr->GetItem( &marc, _T("950$b"), sItem );
		for ( INT j = 0 ; j < sItem.GetLength() ; j++ ) {
			CString sOneWord = sItem.Mid( j, 1 );
			if ( sOneWord >= _T("0") && sOneWord <= _T("9") ) {
				break;
			}
			sPriceCode = sPriceCode + sOneWord;
		}

		pDM->GetCellData( _T("BB_����"), i, sPrice );

		CString sPriceCharacter;
		pDM->GetCellData( _T("BB_����Ư��"), i, sPriceCharacter );
		
		if ( sPrice == _T("0") || sPrice == _T(""))
		{
			if ( sPriceCharacter.IsEmpty() )
			{
				pDM->SetCellData( _T("UDF_��ȯ����"), _T("0"), i );
				pDM->SetCellData( _T("BB_����")		, _T("0"), i );

			}
			else 
			{
				pDM->SetCellData( _T("UDF_��ȯ����"), sPriceCharacter, i );
			}

		}
		else 
		{			
			CString strTempPrice;
			strTempPrice = sPrice;
			INT nPoint;
			if ( sPriceCode == _T("\\") || sPriceCode == _T(""))
			{						
				nPoint= strTempPrice.Find(_T("."), 0 );
				if ( nPoint > 0 ) sPrice.Format(_T("%s"), strTempPrice.Left(nPoint));
				else sPrice.Format(_T("%s"), strTempPrice );
			}
			else
			{
				sPrice.Format(_T("%s"),  strTempPrice);
			}

			ConversionPriceInfoToDesc( sPrice, sPrice );

			pDM->SetCellData( _T("UDF_��ȯ����"), sPrice, i );
		}			
		pDM->SetCellData( _T("UDF_ȭ�󱸺�"), sPriceCode, i );
		//===================================================
	}

	nRowCount = mapRecKey.GetCount();
    
    strMsg.Format(_T("���� : %5d\tå�� : %5d"), nRowCount, nBookCount);
    GetParentFrame()->SendMessage(WM_USER + 2004, NULL, (LPARAM)strMsg.GetBuffer(0));

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//�׸��� �ݹ� �Լ� ���� 
   
	pCM->AllControlDisplay();
	//==============================================
	//2008.07.11 ADD BY PJW : �����ϴܿ� ���¹�LOG���
//  	END_SBL(nBookCount);
	//==============================================

	if( nRowCount > 0 )
		ShowWindow(SW_HIDE);

}

INT CBO_CAT_SEARCH_MGR_GROUP::GridDisplayEventProc(CString &strGridAlias, INT nRow, 
INT nCol, CString &strDMFieldAlias, CString &str)
{
EFS_BEGIN

    if (strGridAlias == "MAIN_GRID") {
        if (strDMFieldAlias == "BB_�ڷ����_�ڵ�") 
		{
			CLocCommonAPI LocApi(this);
			LocApi.GetWorkingStatusDescToCode(str, 1);
			/*
            if ( str == "BOR111N") {
				str = "����ΰ��ڷ�";
			}
			else if ( str == _T("BOR112N") ) {
				str = _T("����ڷ�");
			}
			else if ( str == _T("BOR113O") ) {
				str = _T("�������ڷ�");
			}
			else if ( str == _T("BOC111O") ) {
				str = _T("����������");
			}
			else if ( str == _T("BOC112O") ) {
				str = _T("�з�����ڷ�");
			}
			else if ( str == _T("BOC113N") ) {
				str = _T("��������ڷ�");
			}
			else if ( str == _T("BOC114N") ) {
				str = _T("�����ΰ���");
			}
			else if ( str == _T("BOL111O") ) {
				str = _T("�����ΰ��ڷ�");
			}
			else if ( str >= _T("BOL112N") && str <= _T("BOL215O") ) {
				str = _T("�谡�ڷ�");
			}   
			*/        
        }
    }
	return 0;


EFS_END
return -1;

}

CString CBO_CAT_SEARCH_MGR_GROUP::MakeSelectQry()
{

	CString strQry,strTmp; 
	CString sACQUISIT_CODE, sACQUISIT_YEAR;
	CString sAccessionNoStart, sAccessionNoEnd,sAccessionNoYear;
	CString sRegNoStart, sRegNoEnd;
	CString sTransferDataStart, sTransferDataEnd;
	CString SEQ_NO1,SEQ_NO2,preGroup1,preGroup2;
	INT sAccessionFlag=0;

	//�ڷ����
	// 17/07/20 ������ : å���������ڷ� ���� �߰� 
/*//BEFORE-----------------------------------------------------------------------------
	//========================================================================
	//2008.12.01 UPDATE BY PJW : ���� ��ȣ���� �ڷ���� �߰�
// 	strQry = strQry + _T(" B.WORKING_STATUS IN (")
// 					  _T("'BOR111N', 'BOR112N', 'BOR111O', 'BOR112O', 'BOR113O', ")
// 					  _T("'BOC113N', 'BOC114O', 'BOC2110', ")
// 					  _T("'BOL111O', 'BOL112N', 'BOL113O', 'BOL114O', 'BOL115O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O','BOC112O', 'BOC111O'")
// 					  _T(") ");
//	strQry = strQry + _T(" B.WORKING_STATUS IN (")
//					  _T("'BOR111N', 'BOR112N', 'BOR111O', 'BOR112O', 'BOR113O', ")
//					  _T("'BOC113N', 'BOC114O', 'BOC2110', ")
//					  _T("'BOL111O', 'BOL112N', 'BOL113O', 'BOL114O', 'BOL115O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOC112O', 'BOC111O'")
//					  _T(") ");
	//========================================================
	// 2013.12.01 UPDATE BY BJS : 2013���������ռ���Ȯ�뱸��װ��� (���յ���Ÿ������ ���� �߰�)
	strQry = strQry + _T(" B.WORKING_STATUS IN (")
				  _T("'BOR111N', 'BOR112N', 'BOR111O', 'BOR112O', 'BOR113O', ")
				  _T("'BOC113N', 'BOC114O', 'BOC2110', ")
				  _T("'BOL111O', 'BOL112N', 'BOL113O', 'BOL114O', 'BOL115O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL511O','BOL611O','BOC112O', 'BOC111O' , 'BOL115O', 'BOL116O', 'BOL218O', 'BOL252N'")
				  _T(") ");
	
	//2009.03.23 UPDATE BY LKS : �������ڷ�,���� �ڷ�,��ȣ���������ڷ� �����߰�+�н��ڷ��߰�(2009.10.12)
//	strQry = strQry + _T(" B.WORKING_STATUS IN (")
//				  _T("'BOR111N', 'BOR112N', 'BOR111O', 'BOR112O', 'BOR113O', ")
//				  _T("'BOC113N', 'BOC114O', 'BOC2110', ")
//				  _T("'BOL111O', 'BOL112N', 'BOL113O', 'BOL114O', 'BOL115O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL511O','BOC112O', 'BOC111O' , 'BOL115O', 'BOL116O', 'BOL218O', 'BOL252N'")
//				  _T(") ");
	//========================================================
	//=========================================================================
*///AFTER------------------------------------------------------------------------------
	// 18.09.27 JMJ KOL.RED.2018.008
	strQry = strQry + _T(" B.WORKING_STATUS IN (")
				  _T("'BOR111N', 'BOR112N', 'BOR111O', 'BOR112O', 'BOR113O', ")
				  _T("'BOC111O', 'BOC112O', 'BOC113N', 'BOC114O', 'BOC211O', ")
				  _T("'BOL111O', 'BOL112N', 'BOL113O', 'BOL114O', 'BOL115O', 'BOL116O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL218O', 'BOL219O', 'BOL252N', 'BOL411O', 'BOL511O','BOL611O'")
				  _T(") ");
/*//END-------------------------------------------------------------------------------*/
	
	//strQry += _T(" AND B.BOOK_APPENDIX_FLAG = 'B' ");

	strQry = strQry + _T(" AND REG_NO IS NOT NULL ") ;

	//���� �⵵ 
	GetControlData(_T("CM_����_���_�˻�_����"), _T("BS_���Գ⵵")	  , sACQUISIT_YEAR);
	sACQUISIT_YEAR.TrimLeft();
	sACQUISIT_YEAR.TrimRight();
	
	//���ι�ȣ 
	GetControlData(_T("CM_����_���_�˻�_����"), _T("���ι�ȣ����")   , sAccessionNoStart); 
	GetControlData(_T("CM_����_���_�˻�_����"), _T("���ι�ȣ��")   , sAccessionNoEnd); 

	if ( (sAccessionNoStart != _T(""))&&(sAccessionNoEnd != _T("")) )
	{
		strTmp = strTmp + _T(" AND a.accession_rec_no >= ") + sAccessionNoStart + _T(" AND a.accession_rec_no <= ") + sAccessionNoEnd ;
		if ( sACQUISIT_YEAR != _T("") ) {
			strTmp = strTmp + _T(" AND A.ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("'");
		}
		
	}
	else if ( (sAccessionNoStart != _T(""))&&(sAccessionNoEnd == _T("")) )
	{
		strTmp = strTmp + _T(" AND a.accession_rec_no = ") + sAccessionNoStart;
		
		if ( sACQUISIT_YEAR != _T("") ) {
			strTmp = strTmp + _T(" AND A. ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("'");
		}
		
	}
	else if ( (sAccessionNoStart == _T(""))&&(sAccessionNoEnd != _T("")) )
	{
		strTmp = strTmp + _T(" AND a.accession_rec_no = ") + sAccessionNoEnd;
		
		if ( sACQUISIT_YEAR != _T("") ) {
			strTmp = strTmp + _T(" AND A. ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("'");
		}
		
	}


	//���� ���� �ڵ� & ���Գ⵵ 
	GetControlData(_T("CM_����_���_�˻�_����"), _T("BB_���Ա���_�ڵ�"), sACQUISIT_CODE);
	
	sACQUISIT_CODE.TrimLeft();
	sACQUISIT_CODE.TrimRight();


	if ( sACQUISIT_CODE != _T("") ) {
		strQry = strQry + _T(" AND B.ACQ_CODE = '") + sACQUISIT_CODE + _T("'");
	}

	

	//��Ϲ�ȣ
	GetControlData(_T("CM_����_���_�˻�_����"), _T("BB_��Ϲ�ȣ_����"), sRegNoStart);
	GetControlData(_T("CM_����_���_�˻�_����"), _T("BB_��Ϲ�ȣ_����"), sRegNoEnd);
	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) {
// 		if ( sRegNoStart.GetLength() > 2 ) {
// 			CString sRight;
// 			sRight.Format( _T("%010s") , sRegNoStart.Mid(2) );
// 			sRegNoStart = sRegNoStart.Left(2) + sRight;
// 		}
// 	}
// 	if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) {
// 		if ( sRegNoEnd.GetLength() > 2 ) {
// 			CString sRight;
// 			sRight.Format( _T("%010s") , sRegNoEnd.Mid(2) );
// 			sRegNoEnd = sRegNoEnd.Left(2) + sRight;
// 		}
// 	}

	if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) {
		if ( sRegNoStart.GetLength() > 2 ) {
			sRegNoStart = m_pInfo->MakeRegNo(sRegNoStart.Left(nRegCodeLength), sRegNoStart.Mid(nRegCodeLength) );
		}
	}
	if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) {
		if ( sRegNoEnd.GetLength() > 2 ) {
			sRegNoEnd = m_pInfo->MakeRegNo(sRegNoEnd.Left(nRegCodeLength), sRegNoEnd.Mid(nRegCodeLength) );
		}
	}
	//=====================================================

	

	if ( sRegNoStart != _T("") && sRegNoEnd == _T("")) {
		strTmp = strTmp + _T(" AND B.REG_NO = '") + sRegNoStart  + _T("'");
	}
	if ( sRegNoEnd != _T("") && sRegNoStart == _T("") ) {
		AfxMessageBox( _T("���� ��Ϲ�ȣ�� �Է� �ϼ���."));
		return _T("");
	}
	if ( sRegNoStart != _T("") && sRegNoEnd != _T("") ) {
		strTmp = strTmp+_T(" AND B.REG_NO >= '") + sRegNoStart + _T("' AND B.REG_NO <= '") + sRegNoEnd + _T("'");
	}

	// ������� : ���� ����
	CString strRegDateStart;
	CString strRegDateEnd;

	GetControlData(_T("CM_����_���_�˻�_����"), _T("�������_����"), strRegDateStart);
	GetControlData(_T("CM_����_���_�˻�_����"), _T("�������_����"), strRegDateEnd);

	if ( strRegDateStart != _T("") && strRegDateEnd == _T("") ) {
		/****************************************************************
		 ****************************************************************/
		//���δ�������� �˻��� ����ΰ����ڰ� �ƴ� ������ڷ� �Ѵ�.
		//20080515 UPDATE BY PJW : strTmp = strTmp + " AND B.REG_TRANSFER_DATE = '" + strRegDateStart + "'";
		//strTmp = strTmp + _T(" AND B.REG_TRANSFER_DATE = '") + strRegDateStart + _T("'");
		strTmp = strTmp + _T(" AND B.REG_DATE = '") + strRegDateStart + _T("'");
		/****************************************************************
		 ****************************************************************/
		
	}

	if ( strRegDateStart == _T("") && strRegDateEnd != _T("") ) {
		AfxMessageBox ( _T("���� ���ó������ �Է��ϼ���.") );
		return _T("");
	}

	if ( strRegDateStart != _T("") && strRegDateEnd != _T("") ) {
		/****************************************************************
		 ****************************************************************/
		//20080515 UPDATE BY PJW : strTmp = strTmp + " AND B.REG_TRANSFER_DATE = '" + strRegDateStart + "'";
		//strTmp = strTmp + _T(" AND B.REG_TRANSFER_DATE >= '") + strRegDateStart + _T("' AND B.REG_TRANSFER_DATE <= '") + strRegDateEnd + _T("'");
		strTmp = strTmp + _T(" AND B.REG_DATE >= '") + strRegDateStart + _T("' AND B.REG_DATE <= '") + strRegDateEnd + _T("'");
		/****************************************************************
		 ****************************************************************/
		
	}
	
	/*
	GetControlData(_T("CM_����_���_�˻�_����"), _T("�������_����"), sTransferDataStart);
	GetControlData(_T("CM_����_���_�˻�_����"), _T("�������_����"), sTransferDataEnd);

	if ( sTransferDataStart != _T("") && sTransferDataEnd == _T("") ) {
		strTmp = strTmp + _T(" AND B.REG_TRANSFER_DATE = '") + sTransferDataStart + _T("'");
	}

	if ( sTransferDataStart == _T("") && sTransferDataEnd != _T("") ) {
		AfxMessageBox ( _T("���� ���ó������ �Է��ϼ���.") );
		return _T("");
	}

	if ( sTransferDataStart != _T("") && sTransferDataEnd != _T("") ) {
		strTmp = strTmp + _T(" AND B.REG_TRANSFER_DATE >= '") + sTransferDataStart + _T("' AND B.REG_TRANSFER_DATE <= '") + sTransferDataEnd + _T("'");
	}
	*/

	//�� �˻��� ��� 
	if ( sDetailFlag == 1)
	{
		CString sTitle, sAuthor;
		CString sFORM_CODE, sMEDIA_CODE, sMAT_CODE, sUSE_OBJ_CODE, sMANAGE_CODE;
		CString sWhereTitle;

		//CMakeSearchData *pMakeData = NULL;

		//��ǥ��  
		GetControlData(_T("CM_����_���_�˻�_����"), _T("��ǥ��"), sTitle);	
		sTitle.TrimLeft();
		sTitle.TrimRight();

		if ( sTitle != _T("") ) {
			INT iCh = pMakeData->ChangeHanjaToHangul( sTitle );	//���� �ѱ� ��ȯ
			if ( 0 != iCh ) {			
				AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
				return _T("");
			}						
			iCh = pMakeData->GetKeyWord( sTitle );				//����, Ư������ ����
			if ( 0 != iCh ) {			
				AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
				return _T("");
			}

			
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0") , sTitle );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_TITLE, '%s*')"), sTitle );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType�� �������� �ʾҽ��ϴ�.") );
				return _T("");
			}
			strQry = strQry + _T(" AND ") + sWhereTitle;
			
			/*
			//#ifdef _ORA_CAT_SEARCH_
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0") , sTitle );
			//#else
			//	sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_TITLE, '%s*')"), sTitle );
			//#endif
			strQry = strQry + _T(" AND ") + sWhereTitle;			
			*/
		}

		//������
		GetControlData(_T("CM_����_���_�˻�_����"), _T("������"), sAuthor);
		sTitle.TrimLeft();
		sTitle.TrimRight();

		if ( sAuthor != _T("") ) {
						
			if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_AUTHOR,'%s*',NULL)>0") , sAuthor );
			}
			else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_AUTHOR, '%s*')"), sAuthor );
			}
			else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
				AfxMessageBox( _T("SearchIndexType�� �������� �ʾҽ��ϴ�.") );
				return _T("");
			}
			strQry = strQry + _T(" AND ") + sWhereTitle;
			
			/*
			//#ifdef _ORA_CAT_SEARCH_
				sWhereTitle.Format( _T("CATSEARCH(I.IDX_AUTHOR,'%s*',NULL)>0") , sAuthor );
			//#else
			//	sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_AUTHOR, '%s*')"), sAuthor );
			//#endif
			strQry = strQry + _T(" AND ") + sWhereTitle;
			*/
		}

		//�ڵ� ���а�
		GetControlData(_T("CM_����_���_�˻�_����"), _T("BS_���ı���_�ڵ�"), sFORM_CODE);
		GetControlData(_T("CM_����_���_�˻�_����"), _T("BS_��ü����_�ڵ�"), sMEDIA_CODE);
		GetControlData(_T("CM_����_���_�˻�_����"), _T("BS_�ڷᱸ��_�ڵ�"), sMAT_CODE);
		GetControlData(_T("CM_����_���_�˻�_����"), _T("BS_�̿��󱸺�_�ڵ�"), sUSE_OBJ_CODE);
		GetControlData(_T("CM_����_���_�˻�_����"), _T("BS_��������_�ڵ�"), sMANAGE_CODE);

		//�ڵ尪
		sFORM_CODE.TrimLeft();
		if ( sFORM_CODE != _T("") ) {
			strQry = strQry + _T(" AND I.FORM_CODE = '") + sFORM_CODE + _T("'");		
		}
		sMEDIA_CODE.TrimLeft();
		if ( sMEDIA_CODE != _T("") ) {
			strQry = strQry + _T(" AND I.MEDIA_CODE = '") + sMEDIA_CODE + _T("'");
		}
		sMAT_CODE.TrimLeft();
		if ( sMAT_CODE != _T("") ) {
			strQry = strQry + _T(" AND I.MAT_CODE = '") + sMAT_CODE + _T("'");
		}
		sUSE_OBJ_CODE.TrimLeft();
		if ( sUSE_OBJ_CODE != _T("") ) {
			strQry = strQry + _T(" AND I.USE_OBJ_CODE = '") + sUSE_OBJ_CODE + _T("'");
		}
		sMANAGE_CODE.TrimLeft();
		if ( sMANAGE_CODE != _T("") ) {
			strQry = strQry + _T(" AND I.MANAGE_CODE = '") + sMANAGE_CODE + _T("'");
		}

		//�۾���(��������. ���� 2004.09.15)
		CString sWorker;
		GetControlData(_T("CM_����_���_�˻�_����"), _T("BS_�۾���"), sWorker);
		if ( sWorker != _T("") ) {
			strQry = strQry + _T(" AND B.SPECIES_KEY IN (SELECT REC_KEY FROM BO_SPECIES_TBL WHERE WORKER = '") + sWorker + _T("')");
			//strQry = strQry + _T(" AND B.WORKER = '") + sWorker + _T("'");
		}	

		
	}
	
	if( strTmp == _T("") )
	{
		if( AfxMessageBox(_T("�˻������� �����Ͽ� �˻� �ð��� ���� �ɸ� �� �ֽ��ϴ�. ")
				_T("\n�����ϰ� �˻��Ͻðڽ��ϱ�?")
				_T("\n(��: �˻�, �ƴϿ�: �˻�����)"), MB_YESNO) == IDNO )
			return _T("");
	}
	

	strQry += strTmp;

	return strQry;

}

VOID CBO_CAT_SEARCH_MGR_GROUP::OnbDetailNo() 
{
	// TODO: Add your control notification handler code here

	CString sDetailSee;
	GetDlgItem(IDC_bDetailNo)->GetWindowText(sDetailSee);

	if ( sDetailSee == _T("�����˻�") )
	{
		sDetailFlag = 0;
		CRect rect;
		this->GetWindowRect( rect );		
		rect.bottom -= 165;
		this->MoveWindow( rect );

		GetDlgItem(IDC_bDetailNo)->SetWindowText(_T("�󼼰˻�"));
		GetDlgItem(IDC_guide_line)->ShowWindow( false );		
	}
	else
	{
		sDetailFlag = 1;
		CRect rect;
		this->GetWindowRect( rect );		
		rect.bottom += 165;
		this->MoveWindow( rect );

		GetDlgItem(IDC_bDetailNo)->SetWindowText(_T("�����˻�"));
		GetDlgItem(IDC_guide_line)->ShowWindow( true );
	}

}

VOID CBO_CAT_SEARCH_MGR_GROUP::OnbREG0100CLEAR() 
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_����_���_�˻�_����"));
	if (pCM)
		pCM->AllControlClear();
}

VOID CBO_CAT_SEARCH_MGR_GROUP::SetIME(INT nIDC, INT nMode)
{
    HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(nIDC)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		if ( nMode == 0 ) dwConversion=IME_CMODE_ALPHANUMERIC;
		else dwConversion=IME_CMODE_HANGUL;		

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(nIDC)->m_hWnd,hIME); 
    } 
}

VOID CBO_CAT_SEARCH_MGR_GROUP::OnSetfocusRegAuthor() 
{
	SetIME(IDC_REG_AUTHOR, 1);	
}

VOID CBO_CAT_SEARCH_MGR_GROUP::OnSetfocusRegNo1() 
{
	SetIME(IDC_REG_NO1, 0);	
}

VOID CBO_CAT_SEARCH_MGR_GROUP::OnSetfocusRegNo2() 
{
	SetIME(IDC_REG_NO2, 0);	
}

VOID CBO_CAT_SEARCH_MGR_GROUP::OnSetfocusRegTitle() 
{
	SetIME(IDC_REG_TITLE, 1);	
}


HBRUSH CBO_CAT_SEARCH_MGR_GROUP::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

INT CBO_CAT_SEARCH_MGR_GROUP::ConversionPriceInfoToDesc(CString strSrcData, CString &strDesData)
{
	CString strTmpData, strData1, strData2;
	CStringArray arrTmpData;	arrTmpData.RemoveAll();
	INT nLength = 0;
	INT nPos = -1;
	// 2005-01-13 �Ҽ��� ���ϴ� , ǥ�ø� ���� �ʵ��� �ϱ� ���� �Ҽ��� ���ڸ��� �Ѱ��ش�.
	nPos = strSrcData.Find(_T("."));
	if( nPos < 0 ) // _T(".")�� ���� ���
	{
		strData1 = strSrcData;
		strData2.Empty();
	}
	else
	{
		strData1 = strSrcData.Left(nPos);
		strData2 = strSrcData.Mid(nPos);
	}

	strData1.TrimLeft();		strData1.TrimRight();
	while(!strData1.IsEmpty())
	{
		nLength = strData1.GetLength();
		if(nLength >= 0)
			strTmpData = strData1.Right(3);
		else
			strTmpData = strData1;
		arrTmpData.InsertAt(0, strTmpData);
		if(nLength-3 >= 0)
			strData1 = strData1.Mid(0, nLength-3);
		else 
			strData1.Empty();
	}

	strDesData.Empty();
	INT cnt = arrTmpData.GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		if(strDesData.IsEmpty()) strDesData = arrTmpData.GetAt(i);
		else strDesData += _T(",") + arrTmpData.GetAt(i);
	}
	strDesData += strData2;

	return 0;
}

BOOL CBO_CAT_SEARCH_MGR_GROUP::PreTranslateMessage(MSG* pMsg) 
{
	
	return CDialog::PreTranslateMessage(pMsg);
}
//===================================================
//2010.05.19 ADD BY PJW : ���ι�ȣ �Է½� ���ڸ� üũ�Ѵ�.
void CBO_CAT_SEARCH_MGR_GROUP::OnKillfocuspreGROUPNO1() 
{
	// TODO: Add your control notification handler code here
	INT nIds = 0;
	CString strData;
	CEdit* pedtGROUP_NO1 = (CEdit*)GetDlgItem(IDC_preGROUP_NO1);
	pedtGROUP_NO1->GetWindowText(strData);
	strData.TrimLeft(); strData.TrimRight();
	if( !strData.IsEmpty() )
	{
		nIds = CheckIsDigit(strData);
		if( 0 > nIds )
		{
			AfxMessageBox(_T("���ι�ȣ�� ���ڸ� �Է� �����մϴ�."));
			pedtGROUP_NO1->SetFocus();
		}
	}
}

void CBO_CAT_SEARCH_MGR_GROUP::OnKillfocuspreGROUPNO2() 
{
	// TODO: Add your control notification handler code here
	INT nIds = 0;
	CString strData;
	CEdit* pedtGROUP_NO2 = (CEdit*)GetDlgItem(IDC_preGROUP_NO2);
	pedtGROUP_NO2->GetWindowText(strData);
	strData.TrimLeft(); strData.TrimRight();
	if( !strData.IsEmpty() )
	{
		nIds = CheckIsDigit(strData);
		if( 0 > nIds )
		{
			AfxMessageBox(_T("���ι�ȣ�� ���ڸ� �Է� �����մϴ�."));
			pedtGROUP_NO2->SetFocus();
		}
	}	
}

INT CBO_CAT_SEARCH_MGR_GROUP::CheckIsDigit(CString strData) 
{
	TCHAR ch = 0;
	INT nLength = strData.GetLength();
	for (INT idx = 0; idx < nLength; idx++)
	{
		ch = strData.GetAt(idx);
		if (!_istdigit(ch))
		{
			return -1;
		}
	}
	return 0;
}
//===================================================
