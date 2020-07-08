// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KOLAS3_MAIN.h"
#include "LoginDlg.h"
#include "efs.h"
#include "KOLAS3_MAINview.h"

//2004-05-08 ������ �߰� 
#include "..\\MENU_TOOLBAR_MGR\\MenuToolbar_Mgr.h"

#include "Imm.h"
#include "..\\ESL\\FWDES.h"

// 2012.10.06 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2ȸ(����� ��й�ȣ ��ȣȭ)
#include "..\\..\\..\\����\\����\\�弭����\\LOC_CommonAPI\\LocCommonAPI.h"


// 2013.10.08 ADD BY BJS : 2013���������ռ���Ȯ�뱸��װ��� (�Ϲ�ġ �α׻���)
#include "..\\..\\..\\����\\����\\�弭����\\LOC_CommonAPI\\CfgFileApi.h"

// << 2015�� ���������� �ý��� ���� 2015/11/11 _LCM : GPKI ������ �α���
#include "..\\..\\..\\����\\GPKI\\GPKILoginDlg.h"
#include "NetworkInfoMgr.h"
// >> 2015/11/11 --ADD

// KOL.RED.2018.228 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "ESL_SearchEngine.h"
// KOL.RED.2018.228 ---------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <Winsock2.h>

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginDlg)
	m_strID = _T("");
	m_strPassword = _T("");
	m_bUserLoginMode = FALSE;
	//}}AFX_DATA_INIT

	m_pParentWnd = pParent;
	m_nFailcount = 0;
	m_pInfo = NULL;

	//2015�� ���������� �ý��� ���� 2015/11/23 _LCM : GPKI ������ �α��� ����
	m_bGPKI = FALSE;
	
	#ifdef _ECOLAS_S
		m_bmpLogin.LoadBMP(_T("..\\bmp\\MainImage\\ECOLAS-S\\Main_LOGIN.bmp"));
	#endif
	#ifdef _ECOLAS
		m_bmpLogin.LoadBMP(_T("..\\bmp\\MainImage\\ECOLAS\\LOGIN.bmp"));
	#endif
	#ifdef _KOLASIII
		m_bmpLogin.LoadBMP(_T("..\\bmp\\Main_LOGIN.bmp"));
	#endif
	#ifdef _KOLASIII_TL
		m_bmpLogin.LoadBMP(_T("..\\bmp\\Main_LOGIN.bmp"));
	#endif
	#ifdef _LIGHT
		m_bmpLogin.LoadBMP(_T("..\\bmp\\Main_LOGIN.bmp"));
	#endif
	
	m_bIsThreadRun = FALSE;
	m_hThread= ::CreateEvent(NULL, FALSE, FALSE, _T("ThreadEvent"));
}

CLoginDlg::~CLoginDlg()
{		
	CloseHandle(m_hThread);
}

VOID CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginDlg)
	DDX_Control(pDX, IDC_PROGRESS_MAIN1, m_ProgressCtrl);
	DDX_Text(pDX, IDC_eID, m_strID);
	DDV_MaxChars(pDX, m_strID, 10);
	DDX_Text(pDX, IDC_ePASSWORD, m_strPassword);

	// 2012.10.05 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2ȸ
	// ��й�ȣ ���� �� ����
	DDV_MaxChars(pDX, m_strPassword, 20);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CLoginDlg)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_EN_SETFOCUS(IDC_eID, OnSetfocuseID)
	// << 2015�� ���������� �ý��� ���� 2015/11/11 _LCM : GPKI ������ �α���
	ON_BN_CLICKED(IDC_BUTTON_GPKI_LOGIN, OnButtonGpkiLogin)
	// >> 2015/11/11 --ADD
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CLOSE_DIALOG, OnCloseDialog)
	ON_MESSAGE(WM_USER + 1004  , BeginLogin )	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg Threads
UINT ThreadFunc_LoginProcess(LPVOID pParam)
{
	CLoginDlg *pDlg = (CLoginDlg*)pParam;
	if (pDlg == NULL) return -1;
	
	pDlg->m_bIsThreadRun = TRUE;

	INT nLower = 0;
	INT nUpper = 9;
	INT nLog = 0;

	CString strText;

	CTextProgressCtrl *pProgressCtrl = &pDlg->m_ProgressCtrl;

	// << 2015�� ���������� �ý��� ���� 2015/11/11 _LCM : IP����
	CESL_DataMgr ipDM;
	CString strIP = _T(""), strQueryIP = _T(""), strUserID = _T("");
	BOOL bVerify = FALSE;
	// >> 2015/11/11 --ADD

	// 17/06/15 ������ : �α��� ���н� �α��� ������ �߰�
//*/ ADD ---------------------------------------------------------------------------	
	INT ids = 0;
	INT nCfgFailCnt = 5;
	INT nLimitTime = 0;	
	CESL_DataMgr TmpDM;
	CString strQuery = _T("");
	CString strTemp = _T("");
	CString strLoginIP = pDlg->GetIP();
	BOOL bLoginRestriction = FALSE;

	TmpDM.SetCONNECTION_INFO(_T(""));			
	strQuery = _T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE='LOGINRESTRICTION'");
	TmpDM.GetOneValueQuery(strQuery, strTemp);
	if(!strTemp.IsEmpty() && strTemp.Compare(_T("Y")) == 0)
	{
		bLoginRestriction = TRUE;
		strQuery = _T("SELECT UTIL_VALUE1, UTIL_VALUE2 FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE='LOGINRESTRICTION'");
		ids = TmpDM.RestructDataManager(strQuery);
		if(ids >= 0)
		{
			nCfgFailCnt = _ttoi(TmpDM.GetCellData(0, 0));
			nLimitTime = _ttoi(TmpDM.GetCellData(0, 1));
		}
	}
//*/ END ---------------------------------------------------------------------------

	pProgressCtrl->SetRange(nLower, nUpper);
	
	// 1. �α����Ѵ�.
	if (!pDlg->m_bIsThreadRun) goto CANCEL;
	strText = _T("�����ͺ��̽��� �����ϴ���...");
	pProgressCtrl->SetWindowText(strText);
	nLog = pDlg->Login();
	if ( nLog < 0)
	{
		strText = _T("���̵�, �н����� Ȯ����...");
		pProgressCtrl->SetWindowText(strText);
		pDlg->m_bIsThreadRun = FALSE;

		// << 2015�� ���������� �ý��� ���� 2015/11/26 _LCM : GPKI�α�������
		if(nLog != -100)
		{
			// 17/06/15 ������ : �α��� ���н� �α��� ������ �߰�
/*//BEFORE-----------------------------------------------------------------------------
			// 2012.10.26 UPDATE BY PWR : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 4ȸ
			// 5�������ϸ� ���α׷��� �����Ѵ�.
			if (++pDlg->m_nFailcount == 5) goto ERR;
	// 		// 3�������ϸ� ���α׷��� �����Ѵ�.
	// 		if (++pDlg->m_nFailcount == 3) goto ERR;
					
			AfxMessageBox(_T("���̵� �Ǵ� ��ȣ�� �߸��Ǿ����ϴ�. ���̵�� ��ȣ�� Ȯ���Ͻʽÿ�."));

			// 2012.10.26 UPDATE BY PWR : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 4ȸ
			strText.Format(_T("ID, PASSWORD�� �Է��ϼ���[��õ�:%d/5]"), pDlg->m_nFailcount);
	// 		strText.Format(_T("ID, PASSWORD�� �Է��ϼ���[��õ�:%d/3]"), pDlg->m_nFailcount);
*///AFTER------------------------------------------------------------------------------						
			INT nFailCnt = ++pDlg->m_nFailcount;					
			if(bLoginRestriction != FALSE)
			{	
				strQuery.Format(_T("SELECT LOGIN_FAIL_COUNT FROM CO_LOGIN_RESTRICTION_TBL WHERE LOGIN_IP='%s' AND ROWNUM <= 1"), strLoginIP);
				ids = TmpDM.GetOneValueQuery(strQuery, strTemp);
				if(!strTemp.IsEmpty())
				{
					nFailCnt = _ttoi(strTemp) + 1;
					
					strQuery.Format(_T("SELECT TO_CHAR(LOGIN_RESTRICTION_DATE,'YYYY/MM/DD HH24:MI:SS'), ")
			        _T("DECODE(LOGIN_RESTRICTION_DATE, NULL , '1', SYSDATE-LOGIN_RESTRICTION_DATE) ")
					_T("FROM CO_LOGIN_RESTRICTION_TBL WHERE LOGIN_IP='%s'")
					,strLoginIP);
					ids = TmpDM.RestructDataManager(strQuery);	
					if(ids >= 0)
					{
						CString strLockTime = TmpDM.GetCellData(0, 0);
						CString strLockValue = TmpDM.GetCellData(0, 1);
						if ( strLockValue.Mid(0,1) == '-' ) 
						{
							strTemp.Format(_T("�α��� �������� %dȸ �̻����� [ %s ]���� ������ܵǾ����ϴ�.\n ���Ŀ� �α��� �����մϴ�.")
										  , nCfgFailCnt, strLockTime);
							AfxMessageBox(strTemp);
							goto ERR;
						}						
						else if(_ttoi(strTemp) >= nCfgFailCnt)
						{
							nFailCnt = 1;
						}
					}

					strQuery.Format(_T("UPDATE CO_LOGIN_RESTRICTION_TBL SET LOGIN_FAIL_DATE=SYSDATE, LOGIN_ID='%s', "
							        _T("LOGIN_FAIL_COUNT=%d WHERE LOGIN_IP='%s';")), pDlg->m_strID, nFailCnt, strLoginIP);
				}
				else
				{
					strQuery.Format(_T("INSERT INTO CO_LOGIN_RESTRICTION_TBL "
							        _T("(REC_KEY, LOGIN_IP, LOGIN_ID, LOGIN_FAIL_DATE, LOGIN_FAIL_COUNT) ")
									_T("VALUES (ESL_SEQ.NEXTVAL, '%s', '%s', SYSDATE, %d);"))
									, strLoginIP, pDlg->m_strID, nFailCnt);
				}
				TmpDM.StartFrame();	
				TmpDM.AddFrame(strQuery);		
				ids = TmpDM.SendFrame();
				TmpDM.EndFrame();

				if (nFailCnt >= nCfgFailCnt)
				{
					TmpDM.StartFrame();	
					// ���ѽð����� ���Ұ����ϰ� �Ѵ�.
					strQuery.Format(_T("UPDATE CO_LOGIN_RESTRICTION_TBL SET LOGIN_RESTRICTION_DATE=SYSDATE+%d/1440 WHERE LOGIN_IP='%s';")
							        , nLimitTime, strLoginIP);
					TmpDM.AddFrame(strQuery);		
					ids = TmpDM.SendFrame();
					TmpDM.EndFrame();
					if(ids >= 0)
					{
						strText.Format(_T("�α��� �������� %dȸ �̻����� ����� ���ܵǾ����ϴ�.\n %d�� �Ŀ� �α��� �����մϴ�.")
					      , nCfgFailCnt, nLimitTime);
					}
					else
					{
						strText = _T("���������������� �����߽��ϴ�.");
					}
					AfxMessageBox(strText);
					goto ERR;
				}							
				
			}
			else
			{
				if (nFailCnt == nCfgFailCnt) goto ERR;				
			}
			AfxMessageBox(_T("���̵� �Ǵ� ��ȣ�� �߸��Ǿ����ϴ�. ���̵�� ��ȣ�� Ȯ���Ͻʽÿ�."));			
			strText.Format(_T("ID, PASSWORD�� �Է��ϼ���[��õ�:%d/%d]"), nFailCnt, nCfgFailCnt);
/*//END-------------------------------------------------------------------------------*/
		}
		else
		{
			strText = _T("��õ� �ٶ��ϴ�.");
		}
		// >> 2015/11/26 --MOD

		pProgressCtrl->SetWindowText(strText);

		((CEdit*)pDlg->GetDlgItem(IDC_eID))->SetReadOnly(FALSE);
		((CEdit*)pDlg->GetDlgItem(IDC_eID))->SetFocus();
		((CEdit*)pDlg->GetDlgItem(IDC_ePASSWORD))->SetReadOnly(FALSE);
		pDlg->m_bIsThreadRun = FALSE;
		return -1;
	}
	else if (nLog > 0)
	{
		// 2009.03.19 ADD BY PDJ : �α��� �Ұ����� ó��
		pDlg->m_bIsThreadRun = FALSE;
		goto ERR;
		return -1;
	}

	// << 2015�� ���������� �ý��� ���� 15/10/14  _LCM : IP ����	
	ipDM.SetCONNECTION_INFO(pDlg->m_pInfo->CONNECTION_INFO);	
	//((CEdit*)pDlg->GetDlgItem(IDC_eID))->GetWindowText(strUserID);
	strUserID = pDlg->m_strID;
	strQueryIP.Format(_T("SELECT SECU_LOGIN_IP FROM MN_USER_TBL WHERE ID = '%s' AND PERMITION = 'U'"), strUserID);
	ipDM.GetOneValueQuery(strQueryIP, strIP);
		
	if(strIP.Compare(_T("")) == 0)
	{
		bVerify = TRUE;
	}
	else if(strIP.Compare(_T("")) != 0)
	{
		CString strTok[3];

		int i = 0;

		for(i=0; i<3; i++)
		{
			AfxExtractSubString(strTok[i], strIP, i, ';');		

			if(pDlg->IsMatchIP(strTok[i]))
			{
				bVerify = TRUE;
				break;
			}			
		}
	}

	if(bVerify == FALSE)
	{
		AfxMessageBox(_T("��ϵ� IP�� �ƴմϴ�.\n\nIP��� �� ���ٶ��ϴ�."),MB_OK);
		goto ERR;
	}
	// >> 2015/10/14 --ADD
	
	// 17/06/15 ������ : �α��� ���н� �α��� ������ �߰�
//*/ ADD ---------------------------------------------------------------------------
	if(bLoginRestriction != FALSE)
	{
		CString strLockTime = _T("");
		CString strLockValue = _T("");
		strQuery.Format(_T("SELECT TO_CHAR(LOGIN_RESTRICTION_DATE,'YYYY/MM/DD HH24:MI:SS'), ")
			            _T("DECODE(LOGIN_RESTRICTION_DATE, NULL , '1', SYSDATE-LOGIN_RESTRICTION_DATE) ")
						_T("FROM CO_LOGIN_RESTRICTION_TBL WHERE LOGIN_IP='%s'")
						,strLoginIP);
		ids = TmpDM.RestructDataManager(strQuery);	
		if(ids >= 0)
		{
			strLockTime = TmpDM.GetCellData(0, 0);
			strLockValue = TmpDM.GetCellData(0, 1);
			if ( strLockValue.Mid(0,1) == '-' ) 
			{				
				strTemp.Format(_T("�α��� �������� %dȸ �̻����� [ %s ]���� ������ܵǾ����ϴ�.\n ���Ŀ� �α��� �����մϴ�.")
					          , nCfgFailCnt, strLockTime);
				AfxMessageBox(strTemp);
				goto ERR;
			}
			strQuery.Format(_T("DELETE CO_LOGIN_RESTRICTION_TBL WHERE LOGIN_IP='%s';"), strLoginIP);
			TmpDM.StartFrame();	
			TmpDM.AddFrame(strQuery);		
			ids = TmpDM.SendFrame();
			TmpDM.EndFrame();
		}
	}
//*/ END ---------------------------------------------------------------------------

	pProgressCtrl->StepIt();

	//2003-1106 CESL_Information ���� �۾�-�ڱ���========================================
	//2003-1106 CESL_Information ���� �۾�-�ڱ���++++++++++++++++++++++++++++++++++++++++	
	// 1.5 �ڵ� ���� �ε��Ѵ�.
	if (!pDlg->m_bIsThreadRun) goto CANCEL;
	strText = _T("�ý������� �ٿ�ε���...");
	pProgressCtrl->SetWindowText(strText);
	if (pDlg->DownloadSystemInfoData() < 0)
		goto ERR;
	
	pProgressCtrl->StepIt();
	//2003-1106 CESL_Information ���� �۾�-�ڱ���----------------------------------------	

	// 2. �ڵ� ���� �ε��Ѵ�.
	if (!pDlg->m_bIsThreadRun) goto CANCEL;
	strText = _T("��ȣ ������ �ٿ�ε���...");
	pProgressCtrl->SetWindowText(strText);
	pDlg->LoadCodeInfo();
	pDlg->m_ProgressCtrl.StepIt();

	// 3. Marc Intellisens �� Reference�� �ε��Ѵ�.
	// 
	//if (!pDlg->m_bIsThreadRun) goto CANCEL;
	//strText = _T("��ũ ������ �ٿ�ε���...");
	//pProgressCtrl->SetWindowText(strText);	
	//if (pDlg->DownloadMarcRelation() < 0) 
	//goto ERR;
	
	pDlg->m_pInfo->InitMarcMgr();
	pProgressCtrl->StepIt();	

	// 4. ���ڼ��� �޴´�.
#ifndef _KOLASIII_TL
	if (!pDlg->m_bIsThreadRun) goto CANCEL;
	strText = _T("���� ������ �ٿ�ε���...");
	pProgressCtrl->SetWindowText(strText);
	if (pDlg->DownloadCharSetData() < 0)
		goto ERR;
#endif
	pProgressCtrl->StepIt();
#ifndef _KOLASIII_TL
	// 5. ���� �����͸� �޴´�.
	if (!pDlg->m_bIsThreadRun) goto CANCEL;
	strText = _T("���� ������ �ٿ�ε���...");
	pProgressCtrl->SetWindowText(strText);
	if (pDlg->DownloadManagementData() < 0)
		goto ERR;
#endif
	pProgressCtrl->StepIt();
#ifndef _KOLASIII_TL
	// 6. ESL REFERENCE�� �޴´�.
	if (!pDlg->m_bIsThreadRun) goto CANCEL;
	strText = _T("���α׷� ������ �ٿ�ε���...");
	pProgressCtrl->SetWindowText(strText);
	//if (pDlg->DownloadESLReference() < 0)
	//	goto ERR;

	// 15/02/26 �ڴ�� : ������� �������� ������ �߻��ϴ� ���� ���� ó�� 
	//                   ������� M/W �����۾� �߿� �߻��� �����۾����� ���� ���������� �ҽ� �����۾���
	//                   �ʿ��Ͽ� �Լ��� ȣ���ϵ��� �����Ͽ���
//// ADD -------------------------------------------------------------------------------------
	pDlg->DownloadESLReference();
//// END -------------------------------------------------------------------------------------
#endif
	pProgressCtrl->StepIt();

	// 7. DVM Loding
	if (!pDlg->m_bIsThreadRun) goto CANCEL;
	if (pDlg->DownloadDVM() < 0)
		goto ERR;
	pProgressCtrl->StepIt();
	
	// 8.�޴�, ���ٱ׷�, ���� ������ �ٿ�޴´�.
	//2004-05-06 ������ ���� 
	/*
	static_cast<CKOLAS3_MAINView*>(pDlg->m_pParentWnd)->LoadMenuRef();
	static_cast<CKOLAS3_MAINView*>(pDlg->m_pParentWnd)->LoadToolbarRef();
	static_cast<CKOLAS3_MAINView*>(pDlg->m_pParentWnd)->LoadToolGroupRef();
    */

	// 9. �α� �˻� �� ����
	//2013-10-08 ���缱 �߰�
	if (!pDlg->m_bIsThreadRun) goto CANCEL;
	strText = _T("�α� �˻���...");
	pProgressCtrl->SetWindowText(strText);
	pDlg->ControlLog();
	pDlg->m_ProgressCtrl.StepIt();


	if (pDlg->DownLoadMenuToolbar() < 0)
	goto ERR;

	strText = _T("�α��� ����");
	pProgressCtrl->SetWindowText(strText);
	pProgressCtrl->SetPos(nUpper);

	pDlg->PostMessage(WM_CLOSE_DIALOG, IDOK, 0);

	pDlg->m_bIsThreadRun = FALSE;
	SetEvent(pDlg->m_hThread);
			
	return 0;

CANCEL:
	pDlg->m_bIsThreadRun = FALSE;
	SetEvent(pDlg->m_hThread);
	return -1;

ERR:
	if ( nLog < 0 ) AfxMessageBox(_T("�α��� ����"));
	
	pDlg->PostMessage(WM_CLOSE_DIALOG, IDCANCEL, 0);

	pDlg->m_bIsThreadRun = FALSE;
	SetEvent(pDlg->m_hThread);
	
	return -1;
}

