// ExcelManager.cpp: implementation of the CExcelManager class.
//
//////////////////////////////////////////////////////////////////////
#include <comdef.h>

#include "stdafx.h"
#include "ExcelManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define _MAX_SHORT	32767
#define _MIN_SHORT	-32768


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExcelManager::CExcelManager(C_ExcelManControl *control)
{
	m_Excel = control;
	IsOpen = false;
	IsInit = true;
	m_Excel -> SetMode((short)0);
}

CExcelManager::CExcelManager()
{
	IsInit = false;
	IsOpen = false;
}

VOID CExcelManager::SetManager(C_ExcelManControl * control)
{
	m_Excel = control;
	IsOpen = false;
	IsInit = true;
	m_Excel -> SetMode(0);
}

CExcelManager::~CExcelManager()
{
	if(m_Excel) {
		m_Excel -> CloseManager();
		m_Excel = NULL;
	}
}

bool CExcelManager::OpenManager()
{
	if(IsOpen || IsInit == false) return false;

	if(m_bUseTemplate) {
		if(m_Excel -> OpenManager() == FALSE) 
			return false;
	}
	else {
		if(m_Excel -> OpenManager() == FALSE) 
			return false;
	}
	return true;
}

VOID CExcelManager::SetUseTemplate(bool IsUse, short Index)
{
	if(IsOpen || IsInit == false) return;

	m_bUseTemplate = IsUse;

	if(m_bUseTemplate) {
		// Index�� Template ���ϸ����� ��ȯ
		m_sTemplate = _T("c:\\kiniti\\reference\\report\\Template\\���� ���๰ ���� ��Ȳ-���� �����.xls");
	}
	m_Excel -> SetFileName(m_sTemplate);
	m_Excel -> SetMode(1);
}

VOID CExcelManager::SetUseTemplate(bool IsUse, TCHAR *File)
{
	if(IsOpen || IsInit == false) return;

	m_bUseTemplate = IsUse;

	if(m_bUseTemplate) {
		// Index�� Template ���ϸ����� ��ȯ
		m_sTemplate = File;
	}
	m_Excel -> SetFileName(m_sTemplate);
	m_Excel -> SetMode(1);
}

VOID CExcelManager::SetUseTemplate(bool IsUse, CString &File)
{
	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return;
	}

	m_bUseTemplate = IsUse;

	if(m_bUseTemplate) {
		// Index�� Template ���ϸ����� ��ȯ
		m_sTemplate = File;
	}
	m_Excel -> SetFileName(m_sTemplate);
	m_Excel -> SetMode(1);
}

bool CExcelManager::SaveAs(CString File)
{
	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return false;
	}

	m_Excel -> SaveAs(File);
	return true;
}

bool CExcelManager::Find(CRect rect, CString value, POINT &point)
{
	CString result, startx, starty, endx, endy;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return false;
	}

#ifdef _DEBUG
	if(rect.left < _MIN_SHORT || rect.left > _MAX_SHORT || rect.top < _MIN_SHORT || rect.top > _MAX_SHORT || rect.right < _MIN_SHORT || rect.right > _MAX_SHORT  || rect.bottom < _MIN_SHORT || rect.bottom > _MAX_SHORT) {
		AfxMessageBox(_T("Find���� ��ǥ���� ����(-32768 ~ 32767)�� �Ѿ���ϴ�."));
		return false;
	}
#endif
	startx.Format(_T("%d"), rect.left);
	starty.Format(_T("%d"), rect.top);
	endx.Format(_T("%d"), rect.right);
	endy.Format(_T("%d"), rect.bottom);
	result = m_Excel -> Find_String(startx, starty, endx, endy, value);	

	if(result.IsEmpty()) {
		return false;
	}
	
	result.Replace(_T("$"), _T(" "));
	result.TrimLeft();
	result.TrimRight();

	AfxMessageBox(result);
	return true;
}

bool CExcelManager::Find(RECT rect, CString value, POINT &point)
{
	CString result, startx, starty, endx, endy;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return false;
	}

#ifdef _DEBUG
	if(rect.left < _MIN_SHORT || rect.left > _MAX_SHORT || rect.top < _MIN_SHORT || rect.top > _MAX_SHORT || rect.right < _MIN_SHORT || rect.right > _MAX_SHORT  || rect.bottom < _MIN_SHORT || rect.bottom > _MAX_SHORT) {
		AfxMessageBox(_T("Find���� ��ǥ���� ����(-32768 ~ 32767)�� �Ѿ���ϴ�."));
		return false;
	}
