// SE_CAT_REG_0400.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CAT_REG_0400.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_REG_0400 dialog

CSE_CAT_REG_0400::CSE_CAT_REG_0400(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_REG_0400)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_CAT_REG_0400::~CSE_CAT_REG_0400()
{
}

VOID CSE_CAT_REG_0400::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REG_0400)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}

BEGIN_MESSAGE_MAP(CSE_CAT_REG_0400, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_REG_0400)
	ON_BN_CLICKED(IDC_bACCESSIONNO_GIVE, OnbACCESSIONNOGIVE)
	ON_BN_CLICKED(IDC_rCanRegNo, OnrCanRegNo)
	ON_BN_CLICKED(IDC_rAcqBack, OnrAccCan)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_REG_0400 message handlers

BOOL CSE_CAT_REG_0400::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
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

BOOL CSE_CAT_REG_0400::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;
}

INT CSE_CAT_REG_0400::Display(INT nDir)
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
	if ( NULL == pDM ) 
	{
		AfxMessageBox ( _T("DM_����_����_���_����") );
		return -1;
	}

	CString BookKey,strStatus;
	strStatus = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );

	if (strStatus != _T("SER112N")) //����ڷ� 
	{
		//����ó��
		ESLAfxMessageBox(_T("��� �ڷḸ ó�� �� �� �ֽ��ϴ�."));
		return -1;
	}
	idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));

	while ( idx != -1 ) 
	{
		strStatus = pDM->GetCellData( _T("BB_�ڷ����_�ڵ�") , idx );

		if (strStatus != _T("SER112N")) //����ڷ� 
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

VOID CSE_CAT_REG_0400::OnbACCESSIONNOGIVE() 
{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
EFS_BEGIN

	if(OpenMode == 0)
		CancelRegNo();
	
	else if(OpenMode == 2)
		CancelRegNo();
	
EFS_END
}

VOID CSE_CAT_REG_0400::OnCancel() 
{
EFS_BEGIN

	DestroyWindow();		
	//CDialog::OnCancel();

EFS_END
}

VOID CSE_CAT_REG_0400::OnrCanRegNo() 
{
EFS_BEGIN

	CButton * chkRegCan;
	chkRegCan=(CButton *)GetDlgItem(IDC_rCanRegNo);
	chkRegCan->SetCheck(1);

EFS_END
}

VOID CSE_CAT_REG_0400::OnrAccCan() 
{
EFS_BEGIN

	CButton * chkRegCan;
	chkRegCan=(CButton *)GetDlgItem(IDC_rAcqBack);
	chkRegCan->SetCheck(1);

EFS_END
}

INT CSE_CAT_REG_0400::CancelRegNo()
{		
	m_DM_ALIAS = _T("DM_����_����_���_����");
	m_CM_ALIAS = _T("CM_����_����_���_����");
	
	INT iReturn = Display(0);
	if (iReturn == -1)
		return -1;

// DM, CM, GRID �����ڷ� üũ 
	CESL_DataMgr *pDM;	
	pDM = FindDM(m_DM_ALIAS);
	if (!pDM)
	{
		AfxMessageBox(_T("DM ����!")); 
		return -1; 
	}
	CESL_ControlMgr *pCM;
	pCM = FindCM(m_CM_ALIAS);
	if (!pCM)
	{
		AfxMessageBox(_T("CM ����!")); 
		return -1; 
	}
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));
	pGrid->SelectMakeList();
	INT Index = pGrid->SelectGetCount();
	if (Index < 1)
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�.")); 
		return -1;
	}
	
	INT iCh;
	iCh = pDM->StartFrame();

	//������Ϲ�ȣ �߰� ����
	INT iBookCount = -1; 
	CESL_DataMgr DM_BOOK;

	INT ids;// = 6;
	ids = ::MessageBox( GetSafeHwnd() , _T("�����Ͻ� �ڷ��� ��Ϲ�ȣ�� �����ϰ� ���� ���� �Ͻðڽ��ϱ�?") ,
			_T("�� ��") ,
			MB_YESNO | MB_ICONQUESTION );
	
	if ( ids == IDYES ) 
	{		
		DeleteRegNo();
		BackAcq();
	}
	else
		return -1;

	iCh = pDM->SendFrame();
	if ( iCh == -3 || iCh == -1) 
	{
		AfxMessageBox(_T("SendFrame Query Error!!"));
		return -5;
	}
	iCh = pDM->EndFrame();	
	DestroyWindow();	
	
	return 0;
}

