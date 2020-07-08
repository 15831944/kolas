// UnionReturnDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UnionReturnDlg.h"
#include "UnionUserInfoViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUnionReturnDlg dialog


CUnionReturnDlg::CUnionReturnDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUnionReturnDlg)
	//}}AFX_DATA_INIT
	m_pmcbReturnLibrary = NULL;

	m_nReturnCode = 0;
	m_bStatus = FALSE;

	EnableThemeDialogTexture(GetSafeHwnd()); 
}

CUnionReturnDlg::~CUnionReturnDlg()
{
	///{{++
	/// 2008.05.02 add by pdw
	/// ����ݳ�ȭ���� ����ȭ ��Ű�� ���ؼ� �޽��� ȣ��
	pParentMgr->SendMessage(WM_USER+107);
	///}}--

	if( m_pmcbReturnLibrary )
	{
		delete m_pmcbReturnLibrary;
		m_pmcbReturnLibrary = NULL;
	}

}

void CUnionReturnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUnionReturnDlg)
	DDX_Control(pDX, IDC_EDT_UNION_RETURN_REGNO_USERINPUT, m_edtInputRegNo);
	DDX_Control(pDX, IDC_BTN_UNION_SEARCH, m_btnSearch);
	DDX_Control(pDX, IDC_BTN_UNION_SELECT, m_btnSelect);
	DDX_Control(pDX, IDC_EDT_UNION_RETURN_LIBREGNO, m_edtLibRegNo);
	DDX_Control(pDX, IDC_EDT_UNION_RETURN_REGNO, m_edtRegNo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUnionReturnDlg, CDialog)
	//{{AFX_MSG_MAP(CUnionReturnDlg)
	ON_BN_CLICKED(IDC_BTN_UNION_RETURN_CLOSE, OnBtnUnionReturnClose)
	ON_BN_CLICKED(IDC_BTN_UNION_SEARCH, OnBtnUnionSearch)
	ON_BN_CLICKED(IDC_BTN_UNION_RETURN_HISTORY, OnBtnUnionReturnHistory)
	ON_BN_CLICKED(IDC_BTN_UNION_RETURN_INPUT, OnBtnUnionReturnInput)
	ON_BN_CLICKED(IDC_BTN_UNION_RETURN_CLEAR, OnBtnUnionReturnClear)
	ON_BN_CLICKED(IDC_BTN_UNION_SELECT, OnBtnUnionSelect)
	ON_BN_CLICKED(IDC_RADIO_RETURN_CANCEL, OnRadioReturnCancel)
	ON_BN_CLICKED(IDC_RADIO_RETURN, OnRadioReturn)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUnionReturnDlg message handlers

BOOL CUnionReturnDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam )
	{		
		HWND MsgH = pMsg->hwnd;
		if( GetDlgItem(IDC_EDT_UNION_RETURN_REGNO)->m_hWnd==MsgH )
		{
			if( ((CButton*)GetDlgItem(IDC_RADIO_RETURN))->GetCheck() == 1)
				SearchUnionLoanHistory();
			else
				SearchUnionReturnHistory();
		}
		else if( GetDlgItem(IDC_EDT_UNION_RETURN_LIBREGNO)->m_hWnd == MsgH )
		{
			SelectUnionLoanHistory();
		}
		return 0;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CUnionReturnDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	if( 0 > InitESL_Mgr(_T("UnionReturnDlg")) )
	{
		AfxMessageBox(_T("InitESL_Mgr ERROR !!"));
		return false;
	}	
	
	// 20071128 ADD BY PWR ++++++++++++++++++++++++++++++++++++++++++
	//��Ϲ�ȣ �Է±��� ����
	m_edtRegNo.SetLimitText(12);
	//��������ȣ �Է±��� ����
	m_edtLibRegNo.SetLimitText(6);
	//�����Է� Ȯ��
	m_bUserInput = FALSE;
	//---------------------------------------------------------------

	///��������� �ʱ�ȭ
	if( 0 > InitReturnLibraryList() )
	{
		AfxMessageBox(_T("InitReturnLibraryList ERROR !!"));
		return false;
	}

	///�ڰ����� ����
	if( 0 > InitRequestLibraryInfo() )
	{
		AfxMessageBox(_T("InitRequestLibraryInfo ERROR !!"));
		return false;
	}
	
	/// �ʱ�ȭ
	if( 0 > Init() )
	{
		AfxMessageBox(_T("Init ERROR !!"));
		return false;
	}
	//�ڵ��ݱ� (default:NON)
	((CButton*)GetDlgItem(IDC_CHK_HOLD))->SetCheck(0);
	//�޽���â�ʱ�ȭ
	SetDlgItemText(IDC_STC_MESSAGE, _T(""));

	CenterWindow(this);
	GetWindowRect(m_rect);

	SetControlShow(TRUE);

	return TRUE;
}

void CUnionReturnDlg::OnBtnUnionReturnClose() 
{	
	OnCancel();
}

void CUnionReturnDlg::OnBtnUnionSearch() 
{
EFS_BEGIN

	//20071221 update by pwr
	if( ((CButton*)GetDlgItem(IDC_RADIO_RETURN_CANCEL))->GetCheck() == 0 )
		SearchUnionLoanHistory();
	else
		SearchUnionReturnHistory();

EFS_END	
}

void CUnionReturnDlg::OnBtnUnionSelect() 
{
EFS_BEGIN

	SelectUnionLoanHistory();

EFS_END
}

void CUnionReturnDlg::OnBtnUnionReturnHistory() 
{
EFS_BEGIN

	//20071221 update by pwr
	if( ((CButton*)GetDlgItem(IDC_RADIO_RETURN_CANCEL))->GetCheck() == 0 )
		ReturnUnionLoanHistory();
	else
		ReturnUnionReturnHistory();

EFS_END
}

void CUnionReturnDlg::OnBtnUnionReturnInput() 
{
EFS_BEGIN

	ReturnUnionUserInput();

EFS_END
}

void CUnionReturnDlg::OnBtnUnionReturnClear() 
{
EFS_BEGIN

	//20071224 update by pwr
	if( ((CButton*)GetDlgItem(IDC_RADIO_RETURN_CANCEL))->GetCheck() == 0 )
		Init();	
	else
		OnRadioReturnCancel();
	
EFS_END	
}


