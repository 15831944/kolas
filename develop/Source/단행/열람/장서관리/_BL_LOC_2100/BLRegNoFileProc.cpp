// BLRegNoFileProc.cpp: implementation of the CBLRegNoFileProc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BLRegNoFileProc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBLRegNoFileProc::CBLRegNoFileProc(CESL_Mgr *pWnd)
	: CSPF_Work(pWnd)
{
	m_sTmpRegNoCMAlias = _T("CM_BO_LOC_2110");
	m_sTmpRegNoDMAlias = _T("DM_BO_LOC_2100_REG_NO");
	//m_sTmpRegNoDMAlias = _T("DM_BO_LOC_2110_TMP_REG_NO");
	m_sRegNoInfoCMAlias = _T("CM_BO_LOC_2110_INFO");
	m_sGridAlias = _T("grid");

	m_pTmpRegNoCM = m_pParentMgr->FindCM( m_sTmpRegNoCMAlias );
	if( m_pTmpRegNoCM == NULL ) ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd ,  -1 , _T("CBLRegNoFileProc(CESL_Mgr *pWnd)") );
	
	m_pTmpRegNoDM = m_pParentMgr->FindDM( m_sTmpRegNoDMAlias );
	if( m_pTmpRegNoDM == NULL ) ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd ,  -2 , _T("CBLRegNoFileProc(CESL_Mgr *pWnd)") );

	m_pRegNoInfoCM = m_pParentMgr->FindCM( m_sRegNoInfoCMAlias );
	if( m_pRegNoInfoCM == NULL ) ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd , -3 , _T("CBLRegNoFileProc(CESL_Mgr *pWnd)") );

	m_pParentSpfWork = new CBLBookCheckProc( m_pParentMgr ); // �θ� SPF

	m_nCheckRegNo = 0;

}

CBLRegNoFileProc::~CBLRegNoFileProc()
{
	if( m_pParentSpfWork != NULL ) delete m_pParentSpfWork;
	m_pParentSpfWork = NULL;
}

INT CBLRegNoFileProc::SPFExcutePath( CString sPathAlias )
{
	EFS_BEGIN

	INT ids;

	if( sPathAlias.Compare( DM_ONE_KEYBORD_ADD ) == 0 )
	{
		ids = AddOneRegNoFromKeybord();
	}
	else if( sPathAlias.Compare( DM_ONE_BARCODE_ADD ) == 0 )
	{
		ids = AddOneRegNoFromBarcode();
	}
	else if( sPathAlias.Compare( DM_CLEAR ) == 0 )
	{
		ids = DMClear();
	}
	else if( sPathAlias.Compare( DM_DELETE_ROW ) == 0 )
	{
		ids = DMDeleteRow();
	}
	else if( sPathAlias.Compare( DM_LOAD_FILE ) == 0 )
	{
		// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
		//ids = DMLoadFile();
		ids = DMLoadFile_201809( true );
		// KOL.RED.2018.201 ---------------------------------------------------------------------------
	}
	else if( sPathAlias.Compare( DM_LOAD_FILE2 ) == 0 )
	{
		// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//ids = DMLoadFile2();
		ids = DMLoadFile_201809( false );
		// KOL.RED.2018.201 ---------------------------------------------------------------------------
	}
	else if( sPathAlias.Compare( DM_SAVE_FILE ) == 0 )
	{
		ids = DMSaveFile();
	}
	else if( sPathAlias.Compare( OPEN_REGNO_EDIT ) == 0 )
	{
		ids = OpenRegNoEdit();
	}
	else if( sPathAlias.Compare( SAVE_REGNO_EDIT ) == 0 )
	{
		ids = SaveRegNoEdit();
	}
	else 
	{
		return -1;
	}

	if( ids < 0 ) 
		AfxMessageBox(_T("����� ���������� �������� ���Ͽ����ϴ�.") );

	return 0;

	EFS_END
	return -1;

}


