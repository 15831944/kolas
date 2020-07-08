#include "stdafx.h"
#include "BO_LOC_3190.h"

#include "CfgFileApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBO_LOC_3190::CBO_LOC_3190(CESL_Mgr* pParent /*=NULL*/, CLoanShareManager* pLoanShareManager , CLonaManageValue* pLoanManageValue )
	: CESL_Mgr(IDD, pParent)
{	
	m_sColorDMAlias = _T("DM_BO_LOC_3100_COLOR_SETTING");
	m_pLoanShareManager = m_pLoanShareManager;
	m_pLoanManageValue = pLoanManageValue;	
	
	m_bManageCodeColorUseYN = FALSE;
	m_bReferenceColorUseYN = FALSE;

	m_pLoan = NULL;
	
	m_pDM = NULL;
	m_crColor = 0;
	m_crColor1 = 0;
	m_crColor2 = 0;
}


CBO_LOC_3190::~CBO_LOC_3190()
{

}

VOID CBO_LOC_3190::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3190)
	DDX_Control(pDX, IDbtnCOLORSTATUS, m_bctrlCOLOR);
	DDX_Control(pDX, IDbtnCOLORSTATUS1, m_bctrlCOLOR1);
	DDX_Control(pDX, IDbtnCOLORSTATUS2, m_bctrlCOLOR2);
	DDX_ColorPicker(pDX, IDbtnCOLORSTATUS, m_crColor);
	DDX_ColorPicker(pDX, IDbtnCOLORSTATUS1, m_crColor1);
	DDX_ColorPicker(pDX, IDbtnCOLORSTATUS2, m_crColor2);
	//}}AFX_DATA_MAP
	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3190, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3190)
	ON_BN_CLICKED(IDbtnCOLORSTATUS, OnbtnCOLORSTATUS)
	ON_CBN_EDITCHANGE(IDC_cmbCOLORSELECT, OnEditchangecmbCOLORSELECT)
	ON_CBN_SELCHANGE(IDC_cmbCOLORSELECT, OnSelchangecmbCOLORSELECT)
	ON_BN_CLICKED(IDbtnCANCEL, OnbtnCANCEL)
	ON_BN_CLICKED(IDbtnCOLORSAVE, OnbtnCOLORSAVE)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_chkUNUSE1, OnchkUNUSE1)
	ON_BN_CLICKED(IDC_chkUNUSE2, OnchkUNUSE2)
	ON_CBN_SELCHANGE(IDC_cmbCOLORSELECT2, OnSelchangecmbCOLORSELECT2)
	ON_CBN_SELCHANGE(IDC_cmbCOLORSELECT1, OnSelchangecmbCOLORSELECT1)
	//}}AFX_MSG_MAP
	ON_MESSAGE(CPN_SELENDOK, OnSelChange)
END_MESSAGE_MAP()


BOOL CBO_LOC_3190::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	m_pDM = FindDM( _T("DM_BO_LOC_3100_COLOR_SETTING_TMP") );
	
	INT ids;
	CString strTmp;
	CCfgFileApi api;
	api.SetFile(_T("..\\cfg\\LoanReturn\\ȭ�������_��뿩��.txt"));
	ids = api.GetData(_T("��������"), strTmp);
	if(0 > ids)
	{
		AfxMessageBox(_T("���������� �ҷ����µ� �����Ͽ����ϴ�."));
	}
	if(_T("Y") != strTmp)
	{
		((CButton*)GetDlgItem(IDC_chkUNUSE1))->SetCheck(1);
		GetDlgItem(IDC_cmbCOLORSELECT1)->EnableWindow(FALSE);
		GetDlgItem(IDbtnCOLORSTATUS1)->EnableWindow(FALSE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_chkUNUSE1))->SetCheck(0);
		GetDlgItem(IDC_cmbCOLORSELECT1)->EnableWindow(TRUE);
		GetDlgItem(IDbtnCOLORSTATUS1)->EnableWindow(TRUE);
		m_bManageCodeColorUseYN = TRUE;		
	}
	ids = api.GetData(_T("�ڷ��"), strTmp);
	if(0 > ids)
	{
		AfxMessageBox(_T("���������� �ҷ����µ� �����Ͽ����ϴ�."));
	}
	if(_T("Y") != strTmp)
	{
		((CButton*)GetDlgItem(IDC_chkUNUSE2))->SetCheck(1);
		GetDlgItem(IDC_cmbCOLORSELECT2)->EnableWindow(FALSE);
		GetDlgItem(IDbtnCOLORSTATUS2)->EnableWindow(FALSE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_chkUNUSE2))->SetCheck(0);
		GetDlgItem(IDC_cmbCOLORSELECT2)->EnableWindow(TRUE);
		GetDlgItem(IDbtnCOLORSTATUS2)->EnableWindow(TRUE);
		m_bReferenceColorUseYN = TRUE;
	}
	GetColorFromFile(_T("�ڷ�Ǳ���"));
	GetColorFromFile(_T("��������"));

	CopyDM();

	InitComboBox();

	return TRUE;  
	              

	EFS_END
	return FALSE;

}

