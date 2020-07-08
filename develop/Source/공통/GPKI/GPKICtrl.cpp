// GPKICtrl.cpp: implementation of the CGPKICtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GPKICtrl.h"

#include "..\\..\\����\\����\\�弭����\\LOC_CommonAPI\\CfgFileApi.h"
#include "..\\..\\ESL\\FWDES.h"
//#include "..\\..\\����\\����\\�弭����\\LOC_CommonAPI\\LocCommonAPI.h"

#include <locale.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define WORK_DIR _T(".")
//#define CLIENT_SIGN_CERT_PATH "C:/GPKI/Certificate/class2/533�̱�ȭ001_sig.cer"
//#define CLIENT_SIGN_PRIKEY_PATH "C:/GPKI/Certificate/class2/533�̱�ȭ001_sig.key"
//#define CLIENT_SIGN_PRIKEY_PASSWD "gold0721"


#define SERVER_SIGN_CERT_PATH _T("../CFG/GPKI/Certificate/class1/SVR1371460003_sig.cer")
#define SERVER_SIGN_PRIKEY_PATH _T("../CFG/GPKI/Certificate/class1/SVR1371460003_sig.key")
#define SERVER_PRIKEY_PASSWD _T("rnrwnd@2015")


//#define SERVER_SIGN_CERT_PATH	_T("D:/GPKI/certificate/class1/WMA1371460001_sig.cer")
//#define SERVER_SIGN_PRIKEY_PATH _T("D:/GPKI/certificate/class1/WMA1371460001_sig.key")
//#define SERVER_PRIKEY_PASSWD _T("2014@kolas")

# define API_CONF_FILE_PATH "../CFG/GPKI/gpkiapi.conf"
# define ROOT_CERT_PATH1 "../CFG/GPKI/Certificate/class1/GPKIRootCA1.der"

CGPKICtrl::CGPKICtrl()
{

	m_strServerCert = _T("");
	m_strServerKey = _T("");
	m_strServerPwd = _T("");
	m_strRootCert = _T("");

	CString strGPKIPath = _T("..\\cfg\\GPKI.cfg");
	CFileFind findFile;

	
	CCfgFileApi cfgFileApi;

	cfgFileApi.SetFile(strGPKIPath);

	if(!findFile.FindFile(strGPKIPath))
	{
		FILE *ff = _tfopen(strGPKIPath, _T("w+b"));
		fputc(0xFF, ff); 
		fputc(0xFE, ff);
		fclose(ff);
		cfgFileApi.SetData(_T("GPKI_Path"), _T("C:\\GPKI"));
		cfgFileApi.SetData(_T("SERVER_SIGN_CERT_PATH"), SERVER_SIGN_CERT_PATH);
		cfgFileApi.SetData(_T("SERVER_SIGN_PRIKEY_PATH"), SERVER_SIGN_PRIKEY_PATH);
		cfgFileApi.SetData(_T("ROOT_CERT_PATH1"), ROOT_CERT_PATH1);

		/*
		CFWDES des;		
		des.Simple_Cipher(
		SERVER_PRIKEY_PASSWD,
		_T("k^Ty"),	// ��ȣŰ
		m_strServerPwd	
		);
		*/
		m_strServerPwd = SERVER_PRIKEY_PASSWD;

		CFWDES des;
		des.Write_DES_File(_T("..\\CFG\\GPKIServerInfo.cfg"),_T("k^Ty"), m_strServerPwd);
	}
	findFile.Close();

	cfgFileApi.GetData(_T("SERVER_SIGN_CERT_PATH"),m_strServerCert);
	cfgFileApi.GetData(_T("SERVER_SIGN_PRIKEY_PATH"),m_strServerKey);	
	cfgFileApi.GetData(_T("ROOT_CERT_PATH1"),m_strRootCert);	
	
		
	CFWDES DES;		// �Ϻ�ȣȭ Ŭ����
	int nRtn = DES.Read_DES_File(
		_T("..\\CFG\\GPKIServerInfo.cfg"),
		_T("k^Ty"),	// ��ȣŰ
		m_strServerPwd	// ��ȣȭ�� ����
	);
	if(nRtn != 0)
	{
		m_strServerPwd = SERVER_PRIKEY_PASSWD;

		CFWDES des;
		des.Write_DES_File(_T("..\\CFG\\GPKIServerInfo.cfg"),_T("k^Ty"), m_strServerPwd);
		DES.Read_DES_File(_T("..\\CFG\\GPKIServerInfo.cfg"),_T("k^Ty"),	m_strServerPwd );
	}
	m_strServerPwd.TrimRight();
	

	m_pEditLog = NULL;
}

