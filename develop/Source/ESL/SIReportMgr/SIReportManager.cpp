// SIReportManager.cpp: implementation of the CSIReportManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExportToExcel.h"
// 16/03/29 ������ : �Ѽ� �����ϱ�
//*/ ADD ----------------------------------------------------------------------------------
#include "ExportToHCell.h"
//*/ END ----------------------------------------------------------------------------------
#include "SIReportManager.h"
#include "SIReportPreviewDlg.h"
#include "..\\SIReportData\\SIReportConstant.h"
#include "..\\SIReportData\\SIReportObject_Table_Cell.h"
#include "..\\SIReportData\\SIReportObject_Barcode.h"
// 16/08/04 ������ : ���δ�������� ������½� �����Ͱ� �߷����̴� ���� ����
//*/ ADD ----------------------------------------------------------------------------------
#include "..\\..\\..\\include\\ESL\\CharConvert.h"
//*/ END ----------------------------------------------------------------------------------
#include "StdioFileEx.h"
#include "..\ESL\ESLDlg_ExportType.h"

#include <direct.h>
#include "efs.h"
#include "hancelmanager.h"

//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰�
#include "ESLDlg_UserExportReason.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
bool bGfxUserAbort;

BOOL CALLBACK EXPORT _GfxAbortProc( HDC, INT )
{
	MSG msg;
	while (!bGfxUserAbort && ::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
	{
		if (!AfxGetThread()->PumpMessage()) return FALSE;
	}
	return !bGfxUserAbort;
}

CSIReportManager::CSIReportManager(CWnd *pOwnerWnd)
{
	m_pWorkForm = NULL;
	m_pPageWorkForm = NULL;
	InitDMArray();
	m_DMCursor = -1;		// ���� ����Ÿ ������

	// XML IO Mgr
	m_pXMLIOMgr = new CSIReportXMLIOMgr(MASTER_IS_MANAGER);

	m_pOwnerWnd = pOwnerWnd;
	m_fpEventHandler = NULL;

	// (START) - ���Ͽ��� �޽��� ���� �ʱ�ȭ �κ�
	m_strNoMoreStatement = _T("�� �� �� ��");
	m_nNoMoreStatement = 10; // 10mm

	m_fNoMoreStatement.lfCharSet = DEFAULT_CHARSET;
	ZeroMemory(&m_fNoMoreStatement,sizeof(LOGFONT));
	_tcscpy(m_fNoMoreStatement.lfFaceName,_T("����ü"));
	m_fNoMoreStatement.lfHeight = 10;
	// ( END ) - ���Ͽ��� �޽��� ���� �ʱ�ȭ �κ�

	m_strPartSum = _T("��  ��");			
	m_nColPartSum = 0;
	m_strTotalSum = _T("��  ��");			
	m_nColTotalSum = 0;

	XML_MODE = 0;			// NOT XML MODE�� ����
	m_strOriginalWorkFormFileName = _T("");
	m_strPageHeader = _T("page");

	_tmkdir(_T("rpt_tmp"));
	_tmkdir(MANAGER_TEMP_DIR);

	m_nDefaultPaperType = PAPER_A4_PORTRAIT;
	m_nDM = 0;
	m_pDefaultDM = NULL;
	m_bNoDataSkip = FALSE;
	m_nStartPageNum = 1;

	m_nResolution = 1440;

	// ����ڰ� ���� ������ ���� ����(�������� ����);
	m_nUserPaperSize = -1;
	m_nOrientation = -1;

	// 16/03/29 ������ : �Ѽ� �����ϱ�
//*/ ADD -------------------------------------------------------------------------------------
	m_bIsExcel = TRUE;
	CLSID clsExcelApp;
	if (!FAILED(CLSIDFromProgID(_T("HCell.Application"), &clsExcelApp))) 
	{
		m_bIsExcel = FALSE;	// 16/04/05 ������ : ��ȣ���� ������ ���� �ӽ��ּ�ó�� -> // 16/04/19 ������ : �ּ�Ǯ��
	}	
//*/ END -------------------------------------------------------------------------------------


}

CSIReportManager::~CSIReportManager()
{
	if (m_pWorkForm) {
		delete m_pWorkForm;
		m_pWorkForm = NULL;
	}
	if (m_pXMLIOMgr) {
		delete m_pXMLIOMgr;
		m_pXMLIOMgr = NULL;
	}
	FreePageWorkForm();
}

INT CSIReportManager::FreePageWorkForm(VOID)
{
EFS_BEGIN

	if (m_pPageWorkForm) {
		delete m_pPageWorkForm;
		m_pPageWorkForm = NULL;
	}
	return 0;

EFS_END
return -1;

}

INT CSIReportManager::PrintDocument(INT nCopy, INT nFromPage, INT nToPage, CWnd *pMsg, CProgressCtrl *pBar)
{
EFS_BEGIN

	// ����� �����͸� �����Ѵ�
	CPrintDialog pd(FALSE, PD_NONETWORKBUTTON|PD_HIDEPRINTTOFILE|PD_NOSELECTION|PD_NOSELECTION);
	if (pd.DoModal() != IDOK) return -1;
	Sleep(10);
	m_pd.hDevMode = pd.m_pd.hDevMode;
	m_pd.hDevNames = pd.m_pd.hDevNames;
	CString devicename = pd.GetDeviceName();
	bool m_bPdInit = true;
	bool bError;

	// ��¸�带 ������� ���� �ٲ۴�.
	LPDEVMODE pDevMode = pd.GetDevMode();
	if (m_pWorkForm->paper == PAPER_A4_LANDSCAPE) pDevMode->dmOrientation = DMORIENT_LANDSCAPE;
	if (m_pWorkForm->paper == PAPER_A4_PORTRAIT) pDevMode->dmOrientation = DMORIENT_PORTRAIT;
	if (m_pWorkForm->paper == PAPER_B4_LANDSCAPE) pDevMode->dmOrientation = DMORIENT_LANDSCAPE;
	if (m_pWorkForm->paper == PAPER_B4_PORTRAIT) pDevMode->dmOrientation = DMORIENT_PORTRAIT;
	if (m_pWorkForm->paper == PAPER_FANFOLD_US_LANDSCAPE) pDevMode->dmOrientation = DMORIENT_LANDSCAPE;
	if (m_pWorkForm->paper == PAPER_FANFOLD_US_PORTRAIT) pDevMode->dmOrientation = DMORIENT_PORTRAIT;

	if (m_nUserPaperSize != m_nUserPaperSize) {
		if (m_pWorkForm->paper == PAPER_B4_LANDSCAPE || m_pWorkForm->paper == PAPER_B4_PORTRAIT) pDevMode->dmPaperSize = DMPAPER_B4;
		if (m_pWorkForm->paper == PAPER_A4_LANDSCAPE || m_pWorkForm->paper == PAPER_A4_PORTRAIT) pDevMode->dmPaperSize = DMPAPER_A4;
		if (m_pWorkForm->paper == PAPER_FANFOLD_US_LANDSCAPE || m_pWorkForm->paper == PAPER_FANFOLD_US_PORTRAIT) pDevMode->dmPaperSize = DMPAPER_FANFOLD_US;
	} else {
		//pDevMode->dmPaperSize = 0;
		//pDevMode->dmPaperWidth = (short)m_sUserPaperSize.cx;
		//pDevMode->dmPaperLength = (short)m_sUserPaperSize.cy;
	}
	
	/*
	if (m_nUserPaperSize > 0) {
		pDevMode->dmPaperSize = m_nUserPaperSize;
	}
	*/
	if (m_nOrientation > 0) {
		pDevMode->dmOrientation = m_nOrientation;
	}

	pd.m_pd.hDevMode = pDevMode;
	Sleep(10);
	
	// PRINTER DEVICE�� ���´�.
	LPDEVNAMES lpDevNames = (m_pd.hDevNames != NULL) ? 
					(LPDEVNAMES)::GlobalLock(m_pd.hDevNames) : NULL;
	if (lpDevNames == NULL) return -1;
	LPDEVMODE  lpDevMode = (m_pd.hDevMode != NULL) ?
					(LPDEVMODE)::GlobalLock(m_pd.hDevMode) : NULL;
	if (lpDevMode == NULL) return -1;
	Sleep(10);

	HDC hDC = ::CreateDC((LPCTSTR)lpDevNames + lpDevNames->wDriverOffset,
					  (LPCTSTR)lpDevNames + lpDevNames->wDeviceOffset,
					  (LPCTSTR)lpDevNames + lpDevNames->wOutputOffset,
					  lpDevMode);	
	if (hDC == NULL) return -1;

	if (m_pd.hDevNames != NULL) ::GlobalUnlock(m_pd.hDevNames);
	if (m_pd.hDevMode != NULL) ::GlobalUnlock(m_pd.hDevMode);


	// �׸��� �׸� DC�� �����Ѵ�
    CDC * m_pDC = new CDC;
	Sleep(10);

	m_pDC->Attach(hDC);
	INT nSaveDC = m_pDC->SaveDC();
	Sleep(10);

	// ���� DEVICE�� ������ �����´�
	INT width = m_pDC->GetDeviceCaps(HORZSIZE);
	INT height = m_pDC->GetDeviceCaps(VERTSIZE);
	m_printHorzR = m_pDC->GetDeviceCaps(HORZRES);
	m_printVertR = m_pDC->GetDeviceCaps(VERTRES);
	INT printLogX = m_pDC->GetDeviceCaps(LOGPIXELSX);
	INT printLogY = m_pDC->GetDeviceCaps(LOGPIXELSY);
	INT offsetx = m_pDC->GetDeviceCaps(PHYSICALOFFSETX);
	INT offsety = m_pDC->GetDeviceCaps(PHYSICALOFFSETY);
	Sleep(10);
	
	// PAPER SIZE�� �����Ѵ�.
	m_drawSize.cx = (INT)(m_nResolution * (double)(width / 25.4 ));//tempSize.cx ; 11225
	m_drawSize.cy = (INT)(m_nResolution * (double)(height / 25.4));//tempSize.cy ; 16157
	Sleep(10);

	m_pDC->SetBkMode(TRANSPARENT); // �����ϰ� �Ѵ�.

	DOCINFO di = {sizeof(DOCINFO), _T("ECO SI Report Manager ���"), NULL}; // ��½� �̸� ����
	m_pDC->StartDoc(&di);

	// Work Form�� ����س���. ���������� ��ũ���� ���Ե� ��Ʈ�� ������, ����Ÿ�� �翡 ���� �����ȴ�.
	for (INT i = 0; i < nCopy; i++) {
		PRINT_DrawWorkForm(m_pDC, nFromPage, nToPage, pMsg, pBar);
	}

	bError = FALSE;
	if (!bError) 
		m_pDC->EndDoc();	// print�۾��� ������.
	else 
		m_pDC->AbortDoc();	// print�۾��� ����Ѵ�.
	Sleep(10);
	
	m_pDC->RestoreDC(nSaveDC);	// �ٽ� DC�� RETURN�Ѵ�.
	Sleep(10);

	// print CDC��ü �Ҹ�
	m_pDC->DeleteDC();
	m_pDC->Detach();
    delete m_pDC;
  
    return 0;

EFS_END
return -1;

}

INT CSIReportManager::DrawTextWithFont(INT mode, CDC *dc, CString str, CRect rect, LOGFONT logfont, UINT nFormat /*= DT_TOP|DT_CENTER |DT_WORDBREAK*/)
{
EFS_BEGIN

	if (dc == NULL) return -1;
	
	CFont *pOldFont = NULL;
	INT height = -1;

	INT fontsize = logfont.lfHeight;
	CFont font;

	if (mode == MANAGER_MODE_PRINT) {		// �μ����϶� ������ ���
		logfont.lfHeight = -MulDiv(fontsize, m_nResolution, 72);									
	} else {				// Preview��� �϶��� ������ ���
		logfont.lfHeight = -MulDiv(fontsize, GetDeviceCaps(dc->m_hDC, LOGPIXELSY), 72);
	}
	font.CreateFontIndirect(&logfont);

	pOldFont = dc->SelectObject(&font);

	height = dc->DrawText(str, rect, nFormat);
	dc->SelectObject(pOldFont);
	return height;

EFS_END
return -1;

}

INT CSIReportManager::DrawTextWithFont2(INT mode, CDC *dc, CString str, INT char_len, INT char_cy, CRect rect, LOGFONT logfont, UINT nFormat /*= DT_TOP|DT_CENTER |DT_WORDBREAK*/)
{
EFS_BEGIN

	if (dc == NULL) return -1;
	
	CFont *pOldFont = NULL;
	INT height = -1;

	INT fontsize, nPixel_cy;
	CFont font;
	fontsize = logfont.lfHeight;
	if (mode == MANAGER_MODE_PRINT) {		// �μ���
		logfont.lfHeight = -MulDiv(fontsize, m_nResolution, 72);
	} else {								// Preview ���
		logfont.lfHeight = -MulDiv(fontsize, GetDeviceCaps(dc->m_hDC, LOGPIXELSY), 72);
	}
	nPixel_cy = (logfont.lfHeight < 0) ? logfont.lfHeight * -1 : logfont.lfHeight;
	font.CreateFontIndirect(&logfont);

	pOldFont = dc->SelectObject(&font);
	INT nStartY = rect.top;

	CString token;
	INT pos = 0;

	// 16/08/04 ������ : ���δ�������� ������½� �����Ͱ� �߷����̴� ���� ���� - ������ ������ �ҽ�����(�׽�ƮX)
/*// BEFORE ------------------------------------------------------------------------
	while(true) {
		rect.top = nStartY;
		rect.bottom = nStartY + nPixel_cy;
		if (str.GetLength() == 0) break;
		pos = char_len;
		if (str.GetLength() > pos) {
			if (!IsCanCut(str, pos-1)) {
				pos++;
			}
		} else {
			pos = str.GetLength();
		}
		token = str.Left(pos);
		height = dc->DrawText(token, rect, nFormat);
		str = str.Mid(pos);
		str.TrimLeft();
		str.TrimRight();

		nStartY += nPixel_cy;
	}
*/// AFTER -------------------------------------------------------------------------
		CHAR szCutMsg[2048];
		CString sendMsg;
		CHAR *szMsg = NULL;
		TCHAR *szUnicodeMessage = NULL;
		INT	nLen = 0;
		INT	nUnicodeLen = 0;
		CharConvert cv;
	
		while(true) {
			rect.top = nStartY;
			rect.bottom = nStartY + nPixel_cy;

			cv.UnicodeToMultiByte(str.GetBuffer(0), &szMsg, &nLen );	
			if(NULL == szMsg || 0 == nLen) break;
			
			pos = char_len;
			if (nLen > pos) {
				if (!IsCanCut(szMsg, pos-1)) {
					pos++;
				}
			} else {
				pos = nLen;
			}

			strncpy(szCutMsg , szMsg, pos);
			szCutMsg[pos] = '\0';
			
			if(0 < nLen)  delete [] szMsg;

			cv.MultiByteToUnicode(szCutMsg, &szUnicodeMessage, &nUnicodeLen );
			sendMsg.Format(_T("%s"), szUnicodeMessage );
			
			if(0 < nUnicodeLen) delete [] szUnicodeMessage;

			token = str.Left(pos);
			height = dc->DrawText(token, rect, nFormat);

			if( str.GetLength() < sendMsg.GetLength() ) break;
			str = str.Mid( sendMsg.GetLength() );

			nStartY += nPixel_cy;
		}
//*/ END ---------------------------------------------------------------------------
	dc->SelectObject(pOldFont);
	return height;

EFS_END
return -1;

}

INT CSIReportManager::DrawTextWithFont3(INT mode, CDC *dc, CString str, CRect rect, LOGFONT logfont, UINT nFormat /*= DT_TOP|DT_CENTER |DT_WORDBREAK*/)
{
EFS_BEGIN
	if (str.Find(_T("%GETTEMPVARIABLE("), 0) != -1) {
		str = GetKeywordString(str);
	}
	if (dc == NULL) return -1;
	
	CFont *pOldFont = NULL;
	INT height = -1;

	INT fontsize, nPixel_cy;
	CFont font;
	fontsize = logfont.lfHeight;
	if (mode == MANAGER_MODE_PRINT) {		// �μ���
		logfont.lfHeight = -MulDiv(fontsize, m_nResolution, 72);
	} else {								// Preview ���
		logfont.lfHeight = -MulDiv(fontsize, GetDeviceCaps(dc->m_hDC, LOGPIXELSY), 72);
	}
	nPixel_cy = (logfont.lfHeight < 0) ? logfont.lfHeight * -1 : logfont.lfHeight;
	font.CreateFontIndirect(&logfont);

	pOldFont = dc->SelectObject(&font);
	INT nStartY = rect.top;

	CString token;
	INT pos = 0;
	while(true) {
		rect.top = nStartY;
		rect.bottom = nStartY + nPixel_cy;
		if (str.GetLength() == 0) break;
		pos = str.Find('\n', 0);
		if (pos == -1) pos = str.GetLength();
		token = str.Left(pos);
		//token.TrimLeft();				token.TrimRight();
		token.Replace(_T("\n"), _T(""));
		token.Replace(_T("\r"), _T(""));
		height = dc->DrawText(token, rect, nFormat);
		if (str.GetLength() <= pos) break;
		str = str.Mid(pos+1);
		//str.TrimLeft();		str.TrimRight();

		nStartY += nPixel_cy;
	}
	dc->SelectObject(pOldFont);
	return height;

EFS_END
return -1;

}

INT CSIReportManager::PRINT_DrawWorkForm(CDC *pDC, INT nStartPage, INT nFinishPage, CWnd *pMsg, CProgressCtrl *pBar)
{
EFS_BEGIN

	m_EventInfo.m_pSIReportManager = (CObject*)this;
	m_EventInfo.mode = MANAGER_MODE_PRINT;
	m_DMCursor = -1;		// ���� ����Ÿ ������

	INT nPrintPages = nFinishPage - nStartPage + 1;
	INT nPrintedCount = 0;
	CString msg;

	if (!pDC) return -1;

	CString strTmp;
	for (INT i = nStartPage; i <= nFinishPage; i++) {
		nPrintedCount++;
		pDC->StartPage(); // ������ ��� ����
		pDC->SetMapMode(MM_ANISOTROPIC);
		//pDC->SetWindowExt(m_nResolution, m_nResolution);
		pDC->SetWindowExt(m_nResolution, m_nResolution);
		pDC->SetViewportExt(pDC->GetDeviceCaps(LOGPIXELSX), pDC->GetDeviceCaps(LOGPIXELSY));
		INT x = pDC->GetDeviceCaps(LOGPIXELSX);
		INT y = pDC->GetDeviceCaps(LOGPIXELSY);

		// EventHandleró��
		m_EventInfo.pDC = pDC;
		m_EventInfo.m_nPage = i;
		if (m_pOwnerWnd && m_fpEventHandler) {
			( m_pOwnerWnd->*m_fpEventHandler )( &m_EventInfo );
		}

		// (START) - Page XML�� Load �Ͽ� Print �Ѵ�.
		ShowXMLFileOnePage(MANAGER_MODE_PRINT, pDC, i);
		// ( END ) - Page XML�� Load �Ͽ� Print �Ѵ�.

		//ids = DrawWorkSheet(MANAGER_MODE_PRINT, pDC, i);
		if (pDC->EndPage() < 0 || !_GfxAbortProc(pDC->GetSafeHdc(), 0)) {
			//bError = TRUE;
			break;
		}
		msg.Format(_T("%d/%d(%d������ �μ���)"), nPrintedCount, nPrintPages, i);
		if (pMsg) pMsg->SetWindowText(msg);
		if (pBar) pBar->SetPos((INT)(nPrintedCount * 100 / nPrintPages));
		Sleep(50);
	}

	strTmp.Format(_T("rpt_tmp\\%s%04d.xml"), m_strPageHeader, m_nCurrentPreviewPage);
	CFile fd;
	if (fd.Open(strTmp, CFile::modeRead | CFile::typeBinary)) {
		fd.Close();
		LoadWorkForm(strTmp);
	}
	return 0;


EFS_END
return -1;

}

INT CSIReportManager::InitWorkFormFile(CString strRpt, CString strRptAlias, CString rpt_pk, BOOL bExcelReport )
{
EFS_BEGIN

	m_strRPTALIAS = strRptAlias;
	m_strRPTPK = rpt_pk;

	m_strOriginalWorkFormFileName = strRpt;
	if ( bExcelReport == FALSE ) LoadWorkForm(strRpt);
	return 0;

EFS_END
return -1;

}

INT CSIReportManager::LoadWorkForm(CString strRpt)
{
EFS_BEGIN

	FreeWorkForm();
	m_pWorkForm = new CSIReportWorkForm(MASTER_IS_MANAGER);
	if (!m_pWorkForm) return -1;

	INT ids = m_pWorkForm->LoadWorkForm(strRpt);
	INT t = m_pWorkForm->IncludeList.GetCount();
	INT tt = m_pWorkForm->WorkSheetList.GetCount();

	return 0;

EFS_END
return -1;

}

INT CSIReportManager::FreeWorkForm(VOID)
{
EFS_BEGIN

	if (m_pWorkForm) {
		delete m_pWorkForm;
		m_pWorkForm = NULL;
	}
	return 0;

EFS_END
return -1;

}

CSIReportObject_Table * CSIReportManager::FindTable(CSIReportWorkSheet *pSheet, CString name)
{
EFS_BEGIN

	INT count = pSheet->ObjList.GetCount();
	if (count == 0) return NULL;

	CSIReportObject * pObject;
	POSITION pos;
	INT i;
	pos = pSheet->ObjList.GetHeadPosition();
	for(i = 0; i < count; i++) {
		pObject = NULL;
		pObject = (CSIReportObject*)pSheet->ObjList.GetNext(pos);
		if (!pObject) continue;
		if (pObject->GetObjType() == OBJECT_TABLE) {
			CSIReportObject_Table *pTable = (CSIReportObject_Table *)pObject;
			if (!pTable) continue;
			if (pTable->m_strName == name) return pTable;
		}
	}
	return NULL;

EFS_END
return NULL;

}

CSIReportObject * CSIReportManager::FindObject(CSIReportWorkSheet *pSheet, CString strName)
{
EFS_BEGIN

	INT count = pSheet->ObjList.GetCount();
	if (count == 0) return NULL;

	CSIReportObject * pObject;
	POSITION pos;
	INT i;
	pos = pSheet->ObjList.GetHeadPosition();
	for(i = 0; i < count; i++) {
		pObject = NULL;
		pObject = (CSIReportObject*)pSheet->ObjList.GetNext(pos);
		if (!pObject) continue;
		if (pObject->m_strName == strName) return pObject;
	}
	return NULL;

EFS_END
return NULL;
}

CSIReportWorkSheet * CSIReportManager::GetSheet(INT nIdx)
{
EFS_BEGIN
	if (m_pWorkForm == NULL) return NULL;
	INT count = m_pWorkForm->IncludeList.GetCount();
	if (count == 0) return NULL;
	POSITION pos = m_pWorkForm->IncludeList.FindIndex(nIdx);
	if (pos == NULL) return NULL;
	CSIReportWorkSheet * pSheet = (CSIReportWorkSheet*)m_pWorkForm->IncludeList.GetAt(pos);
	return pSheet;
EFS_END
return NULL;
}

BOOL CSIReportManager::HaveChild(CSIReportObject_Table *pTable)
{
EFS_BEGIN

	if (!pTable) return false;

	INT nRow = pTable->row_count;
	INT nCol = pTable->col_count;
	INT row, col;

	CSIReportObject_Table_Cell *pCell;
	for (row = 0; row < nRow; row++) {
		for (col = 0; col < nCol; col++) {
			pCell = pTable->GetCell(row, col);
			if (!pCell) continue;
			if (pCell->child_count > 0) return true;
		}
	}	
	return false;

EFS_END
return FALSE;

}

BOOL CSIReportManager::IsChild(CSIReportObject_Table *pTable)
{
EFS_BEGIN

	if (!pTable) return false;
	if (pTable->m_strParentName.GetLength() > 0) return true;
	if (pTable->repeat == TRUE) return true;

	return false;
EFS_END
return FALSE;

}

double CSIReportManager::GetHeight(CSIReportObject_Table_Cell *pCell, CString str, INT &nLines)
{
	nLines = GetLines(pCell->char_max, pCell->char_len, str);
	if (nLines == 1) {
		return (pCell->GetRect().bottom - pCell->GetRect().top);
	} else {
		INT nCellHeight = pCell->GetRect().bottom - pCell->GetRect().top;
		INT nLinesHeight = pCell->char_cy * nLines;		
		// START --------------------------------------------------------------------------------------
		// 2005-12-01 UPD BY PDW(Jr)
		// ��¹��� ������ ���� ����� �����Ͽ�
		// ������ ���� ������ ������ ������ ����
		// @������
		/*
		INT nHeight = (pCell->GetRect().bottom - pCell->GetRect().top) + (pCell->TCHAR_cy * (nLines-1));
		*/
		// @������
		INT nHeight = nLinesHeight;
		// END ----------------------------------------------------------------------------------------

		if (nCellHeight > nLinesHeight) return nCellHeight;
		//else if (nCellHeight < nLinesHeight) return nCellHeight;
		else return nHeight;
	}
}

/*
INT CSIReportManager::GetLines(INT char_max, INT char_len, CString str)
{
EFS_BEGIN

	INT length = str.GetLength();
	if (length == 0) return 1;			// ������ ������ ���ٷ� �����Ѵ�.

	INT nLines = 0;
	INT pos;
	// \r\n�� ��� ���� ���� Enter�� Line���� ����Ѵ�.
	if (str.Find('\n', 0) >= 0) {
		return GetLinesByEnter(str);
	} else {
		while(true) {
			if (str.GetLength() == 0) break;
			pos = char_len;
			if (str.GetLength() > pos) {
				if (!IsCanCut(str, pos-1)) {
					pos++;
				}
			} else {
				pos = str.GetLength();
			}
			//token = str.Left(pos);
			//height = dc->DrawText(token, rect, nFormat);
			nLines++;
			str = str.Mid(pos);
			str.TrimLeft();
			str.TrimRight();
		}
		
		//INT nQuotient = length / char_len;
		//INT nRemain = length % char_len;
		//if (nRemain == 0) {
		//	return nQuotient;
		//} else {
		//	return nQuotient+1;
		//}
			
		return nLines;
	}

EFS_END
return -1;

}
*/
INT CSIReportManager::GetLines(INT char_max, INT char_len, CString str)
{
EFS_BEGIN

	INT length = str.GetLength();
	if (length == 0) return 1;			// ������ ������ ���ٷ� �����Ѵ�.


	//########################################################################
	//2005-01-29 leehakjoong
	if(char_max < length )
	{
		str = str.Mid (0,char_max) ;
		length = char_max ;
	}
	//########################################################################


	INT nLines = 0;
	INT pos;
	// \r\n�� ��� ���� ���� Enter�� Line���� ����Ѵ�.
	//2004-10-13 
	bool bEnd = FALSE ;
	if(str.Find ('\n',0) == length ) 
	{
		bEnd = TRUE ;
	}

	// 2005-12-02 UPD BY PDW(Jr) 
	// ����Ű�� �����Ѵٸ�.... ���� 
	/* if (str.Find('\n', 0) >= 0 && bEnd == TRUE ) { */
	if (str.Find('\n', 0) >= 0 || bEnd == TRUE ) {
		return GetLinesByEnter(str);
	} else {
		// 16/08/04 ������ : ���δ�������� ������½� �����Ͱ� �߷����̴� ���� ����
/*// BEFORE ------------------------------------------------------------------------
		while(true) {
			if (str.GetLength() == 0) break;
			pos = char_len;
			if (str.GetLength() > pos) {
				if (!IsCanCut(str, pos-1)) {
					pos++;
				}
			} else {
				pos = str.GetLength();
			}
			//token = str.Left(pos);
			//height = dc->DrawText(token, rect, nFormat);
			nLines++;
			str = str.Mid(pos);
			str.TrimLeft();
			str.TrimRight();
		}
*/// AFTER -------------------------------------------------------------------------
		CHAR szCutMsg[2048];
		CString sendMsg;
		CHAR *szMsg = NULL;
		TCHAR *szUnicodeMessage = NULL;
		INT	nLen = 0;
		INT	nUnicodeLen = 0;
		CharConvert cv;
		
		while(true) {
			cv.UnicodeToMultiByte(str.GetBuffer(0), &szMsg, &nLen );	
			if(NULL == szMsg || 0 == nLen) break;
			
			if( !IsCanCut( szMsg , char_len-1 ) )
				pos = char_len+1;
			else
				pos = char_len;

			strncpy(szCutMsg , szMsg, pos);
			szCutMsg[pos] = '\0';
			
			if(0 < nLen)  delete [] szMsg;

			cv.MultiByteToUnicode(szCutMsg, &szUnicodeMessage, &nUnicodeLen );
			sendMsg.Format(_T("%s"), szUnicodeMessage );
			
			if(0 < nUnicodeLen) delete [] szUnicodeMessage;

			if( str.GetLength() < sendMsg.GetLength() ) break;
			str = str.Mid( sendMsg.GetLength() );
			nLines++;
		}
//*/ END ---------------------------------------------------------------------------

		//2004-10-13
		INT nCnt = 0 ;
		//if ( !str.IsEmpty() )
		//{
			nCnt = GetLinesByEnter(str) ;
			nLines += nCnt ;
		//}
		
		//---------------------------------------
		//2005-01-06 leehakjoong
		INT nQuotient = length / char_len;
		INT nRemain = 0;
		if ( length > char_len )
		{
			nRemain = length % char_len;
		}	
		 
		if (nRemain == 0) {
			//2005-02-18 leehakjoong
			//return nQuotient+1;
			return nLines;
		} else {
				//2005-02-18 leehakjoong
			//return nQuotient+1;
			return 1+nLines;
		}
		//---------------------------------------
		
		return nLines;
	}

EFS_END
return -1;

}


INT CSIReportManager::GetLinesByEnter(CString str)
{
EFS_BEGIN

	INT length = str.GetLength();
	if (length == 0) return 0;			// ������ ������ ���ٷ� �����Ѵ�.

	INT pos = -1;
	INT nRowCount = 1;
	while(true) {
		pos = str.Find('\n', pos+1);
		if (pos == -1) break;
		nRowCount++;
	}
	return nRowCount;

EFS_END
return -1;

}

INT CSIReportManager::DrawLine(CDC *pDC, INT x1, INT y1, INT x2, INT y2)
{
EFS_BEGIN

	if (!pDC) return 0;
	pDC->MoveTo(x1, y1);
	pDC->LineTo(x2, y2);
	return 0;

EFS_END
return -1;

}

INT CSIReportManager::DrawSlash(CDC *pDC, CRect rect, INT mode/* 1-\, 2-/ */)
{
EFS_BEGIN

	if (!pDC) return 0;
	if (mode == 1) /* \ */
	{
		DrawLine(pDC, rect.left, rect.top, rect.right, rect.bottom);
	}
	if (mode == 2) /* / */
	{
		DrawLine(pDC, rect.right, rect.top, rect.left, rect.bottom);
	}
	return 0;

EFS_END
return -1;

}

INT CSIReportManager::DrawBox(CDC *pDC, INT left, INT top, INT right, INT bottom, BOOL bleft, BOOL btop, BOOL bright, BOOL bbottom)
{
EFS_BEGIN

	if (!pDC) return 0;

	if (bleft) {			// LEFT
		pDC->MoveTo(left, bottom);
		pDC->LineTo(left, top);
	}
	if (btop) {				// TOP
		pDC->MoveTo(left, top);
		pDC->LineTo(right, top);
	}
	if (bright)	{			// RIGHT
		pDC->MoveTo(right, top);
		pDC->LineTo(right, bottom);
	}
	if (bbottom) {			// BOTTOM
		pDC->MoveTo(right, bottom);
		pDC->LineTo(left, bottom);
	}
	return 0;

EFS_END
return -1;

}
// << 2015�� ���������� �ý��� ���� 2015/11/11 _LCM : ��±�� �������
INT CSIReportManager::Print(BOOL* pIsPrint)
{
EFS_BEGIN

	CSIReportPreviewDlg dlg;
	dlg.m_pManager = this;

	m_nCurrentPreviewPage = -1;
	
	//++2008.09.30 UPDATE BY PWR {{++
	//EXCEL����� �����ϸ� EXCEL�̸������ �۵��Ѵ�
	CString sStyleDirectory = _T("..\\CFG\\RptStyle");	//���������ġ
	//m_sStyleFile = sStyleDirectory + _T("\\") + m_strRPTALIAS + _T(".xls");	//�������

	CFileFind fFind;

	// 18.10.30 JMJ KOL.RED.2018.021
	//-------------------------------------------------------------------------------------
	CESLDlg_ExportType TempDlg;
	CString strStyleFile;
	INT ids = 0;
	strStyleFile = sStyleDirectory + _T("\\") + m_strRPTALIAS;

	BOOL	bExistsExcelStyle = FALSE, // ���� + �Ѽ� ���̾�
			bExistsCSVStyle = FALSE
			;
	// ��������� �Ѽ����� ���̾� �� ������,  CSV�� ���� �� �� ����.
	if(fFind.FindFile(strStyleFile + _T(".xls")) == TRUE)
	{
		bExistsExcelStyle = TRUE;
	}
	if(fFind.FindFile(strStyleFile + _T(".csv")) == TRUE)
	{
		bExistsCSVStyle = TRUE;
	}

	if(bExistsExcelStyle == TRUE || bExistsCSVStyle == TRUE)
	{
		// RPTStyle�� ���� ��� �ش������� ����ؾ� ��.
		TempDlg.m_bExistsExcelStyle = bExistsExcelStyle;
		TempDlg.m_bExistsCSVStyle = bExistsCSVStyle;
		if(TempDlg.DoModal() == IDOK)
		{
			if(TempDlg.m_bIsExcel == TRUE)
			{
				m_sStyleFile = strStyleFile + _T(".xls");
				ids = ExcelToExport();
			}
			else if(TempDlg.m_bIsHancell == TRUE)
			{
				m_sStyleFile = strStyleFile + _T(".xls");
				ids = HCellToExport();
			}
			else if(TempDlg.m_bIsCSV == TRUE)
			{
				m_sStyleFile = strStyleFile + _T(".csv");
				ids = CSVToExport();
			}
			else
			{
				ids = 10;
			}
		}
		else
		{
			// ��¹� ����� RPTStyle�� �ֱ� ������ MakeXMLPageFiles�� ���� �ȵ�.
			// �����;���
			//ids = 10;
		}
	}

	if( (bExistsExcelStyle == FALSE && bExistsCSVStyle == FALSE) ||	ids == 10 )
	{
		MakeXMLPageFiles();
		if (dlg.DoModal() != IDOK) 
		{
			return 0;
		}
		else
		{
			if(pIsPrint)
				*pIsPrint = TRUE;
		}
	}
	//-------------------------------------------------------------------------------------
/*

	if( !fFind.FindFile(m_sStyleFile) )
	{
		MakeXMLPageFiles();
		if (dlg.DoModal() != IDOK) 
		{
			return 0;
		}
		else
		{
			if(pIsPrint)
				*pIsPrint = TRUE;
		}
	}
	else
	{
		INT ids;
		// 16/03/29 ������ : �Ѽ� �����ϱ�
/// ADD -------------------------------------------------------------------------------------
		// 16/04/05 ������ : ��ȣ���� ������ ���� �ӽ��ּ�ó�� -> // 16/04/19 ������ : �ּ�Ǯ��
		if (m_bIsExcel != TRUE) 
		{
			ids = HCellToExport();
		}	
		else
/// END -------------------------------------------------------------------------------------
		{
			ids = ExcelToExport();
		}

		if(ids == 10)
		{
			MakeXMLPageFiles();
			if (dlg.DoModal() != IDOK) 
			{
				return 0;
			}
			else
			{
				if(pIsPrint)
					*pIsPrint = TRUE;
			}
		}
	}
	*/
	//MakeXMLPageFiles();
	//if (dlg.DoModal() != IDOK) return 0;
	//--2008.09.30 UPDATE BY PWR --}}
	return 0;

EFS_END
return -1;

}

INT CSIReportManager::Print2(BOOL* pIsPrint)
{
EFS_BEGIN

	
	//++2008.10.09 UPDATE BY CJY {{++
	//EXCEL����� �����ϸ� EXCEL�̸������ �۵��Ѵ�
	CString sStyleDirectory = _T("..\\CFG\\RptStyle");	//���������ġ
	//m_sStyleFile = sStyleDirectory + _T("\\") + m_strRPTALIAS + _T(".xls");	//�������

	CFileFind fFind;
		// 18.10.30 JMJ KOL.RED.2018.021
	//-------------------------------------------------------------------------------------
	CString strStyleFile;
	INT ids = 0;
	strStyleFile = sStyleDirectory + _T("\\") + m_strRPTALIAS;

	BOOL	bExistsExcelStyle = FALSE, // ���� + �Ѽ� ���̾�
			bExistsCSVStyle = FALSE
			;
	// ��������� �Ѽ����� ���̾� �� ������,  CSV�� ���� �� �� ����.
	if(fFind.FindFile(strStyleFile + _T(".xls")) == TRUE)
	{
		bExistsExcelStyle = TRUE;
	}
	if(fFind.FindFile(strStyleFile + _T(".csv")) == TRUE)
	{
		bExistsCSVStyle = TRUE;
	}

	if(bExistsExcelStyle == TRUE || bExistsCSVStyle == TRUE)
	{
		CESLDlg_ExportType TempDlg;
		// RPTStyle�� ���� ��� �ش������� ����ؾ� ��.
		TempDlg.m_bExistsExcelStyle = bExistsExcelStyle;
		TempDlg.m_bExistsCSVStyle = bExistsCSVStyle;
		if(TempDlg.DoModal() == IDOK)
		{
			if(TempDlg.m_bIsExcel == TRUE)
			{
				m_sStyleFile = strStyleFile + _T(".xls");
				ids = ExcelToExport();
			}
			else if(TempDlg.m_bIsHancell == TRUE)
			{
				m_sStyleFile = strStyleFile + _T(".xls");
				ids = HCellToExport();
			}
			else if(TempDlg.m_bIsCSV == TRUE)
			{
				m_sStyleFile = strStyleFile + _T(".csv");
				ids = CSVToExport();
			}
			else
			{
				ids = 10;
			}
		}
		else
		{
			ids = 10;
		}
	}

	if( (bExistsExcelStyle == FALSE && bExistsCSVStyle == FALSE) ||	ids == 10 )
	{
		CSIReportPreviewDlg dlg;
		dlg.m_pManager = this;
		
		m_EventInfo.m_nTotalPage = GetTmpPageNum();
		m_nCurrentPreviewPage = -1;
		if (dlg.DoModal() != IDOK) 
		{
			return 0;
		}
		else
		{
			if(pIsPrint)
				*pIsPrint = TRUE;
		}
	}
	/*
	if( !fFind.FindFile(m_sStyleFile) )
	{
		CSIReportPreviewDlg dlg;
		dlg.m_pManager = this;
		
		m_EventInfo.m_nTotalPage = GetTmpPageNum();
		m_nCurrentPreviewPage = -1;
		if (dlg.DoModal() != IDOK) 
		{
			return 0;
		}
		else
		{
			if(pIsPrint)
				*pIsPrint = TRUE;
		}
	}
	else
	{
		INT ids;
		// 16/03/29 ������ : �Ѽ� �����ϱ�
/// ADD -------------------------------------------------------------------------------------
		// 16/04/05 ������ : ��ȣ���� ������ ���� �ӽ��ּ�ó�� -> // 16/04/19 ������ : �ּ�Ǯ��
		if (m_bIsExcel != TRUE) 
		{
			ids = HCellToExport();
		}	
		else
/// END -------------------------------------------------------------------------------------
		{
			ids = ExcelToExport();
		}

		if(ids==10)
		{
			CSIReportPreviewDlg dlg;
			dlg.m_pManager = this;
			
			m_EventInfo.m_nTotalPage = GetTmpPageNum();
			m_nCurrentPreviewPage = -1;
			if (dlg.DoModal() != IDOK) 
			{
				return 0;
			}
			else
			{
				if(pIsPrint)
					*pIsPrint = TRUE;
			}
		}
	}
	*/
//	�����ҽ�
//DEL 	CSIReportPreviewDlg dlg;
//DEL 	dlg.m_pManager = this;
//DEL 
//DEL 	m_EventInfo.m_nTotalPage = GetTmpPageNum();
//DEL 	m_nCurrentPreviewPage = -1;
//DEL 	if (dlg.DoModal() != IDOK) return 0;
	//--2008.10.09 UPDATE BY CJY --}}
	return 0;

EFS_END
return -1;

}
// >> 2015/11/11 --MOD

INT CSIReportManager::PreviewDocument(CDC *pDC, INT page)
{
EFS_BEGIN

	InitArray(m_fTotalSum, MAX_COLUMN);
	INT ids = PREVIEW_DrawWorkForm(pDC, page);

	return ids;

EFS_END
return -1;

}

INT CSIReportManager::PREVIEW_DrawWorkForm(CDC *pDC, INT page, INT startpage)
{
EFS_BEGIN

	m_EventInfo.m_pSIReportManager = (CObject*)this;
	m_EventInfo.mode = 2;
	m_DMCursor = -1;		// ���� ����Ÿ ������

	CDC *pPageDC;
	INT pagecount = startpage;		// ù�������� 1 �������� �Ѵ�.
	INT count = m_pWorkForm->IncludeList.GetCount();
	if (count == 0) return -1;
	CSIReportWorkSheet * pSheet;
	POSITION pos;
	pos = m_pWorkForm->IncludeList.GetHeadPosition();
	INT ids;
	CString strTmp;
	for(INT i = 0; i < count; i++) {
		pSheet = NULL;
		pSheet = (CSIReportWorkSheet*)m_pWorkForm->IncludeList.GetNext(pos);
		if (!pSheet) continue;
		m_nCurrentWorkSheet = i;
		InitArray(m_fTotalSum, MAX_COLUMN);

		// ������ 0�̸� ��Ʈ �μ� ��, 1 �̸� ������������ ���� ��Ʈ ���
		BOOL bFirst = FALSE;
		while(true) {
			if (pagecount == page) pPageDC = pDC;
				else pPageDC = NULL;
			m_EventInfo.pDC = pPageDC;
			// (START) - ���� ������ ����� WorkForm����
			FreePageWorkForm();
			m_pPageWorkForm = NULL;
			if (XML_MODE == 1) 
			{
				m_pPageWorkForm = MakeWorkFormClone(m_pWorkForm);		// ���� ������ ��ũ�� ����(������ �����)
				CSIReportWorkSheet *ptmpWorkSheet = m_pPageWorkForm->GetWorkSheet(0);
				strTmp.Format(_T("������ %04d"), pagecount);
				ptmpWorkSheet->Alias = strTmp;
			}
			// ( END ) - ���� ������ ����� WorkForm����
			ids = DrawWorkSheet(MANAGER_MODE_PREVIEW, pPageDC, pSheet);
			m_EventInfo.m_nPage = pagecount;
			if (m_pOwnerWnd && m_fpEventHandler) {
				( m_pOwnerWnd->*m_fpEventHandler )( &m_EventInfo );
			}
			// (START) - ���� ������ ����
			if (XML_MODE == 1) 
			{
				strTmp.Format(_T("rpt_tmp\\%s%04d.xml"), m_strPageHeader, pagecount);
				m_pXMLIOMgr->SaveToXML(m_pPageWorkForm, strTmp);
				//strTmp.Format(_T("rpt_tmp\\%s%04d.rpt"), m_strPageHeader, pagecount);
				//m_pXMLIOMgr->SaveToXML(m_pPageWorkForm, strTmp);
			}
			// ( END ) - ���� ������ ����
			if (ids != 1 && i < count-1) {
				pagecount++;
				break;
			}
			if (ids != 1) break;
			//if (pagecount == page) break;
			pagecount++;
		}
	}
	m_DMCursor = -1;
	return pagecount;

EFS_END
return -1;

}

INT CSIReportManager::MAKEPAGE_DrawWorkForm(INT MODE/*=1*/)
{
EFS_BEGIN
	// MODE 1 : �ڵ� ����
	// MODE -1 : ��ũ�� manual �ݺ� ���

	if (m_pWorkForm == NULL) return -1;

	m_EventInfo.m_pSIReportManager = (CObject*)this;
	m_EventInfo.mode = MANAGER_MODE_PREVIEW;

	INT pagecount = m_nStartPageNum;
	INT count = m_pWorkForm->IncludeList.GetCount();
	if (count == 0) return -1;
	CSIReportWorkSheet * pSheet;
	POSITION pos = m_pWorkForm->IncludeList.GetHeadPosition();
	if (pos == NULL) return -1;
	INT ids;
	CString strTmp;
	INT nCurrentPageNum;
	for(INT i = 0; i < count; i++) {
		pSheet = NULL;
		pSheet = (CSIReportWorkSheet*)m_pWorkForm->IncludeList.GetNext(pos);
		if (!pSheet) continue;
		m_nCurrentWorkSheet = i;
		m_EventInfo.m_nSheetNum = i;

		if (pSheet->m_strDefaultDMAlias.GetLength() > 0) {
			if (FindDM(pSheet->m_strDefaultDMAlias) < 0) {
				m_pDefaultDM = m_pDM[0];
			}
		} else {
			m_pDefaultDM = m_pDM[0];
		}
		if (m_bNoDataSkip == TRUE) {
			if (m_pDefaultDM) {
				if (m_pDefaultDM->GetRowCount() == 0) continue;
			}
		}
		m_DMCursor = -1;
		InitArray(m_fTotalSum, MAX_COLUMN);
		// ������ 0�̸� ��Ʈ �μ� ��, 1 �̸� ������������ ���� ��Ʈ ���
		while(true) {
			nCurrentPageNum = pagecount;
			if (MODE == -1) nCurrentPageNum = GetTmpPageNum() + 1;
			m_EventInfo.pDC = NULL;
			m_EventInfo.m_nPage = nCurrentPageNum;
			
			// (START) - ���� ������ ����� WorkForm����
			FreePageWorkForm();
			m_pPageWorkForm = MakeWorkFormClone(m_pWorkForm);		// ���� ������ ��ũ�� ����(������ �����)
			CSIReportWorkSheet *ptmpWorkSheet = m_pPageWorkForm->GetWorkSheet(0);
			strTmp.Format(_T("������ %04d"), nCurrentPageNum);
			ptmpWorkSheet->Alias = strTmp;
			// ( END ) - ���� ������ ����� WorkForm����
			ids = PREVIEW_DrawWorkSheet(pSheet);

			if (m_pOwnerWnd && m_fpEventHandler) {
				( m_pOwnerWnd->*m_fpEventHandler )( &m_EventInfo );
			}

			// (START) - ���� ������ ����
			APPLY_RearrangeObjects(m_pPageWorkForm);
			strTmp.Format(_T("rpt_tmp\\%s%04d.xml"), m_strPageHeader, nCurrentPageNum);
			m_pXMLIOMgr->SaveToXML(m_pPageWorkForm, strTmp);
			//strTmp.Format(_T("rpt_tmp\\%s%04d.rpt"), m_strPageHeader, nCurrentPageNum);
			//m_pXMLIOMgr->SaveToXML(m_pPageWorkForm, strTmp);
			// ( END ) - ���� ������ ����

			if (ids != 1 && i < count-1) {
				pagecount++;
				break;
			}
			if (ids != 1) break;
			pagecount++;
		}
	}
	return pagecount;

EFS_END
return -1;

}

CSIReportWorkForm *CSIReportManager::MakeWorkFormClone(CSIReportWorkForm *pWorkFormSource)
{
EFS_BEGIN

	FreePageWorkForm();
	CSIReportWorkForm *pWorkForm = new CSIReportWorkForm(MASTER_IS_MANAGER);
	if (!pWorkForm) return NULL;			// �޸� �Ҵ� ����
	if (!pWorkFormSource) return NULL;		// ������ ����

	pWorkForm->alias = pWorkFormSource->alias;
	pWorkForm->paper = pWorkFormSource->paper;
	pWorkForm->sheet_height = pWorkFormSource->sheet_height;
	pWorkForm->sheet_size = pWorkFormSource->sheet_size;
	pWorkForm->VERSION = pWorkFormSource->VERSION;

	pWorkForm->AddNewSheet();
	CSIReportWorkSheet *ptmpWorkSheet = pWorkForm->GetWorkSheet(0);
	if (ptmpWorkSheet) {
		ptmpWorkSheet->Idx = 0;
		ptmpWorkSheet->master_sheet = true;
		pWorkForm->IncludeList.AddTail((CObject*)ptmpWorkSheet);
	}
	return pWorkForm;

EFS_END
return NULL;

}

INT CSIReportManager::DrawWorkSheet(INT mode, CDC *pDC, CSIReportWorkSheet *pSheet)
{
EFS_BEGIN

	INT count = pSheet->ObjList.GetCount();
	if (count == 0) return -1;

	InitArray(m_fPartSum, MAX_COLUMN);
	m_bPrintTotal = true;		// �Ѱ踦 ���� �Ѵ�. �� ���� ���� �ʾҴ�.
	// Child�� �ƴ� ��ü�߿��� Child�� ���� ��ü ���� �μ��Ѵ�.
	CSIReportObject * pObject;
	POSITION pos;
	INT i;
	pos = pSheet->ObjList.GetHeadPosition();
	for(i = 0; i < count; i++) {
		pObject = NULL;
		pObject = (CSIReportObject*)pSheet->ObjList.GetNext(pos);
		if (!pObject) continue;
		if (pObject->GetObjType() == OBJECT_TABLE) {
			CSIReportObject_Table *pTable = (CSIReportObject_Table *)pObject;
			if (pTable->total_sum) m_bPrintTotal = false;
			if (IsChild(pTable)) continue;
			DrawObject_Table(mode, pDC, pTable);
		}
		if (pObject->GetObjType() == OBJECT_BARCODE) {
			CSIReportObject_Barcode *pBarcode = (CSIReportObject_Barcode *)pObject;
			DrawObject_Barcode(mode, pDC, pBarcode);
		}
		if (pObject->GetObjType() == OBJECT_TEXT_BOX) {
			CSIReportObject_Text_Box *pText_Box = (CSIReportObject_Text_Box *)pObject;
			DrawObject_Text_Box(mode, pDC, pText_Box);
		}
		if (pObject->GetObjType() == OBJECT_LINE) {
			CSIReportObject_Line *pLine = (CSIReportObject_Line *)pObject;
			DrawObject_Line(mode, pDC, pLine);
		}
		if (pObject->GetObjType() == OBJECT_BITMAP) {
			CSIReportObject_Bitmap *pBitmap = (CSIReportObject_Bitmap *)pObject;
			DrawObject_Bitmap(mode, pDC, pBitmap);
		}
	}

	// �ȱ׷������� Child�� ������ Cell�� Child ���̴�. ���� Child�� �μ�����.
	CSIReportObject_Table_Cell *pCell;
	CSIReportObject_Table_Cell *pTmpCell;				// temporary cell
	CSIReportObject_Table * pChild_Table;
	INT row, col, child;
	CString child_name;
	CRect tmpRect;
	pos = pSheet->ObjList.GetHeadPosition();
	for(i = 0; i < count; i++) {
		pObject = NULL;
		pObject = (CSIReportObject*)pSheet->ObjList.GetNext(pos);
		if (!pObject) continue;
		if (pObject->GetObjType() != OBJECT_TABLE) continue;
		
		CSIReportObject_Table *pTable = (CSIReportObject_Table *)pObject;
		if (!HaveChild(pTable)) continue;
		for (row = 0; row < pTable->row_count; row++) {
			for (col = 0; col < pTable->col_count; col++) {
				pCell = pTable->GetCell(row, col);
				if (!pCell) continue;
				if (pCell->bind_status == 2) continue;
				if (pCell->child_count == 0) continue;
				for (child = 0; child < pCell->child_count; child++) {
					pChild_Table = FindTable(pSheet, pCell->child_name[child]);
					if (!pChild_Table) continue;
					// ���� �ݺ� �μ⸦ ��������.
					pTmpCell = pTable->GetCell(pCell->bind_row, pCell->bind_col);				
					tmpRect = pCell->GetRect();
					tmpRect.right = pTmpCell->GetRect().right;
					tmpRect.bottom = pTmpCell->GetRect().bottom;
					DrawObject_Table_Repeat(mode, pDC, pChild_Table, tmpRect);
				}
				tmpRect = pCell->GetRect();
				if (pCell->bind_status == 1) {
					pTmpCell = pTable->GetCell(pCell->bind_row, pCell->bind_col);				
					tmpRect.right = pTmpCell->GetRect().right;
					tmpRect.bottom = pTmpCell->GetRect().bottom;
				}
				DrawBox(pDC, DrawGetMMToPixel(mode, (double)tmpRect.left), DrawGetMMToPixel(mode, (double)tmpRect.top), DrawGetMMToPixel(mode, (double)tmpRect.right), DrawGetMMToPixel(mode, (double)tmpRect.bottom));
			}
		}	
	}
	if (pDC && IsSheetHaveRepeatObject() == FALSE) return 0;
	if (m_DMCursor < 0) return 0;				// Data�� ���� �����ϹǷ� Sheet�� ������������ �Ѿ�� ����
	INT nRowCount = 0;
	if (m_pDefaultDM) nRowCount = m_pDefaultDM->GetRowCount();
	if (nRowCount-1 <= m_DMCursor) {
		if (!m_bPrintTotal) return 1;			// �Ѱ踦 �μ� ���ϰ� ���� ���� ���������� ����� �Ѱ踦 ����ش�.
		return 0;		// Sheet �μ� ��
	} 
	else
	{
		return 1;								// Sheet �μ� ������(���� �������� �ѱ�)
	}

EFS_END
return -1;

}

INT CSIReportManager::PREVIEW_DrawWorkSheet(CSIReportWorkSheet *pSheet)
{
EFS_BEGIN

	INT count = pSheet->ObjList.GetCount();
	if (count == 0) return 0;

	CDC *pDC = NULL;
	INT mode = MANAGER_MODE_PREVIEW;

	InitArray(m_fPartSum, MAX_COLUMN);
	m_bPrintTotal = true;		// �Ѱ踦 ���� �Ѵ�. �� ���� ���� �ʾҴ�.
	// Child�� �ƴ� ��ü�߿��� Child�� ���� ��ü ���� �μ��Ѵ�.
	CSIReportObject * pObject;
	POSITION pos;
	INT i;
	pos = pSheet->ObjList.GetHeadPosition();
	for(i = 0; i < count; i++) {
		pObject = NULL;
		pObject = (CSIReportObject*)pSheet->ObjList.GetNext(pos);
		if (!pObject) continue;
		if (pObject->GetObjType() == OBJECT_TABLE) {
			CSIReportObject_Table *pTable = (CSIReportObject_Table *)pObject;
			if (pTable->total_sum) m_bPrintTotal = false;
			if (IsChild(pTable)) continue;
			DrawObject_Table(mode, pDC, pTable);
		}
		if (pObject->GetObjType() == OBJECT_BARCODE) {
			CSIReportObject_Barcode *pBarcode = (CSIReportObject_Barcode *)pObject;
			DrawObject_Barcode(mode, pDC, pBarcode);
		}
		if (pObject->GetObjType() == OBJECT_TEXT_BOX) {
			CSIReportObject_Text_Box *pText_Box = (CSIReportObject_Text_Box *)pObject;
			DrawObject_Text_Box(mode, pDC, pText_Box);
		}
		if (pObject->GetObjType() == OBJECT_LINE) {
			CSIReportObject_Line *pLine = (CSIReportObject_Line *)pObject;
			DrawObject_Line(mode, pDC, pLine);
		}
		if (pObject->GetObjType() == OBJECT_BITMAP) {
			CSIReportObject_Bitmap *pBitmap = (CSIReportObject_Bitmap *)pObject;
			DrawObject_Bitmap(mode, pDC, pBitmap);
		}
	}

	// �ȱ׷������� Child�� ������ Cell�� Child ���̴�. ���� Child�� �μ�����.
	CSIReportObject_Table_Cell *pCell;
	CSIReportObject_Table_Cell *pTmpCell;				// temporary cell
	CSIReportObject_Table * pChild_Table;
	INT row, col, child;
	CString child_name;
	CRect tmpRect;
	pos = pSheet->ObjList.GetHeadPosition();
	for(i = 0; i < count; i++) {
		pObject = NULL;
		pObject = (CSIReportObject*)pSheet->ObjList.GetNext(pos);
		if (!pObject) continue;
		if (pObject->GetObjType() != OBJECT_TABLE) continue;
		
		CSIReportObject_Table *pTable = (CSIReportObject_Table *)pObject;
		if (!HaveChild(pTable)) continue;
		for (row = 0; row < pTable->row_count; row++) {
			for (col = 0; col < pTable->col_count; col++) {
				pCell = pTable->GetCell(row, col);
				if (!pCell) continue;
				if (pCell->bind_status == 2) continue;
				if (pCell->child_count == 0) continue;
				for (child = 0; child < pCell->child_count; child++) {
					pChild_Table = FindTable(pSheet, pCell->child_name[child]);
					if (!pChild_Table) continue;
					// ���� �ݺ� �μ⸦ ��������.
					pTmpCell = pTable->GetCell(pCell->bind_row, pCell->bind_col);				
					tmpRect = pCell->GetRect();
					tmpRect.right = pTmpCell->GetRect().right;
					tmpRect.bottom = pTmpCell->GetRect().bottom;
					DrawObject_Table_Repeat(mode, pDC, pChild_Table, tmpRect);
				}
				tmpRect = pCell->GetRect();
				if (pCell->bind_status == 1) {
					pTmpCell = pTable->GetCell(pCell->bind_row, pCell->bind_col);				
					tmpRect.right = pTmpCell->GetRect().right;
					tmpRect.bottom = pTmpCell->GetRect().bottom;
				}
				DrawBox(pDC, DrawGetMMToPixel(mode, (double)tmpRect.left), DrawGetMMToPixel(mode, (double)tmpRect.top), DrawGetMMToPixel(mode, (double)tmpRect.right), DrawGetMMToPixel(mode, (double)tmpRect.bottom));
			}
		}	
	}
	if (pDC && IsSheetHaveRepeatObject() == FALSE) return 0;
	if (m_DMCursor < 0) return 0;				// Data�� ���� �����ϹǷ� Sheet�� ������������ �Ѿ�� ����
	INT nRowCount = 0;
	if (m_pDefaultDM) nRowCount = m_pDefaultDM->GetRowCount();
	if (nRowCount-1 <= m_DMCursor) {
		if (!m_bPrintTotal) return 1;			// �Ѱ踦 �μ� ���ϰ� ���� ���� ���������� ����� �Ѱ踦 ����ش�.
		return 0;		// Sheet �μ� ��
	} 
	else
	{
		return 1;								// Sheet �μ� ������(���� �������� �ѱ�)
	}

EFS_END
return -1;

}

INT CSIReportManager::DrawGetMMToPixel(INT mode, double mm)
{
EFS_BEGIN

	if (mode == MANAGER_MODE_PRINT) {
		mm = 297.0 * mm / 980.0;
		//return ((INT)((double)(1440*0.03937) * mm));	
		return ((INT)((double)(m_nResolution*0.03937) * mm));	
	} else {
		return (INT)mm;
	}

EFS_END
return -1;

}

INT CSIReportManager::DrawObject_Table(INT mode, CDC *pDC, CSIReportObject_Table * pTable)
{
EFS_BEGIN

	if (!pTable) return -2;
	INT nRow = pTable->row_count;
	INT nCol = pTable->col_count;
	INT row, col;
	BOOL left, top, right, bottom;
	CSIReportObject_Table_Cell *pCell;
	CSIReportObject_Table_Cell *pTmpCell;
	CRect real_cell_rect;

// (START) - XML Page Output - Copy Table
	if (m_pPageWorkForm) 
	{
		CSIReportWorkSheet *ptmpWorkSheet = m_pPageWorkForm->GetWorkSheet(0);
		CSIReportObject_Table *ptmpObject = (CSIReportObject_Table *)ptmpWorkSheet->AddNewObject(OBJECT_TABLE);
		pTable->Clone(ptmpObject);
		for (row = 0; row < nRow; row++) {
			for (col = 0; col < nCol; col++) {
				pTmpCell = ptmpObject->GetCell(row, col);
				if (!pTmpCell) continue;
				if (pTmpCell->bind_status == 1 && pTmpCell->child_count > 0) {
					pTmpCell->child_count = 0;
					pTmpCell->child_name[0] = _T("");
					pTmpCell->bind_status = pTmpCell->bind_status;
				}
				pTmpCell->text.font.lfHeight = CalcNegativeFontHeight(pTmpCell->text.size);
			}
		}
	}
// ( END ) - XML Page Output - Copy Table

	//if (!pDC) return -1;

	COLORREF cBLACK = RGB(0, 0, 0);
	COLORREF cWHITE = RGB(255, 255, 255);

	for (row = 0; row < nRow; row++) {
		for (col = 0; col < nCol; col++) {
			pCell = pTable->GetCell(row, col);
			left = top = right = bottom = true;
			if (!pCell) continue;
			if (pCell->bind_status == 2) continue;
			if (pCell->bind_status == 0) {
				CRect tmpRect = pCell->GetRect();
				CalcRectOffSet(tmpRect, pCell->indent_len, pCell->indent_len, -pCell->indent_len, -pCell->indent_len);					
				real_cell_rect = ConvertRectMMToPixel(mode, pCell->GetRect());
				if (pCell->fill_color != cWHITE) {
					if (pDC) {
						CBrush bk_brush;
						bk_brush.CreateSolidBrush(pCell->fill_color);
						pDC->FillRect(real_cell_rect, &bk_brush);
						pDC->SetBkColor(pCell->fill_color);
						pDC->SetBkMode(TRANSPARENT);
					}
				}
				real_cell_rect = ConvertRectMMToPixel(mode, tmpRect);
				//DrawTextToCell(mode, pDC, pCell, real_cell_rect);
				if (pCell->text.text == _T("%BARCODE���ڵ�%")) {
					LOC_PrintBarcode(mode, pDC, real_cell_rect);
				} else {
					DrawTextToCell2(mode, pDC, pCell, real_cell_rect);
				}
				real_cell_rect = ConvertRectMMToPixel(mode, pCell->GetRect());
				DrawBox(pDC, real_cell_rect.left, real_cell_rect.top, real_cell_rect.right, real_cell_rect.bottom, pTable->vert_line, pTable->horz_line, pTable->vert_line, pTable->horz_line);
				DrawSlash(pDC, real_cell_rect, pCell->slash);
				if (pDC) pDC->SetBkColor(cWHITE);
			}
			if (pCell->bind_status == 1) {
				if (pCell->child_count > 0) {		// Child �� �ִ� ���� Skip �Ѵ�.
					;
				} else {
					pTmpCell = pTable->GetCell(pCell->bind_row, pCell->bind_col);
					CRect tmpRect = pCell->GetRect();
					tmpRect.right = pTmpCell->GetRect().right;
					tmpRect.bottom = pTmpCell->GetRect().bottom;
					CRect tmpRect_real = tmpRect;
					CalcRectOffSet(tmpRect, pCell->indent_len, pCell->indent_len, -pCell->indent_len, -pCell->indent_len);					
					real_cell_rect = ConvertRectMMToPixel(mode, tmpRect);
					if (pCell->fill_color != cWHITE && pDC ) {
						CBrush bk_brush;
						bk_brush.CreateSolidBrush(pCell->fill_color);
						pDC->FillRect(real_cell_rect, &bk_brush);
						pDC->SetBkColor(pCell->fill_color);
						pDC->SetBkMode(TRANSPARENT);
					}
					//DrawTextToCell(mode, pDC, pCell, real_cell_rect);
					if (pCell->text.text == _T("%BARCODE���ڵ�%")) {
						LOC_PrintBarcode(mode, pDC, real_cell_rect);
					} else {
						DrawTextToCell2(mode, pDC, pCell, real_cell_rect);
					}
					DrawSlash(pDC, real_cell_rect, pCell->slash);
					//real_cell_rect = ConvertRectMMToPixel(mode, pCell->GetRect());
					real_cell_rect = ConvertRectMMToPixel(mode, tmpRect_real);
					DrawBox(pDC, real_cell_rect.left, real_cell_rect.top, real_cell_rect.right, real_cell_rect.bottom, pTable->vert_line, pTable->horz_line, pTable->vert_line, pTable->horz_line);
					if (pDC) pDC->SetBkColor(cWHITE);
				}
			}
		}
	}

	return 0;

EFS_END
return -1;

}

INT CSIReportManager::DrawTextToCell(INT mode, CDC *pDC, CSIReportObject_Table_Cell *pCell, CRect rect)
{
EFS_BEGIN

	if (!pDC) return 0;
	if (!pCell) return 0;
	
	UINT nFormat = DT_NOPREFIX|DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	if (pCell->vert_align == ALIGN_TOP)			nFormat = DT_SINGLELINE|DT_TOP;
	if (pCell->vert_align == ALIGN_VCENTER)		nFormat = DT_SINGLELINE|DT_VCENTER;
	if (pCell->vert_align == ALIGN_BOTTOM)		nFormat = DT_SINGLELINE|DT_BOTTOM;

	if (pCell->horz_align == ALIGN_LEFT)		nFormat = nFormat|DT_LEFT;
	if (pCell->horz_align == ALIGN_HCENTER)		nFormat = nFormat|DT_CENTER;
	if (pCell->horz_align == ALIGN_RIGHT)		nFormat = nFormat|DT_RIGHT;

	CalcRectOffSet(rect, 2, 2, -2, -2);
	pCell->text.font.lfHeight = pCell->text.size;
	DrawTextWithFont(mode, pDC, pCell->text.text, rect, pCell->text.font, nFormat);
	return 0;

EFS_END
return -1;

}

INT CSIReportManager::DrawTextToCell2(INT mode, CDC *pDC, CSIReportObject_Table_Cell *pCell, CRect rect)
{
EFS_BEGIN

	// EnterŰ ������ ������ ©�� ����Ѵ�.
	if (!pDC) return 0;
	if (!pCell) return 0;
	
	UINT nFormat = DT_NOPREFIX|DT_VCENTER|DT_SINGLELINE;
	if (pCell->horz_align == ALIGN_LEFT)		nFormat = nFormat|DT_LEFT;
	if (pCell->horz_align == ALIGN_HCENTER)		nFormat = nFormat|DT_CENTER;
	if (pCell->horz_align == ALIGN_RIGHT)		nFormat = nFormat|DT_RIGHT;

	CalcRectOffSet(rect, pCell->indent_len, pCell->indent_len, -pCell->indent_len, -pCell->indent_len);
	pCell->text.font.lfHeight = pCell->text.size;

	INT nLines = GetLinesByEnter(pCell->text.text);
	INT Height;
	if (mode == MANAGER_MODE_PRINT) {		// �μ���
		Height = MulDiv(pCell->text.size, m_nResolution, 72);
	} else {								// Preview ���
		Height = MulDiv(pCell->text.size, 96, 72);
	}
	if (pCell->vert_align == ALIGN_VCENTER) {
		rect.top = rect.top + (((rect.bottom - rect.top) - (Height*nLines)) / 2);
	}
	if (pCell->vert_align == ALIGN_BOTTOM) {
		rect.top = rect.top + (((rect.bottom - rect.top) - (Height*nLines)));
	}
	DrawTextWithFont3(mode, pDC, pCell->text.text, rect, pCell->text.font, nFormat);
	return 0;

EFS_END
return -1;

}

INT CSIReportManager::DrawObject_Table_Repeat(INT mode, CDC *pDC, CSIReportObject_Table * pTable, CRect rect)
{
EFS_BEGIN

	if (!pTable) return -2;

	double startY = (double)rect.top;								// ���� Pixel(����Ʈ ��ǥ�� A4 : 980 X 693)
	double curY = 0;
	double height, max_height;										// ���� mm
	INT nLines, max_line;											// Line�� Column���߿��� ���� ���� ����
	INT remain_rect_height;
	INT nCol = pTable->col_count;
	INT cursor;
	INT col;
	INT nDMRow = 0;
	if (m_pDefaultDM) nDMRow = m_pDefaultDM->GetRowCount();
	INT nRectTop = rect.bottom - pTable->GetRect().Height();
	INT nIsLast = false;											// ����Ÿ�� ����ϴ� ������ �������ΰ�?
	CString str;
	CRect origin_rect = rect;
	CRect cell_rect;
	CRect RectNoMoreStatement;
	BOOL bIsPrintEmptySpace = FALSE;

	if (m_DMCursor < 0) m_DMCursor = 0;
	if (pTable->part_sum) rect.bottom -= pTable->GetRect().Height();

	// (START) - XML Page Output
	CSIReportWorkSheet *ptmpWorkSheet = NULL;
	CSIReportObject_Table *ptmpObject = NULL;
	CSIReportObject_Table_Cell *pCopyCell;
	INT nCurRow = 0;

	if (m_pPageWorkForm) {
		ptmpWorkSheet = m_pPageWorkForm->GetWorkSheet(0);
		ptmpObject = (CSIReportObject_Table *)ptmpWorkSheet->AddNewObject(OBJECT_TABLE);
		ptmpObject->SetName(pTable->GetName());
		ptmpObject->horz_line = pTable->horz_line;
		ptmpObject->vert_line = pTable->vert_line;
		ptmpObject->SetParentName(pTable->GetParentName());
		ptmpObject->row_count = 0;
		ptmpObject->col_count = nCol;
	}
	
	// ( END ) - XML Page Output

	FILE *fd;
	fd = _tfopen(_T("rpt_tmp\\NEXTPAGE.TMP"), _T("rb"));
	if (fd) {
		fclose(fd);
		DeleteFile(_T("rpt_tmp\\NEXTPAGE.TMP"));
		if (m_DMCursor > 0) m_DMCursor++;
	}

	InitArray(m_fPartSum, MAX_COLUMN);
	CRect real_cell_rect, real_text_cell_rect;;
	CSIReportObject_Table_Cell *pCell;
	for (cursor = m_DMCursor; cursor < nDMRow; cursor++) {
		max_height = 0;
		max_line = 1;
		for (col = 0; col < nCol; col++) {
			str = _T("");
			pCell = pTable->GetCell(0, col);
			if (!pCell) continue;
			if (pCell->bind_status == 2) continue;
			cell_rect = pCell->GetRect();
			if (pCell->data_mgr_name.GetLength() > 0) {
				if (pCell->data_mgr_name.Find(_T("%")) == -1) {
					if (m_pDefaultDM) m_pDefaultDM->GetCellData(pCell->data_field, m_DMCursor, str);
				} else {
					DM_GetCellData(pCell->data_mgr_name, m_DMCursor, str);
				}
			} else {
				if (m_pDefaultDM) m_pDefaultDM->GetCellData(pCell->data_field, m_DMCursor, str);
			}
			str.TrimRight();
			pCell->char_cy = DrawGetchar_cy(mode, pDC, pCell->text.size, false);
			height = GetHeight(pCell, str, nLines);
			if ( height > max_height) 
			{
				max_line = nLines;
				max_height = height;
			}
		}
		remain_rect_height = rect.bottom - rect.top - (INT)(curY);
		if (max_height > remain_rect_height) {
			m_DMCursor--;
			fd = _tfopen(_T("rpt_tmp\\NEXTPAGE.TMP"), _T("wb"));
			if (fd) fclose(fd);
			break;
		}
		curY += max_height;
	// (START) - XML Page Output
		
		if (ptmpObject) {
			ptmpObject->AddRow();
		}
		
	// ( END ) - XML Page Output
		for (col = 0; col < nCol; col++) 
		{
			str = _T("");
			pCell = pTable->GetCell(0, col);
			if (!pCell) continue;
			if (pCell->bind_status == 2) continue;
			cell_rect = pCell->GetRect();
			cell_rect.top = (INT)(startY);
			cell_rect.bottom = (INT)(cell_rect.top + max_height);

			if (m_pDefaultDM) m_pDefaultDM->GetCellData(pCell->data_field, m_DMCursor, str);
			if (pCell->data_mgr_name.GetLength() > 0) {
				if (pCell->data_mgr_name.Find(_T("%")) == -1) {
					if (m_pDefaultDM) m_pDefaultDM->GetCellData(pCell->data_field, m_DMCursor, str);
				} else {
					DM_GetCellData(pCell->data_mgr_name, m_DMCursor, str);
				}
			} else {
				if (m_pDefaultDM) m_pDefaultDM->GetCellData(pCell->data_field, m_DMCursor, str);
			}
			// (START) - XML Page Output
			if (ptmpObject) {
				ptmpObject->SetCellData(nCurRow, col, pCell);
				pCopyCell = ptmpObject->GetCell(nCurRow, col);
				pCopyCell->text.font.lfHeight = CalcNegativeFontHeight(pCopyCell->text.size);
				pCopyCell->SetRect(cell_rect);
				//============================================================
				//2004--03-24 [������]
				if (str.Find(_T("\r\n"), 0) == -1)
				{
					pCopyCell->text.text = SplitStringWithLen(str, pCell->char_len, pCell->char_max);
				}
				else
				{
					//pCopyCell->text.text = str;
					pCopyCell->text.text = SplitStringWithLen(str, pCell->char_len, pCell->char_max);
				}

				//============================================================
			}
			// ( END ) - XML Page Output
			APPLY_Sum(col, pCell->part_sum, pCell->total_sum, str);
		}
		startY += max_height;
		m_DMCursor++;
		nCurRow++;
		if (pCell->GetRect().bottom - pCell->GetRect().top > remain_rect_height) {
			return 0;
		}
	}
	for (col = 0; col < nCol; col++) 
	{	// ���� ���� ������ ���� �ߴ´�.
		pCell = pTable->GetCell(0, col);
		if (!pCell) continue;
		if (pCell->bind_status == 2) continue;
		cell_rect = pCell->GetRect();
		cell_rect.top = rect.top;
		if (m_DMCursor == nDMRow) 
		{
			cell_rect.bottom = (INT)(startY);
			INT a = rect.bottom - cell_rect.bottom;
			INT space = (rect.bottom - cell_rect.bottom);
			if (pTable->part_sum) space -= pTable->GetRect().Height();
			if (pTable->total_sum) space -= pTable->GetRect().Height();
			//if ((double)(space) > (m_nNoMoreStatement * (PAPER_A4_Y*10.0) / 297.0)) 
			if (space > 13) 
			{
				real_cell_rect = rect;
				real_cell_rect.top = cell_rect.bottom;
				nRectTop = real_cell_rect.top;
				nIsLast = true;
				if (pTable->part_sum) real_cell_rect.top += pTable->GetRect().Height();
				if (pTable->total_sum) real_cell_rect.top += pTable->GetRect().Height();
				real_cell_rect.bottom = origin_rect.bottom;
				// ���Ͽ��� ���
				// (START) - XML Page Output
				CRect rTemp = real_cell_rect;
				CalcRectOffSet(rTemp, 2, 2, -2, -2);
				if (m_pPageWorkForm) AddCopyTextWithFont(m_strNoMoreStatement, rTemp, m_fNoMoreStatement, ALIGN_VCENTER, ALIGN_HCENTER);
				bIsPrintEmptySpace = TRUE;
				// ( END ) - XML Page Output
				break;
			} else {
				if (!pTable->part_sum && !m_bPrintTotal) {
					cell_rect.bottom = rect.bottom - pTable->GetRect().Height();
				} else {
					cell_rect.bottom = (INT)startY;
				}
				if (nRectTop + pTable->GetRect().Height() > origin_rect.bottom) {
					cell_rect.bottom = rect.bottom;
				} else {
					cell_rect.bottom = (INT)startY;
				}
				nRectTop = (INT)startY;//origin_rect.bottom - (pTable->GetRect().Height()*2);
				nIsLast = true;
				break;				
			}
		} 
		else 
		{
			cell_rect.bottom = origin_rect.bottom;
		}
	}
	CString strArray[MAX_COLUMN];
	INT nBottom;
	if (pTable->part_sum) {
		for (col = 0; col < nCol; col++) {
			if ((pCell = pTable->GetCell(0, col)) == NULL) continue;
			strArray[col] = _T("");
			if (pCell->part_sum == 1 /*����*/) strArray[col].Format(_T("%d"), (INT)m_fPartSum[col]);
			if (pCell->part_sum == 2 /*�Ǽ�*/) strArray[col].Format(_T("%g"), m_fPartSum[col]);
		}
		strArray[m_nColPartSum] = m_strPartSum;
		// (START) - XML Page Output
		if (ptmpObject) {
			CopyTableRow(ptmpObject, nCurRow-1, nRectTop);
			ptmpObject->AddRow();
			CopyTableRow(pTable, 0, ptmpObject, nCurRow, nRectTop, strArray);
		}
		nCurRow++;
		// ( END ) - XML Page Output
		nBottom = -1;
		if (!pTable->total_sum && origin_rect.bottom - (nRectTop+pTable->GetRect().Height()) < 5) nBottom = origin_rect.bottom;	
		nRectTop += pTable->GetRect().Height();
	}
	if (nRectTop + pTable->GetRect().Height() > origin_rect.bottom) return 0;
	if (pTable->total_sum && nIsLast) {
		for (col = 0; col < nCol; col++) {
			if ((pCell = pTable->GetCell(0, col)) == NULL) continue;
			strArray[col] = _T("");
			if (pCell->total_sum == 1 /*����*/) strArray[col].Format(_T("%d"), (INT)m_fTotalSum[col]);
			if (pCell->total_sum == 2 /*�Ǽ�*/) strArray[col].Format(_T("%g"), m_fTotalSum[col]);
		}
		strArray[m_nColTotalSum] = m_strTotalSum;
		// (START) - XML Page Output
		if (ptmpObject) {
			CopyTableRow(ptmpObject, nCurRow-1, nRectTop);
			ptmpObject->AddRow();
			CopyTableRow(pTable, 0, ptmpObject, nCurRow, nRectTop, strArray);
		}
		nCurRow++;
		// ( END ) - XML Page Output
		nBottom = -1;
		if (origin_rect.bottom - (nRectTop+pTable->GetRect().Height()) < 5) nBottom = origin_rect.bottom;	
		m_bPrintTotal = true;
	}
	if (bIsPrintEmptySpace == FALSE && ptmpObject != NULL) APPLY_RearrangeTables(rect, ptmpObject);
	return 0;

EFS_END
return -1;

}

INT CSIReportManager::APPLY_Sum(INT col, INT part_sum, INT total_sum, CString str)
{
EFS_BEGIN
	
	str.Replace(_T(","), _T(""));

	TCHAR* pEndPtr = NULL;	

	if (part_sum == 1 /*����*/) m_fPartSum[col] += _ttoi(str.GetBuffer(0));
	if (part_sum == 2 /*�Ǽ�*/) m_fPartSum[col] += _tcstod(str.GetBuffer(0),&pEndPtr);
	if (total_sum == 1 /*����*/) m_fTotalSum[col] += _ttoi(str.GetBuffer(0));
	if (total_sum == 2 /*�Ǽ�*/) m_fTotalSum[col] += _tcstod(str.GetBuffer(0),&pEndPtr);			

	return 0;

EFS_END
return -1;

}

INT CSIReportManager::CopyTableRow(CSIReportObject_Table * pTarget, INT nTargetRow, INT nRectBottom)
{
EFS_BEGIN

	if (nTargetRow < 0) return 0;

	CSIReportObject_Table_Cell *pCopyCell;
	CRect cell_rect;
	INT nCol = pTarget->col_count;
	for (INT col = 0; col < nCol; col++) {
		pCopyCell = pTarget->GetCell(nTargetRow, col);
		if (pCopyCell) {
			cell_rect = pCopyCell->GetRect();
			cell_rect.bottom = nRectBottom;
			pCopyCell->SetRect(cell_rect);	
		}
	}

	return 0;

EFS_END
return -1;

}

INT CSIReportManager::CopyTableRow(CSIReportObject_Table * pSource, INT nSourceRow, CSIReportObject_Table * pTarget, INT nTargetRow, INT nRectTop, CString strArray[])
{
EFS_BEGIN

	INT nCol = pSource->col_count;
	CSIReportObject_Table_Cell *pCell;
	CSIReportObject_Table_Cell *pCopyCell;
	CRect cell_rect;

	TCHAR szBuf[64];
	INT nSize, nPos, i;
	CString strResult = _T("");
	for (INT col = 0; col < nCol; col++) {
		pCell = pSource->GetCell(nSourceRow, col);
		cell_rect = pCell->GetRect();
		cell_rect.top = nRectTop;
		cell_rect.bottom = nRectTop + pSource->GetRect().Height();
		pTarget->SetCellData(nTargetRow, col, pCell);
		pCopyCell = pTarget->GetCell(nTargetRow, col);
		pCopyCell->text.font.lfHeight = CalcNegativeFontHeight(pCopyCell->text.size);
		pCopyCell->SetRect(cell_rect);

		strResult = strArray[col];
		if (strArray[col].Find('.') == -1) {
			nSize = strArray[col].GetLength();		
			nPos = 0;
			for (i = 0; i < nSize; i++) {
				if (i > 0 && ((nSize-i) % 3) == 0) {
					szBuf[nPos++] = ',';
				}
				szBuf[nPos++] = strArray[col].GetAt(i);
			}
		}
		szBuf[nPos] = '\0';
		if (col > 0) strResult.Format(szBuf);
		strResult.TrimLeft();		strResult.TrimRight();
		pCopyCell->text.text = strResult;
	}
	return 0;

EFS_END
return -1;

}

INT CSIReportManager::CalcNegativeFontHeight(INT size)
{
EFS_BEGIN

	INT nTmp;
	nTmp = MulDiv(size, 96, 72);
	nTmp = (nTmp > 0) ? -nTmp : nTmp;	

	return nTmp;

EFS_END
return -1;

}

INT CSIReportManager::AddCopyTextWithFont(CString str, CRect rect, LOGFONT logfont, INT vert_align, INT horz_align)
{
EFS_BEGIN

	CSIReportWorkSheet *ptmpWorkSheet = m_pPageWorkForm->GetWorkSheet(0);
	CSIReportObject_Text_Box *ptmpObject = (CSIReportObject_Text_Box *)ptmpWorkSheet->AddNewObject(OBJECT_TEXT_BOX);
	ptmpObject->SetName(str);
	ptmpObject->char_len = 100;
	ptmpObject->char_max = 100;
	ptmpObject->SetRect(rect);
	ptmpObject->text.color = RGB(0, 0, 0);
	ptmpObject->text.font = logfont;
	ptmpObject->text.size = logfont.lfHeight;
	ptmpObject->text.font.lfHeight = CalcNegativeFontHeight(ptmpObject->text.size);
	ptmpObject->text.style = 0;		// ���� �ǹ� ����
	ptmpObject->text.text = str;
	ptmpObject->horz_line = false;
	ptmpObject->vert_line = false;
	ptmpObject->vert_align = vert_align;
	ptmpObject->horz_align = horz_align;
	
	return 0;

EFS_END
return -1;

}

INT CSIReportManager::DrawObject_Table_TotalLine(INT mode, CDC *pDC, CSIReportObject_Table * pTable, INT nRectTop, INT nCol, INT nIsLast, CString strTotal[], INT nBottom)
{
EFS_BEGIN

	INT col;				// Column index variable
	CSIReportObject_Table_Cell *pCell;
	CRect cell_rect;
	CRect real_cell_rect;
	CString strTmp;

	for (col = 0; col < nCol; col++) {
		pCell = pTable->GetCell(0, col);
		if (!pCell) continue;
		if (pCell->bind_status == 2) continue;
		cell_rect = pCell->GetRect();
		cell_rect.top = nRectTop;
		if (nBottom < 0) 
		{
			cell_rect.bottom = nRectTop + pTable->GetRect().Height();
		}
		else 
		{
			cell_rect.bottom = nBottom;
		}
		CRect tmpRect = cell_rect;
		CalcRectOffSet(tmpRect, pCell->indent_len, pCell->indent_len, -pCell->indent_len, -pCell->indent_len);					
		real_cell_rect = ConvertRectMMToPixel(mode, cell_rect);
		if (!nIsLast)				// ������ �������� �ƴҶ�
		{
			DrawBox(pDC, real_cell_rect.left, real_cell_rect.top, real_cell_rect.right, real_cell_rect.bottom, false, true, true, false);
		} 
		else
		{
			DrawBox(pDC, real_cell_rect.left, real_cell_rect.top, real_cell_rect.right, real_cell_rect.bottom, false, true, true, true);
		}
		real_cell_rect = ConvertRectMMToPixel(mode, tmpRect);
		strTmp = pCell->text.text;
		pCell->text.text = strTotal[col];
		DrawTextToCell(mode, pDC, pCell, real_cell_rect);
		pCell->text.text = strTmp;
	}

	return 0;

EFS_END
return -1;

}

// 16/08/04 ������ : ���δ�������� ������½� �����Ͱ� �߷����̴� ���� ����
/*// BEFORE ------------------------------------------------------------------------
BOOL CSIReportManager::IsCanCut(CString sStr, INT nPos)
{
EFS_BEGIN

	INT i, nLastPos = -1;
	//���ڼ� �ʰ��� FALSE
	if(nPos > sStr.GetLength())
		return FALSE;
	TCHAR ch = sStr.GetAt(nPos);
	//������ ���� || �ѱ��� �ƴϸ� TRUE
	if(nPos + 1 == sStr.GetLength() || (ch & (0x80)) != 128)
		return TRUE;
	//ù���ڰ� 1byte���ڸ� TRUE �ƴϸ� FALSE
	if(nPos == 0)
		if((ch & (0x80)) != 128)
			return TRUE;
		else
			return FALSE;

	for(i = 0; i < nPos; i++) {
		ch = sStr.GetAt(i);
		//�ڸ��� �κп��� ���� �����(��������)������ ��ġ
		if((ch & (0x80)) != 128)
			nLastPos = i;
	}
	if (nLastPos == -1 && nPos % 2 == 1) return TRUE;
	if (nLastPos == -1 && nPos % 2 == 0) return FALSE;
	//¦���� ���̳��� TRUE
	BOOL result = !((nPos - nLastPos) % 2);
	return result;

EFS_END
return FALSE;
}
*/// AFTER -------------------------------------------------------------------------
BOOL CSIReportManager::IsCanCut(CHAR *str, INT pos)
{
	INT i, nLastPos = -1;
	//���ڼ� �ʰ��� FALSE
	if( (INT)strlen(str) <= pos )
		return TRUE;
	
	TCHAR ch = str[pos];
	
	//������ ���� || �ѱ��� �ƴϸ� TRUE
	if( pos + 1 == (INT)strlen(str) || ( ch & ( 0x80 ) ) != 128 )
		return true;
	
	//ù���ڰ� 1byte���ڸ� TRUE �ƴϸ� FALSE
	if( !pos )
		if( ( ch & ( 0x80 ) ) != 128 )
			return true;
		else
			return false;

	for( i = 0; i < pos; i++ )
	{
		ch = str[i];
		
		//�ڸ��� �κп��� ���� �����(��������)������ ��ġ
		if( ( ch & ( 0x80 ) ) != 128 )
			nLastPos = i;
	}
	
	if( nLastPos == -1 && pos % 2 == 1 )
		return true;
	
	if( nLastPos == -1 && pos % 2 == 0 )
		return true;
	
	//¦���� ���̳��� TRUE
	BOOL result = !( ( pos - nLastPos ) % 2 );
	return result;
}
//*/ END ----------------------------------------------------------------------------------

INT CSIReportManager::SetTableCellText(CString tbl_name, INT rowidx, INT colidx, CString str, INT vert_align, INT horz_align)
{
EFS_BEGIN

	CSIReportWorkSheet * pSheet = NULL;
	POSITION pos = m_pWorkForm->WorkSheetList.FindIndex(m_nCurrentWorkSheet);
	pSheet = (CSIReportWorkSheet*)m_pWorkForm->WorkSheetList.GetAt(pos);
	if (!pSheet) return -1;

	CSIReportObject_Table *pTable = FindTable(pSheet, tbl_name);
	if (!pTable) return -2;

	CSIReportObject_Table_Cell *pCell = pTable->GetCell(rowidx, colidx);
	if (!pCell) return -3;

	UINT nFormat = DT_NOPREFIX|DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	if (vert_align == ALIGN_TOP)		nFormat = DT_SINGLELINE|DT_TOP;
	if (vert_align == ALIGN_VCENTER)	nFormat = DT_SINGLELINE|DT_VCENTER;
	if (vert_align == ALIGN_BOTTOM)		nFormat = DT_SINGLELINE|DT_BOTTOM;

	if (horz_align == ALIGN_LEFT)		nFormat = nFormat|DT_LEFT;
	if (horz_align == ALIGN_HCENTER)	nFormat = nFormat|DT_CENTER;
	if (horz_align == ALIGN_RIGHT)		nFormat = nFormat|DT_RIGHT;

	CRect real_cell_rect;
	real_cell_rect = ConvertRectMMToPixel(m_EventInfo.mode, pCell->GetRect());
	pCell->text.font.lfHeight = pCell->text.size;
	DrawTextWithFont(m_EventInfo.mode, m_EventInfo.pDC, str, real_cell_rect, pCell->text.font, nFormat);    

// (START) - TEST
	if (m_pPageWorkForm) {
		CSIReportWorkSheet *ptmpWorkSheet = m_pPageWorkForm->GetWorkSheet(0);
		CSIReportObject_Text_Box *ptmpObject = (CSIReportObject_Text_Box *)ptmpWorkSheet->AddNewObject(OBJECT_TEXT_BOX);
		ptmpObject->char_len = 100;
		ptmpObject->char_max = 100;
		ptmpObject->SetRect(pCell->GetRect());
		ptmpObject->text.color = pCell->text.color;
		ptmpObject->text.font = pCell->text.font;
		ptmpObject->text.size = pCell->text.size;
		ptmpObject->text.font.lfHeight = CalcNegativeFontHeight(ptmpObject->text.size);
		ptmpObject->text.style = pCell->text.style;
		ptmpObject->text.text = str;
		ptmpObject->horz_line = false;
		ptmpObject->vert_line = false;
		ptmpObject->vert_align = vert_align;
		ptmpObject->horz_align = horz_align;
	}
// ( END ) - TEST

	return 0;

EFS_END
return -1;

}

INT CSIReportManager::GetWorkFormPaper(VOID)
{
EFS_BEGIN

	if (m_pWorkForm) return m_pWorkForm->paper;
		else return -1;

EFS_END
return -1;

}

INT CSIReportManager::CalcRectOffSet(CRect &rect, INT left, INT top, INT right, INT bottom)
{
EFS_BEGIN

	rect.left += left;
	rect.top += top;
	rect.right += right;
	rect.bottom += bottom;
	return 0;

EFS_END
return -1;

}

CRect CSIReportManager::ConvertRectMMToPixel(INT mode, CRect rect)
{
	CRect result;
	result.left = DrawGetMMToPixel(mode, (double)rect.left);
	result.top = DrawGetMMToPixel(mode, (double)rect.top);
	result.right = DrawGetMMToPixel(mode, (double)rect.right);
	result.bottom = DrawGetMMToPixel(mode, (double)rect.bottom);
	return result;
}

INT CSIReportManager::DrawObject_Text_Box(INT mode, CDC *pDC, CSIReportObject_Text_Box * pTextBox)
{
EFS_BEGIN

	if (pTextBox == NULL) return -1;
	if (pTextBox->m_strName == _T("%BARCODE���ڵ�����%")) return 0;
	if (pTextBox->m_strName == _T("%BARCODE���ڵ尪%")) return 0;
	if (pTextBox->m_nOnlyShowFirstPage == TRUE && m_EventInfo.m_nPage != 1) return 0;

// (START) - XML Page Output - Copy Table
	CSIReportWorkSheet *ptmpWorkSheet = NULL;
	CSIReportObject_Text_Box *ptmpObject = NULL;
	if (m_pPageWorkForm) 
	{
		ptmpWorkSheet = m_pPageWorkForm->GetWorkSheet(0);
		ptmpObject = (CSIReportObject_Text_Box *)ptmpWorkSheet->AddNewObject(OBJECT_TEXT_BOX);
		pTextBox->Clone(ptmpObject);

		INT nTmp = -MulDiv(ptmpObject->text.size, 96, 72);
		nTmp = (nTmp > 0) ? -nTmp : nTmp;
		ptmpObject->text.font.lfHeight = nTmp;
	}
// ( END ) - XML Page Output - Copy Table

	INT nLines = 0;
	CString str, token;
	str = pTextBox->text.text;
	INT pos = 0;
	while(true) {
		if (str.GetLength() == 0) break;
		pos = str.Find('\n', 0);
		if (pos == -1) pos = str.GetLength();
		token = str.Left(pos);
		token.TrimLeft();				token.TrimRight();
		nLines++;
		str = str.Mid(pos);
		str.TrimLeft();			str.TrimRight();
	}
	
	pTextBox->text.font.lfHeight = pTextBox->text.size;
	CRect rect = pTextBox->GetRect();
	CalcRectOffSet(rect, pTextBox->indent_len, pTextBox->indent_len, -pTextBox->indent_len, -pTextBox->indent_len);
	CRect real_text_cell_rect = ConvertRectMMToPixel(mode, rect);
	CRect real_cell_rect = ConvertRectMMToPixel(mode, rect);
	INT nOffset_top = 3;			// Text Box top offset - offset�� 0�̸� ������ ���� ���ΰ� �۾��� ��������.
	INT nPixel_cy;
	if (mode == MANAGER_MODE_PRINT) {		// �μ���
		nPixel_cy = MulDiv(pTextBox->text.font.lfHeight, m_nResolution, 72);
	} else {				// Preview ���
		if (pDC) nPixel_cy = MulDiv(pTextBox->text.font.lfHeight, GetDeviceCaps(pDC->m_hDC, LOGPIXELSY), 72);
			else nPixel_cy = MulDiv(pTextBox->text.font.lfHeight, 96, 72);
	}
	INT char_cy = nPixel_cy;
	nPixel_cy = (nPixel_cy < 0) ? nPixel_cy * -1 : nPixel_cy;
	nPixel_cy *= nLines;
	if (pTextBox->vert_align == ALIGN_TOP)			nOffset_top = 3;
	if (pTextBox->vert_align == ALIGN_VCENTER)		nOffset_top = (real_text_cell_rect.Height() - nPixel_cy) / 2;
	if (pTextBox->vert_align == ALIGN_BOTTOM)		nOffset_top = (real_text_cell_rect.Height() - nPixel_cy) - 3;
	
	real_text_cell_rect.top += nOffset_top;

	UINT nFormat = DT_NOPREFIX|DT_VCENTER|DT_LEFT|DT_SINGLELINE;
	if (pTextBox->horz_align == ALIGN_LEFT)		nFormat = DT_VCENTER|DT_SINGLELINE|DT_LEFT;
	if (pTextBox->horz_align == ALIGN_HCENTER)		nFormat = DT_VCENTER|DT_SINGLELINE|DT_CENTER;
	if (pTextBox->horz_align == ALIGN_RIGHT)		nFormat = DT_VCENTER|DT_SINGLELINE|DT_RIGHT;

	if (ptmpObject) {
		str = GetKeywordString(pTextBox->text.text);
		ptmpObject->text.text = str;
	} else {
		str = pTextBox->text.text;
	}
	COLORREF cWHITE = RGB(255, 255, 255);
	if (pDC) {
		CBrush bk_brush;
		bk_brush.CreateSolidBrush(pTextBox->fill_color);
		pDC->FillRect(real_cell_rect, &bk_brush);
		pDC->SetBkColor(pTextBox->fill_color);
		pDC->SetBkMode(TRANSPARENT);
	}
	DrawTextWithFont3(mode, pDC, str, real_text_cell_rect, pTextBox->text.font, nFormat);
	DrawBox(pDC, real_cell_rect.left, real_cell_rect.top, real_cell_rect.right, real_cell_rect.bottom, pTextBox->vert_line, pTextBox->horz_line, pTextBox->vert_line, pTextBox->horz_line);
	if (pDC) pDC->SetBkColor(cWHITE);
	return 0;
EFS_END
return -1;

}

INT CSIReportManager::DrawObject_Line(INT mode, CDC *pDC, CSIReportObject_Line * pLine)
{
EFS_BEGIN

	if (!pLine) return -2;

	// (START) - XML Page Output - Copy Line
	if (m_pPageWorkForm) {
		CSIReportWorkSheet *ptmpWorkSheet = m_pPageWorkForm->GetWorkSheet(0);
		CSIReportObject_Line *ptmpObject = (CSIReportObject_Line *)ptmpWorkSheet->AddNewObject(OBJECT_LINE);
		pLine->Clone(ptmpObject);
	}
	// ( END ) - XML Page Output - Copy Line
	if (!pDC) return -1;

	CRect rect = ConvertRectMMToPixel(mode, pLine->GetRect());
	
    CPen   pen;
	// ��Ÿ��(0:SOLID, 1:Dash, 2:Dot, 3:DashDot, 4:DashDotDot)
	INT style = PS_SOLID;
	if (pLine->style == 0) style = PS_SOLID;
	if (pLine->style == 1) style = PS_DASH;
	if (pLine->style == 2) style = PS_DOT;
	if (pLine->style == 3) style = PS_DASHDOT;
	if (pLine->style == 4) style = PS_DASHDOTDOT;

	pen.CreatePen(style, pLine->thick, RGB(0, 0, 0));
    CPen*   pOldPen   = (CPen*)   pDC->SelectObject(&pen);

    pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.right, rect.bottom);

    pDC->SelectObject(pOldPen);
    pen.DeleteObject();

	return 0;

EFS_END
return -1;

}

