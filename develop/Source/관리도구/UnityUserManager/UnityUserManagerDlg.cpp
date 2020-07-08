// UnityUserManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UnityUserManager.h"
#include "UnityUserManagerDlg.h"
#include "UUMLoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUnityUserManagerDlg dialog

CUnityUserManagerDlg::CUnityUserManagerDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CUnityUserManagerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUnityUserManagerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CUnityUserManagerDlg::~CUnityUserManagerDlg()
{
	
}

void CUnityUserManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUnityUserManagerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUnityUserManagerDlg, CDialog)
	//{{AFX_MSG_MAP(CUnityUserManagerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnCLEAR, OnbtnCLEAR)
	ON_BN_CLICKED(IDC_btnSEARCH, OnbtnSEARCH)
	ON_BN_CLICKED(IDC_btnADD_UnityUser, OnbtnADDUnityUser)
	ON_BN_CLICKED(IDC_btnADD_ObjectUser, OnbtnADDObjectUser)
	ON_BN_CLICKED(IDC_btnUNITY, OnbtnUNITY)
	ON_EN_SETFOCUS(IDC_edtSEARCH_Name, OnSetfocusedtSEARCHName)
	ON_EN_SETFOCUS(IDC_edtSEARCH_UserID, OnSetfocusedtSEARCHUserID)
	ON_EN_SETFOCUS(IDC_edtSEARCH_UserNO, OnSetfocusedtSEARCHUserNO)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUnityUserManagerDlg message handlers

