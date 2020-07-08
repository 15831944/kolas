// BO_CAT_PROC_1100.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_PROC_1100.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_1100 dialog


CBO_CAT_PROC_1100::CBO_CAT_PROC_1100(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_1100)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CBO_CAT_PROC_1100::~CBO_CAT_PROC_1100()
{
	
}


VOID CBO_CAT_PROC_1100::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_1100)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_1100, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_1100)
	ON_BN_CLICKED(IDC_bFILEOPEN, OnbFILEOPEN)
	ON_BN_CLICKED(IDC_bMAKE_FILE, OnbMAKEFILE)
	ON_BN_CLICKED(IDC_chkDivision, OnchkDivision)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_1100 message handlers

BOOL CBO_CAT_PROC_1100::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}
BOOL CBO_CAT_PROC_1100::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_1100")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CButton * pMarcInfo;
	pMarcInfo=(CButton *)GetDlgItem(IDC_chkMarcInfo);	
	pMarcInfo->SetCheck(1);

	CButton * pGeneralMarc;
	pGeneralMarc = (CButton *)GetDlgItem(IDC_rGeneralMarc);
	pGeneralMarc->SetCheck(1);

	//INT ch, drive, curdrive;	
	INT curdrive;
	/* Save current drive. */
	curdrive = _getdrive();
	CString sPath;
	if(curdrive==3){
		sPath=_T("C:\\KOLAS2UP\\BIN\\����_AV.TXT");
	}
	if(curdrive==4){
		sPath=_T("D:\\KOLAS2UP\\BIN\\����_AV.TXT");
	}
	if(curdrive==5){
		sPath=_T("E:\\KOLAS2UP\\BIN\\����_AV.TXT");
	}

	sPath = _T("..\\BIN\\����_AV.TXT");
	CString sFileName;
	//GetControlData( _T("CM_BO_CAT_PROC_1100"), _T("���ϸ�"), sFileName );
	SetControlData( _T("CM_BO_CAT_PROC_1100"), _T("���ϸ�"), sPath );

	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_1100::OnbFILEOPEN() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CFileDialog dlg(FALSE, _T("*.*"), _T("*.TXT"),
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    _T("��������(*.txt)|All Files(*.*)|*.*||") );
    if ( IDOK != dlg.DoModal() ) return;

    //_tchdir( sCurrentDirectory );

    // ���ϸ��� ���õ����ʰ� 'Ȯ��'�� click���� ���.
	CString sFileName;
    sFileName = dlg.GetFileTitle();
    if ( sFileName.IsEmpty() ) return;
    sFileName = dlg.GetPathName();	

	SetControlData( _T("CM_BO_CAT_PROC_1100"), _T("���ϸ�"), sFileName );

EFS_END
}

VOID CBO_CAT_PROC_1100::OnbMAKEFILE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	//if ( IDNO == AfxMessageBox( _T("������ �ۼ��Ͻðڽ��ϱ�?") , MB_YESNO) ) return;

	//=================================================================
    // File Check / Open
    //=================================================================
    //TCHAR file_name[512];
    CStdioFile file, ref_file;
//	INT ids;
    //ids = ECO_GetDialogManagerData( '.', _T("CTRL_�⺻ȭ��"), _T("���ϸ�"), -1, file_name );
    //if (ids) CHECKERROR_RETURN(_T("ECO_GetDialogManagerData() Error!!!"), FALSE);