INT CSIReportManager::DrawObject_Barcode(INT mode, CDC *pDC, CSIReportObject_Barcode * pBarcode)
{
EFS_BEGIN

	if (!pBarcode) return -2;
	if (pBarcode->m_strName == _T("%BARCODE���ڵ�%")) return 0;

	// (START) - XML Page Output - Copy Barcode
	if (m_pPageWorkForm) {
		CSIReportWorkSheet *ptmpWorkSheet = m_pPageWorkForm->GetWorkSheet(0);
		CSIReportObject_Barcode *ptmpObject = (CSIReportObject_Barcode *)ptmpWorkSheet->AddNewObject(OBJECT_BARCODE);
		pBarcode->Clone(ptmpObject);
	}
	// ( END ) - XML Page Output - Copy Barcode
	if (!pDC) return -1;

	// ���ڵ忡 ���� ������� ���ڵ带 ���� �ʴ´�.
	// 2003.10.8 �����ɾ��� ��û�ؼ� �����մϴ�.
	if (pBarcode->m_strCaption.GetLength() == 0) return 0;

	CRect rect = ConvertRectMMToPixel(mode, pBarcode->GetRect());
	pBarcode->SetStyle(pBarcode->m_nStyle);
	pBarcode->SetCaption(pBarcode->m_strCaption);
	pBarcode->Draw(pDC, rect);
	return 0;

EFS_END
return -1;

}