#endif
	startx.Format(_T("%d"), rect.left);
	starty.Format(_T("%d"), rect.top);
	endx.Format(_T("%d"), rect.right);
	endy.Format(_T("%d"), rect.bottom);

	result = m_Excel -> Find_String(startx, starty, endx, endy, value);

	if(result.IsEmpty()) {
		return false;
	}
	
	result.Replace(_T("$"), _T(" "));
	result.TrimLeft();
	result.TrimRight();
	
	AfxMessageBox(result);
	return true;
}

bool CExcelManager::FindNext(CRect rect, POINT &point)
{
	CString result, startx, starty, endx, endy;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return false;
	}

#ifdef _DEBUG
	if(rect.left < _MIN_SHORT || rect.left > _MAX_SHORT || rect.top < _MIN_SHORT || rect.top > _MAX_SHORT || rect.right < _MIN_SHORT || rect.right > _MAX_SHORT  || rect.bottom < _MIN_SHORT || rect.bottom > _MAX_SHORT) {
		AfxMessageBox(_T("FindNext���� ��ǥ���� ����(-32768 ~ 32767)�� �Ѿ���ϴ�."));
		return false;
	}
#endif

	startx.Format(_T("%d"), rect.left);
	starty.Format(_T("%d"), rect.top);
	endx.Format(_T("%d"), rect.right);
	endy.Format(_T("%d"), rect.bottom);

	result = m_Excel -> FindNext_String(startx, starty, endx, endy);	

	if(result.IsEmpty()) {
		return false;
	}
	
	result.Replace(_T("$"), _T(" "));
	result.TrimLeft();
	result.TrimRight();

	AfxMessageBox(result);
	return true;
}

bool CExcelManager::FindNext(RECT rect, POINT &point)
{
	CString result, startx, starty, endx, endy;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return false;
	}

#ifdef _DEBUG
	if(rect.left < _MIN_SHORT || rect.left > _MAX_SHORT || rect.top < _MIN_SHORT || rect.top > _MAX_SHORT || rect.right < _MIN_SHORT || rect.right > _MAX_SHORT  || rect.bottom < _MIN_SHORT || rect.bottom > _MAX_SHORT) {
		AfxMessageBox(_T("FindNext���� ��ǥ���� ����(-32768 ~ 32767)�� �Ѿ���ϴ�."));
		return false;
	}
#endif
	startx.Format(_T("%d"), rect.left);
	starty.Format(_T("%d"), rect.top);
	endx.Format(_T("%d"), rect.right);
	endy.Format(_T("%d"), rect.bottom);

	result = m_Excel -> FindNext_String(startx, starty, endx, endy);

	if(result.IsEmpty()) {
		return false;
	}
	
	result.Replace(_T("$"), _T(" "));
	result.TrimLeft();
	result.TrimRight();
	
	AfxMessageBox(result);
	return true;
}

bool CExcelManager::Replace(RECT rect, CString search_str, CString replace_str)
{
	BOOL result;
	CString startx, starty, endx, endy;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return false;
	}

#ifdef _DEBUG
	if(rect.left < _MIN_SHORT || rect.left > _MAX_SHORT || rect.top < _MIN_SHORT || rect.top > _MAX_SHORT || rect.right < _MIN_SHORT || rect.right > _MAX_SHORT  || rect.bottom < _MIN_SHORT || rect.bottom > _MAX_SHORT) {
		AfxMessageBox(_T("Replace���� ��ǥ���� ����(-32768 ~ 32767)�� �Ѿ���ϴ�."));
		return false;
	}
#endif
	startx.Format(_T("%d"), rect.left);
	starty.Format(_T("%d"), rect.top);
	endx.Format(_T("%d"), rect.right);
	endy.Format(_T("%d"), rect.bottom);

	result = m_Excel -> Replace_String(startx, starty, endx, endy, search_str, replace_str);

	if(result == FALSE) return false;
	return true;
}
bool CExcelManager::Replace(CRect rect, CString search_str, CString replace_str)
{
	BOOL result;
	CString startx, starty, endx, endy;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return false;
	}

#ifdef _DEBUG
	if(rect.left < _MIN_SHORT || rect.left > _MAX_SHORT || rect.top < _MIN_SHORT || rect.top > _MAX_SHORT || rect.right < _MIN_SHORT || rect.right > _MAX_SHORT  || rect.bottom < _MIN_SHORT || rect.bottom > _MAX_SHORT) {
		AfxMessageBox(_T("Replace���� ��ǥ���� ����(-32768 ~ 32767)�� �Ѿ���ϴ�."));
		return false;
	}
#endif
	startx.Format(_T("%d"), rect.left);
	starty.Format(_T("%d"), rect.top);
	endx.Format(_T("%d"), rect.right);
	endy.Format(_T("%d"), rect.bottom);

	result = m_Excel -> Replace_String(startx, starty, endx, endy, search_str, replace_str);

	if(result == FALSE) return false;	
	return true;
}

