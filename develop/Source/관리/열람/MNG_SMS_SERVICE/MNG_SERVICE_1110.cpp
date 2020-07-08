// MNG_SERVICE_1110.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_SERVICE_1110.h"
#include "..\\..\\..\\SMS\\CO_SMS\\CfgFileApi.h"

#include "../../../SMS/CO_SMS/SMS_MakeMsg.h"
#include "../../../SMS/CO_SMS/SMS_Env_Info.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_SERVICE_1110 dialog


CMNG_SERVICE_1110::CMNG_SERVICE_1110(CESL_Mgr* pParent /*=NULL*/)
	: CMNG_SMS_SERVICE_MIDDLE_DIALOG(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_SERVICE_1110)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_SERVICE_1110::~CMNG_SERVICE_1110()
{	
	
}


VOID CMNG_SERVICE_1110::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_SERVICE_1110)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_SERVICE_1110, CDialog)
	//{{AFX_MSG_MAP(CMNG_SERVICE_1110)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_chkCOM2, OnchkCOM2)
	ON_BN_CLICKED(IDC_chkCOM1, OnchkCOM1)
	ON_BN_CLICKED(IDC_chkCOM3, OnchkCOM3)
	ON_BN_CLICKED(IDC_chkCOM4, OnchkCOM4)
	ON_BN_CLICKED(IDC_chkCOM5, OnchkCOM5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_SERVICE_1110 message handlers

BOOL CMNG_SERVICE_1110::OnInitDialog() 
{
	CDialog::OnInitDialog();

	INT ids = InitESL_Mgr( _T("MNG_SMS_SERVICE_1110") );
	if( 0 > ids )
	{
		AfxMessageBox( _T("InitESL_Mgr") );
	}

	EnableThemeDialogTexture(GetSafeHwnd());
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CMNG_SERVICE_1110::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

VOID CMNG_SERVICE_1110::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if( NULL == GetSafeHwnd() ) 
	{
		return;
	}

	CRect WndPos;
	CWnd* pWnd = GetDlgItem(IDC_MAINGRID);

	if( NULL == pWnd->GetSafeHwnd() )
	{
		return;
	}

	pWnd->GetWindowRect(WndPos);

//	ClientToScreen( WndPos );
	ScreenToClient( WndPos );

	pWnd->MoveWindow( WndPos.left , WndPos.top , WndPos.Width() , cy - WndPos.top - 10 );
}

INT CMNG_SERVICE_1110::ReView()
{
EFS_BEGIN

	// 1. ���� ������ ���� ���� ȭ�鿡 �����ֱ�
	
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_MNG_SMS_SERVICE_1110") , _T("��������������") );
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("ReView") );
	}

	pGrid->Display();
	pGrid->m_bGridEdit = TRUE;
	
	// 2. ���� ���� ȭ�鿡 ǥ�����ֱ�

	CESL_DataMgr* pDM = GetTotalEnvDM();
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("ReView") );
	}

	INT		nRowCnt = pDM->GetRowCount();
	if( 0 < nRowCnt )
	{
		CString sComponent;
		pDM->GetCellData( _T("���ڱ�������") , 0 ,  sComponent );
		ReShowComponentInfo(sComponent);
	}

	// 2009.05.25 ADD BY KSJ : ���� ũ�� ȭ�鿡 ǥ��
	CString		strTitleSize;
	pDM->GetCellData( _T("TITLE_SIZE") , 0 ,  strTitleSize );
	GetDlgItem( IDC_edtBookTitleByte )->SetWindowText( strTitleSize );

	AfxGetApp()->DoWaitCursor(1);
	
	return 0;

EFS_END
return -1;
}


