// TransferDetail.cpp : implementation file
//

#include "stdafx.h"
#include "TransferDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransferDetail dialog


CTransferDetail::CTransferDetail(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CTransferDetail)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_nIdx = -1;
}

CTransferDetail::~CTransferDetail()
{
}


VOID CTransferDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransferDetail)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTransferDetail, CDialog)
	//{{AFX_MSG_MAP(CTransferDetail)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransferDetail message handlers

BOOL CTransferDetail::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	//viewer setting 
	m_BookDetailInfoViewer.InitViewer(IDC_gridTRA_DETAIL, this);
	m_BookDetailInfoViewer.SetViewerWidth(20,75);
	
	
	CString tmp = _T("");
	tmp = m_pDM->GetCellData(_T("ISS_ǥ������"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("����"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("ISS_�����ڻ���"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("������"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("UDF_�����"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("�����"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("ISS_ISSN"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("ISSN"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("ISS_�����ڻ���"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("������"),tmp,TRUE);
	

	tmp = m_pDM->GetCellData(_T("SV_��ȣǥ��"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("��ȣǥ��"),tmp,TRUE);
	

	tmp = m_pDM->GetCellData(_T("SV_��ȣ��"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("��ȣ��"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("SV_��������ȣ��"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("��������ȣ��"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("UDF_�����"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("�����"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("SV_������"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("������"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("SB_�Լ���"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("�Լ���"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("UDF_�ڷ��"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("�ڷ��"),tmp,TRUE);

	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("SV_����"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("����"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("SV_�����"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("�����"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("SV_������Ư��"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("������Ư��"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("SV_åũ��"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("åũ��"),tmp,TRUE);

	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("SV_��ȣ����"),m_nIdx);

	if(_T("1") == tmp)
		m_BookDetailInfoViewer.SetControlData(_T("��ȣ����"),_T("�Ϲ�"),TRUE);
	else if(_T("2") == tmp)	
		m_BookDetailInfoViewer.SetControlData(_T("��ȣ����"),_T("�պ�ȣ"),TRUE);

	m_BookDetailInfoViewer.Display();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CTransferDetail::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