bool CExcelManager::Insert_Rows(RECT rect, short direction)
{
	BOOL result;
	CString startx, starty, endx, endy;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return false;
	}

#ifdef _DEBUG
	if(rect.left < _MIN_SHORT || rect.left > _MAX_SHORT || rect.top < _MIN_SHORT || rect.top > _MAX_SHORT || rect.right < _MIN_SHORT || rect.right > _MAX_SHORT  || rect.bottom < _MIN_SHORT || rect.bottom > _MAX_SHORT) {
		AfxMessageBox(_T("Replace���� ��ǥ���� ����(-32768 ~ 32767)�� �Ѿ���ϴ�."));
		return false;
	}
#endif
	startx.Format(_T("%d"), rect.left);
	starty.Format(_T("%d"), rect.top);
	endx.Format(_T("%d"), rect.right);
	endy.Format(_T("%d"), rect.bottom);

	if(direction == SHIFT_TO_RIGHT)
		result = m_Excel -> Insert_Rows(startx, starty, endx, endy, SHIFT_TO_RIGHT);
	else 
		result = m_Excel -> Insert_Rows(startx, starty, endx, endy, SHIFT_DOWN);

	if(result == FALSE) return false;	
	return true;
}

bool CExcelManager::Insert_Rows(CRect rect, short direction)
{
	BOOL result;
	CString startx, starty, endx, endy;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return false;
	}
#ifdef _DEBUG
	if(rect.left < 1 || rect.top < 1 || rect.right < 1 || rect.bottom < 1) {
		AfxMessageBox(_T("Find���� ��ǥ���� �ùٸ��� �ʽ��ϴ�."));
		return false;
	}
#endif
	startx.Format(_T("%d"), rect.left);
	starty.Format(_T("%d"), rect.top);
	endx.Format(_T("%d"), rect.right);
	endy.Format(_T("%d"), rect.bottom);

	if(direction == SHIFT_TO_RIGHT)
		result = m_Excel -> Insert_Rows(startx, starty, endx, endy, SHIFT_TO_RIGHT);
	else 
		result = m_Excel -> Insert_Rows(startx, starty, endx, endy, SHIFT_DOWN);

	if(result == FALSE) return false;	
	return true;
}

bool CExcelManager::Insert_Entire_Row(POINT point, short direction)
{
	BOOL result;
	CString startx, starty;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return false;
	}

#ifdef _DEBUG
	if(point.x < _MIN_SHORT || point.y > _MAX_SHORT) {
		AfxMessageBox(_T("Find���� ��ǥ���� ����(-32768 ~ 32767)�� �Ѿ���ϴ�."));
		return false;
	}
#endif
	startx.Format(_T("%d"), point.x);
	starty.Format(_T("%d"), point.y);

	if(direction == SHIFT_TO_RIGHT)
		result = m_Excel -> Insert_Entire_Row(startx, starty, SHIFT_TO_RIGHT);
	else 
		result = m_Excel -> Insert_Entire_Row(startx, starty, SHIFT_DOWN);

	if(result == FALSE) return false;	
	return true;
}

bool CExcelManager::Delete_Entire_Row(POINT point, short direction)
{
	BOOL result;
	CString startx, starty;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return false;
	}

#ifdef _DEBUG
	if(point.x < _MIN_SHORT || point.y > _MAX_SHORT) {
		AfxMessageBox(_T("Find���� ��ǥ���� ����(-32768 ~ 32767)�� �Ѿ���ϴ�."));
		return false;
	}
#endif
	startx.Format(_T("%d"), point.x);
	starty.Format(_T("%d"), point.y);

	if(direction == SHIFT_TO_RIGHT)
		result = m_Excel -> Delete_Entire_Row(startx, starty, SHIFT_TO_RIGHT);
	else 
		result = m_Excel -> Delete_Entire_Row(startx, starty, SHIFT_DOWN);

	if(result == FALSE) return false;	
	return true;
}

bool CExcelManager::Merge(CRect rect)
{
	BOOL result;
	CString startx, starty, endx, endy;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return false;
	}

#ifdef _DEBUG
	if(rect.left < _MIN_SHORT || rect.left > _MAX_SHORT || rect.top < _MIN_SHORT || rect.top > _MAX_SHORT || rect.right < _MIN_SHORT || rect.right > _MAX_SHORT  || rect.bottom < _MIN_SHORT || rect.bottom > _MAX_SHORT) {
		AfxMessageBox(_T("Find���� ��ǥ���� ����(-32768 ~ 32767)�� �Ѿ���ϴ�."));
		return false;
	}