INT CSIReportManager::DrawObject_Bitmap(INT mode, CDC *pDC, CSIReportObject_Bitmap * pBitmap)
{
EFS_BEGIN

	if (!pBitmap) return 0;

	// (START) - XML Page Output - Copy Bitmap
	if (m_pPageWorkForm) {
		CSIReportWorkSheet *ptmpWorkSheet = m_pPageWorkForm->GetWorkSheet(0);
		CSIReportObject_Bitmap *ptmpObject = (CSIReportObject_Bitmap *)ptmpWorkSheet->AddNewObject(OBJECT_BITMAP);
		pBitmap->Clone(ptmpObject);
	}
	// ( END ) - XML Page Output - Copy Bitmap
	if (!pDC) return 0;

	CRect real_rect;
	if (mode == MANAGER_MODE_PRINT) {		// �μ����϶� ������ ���
		real_rect = ConvertRectMMToPixel(mode, pBitmap->GetRect());
	} else {								// �̸����� ����϶��� ������ ���
		real_rect = ConvertRectMMToPixel(mode, pBitmap->GetRect());
	}
	
	if (pBitmap->LoadBitmap() >= 0) {
		pBitmap->m_pBitmap->DrawDIB2(pDC, real_rect.left, real_rect.top, real_rect.Width(), real_rect.Height());
	}

	return 0;

EFS_END
return -1;

}

