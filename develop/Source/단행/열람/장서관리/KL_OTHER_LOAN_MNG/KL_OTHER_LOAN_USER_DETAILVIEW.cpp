// KL_OTHER_LOAN_USER_DETAILVIEW.cpp : implementation file
//

#include "stdafx.h"
#include "KL_OTHER_LOAN_USER_DETAILVIEW.h"
#include "FrameManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_LOAN_USER_DETAILVIEW dialog


CKL_OTHER_LOAN_USER_DETAILVIEW::CKL_OTHER_LOAN_USER_DETAILVIEW(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CKL_OTHER_LOAN_USER_DETAILVIEW::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKL_OTHER_LOAN_USER_DETAILVIEW)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nCur = 0;
	m_nTotalCnt = 0;

	m_pGrid = NULL;
	m_pDM = NULL;

	m_strSMAlias = _T("SM_KL_OTHER_LOAN_USER_DETAILVIEW");
	m_strCMAlias = _T("CM_KL_OTHER_LOAN_USER_DETAILVIEW");
	m_strDMAlias = _T("DM_KL_OTHER_LOAN_USER_DETAILVIEW");

	// KOL.UDF.022
	m_pParentGrid = NULL;
	m_pParentDM = NULL;
}


void CKL_OTHER_LOAN_USER_DETAILVIEW::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_OTHER_LOAN_USER_DETAILVIEW)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_OTHER_LOAN_USER_DETAILVIEW, CDialog)
	//{{AFX_MSG_MAP(CKL_OTHER_LOAN_USER_DETAILVIEW)
	ON_BN_CLICKED(IDC_BUTTON_PRE, OnButtonPre)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_LOAN_USER_DETAILVIEW message handlers

void CKL_OTHER_LOAN_USER_DETAILVIEW::OnButtonPre() 
{
	// TODO: Add your control notification handler code here
	--m_nCur;

	if(m_nCur < 0)
	{
		m_nCur = 0;	
	}
	else
	{
		GetData();
		RefreshControl();
	}	
}

void CKL_OTHER_LOAN_USER_DETAILVIEW::OnButtonNext() 
{
	// TODO: Add your control notification handler code here
	++m_nCur;
	
	if(m_nTotalCnt <= m_nCur)
	{
		m_nCur = m_nTotalCnt;
	}
	else
	{
		GetData();
		RefreshControl();
	}	
}

void CKL_OTHER_LOAN_USER_DETAILVIEW::OnButtonClose() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();	
}

