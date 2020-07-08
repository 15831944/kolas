// Lill_Panalty.cpp : implementation file
//

#include "stdafx.h"
#include "Lill_Panalty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLill_Panalty dialog


CLill_Panalty::CLill_Panalty(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CLill_Panalty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLill_Panalty)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLill_Panalty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLill_Panalty)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLill_Panalty, CDialog)
	//{{AFX_MSG_MAP(CLill_Panalty)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	ON_BN_CLICKED(IDC_BTN_PanaltyReplace, OnBTNPanaltyReplace)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLill_Panalty message handlers

BOOL CLill_Panalty::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(InitESL_Mgr(_T("SM_LILL_PANALTY")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	GetPanaltyData();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CLill_Panalty::GetPanaltyData()
{
	INT ids = 0;
	CString sPanaltyExpireDate	 = _T(""), 
			sNoLoanCount		 = _T(""),
			sUserNo				 = _T(""),
			sMsgLog				 = _T("");

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pDM) return -1;

	CESL_DataMgr *pLillDM = FindDM(_T("DM_LILL_PANALTY"));
	if(NULL == pLillDM) return -2;
	
	pLillDM->FreeData();
	pLillDM->InsertRow(-1);

	pDM->GetCellData(_T("�����ڹ�ȣ"), 0, sUserNo);
	
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("LILL_K15_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("���մ����ڹ�ȣ"), sUserNo);
		
	CFrameSender Sender(m_pInfo);	
	CFrameReader Reader; 
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if (0 > ids) 
	{
		while(true) 
		{
			if(IDYES == MessageBox(_T("å�������� �̵������� ��ſ� �����Ͽ����ϴ�.\r\n��õ� �Ͻðڽ��ϱ�?"), _T("��ȸ����"), MB_YESNO | MB_ICONQUESTION)) 
			{
				ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
				if(0 <= ids) continue;
			}else{
				return -3;
			}
		}
	}

	CESL_DataMgr TmpDM;
	Reader.makeDM(&TmpDM);
 	CString strTemp = Reader.getCommand();

	if(1 > TmpDM.GetRowCount())
	{
		return -200;
	}		

	sNoLoanCount = TmpDM.GetCellData(_T("������ȣ�����̴���Ƚ��"), 0);
	sPanaltyExpireDate = TmpDM.GetCellData(_T("������ȣ������û������"), 0);

	pLillDM->SetCellData(_T("���մ����ڹ�ȣ"), sUserNo, 0);
	pLillDM->SetCellData(_T("�̴���Ƚ��"), sNoLoanCount, 0);
	pLillDM->SetCellData(_T("��û������"), sPanaltyExpireDate, 0);

	sMsgLog.Format(_T("�� �̿��� �г�Ƽ ��ȸ - ���մ����ڹ�ȣ:%s / ������ȣ�����̴���Ƚ��:%s / ������ȣ������û������:%s ��"), sUserNo, sNoLoanCount, sPanaltyExpireDate);
	pDM->QueryLog(sMsgLog.GetBuffer(0));
	
	AllControlDisplay(_T("CM_LILL_PANALTY"),0);

	return 0;
}

void CLill_Panalty::OnBtnClose() 
{
	OnCancel();
}

void CLill_Panalty::OnBTNPanaltyReplace() 
{
	INT ids = 0;
	CString sPanaltyExpireDate	 = _T(""), 
			sNoLoanCount		 = _T(""),
			sUserNo				 = _T(""),
			sMsgLog				 = _T("");

	ids = GetControlData(_T("CM_LILL_PANALTY"), _T("������ȣ�����̴���Ƚ��"), sNoLoanCount);
	if(0 > ids) ERROR_MSG_RETURN_VOID2(-1, _T("OnBTNPanaltyReplace"));
	if(sNoLoanCount.IsEmpty() == TRUE)
		sNoLoanCount = _T("0");

	ids = GetControlData(_T("CM_LILL_PANALTY"), _T("������ȣ������û������"), sPanaltyExpireDate);
	if(0 > ids) ERROR_MSG_RETURN_VOID2(-2, _T("OnBTNPanaltyReplace"));

	if(sPanaltyExpireDate.GetLength()!=10){
		AfxMessageBox(_T("��/��/�� �� ����� �Է����ּ���."));
		return;
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pDM) return;

	pDM->GetCellData(_T("�����ڹ�ȣ"), 0, sUserNo);
	
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("LILL_K15_LOC_B02_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("���մ����ڹ�ȣ"), sUserNo);
	Writer.addElement(_T("������ȣ�����̴���Ƚ��"), sNoLoanCount);
	Writer.addElement(_T("������ȣ������û������"), sPanaltyExpireDate);
	
	
	CFrameSender Sender(m_pInfo);	
	CFrameReader Reader; 
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	
	if (0 > ids) 
	{
		while(true) 
		{
			if(IDYES == MessageBox(_T("å�������� �̵������� ��ſ� �����Ͽ����ϴ�.\r\n��õ� �Ͻðڽ��ϱ�?"), _T("��ȸ����"), MB_YESNO | MB_ICONQUESTION)) 
			{
				ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
				if(0 <= ids) continue;
			}else{
				return;
			}
		}
	}
	
	CESL_DataMgr TmpDM;
	Reader.makeDM(&TmpDM);
 	CString strTemp = Reader.getCommand();

	if(strTemp.Compare(_T("ERROR")) == 0){
		CString sErrorID, sErrorMsg, sErrorDetailMsg;
		
		sErrorID		= TmpDM.GetCellData(_T("ID"),0);
		sErrorMsg		= TmpDM.GetCellData(_T("MSG"),0);
		sErrorDetailMsg = TmpDM.GetCellData(_T("ERROR_MSG"),0);

		sMsgLog.Format(_T("ERROR ID : %s / MSG : %s / ERROR_MSG : %s"), sErrorID, sErrorMsg, sErrorDetailMsg);
		pDM->QueryLog(sMsgLog.GetBuffer(0));
		AfxMessageBox(sMsgLog);
		return;
	}
	sMsgLog.Format(_T("�� �̿��� �г�Ƽ ���� - ���մ����ڹ�ȣ:%s / ������ȣ�����̴���Ƚ��:%s / ������ȣ������û������:%s ��"), sUserNo, sNoLoanCount, sPanaltyExpireDate);
	pDM->QueryLog(sMsgLog.GetBuffer(0));

	AfxMessageBox(_T("�����Ǿ����ϴ�."));
}