CGPKICtrl::~CGPKICtrl()
{

}

BOOL CGPKICtrl::VerifyGPKI(CString strCert, CString strPwd, CString &strUserUID, CString& strUserKey)
{
	int nRv = GPKI_OK;
	void *pServerCtx = NULL;
	void *pClientCtx = NULL;
	char szSignTime[ 20] ;
	char szClientDN[ 256] ;

	char szMBSBuf1[512];	
	memset(szMBSBuf1, 0x00, sizeof(szMBSBuf1));
	char szMBSBuf2[512];	
	memset(szMBSBuf2, 0x00, sizeof(szMBSBuf2));
	TCHAR szWCSBuf[512];
	memset(szWCSBuf, 0x00, sizeof(szWCSBuf));

	BOOL bRtn = TRUE;

	CString strKey = _T("");

	BINSTR bsRandom, bsData;
	BINSTR bsCert, bsPriKey;
	BINSTR bsSignedData;
	BINSTR bsClientCert, bsServerCert, bsServerPriKey;
	// ���� �ʱ�ȭ
	memset(szSignTime, 0x00, sizeof(szSignTime));
	memset(szClientDN, 0x00, sizeof(szClientDN));
	GPKI_BINSTR_Create(&bsRandom);
	GPKI_BINSTR_Create(&bsData);
	GPKI_BINSTR_Create(&bsCert);
	GPKI_BINSTR_Create(&bsPriKey);
	GPKI_BINSTR_Create(&bsSignedData);
	GPKI_BINSTR_Create(&bsClientCert);
	GPKI_BINSTR_Create(&bsServerCert);
	GPKI_BINSTR_Create(&bsServerPriKey);
	// # # # # # # # # # # # # # # # # # #
	// # # # ���� # # #
	// # # # # # # # # # # # # # # # # # #
	// API �ʱ�ȭ
	nRv = GPKI_API_Init(&pServerCtx, (char* )WORK_DIR);
	if (nRv != GPKI_OK)
	{
		printf("[SERVER] ErrCode = %d\n", nRv);
		bRtn = FALSE;
		goto memfree;
	}
	// �ܰ� : ��
	nRv = GPKI_CRYPT_GenRandom(pServerCtx, 20, &bsRandom);
	if (nRv != GPKI_OK)
	{
		DisplayServerErr(pServerCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}
	// # # # # # # # # # # # # # # # # # #
	// # # # Ŭ���̾�Ʈ # # #
	// # # # # # # # # # # # # # # # # # #
	// Ŭ���̾�Ʈ �� API �ʱ�ȭ
	nRv = GPKI_API_Init(&pClientCtx, (char* )WORK_DIR);
	if (nRv != GPKI_OK)
	{
		printf("[ CLIENT] ErrCode = %d\n", nRv);
		bRtn = FALSE;
		goto memfree;
	}
	// �ܰ� : ��

	wcstombs(szMBSBuf1, strCert.GetBuffer(512), sizeof(szMBSBuf1));
	nRv = GPKI_STORAGE_ReadCert(pClientCtx, MEDIA_TYPE_FILE_PATH,		
		(char* )szMBSBuf1, DATA_TYPE_GPKI_SIGN, &bsCert);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}
	
	strKey = strCert;

	strKey.Replace(_T(".cer"),_T(".key"));
    wcstombs(szMBSBuf1, strKey.GetBuffer(512), sizeof(szMBSBuf1));
	wcstombs(szMBSBuf2, strPwd.GetBuffer(512), sizeof(szMBSBuf2));

	nRv = GPKI_STORAGE_ReadPriKey(pClientCtx, MEDIA_TYPE_FILE_PATH,
		(char* )szMBSBuf1, (char* )szMBSBuf2,
		DATA_TYPE_GPKI_SIGN, &bsPriKey);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	} 

	// �ܰ� : ��
	nRv = GPKI_CMS_MakeSignedData(pClientCtx, &bsCert, &bsPriKey, &bsRandom, NULL,
		&bsSignedData);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}
	// # # # # # # # # # # # # # # # # # #
	// # # # ���� # # #
	// # # # # # # # # # # # # # # # # # #
	// �ܰ� : ��
	nRv = GPKI_CMS_ProcessSignedData(pServerCtx, &bsSignedData, &bsData, &bsClientCert,
		szSignTime);
	if (nRv != GPKI_OK)
	{
		DisplayServerErr(pServerCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}
	// �ܰ� : ��
	if (bsData.nLen != bsRandom.nLen ||
		memcmp(bsData.pData, bsRandom.pData, bsRandom.nLen) != 0)
	{
		printf("[SERVER] ������ �����ߴ� �������� Ŭ���̾�Ʈ�� �������� �ٸ��ϴ�.\n");
		bRtn = FALSE;
		goto memfree;
	}
	// �ܰ� : ��

	wcstombs( szMBSBuf1, m_strServerCert.GetBuffer(512), sizeof(szMBSBuf1));
	nRv = GPKI_STORAGE_ReadCert(pServerCtx, MEDIA_TYPE_FILE_PATH,		
		(char*)szMBSBuf1, DATA_TYPE_GPKI_SIGN, &bsServerCert);
	if (nRv != GPKI_OK)
	{
		DisplayServerErr(pServerCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}

	wcstombs( szMBSBuf1, m_strServerKey.GetBuffer(512), sizeof(szMBSBuf1));
	wcstombs( szMBSBuf2, m_strServerPwd.GetBuffer(512), sizeof(szMBSBuf2));

	nRv = GPKI_STORAGE_ReadPriKey(pServerCtx, MEDIA_TYPE_FILE_PATH,
		(char* )szMBSBuf1, (char* )szMBSBuf2,
		//DATA_TYPE_GPKI_SIGN, &bsPriKey);
		DATA_TYPE_GPKI_SIGN, &bsServerPriKey);
	if (nRv != GPKI_OK)
	{
		DisplayServerErr(pServerCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}
	
	// �ܰ� : �� (3.2.5. ����)	
	//if (VerifyCert(&bsClientCert, CERT_TYPE_SIGN, &bsServerCert, &bsPriKey) > 0)
	if (VerifyCert(&bsClientCert, CERT_TYPE_SIGN, &bsServerCert, &bsServerPriKey) > 0)
	//if (VerifyCert(&bsServerCert, CERT_TYPE_SIGN, &bsClientCert, &bsPriKey) > 0)
	{
		bRtn = FALSE;
		goto memfree;
	}
	// �ܰ� : ��
	nRv = GPKI_CERT_Load(pServerCtx, &bsClientCert);
	if (nRv != GPKI_OK)
	{
		DisplayServerErr(pServerCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}
	nRv = GPKI_CERT_GetSubjectName(pServerCtx, sizeof(szClientDN), (char* )szClientDN);
	if (nRv != GPKI_OK)
	{
		DisplayServerErr(pServerCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}
	
	nRv = GPKI_CERT_Unload(pServerCtx);
	if (nRv != GPKI_OK)
	{
		DisplayServerErr(pServerCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}

	nRv = GPKI_CERT_Load(pClientCtx, &bsClientCert);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}

	nRv = GPKI_CERT_GetUID(pClientCtx, 512, szMBSBuf1);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}	
	mbstowcs( szWCSBuf, szMBSBuf1, sizeof(szWCSBuf));	
	strUserUID.Format(_T("%s"),szWCSBuf);

	nRv = GPKI_CERT_GetSerialNum(pClientCtx, 512, szMBSBuf1);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}
	mbstowcs( szWCSBuf, szMBSBuf1, sizeof(szWCSBuf));	
	strUserKey.Format(_T("%s"),szWCSBuf);

	nRv = GPKI_CERT_Unload(pClientCtx);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}

	// ȹ���� Ŭ���̾�Ʈ�� ������ �̸� (szClientDN)�� �̿��Ͽ� �ش� ����ڰ�
	// �������� �����ϴ� ���񽺸� �̿��� �� �ִ� ����� ������ ���θ� Ȯ���ϰ�
	// Ŭ���̾�Ʈ�� �α����� �����Ѵ�.	

memfree:
	// ���� �޸� ����
	GPKI_BINSTR_Delete(&bsRandom);
	GPKI_BINSTR_Delete(&bsData);
	GPKI_BINSTR_Delete(&bsCert);
	GPKI_BINSTR_Delete(&bsPriKey);
	GPKI_BINSTR_Delete(&bsSignedData);
	GPKI_BINSTR_Delete(&bsClientCert);
	GPKI_BINSTR_Delete(&bsServerCert);
	GPKI_BINSTR_Create(&bsServerPriKey);
	// API ��� ����
	GPKI_API_Finish(&pServerCtx);
	GPKI_API_Finish(&pClientCtx);

	return bRtn;
}

BOOL CGPKICtrl::VerifyCertList(CString strFile, CString * pStrValues, int nCount)
{
	_tsetlocale(LC_ALL, _T("Kor"));

	void *pClientCtx = NULL;

	BINSTR bsClientCert;
	GPKI_BINSTR_Create(&bsClientCert);

	CString strTmp = _T("");

	char szMBSBuf[512];	
	memset(szMBSBuf, 0x00, sizeof(szMBSBuf));
	TCHAR szWCSBuf[512];
	memset(szWCSBuf, 0x00, sizeof(szWCSBuf));

	int nIdx = 0;

	BOOL bRtn = TRUE;

	int nRv = GPKI_API_Init(&pClientCtx, (char* )WORK_DIR);
	if (nRv != GPKI_OK)
	{
		printf("[ CLIENT] ErrCode = %d\n", nRv);
		bRtn = FALSE;
		goto memfree;
	}
	
	wcstombs( szMBSBuf, strFile.GetBuffer(512), sizeof(szMBSBuf));
	nRv = GPKI_STORAGE_ReadCert(pClientCtx, MEDIA_TYPE_FILE_PATH,		
		(char*)szMBSBuf, DATA_TYPE_GPKI_SIGN, &bsClientCert);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}

	nRv = GPKI_CERT_Load(pClientCtx, &bsClientCert);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}

	nRv = GPKI_CERT_GetKeyUsage(pClientCtx, 512, szMBSBuf);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}
	mbstowcs( szWCSBuf, szMBSBuf, sizeof(szWCSBuf));	
	strTmp.Format(_T("%s"),szWCSBuf);
	if(strTmp.Compare(_T("digitalSignature")) != 0 && strTmp.Compare(_T("digitalSignature, nonRepudiation")) != 0 )
	{
		bRtn = FALSE;
		goto memfree;
	}

	//nIdx = m_pDM->GetRowCount();
	//m_pDM->InsertRow(-1);	
	
	nRv = GPKI_CERT_GetUID(pClientCtx, 512, szMBSBuf);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}	
	mbstowcs( szWCSBuf, szMBSBuf, sizeof(szWCSBuf));	
	strTmp.Format(_T("%s"),szWCSBuf);
	//m_pDM->SetCellData(_T("���������̵�"),strTmp,nIdx);
	pStrValues[0] = strTmp;


	nRv = GPKI_CERT_GetValidity(pClientCtx, 512, szMBSBuf);
	if (nRv != GPKI_OK)
	{
		bRtn = FALSE;DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}
	mbstowcs( szWCSBuf, szMBSBuf, sizeof(szWCSBuf));	
	strTmp.Format(_T("%s"),szWCSBuf);
	strTmp = strTmp.Mid(22, 10);
	//m_pDM->SetCellData(_T("������"),strTmp,nIdx);
	pStrValues[1] = strTmp;
	pStrValues[2] = _T("");


	/*
	nRv = GPKI_CERT_GetKeyUsage(pClientCtx, 512, szMBSBuf);
	if (nRv != GPKI_OK)
	{
		//DisplayClientErr(pClientCtx, nRv);
		goto memfree;
	}
	mbstowcs( szWCSBuf, szMBSBuf, sizeof(szWCSBuf));	
	strTmp.Format(_T("%s"),szWCSBuf);
	m_pDM->SetCellData(_T("�뵵"),strTmp,nIdx);
	*/
	
	nRv = GPKI_CERT_GetIssuerName(pClientCtx, 512, szMBSBuf);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}
	mbstowcs( szWCSBuf, szMBSBuf, sizeof(szWCSBuf));	
	//strTmp.Format(_T("%s"),szWCSBuf);
		
	AfxExtractSubString(strTmp, szWCSBuf, 0, ',');

	strTmp = strTmp.Right(strTmp.GetLength()-3);

	pStrValues[3] = strTmp;

	//m_pDM->SetCellData(_T("�߱ޱ��"),strTmp,nIdx);

	//m_pDM->SetCellData(_T("���ϰ��"),strFile,nIdx);

	nRv = GPKI_CERT_Unload(pClientCtx);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}
	
	memfree:
	// ���� �޸� ����	
	GPKI_BINSTR_Delete(&bsClientCert);	
	// API ��� ����	
	GPKI_API_Finish(&pClientCtx);
	return bRtn;
}