INT CSIReportManager::DrawGetchar_cy(INT mode, CDC *pDC, INT nFontHeight, INT flag)
{
EFS_BEGIN

	INT result = 0;
	if (mode == MANAGER_MODE_PRINT) 
	{		// �μ���
		if (flag == (INT)true) result = MulDiv(nFontHeight, m_nResolution, 72);
			else result = MulDiv(nFontHeight, 96, 72);
	} 
	else 
	{				// Preview ���
		if (pDC) result = MulDiv(nFontHeight, GetDeviceCaps(pDC->m_hDC, LOGPIXELSY), 72);
			else result = MulDiv(nFontHeight, 96, 72);
	}
	return result;

EFS_END
return -1;

}

INT CSIReportManager::DrawGetHorzFormat(INT horz_align)
{
EFS_BEGIN

	UINT nFormat = DT_VCENTER|DT_LEFT|DT_SINGLELINE;
	if (horz_align == ALIGN_LEFT)		nFormat = DT_VCENTER|DT_SINGLELINE|DT_LEFT;
	if (horz_align == ALIGN_HCENTER)		nFormat = DT_VCENTER|DT_SINGLELINE|DT_CENTER;
	if (horz_align == ALIGN_RIGHT)		nFormat = DT_VCENTER|DT_SINGLELINE|DT_RIGHT;	
	
	return nFormat;

EFS_END
return -1;

}

INT CSIReportManager::InitArray(double array[], INT nColumn)
{
EFS_BEGIN

	for (INT i = 0; i < nColumn; i++) {
		array[i] = 0.0;
	}
	return 0;

EFS_END
return -1;

}

INT CSIReportManager::SetBarcodeCaption(CString obj_name, CString strCaption)
{
EFS_BEGIN

	CSIReportWorkSheet * pSheet = NULL;
	POSITION pos = m_pWorkForm->WorkSheetList.FindIndex(m_nCurrentWorkSheet);
	pSheet = (CSIReportWorkSheet*)m_pWorkForm->WorkSheetList.GetAt(pos);
	if (!pSheet) return -1;

	CSIReportObject_Barcode *pBarcode = FindBarcode(pSheet, obj_name);
	if (!pBarcode) return -2;

	pBarcode->m_strCaption = strCaption;
	return 0;

EFS_END
return -1;

}

CSIReportObject_Barcode * CSIReportManager::FindBarcode(CSIReportWorkSheet *pSheet, CString obj_name)
{
EFS_BEGIN

	INT count = pSheet->ObjList.GetCount();
	if (count == 0) return NULL;

	CSIReportObject * pObject;
	POSITION pos;
	INT i;
	pos = pSheet->ObjList.GetHeadPosition();
	for(i = 0; i < count; i++) {
		pObject = NULL;
		pObject = (CSIReportObject*)pSheet->ObjList.GetNext(pos);
		if (!pObject) continue;
		if (pObject->GetObjType() == OBJECT_BARCODE) {
			CSIReportObject_Barcode *pBarcode = (CSIReportObject_Barcode *)pObject;
			if (!pBarcode) continue;
			if (pBarcode->m_strName == obj_name) return pBarcode;
		}
	}
	return NULL;

EFS_END
return NULL;

}

INT CSIReportManager::MakeXMLPageFiles(VOID)
{
EFS_BEGIN

	// ������ ���ϵ��� ��� �����.
	CString strTmp;
	CFile fd;
	INT i;
	for (i = 1; i < 99999; i++) {
		strTmp.Format(_T("rpt_tmp\\%s%04d.rpt"), m_strPageHeader, i);
		if (fd.Open(strTmp, CFile::modeRead | CFile::typeBinary) == false) break;
		fd.Close();
		DeleteFile(strTmp);

	}
	for (i = 1; i < 99999; i++) {
		strTmp.Format(_T("rpt_tmp\\%s%04d.xml"), m_strPageHeader, i);
		if (fd.Open(strTmp, CFile::modeRead | CFile::typeBinary) == false) break;
		fd.Close();
		DeleteFile(strTmp);

	}
	MAKEPAGE_DrawWorkForm();
	m_EventInfo.m_nTotalPage = GetTmpPageNum();
	return 0;

EFS_END
return -1;

}

INT CSIReportManager::DeleteTempPageFiles(VOID)
{
EFS_BEGIN

	// ������ ���ϵ��� ��� �����.
	CString strTmp;
	CFile fd;
	INT i;
	for (i = 1; i < 99999; i++) {
		strTmp.Format(_T("rpt_tmp\\%s%04d.rpt"), m_strPageHeader, i);
		if (fd.Open(strTmp, CFile::modeRead | CFile::typeBinary) == false) break;
		fd.Close();
		DeleteFile(strTmp);

	}
	for (i = 1; i < 99999; i++) {
		strTmp.Format(_T("rpt_tmp\\%s%04d.xml"), m_strPageHeader, i);
		if (fd.Open(strTmp, CFile::modeRead | CFile::typeBinary) == false) break;
		fd.Close();
		DeleteFile(strTmp);

	}
	return 0;

EFS_END
return -1;

}

INT CSIReportManager::GetTmpPageNum(VOID)
{
EFS_BEGIN

	CFile fd;
	INT nPages = 0;
	CString strTmp;
	for (INT i = 1; i < 99999; i++) {
		strTmp.Format(_T("rpt_tmp\\%s%04d.xml"), m_strPageHeader, i);
		if (fd.Open(strTmp, CFile::modeRead | CFile::typeBinary) == false) break;
		fd.Close();
		nPages = i;
	}
	return nPages;

EFS_END
return -1;

}

INT CSIReportManager::ShowXMLFilePage(INT mode, CDC *pDC, INT nStartPage, INT nFinishPage)
{
EFS_BEGIN

	if (mode == MANAGER_MODE_PRINT) {

	} else {
		if (m_nCurrentPreviewPage != nStartPage) {
			LoadPageXMLWorkForm(nStartPage);
		}
		PREVIEW_DrawWorkForm(pDC,m_nCurrentPreviewPage, m_nCurrentPreviewPage);
	}
	return 0;

EFS_END
return -1;

}


INT CSIReportManager::LoadPageXMLWorkForm(INT nPage)
{
EFS_BEGIN

	CString strTmp;
	strTmp.Format(_T("rpt_tmp\\%s%04d.xml"), m_strPageHeader, nPage);
	CFile fd;
	if (fd.Open(strTmp, CFile::modeRead | CFile::typeBinary) == false) {
		AfxMessageBox(_T("������ ������ ã�� �� �����ϴ�.!"));
		return -1;
	}
	fd.Close();
	LoadWorkForm(strTmp);
	m_nCurrentPreviewPage = nPage;
	return 0;

EFS_END
return -1;

}

INT CSIReportManager::SetCONNECTION_INFO(CString strCONNECTION_INFO)
{
EFS_BEGIN

	m_strCONNECTION_INFO = strCONNECTION_INFO;
	return 0;

EFS_END
return -1;

}

