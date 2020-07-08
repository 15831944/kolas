// CharSetInputModifyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CharSetInputModifyDlg.h"
#include "efs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCharSetInputModifyDlg dialog


CCharSetInputModifyDlg::CCharSetInputModifyDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCharSetInputModifyDlg)
	m_strFromData = _T("");
	m_strToData = _T("");
	//}}AFX_DATA_INIT

	m_nMode = 1;

	m_pDM = NULL;
	m_pGrid = NULL;
}
CCharSetInputModifyDlg::~CCharSetInputModifyDlg()
{
	
}


BOOL CCharSetInputModifyDlg::Create(CWnd* pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CCharSetInputModifyDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCharSetInputModifyDlg)
	DDX_Text(pDX, IDC_eFROM_DATA_CM, m_strFromData);
	DDX_Text(pDX, IDC_eTO_DATA_CM, m_strToData);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CCharSetInputModifyDlg, CDialog)
	//{{AFX_MSG_MAP(CCharSetInputModifyDlg)
	ON_BN_CLICKED(IDC_bPREV_CM, OnbPREVCM)
	ON_BN_CLICKED(IDC_bNEXT_CM, OnbNEXTCM)
	ON_BN_CLICKED(IDC_bSAVE_CM, OnbSAVECM)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_WM_CTLCOLOR()   
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCharSetInputModifyDlg message handlers
INT CCharSetInputModifyDlg::Display()
{
	EFS_BEGIN

	if (m_nMode == MODE_INSERT || m_nMode == MODE_INSERT_EXT)
	{
		m_strFromData = _T("");
		m_strToData = _T("");
	}
	else if (m_nMode == MODE_MODIFY || m_nMode == MODE_MODIFY_EXT)
	{
		INT nIdx = m_pGrid->SelectGetIdx();
		if (nIdx < 0) return -1;

		m_pGrid->SetReverse(nIdx);
		//m_pGrid->SetTopRow(nIdx + 1);

		m_strFromData = m_pDM->GetCellData(_T("FROM_DATA"), nIdx);
		m_strToData = m_pDM->GetCellData(_T("TO_DATA"), nIdx);
	}

	if (!m_strCatagory.IsEmpty())
	{
		GetDlgItem(IDC_STATIC_FROM_DATA)->SetWindowText(m_strCatagory);
	}

	UpdateData(FALSE);

	return 0;

	EFS_END
	return -1;

}

/////////////////////////////////////////////////////////////////////////////
// CCharSetInputModifyDlg message handlers

BOOL CCharSetInputModifyDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();

	CString strWindowText;
	
	if (m_nMode == MODE_INSERT || m_nMode == MODE_INSERT_EXT)
	{
		if (m_strCatagory)
			strWindowText.Format(_T("%s �Է�"), m_strCatagory);
		else
			strWindowText = _T("���� ��ȯ ������ �Է�");

		SetWindowText(strWindowText);

		m_pDM = FindDM(_T("DM_����_���ں�ȯ"));
		if (m_pDM == NULL) return FALSE;

		GetDlgItem(IDC_bPREV_CM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_bNEXT_CM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_bSAVE_CM)->SetWindowText(_T("Ȯ��"));
	}
	else if (m_nMode == MODE_MODIFY || m_nMode == MODE_MODIFY_EXT)
	{
		if (m_strCatagory)
			strWindowText.Format(_T("%s ����"), m_strCatagory);
		else
			strWindowText = _T("���� ��ȯ ������ ����");

		SetWindowText(strWindowText);

		m_pDM = FindDM(_T("DM_����_���ں�ȯ"));
		if (m_pDM == NULL) return FALSE;

		if (!m_pGrid)
			m_pGrid = (CESL_Grid*)FindControl(_T("CM_����_���ں�ȯ"), _T("GRID_DATA"));
		if (m_pGrid == NULL) return FALSE;

		//if (m_pGrid->SelectGetHeadPosition() < 0) goto ERR;
	}

	if (m_nMode == MODE_INSERT_EXT || m_nMode == MODE_MODIFY_EXT)
	{
		GetDlgItem(IDC_STATIC_TO_DATA)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_eTO_DATA_CM)->ShowWindow(SW_HIDE);

		CRect rcWnd;
		CWnd *pWnd = GetDlgItem(IDC_STATIC_FROM_DATA);
		pWnd->GetWindowRect(rcWnd);
		ScreenToClient(rcWnd);
		rcWnd.top += 15;
		rcWnd.bottom += 15;
		pWnd->MoveWindow(rcWnd);

		pWnd = GetDlgItem(IDC_eFROM_DATA_CM);
		pWnd->GetWindowRect(rcWnd);
		ScreenToClient(rcWnd);
		rcWnd.top += 15;
		rcWnd.bottom += 15;
		pWnd->MoveWindow(rcWnd);
	}

	Display();

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
	return FALSE;

}

