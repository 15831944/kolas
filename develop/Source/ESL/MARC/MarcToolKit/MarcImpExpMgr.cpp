// MarcImpExpMgr.cpp: implementation of the CMarcImpExpMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MarcImpExpMgr.h"
#include "MarcConvert.h"
#include "efs.h"
//20071206 ADD PJW : CharEncodeMgr ���� Ŭ���� ���
#include "CharConvert.h"
#include "..\\..\\..\\.\\����\\MARC_CONVERT\\MarcConvertApi.h"
// 16/03/29 ������ : �Ѽ� �����ϱ�
//*/ ADD -------------------------------------------------------------------------------------
#include "ExportToHCell.h"
//*/ END -------------------------------------------------------------------------------------
//2020.01.29 JOB.2020.0009 ADD BY KYJ : CSV���� ���� �� �ֵ��� �߰�
#include "ExportToCSV.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//20071206 ADD PJW : FILE MODE �߰�
#define _ANSI_FILE 0
#define _UTF8_FILE 1
#define _UNICODE_FILE 2

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMarcImpExpMgr::CMarcImpExpMgr()
{
	m_pParentWnd = NULL;
	m_pMarcMgr = NULL;
	m_nFileType = XLS;
	m_strFilePath.Empty();
	m_strLastError.Empty();

	m_nFileType = XLS;

	m_pCallBackFunc = NULL;

	bRegNoAutoGrant = FALSE;

	m_pCodeMgr = NULL;
	m_nPubFreqCodeCnt = -1;
}

CMarcImpExpMgr::~CMarcImpExpMgr()
{
	
}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////
INT CMarcImpExpMgr::Init(CWnd *pParentWnd, CMarcMgr *pMarcMgr, INT nFileType, CString strFilePath)
{
	EFS_BEGIN

	InitDMList();
	SetParentWnd(pParentWnd);
	SetMarcMgr(pMarcMgr);
	SetFileType(nFileType);
	SetFilePath(strFilePath);
	
	return 0;

	EFS_END
	return -1;

}

INT CMarcImpExpMgr::InitDMList()
{
	EFS_BEGIN

	m_listDM.RemoveAll();
	return 0;

	EFS_END
	return -1;

}

INT CMarcImpExpMgr::AddDMPtr(CESL_DataMgr *pDM)
{
	EFS_BEGIN

	if (pDM == NULL) return -1;

	m_listDM.AddTail((CObject*)pDM);
	return 0;

	EFS_END
	return -1;

}

INT CMarcImpExpMgr::ImportFile(CString strFilePath , INT nMarcType)
{
	EFS_BEGIN

	INT ids;

	if (m_listDM.GetCount() <= 0) return -1;

	if (m_pParentWnd == NULL) return -1;
	if (m_pMarcMgr == NULL) return -1;
	if (m_strFilePath.IsEmpty()) return -1;
	if (!strFilePath.IsEmpty()) SetFilePath(strFilePath);

	CESL_DataMgr *pDM = NULL;
	POSITION pos = m_listDM.GetHeadPosition();
	while(pos)
	{
		pDM = (CESL_DataMgr*)m_listDM.GetNext(pos);
		if (pDM == NULL) continue;

		pDM->FreeData();
	}
	
	if (m_nFileType == XLS)
	{
		// 16/03/29 ������ : �Ѽ� �����ϱ�
//*/ ADD -------------------------------------------------------------------------------------
		// 16/04/05 ������ : ��ȣ���� ������ ���� �ӽ��ּ�ó�� -> // 16/04/19 ������ : �ּ�Ǯ��
		CLSID clsExcelApp;
		if (!FAILED(CLSIDFromProgID(_T("HCell.Application"), &clsExcelApp))) 
		{
			ImportHCellFile();
		}
		else
//*/ END -------------------------------------------------------------------------------------
		{
			ImportXlsFile();
		}
	}
	else if (m_nFileType == TXT)
	{
		ids = ImportTxtFile(nMarcType);
		if ( ids ) return ids;
	}
	else if (m_nFileType == PARTITION_TXT )
	{
		ImportPartitionTxtFile();
	}
	else if (m_nFileType == MODS )
	{
		ImportMODSFile();
	}
	//2020.01.29 JOB.2020.0009 ADD BY KYJ : CSV���� ���� �� �ֵ��� �߰�
	else if (m_nFileType == CSV )
	{
		ImportCSVFile();
	}

	
	return 0;

	EFS_END
	return -1;

}

