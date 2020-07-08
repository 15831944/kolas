// CO_CENTER_USER_MANAGE_05.cpp : implementation file
//

#include "stdafx.h"
#include "CO_CENTER_USER_MANAGE_05.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_MANAGE_05 dialog


CCO_CENTER_USER_MANAGE_05::CCO_CENTER_USER_MANAGE_05(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCO_CENTER_USER_MANAGE_05)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
// �߰� : DeleteObject���� �Ҹ��� �߰� ��� [2008-05-15]
CCO_CENTER_USER_MANAGE_05::~CCO_CENTER_USER_MANAGE_05()
{
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]
}
VOID CCO_CENTER_USER_MANAGE_05::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCO_CENTER_USER_MANAGE_05)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCO_CENTER_USER_MANAGE_05, CDialog)
	//{{AFX_MSG_MAP(CCO_CENTER_USER_MANAGE_05)
	ON_BN_CLICKED(IDC_btnPREV, OnbtnPREV)
	ON_BN_CLICKED(IDC_btnNEXT, OnbtnNEXT)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnSAVEUSER, OnbtnSAVEUSER)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_MANAGE_05 message handlers

BOOL CCO_CENTER_USER_MANAGE_05::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	if(InitESL_Mgr(_T("CO_CENTER_USER_MANAGE_05")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CCO_CENTER_USER_MANAGE_05::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	bFirstView = FALSE;
	SetScreen();
	bFirstView = TRUE;	
}



VOID CCO_CENTER_USER_MANAGE_05::SetScreen()
{
	EFS_BEGIN

	SetCurrentIndex();
	DisplayWindow();

	EFS_END
}



INT CCO_CENTER_USER_MANAGE_05::DisplayWindow()
{
	EFS_BEGIN

	INT ids;


	// 2. DISPLAY�Ѵ�.
	ids = ViewScreenLinkDM();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CCO_CENTER_USER_MANAGE_05::DisplayWindow()") );
	
	// 3. �θ� �׸��忡 ǥ���Ѵ�.
	ids = SetSelect();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CCO_CENTER_USER_MANAGE_05::DisplayWindow()") );

	return 0;

	EFS_END
	return -1;

}

INT CCO_CENTER_USER_MANAGE_05::SetCurrentIndex()
{
	EFS_BEGIN

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) return -1;

	// ���� ������
	// 'V'üũ�� �� ���� �ִٸ� 'V'üũ �Ȱ͸� �����ָ�
	// 'V'üũ�� �� ���� ���ٸ� ��� �����ش�.
	INT nTmpIndex;
	if( bFirstView ) nTmpIndex = pGrid->GetIdx();
	else nTmpIndex = pGrid->GetIdx();

	pGrid->SelectMakeList();
	INT nSelectCount = pGrid->SelectGetCount();
	if( nSelectCount > 0 ) m_nViewMode = 1;
	else m_nViewMode = 0;

	if( m_nViewMode == 0 )
	{
		m_nCurrentIndex = pGrid->GetIdx();
		m_nLastIndex = pGrid->GetCount() - 1;
		m_nFirstIndex = 0;
	}
	else
	{
		m_nLastIndex = pGrid->SelectGetTailPosition();
		m_nFirstIndex = pGrid->SelectGetHeadPosition();
		for( INT i = 0 ; i < nSelectCount-1 ; i++ )
		{
			if( nTmpIndex == pGrid->SelectGetIdx() )
				break;
			pGrid->SelectGetNext();
		}
		if( i==nSelectCount ) m_nCurrentIndex = pGrid->SelectGetHeadPosition();
		else m_nCurrentIndex = pGrid->SelectGetIdx();
	}
	
	return 0;

	EFS_END
	return -1;

}

VOID CCO_CENTER_USER_MANAGE_05::SetPreNextButton()
{
	EFS_BEGIN

	GetDlgItem(IDC_btnPREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnNEXT)->EnableWindow(TRUE);

	if( m_nCurrentIndex == m_nFirstIndex )
	{
		GetDlgItem(IDC_btnPREV)->EnableWindow(FALSE);
	}
	if( m_nCurrentIndex == m_nLastIndex )
	{
		GetDlgItem(IDC_btnNEXT)->EnableWindow(FALSE);
	}

	EFS_END
}

