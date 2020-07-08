// BO_CAT_SEARCH.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_SEARCH.h"
#include "CommonDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_SEARCH dialog
#include "..\..\..\����\K2UP_Common\MakeSearchData.h"

CBO_CAT_SEARCH::CBO_CAT_SEARCH(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_SEARCH)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	sDetailFlag = 0;
}

CBO_CAT_SEARCH::~CBO_CAT_SEARCH()
{

}



VOID CBO_CAT_SEARCH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_SEARCH)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_SEARCH, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_SEARCH)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bDetailNo, OnbDetailNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_SEARCH message handlers

BOOL CBO_CAT_SEARCH::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	// TODO: Add extra initialization here

	if (InitESL_Mgr(_T("SM_����_����_���_�˻�_�Ŵ���")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	
	if ( OpenMode == 0 ) //��� ��� �ڷ����
	{
		GetDlgItem(IDC_REG_INPUT_YEAR)->SetWindowText(_T("�����⵵"));

	}
	if ( OpenMode == 1 ) //������ �ڷ����
	{
		GetDlgItem(IDC_REG_NO_DESC)->ShowWindow(FALSE);
		GetDlgItem(IDC_GROUP_NO1)->ShowWindow(FALSE);

		GetDlgItem(IDC_REGDATA_STATUS)->ShowWindow(FALSE);
		GetDlgItem(IDC_cobWorkingStatus)->ShowWindow(FALSE);
		GetDlgItem(IDC_REG_INPUT_YEAR)->SetWindowText(_T("�����γ⵵"));

	}
	if (  OpenMode == 2) //������ �ڷ����
	{
		GetDlgItem(IDC_REG_NO_DESC)->ShowWindow(FALSE);
		GetDlgItem(IDC_GROUP_NO1)->ShowWindow(FALSE);

		GetDlgItem(IDC_REGDATA_STATUS)->ShowWindow(FALSE);
		GetDlgItem(IDC_cobWorkingStatus)->ShowWindow(FALSE);

		GetDlgItem(IDC_REGWORK_NO)->SetWindowText(_T("���ι�ȣ"));
		GetDlgItem(IDC_REG_INPUT_YEAR)->SetWindowText(_T("���γ⵵"));

	}

	CESL_DataMgr *pDM;

	if ( OpenMode == 0 ) //����ڷ����
		pDM = FindDM(_T("DM_����_����_���_����"));
	else if ( OpenMode == 1 ) // ������ �ڷ� ����
		pDM = FindDM(_T("DM_����_����_������_����"));
	else if ( OpenMode == 2 ) // ���� ���� ����
		pDM = FindDM(_T("DM_����_����_����_����"));

	CString m_Year;
	CTime mTime = pDM->GetDBTime();
	m_Year.Format(_T("%d"), mTime.GetYear());

	SetControlData(_T("CM_����_���_�˻�"), _T("BS_���Գ⵵"), m_Year);


	//���� ���� 
	OnbDetailNo(); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBO_CAT_SEARCH::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_CAT_SEARCH::OnbSEARCH() 
{
	// TODO: Add your control notification handler code here
	
	CString strQry,strWorkNo1,strWorkNo2,strpreGroup1,strpreGroup2,sACQUISIT_YEAR,sACQUISIT_TYPE;
	INT ids;

	CESL_DataMgr *pDM;
	if ( OpenMode == 0 ) //����ڷ����
		pDM = FindDM(_T("DM_����_����_���_����"));
	else if ( OpenMode == 1 ) // ������ �ڷ� ����
		pDM = FindDM(_T("DM_����_����_������_����"));
	else if ( OpenMode == 2 ) // ���� ���� ����
		pDM = FindDM(_T("DM_����_����_����_����"));

	//�⵵�� ���� �ش�.

	GetControlData(_T("CM_����_���_�˻�"), _T("BS_���Գ⵵")	  , sACQUISIT_YEAR);
	GetControlData(_T("CM_����_���_�˻�"), _T("BB_���Ա���_�ڵ�")	  , sACQUISIT_TYPE , -1 ,1);

	// ���� ��ȣ �� ������ ��ȣ�� ���ο� ���� �ش�.
	GetControlData(_T("CM_����_���_�˻�"), _T("������ȣ����"), strWorkNo1);
	GetControlData(_T("CM_����_���_�˻�"), _T("������ȣ��"), strWorkNo2);
	if(  OpenMode == 2) // ���� �������
	{
		GetControlData(_T("CM_����_���_�˻�"), _T("���ι�ȣ_����"), strpreGroup1);
		strpreGroup2=_T("");
	}
	else
	{
		GetControlData(_T("CM_����_���_�˻�"), _T("�����ι�ȣ����"), strpreGroup1);
		GetControlData(_T("CM_����_���_�˻�"), _T("�����ι�ȣ��"), strpreGroup2);		 
	}

	if( (strWorkNo1 != _T(""))&&(strWorkNo2 != _T("")) )
	{
		strWorkNo1 = strWorkNo1 + _T(" - ") +strWorkNo2;
	}
	if( (strpreGroup1 != _T(""))&&(strpreGroup2 != _T("")) )
	{
		strpreGroup1 = strpreGroup1 + _T(" - ") +strpreGroup2;
	}

	if ( OpenMode == 0 ) //��� �ڷ� ���� 
	{
		SetControlData(_T("CM_����_����_���_����"), _T("������ȣ"), strWorkNo1);
		SetControlData(_T("CM_����_����_���_����"), _T("�����ι�ȣ"), strpreGroup1);
		SetControlData(_T("CM_����_����_���_����"), _T("�⵵"), sACQUISIT_YEAR);
		SetControlData(_T("CM_����_����_���_����"), _T("�Լ�����"), sACQUISIT_TYPE);
	}
	else if ( OpenMode == 1 ) //������ ���� 
	{
		SetControlData(_T("CM_����_����_������_����"), _T("������ȣ"), strWorkNo1);
		SetControlData(_T("CM_����_����_������_����"), _T("�����ι�ȣ"), strpreGroup1);
		SetControlData(_T("CM_����_����_������_����"), _T("�⵵"), sACQUISIT_YEAR);
		SetControlData(_T("CM_����_����_������_����"), _T("�Լ�����"), sACQUISIT_TYPE);

	}
	else if ( OpenMode == 2 ) //���δ��� ���� 
	{
		SetControlData(_T("CM_����_����_����_����"), _T("���ι�ȣ"), strWorkNo1);
		SetControlData(_T("CM_����_����_����_����"), _T("�⵵"), sACQUISIT_YEAR);
		SetControlData(_T("CM_����_����_����_����"), _T("�Լ�����"), sACQUISIT_TYPE);

	}




	strQry = MakeSelectQry();
	if( strQry == _T("")) return;

	ids = pDM->RefreshDataManager(strQry);
	if (ids < 0)
	{
		AfxMessageBox(_T("RefreshDataManager �� Ȯ���� �ּ���"));
		return;
	}


	CESL_ControlMgr *pCM;
	CESL_Grid *pGrid_MainGrid_All;

	if ( OpenMode == 0 ) //����ڷ� ����
	{
		pCM = FindCM(_T("CM_����_����_���_����"));
		pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
		pGrid_MainGrid_All->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	}	
	else if ( OpenMode == 1 ) //������ �ڷ� ����
	{
		pCM = FindCM(_T("CM_����_����_������_����"));
		pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_����_����_������_����"), _T("MAIN_GRID"));
		pGrid_MainGrid_All->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	}
	else if ( OpenMode == 2 ) //���δ��� ����
	{
		pCM = FindCM(_T("CM_����_����_����_����"));
		pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_����_����_����_����"), _T("MAIN_GRID"));
		pGrid_MainGrid_All->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	}

	//�׸��� �ݹ� �Լ� ���� 

	pCM->AllControlDisplay();

}

INT CBO_CAT_SEARCH::GridDisplayEventProc(CString &strGridAlias, INT nRow, 
INT nCol, CString &strDMFieldAlias, CString &str)
{
EFS_BEGIN
    if (strGridAlias == _T("MAIN_GRID")) {
        if (strDMFieldAlias == _T("BB_�ڷ����_�ڵ�")) {
            if ( str == _T("BOR111N")) {
				str = _T("����ΰ��ڷ�");
			}
			else if ( str == _T("BOR112N") ) {
				str = _T("����ڷ�");
			}
			else if ( str == _T("BOR113O") ) {
				str = _T("�������ڷ�");
			}
        }
    }
	return 0;


EFS_END
return -1;

}


CString CBO_CAT_SEARCH::MakeSelectQry()
{

	CString strQry; 
	CString sACQUISIT_CODE, sACQUISIT_YEAR;
	CString sAccessionNoStart, sAccessionNoEnd;
	CString sRegNoStart, sRegNoEnd;
	CString sTransferDataStart, sTransferDataEnd;
	CString SEQ_NO1,SEQ_NO2,preGroup1,preGroup2;
	INT sAccessionFlag=0;

	//�ڷ����
	CString sWorkStatus;
	GetControlData( _T("CM_����_���_�˻�"), _T("�ڷ����"), sWorkStatus );

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	
	if (OpenMode == 0) //����ڷ���� 
	{
		if ( sWorkStatus == _T("��ü") ) {
			strQry = strQry + _T("  B.WORKING_STATUS >= 'BOR111N' AND B.WORKING_STATUS <= 'BOR112N'");
		}
		else if ( sWorkStatus == _T("����ΰ��ڷ�") ) {
			strQry = strQry + _T(" B.WORKING_STATUS = 'BOR111N'");
		}
		else if ( sWorkStatus == _T("����ڷ�") ) {
			strQry = strQry + _T(" B.WORKING_STATUS = 'BOR112N'");
		}
	}
	else if (OpenMode == 1) // �������ڷ����
	{
		strQry = strQry + _T(" B.WORKING_STATUS = 'BOR113O'");
	}
	else if (OpenMode == 2) //�����ڷ����
	{
		strQry = strQry + _T(" B.WORKING_STATUS = 'BOR112N'");
	}

	//���� �⵵ ���� ���� ���  
	GetControlData(_T("CM_����_���_�˻�"), _T("BS_���Գ⵵")	  , sACQUISIT_YEAR);
	sACQUISIT_YEAR.TrimLeft();
	sACQUISIT_YEAR.TrimRight();


	if (OpenMode == 2)
	{
		//������ȣ
		GetControlData(_T("CM_����_���_�˻�"), _T("������ȣ����"), SEQ_NO1);
		GetControlData(_T("CM_����_���_�˻�"), _T("������ȣ��")  , SEQ_NO2);

		SEQ_NO1.TrimLeft();
		SEQ_NO1.TrimRight();
		SEQ_NO2.TrimLeft();
		SEQ_NO2.TrimRight();

		if ( (SEQ_NO1 != _T(""))&&(SEQ_NO2 != _T("")) )
		{
			strQry = strQry + _T(" AND a.accession_rec_no >= ") + SEQ_NO1 + _T(" AND a.accession_rec_no <= ") + SEQ_NO2 ;
			if ( sACQUISIT_YEAR != _T("") ) {
				strQry = strQry + _T(" AND A.ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("'");
			}

		}
		else if ( (SEQ_NO1 != _T(""))&&(SEQ_NO2 == _T("")) )
		{
			strQry = strQry + _T(" AND a.accession_rec_no = ") + SEQ_NO1;
			sAccessionFlag = 1;
			
			if ( sACQUISIT_YEAR != _T("") ) {
				strQry = strQry + _T(" AND A. ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("'");
			}

		}
		else if ( (SEQ_NO1 == _T(""))&&(SEQ_NO2 != _T("")) )
		{

			strQry = strQry + _T(" AND a.accession_rec_no = ") + SEQ_NO2;
			sAccessionFlag = 1;
			
			if ( sACQUISIT_YEAR != _T("") ) {
				strQry = strQry + _T(" AND A. ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("'");
			}

		}
		


	}
	else
	{
		//������ȣ
		GetControlData(_T("CM_����_���_�˻�"), _T("������ȣ����"), SEQ_NO1);
		GetControlData(_T("CM_����_���_�˻�"), _T("������ȣ��")  , SEQ_NO2);

		SEQ_NO1.TrimLeft();
		SEQ_NO1.TrimRight();
		SEQ_NO2.TrimLeft();
		SEQ_NO2.TrimRight();

		if ( (SEQ_NO1 != _T(""))&&(SEQ_NO2 != _T("")) )
		{
			strQry = strQry + _T(" AND B.SEQ_NO >= ") + SEQ_NO1 + _T(" AND B.SEQ_NO <= ") + SEQ_NO2 ;

		}
		else if ( (SEQ_NO1 != _T(""))&&(SEQ_NO2 == _T("")) )
		{
			strQry = strQry + _T(" AND B.SEQ_NO >= ") + SEQ_NO1;

		}
		else if ( (SEQ_NO1 == _T(""))&&(SEQ_NO2 != _T("")) )
		{
			strQry = strQry + _T(" AND B.SEQ_NO >= ") + SEQ_NO2;

		}


		//���ι�ȣ 
		GetControlData(_T("CM_����_���_�˻�"), _T("���ι�ȣ_����")   , sAccessionNoStart); 
		sAccessionNoStart.TrimLeft();
		sAccessionNoStart.TrimRight();
		if (sAccessionNoStart != _T(""))
		{
			strQry = strQry + _T(" AND a.accession_rec_no = ")+sAccessionNoStart;
			sAccessionFlag = 1;
			
			if ( sACQUISIT_YEAR != _T("") ) {
				strQry = strQry + _T(" AND A. ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("'");
			}

		}
		else
		{

			if (OpenMode == 1)
			{
				if ( sACQUISIT_YEAR != _T("") ) {
					strQry = strQry + _T(" AND A.ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("'");
				}
			}
			else
			{
				if ( sACQUISIT_YEAR != _T("") ) {
					strQry = strQry + _T(" AND B.ACQ_YEAR = '") + sACQUISIT_YEAR + _T("'");
				}
			}

		}

	}
	//���� ���� �ڵ� & ���Գ⵵ 
	GetControlData(_T("CM_����_���_�˻�"), _T("BB_���Ա���_�ڵ�"), sACQUISIT_CODE);
	
	sACQUISIT_CODE.TrimLeft();
	sACQUISIT_CODE.TrimRight();


	if ( sACQUISIT_CODE != _T("") ) {
		strQry = strQry + _T(" AND B.ACQ_CODE = '") + sACQUISIT_CODE + _T("'");
	}

	

	//��Ϲ�ȣ
	GetControlData(_T("CM_����_���_�˻�"), _T("BB_��Ϲ�ȣ_����"), sRegNoStart);
	GetControlData(_T("CM_����_���_�˻�"), _T("BB_��Ϲ�ȣ_����"), sRegNoEnd);
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
	//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
	if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) {
		if ( sRegNoStart.GetLength() > nRegCodeLength ) {
			sRegNoStart = m_pInfo->MakeRegNo(sRegNoStart.Left(nRegCodeLength), sRegNoStart.Mid(nRegCodeLength) );
		}
	}

	if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) {
		if ( sRegNoEnd.GetLength() > nRegCodeLength ) {
			sRegNoEnd = m_pInfo->MakeRegNo(sRegNoEnd.Left(nRegCodeLength), sRegNoEnd.Mid(nRegCodeLength) );
		}
	}
	//=====================================================
	

	if ( sRegNoStart != _T("") && sRegNoEnd == _T("")) {
		strQry = _T("B.REG_NO = '") + sRegNoStart  + _T("' AND ") + strQry;
	}
	if ( sRegNoEnd != _T("") && sRegNoStart == _T("") ) {
		AfxMessageBox( _T("���� ��Ϲ�ȣ�� �Է� �ϼ���."));
		return _T("");
	}
	if ( sRegNoStart != _T("") && sRegNoEnd != _T("") ) {
		strQry = _T("B.REG_NO >= '") + sRegNoStart + _T("' AND B.REG_NO <= '") + sRegNoEnd + _T("' AND ") + strQry;
	}

	//������ ��ȣ
	GetControlData(_T("CM_����_���_�˻�"), _T("�����ι�ȣ����"), preGroup1);
	GetControlData(_T("CM_����_���_�˻�"), _T("�����ι�ȣ��")  , preGroup2);

	if(preGroup1 != _T("") && preGroup2 != _T("")){

		strQry = strQry + _T(" AND a.accession_rec_no >= ")+preGroup1+_T(" AND a.accession_rec_no <= ")+preGroup2;


		if(sAccessionFlag == 1)
		{
			AfxMessageBox(_T("���ο� �����δ� ��ÿ� �˻� �Ҽ� �����ϴ�!"));
			return _T("");
		}


	}
	if(preGroup1 != _T("") && preGroup2 == _T("")){

		strQry = strQry + _T(" AND a.accession_rec_no = ")+preGroup1;


		if(sAccessionFlag == 1)
		{
			AfxMessageBox(_T("���ο� �����δ� ��ÿ� �˻� �Ҽ� �����ϴ�!"));
			return _T("");
		}


	}
	if(preGroup1 == _T("") && preGroup2 != _T("")){

		strQry = strQry + _T(" AND a.accession_rec_no = ")+preGroup2;


		if(sAccessionFlag == 1)
		{
			AfxMessageBox(_T("���ο� �����δ� ��ÿ� �˻� �Ҽ� �����ϴ�!"));
			return _T("");
		}


	}


	//�ΰ� ó����  

	GetControlData(_T("CM_����_���_�˻�"), _T("���ó����_����"), sTransferDataStart);
	GetControlData(_T("CM_����_���_�˻�"), _T("���ó����_����"), sTransferDataEnd);

	if ( sTransferDataStart != _T("") && sTransferDataEnd == _T("") ) {
		strQry = strQry + _T(" AND B.CAT_TRANSFER_DATE = '") + sTransferDataStart + _T("'");
	}

	if ( sTransferDataStart == _T("") && sTransferDataEnd != _T("") ) {
		AfxMessageBox ( _T("���� �ΰ�ó������ �Է��ϼ���.") );
		return _T("");
	}

	if ( sTransferDataStart != _T("") && sTransferDataEnd != _T("") ) {
		strQry = strQry + _T(" AND B.CAT_TRANSFER_DATE >= '") + sTransferDataStart + _T("' AND B.CAT_TRANSFER_DATE <= '") + sTransferDataEnd + _T("'");
	}

	//�� �˻��� ��� 
	if ( sDetailFlag == 1)
	{
		CString sTitle, sAuthor;
		CString sFORM_CODE, sMEDIA_CODE, sMAT_CODE, sUSE_OBJ_CODE, sMANAGE_CODE;
		CString sWhereTitle;

		CMakeSearchData *pMakeData = NULL;

		//��ǥ��  
		GetControlData(_T("CM_����_���_�˻�"), _T("��ǥ��"), sTitle);	
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
		GetControlData(_T("CM_����_���_�˻�"), _T("������"), sAuthor);
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
		GetControlData(_T("CM_����_���_�˻�"), _T("BS_���ı���_�ڵ�"), sFORM_CODE);
		GetControlData(_T("CM_����_���_�˻�"), _T("BS_��ü����_�ڵ�"), sMEDIA_CODE);
		GetControlData(_T("CM_����_���_�˻�"), _T("BS_�ڷᱸ��_�ڵ�"), sMAT_CODE);
		GetControlData(_T("CM_����_���_�˻�"), _T("BS_�̿��󱸺�_�ڵ�"), sUSE_OBJ_CODE);
		GetControlData(_T("CM_����_���_�˻�"), _T("BS_��������_�ڵ�"), sMANAGE_CODE);

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

		//�۾���
		CString sWorker;
		GetControlData(_T("CM_����_���_�˻�"), _T("BS_�۾���"), sWorker);
		if ( sWorker != _T("") ) {
//			strQry = strQry + _T(" AND B.WORKER = '") + sWorker + _T("'");
		}	

		
	}
	

	return strQry;

}

VOID CBO_CAT_SEARCH::OnbDetailNo() 
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

BOOL CBO_CAT_SEARCH::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}
