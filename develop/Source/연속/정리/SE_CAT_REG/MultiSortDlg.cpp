// MultiSortDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MultiSortDlg.h"
#include "ESL_DataMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultiSortDlg dialog


CMultiSortDlg::CMultiSortDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CMultiSortDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMultiSortDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nSortCount = 0;
}

CMultiSortDlg::~CMultiSortDlg()
{
}

VOID CMultiSortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMultiSortDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMultiSortDlg, CDialog)
	//{{AFX_MSG_MAP(CMultiSortDlg)	
	ON_BN_CLICKED(IDC_bSORT_MULTI, OnbSORTMULTI)
	ON_BN_CLICKED(IDC_bDEFAULT_VALUE, OnbDefaultValue)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiSortDlg Operations

INT CMultiSortDlg::InitDefaultValue()
{
	INT nCount = m_arrStrSortCols.GetSize();
	CString strCol;
	CComboBox *pComboBox = NULL;
	
	for (INT idx = 0; idx < nCount; idx++)
	{
		strCol = m_arrStrSortCols.GetAt(idx);
		for (INT j = 0; j < 6; j++)
		{
			pComboBox = static_cast<CComboBox*>(GetDlgItem(m_nSortColumeID[j]));
			if (pComboBox == NULL) continue;

			pComboBox->AddString(strCol);
		}
	}

	INT nCurSel = -1;
	for (idx = 0; idx < 6; idx++)
	{
		pComboBox = static_cast<CComboBox*>(GetDlgItem(m_nSortColumeID[idx]));
		if (pComboBox == NULL) continue;
		nCurSel = pComboBox->AddString(_T("������"));
		if (idx < nCurSel)
			pComboBox->SetCurSel(idx);
		else
			pComboBox->SetCurSel(nCurSel);

		pComboBox = static_cast<CComboBox*>(GetDlgItem(m_nSortCondID[idx]));
		if (pComboBox == NULL) continue;

		pComboBox->AddString(_T("��������"));
		pComboBox->AddString(_T("��������"));
		pComboBox->SetCurSel(0);
	}
	
	CString sPath = _T("..\\CFG\\SerialSortElement.cfg");
	
	FILE *fp;
	
	fp = _tfopen( sPath , _T("rb") );
	if ( fp == NULL ) {
		AfxMessageBox( _T("CFG/SerialSortElement.cfg ������ �����ϴ�.") );
		return -1;
	}			
	
	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode�϶� 2byte�ǳʶ��
	FILE* fpUni = fp;

	if( 0xFF == fgetc(fpUni) ) 
	{
		if( 0xFE != fgetc(fpUni) )
		{
			fseek(fpUni, 0, SEEK_SET);
		}
	}
	else
	{
		fseek(fpUni, 0, SEEK_SET);
	}
	//------------------------------------------
	
	TCHAR cLine[256];
	CString sDesc, sOption;
	INT nPos, nCnt;

	nCnt = 0;
	while( _fgetts( cLine, 256, fp ) ) 
	{
		CString sLine;
		sLine.Format( _T("%s"), cLine );
		if ( sLine != _T("") ) 
		{
			nPos = sLine.Find(_T("-"));
			sDesc = sLine.Left(nPos);

			((CComboBox*)(GetDlgItem(m_nSortColumeID[nCnt])))->SetCurSel(_ttoi(sDesc));
			
			sOption = sLine.Mid(nPos+1);
			sOption.Replace(_T("\n"),_T(""));

			((CComboBox*)(GetDlgItem(m_nSortCondID[nCnt])))->SetCurSel(_ttoi(sOption));

		}
		nCnt++;
		if( nCnt >  5)
			break;
	}

	fclose( fp );

	return 0;
}