VOID CBO_LOC_3190::InitComboBox()
{
	EFS_BEGIN

	CComboBox *pComboBox;
	pComboBox = (CComboBox*)GetDlgItem(IDC_cmbCOLORSELECT);

	INT nColumnCnt = m_pDM->RefList.GetCount();

	CString *sColumnName;
	sColumnName = new CString[nColumnCnt];

	m_pDM->GetAllAlias( sColumnName , nColumnCnt );

	for( INT i = 0 ; i < nColumnCnt ; i++ )
	{
		pComboBox->AddString(sColumnName[i]);
	}
	pComboBox->SetCurSel(0);
	CString sSelectColumnName;
	pComboBox->GetLBText( 0 , sSelectColumnName );
	SelectColor(sSelectColumnName);

	delete []sColumnName;	
	
	pComboBox = (CComboBox*)GetDlgItem(IDC_cmbCOLORSELECT1);
	pComboBox->SetCurSel(0);
	pComboBox->GetLBText( 0 , sSelectColumnName );
	CString sColor;
	m_ManageCodeColorDM.GetCellData( sSelectColumnName , 0 , sColor  );
	if(_T("") != sColor)
	{
		m_crColor1 = _ttoi( sColor );
		((CButton*)GetDlgItem(IDbtnCOLORSTATUS1))->SetWindowText(_T("������"));
	}
	else
	{
		m_crColor1 = 16777214;
		((CButton*)GetDlgItem(IDbtnCOLORSTATUS1))->SetWindowText(_T("��������"));
	}
	pComboBox = (CComboBox*)GetDlgItem(IDC_cmbCOLORSELECT2);
	pComboBox->SetCurSel(0);
	pComboBox->GetLBText( 0 , sSelectColumnName );
	m_ReferenceColorDM.GetCellData( sSelectColumnName , 0 , sColor  );
	if(_T("") != sColor)
	{
		m_crColor2 = _ttoi( sColor );
		((CButton*)GetDlgItem(IDbtnCOLORSTATUS2))->SetWindowText(_T("������"));
	}
	else
	{
		m_crColor2 = 16777214;
		((CButton*)GetDlgItem(IDbtnCOLORSTATUS2))->SetWindowText(_T("���þ���"));
	}
	UpdateData(FALSE);

	EFS_END
}

VOID CBO_LOC_3190::SelectColor( CString sColumnName )
{
	EFS_BEGIN

	CString sColor;
	m_pDM->GetCellData( sColumnName , 0 , sColor  );

	m_crColor = _ttoi( sColor );

	UpdateData(FALSE);

	EFS_END
}

VOID CBO_LOC_3190::OnbtnCOLORSTATUS() 
{
	EFS_BEGIN	

	EFS_END
}

VOID CBO_LOC_3190::OnEditchangecmbCOLORSELECT() 
{
	EFS_BEGIN
		
	EFS_END
}

