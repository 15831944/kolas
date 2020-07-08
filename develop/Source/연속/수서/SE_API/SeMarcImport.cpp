// SeMarcImport.cpp : implementation file
//

#include "stdafx.h"
#include "SeMarcImport.h"
#include "SeMarcImportMarcView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeMarcImport dialog


CSeMarcImport::CSeMarcImport(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeMarcImport::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeMarcImport)
	m_nImportOption = -1;
	//}}AFX_DATA_INIT
	
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_nCurrentIdx = -1;

}


CSeMarcImport::~CSeMarcImport()
{
}



VOID CSeMarcImport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeMarcImport)
	DDX_Radio(pDX, IDC_radMARC_IMPORT1, m_nImportOption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeMarcImport, CDialog)
	//{{AFX_MSG_MAP(CSeMarcImport)
	ON_BN_CLICKED(IDC_btnMARC_IMPORT_SEARCH, OnbtnMARCIMPORTSEARCH)
	ON_BN_CLICKED(IDC_btnMARC_IMPORT_CLEAR, OnbtnMARCIMPORTCLEAR)
	ON_BN_CLICKED(IDC_btnMARC_IMPORT_MARC_VIEW, OnbtnMARCIMPORTMARCVIEW)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeMarcImport message handlers

BOOL CSeMarcImport::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("����_����_��ũ����"))<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_��ũ����") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_����_��ũ����"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("�׸���"));
	m_pDM = FindDM(_T("DM_����_��ũ����"));
	

	if(!m_pDM || !m_pCM || !m_pGrid) {
		AfxMessageBox( _T("DM,CM,grid Error : ����_����_��ũ����") );
		return FALSE;
	}	

	m_nImportOption = 0;
	UpdateData(FALSE);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSeMarcImport::OnbtnMARCIMPORTSEARCH() 
{
	// TODO: Add your control notification handler code here
	


	CString ISSN1 = _T("");						m_pCM->GetControlMgrData(_T("ISSN1"),ISSN1);
	CString ISSN2 = _T("");						m_pCM->GetControlMgrData(_T("ISSN2"),ISSN2);		
	CString PUB_FREQ = _T("");					m_pCM->GetControlMgrData(_T("�����"),PUB_FREQ);
	CString TITLE= _T("");						m_pCM->GetControlMgrData(_T("������"),TITLE);
	CString PUBLISHER = _T("");					m_pCM->GetControlMgrData(_T("������"),PUBLISHER);
	CString KEYWORD = _T("");					m_pCM->GetControlMgrData(_T("Ű����"),KEYWORD);
	CString MEDIA_CODE = _T("");				m_pCM->GetControlMgrData(_T("��ü����"),MEDIA_CODE);
	CString CLASS_TYPE = _T("");				m_pCM->GetControlMgrData(_T("�з���ȣ"),CLASS_TYPE);
	CString CLASS_TYPE1 = _T("");				m_pCM->GetControlMgrData(_T("�з���ȣ1"),CLASS_TYPE1);
	CString CLASS_TYPE2 = _T("");				m_pCM->GetControlMgrData(_T("�з���ȣ2"),CLASS_TYPE2);



	ISSN1 = TrimAll(ISSN1);
	ISSN2 = TrimAll(ISSN2);
	PUB_FREQ = TrimAll(PUB_FREQ);
	TITLE = TrimAll(TITLE);
	PUBLISHER = TrimAll(PUBLISHER);
	KEYWORD = TrimAll(KEYWORD);
	MEDIA_CODE = TrimAll(MEDIA_CODE);
	CLASS_TYPE = TrimAll(CLASS_TYPE);
	CLASS_TYPE1 = 	TrimAll(CLASS_TYPE1);
	CLASS_TYPE2 = 	TrimAll(CLASS_TYPE2);


	CString strQuery = _T("");
	CString tmpquery = _T("");

	
	//�������� ���Ͽ� �˻�
	//===================================================
	//2010.01.11 ADD BY PJW : ����, �۾� �Ѵ� �˻��ϵ��� �Ѵ�.
//	strQuery = _T("S.LOC_FLAG = '0' AND I.REC_KEY = S.REC_KEY");
	strQuery = _T("S.LOC_FLAG IN ('0', '1') AND I.REC_KEY = S.REC_KEY");
	//===================================================

	//�з���ȣ �˻� 
	if(_T("1") == CLASS_TYPE)
	{
		tmpquery = MakeRangeQuery(_T("KDC_CLASS"),CLASS_TYPE1,CLASS_TYPE2);
		if(!tmpquery.IsEmpty()) 
		{
			strQuery+= _T(" AND ");
			strQuery += tmpquery;
		}
	} 
	else if(_T("2") == CLASS_TYPE)
	{
		tmpquery = MakeRangeQuery(_T("DDC_CLASS"),CLASS_TYPE1,CLASS_TYPE2);
		if(!tmpquery.IsEmpty()) 
		{
			strQuery+= _T(" AND ");
			strQuery += tmpquery;
		}
	}



	//����
	if(!TITLE.IsEmpty())
	{
		strQuery += _T(" AND ");
		tmpquery = MakeCatsearch(_T("TITLE"), TITLE);
		strQuery += tmpquery;
	}
	

	//������
	if(!PUBLISHER.IsEmpty())
	{
		strQuery += _T(" AND ");
		tmpquery = MakeCatsearch(_T("PUBLISHER"), PUBLISHER);
		strQuery += tmpquery;
	}

	//Ű���� 
	if(!KEYWORD.IsEmpty())
	{
		strQuery += _T(" AND ");
		tmpquery = MakeCatsearch(_T("KEYWORD"), KEYWORD);
		strQuery += tmpquery;
	}


	//��ü����
	if(!MEDIA_CODE.IsEmpty())
	{
		strQuery += _T(" AND ");
		tmpquery.Format(_T(" I.MEDIA_CODE = '%s'"), MEDIA_CODE);
		strQuery += tmpquery;
	}


	//����� 
	if(!PUB_FREQ.IsEmpty())
	{
		strQuery += _T(" AND ");
		tmpquery.Format(_T(" I.PUB_FREQ = '%s'"), PUB_FREQ);
		strQuery += tmpquery;
	}

	
	//ISSN
	CString ISSN;
	if(!ISSN1.IsEmpty())
	{
		if(!ISSN2.IsEmpty())
		{
			ISSN.Format(_T("%s-%s"),ISSN1,ISSN2);
			strQuery += _T(" AND ");
			tmpquery.Format(_T(" I.ST_ISSN = '%s'"), ISSN);
			strQuery += tmpquery;
		}

	}
	
	m_pDM->RefreshDataManager(strQuery);
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("������ڵ�"),_T("UDF_�����"));
	m_pGrid->Display();


}

