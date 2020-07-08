// MNG_SERVICE_1120.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_SERVICE_1120.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_SERVICE_1120 dialog


CMNG_SERVICE_1120::CMNG_SERVICE_1120(CESL_Mgr* pParent /*=NULL*/, INT nDialogType)
	: CMNG_SMS_SERVICE_MIDDLE_DIALOG(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_SERVICE_1120)
	m_sBindID = _T("");
	m_sBindPwd = _T("");
	m_sUserAccessPI = _T("");
	m_sUserDomain = _T("");
	m_sUserID = _T("");
	m_sUserPhoneID = _T("");
	//}}AFX_DATA_INIT

	if( nDialogType == 0 )
	{
		m_lpszTemplateName = MAKEINTRESOURCE( IDD );
		m_nIDHelp = IDD;
	}
	else if( nDialogType == 1 )
	{
		m_lpszTemplateName = MAKEINTRESOURCE( IDD2 );	
		m_nIDHelp = IDD2;		
	}

	m_nDialogType = nDialogType;
}

CMNG_SERVICE_1120::~CMNG_SERVICE_1120()
{
	
}


VOID CMNG_SERVICE_1120::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_SERVICE_1120)
	DDX_Text(pDX, IDC_eBIND_ID, m_sBindID);
	DDV_MaxChars(pDX, m_sBindID, 20);
	DDX_Text(pDX, IDC_eBIND_PWD, m_sBindPwd);
	DDV_MaxChars(pDX, m_sBindPwd, 20);
	DDX_Text(pDX, IDC_eUSER_ACCESS_IP, m_sUserAccessPI);
	DDV_MaxChars(pDX, m_sUserAccessPI, 20);
	DDX_Text(pDX, IDC_eUSER_DOMAIN, m_sUserDomain);
	DDV_MaxChars(pDX, m_sUserDomain, 20);
	DDX_Text(pDX, IDC_eUSER_ID, m_sUserID);
	DDV_MaxChars(pDX, m_sUserID, 20);
	DDX_Text(pDX, IDC_eUSER_PHONE_ID, m_sUserPhoneID);
	DDV_MaxChars(pDX, m_sUserPhoneID, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_SERVICE_1120, CDialog)
	//{{AFX_MSG_MAP(CMNG_SERVICE_1120)
	ON_WM_CTLCOLOR()  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_SERVICE_1120 message handlers

BOOL CMNG_SERVICE_1120::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitScreen();

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CMNG_SERVICE_1120::Create(CWnd* pParentWnd) 
{
	
	if ( m_nDialogType == 2 )
	{
		return CDialog::Create(IDD2, pParentWnd);
	}
	else if ( m_nDialogType == 3 )
	{
		return CDialog::Create(IDD3, pParentWnd);
	}	
	else if ( m_nDialogType == 5 )
	{
		return CDialog::Create(IDD4, pParentWnd);
	}
	// 2013.11.14 ADD BY KJS : 2013���������ռ���Ȯ�뱸��װ��� (��ü��ü �߰� �� ��ü ������ �� �ֵ��� ��)
	else if ( m_nDialogType == 0 )
	{
		return CDialog::Create(IDD, pParentWnd);
	}	
	else 
	{
		return CDialog::Create(IDD5, pParentWnd);
	}

	return CDialog::Create(IDD, pParentWnd);	
}


INT CMNG_SERVICE_1120::ReView()
{
	EFS_BEGIN
		
	INT ids;
	
	CESL_DataMgr* pDM = FindDM(_T("DM_MNG_SMS_TOTAL_ENV"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ReView") );
	
	if( pDM->GetRowCount() < 1 ) return 0;
	
	const INT nCnt = 7;
	
	CString sGetAlias[nCnt] = 
	{
		_T("���ε���̵�") , _T("���ε��н�����") , _T("�̿��ھ��̵�") , _T("�̿��ڵ�����") , _T("�̿��ڹ�ȣ") , _T("�̿�������IP"), _T("��Ʈ")
	};
	
	CString sGetData[nCnt];
	
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = pDM->GetCellData( sGetAlias[i] , 0 , sGetData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ReView") );
	}

	INT nIDC[nCnt] = 
	{
		IDC_eBIND_ID , IDC_eBIND_PWD , IDC_eUSER_ID , IDC_eUSER_DOMAIN , IDC_eUSER_PHONE_ID , IDC_eUSER_ACCESS_IP, IDC_ePORT
	};
	
	for( i = 0 ; i < nCnt ; i++ )
	{
		GetDlgItem(nIDC[i])->SetWindowText(sGetData[i]);
	}		
	
	return 0;
	
	EFS_END
		
	return -1;
	
}

// TempDM�� ����
INT CMNG_SERVICE_1120::Save( CESL_DataMgr* pTypeEnvDM , CESL_DataMgr* pTotalEnvDM )
{
	EFS_BEGIN

	/// ����
	const INT nCnt = 7;
	INT nIDC[nCnt] =
	{
		IDC_eBIND_ID , IDC_eBIND_PWD , IDC_eUSER_ID , IDC_eUSER_DOMAIN , IDC_eUSER_PHONE_ID , IDC_eUSER_ACCESS_IP, IDC_ePORT
	};

	CString sDMFieldAlias[nCnt] =
	{
		_T("���ε���̵�") , _T("���ε��н�����") , _T("�̿��ھ��̵�") , _T("�̿��ڵ�����") , _T("�̿��ڹ�ȣ") , _T("�̿�������IP"), _T("��Ʈ")
	};

	CString sGetData[nCnt];
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		GetDlgItem(nIDC[i])->GetWindowText(sGetData[i]);
		pTotalEnvDM->SetCellData( sDMFieldAlias[i] , sGetData[i] , 0 );
	}
	
	
	return 0;
	
	EFS_END
		
	return -1;
	
}



INT CMNG_SERVICE_1120::InitScreen()
{
	EFS_BEGIN
		
		/// 1. �̿����� ����� ���������� �˾ƺ���
	CString sSql;
	sSql.Format( _T("SELECT PERMITION FROM MN_USER_TBL WHERE id = '%s'") , GetUserID() );
	
	CESL_DataMgr* pDM = GetTypeEnvDM();
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitScreen") );
	
	CString sPermition;
	INT ids = pDM->GetOneValueQuery( sSql , sPermition );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitScreen") );
	
	if( sPermition.Compare(_T("U")) == 0 || sPermition.IsEmpty() )
	{
		const INT nReadOnlyCnt = 6;
		INT nIDC[nReadOnlyCnt] =
		{
			IDC_eBIND_ID , IDC_eBIND_PWD , IDC_eUSER_ID , IDC_eUSER_DOMAIN , IDC_eUSER_PHONE_ID , IDC_eUSER_ACCESS_IP				
		};

		for( INT i = 0 ; i < nReadOnlyCnt ; i++ )
		{
			((CEdit*)GetDlgItem(nIDC[i]))->SetReadOnly(TRUE);
		}
	}

	
	
	return 0;
	
	EFS_END
		
	return -1;

}

HBRUSH CMNG_SERVICE_1120::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