VOID CBO_LOC_3190::OnSelchangecmbCOLORSELECT() 
{
	EFS_BEGIN

	CComboBox *pComboBox;
	pComboBox = (CComboBox*)GetDlgItem(IDC_cmbCOLORSELECT);
	
	CString sColumnName;
	pComboBox->GetLBText( pComboBox->GetCurSel() , sColumnName );

	SelectColor( sColumnName );	

	EFS_END
}	

LRESULT CBO_LOC_3190::OnSelChange(WPARAM wParam, LPARAM lParam) 
{
	UpdateData(TRUE);

	INT nID = (INT)lParam;

	CComboBox *pComboBox;
	CString sColumnName;
	CString sData;
	
	
	if(IDbtnCOLORSTATUS == nID)
	{
		pComboBox = (CComboBox*)GetDlgItem(IDC_cmbCOLORSELECT);	
		pComboBox->GetLBText( pComboBox->GetCurSel() , sColumnName );
				
		if ( m_crColor == CLR_DEFAULT )
			SetAutoColor( sColumnName );
		
		sData.Format( _T("%u") , m_crColor );

		m_pDM->SetCellData( sColumnName , sData , 0 );
	}
	else if(IDbtnCOLORSTATUS1 == nID)
	{
		pComboBox = (CComboBox*)GetDlgItem(IDC_cmbCOLORSELECT1);
		pComboBox->GetLBText( pComboBox->GetCurSel() , sColumnName );

		sData.Format( _T("%u") , m_crColor1 );
		m_ManageCodeColorDM.SetCellData( sColumnName , sData , 0 );
		((CButton*)GetDlgItem(IDbtnCOLORSTATUS1))->SetWindowText(_T("������"));
	}
	else if(IDbtnCOLORSTATUS2 == nID)
	{
		pComboBox = (CComboBox*)GetDlgItem(IDC_cmbCOLORSELECT2);
		pComboBox->GetLBText( pComboBox->GetCurSel() , sColumnName );

		sData.Format( _T("%u") , m_crColor2 );
		m_ReferenceColorDM.SetCellData( sColumnName , sData , 0 );
		((CButton*)GetDlgItem(IDbtnCOLORSTATUS2))->SetWindowText(_T("������"));
	}
	return 0;
}

INT CBO_LOC_3190::SetAutoColor(CString sColumnName)
{

	CString sReverseColumn = _T("");
	CString sData = _T("");
	INT nBaseColor = 0xFFFFFF;									

	if ( sColumnName.Compare( _T("����_���ڻ�") ) == 0 )
		sReverseColumn = _T("����_������");
	else if ( sColumnName.Compare( _T("����_������") ) == 0 )
		sReverseColumn = _T("����_���ڻ�");
	else if ( sColumnName.Compare( _T("�ݳ�_���ڻ�") ) == 0 )
		sReverseColumn = _T("�ݳ�_������");
	else if ( sColumnName.Compare( _T("�ݳ�_������") ) == 0 )
		sReverseColumn = _T("�ݳ�_���ڻ�");
	else if ( sColumnName.Compare( _T("�ݳ�����_���ڻ�") ) == 0 )
		sReverseColumn = _T("�ݳ�����_������");
	else if ( sColumnName.Compare( _T("�ݳ�����_������") ) == 0 )
		sReverseColumn = _T("�ݳ�����_���ڻ�");
	else if ( sColumnName.Compare( _T("��ü_���ڻ�") ) == 0 )
		sReverseColumn = _T("��ü_������");
	else if ( sColumnName.Compare( _T("��ü_������") ) == 0 )
		sReverseColumn = _T("��ü_���ڻ�");
	else if ( sColumnName.Compare( _T("����_���ڻ�") ) == 0 )
		sReverseColumn = _T("����_������");
	else if ( sColumnName.Compare( _T("����_������") ) == 0 )
		sReverseColumn = _T("����_���ڻ�");
	else if ( sColumnName.Compare( _T("�������_���ڻ�") ) == 0 )
		sReverseColumn = _T("�������_������");
	else if ( sColumnName.Compare( _T("�������_������") ) == 0 )
		sReverseColumn = _T("�������_���ڻ�");
	else if ( sColumnName.Compare( _T("ȭ��_������") ) == 0 )	
	{
		m_crColor = nBaseColor;
		return 0;
	}

	m_pDM->GetCellData( sReverseColumn , 0 , sData );
	m_crColor = nBaseColor - _ttoi(sData);

	return 0;
}