// << 2015�� ���������� �ý��� ���� 2015/11/30 _LCM : IP ��Ī
#define DESIRED_WINSOCK_VERSION        0x0202
#define MINIMUM_WINSOCK_VERSION        0x0001

BOOL CLoginDlg::IsMatchIP(CString strIP)
{
	TCHAR parm_buffer[16][24];
	int parm_max_count = 16;

	int count = 0;
    WSADATA wsadata; 	

    // WS2_32.DLL�� ����Ҽ� �ֵ��� �ʱ�ȭ�Ѵ�.
    if(!WSAStartup(DESIRED_WINSOCK_VERSION, &wsadata)){
        // ���� ��ǻ�Ϳ� ��ġ�� ������ �̿밡���� �������� üũ�Ѵ�.
        if(wsadata.wVersion >= MINIMUM_WINSOCK_VERSION){
            IN_ADDR in_address;
            TCHAR host_name[255] = {0, };

            // ���� ��ǻ���� ȣ��Ʈ ��Ī�� ��´�.
            gethostname((LPSTR)(LPCTSTR)host_name, 255);

            // ȣ��Ʈ �����ͺ��̽����� ������ ȣ��Ʈ ��Ī�� �����ϴ� ȣ��Ʈ ������ ��´�.
            HOSTENT *p_host_info = gethostbyname((LPSTR)(LPCTSTR)host_name);
            if(p_host_info != NULL){
                for(int i = 0; p_host_info->h_addr_list[i]; i++){
                    if(parm_max_count > i){
                        memcpy(&in_address, p_host_info->h_addr_list[i], 4);
						TCHAR arrBuff[24] = {0,};
						mbstowcs(arrBuff, inet_ntoa(in_address), sizeof(TCHAR)*24);
                        wcscpy(parm_buffer[i], arrBuff);

						if(wcscmp(parm_buffer[i], strIP.GetBuffer(0)) == 0)
						{
							WSACleanup();
							return TRUE;
						}

                    } else break;
                }
                count = i;
            }
        }

        // WS2_32.DLL�� ����� �ߴ��Ѵ�.
        WSACleanup();
    }
	return FALSE;
}
// >> 2015/11/30 --ADD

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg Operations
CString CLoginDlg::GetIP()
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

