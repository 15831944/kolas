// BO_CAT_REG_0800.cpp : implementation file
//

#include "stdafx.h"
//#include _T("bo_cat_reg.h")
#include "BO_CAT_REG_0800.h"
#include "..\..\..\ESL\SIReportMgr\SIReportManager.h"
#include "..\..\..\����\MarcAdjustApi\CatApi_BO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0800 dialog


CBO_CAT_REG_0800::CBO_CAT_REG_0800(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{

}

CBO_CAT_REG_0800::~CBO_CAT_REG_0800()
{
}     


BOOL CBO_CAT_REG_0800::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_REG_0800::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REG_0800)
	DDX_Check(pDX, IDC_chkAPPENDIX_ADD, m_chkAppendixAdd);
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_REG_0800, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REG_0800)
	ON_BN_CLICKED(IDC_rB4, OnrB4)
	ON_BN_CLICKED(IDC_rA4, OnrA4)
	ON_BN_CLICKED(IDC_rA4_RANDSCAPE, OnrA4RANDSCAPE)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0800 message handlers

BOOL CBO_CAT_REG_0800::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("BO_CAT_REG_0800")) < 0)	
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	
	//++ 2006.12.06 KBS Update - ���������� ����
	GetManageValue( _T("��Ÿ") , _T("����") , _T("����") , _T("����") , m_str64 );
	GetManageValue( _T("��Ÿ") , _T("����") , _T("�����") , _T("�����") , m_str90 );
	GetManageValue( _T("��Ÿ") , _T("����") , _T("5163") , _T("5163") , m_str5163 );
	GetManageValue( _T("����") , _T("����") , _T("�������_����ǥ�ü���") , _T("") , m_strVolValue );
	//-- 2006.12.06 KBS 
	
	CButton *pPart, *pA4;

	pPart = (CButton *)GetDlgItem(IDC_rSHELF_PRINT);
	pPart->SetCheck(1);
	if( _T("Y") == m_str90 )
	{
		pA4 = (CButton *)GetDlgItem(IDC_rB4);
		pA4->SetCheck(1);
	}
	else
	{
		pA4 = (CButton *)GetDlgItem(IDC_rA4);
		pA4->SetCheck(1);
	}
	
	//5163�� ����... �弭���� ���	
	if( _T("Y") != m_str5163 )
	{
		GetDlgItem(IDC_rJANGSEO)->ShowWindow(SW_HIDE);
	}
	

	//===================================================================
	//2007.06.29 ADD PJW : ��ϱ����ڵ� �޺��ڽ� �ʱⰪ ����
	//20080612 UPDATE BY PJW : ��ϱ��а��� ����Ʈ���� �������� ����
	//CComboBox* pCombBOX = NULL;
	//pCombBOX =(CComboBox*) GetDlgItem(IDC_eACCESSION_CODE);
	//pCombBOX->SetCurSel(25);
	CESL_ControlMultiComboBox *pCmb = (CESL_ControlMultiComboBox *)GetDlgItem(IDC_eACCESSION_CODE);
	CString strRegCode;
	m_pInfo->pCodeMgr->GetDefaultCode(_T("��ϱ���"), strRegCode);
	
	pCmb->SetWindowText(strRegCode);
	//===================================================================
	

	// KOL.RED.2018.220 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//��ϱ���|M|12049|DROP_LIST||!��ϱ���.�ڵ�,!��ϱ���.����|,|||
	//��Ϲ�ȣ����|E|1120|||||||
	//��Ϲ�ȣ����|E|1121|||||||
	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_CAT_REG_0800"), _T("��Ϲ�ȣ����"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);
		pCtrl->SetRegCodeCombo((CWnd*)FindControl(_T("CM_BO_CAT_REG_0800"), _T("��ϱ���")), 1);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_CAT_REG_0800"), _T("��Ϲ�ȣ����"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);
		pCtrl->SetRegCodeCombo((CWnd*)FindControl(_T("CM_BO_CAT_REG_0800"), _T("��ϱ���")), 1);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}
	// KOL.RED.2018.220 ---------------------------------------------------------------------------	

	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;



EFS_END
return FALSE;

}

