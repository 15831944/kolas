// SeSpeciesDelete.cpp : implementation file
//

#include "stdafx.h"
#include "SeSpeciesDelete.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeSpeciesDelete dialog


CSeSpeciesDelete::CSeSpeciesDelete(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeSpeciesDelete)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM = NULL;
	m_strSpeciesKey = _T("");
}
CSeSpeciesDelete::~CSeSpeciesDelete()
{
}

VOID CSeSpeciesDelete::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeSpeciesDelete)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeSpeciesDelete, CDialog)
	//{{AFX_MSG_MAP(CSeSpeciesDelete)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeSpeciesDelete message handlers

BOOL CSeSpeciesDelete::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	
	if(InitESL_Mgr(_T("����_������"))<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_������") );
		return FALSE;
	}
	
	m_pDM = FindDM(_T("DM_����_������"));
	

	if(!m_pDM) {
		AfxMessageBox( _T("DM Error : ����_������") );
		return FALSE;
	}	

	//viewer setting 
	m_SpeciesInfoViewer.InitViewer(IDC_gridAPI_SPECIES_DELETE, this);
	m_SpeciesInfoViewer.SetViewerWidth(30,68);
	
	if(m_strSpeciesKey.IsEmpty())
	{
		AfxMessageBox(_T("��KEY�� ã�� �� �����ϴ�!"));
		EndDialog(0);
	}

	
	if(ShowSpeciesInfo()<0)
	{
		AfxMessageBox(_T("�� ������ ã�� �� �����ϴ�!"));
		EndDialog(0);
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


INT CSeSpeciesDelete::ShowSpeciesInfo()
{
	
	CString strWhere;
	strWhere.Format(_T("S.REC_KEY = %s AND S.REC_KEY = I.REC_KEY"),m_strSpeciesKey);
	
	m_pDM->RefreshDataManager(strWhere);

	if(0 == m_pDM->GetRowCount()) return -1;

	
	CString tmp = _T("");
	tmp = m_pDM->GetCellData(_T("�����ȣ"),0);
	m_SpeciesInfoViewer.SetControlData(_T("�����ȣ"),tmp,TRUE);



	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("����"),0);
	m_SpeciesInfoViewer.SetControlData(_T("����"),tmp,TRUE);

	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("������"),0);
	m_SpeciesInfoViewer.SetControlData(_T("������"),tmp,TRUE);
	

	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("ISSN"),0);
	m_SpeciesInfoViewer.SetControlData(_T("ISSN"),tmp,TRUE);
	
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("�����"),0);
	CString strDesc;
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("������ڵ�"),tmp,strDesc);
	m_SpeciesInfoViewer.SetControlData(_T("�����"),strDesc,TRUE);
	strDesc = _T("");


	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("�������"),0);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("��������ڵ�"),tmp,strDesc);
	m_SpeciesInfoViewer.SetControlData(_T("�������"),strDesc,TRUE);
	strDesc = _T("");

	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("�Է���"),0);
	m_SpeciesInfoViewer.SetControlData(_T("�Է���"),tmp,TRUE);


	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("�Ǽ�"),0);
	m_SpeciesInfoViewer.SetControlData(_T("�Ǽ�"),tmp,TRUE);


	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("���ԱǼ�"),0);
	m_SpeciesInfoViewer.SetControlData(_T("���ԱǼ�"),tmp,TRUE);


	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("����å��"),0);
	m_SpeciesInfoViewer.SetControlData(_T("����å��"),tmp,TRUE);
	

	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("�����Ǽ�"),0);
	m_SpeciesInfoViewer.SetControlData(_T("�����Ǽ�"),tmp,TRUE);


	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("����å��"),0);
	m_SpeciesInfoViewer.SetControlData(_T("����å��"),tmp,TRUE);
	

	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("����"),0);
	m_SpeciesInfoViewer.SetControlData(_T("����"),tmp,TRUE);


	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("�ηϼ�"),0);
	m_SpeciesInfoViewer.SetControlData(_T("�ηϼ�"),tmp,TRUE);


	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("�����Լ���ȣ"),0);
	m_SpeciesInfoViewer.SetControlData(_T("�����Լ���ȣ"),tmp,TRUE);


	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("�����Լ���ȣ"),0);
	m_SpeciesInfoViewer.SetControlData(_T("�����Լ���ȣ"),tmp,TRUE);

	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("����å�Լ���"),0);
	m_SpeciesInfoViewer.SetControlData(_T("����å�Լ���"),tmp,TRUE);


	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("����å�Լ���"),0);
	m_SpeciesInfoViewer.SetControlData(_T("����å�Լ���"),tmp,TRUE);



	m_SpeciesInfoViewer.Display();
	

	return 0;
}

HBRUSH CSeSpeciesDelete::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