// 16.09.29 JMJ KOL.UDF.020
CString CLoginDlg::GetMacAddress()
{
	/*
	CString strGateWay = _T("");
	CString strMacAddress = _T("");

	IP_ADAPTER_INFO ipAdapterInfo[5];
	DWORD dwBuflen = sizeof(ipAdapterInfo);


	DWORD dwStatus = GetAdaptersInfo(ipAdapterInfo, &dwBuflen);

	PIP_ADAPTER_INFO pipAdapterInfo = ipAdapterInfo;

	do 
	{
		strGateWay = (CString)pipAdapterInfo->GatewayList.IpAddress.String;
		if(!strGateWay.IsEmpty() && strGateWay[0] == '0'){
			pipAdapterInfo = pipAdapterInfo->Next;
		}else{
			strMacAddress.Format(_T("%02X%02X%02X%02X%02X%02X"),
				pipAdapterInfo->Address[0],
				pipAdapterInfo->Address[1],
				pipAdapterInfo->Address[2],
				pipAdapterInfo->Address[3],
				pipAdapterInfo->Address[4],
				pipAdapterInfo->Address[5]
			);
			break;
		}
	} while (pipAdapterInfo);

	return strMacAddress;
	*/
	// 18.11.12 JMJ KOL.RED.2018.006
	/*
		MAC ADDRESS �������� �� ����
		iphlpapi.h (ipexport.h, iptypes.h) ����
    */
	NCB Ncb;
    UCHAR uRetCode;
    LANA_ENUM lenum;
    int i;
    CString strOutput =_T("");
	CString strIP = _T("*");
    CString string;
    ADAPTER_STATUS Adapter;
	
    memset( &Ncb, 0, sizeof(Ncb) );
    Ncb.ncb_command = NCBENUM;
    Ncb.ncb_buffer = (UCHAR *)&lenum;
    Ncb.ncb_length = sizeof(lenum);
    uRetCode = Netbios( &Ncb );
	
    for(i=0; i < lenum.length ;i++)
    {
        memset( &Ncb, 0, sizeof(Ncb) );
        Ncb.ncb_command = NCBRESET;
        Ncb.ncb_lana_num = lenum.lana[i];
		
        uRetCode = Netbios( &Ncb );
		
		
        memset( &Ncb, 0, sizeof (Ncb) );
        Ncb.ncb_command = NCBASTAT;
        Ncb.ncb_lana_num = lenum.lana[i];
		
        strcpy( (char*)Ncb.ncb_callname, (LPSTR)(LPCTSTR) strIP ); 
        Ncb.ncb_buffer = (unsigned char *) &Adapter;
        Ncb.ncb_length = sizeof(Adapter);
		
        uRetCode = Netbios( &Ncb );
        if ( uRetCode == 0 )
        {
            string.Format(_T("%02X%02X%02X%02X%02X%02X"),
				Adapter.adapter_address[0],
				Adapter.adapter_address[1],
				Adapter.adapter_address[2],
				Adapter.adapter_address[3],
				Adapter.adapter_address[4],
				Adapter.adapter_address[5] );
            strOutput += string ;
			break;
        }
    }
    return strOutput;
}
INT CLoginDlg::Login()
{
	INT ids = -1;

	CString strQuery = _T("");
	
	m_strID.TrimLeft();
	m_strID.TrimRight();

	m_strPassword.TrimLeft();
	m_strPassword.TrimRight();

	m_strID.Replace(_T(" "), _T(""));
	m_strPassword.Replace(_T(" "), _T(""));

	m_strID.Replace(_T("'"), _T(""));
	m_strPassword.Replace(_T("'"), _T(""));		

	// 14/07/02 �ڴ�� : �������߰��� ���� FLAG���� ��� ����
/*// BEFORE ---------------------------------------------------------------------------------
	//                          0        1         2          3		
	strQuery.Format(_T("SELECT NAME, PERMITION, REC_KEY, MANAGE_CODE "
						 _T("FROM MN_USER_TBL ")
						_T("WHERE ID='%s' AND PASSWD='%s'")), m_strID, m_strPassword);

	
	// 2012.10.06 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2ȸ(����� ��й�ȣ ��ȣȭ)
	if(m_pInfo->m_bUserPwEncryption)
	{
		CString strCryptoPassword;
		strCryptoPassword = CLocCommonAPI::StringToSHA256(m_strPassword);

		strQuery.Format(_T("SELECT NAME, PERMITION, REC_KEY, MANAGE_CODE "
						 _T("FROM MN_USER_TBL ")
						_T("WHERE ID='%s' AND PASSWD='%s'")), m_strID, strCryptoPassword);
	}
*/// AFTER -----------------------------------------------------------------------------------
	CString strPW = m_strPassword;
	// 2015�� ���������� �ý��� ���� 2015/11/23 _LCM : GPKI ������ �α��� ���� ����
	if (m_pInfo->m_bUserPwEncryption && !m_bGPKI)
	{
		strPW = CLocCommonAPI::StringToSHA256(m_strPassword);
	}
	// KOL.RED.2018.218 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++	
	/*
	//                          0        1         2          3						4									5
	strQuery.Format(_T("SELECT NAME, PERMITION, REC_KEY, MANAGE_CODE, TO_CHAR(EXPIRY_DATE, 'YYYY/MM/DD'), TO_DATE(TO_CHAR(EXPIRY_DATE, 'YYYY/MM/DD'))-TO_DATE(TO_CHAR(SYSDATE, 'YYYY/MM/DD')) , GPKI_USE, GPKI_KEY_VALUE "
						 _T("FROM MN_USER_TBL ")
						_T("WHERE ID='%s' AND PASSWD='%s'")), m_strID, strPW);
	*/
	//                          0        1         2          3						4									5																			6		7				8
	strQuery.Format(_T("SELECT NAME, PERMITION, REC_KEY, MANAGE_CODE, TO_CHAR(EXPIRY_DATE, 'YYYY/MM/DD'), TO_DATE(TO_CHAR(EXPIRY_DATE, 'YYYY/MM/DD'))-TO_DATE(TO_CHAR(SYSDATE, 'YYYY/MM/DD')) , GPKI_USE, GPKI_KEY_VALUE, NVL(USERGRID_EXPORT_YN,'Y') "
						 _T("FROM MN_USER_TBL ")
						_T("WHERE ID='%s' AND PASSWD='%s'")), m_strID, strPW);	
	// KOL.RED.2018.218 ---------------------------------------------------------------------------	
	
//*/ END -------------------------------------------------------------------------------------
	ids = m_DM.RestructDataManager(strQuery);
	if (ids < 0) return ids;
	if (m_DM.GetRowCount() < 1) return -1;	// �α��� ����

// << 2015�� ���������� �ý��� ���� 2015/11/23 _LCM : GPKI ������ �α��� ���� ����

	if( m_DM.GetCellData(0, 6).CompareNoCase(_T("Y")) == 0 && !m_bGPKI)
	{
		
		AfxMessageBox(_T("GPKI �α��� ������Դϴ�.\r\n")
						_T("���̵� �н������ �α��� �Ұ��մϴ�.\r\n")
						_T("��α��� �ٶ��ϴ�."));			
	
		return -100;
	}
	else if ( m_DM.GetCellData(0, 6).CompareNoCase(_T("Y")) != 0 && m_bGPKI )
	{
		AfxMessageBox(_T("GPKI �α��� �����ٶ��ϴ�.\r\n")
							_T("GPKI�� �α��� �Ұ��մϴ�.\r\n")
							_T("��α��� �ٶ��ϴ�."));			
		return -100;
	}
	else if(m_DM.GetCellData(0, 6).CompareNoCase(_T("Y")) == 0 && m_bGPKI && m_DM.GetCellData(0, 7).CompareNoCase(_T("")) == 0)
	{
		AfxMessageBox(_T("GPKI �α��� �����ٶ��ϴ�.\r\n")
						_T("GPKI�� ��ϵ��� �ʾҽ��ϴ�.\r\n")
						_T("�����ڿ��� ���ǹٶ��ϴ�."));			
		return 100;
	}
		

// >> 2015/11/23 --ADD
	

	// 14/07/02 �ڴ�� : ����� ��ȿ�Ⱓ ��� �߰�
//*/ ADD -------------------------------------------------------------------------------------
	if (m_DM.GetCellData(0, 1).CompareNoCase(_T("U")) == 0) // �Ϲݻ������ ��� Ȯ�� �ʿ�
	{
		CString strExpDate = m_DM.GetCellData(0, 4);
		if (!strExpDate.IsEmpty())
		{
			int nExpTerm = _ttoi(m_DM.GetCellData(0, 5));
			if (nExpTerm < 0)
			{
				AfxMessageBox(_T("������� ��ȿ�Ⱓ�� ����Ǿ� �α����� �� �� ���� �����Դϴ�.\r\n")
							  _T("�����ڿ��� �����Ͻʽÿ�."), MB_ICONSTOP);	
				return 100;
			}
			// ���Ⱓ�� �󸶳��� �ʾҴٸ� �޽����� �˷��ش�
			if (nExpTerm < 10)
			{
				CString strMsg;
				strMsg.Format(_T("�α��� ������� ��ȿ�Ⱓ�� [%d]�� ���ҽ��ϴ�.\r\n")
							  _T("��ȿ�Ⱓ�� ����� ��쿡�� �α����� �Ұ����Ͽ���\r\n")
					          _T("�����ڿ��� �����Ͻñ� �ٶ��ϴ�."),nExpTerm+1);
				AfxMessageBox(strMsg, MB_ICONINFORMATION);	
			}
		}
	}
//*/ END -------------------------------------------------------------------------------------

	// m_pInfo �����Ѵ�.
	m_pInfo->USER_ID		= m_strID;
	m_pInfo->USER_NAME		= m_DM.GetCellData(0, 0); 
	m_pInfo->USER_PRIVILEGE = m_DM.GetCellData(0, 1); 
	m_pInfo->USER_PK		= m_DM.GetCellData(0, 2);
	m_pInfo->LOCAL_IP		= GetIP();
	// 16.09.29 JMJ KOL.UDF.020
	m_pInfo->MAC_ADDRESS	= GetMacAddress();
	m_pInfo->MANAGE_CODE	= m_DM.GetCellData(0, 3);
	m_pInfo->MANAGE_CODE.TrimLeft();		
	m_pInfo->MANAGE_CODE.TrimRight();
	m_DM.MANAGE_CODE = m_pInfo->MANAGE_CODE;

	// KOL.RED.2018.218 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++	
	if (m_pInfo->USER_PRIVILEGE.CompareNoCase(_T("U")) == 0)	// ����� ������ ��쿡�� Ȯ��
		m_pInfo->m_bUserInfoExportYN = (m_DM.GetCellData(0, 8).CompareNoCase(_T("Y")) == 0);
	else // ������ / ���հ����ڴ� ���� ������ �����Ѱ� ��±����� ������
		m_pInfo->m_bUserInfoExportYN = TRUE;
	// KOL.RED.2018.218 ---------------------------------------------------------------------------	


	CString strTemp;
	// 2011.10.04 ADD BY PWR : ���յ�������
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_1='L' AND GROUP_2='��Ÿ' AND GROUP_3='����' AND CLASS_ALIAS='���յ�������' AND VALUE_NAME = '���յ�������' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	m_DM.GetOneValueQuery(strQuery, strTemp);
	strTemp.TrimLeft(); strTemp.TrimRight();
	if(_T("Y") == strTemp)
	{
		m_pInfo->m_bKLUse = TRUE;
	}

	// 2009.11.19 ADD BY PWR : ���մ���Ǽ���뿩�ΰ� �������̸� ����ȯ������ ���� �ʴ´�. (PERMITION�� 'A'�̸� 'S'�� ����)
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE VALUE_NAME='���մ���Ǽ���뿩��' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	m_DM.GetOneValueQuery(strQuery, strTemp);
	strTemp.TrimLeft(); strTemp.TrimRight();
	if(_T("Y") != strTemp)
	{
		if(_T("A") == m_pInfo->USER_PRIVILEGE)
		{
			m_pInfo->USER_PRIVILEGE = _T("S");
		}
	}

	//2008.11.11 ADD  BY PJW : ��ϱ����ڵ� �ڸ��� ����
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS = '��ϱ����ڵ��ڸ�������' AND MANAGE_CODE = '%s'"),m_pInfo->MANAGE_CODE);
	m_DM.GetOneValueQuery(strQuery, strTemp);
	m_pInfo->nREG_CODE_LENGTH = _ttoi(strTemp);
	SaveRegCodeLengthToFile();

	// �ý��� �ð��� �����.
	SYSTEMTIME t;
	CTime curtime = m_DM.GetDBTime();
	curtime.GetAsSystemTime(t);
	SetLocalTime(&t);

	// 2010.08.27 ADD BY PWR : �������ڸ� ����Ѵ�.
	CString strCurrentDateFilePath = _T("..\\cfg\\CurrentDate.cfg");
	CFileFind ff;
	if(TRUE == ff.FindFile(strCurrentDateFilePath)) DeleteFile(strCurrentDateFilePath);
	ff.Close();
	CString strCurrentDate;
	strCurrentDate.Format(_T("%04d/%02d/%02d"), curtime.GetYear(), curtime.GetMonth(), curtime.GetDay());
	FILE *fp = _tfopen(strCurrentDateFilePath, _T("w+b"));
	fputc(0xFF, fp); fputc(0xFE, fp);
	_ftprintf(fp, strCurrentDate);
	fclose(fp);

	//++2008.10.15 DEL BY PWR {{++
	// ���սý��� ����
	// �������к� ������ ���� Y/N �� �о�´�.
//	CString strTmp;
//	m_DM.GetOneValueQuery(	_T("SELECT VALUE_NAME ")
//							_T("FROM ESL_MANAGE_TBL ")
//							_T("WHERE GROUP_1='L' AND GROUP_2='��Ÿ' AND GROUP_3='����' ")
//							_T("AND CLASS_ALIAS='�������к�����������' ")
//							_T("AND MANAGE_CODE=UDF_MANAGE_CODE"), strTmp );
//	strTmp.TrimLeft();		
//	strTmp.TrimRight();
//	strTmp.MakeUpper();
	//--2008.10.15 DEL BY PWR --}}

	// EFS�� ���� USER_INFO�� ���Ͽ� ����
	CString strResult[2];
	CStdioFile file;

	// 2009.03.11 UPDATE BY CJY : �������� ������������ �α��� �� ��츦 ���� PID�� �����Ѵ�.
	DWORD dwSID = ::GetCurrentProcessId();
	CString strCHMfileName;
	strCHMfileName.Format(_T("..\\CFG\\EFS\\user_info(%u).log"), dwSID);
		
// 	if (file.Open(_T("..\\cfg\\efs\\user_info.log"), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	if (file.Open(strCHMfileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		//LIB_CODE = CA
		//USER_ID = MONAMY
		//IP = 127.0.0.1
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}

		//++2008.10.15 UPDATE BY PWR {{++
		// ���սý��� ����
		strQuery.Format(_T("SELECT LIB_CODE, LIB_NAME FROM MN_LIBINFO2_TBL ")
						_T("WHERE MANAGE_CODE=UDF_MANAGE_CODE") );
				m_DM.GetValuesQuery(strQuery, strResult, 2);

		// 2009.04.01 ADD BY CJY : ��������ȣ ����
		m_pInfo->LIB_CODE = strResult[0];
		// 2010.07.26 ADD BY PWR : �������̸� ����
		m_pInfo->LIB_NAME = strResult[1];
