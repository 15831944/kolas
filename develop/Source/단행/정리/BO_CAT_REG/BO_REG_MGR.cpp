// BO_REG_MGR.cpp: implementation of the BO_REG_MGR class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BO_REG_MGR.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "BO_CAT_REG_0400.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BO_REG_MGR::BO_REG_MGR(CESL_Mgr* pParent) : CSPF_Work(pParent)
{

}

BO_REG_MGR::~BO_REG_MGR()
{

}
INT BO_REG_MGR::SPFExecutePath(CString strPath)
{
EFS_BEGIN

	CSPF_Work::SPFExecutePath(strPath);
	
	INT ids=0;
	
	if(strPath.GetLength() == 0) return -1;

	if (strPath == _T("��ϸ������")){
		AccessionNoGive();
	}

	if (strPath == _T("��ϱ��к���")){
		ChangeCode();
	}

	if (strPath == _T("�ڷẹ��")){
		RollBackData();
	}




EFS_END
return -1;

}

INT BO_REG_MGR::ChangeCode()
{

	INT idx;
	CString BookKey,strQry,sTmp,sReg;
	
	CESL_ControlMgr *pCM;
	pCM = m_pParentMgr->FindCM(_T("CM_����_����_���_����"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_����_����_���_���� error") );
		return -2;
	}
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_����_����_���_����"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_����_����_���_����") );
		return -1;
	}

	// ������ ��� ���� �ڵ� �������� 
	m_pParentMgr->GetControlData(_T("CM_����_����_���_�ڵ�"), _T("BB_��ϱ���"), sReg);


	// ��� åŰ ���� ���� 
	m_pParentMgr->SelectMakeList(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	idx = m_pParentMgr->SelectGetHeadPosition(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	BookKey = pDM->GetCellData( _T("BB_åKEY") , idx );

	// ���� spl ����
	INT ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
		return -1;
	}

	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("REG_CODE"), _T("STRING"), sReg );
	ids = pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx);
	if ( 0 > ids ) return -1;
	ids = pDM->SendFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("SendFrame �� Ȯ���ϼ���"));
		return -1;
	}

	pDM->SetCellData(_T("BB_��ϱ���_�ڵ�"), sReg, idx);
	pGrid->DisplayLine(idx);
	
	idx = m_pParentMgr->SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		BookKey = pDM->GetCellData( _T("BB_åKEY") , idx );

		ids = pDM->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
			return -1;
		}

		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("REG_CODE"), _T("STRING"), sReg );
		ids = pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx);
		if ( 0 > ids ) return -1;
		ids = pDM->SendFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("SendFrame �� Ȯ���ϼ���"));
			return -1;
		}


		pDM->SetCellData(_T("BB_��ϱ���_�ڵ�"), sReg, idx);
		pGrid->DisplayLine(idx);
		idx = m_pParentMgr->SelectGetNext(_T("CM_����_����_���_����"), _T("MAIN_GRID"));
	}

	AfxMessageBox(_T("���� �Ǿ����ϴ�."));