int CGPKICtrl::VerifyCert(BINSTR *pCert, int nCertType, BINSTR *pMyCert, BINSTR *pMyPriKey)
{
	int nRv = GPKI_OK;
	void *pContext = 0x00;
	BINSTR bsRootCert;

	char szMBSBuf[512];	
	memset(szMBSBuf, 0x00, sizeof(szMBSBuf));

	// ���� �ʱ�ȭ
	GPKI_BINSTR_Create(&bsRootCert);
	// API �ʱ�ȭ
	nRv = GPKI_API_Init(&pContext, (char* )WORK_DIR);
	if (nRv != GPKI_OK)
	{
		DisplayErrMsg(pContext, nRv);
		goto memfree;
	}
	/*  �� �� ���հ��������� �� �̿��Ͽ� ����
	�� �ŷ��ϴ� �ֻ��� ������� ������ ����
	�� ������ ���� ��� ����
	�� ������ ����
	������ ���� ����
	����
	����
	- 51-
	�������ڼ��� �������� ǥ�ظ�ⰳ�� ǥ�غ���API ������ �Ŵ���*/
	// �ܰ� : ��
	nRv = GPKI_CERT_VerifyByIVS(pContext, (char* )API_CONF_FILE_PATH, pCert, pMyCert);
	if (nRv == GPKI_OK) /* ���� ������ ���� */
	{
		goto memfree;
	}
	else if (nRv == ERR_REVOKED_CERT || /* ������ ���������� Ȯ�� */
		nRv == ERR_EXPIRED_CERT || /* ����� ���������� Ȯ�� */
		nRv == ERR_PATH_VALIDATION ||
		nRv == ERR_PATH_VALIDATION_CERT_POLICIES) /* �ŷ��� �� ���� ���������� Ȯ�� */
	{
		DisplayErrMsg(pContext, nRv);
		goto memfree;
	}
	// �ܰ� : ��
	// ��Ⱥ� ��Ʈ ������ ����
	wcstombs( szMBSBuf, m_strRootCert.GetBuffer(512), sizeof(szMBSBuf));
	nRv = GPKI_STORAGE_ReadCert(pContext, MEDIA_TYPE_FILE_PATH, (char* )szMBSBuf,
		DATA_TYPE_OTHER, &bsRootCert);
	if (nRv != GPKI_OK)
	{
		DisplayErrMsg(pContext, nRv);
		goto memfree;
	}
	nRv = GPKI_CERT_AddTrustedCert(pContext, &bsRootCert);
	if (nRv != GPKI_OK)
	{
		DisplayErrMsg(pContext, nRv);
		goto memfree;
	}
	/*
	// �ѱ����ͳ������ (��)��Ʈ ������ ����
	nRv = GPKI_STORAGE_ReadCert(pContext, MEDIA_TYPE_FILE_PATH, (char* )ROOT_CERT_PATH2,
	DATA_TYPE_OTHER, &bsRootCert);
	if (nRv != GPKI_OK)
	{
	DisplayErrMsg(pContext, nRv);
	goto memfree;
	}
	nRv = GPKI_CERT_AddTrustedCert(pContext, &bsRootCert);
	if (nRv != GPKI_OK)
	{
	DisplayErrMsg(pContext, nRv);
	goto memfree;
	}
	// �ѱ����ͳ������ (��)��Ʈ ������ ����
	nRv = GPKI_STORAGE_ReadCert(pContext, MEDIA_TYPE_FILE_PATH, (char* )ROOT_CERT_PATH3,
	DATA_TYPE_OTHER, &bsRootCert);
	if (nRv != GPKI_OK)
	{
	DisplayErrMsg(pContext, nRv);
	goto memfree;
	}
	nRv = GPKI_CERT_AddTrustedCert(pContext, &bsRootCert);
	if (nRv != GPKI_OK)
	{
	DisplayErrMsg(pContext, nRv);
	goto memfree;
	}
	*/
	// �ܰ� : ��
	
	nRv = GPKI_CERT_SetVerifyEnv(pContext, CERT_VERIFY_FULL_PATH, /* ��ü ��� ���� */
		CERT_REV_CHECK_ALL, /* OCSP �� �̿��Ͽ� �������� Ȯ�� */
		//CERT_REV_CHECK_ARL | CERT_REV_CHECK_OCSP, /* OCSP �� �̿��Ͽ� �������� Ȯ�� */
		//- 52-
		//�������ڼ��� �������� ǥ�ظ�ⰳ�� ǥ�غ���API ������ �Ŵ���
		true, /* ĳ�� ��� */
		NULL, /* �ý��� ���� �ð� ��� */
		NULL); /* OCSP ������ �������� ���ԵǾ������Ƿ� �������� �ʾƵ� �� */
	if (nRv != GPKI_OK)
	{
		DisplayErrMsg(pContext, nRv);
		goto memfree;
	}


	// �ܰ� : ��
	nRv = GPKI_CERT_Verify(pContext, pCert, nCertType, /* �����ϰ��� �ϴ� �������� ���� */
		NULL, /* ��� �뵵�� ������ ���� (Ư�� �������� �����ϰ��� �Ѵٸ� �ش� ��å ����) */
		API_CONF_FILE_PATH, /* ȯ�� ���� ���� */
		//false, NULL, NULL); /* OCSP ��û �޽��� ���� */
		true, pMyCert, pMyPriKey); /* OCSP ��û �޽��� ���� */
	if (nRv != GPKI_OK)
	{
		DisplayErrMsg(pContext, nRv);
		goto memfree;
	}

	/*
	BINSTR  bsReqMsg, bsResMsg;	

	GPKI_BINSTR_Create(&bsReqMsg);
	GPKI_BINSTR_Create(&bsResMsg);
	*/

	/*
	nRv = GPKI_OCSP_MakeOCSPReq(pContext, pCert, true, pMyCert, pMyPriKey, &bsReqMsg);
	if (nRv != GPKI_OK)
	{
		DisplayErrMsg(pContext, nRv);
		//DisplayFail1(MODULE_TYPE_CERT, TEST_TYPE_EXPECTED_VALUE, (char*)SPACE_LEVEL_2, nRv);
		goto memfree;
	}

	
	nRv = GPKI_OCSP_SendAndRecv(pContext, NULL, pCert, &bsReqMsg, &bsResMsg);
	if (nRv != GPKI_OK)
	{
		DisplayErrMsg(pContext, nRv);
		//DisplayFail1(MODULE_TYPE_CERT, TEST_TYPE_EXPECTED_VALUE, (char*)SPACE_LEVEL_2, nRv);
		goto memfree;
	}
	*/

//	GPKI_STORAGE_WriteFile(g_pContext, "c:/ocsp.req", &bsResMsg);

	/*
	nRv = GPKI_OCSP_VerifyResMsg(pContext, &bsReqMsg, &bsResMsg, &nCertStatCnt, &bsOCSPSvrCert);
	if (nRv != GPKI_OK)
	{
		//DisplayFail1(MODULE_TYPE_CERT, TEST_TYPE_EXPECTED_VALUE, (char*)SPACE_LEVEL_2, nRv);
		goto memfree;
	}
	*/

memfree:
	GPKI_BINSTR_Delete(&bsRootCert);
	// API ��� ����
	GPKI_API_Finish(&pContext);
	return nRv;	
}


