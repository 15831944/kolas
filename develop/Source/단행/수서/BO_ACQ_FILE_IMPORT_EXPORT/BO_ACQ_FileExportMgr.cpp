// BO_ACQ_FileExportMgr.cpp: implementation of the CBO_ACQ_FileExportMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BO_ACQ_FileExportMgr.h"
#include "Request_DataExportDlg.h"
#include <direct.h>
#include "../../../����/INCLUDE/BO_ACQ.h"
// 2011.09.08 ADD : 3000Api
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBO_ACQ_FileExportMgr::CBO_ACQ_FileExportMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_FileExportMgr::IDD, pParent)
{
	m_pParentMgr = pParent;
}

CBO_ACQ_FileExportMgr::~CBO_ACQ_FileExportMgr()
{

}

INT CBO_ACQ_FileExportMgr::FileExportProc()
{

	INT ids;

	// ������ �����ϱ� ���� ����ڰ� ���ϴ� ��ġ�� ���´�.
	ids = GetFilePath();
	if(ids < 0) return -1;

	BOOL bCreate;
	// ���� ��ο� IMPORT_EXPORT ���� ����
	m_strExportFilePath += _T("\\IMPORT_EXPORT");
	bCreate = CreateDirectory(m_strExportFilePath, NULL);
	if(!bCreate) {
		AfxMessageBox(_T("���� ��ġ�� ���� �̸��� EXPORT ������ �����մϴ�.\r\n")
			          _T("���� ������ �ٸ� �̸����� ������ �� ����Ͻñ� �ٶ��ϴ�."));
		return -1;
	}

	// 2005-01-06 ���� �ٲ�
	// ���� ���̺� �� ��ŭ ���� ���鼭 �˻� ����� DM�� �����ϰ� DM�� ������ ���Ͽ� �ű��.
	// SM �ҷ�����.
	if (InitESL_Mgr(_T("K2UPGRADE_����_�ڷ����")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return -1;
	}			

	// ������ ���� ����� 
	ids = SetMasterFileInfo();
	if(ids < 0) {
		AfxMessageBox(_T("������������ ���� ����!"));
		return -1;
	}

	BOOL bFileExist;
	CString strSourceFile, strDestFile;

	for(INT Idx=0; Idx < m_nTableCount; Idx++)
	{

		// 1.���̺��.ctl ���� ���� �� ���� ����
		// �̸� cfg�� ������ ���� �� ���̺��� ctl ������ �����Ѵ�.
		strSourceFile.Empty();	strDestFile.Empty();
		strSourceFile.Format(_T("..\\cfg\\%s.ctl"), m_strTableNameArray[Idx]);
		strDestFile = m_strExportFilePath + _T("\\") + m_strTableNameArray[Idx] + _T(".ctl");
		bFileExist = CopyFile(strSourceFile, strDestFile, FALSE);
		if(!bFileExist) return -1;

		if((SPECIES_TYPE)m_nSpeciesType == BO_ORDER)
		{
			ids = CopyFromDMToFile_ORDER(Idx);
			if(ids < 0) return -1;
		}
		else if((SPECIES_TYPE)m_nSpeciesType == BO_REQUEST)
		{
			ids = CopyFromDMToFile_REQUEST(Idx);
			if(ids < 0) return -1;
		}
	}

	// 2005-01-16
	// �˼��ڷ� ���Ϲ����� ��� ��ġ��������� �����ϵ��� ����.
	// DM_����_���Ϲ���_������������ rec_key�� �о �� rec_key�� acq_key�� ���� ��ġ��������� �о� ���Ͽ� �����Ѵ�.
	if((SPECIES_TYPE)m_nSpeciesType == BO_ORDER)
	{
		strSourceFile.Empty();	strDestFile.Empty();
		strSourceFile.Format(_T("..\\cfg\\BO_FURNISH_TBL.ctl"));
		strDestFile = m_strExportFilePath + _T("\\BO_FURNISH_TBL.ctl");
		bFileExist = CopyFile(strSourceFile, strDestFile, FALSE);
		if(!bFileExist) return -1;

		ids = CopyFromPurchaseToFurnish_ORDER();
		if(ids < 0)
			return -1;
	}

	return 0;
}

INT CBO_ACQ_FileExportMgr::GetFilePath()
{
	//����ڰ� ������ ���� ��� ������ �ҽ�.
	ITEMIDLIST        *pidlBrowse;
    TCHAR            pszPathname[MAX_PATH];
 	TCHAR    szCurrentDir[512];
	_tgetcwd(szCurrentDir, 512);

	BROWSEINFO BrInfo;
	// BrInfo.hwndOwner = GetSafeHwnd();
    BrInfo.pidlRoot = NULL;

    memset( &BrInfo, 0, sizeof(BrInfo) );
    BrInfo.pszDisplayName = pszPathname;
    BrInfo.lpszTitle = _T("������ ������ ��ġ�� �����ϼ���");
    BrInfo.ulFlags = BIF_RETURNONLYFSDIRS ;

    // ���̾�α׸� ����
    pidlBrowse = ::SHBrowseForFolder(&BrInfo);    

    if( pidlBrowse != NULL)
    {
        // �н��� ����
        ::SHGetPathFromIDList(pidlBrowse, pszPathname);    
    }
	else return -1;

	_tchdir(szCurrentDir);   

	m_strExportFilePath = pszPathname;

	return 0;

}


/************************************************
 err -1000; //���� ���� ����.
 err -1001; //��Ÿ�Կ� �ش��ϴ� �׸��� ����.
*************************************************/
INT CBO_ACQ_FileExportMgr::SetMasterFileInfo()
{
	BOOL bError;
	INT i;
	CString strFileName, strWrite;

	CStdioFile Masterfile;
	strFileName.Format(_T("%s\\ExportFileInfo.txt"), m_strExportFilePath);

	// 2005-01-16
	CString strQuery, strResult;
	strQuery.Empty();	strResult.Empty();
	CESL_DataMgr * TmpDM;
	TmpDM = FindDM(_T("DM_����_�ڷ����_��ġ�������"));
	if(TmpDM == NULL) return -1002;

	bError = Masterfile.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
	if(!bError) return -1000;
	
	if( 0 == Masterfile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		Masterfile.Write( &cUni, sizeof(TCHAR));
	}
		
	switch((SPECIES_TYPE)m_nSpeciesType)
	{
		case BO_REQUEST:
			// ������ ���̺��� �迭�� �ִ´�.
			m_nTableCount = 0;
			m_strTableNameArray[0] = _T("BO_FURNISH_TBL");	
			m_strDMArray[0] = _T("DM_����_�ڷ����_��ġ�������");	
			m_nTableCount++;
			
			// ���������Է�
			Masterfile.WriteString(_T("#������Ÿ��=BO_REQUEST\r\n"));
			// �̺κп� �˻������׸���� �־� �ش�.
			for(i = 0; i< m_pExportColumn.GetSize(); i++)
			{
				strWrite.Format(_T("#�����׸�=%s\r\n"), m_pExportColumn.GetAt(i));
				Masterfile.WriteString(strWrite);
			}
			Masterfile.WriteString(_T("BO_FURNISH_TBL\r\n"));
			break;
		case BO_ORDER:
			// ������ ���̺��� �迭�� �ִ´�.
			m_nTableCount = 0;
			m_strTableNameArray[0] = _T("BO_PURCHASE_SEQ_NO_TBL");	m_strDMArray[0] = _T("DM_����_�ڷ����_��������");		
			m_strTableNameArray[1] = _T("BO_PURCHASE_WORK_NO_TBL");	m_strDMArray[1] = _T("DM_����_�ڷ����_�۾���ȣ����");	
			m_strTableNameArray[2] = _T("BO_SPECIES_TBL");			m_strDMArray[2] = _T("DM_����_�ڷ����_������");		
			m_strTableNameArray[3] = _T("BO_BOOK_TBL");				m_strDMArray[3] = _T("DM_����_�ڷ����_å����");		
			m_strTableNameArray[4] = _T("BO_EBOOK_TBL");			m_strDMArray[4] = _T("DM_����_�ڷ����_EBOOK����");		
			m_strTableNameArray[5] = _T("BO_PURCHASEINFO_TBL");		m_strDMArray[5] = _T("DM_����_�ڷ����_��������");		
			m_strTableNameArray[6] = _T("IDX_BO_TBL");				m_strDMArray[6] = _T("DM_����_�ڷ����_��������");	
			// 2005-01-16
			// m_strTableNameArray[7] = _T("IDX_TMP_BO_TBL");			m_strDMArray[7] = _T("DM_����_�ڷ����_�����ӽ�����");	

			m_strSelectQueryArray[0].Format(_T("Q.ACQ_YEAR = %s AND Q.SEQ_NO = %s AND Q.MANAGE_CODE = UDF_MANAGE_CODE"), 
											m_strAcqYear, m_strGroupInfo);  m_nTableCount++;
			m_strSelectQueryArray[1].Format(_T("Q.ACQ_YEAR = %s AND Q.SEQ_NO = %s AND Q.MANAGE_CODE = UDF_MANAGE_CODE"), 
											m_strAcqYear, m_strGroupInfo); m_nTableCount++;
			m_strSelectQueryArray[2].Format(_T("P.ORDER_YEAR = '%s' AND I.WORK_CODE = '%s' AND P.ORDER_SEQ_NO = %s AND ")
											_T("P.REC_KEY IN ( SELECT ACQ_KEY FROM BO_BOOK_TBL WHERE ACQ_CODE = '1') AND I.MANAGE_CODE = UDF_MANAGE_CODE"),   //ACQ_CODE = '1':����
											m_strAcqYear, m_strWorkCode, m_strGroupInfo);	m_nTableCount++;
			m_strSelectQueryArray[3].Format(_T("P.ORDER_YEAR = '%s' AND I.WORK_CODE = '%s' AND P.ORDER_SEQ_NO = %s AND B.ACQ_CODE = '1' AND I.MANAGE_CODE = UDF_MANAGE_CODE"),
											m_strAcqYear, m_strWorkCode, m_strGroupInfo);	m_nTableCount++;
			m_strSelectQueryArray[4].Format(_T("P.ORDER_YEAR = '%s' AND I.WORK_CODE = '%s' AND P.ORDER_SEQ_NO = %s AND B.ACQ_CODE = '1' AND I.MANAGE_CODE = UDF_MANAGE_CODE"),
											m_strAcqYear, m_strWorkCode, m_strGroupInfo);	m_nTableCount++;
			m_strSelectQueryArray[5].Format(_T("P.ORDER_YEAR = '%s' AND I.WORK_CODE = '%s' AND P.ORDER_SEQ_NO = %s AND I.MANAGE_CODE = UDF_MANAGE_CODE"),
											m_strAcqYear, m_strWorkCode, m_strGroupInfo);	m_nTableCount++;
			m_strSelectQueryArray[6].Format(_T("P.ORDER_YEAR = '%s' AND I.WORK_CODE = '%s' AND P.ORDER_SEQ_NO = %s AND I.MANAGE_CODE = UDF_MANAGE_CODE"),
											m_strAcqYear, m_strWorkCode, m_strGroupInfo);	m_nTableCount++;
			// m_strSelectQueryArray[7].Format(_T("P.ORDER_YEAR = '%s' AND I.WORK_CODE = '%s' AND P.ORDER_SEQ_NO = %s"),
			//								m_strAcqYear, m_strWorkCode, m_strGroupInfo);	m_nTableCount++;

			// ���������Է�
			Masterfile.WriteString(_T("#������Ÿ��=BO_ORDER\r\n"));
			strWrite.Format(_T("#���Գ⵵=%s\r\n"), m_strAcqYear);
			Masterfile.WriteString(strWrite);
			strWrite.Format(_T("#������ȣ=%s\r\n"), m_strGroupInfo);
			Masterfile.WriteString(strWrite);
			strWrite.Format(_T("#��������=%s\r\n"), m_strWorkCode);
			Masterfile.WriteString(strWrite);

			for(i=0; i < m_nTableCount; i++){
				strWrite.Format(_T("%s\r\n"), m_strTableNameArray[i]);
				Masterfile.WriteString(strWrite);
			}
			break;
		default: 
			Masterfile.Close();
			return -1001; 
			break;
	}

	Masterfile.Close();
	return 0;
}

INT CBO_ACQ_FileExportMgr::CopyFromDMToFile_ORDER(INT Idx)
{
	INT ids;
	INT nRowCount = 0;   // �˻� ��� Row ��
	CString strQuery, strFile, strRowData, strTempData;
	CESL_DataMgr  *pDM;
	CStdioFile tablefile;

	// 2. DM�� ���� ������ ��� �޾ƿ���.
	pDM = FindDM(m_strDMArray[Idx]);
	ids = pDM->RefreshDataManager(m_strSelectQueryArray[Idx]);
	if(ids < 0) return -1;
	nRowCount = pDM->GetRowCount();
	
	// 3. ���̺��.dat ���� ���� �� ���� ����.
	strFile.Empty(); 
	strFile = m_strExportFilePath + _T("\\") + m_strTableNameArray[Idx] + _T(".dat");
	tablefile.Open(strFile, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);

	if( 0 == tablefile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		tablefile.Write( &cUni, sizeof(TCHAR));
	}
		
	// 4. ���Ͽ� �˻� ��� DM�� RowCount��ŭ ���� ���鼭 �� �÷��� ������ �����Ѵ�.
	strRowData.Empty();
	for(INT i=0; i< nRowCount; i++)
	{
		for(INT j=0; j < pDM->GetColCount(); j++)
		{
			strTempData.Empty();
			strTempData = pDM->GetCellData(i, j);
			// 2005-01-12 
			// �����Ϳ� ���Ͱ� �ִ� ���� ���͸� ���ְ� �����Ѵ�.
			strTempData.Replace(_T("\r\n"), _T(""));
			strRowData += _T("<start>") + strTempData + _T("<end>");
		}
		strRowData += _T("|\r\n");
		tablefile.WriteString(strRowData);
		strRowData.Empty();
	}

	tablefile.Close();
	pDM = NULL;
	
	return 0;
}

INT CBO_ACQ_FileExportMgr::CopyFromDMToFile_REQUEST(INT Idx)
{
	INT ids;
	INT nRowCount = 0;   // �˻� ��� Row ��
	CString strFile, strQuery, strRec_key, strRowData, strTempData;
	CESL_DataMgr  *pDM;
	CStdioFile tablefile;

	// 2. DM�� ���� ������ ��� �޾ƿ���.
	pDM = FindDM(m_strDMArray[Idx]);
	
	// 3. ���̺��.dat ���� ���� �� ���� ����.
	strFile.Empty();
	strFile = m_strExportFilePath + _T("\\") + m_strTableNameArray[Idx] + _T(".dat");
	tablefile.Open(strFile, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);

	if( 0 == tablefile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		tablefile.Write( &cUni, sizeof(TCHAR));
	}
		
	// 4. ���Ͽ� �˻� ��� DM�� RowCount��ŭ ���� ���鼭 �� �÷��� ������ �����Ѵ�.
	strRowData.Empty();

	nRowCount = m_SelectedRowIdxArray->GetSize();

	for(INT i=0; i< nRowCount; i++)
	{
		strRec_key = m_SelectedRowIdxArray->GetAt(i);
		strQuery.Format(_T("REC_KEY = %s"), strRec_key);
		ids = pDM->RefreshDataManager(strQuery);
		if(ids < 0) return -1;
		if(pDM->GetRowCount() != 1 ) return -1;

		for(INT j=0; j < pDM->GetColCount(); j++)
		{
			strTempData.Empty();
			strTempData = pDM->GetCellData(0, j);
			// 2005-01-12 
			// �����Ϳ� ���Ͱ� �ִ� ���� ���͸� ���ְ� �����Ѵ�.
			strTempData.Replace(_T("\r\n"), _T(""));
			strRowData += _T("<start>") + strTempData + _T("<end>");
		}
		strRowData += _T("|\r\n");
		tablefile.WriteString(strRowData);
		strRowData.Empty();
	}

	tablefile.Close();
	pDM = NULL;

	return 0;
}

INT CBO_ACQ_FileExportMgr::SetExportColumn()
{

	CRequest_DataExportDlg Dlg(m_pParentMgr);
	if(Dlg.DoModal() == IDCANCEL) return -1;

	for(INT i=0; i < Dlg.m_strCheckArray.GetSize(); i++)
	{
		m_pExportColumn.Add(Dlg.m_strCheckArray.GetAt(i));
	}

	return 0;
}

INT CBO_ACQ_FileExportMgr::CopyFromPurchaseToFurnish_ORDER()
{
	// 2005-01-16
	// ��ġ����ڷ��� ��� DM�� ��ġ����ڷ�_���Ϲ���� ���� ���� ������ 
	// ���� ���� ���� ���ϰ�, ��������TBL���� acq_key�� ���ͼ� �� acq_key�� ���� ��ġ����ڷḦ �˻��Ͽ� �����Ѵ�.

	INT ids;
	INT nPurchaseRowCount = 0;   // �˻� ��� Row ��
	INT nFurnishCount = 0;		 // acq_key�� �����ϴ� �����Ͱ���
	INT nColCount = 0;			 // ��ġ��������� �÷�����
	CString strFile, strAcq_key, strQuery, strTempData, strRowData;
	CESL_DataMgr * pPurchaseDM;
	CESL_DataMgr * pFurnishDM;
	CStdioFile tablefile;

	pPurchaseDM = FindDM(_T("DM_����_�ڷ����_��������"));
	pFurnishDM = FindDM(_T("DM_����_�ڷ����_��ġ�������"));
	if(pPurchaseDM == NULL || pFurnishDM== NULL) return -1001;

	// 3. ���̺��.dat ���� ���� �� ���� ����.
	strFile.Empty();
	strFile = m_strExportFilePath + _T("\\BO_FURNISH_TBL.dat");
	tablefile.Open(strFile, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);

	if( 0 == tablefile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		tablefile.Write( &cUni, sizeof(TCHAR));
	}
		
	// 4. ���Ͽ� �˻� ��� ��������DM�� RowCount��ŭ ���� ���鼭 �� ���ڵ��� REC_Key�� �����´�.
	nPurchaseRowCount = pPurchaseDM->GetRowCount();

	for(INT i=0; i< nPurchaseRowCount; i++)
	{
		strAcq_key = pPurchaseDM->GetCellData(_T("BP_REC_KEY"), i);
		if(strAcq_key.IsEmpty()) return -1;
		strQuery.Format(_T("ACQ_KEY = %s"), strAcq_key);

		ids = pFurnishDM->RefreshDataManager(strQuery);
		if(ids < 0) return -1;

		nFurnishCount = pFurnishDM->GetRowCount();
		if(nFurnishCount < 1 ) continue;
		
		// 2011.09.08 ADD : 3000Api
		if(TRUE == m_pInfo->m_bLibSecuUse)
		{
			CStringArray asDes1,asDes2,asDes3,asDes4,asDes5;
			CString strGetData;
			for(INT i = 0; i < pFurnishDM->GetRowCount(); i++)
			{
				pFurnishDM->GetCellData(_T("�ֹε�Ϲ�ȣ"), i, strGetData);
				asDes1.Add(strGetData);
				pFurnishDM->GetCellData(_T("�ּ�"), i, strGetData);
				asDes2.Add(strGetData);
				pFurnishDM->GetCellData(_T("��ȭ��ȣ"), i, strGetData);
				asDes3.Add(strGetData);
				pFurnishDM->GetCellData(_T("�ڵ���"), i, strGetData);
				asDes4.Add(strGetData);
				pFurnishDM->GetCellData(_T("���ڿ���"), i, strGetData);
				asDes5.Add(strGetData);
			}
			CBO_LOC_3000Api *pApi = new CBO_LOC_3000Api(this);
			if(NULL != pApi)
			{
				if(TRUE == pApi->CheckConnection())
				{
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("CIVIL_NO"), asDes1);
						pApi->CloseSocket();
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("ADDRESS"), asDes2);
						pApi->CloseSocket();
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("TEL"), asDes3);
						pApi->CloseSocket();
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("HANDPHONE"), asDes4);
						pApi->CloseSocket();
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("EMAIL"), asDes5);
						pApi->CloseSocket();
					}
					for(i = 0; i < pFurnishDM->GetRowCount(); i++)
					{
						pFurnishDM->SetCellData(_T("�ֹε�Ϲ�ȣ"), asDes1.GetAt(i), i);
						pFurnishDM->SetCellData(_T("�ּ�"), asDes2.GetAt(i), i);
						pFurnishDM->SetCellData(_T("��ȭ��ȣ"), asDes3.GetAt(i), i);
						pFurnishDM->SetCellData(_T("�ڵ���"), asDes4.GetAt(i), i);
						pFurnishDM->SetCellData(_T("���ڿ���"), asDes5.GetAt(i), i);
					}
				}
				delete pApi;
				pApi = NULL;
			}
			asDes1.RemoveAll(); asDes1.FreeExtra();
			asDes2.RemoveAll(); asDes2.FreeExtra();
			asDes3.RemoveAll(); asDes3.FreeExtra();
			asDes4.RemoveAll(); asDes4.FreeExtra();
			asDes5.RemoveAll(); asDes5.FreeExtra();
		}

		nColCount = pFurnishDM->GetColCount();
		for(INT j=0; j < nFurnishCount; j++)
		{
			for(INT k=0; k < nColCount; k++)
			{
				strTempData.Empty();
				strTempData = pFurnishDM->GetCellData(j, k);
				// 2005-01-12 
				// �����Ϳ� ���Ͱ� �ִ� ���� ���͸� ���ְ� �����Ѵ�.
				strTempData.Replace(_T("\r\n"), _T(""));
				strRowData += _T("<start>") + strTempData + _T("<end>");
			}

			strRowData += _T("|\r\n");
			tablefile.WriteString(strRowData);
			strRowData.Empty();
		}
	}

	tablefile.Close();

	// MasterFile�� _T("BO_FURNISH_TBL") �߰�.
	BOOL bError;
	CString strFileName;
	CStdioFile Masterfile;
	strFileName.Format(_T("%s\\ExportFileInfo.txt"), m_strExportFilePath);
	bError = Masterfile.Open(strFileName, CFile::modeWrite | CFile::typeBinary);
	if(!bError) return -1000;

	if( 0 == Masterfile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		Masterfile.Write( &cUni, sizeof(TCHAR));
	}
		
	Masterfile.SeekToEnd();
	Masterfile.WriteString(_T("BO_FURNISH_TBL"));
	Masterfile.Close();

	return 0;
}
