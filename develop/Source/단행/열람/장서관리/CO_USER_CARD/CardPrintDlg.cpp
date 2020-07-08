// CardPrintDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CardPrintDlg.h"
#include "PreViewCardPrint.h"
#include "direct.h"
#include "FrameManager.h"
#include "..\\BL_LOC_USER\\KL_UserManagerDlg.h"
#include "StdioFileEx.h"
// 2011.09.08 ADD : 3000Api
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCardPrintDlg dialog


CCardPrintDlg::CCardPrintDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_bIsKLReady = FALSE;
}
CCardPrintDlg::~CCardPrintDlg()
{
}

void CCardPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCardPrintDlg, CDialog)
	//{{AFX_MSG_MAP(CCardPrintDlg)
	ON_BN_CLICKED(IDC_btnUserCardPrint, OnbtnUserCardPrint)
	ON_BN_CLICKED(IDC_btn1, Onbtn1)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_cmbCardFormat, OnSelchangecmbFORMAT)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER+1030, OnRFIDServiceMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCardPrintDlg message handlers

BOOL CCardPrintDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd());

	if(0>InitESL_Mgr(_T("SM_CARD_PRINT_DLG")))
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCardPrintDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if( bShow )
	{
		CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_cmbCardFormat);	
		pCmb->SetCurSel(GetFormat());

		LoadRFIDService();

	}
}

BOOL CCardPrintDlg::PreTranslateMessage(MSG* pMsg) 
{
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

VOID CCardPrintDlg::Init()
{
	// �����ڹ�ȣ�� �ֹι�ȣ�� ��� Show/Hide ���� �ɼ�
	INT ids = GetManageValue( _T("����"), _T("����"), _T("�̿��ڰ���"), _T("����ȭ���ֹι�ȣǥ������"), m_strIsChangeSecuriy);
	if(0>ids) return;
	
	InitFormatList();

	CESL_DataMgr* pUserDM = FindDM(_T("DM_ȸ����_�̿�������"));
	pUserDM->FreeData();

	CString sWhere;
	sWhere.Format( _T(" REC_KEY = %s ")  , m_strUserKey );
	pUserDM->RefreshDataManager( sWhere );

	// 2011.09.08 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pUserDM->GetRowCount())
	{
		CStringArray asDes1;
		CString strGetData;
		for(INT i = 0; i < pUserDM->GetRowCount(); i++)
		{
			pUserDM->GetCellData(_T("�ֹε�Ϲ�ȣ"), i, strGetData);
			asDes1.Add(strGetData);
		}
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
		if(NULL != p3000Api)
		{
			if(TRUE == p3000Api->ConnectSocket()) 
			{
				p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes1);
				for(i = 0; i < pUserDM->GetRowCount(); i++)
				{
					pUserDM->SetCellData(_T("�ֹε�Ϲ�ȣ"), asDes1.GetAt(i), i);
				}
				p3000Api->CloseSocket();
			}
			delete p3000Api;
			p3000Api = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
	}

	if(_T("Y") == m_sIsUnityLoanService)
	{
		GetDlgItem(IDC_btn1)->ShowWindow(SW_SHOW);
		// 2012.10.29 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 4ȸ
		GetDlgItem(IDC_chkKLICON)->ShowWindow(SW_SHOW);
	}
}

void CCardPrintDlg::OnbtnUserCardPrint() 
{
	PrintCard();
}

void CCardPrintDlg::Onbtn1() 
{
	if(_T("Y") != m_sIsUnityLoanService)
	{
		m_bIsKLReady = FALSE;
		return;
	}

	CESL_DataMgr* pUserCardDM = FindDM(_T("DM_ȸ����_�̿�������"));
	CString strKLMemberYN = pUserCardDM->GetCellData(_T("����ȸ����ȯ����"), 0);
	if(_T("Y") != strKLMemberYN)
	{
		AfxMessageBox(_T("���õ� �̿��ڴ� å����ȸ���� �ƴմϴ�."));
		m_bIsKLReady = FALSE;
		return;
	}
	m_bIsKLReady = TRUE;
}

void CCardPrintDlg::OnbtnCLOSE() 
{
	OnCancel();	
}

void CCardPrintDlg::OnCancel()
{
	m_bIsKLReady = FALSE;
	UnLoadRFIDService();
	CDialog::OnCancel();
}

void CCardPrintDlg::OnSelchangecmbFORMAT() 
{
	SetCheckBackPagePrint();	
}

INT CCardPrintDlg::GetFormat()
{
	CString sFileName = _T("..\\cfg\\UserCardFormat.cfg");
	CFileFind ff;
	if(!ff.FindFile(sFileName))
	{ // ���� ���ٸ� 0
		return 0;
	}
	else
	{ // �����ִٸ� ���Ͽ� ������ ������ ����
		CString sValue;
		CStdioFile fp;
		if(!fp.Open(sFileName, CFile::modeRead | CFile::typeBinary))
		{
			return 0;
		}
		TCHAR cUni;
		fp.Read(&cUni, sizeof(TCHAR));
		if(0xFEFF != cUni)
		{
			fp.SeekToBegin();
		}
		
		fp.ReadString(sValue);
		fp.Close();
		sValue.TrimLeft();	sValue.TrimRight();
		
		return _ttoi(sValue);
	}

	return 0;
}

INT CCardPrintDlg::SetFormat(INT nIdx)
{
	CString sFileName = _T("..\\cfg\\UserCardFormat.cfg");
	FILE *fp = _tfopen(sFileName, _T("w+b"));
	fputc(0xFF, fp);
	fputc(0xFE, fp);
	_ftprintf( fp, _T("%d"), nIdx);
	fclose( fp );

	return 0;
}

INT CCardPrintDlg::SetCheckBackPagePrint()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbCardFormat);
	INT nIndex = pCombo->GetCurSel();

	if( nIndex == 0 )
		return 0;
	
	CESL_DataMgr* pFormatDM = FindDM( _T("DM_ȸ�����������") );
	
	CString sData;
	pFormatDM->GetCellData( _T("�ո鿩��") , nIndex-1 , sData );
	
	if( sData.Compare(_T("1")) == 0 )
	{
		((CButton*)GetDlgItem(IDC_chkTOGETHER))->SetCheck(0);
		GetDlgItem(IDC_chkTOGETHER)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_chkTOGETHER)->EnableWindow(TRUE);
	}

	// 2009.10.15 ADD BY PWR : ������ ������ ������� ����
	SetFormat(nIndex);

	return 0;
}