VOID CBO_LOC_3190::OnbtnCANCEL() 
{
	EFS_BEGIN

	OnCancel();

	EFS_END
}

INT CBO_LOC_3190::CopyDM()
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = FindDM( m_sColorDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitComboBox()") );

	ids = m_pDM->CopyDM( pDM );

	INT nColumnCnt = pDM->RefList.GetCount();

	CString *sColumnName;
	sColumnName = new CString[nColumnCnt];

	pDM->GetAllAlias( sColumnName , nColumnCnt );

	if( m_pDM->GetRowCount() < 1 ) m_pDM->InsertRow(-1);

	CString sGetData;
	for( INT i = 0 ; i < nColumnCnt ; i++ )
	{
		ids = pDM->GetCellData( sColumnName[i] , 0 , sGetData );

		ids = m_pDM->SetCellData( sColumnName[i] , sGetData , 0 );
	}

	delete []sColumnName;

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3190::FileLoad()
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = FindDM( m_sColorDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitComboBox()") );

	if( pDM->GetRowCount() < 1 ) pDM->InsertRow(-1);

	CStdioFile file;
	TCHAR msg[256];

	if ( !file.Open( COLOR_SETTING_FILE_NAME , CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary) ) {
        _stprintf( msg, _T("���ϸ��� �߸��Ǿ����ϴ�.\n�ùٸ� ������� Ȯ���� �ֽʽÿ�.") );
		AfxMessageBox( msg );
		return -1;
    }

	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	CString sLine;
	CString sColumnName;
	CString sColor;
    while ( file.ReadString( sLine ) ) 
	{
        sLine.TrimRight();
		sLine.TrimLeft();
		if( sLine.IsEmpty() ) continue;

		sColumnName = sLine.Mid( 0 , sLine.Find(_T("["),0) );
		sColor = sLine.Mid( sLine.Find(_T("["),0) + 1 );
		
		ids = pDM->SetCellData( sColumnName , sColor , 0 );
		if ( ids < 0 ) 
		{
			file.Close();
			return -2;
		}
	}

	file.Close();

	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_3190::OnbtnCOLORSAVE() 
{
	EFS_BEGIN

	SaveDM();
	SaveFile();

	CCfgFileApi api;
	api.SetFile(_T("..\\cfg\\LoanReturn\\ȭ�������_��뿩��.txt"));
	if(0 == ((CButton*)GetDlgItem(IDC_chkUNUSE1))->GetCheck())
	{
		m_bManageCodeColorUseYN = TRUE;
	}
	else
	{
		m_bManageCodeColorUseYN = FALSE;
	}
	if(0 == ((CButton*)GetDlgItem(IDC_chkUNUSE2))->GetCheck())
	{
		m_bReferenceColorUseYN = TRUE;
	}
	else
	{
		m_bReferenceColorUseYN = FALSE;
	}
	if(m_bManageCodeColorUseYN)
	{
		api.SetData(_T("��������"), _T("Y"));
		SaveManageCodeColor();
	}
	else
	{
		api.SetData(_T("��������"), _T("N"));
	}
	if(m_bReferenceColorUseYN)
	{
		api.SetData(_T("�ڷ��"), _T("Y"));
		SaveReferenceCodeColor();
	}
	else
	{
		api.SetData(_T("�ڷ��"), _T("N"));
	}

	m_pLoan->m_bManageCodeColorUseYN = m_bManageCodeColorUseYN;
	m_pLoan->m_bReferenceColorUseYN = m_bReferenceColorUseYN;
	if(m_bManageCodeColorUseYN)
	{
		m_pLoan->SetColorDM(_T("��������"));
	}
	else
	{
		m_pLoan->FreeColorDM(_T("��������"));
	}
	if(m_bReferenceColorUseYN)
	{
		m_pLoan->SetColorDM(_T("�ڷ�Ǳ���"));
	}
	else
	{
		m_pLoan->FreeColorDM(_T("�ڷ�Ǳ���"));
	}
	INT ids = m_pLoan->ViewGrid();	

 	AfxMessageBox(_T("�������� �����Ͽ����ϴ�."));

	EFS_END
}

INT CBO_LOC_3190::SaveDM()
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = FindDM( m_sColorDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitComboBox()") );

	INT nColumnCnt = pDM->RefList.GetCount();

	CString *sColumnName;
	sColumnName = new CString[nColumnCnt];

	pDM->GetAllAlias( sColumnName , nColumnCnt );


	CString sGetData;
	for( INT i = 0 ; i < nColumnCnt ; i++ )
	{
		ids = m_pDM->GetCellData( sColumnName[i] , 0 , sGetData );

		ids = pDM->SetCellData( sColumnName[i] , sGetData , 0 );
	}

	delete []sColumnName;

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3190::SaveFile()
{
	EFS_BEGIN

	INT ids;

	CStdioFile file;
	TCHAR msg[256];
	if ( !file.Open( COLOR_SETTING_FILE_NAME , CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) ) {
        _stprintf( msg, _T("���ϸ��� �߸��Ǿ����ϴ�.\n�ùٸ� ������� Ȯ���� �ֽʽÿ�.") );
		AfxMessageBox( msg );
		return -1;
    }

	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}	
	
	CString sRegNo;

	CESL_DataMgr *pDM = FindDM( m_sColorDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitComboBox()") );

	INT nColumnCnt = pDM->RefList.GetCount();

	CString *sColumnName;
	sColumnName = new CString[nColumnCnt];

	pDM->GetAllAlias( sColumnName , nColumnCnt );

	CString sGetData;
	for( INT i = 0 ; i < nColumnCnt ; i++ ) 
	{
		ids = pDM->GetCellData( sColumnName[i] , 0 , sGetData );
		if (ids) 
		{
			delete [] sColumnName;
			return -2;
		}

		file.WriteString( sColumnName[i] );
		file.WriteString( _T("[") );
		file.WriteString( sGetData );
		file.WriteString( _T("\r\n") );
	}

	file.Close();

	delete [] sColumnName;

	return 0;

	EFS_END
	return -1;

}
HBRUSH CBO_LOC_3190::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{

	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}


void CBO_LOC_3190::OnchkUNUSE1() 
{
	INT ids;
	ids = ((CButton*)GetDlgItem(IDC_chkUNUSE1))->GetCheck();			
	if(0 == ids)
	{
		((CComboBox*)GetDlgItem(IDC_cmbCOLORSELECT1))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDbtnCOLORSTATUS1))->EnableWindow(TRUE);
	}
	else
	{
		((CComboBox*)GetDlgItem(IDC_cmbCOLORSELECT1))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDbtnCOLORSTATUS1))->EnableWindow(FALSE);
	}
}

