// BO_CAT_REG_0400.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_REG_0400.h"
#include "BO_REG_MGR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0400 dialog


CBO_CAT_REG_0400::CBO_CAT_REG_0400(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_REG_0400)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_REG_0400::~CBO_CAT_REG_0400()
{	
	
} 

VOID CBO_CAT_REG_0400::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REG_0400)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_REG_0400, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REG_0400)
	ON_BN_CLICKED(IDC_bACCESSIONNO_GIVE, OnbACCESSIONNOGIVE)
	ON_BN_CLICKED(IDC_rCanRegNo, OnrCanRegNo)
	ON_BN_CLICKED(IDC_rAcqBack, OnrAccCan)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0400 message handlers

BOOL CBO_CAT_REG_0400::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_REG_0400")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	if(OpenMode == 0)
	{
		m_DM_ALIAS = _T("DM_����_����_���_����");
		m_CM_ALIAS = _T("CM_����_����_���_����");
	}
	else if(OpenMode == 2)
	{
		m_DM_ALIAS = _T("DM_����_����_����_����"); 
		m_CM_ALIAS = _T("CM_����_����_����_����");
	}
	else 
	{
		AfxMessageBox(_T("OpenMode Error"));
		return FALSE;

	}



	INT iReturn = Display(0);

	//������ �ȵǾ� ������ ȭ���� ����� �ʴ´�.
	if ( iReturn == -1 ) {
		m_bInitDlg = FALSE;
		return FALSE;
	}

	CButton * pCanRegNo;
	pCanRegNo=(CButton *)GetDlgItem(IDC_rCanRegNo);
	pCanRegNo->SetCheck(1);	

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

BOOL CBO_CAT_REG_0400::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

INT CBO_CAT_REG_0400::Display(INT nDir)
{
EFS_BEGIN

	INT idx;
	SelectMakeList(m_CM_ALIAS, _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(m_CM_ALIAS, _T("MAIN_GRID"));

	if(idx < 0)
	{
		//����ó��
		ESLAfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		return -1;
	}	

	CESL_DataMgr *pDM = FindDM(m_DM_ALIAS);
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_����_����_���_����") );
		return -1;
	}

	CString BookKey,strStatus;
	strStatus = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );

	if (strStatus != _T("BOR112N")) //����ڷ� 
	{
		//����ó��
		ESLAfxMessageBox(_T("��� �ڷḸ ó�� �� �� �ֽ��ϴ�."));
		return -1;
	}

	idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));

	while ( idx != -1 ) {
		strStatus = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );

		if (strStatus != _T("BOR112N")) //����ڷ� 
		{
			//����ó��
			ESLAfxMessageBox(_T("��� �ڷḸ ó�� �� �� �ֽ��ϴ�."));
			return -1;
		}

		idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));
	}

	return 0;

EFS_END
return -1;

}

VOID CBO_CAT_REG_0400::OnbACCESSIONNOGIVE() 
{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
EFS_BEGIN


	if(OpenMode == 0)
	{
		CancelRegNo();
	}
	else if(OpenMode == 2)
	{
		CancelRegNo();
	}




EFS_END
}

VOID CBO_CAT_REG_0400::OnCancel() 
{
EFS_BEGIN

	// TODO: Add extra cleanup here
	DestroyWindow();		
	//CDialog::OnCancel();

EFS_END
}

VOID CBO_CAT_REG_0400::OnrCanRegNo() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CButton * chkRegCan;
	chkRegCan=(CButton *)GetDlgItem(IDC_rCanRegNo);
	chkRegCan->SetCheck(1);

EFS_END
}


VOID CBO_CAT_REG_0400::OnrAccCan() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CButton * chkRegCan;
	chkRegCan=(CButton *)GetDlgItem(IDC_rAcqBack);
	chkRegCan->SetCheck(1);

EFS_END
}