//2020.01.29 JOB.2020.0009 ADD BY KYJ : CSV���� ���� �� �ֵ��� �߰�
INT CMarcImpExpMgr::ImportCSVFile()
{
	EFS_BEGIN
		CExportToCSV CSVMgr;
	
	if( CSVMgr.Open(m_strFilePath) == FALSE ) {
		m_strLastError = _T("Open Failed");
		return -1;
	}
	
	//��ũ������ �����̸� ����� ���� �����͸� ���� ������ �ִ´�
	BOOL bIsSerial = FALSE;
	
	if(_T("����") == m_pMarcMgr->GetMarcGroup())
	{
		if(m_pCodeMgr)
		{
			bIsSerial = TRUE;
			BringPubFreqFromCodeMgr();
			
		}
	}
	
	m_arrayExcelHead.RemoveAll();
	
	CStringArray saColumn;
	CSVMgr.GetAlias(saColumn);
	
	for (int i = 0; i < CSVMgr.GetColCount(); i++ )
	{
		m_arrayExcelHead.Add( saColumn.GetAt(i) );
	}
	
	CMarc marc;
	CString strItem;
	CString strRow;
	CString strHeadAlias;
	INT nBookCnt;
	INT nRowCount = 0;
	CString strPubFreqCode;
	
	while (TRUE)
	{
		if ( nRowCount >= CSVMgr.GetRowCount() ) break;
		
		marc.Init();
		for (INT colidx = 0; colidx < CSVMgr.GetColCount(); colidx++)
		{
			CSVMgr.GetCellData( nRowCount, colidx, strItem);
			strHeadAlias = m_arrayExcelHead.GetAt(colidx);
			
			if(strHeadAlias == _T("å��"))
			{
				nBookCnt = _ttoi(strItem.GetBuffer(0));
				for(INT j=0 ; j<nBookCnt ; j++)
				{
					m_pMarcMgr->SetItem(&marc, _T("049$l"), strItem);
				}
			}
			else if(strHeadAlias == _T("�����") && bIsSerial)
			{
				if(GetPubFreqCode(strItem,strPubFreqCode)>=0)
				{
					
					if(_T("_") == strPubFreqCode)
					{
						m_pMarcMgr->SetItem(&marc,_T("008@18"),_T(" "));
					}
					else
					{
						m_pMarcMgr->SetItem(&marc,_T("008@18"),strPubFreqCode);	
					}
				}
			}
			else
			{
				m_pMarcMgr->SetItem(&marc, m_arrayExcelHead.GetAt(colidx), strItem);
			}
			
		}
		
		m_pMarcMgr->CheckDeepSyntax(&marc);
		m_pMarcMgr->CheckSimpleSyntax(&marc);
		m_pMarcMgr->DeleteEmptySubfield(&marc);
		m_pMarcMgr->DeleteEmptyField(&marc);
		MakeDM(&marc);
		
		nRowCount++;		
	}	
	
	return 0;
	
	EFS_END
		
		return -1;
}

