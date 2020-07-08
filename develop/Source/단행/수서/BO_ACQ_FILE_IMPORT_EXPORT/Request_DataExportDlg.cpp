// Request_DataExportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Request_DataExportDlg.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRequest_DataExportDlg dialog


CRequest_DataExportDlg::CRequest_DataExportDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CRequest_DataExportDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParentMgr = pParent;

}

CRequest_DataExportDlg::~CRequest_DataExportDlg()
{
}

VOID CRequest_DataExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRequest_DataExportDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRequest_DataExportDlg, CDialog)
	//{{AFX_MSG_MAP(CRequest_DataExportDlg)
	ON_BN_CLICKED(IDC_bDEFAULT_CONFIG, OnbDEFAULTCONFIG)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRequest_DataExportDlg message handlers

BOOL CRequest_DataExportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_nCheckKeyCount = 0;
	// üũ��ư �˸��ƽ�									// üũ��ư ���ҽ�												
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("����");			m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkTITLE;				m_nCheckKeyCount++;	
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("����");			m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkAUTHOR;				m_nCheckKeyCount++;			
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("������");		m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkPUBLISHER;			m_nCheckKeyCount++;	
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("�����");		m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkPUBLISH_YEAR;		m_nCheckKeyCount++;			
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("����");			m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkPRICE;				m_nCheckKeyCount++;	
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("��õ�ǰ�");		m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkRECOM_OPINION;		m_nCheckKeyCount++;
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("���");			m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkCANCLE_REASON;		m_nCheckKeyCount++;	
//	m_strChkButtonAlias[m_nCheckKeyCount] = _T("SMS���ſ���");	m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkSMS_YN;				m_nCheckKeyCount++;		
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("��ġ����");		m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkFURNISH_STATUS;		m_nCheckKeyCount++;	
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("��û��");		m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkAPPLICANT_DATE;		m_nCheckKeyCount++;		
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("��û��");		m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkAPPLICANT_NAME;		m_nCheckKeyCount++;	
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("E-MAIL");		m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkEMAIL;				m_nCheckKeyCount++;
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("�ֹε�Ϲ�ȣ");	m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkCIVIL_NO;			m_nCheckKeyCount++;	
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("�����ȣ");		m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkZIP_CODE;			m_nCheckKeyCount++;		
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("�ּ�");			m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkADDRESS;			m_nCheckKeyCount++;	
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("��ȭ��ȣ");		m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkTEL;				m_nCheckKeyCount++;		
	m_strChkButtonAlias[m_nCheckKeyCount] = _T("�ڵ���");		m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkHANDPHONE;			m_nCheckKeyCount++;	
//	m_strChkButtonAlias[m_nCheckKeyCount] = _T("���Ϲ߼���");	m_nChkButtonResource[m_nCheckKeyCount] = IDC_chkMAIL_SEND_DATE;		m_nCheckKeyCount++;

	// ���������� �����س��� ������ �о�ͼ� üũ ǥ���Ѵ�.
	SetDefaultCheckedCode();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CRequest_DataExportDlg::OnOK() 
{
	// TODO: Add extra validation here

	// ���̾�α׿��� üũ�� �׸� ����Ʈ�� �߰��Ѵ�.
	INT nCheck = 0;
	CButton * pBtn;
	
	
	
	for (INT i=0; i < m_nCheckKeyCount; i++)
	{
		pBtn = (CButton*)GetDlgItem(m_nChkButtonResource[i]);
		nCheck = pBtn->GetCheck();
		if(nCheck) 
			m_strCheckArray.Add(m_strChkButtonAlias[i]);
	}
	
	
	CDialog::OnOK();
}

VOID CRequest_DataExportDlg::SetDefaultCheckedCode()
{

	CStdioFile fp;
	CButton * pBtn;

	// ��� üũ��ư�� �ʱ�ȭ�Ѵ�.
	for( INT i=0; i < m_nCheckKeyCount; i++)
	{
		pBtn = (CButton*)GetDlgItem(m_nChkButtonResource[i]);
		pBtn->SetCheck(0);
	}

	INT ids;
	if(!fp.Open(_T("..\\Cfg\\RequestExportData.txt"), CFile::modeRead | CFile::typeBinary))
	{
		// ������ ã�� ���� ��쿡�� �ʼ��׸�(����, ��ġ����)�� üũǥ�� �Ѵ�.
		pBtn = (CButton*)GetDlgItem(IDC_chkTITLE);
		pBtn->SetCheck(1);
		// 2005-01-16 
		// ��ġ���´� �ʼ��׸����� �߰��ϰ�, 
		// ��û�ϰ� ��õ�ǰ��� �ʼ��׸��� �ƴ����� üũǥ�ô� �׻� �ϵ��� �Ѵ�.
		pBtn = (CButton*)GetDlgItem(IDC_chkFURNISH_STATUS);
		pBtn->SetCheck(1);
		pBtn = (CButton*)GetDlgItem(IDC_chkRECOM_OPINION);
		pBtn->SetCheck(1);
		pBtn = (CButton*)GetDlgItem(IDC_chkAPPLICANT_DATE);
		pBtn->SetCheck(1);
	}
	else
	{
		// ���Ͽ��� ���ð� �о üũǥ�� �Ѵ�.
		CString strLine;
		CString strAlias;
		
		TCHAR cUni;
		fp.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fp.SeekToBegin();
		}
		
		while (fp.ReadString(strLine))
		{
			strLine.TrimLeft();
			strLine.TrimRight();
			
			strAlias.Empty();

			if (strLine.Left(1) == _T("#") || strLine.IsEmpty()) continue;
			
			ids = _stscanf(strLine, _T("%[^\r\n]"), strAlias.GetBuffer(256));
			
			if (ids == _TEOF) continue;

			strAlias.ReleaseBuffer();

			for( INT i=0; i < m_nCheckKeyCount; i++)
			{
				if(!_tcscmp(m_strChkButtonAlias[i], strAlias))
				{
					pBtn = (CButton*)GetDlgItem(m_nChkButtonResource[i]);
					pBtn->SetCheck(1);
				}
			}
		}
	}

}

BOOL CRequest_DataExportDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CRequest_DataExportDlg::OnbDEFAULTCONFIG() 
{
	// TODO: Add your control notification handler code here
	// üũ�� �׸��� �⺻������ ���Ͽ� �����Ѵ�.
	CString strTmp;
	CStdioFile fp;
	CButton * pBtn;

	// ������ ���� ���ϴ� ��� ���� �����Ѵ�.
	fp.Open(_T("..\\Cfg\\RequestExportData.txt"), CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);
	if ( fp == NULL ) {
		AfxMessageBox(_T("���� ���� ����."));
		return ;
	}
	
	if( 0 == fp.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		fp.Write( &cUni, sizeof(TCHAR));
	}

	for( INT i=0; i < m_nCheckKeyCount; i++)
	{
		pBtn = (CButton*)GetDlgItem(m_nChkButtonResource[i]);
		if(pBtn->GetCheck())
		{
			strTmp = m_strChkButtonAlias[i] + _T("\r\n");
			fp.WriteString(strTmp);
			strTmp.Empty();
		}
	}

	fp.Close();

	AfxMessageBox(_T("�⺻���� �����Ͽ����ϴ�."));

}

HBRUSH CRequest_DataExportDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}