INT CMultiSortDlg::Init()
{
	m_arrStrSortCols.Add(_T("��ǥ��"));
	m_arrStrSortCols.Add(_T("����"));
	m_arrStrSortCols.Add(_T("������"));
	m_arrStrSortCols.Add(_T("������ȣ"));
	m_arrStrSortCols.Add(_T("��ȣ��"));
	m_arrStrSortCols.Add(_T("��������ȣ��"));

	m_mapDMAlias_Desc.SetAt( _T("��ǥ��")		,_T("IBS_��ǥ��")     );
	m_mapDMAlias_Desc.SetAt( _T("����")			,_T("IBS_������")     );
	m_mapDMAlias_Desc.SetAt( _T("������")		,_T("IBS_������")     );
	m_mapDMAlias_Desc.SetAt( _T("������ȣ")		,_T("BB_������ȣ")    );
	m_mapDMAlias_Desc.SetAt( _T("��ȣ��")		,_T("BB_������ηϸ�"));
	m_mapDMAlias_Desc.SetAt( _T("��������ȣ��") ,_T("BB_��������ȣ��"));

	m_nSortColumeID[0] = IDC_cSORT_COLUMN_1;
	m_nSortColumeID[1] = IDC_cSORT_COLUMN_2;
	m_nSortColumeID[2] = IDC_cSORT_COLUMN_3;
	m_nSortColumeID[3] = IDC_cSORT_COLUMN_4;
	m_nSortColumeID[4] = IDC_cSORT_COLUMN_5;
	m_nSortColumeID[5] = IDC_cSORT_COLUMN_6;
		
	m_nSortCondID[0] = IDC_cSORT_DIR_1;
	m_nSortCondID[1] = IDC_cSORT_DIR_2;
	m_nSortCondID[2] = IDC_cSORT_DIR_3;
	m_nSortCondID[3] = IDC_cSORT_DIR_4;
	m_nSortCondID[4] = IDC_cSORT_DIR_5;
	m_nSortCondID[5] = IDC_cSORT_DIR_6;
/*
	for (INT idx = 0; idx < 6; idx++)
	{
		m_arrStrSortAlias[idx].Empty();
		m_arrIntSortCond[idx] = ESL_DATAMGR_SORT_ASC;
	}

	// ������ ���� ���鼭 �޺��� ���� �־��ش�.
	INT nCount = m_arrStrSortCols.GetSize();
	CString strCol;
	CComboBox *pComboBox = NULL;
	for (idx = 0; idx < nCount; idx++)
	{
		strCol = m_arrStrSortCols.GetAt(idx);
		for (INT j = 0; j < 6; j++)
		{
			pComboBox = static_cast<CComboBox*>(GetDlgItem(m_nSortColumeID[j]));
			if (pComboBox == NULL) continue;

			pComboBox->AddString(strCol);
		}
	}

	INT nCurSel = -1;
	for (idx = 0; idx < 6; idx++)
	{
		pComboBox = static_cast<CComboBox*>(GetDlgItem(m_nSortColumeID[idx]));
		if (pComboBox == NULL) continue;
		nCurSel = pComboBox->AddString(_T("������"));
		if (idx < nCurSel)
			pComboBox->SetCurSel(idx);
		else
			pComboBox->SetCurSel(nCurSel);

		pComboBox = static_cast<CComboBox*>(GetDlgItem(m_nSortCondID[idx]));
		if (pComboBox == NULL) continue;

		pComboBox->AddString(_T("��������"));
		pComboBox->AddString(_T("��������"));
		pComboBox->SetCurSel(0);
	}

	if (nCount < 6)
	{
		for (idx = 0; idx < 6 - nCount; idx++)
		{
			pComboBox = static_cast<CComboBox*>(GetDlgItem(m_nSortColumeID[6 - idx]));
			if (pComboBox == NULL) continue;
			pComboBox->EnableWindow(FALSE);

			pComboBox = static_cast<CComboBox*>(GetDlgItem(m_nSortCondID[6 - idx]));
			if (pComboBox == NULL) continue;
			pComboBox->EnableWindow(FALSE);
		}
	}
*/
	return 0;
}

INT CMultiSortDlg::AddSortInfo(CString strDMFieldAlias, CString strDesc)
{
	m_arrStrSortCols.Add(strDesc);
	m_mapDMAlias_Desc.SetAt(strDesc, strDMFieldAlias);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMultiSortDlg message handlers

BOOL CMultiSortDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	Init();

	InitDefaultValue();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMultiSortDlg::OnbSORTMULTI() 
{
	INT nSel = -1;
	INT nCond = -1;
	CString strCol;
	CComboBox *pComboBox = NULL;
	INT n = 0;
	for (INT idx = 0; idx < 6; idx++)
	{
		pComboBox = static_cast<CComboBox*>(GetDlgItem(m_nSortColumeID[idx]));
		if (!pComboBox) continue;
		nSel = pComboBox->GetCurSel();
		pComboBox->GetLBText(nSel, strCol);
		
		if (strCol == _T("������")) continue;

		m_mapDMAlias_Desc.Lookup(strCol, strCol);
		if (strCol.IsEmpty()) continue;

		m_arrStrSortAlias[n] = strCol;


		pComboBox = static_cast<CComboBox*>(GetDlgItem(m_nSortCondID[idx]));
		if (!pComboBox) continue;

		nSel = pComboBox->GetCurSel();
		if (nSel == 0)
			nCond = ESL_DATAMGR_SORT_ASC;
		else
			nCond = ESL_DATAMGR_SORT_DESC;		

		m_arrIntSortCond[n] = nCond;

		n++;
	}

	m_nSortCount = n;

	EndDialog(IDOK);
}

VOID CMultiSortDlg::OnbDefaultValue() 
{
	CString sPath = _T("..\\CFG\\SerialSortElement.cfg");
	//�����о����
	FILE *fp;
	
	fp = _tfopen( sPath , _T("wb") );
	if ( fp == NULL ) {
		AfxMessageBox( _T("CFG/SerialSortElement.cfg ������ �����ϴ�.") );
		return;
	}			
	
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
		
	INT nSel = -1;
	INT nCond = -1;
	CString strCol, strOption;
	CComboBox *pComboBox = NULL;
	INT n = 0;
	for (INT idx = 0; idx < 6; idx++)
	{
		pComboBox = static_cast<CComboBox*>(GetDlgItem(m_nSortColumeID[idx]));
		if (!pComboBox) continue;
		nSel = pComboBox->GetCurSel();
		strCol.Format(_T("%d"),nSel);
		
		pComboBox = static_cast<CComboBox*>(GetDlgItem(m_nSortCondID[idx]));
		if (!pComboBox) continue;

		nSel = pComboBox->GetCurSel();
		if (nSel == 0)
			strOption = _T("0");
		else
			strOption = _T("1");

		strCol = strCol + _T("-") + strOption + _T("\r\n");
		_ftprintf( fp, strCol );
	}
	fclose( fp );

	AfxMessageBox(_T("�⺻���� �����Ͽ����ϴ�."));
}
HBRUSH CMultiSortDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
