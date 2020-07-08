// BO_CAT_REG_0800.cpp : implementation file
//

#include "stdafx.h"
//#include _T("bo_cat_reg.h")
#include "BO_CAT_REG_0800.h"
#include "..\..\..\ESL\SIReportMgr\SIReportManager.h"

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
	//{{AFX_DATA_INIT(CBO_CAT_REG_0800)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
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
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_REG_0800, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REG_0800)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0800 message handlers

BOOL CBO_CAT_REG_0800::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SE_CAT_REG_0800")) < 0)	
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CButton * pPart;
	pPart = (CButton *)GetDlgItem(IDC_rSHELF_PRINT);
	pPart->SetCheck(1);

	//2005-11-11 Ȳ���� �߰�
	CString str90;
	GetManageValue(_T("��Ÿ"), _T("����"), _T("�����"), _T("�����"), str90);
	CButton * pA4;
	if( str90 == _T("Y"))
	{
		pA4 = (CButton *)GetDlgItem(IDC_rA4_RANDSCAPE);
		pA4->SetCheck(1);
	}
	else
	{
		pA4 = (CButton *)GetDlgItem(IDC_rA4);
		pA4->SetCheck(1);
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_REG_0800::OnOK() 
{
EFS_BEGIN

	CButton * pPart;
	pPart = (CButton *)GetDlgItem(IDC_rSHELF_PRINT);
	CString strIS64, str90;
	GetManageValue( _T("��Ÿ") , _T("����") , _T("����") , _T("����") , strIS64 );
	GetManageValue( _T("��Ÿ"), _T("����"), _T("�����"), _T("�����"), str90 );

	CString sYear, sAccessionNo;	
	if ( pPart->GetCheck() == 1 ) {		
		GetControlData( _T("CM_SE_CAT_REG_0800"), _T("�ۼ��⵵"), sYear );
		if ( sYear == _T("") ) {
			AfxMessageBox( _T("�ۼ��⵵�� �Է��ϼ���.") );
			return;
		}
		GetControlData( _T("CM_SE_CAT_REG_0800"), _T("���ι�ȣ"), sAccessionNo );
		if ( sAccessionNo == _T("") ) {
			AfxMessageBox( _T("���ι�ȣ�� �Է��ϼ���.") );
			return;
		}
	

		//��������DM
		CESL_DataMgr *pDmAccession;
		pDmAccession = FindDM( _T("DM_SE_CAT_REG_0800_����") );
		if ( pDmAccession == NULL ) {
			AfxMessageBox( _T("DM_BO_CAT_REG_0800_���� error") );
			return;
		}
		CString sAccQuery;
		sAccQuery.Format( _T("ACCESSION_REC_NO = '%s' AND ACCESSION_REC_MAKE_YEAR = '%s'"), sAccessionNo, sYear );
		if ( pDmAccession->RefreshDataManager( sAccQuery ) < 0 ) return;
		m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDmAccession, _T("�Լ�����"), _T("�����Լ�����"));	
		INT iAccCount = pDmAccession->GetRowCount();


		CESL_DataMgr *pDM;
		pDM = FindDM( _T("DM_SE_CAT_REG_0800_01") );
		if ( pDM == NULL ) {
			AfxMessageBox( _T("DM_BO_CAT_PROC_0800_01") );
			return;
		}

		CString sQuery;
		//sQuery.Format( _T("REG_NO >= '%s' AND REG_NO <= '%s'"), sRegNoStart, sRegNoEnd );
		sQuery.Format( _T("B.ACCESSION_REC_KEY IN(SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO =%s AND ACCESSION_REC_MAKE_YEAR='%s')"), sAccessionNo, sYear );
		if ( pDM->RefreshDataManager( sQuery ) < 0 ) return;

		INT iRowCount = pDM->GetRowCount();
		if ( iRowCount < 1 ) {
			AfxMessageBox( _T("�˻��� �����Ͱ� �����ϴ�.") );
			return;
		}
		
		// 2005-11-10 Ȳ����: ����� ����/�а��� ����/å�� 
		CString strQuery, strSMA, strBMA, strSBR, strBBR, strSTOTAL, strBTOTAL;
		if (str90 == _T("Y"))
		{
//			INT nStotal, nBtotal;
			//���� ����
			strQuery.Format(_T("SELECT COUNT(DISTINCT(SPECIES_KEY)) FROM SE_BOOK_TBL B WHERE "));
			strQuery += sQuery;
			strQuery += _T(" AND MANAGE_CODE = 'MA'");
			pDM->GetOneValueQuery(strQuery, strSMA);
			
			//���� å��
			strQuery.Format(_T("SELECT COUNT(DISTINCT(REG_NO)) FROM SE_BOOK_TBL B WHERE "));
			strQuery += sQuery;
			strQuery += _T(" AND MANAGE_CODE = 'MA'");
			pDM->GetOneValueQuery(strQuery, strBMA);
			
			//�а� ����
			strQuery.Format(_T("SELECT COUNT(DISTINCT(SPECIES_KEY)) FROM SE_BOOK_TBL B WHERE "));
			strQuery += sQuery;
			strQuery += _T(" AND MANAGE_CODE = 'BR'");
			pDM->GetOneValueQuery(strQuery, strSBR);

			//�а� å��
			strQuery.Format(_T("SELECT COUNT(DISTINCT(REG_NO)) FROM SE_BOOK_TBL B WHERE "));
			strQuery += sQuery;
			strQuery += _T(" AND MANAGE_CODE = 'BR'");
			pDM->GetOneValueQuery(strQuery, strBBR);

			//�հ� ����
			strQuery.Format(_T("SELECT COUNT(DISTINCT(SPECIES_KEY)) FROM SE_BOOK_TBL B WHERE "));
			strQuery += sQuery;
			strQuery += _T(" AND MANAGE_CODE IN ('BR','MA') ");
			pDM->GetOneValueQuery(strQuery, strSTOTAL);
			
			//�հ� å��
			strQuery.Format(_T("SELECT COUNT(DISTINCT(REG_NO)) FROM SE_BOOK_TBL B WHERE "));
			strQuery += sQuery;
			strQuery += _T(" AND MANAGE_CODE IN ('BR','MA') ");
			pDM->GetOneValueQuery(strQuery, strBTOTAL);

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
			
			//�հ�
/*			nStotal = _ttoi(strSMA) + _ttoi(strSBR);
			nBtotal = _ttoi(strBMA) + _ttoi(strBBR);
			
			strSTOTAL.Format(_T("%d"), nStotal);
			strBTOTAL.Format(_T("%d"), nBtotal);*/
		}

		CESL_DataMgr *pDM_0800_02;
		pDM_0800_02 = FindDM( _T("DM_SE_CAT_REG_0800_02") );
		if ( pDM_0800_02 == NULL ) {
			AfxMessageBox( _T("DM_BO_CAT_REG_0800_01") );
			return;
		}

		CString sRegCode, sRegCodeAll;
		CString sMinRegNo, sMaxRegNo;
		CString sRegCodeDes;

		//��ϱ��� ��ġ��
		for ( INT i = 1 ; i <= iRowCount ; i++ ) {
			sRegCode = pDM->GetCellData( _T("BB_��ϱ���"), i-1 );
			if ( sRegCode != _T("") ) {
				if ( sRegCodeAll != _T("") ) {
					sRegCodeAll = sRegCodeAll + _T(", '") + sRegCode + _T("'");
				}
				else {
					sRegCodeAll = _T("'") + sRegCode + _T("'");
				}
			}
			/*
			//����
			m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("��ϱ���"), _T("BB_��ϱ���"));
			CString sRegCodeDesTmp = pDM->GetCellData( _T("BB_��ϱ���"), i-1 );
			if ( sRegCodeDes != _T("") ) {
				sRegCodeDes = sRegCodeDes + _T(", ") + sRegCodeDesTmp;
			}
			else {
				sRegCodeDes = sRegCodeDesTmp;
			}
			*/
		}		
		for ( i = 1 ; i <= iRowCount ; i++ ) {						
			//����
			m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("��ϱ���"), _T("BB_��ϱ���"));
			CString sRegCodeDesTmp = pDM->GetCellData( _T("BB_��ϱ���"), i-1 );
			if ( sRegCodeDes != _T("") ) {
				sRegCodeDes = sRegCodeDes + _T(", ") + sRegCodeDesTmp;
			}
			else {
				sRegCodeDes = sRegCodeDesTmp;
			}			
		}
		/*
		//��ϱ��к���
		for ( INT i = 1 ; i <= iRowCount ; i++ ) {			
			sRegCode = pDM->GetCellData( _T("BB_��ϱ���"), i-1 );
			sQuery.Format( _T("I.REC_KEY = B.SPECIES_KEY AND B.REG_CODE = '%s' AND ACCESSION_REC_KEY IN(SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO =%s)"), sRegCode, sAccessionNo );
			if ( pDM_0800_02->RefreshDataManager( sQuery ) < 0 ) return;
			m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("��ϱ���"), _T("BB_��ϱ���"));
			CString sRegCodeDesTmp = pDM->GetCellData( _T("BB_��ϱ���"), i-1 );
			if ( sRegCodeDes != _T("") ) {
				sRegCodeDes = sRegCodeDes + _T(", ") + sRegCodeDesTmp;
			}
			else {
				sRegCodeDes = sRegCodeDesTmp;
			}
			INT iRowCount_02 = pDM_0800_02->GetRowCount();		
			INT iSwReg = 0;
			for ( INT j = 0 ; j < iRowCount_02 ; j ++ ) {
				//���»��� �����
				CString sPage = pDM_0800_02->GetCellData( _T("BB_�����"), j );
				CString sPhysical = pDM_0800_02->GetCellData( _T("BB_������Ư��"), j );
				CString sBookSize = pDM_0800_02->GetCellData( _T("BB_ũ��"), j );
				CString sForm = sPage + _T(" : ") + sPhysical + _T(" ; ") + sBookSize;
				pDM_0800_02->SetCellData( _T("���»���"), sForm, j );
				//û����ȣ �����
				CString sShelfCode = pDM_0800_02->GetCellData( _T("IBS_û��_��ġ��ȣ_�ڵ�"), j );
				CString sClassNo = pDM_0800_02->GetCellData( _T("IBS_û��_�з���ȣ"), j );
				CString sBookCode = pDM_0800_02->GetCellData( _T("IBS_û��_������ȣ"), j );
				CString sVolCode = pDM_0800_02->GetCellData( _T("IBS_û��_��å��ȣ"), j );
				CString sCallNo = sShelfCode + _T(" ") + sClassNo + _T("-") + sBookCode + _T(" ") + sVolCode;
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
			}
		}
		*/
		//��ϱ��� �������
		sQuery.Format( _T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = V.SPECIES_KEY AND V.REC_KEY = B.VOL_KEY AND B.BINDED_BOOK_YN = 'N' AND B.REG_CODE IN (%s) AND B.ACCESSION_REC_KEY IN(SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO =%s AND ACCESSION_REC_MAKE_YEAR='%s') ORDER BY B.REG_CODE, B.REG_NO"), sRegCodeAll, sAccessionNo, sYear );
		if ( pDM_0800_02->RefreshDataManager( sQuery ) < 0 ) return;
		/*
		m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("��ϱ���"), _T("BB_��ϱ���"));
		CString sRegCodeDesTmp = pDM->GetCellData( _T("BB_��ϱ���"), i-1 );
		if ( sRegCodeDes != _T("") ) {
			sRegCodeDes = sRegCodeDes + _T(", ") + sRegCodeDesTmp;
		}
		else {
			sRegCodeDes = sRegCodeDesTmp;
		}
		*/		
		INT iRowCount_02 = pDM_0800_02->GetRowCount();		
		INT iSwReg = 0;
		for ( INT j = 0 ; j < iRowCount_02 ; j ++ ) {
			//���»��� �����
			CString sPage = pDM_0800_02->GetCellData( _T("BB_�����"), j );
			CString sPhysical = pDM_0800_02->GetCellData( _T("BB_������Ư��"), j );
			CString sBookSize = pDM_0800_02->GetCellData( _T("BB_ũ��"), j );
			//CString sForm = sPage + _T(" : ") + sPhysical + _T(" ; ") + sBookSize;
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
			pDM_0800_02->SetCellData( _T("���»���"), sForm, j );
			//û����ȣ �����
			CString sShelfCode = pDM_0800_02->GetCellData( _T("IBS_û��_��ġ��ȣ_�ڵ�"), j );
			CString sClassNo = pDM_0800_02->GetCellData( _T("IBS_û��_�з���ȣ"), j );
			CString sBookCode = pDM_0800_02->GetCellData( _T("IBS_û��_������ȣ"), j );
			CString sVolCode = pDM_0800_02->GetCellData( _T("IBS_û��_��å��ȣ"), j );
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
			CString s245n;
			m_pInfo->pMarcMgr->GetItem( &marc, _T("245$n"), s245n );
			if ( s245n != _T("") ) {
				sTitleInfo = sTitleInfo + _T(" . ") + s245n;
			}
			CString s245p;
			m_pInfo->pMarcMgr->GetItem( &marc, _T("245$p"), s245p );
			if ( s245p != _T("") ) {
				sTitleInfo = sTitleInfo + _T(" , ") + s245p;
			}
			CString s245d;
			m_pInfo->pMarcMgr->GetItem( &marc, _T("245$d"), s245d );
			if ( s245d != _T("") ) {
				sTitleInfo = sTitleInfo + _T(" / ") + s245d;
			}			
			CString s250a;
			m_pInfo->pMarcMgr->GetItem( &marc, _T("250$a"), s250a );
			if ( s250a != _T("") ) {
				sTitleInfo = sTitleInfo + _T(" .- ") + s250a;
			}
			//2006.01.24 hjy ���� : 440 �켱
/*			//�Ѽ�ǥ��,�Ѽ�����
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
			}*/
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

			//2005-11-10 Ȳ���� : ������ ��ȣå��ȣ�� �ڿ� �־��ش�.
			if( str90 == _T("Y"))
			{
				CString strVolCode, strOtherVolTitle;
				pDM_0800_02->GetCellData( _T("VB_��ȣ��"), j, strVolCode );
				pDM_0800_02->GetCellData( _T("VB_��������ȣ��"), j, strOtherVolTitle );
				if(!strVolCode.IsEmpty())
				{
					sTitleInfo += _T(" [");
					sTitleInfo += strVolCode;
					sTitleInfo += _T(" : ");
					sTitleInfo += strOtherVolTitle;
					sTitleInfo += _T(" ]");
				}

				//2005-12-01 Ȳ���� : �����ȣ�߰�
				CString strControlNo;
				strControlNo.Empty();
				m_pInfo->pMarcMgr->GetItem( &marc, _T("001*"), strControlNo);
				pDM_0800_02->SetCellData( _T("IBS_�����ȣ"),strControlNo, j);
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
			//���� ǥ�� �������� �����
			NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
			TCHAR szOUT[20];
			::GetNumberFormat(NULL, NULL, sPrice, &nFmt, szOUT, 20);
			sPrice = _T("\\") + sPrice;
			pDM_0800_02->SetCellData( _T("BB_����"), sPrice, j );

			// 2005-11-10 Ȳ���� : ����� �������� ����, ������ ����
			if(str90 == _T("Y"))
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
				strQuery.Format(_T("SELECT D.DONATOR_NAME ")
							  _T("FROM SE_DONATE_MANAGE_TBL D, SE_BOOK_TBL B ")
							  _T("WHERE B.MANAGE_CODE = UDF_MANAGE_CODE AND B.SPECIES_KEY = D.SPECIES_KEY ")
							  _T("AND B.SPECIES_KEY = '%s'")
							  , strTmp);
				pDM_0800_02->GetOneValueQuery(strQuery,strDonator);
				pDM_0800_02->SetCellData(_T("������"),strDonator,j);
			}

		}

		//����ϱ�
		CSIReportManager *pSIReportManager = new CSIReportManager(this);
		pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
		if (!pSIReportManager) {
			AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
			return;
		}
		INT ids;
		pSIReportManager->SetCONNECTION_INFO(pDM->CONNECTION_INFO);

		//A4,B4 ����
		CButton * pB4;
		pB4 = (CButton *)GetDlgItem(IDC_rB4);
		if ( pB4->GetCheck() == 1 ) {	
			if ( strIS64 == _T("Y") ) ids = pSIReportManager->GetSIReportFileFromDB(_T("SE�������B4_����"));
			else if( str90 == _T("Y") ) ids = pSIReportManager->GetSIReportFileFromDB(_T("SE�������B4_�����"));
			else ids = pSIReportManager->GetSIReportFileFromDB(_T("SE�������B4"));
		}
		else if (static_cast<CButton*>(GetDlgItem(IDC_rA4_RANDSCAPE))->GetCheck() == 1 ) 
		{
			if ( strIS64 == _T("Y") ) ids = pSIReportManager->GetSIReportFileFromDB(_T("SE�������_����_����"));
			else if( str90 == _T("Y") ) ids = pSIReportManager->GetSIReportFileFromDB(_T("SE�������_����_�����"));
			else ids = pSIReportManager->GetSIReportFileFromDB(_T("SE�������_����"));
		}
		else {
			if ( strIS64 == _T("Y") ) ids = pSIReportManager->GetSIReportFileFromDB(_T("SE�������_����"));
			else ids = pSIReportManager->GetSIReportFileFromDB(_T("SE�������"));
		}
		
		if (ids < 0) {
			AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
			return;
		}
		
		//pSIReportManager->SetTempVariable(_T("%BARCODE���ڵ�����%"), _T("����(�̾���)������"));
		CString sAccAcqCode = pDmAccession->GetCellData( _T("�����Լ�����"), 0 );
		CString sPrice = pDM->GetCellData( _T("����"), 0 );
		CString sRegNo = sMinRegNo + _T(" ����  ") + sMaxRegNo + _T(" ����");
		//å�� ���ϱ�		
		CString sBookCount = pDM->GetCellData( _T("å��"), 0 );
		INT iBookCount = pDM->GetRowCount();
		INT iTotBookCount = 0;
		for ( i = 0 ; i < iBookCount ; i ++ ) {
			sBookCount = pDM->GetCellData( _T("å��"), i );
			INT iBookCount = _ttoi( sBookCount );
			iTotBookCount = iTotBookCount + iBookCount;
		}
		sBookCount.Format( _T("%d"), iTotBookCount );

		CString sRegDate = pDmAccession->GetCellData( _T("���ε������"), 0 );
		pSIReportManager->SetTempVariable(_T("���Ա���"), sAccAcqCode);
		pSIReportManager->SetTempVariable(_T("���Գ⵵"), sYear);
		//����
		//���� ǥ�� �������� �����
		NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
		TCHAR szOUT[20];
		::GetNumberFormat(NULL, NULL, sPrice, &nFmt, szOUT, 20);
		CString sPriceFont;		
		sPriceFont.Format( _T("%15s"), szOUT );
		pSIReportManager->SetTempVariable(_T("����"), sPriceFont);
		pSIReportManager->SetTempVariable(_T("��ϱ���"), sRegCodeDes);
		pSIReportManager->SetTempVariable(_T("���ι�ȣ"), sAccessionNo);
		pSIReportManager->SetTempVariable(_T("��Ϲ�ȣ"), sRegNo);
		
		//2005-11-10 Ȳ���� : ����/���� ����
		if(str90 == _T("Y"))
		{
			pSIReportManager->SetTempVariable(_T("����"), strSMA);
			pSIReportManager->SetTempVariable(_T("å��"), strBMA);
			pSIReportManager->SetTempVariable(_T("�а�����"), strSBR);
			pSIReportManager->SetTempVariable(_T("�а�å��"), strBBR);
			pSIReportManager->SetTempVariable(_T("���հ�"), strSTOTAL);
			pSIReportManager->SetTempVariable(_T("å�հ�"), strBTOTAL);
		}

		//2006-01-10 Ȳ���� ���� 
		//������¹��� ����,���� ������ ����.
		CString sClassName,sRegistrant ;
        strQuery.Format (_T("SELECT CLASS_NAME FROM CO_ACCESSION_REC_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND  ACCESSION_REC_NO = %s AND ACCESSION_REC_MAKE_YEAR = '%s'"), sAccessionNo, sYear);
        pDmAccession->GetOneValueQuery (strQuery,sClassName);
        strQuery.Format (_T("SELECT REGISTRANT FROM CO_ACCESSION_REC_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND  ACCESSION_REC_NO = %s AND ACCESSION_REC_MAKE_YEAR = '%s'"),sAccessionNo, sYear );
		pDmAccession->GetOneValueQuery (strQuery,sRegistrant); 
        
		pSIReportManager->SetTempVariable(_T("����"), sClassName);
		pSIReportManager->SetTempVariable(_T("����"), sRegistrant);

		//å�� �� ���߱�
		CString sBookFont;
		sBookFont.Format( _T("%15s"), sBookCount );
		pSIReportManager->SetTempVariable(_T("å��"), sBookFont);		
		pSIReportManager->SetTempVariable(_T("�������"), sRegDate);

		// 2005.08.01 ADD BY PDJ
		// ���� �߰���
		CString sRemark = pDmAccession->GetCellData( _T("���"), 0 );
		pSIReportManager->SetTempVariable(_T("���"), sRemark);
		//�������� ��������
		/*
		CString sPath = _T("..\\CFG\\LocalLibraryInfo.cfg");
		FILE *fp;
		fp = _tfopen( sPath , _T("rb") );
		//fp = _tfopen( sFilePath , _T("rb") );
		
		if ( fp == NULL ) {
			AfxMessageBox( _T("..\\CFG\\LocalLibraryInfo.cfg ������ �����ϴ�.") );
			return;
		}
		TCHAR cLine[256];
		CString sLibraryName;
		while( _fgetts( cLine, 256, fp ) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			if ( sLine != _T("") ) {
				CString sTmp = sLine.Left(10);
				if ( sLine.Left(10) == _T("�������̸�") ) {
					INT iPos = sLine.Find(_T("="));
					sLibraryName = sLine.Mid( iPos+2 );
					sLibraryName = sLibraryName.Left( sLibraryName.GetLength() - 2 );
				}
			}
		}
		*/
		CString sLibraryName;
		//===================================================
		//2010.09.03 UPDATE BY PJW : MANAGE_CODE�� ���������� �����´�.
		//sQuery = _T("SELECT VALUE FROM MN_LIBINFO_TBL WHERE VALUE_NAME ='��������'");
		sQuery = _T("SELECT LIB_NAME FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE");
		//===================================================
		
		pDM_0800_02->GetOneValueQuery( sQuery , sLibraryName );
		//SetControlData( _T("CM_BO_CAT_PROC_0203_01"), _T("��������"), sLibraryName );
		sLibraryName = sLibraryName + _T("�� �������ڷ�� �Ʒ��� ���� �����.");

		pSIReportManager->SetTempVariable(_T("LIB_NAME"), sLibraryName);

	/*	
		m_pDM->InitDataMgrRef(1);
		m_pDM->SetDataMgrRef(0, _T("BB_��Ϲ�ȣ"), _T("REG_NO"), _T("str"), _T(""), _T(""));
		m_pDM->TBL_NAME = _T("BO_BOOK_TBL");

		ids = m_pDM->RefreshDataManager(_T("rownum <= 50"));
		CString str;
		m_pDM->GetCellData(_T("BB_��Ϲ�ȣ"), 0, str);
		INT nRowCount = m_pDM->GetRowCount();
	*/
		/*
		pSIReportManager->SetDataMgr(0, pDM_0800_02);
		ids = pSIReportManager->Print();
		if (pSIReportManager) {
			delete pSIReportManager;
			pSIReportManager = NULL;
		}
		*/

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

	//��Ϲ�ȣ���� ���
	CButton * pACCESSIONNO_PRINT;
	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	pACCESSIONNO_PRINT = (CButton *)GetDlgItem(IDC_rACCESSIONNO_PRINT);	
	if ( pACCESSIONNO_PRINT->GetCheck() == 1 ) {
		CString sRegNoStart, sRegNoEnd;		
		GetControlData( _T("CM_SE_CAT_REG_0800"), _T("��Ϲ�ȣ����"), sRegNoStart );
		GetControlData( _T("CM_SE_CAT_REG_0800"), _T("��Ϲ�ȣ����"), sRegNoEnd );	

		if ( sRegNoStart == _T("") || sRegNoEnd == _T("") ) {
			AfxMessageBox( _T("��Ϲ�ȣ ������ �Է��ϼ���.") );
			return;
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
// 
// 		if ( sRegNoStart.Left(2) != sRegNoEnd.Left(2) ) {
// 			AfxMessageBox( _T("��ϱ����� ���ƾ� �մϴ�.") );
// 			return;
// 		}
		//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
		if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) {		
			if ( sRegNoStart.GetLength() > nRegCodeLength ) {
				sRegNoStart = pParentMgr->m_pInfo->MakeRegNo(sRegNoStart.Left(nRegCodeLength),sRegNoStart.Mid(nRegCodeLength));
			}
		}
		if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) {
			if ( sRegNoEnd.GetLength() > nRegCodeLength ) {
				sRegNoEnd = pParentMgr->m_pInfo->MakeRegNo(sRegNoEnd.Left(nRegCodeLength),sRegNoEnd.Mid(nRegCodeLength));
			}
		}

		if ( sRegNoStart.Left(nRegCodeLength) != sRegNoEnd.Left(nRegCodeLength) ) {
			AfxMessageBox( _T("��ϱ����� ���ƾ� �մϴ�.") );
			return;
		}
		//=====================================================

		

		CESL_DataMgr *pDM;
		pDM = FindDM( _T("DM_SE_CAT_REG_0800_01") );
		if ( pDM == NULL ) {
			AfxMessageBox( _T("DM_SE_CAT_PROC_0800_01") );
			return;
		}

		CString sQuery;
		//=====================================================
		//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 		sQuery.Format( _T("REG_NO >= '%s' AND REG_NO <= '%s' AND REG_CODE = '%s'"), sRegNoStart, sRegNoEnd, sRegNoStart.Left(2) );

		sQuery.Format( _T("REG_NO >= '%s' AND REG_NO <= '%s' AND REG_CODE = '%s'"), sRegNoStart, sRegNoEnd, sRegNoStart.Left(nRegCodeLength) );
		//=====================================================
		
		if ( pDM->RefreshDataManager( sQuery ) < 0 ) return;

		INT iRowCount = pDM->GetRowCount();
		if ( iRowCount < 1 ) {
			AfxMessageBox( _T("�˻��� �����Ͱ� �����ϴ�.") );
			return;
		}
		
		//2005-11-10 Ȳ���� �߰�
		CString strQuery, strSMA, strBMA, strSBR, strBBR, strSTOTAL, strBTOTAL;
		if(str90 == _T("Y"))
		{
//			INT nStotal, nBtotal;

			//���� ����
			strQuery.Format(_T("SELECT COUNT(DISTINCT(SPECIES_KEY)) FROM SE_BOOK_TBL WHERE "));
			strQuery += sQuery;
			strQuery += _T(" AND MANAGE_CODE = 'MA'");
			pDM->GetOneValueQuery(strQuery, strSMA);
			
			//���� å��
			strQuery.Format(_T("SELECT COUNT(REG_NO) FROM SE_BOOK_TBL WHERE "));
			strQuery += sQuery;
			strQuery += _T(" AND MANAGE_CODE = 'MA'");
			pDM->GetOneValueQuery(strQuery, strBMA);
			
			//�а� ����
			strQuery.Format(_T("SELECT COUNT(DISTINCT(SPECIES_KEY)) FROM SE_BOOK_TBL WHERE "));
			strQuery += sQuery;
			strQuery += _T(" AND MANAGE_CODE = 'BR'");
			pDM->GetOneValueQuery(strQuery, strSBR);

			//�а� å��
			strQuery.Format(_T("SELECT COUNT(REG_NO) FROM SE_BOOK_TBL WHERE "));
			strQuery += sQuery;
			strQuery += _T(" AND MANAGE_CODE = 'BR'");
			pDM->GetOneValueQuery(strQuery, strBBR);

			//�հ� ����
			strQuery.Format(_T("SELECT COUNT(DISTINCT(SPECIES_KEY)) FROM SE_BOOK_TBL WHERE "));
			strQuery += sQuery;
			strQuery += _T(" AND MANAGE_CODE IN ('BR','MA') ");
			pDM->GetOneValueQuery(strQuery, strSTOTAL);
			
			//�հ� å��
			strQuery.Format(_T("SELECT COUNT(DISTINCT(REG_NO)) FROM SE_BOOK_TBL WHERE "));
			strQuery += sQuery;
			strQuery += _T(" AND MANAGE_CODE IN ('BR','MA') ");
			pDM->GetOneValueQuery(strQuery, strBTOTAL);

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
			
			//�հ�
/*			nStotal = _ttoi(strSMA) + _ttoi(strSBR);
			nBtotal = _ttoi(strBMA) + _ttoi(strBBR);
			
			strSTOTAL.Format(_T("%d"), nStotal);
			strBTOTAL.Format(_T("%d"), nBtotal);*/
		}

		CESL_DataMgr *pDM_0800_02;
		pDM_0800_02 = FindDM( _T("DM_SE_CAT_REG_0800_02") );
		if ( pDM_0800_02 == NULL ) {
			AfxMessageBox( _T("DM_SE_CAT_REG_0800_02") );
			return;
		}

		CString sRegCode;
		CString sMinRegNo, sMaxRegNo;
		CString sRegCodeDes;
		INT iBookCount;	//å��
		for ( INT i = 1 ; i <= iRowCount ; i++ ) {
			//CString sRegCode = pDM->GetCellData( _T("BB_��ϱ���"), i-1 );
			sRegCode = pDM->GetCellData( _T("BB_��ϱ���"), i-1 );
			sQuery.Format( _T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = V.SPECIES_KEY AND V.REC_KEY = B.VOL_KEY AND B.BINDED_BOOK_YN = 'N' AND B.REG_NO >= '%s' AND B.REG_NO <= '%s' AND B.REG_CODE = '%s' AND B.WORKING_STATUS > 'SEC' ORDER BY B.REG_CODE, B.REG_NO"), sRegNoStart, sRegNoEnd, sRegCode );
			//sQuery.Format( _T("I.REC_KEY = B.SPECIES_KEY AND B.REG_CODE = '%s' AND ACCESSION_REC_KEY IN(SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO =%s)"), sRegCode, sAccessionNo );
			if ( pDM_0800_02->RefreshDataManager( sQuery ) < 0 ) return;
			//m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("��ϱ���"), _T("BB_��ϱ���"));
			sRegCodeDes = pDM->GetCellData( _T("BB_��ϱ���"), i-1 );
			INT iRowCount_02 = pDM_0800_02->GetRowCount();
			//å��
			iBookCount = iRowCount_02;
			INT iSwReg = 0;
			for ( INT j = 0 ; j < iRowCount_02 ; j ++ ) {
				//���»��� �����
				CString sPage = pDM_0800_02->GetCellData( _T("BB_�����"), j );
				CString sPhysical = pDM_0800_02->GetCellData( _T("BB_������Ư��"), j );
				CString sBookSize = pDM_0800_02->GetCellData( _T("BB_ũ��"), j );
				//CString sForm = sPage + _T(" : ") + sPhysical + _T(" ; ") + sBookSize;
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
				pDM_0800_02->SetCellData( _T("���»���"), sForm, j );
				//û����ȣ �����
				CString sShelfCode = pDM_0800_02->GetCellData( _T("IBS_û��_��ġ��ȣ_�ڵ�"), j );
				CString sClassNo = pDM_0800_02->GetCellData( _T("IBS_û��_�з���ȣ"), j );
				CString sBookCode = pDM_0800_02->GetCellData( _T("IBS_û��_������ȣ"), j );
				CString sVolCode = pDM_0800_02->GetCellData( _T("IBS_û��_��å��ȣ"), j );
				CString sDupCode = pDM_0800_02->GetCellData( _T("BB_������ȣ"), j );
				//CString sCallNo = sShelfCode + _T(" ") + sClassNo + _T("-") + sBookCode + _T("=") + sVolCode;
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
				CString s245n;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$n"), s245n );
				if ( s245n != _T("") ) {
					sTitleInfo = sTitleInfo + _T(" . ") + s245n;
				}
				CString s245p;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$p"), s245p );
				if ( s245p != _T("") ) {
					sTitleInfo = sTitleInfo + _T(" , ") + s245p;
				}
				CString s245d;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$d"), s245d );
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
				}*/
				//2006.01.24 hjy ���� : 440 �켱
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
				//2005-11-10 Ȳ���� : ������ ��ȣå��ȣ�� �ڿ� �־��ش�.
				if( str90 == _T("Y"))
				{
					CString strVolCode, strOtherVolTitle;
					pDM_0800_02->GetCellData( _T("VB_��ȣ��"), j, strVolCode );
					pDM_0800_02->GetCellData( _T("VB_��������ȣ��"), j, strOtherVolTitle );
					if(!strVolCode.IsEmpty())
					{
						sTitleInfo += _T(" [");
						sTitleInfo += strVolCode;
						sTitleInfo += _T(" : ");
						sTitleInfo += strOtherVolTitle;
						sTitleInfo += _T(" ]");
					}

					//2005-12-01 Ȳ���� : �����ȣ�߰�
					CString strControlNo;
					strControlNo.Empty();
					m_pInfo->pMarcMgr->GetItem( &marc, _T("001*"), strControlNo);
					pDM_0800_02->SetCellData( _T("IBS_�����ȣ"),strControlNo, j);
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
				//���� ǥ�� �������� �����
				NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
				TCHAR szOUT[20];
				::GetNumberFormat(NULL, NULL, sPrice, &nFmt, szOUT, 20);
				sPrice = _T("\\") + sPrice;
				pDM_0800_02->SetCellData( _T("BB_����"), sPrice, j );

				//2005-11-10 Ȳ���� : ����� ��������, ������ ��
				CString strQuery;
				if(str90 == _T("Y"))
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
					strQuery.Format(_T("SELECT D.DONATOR_NAME ")
								  _T("FROM SE_DONATE_MANAGE_TBL D, SE_BOOK_TBL B ")
								  _T("WHERE B.SPECIES_KEY = D.SPECIES_KEY ")
								  _T("AND B.SPECIES_KEY = '%s' ")
								  _T("AND B.MANAGE_CODE = UDF_MANAGE_CODE")			
								  , strTmp);
					pDM_0800_02->GetOneValueQuery(strQuery,strDonator);
					pDM_0800_02->SetCellData(_T("������"),strDonator,j);
				}
			}			
		}
		m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("��ϱ���"), _T("BB_��ϱ���"));
		//m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("��ϱ���"), sRegCodeDes);
		sRegCodeDes = pDM->GetCellData( _T("BB_��ϱ���"), 0 );
		

		//����ϱ�
		CSIReportManager *pSIReportManager = new CSIReportManager(this);
		pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
		if (!pSIReportManager) {
			AfxMessageBox(_T("Report Manager�� �������� ���Ͽ����ϴ�."));
			return;
		}
		INT ids;
		
		pSIReportManager->SetCONNECTION_INFO(pDM->CONNECTION_INFO);
		//ids = pSIReportManager->GetSIReportFileFromDB(_T("�������"));
		//A4,B4 ����
		CButton * pB4;
		pB4 = (CButton *)GetDlgItem(IDC_rB4);
		if ( pB4->GetCheck() == 1 ) {		
			if ( strIS64 == _T("Y") ) ids = pSIReportManager->GetSIReportFileFromDB(_T("SE�������B4_����"));
			else if( str90 == _T("Y") ) ids = pSIReportManager->GetSIReportFileFromDB(_T("SE�������B4_�����"));
			else ids = pSIReportManager->GetSIReportFileFromDB(_T("SE�������B4"));
		}
		else if (static_cast<CButton*>(GetDlgItem(IDC_rA4_RANDSCAPE))->GetCheck() == 1 ) 
		{
			if ( strIS64 == _T("Y") ) ids = pSIReportManager->GetSIReportFileFromDB(_T("SE�������_����_����"));
			else if( str90 == _T("Y") ) ids = pSIReportManager->GetSIReportFileFromDB(_T("SE�������_����_�����"));
			else ids = pSIReportManager->GetSIReportFileFromDB(_T("SE�������_����"));
		}
		else {
			if ( strIS64 == _T("Y") ) ids = pSIReportManager->GetSIReportFileFromDB(_T("SE�������_����"));
			else ids = pSIReportManager->GetSIReportFileFromDB(_T("SE�������"));
		}
		
		if (ids < 0) {
			AfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
			return;
		}
		
		//pSIReportManager->SetTempVariable(_T("%BARCODE���ڵ�����%"), _T("����(�̾���)������"));
		//CString sAccAcqCode = pDmAccession->GetCellData( _T("�����Լ�����"), 0 );
		CString sPrice = pDM->GetCellData( _T("����"), 0 );
		CString sRegNo = sMinRegNo + _T(" ����  ") + sMaxRegNo + _T(" ����");
		//CString sBookCount = pDM->GetCellData( _T("å��"), 0 );
		CString sBookCount;
		sBookCount.Format( _T("%d"), iBookCount );
		//CString sRegDate = pDmAccession->GetCellData( _T("���ε������"), 0 );
		//pSIReportManager->SetTempVariable(_T("���Ա���"), sAccAcqCode);
		//pSIReportManager->SetTempVariable(_T("���Գ⵵"), sYear);

		//�������� ��������
		CString sPath = _T("..\\CFG\\LocalLibraryInfo.cfg");
		FILE *fp;
		fp = _tfopen( sPath , _T("rb") );
		//fp = _tfopen( sFilePath , _T("rb") );
		
		if ( fp == NULL ) {
			AfxMessageBox( _T("..\\CFG\\LocalLibraryInfo.cfg ������ �����ϴ�.") );
			return;
		}
		
		//++++++++++++++++++++++++++++++++++++++++++
		//Unicode�϶� 2byte�ǳʶ��
		FILE* fpUni = fp;
	
		if( 0xFF == fgetc(fpUni) ) 
		{
			if( 0xFE != fgetc(fpUni) )
			{
				fseek(fpUni, 0, SEEK_SET);
			}
		}
		else
		{
			fseek(fpUni, 0, SEEK_SET);
		}
		//------------------------------------------
		
		TCHAR cLine[256];
		CString sLibraryName;
		while( _fgetts( cLine, 256, fp ) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			if ( sLine != _T("") ) {
				CString sTmp = sLine.Left(10);
				if ( sLine.Left(10) == _T("�������̸�") ) {
					INT iPos = sLine.Find(_T("="));
					sLibraryName = sLine.Mid( iPos+2 );
					sLibraryName = sLibraryName.Left( sLibraryName.GetLength() - 2 );
				}
			}
		}
		fclose(fp);
		
		//SetControlData( _T("CM_BO_CAT_PROC_0203_01"), _T("��������"), sLibraryName );
		sLibraryName = sLibraryName + _T("�� �������ڷ�� �Ʒ��� ���� �����.");

		pSIReportManager->SetTempVariable(_T("LIB_NAME"), sLibraryName);
		//����
		//���� ǥ�� �������� �����
		NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
		TCHAR szOUT[20];
		::GetNumberFormat(NULL, NULL, sPrice, &nFmt, szOUT, 20);
		CString sPriceFont;		
		sPriceFont.Format( _T("%15s"), szOUT );
		pSIReportManager->SetTempVariable(_T("����"), sPriceFont);
		//pSIReportManager->SetTempVariable(_T("����"), sPrice);
		pSIReportManager->SetTempVariable(_T("��ϱ���"), sRegCodeDes);
		//pSIReportManager->SetTempVariable(_T("���ι�ȣ"), sAccessionNo);
		pSIReportManager->SetTempVariable(_T("��Ϲ�ȣ"), sRegNo);
		
		//2005-11-10 Ȳ���� : ����/���� ����
		if(str90 == _T("Y"))
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
		//pSIReportManager->SetTempVariable(_T("�������"), sRegDate);
		
		// 2005.08.01 ADD BY PDJ
		// ���� �߰���
		CString sRemark = pDM->GetCellData( _T("���"), 0 );
		pSIReportManager->SetTempVariable(_T("���"), sRemark);
	/*	
		m_pDM->InitDataMgrRef(1);
		m_pDM->SetDataMgrRef(0, _T("BB_��Ϲ�ȣ"), _T("REG_NO"), _T("str"), _T(""), _T(""));
		m_pDM->TBL_NAME = _T("BO_BOOK_TBL");

		ids = m_pDM->RefreshDataManager(_T("rownum <= 50"));
		CString str;
		m_pDM->GetCellData(_T("BB_��Ϲ�ȣ"), 0, str);
		INT nRowCount = m_pDM->GetRowCount();
	*/
		/*
		pSIReportManager->SetDataMgr(0, pDM_0800_02);
		ids = pSIReportManager->Print();
		if (pSIReportManager) {
			delete pSIReportManager;
			pSIReportManager = NULL;
		}
		*/

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
	
	//CDialog::OnOK();

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

HBRUSH CBO_CAT_REG_0800::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