VOID CBO_CAT_REG_0800::OnOK() 
{
EFS_BEGIN

	INT ids = -1;	
	CString strLibName;
	CCatApi_BO CatApi(this);

	/****************************************************
	 ****************************************************/
	//20080502 UPDATE BY PJW : �������� �ִ� ���������� �޾ƿ´�.
	CatApi.GetLibInfo(&CString(_T("LIB_NAME")), &strLibName, 1);
	/****************************************************
	 ****************************************************/
	strLibName += _T("�� �������ڷ�� �Ʒ��� ���� �����.");

	//5163�� �弭����.. ���� ����.. �������� �Ф�;;
	CButton* pJangSeo = (CButton *)GetDlgItem(IDC_rJANGSEO);
	if( pJangSeo->GetCheck() == 1 ) 
	{
		Print_JangSeo();
		return;
	}

	// ���� ��ȣ�� ���� ���
	CButton* pPart = (CButton*)GetDlgItem(IDC_rSHELF_PRINT);
	if( pPart->GetCheck() == 1 ) 
	{
		CSIReportManager SIReportManager(this);
		SIReportManager.m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
		SIReportManager.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		SIReportManager.DeleteTempPageFiles();
		
		//A4,B4 ����
		CButton * pB4;
		pB4 = (CButton *)GetDlgItem(IDC_rB4);
		if( pB4->GetCheck() == 1 )
		{
			CButton * pB4Serial;
			pB4Serial = (CButton *)GetDlgItem(IDC_ChkSerial);
			if( pB4Serial->GetCheck() == 1 )
			{
				SIReportManager.m_nUserPaperSize = -9999;
				SIReportManager.m_nOrientation = DMORIENT_PORTRAIT;
				SIReportManager.m_nResolution = 1728;
			}
			if		( _T("Y") == m_str64 )	ids = SIReportManager.GetSIReportFileFromDB(_T("�������B4_����"));
			else if	( _T("Y") == m_str90 )	ids = SIReportManager.GetSIReportFileFromDB(_T("�������B4_�����"));
			else							ids = SIReportManager.GetSIReportFileFromDB(_T("�������B4"));
		}
		else if (static_cast<CButton*>(GetDlgItem(IDC_rA4_RANDSCAPE))->GetCheck() == 1 ) 
		{
			if ( _T("Y") == m_str64 )	ids = SIReportManager.GetSIReportFileFromDB(_T("�������_����_����"));
			else						ids = SIReportManager.GetSIReportFileFromDB(_T("�������_����"));
		}
		else
		{
			if ( _T("Y") == m_str64 )	ids = SIReportManager.GetSIReportFileFromDB(_T("�������_����"));
			else						ids = SIReportManager.GetSIReportFileFromDB(_T("�������"));
		}
		
		if( ids < 0 )
		{
			AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
			return;
		}
		
		CString sYear, sAccessionNo;
		GetControlData(_T("CM_BO_CAT_REG_0800"), _T("�ۼ��⵵"), sYear );
		if( sYear == _T("") )
		{
			AfxMessageBox( _T("�ۼ��⵵�� �Է��ϼ���.") );
			return;
		}
		GetControlData( _T("CM_BO_CAT_REG_0800"), _T("���ι�ȣ"), sAccessionNo );
		if( sAccessionNo == _T("") )
		{
			AfxMessageBox( _T("���ι�ȣ�� �Է��ϼ���.") );
			return;
		}

		//��������DM
		CESL_DataMgr *pDmAccession;
		pDmAccession = FindDM( _T("DM_BO_CAT_REG_0800_����") );
		if( pDmAccession == NULL )
		{
			AfxMessageBox( _T("DM_BO_CAT_REG_0800_���� error") );
			return;
		}
		CString sAccQuery;
		sAccQuery.Format( _T("ACCESSION_REC_NO = '%s' AND ACCESSION_REC_MAKE_YEAR = '%s'"), sAccessionNo, sYear );
		if( pDmAccession->RefreshDataManager( sAccQuery ) < 0 )		return;
		m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDmAccession, _T("�Լ�����"), _T("�����Լ�����"));	
		INT iAccCount = pDmAccession->GetRowCount();


		CESL_DataMgr *pDM_RegCode = FindDM( _T("DM_BO_CAT_REG_0800_01") );
		if( pDM_RegCode == NULL )
		{
			AfxMessageBox( _T("DM_BO_CAT_PROC_0800_01") );
			return;
		}

		CString strQuery;
		//rQuery.Format(_T("ACCESSION_REC_KEY IN(SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO =%s)")
		//		, sAccessionNo );
		strQuery.Format(_T("ACCESSION_REC_KEY IN(SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO =%s AND ACCESSION_REC_MAKE_YEAR ='%s')")
						, sAccessionNo, sYear );

		CString strExtraCondition = pDM_RegCode->EXTRA_CONDITION;
		pDM_RegCode->EXTRA_CONDITION = _T("GROUP BY REG_CODE ORDER BY REG_CODE");
		if ( pDM_RegCode->RefreshDataManager( strQuery ) < 0 ) return;

		pDM_RegCode->EXTRA_CONDITION = strExtraCondition;

		INT nRegCodeCount = pDM_RegCode->GetRowCount();
		if (nRegCodeCount < 1) 
		{
			AfxMessageBox( _T("�˻��� �����Ͱ� �����ϴ�.") );
			return;
		}
		
		// 2005-11-10 Ȳ����: ����� ����/�а��� ����/å�� 
		CString sQuery, strSMA, strBMA, strSBR, strBBR, strSTOTAL, strBTOTAL;
		if( m_str90 == _T("Y") )
		{
//			INT nStotal, nBtotal;
			//���� ����
			sQuery.Format(_T("SELECT COUNT(DISTINCT(SPECIES_KEY)) FROM BO_BOOK_TBL WHERE "));
			sQuery += strQuery;
			sQuery += _T(" AND MANAGE_CODE = 'MA'");
			pDM_RegCode->GetOneValueQuery(sQuery, strSMA);
			
			//���� å��
			sQuery.Format(_T("SELECT COUNT(REG_NO) FROM BO_BOOK_TBL WHERE "));
			sQuery += strQuery;
			sQuery += _T(" AND MANAGE_CODE = 'MA'");
			pDM_RegCode->GetOneValueQuery(sQuery, strBMA);
			
			//�а� ����
			sQuery.Format(_T("SELECT COUNT(DISTINCT(SPECIES_KEY)) FROM BO_BOOK_TBL WHERE "));
			sQuery += strQuery;
			sQuery += _T(" AND MANAGE_CODE = 'BR'");
			pDM_RegCode->GetOneValueQuery(sQuery, strSBR);

			//�а� å��
			sQuery.Format(_T("SELECT COUNT(REG_NO) FROM BO_BOOK_TBL WHERE "));
			sQuery += strQuery;
			sQuery += _T(" AND MANAGE_CODE = 'BR'");
			pDM_RegCode->GetOneValueQuery(sQuery, strBBR);
			
			//�հ� ����
			sQuery.Format(_T("SELECT COUNT(DISTINCT(SPECIES_KEY)) FROM BO_BOOK_TBL WHERE "));
			sQuery += strQuery;
			sQuery += _T(" AND MANAGE_CODE IN ('BR','MA') ");
			pDM_RegCode->GetOneValueQuery(sQuery, strSTOTAL);
			
			//�հ� å��
			sQuery.Format(_T("SELECT COUNT(DISTINCT(REG_NO)) FROM BO_BOOK_TBL WHERE "));
			sQuery += strQuery;
			sQuery += _T(" AND MANAGE_CODE IN ('BR','MA') ");
			pDM_RegCode->GetOneValueQuery(sQuery, strBTOTAL);

			if(strSMA.IsEmpty())
				strSMA = _T("0");
			if(strBMA.IsEmpty())
				strBMA = _T("0");
			if(strSBR.IsEmpty())
				strSBR = _T("0");
			if(strBBR.IsEmpty())
				strBBR = _T("0");
			if(strSTOTAL.IsEmpty())
				strSTOTAL = _T("0");
			if(strBTOTAL.IsEmpty())
				strBTOTAL = _T("0");
			
/*			//�հ�
			nStotal = _ttoi(strSMA) + _ttoi(strSBR);
			nBtotal = _ttoi(strBMA) + _ttoi(strBBR);
			
			strSTOTAL.Format(_T("%d"), nStotal);
			strBTOTAL.Format(_T("%d"), nBtotal);*/
		}

		CESL_DataMgr *pDM_0800_02 = FindDM( _T("DM_BO_CAT_REG_0800_02") );
		if( pDM_0800_02 == NULL )
		{
			AfxMessageBox( _T("DM_BO_CAT_REG_0800_01") );
			return;
		}
		
		INT idx = -1;
		INT nPriceSum = 0;
		INT nRowCount = 0;
		INT iSwReg = 0;
		CString sMinRegNo;
		CString sMaxRegNo;
		// ��ϱ��� ���� ����մϴ�.
		for (INT idxRegCode = 0; idxRegCode < nRegCodeCount; idxRegCode++)
		{
			CString strRegCode = pDM_RegCode->GetCellData(_T("BB_��ϱ���"), idxRegCode);

			//��ϱ��� �������
			//strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND S.REC_KEY = B.SPECIES_KEY AND B.REG_CODE = '%s' ")
			//				_T("AND ACCESSION_REC_KEY IN(SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO =%s) ")
			//				_T("ORDER BY B.REG_CODE, B.REG_NO"), strRegCode, sAccessionNo );
			
			//===================================================================
			//2008.07.03 UPDATE BY PJW : ������½� �η��ڷ� ���� ������ ���� ������ �޶������� ����
//			strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND S.REC_KEY = B.SPECIES_KEY AND B.REG_CODE = '%s' ")
//							_T("AND ACCESSION_REC_KEY IN(SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO =%s AND ACCESSION_REC_MAKE_YEAR ='%s') ")
//							_T("ORDER BY B.REG_CODE, B.REG_NO"), strRegCode, sAccessionNo, sYear );

			CButton* pBAppendixAdd;
			pBAppendixAdd = (CButton *)GetDlgItem(IDC_chkAPPENDIX_ADD);
			if( pBAppendixAdd->GetCheck() == 1 )
			{
				strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND S.REC_KEY = B.SPECIES_KEY AND B.REG_CODE = '%s' ")
								_T("AND ACCESSION_REC_KEY IN(SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO =%s AND ACCESSION_REC_MAKE_YEAR ='%s') ")
								_T("ORDER BY B.REG_CODE, B.REG_NO"), strRegCode, sAccessionNo, sYear );
			}
			else
			{
				strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND S.REC_KEY = B.SPECIES_KEY AND B.REG_CODE = '%s' ")
								_T("AND ACCESSION_REC_KEY IN(SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO =%s AND ACCESSION_REC_MAKE_YEAR ='%s') ")
								_T("AND BOOK_APPENDIX_FLAG = 'B' ")
								_T("ORDER BY B.REG_CODE, B.REG_NO"), strRegCode, sAccessionNo, sYear );
			}
			//===================================================================

			if ( pDM_0800_02->RefreshDataManager( strQuery ) < 0 ) return;

			// ��ϱ��� �������� �ٲ���
			CString strRegCodeDesc;
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("��ϱ���"), strRegCode, strRegCodeDesc);

			nRowCount = pDM_0800_02->GetRowCount();
			if (nRowCount < 1) continue;

			nPriceSum = 0;
			iSwReg = 0;
			for (idx = 0; idx < nRowCount; idx++)
			{
				//���»��� �����
				CString sPage = pDM_0800_02->GetCellData( _T("BB_�����"), idx );
				CString sPhysical = pDM_0800_02->GetCellData( _T("BB_������Ư��"), idx );
				CString sBookSize = pDM_0800_02->GetCellData( _T("BB_ũ��"), idx );
				CString sAccompMat = pDM_0800_02->GetCellData( _T("BB_�����ڷ�"), idx );// ���� �߰� 2004.10.01 
				CString sForm = sPage;
				if( sPhysical != _T("") )
				{
					if ( sForm != _T("") )
					{
						sForm = sForm + _T(" : ") + sPhysical;
					}
					else
					{
						sForm = sPhysical;
					}
				}
				if( sBookSize != _T("") )
				{
					if( sForm != _T("") )
					{
						sForm = sForm + _T(" ; ") + sBookSize;
					}
					else
					{
						sForm = sBookSize;
					}				
				}
				// ���� �߰� 2004.10.01 ++++++++++++++++++++++++++++++++++++
				// ���ı��п� �����ڷ� ������ �߰�
				if ( sAccompMat != _T("") )
				{
					if( sForm != _T("") )
					{
						sForm = sForm + _T(" + ") + sAccompMat;
					}
					else 
					{
						sForm = sAccompMat;
					}
				}
				// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				pDM_0800_02->SetCellData( _T("���»���"), sForm, idx );
				//û����ȣ �����
				CString sShelfCode = pDM_0800_02->GetCellData( _T("BB_��ġ��ȣ_�ڵ�"), idx );
				CString sClassNo = pDM_0800_02->GetCellData( _T("BB_�з���ȣ"), idx );
				CString sBookCode = pDM_0800_02->GetCellData( _T("BB_������ȣ"), idx );
				CString sVolCode = pDM_0800_02->GetCellData( _T("BB_�����"), idx );
				CString sDupCode = pDM_0800_02->GetCellData( _T("BB_������ȣ"), idx );
				
				// ��ġ��ȣ �������� �ٲٱ�
				if (!sShelfCode.IsEmpty())
					m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("��ġ��ȣ"), sShelfCode, sShelfCode);
				CString sCallNo = sShelfCode;
				
				if ( sClassNo != _T("") ) {
					if ( sCallNo != _T("") ) {
						sCallNo = sCallNo + _T(" ") + sClassNo;
					}
					else {
						sCallNo = sClassNo;
					}
				}
				if ( sBookCode != _T("") ) {
					if ( sBookCode != _T("") ) {
						sCallNo = sCallNo + _T("-") + sBookCode;
					}
					else {
						sCallNo = sBookCode;
					}
				}
				if ( sVolCode != _T("") ) {
					if ( sVolCode != _T("") ) {
						sCallNo = sCallNo + _T("-") + sVolCode;
					}
					else {
						sCallNo = sVolCode;
					}
				}
				if ( sDupCode != _T("") ) {
					if ( sDupCode != _T("") ) {
						sCallNo = sCallNo + _T("=") + sDupCode;
					}
					else {
						sCallNo = sDupCode;
					}
				}
				pDM_0800_02->SetCellData( _T("û����ȣ"), sCallNo, idx );
				//��Ϲ�ȣ
				CString sRegNo = pDM_0800_02->GetCellData( _T("BB_��Ϲ�ȣ"), idx );
				if ( iSwReg == 0 && sRegNo != _T("") ) {
					sMinRegNo = sRegNo;
					sMaxRegNo = sRegNo;
					iSwReg = 1;
				}
				else {
					if ( sRegNo < sMinRegNo ) {
						sMinRegNo = sRegNo;
					}
					else if ( sRegNo > sMaxRegNo ) {
						sMaxRegNo = sRegNo;
					}
				}
				//��ũ���� ǥ�����ۻ��� �����
				CString sStreamMarc;			
				pDM_0800_02->GetCellData( _T("BS_MARC"), idx, sStreamMarc );
				CMarc marc;
				m_pInfo->pMarcMgr->Decoding( sStreamMarc, &marc );
				CString sTitleInfo;		//ǥ�����ۻ���
				CString s245a;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$a"), s245a );
				sTitleInfo = s245a;

				CString s245h;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$h"), s245h );
				if ( s245h != _T("") ) {
					if ( sTitleInfo != _T("") ) {
						sTitleInfo = sTitleInfo + s245h;
					}
					else {
						sTitleInfo = s245h;
					}
				}
				
				CString s245b;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$b"), s245b );
				if ( s245b != _T("") ) {
					if ( sTitleInfo != _T("") ) {
						sTitleInfo = sTitleInfo + _T(" : ") + s245b;
					}
					else {
						sTitleInfo = s245b;
					}
				}

				//m_pInfo->pMarcMgr->GetItem( &marc, _T("245$n"), s245n );
				//s245n = pDM_0800_02->GetCellData( _T("BB_��å��ȣ"), idx );
				CString s245n = _T("");
				if		( _T("0") == m_strVolValue )	m_pInfo->pMarcMgr->GetItem( &marc, _T("245$n"), s245n );
				else if	( _T("1") == m_strVolValue )	s245n = pDM_0800_02->GetCellData( _T("BB_��å��ȣ"), idx );
				else									s245n = _T("");
				if( FALSE == s245n.IsEmpty() )
				{
					sTitleInfo = sTitleInfo + _T(" . ") + s245n;
				}

				CString s245p;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$p"), s245p );
				s245p = pDM_0800_02->GetCellData( _T("BB_����"), idx );
				if ( s245p != _T("") ) {
					sTitleInfo = sTitleInfo + _T(" , ") + s245p;
				}
				CString s245d;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$d"), s245d );
				s245d = pDM_0800_02->GetCellData( _T("BB_������"), idx );
				if ( s245d != _T("") ) {
					sTitleInfo = sTitleInfo + _T(" / ") + s245d;
				}
				/*
				//++++++++++++++++++++++++++++++++++++++++++++++++++
                //2003.08.28 ������ �߰� 
				//������½� ǥ�����ۻ��׿� 245$e �߰� �ǵ��� ���� 
                CString s245e;
				CArray<CString,CString&> arr245e ;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$e"), s245e ,&arr245e);
				if ( s245e != _T("") ) {
					for(INT i = 0 ; i< arr245e.GetSize ();i++) 
					{
					   sTitleInfo = sTitleInfo + _T(" ; ") +  arr245e.GetAt (i);

					}
				}                
				//+++++++++++++++++++++++++++++++++++++++++++++++++++
				*/

				CString s250a;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("250$a"), s250a );
				if ( s250a != _T("") ) {
					sTitleInfo = sTitleInfo + _T(" .- ") + s250a;
				}