INT CMNG_SERVICE_1110::ReShowComponentInfo( CString sComponent )
{
EFS_BEGIN

	// �������� 
	//	5�ڸ��� ���� 0�̸� ���� ������ ���� , 1�̸� ���� ������ ����
	//	�� : '00000'
	//	1�ڸ� : �̿��ڸ�
	//	2�ڸ� : �⺻�޽���
	//	3�ڸ� : �ݳ�������/��ü��/���ุ����
	//	4�ڸ� : ����
	//	5�ڸ� : ��������

	const INT nCnt = 5;
	INT nIDC[nCnt] =
	{
		IDC_chkCOM1 , IDC_chkCOM3 , IDC_chkCOM4 , IDC_chkCOM5 , IDC_chkCOM2
	};

	INT		i;
	for( i=0 ; i<nCnt ; i++ )
	{
		((CButton*)GetDlgItem(nIDC[i]))->SetCheck(FALSE);

		if( i > sComponent.GetLength()-1 ) 
		{
			continue;
		}

		if( '1' == sComponent.GetAt(i) )
		{
			((CButton*)GetDlgItem(nIDC[i]))->SetCheck(TRUE);

			// 2009.05.08 ADD BY KSJ : ���� Check �Ǹ�, ���� ũ�� ����Ʈ �ڽ� READONLY FALSE�� 
			if( 3 == i )
			{
				CEdit*	pEdtTitleSize = (CEdit*) GetDlgItem ( IDC_edtBookTitleByte );
				DWORD dwStyle = pEdtTitleSize->GetStyle();
				dwStyle &= ES_READONLY;
				if( ES_READONLY == (dwStyle & ES_READONLY) )
				{
					pEdtTitleSize->SetReadOnly( FALSE );
				}
			}
		}
	}

	return 0;

EFS_END
return -1;
}

// TempDM�� ����
INT CMNG_SERVICE_1110::Save( CESL_DataMgr* pTypeEnvDM , CESL_DataMgr* pTotalEnvDM )
{
EFS_BEGIN

	// ��������
	CString		sComponent;
	const INT	nCnt = 5;
	INT nIDC[nCnt] =
	{
		IDC_chkCOM1 , IDC_chkCOM3 , IDC_chkCOM4 , IDC_chkCOM5 , IDC_chkCOM2
	};
	
	INT		i;
	for( i=0 ; i<nCnt ; i++ )
	{
		if( 1 == ((CButton*)GetDlgItem(nIDC[i]))->GetCheck() )
		{
			sComponent += _T("1");
		}
		else
		{
			sComponent += _T("0");
		}
	}

	pTotalEnvDM->SetCellData( _T("���ڱ�������") ,  sComponent , 0  );

	// 2009.04.01 ADD BY KSJ : ���� ũ�� ����
	CString		strTitleSize;
	CEdit*	pedtBookTitleByte	=	(CEdit*)GetDlgItem( IDC_edtBookTitleByte );
	pedtBookTitleByte->GetWindowText( strTitleSize );
	INT		nTitleSize	=	_ttoi( strTitleSize );
	if( 8 > nTitleSize || 30 < nTitleSize )
	{/// ���� ũ��� 8~30���̸� ���� �� �� �ִ�.
		AfxMessageBox(_T("���� ũ��� 8~30 ������ ���ڸ� �Է��Ͽ� �ֽʽÿ�."), MB_ICONWARNING);
		pedtBookTitleByte->SetSel( 0 , -1 );
		return 1;
	}

	pTotalEnvDM->SetCellData( _T("TITLE_SIZE") , strTitleSize , 0  );

	// ���� ������ ��������
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_MNG_SMS_SERVICE_1110") , _T("��������������") );
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("Save") );
	}

	/// ���� , �⺻����
	INT			nNameLength , nMessageLength;
	INT			nRowCnt	=	pTypeEnvDM->GetRowCount();
	CString		sName , sMessage;
	for( i=0 ; i<nRowCnt ; i++ )
	{
		pGrid->GetAt( i , 0 , sName );
		pGrid->GetAt( i , 1 , sMessage );

		// �ڸ��� �˻�
		nNameLength	=	sName.GetLength();
		if( 20 < nNameLength )
		{
			CString sEMsg;
			sEMsg.Format(_T("���� ����(%s)�� 20byte(�ѱ�10��,����20��)�� �ʰ��Ҽ� �����ϴ�.") , sName);
			AfxMessageBox(sEMsg);
			return 1;
		}
		
		// �ڸ��� �˻�
		nMessageLength	=	sMessage.GetLength();
		if( 80 < nMessageLength )
		{
			CString sEMsg;
			sEMsg.Format(_T("%s�Ǳ⺻ ���ڰ� 80byte(�ѱ�40��,����80��)�� �ʰ��Ҽ� �����ϴ�.") , sName);
//			AfxMessageBox(sEMsg);
//			return 2;
		}

		pTypeEnvDM->SetCellData( _T("����") , sName , i );
		pTypeEnvDM->SetCellData( _T("�⺻����") , sMessage , i );
	}
	
	return 0;