BOOL CUnityUserManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CUUMLoginDlg Dlg(this);
	if(IDCANCEL == Dlg.DoModal())
	{
		PostMessage(WM_QUIT, NULL, NULL);
		return FALSE;
	}

	m_strRecKey = Dlg.m_strRecKey;
	m_strID = Dlg.m_strID;
	m_strName = Dlg.m_strName;

	if(0 > InitESL())
	{
		return FALSE;
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUnityUserManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUnityUserManagerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUnityUserManagerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CUnityUserManagerDlg::OnbtnCLOSE() 
{
	OnCancel();	
}

void CUnityUserManagerDlg::OnbtnCLEAR() 
{
	Clear();
}

void CUnityUserManagerDlg::OnbtnSEARCH() 
{
	INT ids;
	ids = SearchUser();
	if(0 > ids)
	{
		Clear();
		return;
	}
}

void CUnityUserManagerDlg::OnbtnADDUnityUser() 
{
	AddUnityUser();
}

void CUnityUserManagerDlg::OnbtnADDObjectUser() 
{
	AddObjectUser();
}

void CUnityUserManagerDlg::OnbtnUNITY() 
{
	UnityProc();
}

INT CUnityUserManagerDlg::InitESL()
{
	if(0 > InitESL_Mgr(_T("SM_UNITY_USER_MANAGER")))
 	{
 		AfxMessageBox(_T("InitESL_Mgr Error : SM_UNITY_USER_MANAGER"));
 		return -1000;
 	}

// 	CESL_DataMgr *pCodeDM = FindDM(_T("DM_UUM_CODE"));
// 	if(NULL == pCodeDM)
// 	{
// 		return -1010;
// 	}
// 	pCodeDM->RefreshDataManager(_T(""));

	return 0;
}

INT CUnityUserManagerDlg::UnityProc()
{
	INT ids;
	// 1. �����̿��� Ȯ��
	CESL_DataMgr *pUnityDM = FindDM(_T("DM_UUM_UNITY_USER"));
	if(NULL == pUnityDM)
	{
		return -1000;
	}
	if(1 > pUnityDM->GetRowCount())
	{
		MessageBox(_T("�����̿��ڸ� ������ �ֽʽÿ�."), _T("�̿���������"), MB_ICONEXCLAMATION);
		return 0;
	}
	// 2. ����̿��� Ȯ��
	CESL_DataMgr *pObjectDM = FindDM(_T("DM_UUM_OBJECT_USER"));
	if(NULL == pObjectDM)
	{
		return -1010;
	}
	if(1 > pObjectDM->GetRowCount())
	{
		MessageBox(_T("����̿��ڸ� ������ �ֽʽÿ�."), _T("�̿���������"), MB_ICONEXCLAMATION);
		return 0;
	}
	// 3. �����̿��ڿ� ����̿��ڰ� �ٸ� �̿������� Ȯ��
	CString strUnityRecKey, strObjectRecKey;
	pUnityDM->GetCellData(_T("REC_KEY"), 0, strUnityRecKey);
	pObjectDM->GetCellData(_T("REC_KEY"), 0, strObjectRecKey);
	if(strUnityRecKey.IsEmpty())
	{
		MessageBox(_T("�����̿��ڸ� ������ �ֽʽÿ�."), _T("�̿���������"), MB_ICONEXCLAMATION);
		return 0;
	}
	if(strObjectRecKey.IsEmpty())
	{
		MessageBox(_T("����̿��ڸ� ������ �ֽʽÿ�."), _T("�̿���������"), MB_ICONEXCLAMATION);
		return 0;
	}
	if(strUnityRecKey == strObjectRecKey)
	{
		MessageBox(_T("����̿��ڴ� �����̿��ڿ� �ٸ� �̿��ڷ� ������ �ֽʽÿ�."), _T("�̿���������"), MB_ICONEXCLAMATION);
		return 0;
	}
	// 4. ��Ȯ�θ޽���
	if(IDYES != MessageBox(_T("������ �����ϸ� ����̿����� ��Ȳ�� �̷���\r\n�����̿��ڷ� ���յǰ� ����̿��ڴ� �����˴ϴ�.\r\n������ �����Ͻðڽ��ϱ�?"), _T("�̿���������"), MB_ICONEXCLAMATION|MB_YESNO))
	{
		return 0;
	}
	// 5. ����̿����� ��Ȳ,�̷� �����̿��ڷ� �̵�
	CESL_DataMgr TranDM; // update, insert �� Ʈ���������� ��������
	TranDM.SetCONNECTION_INFO(_T(""));
 	TranDM.FreeDBFieldData();
 	TranDM.StartFrame();
	ChangeTableValue(TranDM, strUnityRecKey, strObjectRecKey);
// 	MoveTableValue(TranDM, _T("LS_WORK_T01"), strUnityRecKey, strObjectRecKey);
// 	MoveTableValue(TranDM, _T("CO_LOAN_TBL"), strUnityRecKey, strObjectRecKey);
	// 6. ����̿��� �������̺�� �̵�
	MoveObjectUser(TranDM, strUnityRecKey, strObjectRecKey);
 	ids = TranDM.SendFrame();
	if(0 > ids)
	{
		MessageBox(_T("���ս���."), _T("�̿���������"), MB_ICONEXCLAMATION);
		return -1;
	}
 	TranDM.EndFrame();
	// 7. �޽��� 
	MessageBox(_T("���յǾ����ϴ�."), _T("�̿���������"), MB_ICONEXCLAMATION);
	// 8. �̿��� ����
	SearchUser();
	SetUnityUser(strUnityRecKey);
	SetObjectUser(_T(""));

	return 0;
}

INT CUnityUserManagerDlg::ChangeTableValue(CESL_DataMgr &TranDM, CString strUnityRecKey, CString strObjectRecKey)
{
	CESL_DataMgr LS_WORK_TBL_DM;
	LS_WORK_TBL_DM.SetCONNECTION_INFO(_T(""));
	CESL_DataMgr CO_LOAN_TBL_DM;
	CO_LOAN_TBL_DM.SetCONNECTION_INFO(_T(""));

	CString strQuery;
	strQuery.Format(_T("UPDATE LS_WORK_T01 SET USER_KEY=%s, LAST_WORK=TO_CHAR(SYSDATE,'YYYY/MM/DD HH24:MI:SS')||' '||'%s'||'@'||'%s'||' �̿��������� #'||'%s' WHERE USER_KEY=%s;"), strUnityRecKey, m_strID, GetIP(), strObjectRecKey, strObjectRecKey);
	TranDM.AddFrame(strQuery);
	strQuery.Format(_T("UPDATE CO_LOAN_TBL SET USER_KEY=%s, LAST_WORK=TO_CHAR(SYSDATE,'YYYY/MM/DD HH24:MI:SS')||' '||'%s'||'@'||'%s'||' �̿��������� #'||'%s' WHERE USER_KEY=%s;"), strUnityRecKey, m_strID, GetIP(), strObjectRecKey, strObjectRecKey);
	TranDM.AddFrame(strQuery);

	return 0;
}

INT CUnityUserManagerDlg::MoveTableValue(CESL_DataMgr &TranDM, CString strTableName, CString strUnityRecKey, CString strObjectRecKey)
{
	// 1. �� DB��Ȳ�� �´� strTableName�� COLUMNS�� ���Ѵ�.
	CESL_DataMgr SelectDM;
	SelectDM.SetCONNECTION_INFO(_T(""));
	CESL_DataMgr *pColumnsDM = FindDM(_T("DM_UUM_COLUMNS"));
	if(NULL == pColumnsDM)
	{
		return -1020;
	}
	pColumnsDM->FreeData();
	CString strWhere;
	strWhere.Format(_T("TABLE_NAME='%s'"), strTableName);
	pColumnsDM->RefreshDataManager(strWhere);

	INT nUserKeyIndex;
	CStringArray asType;
	// COLUMNS�� INSERT��� UPDATE������ �и��Ͽ� ����
	CString strColumns_Select, strColumns_Insert;
	for(INT i = 0; i < pColumnsDM->GetRowCount()+1; i++)
	{
		if(i == pColumnsDM->GetRowCount())
		{
			strColumns_Select += _T(",REC_KEY");
			strColumns_Insert += _T(",REC_KEY");
			break;
		}

		CString strName, strType;
		pColumnsDM->GetCellData(_T("�÷���"), i, strName);
		pColumnsDM->GetCellData(_T("Ÿ��"), i, strType);
		if(_T("USER_KEY") == strName)
		{ // USER_KEY�� INDEX��� (����̿����� REC_KEY�� �����̿����� REC_KEY�� ��������)
			nUserKeyIndex = i;
		}
		if(0 != i)
		{
			strColumns_Select += _T(",");
			strColumns_Insert += _T(",");
		}
		if(_T("DATE") == strType)
		{ // DATE���� �ú��� �޾ƿ���
			strColumns_Select += _T("TO_CHAR(") + strName + _T(",'YYYY/MM/DD HH24:MI:SS')");
		}
		else
		{
			strColumns_Select += strName;
		}
		strColumns_Insert += strName;

		asType.Add(strType);
	}
	
	// 2. ����̿����� ��Ȳ�̳� �̷��� SELECT �Ѵ�.
	CString strQuery;
	strQuery.Format(_T("SELECT %s FROM %s WHERE USER_KEY=%s"), strColumns_Select, strTableName, strObjectRecKey);
	SelectDM.RestructDataManager(strQuery);

	// 3. �����̿��ڿ��� �̵��Ѵ�.
	for(i = 0; i < SelectDM.GetRowCount(); i++)
	{
		// 3.1 INSERT
		CString strDelRecKey;
		strQuery = _T("INSERT INTO ") + strTableName + _T("(") + strColumns_Insert + _T(") VALUES (");
		for(INT j = 0; j < pColumnsDM->GetRowCount()+1; j++)
		{
			if(0 != j)
			{
				strQuery += _T(",");
			}
			CString strData;
			SelectDM.GetCellData(i, j, strData);
			if(j == pColumnsDM->GetRowCount())
			{ 
				// ���� REC_KEY�̹Ƿ� ���
				strDelRecKey = strData;
				// REC_KEY�� ���λ���
				strQuery += _T("ESL_SEQ.NEXTVAL");
				break;
			}

			CString strType = asType.GetAt(j); 
			if(strData.IsEmpty())
			{
				strQuery += _T("NULL");
			}
			else
			{
				if(_T("CHAR") == strType || _T("LONG") == strType || _T("VARCHAR2") == strType)
				{
					// ' -> ''
					strData.Replace(_T("'"), _T("''"));
					strQuery += _T("'") + strData + _T("'");
				}
				else if(_T("NUMBER") == strType)
				{
					if(j == nUserKeyIndex)
					{ // USER_KEY�� �����̿����� REC_KEY��
						strQuery += strUnityRecKey;
					}
					else
					{
						strQuery += strData;
					}
				}
				else if(_T("DATE") == strType)
				{ // DATE���� �ú��� ����
					strQuery += _T("TO_DATE('") + strData + _T("','YYYY/MM/DD HH24:MI:SS')");
				}
				else
				{
					// error
					return -1;
				}
			}
		}
		strQuery += _T(");");
		//pColumnsDM->QueryLog(strQuery.GetBuffer(0));
		TranDM.AddFrame(strQuery);

		// 3.2 DELETE
		strQuery.Format(_T("DELETE FROM %s WHERE REC_KEY=%s;"), strTableName, strDelRecKey);
		//pColumnsDM->QueryLog(strQuery.GetBuffer(0));
		TranDM.AddFrame(strQuery);
	}
	
	return 0;
}

INT CUnityUserManagerDlg::MoveObjectUser(CESL_DataMgr &TranDM, CString strUnityRecKey, CString strObjectRecKey)
{
	// 1. ����̿��� ���� ��������
	CESL_DataMgr *pObjectDM = FindDM(_T("DM_UUM_OBJECT_USER"));
	if(NULL == pObjectDM)
	{
		AfxMessageBox(_T("FindDM Error : DM_UUM_OBJECT_USER"));
		return 0;
	}
	CString strUserNO, strUserName, strCivilNO;
	pObjectDM->GetCellData(_T("�����ڹ�ȣ"), 0, strUserNO);
	pObjectDM->GetCellData(_T("�̸�"), 0, strUserName);
	pObjectDM->GetCellData(_T("�ֹε�Ϲ�ȣ"), 0, strCivilNO);
	// 2. CO_LOAN_USER_DEL_TBL�� INSERT
	CString strQuery;
	strQuery.Format(
		_T("INSERT INTO CO_LOAN_USER_DEL_TBL ")
		_T("(REC_KEY,WORKER_IP,WORKER_ID,WORKER_NAME,USER_KEY,USER_NO,USER_NAME,CIVIL_NO,WORK_DATE,FIRST_WORK) ")
		_T("VALUES (ESL_SEQ.NEXTVAL,'%s','%s','%s',%s,'%s','%s','%s',SYSDATE,TO_CHAR(SYSDATE,'YYYY/MM/DD HH24:MI:SS')||' �̿��������� #'||'%s');")
		, GetIP(), m_strID, m_strName, strObjectRecKey, strUserNO, strUserName, strCivilNO, strUnityRecKey);
	//pObjectDM->QueryLog(strQuery.GetBuffer(0));
	TranDM.AddFrame(strQuery);
	// 3. ����̿��� ����
	strQuery.Format(_T("DELETE FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s;"), strObjectRecKey);
	//pObjectDM->QueryLog(strQuery.GetBuffer(0));
	TranDM.AddFrame(strQuery);

	return 0;
}

CString CUnityUserManagerDlg::GetIP()
{
	CString strResult;
	TCHAR szHostName[255];
	HOSTENT *pHostent;
	
	if (gethostname((LPSTR)(LPCTSTR)szHostName, 255) == 0)
	{
		pHostent = gethostbyname((LPSTR)(LPCTSTR)szHostName);
		if (pHostent)
		{
			strResult.Format(_T("%d.%d.%d.%d"), 
							(BYTE)pHostent->h_addr_list[0][0], 
							(BYTE)pHostent->h_addr_list[0][1], 
							(BYTE)pHostent->h_addr_list[0][2], 
							(BYTE)pHostent->h_addr_list[0][3]);
		}		
	}
	
	return strResult;
}

INT CUnityUserManagerDlg::ConvertCodeToDesc()
{
	CESL_DataMgr *pCodeDM = FindDM(_T("DM_UUM_CODE"));
	if(NULL == pCodeDM)
	{
		return -1000;
	}
	CESL_DataMgr *pTargetDM = FindDM(_T("DM_UUM_SEARCH"));
	if(NULL == pTargetDM)
	{
		return -1010;
	}

	CString strManageCode, strCode;
	for(INT i = 0; i < pTargetDM->GetRowCount(); i++) 
	{
		pTargetDM->GetCellData(_T("��������"), i, strManageCode);
		for(INT j = 0; j < pCodeDM->GetRowCount(); j++) 
		{
			pCodeDM->GetCellData(_T("�ڵ�"), j, strCode);
			if (strManageCode == strCode) 
			{
				CString strDesc;
				pCodeDM->GetCellData(_T("����"), j, strDesc);
				pTargetDM->SetCellData(_T("�������м���"), strDesc, i);
				break;
			}
		}
	}

	return 0;
}

INT CUnityUserManagerDlg::SearchUser()
{
	CString strWhere;
	GetSearchUserWhere(strWhere);
	if(strWhere.IsEmpty())
	{
		MessageBox(_T("������ �Է��ϰ� �˻��Ͻʽÿ�."), _T("�̿���������"), MB_ICONEXCLAMATION);
		return 0;
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_UUM_SEARCH"));
	if(NULL == pDM)
	{
		AfxMessageBox(_T("FindDM Error : DM_UUM_SEARCH"));
		return -1000;
	}
	pDM->RefreshDataManager(strWhere);
	
// 	if(0 > ConvertCodeToDesc())
// 	{
// 		AfxMessageBox(_T("ConvertManageCodeToDescInDM Error"));
// 		return -1010;
// 	}

	CESL_ControlMgr *pCM = FindCM(_T("CM_UUM_SEARCH"));
	if(NULL == pCM)
	{
		AfxMessageBox(_T("FindCM Error : CM_UUM_SEARCH"));
		return -1000;
	}
	pCM->ControlDisplay(_T("Grid"));

	return 0;
}

INT CUnityUserManagerDlg::GetSearchUserWhere(CString &strWhere)
{
	CString strQuery;
	CString strCivilNO;
	GetDlgItemText(IDC_edtSEARCH_CivilNO, strCivilNO);
	strCivilNO.TrimLeft(); strCivilNO.TrimRight();
	strCivilNO.Replace(_T("'"), _T(""));
	if(!strCivilNO.IsEmpty())
	{
		if(13 == strCivilNO.GetLength())
		{
			CString strTemp;
			strTemp = strCivilNO;
			strCivilNO = strTemp.Left(6) + _T("-") + strTemp.Right(7);
			SetDlgItemText(IDC_edtSEARCH_CivilNO, strCivilNO);
		}
		if(!strWhere.IsEmpty())
		{
			strWhere += _T(" AND ");
		}
		strQuery.Format(_T("CIVIL_NO='%s'"), strCivilNO);
		strWhere += strQuery;
	}
	CString strUserNO;
	GetDlgItemText(IDC_edtSEARCH_UserNO, strUserNO);
	strUserNO.TrimRight(); strUserNO.TrimLeft();
	strUserNO.Replace(_T("'"), _T(""));
	if(!strUserNO.IsEmpty())
	{
		if(!strWhere.IsEmpty())
		{
			strWhere += _T(" AND ");
		}
		strQuery.Format(_T("USER_NO='%s'"), strUserNO);
		strWhere += strQuery;
	}
	CString strName;
	GetDlgItemText(IDC_edtSEARCH_Name, strName);
	strName.TrimRight(); strName.TrimLeft();
	strName.Replace(_T("'"), _T(""));
	if(!strName.IsEmpty())
	{
		if(!strWhere.IsEmpty())
		{
			strWhere += _T(" AND ");
		}
		strQuery.Format(_T("IDX_USER_NAME='%s'"), strName);
		strWhere += strQuery;
	}
	CString strUserID;
	GetDlgItemText(IDC_edtSEARCH_UserID, strUserID);
	strUserID.TrimLeft(); strUserID.TrimRight();
	strUserID.Replace(_T("'"), _T(""));
	if(!strUserID.IsEmpty())
	{
		if(!strWhere.IsEmpty())
		{
			strWhere += _T(" AND ");
		}
		strQuery.Format(_T("USER_ID='%s'"), strUserID);
		strWhere += strQuery;
	}
	CString strHandPhone;
	GetDlgItemText(IDC_edtSEARCH_HandPhone, strHandPhone);
	strHandPhone.TrimRight(); strHandPhone.TrimLeft();
	strHandPhone.Replace(_T("-"), _T(""));
	strHandPhone.Replace(_T("'"), _T(""));
	if(!strHandPhone.IsEmpty())
	{
		if(!strWhere.IsEmpty())
		{
			strWhere += _T(" AND ");
		}
		strQuery.Format(_T("IDX_HANDPHONE='%s'"), strHandPhone);
		strWhere += strQuery;
	}

	return 0;
}

INT CUnityUserManagerDlg::Clear()
{
	SetDlgItemText(IDC_edtSEARCH_Name, _T(""));
	SetDlgItemText(IDC_edtSEARCH_UserNO, _T(""));
	SetDlgItemText(IDC_edtSEARCH_HandPhone, _T(""));
	SetDlgItemText(IDC_edtSEARCH_UserID, _T(""));
	SetDlgItemText(IDC_edtSEARCH_CivilNO, _T(""));

	CESL_DataMgr *pDM = FindDM(_T("DM_UUM_SEARCH"));
	pDM->FreeData();
	SetUnityUser(_T(""));
	SetObjectUser(_T(""));

	ControlDisplay(_T("CM_UUM_SEARCH"), _T("Grid"));

	return 0;
}

INT CUnityUserManagerDlg::GetGridIdx()
{
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_gridUserList);
	if(NULL == pGrid)
	{
		AfxMessageBox(_T("Grid Find Error : CM_UUM_SEARCH(Grid)"));
		return -1;
	}
	pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;

	return pGrid->GetIdx();
}

INT CUnityUserManagerDlg::GetGridRecKey(CString &strRecKey)
{
	INT idx = GetGridIdx();
	if(0 > idx)
	{
		return -1;
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_UUM_SEARCH"));
	if(NULL == pDM)
	{
		AfxMessageBox(_T("FindDM Error : DM_UUM_SEARCH"));
		return -1;
	}

	pDM->GetCellData(_T("REC_KEY"), idx, strRecKey);

	return 0;
}

INT CUnityUserManagerDlg::AddUnityUser()
{
	INT ids;
	CString strRecKey;
	ids = GetGridRecKey(strRecKey);	
	if(0 > ids || strRecKey.IsEmpty())
	{
		MessageBox(_T("�̿��ڸ� �˻��Ͽ� ���ϴ� �̿��ڸ� �����Ͽ� �ֽʽÿ�."), _T("�̿���������"), MB_ICONEXCLAMATION);
		return 0;
	}

	CESL_DataMgr *pObjectDM = FindDM(_T("DM_UUM_OBJECT_USER"));
	if(NULL == pObjectDM)
	{
		AfxMessageBox(_T("FindDM Error : DM_UUM_OBJECT_USER"));
		return 0;
	}
	CString strObjectRecKey;
	pObjectDM->GetCellData(_T("REC_KEY"), 0, strObjectRecKey);
	if(strObjectRecKey == strRecKey)
	{
		MessageBox(_T("��� �̿��ڿ� �ٸ� �̿��ڸ� ������ �ֽʽÿ�."), _T("�̿���������"), MB_ICONEXCLAMATION);
		return 0;
	}

	SetUnityUser(strRecKey);

	return 0;
}

INT CUnityUserManagerDlg::SetUnityUser(CString strRecKey)
{
	CESL_DataMgr *pUnityDM = FindDM(_T("DM_UUM_UNITY_USER"));
	if(NULL == pUnityDM)
	{
		AfxMessageBox(_T("FindDM Error : DM_UUM_UNITY_USER"));
		return 0;
	}

	if(strRecKey.IsEmpty())
	{
		pUnityDM->FreeData();
		SetDlgItemText(IDC_edtUNITY_Name, _T(""));
		SetDlgItemText(IDC_edtUNITY_CivilNO, _T(""));
		SetDlgItemText(IDC_edtUNITY_UserNO, _T(""));
		SetDlgItemText(IDC_edtUNITY_UserID, _T(""));
		SetDlgItemText(IDC_edtUNITY_HomePhone, _T(""));
		SetDlgItemText(IDC_edtUNITY_HandPhone, _T(""));
		SetDlgItemText(IDC_edtUNITY_Status, _T(""));
		SetDlgItemText(IDC_edtUNITY_History, _T(""));
		SetDlgItemText(IDC_edtUNITY_LastEditDate, _T(""));
	}
	else
	{	
		CString strWhere;
		strWhere.Format(_T("CLU.REC_KEY=%s"), strRecKey);
		pUnityDM->RefreshDataManager(strWhere);

		SetDlgItemText(IDC_edtUNITY_Name, pUnityDM->GetCellData(_T("�̸�"), 0));
		SetDlgItemText(IDC_edtUNITY_CivilNO, pUnityDM->GetCellData(_T("�ֹε�Ϲ�ȣ"), 0));
		SetDlgItemText(IDC_edtUNITY_UserNO, pUnityDM->GetCellData(_T("�����ڹ�ȣ"), 0));
		SetDlgItemText(IDC_edtUNITY_UserID, pUnityDM->GetCellData(_T("�����ID"), 0));
		SetDlgItemText(IDC_edtUNITY_HomePhone, pUnityDM->GetCellData(_T("��ȭ��ȣ"), 0));
		SetDlgItemText(IDC_edtUNITY_HandPhone, pUnityDM->GetCellData(_T("�ڵ���"), 0));
		SetDlgItemText(IDC_edtUNITY_Status, pUnityDM->GetCellData(_T("��Ȳ�Ǽ�"), 0));
		SetDlgItemText(IDC_edtUNITY_History, pUnityDM->GetCellData(_T("�̷°Ǽ�"), 0));
		SetDlgItemText(IDC_edtUNITY_LastEditDate, pUnityDM->GetCellData(_T("����������"), 0));
	}

	return 0;
}

INT CUnityUserManagerDlg::AddObjectUser()
{
	INT ids;
	CString strRecKey;
	ids = GetGridRecKey(strRecKey);	
	if(0 > ids || strRecKey.IsEmpty())
	{
		MessageBox(_T("�̿��ڸ� �˻��Ͽ� ���ϴ� �̿��ڸ� �����Ͽ� �ֽʽÿ�."), _T("�̿���������"), MB_ICONEXCLAMATION);
		return 0;
	}

	CESL_DataMgr *pUnityDM = FindDM(_T("DM_UUM_UNITY_USER"));
	if(NULL == pUnityDM)
	{
		AfxMessageBox(_T("FindDM Error : DM_UUM_UNITY_USER"));
		return 0;
	}
	CString strUnityRecKey;
	pUnityDM->GetCellData(_T("REC_KEY"), 0, strUnityRecKey);
	if(strUnityRecKey == strRecKey)
	{
		MessageBox(_T("���� �̿��ڿ� �ٸ� �̿��ڸ� ������ �ֽʽÿ�."), _T("�̿���������"), MB_ICONEXCLAMATION);
		return 0;
	}

	SetObjectUser(strRecKey);

	return 0;
}

INT CUnityUserManagerDlg::SetObjectUser(CString strRecKey)
{
	CESL_DataMgr *pObjectDM = FindDM(_T("DM_UUM_OBJECT_USER"));
	if(NULL == pObjectDM)
	{
		AfxMessageBox(_T("FindDM Error : DM_UUM_OBJECT_USER"));
		return 0;
	}
	
	if(strRecKey.IsEmpty())
	{
		pObjectDM->FreeData();
		SetDlgItemText(IDC_edtOBJECT_Name, _T(""));
		SetDlgItemText(IDC_edtOBJECT_CivilNO, _T(""));
		SetDlgItemText(IDC_edtOBJECT_UserNO, _T(""));
		SetDlgItemText(IDC_edtOBJECT_UserID, _T(""));
		SetDlgItemText(IDC_edtOBJECT_HomePhone, _T(""));
		SetDlgItemText(IDC_edtOBJECT_HandPhone, _T(""));
		SetDlgItemText(IDC_edtOBJECT_Status, _T(""));
		SetDlgItemText(IDC_edtOBJECT_History, _T(""));
		SetDlgItemText(IDC_edtOBJECT_LastEditDate, _T(""));
	}
	else
	{
		CString strWhere;
		strWhere.Format(_T("CLU.REC_KEY=%s"), strRecKey);
		pObjectDM->RefreshDataManager(strWhere);

		SetDlgItemText(IDC_edtOBJECT_Name, pObjectDM->GetCellData(_T("�̸�"), 0));
		SetDlgItemText(IDC_edtOBJECT_CivilNO, pObjectDM->GetCellData(_T("�ֹε�Ϲ�ȣ"), 0));
		SetDlgItemText(IDC_edtOBJECT_UserNO, pObjectDM->GetCellData(_T("�����ڹ�ȣ"), 0));
		SetDlgItemText(IDC_edtOBJECT_UserID, pObjectDM->GetCellData(_T("�����ID"), 0));
		SetDlgItemText(IDC_edtOBJECT_HomePhone, pObjectDM->GetCellData(_T("��ȭ��ȣ"), 0));
		SetDlgItemText(IDC_edtOBJECT_HandPhone, pObjectDM->GetCellData(_T("�ڵ���"), 0));
		SetDlgItemText(IDC_edtOBJECT_Status, pObjectDM->GetCellData(_T("��Ȳ�Ǽ�"), 0));
		SetDlgItemText(IDC_edtOBJECT_History, pObjectDM->GetCellData(_T("�̷°Ǽ�"), 0));
		SetDlgItemText(IDC_edtOBJECT_LastEditDate, pObjectDM->GetCellData(_T("����������"), 0));
	}

	return 0;
}

BOOL CUnityUserManagerDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
 	{
		if(VK_RETURN == pMsg->wParam)
		{
			if(GetDlgItem(IDC_edtSEARCH_Name)->GetSafeHwnd() == pMsg->hwnd)
			{
				SearchUser();
			}
			else if(GetDlgItem(IDC_edtSEARCH_UserNO)->GetSafeHwnd() == pMsg->hwnd)
			{
				SearchUser();
			}
			else if(GetDlgItem(IDC_edtSEARCH_HandPhone)->GetSafeHwnd() == pMsg->hwnd)
			{
				SearchUser();
			}
			else if(GetDlgItem(IDC_edtSEARCH_UserID)->GetSafeHwnd() == pMsg->hwnd)
			{
				SearchUser();
			}
			else if(GetDlgItem(IDC_edtSEARCH_CivilNO)->GetSafeHwnd() == pMsg->hwnd)
			{
				SearchUser();
			}
			return TRUE;
		}
		else if(VK_ESCAPE == pMsg->wParam)
		{
			Clear();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CUnityUserManagerDlg::OnSetfocusedtSEARCHName() 
{
	SetIMEStatus(this, _T("�ѱ�"), IDC_edtSEARCH_Name);
}

void CUnityUserManagerDlg::OnSetfocusedtSEARCHUserID() 
{
	SetIMEStatus(this, _T("����"), IDC_edtSEARCH_UserID);
}

void CUnityUserManagerDlg::OnSetfocusedtSEARCHUserNO() 
{
	SetIMEStatus(this, _T("����"), IDC_edtSEARCH_UserNO);
}
