// BO_SERIAL_NORMAL_CONFIG.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_SERIAL_NORMAL_CONFIG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_SERIAL_NORMAL_CONFIG dialog


CBO_SERIAL_NORMAL_CONFIG::CBO_SERIAL_NORMAL_CONFIG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_SERIAL_NORMAL_CONFIG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CBO_SERIAL_NORMAL_CONFIG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_SERIAL_NORMAL_CONFIG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_SERIAL_NORMAL_CONFIG, CDialog)
	//{{AFX_MSG_MAP(CBO_SERIAL_NORMAL_CONFIG)
	ON_BN_CLICKED(IDC_bSAVE, OnbSAVE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_SERIAL_NORMAL_CONFIG message handlers

BOOL CBO_SERIAL_NORMAL_CONFIG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString sPathUnit = _T("..\\CFG\\����_���ӿ���_�̾����_���.cfg");
	//�����о����
	FILE *fpSerialConfig;
	TCHAR cLineConfig[256];
	fpSerialConfig = _tfopen( sPathUnit , _T("rb") );
	CComboBox *pCobFirstLineSpace = NULL;
	pCobFirstLineSpace = (CComboBox*)GetDlgItem(IDC_CobFirstLineSpace);
	pCobFirstLineSpace->InsertString( 0, _T("���� ����") );
	pCobFirstLineSpace->InsertString( 1, _T("���� ����") );		
	if ( fpSerialConfig == NULL ) {
		//AfxMessageBox( _T("..\\CFG\\����_���_���.cfg ������ �����ϴ�.") );
		GetDlgItem(IDC_EdtSecondLoc)->SetWindowText( _T("0") );
		GetDlgItem(IDC_EdtRegnoLoc)->SetWindowText( _T("30") );	
		
		pCobFirstLineSpace->SetCurSel( 0 );		

		return FALSE;
	}	
	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode�϶� 2byte�ǳʶ��
	FILE* fpUni = fpSerialConfig;

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
	CString sLineConfig;
	CString sFirstLoc, sSecLoc, sColnoLoc, sColCount;
	while( _fgetts( cLineConfig, 256, fpSerialConfig) ) {
		CString sLine;
		sLine.Format( _T("%s"), cLineConfig );
		//CString sItem = sLine.Right(10);
		INT iFind = sLine.Find( _T(":") );
		CString sItem = sLine.Mid( iFind+1 );		
		sItem.Replace( _T(" "), NULL );
		sItem.Replace( _T("\r\n"), NULL );

		if ( sLine.Left(1) == _T("1") ) {
			GetDlgItem(IDC_EdtSecondLoc)->SetWindowText( sItem );		
		}
		else if ( sLine.Left(1) == _T("2") ) {
			GetDlgItem(IDC_EdtRegnoLoc)->SetWindowText( sItem );
		}		
		else if ( sLine.Left(1) == _T("3") ) {
			/*
			if ( sItem == _T("��������") ) {
				pCobFirstLineSpace->SetCurSel( 0 );
			}
			else {
				pCobFirstLineSpace->SetCurSel( 1 );
			}
			*/
			GetDlgItem(IDC_CobFirstLineSpace)->SetWindowText( sItem );
		}
		
	}
	fclose(fpSerialConfig);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_SERIAL_NORMAL_CONFIG::OnbSAVE() 
{
	// TODO: Add your control notification handler code here
	CString sPathUnit = _T("..\\CFG\\����_���ӿ���_�̾����_���.cfg");
	//�����о����
	FILE *fpSerial;
	//TCHAR cLineUnit[256];
	fpSerial = _tfopen( sPathUnit , _T("wb") );
	if ( fpSerial == NULL ) {
		AfxMessageBox( _T("..\\CFG\\����_���ӿ���_�̾����_���.cfg ���� Ȯ��.") );
		return; 
	}
	fseek(fpSerial, 0, SEEK_END);		
	if( 0 == ftell(fpSerial) )
	{
		/*UNCHANGE*/fputc(0xFF, fpSerial);
		/*UNCHANGE*/fputc(0xFE, fpSerial);
	}
	CString sSecLoc, sRegnoLoc, sColnoLoc, sFirstLineSpace;
	GetDlgItem(IDC_EdtSecondLoc)->GetWindowText( sSecLoc );
	GetDlgItem(IDC_EdtRegnoLoc)->GetWindowText( sRegnoLoc );
	GetDlgItem(IDC_CobFirstLineSpace)->GetWindowText( sFirstLineSpace );	

	sSecLoc = _T("1.�ι�° û����ȣ ��ġ : ") + sSecLoc + _T("\r\n");	
	sRegnoLoc = _T("2.��Ϲ�ȣ ��ġ : ") + sRegnoLoc + _T("\r\n");
	sFirstLineSpace = _T("3.ù ���� ���鿩�� : ") + sFirstLineSpace + _T("\r\n");
	
	_ftprintf( fpSerial, sSecLoc );
	_ftprintf( fpSerial, sRegnoLoc );
	_ftprintf( fpSerial, sFirstLineSpace );

	fclose(fpSerial);

	AfxMessageBox( _T("���� �Ͽ����ϴ�.") );
}