INT CSE_CAT_REG_0400::DeleteRegNo()
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
	
	CString strRegNo,strRegCode,sRecKey;
	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	
	while ( idx != -1 ) 
	{
		strRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , idx );
		
		if (strRegNo != _T("")) //����ڷ� 
		{
			strRegCode = pDM->GetCellData( _T("BB_��ϱ���_�ڵ�") , idx );
			sRecKey = pDM->GetCellData( _T("BB_åKEY") , idx );

			// ��� ��ȣ ����
			sQuery.Format( _T("UPDATE SE_BOOK_TBL SET ACCESSION_REC_KEY = NULL, REG_NO = NULL , WORKING_STATUS = 'SER111N' WHERE REC_KEY = %s;") , sRecKey );
			iCh = pDM->AddFrame( sQuery );	

			//���� ��� ���̺� �ִ� �� ���� 
			CString sQuery,sCount;
			//=====================================================
			//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 			sQuery.Format( _T("SELECT COUNT(REG_NO) FROM CO_DROP_ACCESSION_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND REG_CODE ='%s' AND REG_NO =%s") , strRegCode , strRegNo.Mid(2,10));
			sQuery.Format( _T("SELECT COUNT(REG_NO) FROM CO_DROP_ACCESSION_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND REG_CODE ='%s' AND REG_NO =%s") , strRegCode , strRegNo.Mid(nRegCodeLength,12 - nRegCodeLength ));
			//=====================================================
			
			pDM->GetOneValueQuery( sQuery , sCount );
			//������ �Է� 
			if ( sCount == _T("0") ) 
			{
				CString sRecKey;
				pDM->MakeRecKey(sRecKey);

				//=====================================================
				//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 				sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO, MANAGE_CODE ) VALUES ( %s, '%s', '%s', '%s', %s, UDF_MANAGE_CODE );") , sRecKey , _T("SE") , _T("") , strRegCode , strRegNo.Mid(2,10) );
				sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO, MANAGE_CODE ) VALUES ( %s, '%s', '%s', '%s', %s, UDF_MANAGE_CODE );") , sRecKey , _T("SE") , _T("") , strRegCode , strRegNo.Mid(nRegCodeLength,12 - nRegCodeLength) );
				//=====================================================				
				
				iCh = pDM->AddFrame( sQuery );		
			}		
			
			pDM->SetCellData( _T("���ι�ȣ") ,_T(""), idx );
			pDM->SetCellData( _T("BB_��Ϲ�ȣ") ,_T(""), idx );
			pDM->SetCellData( _T("BB_�ڷ����_�ڵ�") ,_T("SER111N"), idx );

			// �ش� �׸��� �ݿ� 
			pGrid->DisplayLine(idx);
		}
		idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));
	}
	return 0;
}

INT CSE_CAT_REG_0400::BackAcq()
{
	INT iCh;	
	CString sQuery;

	CESL_DataMgr *pDM;	
	pDM = FindDM(m_DM_ALIAS);		

	CESL_ControlMgr *pCM;
	pCM = FindCM(m_CM_ALIAS);		
	
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));

	INT idx = 0;
	SelectMakeList(m_CM_ALIAS, _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(m_CM_ALIAS, _T("MAIN_GRID"));
	CString AcqCode, sRecKey;
    CArray <INT, INT> RemoveAllList;

	// �������� �ð� (REG_TRANSFER_DATE)
	CTime t = CTime::GetCurrentTime();
	CString result;
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	while ( idx != -1 ) 
	{
		sRecKey = pDM->GetCellData( _T("BB_åKEY") , idx);

		sQuery.Format( _T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = 'SER111N', PREV_WORKING_STATUS = 'SER112N', TRANSFER_YN = 'Y', REG_TRANSFER_DATE = '%s' WHERE REC_KEY = %s;"), result, sRecKey );
		iCh = pDM->AddFrame( sQuery );	

		RemoveAllList.Add(idx);
		idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));
	}
	
	// DM �ݿ�  
	for ( INT i = RemoveAllList.GetSize()-1 ; i >= 0 ; i-- )
	{ 
		INT RowIdx = RemoveAllList.GetAt(i); 
		pDM->DeleteRow(RowIdx);
	}
	
	pGrid->Display();

	return 0;
}


HBRUSH CSE_CAT_REG_0400::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}