//	pCM->AllControlDisplay();

	pGrid->ReshowSelectedRows();

	return 0;	

}
VOID BO_REG_MGR::RollBackData() {
	
EFS_BEGIN

	CBO_CAT_REG_0400 *pParent = (CBO_CAT_REG_0400*)m_pParentMgr;
	
	CButton *pAccCan;
	pAccCan = (CButton *)pParent->GetDlgItem(IDC_rAccCan);
	CButton *pRegCan;
	pRegCan = (CButton *)pParent->GetDlgItem(IDC_rRegCan);
	CButton *pAcqBack;
	pAcqBack = (CButton *)pParent->GetDlgItem(IDC_rAcqBack);	
	CButton *pChkRegCan;
	pChkRegCan = (CButton *)pParent->GetDlgItem(IDC_chkRegCan);
	CString sQuery;

	CESL_DataMgr *pDM;	
	pDM = pParent->FindDM(_T("DM_����_����_����_����"));		


	INT iCh;	
	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return;

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = pParent->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//������Ϲ�ȣ �߰� ����
	INT iBookCount = -1; 
	CESL_DataMgr DM_BOOK;

	if ( pAccCan->GetCheck() == 1 ) {	//���ι�ȣ ����
		//���ι�ȣ �ο� üũ
		//CString sQueryCount = _T("select count(accession_rec_key) from BO_BOOK_TBL WHERE species_key in(") + pParent->sRecKey + _T(")");
		CString sQueryCount = _T("select count(accession_rec_key) from BO_BOOK_TBL WHERE rec_key in(") + pParent->sRecKey + _T(")");
		CString sCount;	//���� ��Ϲ�ȣ
		pDM->GetOneValueQuery( sQueryCount , sCount );
		if ( sCount == _T("0") ) {
			AfxMessageBox (_T("���ι�ȣ �ο��� �ڷᰡ �����ϴ�."));
			//m_bInitDlg = FALSE;
			return;
		}

		INT ids;

/*		
		ids = ::MessageBox( pParent->GetSafeHwnd() , _T("�����Ͻ� �ڷ��� ���ι�ȣ�� �����Ͻðڽ��ϱ�?") ,
			_T("�� ��") ,
			MB_YESNO | MB_ICONQUESTION );
*/
		
		ids = MessageBox( m_pParentMgr->m_hWnd , _T("�����Ͻ� �ڷ��� ���ι�ȣ�� �����Ͻðڽ��ϱ�?") ,
			_T("�� ��") ,
			MB_YESNO | MB_ICONQUESTION );
		if ( ids == IDYES ) {
			CString sQuery;
			//���� ���̺� ����
			//sQuery.Format( _T("DELETE FROM CO_ACCESSION_REC_TBL WHERE REC_KEY IN (SELECT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE SPECIES_KEY IN (%s) );") , pParent->sRecKey );
			//iCh = pDM->AddFrame( sQuery );

			//sQuery.Format( _T("UPDATE BO_BOOK_TBL SET ACCESSION_REC_KEY = NULL WHERE SPECIES_KEY IN (%s);") , pParent->sRecKey );
			sQuery.Format( _T("UPDATE BO_BOOK_TBL SET ACCESSION_REC_KEY = NULL WHERE REC_KEY IN (%s);") , pParent->sRecKey );
			iCh = pDM->AddFrame( sQuery );
			if ( 0 > iCh ) return;

			//�׸��忡 ���� �����ϱ�
			for ( INT j = 1 ; j <= pParent->ArrSelectCount.GetSize() ; j ++ ) {				
				//pDM->SetCellData( _T("BB_��Ϲ�ȣ") , _T("") , pParent->ArrSelectCount.GetAt(j-1) );
				pDM->SetCellData( _T("���ι�ȣ") , _T("") , pParent->ArrSelectCount.GetAt(j-1) );

				//pDM->DeleteRow(ArrSelectCount.GetAt(j-1));
			}

		}
		else {//�������
			return;
		}
	}
	//else if ( pRegCan->GetCheck() == 1 && pAccCan->GetCheck() == 1)	//��Ϲ�ȣ ����
	else if ( pRegCan->GetCheck() == 1 )	//��Ϲ�ȣ ����
	{
		//��Ϲ�ȣ �˻��Ͽ� ����		
		DM_BOOK.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
		//CString sQuery_BOOK = _T("SELECT REG_NO FROM BO_BOOK_TBL WHERE species_key in(") + pParent->sRecKey + _T(")");
		CString sQuery_BOOK = _T("SELECT REG_NO FROM BO_BOOK_TBL WHERE rec_key in(") + pParent->sRecKey + _T(")");
		DM_BOOK.RestructDataManager(sQuery_BOOK);
		iBookCount = DM_BOOK.GetRowCount();

		//��Ϲ�ȣ �ο� üũ
		//CString sQueryCount = _T("select count(reg_no) from BO_BOOK_TBL WHERE species_key in(") + sRecKey + _T(")");
		//CString sCount;	//���� ��Ϲ�ȣ
		//pDM->GetOneValueQuery( sQueryCount , sCount );
		//if ( sCount == _T("0") ) {
		if ( iBookCount < 1 ) {
			AfxMessageBox (_T("��Ϲ�ȣ �ο��� �ڷᰡ �����ϴ�."));
			//m_bInitDlg = FALSE;
			return;
		}
		INT ids;// = 6;

		ids = ::MessageBox( pParent->GetSafeHwnd() , _T("�����Ͻ� �ڷ��� ���ι�ȣ, ��Ϲ�ȣ�� �����Ͻðڽ��ϱ�?") ,
			_T("�� ��") ,
			MB_YESNO | MB_ICONQUESTION );
	
		if ( ids == IDYES ) {		
			CString sQuery;

			//å���̺� ����
			sQuery.Format( _T("UPDATE BO_BOOK_TBL SET ACCESSION_REC_KEY = NULL, REG_NO = NULL WHERE REC_KEY IN (%s);") , pParent->sRecKey );
			iCh = pDM->AddFrame( sQuery );	
			if ( 0 > iCh ) return;

			//DM���� ����, array�� ����
			CArray <CString, CString> RemoveListArray;
			for ( INT j = 1 ; j <= pParent->ArrSelectCount.GetSize() ; j ++ ) {
				CString sRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ"), pParent->ArrSelectCount.GetAt(j-1) );
				RemoveListArray.Add( sRegNo );
				pDM->SetCellData( _T("BB_��Ϲ�ȣ") , _T("") , pParent->ArrSelectCount.GetAt(j-1) );
				pDM->SetCellData( _T("���ι�ȣ") , _T("") , pParent->ArrSelectCount.GetAt(j-1) );
					//pDM->DeleteRow(ArrSelectCount.GetAt(j-1));
			}
			
			//��ũ���� ��Ϲ�ȣ ����
			CESL_DataMgr DM_MARC;
			DM_MARC.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
			//CString sQuery_marc = _T("SELECT MARC , REC_KEY FROM BO_SPECIES_TBL WHERE REC_KEY IN (") + pParent->sRecKey + _T(")");
			CString sQuery_marc = _T("SELECT MARC , REC_KEY FROM BO_SPECIES_TBL WHERE REC_KEY IN (") + pParent->sSpeciesKey + _T(")");
			DM_MARC.RestructDataManager(sQuery_marc);
			INT iMarcCount = DM_MARC.GetRowCount();
			for ( INT i = 0 ; i < iMarcCount ; i ++ ) {
				CString sMarc = DM_MARC.GetCellData( i , 0 );
				CMarc m_marc;
				DM_MARC.SetMarcMgr(pParent->m_pInfo->pMarcMgr);
				pParent->m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));

				//decoding
				CString strTagCode, strErrMsg, strHelpMsg;
			
				if(pParent->m_pInfo->pMarcMgr->Decoding(sMarc, &m_marc))
				{
					POSITION pos;
					pos = pParent->m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
					while (pos)
					{
						pParent->m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);	
						//UpdateData(FALSE);
					}	
				}
				//��Ϲ�ȣ �����ϱ�
				for ( INT j = 1 ; j <= RemoveListArray.GetSize() ; j ++ ) {
					CString sRegNo = RemoveListArray.GetAt(j-1);
					pParent->m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("049$l"), sRegNo );
				}
				//pParent->m_pInfo->pMarcMgr->DeleteField( &m_marc , _T("049") );
				CString sRegNo;// = m_pInfo->pMarcMgr->GetItem( &m_marc , _T("��Ϲ�ȣ") );
				pParent->m_pInfo->pMarcMgr->GetItem(&m_marc, _T("049$l"), sRegNo);	
				//encoding	//��ũ�� ����		
				pParent->m_pInfo->pMarcMgr->Encoding(&m_marc, sMarc);
				CString sRecKey = DM_MARC.GetCellData( i , 1 );
				sQuery.Format( _T("UPDATE BO_SPECIES_TBL SET MARC = '%s' WHERE REC_KEY ='%s';") , sMarc , pParent->sSpeciesKey );
				iCh = pDM->AddFrame( sQuery );
				//if ( 0 > iCh ) return;
				if ( 1 != iCh ) {
					AfxMessageBox ( _T("DM_MARC.AddFrame Error") );
					return ;
				}
			}
			//������Ϲ�ȣ�߰�
			//CESL_DataMgr *pDM_DROP;	
			//pDM_DROP = FindDM(_T("DM_BO_CAT_REG_0400_DROP"));	
			CESL_DataMgr pDM_DROP;
			pDM_DROP.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
			//iCh = pDM_DROP.StartFrame();
			for ( i = 1 ; i <= iBookCount ; i ++ ) {				
				CString sRecKey;			
				ids = pDM_DROP.MakeRecKey( sRecKey );
				if ( 0 > ids ) return;
				//=====================================================
				//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 				CString sRegCode = DM_BOOK.GetCellData(i-1,0).Left(2);
// 				INT iNumber = _ttoi(DM_BOOK.GetCellData(i-1,0).Right(10).GetBuffer(0));

				CString sRegCode = DM_BOOK.GetCellData(i-1,0).Left(nRegCodeLength);
				INT iNumber = _ttoi(DM_BOOK.GetCellData(i-1,0).Right(12-nRegCodeLength).GetBuffer(0));
				//=====================================================
				

				CString sQuery;
				sQuery.Format( _T("select count(reg_no) from co_drop_accession_no_tbl WHERE reg_code ='%s' and reg_no =%d AND MANAGE_CODE = UDF_MANAGE_CODE") , sRegCode , iNumber);
				CString sCount;	//���� ��Ϲ�ȣ
				pDM->GetOneValueQuery( sQuery , sCount );
				if ( sCount == _T("0") ) {
					sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO, FIRST_WORK, LAST_WORK, MANAGE_CODE ) VALUES ( %s, '%s', '%s', '%s', %d, '%s', '%s', UDF_MANAGE_CODE );") , sRecKey , _T("MO") , _T("") , sRegCode , iNumber, m_pParentMgr->GetWorkLogMsg(_T("���ε��"),WORK_LOG), m_pParentMgr->GetWorkLogMsg(_T("���ε��"),WORK_LOG) );
//DEL 					sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO, FIRST_WORK, LAST_WORK ) VALUES ( %s, '%s', '%s', '%s', %d, '%s', '%s' );") , sRecKey , _T("MO") , _T("") , sRegCode , iNumber, m_pParentMgr->GetWorkLogMsg(_T("���ε��"),WORK_LOG), m_pParentMgr->GetWorkLogMsg(_T("���ε��"),WORK_LOG) );
					iCh = pDM->AddFrame( sQuery );
					if ( 0 > iCh ) return;
				}
			}
			//iCh = pDM_DROP.SendFrame();
			//iCh = pDM_DROP.EndFrame();
			
		}
		else {//�������
			return;
		}
	}
	else if ( pAcqBack->GetCheck() == 1 ) {
		INT ids;
		ids = ::MessageBox( pParent->GetSafeHwnd() , _T("�����Ͻ� �ڷḦ �������� �Ͻðڽ��ϱ�?") ,
			_T("�� ��") ,
			MB_YESNO | MB_ICONQUESTION );
		if ( ids == IDYES ) {	
			/*
			CButton *pChkRegCan;
			pChkRegCan = (CButton *)GetDlgItem(IDC_chkRegCan);
			*/
			CString sQuery;
			if ( pChkRegCan->GetCheck() == 1 ) {	//��������, ��Ϲ�ȣ(O)	
			//if ( true ) {	
				//��Ϲ�ȣ �ο� üũ
				CString sQueryCount = _T("select count(reg_no) from BO_BOOK_TBL WHERE rec_key in(") + pParent->sRecKey + _T(")");
				CString sCount;	//���� ��Ϲ�ȣ
				pDM->GetOneValueQuery( sQueryCount , sCount );
				if ( sCount == _T("0") ) {
					AfxMessageBox (_T("��Ϲ�ȣ �ο��� �ڷᰡ �����ϴ�."));
					//m_bInitDlg = FALSE;
					return;
				}
				sQuery.Format( _T("UPDATE BO_BOOK_TBL SET WORKING_STATUS = 'BOA141N', CAT_TRANSFER_DATE ='', ACCESSION_REC_KEY = NULL, REG_NO = NULL WHERE REC_KEY IN (%s);") , pParent->sRecKey );
				iCh = pDM->AddFrame( sQuery );
				if ( 0 > iCh ) return;
				
				//sQuery.Format( _T("UPDATE BO_SPECIES_TBL SET WORKING_STATUS = 'BOA141N' , CAT_TRANSFER_DATE = '' WHERE REC_KEY IN (%s);") , pParent->sRecKey );
				//iCh = pDM->AddFrame( sQuery );		

				sQuery.Format( _T("UPDATE IDX_BO_TBL SET WORK_STATUS = 'BOA141N' WHERE REC_KEY IN (%s);") , pParent->sSpeciesKey );
				iCh = pDM->AddFrame( sQuery );
				if ( 0 > iCh ) return;
				//��ũ���� ��Ϲ�ȣ ����				
				CESL_DataMgr DM_MARC;
				DM_MARC.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
				CString sQuery_marc = _T("SELECT MARC , REC_KEY FROM BO_SPECIES_TBL WHERE REC_KEY IN (") + pParent->sSpeciesKey + _T(")");
				DM_MARC.RestructDataManager(sQuery_marc);
				INT iMarcCount = DM_MARC.GetRowCount();
				for ( INT i = 0 ; i < iMarcCount ; i ++ ) {
					CString sMarc = DM_MARC.GetCellData( i , 0 );
					CMarc m_marc;
					DM_MARC.SetMarcMgr(pParent->m_pInfo->pMarcMgr);
					pParent->m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));

					//decoding
					CString strTagCode, strErrMsg, strHelpMsg;
				
					if(pParent->m_pInfo->pMarcMgr->Decoding(sMarc, &m_marc))
					{
						POSITION pos;
						pos = pParent->m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
						while (pos)
						{
							pParent->m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);	
							//UpdateData(FALSE);
						}	
					}
					pParent->m_pInfo->pMarcMgr->DeleteField( &m_marc , _T("049") );
					CString sRegNo;// = m_pInfo->pMarcMgr->GetItem( &m_marc , _T("��Ϲ�ȣ") );
					pParent->m_pInfo->pMarcMgr->GetItem(&m_marc, _T("049$l"), sRegNo);	
					//encoding	//��ũ�� ����		
					pParent->m_pInfo->pMarcMgr->Encoding(&m_marc, sMarc);
					CString sRecKey = DM_MARC.GetCellData( i , 1 );
					sQuery.Format( _T("UPDATE BO_SPECIES_TBL SET MARC = '%s' WHERE REC_KEY ='%s';") , sMarc , pParent->sSpeciesKey );	
					iCh = pDM->AddFrame( sQuery );
					if ( 1 != iCh ) {
						AfxMessageBox ( _T("DM_MARC.AddFrame Error") );
						return ;
					}
				}
				//������Ϲ�ȣ�߰�
				CESL_DataMgr pDM_DROP;
				pDM_DROP.SetCONNECTION_INFO(pDM->CONNECTION_INFO);				
				for ( i = 1 ; i <= iBookCount ; i ++ ) {				
					CString sRecKey;			
					iCh = pDM_DROP.MakeRecKey( sRecKey );
					if ( 0 > iCh ) return;
					//=====================================================
					//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 					CString sRegCode = DM_BOOK.GetCellData(i-1,0).Left(2);
// 					INT iNumber = _ttoi(DM_BOOK.GetCellData(i-1,0).Right(10).GetBuffer(0));
				
					CString sRegCode = DM_BOOK.GetCellData(i-1,0).Left(nRegCodeLength);
					INT iNumber = _ttoi(DM_BOOK.GetCellData(i-1,0).Right(12-nRegCodeLength).GetBuffer(0));
					//=====================================================
					

					CString sQuery;
					sQuery.Format( _T("select count(reg_no) from co_drop_accession_no_tbl WHERE reg_code ='%s' and reg_no =%d AND MANAGE_CODE = UDF_MANAGE_CODE") , sRegCode , iNumber);
					CString sCount;	//���� ��Ϲ�ȣ

					pDM->GetOneValueQuery( sQuery , sCount );
					if ( sCount == _T("0") ) {
						sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO, FIRST_WORK, LAST_WORK, MANAGE_CODE ) VALUES ( %s, '%s', '%s', '%s', %d, '%s', '%s', UDF_MANAGE_CODE );") , sRecKey , _T("MO") , _T("") , sRegCode , iNumber, m_pParentMgr->GetWorkLogMsg(_T("���ε��"),WORK_LOG), m_pParentMgr->GetWorkLogMsg(_T("���ε��"),WORK_LOG) );
//DEL 						sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO, FIRST_WORK, LAST_WORK ) VALUES ( %s, '%s', '%s', '%s', %d, '%s', '%s' );") , sRecKey , _T("MO") , _T("") , sRegCode , iNumber, m_pParentMgr->GetWorkLogMsg(_T("���ε��"),WORK_LOG), m_pParentMgr->GetWorkLogMsg(_T("���ε��"),WORK_LOG) );
						iCh = pDM->AddFrame( sQuery );
						if ( 0 > iCh ) return;
					}
					//sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_NO , REG_CODE , ACCESSION_REG_NO ) VALUES ( %s, '%s', '%s', '%s', %d );") , sRecKey , _T("MO") , _T("") , sRegCode , iNumber );	
					//iCh = pDM->AddFrame( sQuery );				
				}
				for ( INT j = 1 ; j <= pParent->ArrSelectCount.GetSize() ; j ++ ) {				
					pDM->DeleteRow(pParent->ArrSelectCount.GetAt(j-1));
				}
			}
			else {	//��������, ��Ϲ�ȣ(X)
				sQuery.Format( _T("UPDATE BO_BOOK_TBL SET WORKING_STATUS = 'BOA_1140', CAT_TRANSFER_DATE = '' WHERE REC_KEY IN (%s);"), pParent->sRecKey );
				iCh = pDM->AddFrame( sQuery );	
				if ( 0 > iCh ) return;
								
				for ( INT j = 1 ; j <= pParent->ArrSelectCount.GetSize() ; j ++ ) {				
					pDM->DeleteRow(pParent->ArrSelectCount.GetAt(j-1));
				}
			}
		}
		else {	//�������
		
			return;
		
		}
	}
	
	iCh = pDM->SendFrame();
	if ( iCh == -3 || iCh == -1) {
		AfxMessageBox(_T("SendFrame Query Error!!"));
		return;
	}
	iCh = pDM->EndFrame();	
	if ( 0 > iCh ) return;
	pParent->AllControlDisplay(_T("CM_����_����_����_����"));	
	pParent->DestroyWindow();	