VOID CCharSetInputModifyDlg::OnbPREVCM() 
{
	EFS_BEGIN

	if (m_pGrid == NULL) return;
	if (m_pGrid->SelectGetPrev() < 0)
		m_pGrid->SelectGetHeadPosition();
	Display();

	EFS_END
}

VOID CCharSetInputModifyDlg::OnbNEXTCM() 
{
	EFS_BEGIN

	if (m_pGrid == NULL) return;
	if (m_pGrid->SelectGetNext() < 0)
		m_pGrid->SelectGetTailPosition();
	Display();

	EFS_END
}

VOID CCharSetInputModifyDlg::OnbSAVECM() 
{
EFS_BEGIN
	INT ids;

	UpdateData();

	//DM ����
	ids = InsertValueToDM();
	if  ( ids < 0  ) return;
	else AfxMessageBox(_T("�����Ͽ����ϴ�"));

	// 040228_HSS ���� : ����ȭ�� ���� �� �ѹ��� Insert �� �ָ� ��
	//���ϰ� DB�� �ݿ�
	//if ( !WriteToFile() ) return;

EFS_END
}

VOID CCharSetInputModifyDlg::OnbCLOSE() 
{
	EFS_BEGIN

	if (m_nMode == MODE_INSERT || m_nMode == MODE_INSERT_EXT)
		EndDialog(IDCANCEL);
	else if (m_nMode == MODE_MODIFY || m_nMode == MODE_MODIFY_EXT)
		EndDialog(IDOK);

	EFS_END
}

BOOL CCharSetInputModifyDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		OnbSAVECM();
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CCharSetInputModifyDlg::WriteToFile()
{
EFS_BEGIN

	CESL_DataMgr DmCharMng;
	DmCharMng.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);	
	CString sQuery = _T("SELECT REC_KEY, VERSION FROM CO_CHARSET_MANAGEMENT_TBL WHERE CHARSET_NAME = '���Ҿ�'");
	DmCharMng.RestructDataManager(sQuery);	

	CString strRecKey = DmCharMng.GetCellData( 0, 0 );
	CString strVersion = DmCharMng.GetCellData( 0, 1 );

	//���� ����
	INT iVersion = _ttoi( strVersion );
	iVersion++;
	strVersion.Format( _T("%d"), iVersion );
	
	CString sPath = _T("..\\CFG\\CO_ROLE_WORD_TBL.txt");
	
	//�����о����
	FILE *fp;
	fp = _tfopen( sPath , _T("wb") );
	if ( fp == NULL ) {
		AfxMessageBox( _T("CO_ROLE_WORD_TBL.txt ������ �����ϴ�.") );
		return FALSE;
	}
	fseek(fp, 0, SEEK_END);		
	if( 0 == ftell(fp) )
	{
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
	}
		
	INT iRowCount = m_pDM->GetRowCount();
	_ftprintf( fp, _T(".FILE_VERSION =  \r\n") );

	for ( INT i = 0 ; i < iRowCount ; i++ ) {
		CString sRoleWord = m_pDM->GetCellData( i, 1 );
		CString sLine = sRoleWord + _T("                          |Y\r\n");
		_ftprintf( fp, sLine );
	}
	fclose( fp );


	//DB�� �ݿ�
	if ( !UpodateInfoToDb(sPath, strRecKey, strVersion) ) return FALSE;

	return TRUE;