INT CMarcImpExpMgr::ImportXlsFile()
{
	EFS_BEGIN

	C_ExcelManControl excelCtrl;
	CRect rect;
	m_pParentWnd->GetWindowRect(rect);

	excelCtrl.Create(_T("Excel Control"), BS_DEFPUSHBUTTON, rect, m_pParentWnd, WM_USER + 10000);
	if (excelCtrl.m_hWnd == NULL) {
		m_strLastError = _T("Excelmanager.ocx�� ��ġ�Ǿ� ���� �ʽ��ϴ�. ��ġ�Ͽ� �ֽʽÿ�.");
		return -1;
	}
    CExcelManager excelMgr;
	excelMgr.SetManager(&excelCtrl);
		
    if(excelMgr.OpenManager() == FALSE) 
	{
        m_strLastError = _T("NOT Installed Excelmanager.ocx or No Excel!!!");
		return -1;
	}

	excelMgr.SetUseTemplate(TRUE, m_strFilePath);
	if (excelMgr.OpenManager() == FALSE)
	{
		m_strLastError = _T("Open Failed");
		return -1;
	}

	//��ũ������ �����̸� ����� ���� �����͸� ���� ������ �ִ´�
	BOOL bIsSerial = FALSE;

	if(_T("����") == m_pMarcMgr->GetMarcGroup())
	{
		if(m_pCodeMgr)
		{
			bIsSerial = TRUE;
			BringPubFreqFromCodeMgr();
	
		}
	}


	// Get Header
	m_arrayExcelHead.RemoveAll();
	
	CString strAlias;
	INT nColCount = 0;
	while (TRUE)
	{
		strAlias = excelMgr.GetTextMatrix(1, nColCount + 1);
		strAlias.TrimLeft();
		strAlias.TrimRight();
		strAlias.Replace(_T(" "), _T(""));
		if (strAlias.IsEmpty()) break;

		m_arrayExcelHead.Add(strAlias);
		nColCount++;
	}

	// Fetch Row
	CMarc marc;
	CString strItem;
	CString strRow;
	CString strHeadAlias;
	INT nBookCnt;
	INT nRowCount = 1;
	CString strPubFreqCode;

	while (TRUE)
	{
		
		strRow.Empty();
		marc.Init();
		for (INT i = 0; i < nColCount; i++)
		{
			strItem = excelMgr.GetTextMatrix(nRowCount + 1, i + 1);
			strHeadAlias = m_arrayExcelHead.GetAt(i);
			
			/*
			if(strHeadAlias == _T("å��"))
			{
				nBookCnt = _ttoi(strItem.GetBuffer(0));
				for(INT j=0 ; j<nBookCnt ; j++)
				{
					m_pMarcMgr->SetItem(&marc, _T("049$l"), strItem);
				}
			}
			else
				m_pMarcMgr->SetItem(&marc, m_arrayExcelHead.GetAt(i), strItem);
			*/


			//modified by loveisverb 2004-03-12
			if(strHeadAlias == _T("å��"))
			{
				nBookCnt = _ttoi(strItem.GetBuffer(0));
				for(INT j=0 ; j<nBookCnt ; j++)
				{
					m_pMarcMgr->SetItem(&marc, _T("049$l"), strItem);
				}
			}
			else if(strHeadAlias == _T("�����") && bIsSerial)
			{
				if(GetPubFreqCode(strItem,strPubFreqCode)>=0)
				{
					
					if(_T("_") == strPubFreqCode)
					{
						m_pMarcMgr->SetItem(&marc,_T("008@18"),_T(" "));
					}
					else
					{
						m_pMarcMgr->SetItem(&marc,_T("008@18"),strPubFreqCode);	
					}
				}
			}
			else
			{
				m_pMarcMgr->SetItem(&marc, m_arrayExcelHead.GetAt(i), strItem);
			}
			strRow += strItem;
		}

		strRow.TrimLeft();
		strRow.TrimRight();
		if (strRow.IsEmpty()) break;
		nRowCount++;
		m_pMarcMgr->CheckDeepSyntax(&marc);
		m_pMarcMgr->CheckSimpleSyntax(&marc);
		m_pMarcMgr->DeleteEmptySubfield(&marc);
		m_pMarcMgr->DeleteEmptyField(&marc);
		MakeDM(&marc);
	}	
	
	excelMgr.CloseManager();
	
	return 0;

	EFS_END
	return -1;

}

INT CMarcImpExpMgr::ImportTxtFile(INT nMarcType)
{
	EFS_BEGIN

	//20071208 UPDATE PJW : UTF8_FILE, ANSI_FILE�� UNICODE_FILE�� ��ȯ�ؼ� �����ϵ��� �߰� 
	CMarcConvertApi MarcConvertApi;
	MarcConvertApi.m_nConvertModeChk = _UNICODE_FILE;
    FileCodeChange(m_strFilePath);

	CStdioFile file;
	if (!file.Open(m_strFilePath, CFile::modeRead | CFile::typeBinary))
	{
		m_strLastError = _T("File Open Failed!!");
		return -1;
	}

	TCHAR cUniTemp;
	file.Read( &cUniTemp, sizeof(TCHAR) );

	
	CString			strLine;	
	CMarcConvert	MarcConvert;
	CMarc			marc;
	TCHAR			*szpStreamMarc;
	INT				ids;

	if ( nMarcType == _USMARC_ )
	{
		szpStreamMarc = new TCHAR[100000];
	}
	
	file.ReadString(strLine);
	do 
	{ 
		INT nLength = strLine.GetLength();
		if ( nLength < 20 ) 
		{
			file.Close();
			AfxMessageBox(_T("�ùٸ� Marc ������ �ƴմϴ�."));
			return -100;
		}
		
		MarcConvertApi.GetMarcStruct(strLine);
		strLine.Format(_T("%s"),MarcConvertApi.m_strUniConvertMarc);
		
/*		if ( nMarcType == _USMARC_ )
		{	
			MarcConvert.ConvertUSMarcToKormarc(strLine.GetBuffer(0) ,szpStreamMarc );
			strLine.Format(_T("%s"), szpStreamMarc );			
		}*/
		marc.Init();
		strLine.TrimLeft();
		strLine.TrimRight();
		if (strLine.IsEmpty()) break;
	
		// ��Ʈ�� ��ũ�� ��ġ �Ѵ�.
		while (TRUE)
		{
			CString strStreamMarc;
			
			INT nFind = strLine.Find(RECORD_TERMINATOR);
			
			if ( nFind == -1 ) {
				nFind = strLine.Find(_T("&"));
				if ( nFind != -1 ) {
					CString sRecordTerminator;
					sRecordTerminator.Format( _T("%c"), RECORD_TERMINATOR );
					strLine.Replace( _T("&"), sRecordTerminator );

					CString sFieldTerMinator;
					sFieldTerMinator.Format( _T("%c"), FIELD_TERMINATOR );				
					strLine.Replace( _T("%"), sFieldTerMinator );

					CString sSubFieldCode;
					sSubFieldCode.Format( _T("%c"), SUBFIELD_CODE );				
					strLine.Replace( _T("$"), sSubFieldCode );
				}
			}
			
			if (nFind < 0)
				break;
			else
			{
				if (nFind == strLine.GetLength() - 1)
				{
					strStreamMarc = strLine;
					strLine.Empty();
				}
				else
				{
					strStreamMarc = strLine.Mid(0, nFind + 1);
					strLine = strLine.Mid(nFind + 1);
				}
			}
			
			// ���� HJY : ���ڵ� ���� ���� ���� ��ȯ�Ѵ�.
			if ( nMarcType == _USMARC_ )
			{	
				MarcConvert.ConvertUSMarcToKormarc((LPTSTR)(LPCTSTR)strStreamMarc, szpStreamMarc);
				strStreamMarc.Format(_T("%s"), szpStreamMarc );			
			}

			ids = m_pMarcMgr->Decoding(strStreamMarc, &marc);
			if (ids < 0) continue;
			
			MakeDM(&marc);
		}
	} while (file.ReadString(strLine));

	if ( nMarcType == _USMARC_ )
	{
		// KOL.UDF.022
		//delete szpStreamMarc;
		delete[] szpStreamMarc;
	}

	//INT iLen = strLine.GetLength();
	file.Close();

	return 0;

	EFS_END
	return -1;

}

