// ESLDlg_ExcelExportWait.cpp : implementation file
//

#include "stdafx.h"
#include "ESLDlg_ExcelExportWait.h"
// 18.10.30 JMJ KOL.RED.2018.021
//--------------------------------------------------------------
#include "ESLDlg_ExportType.h"
//--------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CESLDlg_ExcelExportWait dialog


CESLDlg_ExcelExportWait::CESLDlg_ExcelExportWait(CWnd* pParent /*=NULL*/)
	: CDialog(CESLDlg_ExcelExportWait::IDD, pParent)
{
	//{{AFX_DATA_INIT(CESLDlg_ExcelExportWait)
	
	// 18.10.30 JMJ KOL.RED.2018.021
	//--------------------------------------------------------------
	m_bIsExcel		= FALSE;
	m_bIsHancell	= FALSE;
	m_bIsCSV		= FALSE;

	CESLDlg_ExportType Dlg;
	m_nDialogReturn = Dlg.DoModal();
	if(m_nDialogReturn == IDOK)
	{
		m_bIsExcel	 = Dlg.m_bIsExcel;
		m_bIsHancell = Dlg.m_bIsHancell;
		m_bIsCSV	 = Dlg.m_bIsCSV;
	}
	//--------------------------------------------------------------
	//}}AFX_DATA_INIT
}


VOID CESLDlg_ExcelExportWait::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CESLDlg_ExcelExportWait)
	DDX_Control(pDX, IDC_COM_EXCELEXPORT_PROGRESS, m_Progress1);
	// 18.10.30 JMJ KOL.RED.2018.021
	//--------------------------------------------------------------
	CString strOutputText;
	if(m_bIsExcel == TRUE)
	{
		strOutputText = _T("���� ���Ϸ� �����մϴ�.");
	}
	else if(m_bIsHancell == TRUE)
	{
		strOutputText = _T("�Ѽ� ���Ϸ� �����մϴ�.");
	}
	else if(m_bIsCSV == TRUE)
	{
		strOutputText = _T("CSV ���Ϸ� �����մϴ�.");
	}
	DDX_Text(pDX, IDC_COM_EXCELEXPORT_STATIC, strOutputText);
	//--------------------------------------------------------------
	/*
	// 16/03/29 ������ : �Ѽ� �����ϱ�
	/// ADD -------------------------------------------------------------------------------------
		CLSID clsExcelApp;
		if (!FAILED(CLSIDFromProgID(_T("HCell.Application"), &clsExcelApp))) 
		{
			DDX_Text(pDX, IDC_COM_EXCELEXPORT_STATIC, CString(_T("�Ѽ����")));	// 16/04/05 ������ : ��ȣ���� ������ ���� �ӽ��ּ�ó�� -> // 16/04/19 ������ : �ּ�Ǯ��
		}
	/// END -------------------------------------------------------------------------------------
	*/
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CESLDlg_ExcelExportWait, CDialog)
	//{{AFX_MSG_MAP(CESLDlg_ExcelExportWait)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CESLDlg_ExcelExportWait message handlers