VOID CCO_CENTER_USER_MANAGE_05::OnbtnPREV() 
{
	EFS_BEGIN

	MoveIndex(0);	
	DisplayWindow();

	EFS_END
}


VOID CCO_CENTER_USER_MANAGE_05::OnbtnNEXT() 
{
	EFS_BEGIN

	MoveIndex(1);	
	DisplayWindow();

	EFS_END
}



VOID CCO_CENTER_USER_MANAGE_05::OnbtnCLOSE() 
{
	OnCancel();	
}




INT CCO_CENTER_USER_MANAGE_05::ViewScreenLinkDM()
{
	EFS_BEGIN
		
	INT ids;
	
	CString sCMAlias = _T("CM_CO_CENTER_USER_MANAGE_05" );
	
	CESL_ControlMgr *pChildCM = FindCM( sCMAlias );
	if( pChildCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CBO_LOC_3320::ViewScreenLinkDM()") );
	
	
	// DM�� �ִ� �ڷ� CM�� �����ֱ�
	ids = pChildCM->AllControlDisplay(m_nCurrentIndex);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3320::ViewScreenLinkDM()") );
	
	
	CodeToDesc();

	// ���� / ���� ��ư ����
	SetPreNextButton();
	
	return 0;
	
	EFS_END
	return -1;

}


INT CCO_CENTER_USER_MANAGE_05::SetSelect()
{
	EFS_BEGIN
		
	INT ids;

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3320::SetSelect()") );
	
	ids = pGrid->SetReverse( m_nCurrentIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3320::SetSelect()") );
	
	return 0;
	
	EFS_END
		return -1;
	
}


CESL_Grid* CCO_CENTER_USER_MANAGE_05::GetMainGridPtr()
{
	EFS_BEGIN
		
		
	CString sParentCMAlias = _T("CM_CO_CENTER_USER_MANAGER_01");
	CString sControlAlias = _T("�����̿��������׸���");
		
	
	CESL_ControlMgr *pCM = FindCM( sParentCMAlias );
	if( pCM == NULL ) return NULL;
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sControlAlias));
	if( pGrid == NULL ) return NULL;
	
	return pGrid;
	
	EFS_END
		return NULL;
	
}


// -------------------------------------------------
// nMode 0 -> PREV
//		 1 -> NEXT
INT CCO_CENTER_USER_MANAGE_05::MoveIndex( INT nMode )
{	
	EFS_BEGIN
		
		INT ids;
	ids = SetCurrentIndex();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MoveIndex( INT nMode )") );
	
	if( m_nViewMode == 0 )
	{
		if( nMode == 0 && ( m_nCurrentIndex != m_nFirstIndex )) m_nCurrentIndex--;
		else if( nMode == 1 && ( m_nCurrentIndex != m_nLastIndex ) ) m_nCurrentIndex++;
	}
	else
	{
		CESL_Grid *pGrid = GetMainGridPtr();
		if( nMode == 0 )
		{
			m_nCurrentIndex = pGrid->SelectGetPrev();
		}
		else if( nMode == 1 && ( m_nCurrentIndex != m_nLastIndex ) )
		{
			m_nCurrentIndex = pGrid->SelectGetNext();
		}
	}
	
	return 0;
	
	EFS_END
		return -1;
	
}