INT CMarcImpExpMgr::ImportMODSFile(INT nMarcType)
{
	EFS_BEGIN

	//20071208 UPDATE PJW : UTF8_FILE, ANSI_FILE�� UNICODE_FILE�� ��ȯ�ؼ� �����ϵ��� �߰� 
	CMarcConvertApi MarcConvertApi;
	MarcConvertApi.m_nConvertModeChk = _UNICODE_FILE;
    FileCodeChange(m_strFilePath);

	CStdioFile file;
	if (!file.Open(m_strFilePath, CFile::modeRead | CFile::typeBinary))
	{
		m_strLastError = _T("File Open Failed!!");
		return -1;
	}

	TCHAR cUniTemp;
	file.Read( &cUniTemp, sizeof(TCHAR) );
	
	CString			strLine;
	CMarcConvert	MarcConvert;
	CMarc			marc;
	TCHAR			*szpStreamMarc;
	INT				ids;
	
	CModsConvert cmc;	
	CString strStreamMarc;
	szpStreamMarc = new TCHAR[100000];
	CString strMods;

	strMods = _T("");
	while ( file.ReadString(strLine) )
	{ 
		strLine.TrimLeft();
		strLine.TrimRight();
		if (strLine.IsEmpty())  continue;

		if ( strLine.Find(_T("<MODS"), 0) >= 0 || strLine.Find(_T("<mods"), 0) >= 0)
		{
			strMods = strLine;
			continue;
		}		
		else if ( strLine.Find(_T("</mods"), 0) >= 0 || strLine.Find(_T("</mods"), 0) >= 0 )
		{
			strMods += strLine;			
		}
		else
		{
			strMods += strLine;
			continue;
		}		

		cmc.ConvertMODStoMARC(strMods.GetBuffer(0), szpStreamMarc);
		strStreamMarc.Format(_T("%s"), szpStreamMarc);

		if ( nMarcType == _USMARC_ )
		{	
			MarcConvert.ConvertUSMarcToKormarc(strStreamMarc.GetBuffer(0) ,szpStreamMarc );
			strStreamMarc.Format(_T("%s"), szpStreamMarc );			
		}
		
		MarcConvertApi.GetMarcStruct(strStreamMarc);
		strStreamMarc.Format(MarcConvertApi.m_strUniConvertMarc);

		marc.Init();
		// ��Ʈ�� ��ũ�� ��ġ �Ѵ�.			

		ids = m_pMarcMgr->Decoding(strStreamMarc, &marc);
		if (ids < 0) continue;
		
		MakeDM(&marc);
		
	} 
	// KOL.UDF.022
	//delete szpStreamMarc;
	delete[] szpStreamMarc;
	

	//INT iLen = strLine.GetLength();
	file.Close();

	return 0;

	EFS_END
	return -1;

}

