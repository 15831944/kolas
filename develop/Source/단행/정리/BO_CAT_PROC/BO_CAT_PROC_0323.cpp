// BO_CAT_PROC_0323.cpp : implementation file
//

#include "stdafx.h"
//#include _T("bo_cat_proc.h")
#include "BO_CAT_PROC_0323.h"
#include "BO_CAT_PROC_0320_FORM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0323 dialog


CBO_CAT_PROC_0323::CBO_CAT_PROC_0323(CESL_Mgr* pParent, CMarc &m_marc /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0323)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CBO_CAT_PROC_0323::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0323)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0323, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0323)
	ON_BN_CLICKED(IDC_bTAG_008_EDIT, OnbTAG008EDIT)
	ON_BN_CLICKED(IDC_bVARIABLE, OnbVARIABLE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0323 message handlers

BOOL CBO_CAT_PROC_0323::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_CAT_PROC_0323::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	if (InitESL_Mgr(_T("BO_CAT_PROC_0323")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	//decoding
	CString strTagCode, strErrMsg, strHelpMsg;
	
	if(m_pInfo->pMarcMgr->Decoding(strStreamMarc, &m_marc))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
//			m_strOutput += ( strErrMsg + strHelpMsg + _T("\r\n"));
			UpdateData(FALSE);
		}	
	}

	CString sItemData;	
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@0-5"), sItemData);	
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0323"), _T("�Է³�¥"), sItemData);
	}
	else {
		//sItemData = _T("");
		GetControlData( _T("CM_BO_CAT_PROC_0323"), _T("��¥") , sItemData );
		CString sDate;
		sDate.Format( _T("%s%s%s"), sItemData.Mid(2,2), sItemData.Mid(5,2), sItemData.Right(2) );
		SetControlData( _T("CM_BO_CAT_PROC_0323"), _T("�Է³�¥") , sDate );
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@6"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0323"), _T("����"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@7-10"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0323"), _T("�����1"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@11-14"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0323"), _T("�����2"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@15-17"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0323"), _T("���౹��"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@26-27"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0323"), _T("���к�ȣ"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@28"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0323"), _T("�������ڵ�"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@32"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0323"), _T("�������"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@35-37"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0323"), _T("���"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@38-39"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0323"), _T("���α��"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@18-19"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0323"), _T("��������"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@20"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0323"), _T("�Ǻ�����"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@22"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0323"), _T("�̿��ڴ�����"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@23"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0323"), _T("�ڷ�����"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@24"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0323"), _T("�����ڷ�1"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@25"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0323"), _T("�����ڷ�2"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@30"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0323"), _T("�����ڷ��ǳ���1"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@31"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0323"), _T("�����ڷ��ǳ���2"), sItemData);
	}

	/*
	CESL_ControlMgr *pCM;
	INT ids;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0323"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_PROC_0323 error") );
		return FALSE;
	}
	//iCh = 1;

	CESL_Control_Element *pCME = NULL;
	POSITION pos = pCM->Control_List.GetHeadPosition();
	CString strData;
	CString strAlias;
	CString s008;
	while(pos)
	{
		pCME = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if (pCME == NULL) continue;
		
		strAlias = pCME->GetAlias();
		ids = pCME->GetControlMgrData(strData);
		if (ids < 0) continue;

		// ALIAS = strAlias, DATA = strData
		if ( strData == _T("") ) {
			strData = _T(" ");
		}
		if ( strData == _T("") && strAlias == _T("��Һ���")) {
			strData = _T("   ");
		}
		s008 = s008 + strData;
	}
	*/

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_CAT_PROC_0323::OnbTAG008EDIT() 
{
	// TODO: Add your control notification handler code here
	CBO_CAT_PROC_0320_FORM dlg(this);
	//dlg.DoModal();
	if( IDOK == dlg.DoModal() ) {
		m_pInfo->pMarcMgr->DeleteField(&m_marc,_T("007"));
		m_pInfo->pMarcMgr->SetItem(&m_marc, _T("007*"), dlg.s007);
		m_pInfo->pMarcMgr->SortMarc( &m_marc );
	}	
}

VOID CBO_CAT_PROC_0323::OnbVARIABLE() 
{
	// TODO: Add your control notification handler code here
	CString sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0323"), _T("�Է³�¥"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@0-5"), sData );

	CString sYear1;
	GetControlData(_T("CM_BO_CAT_PROC_0323"), _T("�����1"), sYear1);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@7-10"), sYear1 );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0323"), _T("�����2"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@11-14"), sData );

	sYear1 = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0323"), _T("����"), sYear1);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@6"), sYear1 );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0323"), _T("���౹��"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@15-17"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0323"), _T("���к�ȣ"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@26-27"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0323"), _T("�������ڵ�"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@28"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0323"), _T("�������"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@32"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0323"), _T("���"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@35-37"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0323"), _T("���α��"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@38-39"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0323"), _T("��������"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@18-19"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0323"), _T("�Ǻ�����"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@20"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0323"), _T("�̿��ڴ�����"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@22"), sData );
	
	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0323"), _T("�ڷ�����"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@23"), sData );
	
	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0323"), _T("�����ڷ�1"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@24"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0323"), _T("�����ڷ�2"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@25"), sData );
	
	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0323"), _T("�����ڷ��ǳ���1"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@30"), sData );
	
	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0323"), _T("�����ڷ��ǳ���2"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@31"), sData );

	CDialog::OnOK();
}