/// ����߼����� , ȸ�� ���� ���� 
INT CCO_CENTER_USER_MANAGE_05::CodeToDesc()
{
	EFS_BEGIN
		
	INT ids;
	
//	CString sCMAlias = _T("CM_CO_CENTER_USER_MANAGE_05");
	CString sDMAlias = _T("DM_CO_CENTER_USER_MANAGER_01");

	CString sData;
//	ids = GetControlData( sCMAlias , _T("����߼�����") , sData );
	ids = GetDataMgrData( sDMAlias , _T("����߼�����") , sData , m_nCurrentIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CodeToDesc") );

	if( sData.Compare(_T("H")) == 0 )
		sData = _T("����");
	else
		sData = _T("�ٹ���");
/*
	ids = SetControlData( sCMAlias , _T("����߼�����") , sData );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CodeToDesc") );
*/
	// Combo�� �°� ����
	CComboBox* pCombo = NULL;

	pCombo = (CComboBox*)GetDlgItem(IDC_cmbUSER3);

	CString sTmpData;
	for( INT i = 0 ; i < pCombo->GetCount() ; i++ )
	{
		pCombo->GetLBText( i , sTmpData );

		if( sTmpData.Compare( sData ) == 0 )
			pCombo->SetCurSel(i);
	}

//	ids = GetControlData( sCMAlias , _T("ȸ������") , sData );
	ids = GetDataMgrData( sDMAlias , _T("ȸ������") , sData , m_nCurrentIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CodeToDesc") );
	
	if( sData.Compare(_T("0")) == 0 )
		sData = _T("��ȸ��");
	else
		sData = _T("��ȸ��");
/*
	ids = SetControlData( sCMAlias , _T("ȸ������") , sData );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CodeToDesc") );
*/	
		
	pCombo = (CComboBox*)GetDlgItem(IDC_cmbUSER4);
	
	for( i = 0 ; i < pCombo->GetCount() ; i++ )
	{
		pCombo->GetLBText( i , sTmpData );
		
		if( sTmpData.Compare( sData ) == 0 )
			pCombo->SetCurSel(i);
	}

	return 0;
	
	
	EFS_END
		
		return -1;
}

VOID CCO_CENTER_USER_MANAGE_05::OnbtnSAVEUSER() 
{
	EFS_BEGIN

	AfxGetApp()->DoWaitCursor(-1);

	INT ids = ModifyUser();
	if( ids == 0 )
	{
		AfxMessageBox(_T("�����Ͽ����ϴ�."));
	}

	AfxGetApp()->DoWaitCursor(1);

	EFS_END
}


/// ����
INT CCO_CENTER_USER_MANAGE_05::ModifyUser()
{
	EFS_BEGIN

	INT ids;

	/// 1. Tmp DM ����

	CESL_DataMgr TmpDM;
	
	ids = CopyODMtoTempDM(&TmpDM);
	if( ids > 0 ) return ids;
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ModifyUser") );


	/// 2. CM�� ����� �κ� DM�� ����
	ids = CMToDM(&TmpDM);
	if( ids > 0 ) return ids;
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ModifyUser") );
	

	/// 3. DM���� UpdateFrame���� -> SendFrame
	ids = DMToUpdateFrame(&TmpDM);
	if( ids > 0 ) return ids;
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ModifyUser") );

	/// 4. DM����
	ids = CopyTempDMtoODM(&TmpDM);
	if( ids > 0 ) return ids;
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ModifyUser") );
	
	/// 4. ��

	return 0;

	EFS_END


	return -1;
}

INT CCO_CENTER_USER_MANAGE_05::CopyODMtoTempDM( CESL_DataMgr* pTmpDM )
{
	EFS_BEGIN

	INT ids;

	CString sODMAlias = _T("DM_CO_CENTER_USER_MANAGER_01");

	CESL_DataMgr* pSrcDM = FindDM( sODMAlias );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CopyODMtoTempDM") );

	ids = CLocCommonAPI::MakeCopyDM( this , pSrcDM , pTmpDM );
	if( ids , 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CopyODMtoTempDM") );

	pTmpDM->InsertRow(-1);

	ids = CLocCommonAPI::AddDM( pSrcDM , pTmpDM , m_nCurrentIndex , 0 , this );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CopyODMtoTempDM") );

	return 0;

	EFS_END

	return -1;
}


INT CCO_CENTER_USER_MANAGE_05::CMToDM( CESL_DataMgr* pTmpDM )
{
	EFS_BEGIN

	INT ids;

	const INT nUpdateFieldCnt = 22;

	CString sCMAlias = _T("CM_CO_CENTER_USER_MANAGE_05");
	
	CString sControlAlias[nUpdateFieldCnt] =
	{
		_T("�����ڹ�ȣ") , _T("�����ID") , _T("�߱�Ƚ��") , _T("�̸�") , _T("�ֹε�Ϲ�ȣ") , 
		_T("����") , _T("����TYPE") , _T("�޴���") , _T("E_MAIL") , _T("����߼�����") , 
		_T("�Ҽ�") , _T("����") , _T("����_�����ȣ") , _T("����_�ּ�") , _T("����_��ȭ��ȣ") ,
		_T("�ٹ���_�����ȣ") , _T("�ٹ���_�ּ�") , _T("�ٹ���_��ȭ��ȣ") , _T("�б�") ,_T("�г�") , 
		_T("��") , _T("ȸ������")
	};
	
	
	CString sDMFieldAlias[nUpdateFieldCnt] =
	{
		_T("�����ڹ�ȣ") , _T("������ID") , _T("��߱�Ƚ��") , _T("�̸�") , _T("�ֹε�Ϲ�ȣ") , 
		_T("����") , _T("����_TYPE") , _T("�ڵ���") , _T("E_MAIL") , _T("����߼�����") , 
		_T("����Ҽ�����") , _T("������������") , _T("����_�����ȣ") , _T("����_�ּ�") , _T("����_��ȭ��ȣ") ,
		_T("�ٹ���_�����ȣ") , _T("�ٹ���_�ּ�") , _T("�ٹ���_��ȭ��ȣ") , _T("�б�") ,_T("�г�") , 
		_T("��") , _T("ȸ������")
	};

	CString sValue[nUpdateFieldCnt];

	CESL_ControlMgr* pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CMToDM") );
	
	for( INT i = 0 ; i < nUpdateFieldCnt ; i++ )
	{
		ids = pCM->GetControlMgrData( sControlAlias[i] , sValue[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( - 1, _T("CMToDM") );

		/// ����߼����� ó��
		if( i == 9 )
		{
			if( sValue[i].Compare(_T("����")) == 0 )
				sValue[i] = _T("H");
			else
				sValue[i] = _T("W");
		}
		/// ȸ������ó��
		else if( i == 21 )
		{
			if( sValue[i].Compare(_T("��ȸ��")) == 0 )
				sValue[i] = _T("0");
			else
				sValue[i] = _T("1");			
		}

		ids = pTmpDM->SetCellData( sDMFieldAlias[i] , sValue[i] , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( - 2, _T("CMToDM") );
	}
	
	

	return 0;
	
	EFS_END
		
	return -1;
}

INT CCO_CENTER_USER_MANAGE_05::DMToUpdateFrame( CESL_DataMgr* pTmpDM )
{
	EFS_BEGIN

	INT ids;

	const INT nMasterUpdateFieldCnt = 17;
	const INT nLibUserUpdateFieldCnt = 5;
	
	/// 2���� ������ �ִ�.
	CString sMasterTableName = _T("CO_MASTER_LOAN_USER_TBL");
	CString sLibUserTableName = _T("CO_LIBRARY_USER_TBL");

	CString sMasterDMFieldAlias[nMasterUpdateFieldCnt] =
	{
		_T("�����ڹ�ȣ") , _T("�̸�") , _T("�ֹε�Ϲ�ȣ") , _T("����") , _T("����_TYPE") , 
		_T("�ڵ���") , _T("E_MAIL") , _T("����߼�����") , _T("����_�����ȣ") , _T("����_�ּ�") , 
		_T("����_��ȭ��ȣ") , _T("�ٹ���_�����ȣ") , _T("�ٹ���_�ּ�") , _T("�ٹ���_��ȭ��ȣ") , _T("�б�") ,
		_T("�г�") , _T("��")  
	};

	CString sMasterDBFieldName[nMasterUpdateFieldCnt] =
	{
		_T("USER_NO") , _T("NAME") , _T("CIVIL_NO") , _T("BIRTHDAY") , _T("BIRTHDAY_TYPE") ,
		_T("HANDPHONE") , _T("E_MAIL") , _T("MAIL_SEND_AREA") , _T("H_ZIPCODE") , _T("H_ADDR1") , 
		_T("H_PHONE") , _T("W_ZIPCODE") , _T("W_ADDR1") , _T("W_PHONE") , _T("SCHOOL") , 
		_T("GRADE") , _T("CLASS")

	};

	CString sMasterDBFieldType[nMasterUpdateFieldCnt] =
	{
		_T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") ,
		_T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") ,
		_T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") ,
		_T("STRING") , _T("STRING") 
	};
	
	
	CString sLibUserDMFieldAlias[nLibUserUpdateFieldCnt] =
	{
		_T("������ID") , _T("��߱�Ƚ��") , _T("����Ҽ�����") , _T("������������") , _T("ȸ������")
	};
	
	CString sLibUserDBFieldName[nLibUserUpdateFieldCnt] =
	{
		_T("USER_ID") , _T("REISSUE_CNT") , _T("USER_CLASS_CODE") , _T("USER_POSITION_CODE") , _T("MEMBER_CLASS")
	};


	CString sLibUserDBFieldType[nLibUserUpdateFieldCnt] =
	{
		_T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") 
	};

	CString sMasterRecKey , sLibUserRecKey;

	pTmpDM->GetCellData( _T("�������հ���KEY") , 0 , sMasterRecKey );
	pTmpDM->GetCellData( _T("������������������KEY") , 0 , sLibUserRecKey );
	
	pTmpDM->StartFrame();
	pTmpDM->InitDBFieldData();

	CString sValue;
	for( INT i = 0 ; i < nMasterUpdateFieldCnt ; i++ )
	{
		ids = pTmpDM->GetCellData( sMasterDMFieldAlias[i] , 0 , sValue );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DMToUpdateFrame") );

		pTmpDM->AddDBFieldData( sMasterDBFieldName[i] , sMasterDBFieldType[i] , sValue , TRUE );
	}

	pTmpDM->MakeUpdateFrame( sMasterTableName  , _T("REC_KEY") , _T("NUMERIC") , sMasterRecKey );

	pTmpDM->InitDBFieldData();
	
	for( i = 0 ; i < nLibUserUpdateFieldCnt ; i++ )
	{
		ids = pTmpDM->GetCellData( sLibUserDMFieldAlias[i] , 0 , sValue );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DMToUpdateFrame") );
		
		pTmpDM->AddDBFieldData( sLibUserDBFieldName[i] , sLibUserDBFieldType[i] , sValue , TRUE );
	}
	
	pTmpDM->MakeUpdateFrame( sLibUserTableName  , _T("REC_KEY") , _T("NUMERIC") , sLibUserRecKey );

	pTmpDM->InitDBFieldData();
	
	ids = pTmpDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DMToUpdateFrame") )


	pTmpDM->EndFrame();

	return 0;
	
	EFS_END
		
	return -1;
}

INT CCO_CENTER_USER_MANAGE_05::CopyTempDMtoODM( CESL_DataMgr* pTmpDM )
{
	EFS_BEGIN

	INT ids;

	const INT nMasterUpdateFieldCnt = 17;
	CString sMasterRecKey;


	CESL_Grid* pParentGrid = (CESL_Grid*)FindControl( _T("CM_CO_CENTER_USER_MANAGER_01"),_T("�����̿��������׸���"));
	if( pParentGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CopyTempDMtoODM") );


	pTmpDM->GetCellData( _T("�������հ���KEY") , 0 , sMasterRecKey );

	CString sODMAlias = _T("DM_CO_CENTER_USER_MANAGER_01");
	
	CESL_DataMgr* pSrcDM = FindDM( sODMAlias );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CopyODMtoTempDM") );
	
	CString sMasterDMFieldAlias[nMasterUpdateFieldCnt] =
	{
		_T("�����ڹ�ȣ") , _T("�̸�") , _T("�ֹε�Ϲ�ȣ") , _T("����") , _T("����_TYPE") , 
		_T("�ڵ���") , _T("E_MAIL") , _T("����߼�����") , _T("����_�����ȣ") , _T("����_�ּ�") , 
		_T("����_��ȭ��ȣ") , _T("�ٹ���_�����ȣ") , _T("�ٹ���_�ּ�") , _T("�ٹ���_��ȭ��ȣ") , _T("�б�") ,
		_T("�г�") , _T("��")  
	};

	CString sValue[nMasterUpdateFieldCnt];

	pTmpDM->GetCellData( sMasterDMFieldAlias , nMasterUpdateFieldCnt , sValue , 0 );
	
	CString sCMasterRecKey;
	for( INT i = 0 ; i < pSrcDM->GetRowCount() ; i++ )
	{
		ids = pSrcDM->GetCellData( _T("�������հ���KEY") , i , sCMasterRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CopyTempDMtoODM") );

		if( sCMasterRecKey.Compare( sMasterRecKey )  != 0 ) continue;

		for( INT j = 0 ; j < nMasterUpdateFieldCnt ; j++ )
		{
			ids = pSrcDM->SetCellData( sMasterDMFieldAlias[j] , sValue[j] , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( - 3 , _T("CopyTempDMtoODM") );
		}

		pParentGrid->DisplayLine(i);
	}
		
	return 0;
	
	EFS_END
		
	return -1;
}

HBRUSH CCO_CENTER_USER_MANAGE_05::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
		
/************************************
	 ��� [2008-05-15]
	���̾�α� & ����ƽ ������
	 ************************************/
	// ���⼭���� 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// ������� �߰�

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
