// BO_CAT_WORD_0100.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_WORD_0100.h"
#include "BO_CAT_WORD_0200.h"
#include "BO_CAT_WORD_0300.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_WORD_0100 dialog


CBO_CAT_WORD_0100::CBO_CAT_WORD_0100(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_WORD_0100)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_WORD_0100::~CBO_CAT_WORD_0100()
{
}

VOID CBO_CAT_WORD_0100::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_WORD_0100)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_WORD_0100, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_WORD_0100)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bINPUT, OnbINPUT)
	ON_BN_CLICKED(IDC_bUPDATE, OnbUPDATE)
	ON_BN_CLICKED(IDC_bDELETE_BCPK, OnbDELETE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_WORD_0100 message handlers

BOOL CBO_CAT_WORD_0100::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_WORD_0100::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_WORD_0100")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_WORD_0100::OnbSEARCH() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CString sWord;
	GetControlData( _T("CM_BO_CAT_WORD_0100"), _T("�ܾ�"), sWord );
	if ( sWord.IsEmpty() ){
		AfxMessageBox( _T("�ܾ �Է��ϼ���.") );
		return;
	}

	CESL_DataMgr *pDM;
	pDM = FindDM( _T("DM_BO_CAT_MNG_WORD") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_MNG_WORD error") );
		return;
	}

	CString strWhere;
	strWhere = _T("AUX_WORD LIKE '") + sWord + _T("%'");
	if (pDM->RefreshDataManager(strWhere) < 0)	return;
	if ( pDM->GetRowCount() < 1 ) {
		AfxMessageBox (_T("�˻��� ����� �����ϴ�."));
		return;
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_WORD_0100"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_WORD_0100 error") );
		return;
	}
	pCM->AllControlDisplay();

EFS_END
}

VOID CBO_CAT_WORD_0100::OnbINPUT() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CBO_CAT_WORD_0200 dlg(this);
	//dlg.DoModal();
	if ( dlg.DoModal() == IDOK ) {
		m_bModifyFlag = true;
	}

EFS_END
}

VOID CBO_CAT_WORD_0100::OnbUPDATE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CBO_CAT_WORD_0300 dlg(this);
	//dlg.DoModal();
	if ( dlg.DoModal() == IDOK ) {
		m_bModifyFlag = true;
	}

EFS_END
}

VOID CBO_CAT_WORD_0100::OnbDELETE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CESL_ControlMgr *pCM;
	INT ids;
	pCM = FindCM( _T("CM_BO_CAT_WORD_0100") );
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_WORD_0100 ERROR") );
		return ;
	}

	SelectMakeList(_T("CM_BO_CAT_WORD_0100"), _T("GRD_WORD"));
	INT idx = SelectGetTailPosition(_T("CM_BO_CAT_WORD_0100"), _T("GRD_WORD"));
	if (idx < 0)
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));	// ����
		return ;
	}

	if (MessageBox(_T("������ �ڷḦ �����Ͻðڽ��ϱ�?"), _T("���� Ȯ��"), MB_YESNO) == IDYES)
	{
		CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_MNG_WORD"));
		ids = pDM->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame �� Ȯ���ϼ���"));
			return;
		}


		CString sAccRecKey, sAccRecKeyTmp;
		do
		{
			CString strCondition;
			pDM->GetCellData(_T("REC_KEY"), idx, strCondition);

			sAccRecKeyTmp += _T(",") + strCondition;		//Ű ����

			pDM->MakeDeleteFrame(_T("CO_WORD_TBL"), _T("REC_KEY"), _T("STRING"), strCondition);
			pDM->DeleteRow(idx);

			idx = SelectGetPrev(_T("CM_BO_CAT_WORD_0100"), _T("GRD_WORD"));			
		}		
		while (idx > 0);
/*
		sAccRecKey = sAccRecKeyTmp.Mid(1);
		CString sQuery;
		sQuery.Format( _T("DELETE FROM CO_WORD_TBL WHERE REC_KEY IN (%s);") , sAccRecKey );
		INT iCh = pDM->AddFrame(sQuery);
*/
		ids = pDM->SendFrame();
		if (ids < 0) {
			AfxMessageBox(_T("���������� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
			return ;
		}
		ids = pDM->EndFrame();
		if (ids < 0) {
			AfxMessageBox(_T("���������� DataBase�� �����ϴ��� ������ �߻��Ͽ����ϴ�."));
			return ;
		}
		//���� flag
		m_bModifyFlag = true;
	}
	AllControlDisplay(_T("CM_BO_CAT_WORD_0100"));

	return ;

EFS_END
}