INT CSIReportManager::GetSIReportFileFromDB(CString RptAlias)
{
EFS_BEGIN

	//++2008.10.10 UPDATE BY PWR {{++
	//EXCEL����� �����ϸ� EXCEL�̸������ �۵��Ѵ�
	CString sStyleDirectory = _T("..\\CFG\\RptStyle");	//���������ġ
	m_sStyleFile = sStyleDirectory + _T("\\") + RptAlias + _T(".xls");	//�������
	CString tmp_filename = _T("rpt_tmp\\tmp_SIReportManager.rpt");
	CString PK;
	CFileFind fFind;
	if( !fFind.FindFile(m_sStyleFile) )
	{		
		INT ids = GetSIReportFileFromDB(RptAlias, tmp_filename, PK);
		if (ids >= 0) InitWorkFormFile(tmp_filename, RptAlias, PK);
		return ids;
	}
	else
	{
		InitWorkFormFile(tmp_filename, RptAlias, PK, TRUE);
		return 0;
	}
	//CString tmp_filename = _T("rpt_tmp\\tmp_SIReportManager.rpt");
	//CString PK;
	//INT ids = GetSIReportFileFromDB(RptAlias, tmp_filename, PK);
	//if (ids >= 0) InitWorkFormFile(tmp_filename, RptAlias, PK);
	//return ids;
	//--2008.10.10 UPDATE BY PWR --}}

EFS_END
return -1;

}

INT CSIReportManager::GetSIReportFileFromDB(CString RptName, CString tmpFilename, CString &PK)
{
EFS_BEGIN

	DeleteTempBitmapFile(MASTER_IS_MANAGER);
	CESL_DataMgr *pDM = new CESL_DataMgr;
	if (!pDM) return -1000;
	pDM->SetCONNECTION_INFO(m_strCONNECTION_INFO);

	INT i, ids;
	CString szBuf, sQuery;
	CString strPK = _T(""), strRPTSize;

	sQuery.Format(_T("SELECT PK, RPTSIZE, FLAG, BITMAP_ALIAS FROM ESL_SIREPORT_TBL WHERE RPTALIAS='%s'"), RptName);
	pDM->RestructDataManager(sQuery);
	INT nRow = pDM->GetRowCount();

	// ����� ���� ������ �ִ��� ã�ƺ���. ������ �̰��� ���� ���ĺ��� �켱�Ѵ�.
	for (i = 0; i < nRow; i++) {
		pDM->GetCellData(i, 2, szBuf);
		szBuf.TrimLeft();			szBuf.TrimRight();
		szBuf.MakeUpper();
		if (szBuf == _T("U")) {
			pDM->GetCellData(i, 0, strPK);
			strPK.TrimLeft();		strPK.TrimRight();
			pDM->GetCellData(i, 1, strRPTSize);
			strRPTSize.TrimLeft();	strRPTSize.TrimRight();
			if (strRPTSize.GetLength() == 0) strRPTSize = _T("0");
			break;
		}
	}
	// ����� ���� ������ �������� ���� ������ ã�´�.
	if (strPK.GetLength() == 0) {
		for (i = 0; i < nRow; i++) {
			pDM->GetCellData(i, 2, szBuf);
			szBuf.TrimLeft();			szBuf.TrimRight();
			szBuf.MakeUpper();
			if (szBuf == _T("X")) {
				pDM->GetCellData(i, 0, strPK);
				strPK.TrimLeft();		strPK.TrimRight();
				pDM->GetCellData(i, 1, strRPTSize);
				strRPTSize.TrimLeft();	strRPTSize.TrimRight();
				if (strRPTSize.GetLength() == 0) strRPTSize = _T("0");
				break;
			}
		}
	}
	if (strPK.GetLength() == 0) {
		if (pDM) {
			delete pDM;
			pDM = NULL;
		}
		return -1022;		// �ش� Report �� �����ϴ�.
	}
	PK = strPK;

	// Report Data File�� �����´�.
	ids = GetLOBToFile(pDM, _T("ESL_SIREPORT_TBL"), _T("PK"), _T("NUMERIC"), strPK, _T("RPTDATA"), _ttoi(strRPTSize), tmpFilename);
	if (ids < 0) {
		if (pDM) {
			delete pDM;
			pDM = NULL;
		}
		return -1025;		// Lob�� File�� �ٿ� �޴µ� ����
	}

	CString strTmp, strALIAS;
	for (i = 0; i < nRow; i++) {
		pDM->GetCellData(i, 2, szBuf);
		szBuf.TrimLeft();			szBuf.TrimRight();
		szBuf.MakeUpper();
		if (szBuf == _T("B")) {
			pDM->GetCellData(i, 0, strPK);
			strPK.TrimLeft();		strPK.TrimRight();
			pDM->GetCellData(i, 1, strRPTSize);
			strRPTSize.TrimLeft();	strRPTSize.TrimRight();
			if (strRPTSize.GetLength() == 0) strRPTSize = _T("0");
			pDM->GetCellData(i, 3, strALIAS);
			strALIAS.TrimLeft();	strALIAS.TrimRight();
			if (strALIAS.GetLength() == 0) continue;
			strTmp.Format(_T("%s\\%s.bmp"), MANAGER_TEMP_DIR, strALIAS);
			ids = GetLOBToFile(pDM, _T("ESL_SIREPORT_TBL"), _T("PK"), _T("NUMERIC"), strPK, _T("RPTDATA"), _ttoi(strRPTSize), strTmp);
		}
	}
	if (pDM) {
		delete pDM;
		pDM = NULL;
	}
	
	return 0;

EFS_END
return -1;

}

INT CSIReportManager::GetLOBToFile(CESL_DataMgr *pDM, CString table_name, CString PKFieldname, CString PKFieldtype, CString PKData, CString FieldName, INT Size, CString FileName)
{
EFS_BEGIN

	BYTE *pBuffer;
	INT nBufferSize = Size + 1024;
	INT ids;

	pBuffer = (BYTE*)malloc(sizeof(BYTE) * nBufferSize);
	if (!pBuffer) {
		AfxMessageBox(_T("Malloc Memory Fail!"));
		return -1024;
	}
	
	ids = pDM->LOBGet(table_name, PKFieldname, PKFieldtype, PKData, FieldName, pBuffer, &nBufferSize);
	if (ids < 0) {
		AfxMessageBox(_T("Get Lob Data Fail!"));
		free(pBuffer);
		return -1024;
	}
	if (nBufferSize <= 0) {
		AfxMessageBox(_T("Empty Lob Data!"));
		return -1024;
	}
	CFile f;
    if( !f.Open(FileName, CFile::modeWrite|CFile::modeCreate | CFile::typeBinary) ) {
		free(pBuffer);
		AfxMessageBox(_T("Create Lob File Fail"));
		return - 1025;
	}
/*	
	if( 0 == f.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		f.Write( &cUni, sizeof(TCHAR));
	}
*/		
	f.WriteHuge(pBuffer, nBufferSize);
	f.Close();
	free (pBuffer);

	return 0;

EFS_END
return -1;

}

INT CSIReportManager::SetDataMgr(INT nPos, CESL_DataMgr *pDM, INT startcursor)
{
EFS_BEGIN

	if (nPos < 0) return -1;
	if (nPos >= MAX_DM) return -1;
	m_pDM[nPos] = pDM;
	m_DMCursor = startcursor;

	return 0;

EFS_END
return -1;

}

INT CSIReportManager::InitDMArray(VOID)
{
EFS_BEGIN

	for (INT i = 0; i < MAX_DM; i++) {
		m_pDM[i] = NULL;
	}
	return 0;

EFS_END
return -1;

}

CString CSIReportManager::SplitStringWithLen(CString str, INT char_len, INT char_max)
{
EFS_BEGIN

	//2019.08.09 JOB.2019.0072 EDIT BY KYJ : ������� �ܱ��� �ڷ� ������ ���� ����
	CString strTemp,strResult;
	INT nLen = str.GetLength();
	INT nPos = 0;

	if(nLen > char_max)
	{
		strTemp = str.Left(char_max);
		str = strTemp;
	}

	strTemp.Empty();
	strResult.Empty();

	strTemp = str;

	while(true)
	{
		if ( strTemp.GetLength() > char_len )
		{
			strResult += str.Mid(nPos, char_len) + _T("\r\n");
			nPos += char_len;
			strTemp = str.Mid(nPos);
		}
		else
		{
			if (strResult.IsEmpty() == TRUE)
			{
				strResult = strTemp;
			}
			else
			{
				strResult += strTemp;
			}
			break;
		}
	}

	return strResult;

	/*
	CString result, strTmp;
	if (str.Find(_T("�ѱ�")) !=-1) {
		INT uu = 0;
	}
	result.Empty();

	// 16/08/04 ������ : ���δ�������� ������½� �����Ͱ� �߷����̴� ���� ����
// BEFORE ------------------------------------------------------------------------
	if (str.GetLength() > char_max) {
		if (!IsCanCut(str, char_max-1)) {
			char_max++;
		}
		str = str.Left(char_max);
	}
// AFTER -------------------------------------------------------------------------
	CHAR szCutMsg[2048];
	CString sendMsg;
	CHAR *szMsg = NULL;
	TCHAR *szUnicodeMessage = NULL;
	INT	nLen = 0;
	INT	nUnicodeLen = 0;
	CharConvert cv;

	cv.UnicodeToMultiByte(str.GetBuffer(0), &szMsg, &nLen );	
	if (nLen > char_max) {
		if (!IsCanCut(szMsg, char_max-1)) {
			char_max++;
		}

		strncpy(szCutMsg , szMsg, char_max);
		szCutMsg[char_max] = '\0';
			
		if(0 < nLen) delete [] szMsg;

		cv.MultiByteToUnicode(szCutMsg, &szUnicodeMessage, &nUnicodeLen );
		sendMsg.Format(_T("%s"), szUnicodeMessage );

		if(0 < nUnicodeLen) delete [] szUnicodeMessage; 
		
		if(sendMsg.GetLength() <= char_max){
			str = str.Left(sendMsg.GetLength());
		}
	}
	else if(0 < nLen) {
		delete [] szMsg;
	}
// END ---------------------------------------------------------------------------

   //==============================================================================
   //2004-05-19 ���� 
	INT nReturnPos = 0;
	BOOL bContinue = FALSE;
	BOOL bStop = FALSE;
	while(TRUE)
	{
		bContinue = FALSE;
		CString sLine;

		nReturnPos = str.Find(_T("\r\n"));
		if( nReturnPos == 0 )
		{
			result += _T("\r\n");
			str = str.Mid(2);
			bContinue = TRUE;
		}
		else if( nReturnPos > 0 )
		{
			sLine = str.Mid( 0 , nReturnPos );
			str = str.Mid( nReturnPos+2 );
		}
		else
		{
			sLine = str;
			bStop = TRUE;
		}

		if( bContinue ) continue;

		CString sResult;
		INT nPos = 0;
		BOOL bBreak = FALSE;

		// 16/08/04 ������ : ���δ�������� ������½� �����Ͱ� �߷����̴� ���� ����
// BEFORE ------------------------------------------------------------------------
		while(TRUE)
		{
			if( sLine.GetLength() <= char_len )
			{
				sResult += sLine;
				bBreak = TRUE;
			}

			if( bBreak ) break;

			if( !IsCanCut( sLine , char_len-1 ) )
				nPos = char_len+1;
			else
				nPos = char_len;

			sResult += sLine.Mid(0,nPos);
			sResult += _T("\r\n");
			if( sLine.GetLength() <= nPos+1 ) break;
			sLine = sLine.Mid( nPos );
		}
// AFTER -------------------------------------------------------------------------
		while(TRUE)
		{
			cv.UnicodeToMultiByte(sLine.GetBuffer(0), &szMsg, &nLen );	
			if(NULL == szMsg) break;
			if( nLen <= char_len )
			{
				if(0 < nLen) delete [] szMsg; 

				sResult += sLine;
				bBreak = TRUE;
			}

			if( bBreak ) break;

			if( !IsCanCut( szMsg , char_len-1 ) )
				nPos = char_len+1;
			else
				nPos = char_len;

			strncpy(szCutMsg , szMsg, nPos);
			szCutMsg[nPos] = '\0';
			
			if(0 < nLen) delete [] szMsg;

			cv.MultiByteToUnicode(szCutMsg, &szUnicodeMessage, &nUnicodeLen );

			sendMsg.Format(_T("%s"), szUnicodeMessage );
			sResult += sendMsg;
			sResult += _T("\r\n");

			if( 0 < nUnicodeLen ) delete [] szUnicodeMessage; 
			if( sLine.GetLength() < sendMsg.GetLength() ) break;
			sLine = sLine.Mid( sendMsg.GetLength() );
		}
// END ---------------------------------------------------------------------------
		result += sResult;
		if( nReturnPos > 0 ) result += _T("\r\n");

		if( bStop ) break;

	}
	return result;


	CString token;
	INT pos = 0;
	while(true) {
		if (str.GetLength() == 0) break;
		pos = char_len;

		//=================================================
		//2004-04-01 [������]
		INT nTMP = str.Mid(0).Find (_T("\r\n")) ;
		if(nTMP == 0) 
		{
			result += _T("\r\n");
			if (pos == str.GetLength()) break;
			str = str.Mid(nTMP+1);
			continue ;
		}
        nTMP = str.Find(_T("\r\n"));
		if(nTMP < pos && nTMP != -1)
		{ 
			result += str.Mid (0,nTMP)+_T("\r\n");
            if (pos == str.GetLength()) break;
			str = str.Mid(nTMP+1);
			continue ;
		}

		//=================================================


		if (str.GetLength() > pos) {
			if (!IsCanCut(str, pos-1)) {
				pos++;
			}
		} else {
			pos = str.GetLength();
		}
		strTmp = str.Left(pos);
		token.Format(_T("%s\r\n"),strTmp);
		result += token;
		if (pos == str.GetLength()) break;
		str = str.Mid(pos);
		//2004-03-25 [������ ����]
		//str.TrimLeft();
		//str.TrimRight();
	}
	//2004-04-01 [������ ����]
	result.TrimRight();
	return result;

*/

EFS_END
return _T("");

}

INT CSIReportManager::DeleteTempBitmapFile(INT MODE) 
{
EFS_BEGIN

	CFileFind finder;
	CString strWildCard;
	BOOL bWorking;

	if (MODE == MASTER_IS_MANAGER) {
		strWildCard.Format(_T("%s\\*.bmp"), MANAGER_TEMP_DIR);
	} else {
		strWildCard.Format(_T("%s\\*.bmp"), EDITOR_TEMP_DIR);
	}

	CString strPath, strName;
	bWorking = finder.FindFile(strWildCard);	
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		strPath = finder.GetFilePath();
		strName = finder.GetFileName();

		DeleteFile(strPath);
	}
	
	return 0;

EFS_END
return -1;

}

INT CSIReportManager::GetSIReportFileFromDBbyPK(CString PK, CString RptAlias, CString RptSize)
{
EFS_BEGIN

	CString tmp_filename = _T("rpt_tmp\\tmp_SIReportManager.rpt");
	INT ids = GetSIReportFileFromDBbyPK(PK, RptAlias, tmp_filename, RptSize);
	if (ids >= 0) InitWorkFormFile(tmp_filename, RptAlias, PK);
	return ids;

EFS_END
return -1;

}

INT CSIReportManager::GetSIReportFileFromDBbyPK(CString PK, CString RptName, CString tmpFilename, CString RptSize)
{
EFS_BEGIN

	DeleteTempBitmapFile(MASTER_IS_MANAGER);
	CESL_DataMgr *pDM = new CESL_DataMgr;
	if (!pDM) return -1000;
	pDM->SetCONNECTION_INFO(m_strCONNECTION_INFO);

	INT i, ids;
	CString szBuf, sQuery;
	CString strPK = _T(""), strRPTSize;

	sQuery.Format(_T("SELECT PK, RPTSIZE, FLAG, BITMAP_ALIAS FROM ESL_SIREPORT_TBL WHERE RPTALIAS='%s'"), RptName);
	pDM->RestructDataManager(sQuery);
	INT nRow = pDM->GetRowCount();
	strPK = PK;
	strRPTSize = RptSize;

	// Report Data File�� �����´�.
	ids = GetLOBToFile(pDM, _T("ESL_SIREPORT_TBL"), _T("PK"), _T("NUMERIC"), strPK, _T("RPTDATA"), _ttoi(strRPTSize), tmpFilename);
	if (ids < 0) {
		if (pDM) {
			delete pDM;
			pDM = NULL;
		}
		return -1025;		// Lob�� File�� �ٿ� �޴µ� ����
	}

	CString strTmp, strALIAS;
	for (i = 0; i < nRow; i++) {
		pDM->GetCellData(i, 2, szBuf);
		szBuf.TrimLeft();			szBuf.TrimRight();
		szBuf.MakeUpper();
		if (szBuf == _T("B")) {
			pDM->GetCellData(i, 0, strPK);
			strPK.TrimLeft();		strPK.TrimRight();
			pDM->GetCellData(i, 1, strRPTSize);
			strRPTSize.TrimLeft();	strRPTSize.TrimRight();
			if (strRPTSize.GetLength() == 0) strRPTSize = _T("0");
			pDM->GetCellData(i, 3, strALIAS);
			strALIAS.TrimLeft();	strALIAS.TrimRight();
			if (strALIAS.GetLength() == 0) continue;
			strTmp.Format(_T("%s\\%s.bmp"), MANAGER_TEMP_DIR, strALIAS);
			ids = GetLOBToFile(pDM, _T("ESL_SIREPORT_TBL"), _T("PK"), _T("NUMERIC"), strPK, _T("RPTDATA"), _ttoi(strRPTSize), strTmp);
		}
	}
	if (pDM) {
		delete pDM;
		pDM = NULL;
	}
	
	return 0;

EFS_END
return -1;

}

CString CSIReportManager::GetKeywordString(CString str)
{
EFS_BEGIN
	if (str.Find('%', 0) == -1) {
		return str;
	}

	CTime t = CTime::GetCurrentTime();
	str.TrimLeft();			str.TrimRight();
	//str.MakeUpper();

	CString strTmp;
	INT pos1, pos2;
	CString strGETDM, strDMAlias, strDMFieldAlias, strDMRow;
	if (str.Find(_T("%DATE_TODAY%")) != -1) {
		strTmp.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
		str.Replace(_T("%DATE_TODAY%"), strTmp);
	}
	if (str.Find(_T("%DATE_YEAR%")) != -1) {
		strTmp.Format(_T("%d"), t.GetYear());
		str.Replace(_T("%DATE_YEAR%"), strTmp);
	}
	if (str.Find(_T("%DATE_MONTH%")) != -1) {
		strTmp.Format(_T("%d"), t.GetMonth());
		str.Replace(_T("%DATE_MONTH%"), strTmp);
	}
	if (str.Find(_T("%DATE_DAY%")) != -1) {
		strTmp.Format(_T("%d"), t.GetDay());
		str.Replace(_T("%DATE_DAY%"), strTmp);
	}
	if (str.Find(_T("%PAGE_NUM%")) != -1) {
		strTmp.Format(_T("%d"), m_EventInfo.m_nPage);
		str.Replace(_T("%PAGE_NUM%"), strTmp);
	}
	if (str.Find(_T("%TOTAL_PAGES%")) != -1) {
		strTmp.Format(_T("%d"), m_EventInfo.m_nTotalPage);
		str.Replace(_T("%TOTAL_PAGES%"), strTmp);
	}
	if (str.Find(_T("%GETDMDATA(")) != -1) {
		pos1 = str.Find(_T("%GETDMDATA("));
		pos2 = str.Find(_T(")%"), pos1);
		strGETDM = str.Mid(pos1, pos2 - pos1 + 2);
		if (pos1 != -1 && pos2 != -1) {
			strTmp = str.Mid(pos1 + 11, pos2 - pos1 - 11);
			pos1 = strTmp.Find(_T("."));
			strDMAlias = strTmp.Mid(0, pos1);
			pos2 = strTmp.Find(_T("["));
			strDMFieldAlias = strTmp.Mid(pos1+1, pos2-pos1-1);
			pos1 = pos2;
			pos2 = strTmp.Find(_T("]"));
			strDMRow = strTmp.Mid(pos1+1, pos2-pos1-1);
			GetKeywordString_DMData(strDMAlias, strDMFieldAlias, _ttoi(strDMRow), strTmp);
			str.Replace(strGETDM, strTmp);
		}
	}
	if (str.Find(_T("%GETDEFAULTDMDATA(")) != -1) {
		pos1 = str.Find(_T("%GETDEFAULTDMDATA("));
		pos2 = str.Find(_T(")%"), pos1);
		strGETDM = str.Mid(pos1, pos2 - pos1 + 2);
		if (pos1 != -1 && pos2 != -1) {
			pos2 = str.Find(_T(")%"));
			strDMFieldAlias = str.Mid(pos1 + 18, pos2 - pos1 - 18);
			strTmp = _T("");
			if (m_pDefaultDM) m_pDefaultDM->GetCellData(strDMFieldAlias, 0, strTmp);
			str.Replace(strGETDM, strTmp);
		}
	}
	if (str.Find(_T("%GETTEMPVARIABLE(")) != -1) {
		pos1 = str.Find(_T("%GETTEMPVARIABLE("));
		pos2 = str.Find(_T(")%"), pos1);
		strGETDM = str.Mid(pos1, pos2 - pos1 + 2);
		if (pos1 != -1 && pos2 != -1) {
			pos2 = str.Find(_T(")%"));
			strDMFieldAlias.Format(_T("%s"), str.Mid(pos1 + 17, pos2 - pos1 - 17));
			strDMFieldAlias.TrimLeft();		strDMFieldAlias.TrimRight();
			strTmp = _T("");
			GetTempVariable(strDMFieldAlias, strTmp);
			str.Replace(strGETDM, strTmp);
		}
	}
	return str;

EFS_END
return _T("");

}

INT CSIReportManager::GetKeywordString_DMData(CString strDMAlias, CString strDMFieldAlias, INT DMRow, CString &strResult)
{
EFS_BEGIN

	strResult = _T("");
	INT ids;
	if (strDMAlias.GetLength() == 0) {
		if (m_pDefaultDM) ids= m_pDefaultDM->GetCellData(strDMFieldAlias, DMRow, strResult);
	} else {
		if (m_pDefaultDM) ids= m_pDefaultDM->GetCellData(strDMFieldAlias, DMRow, strResult);
	}

	return ids;

EFS_END
return -1;

}

INT CSIReportManager::DM_GetCellData(CString strDMs, INT nRow, CString &strResult)
{
EFS_BEGIN

	INT pos1, pos2;
	INT ids;
	CString strDM;
	CString strCellResult;
	CString strDMAlias, strDMFieldAlias;

	strResult = strDMs;
	if (strDMs.Find(_T(".")) != -1) {	// Default DM �� �ƴѰ��
		while(TRUE) {
			if (strResult.Find(_T("%")) == -1) break;
			pos1 = strResult.Find(_T("%"), 0);
			pos2 = strResult.Find(_T("%"), pos1+1);
			if (pos1 == -1 || pos2 == -1) break;
			strDM = strResult.Mid(pos1, pos2-pos1+1);
			strDMAlias = strDM.Mid(1, strDM.Find(_T("."))-1);
			strDMFieldAlias = strDM.Mid(strDM.Find(_T("."))+1, strDM.GetLength() - strDM.Find(_T("."))-2);

			ids = GetKeywordString_DMData(strDMAlias, strDMFieldAlias, nRow, strCellResult);
			strResult.Replace(strDM, strCellResult);
		}
	} else {	// Default DM�ΰ��
		while(TRUE) {
			if (strResult.Find(_T("%")) == -1) break;
			pos1 = strResult.Find(_T("%"), 0);
			pos2 = strResult.Find(_T("%"), pos1+1);
			if (pos1 == -1 || pos2 == -1) break;
			strDMAlias = _T("");
			strDM = strResult.Mid(pos1, pos2-pos1+1);
			strDMFieldAlias = strResult.Mid(pos1+1, pos2-pos1-1);

			ids = GetKeywordString_DMData(strDMAlias, strDMFieldAlias, nRow, strCellResult);
			strResult.Replace(strDM, strCellResult);
		}
	}
	return 0;

EFS_END
return -1;

}

/*
Return value range : -51001 ~ -51999 
*/
INT CSIReportManager::LOC_PrintBarcode(INT mode, CDC *pDC, CRect rect)
{
EFS_BEGIN

	m_bPrintTotal = TRUE;

	if (m_pDefaultDM) {
		if (m_pDefaultDM->GetRowCount() <= m_DMCursor) return 0;
	} else {
		return -1;
	}

	CString strTitleObjectName = _T("%BARCODE���ڵ�����%");
	CString strBarcodeObjectName = _T("%BARCODE���ڵ�%");
	CString strValueObjectName = _T("%BARCODE���ڵ尪%");

	CSIReportWorkSheet * pSheet = NULL;
	POSITION pos = m_pWorkForm->WorkSheetList.FindIndex(m_nCurrentWorkSheet);
	pSheet = (CSIReportWorkSheet*)m_pWorkForm->WorkSheetList.GetAt(pos);
	if (!pSheet) return -51100;

	CSIReportObject_Text_Box *pTitle = (CSIReportObject_Text_Box *)FindObject(pSheet, strTitleObjectName);
	CSIReportObject_Barcode *pBarcode = (CSIReportObject_Barcode *)FindObject(pSheet, strBarcodeObjectName);
	CSIReportObject_Text_Box *pValue = (CSIReportObject_Text_Box *)FindObject(pSheet, strValueObjectName);

	if (pTitle == NULL) return -51001;
	if (pBarcode == NULL) return -51002;
	if (pValue == NULL) return -51003;

	if (m_DMCursor < 0) m_DMCursor = 0;

	CRect rTitle_old = pTitle->GetRect();
	CRect rBarcode_old = pBarcode->GetRect();
	CRect rValue_old = pValue->GetRect();

	// CRect( INT l, INT t, INT r, INT b );
	CRect rTitle_new(rect.left, rect.top, rect.left + rTitle_old.Width(), rect.top + rTitle_old.Height());
	CRect rBarcode_new(rect.left, rTitle_new.bottom, rect.left + rBarcode_old.Width(), rTitle_new.bottom + rBarcode_old.Height());
	CRect rValue_new(rect.left, rBarcode_new.bottom, rect.left + rValue_old.Width(), rBarcode_new.bottom + rValue_old.Height());

	CString strTitleObjectNewName, strBarcodeObjectNewName, strValueObjectNewName;
	strTitleObjectNewName.Format(_T("���ڵ�����_%d"), m_DMCursor);
	strBarcodeObjectNewName.Format(_T("���ڵ�_%d"), m_DMCursor);
	strValueObjectNewName.Format(_T("���ڵ尪_%d"), m_DMCursor);

	pTitle->SetRect(rTitle_new);
	pTitle->SetName(strTitleObjectNewName);
	GetTempVariable(_T("%BARCODE���ڵ�����%"), pTitle->text.text);

	CString strBarcodeCaption, strBarcodeValue;
	pBarcode->GetCaption(strBarcodeCaption);
	pBarcode->SetRect(rBarcode_new);
	pBarcode->SetName(strBarcodeObjectNewName);
	if (m_pDefaultDM) strBarcodeValue = m_pDefaultDM->GetCellData(strBarcodeCaption, m_DMCursor);
	pBarcode->SetCaption(strBarcodeValue);

	pValue->SetRect(rValue_new);
	pValue->SetName(strValueObjectNewName);
	pValue->text.text = strBarcodeValue;

	
	DrawObject_Text_Box(mode, pDC, pTitle);
	DrawObject_Barcode(mode, pDC, pBarcode);
	DrawObject_Text_Box(mode, pDC, pValue);


	pTitle->SetRect(rTitle_old);
	pTitle->SetName(strTitleObjectName);

	pBarcode->SetRect(rBarcode_old);
	pBarcode->SetName(strBarcodeObjectName);
	pBarcode->SetCaption(strBarcodeCaption);

	pValue->SetRect(rValue_old);
	pValue->SetName(strValueObjectName);

	m_DMCursor++;
	return 0;

EFS_END
return -1;

}