/*				//�Ѽ�ǥ��,�Ѽ�����
				CString s490a;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("490$a"), s490a );
				if ( s490a != _T("") ) {
					sTitleInfo = sTitleInfo + _T(" .- ( ") + s490a + _T(" )");
				}
				else {
					CString s440;
					CString s440a;
					m_pInfo->pMarcMgr->GetItem( &marc, _T("440$a"), s440a );
					if ( s440a != _T("") ) {
						CString s440v;
						m_pInfo->pMarcMgr->GetItem( &marc, _T("440$v"), s440v );
						s440 = _T("( ") + s440a + _T(" ; ") + s440v + _T(" )");
					}
					if ( s440 != _T("") ) {
						sTitleInfo = sTitleInfo + _T(" .- ") + s440;
					}
				}		
				pDM_0800_02->SetCellData( _T("IBS_ǥ������"), sTitleInfo, idx );
*/				
				//2006-01-24 hjy  ���� : 440 �켱
				//�Ѽ�ǥ��,�Ѽ�����
				
				CString s440;
				CString s440a;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("440$a"), s440a );
				if ( s440a != _T("") )
				{
					CString s440v;
					m_pInfo->pMarcMgr->GetItem( &marc, _T("440$v"), s440v );
					s440 = _T("( ") + s440a + _T(" ; ") + s440v + _T(" )");
				}
				if ( s440 != _T("") )
				{
					sTitleInfo = sTitleInfo + _T(" .- ") + s440;
				}
				else
				{
					CString s490a;
					m_pInfo->pMarcMgr->GetItem( &marc, _T("490$a"), s490a );
					if ( s490a != _T("") ) 
					{
						sTitleInfo = sTitleInfo + _T(" .- ( ") + s490a + _T(" )");
					}
				}
				
				pDM_0800_02->SetCellData( _T("IBS_ǥ������"), sTitleInfo, idx );
				//������� �����
				CString sPublishInfo;		//�������
				CString s260a;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("260$a"), s260a );
				sPublishInfo = s260a;
				
				CString s260b;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("260$b"), s260b );
				if ( s260b != _T("") )
				{
					if ( sPublishInfo != _T("") )
					{
						sPublishInfo = sPublishInfo + _T(" : ") + s260b;
					}
					else
					{
						sPublishInfo = s260b;
					}
				}
				CString s260c;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("260$c"), s260c );
				if ( s260c != _T("") ) {
					if ( sPublishInfo != _T("") ) {
						sPublishInfo = sPublishInfo + _T(", ") + s260c;
					}
					else {
						sPublishInfo = s260c;
					}
				}
				pDM_0800_02->SetCellData( _T("IBS_�����ڻ���"), sPublishInfo, idx );
				
				//����
				CString sPrice;
				pDM_0800_02->GetCellData( _T("BB_����"), idx, sPrice );

				CString sPriceCharacter;
				pDM_0800_02->GetCellData( _T("BB_����Ư��"), idx, sPriceCharacter );
				
				if ( sPrice == _T("0") || sPrice == _T(""))
				{
					pDM_0800_02->SetCellData( _T("BB_����"), sPriceCharacter, idx );

				}
				else 
				{
					nPriceSum += _ttoi(sPrice);
					// 2005.10.07 REM BY PDJ					
					//���� ǥ�� �������� �����					
					//NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
					//TCHAR szOUT[20];
					//::GetNumberFormat(NULL, NULL, sPrice, &nFmt, szOUT, 20);
					//sPrice = _T("\\") + sPrice;
					/*
					sPrice.Format(_T("\\%s"), szOUT);
					pDM_0800_02->SetCellData( _T("BB_����"), sPrice, idx );
					*/
					CString sItem, sPriceCode;
					m_pInfo->pMarcMgr->GetItem( &marc, _T("950$b"), sItem );
					for ( INT i = 0 ; i < sItem.GetLength() ; i++ ) {
						CString sOneWord = sItem.Mid( i, 1 );
						if ( sOneWord >= _T("0") && sOneWord <= _T("9") ) {
							break;
						}
						sPriceCode = sPriceCode + sOneWord;
					}
					
					// 2005.10.07 REM BY PDJ
					// \�� ��� �Ҽ��� ����
					CString strTempPrice;
					strTempPrice = sPrice;
					INT nPoint;
					if ( sPriceCode == _T("\\") || sPriceCode == _T(""))
					{						
						nPoint= strTempPrice.Find(_T("."), 0 );
						if ( nPoint > 0 ) sPrice.Format(_T("\\%s"), strTempPrice.Left(nPoint));
						else sPrice.Format(_T("\\%s"), strTempPrice );
					}
					else
					{
						sPrice.Format(_T("%s%s"), sPriceCode, strTempPrice);
					}

					/*
					if ( sPriceCode != _T("") ) {
						sPrice.Format(_T("%s%s"), sPriceCode, szOUT);
					}
					else {
						sPrice.Format(_T("\\%s"), szOUT);
					}
					*/
					pDM_0800_02->SetCellData( _T("BB_����"), sPrice, idx );
				}			


/* ���� ���� : ������½� ���� ���� ������ ' _T(" '�� ����ϰ� ���� +++
 * ���� ��¥ : 2003.09.03
 * ���� ��� : ��ȫ�� 
 */