///////////////////////////////////////////////////////////////////////
// ���������� ��Ƽ�޺��ڽ��� �ʱ�ȭ �Ѵ�.
// 1. subclassing
// 2. ���մ�������������� ���������� ��������
// 3. combo�� ���������� �Է�
///////////////////////////////////////////////////////////////////////
INT CUnionReturnDlg::InitReturnLibraryList()
{
EFS_BEGIN
	
	INT		ids;	

	// 1. subclassing
	if( m_pmcbReturnLibrary )
	{
		delete m_pmcbReturnLibrary;
		m_pmcbReturnLibrary = NULL;
	}

	m_pmcbReturnLibrary = new CESL_ControlMultiComboBox(NULL);
	m_pmcbReturnLibrary->SubclassDlgItem(IDC_CMB_UNION_RETURN_LIBLIST, this);

	// 2. ���մ�������������� ���������� ��������	
	CRS_SQL		  RemoteDBMgr(this);
	CESL_DataMgr* pdmReturnLibrary = FindDM( _T("DM_UNION_RETURN_LIBINFO"));

//	ids = RemoteDBMgr.SetFetchCount(0, 100);
	if( 0 > ids ) return ids;
//	ids = RemoteDBMgr.LOAN_Select(pdmReturnLibrary, _T(""), _T("DM_UNION_RETURN_LIBINFO"), 1, 1);
	if( 0 > ids ) return ids;

	// 3. combo�� ���������� �Է�
	const int nDataCount = 2;
	CString	  strReturnLibraryData[nDataCount];

	m_pmcbReturnLibrary->InitMultiComboBox(nDataCount);

	strReturnLibraryData[0] = _T("      ");
	strReturnLibraryData[1] = _T("�������");

	ids = m_pmcbReturnLibrary->SetData(strReturnLibraryData, nDataCount);
	if( 0 > ids ) return ids;

	for( INT i = 0; i < pdmReturnLibrary->GetRowCount(); i++ )
	{		
		ids = pdmReturnLibrary->GetCellData( _T("�������ڵ�"), i, strReturnLibraryData[0] );
		if( 0 > ids ) return ids;
		ids = pdmReturnLibrary->GetCellData( _T("��������"),   i, strReturnLibraryData[1] );
		if( 0 > ids ) return ids;
		
		ids = m_pmcbReturnLibrary->SetData(strReturnLibraryData, nDataCount);
		if( 0 > ids ) return ids;
	}

	m_pmcbReturnLibrary->SetCurSel(0);
	
	return 0;
EFS_END
return -1;
}

///////////////////////////////////////////////////////////////////////
// �ڰ� ������ �ڵ������� �����Ѵ�.
///////////////////////////////////////////////////////////////////////
INT CUnionReturnDlg::InitRequestLibraryInfo()
{
EFS_BEGIN

	INT		  ids;
	const int nFieldCnt = 1;
	CString	  strField[ nFieldCnt ] = { _T("LIB_CODE") };
	CString   strLibCode[ nFieldCnt ];

	ids = CLocCommonAPI::GetLibInfo( strField, strLibCode, nFieldCnt, pParentMgr );
	if( 0 > ids ) return ids;

	m_strLibraryCode = strLibCode[0];

	return 0;

EFS_END
return -1;
}

///////////////////////////////////////////////////////////////////////
// �ʱ�ȭ �Լ�
// 1. ��Ʈ���� �ʱ�ȭ �Ѵ�.
// 2. �ݳ���ư�� ��Ȱ��ȭ �Ѵ�.
///////////////////////////////////////////////////////////////////////
INT CUnionReturnDlg::Init()
{
EFS_BEGIN

	INT		ids;

	// 1. ��Ʈ���� �ʱ�ȭ �Ѵ�.
	SetDlgItemText(IDC_EDT_UNION_RETURN_REGNO, _T(""));
	SetDlgItemText(IDC_EDT_UNION_RETURN_REGNO_USERINPUT, _T(""));
	SetDlgItemText(IDC_EDT_UNION_RETURN_LIBREGNO, _T(""));
	//SetDlgItemText(IDC_STC_MESSAGE, _T(""));

	CESL_DataMgr* pdmLoanHistory = FindDM(_T("DM_UNION_LOAN_HISTORY"));
	pdmLoanHistory->FreeData();

	// 20071220 add by pwr
	CESL_DataMgr* pdmTempReturnInfo = FindDM(_T("DM_UNION_TEMP_RETURN_INFO"));
	pdmTempReturnInfo->FreeData();

	ids = AllControlDisplay(_T("CM_UNION_RETURN_DLG"));

	m_pmcbReturnLibrary->SetCurSel(0);
	// 20071220 add by pwr
	((CButton*)GetDlgItem(IDC_RADIO_RETURN ))->SetCheck(1);

	// 2. �ݳ���ư�� ��Ȱ��ȭ �Ѵ�.
	GetDlgItem(IDC_BTN_UNION_RETURN_HISTORY)->EnableWindow(FALSE);	
	GetDlgItem(IDC_BTN_UNION_RETURN_INPUT)->EnableWindow(FALSE);

	// Ȱ��ȭ
	m_edtRegNo.EnableWindow(TRUE);
	m_btnSearch.EnableWindow(TRUE);
	m_edtLibRegNo.EnableWindow(FALSE);
	m_btnSelect.EnableWindow(FALSE);
	m_edtRegNo.SetFocus();
	
	//�ɹ����� �ʱ�ȭ
	//�����Է� Ȯ��
	m_bUserInput = FALSE;
	//��������ȣ
	m_RegNo = "";

	return 0;

EFS_END
return -1;
}

///////////////////////////////////////////////////////////////////////
// �Էµ� ��Ϲ�ȣ�� 12�ڸ��� �����.
///////////////////////////////////////////////////////////////////////
INT CUnionReturnDlg::MakeValidRegNo(CString &strGetRegNo)
{
EFS_BEGIN

	// 1. �Է� ���� Ȯ��
	if( strGetRegNo.IsEmpty() )
	{
		//AfxMessageBox(_T("�ݳ� ��� �ڷ��� ��Ϲ�ȣ�� �Է��ؾ� �մϴ�"));
		SelfMessageView(_T("�ݳ� ��� �ڷ��� ��Ϲ�ȣ�� �Է��ؾ� �մϴ�"), FALSE);
		return -1001;
	}

	// 12 �ڸ� üũ
	INT		nRegNoLength = strGetRegNo.GetLength();

	//=====================================================
	//2008.11.04 ADD BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	if( 12 < nRegNoLength )
	{
		//AfxMessageBox(_T("��Ϲ�ȣ�� 12�ڸ��Դϴ�"));
		//SelfMessageView(_T("��Ϲ�ȣ�� 12�ڸ��Դϴ�"));
		//CBL_LOC_LOAN_PROC::SelfCloseMessageBox(_T("��Ϲ�ȣ�� 12�ڸ��Դϴ�"));
		strGetRegNo.Empty();
		return -1010;
	}
	else if( 12 > nRegNoLength )
	{
		if(1 == nRegNoLength )	return -1020; //20071221 add by pwr

		//=====================================================
		//2008.11.04 UPDATE BY PJW : ��ϱ��а� ��Ϲ�ȣ�� ���ںκ��� �����Ѵ� -> ��ϱ����ڵ� ���氡���ϵ��� ����
// 		CString strRegCode = strGetRegNo.Mid(0,2);
// 		CString strRegNum  = strGetRegNo.Mid(2);
// 
// 		for( INT i = 0; i < (12-nRegNoLength); i++ )
// 		{
// 			strRegNum = _T("0") + strRegNum;
// 		}
// 
// 		strGetRegNo = strRegCode + strRegNum;

		CString strRegCode = strGetRegNo.Mid(0,nRegCodeLength);
		CString strRegNum  = strGetRegNo.Mid(nRegCodeLength);
		
		strGetRegNo = m_pInfo->MakeRegNo(strRegCode, strRegNum);
		//=====================================================
		
	}
	
	return 0;

EFS_END
return -1;
}