EFS_END
return FALSE;
}

INT CCharSetInputModifyDlg::InsertValueToDM()
{
EFS_BEGIN

	INT ids = -1;

	m_strFromData.TrimLeft();
	m_strFromData.TrimRight();

	m_strToData.TrimLeft();
	m_strToData.TrimRight();


	ids = m_pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("DB ���� �ʱ�ȭ Ȯ��"));
		return -1;
	}


	if (m_nMode == MODE_INSERT)
	{
		if (m_strFromData.IsEmpty() || m_strToData.IsEmpty()) return -1;
		
		// �ߺ� �����Ѵ�.
		CString strQuery;
		CString strResult;
		strQuery.Format(_T("SELECT COUNT(*) FROM %s WHERE FROM_DATA = '%s'"), m_pDM->TBL_NAME, m_strFromData);
		ids = m_pDM->GetOneValueQuery(strQuery, strResult);
		if ( ids < 0 )	return -1;

		if (strResult != _T("0"))
		{
			AfxMessageBox(_T("�ߺ��Ǵ� ������ �Դϴ�."));
			return -1;
		}
		EndDialog(IDOK);
	}
	else if (m_nMode == MODE_INSERT_EXT)
	{
		if (m_strFromData.IsEmpty()) return -1;
		
		// �ߺ� �����Ѵ�.
		CString strQuery;
		CString strResult;
		strQuery.Format(_T("SELECT COUNT(*) FROM %s WHERE FROM_DATA = '%s'"), m_pDM->TBL_NAME, m_strFromData);
		ids = m_pDM->GetOneValueQuery(strQuery, strResult);
		if ( ids < 0 )	return -1;
		
		if (strResult != _T("0"))
		{
			AfxMessageBox(_T("�ߺ��Ǵ� ������ �Դϴ�."));
			return -1;
		}
		
		EndDialog(IDOK);
	}
	else if (m_nMode == MODE_MODIFY)
	{
		// UpdateDM�� �����.
		INT nIdx = m_pGrid->SelectGetIdx();
		if (nIdx < 0) return -1;
		
		CString strRecKey;
		strRecKey = m_pDM->GetCellData(_T("REC_KEY"), nIdx);
		if (strRecKey.IsEmpty()) return -1;
		
		m_pDM->InitDBFieldData();
		ids = m_pDM->AddDBFieldData(_T("FROM_DATA"), _T("STRING"), m_strFromData);
		if ( ids < 0 )	return -1;

		// 040228_HSS ����
		ids = m_pDM->AddDBFieldData(_T("TO_DATA"), _T("STRING"), m_strToData);
		if ( ids < 0 )	return -1;

		ids = m_pDM->MakeUpdateFrame(m_pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strRecKey, nIdx);
		if ( ids < 0 )	return -1;
		
		// Grid�� ������Ʈ �����ش�.
		ids = m_pGrid->SetAt(nIdx, 1, m_strFromData);
		if ( ids < 0 )	return -1;

		// 040228_HSS : DM�� Update
		m_pDM->SetCellData( _T("FROM_DATA"), m_strFromData, nIdx );

		// 040228_HSS ���� : Error Code
		ids = m_pGrid->SetAt(nIdx, 2, m_strToData);
		if ( ids < 0 )	return -1;
	}
	else if (m_nMode == MODE_MODIFY_EXT)
	{
		// UpdateDM�� �����.
		INT nIdx = m_pGrid->SelectGetIdx();
		if (nIdx < 0) return -1;
		
		CString strRecKey;
		strRecKey = m_pDM->GetCellData(_T("REC_KEY"), nIdx);
		if (strRecKey.IsEmpty()) return -1;
		
		m_pDM->InitDBFieldData();
		ids =m_pDM->AddDBFieldData(_T("FROM_DATA"), _T("STRING"), m_strFromData);
		if ( ids < 0 )	return -1;

		ids = m_pDM->MakeUpdateFrame(m_pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strRecKey, nIdx);
		if ( ids < 0 )	return -1;
		
		
		// Grid�� ������Ʈ �����ش�.
		ids = m_pGrid->SetAt(nIdx, 1, m_strFromData);
		if ( ids < 0 )	return -1;

		// 040228_HSS : DM�� Update
		m_pDM->SetCellData( _T("FROM_DATA"), m_strFromData, nIdx );

		// 040228_HSS ���� : Error Code
		//ids = m_pGrid->SetAt(nIdx, 2, m_strToData);
		//if ( ids < 0 )	return -1;

	}
	
	
	ids = m_pDM->SendFrame();
	if ( ids < 0 )
	{
		AfxMessageBox(_T("DB ���� ���� Ȯ��"));
		return -1;
	}

	ids = m_pDM->EndFrame();
	if ( ids < 0 )
	{
		AfxMessageBox(_T("DB ���� ���� Ȯ��"));
		return -1;
	}

	return 0;