/*
			CString strSKeyPre = pDM_0800_02->GetCellData( _T("BB_��Ű"), idx-1 );
			CString strSKeyNow = pDM_0800_02->GetCellData( _T("BB_��Ű"), idx );

				if ( strSKeyPre == strSKeyNow ) 
				{	
					pDM_0800_02->SetCellData( _T("IBS_ǥ������"), _T("                '' "), idx );
					pDM_0800_02->SetCellData( _T("IBS_�����ڻ���"), _T("           '' "), idx );
					pDM_0800_02->SetCellData( _T("���»���"), _T("            '' "), idx );
					pDM_0800_02->SetCellData( _T("BB_����"), _T("   ''     "), idx );
					//pDM_0800_02->SetCellData( _T("û����ȣ"), _T("     '' "), idx );
				}
*/
// ���� ���� : ������½� ���� ���� ������ ' _T(" '�� ����ϰ� ���� +++
				
				// 2005-11-10 Ȳ���� : ����� �������� ����, ������ ����
				if( m_str90 == _T("Y"))
				{
					CString strManage;
					pDM_0800_02->GetCellData(_T("BB_��������"),idx,strManage);
					if(strManage == _T("MA"))
						pDM_0800_02->SetCellData(_T("��������"),_T("����"),idx);
					else if(strManage == _T("BR"))
						pDM_0800_02->SetCellData(_T("��������"),_T("�а�"),idx);
					else
						pDM_0800_02->SetCellData(_T("��������"),_T(""),idx);
					
					//������
					CString strDonator, strTmp;
					pDM_0800_02->GetCellData(_T("BB_��Ű"),idx,strTmp);
					sQuery.Format(_T("SELECT D.DONATOR_NAME ")
								  _T("FROM BO_DONINFO_TBL D, BO_BOOK_TBL B ")
								  _T("WHERE B.SPECIES_KEY = D.SPECIES_KEY ")
								  _T("AND B.ACQ_CODE = '2' ")
								  _T("AND B.SPECIES_KEY = '%s'")
								  _T("AND B.MANAGE_CODE = UDF_MANAGE_CODE")
								  , strTmp);
					pDM_0800_02->GetOneValueQuery(sQuery,strDonator);
					pDM_0800_02->SetCellData(_T("������"),strDonator,idx);

					pDM_0800_02->SetCellData(_T("BB_�������"), pDmAccession->GetCellData( _T("���ε������"), 0 ), idx);
				}
			}
			//����ϱ�
			CString sAccAcqCode = pDmAccession->GetCellData( _T("�����Լ�����"), 0 );
			CString sPrice;
			sPrice.Format(_T("%d"), nPriceSum);

			CString sRegNo = sMinRegNo + _T(" ����  ") + sMaxRegNo + _T(" ����");
			//å�� ���ϱ�		
			CString sBookCount;
			sBookCount.Format(_T("%d"), nRowCount);
			
			CString sRegDate = pDmAccession->GetCellData( _T("���ε������"), 0 );
			SIReportManager.SetTempVariable(_T("���Ա���"), sAccAcqCode);
			SIReportManager.SetTempVariable(_T("���Գ⵵"), sYear);
			//����
			//���� ǥ�� �������� �����
			NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
			TCHAR szOUT[20];
			::GetNumberFormat(NULL, NULL, sPrice, &nFmt, szOUT, 20);
			CString sPriceFont;		
			sPriceFont.Format( _T("%15s"), szOUT );
			SIReportManager.SetTempVariable(_T("����"), sPriceFont);
			SIReportManager.SetTempVariable(_T("��ϱ���"), strRegCodeDesc);
			SIReportManager.SetTempVariable(_T("���ι�ȣ"), sAccessionNo);
			SIReportManager.SetTempVariable(_T("��Ϲ�ȣ"), sRegNo);

            //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            //2003.08.23 ������ ���� 
			//������¹��� ����,���� ������ ����.
			CString sClassName,sRegistrant ;
            strQuery.Format (_T("select class_name FROM CO_ACCESSION_REC_TBL WHERE  ACCESSION_REC_NO = %s AND ACCESSION_REC_MAKE_YEAR = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), sAccessionNo, sYear);
            pDmAccession->GetOneValueQuery (strQuery,sClassName);
            strQuery.Format (_T("select REGISTRANT FROM CO_ACCESSION_REC_TBL WHERE  ACCESSION_REC_NO = %s AND ACCESSION_REC_MAKE_YEAR = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"),sAccessionNo, sYear );
			pDmAccession->GetOneValueQuery (strQuery,sRegistrant); 
            
			SIReportManager.SetTempVariable(_T("����"), sClassName);
			SIReportManager.SetTempVariable(_T("����"), sRegistrant);
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			
			//2005-11-10 Ȳ���� : ����/���� ����
			if( m_str90 == _T("Y"))
			{
				SIReportManager.SetTempVariable(_T("��������"), strSMA);
				SIReportManager.SetTempVariable(_T("����å��"), strBMA);
				SIReportManager.SetTempVariable(_T("�а�����"), strSBR);
				SIReportManager.SetTempVariable(_T("�а�å��"), strBBR);
				SIReportManager.SetTempVariable(_T("���հ�"), strSTOTAL);
				SIReportManager.SetTempVariable(_T("å�հ�"), strBTOTAL);
			}

			//å�� �� ���߱�
			CString sBookFont;
			sBookFont.Format( _T("%15s"), sBookCount );
			SIReportManager.SetTempVariable(_T("å��"), sBookFont);		
			SIReportManager.SetTempVariable(_T("�������"), sRegDate);

			// 2005.06.27 ADD BY PDJ
			// ���� �߰���
			CString sRemark = pDmAccession->GetCellData( _T("���"), 0 );
			SIReportManager.SetTempVariable(_T("���"), sRemark);
			
			SIReportManager.SetTempVariable(_T("LIB_NAME"), strLibName);
			
			SIReportManager.SetDataMgr(0, pDM_0800_02);
			//ids = SIReportManager.MakeXMLPageFiles();
			ids = SIReportManager.MAKEPAGE_DrawWorkForm(-1);		// ��ũ�� Manual �ݺ� ���
			// ������ ������ �׸�
			Print_SetLastPageLine();			
		}
		ids = SIReportManager.Print2();
	}

	//��Ϲ�ȣ���� ���
	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	CButton * pACCESSIONNO_PRINT;
	pACCESSIONNO_PRINT = (CButton *)GetDlgItem(IDC_rACCESSIONNO_PRINT);	
	if ( pACCESSIONNO_PRINT->GetCheck() == 1 )
	{
		CString sRegNoStart, sRegNoEnd;
		GetControlData( _T("CM_BO_CAT_REG_0800"), _T("��Ϲ�ȣ����"), sRegNoStart );
		GetControlData( _T("CM_BO_CAT_REG_0800"), _T("��Ϲ�ȣ����"), sRegNoEnd );
		sRegNoStart.Remove(' ');
		sRegNoEnd.Remove(' ');
		/*if ( sRegNoStart == _T("") || sRegNoEnd == _T("") ) {
			AfxMessageBox( _T("��Ϲ�ȣ ������ �Է��ϼ���.") );
			return;
		}*/
		//2007.06.29 ADD PJW : ��Ϲ�ȣ�����ڵ带 �޺��ڽ��� ����
		//=====================================================
		//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 		if ( sRegNoStart == _T("") || sRegNoStart.GetLength() > 10) 
// 		{
// 			AfxMessageBox( _T("���� �˻� ��Ϲ�ȣ�� �ڸ� �� ������ �Է��ϼ���.") );
// 			SetDlgItemText( IDC_eACCESSION_NO_START, _T("") );
// 			return;
// 		}
// 
// 		if (  sRegNoEnd == _T("") || sRegNoEnd.GetLength() > 10) 
// 		{
// 			AfxMessageBox( _T("������ �˻� ��Ϲ�ȣ�� �ڸ� �� ������ �Է��ϼ���.") );
// 			SetDlgItemText(IDC_eACCESSION_NO_END,_T(""));
// 			return;
// 		}
// 		CString sRegNoCode;
// 		GetDlgItemText(IDC_eACCESSION_CODE,sRegNoCode);
// 		sRegNoCode.Remove(' ');
// 
// 		if ( sRegNoCode == _T("") || sRegNoCode.GetLength() !=2 ) 
// 		{
// 			AfxMessageBox( _T("��ϱ��� �ڵ带 ��Ȯ�� �Է��ϼ���.") );
// 			//pCombBOX->SetCurSel(0);
// 			return;
// 		}
// 		
// 		if ( sRegNoStart != _T("") && sRegNoStart.GetLength() <= 10 ) {		
// 			if ( sRegNoStart.GetLength() > 0 ) {
// 				CString sRight;
// 				sRight.Format( _T("%010s") , sRegNoStart );
// 				sRegNoStart = sRegNoCode + sRight;
// 			}
// 		}
// 		
// 
// 		if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() <= 10 ) {
// 			if ( sRegNoEnd.GetLength() > 0 ) {
// 				CString sRight;
// 				sRight.Format( _T("%010s") , sRegNoEnd );
// 				sRegNoEnd = sRegNoCode + sRight;
// 			}
// 		}

		if ( sRegNoStart == _T("") || sRegNoStart.GetLength() > 12-nRegCodeLength) 
		{
			AfxMessageBox( _T("���� �˻� ��Ϲ�ȣ�� �ڸ� �� ������ �Է��ϼ���.") );
			SetDlgItemText( IDC_eACCESSION_NO_START, _T("") );
			return;
		}

		if (  sRegNoEnd == _T("") || sRegNoEnd.GetLength() > 12-nRegCodeLength) 
		{
			AfxMessageBox( _T("������ �˻� ��Ϲ�ȣ�� �ڸ� �� ������ �Է��ϼ���.") );
			SetDlgItemText(IDC_eACCESSION_NO_END,_T(""));
			return;
		}
		CString sRegNoCode;
		GetDlgItemText(IDC_eACCESSION_CODE,sRegNoCode);
		sRegNoCode.Remove(' ');

		if ( sRegNoCode == _T("") || sRegNoCode.GetLength() !=nRegCodeLength ) 
		{
			AfxMessageBox( _T("��ϱ��� �ڵ带 ��Ȯ�� �Է��ϼ���.") );
			//pCombBOX->SetCurSel(0);
			return;
		}
		
		if ( sRegNoStart != _T("") && sRegNoStart.GetLength() <= 12-nRegCodeLength ) {		
			if ( sRegNoStart.GetLength() > 0 ) {
				sRegNoStart = m_pInfo->MakeRegNo(sRegNoCode,sRegNoStart);
			}
		}
		

		if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() <= 12-nRegCodeLength ) {
			if ( sRegNoEnd.GetLength() > 0 ) {
				sRegNoEnd = m_pInfo->MakeRegNo(sRegNoCode,sRegNoEnd);
			}
		}
		//=====================================================
		

		
		

		//2007.06.29 DELETE PJW : ��Ϲ�ȣ�����ڵ带 �޺��ڽ��� �����Ƿ� �ڵ� �� �κ� ����
		/*
		if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) {		
			if ( sRegNoStart.GetLength() > 2 ) {
				CString sRight;
				sRight.Format( _T("%010s") , sRegNoStart.Mid(2) );
				sRegNoStart = sRegNoStart.Left(2) + sRight;
			}
		}
		if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) {
			if ( sRegNoEnd.GetLength() > 2 ) {
				CString sRight;
				sRight.Format( _T("%010s") , sRegNoEnd.Mid(2) );
				sRegNoEnd = sRegNoEnd.Left(2) + sRight;
			}
		}

		if ( sRegNoStart.Left(2) != sRegNoEnd.Left(2) ) {
			AfxMessageBox( _T("��ϱ����� ���ƾ� �մϴ�.") );
			return;
		}
		*/

		CESL_DataMgr *pDM;
		pDM = FindDM( _T("DM_BO_CAT_REG_0800_01") );
		if ( pDM == NULL ) {
			AfxMessageBox( _T("DM_BO_CAT_PROC_0800_01") );
			return;
		}

		CString strQuery;
		//=====================================================
		//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 		strQuery.Format( _T("REG_NO >= '%s' AND REG_NO <= '%s' AND REG_CODE = '%s'"), sRegNoStart, sRegNoEnd, sRegNoStart.Left(2) );

		strQuery.Format( _T("REG_NO >= '%s' AND REG_NO <= '%s' AND REG_CODE = '%s'"), sRegNoStart, sRegNoEnd, sRegNoStart.Left(nRegCodeLength) );
		//=====================================================
		
		if ( pDM->RefreshDataManager( strQuery ) < 0 ) return;

		INT nRowCount = pDM->GetRowCount();
		if ( nRowCount < 1 ) {
			AfxMessageBox( _T("�˻��� �����Ͱ� �����ϴ�.") );
			return;
		}
		
		//2005-11-10 Ȳ���� �߰�
		CString sQuery, strSMA, strBMA, strSBR, strBBR, strSTOTAL, strBTOTAL;
		if( m_str90 == _T("Y"))
		{
//			INT nStotal, nBtotal;

			//���� ����
			sQuery.Format(_T("SELECT COUNT(DISTINCT(SPECIES_KEY)) FROM BO_BOOK_TBL WHERE "));
			sQuery += strQuery;
			sQuery += _T(" AND MANAGE_CODE = 'MA'");
			pDM->GetOneValueQuery(sQuery, strSMA);
			
			//���� å��
			sQuery.Format(_T("SELECT COUNT(REG_NO) FROM BO_BOOK_TBL WHERE "));
			sQuery += strQuery;
			sQuery += _T(" AND MANAGE_CODE = 'MA'");
			pDM->GetOneValueQuery(sQuery, strBMA);
			
			//�а� ����
			sQuery.Format(_T("SELECT COUNT(DISTINCT(SPECIES_KEY)) FROM BO_BOOK_TBL WHERE "));
			sQuery += strQuery;
			sQuery += _T(" AND MANAGE_CODE = 'BR'");
			pDM->GetOneValueQuery(sQuery, strSBR);

			//�а� å��
			sQuery.Format(_T("SELECT COUNT(REG_NO) FROM BO_BOOK_TBL WHERE "));
			sQuery += strQuery;
			sQuery += _T(" AND MANAGE_CODE = 'BR'");
			pDM->GetOneValueQuery(sQuery, strBBR);

			//�հ� ����
			sQuery.Format(_T("SELECT COUNT(DISTINCT(SPECIES_KEY)) FROM BO_BOOK_TBL WHERE "));
			sQuery += strQuery;
			sQuery += _T(" AND MANAGE_CODE IN ('BR','MA') ");
			pDM->GetOneValueQuery(sQuery, strSTOTAL);
			
			//�հ� å��
			sQuery.Format(_T("SELECT COUNT(DISTINCT(REG_NO)) FROM BO_BOOK_TBL WHERE "));
			sQuery += strQuery;
			sQuery += _T(" AND MANAGE_CODE IN ('BR','MA') ");
			pDM->GetOneValueQuery(sQuery, strBTOTAL);

			if(strSMA.IsEmpty())
				strSMA = _T("0");
			if(strBMA.IsEmpty())
				strBMA = _T("0");
			if(strSBR.IsEmpty())
				strSBR = _T("0");
			if(strBBR.IsEmpty())
				strBBR = _T("0");
			if(strSTOTAL.IsEmpty())
				strSTOTAL = _T("0");
			if(strBTOTAL.IsEmpty())
				strBTOTAL = _T("0");
			
/*			//�հ�
			nStotal = _ttoi(strSMA) + _ttoi(strSBR);
			nBtotal = _ttoi(strBMA) + _ttoi(strBBR);
			
			strSTOTAL.Format(_T("%d"), nStotal);
			strBTOTAL.Format(_T("%d"), nBtotal);*/
		}

		CESL_DataMgr *pDM_0800_02 = FindDM( _T("DM_BO_CAT_REG_0800_02") );
		if ( pDM_0800_02 == NULL ) {
			AfxMessageBox( _T("DM_BO_CAT_REG_0800_01") );
			return;
		}

		CString sRegCode;
		CString sMinRegNo, sMaxRegNo;
		CString sRegCodeDes;
		//å��
		INT iBookCount = 0;
		for ( INT i = 1 ; i <= nRowCount ; i++ ) 
		{
			sRegCode = pDM->GetCellData( _T("BB_��ϱ���"), i-1 );
			strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND S.REC_KEY = B.SPECIES_KEY ")
							_T("AND B.REG_NO >= '%s' AND B.REG_NO <= '%s' ")
							_T("AND B.REG_CODE = '%s' AND B.WORKING_STATUS > 'BOC' ORDER BY B.REG_NO")
							, sRegNoStart, sRegNoEnd, sRegCode );
			//===================================================================
			//2008.07.03 UPDATE BY PJW : ������½� �η��ڷ� ���� ������ ���� ������ �޶������� ����
//			strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND S.REC_KEY = B.SPECIES_KEY ")
//							_T("AND B.REG_NO >= '%s' AND B.REG_NO <= '%s' ")
//							_T("AND B.REG_CODE = '%s' AND B.WORKING_STATUS > 'BOC' ORDER BY B.REG_NO")
//							, sRegNoStart, sRegNoEnd, sRegCode );
			CButton* pBAppendixAdd;
			pBAppendixAdd = (CButton *)GetDlgItem(IDC_chkAPPENDIX_ADD);
			if( pBAppendixAdd->GetCheck() == 1 )
			{
				strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND S.REC_KEY = B.SPECIES_KEY ")
								_T("AND B.REG_NO >= '%s' AND B.REG_NO <= '%s' ")
								_T("AND B.REG_CODE = '%s' AND B.WORKING_STATUS > 'BOC' ORDER BY B.REG_NO")
								, sRegNoStart, sRegNoEnd, sRegCode );
			}
			else
			{
				strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND S.REC_KEY = B.SPECIES_KEY ")
								_T("AND B.REG_NO >= '%s' AND B.REG_NO <= '%s' ")
								_T("AND BOOK_APPENDIX_FLAG = 'B' ")
								_T("AND B.REG_CODE = '%s' AND B.WORKING_STATUS > 'BOC' ORDER BY B.REG_NO")
								, sRegNoStart, sRegNoEnd, sRegCode );
			}
			//===================================================================


			if ( pDM_0800_02->RefreshDataManager( strQuery ) < 0 ) return;
			sRegCodeDes = pDM->GetCellData( _T("BB_��ϱ���"), i-1 );
			INT nRowCount = pDM_0800_02->GetRowCount();
			//å��
			iBookCount = nRowCount;
			INT iSwReg = 0;
			for ( INT j = 0 ; j < nRowCount ; j ++ ) 
			{
				//���»��� �����
				CString sPage = pDM_0800_02->GetCellData( _T("BB_�����"), j );
				CString sPhysical = pDM_0800_02->GetCellData( _T("BB_������Ư��"), j );
				CString sBookSize = pDM_0800_02->GetCellData( _T("BB_ũ��"), j );
				CString sAccompMat = pDM_0800_02->GetCellData( _T("BB_�����ڷ�"), j );// ���� �߰� 2004.10.01
				CString sForm = sPage;
				if ( sPhysical != _T("") ) {
					if ( sForm != _T("") ) {
						sForm = sForm + _T(" : ") + sPhysical;
					}
					else {
						sForm = sPhysical;
					}
				}
				if ( sBookSize != _T("") ) {
					if ( sForm != _T("") ) {
						sForm = sForm + _T(" ; ") + sBookSize;
					}
					else {
						sForm = sBookSize;
					}				
				}
				// ���� �߰� 2004.10.01 ++++++++++++++++++++++++++++++++++++
				// ���ı��п� �����ڷ� ������ �߰�
				if ( sAccompMat != _T("") )
				{
					if ( sForm != _T("") )
						sForm = sForm + _T(" + ") + sAccompMat;
					else 
						sForm = sAccompMat;
				}
				// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				pDM_0800_02->SetCellData( _T("���»���"), sForm, j );
				//û����ȣ �����
				CString sShelfCode = pDM_0800_02->GetCellData( _T("BB_��ġ��ȣ_�ڵ�"), j );
				CString sClassNo = pDM_0800_02->GetCellData( _T("BB_�з���ȣ"), j );
				CString sBookCode = pDM_0800_02->GetCellData( _T("BB_������ȣ"), j );
				CString sVolCode = pDM_0800_02->GetCellData( _T("BB_�����"), j );
				CString sDupCode = pDM_0800_02->GetCellData( _T("BB_������ȣ"), j );
				// ��ġ��ȣ �������� �ٲٱ�
				if (!sShelfCode.IsEmpty())
					m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("��ġ��ȣ"), sShelfCode, sShelfCode);
				CString sCallNo = sShelfCode;
				
				if ( sClassNo != _T("") ) {
					if ( sCallNo != _T("") ) {
						sCallNo = sCallNo + _T(" ") + sClassNo;
					}
					else {
						sCallNo = sClassNo;
					}
				}
				if ( sBookCode != _T("") ) {
					if ( sBookCode != _T("") ) {
						sCallNo = sCallNo + _T("-") + sBookCode;
					}
					else {
						sCallNo = sBookCode;
					}
				}
				if ( sVolCode != _T("") ) {
					if ( sVolCode != _T("") ) {
						sCallNo = sCallNo + _T("-") + sVolCode;
					}
					else {
						sCallNo = sVolCode;
					}
				}
				if ( sDupCode != _T("") ) {
					if ( sDupCode != _T("") ) {
						sCallNo = sCallNo + _T("=") + sDupCode;
					}
					else {
						sCallNo = sDupCode;
					}
				}

				pDM_0800_02->SetCellData( _T("û����ȣ"), sCallNo, j );

				//��Ϲ�ȣ
				CString sRegNo = pDM_0800_02->GetCellData( _T("BB_��Ϲ�ȣ"), j );
				if ( iSwReg == 0 && sRegNo != _T("") ) {
					sMinRegNo = sRegNo;
					sMaxRegNo = sRegNo;
					iSwReg = 1;
				}
				else {
					if ( sRegNo < sMinRegNo ) {
						sMinRegNo = sRegNo;
					}
					else if ( sRegNo > sMaxRegNo ) {
						sMaxRegNo = sRegNo;
					}
				}

				//��ũ���� ǥ�����ۻ��� �����
				CString sStreamMarc;			
				pDM_0800_02->GetCellData( _T("BS_MARC"), j, sStreamMarc );
				CMarc marc;
				m_pInfo->pMarcMgr->Decoding( sStreamMarc, &marc );
				CString sTitleInfo;		//ǥ�����ۻ���
				CString s245a;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$a"), s245a );
				sTitleInfo = s245a;

				CString s245h;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$h"), s245h );
				if ( s245h != _T("") ) {
					if ( sTitleInfo != _T("") ) {
						sTitleInfo = sTitleInfo + s245h;
					}
					else {
						sTitleInfo = s245h;
					}
				}

				CString s245b;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$b"), s245b );
				if ( s245b != _T("") ) {
					if ( sTitleInfo != _T("") ) {
						sTitleInfo = sTitleInfo + _T(" : ") + s245b;
					}
					else {
						sTitleInfo = s245b;
					}
				}
				
				//m_pInfo->pMarcMgr->GetItem( &marc, _T("245$n"), s245n );
				//s245n = pDM_0800_02->GetCellData( _T("BB_��å��ȣ"), j );
				CString s245n = _T("");
				if		( _T("0") == m_strVolValue )	m_pInfo->pMarcMgr->GetItem( &marc, _T("245$n"), s245n );
				else if	( _T("1") == m_strVolValue )	s245n = pDM_0800_02->GetCellData( _T("BB_��å��ȣ"), j );
				else									s245n = _T("");
				if( FALSE == s245n.IsEmpty() )
				{
					sTitleInfo = sTitleInfo + _T(" . ") + s245n;
				}

				CString s245p;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$p"), s245p );
				s245p = pDM_0800_02->GetCellData( _T("BB_����"), j );
				if ( s245p != _T("") ) {
					sTitleInfo = sTitleInfo + _T(" , ") + s245p;
				}
				CString s245d;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$d"), s245d );
				s245d = pDM_0800_02->GetCellData( _T("BB_������"), j );
				if ( s245d != _T("") ) {
					sTitleInfo = sTitleInfo + _T(" / ") + s245d;
				}			
				CString s250a;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("250$a"), s250a );
				if ( s250a != _T("") ) {
					sTitleInfo = sTitleInfo + _T(" .- ") + s250a;
				}