// 16/03/29 ������ : �Ѽ� �����ϱ�
//*/ ADD -------------------------------------------------------------------------------------
INT CMarcImpExpMgr::ImportHCellFile()
{
	EFS_BEGIN

	CRect rect;
	m_pParentWnd->GetWindowRect(rect);

	// COM ���� DLL�� �ε��ؼ� COM���� ��ɾ ����� �� �ֵ��� ���ش�
	CoInitialize(NULL);
	CExportToHCell *HCellMgr;
	HCellMgr = new CExportToHCell;				

	// ���� ���� ��� ���� �ʱ�ȭ���� �����Ѵ�.
	INT ids = HCellMgr->InitExcel();
	if( ids ) 
	{
		HCellMgr->Release();
		delete HCellMgr;
		HCellMgr = NULL;		
		return -1;
	}

	if (HCellMgr->Open(m_strFilePath) != 0)
	{
		m_strLastError = _T("Open Failed");
		HCellMgr->Close();
		delete HCellMgr;
		HCellMgr = NULL;
		return -1;
	}

	//��ũ������ �����̸� ����� ���� �����͸� ���� ������ �ִ´�
	BOOL bIsSerial = FALSE;
	if(_T("����") == m_pMarcMgr->GetMarcGroup())
	{
		if(m_pCodeMgr)
		{
			bIsSerial = TRUE;
			BringPubFreqFromCodeMgr();
		}
	}

	// Get Header
	m_arrayExcelHead.RemoveAll();
	
	CString strAlias;
	INT nColCount = 0;
	while (TRUE)
	{
		strAlias = HCellMgr->GetValue(nColCount + 1, 1);
		strAlias.TrimLeft();
		strAlias.TrimRight();
		strAlias.Replace(_T(" "), _T(""));
		if (strAlias.IsEmpty()) break;

		m_arrayExcelHead.Add(strAlias);
		nColCount++;
	}

	// Fetch Row
	CMarc marc;
	CString strItem;
	CString strRow;
	CString strHeadAlias;
	INT nBookCnt;
	INT nRowCount = 1;
	CString strPubFreqCode;

	while (TRUE)
	{	
		strRow.Empty();
		marc.Init();
		for (INT i = 0; i < nColCount; i++)
		{
			strItem = HCellMgr->GetValue(i + 1, nRowCount + 1);
			strHeadAlias = m_arrayExcelHead.GetAt(i);

			//modified by loveisverb 2004-03-12
			if(strHeadAlias == _T("å��"))
			{
				nBookCnt = _ttoi(strItem.GetBuffer(0));
				for(INT j=0 ; j<nBookCnt ; j++)
				{
					m_pMarcMgr->SetItem(&marc, _T("049$l"), strItem);
				}
			}
			else if(strHeadAlias == _T("�����") && bIsSerial)
			{
				if(GetPubFreqCode(strItem,strPubFreqCode)>=0)
				{
					
					if(_T("_") == strPubFreqCode)
					{
						m_pMarcMgr->SetItem(&marc,_T("008@18"),_T(" "));
					}
					else
					{
						m_pMarcMgr->SetItem(&marc,_T("008@18"),strPubFreqCode);	
					}
				}
			}
			else
			{
				m_pMarcMgr->SetItem(&marc, m_arrayExcelHead.GetAt(i), strItem);
			}
			strRow += strItem;
		}

		strRow.TrimLeft();
		strRow.TrimRight();
		if (strRow.IsEmpty()) break;
		nRowCount++;
		m_pMarcMgr->CheckDeepSyntax(&marc);
		m_pMarcMgr->CheckSimpleSyntax(&marc);
		m_pMarcMgr->DeleteEmptySubfield(&marc);
		m_pMarcMgr->DeleteEmptyField(&marc);
		MakeDM(&marc);
	}	
	
	// ����� ���� ��ü�� ��ȯ�Ѵ�.		
	HCellMgr->Close();
	delete HCellMgr;
	HCellMgr = NULL;
	
	return 0;

	EFS_END
	return -1;

}
//*/ END -------------------------------------------------------------------------------------