// Ű����κ��� ��Ϲ�ȣ�� �߰��Ѵ�.
INT CBLRegNoFileProc::AddOneRegNoFromKeybord()
{
	EFS_BEGIN

	INT ids;

	// ��Ʈ�ѿ��� ��Ϲ�ȣ���� ���ؿ´�.
	CString sRegCode;
	CString sRegNo;

	ids = m_pRegNoInfoCM->GetControlMgrData( _T("��ϱ���") , sRegCode , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("AddOneRegNoFromKeybord()") );

	ids = m_pRegNoInfoCM->GetControlMgrData( _T("��Ϲ�ȣ") , sRegNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("AddOneRegNoFromKeybord()") );

	if( sRegNo.IsEmpty() ) return 1;

	sRegNo = sRegCode + sRegNo ;

	// DM�� �߰��Ѵ�.
	ids = AddOneRegNo( sRegNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("AddOneRegNoFromKeybord()") );

	return 0;

	EFS_END
	return -1;

}

// ���ڵ�κ��� ��Ϲ�ȣ�� �߰��Ѵ�.
INT CBLRegNoFileProc::AddOneRegNoFromBarcode()
{
	EFS_BEGIN

	INT ids;
	// ��Ʈ�ѿ��� ��Ϲ�ȣ���� ���ؿ´�.
	CString sRegNo;

	ids = m_pRegNoInfoCM->GetControlMgrData( _T("���ڵ�") , sRegNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("AddOneRegNoFromBarcode()") );

	if( sRegNo.IsEmpty() || sRegNo.GetLength() < 3 ) return 1;


	// DM�� �߰��Ѵ�.
	ids = AddOneRegNo( sRegNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("AddOneRegNoFromBarcode()") );

	return 0;

	EFS_END
	return -1;

}

// sRegNo(��Ϲ�ȣ)�� DM�� �߰��Ѵ�.
INT CBLRegNoFileProc::AddOneRegNo( CString sRegNo )
{
	EFS_BEGIN

	INT ids;

	// ��Ϲ�ȣ�� �ڸ����� ���� 0�� �Է��Ѵ�.
	ids = CLocCommonAPI::MakeRegNoSpaceToZero( m_pParentMgr  ,  sRegNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("AddOneRegNo( CString sRegNo )") ); 

	// DM�� �߰��Ѵ�. - �߰��ϱ� �� Grid�� üũ�� �κ��� DM���� �Ű��ش�.
	// �߰��ϱ����� �ߺ��� ��Ϲ�ȣ���� �����Ѵ�.
	ids = CheckDuplicateRegNo( sRegNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("AddOneRegNo( CString sRegNo )") ); 
	if( ids > 0 ) return 1;

	m_pTmpRegNoDM->InsertRow(-1);

	ids = m_pTmpRegNoDM->SetCellData( _T("��Ϲ�ȣ") , sRegNo , m_pTmpRegNoDM->GetRowCount()-1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("AddOneRegNo( CString sRegNo )") ); 

	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	cMapData_CheckNum.Add( sRegNo );
	// KOL.RED.2018.201 ---------------------------------------------------------------------------

	// �׸��带 �ٽ� �����ش�.
	ids = GridDisplay();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("AddOneRegNo( CString sRegNo )") ); 

	return 0;

	EFS_END
	return -1;

}

// ���� ��Ϲ�ȣ�� �����ϴ��� �˻�
INT CBLRegNoFileProc::CheckDuplicateRegNo( CString sRegNo )
{
	EFS_BEGIN

	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	// �ؽ��� ã��
	// nEqualCnt ���� ���� ������ �ʴ´�.
	/*
	INT ids;

	CString sTmpRegNo;
	INT nEqualCnt = 0;

	for( INT i = 0 ; i < m_pTmpRegNoDM->GetRowCount() ; i++ )
	{
		ids = m_pTmpRegNoDM->GetCellData( _T("��Ϲ�ȣ") , i , sTmpRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("AddOneRegNo( CString sRegNo )") ); 

		if( sRegNo.Compare(sTmpRegNo)==0 )
			nEqualCnt++;
	}

	return nEqualCnt;
	*/
	if( cMapData_CheckNum.Find(sRegNo) != NULL )
	{
		return 1;
	}
	return 0;
	// KOL.RED.2018.201 ---------------------------------------------------------------------------

	EFS_END
	return -1;

}