INT CSIReportManager::SetTempVariable(CString strVariableName, CString strVariableValue)
{
EFS_BEGIN

	INT nTempVariables = m_arrTempVariableName.GetSize();
	for (INT i = 0; i < nTempVariables; i++) {
		if (m_arrTempVariableName.GetAt(i) == strVariableName) {
			m_arrTempVariableValue.SetAt(i, strVariableValue);
			return 0;
		}
	}
	m_arrTempVariableName.Add(strVariableName);
	m_arrTempVariableValue.Add(strVariableValue);
	return 0;

EFS_END
return -1;

}

INT CSIReportManager::GetTempVariable(CString strVariableName, CString &strVariableValue)
{
EFS_BEGIN

	INT nTempVariables = m_arrTempVariableName.GetSize();
	for (INT i = 0; i < nTempVariables; i++) {
		if (m_arrTempVariableName.GetAt(i) == strVariableName) {
			strVariableValue = m_arrTempVariableValue.GetAt(i);
			return 0;
		}
	}
	return -1;

EFS_END
return -1;

}

BOOL CSIReportManager::IsSheetHaveRepeatObject(VOID)
{
EFS_BEGIN

	CSIReportWorkSheet * pSheet = NULL;
	POSITION pos = m_pWorkForm->WorkSheetList.FindIndex(m_nCurrentWorkSheet);
	pSheet = (CSIReportWorkSheet*)m_pWorkForm->WorkSheetList.GetAt(pos);
	if (!pSheet) return FALSE;

	INT count = pSheet->ObjList.GetCount();
	if (count == 0) return FALSE;

	CSIReportObject * pObject;
	pos = NULL;
	pos = pSheet->ObjList.GetHeadPosition();
	for(INT i = 0; i < count; i++) {
		pObject = NULL;
		pObject = (CSIReportObject*)pSheet->ObjList.GetNext(pos);
		if (!pObject) continue;
		if (pObject->GetObjType() == OBJECT_TABLE) {
			CSIReportObject_Table *pTable = (CSIReportObject_Table *)pObject;
			if (!pTable) continue;
			if (pTable->repeat == TRUE) return TRUE;
		}
	}
	return FALSE;

EFS_END
return FALSE;

}

INT CSIReportManager::ShowXMLFileOnePage(INT mode, CDC *pDC, INT nPage)
{
EFS_BEGIN

	if (m_pPageWorkForm) {
		FreePageWorkForm();
	}
	if (m_nCurrentPreviewPage != nPage) {
		LoadPageXMLWorkForm(nPage);
	}
	CSIReportWorkSheet *pSheet = m_pWorkForm->GetWorkSheet(0);
	if (pSheet == NULL) return -1;
	INT count = pSheet->ObjList.GetCount();
	if (count == 0) return -1;

	// Child�� �ƴ� ��ü�߿��� Child�� ���� ��ü ���� �μ��Ѵ�.
	CSIReportObject * pObject;
	POSITION pos;
	INT i;
	pos = pSheet->ObjList.GetHeadPosition();
	for(i = 0; i < count; i++) {
		pObject = NULL;
		pObject = (CSIReportObject*)pSheet->ObjList.GetNext(pos);
		if (!pObject) continue;
		if (pObject->GetObjType() == OBJECT_TABLE) {
			CSIReportObject_Table *pTable = (CSIReportObject_Table *)pObject;
			DrawObject_Table(mode, pDC, pTable);
		}
		if (pObject->GetObjType() == OBJECT_BARCODE) {
			CSIReportObject_Barcode *pBarcode = (CSIReportObject_Barcode *)pObject;
			DrawObject_Barcode(mode, pDC, pBarcode);
		}
		if (pObject->GetObjType() == OBJECT_TEXT_BOX) {
			CSIReportObject_Text_Box *pText_Box = (CSIReportObject_Text_Box *)pObject;
			DrawObject_Text_Box(mode, pDC, pText_Box);
		}
		if (pObject->GetObjType() == OBJECT_LINE) {
			CSIReportObject_Line *pLine = (CSIReportObject_Line *)pObject;
			DrawObject_Line(mode, pDC, pLine);
		}
		if (pObject->GetObjType() == OBJECT_BITMAP) {
			CSIReportObject_Bitmap *pBitmap = (CSIReportObject_Bitmap *)pObject;
			DrawObject_Bitmap(mode, pDC, pBitmap);
		}
	}
	return 0;

EFS_END
return -1;

}

INT CSIReportManager::APPLY_RearrangeObjects(CSIReportWorkForm *pWorkForm)
{
EFS_BEGIN

	return 0;

EFS_END
return -1;

}

INT CSIReportManager::APPLY_RearrangeTables(CRect rect, CSIReportObject_Table *pChildTable)
{
EFS_BEGIN

	INT nRow, nCol;
	if (pChildTable->row_count < 1) return 0;
	nRow = pChildTable->row_count-1;
	CSIReportObject_Table_Cell *pTmpCell;
	CRect rectTmp;
	for (nCol = 0; nCol < pChildTable->col_count; nCol++) {
		pTmpCell = NULL;
		pTmpCell = pChildTable->GetCell(nRow, nCol);
		if (pTmpCell == NULL) continue;
		rectTmp = pTmpCell->GetRect();
		if (rectTmp.bottom != rect.bottom) {
			rectTmp.bottom = rect.bottom;
			pTmpCell->SetRect(rectTmp);
		}
	}
	return 0;

EFS_END
return -1;

}

INT CSIReportManager::FindDM(CString strDMAlias) 
{
EFS_BEGIN
	m_pDefaultDM = NULL;
	for (INT i = 0; i < MAX_DM; i++) {
		if (m_pDM[i] == NULL) continue;
		if (m_pDM[i]->GETDMAlias() == strDMAlias) {
			m_pDefaultDM = m_pDM[i];
			return 0;
		}
	}
	return -1;
EFS_END
return -1;

}

INT CSIReportManager::ClearDM(VOID) 
{
EFS_BEGIN
	for (INT i = 0; i < MAX_DM; i++) {
		m_pDM[i] = NULL;
	}
	return 0;
EFS_END
return -1;
}