EFS_END
return -1;
}

BOOL CCharSetInputModifyDlg::UpodateInfoToDb(CString sPath, CString strRecKey, CString strVersion)
{
EFS_BEGIN

	// ���� ���̺� ����, ���ϱ���, ���������� ������Ʈ �Ѵ�.
	// ������ ��ä�� BLOB���� DB�� �ø���.
	CStdioFile file;
	if (!file.Open(sPath, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("������ ���µ� �����߽��ϴ�."));
		return FALSE;
	}
			TCHAR cUni;
			file.Read( &cUni, sizeof(TCHAR) );
			if( 0xFEFF != cUni )
			{
				file.SeekToBegin();
			}

	INT nFileSize = file.GetLength();
	BYTE *pBuffer = (BYTE*)malloc(sizeof(BYTE) * (nFileSize) + 1024);
	ZeroMemory(pBuffer, nFileSize);
	nFileSize = file.ReadHuge(pBuffer, nFileSize);
	file.Close();

	CString sTmp = m_pDM->TBL_NAME;
	// ����
	INT ids = m_pDM->LOBUpdate( _T("CO_CHARSET_MANAGEMENT_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey, _T("FILE_DATA"), pBuffer, nFileSize);
	if (ids < 0)
	{
		// �޸� ����
		free(pBuffer);
		AfxMessageBox(_T("�������� ����"));
		return FALSE;
	}

	// ���ϱ���, ��������
	CString strLen;
	strLen.Format(_T("%d"), nFileSize);

	ids = m_pDM->StartFrame();	
	if ( ids < 0) 
	{
		AfxMessageBox(_T("DB �ʱ�ȭ Ȯ��"));
		return FALSE;
	}

	m_pDM->InitDBFieldData();
	ids = m_pDM->AddDBFieldData(_T("VERSION"), _T("NUMERIC"), strVersion);
	if (ids < 0 ) return FALSE;
	ids = m_pDM->AddDBFieldData(_T("FILE_LEN"), _T("NUMERIC"), strLen);
	if (ids < 0 ) return FALSE;
	ids = m_pDM->MakeUpdateFrame( _T("CO_CHARSET_MANAGEMENT_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey );
	if (ids < 0 ) return FALSE;

	ids = m_pDM->SendFrame();
	if (ids < 0) 
	{
		// �޸� ����
		free(pBuffer);
		AfxMessageBox(_T("�������� �����߽��ϴ�."));
		return FALSE;
	}

	ids = m_pDM->EndFrame();
	if (ids < 0) 
	{
		// �޸� ����
		free(pBuffer);
		AfxMessageBox(_T("�������� �����߽��ϴ�."));
		return FALSE;
	}
	
	return TRUE;

EFS_END
return FALSE;
}


HBRUSH CCharSetInputModifyDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}