// DM�� ���� ��Ϲ�ȣ�� �����ϸ� �����ִ� �Լ�
INT CBLRegNoFileProc::DeleteDuplicateRegNo()
{
	EFS_BEGIN

	INT ids;

	INT nRowCount = m_pTmpRegNoDM->GetRowCount();
	CString sRegNo;
	//=====================================================
	//2009.03.03 UPDATE BY PJW : ����for������ ����Ÿ���� ���� ��� �ð� �����ɸ� ���� �ٷ� ���� �ϵ��� ����
// 	CString sRegNo;
// 	
// 	for( INT i = nRowCount-1 ; i > -1 ; i-- )
// 	{
// 		ids = m_pTmpRegNoDM->GetCellData( _T("��Ϲ�ȣ") , i , sRegNo );
// 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("DeleteDuplicateRegNo( CString sRegNo )") ); 
// 
// 		ids = CheckDuplicateRegNo( sRegNo );
// 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("DeleteDuplicateRegNo( CString sRegNo )") ); 
// 		if( ids > 1 ) m_pTmpRegNoDM->DeleteRow(i);
// 	}
	CString strCompareRegNo;
	m_pTmpRegNoDM->SORT(_T("��Ϲ�ȣ"));
	
	for( INT i = nRowCount-1 ; i > 0 ; i-- )
	{
		ids = m_pTmpRegNoDM->GetCellData( _T("��Ϲ�ȣ") , i , sRegNo );
		if( ids < 0 ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("DeleteDuplicateRegNo( CString sRegNo )") ); 
		}
		ids = m_pTmpRegNoDM->GetCellData( _T("��Ϲ�ȣ") , i-1 , strCompareRegNo );
		if( ids < 0 ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("DeleteDuplicateRegNo( CString strCompareRegNo )") ); 
		}

		if(strCompareRegNo == sRegNo)
		{
			m_pTmpRegNoDM->DeleteRow(i);
		}
	}
	//=====================================================
	
	

	return 0;

	EFS_END
	return -1;

}