/*
    if ( _tcsclen( file_name ) <= 0 ) {
        _stprintf( msg, _T("���ϸ��� �Է��ؾ� �մϴ�.") );
        CHECKERROR_RETURN(msg, FALSE);
    }

    if ( !file.Open( file_name, CFile::modeCreate | CFile::modeWrite ) ) {
        _stprintf( msg, _T("���ϸ��� �߸��Ǿ����ϴ�.\n�ùٸ� ������� Ȯ���� �ֽʽÿ�.") );
        CHECKERROR_RETURN(msg, FALSE);
    }
    file.Close();
	
	if ( !ref_file.Open( REF_FILE_NAME, CFile::modeCreate | CFile::modeWrite ) )
        CHECKERROR_RETURN(_T("ref_file.Open() Error!!!"), FALSE);

    ref_file.WriteString( file_name );
    ref_file.WriteString( _T("\n") );
    ref_file.Close();

	// ������ �ۼ��Ѵ�.
    file.WriteString( sStreamMarc );
	*/

	/*
	INT ch, drive, curdrive;	
	
	curdrive = _getdrive();
	CString sPath;
	if(curdrive==3){
		sPath=_T("C:\\KOLAS2UP\\BIN\\����_AV.TXT");
	}
	if(curdrive==4){
		sPath=_T("D:\\KOLAS2UP\\BIN\\����_AV.TXT");
	}
	if(curdrive==5){
		sPath=_T("E:\\KOLAS2UP\\BIN\\����_AV.TXT");
	}
	*/

	CString sFileName;
	GetControlData( _T("CM_BO_CAT_PROC_1100"), _T("���ϸ�"), sFileName );
	//SetControlData( _T("CM_BO_CAT_PROC_1100"), _T("���ϸ�"), sPath );
	if ( sFileName.IsEmpty() ) {
		AfxMessageBox( _T("���ϸ��� �Է��ؾ� �մϴ�.") );
		return;
	}

	CString sRegCode;
	GetControlData ( _T("CM_BO_CAT_PROC_1100"), _T("��ϱ���"), sRegCode );
	sRegCode.Replace( _T(" "), NULL );
	if ( sRegCode == _T("") ) {
		AfxMessageBox( _T("��ϱ����� �����ϼ���.") );
		return;
	}

	CString sNoStart, sNoEnd;	
	GetControlData ( _T("CM_BO_CAT_PROC_1100"), _T("��Ϲ�ȣ����"), sNoStart );
	GetControlData ( _T("CM_BO_CAT_PROC_1100"), _T("��Ϲ�ȣ����"), sNoEnd );

	if ( sNoStart == _T("") || sNoEnd == _T("") ) {
		AfxMessageBox( _T("��Ϲ�ȣ�� �Է��ϼ���.") );
		return;
	}

	if ( IDNO == AfxMessageBox( _T("������ �ۼ��Ͻðڽ��ϱ�?") , MB_YESNO) ) return;

	CESL_DataMgr * pDM;
	pDM = FindDM( _T("DM_BO_CAT_PROC_1100") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_1100 error") );
		return;
	}

	//��Ϲ�ȣ �����	
	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	//=====================================================
	//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 	CString sRegNoStart, sRegNoEnd;
