// BO_CAT_DROP_INPUT.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_DROP_INPUT.h"
#include "BO_CAT_REG_PROGRESS_DLG.h"
#include "Ctype.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_DROP_INPUT dialog


CBO_CAT_DROP_INPUT::CBO_CAT_DROP_INPUT(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_DROP_INPUT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_Bo_ProgressDlg = NULL ;
}

CBO_CAT_DROP_INPUT::~CBO_CAT_DROP_INPUT()
{
}

VOID CBO_CAT_DROP_INPUT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_DROP_INPUT)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_DROP_INPUT, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_DROP_INPUT)
	ON_BN_CLICKED(IDC_btnDROP_SAVE, OnbtnDROPSAVE)
	ON_BN_CLICKED(IDC_btnDROP_CLOSE, OnbtnDROPCLOSE)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_DROP_INPUT message handlers
/*
VOID CBO_CAT_DROP_INPUT::OnbtnREGSEARCH() 
{
	// TODO: Add your control notification handler code here
	
}

VOID CBO_CAT_DROP_INPUT::OnbtnREGSEARCHCLOSE() 
{
	// TODO: Add your control notification handler code here
	
}
*/ //2008-05-15 ������ �ΰ��� �Լ��� ���� ���� �ʰ� ������ ���� - �ּ�ó����
BOOL CBO_CAT_DROP_INPUT::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("MNG_CAT_DROP_INPUT")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CButton *pMoCheck;
	pMoCheck = (CButton *)GetDlgItem(IDC_rMO_SEL);

	pMoCheck->SetCheck(1);


	
	CWnd *pRegCheck = GetDlgItem(IDC_REG_CHECK);

	pRegCheck->ShowWindow(SW_HIDE);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control

EFS_END
return FALSE;
}