INT CCardPrintDlg::InitFormatList()
{

	INT ids;
	
	// ================================================================================
	// 1. ��Ƽ�޺��ڽ� �ʱ�ȭ
	// ================================================================================
	CESL_ControlMgr* pCM = FindCM( _T("CM_CARD_PRINT_DLG") );
	
	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("��ĸ���Ʈ")));
	if( NULL==pCMCB ) ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("InitFormatList") );
	pCMCB->InitMultiComboBox(2);
	
	CString sInsertCode[2] = { _T("�������                                    "), _T("") };
	ids = pCMCB->SetData( sInsertCode , 2 );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("InitFormatList") );
	
	// ================================================================================
	// 2. ȸ����DM  �����͸� ��Ƽ�޺��� �Է� 
	// ================================================================================
	CESL_DataMgr* pFormatDM = FindDM( _T("DM_ȸ�����������") );
	pFormatDM->RefreshDataManager(_T(""));

	for( INT i = 0 ; i < pFormatDM->GetRowCount() ; i++ )
	{
		pFormatDM->GetCellData( _T("����̸�") , i , sInsertCode[0] );
		pFormatDM->GetCellData( _T("�ո鿩��") , i , sInsertCode[1] );
		
		if( sInsertCode[1].Compare(_T("0")) == 0 )
			sInsertCode[1] = _T("�ո�");
		else
			sInsertCode[1] = _T("�޸�");

		ids = pCMCB->SetData( sInsertCode , 2 );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("InitFormatList") );
	}
	
	// ================================================================================
	// 3. ������ 
	// ================================================================================
	if(1 < pCMCB->GetRowCount())
	{
 		pCMCB->SetCurSel(GetFormat());
	}
 	else
	{
 		pCMCB->SetCurSel(0);
	}
	
	SetCheckBackPagePrint();

	return 0;
}