/*				//�Ѽ�ǥ��,�Ѽ�����
				CString s490a;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("490$a"), s490a );
				if ( s490a != _T("") ) {
					sTitleInfo = sTitleInfo + _T(" .- ( ") + s490a + _T(" )");
				}
				else {
					CString s440;
					CString s440a;
					m_pInfo->pMarcMgr->GetItem( &marc, _T("440$a"), s440a );
					if ( s440a != _T("") ) {
						CString s440v;
						m_pInfo->pMarcMgr->GetItem( &marc, _T("440$v"), s440v );
						s440 = _T("( ") + s440a + _T(" ; ") + s440v + _T(" )");
					}
					if ( s440 != _T("") ) {
						sTitleInfo = sTitleInfo + _T(" .- ") + s440;
					}
				}		
				pDM_0800_02->SetCellData( _T("IBS_ǥ������"), sTitleInfo, j );
*/
				//2006-01-24 hjy  ���� : 440 �켱
				//�Ѽ�ǥ��,�Ѽ�����
				
				CString s440;
				CString s440a;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("440$a"), s440a );
				if ( s440a != _T("") ) {
					CString s440v;
					m_pInfo->pMarcMgr->GetItem( &marc, _T("440$v"), s440v );
					s440 = _T("( ") + s440a + _T(" ; ") + s440v + _T(" )");
				}
				if ( s440 != _T("") ) {
					sTitleInfo = sTitleInfo + _T(" .- ") + s440;
				}
				else
				{
					CString s490a;
					m_pInfo->pMarcMgr->GetItem( &marc, _T("490$a"), s490a );
					if ( s490a != _T("") ) 
					{
						sTitleInfo = sTitleInfo + _T(" .- ( ") + s490a + _T(" )");
					}
				}

				pDM_0800_02->SetCellData( _T("IBS_ǥ������"), sTitleInfo, j );
				//������� �����
				CString sPublishInfo;		//�������
				CString s260a;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("260$a"), s260a );
				sPublishInfo = s260a;

				CString s260b;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("260$b"), s260b );
				if ( s260b != _T("") ) {
					if ( sPublishInfo != _T("") ) {
						sPublishInfo = sPublishInfo + _T(" : ") + s260b;
					}
					else {
						sPublishInfo = s260b;
					}
				}
				CString s260c;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("260$c"), s260c );
				if ( s260c != _T("") ) {
					if ( sPublishInfo != _T("") ) {
						sPublishInfo = sPublishInfo + _T(", ") + s260c;
					}
					else {
						sPublishInfo = s260c;
					}
				}
				pDM_0800_02->SetCellData( _T("IBS_�����ڻ���"), sPublishInfo, j );
				
				//����
				CString sPrice;
				pDM_0800_02->GetCellData( _T("BB_����"), j, sPrice );

				CString sPriceCharacter;
				pDM_0800_02->GetCellData( _T("BB_����Ư��"), j, sPriceCharacter );
				
				if ( sPrice == _T("0") || sPrice == _T(""))
				{
					pDM_0800_02->SetCellData( _T("BB_����"), sPriceCharacter, j );

				}
				else 
				{
					//===================================================
					//2010.04.30 UPDATE BY PJW : �ܱ� ȭ���ϰ�� �״�� �ֵ��� ������
//					//���� ǥ�� �������� �����
//					TCHAR szOUT[20];
//					//sPrice = _T("\\") + sPrice;
//					/*
//					sPrice.Format(_T("\\%s"), szOUT);
//					pDM_0800_02->SetCellData( _T("BB_����"), sPrice, j );
//					*/
//					CString sItem, sPriceCode;
//					m_pInfo->pMarcMgr->GetItem( &marc, _T("950$b"), sItem );
//					for ( INT i = 0 ; i < sItem.GetLength() ; i++ ) {
//						CString sOneWord = sItem.Mid( i, 1 );
//						if ( sOneWord >= _T("0") && sOneWord <= _T("9") ) {
//							break;
//						}
//						sPriceCode = sPriceCode + sOneWord;
//					}
//
//					if ( sPriceCode == _T("\\") )
//					{
//						NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
//						::GetNumberFormat(NULL, NULL, sPrice, &nFmt, szOUT, 20);
//					}
//					else 
//					{
//						NUMBERFMT nFmt = { 0, 0, 20, _T("."), _T("."), 1 };
//						::GetNumberFormat(NULL, NULL, sPrice, &nFmt, szOUT, 20);
//					}
//
//
//					if ( sPriceCode != _T("") ) {
//						sPrice.Format(_T("%s%s"), sPriceCode, szOUT);
//					}
//					else {
//						sPrice.Format(_T("\\%s"), szOUT);
//					}
//					pDM_0800_02->SetCellData( _T("BB_����"), sPrice, j );
					CString sItem, sPriceCode;
					m_pInfo->pMarcMgr->GetItem( &marc, _T("950$b"), sItem );
					for ( INT i = 0 ; i < sItem.GetLength() ; i++ ) {
						CString sOneWord = sItem.Mid( i, 1 );
						if ( sOneWord >= _T("0") && sOneWord <= _T("9") ) {
							break;
						}
						sPriceCode = sPriceCode + sOneWord;
					}

					// \�� ��� �Ҽ��� ����
					CString strTempPrice;
					strTempPrice = sPrice;
					INT nPoint;
					if ( sPriceCode == _T("\\") || sPriceCode == _T(""))
					{						
						nPoint= strTempPrice.Find(_T("."), 0 );
						if ( nPoint > 0 ) sPrice.Format(_T("\\%s"), strTempPrice.Left(nPoint));
						else sPrice.Format(_T("\\%s"), strTempPrice );
					}
					else
					{
						sPrice.Format(_T("%s%s"), sPriceCode, strTempPrice);
					}

					pDM_0800_02->SetCellData( _T("BB_����"), sPrice, j );
					//===================================================
				}

				//��� 950$c���� �־�޶� �ϼ���
				CString s950c;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("950$c"), s950c );
				pDM_0800_02->SetCellData( _T("BB_���"), s950c, j );
				
				//2005-11-10 Ȳ���� : ����� ��������, ������ ��
				CString sQuery;
				if(m_str90 == _T("Y"))
				{
					CString strManage;
					pDM_0800_02->GetCellData(_T("BB_��������"),j,strManage);
					if(strManage == _T("MA"))
						pDM_0800_02->SetCellData(_T("��������"),_T("����"),j);
					else if(strManage == _T("BR"))
						pDM_0800_02->SetCellData(_T("��������"),_T("�а�"),j);
					else
						pDM_0800_02->SetCellData(_T("��������"),_T(""),j);
					
					//������
					CString strDonator, strTmp;
					pDM_0800_02->GetCellData(_T("BB_��Ű"),j,strTmp);
					sQuery.Format(_T("SELECT D.DONATOR_NAME ")
								  _T("FROM BO_DONINFO_TBL D, BO_BOOK_TBL B ")
								  _T("WHERE B.SPECIES_KEY = D.SPECIES_KEY ")
								  _T("AND B.ACQ_CODE = '2' ")
								  _T("AND B.SPECIES_KEY = '%s' ")
								  _T("AND B.MANAGE_CODE = UDF_MANAGE_COD E")
								  , strTmp);
					pDM_0800_02->GetOneValueQuery(sQuery,strDonator);
					pDM_0800_02->SetCellData(_T("������"),strDonator,j);
				}

			}			
		}
		m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("��ϱ���"), _T("BB_��ϱ���"));
		sRegCodeDes = pDM->GetCellData( _T("BB_��ϱ���"), 0 );
		
		//����ϱ�
		CSIReportManager *pSIReportManager = new CSIReportManager(this);
		pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
		if (!pSIReportManager) {
			AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
			return;
		}
		
		pSIReportManager->SetCONNECTION_INFO(pDM->CONNECTION_INFO);
		//A4,B4 ����
		CButton * pB4;
		pB4 = (CButton *)GetDlgItem(IDC_rB4);
		if( pB4->GetCheck() == 1 )
		{
			CButton* pB4Serial;
			pB4Serial = (CButton *)GetDlgItem(IDC_ChkSerial);
			if( pB4Serial->GetCheck() == 1 )
			{
				pSIReportManager->m_nUserPaperSize = -9999;
				pSIReportManager->m_nOrientation = DMORIENT_PORTRAIT;
				pSIReportManager->m_nResolution = 1728;
			}
			if		( m_str64 == _T("Y") )		ids = pSIReportManager->GetSIReportFileFromDB(_T("�������B4_����"));
			else if ( m_str90 == _T("Y") )		ids = pSIReportManager->GetSIReportFileFromDB(_T("�������B4_�����"));
			else							ids = pSIReportManager->GetSIReportFileFromDB(_T("�������B4"));
		}
		else if (static_cast<CButton*>(GetDlgItem(IDC_rA4_RANDSCAPE))->GetCheck() == 1 ) 
		{
			if ( m_str64 == _T("Y") ) ids = pSIReportManager->GetSIReportFileFromDB(_T("�������_����_����"));
			else ids = pSIReportManager->GetSIReportFileFromDB(_T("�������_����"));
		}
		else
		{
			if ( m_str64 == _T("Y") ) ids = pSIReportManager->GetSIReportFileFromDB(_T("�������_����"));
			else ids = pSIReportManager->GetSIReportFileFromDB(_T("�������"));
		}
		
		if (ids < 0) {
			AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
			return;
		}
		
		CString sPrice = pDM->GetCellData( _T("����"), 0 );
		CString sRegNo = sMinRegNo + _T(" ����  ") + sMaxRegNo + _T(" ����");
		CString sBookCount;
		sBookCount.Format( _T("%d"), iBookCount );
		
		pSIReportManager->SetTempVariable(_T("LIB_NAME"), strLibName);
		//����
		//���� ǥ�� �������� �����
		NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
		TCHAR szOUT[20];
		::GetNumberFormat(NULL, NULL, sPrice, &nFmt, szOUT, 20);
		CString sPriceFont;		
		sPriceFont.Format( _T("%15s"), szOUT );
		pSIReportManager->SetTempVariable(_T("����"), sPriceFont);
		pSIReportManager->SetTempVariable(_T("��ϱ���"), sRegCodeDes);
		pSIReportManager->SetTempVariable(_T("��Ϲ�ȣ"), sRegNo);
		
		//2005-11-10 Ȳ���� : ����/���� ����
		if(m_str90 == _T("Y"))
		{
			pSIReportManager->SetTempVariable(_T("��������"), strSMA);
			pSIReportManager->SetTempVariable(_T("����å��"), strBMA);
			pSIReportManager->SetTempVariable(_T("�а�����"), strSBR);
			pSIReportManager->SetTempVariable(_T("�а�å��"), strBBR);
			pSIReportManager->SetTempVariable(_T("���հ�"), strSTOTAL);
			pSIReportManager->SetTempVariable(_T("å�հ�"), strBTOTAL);
		}

		//å�� �� ���߱�
		CString sBookFont;
		sBookFont.Format( _T("%15s"), sBookCount );
		pSIReportManager->SetTempVariable(_T("å��"), sBookFont);
		pSIReportManager->SetDataMgr(0, pDM_0800_02);
		ids = pSIReportManager->MakeXMLPageFiles();
		// ������ ������ �׸�
		Print_SetLastPageLine();
		ids = pSIReportManager->Print2();
		if (pSIReportManager) {
			delete pSIReportManager;
			pSIReportManager = NULL;
		}
	}