void CBO_LOC_3190::OnchkUNUSE2() 
{
	INT ids;
	ids = ((CButton*)GetDlgItem(IDC_chkUNUSE2))->GetCheck();			
	if(0 == ids)
	{
		((CComboBox*)GetDlgItem(IDC_cmbCOLORSELECT2))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDbtnCOLORSTATUS2))->EnableWindow(TRUE);
	}
	else
	{
		((CComboBox*)GetDlgItem(IDC_cmbCOLORSELECT2))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDbtnCOLORSTATUS2))->EnableWindow(FALSE);
	}
}


INT CBO_LOC_3190::SaveManageCodeColor() 
{
	CCfgFileApi api;
	INT nDefaultValue = 0;
	INT nCount = m_pInfo->pCodeMgr->GetCodeCount( _T("��������") );	
	if(0 > nCount)
	{
		return -100;
	}
	CString* strCode = new CString[nCount];
	CString* strDesc = new CString[nCount];
	nCount++;
	INT ids = m_pInfo->pCodeMgr->GetCode( _T("��������"), strCode, strDesc, nCount, nDefaultValue);
	api.SetFile(_T("..\\cfg\\LoanReturn\\ȭ�������_��������.txt"));
	for(INT i=0; i<nCount; i++)
	{
		CString strTmp, strColor;
		strTmp.Format(_T("%s_���ڻ�"), strDesc[i]);
		strColor = m_ManageCodeColorDM.GetCellData(strTmp, 0); 
		api.SetData(strTmp, strColor);
		strTmp.Format(_T("%s_������"), strDesc[i]);
		strColor = m_ManageCodeColorDM.GetCellData(strTmp, 0); 
		api.SetData(strTmp, strColor);
	}	
	
	return 0;
}


