// ExportToHCell.cpp : implementation file

#include "stdafx.h"
#include "ExportToHCell.h"

/////////////////////////////////////////////////////////////////////////////
// CExportToExcel

// ������̼� ���� ��� �ʿ��� Ŭ������ ����.
COleVariant covTRUE((short)TRUE),
			covFALSE((short)FALSE),
			covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);


CExportToHCell::CExportToHCell()
{
	EnableAutomation();
}

CExportToHCell::~CExportToHCell()
{
}

void CExportToHCell::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CListCtrl::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CExportToHCell, CListCtrl)
	//{{AFX_MSG_MAP(CExportToExcel)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CExportToHCell, CListCtrl)
	//{{AFX_DISPATCH_MAP(CExportToExcel)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IExportToHCell to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {3BEF26D9-5712-4CF9-9E2C-2F2CFAEED53B}
static const IID IID_IExportToHCell =
{ 0x3bef26d9, 0x5712, 0x4cf9, { 0x9e, 0x2c, 0x2f, 0x2c, 0xfa, 0xee, 0xd5, 0x3b } };

BEGIN_INTERFACE_MAP(CExportToHCell, CListCtrl)
	INTERFACE_PART(CExportToHCell, IID_IExportToHCell, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportToHCell message handlers

// ���� ���� ��� ���� �ʱ�ȭ���� �����Ѵ�.
INT CExportToHCell::InitExcel()
{
	// Start HCcel and get Application object...
	// �Ѽ� ���ø����̼��� ����ġ ��Ų��.
	if(!m_app.CreateDispatch(_T("HCell.Application")))
	{
		AfxMessageBox(_T("�Ѽ��� ��ġ�Ǿ� ���� �ʽ��ϴ�."));
		return 10;
	}

	// �Ѽ� ���� �Ұ����� ���� ��� �߰�/�ȶ߰� (TRUE/FALSE) �ϱ�
	m_app.put_DisplayAlerts(TRUE);	
	
	m_nRow = 1;
	m_nColumn = 0;
	m_nCheckSheet = 1;

	return 0;
}

INT CExportToHCell::Open(CString sFileName)
{
	//�����Ҷ� ���ϸ��� �ִٸ� �ش� ���������� ����
	//���ϸ��� ���ٸ� �� ��Ʈ �����Ѵ�.	
	
	if( !sFileName.IsEmpty() )
	{		
		/*
		// �������߼ҽ�
		m_lpDisp = m_app.get_Workbooks();

		if (m_lpDisp == NULL) 
			return -1;

		m_objBooks.AttachDispatch(m_lpDisp);
		
		// �������Ͽ���
		m_lpDisp = m_objBooks.Open( COleVariant(sFileName),
								covOptional, covOptional, covOptional,
								covOptional, covOptional, covOptional,
								covOptional, covOptional, covOptional,
								covOptional, covOptional, covOptional,
								covOptional, covOptional);
		if (m_lpDisp == NULL) 
			return -1;
		
		m_objBook.AttachDispatch(m_lpDisp);

		m_lpDisp = m_objBook.get_Worksheets();
		if (m_lpDisp == NULL) 
			return -1;
		
		m_objSheets.AttachDispatch(m_lpDisp);
		
		m_lpDisp = m_objSheets.get_Item(COleVariant((short)1));
		if (m_lpDisp == NULL) 
			return -1;
		
		m_objSheet.AttachDispatch(m_lpDisp);
		// ��Ʈ ����		
		*/
		m_objBooks	= m_app.get_Workbooks();	

		LPDISPATCH l_lpDisp;
		// �������Ͽ���
		l_lpDisp= m_objBooks.Open( COleVariant(sFileName),
								covOptional, covOptional, covOptional,
								covOptional, covOptional, covOptional,
								covOptional, covOptional, covOptional,
								covOptional, covOptional, covOptional,
								covOptional, covOptional);

		m_objBook.AttachDispatch(l_lpDisp);
		l_lpDisp = m_objBook.get_Sheets();
		m_objSheets.AttachDispatch(l_lpDisp);
		m_objSheets	= m_objBook.get_Worksheets();
		m_objSheet	= m_objSheets.get_Item(COleVariant((short)1));
		m_lpDisp = l_lpDisp;
	}
	else
	{
		m_lpDisp = m_app.get_Workbooks();

		if (m_lpDisp == NULL) 
			return -1;

		m_objBooks.AttachDispatch(m_lpDisp);
		
		m_lpDisp = m_app.get_ActiveWorkbook();
		if (m_lpDisp == NULL) 
			return -1;
		
		m_objBook.AttachDispatch(m_lpDisp);

		m_lpDisp = m_objBook.get_Worksheets();
		if (m_lpDisp == NULL) 
			return -1;
		
		m_objSheets.AttachDispatch(m_lpDisp);
		
		m_lpDisp = m_objSheets.get_Item(COleVariant((short)1));
		if (m_lpDisp == NULL) 
			return -1;
		
		m_objSheet.AttachDispatch(m_lpDisp);
	}

	return 0;
}

// ���� ��ο� �̸��� �ް� �� �̸����� ������ ��Ű�� �Լ�.
INT CExportToHCell::SaveAs()
{
	SelectLocation();
	Save();
	return 0;
}

// ��Ʈ�� ���� ��Ʈ�� �Ѱ��ش�. �θ��� ������ ���Լ��� ȣ��Ǹ� �ο츦 1�� �����ؾ��Ѵ�.
INT CExportToHCell::NextSheet()
{
	VARIANT     varValue;
	COleVariant varCount(m_objSheets.get_Count());
	V_VT(&varValue)       = VT_DISPATCH;
	V_DISPATCH(&varValue) = m_objSheets.get_Item(varCount);
	
	if ( m_nCheckSheet != m_objSheets.get_Count())
	{
		m_nCheckSheet++;
		m_objSheets	= m_objSheets.get_Item(COleVariant((short)m_nCheckSheet));
	}			
	else if( m_nCheckSheet == m_objSheets.get_Count())
	{
		m_objSheets = m_objSheets.Add(covOptional, varValue, covOptional, covOptional);
		m_nCheckSheet++; 
	}
	else
		return -15;
	
	return 0;
}

// ���ϰ�θ� �޾� m_strFileName�� �����ϴ� �Լ�.
INT CExportToHCell::SelectLocation()
{
	CString strDir;

	// ���ϰ�� â �ٿ��� ��θ� ������ �̸� ���� �ޱ�.
	CFileDialog dlg(FALSE, NULL, _T("*.cell"), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
		_T("�Ѽ� ���� ����(*.cell)|*.cell|��� ����|*.*||"), this);
	if(dlg.DoModal() != IDOK)
	{
		MessageBox(_T("�Ѽ� ����� ��ҵǾ����ϴ�."), _T("Warnning!"), MB_ICONEXCLAMATION);
		SetCurrentDirectory(strDir);
        return -5;
	}
	
	// ���� ��� m_strFileName�� ����.
	m_strFileName.Format(_T("%s"), dlg.GetPathName(), dlg.GetFileName());
	return 0;
}

// m_strFileName �̸����� ���������ϴ� �Լ�.
INT CExportToHCell::Save()
{
	// m_strFileName �̸����� �����ϱ�.
	m_objBook.SaveAs(COleVariant(m_strFileName), 
		covOptional,
		covOptional,
		covOptional,
		covOptional,
		covOptional,
		0,
		covOptional,
		covOptional,
		covOptional,
		covOptional,
		covOptional); 

	return 0;
}

INT CExportToHCell::Save(CString szFileName)
{
	RangeSelect(1,1);
	m_strFileName = szFileName;
	szFileName.MakeLower();

	int nFileFormat = -4143;  // HCell Format : -4143, Excel Format : 43
	if (szFileName.Right(3) == _T("xls"))
	{
		nFileFormat = 43;
	}

	m_app.put_DisplayAlerts(FALSE);

	// szFileName �̸����� �����ϱ�.
	m_objBook.SaveAs(COleVariant(szFileName), 
		COleVariant((short)nFileFormat),
		covOptional,
		covOptional,
		covOptional,
		covOptional,
		1,
		covOptional,
		covOptional,
		covOptional,
		covOptional,
		covOptional); 

	m_app.put_DisplayAlerts(TRUE);
	return 0;
}

INT CExportToHCell::SetTextToHCell(INT nExRow, INT nExCol, CString strData, CString DataType, CListCtrl *pListCtrl)
{
	if ( _EXCEL_MAXCOLUMN < nExCol )
		return 10;	

	RangeSelect(nExCol, nExRow);

	m_objRange.put_Value(covOptional, COleVariant(strData));

	m_lpDisp = m_objRange.get_Font();
	if (m_lpDisp == NULL) 
		return -1;

	CFont0 objFont;
	objFont.AttachDispatch(m_lpDisp);
	objFont.put_Bold(covTRUE);
	objFont.ReleaseDispatch();

	m_objRange.put_ColumnWidth(COleVariant(short(pListCtrl->GetColumnWidth(nExCol) / 9)));
	m_objRange.put_Value(covOptional, COleVariant(strData));
	return 0;
}

INT CExportToHCell::SetTextToHCell(INT nExRow, INT nExCol, CString strData, CString DataType, INT ColmnWidth)
{
	if ( _EXCEL_MAXCOLUMN < nExCol )
		return 10;	

	RangeSelect(nExCol, nExRow);

	m_lpDisp = m_objRange.get_Font();
	if (m_lpDisp == NULL) 
		return -1;
	
	CFont0 objFont;
	objFont.AttachDispatch(m_lpDisp);
	objFont.put_Bold(covTRUE);
	objFont.ReleaseDispatch();

	m_objRange.put_ColumnWidth(COleVariant(short(ColmnWidth)));
	m_objRange.put_Value(covOptional, COleVariant(strData));

	return 0;
}

INT CExportToHCell::SetTextToHCell(INT nExRow, INT nExCol, CString strData)
{
	if ( _EXCEL_MAXCOLUMN < nExCol )
		return 10;	

	RangeSelect(nExCol, nExRow);

	m_objRange.put_Value(covOptional, COleVariant(strData));

	return 0;
}

INT CExportToHCell::SetTextToHCell(CString sPoint, CString strData)
{
	RangeSelect(sPoint);
	m_objRange.put_Value(covOptional, COleVariant(strData));

	return 0;
}

// colesafearray������ m_objSafeArray�� �����ϰ� �ʱ�ȭ�Ѵ�. 
INT CExportToHCell::InitArray(INT nMaxRow, INT nMaxCol)
{	
	DWORD numElements[]={nMaxRow,nMaxCol};   //element array
    m_objSafeArray.Create(VT_BSTR, 2, numElements);
	return 0;
}

// colesafearray������ m_objSafeArray �� ��� ���ڸ� ������ �����Ѵ�.
INT CExportToHCell::PutAllbyArray(INT nExRow, INT nExCol, INT nMaxRow, INT nMaxCol)
{
	CString strMax;
	
	// A1~ IV65536 ������ ���� ��ġ�� ��Ʈ������ �����Ѵ�. 
	// ���� ��ġ
	if ( _EXCEL_NALPHAMAX_ < nExCol-1 )
		m_strLocation.Format(_T("%c%c%d"), 'A' + (nExCol-1/_EXCEL_NALPHAMAX_), 'A' + (nExCol-1%_EXCEL_NALPHAMAX_), nExRow);
	else
		m_strLocation.Format(_T("%c%d"), 'A' + nExCol-1, nExRow);

	// ������ ��ġ
	if ( _EXCEL_NALPHAMAX_ < nExCol + nMaxCol-2 )
		strMax.Format(_T("%c%c%d"), 'A' + (nExCol + nMaxCol-2/_EXCEL_NALPHAMAX_), 'A' + (nExCol + nMaxCol-2%_EXCEL_NALPHAMAX_), nExRow + nMaxRow-1);
	else
		strMax.Format(_T("%c%d"), 'A' + nExCol + nMaxCol-2, nExRow + nMaxRow-1);

	m_lpDisp = m_objSheet.get_Range(COleVariant(m_strLocation), COleVariant(strMax));
	if (m_lpDisp == NULL) 
		return -1;
	m_objRange.AttachDispatch(m_lpDisp);
	m_objRange.put_Value(covOptional, COleVariant(m_objSafeArray));

	m_objSafeArray.Detach();
	m_objSafeArray.Destroy();

	// ���� ũ�⸦ �����Ϳ� �����. (�÷����� �÷� ũ�⸦ �޾� ó���Ͽ���)
//	Range colRange(m_objRange.GetColumns());
//	colRange.AutoFit();
	return 0;
}

// colesafearray������ m_objSafeArray �� ���ڸ� �����Ѵ�.
INT CExportToHCell::FillSafeArray(INT nRow, INT nCol, CString strValue)
{
	if ( _EXCEL_MAXCOLUMN < nCol + 1 )
		return 10;

	VARIANT vTemp;
    long index[2]; 

    index[0] = nRow;
    index[1] = nCol;	
	
    VariantInit(&vTemp);
    vTemp.vt = VT_BSTR;
	vTemp.bstrVal = SysAllocString(strValue);
    m_objSafeArray.PutElement(index, vTemp.bstrVal);

    SysFreeString(vTemp.bstrVal);
    VariantClear(&vTemp);
	return 0;
}

// ��Ʈ ����
// DESC : ������ �޾Ƽ� nBold, nItalic, clrForeColor, nSize�� ��Ʈ�� �����Ѵ�.
INT CExportToHCell::SetFont(INT nMinCol, INT nMinRow, INT nMaxCol, INT nMaxRow, BOOL bBold, BOOL bItalic, COLORREF clrForeColor, INT nSize)
{	
	if(nMaxCol > 0 && nMaxRow > 0)
		RangeSelect(nMinCol, nMinRow, nMaxCol, nMaxRow);
	else
		RangeSelect(nMinCol, nMinRow);

	// ��Ʈ ����	
	m_lpDisp = m_objRange.get_Font();
	if (m_lpDisp == NULL) 
		return -1;

	CFont0 objFont;
	objFont.AttachDispatch(m_lpDisp);
	objFont.put_Bold(COleVariant((short)bBold));
	objFont.put_Italic(COleVariant((short)bItalic));
	objFont.put_Color(COleVariant((double)clrForeColor));
	objFont.put_Size(COleVariant((short)nSize));
	objFont.ReleaseDispatch();

	return 0;
}

// �׵θ� ����
// DESC : ������ �޾Ƽ� nLineStyle(������)�� nWeight(����)�� �׵θ��� �׸���.
INT CExportToHCell::SetBorder(INT nMinCol, INT nMinRow, INT nMaxCol, INT nMaxRow, INT nLineStyle, INT nWeight)
{
	RangeSelect(nMinCol, nMinRow, nMaxCol, nMaxRow);

	// �׵θ�ǥ��
	m_lpDisp = m_objRange.get_Borders();
	if (m_lpDisp == NULL) 
		return -1;

	CBorders objBorders;
	objBorders.AttachDispatch(m_lpDisp);
	objBorders.put_LineStyle(COleVariant((short)nLineStyle));
	objBorders.put_Weight(COleVariant((short)nWeight));
	objBorders.ReleaseDispatch();

	return 0;
}

// ���� ����
// DESC : ������ �޾Ƽ� nAlign(����)�� ���������� �Ѵ�. (Gerneral:1, Left:-4131, Center:-4108, Rignt:-4152)
INT CExportToHCell::SetHorizAlign(INT nMinCol, INT nMinRow, INT nMaxCol, INT nMaxRow, INT nAlign)
{
	RangeSelect(nMinCol, nMinRow, nMaxCol, nMaxRow);
	
	// ��������	
	m_objRange.put_HorizontalAlignment(COleVariant((short)nAlign));

	return 0;
}

// �� ���� ����
// DESC : ������ �޾Ƽ� clrBackColr(����)�� �� ����� ä���. 
INT CExportToHCell::SetBkColor(INT nMinCol, INT nMinRow, INT nMaxCol, INT nMaxRow, COLORREF clrBkColor)
{
	RangeSelect(nMinCol, nMinRow, nMaxCol, nMaxRow);

	// ���� ����
	m_lpDisp = m_objRange.get_Interior();
	if (m_lpDisp == NULL) 
		return -1;

	Cnterior objInterior;
	objInterior.AttachDispatch(m_lpDisp);
	objInterior.put_Color(COleVariant((double)clrBkColor));
	objInterior.ReleaseDispatch();

	return 0;
}

// Ʋ����
INT CExportToHCell::SetFreezePanes(INT nCol, INT nRow, BOOL bValue)
{
	RangeSelect(nCol, nRow);

	m_lpDisp = m_app.get_ActiveWindow();
	if (m_lpDisp == NULL) 
		return -1;

	CWindow0 objWindow(m_lpDisp);	
	objWindow.put_FreezePanes(bValue);
	objWindow.ReleaseDispatch();

	return 0;
}

// Ư������ ���� ��������. 
CString CExportToHCell::GetValue(INT nCol, INT nRow)
{	
	RangeSelect(nCol, nRow);
	
	COleVariant sTemp(m_objRange.get_Text());	//�Ѽ��߰�����
	CString sData(sTemp.bstrVal);

	return sData;
}

// Sheet ������ ���Ѵ�.
long CExportToHCell::GetSheetCount()
{
	if(m_objSheets==NULL)
		return 0;

	return m_objSheets.get_Count();
}

// nIndex�� Sheet�̸��� ���Ѵ�.
CString CExportToHCell::GetSheetName(INT nIndex)
{
	if(nIndex < 1 || GetSheetCount() < nIndex)
		return CString(_T("FAILED"));

	m_lpDisp = m_objSheets.get_Item(COleVariant((short)(nIndex)));
	if (m_lpDisp == NULL) 
		return CString(_T("FAILED"));
	m_objSheet.AttachDispatch(m_lpDisp);
	return m_objSheet.get_Name();
}

// nIndex�� Sheet�̵�
VOID CExportToHCell::SetActiveSheet(INT nIndex)
{
	if(nIndex < 1 || GetSheetCount() < nIndex)
		return;
	
	m_lpDisp = m_objSheets.get_Item(COleVariant((short)(nIndex)));
	if (m_lpDisp == NULL) 
		return;
	m_objSheet.AttachDispatch(m_lpDisp);
	m_objSheet.Activate();
}

// ġȯ�ϱ�
BOOL CExportToHCell::Replace(CString sWhat, CString sReplacement)
{
	RangeSelect(1, 1);

	return m_objRange.Replace(COleVariant(sWhat), COleVariant(sReplacement), 
		COleVariant((short)2), COleVariant((short)1), COleVariant((short)0),COleVariant((short)0),
		COleVariant((short)0),COleVariant((short)0));
}

// ���Ϻ���
// DESC : sStyleFile(�������)�� sTempFile(��������)�� ����.
INT CExportToHCell::FileCopy(CString sStyleFile, CString sTempFile ){
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

// �޸� ����
// DESC: nCol, nRow (��������ġ)
INT CExportToHCell::AddComment(INT nCol, INT nRow, CString sValue)
{
	//�Ѽ�������������
	/* 
	RangeSelect(nCol, nRow);
	LPDISPATCH l_lpDisp(m_objRange.GetComment());
	m_objComment.AttachDispatch(l_lpDisp);
	//m_objComment.SetComment(sValue);
	*/
	return 0;
}

// �޸� ��������
// DESC: nCol, nRow (������ �޸���ġ)
CString CExportToHCell::GetComment(INT nCol, INT nRow)
{
	m_lpDisp = m_objSheet.get_Comments();
	if (m_lpDisp == NULL) 
		return _T("");

	m_objComments.AttachDispatch(m_lpDisp);

	if (m_lpDisp == NULL) 
		return _T("");
		
	int nComments = (int)m_objComments.get_Count();
	if(nComments <= 0) 
		return _T("");

	RangeSelect(nCol, nRow);
	
	m_lpDisp = m_objRange.get_Comment();
	if (m_lpDisp == NULL) 
		return _T("");

	m_objComment.AttachDispatch(m_lpDisp);
	return m_objComment.Text(covOptional, covOptional, covOptional);
}

// �޸������
INT CExportToHCell::ClearComments(INT nCol, INT nRow)
{
	RangeSelect(nCol, nRow);
	m_objRange.ClearComments();

	return 0;
}

// ��������
INT CExportToHCell::GetRange(INT nMinCol, INT nMinRow, INT nMaxCol, INT nMaxRow)
{
	// �� ��ĭ
	if(nMaxCol == 0 || nMaxRow == 0)
	{
		RangeSelect(nMinCol, nMinRow);
	}
	else//�� ����
	{
		CString strMax;
		
		// A1~ IV65536 ������ ���� ��ġ�� ��Ʈ������ �����Ѵ�. 
		// ���� ��ġ
		if ( _EXCEL_NALPHAMAX_ < nMinCol-1 )
			m_strLocation.Format(_T("%c%c%d"), 'A' + (nMinCol-1/_EXCEL_NALPHAMAX_), 'A' + (nMinCol-1%_EXCEL_NALPHAMAX_), nMinRow);
		else
			m_strLocation.Format(_T("%c%d"), 'A' + nMinCol-1, nMinRow);

		// ������ ��ġ (������ġ���� Max��ŭ��)
		if ( _EXCEL_NALPHAMAX_ < nMinCol + nMaxCol-2 )
			strMax.Format(_T("%c%c%d"), 'A' + (nMinCol + nMaxCol-2/_EXCEL_NALPHAMAX_), 'A' + (nMinCol + nMaxCol-2%_EXCEL_NALPHAMAX_), nMinRow + nMaxRow-1);
		else
			strMax.Format(_T("%c%d"), 'A' + nMinCol + nMaxCol-2, nMinRow + nMaxRow-1);

		m_lpDisp = m_objSheet.get_Range(COleVariant(m_strLocation), COleVariant(strMax));
		if (m_lpDisp == NULL) 
			return -1;
		m_objRange.AttachDispatch(m_lpDisp);
	}

	return 0;
}

// ������
INT CExportToHCell::RangeSelect(INT nMinCol, INT nMinRow, INT nMaxCol, INT nMaxRow)
{
	CString strMax;

	// A1~ IV65536 ������ ���� ��ġ�� ��Ʈ������ �����Ѵ�. 
	// ���� ��ġ
	if ( _EXCEL_NALPHAMAX_ < nMinCol-1 )
		m_strLocation.Format(_T("%c%c%d"), 'A' + (nMinCol-1/_EXCEL_NALPHAMAX_), 'A' + (nMinCol-1%_EXCEL_NALPHAMAX_), nMinRow);
	else
		m_strLocation.Format(_T("%c%d"), 'A' + nMinCol-1, nMinRow);

	// ������ ��ġ (������ġ���� Max��ŭ��)
	if ( _EXCEL_NALPHAMAX_ < nMinCol + nMaxCol-2 )
		strMax.Format(_T("%c%c%d"), 'A' + (nMinCol + nMaxCol-2/_EXCEL_NALPHAMAX_), 'A' + (nMinCol + nMaxCol-2%_EXCEL_NALPHAMAX_), nMinRow + nMaxRow-1);
	else
		strMax.Format(_T("%c%d"), 'A' + nMinCol + nMaxCol-2, nMaxRow);

	// ��������
    m_lpDisp = m_objSheet.get_Range(COleVariant(m_strLocation), COleVariant(strMax));
	if (m_lpDisp == NULL) 
		return -1;

	m_objRange.AttachDispatch(m_lpDisp);
	m_objRange.Select();

	return 0;
}

INT CExportToHCell::RangeSelect(INT nCol, INT nRow)
{
	// A1~ IV65536 ������ ���� ��ġ�� ��Ʈ������ �����Ѵ�. 
	if ( _EXCEL_NALPHAMAX_ < nCol-1 )
		m_strLocation.Format(_T("%c%c%d"), 'A' + (nCol-1/_EXCEL_NALPHAMAX_), 'A' + (nCol-1%_EXCEL_NALPHAMAX_), nRow);
	else
		m_strLocation.Format(_T("%c%d"), 'A' + nCol-1, nRow);
	
	m_lpDisp = m_objSheet.get_Range(COleVariant(m_strLocation), COleVariant(m_strLocation));
	if (m_lpDisp == NULL) 
		return -1;
	
	m_objRange.AttachDispatch(m_lpDisp);
	m_objRange.Select();
	
	return 0;
}

INT CExportToHCell::RangeSelect(CString sValue)
{
	m_lpDisp = m_objSheet.get_Range(COleVariant(sValue), COleVariant(sValue));
	if (m_lpDisp == NULL) 
		return -1;

	m_objRange.AttachDispatch(m_lpDisp);
	m_objRange.Select();

	return 0;
}

// ��� ���ǵ��� �ݾ��ְ� ����ġ�� ������ ���ִ� ��.
INT CExportToHCell::Close()
{
	// ���� �ݾ��ְ�
	m_objBook.Close(covOptional, COleVariant(m_strFileName), covOptional); 
	m_objBooks.Close();
	
	// ������ ����ġ�� ���ش�.
	m_objComment.ReleaseDispatch();
	m_objComments.ReleaseDispatch();	
	m_objRange.ReleaseDispatch();
	m_objSheet.ReleaseDispatch();
	m_objSheets.ReleaseDispatch();
	m_objBook.ReleaseDispatch();
	m_objBooks.ReleaseDispatch();
	
	// ���ø����̼� ����.
	m_app.Quit();
	m_app.ReleaseDispatch();	

	return 0;
}

INT CExportToHCell::Release()
{
	// ������ ����ġ�� ���ش�.
	m_objComment.ReleaseDispatch();
	m_objComments.ReleaseDispatch();		
	m_objRange.ReleaseDispatch();
	m_objSheet.ReleaseDispatch();
	m_objSheets.ReleaseDispatch();
	m_objBook.ReleaseDispatch();
	m_objBooks.ReleaseDispatch();
	m_app.ReleaseDispatch();	
	return 0;
}

// �Ѽ��� ȭ�鿡 ��� ���̰� ���ִ� �Լ�.
INT CExportToHCell::VisableExcel(BOOL bValue)
{
	// ������ ����. ���̰� ���ش�.
	if(bValue)
		m_app.put_Visible(TRUE);
	else
		m_app.put_Visible(FALSE);

	return 0;
}