#endif
	startx.Format(_T("%d"), rect.left);
	starty.Format(_T("%d"), rect.top);
	endx.Format(_T("%d"), rect.right);
	endy.Format(_T("%d"), rect.bottom);

	result = m_Excel -> Merge(startx, starty, endx, endy);

	if(result == FALSE) return false;	
	return true;
}

bool CExcelManager::Merge(RECT rect)
{
	BOOL result;
	CString startx, starty, endx, endy;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return false;
	}

#ifdef _DEBUG
	if(rect.left < _MIN_SHORT || rect.left > _MAX_SHORT || rect.top < _MIN_SHORT || rect.top > _MAX_SHORT || rect.right < _MIN_SHORT || rect.right > _MAX_SHORT  || rect.bottom < _MIN_SHORT || rect.bottom > _MAX_SHORT) {
		AfxMessageBox(_T("Find���� ��ǥ���� ����(-32768 ~ 32767)�� �Ѿ���ϴ�."));
		return false;
	}
#endif
	startx.Format(_T("%d"), rect.left);
	starty.Format(_T("%d"), rect.top);
	endx.Format(_T("%d"), rect.right);
	endy.Format(_T("%d"), rect.bottom);

	result = m_Excel -> Merge(startx, starty, endx, endy);

	if(result == FALSE) return false;	
	return true;
}


bool CExcelManager::SetTextMatrix(UINT iRow, UINT iCol, TCHAR *value)
{
	CString row, col;
	CString msg;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return false;
	}

	if(iRow < 1 || iCol < 1) {
		msg.Format(_T("SetTextMatrix : row[%d], col[%d]�� 1���� �۽��ϴ�."), iRow, iCol);
		AfxMessageBox(msg);
		return false;
	}

	row.Format(_T("%u"), iRow);
	col.Format(_T("%u"), iCol);

	if(m_Excel -> SetTextMatrix(row, col, value) == TRUE) return true;

	return false;
}

bool CExcelManager::SetTextMatrix(UINT iRow, UINT iCol, CString value)
{
	CString row, col;
	CString msg;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return false;
	}

	if(iRow < 1 || iCol < 1) {
		msg.Format(_T("SetTextMarix : row[%d], col[%d]�� 1���� �۽��ϴ�."), iRow, iCol);
		AfxMessageBox(msg);
		return false;
	}

	row.Format(_T("%u"), iRow);
	col.Format(_T("%u"), iCol);


	if(m_Excel -> SetTextMatrix(row, col, value) == TRUE) return true;

	return false;
}

bool CExcelManager::SetTextMatrix(UINT iRow, UINT iCol, INT int_value)
{
	CString value, row, col;
	CString msg;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return false;
	}

	if(iRow < 1 || iCol < 1) {
		msg.Format(_T("SetTextMatrix - row[%d], col[%d]�� 1���� �۽��ϴ�."), iRow, iCol);
		AfxMessageBox(msg);
		return false;
	}

	value.Format(_T("%d"), int_value);


	// m_Excel -> SetTextMatrix(row, col, &bs);
	row.Format(_T("%u"), iRow);
	col.Format(_T("%u"), iCol);
	if(m_Excel -> SetTextMatrix(row, col, value) == TRUE) return true;

	return false;
}

bool CExcelManager::SetTextMatrix(UINT iRow, UINT iCol, UINT uint_value)
{
	CString value, row, col;
	CString msg;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return false;
	}

	if(iRow < 1 || iCol < 1) {
		msg.Format(_T("SetTextMatrix - row[%d], col[%d]�� 1���� �۽��ϴ�."), iRow, iCol);
		AfxMessageBox(msg);
		return false;
	}

	value.Format(_T("%u"), uint_value);
	row.Format(_T("%u"), iRow);
	col.Format(_T("%u"), iCol);

	if(m_Excel -> SetTextMatrix(row, col, value) == TRUE) return true;

	return false;
}

bool CExcelManager::SetTextMatrix(UINT iRow, UINT iCol, double d_value)
{
	CString value, row, col;
	CString msg;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return false;
	}

	if(iRow < 1 || iCol < 1) {
		msg.Format(_T("SetTextMatrix - row[%d], col[%d]�� 1���� �۽��ϴ�."), iRow, iCol);
		AfxMessageBox(msg);
		return false;
	}

	value.Format(_T("%f"), d_value);
	row.Format(_T("%u"), iRow);
	col.Format(_T("%u"), iCol);

	if(m_Excel -> SetTextMatrix(row, col, value) == TRUE) return true;

	return false;
}