VOID CBO_CAT_DROP_INPUT::OnbtnDROPSAVE() 
{
EFS_BEGIN

	// 1. �޼��� �Ѹ� (������Ϲ�ȣ�� �Է��Ͻðڽ��ϱ�? Y/N)
	INT ids = ::MessageBox( GetSafeHwnd() , _T("������Ϲ�ȣ�� �籸���Ͻðڽ��ϱ�?") ,
		_T("�� ��") ,MB_YESNO | MB_ICONQUESTION );

	if ( ids == IDNO ) return;

	//�������� �������� üũ�ϴ� Radio Button
	CButton *pMoCheck, *pSeCheck;
	CString strMode;
	pMoCheck = (CButton *)GetDlgItem(IDC_rMO_SEL);
	pSeCheck = (CButton *)GetDlgItem(IDC_rSE_SEL);
	
	CString sRegCode;
	CString sRegNoStart, sRegNoEnd;
	CESL_DataMgr *pDM = NULL;
	if ( pMoCheck->GetCheck() == 1 )
	{
		//������ ������ ���		
		pDM = FindDM(_T("DM_BO_CAT_DROP_INPUT"));
		if (pDM == NULL) return;
		
		strMode = _T("MO");
	}
	else if ( pSeCheck->GetCheck() == 1 )
	{
		//������ ������ ���	
		pDM = FindDM(_T("DM_SE_CAT_DROP_INPUT"));
		if (pDM == NULL) return;
		
		strMode = _T("SE");
	}

	// 2. GetControlData���� CM���� ���� ������. (��� ���� ������ ���� Edit Box, ��� ���е� �Բ� ������)
	GetControlData(_T("CM_MNG_CAT_DROP_INPUT"), _T("BB_��ϱ���_�ڵ�"), sRegCode);
	GetControlData(_T("CM_MNG_CAT_DROP_INPUT"), _T("������Ϲ�ȣ����"),  sRegNoStart);
	GetControlData(_T("CM_MNG_CAT_DROP_INPUT"), _T("������Ϲ�ȣ��"), sRegNoEnd);

	CString sRegNoStatrTmp = sRegNoStart;
	CString sRegNoEnDTmp = sRegNoEnd;

	INT iTmpStart, iTmpEnd;
	iTmpStart = _ttoi(sRegNoStart);
	iTmpEnd = _ttoi(sRegNoEnd);

	if ( sRegNoStart == _T("") || sRegNoEnd == _T("") ) {
		AfxMessageBox(_T("��Ϲ�ȣ�� ������ �Է��Ͽ� �ֽʽÿ�."));
		return;
	}
	
	//��������ȣ �˻�
	CString sQueryLastNumber;
	//++2008.10.14 UPDATE BY PWR {{++
	// ���սý��� ����
	sQueryLastNumber.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE='REG_NO' ")
							 _T("AND DIVISION_VALUE='%s' AND MANAGE_CODE='%s'")
							 , sRegCode, m_pInfo->MANAGE_CODE );
	//sQueryLastNumber.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE ='REG_NO' AND DIVISION_VALUE ='%s'"), sRegCode );
	//--2008.10.14 UPDATE BY PWR --}}
	CString sValue;	//���� ��Ϲ�ȣ
	pDM->GetOneValueQuery( sQueryLastNumber , sValue );

	if ( sValue == _T("")) 
	{
		CString strMessage;
		strMessage.Format(_T("��ϱ��� %s�� ���� ������ ��ȣ�� �����ϴ�."),sRegCode);
		AfxMessageBox(strMessage);

		return;
	}

	if ( _ttoi(sValue) < _ttoi(sRegNoStart)) 
	{
		CString strMessage;
		strMessage.Format(_T("��ϱ��� %s�� ���� ������ ��ȣ�� Ȯ���� �ּ���."),sRegCode);
		AfxMessageBox(strMessage);
		return;
	}

	if ( _ttoi(sValue) < _ttoi(sRegNoEnd) )
	{
		sRegNoEnd = sValue;
		iTmpEnd = _ttoi(sValue);
	}


	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	sRegNoStart = sRegCode + sRegNoStart;
	sRegNoEnd = sRegCode + sRegNoEnd;

	if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) {
		//===================================================================
		//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 		if ( sRegNoStart.GetLength() > 2 ) {			
// 			CString sRight;
// 			sRight.Format( _T("%010s") , sRegNoStart.Mid(nRegCodeLength) );
// 			sRegNoStart = sRegNoStart.Left(nRegCodeLength) + sRight;
// 		}

		if ( sRegNoStart.GetLength() > nRegCodeLength ) 
		{
			sRegNoStart = pParentMgr->m_pInfo->MakeRegNo(sRegNoStart.Left(nRegCodeLength),sRegNoStart.Mid(nRegCodeLength));
		}
		//===================================================================
	}
	if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) 
	{
		//===================================================================
		//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 		if ( sRegNoEnd.GetLength() > 2 ) 
// 		{
// 			CString sRight;
// 			sRight.Format( _T("%010s") , sRegNoEnd.Mid(2) );
// 			sRegNoEnd = sRegNoEnd.Left(2) + sRight;
// 		}

		if ( sRegNoStart.GetLength() > nRegCodeLength ) 
		{
			sRegNoEnd = pParentMgr->m_pInfo->MakeRegNo(sRegNoEnd.Left(nRegCodeLength),sRegNoEnd.Mid(nRegCodeLength));
		}
		//===================================================================

	}
	
	
	CString strQuery;
	
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	//++2008.10.14 UPDATE BY PWR {{++
	// ���սý��� ����
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����

// 	if ( strMode == _T("MO") )
// 		strQuery.Format(_T("SELECT REG_NO FROM BO_BOOK_TBL WHERE SUBSTR(REG_NO, 0, 2) != REG_CODE ")
// 						_T("AND REG_NO >='%s' AND REG_NO <='%s' AND MANAGE_CODE='%s'")
// 						, sRegNoStart, sRegNoEnd, m_pInfo->MANAGE_CODE );
// 	else if ( strMode == _T("SE") )
// 		strQuery.Format(_T("SELECT REG_NO FROM SE_BOOK_TBL WHERE SUBSTR(REG_NO, 0, 2) != REG_CODE ")
// 						_T("AND REG_NO >='%s' AND REG_NO <='%s' AND MANAGE_CODE='%s'")
// 						, sRegNoStart, sRegNoEnd, m_pInfo->MANAGE_CODE );
	if ( strMode == _T("MO") )
		strQuery.Format(_T("SELECT REG_NO FROM BO_BOOK_TBL WHERE SUBSTR(REG_NO, 0, %d) != REG_CODE ")
						_T("AND REG_NO >='%s' AND REG_NO <='%s' AND MANAGE_CODE='%s'")
						, nRegCodeLength, sRegNoStart, sRegNoEnd, m_pInfo->MANAGE_CODE );
	else if ( strMode == _T("SE") )
		strQuery.Format(_T("SELECT REG_NO FROM SE_BOOK_TBL WHERE SUBSTR(REG_NO, 0, %d) != REG_CODE ")
						_T("AND REG_NO >='%s' AND REG_NO <='%s' AND MANAGE_CODE='%s'")
						, nRegCodeLength, sRegNoStart, sRegNoEnd, m_pInfo->MANAGE_CODE );
	//=====================================================
	//if ( strMode == _T("MO") )
	//	strQuery.Format(_T("SELECT REG_NO FROM BO_BOOK_TBL WHERE Substr(reg_no, 0, 2) != reg_code AND reg_no >='%s' and reg_no <='%s'"),  sRegNoStart, sRegNoEnd);
	//else if ( strMode == _T("SE") )
	//	strQuery.Format(_T("SELECT REG_NO FROM SE_BOOK_TBL WHERE Substr(reg_no, 0, 2) != reg_code AND reg_no >='%s' and reg_no <='%s'"),  sRegNoStart, sRegNoEnd);
	//--2008.10.14 UPDATE BY PWR --}}

	dm.RestructDataManager(strQuery);

	CString strRegNo = _T("");
	for ( INT iCnt = 0 ; iCnt < dm.GetRowCount(); iCnt++)
	{
		strRegNo += dm.GetCellData(iCnt, 0);
		if ( dm.GetRowCount() != iCnt+1 )
			strRegNo += _T("\n");
	}

	if ( strRegNo != _T(""))
	{
		CString strMessage;
		strMessage.Format(_T("��Ϲ�ȣ\n%s\n�� ��ϱ����� Ȯ���ϼ���."), strRegNo);
		AfxMessageBox(strMessage);
		return;
	}

	CString strWhere;
	//++2008.10.14 UPDATE BY PWR {{++
	// ���սý��� ����
	strWhere.Format(_T("REG_CODE='%s' AND REG_NO>='%s' AND REG_NO<='%s' AND MANAGE_CODE='%s' ORDER BY REG_NO")
					, sRegCode, sRegNoStart, sRegNoEnd, m_pInfo->MANAGE_CODE );
	//strWhere.Format(_T("REG_CODE = '%s' AND REG_NO >= '%s' AND REG_NO <= '%s' ORDER BY reg_no"), sRegCode, sRegNoStart, sRegNoEnd);
	//--2008.10.14 UPDATE BY PWR --}}

	// ������ �߰�  (Order By �߰�)
	if (pDM->RefreshDataManager(strWhere) < 0)	return;
	
	if ( pDM->GetRowCount() < 0 ) {
		AfxMessageBox (_T("�˻��� ����� �����ϴ�."));
		return;
	}
	
	INT cnt = 0;
	INT iCount = iTmpEnd - iTmpStart;

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	m_Bo_ProgressDlg = new CBO_CAT_REG_PROGRESS_DLG(this);
	m_Bo_ProgressDlg->Create (this);
    m_Bo_ProgressDlg->CenterWindow () ;
    m_Bo_ProgressDlg->ShowWindow (SW_SHOW);
	m_Bo_ProgressDlg->UpdateWindow() ;
	
	m_Bo_ProgressDlg->SetTitle (_T("������Ϲ�ȣ�� �˻��ϰ� �ֽ��ϴ�."));
    m_Bo_ProgressDlg->m_ProgressCtrl->SetStep(1);
    m_Bo_ProgressDlg->m_ProgressCtrl->SetPos(0);
	m_Bo_ProgressDlg->m_ProgressCtrl->SetRange (0,iCount) ;   
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	ids = pDM->StartFrame();
	if ( ids < 0 ) return;
	
	//++2008.10.14 UPDATE BY PWR {{++
	// ���սý��� ����
	strWhere.Format(_T("DELETE FROM CO_DROP_ACCESSION_NO_TBL WHERE REG_CODE='%s' AND REG_NO>=%d AND REG_NO<=%d AND MANAGE_CODE='%s';")
					, sRegCode, iTmpStart, iTmpEnd, m_pInfo->MANAGE_CODE );
	//strWhere.Format(_T("DELETE FROM CO_DROP_ACCESSION_NO_TBL WHERE REG_CODE = '%s' AND REG_NO >= %d AND REG_NO <= %d;"), sRegCode, iTmpStart, iTmpEnd);
	//--2008.10.14 UPDATE BY PWR --}}
	ids = pDM->AddFrame(strWhere);
	if ( ids < 0 ) return;

	
	CString strTmpBookRegNo;
	CString strTmpComPare;
	CString sBookRecKey;
	BOOL bInsertDBCheck;
	INT iDbRegNoSearch = 0;
	
	for ( ; iTmpStart <= iTmpEnd ; iTmpStart++)
	{
		
		bInsertDBCheck = TRUE;

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		m_Bo_ProgressDlg->m_ProgressCtrl->StepIt () ;
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		//���ϱ� ���� ���ڿ� ����
		strTmpComPare.Format (_T("%d"), iTmpStart);
		CString sTmp;
		sTmp.Format( _T("%d"), iTmpStart );
		
		strTmpComPare = sRegCode + strTmpComPare;
		//=====================================================
		//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 		if ( strTmpComPare.GetLength() > 2 ) {
// 			CString sRight;
// 			sRight.Format( _T("%010s") , strTmpComPare.Mid(2) );
// 			strTmpComPare = strTmpComPare.Left(2) + sRight;
// 		}
		
		if ( strTmpComPare.GetLength() > nRegCodeLength ) 
		{
			strTmpComPare = pParentMgr->m_pInfo->MakeRegNo(strTmpComPare.Left(nRegCodeLength),strTmpComPare.Mid(nRegCodeLength));
		}

		//=====================================================
		
		
		//DB���� ������Ϲ�ȣ���� �˻�
		for ( INT iDbRegNoCount = iDbRegNoSearch; iDbRegNoCount < pDM->GetRowCount(); iDbRegNoCount++ )
		{
			pDM->GetCellData(iDbRegNoCount, 1, strTmpBookRegNo);

			if ( strTmpComPare == strTmpBookRegNo )
			{
				bInsertDBCheck = FALSE;
				iDbRegNoSearch = iDbRegNoCount;
				break;
			}

		}

		if ( bInsertDBCheck == TRUE ) 
		{
			//���ų� ������ ������Ϲ�ȣ Table�� �ִ´�.
			pDM->InitDBFieldData();
			ids = pDM->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), _T("ESL_SEQ.NEXTVAL"));
			if ( ids < 0 ) return;
			
			if ( strMode == _T("MO") )
				pDM->AddDBFieldData(_T("PUBLISH_FORM_CODE"), _T("STRING"), _T("MO"));
			else if ( strMode == _T("SE") )
				pDM->AddDBFieldData(_T("PUBLISH_FORM_CODE"), _T("STRING"), _T("SE"));
			
			ids = pDM->AddDBFieldData(_T("REG_CODE"), _T("STRING"), sRegCode);			
			if ( ids < 0 ) return;

			ids = pDM->AddDBFieldData(_T("REG_NO"), _T("NUMERIC"), sTmp);
			if ( ids < 0 ) return;

			ids = pDM->AddDBFieldData(_T("FIRST_WORK"), _T("NUMERIC"), GetWorkLogProc());
			if ( ids < 0 ) return;

			ids = pDM->AddDBFieldData(_T("LAST_WORK"), _T("NUMERIC"), GetWorkLogProc());
			if ( ids < 0 ) return;
			//++2008.10.14 ADD BY PWR {{++
			// ���սý��� ����
			ids = pDM->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), m_pInfo->MANAGE_CODE);
			if ( ids < 0 ) return;
			//--2008.10.14 ADD BY PWR --}}

			ids = pDM->MakeInsertFrame(_T("CO_DROP_ACCESSION_NO_TBL"));
			if ( ids < 0 ) return;
		}

	}

	m_Bo_ProgressDlg->SetTitle (_T("������ȣ�� DB�� �� ���� ���Դϴ�. ��� ��ٷ� �ֽʽÿ�."));

	ids = pDM->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("���������� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
		return;
	}

	ids = pDM->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("���������� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
		return;
	}
	
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if(m_Bo_ProgressDlg != NULL)
	{
		delete m_Bo_ProgressDlg ;
		m_Bo_ProgressDlg = NULL ;
	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	AfxMessageBox(_T("���������� ������ȣ�� �籸�� �Ǿ����ϴ�."));
	return;

EFS_END	
}