// 	sRegNoStart.Format( _T("%s%010s"), sRegCode, sNoStart );
// 	sRegNoEnd.Format( _T("%s%010s"), sRegCode, sNoEnd );

	CString sRegNoStart, sRegNoEnd;
	sRegNoStart = m_pInfo->MakeRegNo( sRegCode, sNoStart );
	sRegNoEnd = m_pInfo->MakeRegNo( sRegCode, sNoEnd );
	//=====================================================
	

	CString strWhere;

	CButton * pStartReg;
	pStartReg=(CButton *)GetDlgItem(IDC_chCLASS1);	

	//�ڷ��
	CString sShelfCode;
	GetControlData( _T("CM_BO_CAT_PROC_1100"), _T("�ڷ�Ǳ���"), sShelfCode );
	sShelfCode.Replace( _T(" "), NULL );
	
	if ( pStartReg->GetCheck() == 1 ) {		//ù��Ϲ�ȣ üũ
		strWhere.Format( _T("bo_get_min_reg_no(rec_key) >= '%s' AND bo_get_min_reg_no(rec_key) <= '%s'"), sRegNoStart, sRegNoEnd );
		if ( sShelfCode != _T("") ) {
			strWhere = strWhere + _T(" AND REC_KEY IN ( SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE SHELF_LOC_CODE ='") + sShelfCode + _T("')");
		}
	}
	else {
		strWhere.Format( _T("REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE REG_NO >= '%s' AND REG_NO <= '%s')"), sRegNoStart, sRegNoEnd );
		if ( sShelfCode != _T("") ) {
			strWhere.Format( _T("REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE REG_NO >= '%s' AND REG_NO <= '%s' AND SHELF_LOC_CODE ='%s')"), sRegNoStart, sRegNoEnd, sShelfCode );
		}
	}

	if (pDM->RefreshDataManager(strWhere) < 0)	return;
	INT iRowCount;
	iRowCount = pDM->GetRowCount();

	if ( pDM->GetRowCount() < 1 ) {
		AfxMessageBox (_T("�˻��� ����� �����ϴ�."));
	}

	CButton * pMarcInfo;
	pMarcInfo=(CButton *)GetDlgItem(IDC_chkMarcInfo);	
	
	//���ϳ�����
	CButton * pDivision;
	pDivision = (CButton *)GetDlgItem(IDC_chkDivision);	

	FILE *fp;
	//TCHAR tmp[256];
	if ( pDivision->GetCheck() != 1 ) {		//�����ڵ����� �ƴҶ�
		fp = _tfopen( sFileName , _T("wb") );
		if ( fp == NULL ) {
			AfxMessageBox(_T("������ �����ϼ���."));
			return;
		}
				/*UNCHANGE*/fputc(0xFF, fp);
				/*UNCHANGE*/fputc(0xFE, fp);
		
		//_ftprintf( fp, _T("adf\r\n"));
	}
	
	
	if ( pDivision->GetCheck() == 1 ) {		//�����ڵ�����
		CString sDataCount;
		GetControlData( _T("CM_BO_CAT_PROC_1100"), _T("�Ǽ�"), sDataCount );
		if ( sDataCount == _T("") ) {
			AfxMessageBox( _T("�Ǽ��� �����ϼ���.") );
			return;
		}
		INT iDataCount;
		iDataCount = _ttoi( sDataCount.GetBuffer(0) );
		INT iFileCount, iLastCount;
		iFileCount = iRowCount / iDataCount;

		iLastCount = iRowCount % iDataCount;

		for ( INT i = 1 ; i < iFileCount ; i++ ) {
			FILE *fp;
			CString sFileCount;
			sFileCount.Format( _T("%d"), i );

			CString sDivFileName;
			sDivFileName.Format( _T("%s%s.TXT"), sFileName.Left( sFileName.GetLength()-4 ), sFileCount );

			//sFileName += sFileCount;
			fp = _tfopen( sDivFileName, _T("wb") );

			for ( INT j = 0 ; j < iDataCount ; j++ ) {
				CString sMarc;
				INT iRow = i*iDataCount + j;
				sMarc = pDM->GetCellData( _T("BS_MARC"), iRow );

				if ( pMarcInfo->GetCheck() == 0 ) {	
					//�ڰ����� ����
					CMarc marc;
					//decoding
					CString strTagCode, strErrMsg, strHelpMsg;			
					if(m_pInfo->pMarcMgr->Decoding(sMarc, &marc)) {
						POSITION pos;
						pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
						while (pos)	{
							m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
							UpdateData(FALSE);
						}	
					}
					m_pInfo->pMarcMgr->DeleteField( &marc, _T("049") );
					m_pInfo->pMarcMgr->DeleteField( &marc, _T("090") );

					//Encoding
					m_pInfo->pMarcMgr->Encoding( &marc, sMarc );
				}
				
				/*UNCHANGE*/fputc(0xFF, fp);
				/*UNCHANGE*/fputc(0xFE, fp);
		
				sMarc += _T("\r\n");
				_ftprintf( fp, sMarc );
			}
			fclose( fp );
		}

		if ( iLastCount != 0 ) {
			FILE *fp;

			CString sFileCount;
			sFileCount.Format( _T("%d"), iFileCount );

			CString sDivFileName;
			sDivFileName.Format( _T("%s%s.TXT"), sFileName.Left( sFileName.GetLength()-4 ), sFileCount );
			fp = _tfopen( sDivFileName, _T("wb") );

			for ( i = 0 ; i < iLastCount ; i++ ) {
				
				CString sMarc;
				INT iRow = iFileCount*iDataCount + i;
				sMarc = pDM->GetCellData( _T("BS_MARC"), iRow );

				if ( pMarcInfo->GetCheck() == 0 ) {	
					//�ڰ����� ����
					CMarc marc;
					//decoding
					CString strTagCode, strErrMsg, strHelpMsg;			
					if(m_pInfo->pMarcMgr->Decoding(sMarc, &marc)) {
						POSITION pos;
						pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
						while (pos)	{
							m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
							UpdateData(FALSE);
						}	
					}
					m_pInfo->pMarcMgr->DeleteField( &marc, _T("049") );
					m_pInfo->pMarcMgr->DeleteField( &marc, _T("090") );

					//Encoding
					m_pInfo->pMarcMgr->Encoding( &marc, sMarc );
				}
				
				/*UNCHANGE*/fputc(0xFF, fp);
				/*UNCHANGE*/fputc(0xFE, fp);

				sMarc += _T("\r\n");
				_ftprintf( fp, sMarc );
			}
			fclose( fp );
		}
	}
	else {
		for ( INT i = 1 ; i <= iRowCount ; i++ ) {
			CString sMarc;
			sMarc = pDM->GetCellData( _T("BS_MARC"), i-1 );

			if ( pMarcInfo->GetCheck() == 0 ) {	
				//�ڰ����� ����
				CMarc marc;
				//decoding
				CString strTagCode, strErrMsg, strHelpMsg;			
				if(m_pInfo->pMarcMgr->Decoding(sMarc, &marc)) {
					POSITION pos;
					pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
					while (pos)	{
						m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
						UpdateData(FALSE);
					}	
				}
				m_pInfo->pMarcMgr->DeleteField( &marc, _T("049") );
				m_pInfo->pMarcMgr->DeleteField( &marc, _T("090") );

				//Encoding
				m_pInfo->pMarcMgr->Encoding( &marc, sMarc );
			}

			sMarc += _T("\n");
			_ftprintf( fp, sMarc );
		}
	}

	fclose( fp );

	CString sMessage;
	sMessage.Format( _T("�˻��� %d�ǿ� ���� �����ۼ��� �Ϸ�Ǿ����ϴ�."), iRowCount );
	AfxMessageBox( sMessage );