CString CExcelManager::GetTextMatrix(UINT iRow, UINT iCol)
{
	CString row, col;
	CString msg;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return _T("");
	}

	if(iRow < 1 || iCol < 1) {
		msg.Format(_T("GetTextMatrix - row[%d], col[%d]�� 1���� �۽��ϴ�."), iRow, iCol);
		AfxMessageBox(msg);
		return _T("");
	}

	row.Format(_T("%u"), iRow);
	col.Format(_T("%u"), iCol);
	return m_Excel -> GetTextMatrix(row, col);
}

bool CExcelManager::SetTextMatrix(TCHAR *name, TCHAR *value)
{
	CString msg;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return false;
	}

	if(m_Excel -> SetTextMatrix_By_Name(name, value) == TRUE) return true;

	return false;
}

bool CExcelManager::SetTextMatrix(TCHAR *name, CString value)
{
	CString msg;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return false;
	}

	if(m_Excel -> SetTextMatrix_By_Name(name, value) == TRUE) return true;

	return false;
}

bool CExcelManager::SetTextMatrix(TCHAR *name, INT int_value)
{
	CString value;
	CString msg;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return false;
	}

	value.Format(_T("%d"), int_value);


	// m_Excel -> SetTextMatrix(row, col, &bs);

	if(m_Excel -> SetTextMatrix_By_Name(name, value) == TRUE) return true;

	return false;
}

bool CExcelManager::SetTextMatrix(TCHAR *name, UINT uint_value)
{
	CString value;
	CString msg;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return false;
	}

	value.Format(_T("%u"), uint_value);

	if(m_Excel -> SetTextMatrix_By_Name(name, value) == TRUE) return true;

	return false;
}

bool CExcelManager::SetTextMatrix(TCHAR *name, double d_value)
{
	CString value;
	CString msg;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return false;
	}

	value.Format(_T("%f"), d_value);

	if(m_Excel -> SetTextMatrix_By_Name(name, value) == TRUE) return true;

	return false;
}

VOID CExcelManager::CloseManager()
{
	if(m_Excel) {
		m_Excel -> CloseManager();
		m_Excel = NULL;
	}
}

VOID CExcelManager::Font(UINT iStartx, UINT iStarty, UINT iEndx, UINT iEndy, bool bold, bool italic, short color, short size)
{
	CString startx, starty, endx, endy;
	CString msg;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return;
	}

	startx.Format(_T("%d"), iStartx);
	starty.Format(_T("%d"), iStarty);
	endx.Format(_T("%d"), iEndx);
	endy.Format(_T("%d"), iEndy);

	if(iStartx < 1 || iStarty < 1) {
		msg.Format(_T("Cell_Format - startx[%d], starty[%d]�� 1���� �۽��ϴ�."), iStartx, iStarty);
		AfxMessageBox(msg);
		return;
	}
	if(iEndx < 1 || iEndy < 1) {
		msg.Format(_T("Cell_Format - endx[%d], endy[%d]�� 1���� �۽��ϴ�."), iEndx, iEndy);
		AfxMessageBox(msg);
		return;
	}

	if(bold) m_Excel -> Font_Bold(startx, starty, endx, endy, TRUE);
	else m_Excel -> Font_Bold(startx, starty, endx, endy, FALSE);

	if(italic) m_Excel -> Font_Italic(startx, starty, endx, endy, TRUE);
	else m_Excel -> Font_Italic(startx, starty, endx, endy, FALSE);

	m_Excel -> Font_Size(startx, starty, endx, endy, size);
	m_Excel -> Font_Color(startx, starty, endx, endy, color);
}

VOID CExcelManager::Border(UINT iStartx, UINT iStarty, UINT iEndx, UINT iEndy, short style, short weight, short color)
{
	CString startx, starty, endx, endy;
	CString msg;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return;
	}

	if(style < 1 || style > 13) {
		AfxMessageBox(_T("Board�� Style�� ����!!!"));
		return;
	}

	if(weight < 1 || weight > 4) {
		AfxMessageBox(_T("Board�� Weight�� ����!!!"));
		return;
	}

	if(iStartx < 1 || iStarty < 1) {
		msg.Format(_T("Cell_Format - startx[%d], starty[%d]�� 1���� �۽��ϴ�."), iStartx, iStarty);
		AfxMessageBox(msg);
		return;
	}
	if(iEndx < 1 || iEndy < 1) {
		msg.Format(_T("Cell_Format - endx[%d], endy[%d]�� 1���� �۽��ϴ�."), iEndx, iEndy);
		AfxMessageBox(msg);
		return;
	}

	startx.Format(_T("%d"), iStartx);
	starty.Format(_T("%d"), iStarty);
	endx.Format(_T("%d"), iEndx);
	endy.Format(_T("%d"), iEndy);

	m_Excel -> Border(startx, starty, endx, endy, style, weight, color);
}