VOID CSeMarcImport::OnbtnMARCIMPORTCLEAR() 
{
	// TODO: Add your control notification handler code here
	m_pCM->AllControlClear();

}

VOID CSeMarcImport::OnbtnMARCIMPORTMARCVIEW() 
{
	// TODO: Add your control notification handler code here
	if(m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("���õ� �׸��� �����ϴ�!"));
		return;
	}


	m_Marc.Init();
	CString strStreamMarc;
	strStreamMarc = m_pDM->GetCellData(_T("MARC"),m_nCurrentIdx);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
	m_pInfo->pMarcMgr->Decoding(strStreamMarc, &m_Marc);	

	
	CSeMarcImportMarcView dlg(this);
	dlg.m_pMarc = &m_Marc;
	dlg.DoModal();


}

VOID CSeMarcImport::OnOK() 
{
	// TODO: Add extra validation here
	
	UpdateData(TRUE);
	m_Marc.Init();
	CString strStreamMarc;
	strStreamMarc = m_pDM->GetCellData(_T("MARC"),m_nCurrentIdx);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
	m_pInfo->pMarcMgr->Decoding(strStreamMarc, &m_Marc);	

	CDialog::OnOK();
}

CString CSeMarcImport::MakeRangeQuery(CString strFieldName, CString strData1, CString strData2)
{
	CString tmpquery = _T("");	
	INT nOption;
	
	if(!strData1.IsEmpty()) nOption = 1;	
	if(!strData2.IsEmpty()) nOption = 2;	
	if(!strData1.IsEmpty() && !strData2.IsEmpty()) nOption = 3;

	if(nOption != 0)
	{
		if(nOption == 1) tmpquery.Format(_T("%s >= '%s'"),strFieldName,strData1);
		if(nOption == 2) tmpquery.Format(_T("%s <= '%s'"),strFieldName,strData2);
		if(nOption == 3) tmpquery.Format(_T("%s BETWEEN '%s' AND '%s'"),strFieldName,strData1,strData2);
	}

	return tmpquery;
}

CString CSeMarcImport::MakeCatsearch(CString strFieldName,CString strData)
{
	CString tmpquery = _T("");
	
	if(ORA_CATSEARCH == m_pInfo->GetSearchIndexType())
	{
		tmpquery.Format(_T(" CATSEARCH(I.IDX_%s, '%s*', NULL)>0"),strFieldName, strData);
	}
	else
	{
		tmpquery.Format(_T(" &ECOSEARCH(I.REC_KEY,SE_IDX_%s, '%s*')"),strFieldName, strData);
	}

	return tmpquery;
}

CString CSeMarcImport::TrimAll(CString strData)
{
	strData.TrimLeft();
	strData.TrimRight();

	return strData;
}

BEGIN_EVENTSINK_MAP(CSeMarcImport, CDialog)
    //{{AFX_EVENTSINK_MAP(CSeMarcImport)
	ON_EVENT(CSeMarcImport, IDC_gridMARC_IMPORT, -600 /* Click */, OnClickgridMARCIMPORT, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSeMarcImport::OnClickgridMARCIMPORT() 
{
	// TODO: Add your control notification handler code here
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		
		m_nCurrentIdx = row-1;
	
	}
}

BOOL CSeMarcImport::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_RETURN )
		{
			OnbtnMARCIMPORTSEARCH();
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CSeMarcImport::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