VOID CBO_CAT_WORD_0100::OnCancel() 
{
EFS_BEGIN

	// TODO: Add extra cleanup here
	if ( m_bModifyFlag > 0 ) {
		CESL_DataMgr *pDM;
		pDM = FindDM( _T("DM_BO_CAT_PROC_0390_WORD") );
		if ( pDM == NULL ) {
			AfxMessageBox( _T("DM_BO_CAT_PROC_0390_WORD error") );
			return;
		}

		CESL_DataMgr *m_pDM_Manage;
		m_pDM_Manage = FindDM( _T("DM_BO_CAT_PROC_0390_WORD") );
		if ( m_pDM_Manage == NULL ) {
			AfxMessageBox( _T("DM_BO_CAT_PROC_0390_WORD error") );
			return;
		}
	
		CESL_DataMgr DmCharMng;
		DmCharMng.SetCONNECTION_INFO(pDM->CONNECTION_INFO);	
		CString sQuery = _T("SELECT REC_KEY, VERSION FROM CO_CHARSET_MANAGEMENT_TBL WHERE CHARSET_NAME = '�ܾ�'");
		DmCharMng.RestructDataManager(sQuery);	
		CString strRecKey = DmCharMng.GetCellData( 0, 0 );
		CString strVersion = DmCharMng.GetCellData( 0, 1 );
		INT iVersion = _ttoi( strVersion );
		iVersion++;
		strVersion.Format( _T("%d"), iVersion );
		
		CString strWhere = _T("");
		if (pDM->RefreshDataManager(strWhere) < 0)	return;
	
		CString sPath = _T("..\\CFG\\CO_WORD_TBL.txt");
		//�����о����
		FILE *fp;
		//TCHAR cLine[256];
		fp = _tfopen( sPath , _T("wb") );
		if ( fp == NULL ) {
			AfxMessageBox( _T("CO_WORD_TBL.txt ������ �����ϴ�.") );
			return;
		}			
		
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
		
		/*
		while( _fgetts( cLine, 256, fp) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
		}
		*/
		INT iRowCount = pDM->GetRowCount();
		_ftprintf( fp, _T(".FILE_VERSION =  \r\n") );
		for ( INT i = 0 ; i < iRowCount ; i++ ) {
			CString sRoleWord = pDM->GetCellData( _T("�ܾ�"), i );
			CString sLine = sRoleWord + _T("                          |Y\r\n");
			_ftprintf( fp, sLine );
		}
		fclose( fp );


		// ���� ���̺� ����, ���ϱ���, ���������� ������Ʈ �Ѵ�.
		// ������ ��ä�� BLOB���� DB�� �ø���.
		CStdioFile file;
		INT nFileSize = 0;
		//if (!file.Open(strTempFileName, CFile::modeRead))
		if (!file.Open(sPath, CFile::modeRead | CFile::typeBinary))
		{
			MessageBox(_T("������ ���µ� �����߽��ϴ�."));
			return;
		}
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
		
		nFileSize = file.GetLength();
		BYTE *pBuffer = (BYTE*)malloc(sizeof(BYTE) * (nFileSize) + 1024);
		ZeroMemory(pBuffer, nFileSize);
		nFileSize = file.ReadHuge(pBuffer, nFileSize);
		file.Close();

		CString sTmp = m_pDM_Manage->TBL_NAME;
		// ����
		//INT ids = m_pDM_Manage->LOBUpdate(m_pDM_Manage->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strRecKey, _T("FILE_DATA"), pBuffer, nFileSize);
		INT ids = m_pDM_Manage->LOBUpdate( _T("CO_CHARSET_MANAGEMENT_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey, _T("FILE_DATA"), pBuffer, nFileSize);
		if (ids < 0)
		{
			// �޸� ����
			free(pBuffer);
			MessageBox(_T("�������� ����"));
			return;
		}
		
		// ���ϱ���, ��������
		CString strLen;
		strLen.Format(_T("%d"), nFileSize);

		ids = m_pDM_Manage->StartFrame();	
		
		m_pDM_Manage->InitDBFieldData();
		m_pDM_Manage->AddDBFieldData(_T("VERSION"), _T("NUMERIC"), strVersion);
		m_pDM_Manage->AddDBFieldData(_T("FILE_LEN"), _T("NUMERIC"), strLen);
		//m_pDM_Manage->AddDBFieldData(_T("FILE_NAME"), _T("STRING"), m_pDM_Manage->GetCellData(_T("TABLE_NAME"), nManagementDMRow) + _T(".TXT"));
		//m_pDM_Manage->MakeUpdateFrame(m_pDM_Manage->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strRecKey, nManagementDMRow);
		m_pDM_Manage->MakeUpdateFrame( _T("CO_CHARSET_MANAGEMENT_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey );
		//INSERT INTO CO_CHARSET_MANAGEMENT_TBL ('50364199', '����', '1', '����', '����', 'CO_AUXILIARY_WORD_TBL', 'CO_AUXILIARY_WORD_TBL.TXT', 
		
		ids = m_pDM_Manage->SendFrame();
		if (ids < 0) 
		{
			// �޸� ����
			free(pBuffer);
			MessageBox(_T("�������� �����߽��ϴ�."));
			return;
		}
		ids = m_pDM_Manage->EndFrame();
		if (ids < 0) 
		{
			// �޸� ����
			free(pBuffer);
			MessageBox(_T("�������� �����߽��ϴ�."));
			return;
		}
	}

	CDialog::OnCancel();

EFS_END
}

BOOL CBO_CAT_WORD_0100::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)	
	{
		OnbSEARCH();
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CBO_CAT_WORD_0100::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}