//		if (strTmp == _T("Y")) {
//			if (m_pInfo->MANAGE_CODE.GetLength() == 0) {
//				m_DM.GetValuesQuery(_T("SELECT LIB_CODE, LIB_NAME FROM MN_LIBINFO2_TBL "
//										_T("WHERE MANAGE_CODE IS NULL")), strResult, 2);
//			} else {
//				strQuery.Format(_T("SELECT LIB_CODE, LIB_NAME FROM MN_LIBINFO2_TBL "
//									_T("WHERE MANAGE_CODE = '%s'")), m_pInfo->MANAGE_CODE);
//				m_DM.GetValuesQuery(strQuery, strResult, 2);
//			}
//		} else {
//			m_DM.GetValuesQuery(_T("SELECT LIB_CODE, LIB_NAME FROM MN_LIBINFO2_TBL "
//									_T("WHERE MANAGE_CODE IS NULL")), strResult, 2);
//		}
		//--2008.10.15 UPDATE BY PWR --}}
		
		CString strLine;
		//strLine.Format(_T("LIB_CODE = %s\r\n"), strResult[0]);
		//file.WriteString(strLine);

		strLine.Format(_T("%s\r\n"), m_pInfo->MANAGE_CODE);
		file.WriteString(strLine);

		//strLine.Format(_T("USER_ID = %s\r\n"), m_pInfo->USER_ID);
		//file.WriteString(strLine);



		//strLine.Format(_T("IP = %s\r\n"), m_pInfo->LOCAL_IP);
		//file.WriteString(strLine);

		file.Close();

		// 2009.03.18 ADD BY PDJ : ���������� ���� ��� ������ �� ����.
		if ( m_pInfo->MANAGE_CODE == _T("") )
		{
			AfxMessageBox(_T("�α��� ������� ���������� �������� �ʾҽ��ϴ�.\r\n")
							_T("���������� ���� ���·� ������ �� �����ϴ�.\r\n")
							_T("�����ڿ��� �����Ͻʽÿ�."));			
			return 100;
		}
	}

	// MENU �� SPF_PATH, SPF_STEP, SM�ʱ�ȭ
	SetProgramPath(_T("SPF_PATH"),	_T("NULL"));
	SetProgramPath(_T("SPF_STEP"),	_T("NULL"));
	SetProgramPath(_T("SM"),		_T("NULL"));
	SetProgramPath(_T("MENU"),		_T("HOME"));

	// �������� ������
	m_pInfo->TITLE = strResult[1];

	//strQuery.Format(_T("SELECT VALUE FROM MN_LIBINFO_TBL WHERE VALUE_NAME='��������'"));
	//ids = m_DM.GetOneValueQuery(strQuery, m_pInfo->TITLE);

	//=====================================================
	//2009.01.28 ADD BY PJW : ��ȣ������ ����� ���������� �α���  USER�� ������������ �����Ѵ�.
	CString strSubQuery;
	m_DM.StartFrame();
	strSubQuery.Format(_T("SELECT REC_KEY FROM CD_CODE_TBL WHERE CLASS = 1"));
	strQuery.Format(_T("UPDATE MN_USER_CODE_TBL SET DEFAULT_VALUE = 'N' WHERE CODE_KEY IN ( %s ) AND USER_KEY = %s AND DEFAULT_VALUE = 'Y';"),strSubQuery,m_pInfo->USER_PK);
	m_DM.AddFrame(strQuery);	
	strQuery.Format(_T("UPDATE MN_USER_CODE_TBL SET DEFAULT_VALUE = 'Y', ACTIVE = 'Y' WHERE CODE_KEY IN ( %s AND CODE = UDF_MANAGE_CODE) AND USER_KEY = %s;"),strSubQuery,m_pInfo->USER_PK);
	m_DM.AddFrame(strQuery);		
	ids = m_DM.SendFrame();
	if( 0 > ids)
	{
		AfxMessageBox(_T("���������� �����ϴµ� �����߽��ϴ�."));
	}
	m_DM.EndFrame();
	//=====================================================
	
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='���յ�������' AND VALUE_NAME='���ü���' AND MANAGE_CODE = '%s'"),m_pInfo->MANAGE_CODE);
	m_DM.GetOneValueQuery(strQuery, strTemp);

	strCHMfileName.Format(_T("..\\CFG\\EFS\\UnionLoanServiceInfo(%u).log"), dwSID);
	if(file.Open(strCHMfileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}
		CString strLine;
		strLine.Format(_T("%s\r\n"), strTemp);
		file.WriteString(strLine);
		file.Close();
	}

	// 2011.12.12 ADD BY PWR : ����������ȣ
	if(10000 != m_pInfo->MODE)
	{
		strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='����������ȣ' AND VALUE_NAME='�Ǹ�������뿩��' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		m_DM.GetOneValueQuery(strQuery, strTemp);
		if(_T("Y") == strTemp) m_pInfo->m_bCertifyUse = TRUE;
		// 2014/07/02 �ڴ�� : �����ɻ������ FLAG �߰�
		//                     �����ɰ� �Ǹ���������� ���� ����� �� ����
		//					   �����ɻ�뿩�� �켱������ ����
/*//ADD------------------------------------------------------------------------*/
		strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='����������ȣ' AND VALUE_NAME='�����ɻ�뿩��' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		m_DM.GetOneValueQuery(strQuery, strTemp);
		if(_T("Y") == strTemp) m_pInfo->m_bCertifyUse = FALSE;
/*//END------------------------------------------------------------------------*/
		// 2014/07/26 �ڴ�� : �����ɻ������ FLAG �߰�
		//                     �����ɻ������ FLAG �߰�
		//					   �����ɻ�뿩�ΰ� �Ǹ��������� �켱������ ���� ����
/*//ADD------------------------------------------------------------------------*/
		CString strMypinUsed;
		strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='����������ȣ' AND VALUE_NAME='MYPIN��뿩��' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		m_DM.GetOneValueQuery(strQuery, strMypinUsed);	
		if(_T("Y") == strMypinUsed) m_pInfo->m_bCertifyUse = FALSE;
/*//END------------------------------------------------------------------------*/

		// 15/08/18 ������ : �޴������� �߰�
		CString strPhoneUsed;
		strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='����������ȣ' AND VALUE_NAME='�޴���������뿩��' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		m_DM.GetOneValueQuery(strQuery, strPhoneUsed);	
		if(_T("Y") == strPhoneUsed) m_pInfo->m_bCertifyUse = FALSE;

		// 2014/07/26 �ڴ�� : �����ɻ������ FLAG �߰�
/*//BEFORE--------------------------------------------------------------------------
		if(TRUE == m_pInfo->m_bCertifyUse)
*///AFTER---------------------------------------------------------------------------
		// 15/08/18 ������ : �޴������� �߰�
		if(TRUE == m_pInfo->m_bCertifyUse || _T("Y") == strMypinUsed || _T("Y") == strPhoneUsed)
/*//END------------------------------------------------------------------------*/
		{
			// 2012.01.25 ADD BY PWR : �ֹε�Ϲ�ȣ������ȸ
			strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='����������ȣ' AND VALUE_NAME='�ֹε�Ϲ�ȣ������ȸ' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
			m_DM.GetOneValueQuery(strQuery, strTemp);
			if(_T("Y") == strTemp) m_pInfo->m_bCivilNoSearch = TRUE;
			// �Ǹ����� �̵���� ��������
			strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='����������ȣ' AND VALUE_NAME='�̵������������' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
			m_DM.GetOneValueQuery(strQuery, strTemp);
			strCHMfileName.Format(_T("..\\CFG\\EFS\\CertifyServiceInfo(%u).log"), dwSID);
			if(file.Open(strCHMfileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
			{
				if( 0 == file.GetLength() )
				{			
					TCHAR cUni = 0xFEFF;
					file.Write( &cUni, sizeof(TCHAR));
				}
				CString strLine;
				strLine.Format(_T("%s\r\n"), strTemp);
				file.WriteString(strLine);
				file.Close();
			}
			// �Ǹ����� �ſ����������
			strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='����������ȣ' AND VALUE_NAME='�ſ����������' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
			m_DM.GetOneValueQuery(strQuery, strTemp);
			if(_T("1") == strTemp) m_pInfo->m_nCertifyAgency = 1;
			strQuery = _T("SELECT AGENCY,CODE,DESCRIPTION,ACTION FROM CERTIFY_FAIL_CAUSE_TBL");
			m_pInfo->m_CertifyFailCodeDM.SetCONNECTION_INFO(CONNECTION_INFO);
			m_pInfo->m_CertifyFailCodeDM.RestructDataManager(strQuery);
			
			// 2012.12.13 DEL BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 8ȸ
			// ���Ǳ�Ͽ���
//			strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='����������ȣ' AND VALUE_NAME='���Ǳ�Ͽ���' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
//			m_DM.GetOneValueQuery(strQuery, strTemp);
//			if(_T("Y") == strTemp)
//			{
//				m_pInfo->m_bAgreeInfoRecordYN = TRUE;
//			}
//			return 0;
		}
	}

	// 2012.09.26 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2ȸ
	// ���űⰣ����˸� ��뿩��
	if(TRUE == m_pInfo->m_bKLUse || TRUE == m_pInfo->m_bCertifyUse)
	{
		strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='����������ȣ' AND VALUE_NAME='���űⰣ����˸�' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		m_DM.GetOneValueQuery(strQuery, strTemp);
		if(_T("Y") == strTemp)
		{
			m_pInfo->m_bNotifyExpirationYN = TRUE;
		}
	}
	// 2012.09.26 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2ȸ
	// ȸ������ ���űⰣ
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='�̿��ڰ���' AND VALUE_NAME='ȸ���������űⰣ' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	m_DM.GetOneValueQuery(strQuery, strTemp);
	if(_T("") != strTemp)
	{
		m_pInfo->m_nUserInfoRenewalDay = _ttoi(strTemp);
	}

	// 2012.11.06 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2��
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='����ڷ��û���ѻ�뿩��' AND VALUE_NAME='����ڷ��û���ѻ�뿩��' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	m_DM.GetOneValueQuery(strQuery, strTemp);
	if(_T("Y") == strTemp)
	{
		m_pInfo->m_bFurnishWishLimitYN = TRUE;
	}
	else
	{
		m_pInfo->m_bFurnishWishLimitYN = FALSE;
	}

	//job.2018.0071 �ִ��� ��û�Ǽ� ���� 
	//==========================================================================================================================================================================================
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='����ڷ��û�ִ������ѻ�뿩��' AND VALUE_NAME='����ڷ��û�ִ������ѻ�뿩��' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	m_DM.GetOneValueQuery(strQuery, strTemp);
	if(_T("Y") == strTemp)
	{
		m_pInfo->m_bFurnishLimitByTheWeek = TRUE;
	}
	else
	{
		m_pInfo->m_bFurnishLimitByTheWeek = FALSE;
	}
	//==========================================================================================================================================================================================
	if(m_pInfo->m_bFurnishWishLimitYN)
	{
		strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='����ڷ��û���ѱǼ�' AND VALUE_NAME='����ڷ��û���ѱǼ�' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		m_DM.GetOneValueQuery(strQuery, strTemp);
		if(_T("") != strTemp)
		{
			m_pInfo->m_nFurnishWishMaxCnt = _ttoi(strTemp);
		}
	}

	//job.2018.0071 �ִ��� ��û�Ǽ� ����
	//==========================================================================================================================================================================================
	else if (m_pInfo->m_bFurnishLimitByTheWeek)
	{
		strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='����ڷ��û�ִ������ѱǼ�' AND VALUE_NAME='����ڷ��û�ִ������ѱǼ�' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		m_DM.GetOneValueQuery(strQuery, strTemp);
		if(_T("") != strTemp)
		{
			m_pInfo->m_nFurnishWishMaxCnt = _ttoi(strTemp);
		}
	}
	//==========================================================================================================================================================================================
		
	//JOB.2020.0004 ADD BY KYJ : ȸ������ ��ġ�����û ���ѱ�� �߰�
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='����ڷ�ȸ���������ѻ�뿩��' AND VALUE_NAME='����ڷ�ȸ���������ѻ�뿩��' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	m_DM.GetOneValueQuery(strQuery, strTemp);
	if(_T("Y") == strTemp)
	{
		m_pInfo->m_bRestictionToApply = TRUE;
	}
	
	// 2012.12.13 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 8ȸ
	// ���Ǳ�Ͽ���
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='����������ȣ' AND VALUE_NAME='���Ǳ�Ͽ���' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	m_DM.GetOneValueQuery(strQuery, strTemp);
	if(_T("Y") == strTemp)
	{
		m_pInfo->m_bAgreeInfoRecordYN = TRUE;
	}

	// 2017/08/25 : 2017�� ���������� å�������� Ȯ�뱸�� �� �ý��۰���
	// ���ո�ũ ���� ����
	//----------------------------------------------------------------------------------------------------------
	// ������ ���ո�ũ�� ����ϱ� ���ؼ� DB�� ������ ���ո�ũ ������ ���Ǵ� �׷���� ��ȸ�Ͽ� ���Ͽ� �����Ѵ�.
	// ������ ���ո�ũ���� ���Ǵ� �׷���� "cfg\\MarcRelation\\���ո�ũ_����_�׷��.cfg" ���Ͽ� ��ϵȴ�. 
	// KOLASIII�� ��ũ �׷���� "���ո�ũ"�� ����Ҷ� ���Ͽ� ������ �׷������ ��ü�ȴ�. 
	// ���ؽ� üũ�� ���Ǵ� ���ϸ��� ������ ���� ��ü�ȴ�. 
	// SimpleRelation_���ո�ũ.txt  ->  SimpleRelation_���Ͽ�����ȱ׷��.txt
	// ���Ͽ� ����� �׷���� "���ո�ũ_����1"�̸� ���� ������ �������� ���ɶ� �����ذ��鼭 ����ϸ� �ȴ�. 
	// �׷�� ��ü �۾�
	//  - �ҽ� : \Source\ESL\ESL\MarcRelation.cpp
	//  - �Լ� : INT CMarcRelation::MakeMarcRelation(CString strMarcGroup)
	//  - �ּ� : ���ո�ũ ���� ����
/*//NEW------------------------------------------------------------------------*/
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='���ո�ũ����' AND VALUE_NAME='�׷��' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	m_DM.GetOneValueQuery(strQuery, strTemp);
	CString strFileName = _T("..\\cfg\\MarcRelation\\���ո�ũ_����_�׷��.cfg");
	DeleteFile(strFileName);
	if (!strTemp.IsEmpty()) {
		FILE *fp = _tfopen(strFileName, _T("w+b"));
		fputc(0xFF, fp); fputc(0xFE, fp);
		_ftprintf(fp, _T("�׷��=%s"), strTemp);
		fclose(fp);
	}
/*//END------------------------------------------------------------------------*/

	// KOL.RED.2018.220 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	strQuery.Format(_T("SELECT MAX(VALUE_1) FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='��ϱ����ڵ��ڸ�������' AND VALUE_NAME='��ϱ����ڸ���' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	m_DM.GetOneValueQuery(strQuery, strTemp);
	if (!strTemp.IsEmpty()) {
		m_pInfo->m_nRegCodeLength = _ttoi(strTemp);
	}
	// KOL.RED.2018.220 ---------------------------------------------------------------------------
	
	// KOL.RED.2018.228 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++	
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='KOLAS�˻���������' AND VALUE_NAME='�˻������������' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	m_DM.GetOneValueQuery(strQuery, strTemp);
	if (strTemp.CompareNoCase(_T("Y")) == 0)
	{
		CString strURL;
		strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='KOLAS�˻���������' AND VALUE_NAME='�˻�����URL' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		m_DM.GetOneValueQuery(strQuery, strURL);

		CESL_SearchEngine::SetURL(strURL);
		//CESL_SearchEngine::SetURL(_T("http://192.168.1.86:8080/solr"));

		strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='KOLAS�˻���������' AND VALUE_NAME='�˻������˻����' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		m_DM.GetOneValueQuery(strQuery, strTemp);

		CESL_SearchEngine::SetSearchMode(_ttoi(strTemp));

		// �⺻�� ����
		CESL_SearchEngine::SetTimeOut(5);
		CESL_SearchEngine::SetStart(0);
		CESL_SearchEngine::SetRows(20000);

		//{{�˻����� ���� Ȯ��
		const INT nCoreCount = 4;
		CString strCoreAlias[nCoreCount] = {
			_T("IDX_BO_TBL"), _T("IDX_BO_TOC_TBL"),
			_T("IDX_SE_TBL"), _T("IDX_SE_TOC_TBL")
		};

		// ���������� �ʱ�ȭ
		for (INT i=0; i<nCoreCount; i++)
			CESL_SearchEngine::SetStatus(strCoreAlias[i], FALSE);
		
		for (i=0; i<nCoreCount; i++)
		{
			CString strStatus;
			ids = CESL_SearchEngine::RequestPingQuery(strCoreAlias[i], strStatus);
			if (ids < 0) break; // ������ �߻��Ͽ��ٸ� �� �̻� Ȯ������ �ʴ´� 
			CESL_SearchEngine::SetStatus(strCoreAlias[i], (strStatus.CompareNoCase(_T("OK")) == 0));
		}
		//}}

		// �ھ� ���¿� ���� �˻����� ��밡�� ���θ� �����Ѵ�
		BOOL bUsedFlag = FALSE;
		for (i=0; i<nCoreCount; i++)
		{
			bUsedFlag = bUsedFlag | CESL_SearchEngine::GetStatus(strCoreAlias[i]); // ����� �� �ִ� �ھ �ִ��� Ȯ��
			// �����ڷ�� Ư��ó��
			if (i == 0 && !bUsedFlag) break;
		}
		
		CESL_SearchEngine::SetUsed(bUsedFlag);


	}
	// KOL.RED.2018.228 ---------------------------------------------------------------------------
	// >> 2015/10/13 --MOD
	

	return 0;
}



//2003-1106 CESL_Information ���� �۾�-�ڱ���========================================	
INT CLoginDlg::DownloadSystemInfoData()
{
	INT ids;
	CString strTmp;
	CString sTemp;
	INT nMngMode = -1;
	// 2018.11.13 LJY : RFID ��뿩��=====================================================================
	ids = m_DM.GetOneValueQuery( _T("SELECT VALUE_NAME ")
					 			 _T("FROM ESL_MANAGE_TBL ")
						         _T("WHERE CLASS_ALIAS='RFID��뿩��' ")
								 _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), strTmp );
	if (0>ids && -4007!=ids) 
	{
		sTemp.Format(_T("�ý������� �ٿ�ε��� ����[RFID][%d]..."), ids);
		AfxMessageBox(sTemp);
		return -1;
	}
	/*
	nMngMode = 0;	//default, �ش緹�ڵ尡 ���� ����� ��
	m_bUseRFID = FALSE;
	if (-4007!=ids) 
	{
		strTmp.TrimLeft();		
		strTmp.TrimRight();
		strTmp.MakeUpper();

		if (strTmp == _T("Y")) m_bUseRFID = TRUE;

		// ==================================================================================
		nMngMode = (m_bUseRFID) ? 1 : 0;
	}
	m_pInfo->SetRFIDUseMode(nMngMode);
	*/
	//----------------------------------------------------------------------------------------------------------------------

	// SMS ���� ��뿩�� �� ���� ����==================================================
	ids = m_DM.GetOneValueQuery( _T("SELECT VALUE_NAME ")
						         _T("FROM ESL_MANAGE_TBL ")
						         _T("WHERE CLASS_ALIAS='SMS��������' ")
								 _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), strTmp ); // ADD BY PWR (���սý��� ����)
	if (0>ids && -4007!=ids) 
	{
		sTemp.Format(_T("�ý������� �ٿ�ε��� ����[SMS][%d]..."), ids);
		AfxMessageBox(sTemp);
		return -1;
	}
	nMngMode = 0;	//default, �ش緹�ڵ尡 ���� ����� ��
	if (-4007!=ids) 
	{
		strTmp.TrimLeft();		
		strTmp.TrimRight();
		strTmp.MakeUpper();
		//++2008.11.20 UPDATE BY KSJ {{++
		//<< K2UP���� SMS�������� ����Ѵٴ� ���ǰ� 'P(Premium)','S(Standard)','N(No)' ������
		//	 KOLASIII���� 'Y(Yes)','N(No)'��  �����Ѵ� >>
		if		(strTmp == _T("N")) nMngMode = 0;
		else if (strTmp == _T("Y")) nMngMode = 1;
		else	nMngMode = 0;
		//else if (strTmp == _T("S")) nMngMode = 1;
		//else if (strTmp == _T("P")) nMngMode = 2;
		//--2008.11.20 UPDATE BY KSJ --}}
	}
	m_pInfo->SetSMSUseMode(nMngMode);

	// MONETA ���� ��뿩��============================================================
	ids = m_DM.GetOneValueQuery( _T("SELECT VALUE_NAME ")
								 _T("FROM ESL_MANAGE_TBL ")
						         _T("WHERE CLASS_ALIAS='MONETA��뿩��' ")
								 _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), strTmp ); // ADD BY PWR (���սý��� ����)
	if (0>ids && -4007!=ids) 
	{
		sTemp.Format(_T("�ý������� �ٿ�ε��� ����[MONETA][%d]..."), ids);
		AfxMessageBox(sTemp);
		return -1;
	}
	nMngMode = 0;	//default, �ش緹�ڵ尡 ���� ����� ��
	if (-4007!=ids) 
	{
		strTmp.TrimLeft();		
		strTmp.TrimRight();
		strTmp.MakeUpper();
		if		(strTmp == _T("N")) nMngMode = 0;
		else if (strTmp == _T("Y")) nMngMode = 1;
	}
	m_pInfo->SetMONETAUseMode(nMngMode);

	// �����ڰ������=================================================================
	ids = m_DM.GetOneValueQuery( _T("SELECT VALUE_1 ")
								 _T("FROM ESL_MANAGE_TBL ")
								 _T("WHERE CLASS_ALIAS='�������' ")
								 _T("AND VALUE_NAME='�����ڰ�������' ")
								 _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), strTmp ); // ADD BY PWR (���սý��� ����)
	if (0>ids && -4007!=ids) 
	{
		sTemp.Format(_T("�ý������� �ٿ�ε��� ����[�����ڰ���][%d]..."), ids);
		AfxMessageBox(sTemp);
		return -1;
	}
	nMngMode = 0;	//default, �ش緹�ڵ尡 ���� ����� ��
	if (-4007!=ids) 
	{
		strTmp.TrimLeft();		
		strTmp.TrimRight();
		strTmp.MakeUpper();
		if		(strTmp == _T("0")) nMngMode = 0;
		else if (strTmp == _T("1")) nMngMode = 1;
		else if (strTmp == _T("2")) nMngMode = 2;
	}
	m_pInfo->SetLoanUserShareMode(nMngMode);

	// ����ݳ��������=================================================================
	ids = m_DM.GetOneValueQuery( _T("SELECT VALUE_1 ")
								 _T("FROM ESL_MANAGE_TBL ")
								 _T("WHERE CLASS_ALIAS='�������' ")
								 _T("AND VALUE_NAME='����ݳ���������' ")
								 _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), strTmp ); // ADD BY PWR (���սý��� ����)
	if (0>ids && -4007!=ids) 
	{
		sTemp.Format(_T("�ý������� �ٿ�ε��� ����[����ݳ�����][%d]..."), ids);
		AfxMessageBox(sTemp);
		return -1;
	}
	nMngMode = 0;	//default, �ش緹�ڵ尡 ���� ����� ��
	if (-4007!=ids) 
	{
		strTmp.TrimLeft();		
		strTmp.TrimRight();
		strTmp.MakeUpper();
		if		(strTmp == _T("0")) nMngMode = 0;
		else if (strTmp == _T("1")) nMngMode = 1;
		else if (strTmp == _T("2")) nMngMode = 2;
	}
	m_pInfo->SetLoanInfoShareMode(nMngMode);

	// SEARCH_INDEX_TYPE=================================================================
	//		: 'C' - Oracle catsearch ���, 'E' - ECO search ���=========================
	ids = m_DM.GetOneValueQuery( _T("SELECT VALUE_NAME ")
							     _T("FROM ESL_MANAGE_TBL ")
			  			         _T("WHERE CLASS_ALIAS='SEARCH_INDEX_TYPE' ")
								 _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), strTmp ); // ADD BY PWR (���սý��� ����)
	if (0>ids && -4007!=ids) 
	{
		sTemp.Format(_T("�ý������� �ٿ�ε��� ����[SEARCH_TYPE][%d]..."), ids);
		AfxMessageBox(sTemp);
		return -1;
	}

	nMngMode = 0;	//default, �ش緹�ڵ尡 ���� ����� ��
	if (-4007!=ids) 
	{
		strTmp.TrimLeft();		
		strTmp.TrimRight();
		strTmp.MakeUpper();
		if		(strTmp == _T("C")) nMngMode = 0;
		else if (strTmp == _T("E")) nMngMode = 1;
	}
	m_pInfo->SetSearchIndexType(nMngMode);

	// KOL.RED.2018.228 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (nMngMode != 1 && CESL_SearchEngine::GetUsed())
	{
		// �˻������� ����ϴ� ��� ������ ECOSEARCH���� ���۽�Ų��
		m_pInfo->SetSearchIndexType(1);
	}
	// KOL.RED.2018.228 ---------------------------------------------------------------------------

	//KOLAS II Upgrade System Client Version=============================================
	//		: 'U' - Upgrade, 'L' - Light=================================================
	ids = m_DM.GetOneValueQuery( _T("SELECT VALUE_NAME ")
								 _T("FROM ESL_MANAGE_TBL ")
								 _T("WHERE CLASS_ALIAS='LIGHT����' ")
								 _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), strTmp ); // ADD BY PWR (���սý��� ����)

	TCHAR cClientVersion;		

	if (-4007==ids) 
	{

		if (static_cast<CKOLAS3_MAINView*>(m_pParentWnd)->IsDebugMode()) 
		{
			sTemp.Format(_T("�ý������� �ٿ�ε��� ����[KOLAS II Up Version ����][��������]...\n\n")
						  _T("***�������(ServerInstall)*** �� �̿��Ͽ�\n")
						  _T("�ش� �������� KOLAS II Up Version ������ �����Ͽ��� �մϴ�.[MANAGE ��]\n")
						  _T("���� �ش� ������������ ���� ���α׷�(KOLAS II UP.EXE)�� ������ ���� �ڵ����� �α��� �˴ϴ�.\n\n")
						  _T("[_DEBUG���] Upgrade �ý������� �α��� �Ͻðڽ��ϱ�?\n\n")
						  _T("       ->\t'��'(Yes)\t\t: Ugrade �ý������� �α���\n")
						  _T("\t'�ƴϿ�'(No)\t: Light  �ý������� �α���\n")
						  _T("\t'���'(Cancel)\t: �ý��� ����"));
			INT ret = AfxMessageBox(sTemp, MB_YESNOCANCEL);
			if		(IDYES==ret) { cClientVersion = _T('U'); strTmp = _T("N"); }
			else if (IDNO ==ret) { cClientVersion = _T('L'); strTmp = _T("Y"); }
			else return -1;

			ids = 0;
		}
		else
		{
			//2003-1112[�ڱ���] ====================================================
			//sTemp.Format(_T("�ý������� �ٿ�ε��� ����[KIIClient][��������]..."));
			//AfxMessageBox(sTemp);
			//return -1;
			//2003-1112[�ڱ���] ++++++++++++++++++++++++++++++++++++++++++++++++++++
			//������ ������ KOLAS II UP.EXE ���α׷��� ������ �ɼǿ� ���� �α��� �ǵ��� �Ѵ�. <- #######
#ifdef _LIGHT
			strTmp == _T("Y");
#else
			strTmp == _T("N");
#endif
			//2003-1112[�ڱ���] ----------------------------------------------------
		}

	}
	
	if (0>ids && -4007!=ids) 
	{
		sTemp.Format(_T("�ý������� �ٿ�ε��� ����[KOLAS II Up Version ����][%d]..."), ids);
		AfxMessageBox(sTemp);
		return -1;
	}

	strTmp.TrimLeft();		
	strTmp.TrimRight();
	strTmp.MakeUpper();

	if		(strTmp == _T("N")) cClientVersion = _T('U');
	else if (strTmp == _T("Y")) cClientVersion = _T('L');
	else 
	{

		if (static_cast<CKOLAS3_MAINView*>(m_pParentWnd)->IsDebugMode()) 
		{
			sTemp.Format(_T("�ý������� �ٿ�ε��� ����[KOLAS II Up Version �߸��� ����=(%s)]...\n\n")
						  _T("***�������(ServerInstall)*** �� �̿��Ͽ�\n")
						  _T("�ش� �������� KOLAS II Up Version ������ �����Ͽ��� �մϴ�.[MANAGE ��]\n")
						  _T("���� �ش� ������������ ���� ���α׷�(KOLAS II UP.EXE)�� ������ ���� �ڵ����� �α��� �˴ϴ�.\n\n")
						  _T("[_DEBUG���] Upgrade �ý������� �α��� �Ͻðڽ��ϱ�?\n\n")
						  _T("       ->\t'��'(Yes)\t\t: Ugrade �ý������� �α���\n")
						  _T("\t'�ƴϿ�'(No)\t: Light  �ý������� �α���\n")
						  _T("\t'���'(Cancel)\t: �ý��� ����"), strTmp);
			INT ret = AfxMessageBox(sTemp, MB_YESNOCANCEL);
			if		(IDYES==ret) cClientVersion = _T('U');
			else if (IDNO ==ret) cClientVersion = _T('L');
			else return -1;
		}
		else
		{
			//2003-1112[�ڱ���] ====================================================
			//sTemp.Format(_T("�ý������� �ٿ�ε��� ����[KIIClient �߸��� ����=(%s)]..."), strTmp);
			//AfxMessageBox(sTemp);
			//return -1;
			//2003-1112[�ڱ���] ++++++++++++++++++++++++++++++++++++++++++++++++++++
			//������ ������ KOLAS II UP.EXE ���α׷��� ������ �ɼǿ� ���� �α��� �ǵ��� �Ѵ�. <- #######
#ifdef _LIGHT
			cClientVersion = _T('L');
#else
			cClientVersion = _T('U');
#endif
			//2003-1112[�ڱ���] ----------------------------------------------------
		}

	}
	m_pInfo->SetKIIUPClientVersion(cClientVersion);

	return 0;
}
//2003-1106 CESL_Information ���� �۾�-�ڱ���----------------------------------------	


