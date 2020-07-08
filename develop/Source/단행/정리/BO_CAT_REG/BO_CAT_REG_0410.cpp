// BO_CAT_REG_0410.cpp : implementation file
//

#include "stdafx.h"
#include "BO_REG_MGR.h"
#include "BO_CAT_REG_0410.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
	if ( 0 > iCh ) return -1;

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
	if ( 0 > iCh ) return -1;

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
	CString AcqCode, sRecKey, strSpeciesKey, strWorkingStatus;
	CArray <INT, INT> RemoveAllList;
	INT idx;  

	SelectMakeList(m_CM_ALIAS, _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(m_CM_ALIAS, _T("MAIN_GRID"));
		
	while ( idx != -1 ) 
	{
		AcqCode = pDM->GetCellData( _T("BB_���Ա���") , idx );
		sRecKey = pDM->GetCellData( _T("BB_åKEY") , idx);
		strSpeciesKey = pDM->GetCellData(_T("IBS_��KEY"), idx);
		
		AcqCode.TrimLeft();
		AcqCode.TrimRight();
		
		if (AcqCode != _T("1") && AcqCode != _T("2")) //����ڷ� 	
			nNoAcqCode++;
		else
		{
			if ( AcqCode == _T("1")) // �����ڷ� 
			{
				if (CancelType == 1)
				{
					strWorkingStatus = _T("BOA113O");
					sQuery.Format(
						_T("UPDATE BO_BOOK_TBL SET DELIVERY_YN ='N', LAST_WORK='%s', WORKING_STATUS = '%s', PREV_WORKING_STATUS = 'BOR112N' WHERE REC_KEY = %s;"),
						GetWorkLogMsg(_T("������"),WORK_LOG), strWorkingStatus, sRecKey);
					iCh = pDM->AddFrame( sQuery );	
					if ( 0 > iCh ) return -1;
				}
				else 
				{
					// ���� �ΰ� ����ڷ� ����
					strWorkingStatus = _T("BOA114O");
					sQuery.Format( 
						_T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', WORKING_STATUS = '%s', PREV_WORKING_STATUS = 'BOR112N' WHERE REC_KEY = %s;"),
						GetWorkLogMsg(_T("������"),WORK_LOG), strWorkingStatus, sRecKey);
					iCh = pDM->AddFrame( sQuery );
					if ( 0 > iCh ) return -1;
				}
			}
			else if ( AcqCode == _T("2")) // �����ڷ� 
			{
				// ���� �ΰ� ����ڷ� ����
				strWorkingStatus = _T("BOA211O");
				sQuery.Format( 
					_T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', WORKING_STATUS = '%s', PREV_WORKING_STATUS = 'BOR112N' WHERE REC_KEY = %s;"),
					GetWorkLogMsg(_T("������"),WORK_LOG), strWorkingStatus, sRecKey);
				iCh = pDM->AddFrame( sQuery );	
				if ( 0 > iCh ) return -1;
			}
			sQuery.Format(
				_T("UPDATE IDX_BO_TBL SET LAST_WORK='%s', WORKING_STATUS=TESTWORKSTATUS(%s) WHERE REC_KEY=%s;"),
				GetWorkLogMsg(_T("������"),WORK_LOG), strSpeciesKey, strSpeciesKey);
			iCh = pDM->AddFrame( sQuery );	
			if ( 0 > iCh ) return -1;
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
			strNoAcqCode.Format(_T("���Ա����� ����/������ �ƴ� �ڷ� %d���� �˼�������� ���߽��ϴ�."), nNoAcqCode);
			AfxMessageBox(strNoAcqCode); 
		}
		else
		{
			strNoAcqCode.Format(_T("���Ա����� ����/������ �ƴ� �ڷ� %d���� ����ΰ�������� ���߽��ϴ�."), nNoAcqCode);
			AfxMessageBox(strNoAcqCode); 
		}
	}
	
	pGrid->Display();
	return 0;
}