BOOL CKL_OTHER_LOAN_USER_DETAILVIEW::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	EnableThemeDialogTexture(GetSafeHwnd());

	if(!m_strSMAlias.IsEmpty())
	{
		InitESL_Mgr(m_strSMAlias);

		m_pDM = FindDM(m_strDMAlias);
		m_pGrid = (CESL_Grid*)FindControl(m_strCMAlias, _T("GRID"));	
	}

	//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰�
	//-------------------------------------------------------------------------
	if(!m_pInfo->m_bUserInfoExportYN)
	{
		m_pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		m_pGrid->m_bPrivacyPrintLogRecord = TRUE;
		m_pGrid->m_strPrintInfo = _T("Ÿ��������� �̿��ڻ󼼺���");
	}
	//-------------------------------------------------------------------------
	
	RefreshData();
	RefreshControl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKL_OTHER_LOAN_USER_DETAILVIEW::GetData()
{
	if(m_nCur < 0)
	{
		m_pDM->FreeData();
		return;
	}
		
	m_pParentGrid->SelectGetHeadPosition();
		
	for(int i=0; i<m_nCur; i++)
	{
		m_pParentGrid->SelectGetNext();						
	}
	
	CString strData;

	INT ids=0;	
	CString strMessage;
	CString strName;
	CString strUserNo;
	CString strEmail;
	CString strPhone;
	CString strHandPhone;
	CString strMemberClass;
	CString strJoinLib = _T("");
	CString strQuery;
	m_pParentDM->GetCellData(_T("�̿��ڹ�ȣ"), m_pParentGrid->SelectGetIdx(), strUserNo);

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));
		
	strQuery.Format(_T("SELECT NAME,USER_NO,E_MAIL,H_PHONE,HANDPHONE,MEMBER_CLASS FROM CO_LOAN_USER_TBL WHERE USER_NO = '%s'"), strUserNo);
			
	TmpDM.RestructDataManager(strQuery);
	TmpDM.GetCellData(0,0,strName);
	TmpDM.GetCellData(0,1,strUserNo);
	TmpDM.GetCellData(0,2,strEmail);
	TmpDM.GetCellData(0,3,strPhone);
	TmpDM.GetCellData(0,4,strHandPhone);
	TmpDM.GetCellData(0,5,strMemberClass);

	if( strMemberClass.Compare(_T("0")) == 0)
	{
		strMemberClass = _T("����");
	}
	else if( strMemberClass.Compare(_T("1")) == 0)
	{
		strMemberClass = _T("����");
	}
	else if( strMemberClass.Compare(_T("2")) == 0)
	{
		strMemberClass = _T("����");
	}
	else if( strMemberClass.Compare(_T("3")) == 0)
	{
		strMemberClass = _T("Ż��");
	}

	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K48_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("���մ����ڹ�ȣ"), strUserNo);
	Writer.addElement(_T("G-PINHASH�����ڵ�"), _T(""));
	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 <= ids)
	{
		TmpDM.FreeData();
		Reader.makeDM(&TmpDM);	
		
		CString strLibCode;
		for(i = 0; i < TmpDM.GetRowCount(); i++)
		{
			TmpDM.GetCellData(_T("��������"), i, strLibCode);
			if(i != 0)
				strJoinLib += _T(",");

			strJoinLib += strLibCode;
		}
	}

	m_pDM->FreeData();
	m_pDM->InsertRow(-1);
	m_pDM->SetCellData(_T("����"),_T("�̿��ڸ�"), 0);
	m_pDM->SetCellData(_T("����"),strName, 0);	
	m_pDM->InsertRow(-1);
	m_pDM->SetCellData(_T("����"),_T("�����ڹ�ȣ"), 1);
	m_pDM->SetCellData(_T("����"),strUserNo, 1);
	m_pDM->InsertRow(-1);
	m_pDM->SetCellData(_T("����"),_T("�̸���"), 2);
	m_pDM->SetCellData(_T("����"),strEmail, 2);
	m_pDM->InsertRow(-1);
	m_pDM->SetCellData(_T("����"),_T("��ȭ��ȣ"), 3);
	m_pDM->SetCellData(_T("����"),strPhone, 3);
	m_pDM->InsertRow(-1);
	m_pDM->SetCellData(_T("����"),_T("�޴���"), 4);
	m_pDM->SetCellData(_T("����"),strHandPhone, 4);
	m_pDM->InsertRow(-1);
	m_pDM->SetCellData(_T("����"),_T("ȸ������"), 5);
	m_pDM->SetCellData(_T("����"),strMemberClass, 5);
	m_pDM->InsertRow(-1);
	m_pDM->SetCellData(_T("����"),_T("���Ե�����"), 6);
	m_pDM->SetCellData(_T("����"),strJoinLib, 6);
		
	m_pGrid->Display();				
}

void CKL_OTHER_LOAN_USER_DETAILVIEW::RefreshData()
{
	m_nCur = 0;	
	m_pParentGrid->SelectMakeList();
	m_nTotalCnt = m_pParentGrid->SelectGetCount();
	m_pParentGrid->SelectGetHeadPosition();

	GetData();
}

void CKL_OTHER_LOAN_USER_DETAILVIEW::RefreshControl()
{	
	if(m_nCur <= 0)
	{
		GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(FALSE);
	}
	else
	{ 
		GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(TRUE);
	}

	if(m_nTotalCnt - 1 <= m_nCur)
	{
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);		
	}
}