EFS_END
}

INT CBO_CAT_REG_0800::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;

EFS_END
return -1;

}

VOID CBO_CAT_REG_0800::Print_SetLastPageLine(VOID)
{
EFS_BEGIN

	INT ids;

	// XML �� WorkForm��ü�� �����Ѵ�.
	CSIReportXMLIOMgr *pXMLIOMgr = new CSIReportXMLIOMgr(MASTER_IS_MANAGER);
	CSIReportWorkForm *pPageWorkForm = new CSIReportWorkForm(MASTER_IS_MANAGER);
	
	if (pXMLIOMgr == NULL) {
		AfxMessageBox(_T("XML ó���� ������ �����Ͽ����ϴ�."));
		return;
	}
	if (pPageWorkForm == NULL) {
		AfxMessageBox(_T("�߰� XML ó���� ������ �����Ͽ����ϴ�."));
		return;
	}

	// ���� ������ ������ ��ȣ�� �����´�.
	INT nMaxTmpPageNum = pXMLIOMgr->GetMaxTmpPageNum();
	if (nMaxTmpPageNum <= 0) {
		AfxMessageBox(_T("������ ������ ã�� �� �����ϴ�."));
		return;
	}
	CString strFileName;
	strFileName.Format(_T("rpt_tmp\\page%04d.xml"), nMaxTmpPageNum);
	// ���� ������ ������ ������ �ҷ��´�.
	ids = pPageWorkForm->LoadWorkForm(strFileName);
	if (ids != 0) {
		AfxMessageBox(_T("�������� �ε� ���� ���Ͽ����ϴ�."));
		return;
	}
	// �ҷ��� WorkForm�߿��� First Sheet�� �����´�.(�߰� ������ ���Ͽ��� Sheet�� �Ѱ����̴�.)
	CSIReportWorkSheet *pSheet = pPageWorkForm->FindSheet(0);
	if (pSheet == NULL) {
		AfxMessageBox(_T("�������� ã�� �� �����ϴ�."));
		return;
	}
	// ������ ������ ó���� �̹� �Ǿ� �ִ��� ����. �̹� ó���Ǿ� �ִٸ� �ι� �� �ʿ� ����.
	CSIReportObject * pObject = pSheet->FindObject(_T("�߰��ȸ�����������"));
	if (pObject) return;
	// �ܺ����̺� �� �ݺ� ���̺��� �����´�.(���̺� �̸��� ��Ȳ�� ���� ������ ��)
	CSIReportObject_Table * pIn = (CSIReportObject_Table*)pSheet->FindObject(_T("���̺�_17"));
	CSIReportObject_Table * pOut = (CSIReportObject_Table*)pSheet->FindObject(_T("���̺�_43"));
	if (pIn == NULL) return;
	if (pOut == NULL) return;

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
	return;

EFS_END
}

VOID CBO_CAT_REG_0800::OnrB4() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_ChkSerial)->EnableWindow(true);
}

VOID CBO_CAT_REG_0800::OnrA4() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_ChkSerial)->EnableWindow(false);
}

VOID CBO_CAT_REG_0800::OnrA4RANDSCAPE() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_ChkSerial)->EnableWindow(false);
}