VOID CExcelManager::Fill_Border(UINT iStartx, UINT iStarty, UINT iEndx, UINT iEndy, short style, short weight, short color)
{
	CString startx, starty, endx, endy;
	CString msg;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return;
	}

	if(style < 1 || style > 13) {
		AfxMessageBox(_T("Board�� Style�� ����!!!"));
		return;
	}

	if(weight < 1 || weight > 4) {
		AfxMessageBox(_T("Board�� Weight�� ����!!!"));
		return;
	}

	if(iStartx < 1 || iStarty < 1) {
		msg.Format(_T("Cell_Format - startx[%d], starty[%d]�� 1���� �۽��ϴ�."), iStartx, iStarty);
		AfxMessageBox(msg);
		return;
	}
	if(iEndx < 1 || iEndy < 1) {
		msg.Format(_T("Cell_Format - endx[%d], endy[%d]�� 1���� �۽��ϴ�."), iEndx, iEndy);
		AfxMessageBox(msg);
		return;
	}

	startx.Format(_T("%d"), iStartx);
	starty.Format(_T("%d"), iStarty);
	endx.Format(_T("%d"), iEndx);
	endy.Format(_T("%d"), iEndy);

	m_Excel -> Fill_Border(startx, starty, endx, endy, style, weight, color);
}

VOID CExcelManager::TestFunction()
{
	// 1 - 12����

	
}


// ���� ���� : ���� Border(..)�� ȣ���� ���� ����ؾ� �Ѵ�.
// border_type
//		1 : ���õ� ������ ��� Cell�� ����
//		2 : ���õ� ������ ��� Cell�� ������
//		3 : ���õ� ������ ��� Cell�� ����
//		4 :	���õ� ������ ��� Cell�� �Ʒ���
//		5 :	���õ� ������ ��� Cell�� �밢��
//		6 : ���õ� ������ ��� Cell�� �밢��
//		7 : ���õ� ������ ��� Cell �߿� ���� ���� Cell�� ����
//		8 : ���õ� ������ ��� Cell �߿� ���� ���� Cell�� ����
//		9 : ���õ� ������ ��� Cell �߿� ���� �Ʒ��� Cell�� �Ʒ���
//		10 : ���õ� ������ ��� Cell �߿� ���� ������ Cell�� ������

VOID CExcelManager::DrawLine(UINT iStartx, UINT iStarty, UINT iEndx, UINT iEndy, short border_type, short style, short weight, short color)
{
	CString startx, starty, endx, endy;
	CString msg;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return;
	}

	if(border_type < 1 || border_type > 10) {
		AfxMessageBox(_T("Board�� type�� ����!!!"));
		return;
	}


	if(style < 0 || style > 13) {
		AfxMessageBox(_T("Board�� Style�� ����!!!"));
		return;
	}

	if(weight < 1 || weight > 4) {
		AfxMessageBox(_T("Board�� Style�� ����!!!"));
		return;
	}

	if(iStartx < 1 || iStarty < 1) {
		msg.Format(_T("Cell_Format - startx[%d], starty[%d]�� 1���� �۽��ϴ�."), iStartx, iStarty);
		AfxMessageBox(msg);
		return;
	}
	if(iEndx < 1 || iEndy < 1) {
		msg.Format(_T("Cell_Format - endx[%d], endy[%d]�� 1���� �۽��ϴ�."), iEndx, iEndy);
		AfxMessageBox(msg);
		return;
	}
/*
	if(color < 1 || color > 4) {
		AfxMessageBox(_T("Board�� Style�� ����!!!"));
		return;
	}

	CString str;
	str.Format(_T("Border_Type : %d  Style : %d Weight : %d Color : %d"), border_type, style, weight, color);
	AfxMessageBox(str);
*/
	startx.Format(_T("%d"), iStartx);
	starty.Format(_T("%d"), iStarty);
	endx.Format(_T("%d"), iEndx);
	endy.Format(_T("%d"), iEndy);

	m_Excel -> DrawLine(startx, starty, endx, endy, border_type, style, weight, color);
}

VOID CExcelManager::Cell_Format(UINT iStartx, UINT iStarty, UINT iEndx, UINT iEndy, LPCTSTR format)
{
	CString startx, starty, endx, endy;
	CString msg;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return;
	}

	if(iStartx < 1 || iStarty < 1) {
		msg.Format(_T("Cell_Format - startx[%d], starty[%d]�� 1���� �۽��ϴ�."), iStartx, iStarty);
		AfxMessageBox(msg);
		return;
	}
	if(iEndx < 1 || iEndy < 1) {
		msg.Format(_T("Cell_Format - endx[%d], endy[%d]�� 1���� �۽��ϴ�."), iEndx, iEndy);
		AfxMessageBox(msg);
		return;
	}

	startx.Format(_T("%d"), iStartx);
	starty.Format(_T("%d"), iStarty);
	endx.Format(_T("%d"), iEndx);
	endy.Format(_T("%d"), iEndy);

	m_Excel -> Cell_Format(startx, starty, endx, endy, format);
}