EFS_END
}

VOID BO_REG_MGR::AccessionNoGive() {
	
EFS_BEGIN

	CBO_CAT_REG_0400 *pParent = (CBO_CAT_REG_0400*)m_pParentMgr;
	
	CButton *pAccCan;
	pAccCan = (CButton *)pParent->GetDlgItem(IDC_rAccCan);
	CButton *pRegCan;
	pRegCan = (CButton *)pParent->GetDlgItem(IDC_rRegCan);
	CButton *pAcqBack;
	pAcqBack = (CButton *)pParent->GetDlgItem(IDC_rAcqBack);	
	CButton *pChkRegCan;
	pChkRegCan = (CButton *)pParent->GetDlgItem(IDC_chkRegCan);
	CString sQuery;

	CESL_DataMgr *pDM;	
	pDM = pParent->FindDM(_T("DM_����_����_���_����"));		


	INT iCh;	
	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return;

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = pParent->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//������Ϲ�ȣ �߰� ����
	INT iBookCount = -1; 
	CESL_DataMgr DM_BOOK;

	if ( pAccCan->GetCheck() == 1 ) {	//���ι�ȣ ����
		//���ι�ȣ �ο� üũ
		//CString sQueryCount = _T("select count(accession_rec_key) from BO_BOOK_TBL WHERE species_key in(") + pParent->sRecKey + _T(")");
		CString sQueryCount = _T("select count(accession_rec_key) from BO_BOOK_TBL WHERE rec_key in(") + pParent->sRecKey + _T(")");
		CString sCount;	//���� ��Ϲ�ȣ
		pDM->GetOneValueQuery( sQueryCount , sCount );
		if ( sCount == _T("0") ) {
			AfxMessageBox (_T("���ι�ȣ �ο��� �ڷᰡ �����ϴ�."));
			//m_bInitDlg = FALSE;
			return;
		}

		INT ids;

/*		
		ids = ::MessageBox( pParent->GetSafeHwnd() , _T("�����Ͻ� �ڷ��� ���ι�ȣ�� �����Ͻðڽ��ϱ�?") ,
			_T("�� ��") ,
			MB_YESNO | MB_ICONQUESTION );
*/
		
		ids = MessageBox( m_pParentMgr->m_hWnd , _T("�����Ͻ� �ڷ��� ���ι�ȣ�� �����Ͻðڽ��ϱ�?") ,
			_T("�� ��") ,
			MB_YESNO | MB_ICONQUESTION );
		if ( ids == IDYES ) {
			CString sQuery;
			//���� ���̺� ����
			//sQuery.Format( _T("DELETE FROM CO_ACCESSION_REC_TBL WHERE REC_KEY IN (SELECT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE SPECIES_KEY IN (%s) );") , pParent->sRecKey );
			//iCh = pDM->AddFrame( sQuery );

			//sQuery.Format( _T("UPDATE BO_BOOK_TBL SET ACCESSION_REC_KEY = NULL WHERE SPECIES_KEY IN (%s);") , pParent->sRecKey );
			sQuery.Format( _T("UPDATE BO_BOOK_TBL SET ACCESSION_REC_KEY = NULL WHERE REC_KEY IN (%s);") , pParent->sRecKey );
			iCh = pDM->AddFrame( sQuery );
			if ( 0 > iCh ) return;

			//�׸��忡 ���� �����ϱ�
			for ( INT j = 1 ; j <= pParent->ArrSelectCount.GetSize() ; j ++ ) {				
				//pDM->SetCellData( _T("BB_��Ϲ�ȣ") , _T("") , pParent->ArrSelectCount.GetAt(j-1) );
				pDM->SetCellData( _T("���ι�ȣ") , _T("") , pParent->ArrSelectCount.GetAt(j-1) );

				//pDM->DeleteRow(ArrSelectCount.GetAt(j-1));
			}

		}
		else {//�������
			return;
		}
	}
	//else if ( pRegCan->GetCheck() == 1 && pAccCan->GetCheck() == 1)	//��Ϲ�ȣ ����
	else if ( pRegCan->GetCheck() == 1 )	//��Ϲ�ȣ ����
	{
		//��Ϲ�ȣ �˻��Ͽ� ����		
		DM_BOOK.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
		//CString sQuery_BOOK = _T("SELECT REG_NO FROM BO_BOOK_TBL WHERE species_key in(") + pParent->sRecKey + _T(")");
		CString sQuery_BOOK = _T("SELECT REG_NO FROM BO_BOOK_TBL WHERE rec_key in(") + pParent->sRecKey + _T(")");
		DM_BOOK.RestructDataManager(sQuery_BOOK);
		iBookCount = DM_BOOK.GetRowCount();

		//��Ϲ�ȣ �ο� üũ
		//CString sQueryCount = _T("select count(reg_no) from BO_BOOK_TBL WHERE species_key in(") + sRecKey + _T(")");
		//CString sCount;	//���� ��Ϲ�ȣ
		//pDM->GetOneValueQuery( sQueryCount , sCount );
		//if ( sCount == _T("0") ) {
		if ( iBookCount < 1 ) {
			AfxMessageBox (_T("��Ϲ�ȣ �ο��� �ڷᰡ �����ϴ�."));
			//m_bInitDlg = FALSE;
			return;
		}
		INT ids;// = 6;

		ids = ::MessageBox( pParent->GetSafeHwnd() , _T("�����Ͻ� �ڷ��� ���ι�ȣ, ��Ϲ�ȣ�� �����Ͻðڽ��ϱ�?") ,
			_T("�� ��") ,
			MB_YESNO | MB_ICONQUESTION );
	
		if ( ids == IDYES ) {		
			CString sQuery;
			//���� ���̺� ����
			//sQuery.Format( _T("DELETE FROM CO_ACCESSION_REC_TBL WHERE REC_KEY IN (SELECT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE SPECIES_KEY IN (%s) );") , pParent->sRecKey );
			//iCh = pDM->AddFrame( sQuery );

			//å���̺� ����
			//sQuery.Format( _T("UPDATE BO_BOOK_TBL SET ACCESSION_REC_KEY = NULL, REG_NO = NULL WHERE SPECIES_KEY IN (%s);") , pParent->sRecKey );
			sQuery.Format( _T("UPDATE BO_BOOK_TBL SET ACCESSION_REC_KEY = NULL, REG_NO = NULL WHERE REC_KEY IN (%s);") , pParent->sRecKey );
			iCh = pDM->AddFrame( sQuery );	
			if ( 0 > iCh ) return;
			//DM���� ����, array�� ����
			CArray <CString, CString> RemoveListArray;
			for ( INT j = 1 ; j <= pParent->ArrSelectCount.GetSize() ; j ++ ) {
				CString sRegNo = pDM->GetCellData( _T("BB_��Ϲ�ȣ"), pParent->ArrSelectCount.GetAt(j-1) );
				RemoveListArray.Add( sRegNo );
				pDM->SetCellData( _T("BB_��Ϲ�ȣ") , _T("") , pParent->ArrSelectCount.GetAt(j-1) );
				pDM->SetCellData( _T("���ι�ȣ") , _T("") , pParent->ArrSelectCount.GetAt(j-1) );
					//pDM->DeleteRow(ArrSelectCount.GetAt(j-1));
			}
			
			//��ũ���� ��Ϲ�ȣ ����
			CESL_DataMgr DM_MARC;
			DM_MARC.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
			//CString sQuery_marc = _T("SELECT MARC , REC_KEY FROM BO_SPECIES_TBL WHERE REC_KEY IN (") + pParent->sRecKey + _T(")");
			CString sQuery_marc = _T("SELECT MARC , REC_KEY FROM BO_SPECIES_TBL WHERE REC_KEY IN (") + pParent->sSpeciesKey + _T(")");
			DM_MARC.RestructDataManager(sQuery_marc);
			INT iMarcCount = DM_MARC.GetRowCount();
			for ( INT i = 0 ; i < iMarcCount ; i ++ ) {
				CString sMarc = DM_MARC.GetCellData( i , 0 );
				CMarc m_marc;
				DM_MARC.SetMarcMgr(pParent->m_pInfo->pMarcMgr);
				pParent->m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));

				//decoding
				CString strTagCode, strErrMsg, strHelpMsg;
			
				if(pParent->m_pInfo->pMarcMgr->Decoding(sMarc, &m_marc))
				{
					POSITION pos;
					pos = pParent->m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
					while (pos)
					{
						pParent->m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);	
						//UpdateData(FALSE);
					}	
				}
				//��Ϲ�ȣ �����ϱ�
				for ( INT j = 1 ; j <= RemoveListArray.GetSize() ; j ++ ) {
					CString sRegNo = RemoveListArray.GetAt(j-1);
					pParent->m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("049$l"), sRegNo );
				}
				//pParent->m_pInfo->pMarcMgr->DeleteField( &m_marc , _T("049") );
				CString sRegNo;// = m_pInfo->pMarcMgr->GetItem( &m_marc , _T("��Ϲ�ȣ") );
				pParent->m_pInfo->pMarcMgr->GetItem(&m_marc, _T("049$l"), sRegNo);	
				//encoding	//��ũ�� ����		
				pParent->m_pInfo->pMarcMgr->Encoding(&m_marc, sMarc);
				CString sRecKey = DM_MARC.GetCellData( i , 1 );
				sQuery.Format( _T("UPDATE BO_SPECIES_TBL SET MARC = '%s' WHERE REC_KEY ='%s';") , sMarc , pParent->sSpeciesKey );
				iCh = pDM->AddFrame( sQuery );				
				if ( 1 != iCh ) {
					AfxMessageBox ( _T("DM_MARC.AddFrame Error") );
					return ;
				}
			}
			//������Ϲ�ȣ�߰�
			//CESL_DataMgr *pDM_DROP;	
			//pDM_DROP = FindDM(_T("DM_BO_CAT_REG_0400_DROP"));	
			CESL_DataMgr pDM_DROP;
			pDM_DROP.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
			//iCh = pDM_DROP.StartFrame();
			for ( i = 1 ; i <= iBookCount ; i ++ ) {				
				CString sRecKey;			
				iCh = pDM_DROP.MakeRecKey( sRecKey );
				if ( 0 > iCh ) return;
				//=====================================================
				//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 				CString sRegCode = DM_BOOK.GetCellData(i-1,0).Left(2);
// 				INT iNumber = _ttoi(DM_BOOK.GetCellData(i-1,0).Right(10).GetBuffer(0));

				CString sRegCode = DM_BOOK.GetCellData(i-1,0).Left(nRegCodeLength);
				INT iNumber = _ttoi(DM_BOOK.GetCellData(i-1,0).Right(12-nRegCodeLength).GetBuffer(0));
				//=====================================================
				

				CString sQuery;
				sQuery.Format( _T("select count(accession_reg_no) from co_drop_accession_no_tbl WHERE reg_code ='%s' and accession_reg_no =%d AND MANAGE_CODE = UDF_MANAGE_CODE") , sRegCode , iNumber);
				CString sCount;	//���� ��Ϲ�ȣ
				pDM->GetOneValueQuery( sQuery , sCount );
				if ( sCount == _T("0") ) {
					sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_NO , REG_CODE , ACCESSION_REG_NO, FIRST_WORK, LAST_WORK, UDF_MANAGE_CODE ) VALUES ( %s, '%s', '%s', '%s', %d, '%s', '%s', UDF_MANAGE_CODE );") , sRecKey , _T("MO") , _T("") , sRegCode , iNumber, m_pParentMgr->GetWorkLogMsg(_T("���ε��"),WORK_LOG), m_pParentMgr->GetWorkLogMsg(_T("���ε��"),WORK_LOG) );
//DEL 					sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_NO , REG_CODE , ACCESSION_REG_NO, FIRST_WORK, LAST_WORK ) VALUES ( %s, '%s', '%s', '%s', %d, '%s', '%s' );") , sRecKey , _T("MO") , _T("") , sRegCode , iNumber, m_pParentMgr->GetWorkLogMsg(_T("���ε��"),WORK_LOG), m_pParentMgr->GetWorkLogMsg(_T("���ε��"),WORK_LOG) );
					iCh = pDM->AddFrame( sQuery );
					if ( 0 > iCh ) return;
				}
			}
			//iCh = pDM_DROP.SendFrame();
			//iCh = pDM_DROP.EndFrame();
			
		}
		else {//�������
			return;
		}
	}
	else if ( pAcqBack->GetCheck() == 1 ) {
		INT ids;
		ids = ::MessageBox( pParent->GetSafeHwnd() , _T("�����Ͻ� �ڷḦ �������� �Ͻðڽ��ϱ�?") ,
			_T("�� ��") ,
			MB_YESNO | MB_ICONQUESTION );
		if ( ids == IDYES ) {	
			/*
			CButton *pChkRegCan;
			pChkRegCan = (CButton *)GetDlgItem(IDC_chkRegCan);
			*/
			CString sQuery;
			if ( pChkRegCan->GetCheck() == 1 ) {	//��������, ��Ϲ�ȣ(O)	
			//if ( true ) {	
				//��Ϲ�ȣ �ο� üũ
				CString sQueryCount = _T("select count(reg_no) from BO_BOOK_TBL WHERE rec_key in(") + pParent->sRecKey + _T(")");
				CString sCount;	//���� ��Ϲ�ȣ
				pDM->GetOneValueQuery( sQueryCount , sCount );
				if ( sCount == _T("0") ) {
					AfxMessageBox (_T("��Ϲ�ȣ �ο��� �ڷᰡ �����ϴ�."));
					//m_bInitDlg = FALSE;
					return;
				}
				sQuery.Format( _T("UPDATE BO_BOOK_TBL SET WORKING_STATUS = 'BOA141N', CAT_TRANSFER_DATE ='', ACCESSION_REC_KEY = NULL, REG_NO = NULL WHERE REC_KEY IN (%s);") , pParent->sRecKey );
				iCh = pDM->AddFrame( sQuery );	
				if ( 0 > iCh ) return;
				
				//sQuery.Format( _T("UPDATE BO_SPECIES_TBL SET WORKING_STATUS = 'BOA141N' , CAT_TRANSFER_DATE = '' WHERE REC_KEY IN (%s);") , pParent->sRecKey );
				//iCh = pDM->AddFrame( sQuery );		

				sQuery.Format( _T("UPDATE IDX_BO_TBL SET WORK_STATUS = 'BOA141N' WHERE REC_KEY IN (%s);") , pParent->sSpeciesKey );
				iCh = pDM->AddFrame( sQuery );	
				if ( 0 > iCh ) return;
				//��ũ���� ��Ϲ�ȣ ����				
				CESL_DataMgr DM_MARC;
				DM_MARC.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
				CString sQuery_marc = _T("SELECT MARC , REC_KEY FROM BO_SPECIES_TBL WHERE REC_KEY IN (") + pParent->sSpeciesKey + _T(")");
				DM_MARC.RestructDataManager(sQuery_marc);
				INT iMarcCount = DM_MARC.GetRowCount();
				for ( INT i = 0 ; i < iMarcCount ; i ++ ) {
					CString sMarc = DM_MARC.GetCellData( i , 0 );
					CMarc m_marc;
					DM_MARC.SetMarcMgr(pParent->m_pInfo->pMarcMgr);
					pParent->m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));

					//decoding
					CString strTagCode, strErrMsg, strHelpMsg;
				
					if(pParent->m_pInfo->pMarcMgr->Decoding(sMarc, &m_marc))
					{
						POSITION pos;
						pos = pParent->m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
						while (pos)
						{
							pParent->m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);	
							//UpdateData(FALSE);
						}	
					}
					pParent->m_pInfo->pMarcMgr->DeleteField( &m_marc , _T("049") );
					CString sRegNo;// = m_pInfo->pMarcMgr->GetItem( &m_marc , _T("��Ϲ�ȣ") );
					pParent->m_pInfo->pMarcMgr->GetItem(&m_marc, _T("049$l"), sRegNo);	
					//encoding	//��ũ�� ����		
					pParent->m_pInfo->pMarcMgr->Encoding(&m_marc, sMarc);
					CString sRecKey = DM_MARC.GetCellData( i , 1 );
					sQuery.Format( _T("UPDATE BO_SPECIES_TBL SET MARC = '%s' WHERE REC_KEY ='%s';") , sMarc , pParent->sSpeciesKey );	
					iCh = pDM->AddFrame( sQuery );
					if ( 1 != iCh ) {
						AfxMessageBox ( _T("DM_MARC.AddFrame Error") );
						return ;
					}
				}
				//������Ϲ�ȣ�߰�
				CESL_DataMgr pDM_DROP;
				pDM_DROP.SetCONNECTION_INFO(pDM->CONNECTION_INFO);				
				for ( i = 1 ; i <= iBookCount ; i ++ ) {				
					CString sRecKey;			
					iCh = pDM_DROP.MakeRecKey( sRecKey );
					if ( 0 > iCh ) return;
					//=====================================================
					//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 					CString sRegCode = DM_BOOK.GetCellData(i-1,0).Left(2);
// 					INT iNumber = _ttoi(DM_BOOK.GetCellData(i-1,0).Right(10).GetBuffer(0));
					
					CString sRegCode = DM_BOOK.GetCellData(i-1,0).Left(nRegCodeLength);
					INT iNumber = _ttoi(DM_BOOK.GetCellData(i-1,0).Right(12-nRegCodeLength).GetBuffer(0));
					//=====================================================
					

					CString sQuery;
					sQuery.Format( _T("select count(accession_reg_no) from co_drop_accession_no_tbl WHERE reg_code ='%s' and accession_reg_no =%d AND MANAGE_CODE = UDF_MANAGE_CODE") , sRegCode , iNumber);
					CString sCount;	//���� ��Ϲ�ȣ
					pDM->GetOneValueQuery( sQuery , sCount );
					if ( sCount == _T("0") ) {
						sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_NO , REG_CODE , ACCESSION_REG_NO, FIRST_WORK, LAST_WORK, MANAGE_CODE ) VALUES ( %s, '%s', '%s', '%s', %d, '%s', '%s', UDF_MANAGE_CODE );") , sRecKey , _T("MO") , _T("") , sRegCode , iNumber, m_pParentMgr->GetWorkLogMsg(_T("���ε��"),WORK_LOG), m_pParentMgr->GetWorkLogMsg(_T("���ε��"),WORK_LOG) );
//DEL 						sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_NO , REG_CODE , ACCESSION_REG_NO, FIRST_WORK, LAST_WORK ) VALUES ( %s, '%s', '%s', '%s', %d, '%s', '%s' );") , sRecKey , _T("MO") , _T("") , sRegCode , iNumber, m_pParentMgr->GetWorkLogMsg(_T("���ε��"),WORK_LOG), m_pParentMgr->GetWorkLogMsg(_T("���ε��"),WORK_LOG) );
						iCh = pDM->AddFrame( sQuery );
						if ( 0 > iCh ) return;
					}
					//sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_NO , REG_CODE , ACCESSION_REG_NO ) VALUES ( %s, '%s', '%s', '%s', %d );") , sRecKey , _T("MO") , _T("") , sRegCode , iNumber );	
					//iCh = pDM->AddFrame( sQuery );				
				}
				for ( INT j = 1 ; j <= pParent->ArrSelectCount.GetSize() ; j ++ ) {				
					pDM->DeleteRow(pParent->ArrSelectCount.GetAt(j-1));
				}
			}
			else {	//��������, ��Ϲ�ȣ(X)
				sQuery.Format( _T("UPDATE BO_BOOK_TBL SET WORKING_STATUS = 'BOA141N', CAT_TRANSFER_DATE = '' WHERE REC_KEY IN (%s);"), pParent->sRecKey );
				iCh = pDM->AddFrame( sQuery );
				if ( 0 > iCh ) return;
				
				//sQuery.Format( _T("UPDATE BO_SPECIES_TBL SET WORKING_STATUS = 'BOA141N' , CAT_TRANSFER_DATE = '' WHERE REC_KEY IN (%s);") , pParent->sRecKey );
				//iCh = pDM->AddFrame( sQuery );	
				
				sQuery.Format( _T("UPDATE IDX_BO_TBL SET WORK_STATUS = 'BOA141N' WHERE REC_KEY IN (%s);") , pParent->sSpeciesKey );
				iCh = pDM->AddFrame( sQuery );
				if ( 0 > iCh ) return;
				
				for ( INT j = 1 ; j <= pParent->ArrSelectCount.GetSize() ; j ++ ) {				
					pDM->DeleteRow(pParent->ArrSelectCount.GetAt(j-1));
				}
			}
		}
		else {	//�������
		
			return;
		
		}
	}
	
	iCh = pDM->SendFrame();
	if ( iCh == -3 || iCh == -1) {
		AfxMessageBox(_T("SendFrame Query Error!!"));
		return;
	}
	iCh = pDM->EndFrame();
	if ( 0 > iCh ) return;
	pParent->AllControlDisplay(_T("CM_����_����_���_����"));	
	pParent->DestroyWindow();	

EFS_END
}