// 20071128 ADD BY PWR ++++++++++++++++++++++++++++++++++++++++++++++++
///////////////////////////////////////////////////////////////////////
// �����̷� �˻���� ����
// 1. ��������ȣ�� �����´�.
// 2. �˻��� �����̷��� �ִ��� ����Ȯ��
// 2.1. �˻��� �����̷������� �Է��� ��������ȣ������ �������ڵ尡 ���翩�� Ȯ��
// 2.2. �ش� �ڷ� ����
// 3. �˻��� �����̷��� ���� <�����Է�>
// 3.1. �޺����� �ش� ���������� ����
// 4. <��������ȣ ���Է� >
// 4.1. ó���Է³���� ���Է³����� ��
// 4.2. �����Է� ����
// 4.3. �ʱ�ȭ   
///////////////////////////////////////////////////////////////////////
INT CUnionReturnDlg::SelectUnionLoanHistory()
{
EFS_BEGIN

	// 1. ��������ȣ�� �����´�.
	CString strLibRegNo;
    GetDlgItemText(IDC_EDT_UNION_RETURN_LIBREGNO, strLibRegNo);

	if( strLibRegNo.IsEmpty() )
	{
		//AfxMessageBox(_T("�ݳ� ��� �ڷ��� ��������ȣ�� �Է��ؾ� �մϴ�"));
		SelfMessageView(_T("�ݳ� ��� �ڷ��� ��������ȣ�� �Է��ؾ� �մϴ�"), FALSE);
		return -1001;
	}
	
	// 4. <��������ȣ ���Է� >
	// 4.1. ó���Է³���� ���Է³����� ��
	if(m_RegNo == strLibRegNo)
	{
		// 4.2. �����Է� ����
		if(!m_bUserInput)
		{
			//�ڵ��Է�
			OnBtnUnionReturnHistory();				
		}
		else
		{
			//�����Է�
			OnBtnUnionReturnInput();				
		}
		// 4.3. �ʱ�ȭ
		Init();
		return 0;
	}
	else
	{
		//ó���Է�
		m_RegNo = strLibRegNo;
	}

	CESL_DataMgr* pdmLoanHistory = FindDM(_T("DM_UNION_LOAN_HISTORY"));
	BOOL bResult = FALSE;

	// 2. �˻��� �����̷��� �ִ��� ����Ȯ��.
	if(0 < pdmLoanHistory->GetRowCount())
	{
		// 2.1. �˻��� �����̷������� �Է��� ��������ȣ������ �������ڵ尡 ���翩�� Ȯ��
		for(int i = 0; i < pdmLoanHistory->GetRowCount(); i++)
		{
			CString strLibCode; //�����̷������� ������ �ڵ�

			pdmLoanHistory->GetCellData("�������ڵ�", i, strLibCode);

			if(strLibCode == strLibRegNo)
			{
				// 2.2. �ش� �ڷ� ����
				CESL_Grid* pgrid = (CESL_Grid*)FindControl(_T("CM_UNION_RETURN_DLG"), _T("���մ����̷±׸���"));
				pgrid->SetAt(i, pgrid->m_nSelectIdx, pgrid->m_sSelectString);

				SetDlgItemText(IDC_EDT_UNION_RETURN_LIBREGNO, _T(""));
				SetDlgItemText(IDC_STC_MESSAGE, _T(""));
				m_bUserInput = FALSE;
				bResult = TRUE;
				break;
			}
		}
	}

	if(!bResult)
	{
		SelfMessageView(_T("�Է��� ��������ȣ�� �ش��ϴ� �������� �������� �ʽ��ϴ�"), FALSE);
		SetDlgItemText(IDC_EDT_UNION_RETURN_LIBREGNO, _T(""));
		return -1002;
	}

	// 3. �˻��� �����̷��� ���� <�����Է�>
	CESL_DataMgr* pdmReturnLibrary = FindDM( _T("DM_UNION_RETURN_LIBINFO"));

	CString strCode;
	INT		ids;

	for( INT i = 0; i < pdmReturnLibrary->GetRowCount(); i++ )
	{		
		ids = pdmReturnLibrary->GetCellData( _T("�������ڵ�"), i, strCode );
		if( 0 > ids ) return ids;
			
		if(strCode == strLibRegNo)
		{
			// 3.1. �޺����� �ش� ���������� ����
			m_pmcbReturnLibrary->SetCurSel(i+1);
			SetDlgItemText(IDC_EDT_UNION_RETURN_LIBREGNO, _T(""));
			m_bUserInput = TRUE;
			break;
		}
	}

	return 0;

EFS_END
return -1;
}