VOID CExcelManager::Cell_Format_General(UINT iStartx, UINT iStarty, UINT iEndx, UINT iEndy)
{
	CString startx, starty, endx, endy;
	CString msg;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return;
	}

	if(iStartx < 1 || iStarty < 1) {
		msg.Format(_T("Cell_Format - startx[%d], starty[%d]�� 1���� �۽��ϴ�."), iStartx, iStarty);
		AfxMessageBox(msg);
		return;
	}
	if(iEndx < 1 || iEndy < 1) {
		msg.Format(_T("Cell_Format - endx[%d], endy[%d]�� 1���� �۽��ϴ�."), iEndx, iEndy);
		AfxMessageBox(msg);
		return;
	}

	m_Excel -> Cell_Format(startx, starty, endx, endy, _T("G/ǥ��"));
}

VOID CExcelManager::Cell_Format_String(UINT iStartx, UINT iStarty, UINT iEndx, UINT iEndy)
{
	CString startx, starty, endx, endy;
	CString msg;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return;
	}

	if(iStartx < 1 || iStarty < 1) {
		msg.Format(_T("Cell_Format - startx[%d], starty[%d]�� 1���� �۽��ϴ�."), iStartx, iStarty);
		AfxMessageBox(msg);
		return;
	}
	if(iEndx < 1 || iEndy < 1) {
		msg.Format(_T("Cell_Format - endx[%d], endy[%d]�� 1���� �۽��ϴ�."), iEndx, iEndy);
		AfxMessageBox(msg);
		return;
	}

	startx.Format(_T("%d"), iStartx);
	starty.Format(_T("%d"), iStarty);
	endx.Format(_T("%d"), iEndx);
	endy.Format(_T("%d"), iEndy);

	m_Excel -> Cell_Format(startx, starty, endx, endy, _T("@"));
}

VOID CExcelManager::Cell_Color(UINT iStartx, UINT iStarty, UINT iEndx, UINT iEndy, short color)
{
	CString startx, starty, endx, endy;
	CString msg;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return;
	}

	if(iStartx < 1 || iStarty < 1) {
		msg.Format(_T("Cell_Format - startx[%d], starty[%d]�� 1���� �۽��ϴ�."), iStartx, iStarty);
		AfxMessageBox(msg);
		return;
	}
	if(iEndx < 1 || iEndy < 1) {
		msg.Format(_T("Cell_Format - endx[%d], endy[%d]�� 1���� �۽��ϴ�."), iEndx, iEndy);
		AfxMessageBox(msg);
		return;
	}

	startx.Format(_T("%d"), iStartx);
	starty.Format(_T("%d"), iStarty);
	endx.Format(_T("%d"), iEndx);
	endy.Format(_T("%d"), iEndy);

	m_Excel -> Cell_Color(startx, starty, endx, endy, color);
}

// ���� - Horizontal 
//		1 - ǥ��
//		2 - ����
//		3 - ���
//		4 - ������
//		5 - ä���
//		6 - ���� ����
//		7 - ���� ������ �����
//		8 - ���

VOID CExcelManager::Horizontal_Alignment(UINT iStartx, UINT iStarty, UINT iEndx, UINT iEndy, short alignment)
{
	CString startx, starty, endx, endy;
	CString msg;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return;
	}

	if(iStartx < 1 || iStarty < 1) {
		msg.Format(_T("Cell_Format - startx[%d], starty[%d]�� 1���� �۽��ϴ�."), iStartx, iStarty);
		AfxMessageBox(msg);
		return;
	}
	if(iEndx < 1 || iEndy < 1) {
		msg.Format(_T("Cell_Format - endx[%d], endy[%d]�� 1���� �۽��ϴ�."), iEndx, iEndy);
		AfxMessageBox(msg);
		return;
	}

	if(alignment < 1 || alignment > 8) {
		AfxMessageBox(_T("���� ���� ��� ����"));
		return;
	}

	startx.Format(_T("%d"), iStartx);
	starty.Format(_T("%d"), iStarty);
	endx.Format(_T("%d"), iEndx);
	endy.Format(_T("%d"), iEndy);

	m_Excel ->Horizontal_Alignment(startx, starty, endx, endy, alignment);
}