INT CMarcImpExpMgr::CheckMarcSyntax(CMarc *pMarc, CString &strMarcCheckResult)
{
	if (pMarc == NULL) return -1;

	strMarcCheckResult.Empty();

	CStringArray arrStrMarcCheckErr;
	CString strTemp;

	INT ids = -1;
	// ������, ���ñ�ȣ �ڵ�����
	ids = m_pMarcMgr->CheckSimpleSyntax(pMarc);
	// ���� üũ
	ids = m_pMarcMgr->CheckSimpleSyntax(pMarc);
	if (ids < 0) 
	{
		CString strTagCode;
		CString strErrMsg;
		CString strHelpMsg;
		POSITION pos = m_pMarcMgr->GetHeadPositionErrMsg();
		if (pos)
		{
			m_pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			strTemp.Format(_T("[%s] : %s - %s\r\n"), strTagCode, strErrMsg, strHelpMsg);
			arrStrMarcCheckErr.Add(strTemp);
		}
	}

	//===================================================
	//2010.05.03 ADD BY PJW : ������ ���ڰ� �� �ִ��� üũ�Ѵ�.
	CString strTempPrice;
	ids = m_pMarcMgr->GetItem(pMarc, _T("950$b"), strTempPrice);
	if( !strTempPrice.IsEmpty() )
	{
		TCHAR ch = 0;
		INT i;
		INT nLength = 0;
		INT nMidPos = 0;
		nLength = strTempPrice.GetLength();
		for (i = 0; i < nLength; i++)
		{
			ch = strTempPrice.GetAt(i);
			if ( _istdigit(ch) )
			{
				strTempPrice = strTempPrice.Mid(nMidPos);
				break;
			}
			++nMidPos;
		}

		nLength = strTempPrice.GetLength();
		for ( i = 0; i < nLength; i++)
		{
			ch = strTempPrice.GetAt(i);
			if ( !_istdigit(ch) )
			{
				strTemp.Format(_T("[950$b] : ���� ���� �� �� �Ǿ����ϴ�. �������ּ���\r\n"));
				arrStrMarcCheckErr.Add(strTemp);
				break;
			}
		}
	}
	//===================================================
	
	// �ݹ����
	if (m_pCallBackFunc)
	{
		ids = (m_pParentWnd->*m_pCallBackFunc)(pMarc, strTemp);
		if (ids < 0)
			arrStrMarcCheckErr.Add(strTemp);
	}

	// ���� ��ġ
	INT nCount = arrStrMarcCheckErr.GetSize();
	for (INT idx = 0; idx < nCount; idx++)
	{
		strMarcCheckResult += arrStrMarcCheckErr.GetAt(idx);
	}

	if (nCount <= 0) 
		return 0;
	else
		return -1;
}

VOID CMarcImpExpMgr::MakeDM(CMarc *pMarc)
{
	EFS_BEGIN

	if (pMarc == NULL) return;
	
	CString strISBN;
	CString strToc;
	CString strAbs;
	INT ids;
	INT nIdx;
	BOOL bTocFind;
	BOOL bAbsFind;
	

	CArray<CString, CString&> arrItem;
	CString		 strItemData;
	if ( bRegNoAutoGrant )
	{		
		m_pMarcMgr->DeleteItem(pMarc,_T("049$f"));	
	}
	
	// �ϴ� ����üũ�� �Ѵ�.
	CString strMarcCheckResult;
	ids = CheckMarcSyntax(pMarc, strMarcCheckResult);
	
	// �����͸� �־��ش�.
	CESL_DataMgr *pDM = NULL;
	POSITION pos = m_listDM.GetHeadPosition();
	while (pos)
	{
		
		pDM = (CESL_DataMgr*)m_listDM.GetNext(pos);
		if (pDM == NULL) continue;

		ids = pDM->InsertRow(-1);
		if (ids < 0) 
			return;

		CString strAlias[MAX_PATH];
		INT nColCount = pDM->RefList.GetCount();
		ids = pDM->GetAllAlias(strAlias, nColCount);
		if (ids < 0)
			return;

		nIdx = pDM->GetRowCount() - 1;
		CString strItemData;
		for (INT nCol = 0; nCol < pDM->RefList.GetCount(); nCol++)
		{
			strItemData.Empty();

			// ��ũ �����޽����� �ִ´�.
			if (strAlias[nCol] == m_strDMErrChkAlias)
			{
				if (!strMarcCheckResult.IsEmpty())
				{
					strItemData = strMarcCheckResult;
				}
			}
			// ��ũ��Ʈ���� �ִ´�.
			else if (strAlias[nCol] == m_strDMMarcstreamAlias)
			{
				ids = m_pMarcMgr->Encoding(pMarc, strItemData);
				if (ids < 0)
					continue;
			}
			// å���� �ִ´�.
			else if (strAlias[nCol] == m_strDMBookCountAlias)
			{
				// å���� ã�´�. --> 049 $l�� ���� ã���� �ȴ�.
				CArray<CString, CString&> arrItem;
				m_pMarcMgr->GetItem(pMarc, _T("049$l"), strItemData, &arrItem);
				INT nBookCount = arrItem.GetSize();
				if (nBookCount > 0)
					strItemData.Format(_T("%d"), nBookCount);
				else
					strItemData = _T("1");
			}
			// �ʵ忡 �ش��ϴ� �����͸� �ִ´�. (�ʿ� ���� ����̴�..)
			else
			{
				continue;
				/*
				CString strMarcAlias;
				strMarcAlias = strAlias[nCol];
				strMarcAlias.Replace(_T("BS_"), _T(""));
				strMarcAlias.Replace(_T("BB_"), _T(""));
				strMarcAlias.Replace(_T("BP_"), _T(""));

				ids = m_pMarcMgr->GetItem(pMarc, strMarcAlias, strItemData);
				if (ids < 0) continue;
				*/
			}

			pDM->SetCellData(strAlias[nCol], strItemData, nIdx );
		}

		// �����ʷ��� �ִ��� Ȯ���Ѵ�.
		// �����ʷ� ���� ǥ�ÿ� ����, �ʷ� �׸��� �����´�.
		arrItem.RemoveAll();
		m_pMarcMgr->GetItem(pMarc, _T("020$a"), strItemData, &arrItem);
		
		bTocFind = FALSE;
		bAbsFind = FALSE;
		strToc = _T("");
		strAbs = _T("");
		for ( int i=0 ; i<arrItem.GetSize() ; i++ )
		{			
			strISBN = arrItem.GetAt(i);
			if ( strISBN != _T("") && bTocFind == FALSE )
			{
				bTocFind = GetTocAbsFile(strISBN, _TOC_, strToc);
			}			
			if ( strISBN != _T("") && bAbsFind == FALSE )
			{
				bAbsFind = GetTocAbsFile(strISBN, _ABS_, strAbs);
			}

			// �Ѵ� ã������ ������
			if ( bTocFind && bAbsFind ) break;
		}

		// ���� ���� �� ���� �ֱ�
		// N�ΰŴ� �����ָ� �� �򰥸�. �ִ°Ÿ� �����ִ°� ���� ����.
		if ( bTocFind )
		{
			pDM->SetCellData(_T("IB_��������"), _T("Y"), nIdx );
			pDM->SetCellData(_T("����"), strToc, nIdx );			
		}
		
		// �ʷ� ���� �� �ʷ� �ֱ�
		if ( bAbsFind )
		{
			pDM->SetCellData(_T("IB_�ʷ�����"), _T("Y"), nIdx );
			pDM->SetCellData(_T("�ʷ�"), strAbs, nIdx );
		}		
	}

	EFS_END
}