EFS_END
}

INT CBO_CAT_PROC_1100::MakeFileSelected(CString sCMAlias, CString sGridAlias)
{
	EFS_BEGIN

	INT idx;
	SelectMakeList(sCMAlias, sGridAlias);
	//DELidx = SelectGetHeadPosition(sCMAlias, sGridAlias);
	idx = SelectGetTailPosition(sCMAlias, sGridAlias);
	if (idx < 0)
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));	// ����
		return -1;		
	}

	TCHAR    szCurrentDir[512];
	_tgetcwd(szCurrentDir, 512);

	CFileDialog dlg(FALSE, _T("txt"), _T("����.TXT"),
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    _T("��������(*.txt)|All Files(*.*)|*.*||") );
    if ( IDOK != dlg.DoModal() ) return -1;

	_tchdir(szCurrentDir);

	CString sFileName;
    sFileName = dlg.GetFileTitle();
    if ( sFileName.IsEmpty() ) return -1;
    sFileName = dlg.GetPathName();	


	//if ( IDNO == AfxMessageBox( _T("������ �ۼ��Ͻðڽ��ϱ�?") , MB_YESNO) ) return -1;

	CESL_DataMgr * pDmRegMain;
	pDmRegMain = FindDM( _T("DM_����_����_���_����") );
	if ( pDmRegMain == NULL ) {
		AfxMessageBox( _T("DM_����_����_���_���� Ȯ��") );
		return -1;
	}

	CESL_DataMgr * pDM;
	pDM = FindDM( _T("DM_BO_CAT_PROC_1100") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_1100 error") );
		return -1;
	}

	FILE *fp;
	fp = _tfopen( sFileName , _T("wb") );
	if ( fp == NULL ) {
		AfxMessageBox(_T("������ �����ϼ���."));
		return -1;
	}
	
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);

	//idx = SelectGetNext(sCMAlias, sGridAlias);
	CString sBookKeyList;
	CStringArray aBookKeyList;
	while ( idx >= 0 ) {
		CString sBookKey = pDmRegMain->GetCellData( _T("IBS_��KEY") , idx );
		sBookKeyList = sBookKeyList + _T(", ") + sBookKey;

		if ( ((idx+1) % 999 ) == 0 ) {
		//if ( idx == 950 ) {
			aBookKeyList.Add( sBookKeyList );
			sBookKeyList = _T("");
			//break;
		}

		//DELidx = SelectGetNext(sCMAlias, sGridAlias);
		idx = SelectGetPrev(sCMAlias, sGridAlias);
	}
	//sBookKeyList = sBookKeyList.Mid(2);
	aBookKeyList.Add( sBookKeyList );