INT CLoginDlg::LoadCodeInfo()
{
	m_pInfo->pCodeMgr->CONNECTION_INFO = CONNECTION_INFO;

	return m_pInfo->pCodeMgr->ReadCodeData(m_pInfo->USER_PK, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg ���۷��� �ٿ�ޱ�..
INT CLoginDlg::DownloadESLReference()
{

	// 15/02/26 �ڴ�� : ������� �������� ������ �߻��ϴ� ���� ���� ó�� 
	//                   ������� M/W �����۾� �߿� �߻��� �����۾����� ���� ���������� �ҽ� �����۾���
	//                   �ʿ��Ͽ� �Լ��� ȣ���ϵǵ��� �����Ͽ���
//// ADD -------------------------------------------------------------------------------------
	m_pInfo->pESLMgr->SetCONNECTION_INFO(CONNECTION_INFO);

	return 0;
//// END -------------------------------------------------------------------------------------

	/**
		ESL REFERENCE�� �ٿ� �޴´�.
	*/

	// esl_mgr_tbl�� esl_mgr_sub_tbl�� Join �� Table�� ���� �´�.
	m_pInfo->pESLMgr->SetCONNECTION_INFO(CONNECTION_INFO);
	
	CString strName;
	INT nVersion, nPrevVersion;
	BOOL bIsLoadRef = IsUpdateESLReference(strName, nVersion, nPrevVersion);
	if (bIsLoadRef)
	{		
		FILE *fd = _tfopen(_T("..\\CFG\\_DEBUG"), _T("rb"));
		if (fd) {
			fclose(fd);
			if (AfxMessageBox(_T("Reference�� �ٽ� �ٿ� �����ðڽ��ϱ�?\r\n\r\n����� DEBUG ��� �Դϴ�.(Release���� ��ȯ�Ͻ÷��� CFG/_DEBUG ������ �����Ͽ� �ֽʽÿ�.)"), MB_YESNO) != IDYES) return 0;
		}

		CESL_ReferenceMgr rm;
		rm.SetCONNECTION_INFO(m_DM.CONNECTION_INFO);
		if (nVersion == 0) {
			rm.FULLDownLoad(m_pInfo, &m_ProgressCtrl);
		} else {
			if (nPrevVersion == 0) {
				rm.FULLDownLoad(m_pInfo, &m_ProgressCtrl);
			} else {
				rm.OPTIMIZEDownLoad(m_pInfo, &m_ProgressCtrl);
			}
		}

		UpdateESLReferenceVersionInfo(strName, nVersion);
	}

	return 0;
}

BOOL CLoginDlg::IsUpdateESLReference(CString &strName, INT &nVersion, INT &nPrevVersion)
{
	LPCTSTR lpszFileName = _T("..\\CFG\\Reference\\Version.cfg");
	
	INT nFind = -1;
	CString strTemp;
	CString strLine;

	nPrevVersion = 0;
	CStdioFile file;
	if (file.Open(lpszFileName, CFile::modeRead|CFile::typeBinary))
	{
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
		
		while (file.ReadString(strLine))
		{
			strLine.TrimLeft();
			strLine.TrimRight();

			if (strLine.IsEmpty() || strLine.Left(1) == _T('.') || strLine.Left(1) == _T('#')) continue;

			nFind = strLine.Find('=');
			if (nFind < 0) continue;

			strName = strLine.Mid(0, nFind);
			nVersion = _ttoi(strLine.Mid(nFind + 1));
			nPrevVersion = nVersion;
		}
	} else {
		nPrevVersion = 0;
	}

	strName.TrimLeft();
	strName.TrimRight();
	if (strName.IsEmpty())
		strName = _T("ESL_REFERENCE");
	
	CString strQuery;
	CString strResult;
	strQuery.Format(_T("SELECT VERSION FROM ESL_VERSION_TBL WHERE NAME = '%s'"), strName, nVersion);
	if (m_DM.GetOneValueQuery(strQuery, strResult) < 0) return FALSE;
		
	nVersion = _ttoi(strResult);
	if (nPrevVersion != nVersion) return TRUE;
	return FALSE;
}

VOID CLoginDlg::UpdateESLReferenceVersionInfo(LPCTSTR lpcszName, const INT nVersion)
{
	FILE *fp = _tfopen(_T("..\\CFG\\Reference\\Version.cfg"), _T("wb"));
	if (fp == NULL) return;

		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
	
	_ftprintf(fp, _T("%s=%d"), lpcszName, nVersion);

	fclose(fp);
}

INT CLoginDlg::DownloadDVM()
{
	m_pInfo->pESLDataVerifyMgr->SetCONNECTION_INFO(CONNECTION_INFO);
    return m_pInfo->pESLDataVerifyMgr->RestructDataManager(_T("select dvm.pk, dvm.db_verify_mgr_alias, dvm.connection_info, dvm.tbl_name, dvm.condition, dvm.extra_condition, dvms.field_alias, dvms.field_name, dvms.field_type, dvms.init_value, dvms.data_type, dvms.filter1, dvms.filter2 from esl_db_verify_mgr_tbl dvm, esl_db_verify_mgr_sub_tbl dvms where dvm.pk=dvms.db_verify_mgr_pk"));
}

INT CLoginDlg::DownloadCharSetData()
{
	// ������ üũ�ؼ� ������ ���� ������ �ٿ�ε� �Ѵ�.
	CString strVersionFilePath = _T("..\\cfg\\CharSetVersion.cfg");
	
	// ��ġ�Ѵ�.
	CStdioFile file;
	CString strLine;
	if (!file.Open(strVersionFilePath, CFile::modeRead|CFile::typeBinary))
	{
		if (!file.Open(strVersionFilePath, CFile::modeReadWrite | CFile::modeCreate|CFile::typeBinary))
			return -1;
			
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}	
		
		file.Close();
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	CMap<CString, LPCTSTR, INT, INT> map;
	
	INT nFind = -1;
	while (file.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();

		if (strLine.IsEmpty()) continue;
		if (strLine.Left(1) == '.' || strLine.Left(1) == '#') continue;

		nFind = strLine.Find('=');
		if (nFind < 0) continue;

		map.SetAt(strLine.Mid(0, nFind), _ttoi(strLine.Mid(nFind + 1)));
	}

	file.Close();
	// ���� ������ �����.
	DeleteFile(strVersionFilePath);

	// ���� ���̺�� �������ϰ��� ��ũ�� �����.
	//CString strQuery = _T("SELECT CHARSET_NAME, VERSION, FROM_SET, TO_SET, FILE_NAME FROM CO_CHARSET_MANAGEMENT_TBL WHERE DOWN_FLAG = 'T'");
	CString strQuery = _T("SELECT REC_KEY, CHARSET_NAME, VERSION, FROM_SET, TO_SET, FILE_NAME, FILE_LEN FROM CO_CHARSET_MANAGEMENT_TBL WHERE DOWN_FLAG = 'T'");
	if (m_DM.RestructDataManager(strQuery) < 0) return -1;

	CString strManageFilePath = _T("..\\cfg\\_CO_CHARSET_MANAGEMENT_TBL.txt");
	DeleteFile(strManageFilePath);


	CString strCharSetName;
	CString strVersion;

	FILE *fVersionFile = NULL;			// �������� ���� ������
	FILE *fManageFile = NULL;
	INT nVersion;
	INT nRowCount = m_DM.GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strCharSetName = m_DM.GetCellData(idx, 1);
		strVersion = m_DM.GetCellData(idx, 2);

		if (strCharSetName.IsEmpty() || strVersion.IsEmpty()) continue;

		// _CO_CHARSET_MANAGEMENT_TBL.TXT �� FROM_SET TO_SET FILE_NAME�� �ִ´�.
		fManageFile = _tfopen(strManageFilePath, _T("a+b"));
		strLine.Format(_T("%-30s|%-30s|..\\cfg\\%s|\r\n"), m_DM.GetCellData(idx, 3), m_DM.GetCellData(idx, 4), m_DM.GetCellData(idx, 5));
		if (fManageFile) {
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		// KOLAS 2 UP UNICODE �����۾�
		// ���� �����ÿ��� UNICODE ����� �Է��ؾ��Ѵ�.
		fseek(fManageFile, 0, SEEK_END);		
		if( 0 == ftell(fManageFile) )
		{
			/*UNCHANGE*/fputc(0xFF, fManageFile);
			/*UNCHANGE*/fputc(0xFE, fManageFile);
		}
		// ------------------------------------------------------------------------------
			
			_ftprintf(fManageFile, _T("%s"), strLine);
			fclose(fManageFile);
		}

		if (!map.Lookup(strCharSetName, nVersion))
		{
			map.SetAt(strCharSetName, -1);		
			nVersion = -1;	
		}

		/// ���� ��
		if (nVersion < _ttoi(strVersion))
		{
			if (DownloadCharSetData(m_DM.GetCellData(idx, 0), _T("..\\CFG\\") + m_DM.GetCellData(idx, 5), _ttoi(m_DM.GetCellData(idx, 6)))) return -1;
			// ���� ���� ������ ������Ʈ �Ѵ�.
			map.SetAt(strCharSetName, _ttoi(strVersion));
			nVersion = _ttoi(strVersion);
		}
		fVersionFile = _tfopen(strVersionFilePath, _T("a+b"));
		if (fVersionFile) {
			
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		// KOLAS 2 UP UNICODE �����۾�
		// ���� �����ÿ��� UNICODE ����� �Է��ؾ��Ѵ�.
		fseek(fVersionFile, 0, SEEK_END);		
		if( 0 == ftell(fVersionFile) )
		{
			/*UNCHANGE*/fputc(0xFF, fVersionFile);
			/*UNCHANGE*/fputc(0xFE, fVersionFile);
		}
		// ------------------------------------------------------------------------------
			
			_ftprintf(fVersionFile, _T("%s=%d\r\n"), strCharSetName, nVersion);
			fclose(fVersionFile);
		}
	}
	return 0;
}

INT CLoginDlg::DownloadCharSetData(CString strRecKey, CString strFileName, INT nFileSize)
{
	if (strRecKey.IsEmpty()) return -1;

	INT ids = -1;
	CFile file;
	
	INT nBufferSize = sizeof(BYTE) * nFileSize + 1024;
	BYTE *pBuffer = (BYTE*)malloc(nBufferSize);
	ZeroMemory(pBuffer, sizeof(BYTE) * nBufferSize);
	if (pBuffer == NULL)
	{
		MessageBox(_T("Malloc Memory Fail!"));
		return -1;
	}

	ids = m_DM.LOBGet(_T("CO_CHARSET_MANAGEMENT_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey, _T("FILE_DATA"), pBuffer, &nBufferSize);
	if (ids < 0) goto ERR;
		
	//if (nBufferSize <= 0) goto END;
	
	// ���Ͽ� �����Ѵ�.
	if (!file.Open(strFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) goto ERR;

	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}	
		
	file.WriteHuge(pBuffer, nBufferSize);
	file.Close();	
	goto END;

END:
	free(pBuffer);
	return 0;

ERR:
	CString strMsg;
	strMsg.Format(_T("[%s] �ٿ�ε忡 �����߽��ϴ�."), strFileName);
	MessageBox(strMsg);
	free(pBuffer);
	return -1;
}

INT CLoginDlg::DownloadManagementData()
{
	// ������ üũ�ؼ� ������ ���� ������ �ٿ�ε� �Ѵ�.
	CString strVersionFilePath = _T("..\\cfg\\ManagementVersion.cfg");
	
	// ��ġ�Ѵ�.
	CStdioFile file;
	CString strLine;
	if (!file.Open(strVersionFilePath, CFile::modeRead|CFile::typeBinary))
	{
		if (!file.Open(strVersionFilePath, CFile::modeReadWrite | CFile::modeCreate | CFile::typeBinary))
			return -1;
			
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}	
	}

	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	CMap<CString, LPCTSTR, INT, INT> map;
	
	INT nFind = -1;
	while (file.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();

		if (strLine.IsEmpty()) continue;
		if (strLine.Left(1) == '.' || strLine.Left(1) == '#') continue;

		nFind = strLine.Find('=');
		if (nFind < 0) continue;

		map.SetAt(strLine.Mid(0, nFind), _ttoi(strLine.Mid(nFind + 1)));
	}

	file.Close();
	// ���� ������ �����.
	DeleteFile(strVersionFilePath);

	// ��ũ �׷� ���̺�� �������ϰ��� ��ũ�� �����.
	CString strQuery = _T("SELECT REC_KEY, ALIAS, VERSION, FILE_NAME, FILE_SIZE FROM MN_MANAGE_FILE_TBL");
	if (m_DM.RestructDataManager(strQuery) < 0) return -1;

	CString strFileName;
	CString strAlias;
	CString strVersion;

	FILE *fVersionFile = NULL;
	INT nVersion;
	INT nRowCount = m_DM.GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strAlias = m_DM.GetCellData(idx, 1);
		strVersion = m_DM.GetCellData(idx, 2);

		if (strAlias.IsEmpty() || strVersion.IsEmpty()) continue;

		if (!map.Lookup(strAlias, nVersion))
		{
			map.SetAt(strAlias, -1);
			nVersion = -1;
		}

		/// ���� ��
		if (nVersion < _ttoi(strVersion))
		{
			strFileName.Format(_T("..\\cfg\\%s"), m_DM.GetCellData(idx, 3));
			if (DownloadManagementData(m_DM.GetCellData(idx, 0), strFileName, _ttoi(m_DM.GetCellData(idx, 4)))) return -1;
			// ���� ���� ������ ������Ʈ �Ѵ�.
			map.SetAt(strAlias, _ttoi(strVersion));
			nVersion = _ttoi(strVersion);
		}
		fVersionFile = _tfopen(strVersionFilePath, _T("a+b"));
		if (fVersionFile) {
			
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		// KOLAS 2 UP UNICODE �����۾�
		// ���� �����ÿ��� UNICODE ����� �Է��ؾ��Ѵ�.
		fseek(fVersionFile, 0, SEEK_END);		
		if( 0 == ftell(fVersionFile) )
		{
			/*UNCHANGE*/fputc(0xFF, fVersionFile);
			/*UNCHANGE*/fputc(0xFE, fVersionFile);
		}
		// ------------------------------------------------------------------------------
			
			_ftprintf(fVersionFile, _T("%s=%d\r\n"), strAlias, nVersion);
			fclose(fVersionFile);
		}
	}
	
	return 0;
}

INT CLoginDlg::DownloadManagementData(CString strRecKey, CString strFileName, INT nFileSize)
{
	if (strRecKey.IsEmpty()) return -1;

	INT ids = -1;
	CFile file;
	
	INT nBufferSize = sizeof(BYTE) * nFileSize + 1024;
	BYTE *pBuffer = (BYTE*)malloc(nBufferSize);
	ZeroMemory(pBuffer, sizeof(BYTE) * nBufferSize);
	if (pBuffer == NULL)
	{
		MessageBox(_T("Malloc Memory Fail!"));
		return -1;
	}

	ids = m_DM.LOBGet(_T("MN_MANAGE_FILE_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey, _T("FILE_DATA"), pBuffer, &nBufferSize);
	if (ids < 0) goto ERR;
		
	//if (nBufferSize <= 0) goto END;
	
	// ���Ͽ� �����Ѵ�.
	if (!file.Open(strFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) goto ERR;

	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}
		
	file.WriteHuge(pBuffer, nBufferSize);
	file.Close();	
	goto END;


END:
	free(pBuffer);
	return 0;

ERR:
	CString strMsg;
	strMsg.Format(_T("[%s] �ٿ�ε忡 �����߽��ϴ�."), strFileName);
	MessageBox(strMsg);
	free(pBuffer);
	return -1;
}

INT CLoginDlg::DownloadMarcRelation()
{
	EFS_BEGIN
	
	// ������ üũ�ؼ� ������ ���� ������ �ٿ�ε� �Ѵ�.
	CString strVersionFilePath = _T("..\\cfg\\MarcRelation\\Version.cfg");
	
	// ��ġ�Ѵ�.
	CStdioFile file;
	CString strLine;
	if (!file.Open(strVersionFilePath, CFile::modeRead | CFile::typeBinary))
	{
		if (!file.Open(strVersionFilePath, CFile::modeReadWrite | CFile::modeCreate | CFile::typeBinary))
			return -1;
			
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}
	}
	
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	CMap<CString, LPCTSTR, INT, INT> map;
	
	INT nFind = -1;
	while (file.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();

		if (strLine.IsEmpty()) continue;
		if (strLine.Left(1) == '.' || strLine.Left(1) == '#') continue;

		nFind = strLine.Find('=');
		if (nFind < 0) continue;

		map.SetAt(strLine.Mid(0, nFind), _ttoi(strLine.Mid(nFind + 1)));
	}

	file.Close();
	DeleteFile(strVersionFilePath);

	// ��ũ �׷� ���̺�� �������ϰ��� ��ũ�� �����.
	CString strQuery = _T("SELECT PK, MARC_GROUP_CODE, VERSION, SIMPLE_FILE_SIZE FROM MARC_GROUP_TBL");
	if (m_DM.RestructDataManager(strQuery) < 0) return -1;

	CString strFileName;
	CString strMarcGroup;
	CString strVersion;

	FILE *fVersionFile;
	INT nVersion;
	INT nRowCount = m_DM.GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strMarcGroup = m_DM.GetCellData(idx, 1);
		strVersion = m_DM.GetCellData(idx, 2);

		if (strMarcGroup.IsEmpty() || strVersion.IsEmpty()) continue;

		if (!map.Lookup(strMarcGroup, nVersion))
		{
			map.SetAt(strMarcGroup, -1);
			nVersion = -1;
		}

		/// ���� ��
		if (nVersion < _ttoi(strVersion))
		{
			strFileName.Format(_T("..\\cfg\\MarcRelation\\SimpleRelation_%s.txt"), strMarcGroup);
			if (DownloadMarcRelation(m_DM.GetCellData(idx, 0), strFileName, _ttoi(m_DM.GetCellData(idx, 3)))) return -1;
			// ���� ���� ������ ������Ʈ �Ѵ�.
			map.SetAt(strMarcGroup, _ttoi(strVersion));
			nVersion = _ttoi(strVersion);
		}
		fVersionFile = _tfopen(strVersionFilePath, _T("a+b"));
		if (fVersionFile) {
			
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		// KOLAS 2 UP UNICODE �����۾�
		// ���� �����ÿ��� UNICODE ����� �Է��ؾ��Ѵ�.
		fseek(fVersionFile, 0, SEEK_END);		
		if( 0 == ftell(fVersionFile) )
		{
			/*UNCHANGE*/fputc(0xFF, fVersionFile);
			/*UNCHANGE*/fputc(0xFE, fVersionFile);
		}
		// ------------------------------------------------------------------------------
			
			_ftprintf(fVersionFile, _T("%s=%d\r\n"), strMarcGroup, nVersion);
			fclose(fVersionFile);
		}
	}

	EFS_END
	return 0;
}

INT CLoginDlg::DownloadMarcRelation(CString strRecKey, CString strFileName, INT nFileSize)
{
	if (strRecKey.IsEmpty()) return -1;

	INT ids = -1;
	CFile file;
	
	INT nBufferSize = sizeof(BYTE) * nFileSize + 1024;
	BYTE *pBuffer = (BYTE*)malloc(nBufferSize);
	ZeroMemory(pBuffer, sizeof(BYTE) * nBufferSize);
	if (pBuffer == NULL)
	{
		MessageBox(_T("Malloc Memory Fail!"));
		return -1;
	}

	ids = m_DM.LOBGet(_T("MARC_GROUP_TBL"), _T("PK"), _T("NUMERIC"), strRecKey, _T("SIMPLE_FILE_DATA"), pBuffer, &nBufferSize);
	if (ids < 0) goto ERR;
		
	//if (nBufferSize <= 0) goto END;
	
	// ���Ͽ� �����Ѵ�.
	if (!file.Open(strFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) goto ERR;
	
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}	
		
	file.WriteHuge(pBuffer, nBufferSize);
	file.Close();	
	goto END;


END:
	free(pBuffer);
	return 0;

ERR:
	CString strMsg;
	strMsg.Format(_T("[%s] �ٿ�ε忡 �����߽��ϴ�."), strFileName);
	MessageBox(strMsg);
	free(pBuffer);
	return -1;
}


/////////////////////////////////////////////////////////////////////////////
// CLoginDlg message handlers

BOOL CLoginDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	if (m_pInfo == NULL) 
	{
		MessageBox(_T("m_pInfo IS NULL"));
		EndDialog(IDCANCEL);
		return FALSE;
	}
		
	FILE *fp = NULL;

	// �ڵ��α���
	fp =  _tfopen(_T("..\\CFG\\_DEBUG"), _T("rb"));
	if (fp != NULL) 
	{
		fclose(fp);
		SetTimer(0, 100, NULL);		
	}

	// ����� �������� �α���
	else
	{
		fp =  _tfopen(_T("..\\CFG\\UserLoginInfo.cfg"), _T("rb"));
		if(fp != NULL)
		{
			fclose(fp);
			if(m_strUser.IsEmpty() || m_strUser.Left(1) == _T("!") || m_strUser.Left(1) == _T("@") ) //K-BAR�� �α������� �ʴ� ���!!
				SetTimer(5, 100, NULL);
		}
	}

	if ( !m_strUser.IsEmpty() && ( m_strUser.Left(1) != _T("!") && m_strUser.Left(1) != _T("@")) )
	{		
		INT		nOffset;
		INT     nIndex;

		nOffset = m_strUser.Find(_T(" "));
		m_strID = m_strUser.Left(nOffset);
		nIndex = nOffset+1;
		nOffset = m_strUser.Find(_T(" "), nIndex);	
		if ( nOffset == -1 )
		{
			m_strPassword = m_strUser.Mid(nIndex);
			m_AutoLoadMenu = _T("");
		}
		else
		{		
			m_strPassword = m_strUser.Mid(nIndex, nOffset - nIndex);
			m_AutoLoadMenu = m_strUser.Mid(nOffset+1);
		}
		SetTimer(0, 100, NULL);		
	}

	// 2005.10.11 ADD BY PDW(Jr)
	// ���ϸ��α���
	fp =  _tfopen(_T("..\\CFG\\_���ϸ�����"), _T("rb"));
	if (fp != NULL) 
	{
		SetTimer(3, 100, NULL);
		fclose(fp);
	}

	m_DM.SetDB(DBKIND);
	m_DM.SetCONNECTION_INFO(CONNECTION_INFO);

	// ���α׷����� ����
	m_ProgressCtrl.SetFont(GetFont());
	m_ProgressCtrl.SetWindowText(_T("���̵�� �н����带 �Է��ϼ���."));
	m_ProgressCtrl.SetForeColour(RGB(0,128,255));
	m_ProgressCtrl.SetBkColour(RGB(0,0,0));
	m_ProgressCtrl.SetTextForeColour(RGB(64,224,208));

	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt(_T("Style"), _T("Value"), 0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CLoginDlg::OnTimer(UINT nIDEvent) 
{
	// ����� �������� �α���
	if (nIDEvent == 5) 
	{
		KillTimer(5);
		CStdioFile file;
		CString sLoginInfo;
		CFWDES DES;		// �Ϻ�ȣȭ Ŭ����
		DES.Read_DES_File(
			_T("..\\CFG\\UserLoginInfo.cfg"),
			_T("k^Ty"),	// ��ȣŰ
			sLoginInfo	// ��ȣȭ�� ����
		);	
		// ���̵�� ��ȣ�� �о���δ�.
		INT nPos = sLoginInfo.Find(_T("\n"));
		m_strID = sLoginInfo.Left(nPos);
		m_strID.TrimLeft(); m_strID.TrimRight();
		nPos+=1;
		m_strPassword = sLoginInfo.Mid(nPos);
		m_strPassword.TrimLeft(); m_strPassword.TrimRight();

		// �о���� ���̵� OR ��ȣ�� NULL�� �ƴѰ�� üũ�ڽ� CHK!!
		if(!m_strID.IsEmpty())
		{
			((CButton*)(GetDlgItem(IDC_CHK_ID)))->SetCheck(TRUE);
			//((CButton*)(GetDlgItem(IDC_CHK_ID)))->SetButtonStyle(BS_CHECKBOX);
			// ���̵�� ������ ��ȣ�� NULL�� ��� ��ȣ EDTBOX�� ��Ŀ��!!
			if(m_strPassword.IsEmpty())
			{
				((CEdit*)GetDlgItem(IDC_ePASSWORD))->SetFocus();
			}
		}
		if(!m_strPassword.IsEmpty())
		{
			((CButton*)(GetDlgItem(IDC_CHK_PWD)))->SetCheck(TRUE);
		}
		
		// EDTBOX�� �о���� ���̵� �� ��ȣ SET!!
		((CEdit*)GetDlgItem(IDC_eID))->SetWindowText(m_strID);
		((CEdit*)GetDlgItem(IDC_ePASSWORD))->SetWindowText(m_strPassword);
		
		m_bUserLoginMode = TRUE;
	}
	
	// �ڵ��α���
	if (nIDEvent == 0) {
		KillTimer(0);
		
		// 2005.11.15 ADD BY PDJ
		// ���ϴ� ���̵�� �ڵ� ����
		CStdioFile fFile;
		CString sLine;
		if ( fFile.Open( _T("..\\CFG\\_DEBUG"), CFile::modeRead | CFile::typeBinary ) )
		{
			TCHAR cUni;
			fFile.Read( &cUni, sizeof(TCHAR) );
			if( 0xFEFF != cUni )
			{
				fFile.SeekToBegin();
			}

			sLine.TrimRight();
			if ( fFile.ReadString( sLine ) ) m_strID = sLine;
			if ( fFile.ReadString( sLine ) ) m_strPassword = sLine;
			fFile.Close();
			if ( m_strID.IsEmpty() ) 
			{
				m_strID = _T("TKATLDH");
				m_strPassword = _T("ECO8369");				
			}
		}	

		if ( m_strID.IsEmpty() ) 
		{
			m_strID = _T("");
			m_strPassword = _T("");
		}	

		UpdateData(false);
		SetTimer(1, 500, NULL);		
	}
	if (nIDEvent == 1)
	{
		KillTimer(1);
		PostMessage(WM_KEYDOWN, VK_RETURN, 0);
	}
	// 2005.10.11 ADD BY PDW(Jr)
	// ���ϸ��α���
	if (nIDEvent == 3)
	{
		KillTimer(3);
		m_strID = _T("���ϸ�");
		m_strPassword = _T("apdlffld");
		UpdateData(false);
		SetTimer(1, 500, NULL);
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN )
		{
			m_bGPKI = FALSE;
			BeginLogin();
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE)
		{
			EndLogin(IDCANCEL);
			return FALSE;
		}
		
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

INT CLoginDlg::BeginLogin()
{
	if (m_bIsThreadRun == TRUE) return -1;
		
	// << 2015�� ���������� �ý��� ���� 2015/11/23 _LCM : GPKI ������ �α��� ���� ����
	if(!m_bGPKI)
	{
		UpdateData();
		if (m_strID.IsEmpty() || m_strPassword.IsEmpty()) return -1;
	}
	// >> 2015/11/23 --MOD

	((CEdit*)GetDlgItem(IDC_eID))->SetReadOnly();
	((CEdit*)GetDlgItem(IDC_ePASSWORD))->SetReadOnly();

	//����� �������� �α����� ���(K-BAR �α����� �ƴѰ��)
	if( m_bUserLoginMode )
	{
		//����� ���������� �����Ѵ�.
		
		//DES Algorithm�� ������ �ִ°�� �̾�� ������ ���� ������ �����!!
		FILE *fp = NULL;
		CString sLogInfoFilePath;
		sLogInfoFilePath = _T("..\\CFG\\UserLoginInfo.cfg");
		fp =  _tfopen(sLogInfoFilePath, _T("rb"));
		if (fp != NULL) 
		{
			fclose(fp);
			CFile::Remove(sLogInfoFilePath);
		}

		//���̵� �� ��ȣ�� ��ȣȭ�Ѵ�. (üũ�Ǿ� ���� ���� ��� NULL���� �ѱ��.)
		CFWDES des;
		CString sLoginInfo,sInputID,sInputPWD;
		INT bCheckID,bCheckPSW;
		bCheckID = ((CButton*)(GetDlgItem(IDC_CHK_ID)))->GetCheck();
		bCheckPSW = ((CButton*)(GetDlgItem(IDC_CHK_PWD)))->GetCheck();
		if(bCheckID == 1)
		{
			sInputID = m_strID;
		}
		if(bCheckPSW == 1)
		{
			sInputPWD = m_strPassword;
		}
		sLoginInfo.Format(_T("%s\n%s") , sInputID , sInputPWD);
		des.Write_DES_File(_T("..\\CFG\\UserLoginInfo.cfg"),_T("k^Ty"),sLoginInfo);
		////////////////////////////////////
	}
	// ������� �α��� ���μ����� �����Ѵ�.
	AfxBeginThread(ThreadFunc_LoginProcess, this);
	
	return 0;
}


INT CLoginDlg::EndLogin(INT nResult) 
{
	if (m_bIsThreadRun)
	{
		m_bIsThreadRun = FALSE;
		MSG msg;
		while (TRUE)
		{
			if (::PeekMessage(&msg,GetSafeHwnd(), 1, 1, PM_NOREMOVE))
				AfxGetApp()->PumpMessage();

			if (WaitForSingleObject(m_hThread, 0) == WAIT_OBJECT_0)
				break;
		}
	}

	if (GetSafeHwnd())
		EndDialog(nResult);
	return 0;
}

VOID CLoginDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	dc.SetBkMode(TRANSPARENT);
	m_bmpLogin.DrawDIB(&dc, 0, 0);

	// << 2015�� ���������� �ý��� ���� 2015/11/10 _LCM : GPKI �α��� �̹��� ����
	HBITMAP hBmp = 0;
	BITMAP bitmap;
	hBmp = (HBITMAP)LoadImage(0, _T("..\\bmp\\btn_gpki1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	GetObject(hBmp, sizeof(BITMAP), &bitmap);

	((CButton*)(GetDlgItem(IDC_BUTTON_GPKI_LOGIN)))->SetBitmap(hBmp);
	// >> 2015/11/10 _LCM --MOD
	// Do not call CDialog::OnPaint() for painting messages
}

UINT CLoginDlg::OnCloseDialog(WPARAM wParam, LPARAM lParam)
{
	if (m_bIsThreadRun)
	{
		m_bIsThreadRun = FALSE;
		MSG msg;
		while (TRUE)
		{
			if (::PeekMessage(&msg,GetSafeHwnd(), 1, 1, PM_NOREMOVE))
				AfxGetApp()->PumpMessage();

			if (WaitForSingleObject(m_hThread, 0) == WAIT_OBJECT_0)
				break;
		}
	}

	EndLogin(wParam);
	
	return 0;
}


INT CLoginDlg::DownLoadMenuToolbar()
{

	static_cast<CKOLAS3_MAINView*>(m_pParentWnd)->LoadMenuRef();
	static_cast<CKOLAS3_MAINView*>(m_pParentWnd)->LoadToolbarRef();
	static_cast<CKOLAS3_MAINView*>(m_pParentWnd)->LoadToolGroupRef();
/*
	CString strVersionFilePath ;
    strVersionFilePath = _T("..\\cfg\\menutoolbar\\version.cfg") ; 

	BOOL bCheck ;


    CFileFind ff;
    if( ff.FindFile(strVersionFilePath) )
    {
         bCheck = TRUE ;
    }
	else bCheck = FALSE ;
	


	CMenuToolbar_Mgr MenuToolbar_Mgr(NULL) ;
	MenuToolbar_Mgr.SetConnectionInfo (m_pInfo->CONNECTION_INFO );
	MenuToolbar_Mgr.SetDefalutValue (m_strID ,_T("U"),_T("y"));
	MenuToolbar_Mgr.SetUploadValue (TRUE,TRUE,TRUE) ;
    
	
	if(bCheck == FALSE)
	{
		//AfxMessageBox(_T("Menu&Toolbar File �������� �ð��� ���� �˴ϴ�."));
		MenuToolbar_Mgr.Upload_File () ;
		MenuToolbar_Mgr.Make_Version_File ();
		MenuToolbar_Mgr.Download_File () ;
	}
	else 
	{
		MenuToolbar_Mgr.Download_File () ;
	}

	static_cast<CKOLAS3_MAINView*>(m_pParentWnd)->LoadMenuRef_2();
	static_cast<CKOLAS3_MAINView*>(m_pParentWnd)->LoadToolbarRef_2();
	static_cast<CKOLAS3_MAINView*>(m_pParentWnd)->LoadToolGroupRef_2();
*/ 
	return 0 ;
}


//=====================================================
//2008.11.05 ADD BY PJW : ��ϱ����ڵ��� LENGTH�� ���Ͽ� �����Ѵ�.
INT CLoginDlg::SaveRegCodeLengthToFile()
{
	// EFS�� ���� USER_INFO�� ���Ͽ� ����
	CString strResult[2];
	CStdioFile file;
	if (file.Open(_T("..\\Cfg\\RegCodeLength.TXT"), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}

		CString strLine;
		strLine.Format(_T("��ϱ����ڵ��ڸ���|%d\r\n"), m_pInfo->nREG_CODE_LENGTH);
		file.WriteString(strLine);

		file.Close();
	}
	return 0;
}
//=====================================================

void CLoginDlg::OnSetfocuseID() 
{
	// TODO: Add your control notification handler code here
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_eID)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion = dwConversion & ~IME_CMODE_LANGUAGE;
		dwConversion |= IME_CMODE_NATIVE;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_eID)->m_hWnd,hIME); 
    }
}


