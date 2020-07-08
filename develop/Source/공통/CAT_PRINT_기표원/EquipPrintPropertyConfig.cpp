// EquipPrintPropertyConfig.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "EquipPrintPropertyConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEquipPrintPropertyConfig dialog


CEquipPrintPropertyConfig::CEquipPrintPropertyConfig(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CEquipPrintPropertyConfig)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// KOL.UDF.022 ��ť���ڵ� ����
	m_pCM = NULL;
}


VOID CEquipPrintPropertyConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEquipPrintPropertyConfig)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEquipPrintPropertyConfig, CDialog)
	//{{AFX_MSG_MAP(CEquipPrintPropertyConfig)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEquipPrintPropertyConfig message handlers

BOOL CEquipPrintPropertyConfig::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if (InitESL_Mgr(_T("SM_����_���_���_�Ӽ�_����")) < 0) 
	{
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_����_���_���_�Ӽ�_����"));
	
	if ( m_pCM == NULL)
		return FALSE;


	CString sUnit;
	CString m_sUnit;
	
	CComboBox * pRegCode;	
	CString sRegCode;
	pRegCode=(CComboBox *)GetDlgItem(IDC_CobUnit);

	pRegCode->AddString(_T("�ȼ�(pixel)"));
	pRegCode->AddString(_T("�и�����(mm)"));
	if ( sUnit == _T("�и�����(mm)") ) {
		pRegCode->SetCurSel(0);
		m_sUnit = _T("�и�����(mm)");
	}
	else {
		pRegCode->SetCurSel(1);
		m_sUnit = _T("�ȼ�(pixel)");
	}

	


/*
	CPrintPropertyInfo *pInfo = NULL;

	// ã�Ƽ� ������ �����Ѵ�.
	if (!m_mapPrintPropertyInfo.Lookup(m_strPrintFormatKey, (VOID*&)pInfo))
	{
		pInfo = new CPrintPropertyInfo;
		m_mapPrintPropertyInfo.SetAt(m_strPrintFormatKey, pInfo);
	}

	if (!pInfo) return -1;

	// ������Ƽ�� �����ͼ� �����ش�.
	CString strData;
	
	strData.Format(_T("%d"), pInfo->m_nMarginUp);
	m_pCM->SetControlMgrData(_T("���ʿ���"), strData);

	strData.Format(_T("%d"), pInfo->m_nMarginSide);
	m_pCM->SetControlMgrData(_T("���ʿ���"), strData);

	strData.Format(_T("%d"), pInfo->m_nSpaceVert);
	m_pCM->SetControlMgrData(_T("���ΰ���"), strData);

	strData.Format(_T("%d"), pInfo->m_nSpaceHorz);
	m_pCM->SetControlMgrData(_T("���ΰ���"), strData);

	strData.Format(_T("%d"), pInfo->m_nLabelHight);
	m_pCM->SetControlMgrData(_T("���̺����"), strData);

	strData.Format(_T("%d"), pInfo->m_nLabelWidth);
	m_pCM->SetControlMgrData(_T("���̺�ʺ�"), strData);

	strData.Format(_T("%d"), pInfo->m_nRowCount);
	m_pCM->SetControlMgrData(_T("���ΰ���"), strData);

	strData.Format(_T("%d"), pInfo->m_nColCount);
	m_pCM->SetControlMgrData(_T("���ΰ���"), strData);

	strData.Format(_T("%d"), pInfo->m_nMarginBarCode);
	m_pCM->SetControlMgrData(_T("���ڵ忩��"), strData);

	// ��Ʈ
	CString strFont;
	strFont.Format(_T("%s, %d"), pInfo->m_strFontFace, pInfo->m_nFontSize);
	GetDlgItem(IDC_bFONT)->SetWindowText(strFont);	
*/	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
