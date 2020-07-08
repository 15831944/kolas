// MngLocMgrAPI.cpp: implementation of the CMngLocMgrAPI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MngLocMgrAPI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMngLocMgrAPI::CMngLocMgrAPI(CESL_Mgr *pWnd )
	: CSPF_Work(pWnd)
{
	SettingVariable();
}

CMngLocMgrAPI::~CMngLocMgrAPI()
{
	for( INT i = 0 ; i < 4 ; i++ )
	{
		delete []m_sControlAlias[i];
		delete []m_sManageAlias[i];
		delete []m_sManageValue[i];
		delete []m_sResultValue[i];
	}

	delete []m_sControlAlias;
	delete []m_sManageAlias;
	delete []m_sManageValue;
	delete []m_sResultValue;

}

INT CMngLocMgrAPI::AllWndView()
{
	EFS_BEGIN

	INT ids;

	ids = Wnd01View();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("AllWndView") );

	ids = Wnd02View();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("AllWndView") );

	ids = Wnd03View();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("AllWndView") );

	ids = Wnd04View();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("AllWndView") );

	return 0;

	EFS_END
	return -1;

}

// ù��° ȭ�� �����ֱ�
INT CMngLocMgrAPI::Wnd01View()
{
	EFS_BEGIN

	INT ids;
	INT nIndex = 0;

	// Ÿ�Կ� ����..
	ids = ViewScreen( nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("Wnd01View") );

	return 0;

	EFS_END
	return -1;

}

// �ι�° ȭ�� �����ֱ�
INT CMngLocMgrAPI::Wnd02View()
{
	EFS_BEGIN

	INT ids;
	INT nIndex = 1;

	// Ÿ�Կ� ����..
	ids = ViewScreen( nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("Wnd01View") );
	return 0;

	EFS_END
	return -1;

}

// ����° ȭ�� �����ֱ�
INT CMngLocMgrAPI::Wnd03View()
{
	EFS_BEGIN

	INT ids;
	INT nIndex = 2;

	// Ÿ�Կ� ����..
	ids = ViewScreen( nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("Wnd01View") );
	return 0;

	EFS_END
	return -1;

}

// �׹�° ȭ�� �����ֱ�
INT CMngLocMgrAPI::Wnd04View()
{
	EFS_BEGIN

	INT ids;
	CString sDMAlias = _T("DM_MNG_LOC_MGR_04_DLG");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("Wnd01View") );

//	ids = pDM->RefreshDataManager( _T("") );
	ids = CLocCommonAPI::RefreshDataManager( m_pParentMgr , pDM , _T("") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("Wnd01View") ); 

	ids = m_pParentMgr->AllControlDisplay( m_sCMAlias[3] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("Wnd01View") ); 

	return 0;

	EFS_END
	return -1;

}


INT CMngLocMgrAPI::ViewScreen( INT nIndex )
{
	EFS_BEGIN

	/*
	switch( nIndex ) {
	case 0:
			ViewScreen01()
	*/	

	/*
	INT ids;
	CString sValue;
	for( INT i = 0 ; i < m_nControlAliasCnt[nIndex] ; i++ )
	{
		// ���� �����´�.
		// ȭ�鿡 �����ش�.
		ids = m_pParentMgr->GetManageValue( m_sGroup2 , m_sGroup3 , m_sManageAlias[nIndex][i] , m_sManageValue[nIndex][i] , sValue );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ViewScreen") );

		CESL_ControlMgr *pCM = m_pParentMgr->FindCM( m_sCMAlias[nIndex]);
		if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ViewScreen") );

		CESL_Control_Element *pE = pCM->FindControlElement( m_sControlAlias[nIndex][i] );
		if( pE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ViewScreen") );

		if( (m_sResultValue[nIndex][i].Compare(_T("Y"))==0) || (m_sResultValue[nIndex][i].Compare(_T("N"))==0) )
		{
			CButton *pB = (CButton*)(m_pParentMgr->GetDlgItem( pE->CTRL_ID ));
			if( pB == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("ViewScreen") );

			if( m_sResultValue[nIndex][i].Compare( sValue ) == 0 ) 
				pB->SetCheck(1);
			else 
				pB->SetCheck(0);
		}
		else if( m_sResultValue[nIndex][i].Compare(_T("CMB")) == 0 )
		{
			CComboBox *pComboBox = (CComboBox*)(m_pParentMgr->GetDlgItem( pE->CTRL_ID ));
			if( pComboBox == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("ViewScreen") );

			ids = pComboBox->SetCurSel( _ttoi(sValue) );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("ViewScreen") );

		}
		else if( m_sResultValue[nIndex][i].Compare(_T("EDT")) == 0 )
		{
			ids = m_pParentMgr->SetDataMgrData( m_sCMAlias[nIndex] , m_sControlAlias[nIndex][i] , sValue );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("ViewScreen") );
		}
		
	}
	*/

	return 0;

	EFS_END
	return -1;

}