///////////////////////////////////////////////////////////////////////
// ���մ����̷� �˻�
// 1. ��Ϲ�ȣ�� �����´�.
// 2. ���մ����̷� �˻� ������ �����.
// 3. ���մ����̷� �˻� 
// 4. �˻� ��� ȭ�� ���
// 5. ������
///////////////////////////////////////////////////////////////////////
INT CUnionReturnDlg::SearchUnionLoanHistory()
{
EFS_BEGIN

	INT		ids;

	// 1. ��Ϲ�ȣ�� �����´�.	
	CString strReturnBookRegNo;
    GetDlgItemText(IDC_EDT_UNION_RETURN_REGNO, strReturnBookRegNo);

	if( strReturnBookRegNo.IsEmpty() )
	{
		//AfxMessageBox(_T("�ݳ� ��� �ڷ��� ��Ϲ�ȣ�� �Է��ؾ� �մϴ�"));
		SelfMessageView(_T("�ݳ� ��� �ڷ��� ��Ϲ�ȣ�� �Է��ؾ� �մϴ�"), FALSE);
		return -1001;
	}

	Init();

	ids = MakeValidRegNo(strReturnBookRegNo);
	if( 0 > ids )	return -1010;

	// 2. ���մ����̷� �˻� ������ �����.
	CString strQuery;
	strQuery.Format(_T("CCL.REG_NO='%s'"), strReturnBookRegNo);

	// 3. ���մ����̷� �˻� 	
	CRS_SQL		  RemoteDBMgr(this);
	CESL_DataMgr* pdmLoanHistory = FindDM(_T("DM_UNION_LOAN_HISTORY"));

//	ids = RemoteDBMgr.SetFetchCount(0, 100);
	if( 0 > ids ) return ids;
//	ids = RemoteDBMgr.LOAN_Select(pdmLoanHistory, strQuery, _T("DM_UNION_LOAN_HISTORY"), 1, 1);
	if( 0 > ids ) return ids;

	// 4. �˻� ��� ȭ�� ���	
	ids = AllControlDisplay(_T("CM_UNION_RETURN_DLG"));
	if( 0 > ids ) return ids;

	// 5. ������
	SetDlgItemText(IDC_EDT_UNION_RETURN_REGNO, _T(""));

	if( 0 < pdmLoanHistory->GetRowCount() )
	{
		if( 1 == pdmLoanHistory->GetRowCount() )
		{
			CString strLibCode;
			CString strCode;
			INT		ids;

			pdmLoanHistory->GetCellData("�������ڵ�", 0, strLibCode);

			CESL_DataMgr* pdmReturnLibrary = FindDM( _T("DM_UNION_RETURN_LIBINFO"));
			
			for( INT i = 0; i < pdmReturnLibrary->GetRowCount(); i++ )
			{		
				ids = pdmReturnLibrary->GetCellData( _T("�������ڵ�"), i, strCode );
				if( 0 > ids ) return ids;
				
				if(strCode == strLibCode)
				{
					//�޺����� �ش� ���������� ����
					m_pmcbReturnLibrary->SetCurSel(i+1);
					SetDlgItemText(IDC_EDT_UNION_RETURN_LIBREGNO, _T(""));
					m_bUserInput = TRUE;
					break;
				}
			}

			CESL_Grid* pgrid = (CESL_Grid*)FindControl(_T("CM_UNION_RETURN_DLG"), _T("���մ����̷±׸���"));
			pgrid->SetAt(0, pgrid->m_nSelectIdx, pgrid->m_sSelectString);

			m_RegNo = strLibCode;
		}
		//AfxMessageBox(_T("���� �̷��� �˻� �Ǿ����ϴ�"));
		SelfMessageView(_T("���� �̷��� �˻� �Ǿ����ϴ�"), TRUE);

		GetDlgItem(IDC_BTN_UNION_RETURN_HISTORY)->EnableWindow();		
		GetDlgItem(IDC_BTN_UNION_RETURN_INPUT)->EnableWindow();

		// 20071128 ADD BY PWR ++++++++++++++++++++++++++++++++++++++++++
		// ��Ϲ�ȣ�� �˻��� �Ǹ� ��Ϲ�ȣ ����Ʈ�ڽ��� �˻���ư ��Ȱ��ȭ
		// ��������ȣ ����Ʈ�ڽ��� ���ù�ư Ȱ��ȭ�ϸ鼭
		// ��������ȣ ����Ʈ�ڽ��� ��Ŀ���̵�
		m_edtRegNo.EnableWindow(FALSE);
		m_btnSearch.EnableWindow(FALSE);
		m_edtLibRegNo.EnableWindow(TRUE);
		m_btnSelect.EnableWindow(TRUE);
		m_edtLibRegNo.SetFocus();
		// --------------------------------------------------------------
	}
	else
	{
		//AfxMessageBox(_T("�˻��� ���� �̷��� �����ϴ�"));
		SelfMessageView(_T("�˻��� ���� �̷��� �����ϴ�"), FALSE);

		GetDlgItem(IDC_BTN_UNION_RETURN_INPUT)->EnableWindow();

		// 20071128 ADD BY PWR ++++++++++++++++++++++++++++++++++++++++++
		// �����̷��� ������ ��Ϲ�ȣ ����Ʈ�ڽ��� �˻���ư ��Ȱ��ȭ
		// ��������ȣ ����Ʈ�ڽ��� ���ù�ư Ȱ��ȭ�ϸ鼭
		// ��������ȣ ����Ʈ�ڽ��� ��Ŀ���̵�
		m_edtRegNo.EnableWindow(FALSE);
		m_btnSearch.EnableWindow(FALSE);
		m_edtLibRegNo.EnableWindow(TRUE);
		m_btnSelect.EnableWindow(TRUE);
		m_edtLibRegNo.SetFocus();
		// --------------------------------------------------------------
	}

	SetDlgItemText(IDC_EDT_UNION_RETURN_REGNO_USERINPUT, strReturnBookRegNo);

	return 0;

EFS_END
return -1;
}

///////////////////////////////////////////////////////////////////////
// ���մ����̷� Ÿ�� �ݳ�
// 1. �˻������ ���� Ȯ��
// 2. �����̷� �׸��忡�� ���ð� Ȯ��
// 3. ������ �ڵ带 �����´�.
// 4. ��Ϲ�ȣ�� �����´�.
// 5. ���մ����ڼ����� ���� ����
///////////////////////////////////////////////////////////////////////
INT CUnionReturnDlg::ReturnUnionLoanHistory()
{
EFS_BEGIN

	INT		ids;

	// 1. �˻������ ���� Ȯ��
	CESL_DataMgr* pdmLoanHistory = FindDM(_T("DM_UNION_LOAN_HISTORY"));
	if( 0 == pdmLoanHistory->GetRowCount() )
	{
		//AfxMessageBox(_T("���մ����̷��� �˻��� �ֽʽÿ�"));
		SelfMessageView(_T("���մ����̷��� �˻��� �ֽʽÿ�"), FALSE);
		return -1000;
	}

	// 2. �����̷� �׸��忡�� ���ð� Ȯ��
	CESL_Grid* pgrid = (CESL_Grid*)FindControl(_T("CM_UNION_RETURN_DLG"), _T("���մ����̷±׸���"));		
	pgrid->SelectMakeList();
	
	INT		nSelectCount = pgrid->SelectGetCount();
	if( 0 == nSelectCount )
	{
		//AfxMessageBox(_T("�ݳ� ��� �̷��� ������ �ֽʽÿ�"));
		SelfMessageView(_T("�ݳ� ��� �̷��� ������ �ֽʽÿ�"), FALSE);
		return -1010;
	}
	else if( 1 < nSelectCount )
	{
		//AfxMessageBox(_T("�Ѱ��� �ݳ� ��� �̷¸� ������ �ֽʽÿ�"));
		SelfMessageView(_T("�Ѱ��� �ݳ� ��� �̷¸� ������ �ֽʽÿ�"), FALSE);
		pgrid->UnSelectAll();
		return -1020;
	}

	// 3. ������ �ڵ带 �����´�.
	pgrid->SelectGetHeadPosition();
	INT		nSelectIdx = pgrid->SelectGetIdx();

	CString strReturnLibraryCode;
	ids = pdmLoanHistory->GetCellData(_T("�������ڵ�"), nSelectIdx, strReturnLibraryCode);
	if( 0 > ids ) return -1030;

	// 4. ��Ϲ�ȣ�� �����´�.
	CString strReturnBookRegNo;
	ids = pdmLoanHistory->GetCellData(_T("��Ϲ�ȣ"), nSelectIdx, strReturnBookRegNo);
	if( 0 > ids )	return -1040;

	// 5. ���մ����ڼ����� ���� ����
	ids = SendUnionReturnInfo(strReturnLibraryCode, strReturnBookRegNo);
	if( 0 > ids )	return -1050;

	return 0;

EFS_END
return -1;
}