INT CBO_CAT_REG_0400::CancelRegNo()
{	
	//��Ϲ�ȣ ���
	CButton *pCanRegNo;
	pCanRegNo = (CButton *)GetDlgItem(IDC_rCanRegNo);

	//���� ���� 
	CButton *pAcqBack;
	pAcqBack = (CButton *)GetDlgItem(IDC_rAcqBack);
	

	INT iCh;
	CESL_DataMgr *pDM;	
	pDM = FindDM(m_DM_ALIAS);		

	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return -1;

	//������Ϲ�ȣ �߰� ����
	INT iBookCount = -1; 
	CESL_DataMgr DM_BOOK;


	if ( pCanRegNo->GetCheck() == 1 ) {	//��� ��ȣ ���


		INT ids;// = 6;

		ids = ::MessageBox( GetSafeHwnd() , _T("�����Ͻ� �ڷ��� ��Ϲ�ȣ�� �����Ͻðڽ��ϱ�?") ,
			_T("�� ��") ,
			MB_YESNO | MB_ICONQUESTION );
	
		if ( ids == IDYES ) {		

			DeleteRegNo();

		}
		else {//�������
			return -1;
		}

	}
	else if ( pAcqBack->GetCheck() == 1 ) //���� ����
	{

		INT ids;// = 6;

		ids = ::MessageBox( GetSafeHwnd() , _T("�����Ͻ� �ڷ��� ��Ϲ�ȣ�� �����ϰ� ���� ���� �Ͻðڽ��ϱ�?") ,
			_T("�� ��") ,
			MB_YESNO | MB_ICONQUESTION );
	
		if ( ids == IDYES ) {	
			

/* ���� ���� : �������ͽ� ���Ա����� ������ return �ǵ��� ���� +++
 * ���� ��¥ : 2003.10.17
 * ���� ��� : ��ȫ�� 
 */
			
			INT idx = SelectGetHeadPosition(m_CM_ALIAS, _T("MAIN_GRID"));
			CString strRegCode;
			pDM->GetCellData(_T("BB_���Ա���"), idx, strRegCode);
			if (strRegCode == _T(""))
			{
				AfxMessageBox(_T("���Ա����� �����ϴ�"));
				return -1;
			}
// ���� ���� : �������ͽ� ��ϱ����� ������ return �ǵ��� ���� ---

			DeleteRegNo();
			BackAcq();

		}
		else {//�������
			return -1;
		}

	}
	
	iCh = pDM->SendFrame();
	if ( iCh == -3 || iCh == -1) {
		AfxMessageBox(_T("SendFrame Query Error!!"));
		return -5;
	}
	iCh = pDM->EndFrame();	
	if ( 0 > iCh ) return -1;

	DestroyWindow();	
	
	return 0;
}

