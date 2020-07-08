// BO_CAT_DUP_FREE2.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_DUP_FREE2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_DUP_FREE dialog


CBO_CAT_DUP_FREE::CBO_CAT_DUP_FREE(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_DUP_FREE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_DUP_FREE::~CBO_CAT_DUP_FREE()
{
}


BOOL CBO_CAT_DUP_FREE::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_DUP_FREE::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_DUP_FREE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_DUP_FREE, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_DUP_FREE)
	ON_BN_CLICKED(IDC_bDupFree, OnbDupFree)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_DUP_FREE message handlers
VOID CBO_CAT_DUP_FREE::SetParentInfo(CString strDMAlias, CString strCMAlias, CString strGridAlias)
{
	m_strParentDMAlias = strDMAlias;
	m_strParentCMAlias = strCMAlias;
	m_strParentGridAlias = strGridAlias;
}

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_DUP_FREE message handlers

BOOL CBO_CAT_DUP_FREE::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// Free ���� ����
	m_strFreeFlag = _T("N");

	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_DUP_FREE")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	if (m_strParentCMAlias.IsEmpty())
		m_strParentCMAlias = _T("CM_BO_CAT_PROC_0000");
	if (m_strParentDMAlias.IsEmpty())
		m_strParentDMAlias = _T("DM_BO_CAT_PROC_0000");
	if (m_strParentGridAlias.IsEmpty())
		m_strParentGridAlias = _T("ProcGrid");

	INT idx;
	SelectMakeList(m_strParentCMAlias, m_strParentGridAlias);
	idx = SelectGetHeadPosition(m_strParentCMAlias, m_strParentGridAlias);

	if ( idx < 0 ) {
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));	// ����
		EndDialog(IDCANCEL);
		//m_bInitDlg = FALSE;
		//return 0;
		return FALSE;
	}

	CESL_DataMgr *pDM = FindDM( m_strParentDMAlias );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0000 error") );
		return FALSE;
	}

	CString sRecKey;
	while ( idx != -1 ) {
		CString sDupFlag = pDM->GetCellData( _T("BS_��������"), idx );
		CString sKey;
		if ( sDupFlag == _T("V") ) {
			if ( sRecKey == _T("") ) {
				sRecKey = pDM->GetCellData( _T("BS_��KEY") , idx );
			}
			else {
				sKey = pDM->GetCellData( _T("BS_��KEY") , idx );
				sRecKey = sRecKey + _T(", ") + sKey;
			}
		}		
		idx = SelectGetNext(m_strParentCMAlias, m_strParentGridAlias);		
	} 	
	
	if ( sRecKey == _T("") ) {
		AfxMessageBox( _T("���������� �ڷᰡ �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	

	CESL_DataMgr *pDM_ORI = FindDM( _T("DM_BO_CAT_DUP_FREE_ORI") );
	if ( pDM_ORI == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_DUP_FREE_ORI error") );
		return FALSE;
	}

	CString sWhere;
	sWhere.Format( _T("I.REC_KEY = S.REC_KEY AND I.REC_KEY IN ( %s )"), sRecKey );
	
	if (pDM_ORI->RefreshDataManager(sWhere) < 0)	return FALSE;
	INT iRowCount = pDM_ORI->GetRowCount();
	if ( pDM_ORI->GetRowCount() < 1 ) {
		AfxMessageBox (_T("�˻��� ����� �����ϴ�."));
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_DUP_FREE"));
	if ( pCM == NULL ) {
		AfxMessageBox ( _T("CM_BO_CAT_DUP_FREE error") );
		return FALSE;
	}
	pCM->AllControlDisplay();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  

EFS_END
return FALSE;

}

BEGIN_EVENTSINK_MAP(CBO_CAT_DUP_FREE, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_CAT_DUP_FREE)
	ON_EVENT(CBO_CAT_DUP_FREE, IDC_GRD_ORI, -600 /* Click */, OnClickGrdOri, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_CAT_DUP_FREE::OnClickGrdOri() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_CAT_DUP_FREE_ORI") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_DUP_FREE_ORI error") );
		return;
	}

	CESL_ControlMgr *pCM_SUB;
	pCM_SUB = FindCM(_T("CM_BO_CAT_DUP_FREE"));
	pCM_SUB->FindControl(_T("�׸���_������"));
	
	CESL_Grid * pGrid_sub = NULL;
	pGrid_sub = (CESL_Grid*)pCM_SUB->FindControl(_T("�׸���_������"));

	//pGrid_sub->SetRows(iRowCount+1);
	INT iRow = pGrid_sub->GetRow();
	if ( iRow < 1 ) {
		return;
	}
	INT iCol = pGrid_sub->GetCol();

	CString sDupSpeciesKey;
	if ( iCol == 1 ) {
		sDupSpeciesKey = pDM->GetCellData( _T("BS_��KEY"), iRow-1 );
		m_sStreamMarc = pDM->GetCellData( _T("BS_MARC"), iRow-1 ); 
	}
	else {
		return;
	}
	m_sSpeciesKeyOri = sDupSpeciesKey;

	CESL_DataMgr *pDM_DUP = FindDM( _T("DM_BO_CAT_DUP_FREE_DUP") );
	if ( pDM_DUP == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_DUP_FREE_DUP error") );
		return;
	}

	CString sWhere;

	sWhere.Format( _T("I.REC_KEY = S.REC_KEY AND S.REC_KEY IN ( SELECT DUP_SPECIES_KEY FROM BO_BOOK_TBL WHERE SPECIES_KEY = %s AND DUP_SPECIES_KEY IS NOT NULL)"), m_sSpeciesKeyOri );

	if (pDM_DUP->RefreshDataManager(sWhere) < 0)	return;
	INT iRowCount = pDM_DUP->GetRowCount();
	if ( pDM_DUP->GetRowCount() < 1 ) {
		AfxMessageBox (_T("�˻��� ����� �����ϴ�."));
	}

	/* 040220_HSS ������ ��Key ���� �߰� (DUP_SPECIES_KEY = BS_��KEY) */
	CString sQueryCount;
	CString sSpeciesKey;
	CString sStartRegNo;
	for ( INT i = 0 ; i < pDM_DUP->GetRowCount() ; i++ ) {	
		pDM_DUP->GetCellData(_T("BS_��KEY"), i, sSpeciesKey);
		//===================================================
		//2009.03.06 UPDATE BY PJW : �ּ� ��Ϲ�ȣ �����ö� å������ �����´�.
// 		sQueryCount.Format(_T("SELECT MIN(REG_NO) FROM bo_book_tbl WHERE SPECIES_KEY = %s AND DUP_SPECIES_KEY = %s"), m_sSpeciesKeyOri, sSpeciesKey);
		sQueryCount.Format(_T("SELECT MIN(REG_NO) FROM bo_book_tbl WHERE SPECIES_KEY = %s AND DUP_SPECIES_KEY = %s AND BOOK_APPENDIX_FLAG = 'B'"), m_sSpeciesKeyOri, sSpeciesKey);
		//===================================================
		
		pDM_DUP->GetOneValueQuery( sQueryCount , sStartRegNo );
		pDM_DUP->SetCellData( _T("���۵�Ϲ�ȣ"), sStartRegNo, i );
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_DUP_FREE"));
	if ( pCM == NULL ) {
		AfxMessageBox ( _T("CM_BO_CAT_DUP_FREE error") );
		return;
	}
	pCM->AllControlDisplay();


EFS_END
}

VOID CBO_CAT_DUP_FREE::OnbDupFree() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CString sQuery;
	INT ids, idx;
	INT nFreeDupCnt, nDupCnt;	// ������ �ڷ� �� / ���� �ڷ� ��ü ��
	CString sDupRecKeyList;		// ������ �ڷ� ��Ű List - key1,key2,key3,,,
	
	// *** �⺻ ���� ���� ********************************************
	SelectMakeList(_T("CM_BO_CAT_DUP_FREE"), _T("�׸���_������"));
	idx = SelectGetHeadPosition(_T("CM_BO_CAT_DUP_FREE"), _T("�׸���_������"));

	if ( idx < 0 ) 
	{
		AfxMessageBox(_T("�������ڷḦ �����ϼ���"));	// ����
		return;
	}

	CESL_DataMgr *pDM_DUP = FindDM( _T("DM_BO_CAT_DUP_FREE_DUP") );
	if ( pDM_DUP == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_DUP_FREE_DUP error") );
		return;
	}

	// ���� �ڷ� ��ü �� 
	nDupCnt = pDM_DUP->GetRowCount();

	// ������ �ڷ� ��Ű List,  ������ �ڷ� ��
	nFreeDupCnt = 0;
	sDupRecKeyList = _T("");
	while ( idx != -1 ) 
	{
		if (sDupRecKeyList.GetLength() > 0) sDupRecKeyList += _T(",");
		sDupRecKeyList += pDM_DUP->GetCellData( _T("BS_��KEY") , idx );

		idx = SelectGetNext(_T("CM_BO_CAT_DUP_FREE"), _T("�׸���_������"));
		nFreeDupCnt++;
	}

	if ( sDupRecKeyList.GetLength()==0 ){
		AfxMessageBox(_T("������ �����ڷ��� ������ ������ �ֽ��ϴ�."));	// ����
		return;
	}
	// ****************************************************************

	// *** ��ũ ���� 049tag *******************************************
	INT nBookCnt;			// �˻��� å �� - ���� �� ���� �ڷ� ��
	CMarc m_marc;			// ��ũ

	//å DM ����� ��Ϲ�ȣ �����ϱ� - ������ �ڷ�� ����
	CESL_DataMgr *pDmBookMain = FindDM( _T("DM_BO_CAT_DUP_FREE_å") );
	if ( pDmBookMain == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_DUP_FREE_å error") );
		return;
	}
	
	CString strWhere = _T("species_key = ") + m_sSpeciesKeyOri + _T(" and (DUP_SPECIES_KEY is NULL or DUP_SPECIES_KEY not in (") + sDupRecKeyList + _T("))");
	if ( pDmBookMain->RefreshDataManager(strWhere) < 0 ) return;
	nBookCnt = pDmBookMain->GetRowCount();
	if (nBookCnt<=0) {
		AfxMessageBox( _T("�����ڷᰡ �����ϴ�. ���������� �����Ͽ����ϴ�.") );
		return;
	}

	// 049 ��ü ���� (��Ϲ�ȣ, ��å��ȣ, ������ȣ, ��ġ��ȣ)
	m_pInfo->pMarcMgr->Decoding( m_sStreamMarc, &m_marc );
	ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("049$l") );
	ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("049$v") );
	ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("049$c") );
	ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("049$f") );

	CString sVolNoPre;
	CString sRegNo = _T("");		//�� �Ǻ� ��Ϲ�ȣ
	CString sItemData = _T("");		// tmp Data
	CString sShelfCode = _T("");	//��ġ��ȣ
	CString sPreShelfCode = _T("");	//���� ��ġ��ȣ
	INT iSwShelfCode = 0;		// ��ġ��ȣ flag
	CString s049field = _T("");			// 049 tag ��ü 
	
	for ( INT i = 0 ; i < nBookCnt ; i ++ ) {
		ids = pDmBookMain->GetCellData( _T("BB_��Ϲ�ȣ"), i, sRegNo );		
		
		if ( sRegNo != _T("") ) {
			// ��ġ��ȣ Ȯ�� - ���� �� ���ñ�ȣ 0 -> 1 �� �ٲپ� �ֱ�
			sShelfCode = _T("");
			ids = pDmBookMain->GetCellData( _T("BB_��ġ��ȣ_�ڵ�"), i, sShelfCode );
			if ( i != 0 && iSwShelfCode == 0 && sShelfCode != _T("")
				&& sPreShelfCode != _T("") && sPreShelfCode != sShelfCode	) {
				m_pInfo->pMarcMgr->GetField( &m_marc, _T("049"), s049field );
				s049field = _T("1") + s049field.Mid(1);
				m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("049") );
				m_pInfo->pMarcMgr->InsertField( &m_marc, _T("049") + s049field );
				iSwShelfCode = 1;
			}else sPreShelfCode = sShelfCode;

			// ��Ϲ�ȣ insert
			m_pInfo->pMarcMgr->SetItem( &m_marc, _T("049$l"), sRegNo );
			// ��å��ȣ
			ids = pDmBookMain->GetCellData( _T("BB_��å��ȣ"), i, sItemData );
			if ( sItemData != _T("") )
				m_pInfo->pMarcMgr->SetItem( &m_marc, _T("049$v"), sItemData );

			// ������ȣ
			ids = pDmBookMain->GetCellData( _T("BB_������ȣ"), i, sItemData );
			if ( sItemData != _T("") ) 
				m_pInfo->pMarcMgr->SetItem( &m_marc, _T("049$c"), sItemData );

			// ��ġ��ȣ
			if ( sShelfCode != _T("") )
				ids = m_pInfo->pMarcMgr->SetItem( &m_marc, _T("049$f"), sShelfCode );
		}		
	}

	//049 �ֱ�
	if ( iSwShelfCode == 0 ) {
		m_pInfo->pMarcMgr->GetField( &m_marc, _T("049"), s049field );
		if ( s049field != _T("") ) {
			s049field = _T("0") + s049field.Mid(1);
			m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("049") );
			m_pInfo->pMarcMgr->InsertField( &m_marc, _T("049") + s049field );
			for ( INT i = 0 ; i < nBookCnt ; i ++ ) {
				ids = pDmBookMain->GetCellData( _T("BB_��ġ��ȣ_�ڵ�"), i, sShelfCode );
				if ( sShelfCode != _T("") )
				{
					ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("049$f"), sShelfCode );
					ids = m_pInfo->pMarcMgr->SetItem( &m_marc, _T("049$f"), sShelfCode );
					break;
				}
			}
		}
	}

	m_pInfo->pMarcMgr->SortMarc( &m_marc );
	m_pInfo->pMarcMgr->Encoding(&m_marc, m_sStreamMarc);	
	// ****************************************************************
	
	// Update Frame Making, Send **************************************
	// StartFrame
	ids = pDM_DUP->StartFrame();
	if ( 0 > ids ) return;

	// Making ���������ڷ� å Update Frame
	//CString sStatus;			// �����ڷ� ����	-> �������� å������ �־��ֱ�
	CString sDupRecKey;			// �����ڷ� ��Ű
	CString		strOriStreamMarc; // �����ڷ� ��ũ
	BOOL		bDupSpeciesUpdate;
	CString		strDupMediaCode; // ��ü�����ڵ带 �߰���


	bDupSpeciesUpdate = FALSE;

	SelectMakeList(_T("CM_BO_CAT_DUP_FREE"), _T("�׸���_������"));
	idx = SelectGetHeadPosition(_T("CM_BO_CAT_DUP_FREE"), _T("�׸���_������"));
	while ( idx != -1 ) 
	{
		sDupRecKey = pDM_DUP->GetCellData( _T("BS_��KEY") , idx );
		//===================================================
		//2009.05.25 ADD BY PJW : ��ü�����ڵ嵵 UPDATE���ش�.
		strDupMediaCode = pDM_DUP->GetCellData( _T("IBS_��ü����_�ڵ�") , idx );
		//===================================================
		//sStatus = pDM_DUP->GetCellData( _T("IBS_�۾�����INFO"), idx );
		//sStatus = _T("BOC111O");		// ���������� �׻� �������� ����ڷ� ���·� �ǵ�����
		
		// å
		/*
		sQuery.Format( _T("UPDATE BO_BOOK_TBL SET WORKING_STATUS = '%s', SPECIES_KEY = %s")
						_T(", DUP_SPECIES_KEY = NULL, LAST_WORK='%s' WHERE DUP_SPECIES_KEY = %s and SPECIES_KEY = %s;")
					, sStatus, sDupRecKey, GetWorkLogMsg(_T("��������"),WORK_LOG), sDupRecKey, m_sSpeciesKeyOri );
					*/
		//===================================================
		//2009.05.27 ADD BY PJW : ��ü�����ڵ嵵 UPDATE���ش�.
//		sQuery.Format( _T("UPDATE BO_BOOK_TBL SET SPECIES_KEY = %s")
//						_T(", DUP_SPECIES_KEY = NULL, LAST_WORK='%s' WHERE DUP_SPECIES_KEY = %s and SPECIES_KEY = %s;")
//					, sDupRecKey, GetWorkLogMsg(_T("��������"),WORK_LOG), sDupRecKey, m_sSpeciesKeyOri );
		sQuery.Format( _T("UPDATE BO_BOOK_TBL SET SPECIES_KEY = %s, MEDIA_CODE = '%s'")
						_T(", DUP_SPECIES_KEY = NULL, LAST_WORK='%s' WHERE DUP_SPECIES_KEY = %s and SPECIES_KEY = %s;")
					, sDupRecKey, strDupMediaCode, GetWorkLogMsg(_T("��������"),WORK_LOG), sDupRecKey, m_sSpeciesKeyOri );
		//===================================================

		ids = pDM_DUP->AddFrame(sQuery);
		if ( 0 > ids ) return;

		// 2005.10.28 ADD BY PDJ
		// ���������� 049 ������°��( ���� ����������� �ʾ��� )
		// �����ڷ��� ���� 049 ������ ����
		// ��, 049�� ���� ���.		
		CMarc		marc;		
		CString		strOriMarc;
		CString		strOriKey;		
		INT			nRowCount;
		BOOL		bFind;

		CESL_DataMgr *pDM_ORI_BOOK = FindDM( _T("DM_BO_CAT_DUP_FREE_ORI_å") );
		if ( pDM_ORI_BOOK == NULL ) {
			AfxMessageBox( _T("DM_BO_CAT_DUP_FREE_ORI_å error") );
			return;
		}

		nRowCount = pDM_DUP->GetRowCount();
		bFind = FALSE;
		if ( nRowCount > 0 )
		{
			for ( i=0 ; i<nRowCount ; i++ )
			{
				strOriKey = pDM_DUP->GetCellData( _T("BS_��KEY"), i ); 
				if ( sDupRecKey == strOriKey ) 
				{
					strOriMarc = pDM_DUP->GetCellData( _T("BS_MARC"), i ); 				
					bFind = TRUE;
					break;
				}
			}
			if ( bFind == TRUE )
			{
				s049field=_T("");
				m_pInfo->pMarcMgr->Decoding(strOriMarc, &marc);
				bDupSpeciesUpdate = m_pInfo->pMarcMgr->GetField (&marc,_T("049"),s049field);
				if ( s049field.IsEmpty() ) 
				{
					bDupSpeciesUpdate = TRUE;					
					
					sQuery.Format( _T("DUP_SPECIES_KEY = %s and SPECIES_KEY = %s")
						, sDupRecKey, m_sSpeciesKeyOri );
					pDM_ORI_BOOK->RefreshDataManager(sQuery);
					
					nBookCnt = pDM_ORI_BOOK->GetRowCount();
					s049field = _T("");
					if ( nBookCnt > 0 )
					{
						sVolNoPre=_T("");
						sRegNo = _T("");		//�� �Ǻ� ��Ϲ�ȣ
						sItemData = _T("");		// tmp Data
						sShelfCode = _T("");	//��ġ��ȣ
						sPreShelfCode = _T("");	//���� ��ġ��ȣ
						iSwShelfCode = 0;		// ��ġ��ȣ flag
						s049field = _T("");			// 049 tag ��ü 
						
						for ( INT i = 0 ; i < nBookCnt ; i ++ ) {
							ids = pDM_ORI_BOOK->GetCellData( _T("BB_��Ϲ�ȣ"), i, sRegNo );		
							
							if ( sRegNo != _T("") ) {
								// ��ġ��ȣ Ȯ�� - ���� �� ���ñ�ȣ 0 -> 1 �� �ٲپ� �ֱ�
								sShelfCode = _T("");
								ids = pDM_ORI_BOOK->GetCellData( _T("BB_��ġ��ȣ_�ڵ�"), i, sShelfCode );
								if ( i != 0 && iSwShelfCode == 0 && sShelfCode != _T("")
									&& sPreShelfCode != _T("") && sPreShelfCode != sShelfCode	) {
									m_pInfo->pMarcMgr->GetField( &m_marc, _T("049"), s049field );
									s049field = _T("1") + s049field.Mid(1);
									m_pInfo->pMarcMgr->DeleteField( &marc, _T("049") );
									m_pInfo->pMarcMgr->InsertField( &marc, _T("049") + s049field );
									iSwShelfCode = 1;
								}else sPreShelfCode = sShelfCode;
								
								// ��Ϲ�ȣ insert
								m_pInfo->pMarcMgr->SetItem( &marc, _T("049$l"), sRegNo );
								// ��å��ȣ
								ids = pDM_ORI_BOOK->GetCellData( _T("BB_��å��ȣ"), i, sItemData );
								if ( sItemData != _T("") )
									m_pInfo->pMarcMgr->SetItem( &marc, _T("049$v"), sItemData );
								
								// ������ȣ
								ids = pDM_ORI_BOOK->GetCellData( _T("BB_������ȣ"), i, sItemData );
								if ( sItemData != _T("") ) 
									m_pInfo->pMarcMgr->SetItem( &marc, _T("049$c"), sItemData );
								
								// ��ġ��ȣ
								if ( sShelfCode != _T("") )
									ids = m_pInfo->pMarcMgr->SetItem( &marc, _T("049$f"), sShelfCode );
							}		
						} // END FOR
						
						//049 �ֱ�
						if ( iSwShelfCode == 0 ) {
							m_pInfo->pMarcMgr->GetField( &marc, _T("049"), s049field );
							if ( s049field != _T("") ) {
								s049field = _T("0") + s049field.Mid(1);
								m_pInfo->pMarcMgr->DeleteField( &marc, _T("049") );
								m_pInfo->pMarcMgr->InsertField( &marc, _T("049") + s049field );
								for ( INT i = 0 ; i < nBookCnt ; i ++ ) {
									ids = pDM_ORI_BOOK->GetCellData( _T("BB_��ġ��ȣ_�ڵ�"), i, sShelfCode );
									if ( sShelfCode != _T("") )
									{
										ids = m_pInfo->pMarcMgr->DeleteItem( &marc, _T("049$f"), sShelfCode );
										ids = m_pInfo->pMarcMgr->SetItem( &marc, _T("049$f"), sShelfCode );
										break;
									}
								}// END FOR
							}
						} // END IF
					} // END IF
					
					m_pInfo->pMarcMgr->SortMarc( &marc );
					m_pInfo->pMarcMgr->Encoding(&marc, strOriStreamMarc);
				}
			} // END IF
		}
		//			

		// ��
		sQuery.Format( _T("UPDATE IDX_BO_TBL SET WORKING_STATUS = '%s' WHERE REC_KEY=%s;")
						, _T("BOC113N"), sDupRecKey);
		ids = pDM_DUP->AddFrame(sQuery);
		if ( 0 > ids ) return;

		// tmp ���̺� insert
		CString sDate;
		CTime t = CTime::GetCurrentTime();
		sDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
		
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL ������ ���� �ּ�ó����
// 		sQuery.Format(_T("INSERT INTO IDX_TMP_BO_TBL (REC_KEY, SPECIES_KEY, IDX_WORK_FLAG, INPUT_DATE) ")
// 						_T("VALUES (ESL_SEQ.NEXTVAL, %s, 'I', '%s');"),
// 						sDupRecKey, sDate);
// 		ids = pDM_DUP->AddFrame(sQuery);
		//=====================================================
		

		// ���� �������� Tbl �� ���� +++
		sQuery.Format(_T("UPDATE BO_PURCHASEINFO_TBL SET SPECIES_KEY=%s WHERE REC_KEY IN ")
		_T("(SELECT ACQ_KEY FROM BO_BOOK_TBL WHERE SPECIES_KEY=%s);"),
		sDupRecKey, sDupRecKey);
		pDM_DUP->AddFrame(sQuery);

		sQuery.Format(_T("UPDATE BO_DONINFO_TBL SET SPECIES_KEY=%s WHERE REC_KEY IN ")
			_T("(SELECT ACQ_KEY FROM BO_BOOK_TBL WHERE SPECIES_KEY=%s);"),
			sDupRecKey, sDupRecKey);
		pDM_DUP->AddFrame(sQuery);
		// ���� �������� Tbl �� ���� ---
		//======================================================
		//20080527 ADD BY PJW : ���������� ��ǥå�� �������ش�.
		sQuery.Format(_T("UPDATE BO_BOOK_TBL B SET B.REPRESENT_BOOK_YN = 'Y' WHERE  B.REC_KEY IN ")
					  _T("(SELECT MIN(REC_KEY) FROM BO_BOOK_TBL B1 WHERE  SPECIES_KEY = %s AND B1.VOL_SORT_NO IN ")
					  _T("(SELECT MIN(VOL_SORT_NO) FROM   BO_BOOK_TBL B2 WHERE  B1.REC_KEY = B2.REC_KEY ));"),
					  sDupRecKey);
		pDM_DUP->AddFrame(sQuery);
		//======================================================

		idx = SelectGetNext(_T("CM_BO_CAT_DUP_FREE"), _T("�׸���_������"));		
	}

	// 2005.10.28 ADD BY PDJ
	// ������ ��ũ ������Ʈ
	// 049�� �����ؾ� �Ǵ� ����̸� ������ ��ũ�� ������Ʈ
	if  (bDupSpeciesUpdate == TRUE )
	{
		pDM_DUP->InitDBFieldData();	
		pDM_DUP->AddDBFieldData(_T("MARC"), _T("STRING"), strOriStreamMarc,1);	
		ids = pDM_DUP->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sDupRecKey );
		if ( 0 > ids ) return;
	}

	// ������ ��ũ ������Ʈ
	pDM_DUP->InitDBFieldData();	
	pDM_DUP->AddDBFieldData(_T("MARC"), _T("STRING"), m_sStreamMarc,1);	
	if ( nDupCnt == nFreeDupCnt ) pDM_DUP->AddDBFieldData( _T("DUP_FLAG"), _T("STRING"), _T(""), 1 );
	pDM_DUP->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("��������"), WORK_LOG), 1 );	

	ids = pDM_DUP->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), m_sSpeciesKeyOri );
	if ( 0 > ids ) return;

	// 2006.07.20 Ȳ���� ����: ���� �����ڵ� �籸�� : 
	sQuery.Format(_T(" update idx_bo_tbl set USE_LIMIT_CODE=GET_BO_BOOK_INFO('%s', 'USE_LIMIT_CODE'),")
				  _T("MANAGE_CODE=GET_BO_BOOK_INFO('%s', 'MANAGE_CODE'),")
				  _T("REG_CODE=GET_BO_BOOK_INFO('%s', 'REG_CODE'),")
				  _T("IDX_VOL_TITLE=GET_BO_BOOK_INFO('%s', 'VOL_TITLE_INDEX'),")
				  _T("PLACE_INFO=GET_BO_BOOK_INFO('%s', 'SHELF_LOC_CODE'),")
				  _T("IDX_VOL=GET_BO_BOOK_INFO('%s', 'VOL_INDEX'),")
				  _T("IDX_SE_SHELF_CODE=GET_BO_BOOK_INFO('%s', 'SEPARATE_SHELF_CODE'), ")
				  _T("DUP_YN='N' ")
				  _T("where rec_key='%s';")
				  ,m_sSpeciesKeyOri,m_sSpeciesKeyOri,m_sSpeciesKeyOri,m_sSpeciesKeyOri,m_sSpeciesKeyOri
				  ,m_sSpeciesKeyOri,m_sSpeciesKeyOri,m_sSpeciesKeyOri);
	ids = pDM_DUP->AddFrame(sQuery);
	if ( 0 > ids ) return;

	

	// SendFrame 
	ids = pDM_DUP->SendFrame();
	if ( ids < 0 ) {
		AfxMessageBox( _T("DB Update Error!") );
		return;
	}
	else {
		AfxMessageBox( _T("�������� �Ϸ�."));
		m_strFreeFlag = _T("Y");
	}

	// EndFrame
	ids = pDM_DUP->EndFrame();
	if ( 0 > ids ) return;
	// ****************************************************************

	CDialog::OnOK();

EFS_END
}


HBRUSH CBO_CAT_DUP_FREE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}