// 2013.10.08 ADD BY BJS : 2013���������ռ���Ȯ�뱸��װ��� (�Ϲ�ġ �α׻���)
// 6���� ���� �α� ����
void CLoginDlg::ControlLog()
{
	CCfgFileApi m_CfgFileApi;
	CString filename = _T("");
	CString filepath = _T("");
	CString strDir = _T("");
	CString strDeleteFile = _T("");
	CString strFileTime = _T("");
	CString strDeleteDelay = _T("");
	CString strCurrentDay = _T("");
	CString strGetCfgDay = _T("");
	INT nDeleteDelay;
	CFileFind finder;
	CFile file;

	//���� �ǽ��Ͽ������� �˻�

	CString strRunCheck = _T("..\\cfg\\�α׻�������.cfg");
	m_CfgFileApi.SetFile(strRunCheck);
	CFileFind findFile;
	CTime t = CTime::GetCurrentTime();
	strCurrentDay.Format(_T("%04d-%02d-%02d"),t.GetYear(),t.GetMonth(),t.GetDay());	
	if(!findFile.FindFile(strRunCheck))
	{
		FILE *ff = _tfopen(strRunCheck, _T("w+b"));
		fputc(0xFF, ff); 
		fputc(0xFE, ff);
		fclose(ff);
		m_CfgFileApi.SetData(_T("�����˻���"), strCurrentDay);
	}
	findFile.Close();

	//�����˻����� ��¥�� ���� ��¥�� ��, �����˻����� ������ �ƴ� ��쿡�� �α� ������ �ǽ��Ѵ�.
	m_CfgFileApi.GetData(_T("�����˻���"), strGetCfgDay);
	if(strGetCfgDay != strCurrentDay)
	{

		//�α� �����ֱ� ���ϰ˻�

		CString strCfgFilePath = _T("..\\cfg\\�α׻����ֱ�.cfg");
		m_CfgFileApi.SetFile(strCfgFilePath);		
		CFileFind find;
	
		// ������ ���ٸ� �����ϰ� �⺻��(180��)���� ���
		if(!find.FindFile(strCfgFilePath))
		{ 
			FILE *fp = _tfopen(strCfgFilePath, _T("w+b"));
			fputc(0xFF, fp); 
			fputc(0xFE, fp);
			fclose(fp);
			m_CfgFileApi.SetData(_T("�����ֱ�"), _T("180"));
		}
		find.Close();
	
		//cfg ���� ���� _log ���� ����
		if(find.FindFile(_T("..\\cfg\\_log")))
		{
			DeleteFile(_T("..\\cfg\\_log"));	
		}
		find.Close();	
	
		//cfg ���� ���� _framelog ���� ����
		if(find.FindFile(_T("..\\cfg\\_framelog")))
		{
			DeleteFile(_T("..\\cfg\\_framelog"));
		}
		find.Close();
	
		strDir = _T("..\\tunning\\*.*");
		filepath = _T("..\\tunning\\");
		BOOL bWorking = finder.FindFile(strDir);
		
		//cfg ���� ���� �α׻����ֱ�.txt ������ ���� ���� �ֱ⸦ �����´�.
		m_CfgFileApi.GetData(_T("�����ֱ�"), strDeleteDelay);
		nDeleteDelay = _ttoi(strDeleteDelay);
		
	
		while(bWorking)
		{
			bWorking = finder.FindNextFile();
			filename = finder.GetFileName();			
			
			//������ �ð��� ���� �ð��� ���� ���̸� ���Ѵ�.
			CString strFilenameStart;
			CString strFilenameEnd;
	
			strDeleteFile.Format(_T("%s%s"),filepath,filename);
			strFilenameStart = filename.Left(3);
			strFilenameEnd = filename.Right(3);
			if(strFilenameStart == _T("ESL"))
			{
				strFileTime = filename.Mid(7,8);
				
				// 15/05/21 �ڴ�� : 8�ڸ��� ���� �������� Ȯ��
//// ADD -------------------------------------------------------------------------------------
				INT nTimeIdx = 0;
				INT nTimeLen = strFileTime.GetLength();
				for (; nTimeIdx < nTimeLen; nTimeIdx++)
				{
					TCHAR ch = strFileTime[nTimeIdx];
					if (ch < '0' || ch > '9')
						break;
				}
				if (nTimeIdx < nTimeLen)
					continue;
//// END -------------------------------------------------------------------------------------

				CTime tCurrentTime = CTime::GetCurrentTime();
				CTime tRenewalTime(_ttoi(strFileTime.Left(4)),_ttoi(strFileTime.Mid(4,2)),_ttoi(strFileTime.Right(2)),0,0,0);
				CTimeSpan tSpan = tCurrentTime - tRenewalTime;
				INT nElapseDay = (INT)tSpan.GetDays();
				//���̰� cfg ���Ͽ� ������ ���ں��� Ŭ ���, �α׸� �����Ѵ�.
				if(nDeleteDelay < nElapseDay)
				{	
					DeleteFile(strDeleteFile);	
				}
			}			
			//tunning ���� ���� txt ���� ��� ����
			if(strFilenameEnd == _T("txt"))	DeleteFile(strDeleteFile);
			//tunning ���� ���� log ���� ��� ����
			if(strFilenameEnd == _T("log")) DeleteFile(strDeleteFile);			
		}		
		finder.Close();		
		//�α� ������ ���� �� ���� �˻����� ���� ��¥�� �����Ѵ�.
		m_CfgFileApi.SetFile(strRunCheck);
		m_CfgFileApi.SetData(_T("�����˻���"), strCurrentDay);
	}
}

// << 2015�� ���������� �ý��� ���� 2015/11/11 _LCM : GPKI ������ �α���
void CLoginDlg::OnButtonGpkiLogin() 
{
	// TODO: Add your control notification handler code here

	//CString str1, str2;
	//ShowGPKILoginDlg(str1, str2);

	//2015�� ���������� �ý��� ���� 2015/11/23 _LCM : GPKI ������ �α��� ����
	m_bGPKI = FALSE;
	
	CGPKILoginDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		m_bUserLoginMode = FALSE;
		m_bGPKI = TRUE;
		
		m_strID = dlg.m_strLoginInfo[0];
		m_strPassword = dlg.m_strLoginInfo[1];
		//UpdateData(FALSE);		
		BeginLogin();
	}	
}
// >> 2015/11/11 --ADD