INT CBO_CAT_REG_0400::DeleteRegNo()
{

	INT iCh;	
	CString sQuery;

	CESL_DataMgr *pDM;	
	pDM = FindDM(m_DM_ALIAS);		

	CESL_ControlMgr *pCM;
	pCM = FindCM(m_CM_ALIAS);		
	
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));

	
	// ��� ��ȣ�� �����Ͽ� ������� ��ȣ�� �ִ´�. 
	INT idx;
	SelectMakeList(m_CM_ALIAS, _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(m_CM_ALIAS, _T("MAIN_GRID"));
	
	CString strRegNo,strRegCode,strAccNo,sRecKey;
	strRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , idx );
	
	strRegNo.TrimLeft();
	strRegNo.TrimRight();

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	
	if (strRegNo != _T("")) //����ڷ� 
	{
		strRegCode = pDM->GetCellData( _T("BB_��ϱ���_�ڵ�") , idx );
		strAccNo = pDM->GetCellData( _T("���ι�ȣ") , idx );
		sRecKey = pDM->GetCellData( _T("BB_åKEY") , idx );
		
		// ��� ��ȣ ����
		sQuery.Format( _T("UPDATE BO_BOOK_TBL SET ACCESSION_REC_KEY = NULL, REG_NO = NULL , WORKING_STATUS = 'BOR111N' WHERE REC_KEY = %s;") , sRecKey );
		iCh = pDM->AddFrame( sQuery );	
		if ( 0 > iCh ) return -1;
		
		//���� ��� ���̺� �ִ� �� ���� 
		CString sQuery,sCount;
		//=====================================================
		//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 		sQuery.Format( _T("select count(reg_no) from co_drop_accession_no_tbl WHERE reg_code ='%s' and reg_no =%s AND MANAGE_CODE = UDF_MANAGE_CODE") , strRegCode , strRegNo.Mid(2,10) );
		sQuery.Format( _T("select count(reg_no) from co_drop_accession_no_tbl WHERE reg_code ='%s' and reg_no =%s AND MANAGE_CODE = UDF_MANAGE_CODE") , strRegCode , strRegNo.Mid(nRegCodeLength,12 - nRegCodeLength ) );
		//=====================================================
		pDM->GetOneValueQuery( sQuery , sCount );
		//������ �Է� 
		if ( sCount == _T("0") ) {
			CString sRecKey;
			INT ids = pDM->MakeRecKey(sRecKey);
			if ( 0 > ids ) return -1;
			//=====================================================
			//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 			sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO, FIRST_WORK, LAST_WORK ) VALUES ( %s, '%s', '%s', '%s', %s, '%s', '%s' );") , sRecKey , _T("MO") , _T("") , strRegCode , strRegNo.Mid(2,10), GetWorkLogMsg(_T("��Ϲ�ȣ�ο����"),WORK_LOG), GetWorkLogMsg(_T("��Ϲ�ȣ�ο����"),WORK_LOG) );
			sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO, FIRST_WORK, LAST_WORK ) VALUES ( %s, '%s', '%s', '%s', %s, '%s', '%s' );") , sRecKey , _T("MO") , _T("") , strRegCode , strRegNo.Mid(nRegCodeLength,12 - nRegCodeLength ), GetWorkLogMsg(_T("��Ϲ�ȣ�ο����"),WORK_LOG), GetWorkLogMsg(_T("��Ϲ�ȣ�ο����"),WORK_LOG) );
			//=====================================================
			
			iCh = pDM->AddFrame( sQuery );
			if ( 0 > iCh ) return -1;			
		}
		
		// DM �ٷ� �ݿ� ���ش� 
		pDM->SetCellData( _T("���ι�ȣ") ,_T(""), idx );
		pDM->SetCellData( _T("BB_��Ϲ�ȣ") ,_T(""), idx );
		pDM->SetCellData( _T("BB_�ڷ����_�ڵ�") ,_T("BOR111N"), idx );

		// �ش� �׸��� �ݿ� 
		pGrid->DisplayLine(idx);
		
	}
	
	idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));
	
	
	while ( idx != -1 ) {
		strRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , idx );
		
		if (strRegNo != _T("")) //����ڷ� 
		{
			strRegCode = pDM->GetCellData( _T("BB_��ϱ���_�ڵ�") , idx );
			strAccNo = pDM->GetCellData( _T("���ι�ȣ") , idx );
			sRecKey = pDM->GetCellData( _T("BB_åKEY") , idx );

			// ��� ��ȣ ����
			sQuery.Format( _T("UPDATE BO_BOOK_TBL SET ACCESSION_REC_KEY = NULL, REG_NO = NULL , WORKING_STATUS = 'BOR111N' WHERE REC_KEY = %s;") , sRecKey );
			iCh = pDM->AddFrame( sQuery );	
			if ( 0 > iCh ) return -1;

			//���� ��� ���̺� �ִ� �� ���� 
			CString sQuery,sCount;
			//=====================================================
			//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 			sQuery.Format( _T("select count(reg_no) from co_drop_accession_no_tbl WHERE reg_code ='%s' and reg_no =%s AND MANAGE_CODE = UDF_MANAGE_CODE") , strRegCode , strRegNo.Mid(2,10));
			sQuery.Format( _T("select count(reg_no) from co_drop_accession_no_tbl WHERE reg_code ='%s' and reg_no =%s AND MANAGE_CODE = UDF_MANAGE_CODE") , strRegCode , strRegNo.Mid(nRegCodeLength,12 - nRegCodeLength ));
			//=====================================================
			
			pDM->GetOneValueQuery( sQuery , sCount );
			//������ �Է� 
			if ( sCount == _T("0") ) {
				CString sRecKey;
				INT ids = pDM->MakeRecKey(sRecKey);
				if ( 0 > ids ) return -1;
			
				//=====================================================
				//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
//	 			sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO, FIRST_WORK, LAST_WORK ) VALUES ( %s, '%s', '%s', '%s', %s, '%s', '%s' );") , sRecKey , _T("MO") , _T("") , strRegCode , strRegNo.Mid(2,10), GetWorkLogMsg(_T("��Ϲ�ȣ�ο����"),WORK_LOG), GetWorkLogMsg(_T("��Ϲ�ȣ�ο����"),WORK_LOG) );
				sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO, FIRST_WORK, LAST_WORK ) VALUES ( %s, '%s', '%s', '%s', %s, '%s', '%s' );") , sRecKey , _T("MO") , _T("") , strRegCode , strRegNo.Mid(nRegCodeLength,12 - nRegCodeLength ), GetWorkLogMsg(_T("��Ϲ�ȣ�ο����"),WORK_LOG), GetWorkLogMsg(_T("��Ϲ�ȣ�ο����"),WORK_LOG) );
				//=====================================================
				
				iCh = pDM->AddFrame( sQuery );		
				if ( 0 > iCh ) return -1;				
			}		
			
			pDM->SetCellData( _T("���ι�ȣ") ,_T(""), idx );
			pDM->SetCellData( _T("BB_��Ϲ�ȣ") ,_T(""), idx );
			pDM->SetCellData( _T("BB_�ڷ����_�ڵ�") ,_T("BOR111N"), idx );

			// �ش� �׸��� �ݿ� 
			pGrid->DisplayLine(idx);
			
		}
		
		idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));
	}
	
	
	return 0;

}