///////////////////////////////////////////////////////////////////////
// ����� �Է� Ÿ�� �ݳ�
// 1. �޺��ڽ����� ������ �ڵ带 �����´�.
// 2. ��Ϲ�ȣ�� �����´�.
// 3. Ÿ���ݳ���û ������ �����.
// 4. ���մ����ڼ����� ���� ����
///////////////////////////////////////////////////////////////////////
INT CUnionReturnDlg::ReturnUnionUserInput()
{
EFS_BEGIN

	INT		ids;

	// 1. �޺��ڽ����� ������ �ڵ带 �����´�.
	CString strReturnLibraryCode;
	ids = m_pmcbReturnLibrary->GetData(strReturnLibraryCode);
	
	strReturnLibraryCode.TrimLeft();
	strReturnLibraryCode.TrimRight();

	if( strReturnLibraryCode.IsEmpty() )
	{
		//AfxMessageBox(_T("�ݳ� �������� �����ؾ� �մϴ�"));
		SelfMessageView(_T("�ݳ� �������� �����ؾ� �մϴ�"), FALSE);
		return -1010;
	}

	// 2. ��Ϲ�ȣ�� �����´�.
	CString strReturnBookRegNo;
	GetDlgItemText(IDC_EDT_UNION_RETURN_REGNO_USERINPUT, strReturnBookRegNo);

	if( strReturnBookRegNo.IsEmpty() )
	{
		//AfxMessageBox(_T("�ݳ� ��� �ڷ��� ��Ϲ�ȣ�� �Է��ؾ� �մϴ�"));
		SelfMessageView(_T("�ݳ� ��� �ڷ��� ��Ϲ�ȣ�� �Է��ؾ� �մϴ�"), FALSE);
		return -1001;
	}

	ids = MakeValidRegNo(strReturnBookRegNo);
	if( 0 > ids )	return -1020;

	// 3. Ÿ���ݳ���û ������ �����.
	// 4. ���մ����ڼ����� ���� ����
	ids = SendUnionReturnInfo(strReturnLibraryCode, strReturnBookRegNo);
	if( 0 > ids )	return -1030;

	return 0;

EFS_END
return -1;
}

///////////////////////////////////////////////////////////////////////
// Ÿ�� �ݳ� ������ ���մ����ڼ����� �����Ѵ�.
// 1. Ÿ���ݳ���û ������ �����.
// 2. ���մ����ڼ����� ���� ����
///////////////////////////////////////////////////////////////////////
INT CUnionReturnDlg::SendUnionReturnInfo(CString strReturnLibraryCode, CString strReturnBookRegNo)
{
EFS_BEGIN
	
	INT		ids;

	// 1. Ÿ���ݳ���û ������ �����.	
	// �α׸� �����Ѵ�.	
	CString strWorkLog;
	strWorkLog = GetWorkLogMsg( _T("Ÿ���ڷ�ݳ���û"), __FILE__, __LINE__ );	

	// MAKE QUERY
	CString strQuery;
	strQuery.Format(_T("INSERT INTO CT_TEMP_RETURN_INFO_TBL(REC_KEY, REQUEST_LIB_CODE, UPDATE_LIB_CODE, REG_NO, REQUEST_DATE, STATUS, FIRST_WORK) ")
					_T("VALUES(ESL_SEQ.NEXTVAL, '%s', '%s', '%s', SYSDATE, '0', '%s');"), m_strLibraryCode, strReturnLibraryCode, strReturnBookRegNo, strWorkLog );
	
	// 2. ���մ����ڼ����� ���� ����	
	CRS_SQL RemoteDBMgr( this );
//	ids = RemoteDBMgr.LOAN_UnSelect( strQuery );
	if( 0 > ids )
	{
		//AfxMessageBox( _T("���մ����ڼ��� ���� ���� ����!!") );
		SelfMessageView(_T("���մ����ڼ��� ���� ���� ����!! (1)"), FALSE);
		return ids;
	}

	///{{++
	/// 2008.05.02 add by pdw
	/// Ÿ���ݳ��� �̿��� ������ �������� ������ �̸� ���ش�
	/// ��� �̷��� �̸� update �Ͽ� ���մ���å�� ���� ��ɰ� ����ɼ� �ֵ����Ѵ�
	/// 1. �̿��� ������ ������Ʈ �Ѵ�	
	/*
UPDATE CT_LIBRARY_LOAN_USER_TBL@K2UPLIS
   SET USER_CLASS = DECODE( USER_CLASS, 0, 
						   DECODE( (SELECT SIGN(TO_DATE(RETURN_PLAN_DATE)-TO_DATE(SYSDATE)) 
									  FROM CT_CO_LOAN_INFO_TBL@K2UPLIS 
									 WHERE STATUS IN ('0','2') 
									   AND LIB_CODE='121007' AND REG_NO='EM0000131513'), -1, 1, 0), USER_CLASS)
 WHERE MASTER_USER_KEY = ( SELECT MASTER_USER_KEY 
                             FROM CT_CO_LOAN_INFO_TBL@K2UPLIS 
							WHERE STATUS IN ('0','2') 
                              AND LIB_CODE='121007' AND REG_NO='EM0000131513' )
   AND LIB_CODE = '121007';   
	*/
	strQuery.Format(_T("UPDATE CT_LIBRARY_LOAN_USER_TBL ")
		              _T(" SET INFERIOR_CLASS = DECODE( INFERIOR_CLASS, 0, ")
					                          _T(" DECODE( (SELECT SIGN(TO_DATE(RETURN_PLAN_DATE)-TO_DATE(SYSDATE)) ")
						        			             _T(" FROM CT_CO_LOAN_INFO_TBL ")
													    _T(" WHERE STATUS IN ('0','2') ")
   													      _T(" AND LIB_CODE='%s' AND REG_NO='%s'), -1, 1, 0), INFERIOR_CLASS) ")
                       _T(" WHERE MASTER_USER_KEY = ( SELECT MASTER_USER_KEY ")
					                            _T(" FROM CT_CO_LOAN_INFO_TBL ")
											   _T(" WHERE STATUS IN ('0','2') ")
											     _T(" AND LIB_CODE='%s' AND REG_NO='%s' ) ")
	                  _T(" AND LIB_CODE = '%s';"), strReturnLibraryCode, strReturnBookRegNo, 
												  strReturnLibraryCode, strReturnBookRegNo, strReturnLibraryCode);
//	ids = RemoteDBMgr.LOAN_UnSelect( strQuery );
	if( 0 > ids )
	{
		//AfxMessageBox( _T("���մ����ڼ��� ���� ���� ����!!") );
		SelfMessageView(_T("���մ����ڼ��� ���� ���� ����!! (2)"), FALSE);
		return ids;
	}
	/// 2. �����̷� ������ ������Ʈ �Ѵ�
	/*
UPDATE CT_CO_LOAN_INFO_TBL@K2UPLIS 
   SET STATUS = '7', RETURN_DATE = SYSDATE
 WHERE STATUS IN ('0','2') AND LIB_CODE='121007' AND REG_NO='EM0000131513'  
	*/
	strQuery.Format(_T("UPDATE CT_CO_LOAN_INFO_TBL ")
		              _T(" SET STATUS = '7', RETURN_DATE = SYSDATE ")
				    _T(" WHERE STATUS IN ('0','2') AND LIB_CODE='%s' AND REG_NO='%s'; "),
					strReturnLibraryCode, strReturnBookRegNo);
//	ids = RemoteDBMgr.LOAN_UnSelect( strQuery );
	if( 0 > ids )
	{
		//AfxMessageBox( _T("���մ����ڼ��� ���� ���� ����!!") );
		SelfMessageView(_T("���մ����ڼ��� ���� ���� ����!! (3)"), FALSE);
		return ids;
	}
	///}}--

	if( ((CButton*)GetDlgItem(IDC_CHK_HOLD))->GetCheck() == 0 )
		SelfMessageView(_T("�ݳ� ��û�Ǿ����ϴ�"), TRUE);
	else
	{
		m_nReturnCode = 10000;
		OnOK();
	}

	Init();

	return 0;

EFS_END
return -1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// 20071220 add by pwr 
// ** ��Ʈ�� SHOW/HIDE
void CUnionReturnDlg::SetControlShow(BOOL bShow)
{
EFS_BEGIN
	if(!bShow)
	{
		((CButton*)GetDlgItem(IDC_BTN_UNION_RETURN_INPUT))->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_EDT_UNION_RETURN_REGNO_USERINPUT))->ShowWindow(SW_HIDE);
		((CComboBox*)GetDlgItem(IDC_CMB_UNION_RETURN_LIBLIST))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_STC_REGNO))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_STC_LIB))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_STC_HANDOPERATED))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_STC_LIBNO))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_STC_LIBNO2))->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_EDT_UNION_RETURN_LIBREGNO))->ShowWindow(SW_HIDE);
		((CButton*)GetDlgItem(IDC_BTN_UNION_SELECT))->ShowWindow(SW_HIDE);
		((CButton*)GetDlgItem(IDC_EDT_UNION_RETURN_REGNO))->ShowWindow(SW_SHOW);
		((CMSHFlexGrid*)GetDlgItem(IDC_GRD_LOAN_HISTORY))->ShowWindow(SW_HIDE);
		((CMSHFlexGrid*)GetDlgItem(IDC_GRD_RETURN_HISTORY))->ShowWindow(SW_SHOW);		
		((CButton*)GetDlgItem(IDC_BTN_UNION_SEARCH))->SetWindowText(_T("�� ��"));
		((CButton*)GetDlgItem(IDC_BTN_UNION_RETURN_HISTORY))->SetWindowText(_T("�ݳ����"));
		((CButton*)GetDlgItem(IDC_CHK_HOLD))->SetWindowText(_T("�ݳ������ ȭ��ݱ�"));
		SetDlgItemText(IDC_STC_MESSAGE, _T(""));
		GetDlgItem(IDC_BTN_UNION_RETURN_HISTORY)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDT_UNION_RETURN_REGNO)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_UNION_SEARCH)->EnableWindow(TRUE);
		SetContolPos(bShow);	
	}
	else
	{
		Init();
		((CButton*)GetDlgItem(IDC_BTN_UNION_RETURN_INPUT))->ShowWindow(SW_SHOW);
		((CEdit*)GetDlgItem(IDC_EDT_UNION_RETURN_REGNO_USERINPUT))->ShowWindow(SW_SHOW);
		((CComboBox*)GetDlgItem(IDC_CMB_UNION_RETURN_LIBLIST))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_STC_REGNO))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_STC_LIB))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_STC_HANDOPERATED))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_STC_LIBNO))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_STC_LIBNO2))->ShowWindow(SW_SHOW);
		((CEdit*)GetDlgItem(IDC_EDT_UNION_RETURN_LIBREGNO))->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_BTN_UNION_SELECT))->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_EDT_UNION_RETURN_REGNO))->ShowWindow(SW_SHOW);
		((CMSHFlexGrid*)GetDlgItem(IDC_GRD_LOAN_HISTORY))->ShowWindow(SW_SHOW);
		((CMSHFlexGrid*)GetDlgItem(IDC_GRD_RETURN_HISTORY))->ShowWindow(SW_HIDE);
		((CButton*)GetDlgItem(IDC_BTN_UNION_SEARCH))->SetWindowText(_T("�� ��"));
		((CButton*)GetDlgItem(IDC_BTN_UNION_RETURN_HISTORY))->SetWindowText(_T("Ÿ���ݳ�"));
		((CButton*)GetDlgItem(IDC_CHK_HOLD))->SetWindowText(_T("Ÿ���ݳ��� ȭ��ݱ�"));
		SetDlgItemText(IDC_STC_MESSAGE, _T(""));
		SetContolPos(bShow);
	}