/*
	CString strWhere;
	strWhere = _T(" REC_KEY IN ( ") + sBookKeyList + _T(" ) ORDER BY INPUT_DATE");
	if (pDM->RefreshDataManager(strWhere) < 0)	return -1;
*/
	INT iRowCount;
	INT iTotCount = 0;
	for ( INT j = 0 ; j < aBookKeyList.GetSize() ; j++ ) {
		//sBookKeyList = sBookKeyList.Mid(2);
		sBookKeyList = aBookKeyList.GetAt(j).Mid(2);

		CString strWhere;
		//strWhere = _T(" REC_KEY IN ( ") + sBookKeyList + _T(" ) ORDER BY INPUT_DATE");
		//===================================================
		//2009.05.14 UPDATE BY LKS : 
		//strWhere = _T(" REC_KEY IN ( ") + sBookKeyList + _T(" ) ORDER BY INPUT_DATE");
		strWhere = _T(" REC_KEY IN ( ") + sBookKeyList + _T(" )");
		//===================================================
		if (pDM->RefreshDataManager(strWhere) < 0)	return -1;

		iRowCount = pDM->GetRowCount();
		iTotCount = iTotCount + iRowCount;
	/*
		FILE *fp;
		fp = _tfopen( sFileName , _T("w") );
		if ( fp == NULL ) {
			AfxMessageBox(_T("������ �����ϼ���."));
			return -1;
		}
	*/
		for ( INT i = 1 ; i <= iRowCount ; i++ ) {
			CString sMarc;
			sMarc = pDM->GetCellData( _T("BS_MARC"), i-1 );
			/*
			if ( pMarcInfo->GetCheck() == 0 ) {	
				//�ڰ����� ����
				CMarc marc;
				//decoding
				CString strTagCode, strErrMsg, strHelpMsg;			
				if(m_pInfo->pMarcMgr->Decoding(sMarc, &marc)) {
					POSITION pos;
					pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
					while (pos)	{
						m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
						UpdateData(FALSE);
					}	
				}
				m_pInfo->pMarcMgr->DeleteField( &marc, _T("049") );
				m_pInfo->pMarcMgr->DeleteField( &marc, _T("090") );

				//Encoding
				m_pInfo->pMarcMgr->Encoding( &marc, sMarc );
			}
			*/
			CMarc marc;
			m_pInfo->pMarcMgr->Decoding( sMarc, &marc );
			m_pInfo->pMarcMgr->Encoding( &marc, sMarc, TRUE );

			sMarc += _T("\r\n");
			_ftprintf( fp, sMarc );
		}
	}

	fclose( fp );

	CString sMessage;
	sMessage.Format( _T("������ %d�ǿ� ���� �����ۼ��� �Ϸ�Ǿ����ϴ�."), iTotCount );
	AfxMessageBox( sMessage );

	return 0;
	EFS_END
	return -1;
}

VOID CBO_CAT_PROC_1100::OnchkDivision() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CButton * pDivision;
	pDivision = (CButton *)GetDlgItem(IDC_chkDivision);	

	CButton * pDataCount;
	pDataCount = (CButton *)GetDlgItem(IDC_cDataCount);	
	if ( pDivision->GetCheck() == 1 ) {
		pDataCount->EnableWindow(TRUE);
	}
	else {
		pDataCount->EnableWindow(FALSE);
	}

EFS_END
}

VOID CBO_CAT_PROC_1100::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

HBRUSH CBO_CAT_PROC_1100::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