INT CCardPrintDlg::LoadRFIDService()
{
 	if(_T("Y") != m_sIsUnityLoanService)
	{
	 	return 1000;
	}

	CStdioFileEx File;
	if(!File.Open(_T("..\\cfg\\KL\\KLRFID"), CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode))
	{
		return -1000;
	}

	CString strData;
	strData.Format(_T("%d"), GetSafeHwnd());
	File.WriteString(strData);
	File.Close();

	// �ӽ�
// 	return 0;

	CString strCmd = _T("MLoader.exe KLRFIDService.exe");
	TCHAR lpCurrentDirectory[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, lpCurrentDirectory);

	PROCESS_INFORMATION pinfo;
	STARTUPINFO Startupinfo1;
	ZeroMemory(&Startupinfo1, sizeof(Startupinfo1));
	Startupinfo1.cb = sizeof(STARTUPINFO);	
   	Startupinfo1.dwFlags = STARTF_USESHOWWINDOW;
	Startupinfo1.wShowWindow = SW_SHOW;
	INT result = ::CreateProcess( NULL, strCmd.GetBuffer(0), NULL, NULL, FALSE, 0, NULL, lpCurrentDirectory, &Startupinfo1, &pinfo);
	strCmd.ReleaseBuffer();

	// �̴����� �ڽ� ���μ����� ����Ǹ� ó���� ������ �ڵ�
	CloseHandle( pinfo.hProcess );
	CloseHandle( pinfo.hThread );

	return 0;
}

INT CCardPrintDlg::UnLoadRFIDService()
{
 	if(_T("Y") != m_sIsUnityLoanService)
	{
	 	return 1000;
	}

	CStdioFileEx File;
	if(File.Open(_T("..\\cfg\\KL\\KLRFID"), CFile::modeRead|CFile::typeText))
	{
		CString strData1, strData2;	
		File.ReadString(strData1);
		File.Close();
		
		strData2.Format(_T("%d"), GetSafeHwnd());
		if(strData1 != strData2)
		{
			return 1000;
		}
	}
	
	HWND hWnd = ::FindWindow(NULL, _T("KLRFIDService"));
	if(hWnd)
	{
		::PostMessage(hWnd, WM_QUIT, NULL, NULL);
	}

	return 0;
}

INT CCardPrintDlg::IsRegsisterUID(CString& strUserNo, CString& strUID, BOOL& bRegister)
{ // 10200
	bRegister = FALSE;
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));

	CString strQuery, strResult;
	if(strUserNo.IsEmpty())
	{
		strQuery.Format(
			_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL WHERE WORKNO = '%s' OR PB_WORKNO = '%s'"),
			strUID, strUID);
	}
	else
	{
		strQuery.Format(
			_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL WHERE USER_NO = '%s' OR WORKNO = '%s' OR PB_WORKNO = '%s'"),
			strUserNo, strUID, strUID);
	}
	TmpDM.GetOneValueQuery(strQuery, strResult);

	if(strResult.IsEmpty())
	{
		return -10200;
	}
	if(_ttoi(strResult) > 0)
	{
		bRegister = TRUE;
	}

	return 0;
}