// �׸��� �����ش�.
INT CBLRegNoFileProc::GridDisplay( BOOL bCheckCopy/* = TRUE */)
{
	EFS_BEGIN

	return 0;
		
	INT ids;
	
	// �׸����� üũ�� DM���� �̵�
	if( bCheckCopy )
	{
		ids = MoveGridCheckToDM();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GridDisplay()") ); 
	}	

	ids = m_pParentMgr->AllControlDisplay( m_sTmpRegNoCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GridDisplay()") ); 

	// ���� ����
	INT nCount = m_pTmpRegNoDM->GetRowCount();
	CString sCount;
	sCount.Format(_T("%d") , nCount );
	ids = m_pRegNoInfoCM->SetControlMgrData( _T("����") , sCount );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GridDisplay()") ); 

	return 0;

	EFS_END
	return -1;

}

// �׸����� üũ�� DM���� �̵�
INT CBLRegNoFileProc::MoveGridCheckToDM()
{
	EFS_BEGIN

	INT ids;

	if( m_pTmpRegNoDM->GetRowCount() < 1 ) return 0;

	CESL_Grid *pGrid = (CESL_Grid*)(m_pTmpRegNoCM->FindControl( m_sGridAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MoveGridCheckToDM()") );

	TCHAR *sDMColumnAlias[1] = { _T("����") };
	INT nColIndex[1] = { -1 };
	ids = CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pGrid , sDMColumnAlias , 1 , nColIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ChangeBookCheckYN") );
	INT nCheckColIndex = nColIndex[0];

	CString sCheck;
	for( INT i = 0 ; i < pGrid->GetCount() ; i++ )
	{
		ids = pGrid->GetAt( i , nCheckColIndex , sCheck );
		if( ids < 0 )
		{
			CString sMsg;
			sMsg.Format(_T("i : %d , nCheckColIndex : %d , sCheck : %s") , i , nCheckColIndex , sCheck );
			AfxMessageBox( sMsg );
		}
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MoveGridCheckToDM()") ); 

		ids = m_pTmpRegNoDM->SetCellData( _T("����") , sCheck , i );
		if( ids < 0 )
		{
			CString sMsg;
			sMsg.Format(_T("i : %d , nCheckColIndex : %d , sCheck : %s") , i , nCheckColIndex , sCheck );
			AfxMessageBox( sMsg );
		}
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("MoveGridCheckToDM()") );
	}

	return 0;

	EFS_END
	return -1;

}

// �ʱ�ȭ
INT CBLRegNoFileProc::DMClear()
{
	EFS_BEGIN

	INT ids;

	// DM ����
	m_pTmpRegNoDM->FreeData();

	// ȭ�� ���ε�
	ids = GridDisplay(FALSE);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("DMClear()") ); 

	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	cMapData_CheckNum.Clear();
	// KOL.RED.2018.201 ---------------------------------------------------------------------------

	return 0;

	EFS_END
	return -1;

}


// ����
// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
// ���Լ� �Ⱦ��� �� ���� , BO �ʿ��� ���� ���°� ����.
// KOL.RED.2018.201 ---------------------------------------------------------------------------
INT CBLRegNoFileProc::DMDeleteRow()
{
	EFS_BEGIN

	INT ids;
	
	ids = MoveGridCheckToDM();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("DMDeleteRow()") ); 
	
	CString sCheck;
	INT nRowCount = m_pTmpRegNoDM->GetRowCount();
	for( INT i=nRowCount-1 ;i > -1 ; i-- )
	{
		ids = m_pTmpRegNoDM->GetCellData( _T("����") , i , sCheck );
		if( ids < 0 && ids != -4012 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("DMDeleteRow()") ); 

		if( sCheck.Compare( _T("V") ) == 0 ) 
			m_pTmpRegNoDM->DeleteRow(i);
	}

	ids = GridDisplay( FALSE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("DMDeleteRow()") ); 

	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
	DeleteGrid_UpdateHash();
	// KOL.RED.2018.201 ---------------------------------------------------------------------------

	return 0;

	EFS_END
	return -1;

}

// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
// �ߺ� üũ�� �ؽ� ���̺���  �ٽ� �����.
void	CBLRegNoFileProc::DeleteGrid_UpdateHash()
{
	EFS_BEGIN
	INT ids;
	int i=0;
	cMapData_CheckNum.Clear();
	int	nRowCount = m_pTmpRegNoDM->GetRowCount();
	CString sRegNo;
	for( i = 0 ; i < nRowCount ; i++ )
	{
		ids = m_pTmpRegNoDM->GetCellData( _T("��Ϲ�ȣ") , i , sRegNo );
		cMapData_CheckNum.Add( sRegNo );
	}
	EFS_END
}
// KOL.RED.2018.201 ---------------------------------------------------------------------------


// ���Ͽ��� �ҷ�����
INT CBLRegNoFileProc::DMLoadFile()
{
	EFS_BEGIN

	// DM ���� - ����
	// m_pTmpRegNoDM
	
	INT ids = SanFileAPI::RegNoFileLoad( m_pTmpRegNoDM , m_pParentMgr );//, m_nCheckRegNo );
	
	if( ids == 100 ) return 1;
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("DMLoadFile()") );

	// 2008.07.11 REMOVE BY PDW {{++