INT CBO_CAT_REG_0400::BackAcq()
{

	
	INT iCh;	
	CString sQuery;

	CESL_DataMgr *pDM;	
	pDM = FindDM(m_DM_ALIAS);		

	CESL_ControlMgr *pCM;
	pCM = FindCM(m_CM_ALIAS);		
	
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));

	
	// ��� ��ȣ�� �����Ͽ� ������� ��ȣ�� �ִ´�. 
	INT idx,Dm_index=0;
	SelectMakeList(m_CM_ALIAS, _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(m_CM_ALIAS, _T("MAIN_GRID"));
	
	CString AcqCode,sRecKey;
	AcqCode = pDM->GetCellData( _T("BB_���Ա���") , idx );
	sRecKey = pDM->GetCellData( _T("BB_åKEY") , idx );
	
	AcqCode.TrimLeft();
	AcqCode.TrimRight();
	
	if (AcqCode == _T("")) //����ڷ� 
	{
		AfxMessageBox(_T("���Ա����� �����ϴ�. ���� �Ҽ� �����ϴ�.!")); 
		return 0;
	} 
	else if ( AcqCode == _T("1")) // �����ڷ� 
	{
		// �˼������ ���, �̳��ڷ�� ���º��� [2003/09/30/������]
		if (CancelType == 1)
		{
			sQuery.Format( _T("UPDATE BO_BOOK_TBL SET DELIVERY_YN ='N', LAST_WORK='%s', WORKING_STATUS = 'BOA113O', PREV_WORKING_STATUS = 'BOR112N' WHERE REC_KEY = %s;") ,GetWorkLogMsg(_T("��Ϲ�ȣ�ο����"),WORK_LOG), sRecKey );
			iCh = pDM->AddFrame( sQuery );	
			if ( 0 > iCh ) return -1;
		}
		else 
		{
			// ���� �ΰ� ����ڷ� ����d
			sQuery.Format( _T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', WORKING_STATUS = 'BOA114O', PREV_WORKING_STATUS = 'BOR112N' WHERE REC_KEY = %s;") ,GetWorkLogMsg(_T("��Ϲ�ȣ�ο����"),WORK_LOG), sRecKey );
			iCh = pDM->AddFrame( sQuery );
			if ( 0 > iCh ) return -1;
		}

	}
	else if ( AcqCode == _T("2")) // �����ڷ� 
	{
		// ���� �ΰ� ����ڷ� ����
		sQuery.Format( _T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', WORKING_STATUS = 'BOA211O', PREV_WORKING_STATUS = 'BOR112N' WHERE REC_KEY = %s;") ,GetWorkLogMsg(_T("��Ϲ�ȣ�ο����"),WORK_LOG), sRecKey );
		iCh = pDM->AddFrame( sQuery );	
		if ( 0 > iCh ) return -1;
	}
		
	// DM �ٷ� �ݿ� ���ش� 
	pDM->DeleteRow(idx);
	Dm_index++;		
	
	idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));
	
	
	while ( idx != -1 ) {
		AcqCode = pDM->GetCellData( _T("BB_���Ա���") , idx - Dm_index);
		sRecKey = pDM->GetCellData( _T("BB_åKEY") , idx - Dm_index);
		
		AcqCode.TrimLeft();
		AcqCode.TrimRight();
		
		if (AcqCode == _T("")) //����ڷ� 
		{
			AfxMessageBox(_T("���Ա����� �����ϴ�. ���� �Ҽ� �����ϴ�.!")); 
		} 
		else if ( AcqCode == _T("1")) // �����ڷ� 
		{
			if (CancelType == 1)
			{
				sQuery.Format( _T("UPDATE BO_BOOK_TBL SET DELIVERY_YN ='N', LAST_WORK='%s', WORKING_STATUS = 'BOA113O', PREV_WORKING_STATUS = 'BOR112N' WHERE REC_KEY = %s;") ,GetWorkLogMsg(_T("��Ϲ�ȣ�ο����"),WORK_LOG), sRecKey );
				iCh = pDM->AddFrame( sQuery );	
				if ( 0 > iCh ) return -1;
			}
			else 
			{
				// ���� �ΰ� ����ڷ� ����
				sQuery.Format( _T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', WORKING_STATUS = 'BOA114O', PREV_WORKING_STATUS = 'BOR112N' WHERE REC_KEY = %s;") ,GetWorkLogMsg(_T("��Ϲ�ȣ�ο����"),WORK_LOG), sRecKey );
				iCh = pDM->AddFrame( sQuery );
				if ( 0 > iCh ) return -1;
			}
			
		}
		else if ( AcqCode == _T("2")) // �����ڷ� 
		{
			// ���� �ΰ� ����ڷ� ����
			sQuery.Format( _T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', WORKING_STATUS = 'BOA211O', PREV_WORKING_STATUS = 'BOR112N' WHERE REC_KEY = %s;") ,GetWorkLogMsg(_T("��Ϲ�ȣ�ο����"),WORK_LOG), sRecKey );
			iCh = pDM->AddFrame( sQuery );	
			if ( 0 > iCh ) return -1;			
		}
		
		// DM �ٷ� �ݿ� ���ش� 
		pDM->DeleteRow(idx - Dm_index);
		Dm_index++;		
		
		idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));
	}
	
	pGrid->Display();


	return 0;
}


HBRUSH CBO_CAT_REG_0400::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
