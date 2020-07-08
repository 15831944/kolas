// CO_CENTER_USER_MANAGE_03.cpp : implementation file
//

#include "stdafx.h"
#include "CO_CENTER_USER_MANAGE_03.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_MANAGE_03 dialog


CCO_CENTER_USER_MANAGE_03::CCO_CENTER_USER_MANAGE_03(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCO_CENTER_USER_MANAGE_03)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
// �߰� : DeleteObject���� �Ҹ��� �߰� ��� [2008-05-15]
CCO_CENTER_USER_MANAGE_03::~CCO_CENTER_USER_MANAGE_03()
{
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]
}

VOID CCO_CENTER_USER_MANAGE_03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCO_CENTER_USER_MANAGE_03)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCO_CENTER_USER_MANAGE_03, CDialog)
	//{{AFX_MSG_MAP(CCO_CENTER_USER_MANAGE_03)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnPREV, OnbtnPREV)
	ON_BN_CLICKED(IDC_btnNEXT, OnbtnNEXT)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_MANAGE_03 message handlers

BOOL CCO_CENTER_USER_MANAGE_03::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	if(InitESL_Mgr(_T("CO_CENTER_USER_MANAGE_03")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CCO_CENTER_USER_MANAGE_03::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	bFirstView = FALSE;
	SetScreen();
	bFirstView = TRUE;	
}



VOID CCO_CENTER_USER_MANAGE_03::SetScreen()
{
	EFS_BEGIN

	SetCurrentIndex();
	DisplayWindow();

	EFS_END
}



INT CCO_CENTER_USER_MANAGE_03::DisplayWindow()
{
	EFS_BEGIN

	INT ids;


	// 2. DISPLAY�Ѵ�.
	ids = ViewScreenLinkDM();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CCO_CENTER_USER_MANAGE_03::DisplayWindow()") );
	
	// 3. �θ� �׸��忡 ǥ���Ѵ�.
	ids = SetSelect();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CCO_CENTER_USER_MANAGE_03::DisplayWindow()") );

	return 0;

	EFS_END
	return -1;

}

INT CCO_CENTER_USER_MANAGE_03::SetCurrentIndex()
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

VOID CCO_CENTER_USER_MANAGE_03::SetPreNextButton()
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

VOID CCO_CENTER_USER_MANAGE_03::OnbtnPREV() 
{
	EFS_BEGIN

	MoveIndex(0);	
	DisplayWindow();

	EFS_END
}


VOID CCO_CENTER_USER_MANAGE_03::OnbtnNEXT() 
{
	EFS_BEGIN

	MoveIndex(1);	
	DisplayWindow();

	EFS_END
}



VOID CCO_CENTER_USER_MANAGE_03::OnbtnCLOSE() 
{
	OnCancel();	
}




INT CCO_CENTER_USER_MANAGE_03::ViewScreenLinkDM()
{
	EFS_BEGIN
		
	INT ids;
	
	CString sCMAlias = _T("CM_CO_CENTER_USER_MANAGE_03" );
	
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


INT CCO_CENTER_USER_MANAGE_03::SetSelect()
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


CESL_Grid* CCO_CENTER_USER_MANAGE_03::GetMainGridPtr()
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
INT CCO_CENTER_USER_MANAGE_03::MoveIndex( INT nMode )
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
INT CCO_CENTER_USER_MANAGE_03::CodeToDesc()
{
	EFS_BEGIN
		
	INT ids;
	
	CString sCMAlias = _T("CM_CO_CENTER_USER_MANAGE_03");

	CString sData;
	ids = GetControlData( sCMAlias , _T("����߼�����") , sData );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CodeToDesc") );

	if( sData.Compare(_T("H")) == 0 )
		sData = _T("����");
	else
		sData = _T("�ٹ���");

	ids = SetControlData( sCMAlias , _T("����߼�����") , sData );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CodeToDesc") );

	ids = GetControlData( sCMAlias , _T("ȸ������") , sData );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CodeToDesc") );
	
	if( sData.Compare(_T("0")) == 0 )
		sData = _T("��ȸ��");
	else
		sData = _T("��ȸ��");

	ids = SetControlData( sCMAlias , _T("ȸ������") , sData );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CodeToDesc") );
	
		
	return 0;
	
	
	EFS_END
		
		return -1;
}

HBRUSH CCO_CENTER_USER_MANAGE_03::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