BOOL CMarcImpExpMgr::GetTocAbsFile(CString strISBN, INT nType, CString &strData)
{
	CString strFilePath;
	INT		nPaserIndex;

	// 0. ���� ��� ���� 		
	nPaserIndex = m_strFilePath.GetLength();
	for ( int i=0 ; i<nPaserIndex  ; i++ )
	{
		if ( m_strFilePath.Mid(nPaserIndex-i, 1) == _T("\\") )
		{
			strFilePath = m_strFilePath.Left(nPaserIndex-i);
			break;
		}
	}
	if ( nType == _TOC_ )
	{
		strFilePath += _T("\\") + strISBN + _T(".toc");
	}
	else
	{
		strFilePath += _T("\\") + strISBN + _T(".abs");
	}		

	// 1. �������� Ȯ��	
	CStdioFile file;
	if (!file.Open(strFilePath, CFile::modeRead | CFile::typeBinary))
	{
		// ���� ����
		strData = _T("");
		return FALSE;
	}
	file.Close();

	// 2. �����ڵ�� ��ȯ
	FileCodeChange(strFilePath);

	// 3. ���� �б�	
	if (!file.Open(strFilePath, CFile::modeRead | CFile::typeBinary))
	{
		m_strLastError = _T("File Open Failed!!");
		return -1;
	}
	
	TCHAR cUniTemp;	
	file.Read( &cUniTemp, sizeof(TCHAR) );

	CString strLine;

	while ( file.ReadString(strLine) )
	{	
		strData += strLine;		
	}

	strData.Replace(_T("\r"), _T("\r\n"));
	file.Close();

	// 4. Syntax �˻�

	// 5. ����

	return TRUE;
}

INT CMarcImpExpMgr::BringPubFreqFromCodeMgr()
{
	if(!m_pCodeMgr) return -1;
	
	CString strClassName;
	strClassName = _T("������ڵ�");

  	m_nPubFreqCodeCnt = m_pCodeMgr->GetCodeCount(strClassName);

	INT nCode = 64;
	INT nDefaultValue = 0;
	m_pCodeMgr->GetCode(strClassName, m_strPubFreqCode, m_strPubFreqDesc, nCode, nDefaultValue);	

	return 0;
}

INT CMarcImpExpMgr::GetPubFreqCode(CString strData,CString &strOutput)
{
	//����� ������ �ڵ�� �ٲپ� �ִ� �Լ�
	//�ڵ�� �Էµ� �ڷ�� �´� �ڵ����� Ȯ���ϰ�, 
	//���̿��� �ڷ�� ����� ��ġ�ϴ��� Ȯ���� �ڵ�� �ٲپ� �ش�  	

	CString strTmp;
	strTmp = strData;
	strTmp.TrimLeft();
	strTmp.TrimRight();
	if(strTmp.IsEmpty()) return -1;
	
	for(INT i=0;i<m_nPubFreqCodeCnt;i++)
	{

		if(strTmp == m_strPubFreqCode[i])
		{
			strOutput = m_strPubFreqCode[i];
			return 0;
		}
	}

	
	for(i=0;i<m_nPubFreqCodeCnt;i++)
	{

		if(strTmp == m_strPubFreqDesc[i])
		{
			strOutput = m_strPubFreqCode[i];
			return 0;
		}
	}

	return -1;
}