//	if( CHECK_DUPLICATEREGNO_IS_DONE != ids )
	// 2008.07.11 REMOVE BY PDW --}}
	{
		ids = DeleteDuplicateRegNo();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("DMLoadFile()") );
	}	

	ids = GridDisplay( FALSE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("DMLoadFile()") ); 

	return 0;

	EFS_END
	return -1;

}

// ���Ͽ��� �ҷ�����
INT CBLRegNoFileProc::DMLoadFile2()
{
	EFS_BEGIN

	// DM ���� - ����
	// m_pTmpRegNoDM
	
	INT ids = SanFileAPI::RegNoFileLoad( m_pTmpRegNoDM , m_pParentMgr );//, m_nCheckRegNo );
	
	if( ids == 100 ) return 1;
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("DMLoadFile()") );

	ids = GridDisplay( FALSE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("DMLoadFile()") ); 

	return 0;

	EFS_END
	return -1;
}

// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
//
// ���Ϸ� ��Ϲ�ȣ ����ϱ� 
// bool check_dupl : ������ �׸���� ������ü �ߺ�üũ���ο� ���� �߰����� üũ
// �� �Լ� , DMLoadFile ,DMLoadFile2 �� ��ü�Ѵ�.
INT CBLRegNoFileProc::DMLoadFile_201809( bool check_dupl )
{
	EFS_BEGIN

	CESL_DataMgr cDM;
	INT ids;
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , m_pTmpRegNoDM , &cDM );

	// KOL.RED.2018.204 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	//ids = SanFileAPI::RegNoFileLoad( &cDM , m_pParentMgr );//, m_nCheckRegNo );
	TCHAR TmpDir[1025];
	GetCurrentDirectory(1024,TmpDir);

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
                    _T("���� ����(*.xls)|.xls|�Ѽ� ���� ����(*.cell)|.cell|��� ����(*.*)|*.*||"), m_pParentMgr);

	INT result_dlg = dlg.DoModal(); 
	SetCurrentDirectory(TmpDir);
	if(result_dlg != IDOK) 
	{
		return 0;
	}

	CString strExt = dlg.GetFileExt();
	if (strExt.CompareNoCase(_T("XLS")) == 0 || strExt.CompareNoCase(_T("CELL")) == 0)
	{
		// BO_LOC_3390 ����.
		/////////////////////////////////////////////////////////////////////////////////
		// 1. ���� ������ ����
		/////////////////////////////////////////////////////////////////////////////////
		CRect rc;
		m_pParentMgr->GetWindowRect(rc);
		C_ExcelManControl ExcelCtrl;
		
		ExcelCtrl.Create(_T("Excel Control"), BS_DEFPUSHBUTTON, rc, m_pParentMgr, WM_USER + 10000);
		if (ExcelCtrl.m_hWnd == NULL) 
		{
			AfxMessageBox( _T("Excelmanager.ocx�� ��ġ�Ǿ� ���� �ʽ��ϴ�. ��ġ�Ͽ� �ֽʽÿ�.") );
			return 1;
		}

		CExcelManager ExcelMgr;
		ExcelMgr.SetManager(&ExcelCtrl);		
		if(ExcelMgr.OpenManager() == FALSE) 
		{
			AfxMessageBox( _T("NOT Installed Excelmanager.ocx or No Excel!!!") );
			return 2;
		}

		ExcelMgr.SetUseTemplate(TRUE, dlg.GetPathName() );
		if (ExcelMgr.OpenManager() == FALSE)
		{
			AfxMessageBox( _T("Open Failed") );
			return 3;
		}
			
		CString sValue = _T("");
		INT nRow = 0;
		//cDM.SetDataMgrRef( 0 , _T("��Ϲ�ȣ") , _T("") , _T("")  , _T("")  , _T("") );
		// 3.2) ���� ������ ���� �� ��ü�� ��� �ְų� #EOF�� �����ϴ� ���̴�.

		CString log;
		while(TRUE)
		{	
			sValue = ExcelMgr.GetTextMatrix(nRow + 1, 1);
			if( _tcscmp(sValue, _T("#_TEOF")) == 0 )
			{
				break;
			}
			if( sValue.IsEmpty() )
			{
				break;
			}
			nRow = cDM.InsertRow( -1 );
			cDM.SetCellData(nRow, 0, sValue);
			log.Format( _T("excel file  read -> : %d %s \n") , nRow , sValue ) ;
			OutputDebugString( log );
		}

		ExcelMgr.CloseManager();

	}
	else
	{
		ids = SanFileAPI::RegNoFileLoad(&cDM,  m_pParentMgr , dlg.GetPathName());
	}

	// KOL.RED.2018.204 ---------------------------------------------------------------------------	

	if( ids == 100 ) return 1;
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("DMLoadFile_201809()") );

	INT nRowCount = cDM.GetRowCount();
	CString sRegNo;
	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		ids = cDM.GetCellData( _T("��Ϲ�ȣ") , i , sRegNo );
		if( check_dupl )
		{
			if( cMapData_CheckNum.Find(sRegNo) != NULL )
			{
				continue;
			}
		}
		m_pTmpRegNoDM->InsertRow(-1);
		ids = m_pTmpRegNoDM->SetCellData( _T("��Ϲ�ȣ") , sRegNo , m_pTmpRegNoDM->GetRowCount()-1 );
		cMapData_CheckNum.Add( sRegNo );
	}
	ids = GridDisplay( FALSE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -20 , _T("DMLoadFile_201809()") ); 

	return 0;
	EFS_END
	return -1;
}
// KOL.RED.2018.201 ---------------------------------------------------------------------------