EFS_END
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// 20071221 add by pwr 
// ** �޽����信 �޽����� �����ش�.
void CUnionReturnDlg::SelfMessageView(CString sMsg, BOOL bStatus)
{
EFS_BEGIN

	//���ٲٴ°� �߰� ����
	if(bStatus)
		m_bStatus = TRUE;
	else
		m_bStatus = FALSE;

//	CStatic* stcMsg = (CStatic*)GetDlgItem();
	SetDlgItemText(IDC_STC_MESSAGE,"  " + sMsg);
//	stcMsg->SetWindowText();

	UpdateWindow();	

	//�ڵ� ���� ����

EFS_END
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// 20071221 add by pwr 
// **��Ϲ�ȣ�� �Է��� ���� ó���Է°��� �ι�° �Է°��� ������ �ش� �ڷ� �ݳ����ó���Ѵ�.
// 1. ��Ϲ�ȣ�� �����´�.	
// 2. ��Ϲ�ȣ ��ȿ������ �˻�
// 3. ��Ϲ�ȣ�� 12�ڸ��� ������ش�.
// 4. ó���Է��ϴ°����� Ȯ�������� �˻�
//		( ó���Է°��� �ι�° �Է°��� ������ �ݳ����ó��, Ʋ���� ���ο��(ó���Է�) )
//	- 4.1. �ݳ����ó�� ����
//	- 4.2. �����Է�(ó���Է�)
// 5. �Է°��� �ش��ϴ� ����Ÿ ����
// 6. �������� �Ѱ������� �˻�
// 7. ������ ��Ϲ�ȣ�� ���������� ���� (Ȯ���Ҷ� ������)
INT CUnionReturnDlg::SearchUnionReturnHistory()
{
EFS_BEGIN

	INT		ids;

	// 1. ��Ϲ�ȣ�� �����´�.	
	CString strReturnBookRegNo;
    GetDlgItemText(IDC_EDT_UNION_RETURN_REGNO, strReturnBookRegNo);
	SetDlgItemText(IDC_EDT_UNION_RETURN_REGNO, _T(""));

	// 2. ��Ϲ�ȣ ��ȿ������ �˻�
	if( strReturnBookRegNo.IsEmpty() )
	{
		SelfMessageView(_T(" �ݳ� ��� �ڷ��� ��Ϲ�ȣ�� �Է��ؾ� �մϴ�"), FALSE);
		return -1001;
	}

	// 3. ��Ϲ�ȣ�� 12�ڸ��� ������ش�.
	ids = MakeValidRegNo(strReturnBookRegNo);
	if( 0 > ids )	return -1010;

	// 4. ó���Է��ϴ°����� Ȯ�������� �˻�
	//    ( ó���Է°��� �ι�° �Է°��� ������ �ݳ����ó��, Ʋ���� �����Է�(ó���Է�) )
	if( m_ReturnRegNo == strReturnBookRegNo)
	{
		// 4.1 �ݳ����ó�� ����
		ReturnUnionReturnHistory();
		return 0;
	}
	else
	{
		// 4.2 �����Է�(ó���Է�)
		ControlDisplay(_T("CM_UNION_RETURN_DLG"), _T("Ÿ���ݳ��̷±׸���"));
	}

	// 5. �Է°��� �ش��ϴ� ����Ÿ ����
	int nCheck = 0;
	for(int i = 0; i < m_pDMReturnHistory->GetRowCount(); i++)
	{
		CString strRegNo;
		m_pDMReturnHistory->GetCellData( _T("��Ϲ�ȣ"), i, strRegNo );
		if(strReturnBookRegNo == strRegNo)
		{
			m_pReturnHistoryGrid->SetAt(i, m_pReturnHistoryGrid->m_nSelectIdx, 
											m_pReturnHistoryGrid->m_sSelectString);
			nCheck++;
		}
	}

	// 6. �������� �Ѱ������� �˻�
	if( nCheck > 1 )
	{
		SelfMessageView(_T(" �˻��� �̷��� �ΰ��̻� �Դϴ�. ���� ������ �ּ���"), FALSE);
		ControlDisplay(_T("CM_UNION_RETURN_DLG"), _T("Ÿ���ݳ��̷±׸���"));
		return -1020;
	}

	// 7. ������ ��Ϲ�ȣ�� ���������� ���� (Ȯ���Ҷ� ������)
	m_ReturnRegNo = strReturnBookRegNo;

	return 0;

EFS_END
return -1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// 20071221 add by pwr
// **�ݳ����ó�� ����
// 1. ��󵵼����ڵ带 �����´�.
// 2. ��Ϲ�ȣ�� �����´�.
// 3. rec_key�� �����´�.
// 4. ���մ����ڼ����� ���� ����
INT CUnionReturnDlg::ReturnUnionReturnHistory()
{
EFS_BEGIN

	INT	ids;
	
	m_pReturnHistoryGrid->SelectMakeList();
	m_pReturnHistoryGrid->SelectGetHeadPosition();
	INT	nSelectIdx = m_pReturnHistoryGrid->SelectGetIdx();

	INT	nSelectCount = m_pReturnHistoryGrid->SelectGetCount();

	if( 0 > nSelectCount )
	{
		SelfMessageView(_T(" �ݳ���� ��� �̷��� ������ �ֽʽÿ�"), FALSE);
		return -1010;
	}
	else if( 1 < nSelectCount )
	{
		SelfMessageView(_T(" �Ѱ��� �ݳ���� ��� �̷¸� ������ �ֽʽÿ�"), FALSE);
		m_pReturnHistoryGrid->UnSelectAll();
		return -1020;
	}

	// 1. ��󵵼����ڵ带 �����´�.
	CString strReturnLibraryCode;
	ids = m_pDMReturnHistory->GetCellData(_T("��󵵼����ڵ�"), nSelectIdx, strReturnLibraryCode);
	if( 0 > ids ) return -1030;

	// 2. ��Ϲ�ȣ�� �����´�.
	CString strReturnBookRegNo;
	ids = m_pDMReturnHistory->GetCellData(_T("��Ϲ�ȣ"), nSelectIdx, strReturnBookRegNo);
	if( 0 > ids )	return -1040;

	// 3. rec_key�� �����´�.
	CString strRecKey;
	ids = m_pDMReturnHistory->GetCellData(_T("REC_KEY"), nSelectIdx, strRecKey);

	// 4. ���մ����ڼ����� ���� ����
	ids = SendUnionReturnCancelInfo(strReturnLibraryCode, strReturnBookRegNo, strRecKey);
	if( 0 > ids )	return -1050;

	ControlDisplay(_T("CM_UNION_RETURN_DLG"), _T("Ÿ���ݳ��̷±׸���"));

	return 0;

EFS_END
return -1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// 20071221 add by pwr
// ** �ݳ����ó�� ������ư ����
void CUnionReturnDlg::OnRadioReturnCancel() 
{
EFS_BEGIN

	// 1. ��Ʈ�� ó��
	SetControlShow(FALSE);

	// 5. ��Ϲ�ȣ ����Ʈ�ڽ��� ��Ŀ�� �̵�
	((CEdit*)GetDlgItem(IDC_EDT_UNION_RETURN_REGNO))->SetFocus();

	INT ids;
	// 2. ���մ����̷� �˻� 	
	CRS_SQL		  RemoteDBMgr(this);
	m_pDMReturnHistory = FindDM(_T("DM_UNION_TEMP_RETURN_INFO"));
	m_pReturnHistoryGrid = (CESL_Grid*)FindControl(_T("CM_UNION_RETURN_DLG"), _T("Ÿ���ݳ��̷±׸���"));
	
//	ids = RemoteDBMgr.SetFetchCount(0, 100);
	if( 0 > ids ) SelfMessageView(_T(" ERROR!! : RS_SQL - FetchCount"), FALSE);
	
	// ������ �����͸��� ������������ ���ǻ���
	CString sQuery;
	GetWeekQuery(sQuery);

//	ids = RemoteDBMgr.LOAN_Select(m_pDMReturnHistory, sQuery, _T("DM_UNION_TEMP_RETURN_INFO"), 1, 1);
	if( 0 > ids ) SelfMessageView(_T(" ERROR!! : RS_SQL - Select"), FALSE);
	
	// 3. �˻� ��� ȭ�� ���
	ids = ControlDisplay("CM_UNION_RETURN_DLG", "Ÿ���ݳ��̷±׸���");
	if( 0 > ids ) SelfMessageView(_T(" ERROR!! : Grid Display"), FALSE);
	
	// 4. �˻���� ���� Ȯ��
	if( 0 == m_pDMReturnHistory->GetRowCount() )
	{
		SelfMessageView(_T(" �����ϵ��� �˻��� �ݳ��̷��� �����ϴ�."), FALSE);
	}

EFS_END
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// 20071221 add by pwr 
// ** �ݳ�ó�� ������ư ����
void CUnionReturnDlg::OnRadioReturn() 
{
EFS_BEGIN

	//1. ��Ʈ�� ó��
	SetControlShow(TRUE);
	
	//2. ��Ϲ�ȣ ����Ʈ�ڽ��� ��Ŀ�� �̵�
	((CEdit*)GetDlgItem(IDC_EDT_UNION_RETURN_REGNO))->SetFocus();

EFS_END
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// 20071221 add by pwr 
// ** �ݳ���� ������ ���մ����ڼ����� �����Ѵ�.
// 1. �ݳ���ҿ�û ������ �����.
// 2. ���մ����ڼ����� ���� ����
INT CUnionReturnDlg::SendUnionReturnCancelInfo(CString strReturnLibraryCode, CString strReturnBookRegNo, CString strRecKey)
{
EFS_BEGIN
	
	INT		ids;

	// 1. �ݳ���ҿ�û ������ �����.	
	// �α׸� �����Ѵ�.	
	CString strWorkLog;
	strWorkLog = GetWorkLogMsg( _T("Ÿ���ڷ�ݳ���ҽ�û"), __FILE__, __LINE__ );	

	// MAKE QUERY
	CString strQuery;
	strQuery.Format(_T("INSERT INTO CT_TEMP_RETURN_INFO_TBL(REC_KEY, REQUEST_LIB_CODE, UPDATE_LIB_CODE, REG_NO, REQUEST_DATE, STATUS, CANCEL_DES_REC_KEY, FIRST_WORK) ")
					_T("VALUES(ESL_SEQ.NEXTVAL, '%s', '%s', '%s', SYSDATE, '2', '%s', '%s');"), m_strLibraryCode, strReturnLibraryCode, strReturnBookRegNo, strRecKey, strWorkLog );
	
	// 2. ���մ����ڼ����� ���� ����	
	CRS_SQL RemoteDBMgr( this );
//	ids = RemoteDBMgr.LOAN_UnSelect( strQuery );
	if( 0 > ids )
	{
		SelfMessageView(_T(" ���մ����ڼ��� ���� ���� ����!! (1)"), FALSE);
		return ids;
	}
	///{{++
	/// 2008.05.02 add by pdw
	/// Ÿ���ݳ��� �̿��� ������ �������� ������ �̸� ���ش�
	/// ��� �̷��� �̸� update �Ͽ� ���մ���å�� ���� ��ɰ� ����ɼ� �ֵ����Ѵ�
	/// 1. �̿������� ������Ʈ
	///    ��ü�ڷḦ �ݳ��� ��� ������ ��츸 ���������� ����Ǵ�
	///    �ݴ��� ��� ��ü�ڷḦ �������ִٰ� �Ǵܵȴ�
	///    ���� �̿��������� ������Ʈ �� �ʿ䰡 ����
	/// 2. �����̷����� ������Ʈ
	/*
UPDATE CT_CO_LOAN_INFO_TBL@K2UPLIS 
   SET STATUS = DECODE(NVL(DELAY_CNT,0), 0, '0', '2');
       RETURN_DATE = NULL
 WHERE STATUS='7' AND LIB_CODE='121007' AND REG_NO='EM0000131513'
	*/
	strQuery.Format(_T("UPDATE CT_CO_LOAN_INFO_TBL ")
					  _T(" SET STATUS=DECODE(NVL(DELAY_CNT,0),0,'0','2'), RETURN_DATE=NULL ")
					_T(" WHERE STATUS='7' AND LIB_CODE='%s' AND REG_NO='%s'; "), 
					strReturnLibraryCode, strReturnBookRegNo);
//	ids = RemoteDBMgr.LOAN_UnSelect( strQuery );
	if( 0 > ids )
	{
		SelfMessageView(_T(" ���մ����ڼ��� ���� ���� ����!! (2)"), FALSE);
		return ids;
	}
	///}}--

	if( ((CButton*)GetDlgItem(IDC_CHK_HOLD))->GetCheck() == 0 )
	{
		OnRadioReturnCancel();
		SelfMessageView(_T(" �ݳ���� ��û �Ǿ����ϴ�."), TRUE);
	}
	else
	{
		m_nReturnCode = 20000;
		OnOK();
	}

	return 0;

EFS_END
return -1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// 20071221 add by pwr
// ** ���ǹ����� ���� (���糯¥ �������� ������������)
void CUnionReturnDlg::GetWeekQuery(CString& sQuery)
{
	CString sDay1;
	CString sDay2;

	// 2010.08.27 UPDATE BY PWR : �α��ν� ����� �������� ���
	CString strGetData;
	CTime tSysdate = CLocCommonAPI::GetCurrentDateTime(this, strGetData);
// 	CTime tSysdate = CLocCommonAPI::GetDBTime(this, NULL);
	CTimeSpan tExpDay( 6 , 0 , 0 , 0 );
	
	sDay1.Format(_T("%s"),tSysdate.Format(_T("%Y/%m/%d")));
	tSysdate -= tExpDay;
	sDay2.Format(_T("%s"),tSysdate.Format(_T("%Y/%m/%d")));

	sQuery.Format(_T("REQUEST_DATE BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), sDay2, sDay1);
}

void CUnionReturnDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

}

/////////////////////////////////////////////////////////////////////////////////////////////////
// 20071226 add by pwr 
// ** ��Ʈ���� ��ġ�� ���̾�α��� ũ�⸦ �ٲ۴�
void CUnionReturnDlg::SetContolPos(BOOL bCheck)
{
	INT nX		= m_rect.left;
	INT nY		= m_rect.top;
	INT nWidth	= m_rect.Width();
	INT nHeight = m_rect.Height()-80;

	CButton* pChkAutoClose = (CButton*)GetDlgItem(IDC_CHK_HOLD);
	CButton* pBtnClear = (CButton*)GetDlgItem(IDC_BTN_UNION_RETURN_CLEAR);
	CButton* pBtnClose = (CButton*)GetDlgItem(IDC_BTN_UNION_RETURN_CLOSE);

	CRect rChk;
	CRect rClear;
	CRect rClose;

	pChkAutoClose->GetWindowRect(rChk);	//äũ�ڽ�
	pBtnClear->GetWindowRect(rClear);	//�ʱ�ȭ��ư
	pBtnClose->GetWindowRect(rClose);	//�ݱ��ư

	if(!bCheck)
	{
		MoveWindow(nX, nY, nWidth, nHeight);

		pChkAutoClose->MoveWindow(10, nHeight-60, rChk.Width(), rChk.Height());
		pBtnClear->MoveWindow(m_rect.Width()-200, nHeight-70, rClear.Width(), rClear.Height());
		pBtnClose->MoveWindow(m_rect.Width()-110, nHeight-70, rClose.Width(), rClose.Height());
	}
	else
	{
		MoveWindow(nX, nY, nWidth, nHeight + 80);

		pChkAutoClose->MoveWindow(10, nHeight+20, rChk.Width(), rChk.Height());
		pBtnClear->MoveWindow(m_rect.Width()-200, nHeight+10, rClear.Width(), rClear.Height());
		pBtnClose->MoveWindow(m_rect.Width()-110, nHeight+10, rClose.Width(), rClose.Height());
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// 20071228 add by pwr 
// ** ��Ʈ���� ������
HBRUSH CUnionReturnDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// �޽������� �۾����� ���������� ����
	if (pWnd->GetDlgCtrlID() == IDC_STC_MESSAGE)
	{
		if(m_bStatus)
			pDC->SetTextColor(RGB(0, 0, 0));
		else
			pDC->SetTextColor(RGB(255, 0, 0));
	}
	
	return hbr; 
}