INT CMarcImpExpMgr::ImportPartitionTxtFile()
{
	EFS_BEGIN

	CStdioFile file;
	if (!file.Open(m_strFilePath, CFile::modeRead | CFile::typeBinary))
	{
		m_strLastError = _T("File Open Failed!!");
		return -1;
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	CString strLine;
	CMarc marc;
	
	// ù��° ���ο� �ִ� Tag Code�� ������ ������..
	CArray<CString, CString&> arrTagCode;
	arrTagCode.RemoveAll();
	

	file.ReadString(strLine);
	//ù��° ���ο��� Marc TagCode �� �־�� �Ѵ�.... ������.. ���� ������... -_-;;;
	//EX : 245$d| 245$d| 245$d| 245$d| 245$d| 245$d| 
	strLine.TrimLeft();
	INT nFindBar = strLine.Find(_T("|"));
	INT nTag;
	nTag = strLine.Find( _T("|"));
	while ( nTag != -1 )
	{
		// ù��° ���ο��� TagCode ���� ������ �ͼ� arrTagCode Array�� ���� ��Ű�� ����
		
		CString strTag = strLine.Left(nTag +1 ) ;
		strLine = strLine.Mid( strTag.GetLength() );
		strTag.TrimRight(_T("|"));
		strTag.TrimLeft();
		strTag.TrimRight();

		arrTagCode.Add(strTag);
		nTag = strLine.Find( _T("|"));
		if ( nTag == -1 ) 
			arrTagCode.Add( strLine );
	}

	// �ι�° ���ο������� �����͸� ������ͼ� Marc ����
	file.ReadString(strLine);
	do 
	{
		marc.Init();
		strLine.TrimLeft();
		strLine.TrimRight();
		if (strLine.IsEmpty()) break;

		// _T("|") ��ȣ�� ���е� Marc �����͸� STREAM MARC �� �����Ѵ�.
		nTag = strLine.Find( _T("|"));
		INT nArrIdx = 0;
		while ( nTag != -1 ) 
		{
			CString strTag = strLine.Left(nTag +1 ) ;
			strLine = strLine.Mid( strTag.GetLength() );
			strTag.TrimRight(_T("|"));
			strTag.TrimLeft();
			strTag.TrimRight();
			
			CString strTagCode = arrTagCode.GetAt(nArrIdx);
			m_pMarcMgr->SetItem(&marc, strTagCode , strTag);
			nArrIdx++;
			nTag = strLine.Find( _T("|"));
			if ( nTag == -1 ) 
			{
				strTagCode = arrTagCode.GetAt(nArrIdx);
				m_pMarcMgr->SetItem(&marc, strTagCode , strLine);
			}
		}

		MakeDM(&marc);
		
	} while (file.ReadString(strLine));

	file.Close();

	return 0;

	EFS_END
	return -1;



}

INT CMarcImpExpMgr::FileCodeChange(LPCTSTR lpFilePath)
{
	/// ������ ������ UTF-16���Ϸ� �ٲ۴�.
	INT		ids, nByte;
	CHAR	*sMultyByte = NULL;
	TCHAR	*sWideByte = NULL;
	CharConvert Convert;
	
	ids = Convert.IsUnicodeFile((TCHAR*)lpFilePath, &nByte);
	if ( ids < 0 ) return -10;
	if ( ISUNICODE != ids ) {
		ids = Convert.ReadAllOfFile((TCHAR*)lpFilePath, &sMultyByte, &nByte);
		if ( ids ) return -20;

		ids = Convert.IsUTF8File((TCHAR*)lpFilePath, &nByte);
		if ( ids < 0 ) {
			free(sMultyByte);
			return -30;
		}
		if ( ISUTF8 != ids ) {
			ids = Convert.MultiByteToUnicode(sMultyByte, &sWideByte, &nByte);
			if ( ids ) {
				free(sMultyByte);
				return -40;
			}
		} else {
			ids = Convert.UTF8ToUnicode(sMultyByte, &sWideByte, &nByte);
			if ( ids ) {
				free(sMultyByte);
				return -50;
			}
		}
		ids = Convert.WriteToFile((TCHAR*)lpFilePath, sWideByte, nByte);
		free(sMultyByte);
		free(sWideByte);
		if ( ids ) return -60;
	}
	return 0;
}