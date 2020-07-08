// CheckinVolSpeciesInfo.cpp : implementation file
//

#include "stdafx.h"
#include "CheckinVolSpeciesInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckinVolSpeciesInfo dialog


CCheckinVolSpeciesInfo::CCheckinVolSpeciesInfo(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CCheckinVolSpeciesInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckinVolSpeciesInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM_IDX = NULL;
	m_strRECEIPT_NO = _T("");
	m_strACQ_YEAR = _T("");
	m_nACQ_TYPE = 1; //default
}

CCheckinVolSpeciesInfo::~CCheckinVolSpeciesInfo()
{
}


VOID CCheckinVolSpeciesInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckinVolSpeciesInfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckinVolSpeciesInfo, CDialog)
	//{{AFX_MSG_MAP(CCheckinVolSpeciesInfo)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckinVolSpeciesInfo message handlers

BOOL CCheckinVolSpeciesInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(InitESL_Mgr(_T("����_����_üũ��_�Ǵ���_������"))<0) {
		AfxMessageBox(_T("InitESL_Mgr Error : ����_����_üũ��_�Ǵ���_������"));
		return FALSE;
	}
	
	m_pDM_IDX = FindDM(_T("DM_üũ��_����"));
	
	if(!m_pDM_IDX) 	{
		AfxMessageBox(_T("DM Error : ����_����_üũ��_�Ǵ���_������"));
		return FALSE;
	}
	
	//viewer setting 
	m_SpeciesInfoViewer.InitViewer(IDC_gridCHECKIN_VOL_SPECIES, this);
	m_SpeciesInfoViewer.SetViewerWidth(20,75);
	
	m_AcqInfoViewer.InitViewer(IDC_gridCHECKIN_VOL_SPECIES_ACQ, this);
	m_AcqInfoViewer.SetViewerWidth(20,75);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CCheckinVolSpeciesInfo::ShowSpeciesInfo(INT nIdx)
{

	CString tmp = _T("");
	tmp = m_pDM_IDX->GetCellData(_T("ISS_ǥ������"),nIdx);
	m_SpeciesInfoViewer.SetControlData(_T("����"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM_IDX->GetCellData(_T("ISS_�����ڻ���"),nIdx);
	m_SpeciesInfoViewer.SetControlData(_T("������"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM_IDX->GetCellData(_T("UDF_�����"),nIdx);
	m_SpeciesInfoViewer.SetControlData(_T("�����"),tmp,TRUE);
	
	
	tmp = _T("");
	tmp = m_pDM_IDX->GetCellData(_T("ISS_�������"),nIdx);
	CString strDesc;
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("��������ڵ�"),tmp,strDesc);
	m_SpeciesInfoViewer.SetControlData(_T("�������"),strDesc,TRUE);
	strDesc = _T("");

	tmp = _T("");
	tmp = m_pDM_IDX->GetCellData(_T("ISS_ISSN"),nIdx);
	m_SpeciesInfoViewer.SetControlData(_T("ISSN"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM_IDX->GetCellData(_T("SS_��������"),nIdx);
	m_SpeciesInfoViewer.SetControlData(_T("��������"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM_IDX->GetCellData(_T("SS_�谡��ġ_�ڵ�"),nIdx);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("�ڷ�Ǳ���"),tmp,strDesc);
	m_AcqInfoViewer.SetControlData(_T("�ڷ��"),strDesc,TRUE);
	strDesc = _T("");
	
	tmp = _T("");
	if(!m_strACQ_YEAR.IsEmpty())
	{
		tmp.Format(_T("%s - %s"),m_strACQ_YEAR,m_strRECEIPT_NO);
	}
	
	m_AcqInfoViewer.SetControlData(_T("������ȣ"),tmp,TRUE);

	CString strSpeciesKey = m_pDM_IDX->GetCellData(_T("ISS_��KEY"),nIdx);
	

	if(1 == m_nACQ_TYPE) ShowPurchaseInfo(strSpeciesKey);
	else if(2 == m_nACQ_TYPE) ShowDonateInfo(strSpeciesKey);


	m_SpeciesInfoViewer.Display();


	return 0;
}


VOID CCheckinVolSpeciesInfo::ShowPurchaseInfo(CString strSpeciesKey)
{
	CESL_DataMgr* pDM_ACQ = FindDM(_T("DM_üũ��_�Ǵ���_������_��������"));
	if(!pDM_ACQ || strSpeciesKey.IsEmpty()) return;
	
	CString strQuery;
	strQuery.Format(_T("SPECIES_KEY = %s"),strSpeciesKey);
	
	pDM_ACQ->RefreshDataManager(strQuery);

	if(0 == pDM_ACQ->GetRowCount()) return;

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM_ACQ,_T("����ó����"),_T("SP_����ó_�ڵ�"));

	CString tmp = _T("");
	tmp = pDM_ACQ->GetCellData(_T("SP_�����켱����"),0);
	m_AcqInfoViewer.SetControlData(_T("�����켱����"),tmp,TRUE);
	
	tmp = _T("");
	tmp = pDM_ACQ->GetCellData(_T("SP_����ó_�ڵ�"),0);
	m_AcqInfoViewer.SetControlData(_T("����ó"),tmp,TRUE);
		
	tmp = _T("");
	tmp = pDM_ACQ->GetCellData(_T("SP_���å��"),0);
	m_AcqInfoViewer.SetControlData(_T("���å��"),tmp,TRUE);	
	
	tmp = _T("");
	CString strStartDate,strEndDate;
	
	strStartDate = pDM_ACQ->GetCellData(_T("SP_����������"),0);
	strEndDate = pDM_ACQ->GetCellData(_T("SP_����������"),0);
	
	if(!strStartDate.IsEmpty() || !strEndDate.IsEmpty())
	{
		tmp = strStartDate + _T(" ~ ") + strEndDate;
	}

	m_AcqInfoViewer.SetControlData(_T("�����Ⱓ"),tmp,TRUE);
	

	CString strValue;
	GetManageValue( _T("��Ÿ") , _T("����") , _T("����") , _T("����") , strValue );
	if ( strValue == _T("Y") )
	{
		CString strDesc;
		tmp = pDM_ACQ->GetCellData(_T("SP_��û�а�"),0);
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("�а�"),tmp,strDesc);
		m_AcqInfoViewer.SetControlData(_T("��û�а�"),strDesc);
	}

	m_AcqInfoViewer.Display();

}

VOID CCheckinVolSpeciesInfo::ShowDonateInfo(CString strSpeciesKey)
{
	CESL_DataMgr* pDM_ACQ = FindDM(_T("DM_üũ��_�Ǵ���_������_��������"));
	if(!pDM_ACQ || strSpeciesKey.IsEmpty()) return;
	
	CString strQuery;
	strQuery.Format(_T("SPECIES_KEY = %s"),strSpeciesKey);

	pDM_ACQ->RefreshDataManager(strQuery);

	if(0 == pDM_ACQ->GetRowCount()) return;
	
	CString tmp = _T("");
	tmp = pDM_ACQ->GetCellData(_T("SD_�����켱����"),0);
	m_AcqInfoViewer.SetControlData(_T("�����켱����"),tmp,TRUE);
			
	tmp = _T("");
	tmp = pDM_ACQ->GetCellData(_T("SD_�����ڸ�"),0);
	m_AcqInfoViewer.SetControlData(_T("�����ڸ�"),tmp,TRUE);

	tmp = _T("");
	tmp = pDM_ACQ->GetCellData(_T("SD_���å��"),0);
	m_AcqInfoViewer.SetControlData(_T("���å��"),tmp,TRUE);	
	
	tmp = _T("");
	tmp = pDM_ACQ->GetCellData(_T("SD_����������"),0);
	m_AcqInfoViewer.SetControlData(_T("����������"),tmp,TRUE);


	CString strValue;
	GetManageValue( _T("��Ÿ") , _T("����") , _T("����") , _T("����") , strValue );
	if ( strValue == _T("Y") )
	{
		CString strDesc;
		tmp = pDM_ACQ->GetCellData(_T("SD_��û�а�"),0);
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("�а�"),tmp,strDesc);
		m_AcqInfoViewer.SetControlData(_T("��û�а�"),strDesc);
	}

	m_AcqInfoViewer.Display();

}

VOID CCheckinVolSpeciesInfo::SetReceiptNo(CString strYear, CString strNo)
{
	m_strRECEIPT_NO = strNo;
	m_strACQ_YEAR = strYear;
}

BOOL CCheckinVolSpeciesInfo::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}


HBRUSH CCheckinVolSpeciesInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
