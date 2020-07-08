// BO_ACQ_PrintDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_PrintDlg.h"
#include "..\K2UP_Common\ExcelExporter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_PrintDlg dialog


CBO_ACQ_PrintDlg::CBO_ACQ_PrintDlg(SPECIES_TYPE nSpeciesType, ACQ_TYPE nAcqType, CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_PrintDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_PrintDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pPrintMgr = NULL;
	m_arrData.RemoveAll();
	m_arrReportFileName.RemoveAll();
	m_arrReportType.RemoveAll();
	m_nSpeciesType = nSpeciesType;
	m_nAcqType = nAcqType;
	m_bCreate = FALSE;
	m_strReportFileName.Empty();
	m_nReportType = -1;
	m_pPrintDM = NULL;
	m_pGroupCollection = NULL;
	m_bSpecies = TRUE;
	m_bSeriesPaper = FALSE;
	m_bExcelExport = FALSE;
	m_bKOLAS2Format = FALSE;
}

CBO_ACQ_PrintDlg::~CBO_ACQ_PrintDlg()
{
	if(m_pPrintMgr)
	{
		delete m_pPrintMgr;
		m_pPrintMgr = NULL;
	}
}

VOID CBO_ACQ_PrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_PrintDlg)
	DDX_Control(pDX, IDC_cmbCPNT_PRINT_LIST, m_ComboBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_PrintDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_PrintDlg)
	ON_CBN_SELCHANGE(IDC_cmbCPNT_PRINT_LIST, OnSelchangecmbPRINTLIST)
	ON_BN_CLICKED(IDC_btnSERIES_PAPER, OnbtnSERIESPAPER)
	ON_BN_CLICKED(IDC_btnEXCEL_EXPORT, OnbtnEXCELEXPORT)
	ON_BN_CLICKED(IDC_btnKOLAS2_FORMAT, OnbtnKOLAS2FORMAT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_PrintDlg message handlers

BOOL CBO_ACQ_PrintDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_pPrintMgr = new CBasicPrintMgr(this);
	m_ComboBox.ResetContent();
	
	if(!Init())
	{
		ESLAfxMessageBox(_T("��� ����Ʈ ���� ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBO_ACQ_PrintDlg::Init()
{
	if(m_nSpeciesType == SPECIES_TYPE_INVALID) return FALSE;

	if(m_nSpeciesType == BO_BASIC)
	{
		SetWindowText(_T("�����ڷ� ��¹� ����Ʈ"));
		m_arrData.Add(_T("�����ڷ���"));				m_arrReportFileName.Add(_T("K2UP_�����ڷ���"));				m_arrReportType.Add(BO_ACQ_BASIC_DATA);

	}
	else if(m_nSpeciesType == BO_IMPORT)
	{
		SetWindowText(_T("�����ڷ� ��¹� ����Ʈ"));
		m_arrData.Add(_T("�����ڷ���"));				m_arrReportFileName.Add(_T("K2UP_�����ڷ���"));				m_arrReportType.Add(BO_ACQ_IMPORT_DATA);
	}
	else if(m_nSpeciesType == BO_RECYCLE)
	{
		SetWindowText(_T("��Ȱ���ڷ� ��¹� ����Ʈ"));
		m_arrData.Add(_T("��Ȱ���ڷ���"));			m_arrReportFileName.Add(_T("K2UP_��Ȱ���ڷ���"));				m_arrReportType.Add(BO_ACQ_RECYCLE_DATA);
	}
	else if(m_nSpeciesType == BO_REQUEST)
	{
		SetWindowText(_T("��ġ����ڷ� ��¹� ����Ʈ"));
		m_arrData.Add(_T("��ġ����ڷ���"));			m_arrReportFileName.Add(_T("K2UP_��ġ����ڷ���"));			m_arrReportType.Add(BO_ACQ_REQUEST_DATA);
		m_arrData.Add(_T("���Ϲ߼۸��"));				m_arrReportFileName.Add(_T("K2UP_��ġ������Ϲ߼۸��"));		m_arrReportType.Add(BO_ACQ_REQUEST_MAIL_SEND_LIST);
	}	
	else if(m_nSpeciesType == BO_PURCHASE)
	{
		SetWindowText(_T("���Դ���ڷ� ��¹� ����Ʈ"));
		m_arrData.Add(_T("���Դ���ڷ���"));				m_arrReportFileName.Add(_T("K2UP_���Դ���ڷ���"));				m_arrReportType.Add(BO_ACQ_PURCHASE_DATA);
		m_arrData.Add(_T("���Դ���ڷ���(��ϱ��к�)"));	m_arrReportFileName.Add(_T("K2UP_���Դ���ڷ���_��ϱ��к�"));	m_arrReportType.Add(BO_ACQ_PURCHASE_DATA_REGCODE);
		m_arrData.Add(_T("���Ǵ���ڷ���(������)"));		m_arrReportFileName.Add(_T("K2UP_���Ǵ���ڷ���"));				m_arrReportType.Add(BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO);
		m_arrData.Add(_T("���Ǵ���ڷ���(����ó��)"));	m_arrReportFileName.Add(_T("K2UP_���Ǵ���ڷ���"));				m_arrReportType.Add(BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER);
		m_arrData.Add(_T("���Ǵ���ڷ���(�������к�)"));	m_arrReportFileName.Add(_T("K2UP_���Ǵ���ڷ���"));				m_arrReportType.Add(BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE);
		m_arrData.Add(_T("���ǰ���ڷ���(������)"));		m_arrReportFileName.Add(_T("K2UP_���ǰ���ڷ���"));				m_arrReportType.Add(BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO);
		m_arrData.Add(_T("���ǰ���ڷ���(����ó��)"));	m_arrReportFileName.Add(_T("K2UP_���ǰ���ڷ���"));				m_arrReportType.Add(BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER);
		m_arrData.Add(_T("���ǰ���ڷ���(�������к�)"));	m_arrReportFileName.Add(_T("K2UP_���ǰ���ڷ���"));				m_arrReportType.Add(BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE);
		m_arrData.Add(_T("�ֹ��ڷ���(������)"));			m_arrReportFileName.Add(_T("K2UP_�ֹ��ڷ���_������"));			m_arrReportType.Add(BO_ACQ_ORDER_DATA_DIVNO);
		m_arrData.Add(_T("�ֹ��ڷ���(����ó��)"));		m_arrReportFileName.Add(_T("K2UP_�ֹ��ڷ���_����ó��"));			m_arrReportType.Add(BO_ACQ_ORDER_DATA_PROVIDER);
		m_arrData.Add(_T("�ֹ��ڷ���(�������к�)"));		m_arrReportFileName.Add(_T("K2UP_�ֹ��ڷ���_�������к�"));		m_arrReportType.Add(BO_ACQ_ORDER_DATA_SUBJECTCODE);
	}
	else if(m_nSpeciesType == BO_ORDER)
	{
		SetWindowText(_T("�˼�����ڷ� ��¹� ����Ʈ"));
		m_arrData.Add(_T("�˼�����ڷ���"));			m_arrReportFileName.Add(_T("K2UP_�˼�����ڷ���"));			m_arrReportType.Add(BO_ACQ_CHECKINING_DATA);
		m_arrData.Add(_T("�ֹ��ڷ���(������)"));		m_arrReportFileName.Add(_T("K2UP_�ֹ��ڷ���_������"));		m_arrReportType.Add(BO_ACQ_ORDER_DATA_DIVNO);
		m_arrData.Add(_T("�ֹ��ڷ���(����ó��)"));	m_arrReportFileName.Add(_T("K2UP_�ֹ��ڷ���_����ó��"));		m_arrReportType.Add(BO_ACQ_ORDER_DATA_PROVIDER);
		m_arrData.Add(_T("�ֹ��ڷ���(�������к�)"));	m_arrReportFileName.Add(_T("K2UP_�ֹ��ڷ���_�������к�"));	m_arrReportType.Add(BO_ACQ_ORDER_DATA_SUBJECTCODE);
		m_arrData.Add(_T("�˼��ڷ���"));				m_arrReportFileName.Add(_T("K2UP_�˼��ڷ���"));				m_arrReportType.Add(BO_ACQ_CHECKIN_DATA);
		m_arrData.Add(_T("�̰˼��ڷ���"));			m_arrReportFileName.Add(_T("K2UP_�̰˼��ڷ���"));				m_arrReportType.Add(BO_ACQ_NONCHECKIN_DATA);
		m_arrData.Add(_T("���Խ�û�ڷᵵ�����"));		m_arrReportFileName.Add(_T("K2UP_���Խ�û�ڷᵵ�����"));		m_arrReportType.Add(BO_ACQ_CHECKIN_REQUEST_DATA);
		m_arrData.Add(_T("�˼��ڷ����"));				m_arrReportFileName.Add(_T("K2UP_�˼��ڷ����"));				m_arrReportType.Add(BO_ACQ_CHECKIN_STATISTICS);
		m_arrData.Add(_T("�̰˼��ڷ����"));			m_arrReportFileName.Add(_T("K2UP_�̰˼��ڷ����"));				m_arrReportType.Add(BO_ACQ_NONCHECKIN_STATISTICS);
	}
	else if(m_nSpeciesType == BO_MISSING)
	{
		SetWindowText(_T("�̳��ڷ� ��¹� ����Ʈ"));
		m_arrData.Add(_T("�̳��ڷ���"));				m_arrReportFileName.Add(_T("K2UP_�̳��ڷ���"));				m_arrReportType.Add(BO_ACQ_MISSING_DATA);
		m_arrData.Add(_T("�̰˼��ڷ����"));			m_arrReportFileName.Add(_T("K2UP_�̰˼��ڷ����"));				m_arrReportType.Add(BO_ACQ_NONCHECKIN_STATISTICS);
	}
	else if(m_nSpeciesType == BO_REGOBJ && m_nAcqType == PURCHASE)
	{
		SetWindowText(_T("���Ե���ΰ����ڷ� ��¹� ����Ʈ"));
		m_arrData.Add(_T("���Ե���ΰ����ڷ���"));	m_arrReportFileName.Add(_T("K2UP_���Ե���ΰ����ڷ���"));	m_arrReportType.Add(BO_ACQ_PURCHASE_REGOBJ_DATA);
		m_arrData.Add(_T("�˼��Ϸ��ڷ���"));			m_arrReportFileName.Add(_T("K2UP_�˼��Ϸ��ڷ���"));			m_arrReportType.Add(BO_ACQ_CHECKIN_COMPLETE_DATA);
		m_arrData.Add(_T("�˼��ڷ����"));				m_arrReportFileName.Add(_T("K2UP_�˼��ڷ����"));				m_arrReportType.Add(BO_ACQ_CHECKIN_STATISTICS);
	}
	else if(m_nSpeciesType == BO_DONATE)
	{
		SetWindowText(_T("���������ڷ� ��¹� ����Ʈ"));
		m_arrData.Add(_T("���������ڷ���"));			m_arrReportFileName.Add(_T("K2UP_���������ڷ���"));			m_arrReportType.Add(BO_ACQ_DONATE_DATA);
	}
	else if(m_nSpeciesType == BO_NONDONATE)
	{
		SetWindowText(_T("�����̵���ڷ� ��¹� ����Ʈ"));
		m_arrData.Add(_T("�����̵���ڷ���"));		m_arrReportFileName.Add(_T("K2UP_�����̵���ڷ���"));			m_arrReportType.Add(BO_ACQ_NONDONATE_DATA);
	}
	else if(m_nSpeciesType == BO_REGOBJ && m_nAcqType == DONATE)
	{
		SetWindowText(_T("��������ΰ����ڷ� ��¹� ����Ʈ"));
		m_arrData.Add(_T("��������ΰ����ڷ���"));	m_arrReportFileName.Add(_T("K2UP_��������ΰ����ڷ���"));	m_arrReportType.Add(BO_ACQ_DONATE_REGOBJ_DATA);
	}
	else if(m_nSpeciesType == BO_PURCHASE_DATA_PRINT)
	{
		SetWindowText(_T("�����ڷ� ��¹� ����Ʈ"));
		m_arrData.Add(_T("�����ڷ���"));	m_arrReportFileName.Add(_T("K2UP_�����ڷ���"));	m_arrReportType.Add(BO_PURCHASE_DATA);
	}
	else if(m_nSpeciesType == BO_DONATE_DATA_PRINT)
	{
		SetWindowText(_T("�����ڷ� ��¹� ����Ʈ"));
		m_arrData.Add(_T("�����ڷ���"));	m_arrReportFileName.Add(_T("K2UP_�����ڷ���"));	m_arrReportType.Add(BO_DONATE_DATA);
		m_arrData.Add(_T("�����ڸ��"));	m_arrReportFileName.Add(_T("K2UP_�����ڸ��"));		m_arrReportType.Add(BO_ACQ_DEFAULT);      
	}
	INT cnt = m_arrData.GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		m_ComboBox.InsertString(i, (m_arrData.GetAt(i)).GetBuffer(0));
	}
	m_ComboBox.SetCurSel(0);

	OnSelchangecmbPRINTLIST();

	return TRUE;
}

BOOL CBO_ACQ_PrintDlg::Create(CWnd * pParentWnd)
{
	m_bCreate = TRUE;
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_ACQ_PrintDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(m_nReportType < 0) return ;
	INT nExtraInfo;
	if(m_bKOLAS2Format) nExtraInfo = 1000;
	else nExtraInfo = 0;

	::SendMessage(pParentMgr->GetSafeHwnd(), MAKE_PRINT_DM, m_nReportType, nExtraInfo);
}

BOOL CBO_ACQ_PrintDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

INT CBO_ACQ_PrintDlg::Print()
{
	INT ids;
	if(m_bExcelExport)
		ids = PrintExcel();
	else
		ids = PrintPaper();
	m_bExcelExport = FALSE;
	m_bKOLAS2Format = FALSE;
	return ids;
}

VOID CBO_ACQ_PrintDlg::OnSelchangecmbPRINTLIST() 
{
	// TODO: Add your control notification handler code here
	INT cnt = m_ComboBox.GetCount();
	INT nCurSel = m_ComboBox.GetCurSel();
	CString str;
	m_ComboBox.GetWindowText(str);
	m_strReportFileName = m_arrReportFileName.GetAt(nCurSel);
	m_nReportType = m_arrReportType.GetAt(nCurSel);
	if(m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO || m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER ||
		m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE || m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER || m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE)
		GetDlgItem(IDC_btnSERIES_PAPER)->ShowWindow(SW_SHOW);
	else
		GetDlgItem(IDC_btnSERIES_PAPER)->ShowWindow(SW_HIDE);

	if(m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO || m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER ||
		m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE || m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER || m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE ||
		m_nReportType == BO_ACQ_ORDER_DATA_DIVNO || m_nReportType == BO_ACQ_ORDER_DATA_SUBJECTCODE ||
		m_nReportType == BO_ACQ_ORDER_DATA_PROVIDER)
		GetDlgItem(IDC_btnKOLAS2_FORMAT)->ShowWindow(SW_SHOW);
	else
		GetDlgItem(IDC_btnKOLAS2_FORMAT)->ShowWindow(SW_HIDE);
}

VOID CBO_ACQ_PrintDlg::OnbtnSERIESPAPER() 
{
	// TODO: Add your control notification handler code here
	m_bSeriesPaper = TRUE;
	PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
}

VOID CBO_ACQ_PrintDlg::OnbtnEXCELEXPORT() 
{
	// TODO: Add your control notification handler code here
	m_bExcelExport = TRUE;
	PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
}

INT CBO_ACQ_PrintDlg::PrintPaper()
{
	if(m_pPrintDM == NULL) return -1;
	if(m_pPrintDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("��� �����Ͱ� �����ϴ�."));
		return -1;
	}
	CString strAcqYear, strGroupInfo;
	m_pPrintMgr->InitPrintData();
	strAcqYear = _T("�������");
	strGroupInfo = _T("�������");
	if(m_pGroupCollection != NULL && m_pGroupCollection->GetGroupCount() > 0)
	{
		strAcqYear = m_pGroupCollection->GetAcqYear();
		m_pGroupCollection->GetGroupInfo(strGroupInfo);
		if(strAcqYear.IsEmpty()) strAcqYear = _T("�������");
		if(strGroupInfo.IsEmpty()) strGroupInfo = _T("�������");
		
		m_pPrintMgr->AddPrintData(_T("���Գ⵵"), strAcqYear);
		if(m_nAcqType == PURCHASE) m_pPrintMgr->AddPrintData(_T("������ȣ"), strGroupInfo);
		else if(m_nAcqType == DONATE) m_pPrintMgr->AddPrintData(_T("������ȣ"), strGroupInfo);
		else if(m_nAcqType == ACQ_TYPE_INVALID) m_pPrintMgr->AddPrintData(_T("�׷��̸�"), strGroupInfo);
	}
	m_pPrintMgr->SetPrintDMAlias(m_pPrintDM->DB_MGR_ALIAS);
	if(m_bSeriesPaper)
	{
		if(m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO || m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER ||
			m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE)
		{
			m_strReportFileName = _T("K2UP_���Ǵ���ڷ���_���ӿ���");			
		}
		else if(m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO || m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER ||
			m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE)
		{
			m_strReportFileName = _T("K2UP_���ǰ���ڷ���_���ӿ���");	
		}
		m_nReportType = SERIES_PAPER;
		m_pPrintMgr->SetPrintType(m_nReportType);
		m_pPrintMgr->SetSIReportFilename(m_strReportFileName);
	}
	else if(m_bKOLAS2Format)
	{
		if(m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO || m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER ||
			m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE)
		{
			m_strReportFileName = _T("K2_���Ǵ���ڷ���");			
		}
		else if(m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO || m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER ||
			m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE)
		{
			m_strReportFileName = _T("K2_���ǰ���ڷ���");	
		}
		else if(m_nReportType == BO_ACQ_ORDER_DATA_DIVNO || m_nReportType == BO_ACQ_ORDER_DATA_PROVIDER ||
			m_nReportType == BO_ACQ_ORDER_DATA_SUBJECTCODE)
		{
			m_strReportFileName = _T("K2_�ֹ��ڷ���");	
		}
		
		m_pPrintMgr->SetPrintType(m_nReportType);
		m_pPrintMgr->SetSIReportFilename(m_strReportFileName);
	}
	else
	{            
		CString strReportFileName;
		m_pPrintMgr->SetPrintType(m_nReportType);
		if(!m_bSpecies && m_nReportType == BO_ACQ_CHECKINING_DATA) 
			strReportFileName = m_strReportFileName + _T("_��");
		else strReportFileName = m_strReportFileName;
		m_pPrintMgr->SetSIReportFilename(strReportFileName);
	}

	INT ids = 0;
	if(m_nReportType != BO_ACQ_PURCHASE_DATA_REGCODE)
		m_pPrintMgr->Print();
	else
		ids = m_pPrintMgr->Print(m_pPrintDM);
	
	m_bSeriesPaper = FALSE;
	return ids;
}

INT CBO_ACQ_PrintDlg::PrintExcel()
{
	CSIReportManager * pSIReportManager = m_pPrintMgr->GetSIReportManager();
	if(pSIReportManager == NULL) return -1;
	CExcelExporter ExcelExporter(this, pSIReportManager, m_pPrintDM, m_strReportFileName);
	INT ids = ExcelExporter.InitExcelExporter();
	if(ids < 0) return ids;

	ids = ExcelExporter.ExeclExport();
	if(ids < 0) return ids;

	return 0;
}

INT CBO_ACQ_PrintDlg::CreatePrintMgr()
{
	if(m_pPrintMgr) return  0;
	else
	{
		m_pPrintMgr = new CBasicPrintMgr(this);
	}
	return 0;
}

VOID CBO_ACQ_PrintDlg::OnbtnKOLAS2FORMAT() 
{
	// TODO: Add your control notification handler code here
	m_bKOLAS2Format = TRUE;
	PostMessage(WM_COMMAND, IDOK, BN_CLICKED);	
}