INT CCardPrintDlg::PrintCard()
{
	INT ids;

	// �޸� ��� ����
	BOOL bPrintBackPage;

	if( ((CButton*)GetDlgItem(IDC_chkTOGETHER))->GetCheck() == 1 )
		bPrintBackPage = TRUE;
	else
		bPrintBackPage = FALSE;

	CESL_ControlMultiComboBox* pCombo = (CESL_ControlMultiComboBox*)GetDlgItem(IDC_cmbCardFormat);
	INT nFormatIndex = pCombo->GetCurSel();	
	if( 0 == nFormatIndex )
	{
		AfxMessageBox(_T("����� �������ֽʽÿ�"));
		return 0;
	}

	// �޸��� ����Ҽ� �ִ��� �˾ƺ���.
	CESL_DataMgr* pFormatDM = FindDM( _T("DM_ȸ�����������") );
	CString sFrontFormKey , sBackFormKey;
	CString sFormName , sFormName2;

	pFormatDM->GetCellData( _T("����̸�") , nFormatIndex-1 , sFormName );
	pFormatDM->GetCellData( _T("������KEY") , nFormatIndex-1 , sFrontFormKey );

	if( bPrintBackPage )
	{
		
		pFormatDM->GetCellData( _T("����̸�") , nFormatIndex , sFormName2 );
		pFormatDM->GetCellData( _T("������KEY") , nFormatIndex , sBackFormKey );
		
		if( sFormName.Compare(sFormName2) != 0 )
		{
			AfxMessageBox(_T("�޸� ����� �������� �ʾ� �޸��� ����Ҽ� �����ϴ�.") );
			return 0;
		}
	}
	
	// �����Ѵ�.
	CESL_DataMgr* pUserDM = FindDM(_T("DM_ȸ����_�̿�������"));

	const INT nCnt = 2;
	CString sAlias[nCnt] = {_T("�ֹε�Ϲ�ȣ") , _T("����")};

	CString sResult[nCnt];
	/// ���� �����ޱ�
	pUserDM->GetCellData( sAlias[1] , 0 , sResult[1] );

	CString sFileName;
	if( !sResult[1].IsEmpty() )
	{
		_tmkdir(_T("../������"));
		_tmkdir(_T("../������/�̿��ڻ���"));
		
		CString sFileName;
		sFileName.Format(_T("../������/�̿��ڻ���/%s.%s"),m_strUserKey,sResult[1]);
		// 2. �̿��� ������ DB���� ��������
		BYTE *pBuffer = (BYTE *)malloc(MAX_BLOB_BUFFER);
		
		INT nSize;
		ids = pUserDM->LOBGet( _T("CO_LOAN_USER_TBL") , _T("REC_KEY") , _T("NUMERIC") , m_strUserKey , _T("USER_PICTURE") , pBuffer , &nSize );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("PrintCard") );
		
		CFile fd;
		if (fd.Open( sFileName , CFile::modeCreate|CFile::modeWrite | CFile::typeBinary) == NULL)
		{
			AfxMessageBox(_T("����/�ݳ� Tmp File Open Error~"));
			free( pBuffer );
			return 1;
		}

		fd.WriteHuge(pBuffer, nSize);
		free( pBuffer );
		
		fd.Close();

		pUserDM->SetCellData( sAlias[1] , sFileName , 0 );
	}

	/// �ֹε�Ϲ�ȣ �Է�
	pUserDM->GetCellData( sAlias[0] , 0 , sResult[0] );
	if( !sResult[0].IsEmpty() )
	{
		CLocCommonAPI::ChangeSecuriyCivilNo( this , sResult[0] , m_sIsUnityLoanService );
		pUserDM->SetCellData( sAlias[0] , sResult[0], 0  );
	}
	
	pUserDM->SetCellData( _T("�������KEY") , sFrontFormKey , 0 );

	CString strKLMemberYN;
	if(_T("Y")==m_sIsUnityLoanService)
	{
		pUserDM->GetCellData( _T("����ȸ����ȯ����") , 0 , strKLMemberYN );
		strKLMemberYN.TrimLeft();	strKLMemberYN.TrimRight();
		strKLMemberYN.MakeUpper();
	}
	
	if( bPrintBackPage )
	{
		/// �ֹε�Ϲ�ȣ �Է�
		pUserDM->GetCellData( sAlias[0] , 0 , sResult[0] );
		
		if( !sResult[0].IsEmpty() )
		{
			CLocCommonAPI::ChangeSecuriyCivilNo( this , sResult[0] , m_sIsUnityLoanService );
			pUserDM->SetCellData( sAlias[0] , sResult[0], 0  );
		}
		
		pUserDM->SetCellData( _T("�������KEY") , sBackFormKey , 0 );
		pUserDM->SetCellData( sAlias[1] , sFileName , 0 );
	}

	if(_T("Y")==m_sIsUnityLoanService)
	{
		if(_T("Y")==strKLMemberYN)
		{/// ����ȸ���� ȸ���� ��½� ����� '����ȸ����'�� ���õǾ� �־�� �Ѵ�.
			CString strCode;
			ids = pCombo->GetData(strCode, nFormatIndex, 0);
			if(_T("����ȸ����")!=strCode)
			{
				if(IDYES != AfxMessageBox(_T("����ȸ���Դϴ�. ���� ������� ����Ͻðڽ��ϱ�?"),MB_YESNO))
				{
					return 1020;
				}
			}
		}
		else
		{/// �Ϲ�ȸ���� ȸ���� ��½� ����� '����ȸ����'�� �ƴ� ����� ���õǾ� �־�� �Ѵ�.
// 			CString strCode;
// 			ids = pCombo->GetData(strCode, nFormatIndex, 0);
// 			if(_T("����ȸ����")==strCode)
// 			{
// 				AfxMessageBox(	_T("�Ϲ�ȸ���� ȸ���� ��½� ����� '����ȸ����'�� �ƴ� ����� ���� �Ǿ� �־�� �մϴ�.\r\n")
// 								_T("��� ����� Ȯ�����ֽʽÿ�."));
// 				return 1030;
// 			}
		}
	}

	CPreViewCardPrint Dlg(this);
	// 2012.10.29 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 4ȸ
	if(1 == ((CButton*)GetDlgItem(IDC_chkKLICON))->GetCheck())
	{
		Dlg.m_bShowKLIconYN = TRUE;		
	}
	else
	{
		Dlg.m_bShowKLIconYN = FALSE;
	}
	Dlg.SetPrintList( pUserDM );
	Dlg.DoModal();

	// 2011.01.10 DELETE BY KSJ : ����ȸ���� �߱� �̷� ���� �̿��ڼ���â���� �ϵ��� ����
//DEL 	if(Dlg.m_bIsPrint)
//DEL 	{
//DEL 		if(_T("Y") == m_sIsUnityLoanService)
//DEL 		{
//DEL 			// ���� ȸ�� ���� �ý��� - ����ȸ���� �߱� �̷� ����	
//DEL 			CString strKLMemberYN, strCardIssueYN;
//DEL 			INT nRowCnt = pUserDM->GetRowCount();
//DEL 
//DEL 			for(INT nRow = 0; nRow < nRowCnt; nRow++)
//DEL 			{
//DEL 				strKLMemberYN = pUserDM->GetCellData(_T("����ȸ����ȯ����"), nRow);
//DEL 				strKLMemberYN.TrimLeft();	strKLMemberYN.TrimRight();
//DEL 
//DEL 				strCardIssueYN = pUserDM->GetCellData(_T("ȸ������¿���"), nRow);
//DEL 				strCardIssueYN.TrimLeft();	strCardIssueYN.TrimRight();
//DEL 
//DEL 				if(_T("Y")==strKLMemberYN && _T("Y")==strCardIssueYN)
//DEL 				{
//DEL 					CKL_UserManagerDlg dlg(this);
//DEL 					dlg.SetMode(6);
//DEL 					dlg.SetUserNo(pUserDM->GetCellData(_T("�̿��ڹ�ȣ(TEXT)"), nRow));
//DEL 					// 2011.01.10 UPDATE BY KSJ : �̵���� ����� �� ���� ��� ó��
//DEL 					//DEL dlg.DoModal();
//DEL 					dlg.KL_Membershipcard();
//DEL 				}
//DEL 			}
//DEL 		}
//DEL 	}

	return 0;
}

LRESULT CCardPrintDlg::OnRFIDServiceMessage(WPARAM wParam, LPARAM lParam)
{
	INT ids = 0;
	CStdioFileEx File;
	CString strLine, strCommand, strData1, strData2, strUserNo;

	if(TRUE == wParam)
	{
		strLine = *(CString*)lParam;
	}
	else
	{
		if(!File.Open(_T("..\\cfg\\KL\\KLRFIDS"), CFile::modeRead|CFile::typeText))
		{
			return -10000;
		}		
		File.ReadString(strLine);
		File.Close();
	}
	AfxExtractSubString(strCommand, strLine, 0, '|');
	AfxExtractSubString(strData1, strLine, 1, '|');
	AfxExtractSubString(strData2, strLine, 2, '|');

	if(!m_bIsKLReady) ids = -10005;

	CESL_DataMgr* pDM = FindDM(_T("DM_ȸ����_�̿�������"));
	if(NULL == pDM) ids = -10003;

	pDM->GetCellData( _T("�̿��ڹ�ȣ(TEXT)") , 0 , strUserNo);

	if(!File.Open(_T("..\\cfg\\KL\\KLRFIDR"), CFile::modeWrite|CFile::modeCreate|CStdioFileEx::modeWriteUnicode))
	{
		ids = -10004;
	}

	if(_T("KL_MEMBER_CARD_OPEN") == strCommand)
	{ // 10100
		if(m_bIsKLReady && ids >= 0 && !m_strUserKey.IsEmpty() && !strUserNo.IsEmpty())
		{
			File.WriteString(_T("KL_MEMBER_CARD_OPEN|����"));
		}
		else
		{
			if(0 >= ids) ids = -10100;
			File.WriteString(_T("KL_MEMBER_CARD_OPEN|����"));
		}
		File.Close();
		return ids;
	}
	else if(_T("KL_MEMBER_STATUS") == strCommand)
	{ // 10200
		BOOL bIsRegister = FALSE;
		ids = IsRegsisterUID(strData1, strData2, bIsRegister);
		if(bIsRegister)
		{
			File.WriteString(_T("KL_MEMBER_STATUS|TRUE"));
		}
		else
		{
			File.WriteString(_T("KL_MEMBER_STATUS|FALSE"));
		}
		File.Close();
		return ids;
	}
	else if(_T("KL_LIBRARY") == strCommand)
	{ // 10300
		File.WriteString(_T("KL_LIBRARY|") + m_pInfo->LIB_CODE);
		File.Close();
		return ids;
	}
	else if(_T("KL_MEMBER_NO") == strCommand)
	{ // 10400
		File.WriteString(_T("KL_MEMBER_NO|") + strUserNo);
		File.Close();
		return ids;
	}
	else if(_T("KL_MEMBER_WRTIE_RFID_UID") == strCommand)
	{ // 10500
		if(!strData1.IsEmpty())
		{
			if(strData1 != strUserNo)
			{
				File.WriteString(_T("KL_MEMBER_WRTIE_RFID_UID|����"));
				ids = -10500;
			}
		}

		ids = HandphoneRFIDInput(m_strUserKey, strData2);
		if(0 == ids)
		{
			File.WriteString(_T("KL_MEMBER_WRTIE_RFID_UID|����"));
		}
		else
		{
			File.WriteString(_T("KL_MEMBER_WRTIE_RFID_UID|����"));
		}
		File.Close();
		return ids;
	}
	else if(_T("KL_MEMBER_CARD_CLOSE") == strCommand)
	{ // 10700
		File.WriteString(_T("KL_MEMBER_CARD_CLOSE|����"));

		m_bIsKLReady = FALSE;
		File.Close();
		return ids;
	}		
	File.Close();

	return ids;
}