VOID CMngLocMgrAPI::SettingVariable()
{
	EFS_BEGIN

	/*
	const INT nCnt = 4;
	INT nControlAliasCnt[nCnt];
	nControlAliasCnt[0] = 10;
	nControlAliasCnt[1] = 17;
	nControlAliasCnt[2] = 14;
	nControlAliasCnt[3] = 1;

	for( INT i = 0 ; i < nCnt ; i++ )
		m_nControlAliasCnt[i] = nControlAliasCnt[i] ;

	m_sControlAlias = new CString*[nCnt];
	m_sManageAlias = new CString*[nCnt];
	m_sManageValue = new CString*[nCnt];
	m_sResultValue = new CString*[nCnt];

	m_sCMAlias = new CString[nCnt];

	CString sCMAlias[nCnt] = 
	{
		_T("CM_MNG_LOC_MGR_01_DLG") , _T("CM_MNG_LOC_MGR_02_DLG") , _T("CM_MNG_LOC_MGR_03_DLG") , _T("CM_MNG_LOC_MGR_04_DLG")
	};


	CString sControlAlias01[10][4] =
	{
		{ _T("��������⿩��Y") , _T("�������") , _T("��������⿩��") , _T("Y") } ,
		{ _T("��������⿩��N") , _T("�������") , _T("��������⿩��") , _T("N") } ,
		{ _T("�������⿩��Y") , _T("�������") , _T("��������⿩��") , _T("Y")} ,
		{ _T("�������⿩��N") , _T("�������") , _T("��������⿩��") , _T("N") } ,
		{ _T("�񵵼����⿩��Y") , _T("�������") , _T("�񵵼�����⿩��") ,_T("Y") } ,
		{ _T("�񵵼����⿩��N") , _T("�������") , _T("�񵵼�����⿩��") , _T("N") } ,
		{ _T("��������⿩��Y") , _T("�������") , _T("��������⿩��") , _T("Y") } ,
		{ _T("��������⿩��N") , _T("�������") , _T("��������⿩��") , _T("N") } ,
		{ _T("�������а����⿩��Y") , _T("�������") , _T("�ٸ��������а����⿩��") , _T("Y")} ,
		{ _T("�������а����⿩��N") , _T("�������") , _T("�ٸ��������а����⿩��") , _T("N") }
	};
	CString sControlAlias02[17][4] =
	{
		{ _T("�Ϲݴ����뿩��Y") , _T("�������Ļ�뿩��") , _T("�Ϲ�") , _T("Y") } ,
		{ _T("�Ϲݴ����뿩��N") , _T("�������Ļ�뿩��") , _T("�Ϲ�") , _T("N") } ,
		{ _T("���������뿩��Y") , _T("�������Ļ�뿩��") , _T("����") , _T("Y")} ,
		{ _T("���������뿩��N") , _T("�������Ļ�뿩��") , _T("����") , _T("N") } ,
		{ _T("Ư�������뿩��Y") , _T("�������Ļ�뿩��") , _T("Ư��") , _T("Y")} ,
		{ _T("Ư�������뿩��N") , _T("�������Ļ�뿩��") , _T("Ư��") , _T("N") } ,
		{ _T("�Ϲݹݳ���뿩��Y") , _T("�ݳ����Ļ�뿩��") , _T("�Ϲ�") , _T("Y")} ,
		{ _T("�Ϲݹݳ���뿩��N") , _T("�ݳ����Ļ�뿩��") , _T("�Ϲ�") , _T("N") } ,
		{ _T("�ϰ��ݳ���뿩��Y") , _T("�ݳ����Ļ�뿩��") , _T("�ϰ�") , _T("Y")} ,
		{ _T("�ϰ��ݳ���뿩��N") , _T("�ݳ����Ļ�뿩��") , _T("�ϰ�") , _T("N") } ,
		{ _T("�ұ޹ݳ���뿩��Y") , _T("�ݳ����Ļ�뿩��") , _T("�ұ�") , _T("Y")} ,
		{ _T("�ұ޹ݳ���뿩��N") , _T("�ݳ����Ļ�뿩��") , _T("�ұ�") , _T("N") } ,
		{ _T("�ݳ�����ÿ���켱��������") , _T("�ݳ��������") , _T("�ݳ����⿹��켱����") , _T("CMB")} ,
		{ _T("�����ɻ�뿩��Y") , _T("�������") , _T("�����ɻ�뿩��") , _T("Y") } ,
		{ _T("�����ɻ�뿩��N") , _T("�������") , _T("�����ɻ�뿩��") , _T("N") } ,
		{ _T("�����ڵ���ҿ���Y") , _T("�������") , _T("�����ڵ���ҿ���") , _T("Y") } ,
		{ _T("�����ڵ���ҿ���N") , _T("�������") , _T("�����ڵ���ҿ���") , _T("N") }
	};
	CString sControlAlias03[14][4] =
	{
		{ _T("���������Ϻο�����Y") , _T("��ü����") , _T("���������Ϻο�����") , _T("Y")} , 
		{ _T("���������Ϻο�����N") , _T("��ü����") , _T("���������Ϻο�����") , _T("N")} , 
		{ _T("��ü��ο�����Y") , _T("��ü����") , _T("��ü��ο�����") , _T("Y")} , 
		{ _T("��ü��ο�����N") , _T("��ü����") , _T("��ü��ο�����") , _T("N")} , 
		{ _T("��ü��ο����") , _T("��ü����") , _T("��ü�ϸ��") , _T("CMB")} , 
		{ _T("��ü�ᰡ��ġ") , _T("��ü����") , _T("��ü����ġ") , _T("EDT")} , 
		{ _T("�⺻��ü��") , _T("��ü����") , _T("��ü��") , _T("EDT")} , 
		{ _T("������üȽ���ο����") , _T("��ü����") , _T("������üȽ���ο����") , _T("CMB")} , 
		{ _T("����ȸ����ɻ�뿩��Y"), _T("�̿��ڰ���") , _T("����ȸ����ɻ�뿩��") , _T("Y")} , 
		{ _T("����ȸ����ɻ�뿩��N") , _T("�̿��ڰ���") , _T("����ȸ����ɻ�뿩��") , _T("N")} , 
		{ _T("�����ڹ�ȣ�ڵ��ο�����Y") , _T("�̿��ڰ���") , _T("�����ڹ�ȣ�ڵ��ο�����") , _T("Y")} , 
		{ _T("�����ڹ�ȣ�ڵ��ο�����N") , _T("�̿��ڰ���") , _T("�����ڹ�ȣ�ڵ��ο�����") , _T("N")} , 
		{ _T("�����������ڵ��ϰ�������ɻ�뿩��Y") , _T("�̿��ڰ���") , _T("���������ϰ���������") , _T("Y")} , 
		{ _T("�����������ڵ��ϰ�������ɻ�뿩��N") , _T("�̿��ڰ���") , _T("���������ϰ���������") , _T("N")}
	};
	CString sControlAlias04[1][4] =
	{
		{ _T("�������޺�ȯ�漳���׸���") , _T("��Ÿ") , _T("��Ÿ") }
	};

	for( i=0 ; i < nCnt ; i++ )
	{
		m_sControlAlias[i] = new CString[nControlAliasCnt[i]];
		m_sManageAlias[i] = new CString[nControlAliasCnt[i]];
		m_sManageValue[i] = new CString[nControlAliasCnt[i]];
		m_sResultValue[i] = new CString[nControlAliasCnt[i]];

		m_sCMAlias[i] = sCMAlias[i];

		for( INT j = 0 ; j < nControlAliasCnt[i] ; j++ )
		{

			if( i == 0 )
			{
				m_sControlAlias[i][j] = sControlAlias01[j][0];
				m_sManageAlias[i][j] = sControlAlias01[j][1];
				m_sManageValue[i][j] = sControlAlias01[j][2];
			}
			else if( i == 1 )
			{
				m_sControlAlias[i][j] = sControlAlias02[j][0];
				m_sManageAlias[i][j] = sControlAlias02[j][1];
				m_sManageValue[i][j] = sControlAlias02[j][2];
			}
			else if( i == 2 ) 
			{
				m_sControlAlias[i][j] = sControlAlias03[j][0];
				m_sManageAlias[i][j] = sControlAlias03[j][1];
				m_sManageValue[i][j] = sControlAlias03[j][2];
			}
			else if( i == 3 )
			{
				m_sControlAlias[i][j] = sControlAlias04[j][0];
				m_sManageAlias[i][j] = sControlAlias04[j][1];
				m_sManageValue[i][j] = sControlAlias04[j][2];
			}
		}
	}
	*/

EFS_END
}