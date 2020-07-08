// LOG_SEND_MNG_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "LOG_SEND_MNG_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLOG_SEND_MNG_DLG dialog


CLOG_SEND_MNG_DLG::CLOG_SEND_MNG_DLG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CLOG_SEND_MNG_DLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLOG_SEND_MNG_DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CLOG_SEND_MNG_DLG::~CLOG_SEND_MNG_DLG()
{

}

void CLOG_SEND_MNG_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLOG_SEND_MNG_DLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLOG_SEND_MNG_DLG, CDialog)
	//{{AFX_MSG_MAP(CLOG_SEND_MNG_DLG)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLOG_SEND_MNG_DLG message handlers

BOOL CLOG_SEND_MNG_DLG::Create(CWnd* pParent) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParent);
}

BOOL CLOG_SEND_MNG_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	
	EnableThemeDialogTexture(GetSafeHwnd());
	

	CString fileName, filePath;
	CFileFind finder;
	int rowIdx=0;
	BOOL bWorking = finder.FindFile(_T("..\\tunning\\*.tunning"));



	if(0>InitESL_Mgr(_T("SM_�α�����")))
	{
		return FALSE;
	}
	
	CESL_DataMgr *pDM=FindDM(_T("DM_�α�����"));
		
		
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		fileName = finder.GetFileName();
		filePath = finder.GetFilePath();

		pDM->InsertRow(-1);
		pDM->SetCellData(_T("���ϸ�"),fileName,rowIdx);
		pDM->SetCellData(_T("���ϰ��"),filePath,rowIdx);
		rowIdx++;
	}
	finder.Close();

	CESL_ControlMgr *pCM=FindCM(_T("CM_�α�����"));
	pCM->ControlDisplay(_T("MAINGRID"));


	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLOG_SEND_MNG_DLG::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);
	
	if(pGrid->GetSafeHwnd() == NULL) {
		return;
	}

	pGrid->MoveWindow(0, 0, cx, cy);
}

//GRID ���ΰ�ħ
BOOL CLOG_SEND_MNG_DLG::OnRefreshGrid() 
{
	CESL_DataMgr *pDM=FindDM(_T("DM_�α�����"));
	pDM->FreeData();

	INT rowIdx=0;
	CString fileName, filePath;
	CFileFind findFile;
	BOOL bWorking = findFile.FindFile(_T("..\\tunning\\*.tunning"));

	while (bWorking)
	{
		bWorking = findFile.FindNextFile();
		fileName = findFile.GetFileName();
		filePath = findFile.GetFilePath();

		pDM->InsertRow(-1);
		pDM->SetCellData(_T("���ϸ�"),fileName,rowIdx);
		pDM->SetCellData(_T("���ϰ��"),filePath,rowIdx);
		rowIdx++;
	}
	findFile.Close();

	CESL_ControlMgr *pCM=FindCM(_T("CM_�α�����"));
	pCM->ControlDisplay(_T("MAINGRID"));

	return TRUE;
}

BOOL CLOG_SEND_MNG_DLG::SendLog()
{

	INT ids;
	INT nRowCount, nIdx;
	CString fileName, filePath;
	CString sDMAlias = _T("DM_�α�����");

	
	CESL_DataMgr *pDM=FindDM(_T("DM_�α�����"));
	CESL_ControlMgr *pCM=FindCM(_T("CM_�α�����"));
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAINGRID"));
	ids = pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	nRowCount=pGrid->SelectGetCount();

	if(1>nRowCount)
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�"));
		return FALSE;
	}

	//�ش� pc ip�� �������� �κ�
	WSADATA wsadata;
	CString strIP;
		if( !WSAStartup( DESIRED_WINSOCK_VERSION, &wsadata))
	{
		if(wsadata.wVersion >= MINIMUM_WINSOCK_VERSION)
		{
			
			HOSTENT *p_host_info;
			IN_ADDR in;
			char host_name[128]={0,};
				gethostname(host_name, 128);
			p_host_info = gethostbyname(host_name);
				if(p_host_info !=NULL)
			{
				for(int q=0; p_host_info->h_addr_list[q];q++)
				{
					memcpy( &in, p_host_info->h_addr_list[q], 4);
					strIP = inet_ntoa(in);
				}
			}
		}
		WSACleanup();
	}

	//id�� �������� �κ�
	//CString userID = GetUserID();


	for(int i=0;i<nRowCount;i++)
	{
		nIdx = pGrid->SelectGetIdx();
		fileName = pDM->GetCellData(_T("���ϸ�"),nIdx);
		filePath = pDM->GetCellData(_T("���ϰ��"),nIdx);

		//DB�� ������ �����Ѵ� - ������ ���� ���� ũ�⸦ ���Ѵ�
		CStdioFile file;
		
		INT nFileSize = 0;
		if (!file.Open(_T("..\\tunning\\")+fileName, CFile::modeRead | CFile::typeBinary))
		{
			MessageBox(_T("������ ���µ� �����߽��ϴ�."));
			return FALSE;
		}
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
		
		nFileSize = file.GetLength();
		BYTE *pBuffer = (BYTE*)malloc(sizeof(BYTE) * (nFileSize) + 1024);
		ZeroMemory(pBuffer, nFileSize);
		nFileSize = file.ReadHuge(pBuffer, nFileSize);
		file.Close();
	
		CString query;
		CString REC_KEY;
		CESL_DataMgr dm;

	
		ids = pDM->StartFrame();	
		pDM->MakeRecKey(REC_KEY, _T("ESL_SEQ"));
		INT REC_KEY_NUM = _ttoi(REC_KEY);
		
		//�ش� LOB ������ ���� ROW�� ����
		CString strQuery;
		strQuery.Format(_T("INSERT INTO KO_LOG_TBL VALUES(%d,'%s','%s',SYSDATE,'%s',%d,'');"),REC_KEY_NUM,m_pInfo->USER_ID,strIP, fileName, nFileSize);
		pDM->AddFrame(strQuery);
		ids = pDM->SendFrame();
		if (ids < 0) 
		{
			MessageBox(_T("���̺� ���� ERROR"));
			return FALSE;
		}
		ids = pDM->EndFrame();
		if (ids < 0) 
		{
			MessageBox(_T("���̺� ���� ERROR"));
			return FALSE;
		}

		//������ ROW�� LOB ������ �Է�
		ids = pDM->LOBUpdate(_T("KO_LOG_TBL"), _T("REC_KEY"), _T("NUMBER"), REC_KEY, _T("FILE_DATA"), pBuffer, nFileSize);
		if (ids < 0)
		{
			MessageBox(_T("LOB Update ����"));
			return FALSE;
		}
		//�޸� ����
		free(pBuffer);
		pGrid->SelectGetNext();
	}
	AfxMessageBox(_T("���� �Ϸ�"));

	return TRUE;


 }