EFS_END
return -1;
}

/*
@Create
 - 
@Description
 - �̸����� ���� ����
@Parameter

@Return Value
 - 
 - 0, ��� : ����
 - ���� : ����
@Mini Spec
 1. 
*/
INT CMNG_SERVICE_1110::ShowPreView()
{
	EFS_BEGIN

	// ���� ������ ��������
	CESL_Grid*		pGrid = (CESL_Grid*)FindControl( _T("CM_MNG_SMS_SERVICE_1110") , _T("��������������") );
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("ShowPreView") );
	}

	INT nIndex = pGrid->GetIdx();

	CString		strSMSType;
	GetTypeEnvDM()->GetCellData( _T("Ÿ��") , nIndex , strSMSType);

	SMS_MSG_INFO		SMS_MsgInfo;

	// 2009.04.17 ADD BY KSJ : �̸����� ������ DB���� ���������� ��Ŀ��� DM���� �������� ������� ����
//DEL 	SMS_CONNECTION_INFO SMS_ConnectionInfo;
//DEL 	CSMS_Env_Info		SMSEnvInfo(this);
//DEL 	SMSEnvInfo.MakeSMSEnv();
//DEL 	SMSEnvInfo.GetSMSEnv( strSMSType , &SMS_ConnectionInfo , &SMS_MsgInfo );
	GetTotalEnvDM()->GetCellData( _T("ȸ����ȭ��ȣ"),			0 , SMS_MsgInfo.sCallBackNumber );
	GetTotalEnvDM()->GetCellData( _T("�����������뿩��"),	0 , SMS_MsgInfo.sDivideYN );
	// 2009.10.15 UPDATE BY KSJ : �������� MN_LIBINFO2_TBL�� SMS_LIB_NAME ���������� ����
	//DEL GetTotalEnvDM()->GetCellData( _T("��������"),				0 , SMS_MsgInfo.sLibName );
	SMS_MsgInfo.sLibName = m_strLibName;

	CString		strTitleSize;
	GetTotalEnvDM()->GetCellData( _T("TITLE_SIZE") , 0 , strTitleSize );

	INT		nTitleSize				=	_ttoi( strTitleSize );

	SMS_MsgInfo.nTitleSize			=	nTitleSize;
	SMS_MsgInfo.sDefaultMsg			=	_T("");
	SMS_MsgInfo.sTitle				=	_T("");
	SMS_MsgInfo.sMsgComponentInfo	=	_T("");

	const INT nCnt = 5;
	INT nIDC[nCnt] =
	{
		IDC_chkCOM1 , IDC_chkCOM3 , IDC_chkCOM4 , IDC_chkCOM5 , IDC_chkCOM2
	};
	
	INT		i;
	for( i=0 ; i<nCnt ; i++ )
	{
		if( ((CButton*)GetDlgItem(nIDC[i]))->GetCheck() == 1 )
		{
			SMS_MsgInfo.sMsgComponentInfo += _T("1");

			// 2009.05.08 ADD BY KSJ : ���� Check�� �Ǿ� ������ ���� ũ�� ����Ʈ �ڽ� ReadOnly FALSE
			if( 3 == i )
			{
				CEdit*	pEdtTitleSize = (CEdit*) GetDlgItem ( IDC_edtBookTitleByte );
				DWORD dwStyle = pEdtTitleSize->GetStyle();
				dwStyle &= ES_READONLY;
				if( ES_READONLY == (dwStyle & ES_READONLY) )
				{
					pEdtTitleSize->SetReadOnly( FALSE );
				}
			}
		}
		else
		{
			SMS_MsgInfo.sMsgComponentInfo += _T("0");

			// 2009.05.08 ADD BY KSJ : ���� Check�� �Ǿ� ������ ���� ũ�� ����Ʈ �ڽ� ReadOnly TRUE
			if( 3 == i )
			{
				CEdit*	pEdtTitleSize = (CEdit*) GetDlgItem ( IDC_edtBookTitleByte );
				DWORD dwStyle = pEdtTitleSize->GetStyle();
				dwStyle &= ES_READONLY;
				if( (dwStyle & ES_READONLY) != ES_READONLY)
				{
					pEdtTitleSize->SetReadOnly( TRUE );// �б⸸ ����
				}
			}
		}
	}
	
	pGrid->GetAt( nIndex , 0  , SMS_MsgInfo.sTitle );
	pGrid->GetAt( nIndex , 1  , SMS_MsgInfo.sDefaultMsg );

	CSMS_MakeMsg MakeSMS(this);

	CString		strMsg;
	MakeSMS.MakeSMSMsg( strSMSType , &SMS_MsgInfo , _T("ȫ�浿") , _T("2003/10/10") , _T("���ϼҼ� ����ȫ�浿") , _T("3") , strMsg );

	GetDlgItem(IDC_edtPREVIEW)->SetWindowText( strMsg );

	// 2009.04.01 ADD BY KSJ : ���� ũ�� ��Ʈ�ѿ� �� Setting
	//CString		strTitleSize;
	strTitleSize.Format( _T("%d") , SMS_MsgInfo.nTitleSize );
	GetDlgItem( IDC_edtBookTitleByte )->SetWindowText( strTitleSize );

	return 0;