INT CBO_LOC_3190::SaveReferenceCodeColor() 
{
	CCfgFileApi api;
	INT nDefaultValue = 0;
	INT nCount = m_pInfo->pCodeMgr->GetCodeCount( _T("�ڷ�Ǳ���") );	
	if(0 > nCount)
	{
		return -100;
	}
	CString* strCode = new CString[nCount];
	CString* strDesc = new CString[nCount];
	nCount++;
	INT ids = m_pInfo->pCodeMgr->GetCode( _T("�ڷ�Ǳ���"), strCode, strDesc, nCount, nDefaultValue);
	api.SetFile(_T("..\\cfg\\LoanReturn\\ȭ�������_�ڷ��.txt"));
	CString strTmp, strColor;
	for(INT i=0; i<nCount; i++)
	{
		strTmp.Format(_T("%s_���ڻ�"), strDesc[i]);
		strColor = m_ReferenceColorDM.GetCellData(strTmp, 0); 
		api.SetData(strTmp, strColor);
		strTmp.Format(_T("%s_������"), strDesc[i]);
		strColor = m_ReferenceColorDM.GetCellData(strTmp, 0); 
		api.SetData(strTmp, strColor);
	}	
	
	return 0;
}


INT CBO_LOC_3190::GetColorFromFile(CString strAlias) 
{
	CCfgFileApi api;
	INT nDefaultValue = 0;
	INT nCount = m_pInfo->pCodeMgr->GetCodeCount(strAlias);
	INT nFiledCnt = nCount * 2;
	nCount++;
	if(_T("�ڷ�Ǳ���") == strAlias)
	{
		CString* strCode = new CString[nCount];
		CString* strDesc = new CString[nCount];
		INT ids = m_pInfo->pCodeMgr->GetCode(strAlias, strCode, strDesc, nCount, nDefaultValue);
		if(0 > ids)
		{
			return -100;
		}
		api.SetFile(_T("..\\cfg\\LoanReturn\\ȭ�������_�ڷ��.txt"));
		INT col = 0;
		m_ReferenceColorDM.InitDataMgrRef(nFiledCnt);
		m_ReferenceColorDM.InsertRow(-1);
		nCount--;
		for(INT i=0; i<nCount+1; i++)
		{
			CString strTmp, strColorCode;
			strTmp.Format(_T("%s_���ڻ�"), strDesc[i]);
			((CComboBox*)GetDlgItem(IDC_cmbCOLORSELECT2))->AddString(strTmp);	
			
			m_ReferenceColorDM.SetDataMgrRef(col, strTmp, _T(""), _T("str"), _T(""), _T(""));
			col++;
			
			strTmp.Format(_T("%s_������"), strDesc[i]);
			((CComboBox*)GetDlgItem(IDC_cmbCOLORSELECT2))->AddString(strTmp);
			
			m_ReferenceColorDM.SetDataMgrRef(col, strTmp, _T(""), _T("str"), _T(""), _T(""));
			col++;
		}
		
		CString strTmp, strColor;
		for(INT h=0; h<nCount; h++)
		{
			strTmp.Format(_T("%s_���ڻ�"), strDesc[h]);
			api.GetData(strTmp, strColor);
			m_ReferenceColorDM.SetCellData(strTmp, strColor, 0);
			strTmp.Format(_T("%s_������"), strDesc[h]);
			api.GetData(strTmp, strColor);
			m_ReferenceColorDM.SetCellData(strTmp, strColor, 0);
		}
	}
	else if(_T("��������") == strAlias)
	{
		CString* strCode = new CString[nCount];
		CString* strDesc = new CString[nCount];
		INT ids = m_pInfo->pCodeMgr->GetCode(strAlias, strCode, strDesc, nCount, nDefaultValue);
		if(0 > ids)
		{
			return -200;
		}
		api.SetFile(_T("..\\cfg\\LoanReturn\\ȭ�������_��������.txt"));
		INT col = 0;
		m_ManageCodeColorDM.InitDataMgrRef(nFiledCnt);
		m_ManageCodeColorDM.InsertRow(-1);
		nCount--;
		for(INT i=0; i<nCount+1; i++)
		{
			CString strTmp, strColorCode;
			strTmp.Format(_T("%s_���ڻ�"), strDesc[i]);
			((CComboBox*)GetDlgItem(IDC_cmbCOLORSELECT1))->AddString(strTmp);	
			
			m_ManageCodeColorDM.SetDataMgrRef(col, strTmp, _T(""), _T("str"), _T(""), _T(""));
			col++;
			
			strTmp.Format(_T("%s_������"), strDesc[i]);
			((CComboBox*)GetDlgItem(IDC_cmbCOLORSELECT1))->AddString(strTmp);
			
			m_ManageCodeColorDM.SetDataMgrRef(col, strTmp, _T(""), _T("str"), _T(""), _T(""));
			col++;
		}	
		
		CString strTmp, strColor;
		for(INT h=0; h<nCount; h++)
		{
			strTmp.Format(_T("%s_���ڻ�"), strDesc[h]);
			api.GetData(strTmp, strColor);
			m_ManageCodeColorDM.SetCellData(strTmp, strColor, 0);
			strTmp.Format(_T("%s_������"), strDesc[h]);
			api.GetData(strTmp, strColor);
			m_ManageCodeColorDM.SetCellData(strTmp, strColor, 0);
		}
	}

	return 0;
}


