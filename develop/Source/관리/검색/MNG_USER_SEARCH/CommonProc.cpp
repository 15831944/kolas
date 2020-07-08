// CommonProc.cpp: implementation of the CCommonProc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommonProc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommonProc::CCommonProc(CESL_Mgr* pParent, CESL_DataMgr* pDM )
{
	m_pParent = pParent;

	m_pDM = pDM;

	m_pLocCommonAPI = NULL;
	m_pLocCommonAPI = new CLocCommonAPI(m_pParent);
	m_pPreRegNoMgr = NULL;
	m_pPreRegNoMgr = new CPreRegCodeMgr(m_pParent);

	if(m_pParent->InitESL_Mgr(_T("SM_USERMAIN_MANAGER")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return ;
	}

	m_bLogin = FALSE;

	// KOL.UDF.022 ��ť���ڵ� ����
	m_pRegCodeDM = NULL;
	m_pSeCodeDM = NULL;
	m_pUseLimitCodeDM = NULL;
	m_pUseShelfLocCodeDM = NULL;
	m_pUserPositionDM = NULL;
	m_pIndividualUserPositionDM = NULL;
	m_pRegClassDM = NULL;
	m_pHolydayDM = NULL;
	m_pLoanInfoDM = NULL;
	m_pMemberInfoDM = NULL;
	m_nBookInfoIdx = -1;
	m_nMode = -1;
	m_strMaxRowCnt = _T("");
}

CCommonProc::~CCommonProc()
{
	if(m_pLocCommonAPI != NULL) 
	{
		delete m_pLocCommonAPI;
		m_pLocCommonAPI = NULL;
	}
	if(m_pPreRegNoMgr != NULL)
	{
		delete m_pPreRegNoMgr;
		m_pPreRegNoMgr = NULL;
	}
}

/*
@Create
 - 2009.09.15 BY PWR
@Description
 - ������ ����
@Parameter
 - 
@Return Value
 - -1000 : ��������� Restruct����
 - -1001 : ������ '�˻�������ID'�� ���� ��������(REC_KEY, ID, NAME)�� ���°��� �ִ�
 - -1002 : ���������� Restruct����
 - -1003 : ������ '�α��θ��','�ʼ��Է��׸�','SMS���ſ��α⺻üũ����','���ϼ��ſ��α⺻üũ����','�ڷ�˻����'�� ���� VALUE_1�� ���� ���°��� �ִ�
 - -1004 : ������ '�ʼ��Է��׸�'�� VALUE_1�� ���� 6�ڸ��� �ȵ�
 - -1005 : �ڵ� DM ��������
 - 0 : ����
@Mini Spec
 1. 
*/
INT CCommonProc::LoadManageValue()
{
	INT ids;
	CString strQuery;		

//  	strQuery.Format(_T("SELECT REC_KEY, MANAGE_CODE FROM MN_USER_TBL ")
//  					_T("WHERE REC_KEY = %s"), m_pParent->m_pInfo->USER_PK);
// 	ids = m_pDM->RestructDataManager(strQuery);
// 	if(0 > ids)
// 	{
// 		AfxMessageBox(_T("�˻��� ����������� �������µ� �����Ͽ����ϴ�.\r\n�˻��� ȯ�漳���� ���� �������ּ���."));
// 		return -1000;
// 	}
// 	// REC_KEY
// 	m_pDM->GetCellData(0, 0, m_strUserPK);	
// 	// MANAGE_CODE
// 	m_pDM->GetCellData(0, 1, m_strManageCode);	
	m_strManageCode = m_pParent->m_pInfo->MANAGE_CODE;
	if(m_strManageCode.IsEmpty())
	{
		AfxMessageBox(_T("����ڿ� ���� ���������� �����Ǿ����� �ʽ��ϴ�."));
		return -1001;
	}
	
	strQuery.Format(
		_T("SELECT ")
		_T("(SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='�˻���' AND VALUE_NAME='�ڷ�˻����' AND MANAGE_CODE=UDF_MANAGE_CODE AND ROWNUM=1),")
		_T("(SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='��ϱ����ڵ��ڸ�������' AND MANAGE_CODE=UDF_MANAGE_CODE AND ROWNUM=1) ")
		_T("FROM DUAL") );
	ids = m_pDM->RestructDataManager(strQuery);
	if(0 > ids) 
	{
		AfxMessageBox(_T("�˻��� ������������ �������µ� �����Ͽ����ϴ�.\r\n�˻��� ȯ�漳���� ���� �������ּ���."));
		return -1002;
	}	
	// �ڷ�˻����
	m_pDM->GetCellData(0, 0, m_strSearchMode);
	if(m_strSearchMode.IsEmpty())
	{
		m_strSearchMode = _T("0");
	}
	// ��ϱ����ڵ��ڸ�������
	m_pDM->GetCellData(0, 1, m_strRegCodeLength);
	if(m_strRegCodeLength.IsEmpty())
	{
		m_strRegCodeLength = _T("2");
	}
	
	// LOCAL IP
	//m_strIP = GetIP();
	// û����ȣ���
	// 2010.05.12 UPDATE BY PWR : ��������� �������� EPAC�뿡�� KOLAS������ �ٲ�
	CStdioFile file;		
	CString strTemp;		
	CString strLine;
	strTemp = _T("..\\cfg\\ReceiptPrintCode.cfg") ;	
	if(!file.Open (strTemp,CFile::modeRead | CFile::typeBinary)) 
	{
		m_strReceiptUseYN = _T("N");
	}
	else
	{
		TCHAR cUni;
		file.Read(&cUni, sizeof(TCHAR));
		if(0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
		ids = file.ReadString (strLine);
		if(ids) m_strReceiptUseYN = strLine;
		// 16/07/01 ������ : ������ ��±� ������ ������� ��� �����ϵ��� ��� ����
//*/ ADD ---------------------------------------------------------------------------
		m_strReceiptUseYN.TrimLeft();		m_strReceiptUseYN.TrimRight();
//*/ END ---------------------------------------------------------------------------
		if(m_strReceiptUseYN == _T("N") || m_strReceiptUseYN.IsEmpty())
		{
			m_strReceiptUseYN = _T("N");
		}
		file.Close();
	}
// 	CString sFileName = _T("..\\cfg\\UserMainReceipt.cfg");
// 	CString sCertifyFileName = _T("..\\cfg\\UserMainReceiptCode.cfg");
// 	CFileFind file;
// 	if(!file.FindFile(sFileName))
// 	{ // ���� ���ٸ� �����Լ���
// 		FILE *fp = _tfopen(sFileName, _T("w+b"));
// 		fputc(0xFF, fp); 
// 		fputc(0xFE, fp);
// 		
// 		_ftprintf(fp, _T("%s"), _T("N"));
// 		fclose( fp );
// 		m_strReceiptUseYN = _T("N");
// 	}
// 	else
// 	{ // �����ִٸ� ���Ͽ� ������ ������ ����
// 		CString sValue;
// 		CStdioFile fp;
// 		if(!fp.Open(sFileName, CFile::modeRead | CFile::typeBinary))
// 		{
// 			return -1;
// 		}
// 		TCHAR cUni;
// 		fp.Read(&cUni, sizeof(TCHAR));
// 		if(0xFEFF != cUni)
// 		{
// 			fp.SeekToBegin();
// 		}
// 		fp.ReadString(sValue);
// 		fp.Close();
// 		sValue.TrimLeft();	sValue.TrimRight();
// 		if(_T("Y") == sValue)
// 		{ // �������� ������̶�� ���������� ������ ����� �� �ִ�.
// 			CFileFind file2;
// 			if(!file2.FindFile(sCertifyFileName))
// 			{ // ��������
// 				m_strReceiptUseYN = _T("N");
// 			}
// 			else
// 			{ // ��������
// 				m_strReceiptUseYN = sValue;
// 			}
// 		}
// 		else
// 		{
// 			m_strReceiptUseYN = _T("N");
// 		}
// 	}

	ids = SetCodeDM();
	if(0 > ids)
	{
		AfxMessageBox(_T("�ڵ������� �������µ� �����Ͽ����ϴ�."));
		return -1005;
	}

	return 0;
}

INT CCommonProc::SendSearchBookInfoSQL(CESL_DataMgr *pDM, CString sPublishForm, CString sBoWhere, CString sSeWhere, CString &sResult, BOOL bReSearch)
{
	DWORD nSTART , nFINISH;
	nSTART = GetTickCount();

	INT ids;
	CESL_DataMgr pSeRSDM;
		
	INT nTotalCnt, nBOCnt, nSECnt;
	nTotalCnt = nBOCnt = nSECnt = 0;

	// ���� Restruct�Ҷ� ���Ǵ� 
	CString strRestructBoWhere;
	CString strRestructSeWhere;
	// ����WHERE�� ��� ����
	CString strTempBoWhere = m_strBoWhere;
	CString strTempSeWhere = m_strSeWhere;
	// �������˻��� �ƴϸ� ����WHERE�� ���� �ӽ�WHERE�ʱ�ȭ 
	if(TRUE != bReSearch)
	{
		strTempBoWhere = _T("");
		strTempSeWhere = _T("");
	}

	// Query����
	CString strBoQuery = _T("");
	CString strSeQuery = _T("");
	CString strCntQuery, strCount;
	// �ڷ�˻���� (0:å�����˻�, 1:�������˻�)
	if(_T("0") == m_strSearchMode)
	{
		pSeRSDM.InitDMPKFromFile(_T("DM_USERMAIN_BOOKSEARCH_B_SE"));
		pSeRSDM.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
		pSeRSDM.FreeData();

		if(TRUE == bReSearch)
		{ // ��˻��̸� ����WHERE�� ����WHERE ��ģ �������� ���
			strRestructBoWhere = strTempBoWhere + _T(" AND IB.REC_KEY IN ") + sBoWhere + _T(" AND ROWNUM <= ") + m_strMaxRowCnt + _T(" ");
			strRestructSeWhere = strTempSeWhere + _T(" AND IB.REC_KEY IN ") + sSeWhere + _T(" AND ROWNUM <= ") + m_strMaxRowCnt + _T(" ");
		}
		else
		{ // ��˻��� �ƴϸ� ����WHERE�� ��� (�⺻������ ��ģ)
			strRestructBoWhere = 
				_T("IB.REC_KEY=BB.SPECIES_KEY AND BS.REC_KEY=BB.SPECIES_KEY AND ")
				+ sBoWhere + _T(" AND ROWNUM <= ") + m_strMaxRowCnt + _T(" ");
			strRestructSeWhere = 
				_T("IB.REC_KEY=BB.SPECIES_KEY AND BS.REC_KEY=BB.SPECIES_KEY AND ")
				_T("V.REC_KEY=BB.VOL_KEY AND ")
				+ sSeWhere + _T(" AND ROWNUM <= ") + m_strMaxRowCnt + _T(" ");
		}

		if(_T("����") == sPublishForm)
		{
			pSeRSDM.FreeData();
			ids = pDM->RefreshDataManager(strRestructBoWhere);
 			if(0 > ids) 
 			{
 				return -3;
 			}
			nBOCnt = pDM->GetRowCount();
			nSECnt = 0;
			nTotalCnt = nBOCnt+nSECnt;
		}
		else if(_T("����") == sPublishForm)
		{
			pDM->FreeData();
			ids = pSeRSDM.RefreshDataManager(strRestructSeWhere);
 			if(0 > ids) 
 			{
 				return -3;
 			}
			nBOCnt = 0;
			nSECnt = pSeRSDM.GetRowCount();
			nTotalCnt = nBOCnt+nSECnt;
		}
		else
		{
 			ids = pDM->RefreshDataManager(strRestructBoWhere);
 			if(0 > ids) 
 			{
 				return -3;
 			}
 			ids = pSeRSDM.RefreshDataManager(strRestructSeWhere);
 			if(0 > ids) 
 			{
 				return -3;
 			}
			nBOCnt = pDM->GetRowCount();
			nSECnt = pSeRSDM.GetRowCount();
			nTotalCnt = nBOCnt+nSECnt;
		}

		// DM ����
		CString sData;
		const INT nColCnt = 46;
		CString sCAlias[nColCnt] =
		{
			_T("���������̷°Ǽ�")	, _T("�ݳ�������")				, _T("�η��ڷ��")			, _T("�ڷ�����")			,
			_T("��ü����")			, _T("����/���ӱ���")			, _T("IBS_û��_������ȣ")	, _T("IBS_û��_�з���ȣ")	,
			_T("�з���ȣ����")		, _T("IBS_û��_������ȣ")		, _T("�����")				, _T("BOOK_KEY")			,
			_T("��Ϲ�ȣ")			, _T("IBS_û��_��ġ��ȣ_�ڵ�")	, _T("�ڷ���ڵ�")			, _T("IBS_û��_��å��ȣ")	,
			_T("����")				, _T("�ڷ����")				, _T("�����ڻ���")			, _T("�����ڻ���")			,
			_T("IDX_KEY")			, _T("ǥ������")				, _T("���̸�")				, _T("��������")			,
			_T("�̿����ѱ���")		, _T("��ϱ���")				, _T("�ηϿ���")			, _T("��/����")				,
			_T("�����")			, _T("������Ư��")				, _T("ũ��")				, _T("�����ڷ�")			,
			_T("����")				, _T("��KEY")					, _T("��ȣ����KEY")			, _T("��ǥ��")				,
			_T("��������")			, _T("�ʷ�����")				, _T("����Ű")				, _T("�̿��󱸺�")		,
			_T("û����ȣ")			, _T("���Ⱑ�ɿ���")			, _T("�ڷ����¼���")		, _T("�ڷ���ڵ弳��")		,
			_T("�ڷ���¼���")		, _T("�������м���")
		};
		for(INT i = 0; i < pSeRSDM.GetRowCount(); i++)
		{
			INT nRow = pDM->GetRowCount();
			pDM->InsertRow(-1);
			CString sData;
			for(INT j = 0; j < nColCnt; j++)
			{
				sData.Empty();
				ids = pSeRSDM.GetCellData(sCAlias[j], i, sData);
				if(0 > ids) 
				{
					return -3;
				}
				ids = pDM->SetCellData(sCAlias[j], sData, nRow);
				if(0 > ids) 
				{
					return -3;
				}
			}
		}

		// �Ǽ� Ȯ��
		if(1 > nTotalCnt)
		{
			return 1000;
		}

		// ��������
		m_strBoWhere = strRestructBoWhere;
		m_strSeWhere = strRestructSeWhere;
	}
	else
	{ // �������˻�
		pSeRSDM.InitDMPKFromFile(_T("DM_USERMAIN_BOOKSEARCH_S_SE"));
		pSeRSDM.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
		pSeRSDM.FreeData();

		if(TRUE == bReSearch)
		{
			strRestructBoWhere = strTempBoWhere + _T(" AND IB.REC_KEY IN ") + sBoWhere + _T(" ");
			strRestructSeWhere = strTempSeWhere + _T(" AND IB.REC_KEY IN ") + sSeWhere + _T(" ");
		}
		else
		{
			strRestructBoWhere = 
				_T("IB.REC_KEY=BB.SPECIES_KEY AND BS.REC_KEY=BB.SPECIES_KEY AND ")
				+ sBoWhere + _T(" ");
			strRestructSeWhere = 
				_T("IB.REC_KEY=BB.SPECIES_KEY AND BS.REC_KEY=BB.SPECIES_KEY AND ")
				_T("V.REC_KEY=BB.VOL_KEY AND ")
				+ sSeWhere + _T(" ");
		}

		if(_T("����") == sPublishForm)
		{
			pSeRSDM.FreeData();
			ids = pDM->RefreshDataManager(strRestructBoWhere);
 			if(0 > ids) 
 			{
 				return -3;
 			}
			nBOCnt = pDM->GetRowCount();
			nSECnt = 0;
			nTotalCnt = nBOCnt+nSECnt;
		}
		else if(_T("����") == sPublishForm)
		{
			pDM->FreeData();
			ids = pSeRSDM.RefreshDataManager(strRestructSeWhere);
 			if(0 > ids) 
 			{
 				return -3;
 			}
			nBOCnt = 0;
			nSECnt = pSeRSDM.GetRowCount();
			nTotalCnt = nBOCnt+nSECnt;
		}
		else
		{
 			ids = pDM->RefreshDataManager(strRestructBoWhere);
 			if(0 > ids) 
 			{
 				return -3;
 			}
 			ids = pSeRSDM.RefreshDataManager(strRestructSeWhere);
 			if(0 > ids) 
 			{
 				return -3;
 			}
			nBOCnt = pDM->GetRowCount();
			nSECnt = pSeRSDM.GetRowCount();
			nTotalCnt = nBOCnt+nSECnt;
		}

		// DM����
//+++++ 2011.9.16 ä��ȣ [�߰�]
// ���� : ����ñ���û ��ɰ������
// �䱸 : �ڷ�ã�⿡�� �����ڷ��� ��� �������� ǥ�õǵ��� ����
		CString sData;
		const INT nColCnt = 23;
		CString sCAlias[nColCnt] =
		{
			_T("���������̷°Ǽ�"), _T("�ڷ�����")		, _T("��ü����")	, _T("����/���ӱ���")	, _T("������ȣ")	, 
			_T("�з���ȣ")		, _T("�����")		, _T("��ġ��ȣ")		, _T("�����ڻ���")	, 
			_T("�����ڻ���")	, _T("��������")	, _T("IDX_KEY")			, _T("ǥ������")	, 
			_T("�˻��뺻ǥ��")	, _T("��������")	, _T("�ʷ�����")		, _T("����Ű")		,
			_T("�ڷ����")		, _T("�ڷ���ڵ�")	, _T("�ڷ���¼���")	, _T("�ڷ���ڵ弳��")	,
			_T("�������м���")	, _T("�ڷ����¼���")
		};
//-----
		for(INT i = 0; i < pSeRSDM.GetRowCount(); i++)
		{
			INT nRow = pDM->GetRowCount();
			pDM->InsertRow(-1);
			CString sData;
			for(INT j = 0; j < nColCnt; j++)
			{
				sData.Empty();
				ids = pSeRSDM.GetCellData(sCAlias[j], i, sData);
				if(0 > ids) 
				{
					return -3;
				}
				ids = pDM->SetCellData(sCAlias[j], sData, nRow);
				if(0 > ids) 
				{
					return -3;
				}
			}
		}

		// �Ǽ� Ȯ��
		if(1 > nTotalCnt)
		{
			return 1000;
		}
		// ��������
		m_strBoWhere = strRestructBoWhere;
		m_strSeWhere = strRestructSeWhere;
	}

	nFINISH = GetTickCount();
	CString sTotalCnt, sBOCnt, sSECnt;
	sTotalCnt.Format(_T("%d"), nTotalCnt);
	sBOCnt.Format(_T("%d"), nBOCnt);
	sSECnt.Format(_T("%d"), nSECnt);
	SetThousandFormat(sTotalCnt);
	SetThousandFormat(sBOCnt);
	SetThousandFormat(sSECnt);
	sResult.Format(_T("��ü %s�� (����:%s��, ����:%s��)     %d.%d sec."), sTotalCnt, sBOCnt, sSECnt, (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);

	return 0;
}


INT CCommonProc::MakeShelfCode(CESL_DataMgr *pDM)
{
	INT ids;

	INT nRowCount = pDM->GetRowCount();
	
	INT i = 0;
	CString strPublishFormCode;
	CString sShelfCode;
	for(i = 0; i < nRowCount; i++)
	{
		pDM->GetCellData(_T("����/���ӱ���"), i, strPublishFormCode);
		if(_T("NB") == strPublishFormCode || _T("���") == strPublishFormCode)
		{
			continue;
		}

		ids = m_pLocCommonAPI->MakeShelfCode(m_pParent, pDM, i, sShelfCode);
		if(0 > ids ) 
		{
			return -2;
		}
	}

	return 0;
}

INT CCommonProc::IsLoanCodeYN(CString sCodeAlias, CString sCode, CString &sValue)
{
	INT ids;
	if(sCode.IsEmpty())
	{
		sValue = _T("Y");
		return 0;
	}

	CESL_DataMgr *pCodeMgr;

	if(_T("��ϱ���") == sCodeAlias)
	{
		pCodeMgr = m_pRegCodeDM;
	}
	else if(_T("��ġ��ȣ") == sCodeAlias)
	{
		pCodeMgr = m_pSeCodeDM;
	}
	else if(_T("�̿����ѱ���") == sCodeAlias)
	{
		pCodeMgr = m_pUseLimitCodeDM;
	}
	else if(_T("�ڷ�Ǳ���") == sCodeAlias)
	{
		pCodeMgr = m_pUseShelfLocCodeDM;
	}
	else 
	{
		sValue = _T("Y");
		return 0;
	}

	CString sLoanYN;
	CString sCodes;
	for(INT i = 0; i < pCodeMgr->GetRowCount(); i++)
	{
		ids = pCodeMgr->GetCellData(_T("�ڵ�"), i, sCodes);
		if(0 > ids) 
		{
			return -1;
		}
		sCodes.TrimLeft(); sCodes.TrimRight();

		if(sCodes == sCode)
		{
			ids = pCodeMgr->GetCellData(_T("���⿩��"), i, sLoanYN );
			if(0 > ids) 
			{
				return -2;
			}
			sLoanYN.TrimLeft(); sLoanYN.TrimRight();
			sValue = sLoanYN;

			return 0;
		}
	}

	return 0;
}

INT CCommonProc::GetWorkingStatusDescToCode(CString &sCodeValue, INT nMode)
{
	m_pLocCommonAPI->GetWorkingStatusDescToCode(sCodeValue, nMode);
	return 0;
}


VOID CCommonProc::SetThousandFormat(CString &sValue)
{
	sValue.Replace( _T(","), _T("") );
	CString strBuf = sValue;

	INT nSize = ( strBuf.GetLength() * 3 ) + 100;
	_TCHAR *pszFormattedNumber = new _TCHAR [nSize];

	NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 0 };

	::GetNumberFormat(NULL, NULL, strBuf, &nFmt, pszFormattedNumber, nSize - 1);

	sValue = pszFormattedNumber;

	if (pszFormattedNumber)
	{
		delete [] pszFormattedNumber;
	}
}

INT CCommonProc::SetCodeDM()
{
	INT ids;

// 	CESL_DataMgr pCodeDM;
// 	pCodeDM.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	
	// 1. DM_��ϱ���
	m_pRegCodeDM = m_pParent->FindDM(_T("DM_USERMAIN_��ϱ���"));
	if(NULL == m_pRegCodeDM) 
	{
		return -1;
	}
	m_pRegCodeDM->RefreshDataManager(_T(""));
// 	pCodeDM.InitDMPKFromFile(_T("DM_USERMAIN_��ϱ���"));
// 	ids = pCodeDM.RefreshDataManager(_T(""));
// 	if(0 > ids)  
// 	{
// 		return -2;
// 	}
// 	CopyDM2DM(&pCodeDM, m_pRegCodeDM);
	// 2. DM_��ġ��ȣ
	m_pSeCodeDM = m_pParent->FindDM(_T("DM_USERMAIN_��ġ��ȣ"));
	if(NULL == m_pSeCodeDM) 
	{
		return -3;
	}
	m_pSeCodeDM->RefreshDataManager(_T(""));
// 	pCodeDM.InitDMPKFromFile(_T("DM_USERMAIN_��ġ��ȣ"));
// 	ids = pCodeDM.RefreshDataManager(_T(""));
// 	if(0 > ids)  
// 	{
// 		return -4;
// 	}
// 	CopyDM2DM(&pCodeDM, m_pSeCodeDM);
	
	// 3. DM_�̿����ѱ���
	m_pUseLimitCodeDM = m_pParent->FindDM(_T("DM_USERMAIN_�̿����ѱ���"));
	if(NULL == m_pUseLimitCodeDM) 
	{
		return -5;
	}
	m_pUseLimitCodeDM->RefreshDataManager(_T(""));
// 	pCodeDM.InitDMPKFromFile(_T("DM_USERMAIN_�̿����ѱ���"));
// 	ids = pCodeDM.RefreshDataManager(_T(""));
// 	if(0 > ids)  
// 	{
// 		return -6;
// 	}
// 	CopyDM2DM(&pCodeDM, m_pUseLimitCodeDM);
	
	// 4. DM_�ڷ��
	m_pUseShelfLocCodeDM = m_pParent->FindDM(_T("DM_USERMAIN_�ڷ��"));
	if(NULL == m_pUseShelfLocCodeDM) 
	{
		return -7;
	}
	m_pUseShelfLocCodeDM->RefreshDataManager(_T(""));
// 	pCodeDM.InitDMPKFromFile(_T("DM_USERMAIN_�ڷ��"));
// 	ids = pCodeDM.RefreshDataManager(_T(""));
// 	if(0 > ids)  
// 	{
// 		return -8;
// 	}
// 	CopyDM2DM(&pCodeDM, m_pUseShelfLocCodeDM);

	// 5. DM_���޺��������а�
	m_pUserPositionDM = m_pParent->FindDM(_T("DM_USERMAIN_���޺��������а�"));
	if(NULL == m_pUserPositionDM) 
	{
		return -7;
	}
	m_pUserPositionDM->RefreshDataManager(_T(""));
// 	pCodeDM.InitDMPKFromFile(_T("DM_USERMAIN_���޺��������а�"));
// 	ids = pCodeDM.RefreshDataManager(_T(""));
// 	if(0 > ids)  
// 	{
// 		return -8;
// 	}
// 	CopyDM2DM(&pCodeDM, m_pUserPositionDM);

	// 6. DM_���޺��������а�_��������
	m_pIndividualUserPositionDM = m_pParent->FindDM(_T("DM_USERMAIN_���޺��������а�_��������"));
	if(NULL == m_pIndividualUserPositionDM) 
	{
		return -7;
	}
	CString strWhere;
// 	strWhere.Format(_T("MANAGE_CODE='%s'"), m_strManageCode);
	strWhere.Format(_T("MANAGE_CODE=UDF_MANAGE_CODE"));
	m_pIndividualUserPositionDM->RefreshDataManager(strWhere);
// 	pCodeDM.InitDMPKFromFile(_T("DM_USERMAIN_���޺��������а�_��������"));
// 	ids = pCodeDM.RefreshDataManager(strWhere);
// 	if(0 > ids)  
// 	{
// 		return -8;
// 	}
// 	CopyDM2DM(&pCodeDM, m_pIndividualUserPositionDM);

	// 7. DM_������޼�����
	m_pRegClassDM = m_pParent->FindDM(_T("DM_USERMAIN_������޼�����"));
	if(NULL == m_pRegClassDM) 
	{
		return -7;
	}
	m_pRegClassDM->RefreshDataManager(_T(""));
// 	pCodeDM.InitDMPKFromFile(_T("DM_USERMAIN_������޼�����"));
// 	ids = pCodeDM.RefreshDataManager(_T(""));
// 	if(0 > ids)  
// 	{
// 		return -8;
// 	}
// 	CopyDM2DM(&pCodeDM, m_pRegClassDM);

	// 8. DM_�ް���
	m_pHolydayDM = m_pParent->FindDM(_T("DM_USERMAIN_HOLYDAY_INFO"));
	if( NULL == m_pHolydayDM ) 
	{
		return -9;
	}
	//strWhere.Format( _T("MANAGE_CODE='%s'") , m_strManageCode );
	strWhere.Format( _T("MANAGE_CODE=UDF_MANAGE_CODE"));
	m_pHolydayDM->RefreshDataManager(strWhere);
// 	pCodeDM.InitDMPKFromFile(_T("DM_USERMAIN_HOLYDAY_INFO"));
// 	ids = pCodeDM.RefreshDataManager(strWhere);
// 	if( 0 > ids )
// 	{
// 		return -10;
// 	}
// 	CopyDM2DM(&pCodeDM, m_pHolydayDM);

	return 0;
}

INT CCommonProc::CopyDM2DM(CESL_DataMgr *pSourceDM, CESL_DataMgr *pTargetDM)
{
	pTargetDM->FreeData();
	INT nRow = pSourceDM->GetRowCount();
	INT nCurRow;
	INT row, col;
	INT nCols;	
	CString strTmp;

	if (nRow == 0) return 0;		// �ҽ��� ����Ÿ�� ����
	for (row = 0; row < nRow; row++) 
	{
		nCols = pSourceDM->m_nCols;
		pTargetDM->InsertRow(-1);
		nCurRow = pTargetDM->GetRowCount() - 1;
		for (col = 0; col < pSourceDM->m_nCols; col++) 
		{
			strTmp = pSourceDM->GetCellData(row, col);
			pTargetDM->SetCellData(nCurRow, col, strTmp);
		}
	}
	return 0;
}