INT CCardPrintDlg::HandphoneRFIDInput(CString& strUserKey, CString& strUID)
{
	CString strQuery;
	CESL_DataMgr* pDM = FindDM(_T("DM_ȸ����_�̿�������"));

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));
	TmpDM.StartFrame();
	strQuery.Format(_T("UPDATE BO_BOOK_TBL SET WORKNO = NULL WHERE WORKNO = '%s';"), strUID);
	TmpDM.AddFrame(strQuery);
	strQuery.Format(_T("UPDATE SE_BOOK_TBL SET WORKNO = NULL WHERE WORKNO = '%s';"), strUID);
	TmpDM.AddFrame(strQuery);
	strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET WORKNO = NULL WHERE WORKNO = '%s';"), strUID);
	TmpDM.AddFrame(strQuery);
	strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET PB_WORKNO = NULL WHERE PB_WORKNO = '%s';"), strUID);
	TmpDM.AddFrame(strQuery);
	strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET WORKNO = '%s' WHERE REC_KEY = %s;"), strUID, m_strUserKey);
	TmpDM.AddFrame(strQuery);
	INT ids = TmpDM.SendFrame();
	TmpDM.FreeFrame();
	TmpDM.EndFrame();

	if(0 == ids)
	{
		CString strUserNo = pDM->GetCellData(_T("�̿��ڹ�ȣ(TEXT)") , 0);

		CFrameWriter Writer(m_pInfo);
		Writer.setCommand(_T("IL_K24_LOC_B01_SERVICE"));
		Writer.addRecord();
		Writer.addElement(_T("���մ����ڹ�ȣ"), strUserNo);
		Writer.addElement(_T("ȸ����RFID"), strUID);

		// 2011.02.02 ADD BY KSJ : �������� �߰�
		CString strModifyDate;
		// 2011.02.15 ADD BY KSJ : �Լ�����
		//DEL strModifyDate.Format(_T("%d/%d/%d %02d:%02d:%02d"), tCurrentTime.GetYear(), tCurrentTime.GetMonth(), tCurrentTime.GetDay() , tCurrentTime.GetHour(), tCurrentTime.GetMinute(), tCurrentTime.GetSecond());
		CTime t = CLocCommonAPI::GetCurrentDateTime(this, strModifyDate);
		Writer.addElement(_T("��������"), strModifyDate);
		Writer.addElement(_T("������������ȣ"), m_pInfo->LIB_CODE);

		CFrameSender Sender(m_pInfo);
		CFrameReader Reader;
		Sender.SendTempTbl(&Writer, GetWorkLogMsg( _T("����RFID���"), __WFILE__, __LINE__ ));
	}

	return ids;
}