EFS_END
return -1;
}

BEGIN_EVENTSINK_MAP(CMNG_SERVICE_1110, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_SERVICE_1110)
	ON_EVENT(CMNG_SERVICE_1110, IDC_MAINGRID, -600 /* Click */, OnClickMaingrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_SERVICE_1110::OnClickMaingrid() 
{
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_MNG_SMS_SERVICE_1110") , _T("��������������") );
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_VOID2( -1010 , _T("OnClickMaingrid") );
	}

	if( 1 > pGrid->GetMouseRow() ) 
	{
		return ;
	}

	ShowPreView();
}


HBRUSH CMNG_SERVICE_1110::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

LRESULT CMNG_SERVICE_1110::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// 2009.05.15 ADD BY KSJ : GridSelEditor���� Enter�� �������� ��� �̸����� ����
	if( ESL_GRID_MODIFY == message)
	{
		ShowPreView();
	}

	// 2009.10.15 ADD BY KSJ : �������� Setting
	if( WM_SHOWWINDOW == message )
	{
		if( TRUE == wParam )
		{
			CESL_DataMgr* pDM = GetTotalEnvDM();
			CString strQuery;
			strQuery.Format( _T("SELECT SMS_LIB_NAME FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE") );
			pDM->GetOneValueQuery( strQuery , m_strLibName );

			// 2010.12.22 ADD BY PWR : ȸ�Ź�ȣ, �������� PC������ ����
			CCfgFileApi cfgFileApi;
			CString strCfgFilePath = _T("..\\cfg\\SMS_Callback.cfg");
			cfgFileApi.SetFile(strCfgFilePath);
			CFileFind ff;
			if(ff.FindFile(strCfgFilePath))
			{
				CString strLibNameTarget;
				cfgFileApi.GetData(_T("����������������"), strLibNameTarget);
				if(_T("1") == strLibNameTarget)
				{
					CString strValue;
					cfgFileApi.GetData(_T("��������"), strValue);
					if(!strValue.IsEmpty()) m_strLibName = strValue;
				}
			}
		}
	}
	
	return CDialog::WindowProc(message, wParam, lParam);
}

/*
@Create
 - 2009.05.15 BY KSJ
@Description
 - �������� ����� �̸����� ȭ�� ����
@Parameter

@Return Value
 - 
 - 0, ��� : ����
 - ���� : ����
@Mini Spec
 1. ��������
 2. �������̸�
 3. �⺻����
 4. �ݳ�������/���ุ����
 5. ����
*/
void CMNG_SERVICE_1110::OnchkCOM2() 
{/// 1. ��������
	ShowPreView();		
}

void CMNG_SERVICE_1110::OnchkCOM1() 
{/// 2. �������̸�
	ShowPreView();
}

void CMNG_SERVICE_1110::OnchkCOM3() 
{/// 3. �⺻����
	ShowPreView();
}

void CMNG_SERVICE_1110::OnchkCOM4() 
{/// 4. �ݳ�������/���ุ����
	ShowPreView();
}

void CMNG_SERVICE_1110::OnchkCOM5() 
{/// 5. ����
	ShowPreView();
}
