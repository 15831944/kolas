// SMS_LifeLong_Contents_Editor.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_LifeLong_Contents_Editor.h"
#include "SMS_LifeLong1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMS_LifeLong_Contents_Editor dialog


CSMS_LifeLong_Contents_Editor::CSMS_LifeLong_Contents_Editor(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMS_LifeLong_Contents_Editor)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CSMS_LifeLong_Contents_Editor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMS_LifeLong_Contents_Editor)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMS_LifeLong_Contents_Editor, CDialog)
	//{{AFX_MSG_MAP(CSMS_LifeLong_Contents_Editor)
	ON_CBN_SELCHANGE(IDC_cSMS_TYPE, OnSelchangecSMSTYPE)
	ON_BN_CLICKED(IDC_bSAVE, OnbSAVE)
	ON_EN_CHANGE(IDC_eSMS_CONTENTS, OnChangeeSMSCONTENTS)
	ON_BN_CLICKED(IDC_chkLibName, OnchkLibName)
	ON_BN_CLICKED(IDC_chkReciever, OnchkReciever)
	ON_BN_CLICKED(IDC_chkProgram, OnchkProgram)
	ON_BN_CLICKED(IDC_chkOpen, OnchkOpen)
	ON_BN_CLICKED(IDC_chkDefaultMsg, OnchkDefaultMsg)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_EN_CHANGE(IDC_eURL, OnChangeeURL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_LifeLong_Contents_Editor message handlers

BOOL CSMS_LifeLong_Contents_Editor::Create(CWnd* pParentWnd) 
{
EFS_BEGIN	
	return CDialog::Create(IDD, pParentWnd);
EFS_END
	return FALSE;
}

BOOL CSMS_LifeLong_Contents_Editor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids;
	/////////////////////////////////////////////////////////////////////////
	// 0. ������ȭ�� ���߱�
	/////////////////////////////////////////////////////////////////////////
	FILE* pf = _tfopen( _T("..\\CFG\\_debug"), _T("rb") );
	if ( pf == NULL )
	{
		CRect rc;
		this->GetClientRect(rc);		
		this->MoveWindow( rc.left,rc.top, rc.Width() - 310, rc.Height() + 25 );
	}

	/////////////////////////////////////////////////////////////////////////
	// 1. SMS �޺��ڽ��� �ʱ�ȭ �Ѵ�.
	/////////////////////////////////////////////////////////////////////////
	ids = InitSMSGroup();
	if (ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Contents_Editor::OnInitDialog") );

	/////////////////////////////////////////////////////////////////////////
	// 2. ���� ���� üũ�ڽ� �ʱ�ȭ 
	/////////////////////////////////////////////////////////////////////////
//	ids =InitContent();
//	if (ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Contents_Editor::OnInitDialog") );
	/////////////////////////////////////////////////////////////////////////
	// 3. �߰� ���� �ʱ�ȭ 
	/////////////////////////////////////////////////////////////////////////
	ids = InitSMSExInfo();
	if (ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CSMS_LifeLong_Contents_Editor::OnInitDialog") );
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// SMS ���� �޺��ڽ� �ʱ�ȭ
//////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Contents_Editor::InitSMSGroup()
{
EFS_BEGIN
	
	INT ids;

	///////////////////////////////////////////////////////////////////////////////////
	// 1. SMS DM �ʱ�ȭ 
	///////////////////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_SMSGroup") );
	if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Contents_Editor::InitSMSGroup") );
	ids = pDM->RefreshDataManager( _T("") );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Contents_Editor::InitSMSGroup") );

	///////////////////////////////////////////////////////////////////////////////////
	// 2. �޺��ڽ��� �Է�
	///////////////////////////////////////////////////////////////////////////////////
	INT nCnt = pDM->GetRowCount();
	CString sTemp = _T("");
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_cSMS_TYPE);

	for (INT i = 0;i < nCnt;i++)
	{
		pDM->GetCellData( _T("SMS��"), i, sTemp);
		sTemp.TrimLeft();sTemp.TrimRight();
		if ( !sTemp.IsEmpty() )
			pCB->AddString(sTemp);		
	}
	
	return 0;
EFS_END
	return -1;
}

//////////////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// SMS ���� �����̺�Ʈ
//////////////////////////////////////////////////////////////////////////////////////
VOID CSMS_LifeLong_Contents_Editor::OnSelchangecSMSTYPE() 
{
	INT ids;

	///////////////////////////////////////////////////////////////////////////////////
	// 1. ���� ���õ� ���� �д´�.
	///////////////////////////////////////////////////////////////////////////////////
	CString sSMSName = _T("");
	CString sErrMsg  = _T("");
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_cSMS_TYPE);
	pCB->GetLBText(pCB->GetCurSel(), sSMSName);
	if ( sSMSName.IsEmpty() )
	{
		sErrMsg.Format( _T("���õ� SMS������ �����ϴ�.") );
		AfxMessageBox(sErrMsg);
		return;
	}
	
	///////////////////////////////////////////////////////////////////////////////////
	// 2. ���õ� ���� DEFAULT_MESSAGE�� ȭ�鿡 �Ѹ���.
	///////////////////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_SMSGroup") );
	if (pDM == NULL) 
	{
		sErrMsg.Format( _T("ERROR POSITION : CSMS_LifeLong_Contents_Editor::OnSelchangecSMSTYPE()") );
		AfxMessageBox(sErrMsg);
		return;
	}

	INT nCnt = pDM->GetRowCount();
	CString sTemp = _T("");

	for (INT i = 0;i < nCnt;i++)
	{
		pDM->GetCellData( _T("SMS��"), i, sTemp );
		if ( _tcscmp(sTemp, sSMSName) == 0 )
		{
			pDM->GetCellData( _T("Ÿ��"), i, m_sSMSType );
			GetDefaultMsg( &sTemp );
			CEdit* pEdit = ( CEdit* )GetDlgItem( IDC_eSMS_CONTENTS );
			pEdit->SetWindowText( sTemp );
			
			// ADD WORK üũ�ڽ� ���� ����
			if ( m_sSMSType == _T("LFE03") || m_sSMSType == _T("LFE04") )
			{
				GetDlgItem( IDC_chkProgram )->EnableWindow( FALSE );
				GetDlgItem( IDC_chkOpen )->EnableWindow( FALSE );
			}
			else
			{	
				GetDlgItem( IDC_chkProgram )->EnableWindow( TRUE );
				GetDlgItem( IDC_chkOpen )->EnableWindow( TRUE );
			}			

			// ADD WORK üũ�ڽ� ��ȭ
			ids = SetChkContents( m_sSMSType );
			if (ids < 0) 
			{
				sErrMsg.Format( _T("ERROR POSITION : CSMS_LifeLong_Contents_Editor::OnSelchangecSMSTYPE()") );
				AfxMessageBox(sErrMsg);
				return;
			}

			break;
		}
	}

	SetContentByte();
	///////////////////////////////////////////////////////////////////////////////////
	// 3. ������ ����
	///////////////////////////////////////////////////////////////////////////////////
	pDM->GetCellData( _T("REC_KEY"), i, sTemp);
	CEdit* pEdt = (CEdit*)GetDlgItem(IDC_TypeInfo1);
	pEdt->SetWindowText( sTemp );

	pEdt = (CEdit*)GetDlgItem(IDC_TypeInfo2);
	pEdt->SetWindowText( m_sSMSType );

	pDM->GetCellData( _T("�ڵ��߼�"), i, sTemp);
	pEdt = (CEdit*)GetDlgItem(IDC_TypeInfo3);
	pEdt->SetWindowText( sTemp );

	pDM->GetCellData( _T("SMS��"), i, sTemp );	
	pEdt = (CEdit*)GetDlgItem(IDC_TypeInfo4);
	pEdt->SetWindowText( sTemp );
}

//////////////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// CFG ������ ������ �°� üũ �ڽ� ǥ��
// ** ������ 
//////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Contents_Editor::InitContent()
{
	CString sErrMsg = _T("");
	///////////////////////////////////////////////////////////////////////////////////
	// 1. ���� ���� üũ�ڽ� ǥ��
	//    �������� 
	//    ������
	//    ���ڸ�
	//    ������
	//    �⺻�޼���
	///////////////////////////////////////////////////////////////////////////////////
	CStdioFile File;
	if ( !File.Open(FILE_PATH, CFile::modeRead | CFile::typeBinary) )
	{
		sErrMsg.Format( _T("%s ������ ã���� �����ϴ�."), FILE_PATH );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	TCHAR cUni;
	File.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		File.SeekToBegin();
	}

	CString sTemp = _T("");

	while ( File.ReadString( sTemp ) )
	{
		if ( _tcscmp( sTemp.Left(1) , _T("#") ) == 0 )	continue;
		else										break;
	}

	File.Close();
	
	if ( sTemp.GetLength() != 5 )
	{
		sErrMsg.Format( _T("������������ �߸��Ǿ� �ֽ��ϴ�."), FILE_PATH );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	INT nChk;
	CButton* pBT;
	// ��������
	nChk = _ttoi( sTemp.Mid(0,1) );
	pBT = (CButton*)GetDlgItem(IDC_chkLibName);
	pBT->SetCheck(nChk);
	// �����ڸ�
	nChk = _ttoi( sTemp.Mid(1,1) );
	pBT = (CButton*)GetDlgItem(IDC_chkReciever);
	pBT->SetCheck(nChk);
	// ���¸�
	nChk = _ttoi( sTemp.Mid(2,1) );
	pBT = (CButton*)GetDlgItem(IDC_chkProgram);
	pBT->SetCheck(nChk);
	// ������
	nChk = _ttoi( sTemp.Mid(3,1) );
	pBT = (CButton*)GetDlgItem(IDC_chkOpen);
	pBT->SetCheck(nChk);

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// �߰� ����
//////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Contents_Editor::InitSMSExInfo()
{
	INT ids;

	///////////////////////////////////////////////////////////////////////////////////
	// 1. SMS �߰����� DM �ʱ�ȭ 
	///////////////////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_SMSInfo") );
	if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Contents_Editor::InitSMSExInfo") );

	// ���� ������ �����´� 
	CString sQuery = _T("");
	sQuery.Format( _T("SMS_TYPE='3'") );
	ids = pDM->RefreshDataManager( sQuery );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Contents_Editor::InitSMSExInfo") );

	///////////////////////////////////////////////////////////////////////////////////
	// 2. ������ ǥ���Ѵ�. 
	//    1�� �ο� ������ ��ȿ�ϴٰ� �����Ѵ�.
	///////////////////////////////////////////////////////////////////////////////////
	// 2.1) ȸ�Ź�ȣ
	CString sGetValue = _T("");
	pDM->GetCellData( _T("ȸ�Ź�ȣ"), 0, sGetValue );
	SetDlgItemText(IDC_eRECEIVE_NUMBER, sGetValue);
	// 2.2) URL
	pDM->GetCellData( _T("URL"), 0, sGetValue );
	SetDlgItemText(IDC_eURL, sGetValue);
	// ADD WORK MSG_GB �߰�
	// ����� ������ �����Ѵ�.
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_COMBO1);
	pCB->AddString(_T("SMS"));
	pCB->AddString(_T("URL"));

	pDM->GetCellData( _T("MSG_GB"), 0, sGetValue );
	SetDlgItemText(IDC_eURL, sGetValue);
	if ( _tcscmp( sGetValue, _T("0") ) == 0 )
		pCB->SetCurSel(0);
	else if ( _tcscmp( sGetValue, _T("5") ) == 0 )
		pCB->SetCurSel(1);

	// ���Ϸ� ������ �����̴�. ////////////////////////////////////////////////////////
	// 2.3) REC_KEY
	pDM->GetCellData( _T("REC_KEY"), 0, sGetValue );
	SetDlgItemText(IDC_TotalInfo1, sGetValue);
	// 2.4) �߶󺸳���
	pDM->GetCellData( _T("�߶󺸳���"), 0, sGetValue );
	SetDlgItemText(IDC_TotalInfo2, sGetValue);
	// 2.5) ���ε�ID
	pDM->GetCellData( _T("���ε�ID"), 0, sGetValue );
	SetDlgItemText(IDC_TotalInfo3, sGetValue);
	// 2.6) ���ε�PWD
	pDM->GetCellData( _T("���ε�PW"), 0, sGetValue );
	SetDlgItemText(IDC_TotalInfo4, sGetValue);
	// 2.7) SMS_TYPE
	pDM->GetCellData( _T("Ÿ��"), 0, sGetValue );
	SetDlgItemText(IDC_TotalInfo5, sGetValue);
	// 2.8) MSG_GB
	pDM->GetCellData( _T("MSG_GB"), 0, sGetValue );
	SetDlgItemText(IDC_TotalInfo6, sGetValue);

	// 2006-01-04 ADD BY PDW(Jr) =====================================================
	// 2.9) ��������
	pDM->GetCellData( _T("��������"), 0, sGetValue );
	SetDlgItemText(IDC_edtLibName, sGetValue);
	// ===============================================================================
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// CFG ������ ������ �°� üũ �ڽ� ǥ��
//////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Contents_Editor::SetChkContents(CString sType)
{
	///////////////////////////////////////////////////////////////////////////////////
	// 1. ���� ���� üũ�ڽ� ǥ��
	//    �������� 
	//    ������
	//    ���ڸ�
	//    ������
	///////////////////////////////////////////////////////////////////////////////////
	CString sTemp = _T("");
	GetCFGContents( &sTemp );

	INT nChk;
	CButton* pBT;
	// ��������
	nChk = _ttoi( sTemp.Mid(0,1) );
	pBT = (CButton*)GetDlgItem(IDC_chkLibName);
	pBT->SetCheck(nChk);
	// �����ڸ�
	nChk = _ttoi( sTemp.Mid(1,1) );
	pBT = (CButton*)GetDlgItem(IDC_chkReciever);
	pBT->SetCheck(nChk);
	// ���¸�
	nChk = _ttoi( sTemp.Mid(2,1) );
	pBT = (CButton*)GetDlgItem(IDC_chkProgram);
	pBT->SetCheck(nChk);
	// ������
	nChk = _ttoi( sTemp.Mid(3,1) );
	pBT = (CButton*)GetDlgItem(IDC_chkOpen);
	pBT->SetCheck(nChk);
	// �⺻�޼���
	nChk = _ttoi( sTemp.Mid(4,1) );
	pBT = (CButton*)GetDlgItem(IDC_chkDefaultMsg);
	pBT->SetCheck(nChk);

	return 0;
}

VOID CSMS_LifeLong_Contents_Editor::OnbSAVE() 
{
	INT ids;

	ids = UpdateInDB();
	if (ids < 0)
	{
		AfxMessageBox( _T("���忡 ���� �߽��ϴ�.") );
		return;
	}

	AfxMessageBox( _T("���� �߽��ϴ�.") );

	SetContentByte();
}

//////////////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// ȭ�� ������ DB�� �ݿ��Ѵ�.
//////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Contents_Editor::UpdateInDB()
{	
	INT ids;

	///////////////////////////////////////////////////////////////////////////////////
	// 1. SMS�� ���õǾ� �ִ��� Ȯ���Ѵ�.
	///////////////////////////////////////////////////////////////////////////////////
	CString sRecKey = _T("");
	GetDlgItemText( IDC_TypeInfo1, sRecKey );
	if ( sRecKey.IsEmpty() )
	{
		CString sErrMsg = _T("");
		sErrMsg.Format( _T("���õ� SMS ������ �����ϴ�.") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	ids = UpdateTypeInfo();
	if (ids < 0) return ids;

	ids = UpdateTotalInfo();
	if (ids < 0) return ids;

	ids = UpdateContentInfo();
	if (ids < 0) return ids;
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// MN_SMS_TYPE_ENV_TBL UPDATE
//////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Contents_Editor::UpdateTypeInfo()
{
	INT ids;
	
	CESL_DataMgr *pDM = FindDM( _T("DM_SMS_LifeLong_SMSGroup") );
	if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Contents_Editor::UpdateTypeInfo") );
	
	const INT nTypeCnt = 4;

	////////////////////////////////////////////////////////////////////////////////
	// 1. �ʵ� �˸��ƽ� 
	////////////////////////////////////////////////////////////////////////////////
	CString sFieldAlias[nTypeCnt] = { _T("DEFAULT_MESSAGE"),  _T("TYPE"),   _T("NAME"),   _T("AUTO_SEND_YN") };
 
	////////////////////////////////////////////////////////////////////////////////
	// 2. �ʵ� Ÿ��
	////////////////////////////////////////////////////////////////////////////////
	CString sFieldType[nTypeCnt]  = { _T("STRING"),           _T("STRING"), _T("STRING"), _T("STRING") };

	////////////////////////////////////////////////////////////////////////////////
	// 3. ȭ�� ������
	////////////////////////////////////////////////////////////////////////////////
	CString sUpdateData[nTypeCnt];
	GetDlgItemText( IDC_eSMS_CONTENTS, sUpdateData[0] );
	GetDlgItemText( IDC_TypeInfo2,     sUpdateData[1] );
	GetDlgItemText( IDC_TypeInfo4,     sUpdateData[2] );
	GetDlgItemText( IDC_TypeInfo3,	   sUpdateData[3] );

	////////////////////////////////////////////////////////////////////////////////
	// 4. REC_KEY ���ϱ� 
	////////////////////////////////////////////////////////////////////////////////
	/*	ȭ��� �ѷ��� REC_KEY�� �����´�
	CString sQuery = T(_T(""));
	sQuery.Format ( _T("SELECT MIN(REC_KEY) FROM MN_SMS_TYPE_ENV_TBL WHERE TYPE = '%s'"), sFieldType[0] );

	CString sRecKey = _T("");
	ids = pDM->GetOneValueQuery( sQuery , sRecKey );	
	if (ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Contents_Editor::UpdateTypeInfo") );

	ids = pDM->StartFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CSMS_LifeLong_Contents_Editor::UpdateTypeInfo") );
	*/	
	CString sRecKey = _T("");
	GetDlgItemText( IDC_TypeInfo1, sRecKey );

	////////////////////////////////////////////////////////////////////////////////
	// 5. ������ ����
	////////////////////////////////////////////////////////////////////////////////
	ids = pDM->StartFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CSMS_LifeLong_Contents_Editor::UpdateTypeInfo") );

	pDM->InitDBFieldData();	
	for (INT i = 0;i < nTypeCnt;i++)
	{
		ids = pDM->AddDBFieldData( sFieldAlias[i]	, sFieldType[i] , sUpdateData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CSMS_LifeLong_Contents_Editor::UpdateTypeInfo") );
	}
			
	ids = pDM->MakeUpdateFrame( _T("MN_SMS_TYPE_ENV_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CSMS_LifeLong_Contents_Editor::UpdateTypeInfo") );

	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CSMS_LifeLong_Contents_Editor::UpdateTypeInfo") );
	
	pDM->EndFrame();

	ids = pDM->RefreshDataManager( _T("") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("CSMS_LifeLong_Contents_Editor::UpdateTypeInfo") );

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// MN_SMS_TOTAL_ENV_TBL UPDATE
//////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Contents_Editor::UpdateTotalInfo()
{
	INT ids;

	CESL_DataMgr *pDM = FindDM( _T("DM_SMS_LifeLong_SMSInfo") );
	if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Contents_Editor::UpdateTotalInfo") );
	
	const INT nTotalCnt = 7;

	////////////////////////////////////////////////////////////////////////////////
	// 1. �ʵ� �˸��ƽ�
	////////////////////////////////////////////////////////////////////////////////
	CString sFieldAlias[nTotalCnt] = { _T("CALLBACK_PHONE"),  _T("URL"),       _T("SEND_DIVIDE_YN"),
		                               _T("BIND_ID"),         _T("BIND_PWD"),  _T("SMS_TYPE")      ,
									   _T("MSG_GB")													};
 
	////////////////////////////////////////////////////////////////////////////////
	// 2. �ʵ� Ÿ��
	////////////////////////////////////////////////////////////////////////////////
	CString sFieldType[nTotalCnt]  = { _T("STRING"),          _T("STRING"),    _T("STRING"), 
		                               _T("STRING"),          _T("STRING"),    _T("STRING"),
									   _T("STRING")										    };

	////////////////////////////////////////////////////////////////////////////////
	// 3. ȭ�� ������
	////////////////////////////////////////////////////////////////////////////////
	CString sUpdateData[nTotalCnt];
	GetDlgItemText(IDC_eRECEIVE_NUMBER, sUpdateData[0]);
	GetDlgItemText(IDC_eURL,            sUpdateData[1]);
	GetDlgItemText(IDC_TotalInfo2,      sUpdateData[2]);
	GetDlgItemText(IDC_TotalInfo3,      sUpdateData[3]); 
	GetDlgItemText(IDC_TotalInfo4,      sUpdateData[4]); 
	GetDlgItemText(IDC_TotalInfo5,      sUpdateData[5]);
	
	CString sTemp = _T("");
	GetDlgItemText( IDC_COMBO1, sTemp );
	if ( _tcscmp( sTemp , _T("SMS") ) == 0 )
		sUpdateData[6] = _T("0");
	else if ( _tcscmp( sTemp , _T("URL") ) == 0 )
		sUpdateData[6] = _T("5");
	////////////////////////////////////////////////////////////////////////////////
	// 4. REC_KEY ���ϱ� 
	////////////////////////////////////////////////////////////////////////////////
	CString sRecKey = _T("");
	GetDlgItemText(IDC_TotalInfo1, sRecKey);

	////////////////////////////////////////////////////////////////////////////////
	// 5. ������ ����
	////////////////////////////////////////////////////////////////////////////////
	ids = pDM->StartFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Contents_Editor::UpdateTotalInfo") );

	pDM->InitDBFieldData();	
	for (INT i = 0;i < nTotalCnt;i++)
	{
		ids = pDM->AddDBFieldData( sFieldAlias[i]	, sFieldType[i] , sUpdateData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CSMS_LifeLong_Contents_Editor::UpdateTotalInfo") );
	}

	ids = pDM->MakeUpdateFrame( _T("MN_SMS_TOTAL_ENV_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CSMS_LifeLong_Contents_Editor::UpdateTotalInfo") );

	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CSMS_LifeLong_Contents_Editor::UpdateTotalInfo") );
	
	pDM->EndFrame();
	
	ids = pDM->RefreshDataManager( _T("") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CSMS_LifeLong_Contents_Editor::UpdateTotalInfo") );

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// CFG���� ���� UPDATE
//////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Contents_Editor::UpdateContentInfo()
{
	//////////////////////////////////////////////////////////////////////////////////
	// 1. ���� üũ ������ ���Ѵ�.
	//////////////////////////////////////////////////////////////////////////////////
	CString sCurStatus = _T("");
	GetCurContents(&sCurStatus);

	//////////////////////////////////////////////////////////////////////////////////
	// 2. ������ ��� Ÿ���� ���ٸ� ������ ��ȯ�ؼ� �ִ´�.
	//////////////////////////////////////////////////////////////////////////////////
	CStdioFile File;
	if ( !File.Open(FILE_PATH, CFile::modeRead | CFile::typeBinary) )
	{
		CString sErrMsg =_T("");
		sErrMsg.Format( _T("%s ������ ã���� �����ϴ�."), FILE_PATH );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	TCHAR cUni;
	File.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		File.SeekToBegin();
	}

	CString sType = _T("");
	GetDlgItemText(IDC_TypeInfo2, sType);

	CString sLine = _T("");
	CString sBuf  = _T("");

	while ( File.ReadString( sLine ) )
	{
		if ( sLine.Find( sType ) != -1 )
			sLine = sType + _T(",") + sCurStatus;
		
		sLine += _T("\n");
		sBuf += sLine;
	}
		
	File.Close();
	
	if ( !File.Open(FILE_PATH, CFile::modeWrite) )
	{
		CString sErrMsg =_T("");
		sErrMsg.Format( _T("%s ������ ã���� �����ϴ�."), FILE_PATH );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	File.WriteString(sBuf);
	File.Close();

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// ȭ�鿡 ������ CHK�� ���¸� ��ȯ�Ѵ�.
//////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Contents_Editor::GetCurContents(CString *sResult)
{
	//////////////////////////////////////////////////////////////////////////////////
	// 1. üũ�ڽ��� ������¸� ���Ѵ�.
	//////////////////////////////////////////////////////////////////////////////////
	INT nChk1 = 0, nChk2 = 0, nChk3 = 0, nChk4 = 0, nChk5 = 0;
	CButton* pBT;
	pBT = (CButton*)GetDlgItem(IDC_chkLibName);
	nChk1 = pBT->GetCheck();
	
	pBT = (CButton*)GetDlgItem(IDC_chkReciever);
	nChk2 = pBT->GetCheck();

	pBT = (CButton*)GetDlgItem(IDC_chkProgram);
	nChk3 = pBT->GetCheck();

	pBT = (CButton*)GetDlgItem(IDC_chkOpen);
	nChk4 = pBT->GetCheck();

	pBT = (CButton*)GetDlgItem(IDC_chkDefaultMsg);
	nChk5 = pBT->GetCheck();

	//////////////////////////////////////////////////////////////////////////////////
	// 2. ��Ʈ������ ��ȯ�Ѵ�.
	//////////////////////////////////////////////////////////////////////////////////
	sResult->Format(_T("%d%d%d%d%d"), nChk1, nChk2, nChk3, nChk4, nChk5);
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// ���� ����Ʈ ǥ��
//////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Contents_Editor::SetContentByte()
{
/*	CString sTemp = _T("");
	GetDlgItemText( IDC_eSMS_CONTENTS, sTemp);
	INT nByte = sTemp.GetLength();
	CString sTitle = _T("");
	sTitle.Format( _T("���� ���� ( %d Byte )"), nByte );
	SetDlgItemText(	IDC_sSMS_CONTENTS , sTitle );
*/
	INT nCurPayByte = 0;
	INT nCurMAXByte = 80;

/*	  1. URL ũ�⸦ ���Ѵ�.( MAX �� ���� )
	  2. �⺻ ���� ����Ʈ�� ���Ѵ�.
	  3. ���� ������ ���Ѵ�.	  
      4. ���� BYTE�� ǥ���Ѵ�. 
*/
	
	// 1. URL ũ�⸦ ���Ѵ�.( MAX �� ���� )
	CString sSMSType;
	GetDlgItemText( IDC_COMBO1 ,sSMSType );
	sSMSType.TrimLeft();sSMSType.TrimRight();
	if ( sSMSType.Replace( _T("URL") , _T("") )  > 0 )
	{
		CEdit* pEdtURL = ( CEdit* )GetDlgItem( IDC_eURL );
		if ( !pEdtURL ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CSMS_LifeLong_Contents_Editor::SetContentByte()") );	
			
		// ������ ���� �߰��� 1����Ʈ�� �� ������.
		if ( pEdtURL->GetWindowTextLength() > 0 )
		{
			nCurMAXByte -= ( pEdtURL->GetWindowTextLength() + 1 );	
		}			 
	}
	
	// 2. �⺻ ���� ����Ʈ�� ���Ѵ�.
	//    �⺻���ڰ� ����Ǿ���Ѵ�.
/*	if ( GetDlgItem( IDC_cSMS_TYPE )->GetWindowTextLength() > 0 )
	{
		nCurPayByte += GetDlgItem( IDC_cSMS_TYPE )->GetWindowTextLength() + 2;
	}
*/
	// 3. ���� ������ ���Ѵ�.
	// able �Ǿ� �ִ� ���� && üũ�Ǿ� �ִ� ������ ����Ѵ�.
	const INT nEleCnt = 5;
	CButton* pBtn     = NULL;
	                     // ��������       �̿��ڸ�         ���¸�          ��¥����     �⺻�޼��� 
	INT nID[ nEleCnt ] = { IDC_chkLibName, IDC_chkReciever, IDC_chkProgram, IDC_chkOpen, IDC_chkDefaultMsg };
	for ( INT i = 0;i < nEleCnt;i++ )
	{		
		pBtn = (CButton*)GetDlgItem( nID[ i ] );

		if ( pBtn->IsWindowEnabled() && pBtn->GetCheck() )
		{
			//    0 : ��������, 1 : �����ڸ�, 2 : ���¸�, 3 : ������, 4 :�⺻�޼���
			switch( i )
			{
				case 0:		// ��������
					{						
						// ���� 2����Ʈ �߰� 
						nCurPayByte += GetDlgItem( IDC_edtLibName )->GetWindowTextLength() + 2;
					}
					break;
				case 1:		// �����ڸ�
					{
						// ���� ���� ������� ����
					}
					break;
				case 2:		// ���¸�
					{
						// ���� ���� ������� ����
					}
					break;
				case 3:		// ��¥����
					{
						nCurPayByte += 18; // ������ �����Ǿ� �ִ�. ',������:2005/12/12'
					}
					break;
				case 4:		// �⺻�޼��� 
					{
						nCurPayByte += GetDlgItem(IDC_eSMS_CONTENTS)->GetWindowTextLength();
					}
					break;
			}
		}
	}

	// 4. ���� ����Ʈ�� ǥ���Ѵ�.
	CString sBYTE;
	sBYTE.Format( _T("%d / %d"), nCurPayByte, nCurMAXByte);
	SetDlgItemText(	IDC_sSMS_CONTENTS , sBYTE );

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// �⺻ �޼����� ���Ѵ�.
//////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Contents_Editor::GetDefaultMsg(CString *sGetMsg)
{
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_SMSGroup") );
	if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Contents_Editor::GetDefaultMsg") );
	
	INT nCnt = pDM->GetRowCount();
	if (nCnt < 1) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Contents_Editor::GetDefaultMsg") );

	CString sTemp = _T("");
	for (INT i = 0;i < nCnt;i++)
	{
		pDM->GetCellData( _T("Ÿ��"), i, sTemp );
		if ( _tcscmp(sTemp, m_sSMSType) == 0 )
		{
			pDM->GetCellData( _T("�⺻�޼���"), i, *sGetMsg );
			break;
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// CFG�� ���� �����Ѵ�.
//////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Contents_Editor::GetCFGContents(CString* psResult)
{
	CString sErrMsg = _T("");
	CStdioFile File;
	if ( !File.Open(FILE_PATH, CFile::modeRead | CFile::typeBinary) )
	{
		sErrMsg.Format( _T("%s ������ ã���� �����ϴ�."), FILE_PATH );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	TCHAR cUni;
	File.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		File.SeekToBegin();
	}

	CString sTemp = _T("");

	while ( File.ReadString( sTemp ) )
	{
		if ( _tcscmp( sTemp.Mid(0,1) , _T("#") ) == 0 )	continue;
		if ( sTemp.Replace( m_sSMSType, _T("") ) > 0 )		break;
	}

	File.Close();

	sTemp.Replace( _T(","), _T("") );

	if ( sTemp.GetLength() != 5 )
	{
		sErrMsg.Format( _T("������������ �߸��Ǿ� �ֽ��ϴ�.") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	*psResult = sTemp;

	return 0;
}

VOID CSMS_LifeLong_Contents_Editor::OnCancel() 
{
	// TODO: Add extra cleanup here
//	this->DestroyWindow();	
	CDialog::OnCancel();
}

VOID CSMS_LifeLong_Contents_Editor::OnChangeeSMSCONTENTS() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	SetContentByte();
}

VOID CSMS_LifeLong_Contents_Editor::OnchkLibName() 
{
	SetContentByte();
}

VOID CSMS_LifeLong_Contents_Editor::OnchkReciever() 
{
	SetContentByte();
}

VOID CSMS_LifeLong_Contents_Editor::OnchkProgram() 
{
	SetContentByte();
}

VOID CSMS_LifeLong_Contents_Editor::OnchkOpen() 
{
	SetContentByte();
}

VOID CSMS_LifeLong_Contents_Editor::OnchkDefaultMsg() 
{
	SetContentByte();
}

VOID CSMS_LifeLong_Contents_Editor::OnSelchangeCombo1() 
{
	SetContentByte();
}

VOID CSMS_LifeLong_Contents_Editor::OnChangeeURL() 
{
	SetContentByte();
}