// DM�� �ִ� ��Ϲ�ȣ�� ���Ͽ� �����ϴ� �Լ�
INT CBLRegNoFileProc::DMSaveFile()
{
	EFS_BEGIN

	INT ids;

	// KOL.RED.2018.204 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	/*
	//=====================================================
	//2009.01.13 UPDATGE BY PJW : ���� ���̾�α׸� ���� ���ϸ� �Ѱ� �ش�.
// 	ids = SanFileAPI::RegNoFileSave( m_pTmpRegNoDM );
	CTime t = CTime::GetCurrentTime();
	CString strFileName;
 	strFileName.Format(_T("(�弭����)��Ϲ�ȣ����_%04d%02d%02d%02d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	ids = SanFileAPI::RegNoFileSave( m_pTmpRegNoDM,strFileName );
	if( ids == 100 ) return 0;
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("DMLoadFile()") );
	//=====================================================
	*/
	ids = m_pTmpRegNoDM->ExportToExcelWithFileDialog(m_pParentMgr);
	// KOL.RED.2018.204 ---------------------------------------------------------------------------		

	return 0;

	EFS_END
	return -1;

}

// �θ� ��Ϲ�ȣ DM���� �ڽ� ��Ϲ�ȣ DM���� ī���ϴ� �Լ�
INT CBLRegNoFileProc::OpenRegNoEdit()
{
	EFS_BEGIN

	INT ids;

	// �θ��� ��Ϲ�ȣ DM�� ���� ������ ��Ϲ�ȣ DM���� �̵�
	ids = m_pParentSpfWork->SPFExcutePath(OPEN_REGNO_EDIT);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("OnbtnFileSave()") );	

	ids = GridDisplay(FALSE);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("OnbtnFileSave()") );	

	return 0;

	EFS_END
	return -1;

}

// �ڽ� ��Ϲ�ȣ DM���� �θ� ��Ϲ�ȣ DM���� ī���ϴ� �Լ�
INT CBLRegNoFileProc::SaveRegNoEdit()
{
	EFS_BEGIN

	INT ids;
	ids = m_pParentSpfWork->SPFExcutePath(SAVE_REGNO_EDIT);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,  -1 , _T("OnbtnFileSave()") );		

	return 0;

	EFS_END
	return -1;

}

VOID CBLRegNoFileProc::SetCheckRegNoOption( INT flag )
{
	m_nCheckRegNo = flag;
}