// ���� - Vertical
//		1 - ����
//		2 - ���
//		3 - �Ʒ���
//		4 - ���� ����
//		5 - ���

VOID CExcelManager::Vertical_Alignment(UINT iStartx, UINT iStarty, UINT iEndx, UINT iEndy, short alignment)
{
	CString startx, starty, endx, endy;
	CString msg;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return;
	}

	if(iStartx < 1 || iStarty < 1) {
		msg.Format(_T("Cell_Format - startx[%d], starty[%d]�� 1���� �۽��ϴ�."), iStartx, iStarty);
		AfxMessageBox(msg);
		return;
	}
	if(iEndx < 1 || iEndy < 1) {
		msg.Format(_T("Cell_Format - endx[%d], endy[%d]�� 1���� �۽��ϴ�."), iEndx, iEndy);
		AfxMessageBox(msg);
		return;
	}

	if(alignment < 1 || alignment > 5) {
		AfxMessageBox(_T("���� ���� ��� ����"));
		return;
	}

	startx.Format(_T("%d"), iStartx);
	starty.Format(_T("%d"), iStarty);
	endx.Format(_T("%d"), iEndx);
	endy.Format(_T("%d"), iEndy);

	m_Excel ->Vertical_Alignment(startx, starty, endx, endy, alignment);
}

bool CExcelManager::WorkSheet_Add(short number_of_sheet, BOOL isBefore)
{
	if(number_of_sheet < 1) {
		AfxMessageBox(_T("�߰��Ϸ��� WorkSheet�� ������ 1���� �۽��ϴ�."));
		return false;
	}
	m_Excel -> WorkSheet_Add(number_of_sheet, isBefore);
	return true;
}

bool CExcelManager::WorkSheet_Activate(short number_of_sheet)
{
	if(number_of_sheet < 1) {
		AfxMessageBox(_T("�߰��Ϸ��� WorkSheet�� ������ 1���� �۽��ϴ�."));
		return false;
	}
	m_Excel -> WorkSheet_Activate(number_of_sheet);
	return true;
}

short CExcelManager::WorkSheet_Count()
{
	return m_Excel -> WorkSheet_Count();
}

bool CExcelManager::WorkSheet_Delete(short number_of_sheet)
{
	if(number_of_sheet < 1) {
		AfxMessageBox(_T("�߰��Ϸ��� WorkSheet�� ������ 1���� �۽��ϴ�."));
		return false;
	}
	m_Excel -> WorkSheet_Delete(number_of_sheet);
	return true;
}

short CExcelManager::WorkSheet_Index()
{
	return m_Excel -> WorkSheet_Index();
}

bool CExcelManager::Close(bool saveChanges)
{
	if(saveChanges) {
		m_Excel -> Close_WorkSheets();
		return true;
	}
	else {
		m_Excel -> Close_WorkSheets();
		return true;
	}
}


bool CExcelManager::Set_Name(UINT iStartx, UINT iStarty, UINT iEndx, UINT iEndy, TCHAR *name)
{
	CString startx, starty, endx, endy;
	CString msg;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return false;
	}

	if(iStartx < 1 || iStarty < 1) {
		msg.Format(_T("Cell_Format - startx[%d], starty[%d]�� 1���� �۽��ϴ�."), iStartx, iStarty);
		AfxMessageBox(msg);
		return false;
	}
	if(iEndx < 1 || iEndy < 1) {
		msg.Format(_T("Cell_Format - endx[%d], endy[%d]�� 1���� �۽��ϴ�."), iEndx, iEndy);
		AfxMessageBox(msg);
		return false;
	}

	if(name == NULL) {
		AfxMessageBox(_T("name�� NULL�Դϴ�."));
		return false;
	}

	startx.Format(_T("%d"), iStartx);
	starty.Format(_T("%d"), iStarty);
	endx.Format(_T("%d"), iEndx);
	endy.Format(_T("%d"), iEndy);

	if(m_Excel ->Set_Name(startx, starty, endx, endy, name) == TRUE) {
		return true;
	}
	return false;
}

bool CExcelManager::Set_Name(TCHAR *range, TCHAR *name)
{
	CString msg;

	if(IsOpen || IsInit == false) {
		AfxMessageBox(_T("CExcelManager�� �ʱ�ȭ ���� �ʾҽ��ϴ�"));
		return false;
	}

	if(name == NULL) {
		AfxMessageBox(_T("name�� NULL�Դϴ�."));
		return false;
	}

	if(range == NULL) {
		AfxMessageBox(_T("range�� NULL�Դϴ�."));
		return false;
	}

	if(m_Excel ->Set_Name_By_Range(range, name) == TRUE) {
		return true;
	}
	return false;
}