void CGPKICtrl::DisplayErrMsg(void *pContext, int nErrCode)
{
	CString strMsg;
	char szErrMsg[512] = {0,};
	TCHAR tszErrMsg[512] = {0,};
	if (GPKI_API_GetErrInfo(pContext, 512, szErrMsg) == GPKI_OK)
	{
		mbstowcs( tszErrMsg, szErrMsg, sizeof(tszErrMsg));		
		strMsg.Format(_T("%s"), tszErrMsg);
		//strMsg.Format(_T("ErrCode = %d, ErrInfo = %s\n"), nErrCode, tszErrMsg);
	}
	else
	{
		strMsg.Format(_T("ErrCode = %d"), nErrCode);
	}

	if(m_pEditLog)
	{
		m_pEditLog->SetWindowText(strMsg);
	}
	else
	{
		AfxMessageBox(strMsg);
	}
}
/*������ ���� �޽���*/
void CGPKICtrl::DisplayServerErr(void *pContext, int nErrCode)
{
	CString strMsg;
	char szErrMsg[ 512] = {0,};
	TCHAR tszErrMsg[512] = {0,};
	if (GPKI_API_GetErrInfo(pContext, 512, szErrMsg) == GPKI_OK)
	{
		mbstowcs( tszErrMsg, szErrMsg, sizeof(tszErrMsg));		
		//strMsg.Format(_T("[ SERVER] \nErrCode=%d,ErrMsg=%s\n"), nErrCode, tszErrMsg);
		strMsg.Format(_T("%s"), tszErrMsg);
	}
	else
	{
		strMsg.Format(_T("[ SERVER] \nErrCode=%d"), nErrCode);
	}
	
	if(m_pEditLog)
	{
		m_pEditLog->SetWindowText(strMsg);
	}
	else
	{
		AfxMessageBox(strMsg);
	}

}
/*Ŭ���̾�Ʈ�� ���� �޽���*/
void CGPKICtrl::DisplayClientErr(void *pContext, int nErrCode)
{
	CString strMsg;
	char szErrMsg[ 512] = {0,} ;
	TCHAR tszErrMsg[512] = {0,};
	if (GPKI_API_GetErrInfo(pContext, 512, szErrMsg) == GPKI_OK)
	{
		mbstowcs( tszErrMsg, szErrMsg, sizeof(tszErrMsg));
		//strMsg.Format(_T("[ CLIENT] \nErrCode=%d,ErrMsg=%s\n"), nErrCode, tszErrMsg);		
		strMsg.Format(_T("%s"), tszErrMsg);
	}
	else
	{
		strMsg.Format(_T("[ CLIENT] \nErrCode=%d"), nErrCode);
	}

	if(m_pEditLog)
	{
		m_pEditLog->SetWindowText(strMsg);
	}
	else
	{
		AfxMessageBox(strMsg);
	}
}