INT CBO_CAT_REG_0410::DeleteRegNo()
{
	INT iCh;
	INT nIds;
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
	
	CString strRegNo,strRegCode,strAccNo,sRecKey, strSpeciesKey;
	strRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , idx );
	
	strRegNo.TrimLeft();
	strRegNo.TrimRight();

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	
	CMarc pMarc;
	CString sMarc;
	//m_pInfo->pMarcMgr->Decoding(strMarc, &pMarc);

	while ( idx != -1 ) {
		strRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ") , idx );
		
		if (strRegNo != _T("")) //����ڷ� 
		{
			strRegCode = pDM->GetCellData( _T("BB_��ϱ���_�ڵ�") , idx );
			strAccNo = pDM->GetCellData( _T("���ι�ȣ") , idx );
			sRecKey = pDM->GetCellData( _T("BB_åKEY") , idx );
			strSpeciesKey = pDM->GetCellData(_T("IBS_��KEY"), idx);

			//===================================================
			//2009.05.06 ADD BY LKS : �Էµ�(��ũ��ü�� ����) 049�±׸� �����ش�.			
			sMarc = pDM->GetCellData( _T("BS_MARC") , idx );
			if(!sMarc.IsEmpty())
			{
				m_pInfo->pMarcMgr->Decoding(sMarc, &pMarc);
				m_pInfo->pMarcMgr->DeleteField(&pMarc, _T("049"));
				m_pInfo->pMarcMgr->Encoding(&pMarc, sMarc);	
				//===================================================
				//2009.08.20 UPDATE BY PJW : MARC���̰����츦 ����ؼ� ������
//				//===================================================
//				//2009.06.01 ADD BY LKS : UPDATE�� " ' "�� ������� error!!
//				//							" ' "�� " '' "�� ��ü
//				
//				sMarc.Replace(_T("'"),_T("''"));
//				//===================================================
//				sQuery.Format(
//					_T("UPDATE BO_SPECIES_TBL SET MARC = '%s' WHERE REC_KEY = %s;"), 
//					sMarc, strSpeciesKey);
//				iCh = pDM->AddFrame( sQuery );
//				if ( 0 > iCh ) return -1;
				pDM->InitDBFieldData();
				nIds = pDM->AddDBFieldData(_T("MARC"), _T("STRING"), sMarc, 1);
				if(nIds < 0) return nIds;
				nIds = pDM->MakeUpdateFrame(_T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);
				if(nIds < 0) return nIds;
				//===================================================
			}
			//===================================================
			
			// ��� ��ȣ ����
			sQuery.Format(
				_T("UPDATE BO_BOOK_TBL SET ACCESSION_REC_KEY = NULL, REG_NO = NULL , REG_DATE = '', WORKING_STATUS = 'BOR111N', LAST_WORK='%s' WHERE REC_KEY = %s;"), 
				GetWorkLogMsg(_T("������"),WORK_LOG), sRecKey);
			iCh = pDM->AddFrame( sQuery );	
			if ( 0 > iCh ) return -1;
			sQuery.Format(
				_T("UPDATE IDX_BO_TBL SET WORKING_STATUS = TESTWORKSTATUS(%s), LAST_WORK='%s' ")
				_T("WHERE REC_KEY = %s;"), 
				strSpeciesKey, GetWorkLogMsg(_T("������"),WORK_LOG), strSpeciesKey);
			iCh = pDM->AddFrame( sQuery );	
			if ( 0 > iCh ) return -1;
			
			//���� ��� ���̺� �ִ� �� ���� 
			CString sQuery,sCount;
			
			//=====================================================
			//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 			sQuery.Format( _T("select count(reg_no) from co_drop_accession_no_tbl WHERE reg_code ='%s' and reg_no =%s AND MANAGE_CODE = UDF_MANAGE_CODE") , strRegCode , strRegNo.Mid(2,10));
			sQuery.Format( _T("select count(reg_no) from co_drop_accession_no_tbl WHERE reg_code ='%s' and reg_no =%s AND MANAGE_CODE = UDF_MANAGE_CODE") , strRegCode , strRegNo.Mid(nRegCodeLength,12 - nRegCodeLength ) );
			//=====================================================
			
			pDM->GetOneValueQuery( sQuery , sCount );
			//������ �Է� 
			if ( sCount == _T("0") ) {
				CString sRecKey;
				iCh = pDM->MakeRecKey(sRecKey);
				if ( 0 > iCh ) return -1;
		
				//=====================================================
				//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 				sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO, MANAGE_CODE ) VALUES ( %s, '%s', '%s', '%s', %s, UDF_MANAGE_CODE );") , sRecKey , _T("MO") , _T("") , strRegCode , strRegNo.Mid(2,10) );				
				sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO, MANAGE_CODE ) VALUES ( %s, '%s', '%s', '%s', %s, UDF_MANAGE_CODE );") , sRecKey , _T("MO") , _T("") , strRegCode , strRegNo.Mid(nRegCodeLength,12 - nRegCodeLength ) );
				//=====================================================
				
//DEL 				sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO ) VALUES ( %s, '%s', '%s', '%s', %s );") , sRecKey , _T("MO") , _T("") , strRegCode , strRegNo.Mid(2,10) );
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

VOID CBO_CAT_REG_0410::SetOperationType(INT nType)
{
	m_nType = nType;
}