INT CBO_LOC_3190::SetAutoColor(CString sColumnName, INT nMode)
{
	return 0;
}


void CBO_LOC_3190::OnSelchangecmbCOLORSELECT2() 
{
	EFS_BEGIN

	CComboBox *pComboBox;
	pComboBox = (CComboBox*)GetDlgItem(IDC_cmbCOLORSELECT2);
	
	CString sColumnName;
	pComboBox->GetLBText( pComboBox->GetCurSel() , sColumnName );

	CString sColor;
	m_ReferenceColorDM.GetCellData( sColumnName , 0 , sColor  );
	if(_T("") != sColor)
	{
		m_crColor2 = _ttoi( sColor );
		((CButton*)GetDlgItem(IDbtnCOLORSTATUS1))->SetWindowText(_T("������"));
	}
	else
	{
		m_crColor2 = 16777214;
		((CButton*)GetDlgItem(IDbtnCOLORSTATUS2))->SetWindowText(_T("���þ���"));
	}

	UpdateData(FALSE);

	EFS_END
}


void CBO_LOC_3190::OnSelchangecmbCOLORSELECT1() 
{
	EFS_BEGIN

	CComboBox *pComboBox;
	pComboBox = (CComboBox*)GetDlgItem(IDC_cmbCOLORSELECT1);
	
	CString sColumnName;
	pComboBox->GetLBText( pComboBox->GetCurSel() , sColumnName );

	CString sColor;
	m_ManageCodeColorDM.GetCellData( sColumnName , 0 , sColor  );
	if(_T("") != sColor)
	{
		m_crColor1 = _ttoi( sColor );
		((CButton*)GetDlgItem(IDbtnCOLORSTATUS1))->SetWindowText(_T("������"));
	}
	else
	{
		m_crColor1 = 16777214;
		((CButton*)GetDlgItem(IDbtnCOLORSTATUS1))->SetWindowText(_T("���þ���"));
	}

	UpdateData(FALSE);

	EFS_END
}



BOOL CBO_LOC_3190::Create(CWnd* pParentWnd) 
{
	
	
	return CDialog::Create(IDD, pParentWnd);
}
