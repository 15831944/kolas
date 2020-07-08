// BO_CAT_REG_0410.cpp : implementation file
//

#include "stdafx.h"
// #include _T("BO_REG_MGR.h")
#include "BO_CAT_REG_0410.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WORK_LOG GetWorkLogMsg(_T("������"), _T(__FILE__), __LINE__)
/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0410 dialog


CBO_CAT_REG_0410::CBO_CAT_REG_0410(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_REG_0410)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CBO_CAT_REG_0410::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REG_0410)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_REG_0410, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REG_0410)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0410 message handlers

INT CBO_CAT_REG_0410::CancelRegNo()
{
	m_DM_ALIAS = _T("DM_����_����_���_����");
	m_CM_ALIAS = _T("CM_����_����_���_����");

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
//��ȣ��� 
	if ( m_nType == 0 ) 
		DeleteRegNo();
//����ΰ����, �˼����
	else if ( m_nType == 1 ) 
	{
		DeleteRegNo();
		BackAcq();
	}
	
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

INT CBO_CAT_REG_0410::BackAcq()
{
	INT iCh;	
	CString sQuery;

	CESL_DataMgr *pDM;	
	pDM = FindDM(m_DM_ALIAS);		

	CESL_ControlMgr *pCM;
	pCM = FindCM(m_CM_ALIAS);		
	
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));

// ��� ��ȣ�� �����Ͽ� ������� ��ȣ�� �ִ´�. 

	INT nNoAcqCode = 0; 
	CString AcqCode, sRecKey;
	CArray <INT, INT> RemoveAllList;
	INT idx;  

	SelectMakeList(m_CM_ALIAS, _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(m_CM_ALIAS, _T("MAIN_GRID"));
		
	while ( idx != -1 ) 
	{
		AcqCode = pDM->GetCellData( _T("BB_���Ա���") , idx );
		sRecKey = pDM->GetCellData( _T("BB_åKEY") , idx);
		
		AcqCode.TrimLeft();
		AcqCode.TrimRight();
		
		if (AcqCode == _T("") || AcqCode == _T("3")) //����ڷ� 	
			nNoAcqCode++;

		else
		{
			if ( AcqCode == _T("1")) // �����ڷ� 
			{
				if (CancelType == 1)
				{
					sQuery.Format( _T("UPDATE BO_BOOK_TBL SET DELIVERY_YN ='N', LAST_WORK='%s', WORKING_STATUS = 'BOA113O', PREV_WORKING_STATUS = 'BOR112N' WHERE REC_KEY = %s;") ,WORK_LOG, sRecKey );
					iCh = pDM->AddFrame( sQuery );	
				}
				else 
				{
					// ���� �ΰ� ����ڷ� ����
					sQuery.Format( _T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', WORKING_STATUS = 'BOA114O', PREV_WORKING_STATUS = 'BOR112N' WHERE REC_KEY = %s;") ,WORK_LOG, sRecKey );
					iCh = pDM->AddFrame( sQuery );
				}
			}
			else if ( AcqCode == _T("2")) // �����ڷ� 
			{
				// ���� �ΰ� ����ڷ� ����
				sQuery.Format( _T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', WORKING_STATUS = 'BOA211O', PREV_WORKING_STATUS = 'BOR112N' WHERE REC_KEY = %s;") ,WORK_LOG, sRecKey );
				iCh = pDM->AddFrame( sQuery );	
			}
			RemoveAllList.Add(idx); 
		}
		idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));
	}
// DM����	 
	for ( INT i = RemoveAllList.GetSize()-1 ; i >= 0 ; i-- )
	{ 
		INT RowIdx = RemoveAllList.GetAt(i); 
		pDM->DeleteRow(RowIdx);
		
	}
			
	if (nNoAcqCode > 0) //����ڷ� 
	{	
		CString strNoAcqCode; 
		if (CancelType == 1)
		{
			strNoAcqCode.Format(_T("���Ա����� ���ų� ��ȯ�� �ڷ� %d���� �˼�������� ���ߴϴ�."), nNoAcqCode);
			AfxMessageBox(strNoAcqCode); 
		}
		strNoAcqCode.Format(_T("���Ա����� ���ų� ��ȯ�� �ڷ� %d���� ����ΰ�������� ���ߴϴ�."), nNoAcqCode);
		AfxMessageBox(strNoAcqCode); 
	}
	
	pGrid->Display();
	return 0;
}

INT CBO_CAT_REG_0410::DeleteRegNo()
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
	
	while ( idx != -1 ) {
		strRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , idx );
		
		if (strRegNo != _T("")) //����ڷ� 
		{
			strRegCode = pDM->GetCellData( _T("BB_��ϱ���_�ڵ�") , idx );
			strAccNo = pDM->GetCellData( _T("���ι�ȣ") , idx );
			sRecKey = pDM->GetCellData( _T("BB_åKEY") , idx );

			// ��� ��ȣ ����
			sQuery.Format( _T("UPDATE BO_BOOK_TBL SET ACCESSION_REC_KEY = NULL, REG_NO = NULL , WORKING_STATUS = 'BOR111N' WHERE REC_KEY = %s AND MANAGE_CODE = UDF_MANAGE_CODE;") , sRecKey );
			iCh = pDM->AddFrame( sQuery );	

			//���� ��� ���̺� �ִ� �� ���� 
			CString sQuery,sCount;
			sQuery.Format( _T("select count(reg_no) from co_drop_accession_no_tbl WHERE reg_code ='%s' and reg_no =%s AND MANAGE_CODE = UDF_MANAGE_CODE") , strRegCode , strRegNo.Mid(2,10));
			pDM->GetOneValueQuery( sQuery , sCount );
			//������ �Է� 
			if ( sCount == _T("0") ) {
				CString sRecKey;
				pDM->MakeRecKey(sRecKey);
				
				sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO , MANAGE_CODE) VALUES ( %s, '%s', '%s', '%s', %s , UDF_MANAGE_CODE);") , sRecKey , _T("MO") , _T("") , strRegCode , strRegNo.Mid(2,10) );
				iCh = pDM->AddFrame( sQuery );		
				
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

VOID CBO_CAT_REG_0410::SetOperationType(INT nType)
{
	m_nType = nType;
}