VOID CBO_CAT_DROP_INPUT::OnbtnDROPCLOSE() 
{
	// TODO: Add your control notification handler code here

	CDialog::OnCancel();
}

VOID CBO_CAT_DROP_INPUT::OnButton1() 
{
EFS_BEGIN

	// ��Ϲ�ȣ ��ȿ�� �˻�.
	CString sRegCode, sRegNoStart, sRegNoEnd;
	GetControlData(_T("CM_MNG_CAT_DROP_INPUT"), _T("BB_��ϱ���_�ڵ�"), sRegCode);
	GetControlData(_T("CM_MNG_CAT_DROP_INPUT"), _T("������Ϲ�ȣ����"),  sRegNoStart);
	GetControlData(_T("CM_MNG_CAT_DROP_INPUT"), _T("������Ϲ�ȣ��"), sRegNoEnd);

	if ( sRegCode == _T("") || sRegNoStart == _T("") || sRegNoEnd == _T("") )
	{
		AfxMessageBox(_T("���� ��� �Է��� �ּ���"));
		return ;
	}
	
	CButton *pMoCheck, *pSeCheck;
	CString strMode;
	pMoCheck = (CButton *)GetDlgItem(IDC_rMO_SEL);
	pSeCheck = (CButton *)GetDlgItem(IDC_rSE_SEL);
	
	CESL_DataMgr *pDM = NULL;
	if ( pMoCheck->GetCheck() == 1 )
	{
		//������ ������ ���		
		pDM = FindDM(_T("DM_BO_CAT_DROP_INPUT"));
		strMode = _T("MO");
	}
	else if ( pSeCheck->GetCheck() == 1 )
	{
		//������ ������ ���	
		pDM = FindDM(_T("DM_SE_CAT_DROP_INPUT"));
		strMode = _T("SE");
	}
	CString sRegNoList;
	
	CESL_DataMgr DM_SUB;
	DM_SUB.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //2003.10.28 ���α׷�����
	m_Bo_ProgressDlg = new CBO_CAT_REG_PROGRESS_DLG(this);
	m_Bo_ProgressDlg->Create (this);
    m_Bo_ProgressDlg->CenterWindow () ;
    m_Bo_ProgressDlg->ShowWindow (SW_SHOW);
	m_Bo_ProgressDlg->UpdateWindow() ;
	
	m_Bo_ProgressDlg->SetTitle (_T("å Table ���� ��Ϲ�ȣ üũ ���Դϴ�."));
    m_Bo_ProgressDlg->m_ProgressCtrl->SetStep(1);
    m_Bo_ProgressDlg->m_ProgressCtrl->SetPos(0);
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	CString sRegNoDbSearchStart, sRegNoDbSearchEnd, strTmpComPare;


	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	strTmpComPare = sRegCode + strTmpComPare;
// 	if ( strTmpComPare.GetLength() > 2 ) {
// 		CString sRight;
// 		sRight.Format( _T("%010s") , strTmpComPare.Mid(2) );
// 		strTmpComPare = strTmpComPare.Left(2) + sRight;
// 	}
// 
// 
// 	sRegNoStart.Format(_T("%10s") , sRegNoStart);
// 	sRegNoEnd.Format(_T("%10s") , sRegNoEnd);
// 
// 	sRegNoDbSearchStart = sRegCode + sRegNoStart;
// 	sRegNoDbSearchEnd = sRegCode + sRegNoStart;

	strTmpComPare = sRegCode + strTmpComPare;
	if ( strTmpComPare.GetLength() > nRegCodeLength ) 
	{
		strTmpComPare = pParentMgr->m_pInfo->MakeRegNo(strTmpComPare.Left(nRegCodeLength),strTmpComPare.Mid(nRegCodeLength));
	}
	
	sRegNoDbSearchStart = pParentMgr->m_pInfo->MakeRegNo(sRegCode,sRegNoStart);
	sRegNoStart = sRegNoDbSearchStart.Mid(nRegCodeLength);

	sRegNoDbSearchEnd = pParentMgr->m_pInfo->MakeRegNo(sRegCode,sRegNoStart);
	sRegNoEnd = sRegNoDbSearchEnd.Mid(nRegCodeLength);
	//=====================================================
	

	//++2008.10.14 UPDATE BY PWR {{++
	// ���սý��� ����
	CString sQuery_sub = _T("SELECT REG_NO FROM BO_BOOK_TBL WHERE REG_NO >= '") + sRegNoDbSearchStart + 
						 _T("' AND REG_NO <= '") + sRegNoDbSearchEnd + 
						 _T("' AND MANAGE_CODE='") + m_pInfo->MANAGE_CODE + _T("' ORDER BY REG_NO");
	//CString sQuery_sub = _T("SELECT REG_NO FROM BO_BOOK_TBL WHERE REG_NO >= '") + sRegNoDbSearchStart + _T("' AND REG_NO <= '") + sRegNoDbSearchEnd + _T("' ORDER BY REG_NO");
	//--2008.10.14 UPDATE BY PWR --}}
	DM_SUB.RestructDataManager(sQuery_sub);
	
	
	CString sRegNo;
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2003.11.01 ���α׷�����  
    m_Bo_ProgressDlg->m_ProgressCtrl->SetRange (0,DM_SUB.GetRowCount()) ;   
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	for ( INT j = 0; j < DM_SUB.GetRowCount(); j++)
	{	
		BOOL bCheckRegNo = TRUE;			
		sRegNo = DM_SUB.GetCellData(j,0);

		TCHAR ch;

		for (INT i = 3; i < sRegNo.GetLength(); i++) {	
			ch = sRegNo.GetAt(i);
			if (isdigit(ch)) {
			} else {
				bCheckRegNo = FALSE;
			}
			
		}

		if ( bCheckRegNo == FALSE ) {

			sRegNoList = sRegNoList + sRegNo + _T("\r\n");

		}
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        //2003.10.28 ���α׷����� �߰�
		m_Bo_ProgressDlg->m_ProgressCtrl->StepIt () ;
		
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		
	}

	CString sMessage;
	if ( sRegNoList == _T(""))
	{
		sMessage.Format(_T("��ϱ��� %s�� ���� ���� ��Ϲ�ȣ�� �����ϴ�."), sRegCode);
		AfxMessageBox(sMessage);
	}
	else 
		AfxMessageBox(sRegNoList);


	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2003.10.28 ���α׷����� �߰� 
	if(m_Bo_ProgressDlg != NULL)
	{
		delete m_Bo_ProgressDlg ;
		m_Bo_ProgressDlg = NULL ;
	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

EFS_END
return ;
}

HBRUSH CBO_CAT_DROP_INPUT::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