INT CSIReportManager::SetUserDefinePaperSize(INT cx, INT cy)
{
	m_nUserPaperSize = SIReportMgr_UserDefinePaper;
	m_sUserPaperSize.cx = cx;
	m_sUserPaperSize.cy = cy;
	return 0;
}
INT CSIReportManager::FileCopy(CString sStyleFile, CString sTempFile ){
	HANDLE hSource = INVALID_HANDLE_VALUE;
	HANDLE hTarget = INVALID_HANDLE_VALUE;
	DWORD dwFileSize = 0;
	
	try
	{
		// �������(����) �ڵ�
		hSource = CreateFile(sStyleFile, GENERIC_READ, 0, NULL,	OPEN_EXISTING, 0, NULL);
		if(hSource == INVALID_HANDLE_VALUE)
			throw MAKE_HRESULT(1, FACILITY_WIN32, GetLastError());		
		
		// ��������(���纻) �ڵ�
		hTarget = CreateFile(sTempFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
		if(hTarget == INVALID_HANDLE_VALUE)
			throw MAKE_HRESULT(1, FACILITY_WIN32, GetLastError());
		
		dwFileSize = GetFileSize(hSource, NULL);
		
		BYTE pBuff[BLOCK_LEN];		
		DWORD dwCopyLength = BLOCK_LEN;
		DWORD dwTransffered = 0;
		DWORD dwReadBytes = 0;
		DWORD dwWroteBytes = 0;
		
		while( dwTransffered < dwFileSize )
		{
			dwCopyLength = dwFileSize - dwTransffered;
			if(dwCopyLength >= BLOCK_LEN)
				dwCopyLength = BLOCK_LEN;
			
			if(!ReadFile(hSource, pBuff, dwCopyLength, &dwReadBytes, NULL) )
				throw MAKE_HRESULT(1, FACILITY_WIN32, GetLastError());
			
			if(!WriteFile(hTarget, pBuff, dwCopyLength, &dwWroteBytes, NULL) )
				throw MAKE_HRESULT(1, FACILITY_WIN32, GetLastError());
			
			dwTransffered += dwCopyLength;
		} // end of while
		
	}
	
	catch (HRESULT hr)
	{
		TRACE1("Error Occured .. %d", hr);
		return -1;
	}
	
	// file handle close
	if(hSource != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hSource);
		hSource = NULL;
	}
	if(hTarget != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hTarget);
		hTarget = NULL;
	}
	
	return 0;
}
INT CSIReportManager::ExportHancell()
{
	INT ids, i, j;
	INT nCnt = 0;
	CString sCnt, strMsg;
	sCnt.Format(_T("%d"), nCnt);
//	DWORD nSTART, nFINISH;
	CString sTempDirectory = _T("rpt_tmp\\Excel");						//tmp������ġ
	CString sTempFileName = _T("tmp_ExcelExport");
	CString sCurrentFileName = sTempFileName+_T(".cell");
	CString sTempFile = sTempDirectory + _T("\\") + sCurrentFileName;	//tmp����	
	
	//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰�
	CESL_DataMgr *pDM = m_pDM[0];
	INT nRow = pDM->GetRowCount();
	if(m_bPrivacyPrintLogRecord == TRUE && nRow > 0 ) {
		if( PrivacyPrintLogRecord(pDM, _T("H")) < 0 ) return -1;
	}

	// KU0133 20160211 JMC �Ѽ�����.
	CString strDebugFile;
	strDebugFile.Format(_T("..\\CFG\\_rptstyledebug"));
	CFileFind fDebugFind;
	if( fDebugFind.FindFile(strDebugFile) ) {		
		AfxMessageBox(m_sStyleFile);		
	}

	/*
	//////////////////////////////////////////////////////////////////////////////////////////////
	// 1. ExcelMgr���� �� �ʱ�ȭ
//	nSTART = GetTickCount();
	// COM ���� DLL�� �ε��ؼ� COM���� ��ɾ ����� �� �ֵ��� ���ش�
	CoInitialize(NULL);
	CExportToExcel *ExcelMgr;
	ExcelMgr = new CExportToExcel;
	// ���� ���� ��� ���� �ʱ�ȭ���� �����Ѵ�.
	ids = ExcelMgr->InitExcel();
	if( ids ) 
	{
		ExcelMgr->Release();
		delete ExcelMgr;
		ExcelMgr = NULL;
		return 10;
	}*/
//	nFINISH = GetTickCount();
//	TRACE2("1. ExcelMgr�������ʱ�ȭ[Time = %d.%03d sec]\r\n", (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000 );

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 2. tmp���� ���� �� ����
//	nSTART = GetTickCount();
	CString sErrorMsg;
	CFileFind fFind;
	if (!fFind.FindFile(sTempDirectory))
	{
		// ���丮 ������ ����
		if (!CreateDirectory(sTempDirectory, NULL))
		{
			//tmp���丮��������
			//ExcelMgr->Release();
			//delete ExcelMgr;
			//ExcelMgr = NULL;

			TRACE(_T("Error Occured = tmp���丮��������\r\n"));
			AfxMessageBox(_T("tmp���丮 ������ �����߽��ϴ�."));
			return -1;
		}
	}
	// 2009.05.11 UPDATE BY PWR : ������� �ߺ������ tmp������ ���� ���鵵�� ����
	// tmp���� ����� ����
	while(fFind.FindFile(sTempFile))
	{
		CFileException e;
		CFile f;
		f.Open(sTempFile, CFile::modeRead | CFile::typeBinary, &e);
		//if(e.m_cause == CFileException::sharingViolation) // �̹� ����Ǿ��־� lock�ɸ� ����
		if(e.m_cause != 0) // ����ó���߻�
		{
			nCnt++;
			sCnt.Format(_T("%d"), nCnt);
			sCurrentFileName = sTempFileName + sCnt + _T(".cell");
			sTempFile = sTempDirectory + _T("\\") + sCurrentFileName;	//tmp����
		}
		else
		{
			f.Close();
			if(!DeleteFile(sTempFile))
			{
				//tmp���丮��������
				//ExcelMgr->Release();
				//delete ExcelMgr;
				//ExcelMgr = NULL;

				TRACE(_T("Error Occured = tmp���ϻ�������\r\n"));
				AfxMessageBox(_T("����tmp���� ������ �����߽��ϴ�."));
				return -1;
			}
		}
	}
	// �ش� ��������� �������Ϸ� �����Ͽ� ����Ѵ�.
	ids = FileCopy( m_sStyleFile, sTempFile );
	if(ids<0) 
	{
		//ExcelMgr->Release();
		//delete ExcelMgr;
		//ExcelMgr = NULL;

		TRACE(_T("Error Occured = FileCopy"));
		strMsg.Format(_T("���Ϻ��翡 ���� �Ͽ����ϴ�.\r\n�ش�Excel������ ���μ����� ���������� Ȯ���� �ֽʽÿ�.\n[%s]"), m_sStyleFile);
		AfxMessageBox(strMsg);
		//AfxMessageBox(_T("���Ϻ��翡 ���� �Ͽ����ϴ�.\r\n�ش�Excel������ ���μ����� ���������� Ȯ���� �ֽʽÿ�.\n[%s]"), m_sStyleFile);
		return -1;
	}
//	nFINISH = GetTickCount();
//	TRACE2("2. tmp���Ϻ���׻���[Time = %d.%03d sec]\r\n", (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000 );

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 3. ������ tmp���� ����
//	nSTART = GetTickCount();
	TCHAR TmpDir[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH, TmpDir);

	CString sFile;
	sFile.Format(_T("%s\\%s"), TmpDir, sTempFile);

	CLSID clsid;
	HRESULT hr = CLSIDFromProgID(L"HCell.Application", &clsid);
	if(FAILED(hr)) {
		::MessageBox(NULL, _T("CLSIDFromProgID() failed"), _T("Error"), 0x10010);
		return -1;
	}
	
	IDispatch *pXlApp;
	hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void **)&pXlApp);
	if(FAILED(hr)) {
		::MessageBox(NULL, _T("Hancell not registered properly"), _T("Error"), 0x10010);
		return -1;
	}
	
	COleVariant vopt((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	_ApplicationHanCell application(pXlApp);
	
	WorkbooksHanCell books = application.GetWorkbooks();
	books.Open(COleVariant(sFile), vopt, vopt, vopt, vopt, vopt, vopt, vopt, vopt, vopt, vopt, vopt, vopt, vopt, vopt);
	
	_WorkbookHanCell book = application.GetActiveWorkbook();
	application.SetDisplayAlerts(TRUE);
	_WorksheetHanCell sheet = book.GetActiveSheet();
	
//	nFINISH = GetTickCount();
//	TRACE2("3. ������ tmp���� ����[Time = %d.%03d sec]\r\n", (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000 );

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 4. ȯ�漳�� ���� �� TEXT����(ġȯ)
//	nSTART = GetTickCount();
	//�޸� ��������(�޸� ȯ�漳���� �ִ�)
	//DM_USE@Y;TITLE_START_COL@1;TITLE_END_COL@13;TITLE_ROW@3;TEXT@A1@���Գ⵵ : %GETTEMPVARIABLE(���Գ⵵)%;TEXT@D1@%GETTEMPVARIABLE(������ȣ)%;
	//= ExcelMgr->GetComment(1,1);
	CString sComment; 

	RangeHanCell range = sheet.GetRangeFromLong(1, 1);
	VARIANT var = range.GetValue2();
	
	if ( var.bstrVal == NULL ) {
		application.Quit();
		TRACE(_T("Error Occured = Comment Isimpty"));
		AfxMessageBox(_T("ȯ�漳���� �����ü� �����ϴ�.\r\n�ش� �Ѽ������ �޸� Ȯ�����ֽʽÿ�."));
		return -1;
	}

	range.ChangeRangeToData(var, sComment);

	// KU0133 20160211 JMC �Ѽ�����.
	//ȯ�漳���� ���������� �����.
	CString strTmp = _T("");
	range = sheet.GetRangeFromLong(1, 1);
	range.SetValue2(COleVariant(strTmp));
/*	
	switch(var.vt)
	{
		case VT_EMPTY:
		case VT_NULL:
			sComment.Empty();
			break;
		case VT_BSTR:
			sComment.Format(_T("%s"), var.bstrVal);
			break;
		case VT_BSTR | VT_BYREF:
			sComment.Format(_T("%s"), var.pbstrVal);
			break;
		case VT_I2:
		case VT_I4:
			sComment.Format(_T("%d"), var.lVal);
			break;
		case VT_I2 | VT_BYREF:
		case VT_I4 | VT_BYREF:
			sComment.Format(_T("%d"), var.plVal);
			break;
		case VT_R4:
		case VT_R8:
			if ( var.dblVal != (INT)var.dblVal )
			{
				sComment.Format(_T("%.2f"), var.dblVal);
			}
			else
			{
				sComment.Format(_T("%d"), (INT)var.dblVal);
			}
			break;
		case VT_R4 | VT_BYREF:
		case VT_R8 | VT_BYREF:
			if ( var.pdblVal != (double*)(INT)var.pdblVal )
			{
				sComment.Format(_T("%.2f"), var.pdblVal);
			}
			else
			{
				sComment.Format(_T("%d"), (INT)var.pdblVal);
			}
			break;
		default:
		sComment.Empty();
	}*/
	
	// ������ ����Ʈ����
	CStringArray sList;
	CString sToken = _T(";"); //������
	while(sComment.Find(sToken) >= 0)
	{
		INT nCnt = sComment.Find(sToken);
		CString sTemp = sComment.Left( nCnt+1 );
		sComment = sComment.Right( sComment.GetLength() - (nCnt+1) );	
		sTemp.Replace(sToken, _T(""));
		sTemp.TrimLeft(); sTemp.TrimRight();
		if(!sTemp.IsEmpty()) sList.Add(sTemp);
	}
	sToken = _T("@"); //������
	CString sDM_USE, sTITLE_START_COL, sTITLE_END_COL, sTITLE_ROW, sTEXT;
	for(i = 0; i < sList.GetSize(); i++)
	{
		CString sData = sList.GetAt(i);
		INT nIdx = sData.Find(sToken);
		CString sType = sData.Left(nIdx);
		if(sType == _T("DM_USE")) //DM�������
			sDM_USE = sData.Right( sData.GetLength() - (nIdx+1) );
		else if(sType == _T("TITLE_START_COL")) //�����÷��� COL��°
			sTITLE_START_COL = sData.Right( sData.GetLength() - (nIdx+1) );
		else if(sType == _T("TITLE_END_COL")) //�������÷��� COL��°
			sTITLE_END_COL = sData.Right( sData.GetLength() - (nIdx+1) );
		else if(sType == _T("TITLE_ROW")) //�÷��� ROW��°
			sTITLE_ROW = sData.Right( sData.GetLength() - (nIdx+1) );
		else if(sType == _T("TEXT")) //TEXT(��ġ,����(ġȯ����))
		{
			//A1@���Գ⵵ : %GETTEMPVARIABLE(���Գ⵵)%
			sData = sData.Right( sData.GetLength() - (nIdx+1) );
			nIdx = sData.Find(sToken);
			CString sInsertPoint = sData.Left(nIdx);
			sTEXT = sData.Right( sData.GetLength() - (nIdx+1) );
			if( sTEXT.Find(_T("GETTEMPVARIABLE")) >= 0 )
			{//ġȯ�� ������ �ִ� TEXT(ġȯ�ؾ��Ѵ�)
				//���Գ⵵ : %GETTEMPVARIABLE(���Գ⵵)%
				INT nPos1, nPos2;
				nPos1 = sTEXT.Find(_T("%GETTEMPVARIABLE("));
				nPos2 = sTEXT.Find(_T(")%"));
				sData = sTEXT.Mid(nPos1, nPos2 - nPos1 + 2);//%GETTEMPVARIABLE(���Գ⵵)%
				CString sVariableName, sVariableValue;
				if (nPos1 != -1 && nPos2 != -1) {
					nPos2 = sTEXT.Find(_T(")%"));
					sVariableName.Format(_T("%s"), sTEXT.Mid(nPos1 + 17, nPos2 - nPos1 - 17));//���Գ⵵
					sVariableName.TrimLeft();	sVariableName.TrimRight();
					sVariableValue = _T("");
					GetTempVariable(sVariableName, sVariableValue);
					sTEXT.Replace(sData, sVariableValue);
				}
				range = sheet.GetRange(COleVariant(sInsertPoint), COleVariant(sInsertPoint));
				range.SetValue2(COleVariant(sTEXT));

				//ExcelMgr->SetTextToExcel(sInsertPoint, sTEXT);
			}
			else
			{//ġȯ������� TEXT(�ٷ��Է�)
				range = sheet.GetRange(COleVariant(sInsertPoint), COleVariant(sInsertPoint));
				range.SetValue2(COleVariant(sTEXT));
				//ExcelMgr->SetTextToExcel(sInsertPoint, sTEXT);
			}
		}
	}	
	
//	nFINISH = GetTickCount();
//	TRACE2("4. ȯ�漳��������TEXT����(ġȯ)[Time = %d.%03d sec]\r\n", (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000 );

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 5. ��¿�DM�� �����ϴ� ��¹��� ���ؼ��� DATA����
//	nSTART = GetTickCount();

	if( sDM_USE == _T("Y") )
	{
		INT nTITLE_START_COL	= _ttoi(sTITLE_START_COL);
		INT nTITLE_END_COL		= _ttoi(sTITLE_END_COL);
		INT nTITLE_ROW			= _ttoi(sTITLE_ROW);
		CStringArray sColumns;
		for(i = nTITLE_START_COL; i < nTITLE_END_COL + 1; i++)
		{
			CString sColumnData;			
			range = sheet.GetRangeFromLong(nTITLE_ROW, i);
			var = range.GetValue2();
			range.ChangeRangeToData(var, sColumnData);			
			//CString sColumnData = ExcelMgr->GetValue( i, nTITLE_ROW );
			sColumns.Add(sColumnData);
		}
		
		CESL_DataMgr *pDM = m_pDM[0];
		INT nRow = pDM->GetRowCount();
		//ExcelMgr->InitArray( nRow, nTITLE_END_COL );	
		
		for( i = 0; i < nRow; i++)
		{
			for(j = 0; j < nTITLE_END_COL; j++)
			{
				CString sData;
				pDM->GetCellData( sColumns.GetAt(j), i, sData);
				range = sheet.GetRangeFromLong(nTITLE_ROW+i, j+1);
				range.SetValue2(COleVariant(sData));
				//ExcelMgr->FillSafeArray( i, j, sData );
			}
		}
		//ExcelMgr->PutAllbyArray( nTITLE_ROW, nTITLE_START_COL, nRow, nTITLE_END_COL );
		// �׵θ�		
		//ExcelMgr->SetBorder(nTITLE_START_COL,nTITLE_ROW-1,nTITLE_END_COL,nRow+1, -4142,1);
	}
//	nFINISH = GetTickCount();
//	TRACE2("5. DATA����[Time = %d.%03d sec]\r\n", (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000 );

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 6. ȭ��ǥ�� �� Release
//	nSTART = GetTickCount();
	// ȭ��ǥ��	
	application.SetVisible(TRUE);			

/*	ExcelMgr->VisableExcel(TRUE);
	// Release
	ExcelMgr->Release();
	delete ExcelMgr;
	ExcelMgr = NULL;*/
//	nFINISH = GetTickCount();
//	TRACE2("6. ȭ��ǥ�ù�Release[Time = %d.%03d sec]\r\n", (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000 );
	
	//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰� ��ġ�̵�
	/*
	// KU0303	�������� ��·α� - ����.
	CESL_DataMgr *pDM = m_pDM[0];
	INT nRow = pDM->GetRowCount();
	if( sDM_USE == _T("Y") && m_bPrivacyPrintLogRecord == TRUE && nRow > 0 ) {
		ids = PrivacyPrintLogRecord(pDM, _T("H"));
	}
	*/

	return 0;
}
// 2008.10.01 ADD BY PWR
// *������±��
// DESC : EXCEL��������� �̿��Ͽ� ������¹��� �����.
// 1. ExcelMgr���� �� �ʱ�ȭ
// 2. tmp���� ���� �� ����
// 3. ������ tmp���� ����
// 4. ȯ�漳�� ���� �� TEXT����(ġȯ)
// 5. ��¿�DM�� �����ϴ� ��¹��� ���ؼ��� DATA����
// 6. ȭ��ǥ�� �� Release
INT CSIReportManager::ExcelToExport()
{
	INT ids, i, j;
	INT nCnt = 0;
	CString sCnt;
	sCnt.Format(_T("%d"), nCnt);
//	DWORD nSTART, nFINISH;
	CString sTempDirectory = _T("rpt_tmp\\Excel");						//tmp������ġ
	CString sTempFileName = _T("tmp_ExcelExport");
	CString sCurrentFileName = sTempFileName+_T(".xls");
	CString sTempFile = sTempDirectory + _T("\\") + sCurrentFileName;	//tmp����
		
	//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰�
	CESL_DataMgr *pDM = m_pDM[0];
	INT nRow = pDM->GetRowCount();
	if(m_bPrivacyPrintLogRecord == TRUE && nRow > 0 ) {
		if( PrivacyPrintLogRecord(pDM, _T("E")) < 0 ) return -1;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////
	// 1. ExcelMgr���� �� �ʱ�ȭ
//	nSTART = GetTickCount();
	// COM ���� DLL�� �ε��ؼ� COM���� ��ɾ ����� �� �ֵ��� ���ش�
	CoInitialize(NULL);
	CExportToExcel *ExcelMgr;
	ExcelMgr = new CExportToExcel;
	// ���� ���� ��� ���� �ʱ�ȭ���� �����Ѵ�.
	ids = ExcelMgr->InitExcel();
	if( ids ) 
	{
		ExcelMgr->Release();
		delete ExcelMgr;
		ExcelMgr = NULL;
		return 10;
	}
//	nFINISH = GetTickCount();
//	TRACE2("1. ExcelMgr�������ʱ�ȭ[Time = %d.%03d sec]\r\n", (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000 );

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 2. tmp���� ���� �� ����
//	nSTART = GetTickCount();
	CString sErrorMsg;
	CFileFind fFind;
	if (!fFind.FindFile(sTempDirectory))
	{
		// ���丮 ������ ����
		if (!CreateDirectory(sTempDirectory, NULL))
		{
			//tmp���丮��������
			ExcelMgr->Release();
			delete ExcelMgr;
			ExcelMgr = NULL;

			TRACE(_T("Error Occured = tmp���丮��������\r\n"));
			AfxMessageBox(_T("tmp���丮 ������ �����߽��ϴ�."));
			return -1;
		}
	}
	// 2009.05.11 UPDATE BY PWR : ������� �ߺ������ tmp������ ���� ���鵵�� ����
	// tmp���� ����� ����
	while(fFind.FindFile(sTempFile))
	{
		CFileException e;
		CFile f;
		f.Open(sTempFile, CFile::modeRead | CFile::typeBinary, &e);
		//if(e.m_cause == CFileException::sharingViolation) // �̹� ����Ǿ��־� lock�ɸ� ����
		if(e.m_cause != 0) // ����ó���߻�
		{
			nCnt++;
			sCnt.Format(_T("%d"), nCnt);
			sCurrentFileName = sTempFileName + sCnt + _T(".xls");
			sTempFile = sTempDirectory + _T("\\") + sCurrentFileName;	//tmp����
		}
		else
		{
			f.Close();
			if(!DeleteFile(sTempFile))
			{
				//tmp���丮��������
				ExcelMgr->Release();
				delete ExcelMgr;
				ExcelMgr = NULL;

				TRACE(_T("Error Occured = tmp���ϻ�������\r\n"));
				AfxMessageBox(_T("����tmp���� ������ �����߽��ϴ�."));
				return -1;
			}
		}
	}
	// �ش� ��������� �������Ϸ� �����Ͽ� ����Ѵ�.
	ids = ExcelMgr->FileCopy( m_sStyleFile, sTempFile );
	if(ids<0) 
	{
		ExcelMgr->Release();
		delete ExcelMgr;
		ExcelMgr = NULL;

		TRACE(_T("Error Occured = FileCopy"));
		AfxMessageBox(_T("���Ϻ��翡 ���� �Ͽ����ϴ�.\r\n�ش�Excel������ ���μ����� ���������� Ȯ���� �ֽʽÿ�."));
		return -1;
	}
//	nFINISH = GetTickCount();
//	TRACE2("2. tmp���Ϻ���׻���[Time = %d.%03d sec]\r\n", (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000 );

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 3. ������ tmp���� ����
//	nSTART = GetTickCount();
	TCHAR TmpDir[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH, TmpDir);
	CString sFile;
	sFile.Format(_T("%s\\%s"), TmpDir, sTempFile);
	ExcelMgr->Open(sFile);
//	nFINISH = GetTickCount();
//	TRACE2("3. ������ tmp���� ����[Time = %d.%03d sec]\r\n", (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000 );

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 4. ȯ�漳�� ���� �� TEXT����(ġȯ)
//	nSTART = GetTickCount();
	//�޸� ��������(�޸� ȯ�漳���� �ִ�)
	//DM_USE@Y;TITLE_START_COL@1;TITLE_END_COL@13;TITLE_ROW@3;TEXT@A1@���Գ⵵ : %GETTEMPVARIABLE(���Գ⵵)%;TEXT@D1@%GETTEMPVARIABLE(������ȣ)%;
	CString sComment = ExcelMgr->GetComment(1,1);
	if( sComment.IsEmpty() )
	{
		ExcelMgr->Release();
		delete ExcelMgr;
		ExcelMgr = NULL;

		TRACE(_T("Error Occured = Comment Isimpty"));
		AfxMessageBox(_T("ȯ�漳���� �����ü� �����ϴ�.\r\n�ش� EXCEL����� �޸� Ȯ�����ֽʽÿ�."));
		return -1;
	}
	// ������ ����Ʈ����
	CStringArray sList;
	CString sToken = _T(";"); //������
	while(sComment.Find(sToken) >= 0)
	{
		INT nCnt = sComment.Find(sToken);
		CString sTemp = sComment.Left( nCnt+1 );
		sComment = sComment.Right( sComment.GetLength() - (nCnt+1) );	
		sTemp.Replace(sToken, _T(""));
		sTemp.TrimLeft(); sTemp.TrimRight();
		if(!sTemp.IsEmpty()) sList.Add(sTemp);
	}
	sToken = _T("@"); //������
	CString sDM_USE, sTITLE_START_COL, sTITLE_END_COL, sTITLE_ROW, sTEXT;
	for(i = 0; i < sList.GetSize(); i++)
	{
		CString sData = sList.GetAt(i);
		INT nIdx = sData.Find(sToken);
		CString sType = sData.Left(nIdx);
		if(sType == _T("DM_USE")) //DM�������
			sDM_USE = sData.Right( sData.GetLength() - (nIdx+1) );
		else if(sType == _T("TITLE_START_COL")) //�����÷��� COL��°
			sTITLE_START_COL = sData.Right( sData.GetLength() - (nIdx+1) );
		else if(sType == _T("TITLE_END_COL")) //�������÷��� COL��°
			sTITLE_END_COL = sData.Right( sData.GetLength() - (nIdx+1) );
		else if(sType == _T("TITLE_ROW")) //�÷��� ROW��°
			sTITLE_ROW = sData.Right( sData.GetLength() - (nIdx+1) );
		else if(sType == _T("TEXT")) //TEXT(��ġ,����(ġȯ����))
		{
			//A1@���Գ⵵ : %GETTEMPVARIABLE(���Գ⵵)%
			sData = sData.Right( sData.GetLength() - (nIdx+1) );
			nIdx = sData.Find(sToken);
			CString sInsertPoint = sData.Left(nIdx);
			sTEXT = sData.Right( sData.GetLength() - (nIdx+1) );
			if( sTEXT.Find(_T("GETTEMPVARIABLE")) >= 0 )
			{//ġȯ�� ������ �ִ� TEXT(ġȯ�ؾ��Ѵ�)
				//���Գ⵵ : %GETTEMPVARIABLE(���Գ⵵)%
				INT nPos1, nPos2;
				nPos1 = sTEXT.Find(_T("%GETTEMPVARIABLE("));
				nPos2 = sTEXT.Find(_T(")%"));
				sData = sTEXT.Mid(nPos1, nPos2 - nPos1 + 2);//%GETTEMPVARIABLE(���Գ⵵)%
				CString sVariableName, sVariableValue;
				if (nPos1 != -1 && nPos2 != -1) {
					nPos2 = sTEXT.Find(_T(")%"));
					sVariableName.Format(_T("%s"), sTEXT.Mid(nPos1 + 17, nPos2 - nPos1 - 17));//���Գ⵵
					sVariableName.TrimLeft();	sVariableName.TrimRight();
					sVariableValue = _T("");
					GetTempVariable(sVariableName, sVariableValue);
					sTEXT.Replace(sData, sVariableValue);
				}
				ExcelMgr->SetTextToExcel(sInsertPoint, sTEXT);
			}
			else
			{//ġȯ������� TEXT(�ٷ��Է�)
				ExcelMgr->SetTextToExcel(sInsertPoint, sTEXT);
			}
		}
	}
	// �޸������
	ExcelMgr->ClearComments(1,1);
//	nFINISH = GetTickCount();
//	TRACE2("4. ȯ�漳��������TEXT����(ġȯ)[Time = %d.%03d sec]\r\n", (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000 );

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 5. ��¿�DM�� �����ϴ� ��¹��� ���ؼ��� DATA����
//	nSTART = GetTickCount();
	if( sDM_USE == _T("Y") )
	{
		INT nTITLE_START_COL	= _ttoi(sTITLE_START_COL);
		INT nTITLE_END_COL		= _ttoi(sTITLE_END_COL);
		INT nTITLE_ROW			= _ttoi(sTITLE_ROW);
		CStringArray sColumns;
		for(i = nTITLE_START_COL; i < nTITLE_END_COL + 1; i++)
		{
			CString sColumnData = ExcelMgr->GetValue( i, nTITLE_ROW );
			sColumns.Add(sColumnData);
		}
		
		CESL_DataMgr *pDM = m_pDM[0];
		INT nRow = pDM->GetRowCount();
		ExcelMgr->InitArray( nRow, nTITLE_END_COL );	
		
		for( i = 0; i < nRow; i++)
		{
			for(j = 0; j < nTITLE_END_COL; j++)
			{
				CString sData;
				pDM->GetCellData( sColumns.GetAt(j), i, sData);
				ExcelMgr->FillSafeArray( i, j, sData );
			}
		}
		ExcelMgr->PutAllbyArray( nTITLE_ROW, nTITLE_START_COL, nRow, nTITLE_END_COL );
		// �׵θ�
		ExcelMgr->SetBorder(nTITLE_START_COL,nTITLE_ROW-1,nTITLE_END_COL,nRow+1, -4142,1);
	}
//	nFINISH = GetTickCount();
//	TRACE2("5. DATA����[Time = %d.%03d sec]\r\n", (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000 );

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 6. ȭ��ǥ�� �� Release
//	nSTART = GetTickCount();
	// ȭ��ǥ��
	ExcelMgr->VisableExcel(TRUE);
	// Release
	ExcelMgr->Release();
	delete ExcelMgr;
	ExcelMgr = NULL;
//	nFINISH = GetTickCount();
//	TRACE2("6. ȭ��ǥ�ù�Release[Time = %d.%03d sec]\r\n", (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000 );
	
	//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰� ��ġ�̵�
	/*
	//JOB.2019.0017 ADD BY KYJ
	//�������� ���� ����,�α� ����
	// KU0303	�������� ��·α� - ����.
	CESL_DataMgr *pDM = m_pDM[0];
	INT nRow = pDM->GetRowCount();
	if( sDM_USE == _T("Y") && m_bPrivacyPrintLogRecord == TRUE && nRow > 0 ) {
		ids = PrivacyPrintLogRecord(pDM, _T("E"));
	}
	*/

	return 0;
}

// 16/03/29 ������ : �Ѽ� �����ϱ�
//*/ ADD -------------------------------------------------------------------------------------
// *�Ѽ���±��
// DESC : EXCEL��������� �̿��Ͽ� �Ѽ���¹��� �����.
// 1. HCellMgr���� �� �ʱ�ȭ
// 2. tmp���� ���� �� ����
// 3. ������ tmp���� ����
// 4. ȯ�漳�� ���� �� TEXT����(ġȯ)
// 5. ��¿�DM�� �����ϴ� ��¹��� ���ؼ��� DATA����
// 6. ȭ��ǥ�� �� Release
INT CSIReportManager::HCellToExport()
{
	INT ids, i, j;
	INT nCnt = 0;
	CString sCnt;
	sCnt.Format(_T("%d"), nCnt);

	CString sTempDirectory = _T("rpt_tmp\\Excel");						//tmp������ġ
	CString sTempFileName = _T("tmp_ExcelExport");
	CString sCurrentFileName = sTempFileName+_T(".cell");				
	CString sTempFile = sTempDirectory + _T("\\") + sCurrentFileName;	//tmp����

	//////////////////////////////////////////////////////////////////////////////////////////////

	//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰�
	CESL_DataMgr *pDM = m_pDM[0];
	INT nRow = pDM->GetRowCount();
	if(m_bPrivacyPrintLogRecord == TRUE && nRow > 0 ) {
		if( PrivacyPrintLogRecord(pDM, _T("H")) < 0 ) return -1;
	}

	// 1. ExcelMgr���� �� �ʱ�ȭ
	// COM ���� DLL�� �ε��ؼ� COM���� ��ɾ ����� �� �ֵ��� ���ش�
	CoInitialize(NULL);
	CExportToHCell *HCellMgr;
	HCellMgr = new CExportToHCell;

	// ���� ���� ��� ���� �ʱ�ȭ���� �����Ѵ�.
	ids = HCellMgr->InitExcel();
	if( ids ) 
	{
		HCellMgr->Release();
		delete HCellMgr;
		HCellMgr = NULL;		
		// 18.10.30 JMJ KOL.RED.2018.021
		// ���� SIReport�� �����Ϸ��� -1�� �ƴ� 10�� �Ǿ�� ��.
		//return -1;
		return 10;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 2. tmp���� ���� �� ����
	CString sErrorMsg;
	CFileFind fFind;
	if (!fFind.FindFile(sTempDirectory))
	{
		// ���丮 ������ ����
		if (!CreateDirectory(sTempDirectory, NULL))
		{
			//tmp���丮��������
			HCellMgr->Close();
			delete HCellMgr;
			HCellMgr = NULL;

			TRACE(_T("Error Occured = tmp���丮��������\r\n"));
			AfxMessageBox(_T("tmp���丮 ������ �����߽��ϴ�."));
			return -1;
		}
	}
	// 2009.05.11 UPDATE BY PWR : ������� �ߺ������ tmp������ ���� ���鵵�� ����
	// tmp���� ����� ����
	while(fFind.FindFile(sTempFile))
	{
		CFileException e;
		CFile f;
		f.Open(sTempFile, CFile::modeRead | CFile::typeBinary, &e);
		//if(e.m_cause == CFileException::sharingViolation) // �̹� ����Ǿ��־� lock�ɸ� ����
		if(e.m_cause != 0) // ����ó���߻�
		{
			nCnt++;
			sCnt.Format(_T("%d"), nCnt);
			sCurrentFileName = sTempFileName + sCnt + _T(".cell");		//����
			sTempFile = sTempDirectory + _T("\\") + sCurrentFileName;	//tmp����
		}
		else
		{
			f.Close();
			if(!DeleteFile(sTempFile))
			{
				//tmp���丮��������
				HCellMgr->Close();
				delete HCellMgr;
				HCellMgr = NULL;

				TRACE(_T("Error Occured = tmp���ϻ�������\r\n"));
				AfxMessageBox(_T("����tmp���� ������ �����߽��ϴ�."));
				return -1;
			}
		}
	}
	// �ش� ��������� �������Ϸ� �����Ͽ� ����Ѵ�.
	ids = HCellMgr->FileCopy( m_sStyleFile, sTempFile );
	if(ids<0) 
	{
		HCellMgr->Close();
		delete HCellMgr;
		HCellMgr = NULL;

		TRACE(_T("Error Occured = FileCopy"));
		AfxMessageBox(_T("���Ϻ��翡 ���� �Ͽ����ϴ�.\r\n�ش� HCELL������ ���μ����� ���������� Ȯ���� �ֽʽÿ�.")); 
		return -1;
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////
	// 3. ������ tmp���� ����
	TCHAR TmpDir[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH, TmpDir);
	CString sFile;
	sFile.Format(_T("%s\\%s"), TmpDir, sTempFile);
	HCellMgr->Open(sFile);

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 4. ȯ�漳�� ���� �� TEXT����(ġȯ)
	//�޸� ��������(�޸� ȯ�漳���� �ִ�)
	//DM_USE@Y;TITLE_START_COL@1;TITLE_END_COL@3;TITLE_ROW@2;TEXT@A1@���Գ⵵ : %GETTEMPVARIABLE(���Գ⵵)%;TEXT@D1@%GETTEMPVARIABLE(������ȣ)%;
	CString sComment = HCellMgr->GetComment(1,1);
	if( sComment.IsEmpty() )
	{
		HCellMgr->Close();
		delete HCellMgr;
		HCellMgr = NULL;

		TRACE(_T("Error Occured = Comment Isimpty"));
		AfxMessageBox(_T("ȯ�漳���� �����ü� �����ϴ�.\r\n�ش� EXCEL����� �޸� Ȯ�����ֽʽÿ�."));
		return -1;
	}

	// ������ ����Ʈ����
	CStringArray sList;
	CString sToken = _T(";"); //������
	while(sComment.Find(sToken) >= 0)
	{
		INT nCnt = sComment.Find(sToken);
		CString sTemp = sComment.Left( nCnt+1 );
		sComment = sComment.Right( sComment.GetLength() - (nCnt+1) );	
		sTemp.Replace(sToken, _T(""));
		sTemp.TrimLeft(); sTemp.TrimRight();
		if(!sTemp.IsEmpty()) sList.Add(sTemp);
	}
	sToken = _T("@"); //������
	CString sDM_USE, sTITLE_START_COL, sTITLE_END_COL, sTITLE_ROW, sTEXT;
	for(i = 0; i < sList.GetSize(); i++)
	{
		CString sData = sList.GetAt(i);
		INT nIdx = sData.Find(sToken);
		CString sType = sData.Left(nIdx);
		if(sType == _T("DM_USE")) //DM�������
			sDM_USE = sData.Right( sData.GetLength() - (nIdx+1) );
		else if(sType == _T("TITLE_START_COL")) //�����÷��� COL��°
			sTITLE_START_COL = sData.Right( sData.GetLength() - (nIdx+1) );
		else if(sType == _T("TITLE_END_COL")) //�������÷��� COL��°
			sTITLE_END_COL = sData.Right( sData.GetLength() - (nIdx+1) );
		else if(sType == _T("TITLE_ROW")) //�÷��� ROW��°
			sTITLE_ROW = sData.Right( sData.GetLength() - (nIdx+1) );
		else if(sType == _T("TEXT")) //TEXT(��ġ,����(ġȯ����))
		{
			//A1@���Գ⵵ : %GETTEMPVARIABLE(���Գ⵵)%
			sData = sData.Right( sData.GetLength() - (nIdx+1) );
			nIdx = sData.Find(sToken);
			CString sInsertPoint = sData.Left(nIdx);
			sTEXT = sData.Right( sData.GetLength() - (nIdx+1) );
			if( sTEXT.Find(_T("GETTEMPVARIABLE")) >= 0 )
			{//ġȯ�� ������ �ִ� TEXT(ġȯ�ؾ��Ѵ�)
				//���Գ⵵ : %GETTEMPVARIABLE(���Գ⵵)%
				INT nPos1, nPos2;
				nPos1 = sTEXT.Find(_T("%GETTEMPVARIABLE("));
				nPos2 = sTEXT.Find(_T(")%"));
				sData = sTEXT.Mid(nPos1, nPos2 - nPos1 + 2);//%GETTEMPVARIABLE(���Գ⵵)%
				CString sVariableName, sVariableValue;
				if (nPos1 != -1 && nPos2 != -1) {
					nPos2 = sTEXT.Find(_T(")%"));
					sVariableName.Format(_T("%s"), sTEXT.Mid(nPos1 + 17, nPos2 - nPos1 - 17));//���Գ⵵
					sVariableName.TrimLeft();	sVariableName.TrimRight();
					sVariableValue = _T("");
					GetTempVariable(sVariableName, sVariableValue);
					sTEXT.Replace(sData, sVariableValue);
				}
				HCellMgr->SetTextToHCell(sInsertPoint, sTEXT);
			}
			else
			{//ġȯ������� TEXT(�ٷ��Է�)
				HCellMgr->SetTextToHCell(sInsertPoint, sTEXT);
			}			
		}
	}
	// �޸������
	HCellMgr->ClearComments(1,1);

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 5. ��¿�DM�� �����ϴ� ��¹��� ���ؼ��� DATA����
	if( sDM_USE == _T("Y") )
	{
		INT nTITLE_START_COL	= _ttoi(sTITLE_START_COL);
		INT nTITLE_END_COL		= _ttoi(sTITLE_END_COL);
		INT nTITLE_ROW			= _ttoi(sTITLE_ROW);
		CStringArray sColumns;
		for(i = nTITLE_START_COL; i < nTITLE_END_COL + 1; i++)
		{
			CString sColumnData = HCellMgr->GetValue( i, nTITLE_ROW );
			sColumns.Add(sColumnData);
		}
		
		CESL_DataMgr *pDM = m_pDM[0];
		INT nRow = pDM->GetRowCount();
		if(nRow > 0)
		{
			HCellMgr->InitArray( nRow, nTITLE_END_COL );	
			
			CString strMsg;
			for( i = 0; i < nRow; i++)
			{
				for(j = 0; j < nTITLE_END_COL; j++)
				{
					CString sData;
					pDM->GetCellData( sColumns.GetAt(j), i, sData);
					HCellMgr->FillSafeArray( i, j, sData );	
				}
			}		
			HCellMgr->PutAllbyArray( nTITLE_ROW, nTITLE_START_COL, nRow, nTITLE_END_COL );
		}
		// 16/07/20 ������ : �����ڷ����/�ݳ��ڷ����� ����� ���� �ʴ� ���� ���� (Border ����)
/*// BEFORE ----------------------------------------------------------------------------------
		HCellMgr->SetBorder( nTITLE_START_COL, nTITLE_ROW-1, nTITLE_END_COL, nRow+1, -4142, 1 );
*/// AFTER ----------------------------------------------------------------------------------
		HCellMgr->SetBorder( nTITLE_START_COL, nTITLE_ROW-1, nTITLE_END_COL, nTITLE_ROW+nRow-1, -4142, 1 );
//*/ END -------------------------------------------------------------------------------------
		HCellMgr->RangeSelect(_ttoi(sTITLE_END_COL), _ttoi(sTITLE_ROW));	//������ ������ ���߱� ���� ù°�� �������� ��ġ
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 6. ȭ��ǥ�� �� Release
	// ȭ��ǥ��
	HCellMgr->VisableExcel(TRUE);
	HCellMgr->Release();
	delete HCellMgr;
	HCellMgr = NULL;

	return 0;
}
// 18.10.30 JMJ KOL.RED.2018.021
INT CSIReportManager::CSVToExport()
{
	INT ids, i, j;
	INT nCnt = 0;
	CString sCnt;
	sCnt.Format(_T("%d"), nCnt);

	CString sTempDirectory = _T("rpt_tmp\\Excel");						//tmp������ġ
	CString sTempFileName = _T("tmp_ExcelExport");
	CString sCurrentFileName = sTempFileName+_T(".csv");				
	CString sTempFile = sTempDirectory + _T("\\") + sCurrentFileName;	//tmp����
	//////////////////////////////////////////////////////////////////////////////////////////////

	//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰�
	CESL_DataMgr *pDM = m_pDM[0];
	INT nRow = pDM->GetRowCount();
	if(m_bPrivacyPrintLogRecord == TRUE && nRow > 0 ) {
		if( PrivacyPrintLogRecord(pDM, _T("C")) < 0 ) return -1;
	}

	// 3. ������ tmp���� ����
	TCHAR TmpDir[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH, TmpDir);
	CString sFile;
	sFile.Format(_T("%s\\%s"), TmpDir, sTempFile);



	CStdioFileEx	fFile;
	if( fFile.Open( m_sStyleFile ,  CFile::modeRead | CFile::typeBinary) == FALSE ) 
	{
		AfxMessageBox( m_sStyleFile );
		return -1010;
	}
	INT nFileLength = fFile.GetLength();
	if( 1 > nFileLength )
	{
		AfxMessageBox( _T("��� �ִ� File �Դϴ�. File�� Ȯ���Ͽ� �ֽʽÿ�.") , MB_ICONINFORMATION );
		return -1020;
	}
	
	TCHAR cUni;
	fFile.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{/// Unicode �ƴ϶��
		fFile.SeekToBegin();
		fFile.Read( &cUni, 3 );
		if(  0xBBEF == cUni  )
		{/// UTF�� ���
			fFile.SetCodePage(CP_UTF8);
		}
		else
		{/// ANSI�� ���
			//fFile.SetCodePage(CP_ACP);
			fFile.SeekToBegin();
		}
	}

	CStringArray saTempData;
	CString strLine;
	CString sComment;
	while( fFile.ReadString( strLine ) )
	{
		strLine.TrimLeft();		strLine.TrimRight();
		/*
		if( !strLine.GetLength() )
		{
			continue;
		}
		*/
		/*
			CSV ������
			ù°�ٿ� ���� ������ �޸�
			��°�ٿ� ǥ�� Ÿ��Ʋ
			��°�ٿ� DM�� ALIAS

			3������ �־����.

LINE 1 : DM_USE@Y;TITLE_START_COL@1;TITLE_END_COL@3;TITLE_ROW@2;
LINE 2 : ����\t��Ϲ�ȣ
LINE 3 : DM.����ALIAS\tDM.��Ϲ�ȣALIAS
		*/
		saTempData.Add(strLine);
	}

	// 2010.09.06 ADD BY PWR : CLOSE
	fFile.Close();
	//////////////////////////////////////////////////////////////////////////////////////////////
	// 4. ȯ�漳�� ���� �� TEXT����(ġȯ)
	//�޸� ��������(�޸� ȯ�漳���� �ִ�)
	//DM_USE@Y;TITLE_START_COL@1;TITLE_END_COL@3;TITLE_ROW@2;
	if(saTempData.GetSize() != 3)
	{
		TRACE(_T("Error Occured = Comment IsEmpty"));
		AfxMessageBox(_T("ȯ�漳���� �����ü� �����ϴ�.\r\n�ش� CSV����� �����͸� Ȯ�����ֽʽÿ�."));
		return -1;

	}

	sComment = saTempData.GetAt(0);
	if( sComment.IsEmpty() )
	{
		TRACE(_T("Error Occured = Comment IsEmpty"));
		AfxMessageBox(_T("ȯ�漳���� �����ü� �����ϴ�.\r\n�ش� CSV����� �����͸� Ȯ�����ֽʽÿ�."));
		return -2;
	}

	CStringList pStringList;

	// ������ ����Ʈ����
	CStringArray sList;
	CString sToken = _T(";"); //������
	while(sComment.Find(sToken) >= 0)
	{
		INT nCnt = sComment.Find(sToken);
		CString sTemp = sComment.Left( nCnt+1 );
		sComment = sComment.Right( sComment.GetLength() - (nCnt+1) );	
		sTemp.Replace(sToken, _T(""));
		sTemp.TrimLeft(); sTemp.TrimRight();
		if(!sTemp.IsEmpty()) sList.Add(sTemp);
	}
	sToken = _T("@"); //������
	CString sDM_USE, sTITLE_START_COL, sTITLE_END_COL, sTITLE_ROW, sTEXT;
	for(i = 0; i < sList.GetSize(); i++)
	{
		CString sData = sList.GetAt(i);
		INT nIdx = sData.Find(sToken);
		CString sType = sData.Left(nIdx);
		if(sType == _T("DM_USE")) //DM�������
			sDM_USE = sData.Right( sData.GetLength() - (nIdx+1) );
		else if(sType == _T("TITLE_START_COL")) //�����÷��� COL��°
			sTITLE_START_COL = sData.Right( sData.GetLength() - (nIdx+1) );
		else if(sType == _T("TITLE_END_COL")) //�������÷��� COL��°
			sTITLE_END_COL = sData.Right( sData.GetLength() - (nIdx+1) );
		else if(sType == _T("TITLE_ROW")) //�÷��� ROW��°
			sTITLE_ROW = sData.Right( sData.GetLength() - (nIdx+1) );
		/*
		// ������ǥ�� ���� ������
		else if(sType == _T("TEXT")) //TEXT(��ġ,����(ġȯ����))
		{
			//A1@���Գ⵵ : %GETTEMPVARIABLE(���Գ⵵)%
			sData = sData.Right( sData.GetLength() - (nIdx+1) );
			nIdx = sData.Find(sToken);
			CString sInsertPoint = sData.Left(nIdx);
			sTEXT = sData.Right( sData.GetLength() - (nIdx+1) );
			if( sTEXT.Find(_T("GETTEMPVARIABLE")) >= 0 )
			{//ġȯ�� ������ �ִ� TEXT(ġȯ�ؾ��Ѵ�)
				//���Գ⵵ : %GETTEMPVARIABLE(���Գ⵵)%
				INT nPos1, nPos2;
				nPos1 = sTEXT.Find(_T("%GETTEMPVARIABLE("));
				nPos2 = sTEXT.Find(_T(")%"));
				sData = sTEXT.Mid(nPos1, nPos2 - nPos1 + 2);//%GETTEMPVARIABLE(���Գ⵵)%
				CString sVariableName, sVariableValue;
				if (nPos1 != -1 && nPos2 != -1) {
					nPos2 = sTEXT.Find(_T(")%"));
					sVariableName.Format(_T("%s"), sTEXT.Mid(nPos1 + 17, nPos2 - nPos1 - 17));//���Գ⵵
					sVariableName.TrimLeft();	sVariableName.TrimRight();
					sVariableValue = _T("");
					GetTempVariable(sVariableName, sVariableValue);
					sTEXT.Replace(sData, sVariableValue);
				}
				// �������
				//HCellMgr->SetTextToHCell(sInsertPoint, sTEXT);
			}
			else
			{//ġȯ������� TEXT(�ٷ��Է�)
				//�������
				//HCellMgr->SetTextToHCell(sInsertPoint, sTEXT);
			}			
		}
		*/
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 5. ��¿�DM�� �����ϴ� ��¹��� ���ؼ��� DATA����
	if( sDM_USE == _T("Y") )
	{
		INT nTITLE_START_COL	= _ttoi(sTITLE_START_COL);
		INT nTITLE_END_COL		= _ttoi(sTITLE_END_COL);
		INT nTITLE_ROW			= _ttoi(sTITLE_ROW);
		CStringArray sColumns;
		CString strAlias = saTempData.GetAt(1);
		strAlias.Replace(_T(","), _T("\t"));
		pStringList.AddTail(strAlias);
		for(i = nTITLE_START_COL - 1; i < nTITLE_END_COL; i++)
		{
			CString strColumnData;
			AfxExtractSubString(strColumnData, saTempData.GetAt(2), i, ',');
			sColumns.Add(strColumnData);
		}
		
		CESL_DataMgr *pDM = m_pDM[0];
		INT nRow = pDM->GetRowCount();
		if(nRow > 0)
		{			
			CString strMsg;
			CString strResult;
			CString strTemp;
			for( i = 0; i < nRow; i++)
			{
				strTemp.Empty();
				for(j = 0; j < nTITLE_END_COL; j++)
				{
					CString sData;
					pDM->GetCellData( sColumns.GetAt(j), i, sData);
					/*
					if(sData.Find(_T("\t")) >= 0)
					{
						strTemp += _T("\"") + sData + _T("\"");
					}
					*/
					if(sData.Find(_T("\t")) >= 0)
					{
						sData.Replace(_T("\t"), _T(" "));
						strTemp += sData;
					}
					else
					{
						strTemp += sData;
					}

					if(j < nTITLE_END_COL - 1)
					{
						strTemp += _T("\t");
					}

				}
				pStringList.AddTail(strTemp);
			}		
		}
	}

	DeleteFile(sFile);
	CFile fCSVFile;
	if( fCSVFile.Open( sFile ,  CFile::modeCreate | CFile::modeWrite ) == FALSE ) 
	{
		AfxMessageBox( sFile );
		return -1010;
	}
	USHORT nShort = 0xfeff;  // Unicode BOM(Byte Order Mark).
	fCSVFile.Write(&nShort,2);
	POSITION pos = pStringList.GetHeadPosition();
	
	while(pos)
 	{
 		strLine = pStringList.GetNext(pos) + _T("\r\n");
		fCSVFile.Write(strLine, strLine.GetLength()*2);
 	}
	
	fCSVFile.Close();

 
	//////////////////////////////////////////////////////////////////////////////////////////////
	// 6. ȭ��ǥ�� �� Release
	// ȭ��ǥ��
	//HCellMgr->VisableExcel(TRUE);
	//HCellMgr->Release();
	//delete HCellMgr;
	//HCellMgr = NULL;

	return 0;
}

INT CSIReportManager::PrintOfferList()
{
	EFS_BEGIN
		INT ids;	
	CSIReportPreviewDlg dlg;
	dlg.m_pManager = this;
	
	m_nCurrentPreviewPage = -1;
	
	//++2008.09.30 UPDATE BY PWR {{++
	//EXCEL����� �����ϸ� EXCEL�̸������ �۵��Ѵ�
	CString sStyleDirectory = _T("..\\CFG\\RptStyle");	//���������ġ
	m_sStyleFile = sStyleDirectory + _T("\\") + m_strRPTALIAS + _T(".xls");	//�������	
	
	CFileFind fFind;
	if( !fFind.FindFile(m_sStyleFile) ) {
		MakeXMLPageFiles();
		if (dlg.DoModal() != IDOK) return 0;
	}
	else {
		BOOL	isExcel = TRUE, isHancell = TRUE;
		CLSID clsExcelApp, clsid;
		IDispatch *pExcelApp, *pXlApp;
		HRESULT hr = CLSIDFromProgID(L"Excel.Application", &clsExcelApp);
		if(FAILED(hr)) {
			isExcel = FALSE;
		}
		hr = CoCreateInstance(clsExcelApp, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void **)&pExcelApp);
		if(FAILED(hr)) {
			isExcel = FALSE;
		}
		// �ϴ� ������ ��ȿ�ϰ� �۾��Ѵ�.
		isHancell = FALSE;
		/*
		hr = CLSIDFromProgID(L"HCell.Application", &clsid);
		if(FAILED(hr)) {
			isHancell = FALSE;
		}
		hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void **)&pXlApp);
		if(FAILED(hr)) {
			isHancell = FALSE;
		}*/
		
		CString strNotify;
		if ( isExcel == TRUE && isHancell == TRUE )	{
			strNotify.Format(_T("�����Ǵ� �Ѽ��� ����մϴ�.\r\n\r\n")
				_T("��      : �������\r\n")
				_T("�ƴϿ�: �Ѽ����\r\n"));
			INT nType = AfxMessageBox(strNotify, MB_YESNOCANCEL);
			if ( nType == IDYES ) {
				ids = ExcelToExportOfferList();
				if(ids == 10) {
					MakeXMLPageFiles();
					if (dlg.DoModal() != IDOK) return 0;
				}
			}
			else if ( nType == IDNO ) {
				m_sStyleFile = sStyleDirectory + _T("\\") + m_strRPTALIAS + _T(".cell");	//�������
				ExportHancell();
			}
			else
				return -1;
		}
		else if ( isExcel == TRUE ) {
			ids = ExcelToExportOfferList();
			if(ids == 10) {
				MakeXMLPageFiles();
				if (dlg.DoModal() != IDOK) return 0;
			}
		}
		else if ( isHancell == TRUE ) {
			m_sStyleFile = sStyleDirectory + _T("\\") + m_strRPTALIAS + _T(".cell");	//�������
			ExportHancell();
		}
		else
		{
			AfxMessageBox(_T("��ġ�� ���� �Ǵ� �Ѽ��� ã���� �����ϴ�."));
			return 0;
		}		
	}	
	return 0;
	
	EFS_END
		return -1;
	
}

// �����θ� �۾��Ѵ�.
// 18.10.30 JMJ KOL.RED.2018.021
INT CSIReportManager::ExcelToExportOfferList()
{
	INT ids, i;
	INT nCnt = 0;
	CString sCnt;
	sCnt.Format(_T("%d"), nCnt);
	CString sTempDirectory = _T("rpt_tmp\\Excel");						//tmp������ġ
	CString sTempFileName = _T("tmp_ExcelExport");
	CString sCurrentFileName = sTempFileName+_T(".xls");
	CString sTempFile = sTempDirectory + _T("\\") + sCurrentFileName;	//tmp����
		

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 1. ExcelMgr���� �� �ʱ�ȭ
//	nSTART = GetTickCount();
	// COM ���� DLL�� �ε��ؼ� COM���� ��ɾ ����� �� �ֵ��� ���ش�
	CoInitialize(NULL);
	CExportToExcel *ExcelMgr;
	ExcelMgr = new CExportToExcel;
	// ���� ���� ��� ���� �ʱ�ȭ���� �����Ѵ�.
	ids = ExcelMgr->InitExcel();
	if( ids ) 
	{
		ExcelMgr->Release();
		delete ExcelMgr;
		ExcelMgr = NULL;
		return 10;
	}
//	nFINISH = GetTickCount();
//	TRACE2("1. ExcelMgr�������ʱ�ȭ[Time = %d.%03d sec]\r\n", (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000 );

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 2. tmp���� ���� �� ����
//	nSTART = GetTickCount();
	CString sErrorMsg;
	CFileFind fFind;
	if (!fFind.FindFile(sTempDirectory))
	{
		// ���丮 ������ ����
		if (!CreateDirectory(sTempDirectory, NULL))
		{
			//tmp���丮��������
			ExcelMgr->Release();
			delete ExcelMgr;
			ExcelMgr = NULL;

			TRACE(_T("Error Occured = tmp���丮��������\r\n"));
			AfxMessageBox(_T("tmp���丮 ������ �����߽��ϴ�."));
			return -1;
		}
	}
	// 2009.05.11 UPDATE BY PWR : ������� �ߺ������ tmp������ ���� ���鵵�� ����
	// tmp���� ����� ����
	while(fFind.FindFile(sTempFile))
	{
		CFileException e;
		CFile f;
		f.Open(sTempFile, CFile::modeRead | CFile::typeBinary, &e);
		//if(e.m_cause == CFileException::sharingViolation) // �̹� ����Ǿ��־� lock�ɸ� ����
		if(e.m_cause != 0) // ����ó���߻�
		{
			nCnt++;
			sCnt.Format(_T("%d"), nCnt);
			sCurrentFileName = sTempFileName + sCnt + _T(".xls");
			sTempFile = sTempDirectory + _T("\\") + sCurrentFileName;	//tmp����
		}
		else
		{
			f.Close();
			if(!DeleteFile(sTempFile))
			{
				//tmp���丮��������
				ExcelMgr->Release();
				delete ExcelMgr;
				ExcelMgr = NULL;

				TRACE(_T("Error Occured = tmp���ϻ�������\r\n"));
				AfxMessageBox(_T("����tmp���� ������ �����߽��ϴ�."));
				return -1;
			}
		}
	}
	// �ش� ��������� �������Ϸ� �����Ͽ� ����Ѵ�.
	ids = ExcelMgr->FileCopy( m_sStyleFile, sTempFile );
	if(ids<0) 
	{
		ExcelMgr->Release();
		delete ExcelMgr;
		ExcelMgr = NULL;

		TRACE(_T("Error Occured = FileCopy"));
		AfxMessageBox(_T("���Ϻ��翡 ���� �Ͽ����ϴ�.\r\n�ش�Excel������ ���μ����� ���������� Ȯ���� �ֽʽÿ�."));
		return -1;
	}
//	nFINISH = GetTickCount();
//	TRACE2("2. tmp���Ϻ���׻���[Time = %d.%03d sec]\r\n", (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000 );

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 3. ������ tmp���� ����
//	nSTART = GetTickCount();
	TCHAR TmpDir[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH, TmpDir);
	CString sFile;
	sFile.Format(_T("%s\\%s"), TmpDir, sTempFile);
	ExcelMgr->Open(sFile);
//	nFINISH = GetTickCount();
//	TRACE2("3. ������ tmp���� ����[Time = %d.%03d sec]\r\n", (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000 );

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 4. ȯ�漳�� ���� �� TEXT����(ġȯ)
//	nSTART = GetTickCount();
	//�޸� ��������(�޸� ȯ�漳���� �ִ�)
	//DM_USE@Y;MAX_ROW@6;USER_NAME@E;BOOK_TITLE@G;BOOK_AUTHOR@K;GIVE_DATE@K3;GIVELIB_GIVE@B6;GIVELIB_RETURN@H19;LOANLIB@H6;GIVEBOOKCNT@C12;
	CString sComment = ExcelMgr->GetComment(1,1);
	if( sComment.IsEmpty() )
	{
		ExcelMgr->Release();
		delete ExcelMgr;
		ExcelMgr = NULL;

		TRACE(_T("Error Occured = Comment Isimpty"));
		AfxMessageBox(_T("ȯ�漳���� �����ü� �����ϴ�.\r\n�ش� EXCEL����� �޸� Ȯ�����ֽʽÿ�."));
		return -1;
	}
	// ������ ����Ʈ����
	CStringArray sList;
	CString sToken = _T(";"); //������
	while(sComment.Find(sToken) >= 0)
	{
		INT nCnt = sComment.Find(sToken);
		CString sTemp = sComment.Left( nCnt+1 );
		sComment = sComment.Right( sComment.GetLength() - (nCnt+1) );	
		sTemp.Replace(sToken, _T(""));
		sTemp.TrimLeft(); sTemp.TrimRight();
		if(!sTemp.IsEmpty()) sList.Add(sTemp);
	}
	sToken = _T("@"); //������
	CString strMAX_ROW = _T(""), strSTART_ROW = _T(""), strUSER_NAME=_T(""), strBOOK_TITLE=_T(""), strBOOK_AUTHOR=_T("");
	CString strGIVE_DATE=_T(""), strGIVELIB_GIVE=_T(""), strGIVELIB_RETURN=_T(""), strLOANLIB=_T(""), strGIVEBOOKCNT=_T("");
	CString strDM_USE = _T("");
	
	for(i = 0; i < sList.GetSize(); i++)
	{
		CString sData = sList.GetAt(i);
		INT nIdx = sData.Find(sToken);
		CString sType = sData.Left(nIdx);
		if(sType == _T("DM_USE")) //DM�������
			strDM_USE = sData.Right( sData.GetLength() - (nIdx+1) );		
		else if(sType == _T("MAX_ROW")) //�ִ����
			strMAX_ROW = sData.Right( sData.GetLength() - (nIdx+1) );
		else if(sType == _T("START_ROW")) //�����
			strSTART_ROW = sData.Right( sData.GetLength() - (nIdx+1) );
		else if(sType == _T("USER_NAME")) //�̿��ڸ�
			strUSER_NAME = sData.Right( sData.GetLength() - (nIdx+1) );
		else if(sType == _T("BOOK_TITLE")) //����
			strBOOK_TITLE = sData.Right( sData.GetLength() - (nIdx+1) );
		else if(sType == _T("BOOK_AUTHOR")) //����
			strBOOK_AUTHOR = sData.Right( sData.GetLength() - (nIdx+1) );
		else if(sType == _T("GIVE_DATE")) //�߼���(����->��û)
			strGIVE_DATE = sData.Right( sData.GetLength() - (nIdx+1) );
		else if(sType == _T("GIVELIB_GIVE")) //����������(�߼۵�����)
			strGIVELIB_GIVE = sData.Right( sData.GetLength() - (nIdx+1) );
		else if(sType == _T("GIVELIB_RETURN")) //����������(���͵�����)
			strGIVELIB_RETURN = sData.Right( sData.GetLength() - (nIdx+1) );
		else if(sType == _T("LOANLIB")) //��û������(���⵵����)
			strLOANLIB = sData.Right( sData.GetLength() - (nIdx+1) );
		else if(sType == _T("GIVEBOOKCNT")) //�߼۵����Ǽ�
			strGIVEBOOKCNT = sData.Right( sData.GetLength() - (nIdx+1) );
	}

	// �޸������
	ExcelMgr->ClearComments(1,1);
//	nFINISH = GetTickCount();
//	TRACE2("4. ȯ�漳��������TEXT����(ġȯ)[Time = %d.%03d sec]\r\n", (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000 );

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 5. ��¿�DM�� �����ϴ� ��¹��� ���ؼ��� DATA����
//	nSTART = GetTickCount();
	CString sInsertPoint = _T(""), strData = _T(""), strData2 = _T(""), strPutData = _T("");
	INT nStartRow = 1;
	if( strDM_USE == _T("Y") ) {		
		CESL_DataMgr *pDM = m_pDM[0];
		INT nRow = pDM->GetRowCount();		
		
		for( i = 0; i < nRow; i++) {
			strData  = _T("");
			strData2 = _T("");
			pDM->GetCellData(_T("�̿��ڸ�"), i, strData);
			pDM->GetCellData(_T("�̿���_��ȣ"), i, strData2);
			strPutData.Format(_T("%s(%s)"), strData, strData2);
			sInsertPoint.Format(_T("%s%d"), strUSER_NAME, i+_ttoi(strSTART_ROW) );
			ExcelMgr->SetTextToExcel( sInsertPoint, strPutData);

			pDM->GetCellData(_T("����"), i, strPutData);
			sInsertPoint.Format(_T("%s%d"), strBOOK_TITLE, i+_ttoi(strSTART_ROW) );
			ExcelMgr->SetTextToExcel( sInsertPoint, strPutData);

			pDM->GetCellData(_T("��������"), i, strPutData);
			sInsertPoint.Format(_T("%s%d"), strBOOK_AUTHOR, i+_ttoi(strSTART_ROW) );
			ExcelMgr->SetTextToExcel( sInsertPoint, strPutData);
		}

		// �߼���
		pDM->GetCellData(_T("�߼���"), 0, strPutData);
		ExcelMgr->SetTextToExcel( strGIVE_DATE, strPutData);

		// ����������
		pDM->GetCellData(_T("����������_�̸�"), 0, strPutData);
		ExcelMgr->SetTextToExcel( strGIVELIB_GIVE, strPutData);
		ExcelMgr->SetTextToExcel( strGIVELIB_RETURN, strPutData);

		// ���⵵����
		pDM->GetCellData(_T("��û������_�̸�"), 0, strPutData);
		ExcelMgr->SetTextToExcel( strLOANLIB, strPutData);		

		// �߼۵����Ǽ�
		strPutData.Format(_T("%d"), nRow);
		ExcelMgr->SetTextToExcel( strGIVEBOOKCNT, strPutData);
	}	

//	nFINISH = GetTickCount();
//	TRACE2("5. DATA����[Time = %d.%03d sec]\r\n", (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000 );

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 6. ȭ��ǥ�� �� Release
//	nSTART = GetTickCount();
	// ȭ��ǥ��*/
	ExcelMgr->VisableExcel(TRUE);
	// Release
	ExcelMgr->Release();
	delete ExcelMgr;
	ExcelMgr = NULL;
//	nFINISH = GetTickCount();
//	TRACE2("6. ȭ��ǥ�ù�Release[Time = %d.%03d sec]\r\n", (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000 );

	return 0;
}

// 18.10.30 JMJ KOL.RED.2018.021
// KU0303 JMC 20170410 �������� ��·α� - ����.
void CSIReportManager::SetPrivacyPrintLogParm(BOOL bFlag, CString strKey, CString strID, CString strTitle, CString strManageCode, CString strLogMsg, INT nAccessLevel)
{
	m_bPrivacyPrintLogRecord = bFlag;
	m_strWorkerKey = strKey;
	m_strWorkerID = strID;
	m_strTitle = strTitle;
	m_strManageCode = strManageCode;
	m_strWorkLogMsg = strLogMsg;
	m_nPrivacyAccessLevel = nAccessLevel;

}

// 18.10.30 JMJ KOL.RED.2018.021
// KU0303 JMC 20170410 �������� ��·α� - ����.
void CSIReportManager::InitPrivacyPrintLogParm()
{
	m_bPrivacyPrintLogRecord = FALSE;
	m_strWorkerKey = _T("");
	m_strWorkerID = _T("");
	m_strTitle = _T("");
	m_strManageCode = _T("");
	m_strWorkLogMsg = _T("");
	m_nPrivacyAccessLevel = 0;
}

// 18.10.30 JMJ KOL.RED.2018.021
INT CSIReportManager::PrivacyPrintLogRecord(CESL_DataMgr *pDM, CString strPrintType)
{
EFS_BEGIN
	INT nProcCnt= 0;
	CString strQuery;
	CString strPrintKey;	

	//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰�
	CString strReason;
	if(strPrintType != _T("G"))
	{
		CESLDlg_UserExportReason Dlg;
		if(Dlg.DoModal() != IDOK)
		{
			AfxMessageBox(_T("����� ����մϴ�."));
			return -200;
		}
		strReason = Dlg.m_strReason;
	}
	else
	{
		strReason.Format(_T("KOLAS III - �Ϲ���ȸ"));
	}
	
	strQuery.Format(_T("SELECT ESL_SEQ.NEXTVAL FROM DUAL"));
	pDM->GetOneValueQuery(strQuery, strPrintKey);
	if(_T("") == strPrintKey) 
	{
		AfxMessageBox(_T("�α׻����� �����Ͽ����ϴ�."));
		return -100;
	}	
	
	CString strMaskingYN = _T("N");
	if(1 == m_nPrivacyAccessLevel) strMaskingYN = _T("Y");
	//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰�
	strQuery.Format(_T("INSERT INTO MN_PRIVACY_PRINT_LOG_TBL (REC_KEY, PRINT_USER_KEY, PRINT_USER_ID, PRINT_TYPE, PRINT_INFO, PRINT_DATE, PRINT_COUNT, MASKING_YN, FIRST_WORK, MANAGE_CODE, REASON) ")
					_T("VALUES (%s, %s, '%s', '%s', '%s', SYSDATE, %d, '%s', '%s', '%s', '%s');")
					,strPrintKey, m_strWorkerKey, m_strWorkerID, strPrintType, m_strTitle, pDM->GetRowCount(), strMaskingYN, m_strWorkLogMsg, m_strManageCode, strReason);
	pDM->StartFrame();
	pDM->AddFrame(strQuery);
	pDM->SendFrame();
	pDM->EndFrame();
	
	CString strDmAlais = pDM->GETDMAlias();
	CString strUserKeyAlias = _T("");
	
	//�󼼷α׸� ����ϱ� ���� ����ó��...�밡��..��...
	//������ DM���� �̿���KEY�� ����ִ� ALIAS�̸��� �ٸ���.
	if(_T("DM_BO_LOC_3300_LIST") == strDmAlais)	strUserKeyAlias = _T("REC_KEY");
	else if(_T("DM_BO_LOC_3300_PRINT") == strDmAlais)	strUserKeyAlias = _T("REC_KEY");
	else if(_T("DM_BO_LOC_3200") == strDmAlais)	strUserKeyAlias = _T("������KEY");
	else if(_T("DM_BO_LOC_RESERVE") == strDmAlais)	strUserKeyAlias = _T("USER_KEY");
	else if(_T("DM_BO_LOC_RESERVE_SE") == strDmAlais)	strUserKeyAlias = _T("USER_KEY");
	else if(_T("DM_BO_LOC_3400") == strDmAlais)	strUserKeyAlias = _T("�̿���KEY");				
	else if(_T("DM_SMS_DLG_01") == strDmAlais)	strUserKeyAlias = _T("USER_KEY");			
	else if(_T("DM_SMS_DLG_03_USER") == strDmAlais)	strUserKeyAlias = _T("USER_KEY");		
	else if(_T("DM_SMS_4100") == strDmAlais)	strUserKeyAlias = _T("�̿���KEY");							
	else if(_T("DM_BO_LOC_3100_MEMBER_INFO") == strDmAlais)	strUserKeyAlias = _T("������KEY");				
	else if(_T("DM_BOOK_COOPERATIVE_GROUP_OFFER") == strDmAlais)	strUserKeyAlias = _T("�̿���KEY");		
	else if(_T("DM_BOOK_COOPERATIVE_GROUP_REQUEST") == strDmAlais)	strUserKeyAlias = _T("�̿���KEY");		
	else if(_T("DM_BO_LOC_SECEDER_LIST") == strDmAlais)	strUserKeyAlias = _T("REC_KEY");	
	else if(_T("DM_BO_LOC_LOAN_HIS_USER") == strDmAlais)	strUserKeyAlias = _T("REC_KEY");	
	else if(_T("DM_USER_PRIVACY_ACCESS_INFO") == strDmAlais)	strUserKeyAlias = _T("USER_KEY");	
	else if(_T("DM_BO_LOC_3240_BO") == strDmAlais)	strUserKeyAlias = _T("������KEY");	
	else if(_T("DM_BO_LOC_3170_RESERVE_INFO") == strDmAlais)	strUserKeyAlias = _T("������KEY");	
	else if(_T("DM_FEE_INSERT_USER") == strDmAlais)	strUserKeyAlias = _T("REC_KEY");
	else if(_T("DM_SMSSendDlg") == strDmAlais)	strUserKeyAlias = _T("USER_KEY");	
	else if(_T("DM_FAMILY_ID_INFO") == strDmAlais)	strUserKeyAlias = _T("������KEY");		
	else if(_T("DM_FAMILY_LOAN_HISTORY") == strDmAlais)	strUserKeyAlias = _T("������KEY");
	//2019.07.17 JOB.2019.0059 ADD BY KYJ : �α� ��Ͻ� ���� �߰�
	else if(_T("DM_SMS_5120_KatokPL") == strDmAlais)	strUserKeyAlias = _T("�����̷�����KEY");		
	
	pDM->StartFrame();
	if(_T("") != strUserKeyAlias)
	{
		for(INT i=0; i<pDM->GetRowCount(); i++)
		{
			CString strUserKey;
			strUserKey = pDM->GetCellData(strUserKeyAlias, i);
			if(_T("") == strUserKey || _T("") == strPrintKey) continue;
			strQuery.Format(_T("INSERT INTO MN_PRIVACY_PRINT_DETAIL (REC_KEY, PRINT_KEY, USER_KEY) ")
				_T("VALUES (ESL_SEQ.NEXTVAL, %s, %s);")
				,strPrintKey, strUserKey);
			pDM->AddFrame(strQuery);
			// 50 �Ǹ��� Ŀ��.
			nProcCnt++;
			if ( nProcCnt%50 == 0 ) {
				pDM->SendFrame();
				pDM->EndFrame();
				pDM->StartFrame();
				pDM->InitDBFieldData();
			}
		}
	}
	pDM->SendFrame();
	pDM->EndFrame();
	return 0;
EFS_END
	return -1;
}