BOOL CBO_CAT_REG_0800::Print_JangSeo()
{

	CESL_DataMgr *pDM;
	pDM = FindDM ( _T("DM_�弭����") );
	if ( pDM == NULL ) 
	{
		AfxMessageBox( _T("�弭���� DM �ʱ�ȭ ����") );
		return FALSE;
	}

	//�����͸� �������� SIREPORT�� ����Ѵ�.
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	pSIReportManager->DeleteTempPageFiles();

	INT ids = pSIReportManager->GetSIReportFileFromDB(_T("�弭����"));
	if (ids < 0) {
		AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return 1;
	}

	//5163�� ��� �弭���� ��·���
	CESL_DataMgr dm;
	CButton * pAccNo = (CButton *)GetDlgItem(IDC_rSHELF_PRINT);
	if ( pAccNo->GetCheck() == 1 ) 
	{
		//���ι�ȣ�� �˻�
		CString sYear, sAccessionNo;
		GetControlData( _T("CM_BO_CAT_REG_0800"), _T("�ۼ��⵵"), sYear );
		if ( sYear == _T("") ) {
			AfxMessageBox( _T("�ۼ��⵵�� �Է��ϼ���.") );
			return FALSE;
		}
		GetControlData( _T("CM_BO_CAT_REG_0800"), _T("���ι�ȣ"), sAccessionNo );
		if ( sAccessionNo == _T("") ) {
			AfxMessageBox( _T("���ι�ȣ�� �Է��ϼ���.") );
			return FALSE;
		}

		CESL_DataMgr *pDmAccession;
		pDmAccession = FindDM( _T("DM_BO_CAT_REG_0800_����") );
		if ( pDmAccession == NULL ) {
			AfxMessageBox( _T("DM_BO_CAT_REG_0800_���� error") );
			return FALSE;
		}
		CString sAccQuery;
		sAccQuery.Format( _T("ACCESSION_REC_NO = '%s' AND ACCESSION_REC_MAKE_YEAR = '%s'"), sAccessionNo, sYear );
		if ( pDmAccession->RefreshDataManager( sAccQuery ) < 0 ) return FALSE;
		m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDmAccession, _T("�Լ�����"), _T("�����Լ�����"));	
		INT iAccCount = pDmAccession->GetRowCount();

		CString strQuery;
		//strQuery.Format(_T("SELECT B.INPUT_DATE, B.REG_NO, S.MARC, B.PRICE, B.CLASS_NO, B.BOOK_CODE, B.VOL_CODE, B.COPY_CODE, B.SEPARATE_SHELF_CODE FROM BO_BOOK_TBL B, BO_SPECIES_TBL S WHERE B.ACCESSION_REC_KEY IN(SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE S.REC_KEY = B.SPECIES_KEY AND ACCESSION_REC_NO =%s AND ACCESSION_REC_MAKE_YEAR ='%s')")
		//				, sAccessionNo, sYear );
		strQuery.Format(_T("SELECT B.INPUT_DATE, B.REG_NO, S.MARC, B.PRICE, ")
						_T("B.CLASS_NO, B.BOOK_CODE, B.VOL_CODE, B.COPY_CODE, ")
						_T("B.SEPARATE_SHELF_CODE, B.BOOK_APPENDIX_FLAG, ")
						_T("B.VOL_TITLE, B.AUTHOR, B.VOL, B.PUBLISH_YEAR ")
						_T("FROM BO_BOOK_TBL B, BO_SPECIES_TBL S ")
						_T("WHERE B.ACCESSION_REC_KEY IN(SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE S.REC_KEY = B.SPECIES_KEY AND ACCESSION_REC_NO =%s AND ACCESSION_REC_MAKE_YEAR ='%s') ")
						_T("AND B.MANAGE_CODE = UDF_MANAGE_CODE ")
						_T("ORDER BY B.REG_NO")
						, sAccessionNo, sYear );

		dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		dm.RestructDataManager( strQuery );

		//SIREPORT ���� 
		CString strTmp;
		strTmp.Format(_T(" ���ι�ȣ : %s �⵵ %s ����"), sYear, sAccessionNo );
		pSIReportManager->SetTempVariable(_T("�˻�����"), strTmp);
	}


	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	CButton * pRegNo = (CButton *)GetDlgItem(IDC_rACCESSIONNO_PRINT);	
	if ( pRegNo->GetCheck() == 1 )
	{
		//��Ϲ�ȣ�� �˻�
		CString sRegNoStart, sRegNoEnd;
		GetControlData( _T("CM_BO_CAT_REG_0800"), _T("��Ϲ�ȣ����"), sRegNoStart );
		GetControlData( _T("CM_BO_CAT_REG_0800"), _T("��Ϲ�ȣ����"), sRegNoEnd );	

		if ( sRegNoStart == _T("") || sRegNoEnd == _T("") ) {
			AfxMessageBox( _T("��Ϲ�ȣ ������ �Է��ϼ���.") );
			return FALSE;
		}
		//=====================================================
		//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 		if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) {		
// 			if ( sRegNoStart.GetLength() > 2 ) {
// 				CString sRight;
// 				sRight.Format( _T("%010s") , sRegNoStart.Mid(2) );
// 				sRegNoStart = sRegNoStart.Left(2) + sRight;
// 			}
// 		}
// 		if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) {
// 			if ( sRegNoEnd.GetLength() > 2 ) {
// 				CString sRight;
// 				sRight.Format( _T("%010s") , sRegNoEnd.Mid(2) );
// 				sRegNoEnd = sRegNoEnd.Left(2) + sRight;
// 			}
// 		}

		if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) {		
			if ( sRegNoStart.GetLength() > nRegCodeLength ) {
				sRegNoStart = m_pInfo->MakeRegNo(sRegNoStart.Left(nRegCodeLength),sRegNoStart.Mid(nRegCodeLength));
			}
		}
		if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) {
			if ( sRegNoEnd.GetLength() > nRegCodeLength ) {
				sRegNoEnd = m_pInfo->MakeRegNo(sRegNoEnd.Left(nRegCodeLength),sRegNoEnd.Mid(nRegCodeLength));
			}
		}
		//=====================================================
		

		if ( sRegNoStart.Left(2) != sRegNoEnd.Left(2) ) {
			AfxMessageBox( _T("��ϱ����� ���ƾ� �մϴ�.") );
			return FALSE;
		}

		CString strQuery;
		//strQuery.Format( _T("SELECT B.INPUT_DATE, B.REG_NO, S.MARC, B.PRICE, B.CLASS_NO, B.BOOK_CODE, B.VOL_CODE, B.COPY_CODE, B.SEPARATE_SHELF_CODE FROM BO_BOOK_TBL B, BO_SPECIES_TBL S")
		//	_T(" WHERE B.SPECIES_KEY = S.REC_KEY AND B.REG_NO >= '%s' AND B.REG_NO <= '%s' "), sRegNoStart, sRegNoEnd );

		strQuery.Format( _T("SELECT B.INPUT_DATE, B.REG_NO, S.MARC, B.PRICE, ")
							_T("B.CLASS_NO, B.BOOK_CODE, B.VOL_CODE, B.COPY_CODE, ")
							_T("B.SEPARATE_SHELF_CODE, B.BOOK_APPENDIX_FLAG, ")
							_T("B.VOL_TITLE, B.AUTHOR, B.VOL, B.PUBLISH_YEAR ")
							_T("FROM BO_BOOK_TBL B, BO_SPECIES_TBL S")
							_T(" WHERE B.SPECIES_KEY = S.REC_KEY AND B.REG_NO >= '%s' AND B.REG_NO <= '%s' ")
							_T("AND B.MANAGE_CODE = UDF_MANAGE_COD E")
							_T("ORDER BY B.REG_NO "), sRegNoStart, sRegNoEnd );

		//�˻� ���ǿ� �´� �����͸� �����´�.
		dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		dm.RestructDataManager( strQuery );

		//SIREPORT ���� 
		CString strTmp;
		strTmp.Format(_T("��Ϲ�ȣ :  %s - %s "), sRegNoStart, sRegNoEnd );
		pSIReportManager->SetTempVariable(_T("�˻�����"), strTmp);

	}


	//��¿� DM�� ����
	pDM->FreeData();

	CString strTmpItem;
	CString strInputDate;
	CString strRegNo;
	CString strStreamMarc;
	CString strPrice;
	CString strClassNo;
	CString strBookCode;
	CString strVolCode;
	CString strCopyCode;
	CString strSeparateShelfCode;
	CString strVolTitle;
	CString strAuthor;
	CString strVol;
	CString strPublishYear;
	CString strBookAppendixFlag;


	INT nPageNum = 1;
	INT nSumPrice = 0;
	INT nPrintPerCount = 0;

	for ( INT idx = 0; idx < dm.GetRowCount() ; idx++ ) 
	{
		nPrintPerCount++;
		//0 �Լ�����
		//1 ��Ϲ�ȣ
		//2 ��ũ
		//3 ����
		strInputDate		= dm.GetCellData( idx, 0 );
		strRegNo			= dm.GetCellData( idx, 1 );
		strStreamMarc		= dm.GetCellData( idx, 2 );
		strPrice			= dm.GetCellData( idx, 3 );
		strClassNo			= dm.GetCellData( idx, 4 );
		strBookCode			= dm.GetCellData( idx, 5 );
		strVolCode			= dm.GetCellData( idx, 6 );
		strCopyCode			= dm.GetCellData( idx, 7 );
		strSeparateShelfCode= dm.GetCellData( idx, 8 );
		strBookAppendixFlag = dm.GetCellData( idx, 9 );
		strVolTitle			= dm.GetCellData( idx, 10 );
		strAuthor			= dm.GetCellData( idx, 11 );
		strVol				= dm.GetCellData( idx, 12 );
		strPublishYear		= dm.GetCellData( idx, 13 );
		
		ConversionPriceInfoToDesc ( strPrice, strPrice );

		pDM->InsertRow(-1);
		//�Լ����� �� ��Ϲ�ȣ
		strTmpItem = strInputDate + _T("\r\n") + strRegNo ;
		pDM->SetCellData(_T("�Լ����ڵ�Ϲ�ȣ"), strTmpItem , idx );

		// ��������
		CMarc marc;
		CString strMarc245Field;
		CString strMarc260Field;
		CString strMarc300Field;
		m_pInfo->pMarcMgr->Decoding( strStreamMarc, &marc );
		m_pInfo->pMarcMgr->GetField( &marc, _T("245"), strMarc245Field );
		m_pInfo->pMarcMgr->GetField( &marc, _T("260"), strMarc260Field );
		m_pInfo->pMarcMgr->GetField( &marc, _T("300"), strMarc300Field );

		strMarc245Field.Replace(SUBFIELD_CODE, '$');
		strMarc260Field.Replace(SUBFIELD_CODE, '$');
		strMarc300Field.Replace(SUBFIELD_CODE, '$');

		if ( strBookAppendixFlag==_T("A") )		
		{
			strTmpItem.Empty();
			// 2005.08.01 �ηϸ� ��½� ���������� �η������� ����
			if ( !strVolTitle.IsEmpty() )
			{
				strTmpItem+=_T("[�η�] ")+strVolTitle;
			}
			if ( !strAuthor.IsEmpty() )
			{
				strTmpItem+=_T(" /")+strAuthor;
			}
			if ( !strVol.IsEmpty() )
			{
				strTmpItem+=_T(" .")+strVol;
			}
			if ( !strPublishYear.IsEmpty() )
			{
				strTmpItem+=_T(" ")+strPublishYear;
			}						
		}
		else
		{
			if ( !strMarc245Field.IsEmpty() )
			{
				strMarc245Field = strMarc245Field.Mid(2);
				strMarc245Field.Replace(_T("$a"), _T(" "));
				strMarc245Field.Replace(_T("$d"), _T(" "));
				strMarc245Field.Replace(_T("$e"), _T(" "));
				strMarc245Field.Replace(_T("$b"), _T(" "));
				strMarc245Field.Replace(_T("$n"), _T(" "));
				strMarc245Field.Replace(_T("$p"), _T(" "));
				strMarc245Field.Replace(_T("$x"), _T(" "));
				strMarc245Field.Replace(_T("$c"), _T(" "));
				strMarc245Field.Replace(_T("$h"), _T(" "));
				strMarc245Field.Replace(_T("$6"), _T(" "));
			}
			
			if ( !strMarc260Field.IsEmpty() )
			{
				strMarc260Field = strMarc260Field.Mid(2);
				strMarc260Field.Replace(_T("$a"), _T(" "));
				strMarc260Field.Replace(_T("$b"), _T(" "));
				strMarc260Field.Replace(_T("$c"), _T(" "));
				strMarc260Field.Replace(_T("$d"), _T(" "));
				strMarc260Field.Replace(_T("$e"), _T(" "));
				strMarc260Field.Replace(_T("$f"), _T(" "));
				strMarc260Field.Replace(_T("$g"), _T(" "));
			}
			if ( !strMarc300Field.IsEmpty() )
			{
				strMarc300Field = strMarc300Field.Mid(2);
				strMarc300Field.Replace(_T("$a"), _T(" "));
				strMarc300Field.Replace(_T("$b"), _T(" "));
				strMarc300Field.Replace(_T("$c"), _T(" "));
				strMarc300Field.Replace(_T("$d"), _T(" "));
				strMarc300Field.Replace(_T("$e"), _T(" "));
			}
			
			strMarc245Field.Replace( FIELD_TERMINATOR , ' ');
			strMarc260Field.Replace( FIELD_TERMINATOR , ' ');
			strMarc300Field.Replace( FIELD_TERMINATOR , ' ');
			
			strTmpItem.Empty();
			strTmpItem = strMarc245Field + _T(" ") + strMarc260Field + _T(" ") + strMarc300Field;
		}

		pDM->SetCellData(_T("��������"), strTmpItem , idx );

		//����
		pDM->SetCellData(_T("����"), strPrice , idx );

		//û����ȣ (�з���ȣ����, 090$b����)
		strTmpItem.Empty();

		//��ġ�� ��������
		if ( !strSeparateShelfCode.IsEmpty() ) 
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("��ġ��ȣ"), strSeparateShelfCode, strSeparateShelfCode);

		strTmpItem = strTmpItem + strSeparateShelfCode ;	
		
		strTmpItem = strTmpItem + _T("\r\n") + strClassNo ;
		strTmpItem = strTmpItem + _T("\r\n") + strBookCode ;

		// 2005.04.11 ADD BY PDJ
		// û����ȣ ��½� ��å��ȣ �κ� �߰���.
		if ( strVolCode.IsEmpty() )
		{
			if ( !strCopyCode.IsEmpty() ) 
			strTmpItem = strTmpItem + _T("\r\nc.") + strCopyCode ;			
		}
		else
		{
			strTmpItem = strTmpItem + _T("\r\n") + strVolCode ;
			if ( !strCopyCode.IsEmpty() ) 
			strTmpItem = strTmpItem + _T("=c.") + strCopyCode ;
		}
		
		

		pDM->SetCellData(_T("û����ȣ"), strTmpItem , idx );

		strPrice.Replace( _T(","), _T(""));
		nSumPrice = nSumPrice + _ttoi(strPrice);

		//�������� ���� �ջ� �߰�
		if ( nPrintPerCount == 15 )
		{
			CString strTmpData, strTmpSumPrice;
			strTmpData.Format(_T("��%d"), nPageNum);
			strTmpSumPrice.Format(_T("%d"), nSumPrice);
			ConversionPriceInfoToDesc(strTmpSumPrice, strTmpSumPrice);
			pSIReportManager->SetTempVariable(strTmpData , strTmpSumPrice );
			nSumPrice = 0;
			nPageNum++;
			nPrintPerCount = 0;
		}

	}

	//�������� ���� �߰�
	if ( nPrintPerCount != 1 ) 
	{
		CString strTmpData, strTmpSumPrice;
		strTmpData.Format(_T("��%d"), nPageNum);
		strTmpSumPrice.Format(_T("%d"), nSumPrice);
		ConversionPriceInfoToDesc(strTmpSumPrice, strTmpSumPrice);
		pSIReportManager->SetTempVariable(strTmpData , strTmpSumPrice );
	} 

	
	pSIReportManager->SetDataMgr(0, pDM );
	pSIReportManager->MAKEPAGE_DrawWorkForm(-1);		// ��ũ�� Manual �ݺ� ���

	Print_SetLastPageLine();

	ids = pSIReportManager->Print2();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	

	return TRUE;
}


INT CBO_CAT_REG_0800::ConversionPriceInfoToDesc(CString strSrcData, CString &strDesData)
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
HBRUSH CBO_CAT_REG_0800::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

