// BO_CAT_PROC_0321.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_PROC_0321.h"
#include "BO_CAT_PROC_0320_FORM.h"

//#include _T("..\..\..\esl\marc\Marcedit\MarcEditor.h")
#include "..\..\..\����\KOMARC\KOMarcGetter\KOMarcGetterDlg.h"
#include "..\..\..\ESL\ESL\DlgClassCode.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0321 dialog


CBO_CAT_PROC_0321::CBO_CAT_PROC_0321(CESL_Mgr* pParent, CMarc &m_marc /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0321)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	pDM = FindDM(_T("DM_BO_CAT_PROC_0000"));
	pDM->SetMarcMgr(m_pInfo->pMarcMgr);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
	m_pInfo->pMarcMgr->Encoding(&m_marc, strStreamMarc);

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
}


VOID CBO_CAT_PROC_0321::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0321)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0321, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0321)
	ON_BN_CLICKED(IDC_bTAG_008_EDIT, OnbTAG008EDIT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0321 message handlers

BOOL CBO_CAT_PROC_0321::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_CAT_PROC_0321::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0321")) < 0)
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
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("�Է³�¥"), sItemData);
	}
	else {
		//sItemData = _T("");
		GetControlData( _T("CM_BO_CAT_PROC_0321"), _T("��¥") , sItemData );
		CString sDate;
		sDate.Format( _T("%s%s%s"), sItemData.Mid(2,2), sItemData.Mid(5,2), sItemData.Right(2) );
		SetControlData( _T("CM_BO_CAT_PROC_0321"), _T("�Է³�¥") , sDate );
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@6"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("���������"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@7-10"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("�����1"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@11-14"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("�����2"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@15-17"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("���౹��ȣ"), sItemData);
	}

	/*
	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("�ѱ����к�ȣ"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("�ѱ����к�ȣ"), sItemData);
	}
	*/

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@26-27"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("�ѱ����к�ȣ"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@28"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("����"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@32"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("�������"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@35-37"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("����"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@38-39"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("���α����ȣ"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@22"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("�̿����"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@25"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("��ǻ����������"), sItemData);
	}

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_CAT_PROC_0321::OnOK() 
{
	// TODO: Add extra validation here
	CString sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0321"), _T("�Է³�¥"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@0-5"), sData );

	CString sYear1;
	GetControlData(_T("CM_BO_CAT_PROC_0321"), _T("�����1"), sYear1);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@7-10"), sYear1 );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0321"), _T("�����2"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@11-14"), sData );

	sYear1 = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0321"), _T("���������"), sYear1);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@6"), sYear1 );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0321"), _T("���౹��ȣ"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@15-17"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0321"), _T("�ѱ����к�ȣ"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@26-27"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0321"), _T("����"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@28"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0321"), _T("�������"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@32"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0321"), _T("����"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@35-37"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0321"), _T("���α����ȣ"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@38-39"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0321"), _T("�̿����"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@22"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0321"), _T("��ǻ����������"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@25"), sData );

	
	CDialog::OnOK();
}

VOID CBO_CAT_PROC_0321::OnbTAG008EDIT() 
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

BOOL CBO_CAT_PROC_0321::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F9 )
	{	
		CWnd *pWnd;
		UINT wndID[ 4 ] = {
		IDC_cPUBLICATE_COUNTRY, IDC_cUNIVERCITY_CODE, IDC_cLANG, IDC_cGOVERNMENT };
		//	IDC_cSTATUS, IDC_cFORM, IDC_cSEOJI, IDC_cINPUT_LEVEL, IDC_cSTYLE
		
		//_T("���౹��ȣ"), _T("�ѱ����к�ȣ"), _T("���к�ȣ"), _T("�ѱ����α����ȣ"), _T("�ѱ��������к�ȣ"), 
		//_T("�ܱ��������к�ȣ"), _T("������ȣ"), _T("�ѱ���������ȣ") };

		BOOL bScreenYn = FALSE; // ȭ�� �ߴ��� ����
		for ( INT i = 0 ; i < 4 ; i++ ) {
			pWnd = ( CWnd * )GetDlgItem( wndID[ i ] );
			if ( pWnd->m_hWnd == pMsg->hwnd ) {
				CKOMarcGetterDlg dlg( this );
				INT iMode;
				
				CString sControlAlias;
				switch( i ) {
					case 0 : 
						iMode = 0;
						sControlAlias = _T("���౹��ȣ");
						break;
					case 1 : 
						iMode = 1;
						sControlAlias = _T("�ѱ����к�ȣ");
						break;
					case 2 :
						iMode = 2;
						sControlAlias = _T("���");
						break;
					case 3 :
						iMode = 3;
						sControlAlias = _T("���α����ȣ");
						break;
				}					

				CString sValue;
				pWnd->GetWindowText( sValue );
				dlg.InitSetProperty( iMode, sValue );
				if ( dlg.DoModal() == IDOK ) {
					CString sCode = dlg.m_code;
					SetControlData( _T("CM_BO_CAT_PROC_0321"), sControlAlias, sCode );
				}
				bScreenYn = TRUE;
			}
		}
		if ( bScreenYn == TRUE ) {
			return true;
		}

		//��ȣ�ֱ�
		//CWnd *pWnd;
		UINT wndCodeID[ 5 ] = {		
			IDC_cSTYLE, IDC_cUPDATE, IDC_cLIST_JUNGEO, IDC_cUSER_LEVEL1, IDC_cFILE_FORM };
			//IDC_cDISCERN, IDC_cDATA_TYPE1, IDC_cDATA_TYPE2, IDC_cUPDATE, IDC_cCONFERENCE,
			//IDC_cTHESIS, IDC_cINDEX, IDC_cLIST_JUNGEO, IDC_cLITERATURE, IDC_cJUNGEO };
			//IDC_cSTATUS, IDC_cFORM, IDC_cSEOJI, IDC_cINPUT_LEVEL, IDC_cSTYLE };

		CString sValue;		//����, ��ǥ��...
		CString sControlAlias;
		for ( i = 0 ; i < 5 ; i++ ) {
			pWnd = ( CWnd * )GetDlgItem( wndCodeID[ i ] );
			if ( pWnd->m_hWnd == pMsg->hwnd ) {
				CKOMarcGetterDlg dlg( this );

				//INT iMode;				
				//CString sControlAlias;
				switch( i ) {
					case 0 : 						
						sValue = _T("(���������)");
						sControlAlias = _T("���������");
						break;
					case 1 :
						sValue = _T("(�������ڵ�)");
						sControlAlias = _T("����");
						break;
					case 2 :
						sValue = _T("(�������)");
						sControlAlias = _T("�������");
						break;
					case 3 :
						sValue = _T("(�̿����ڼ���)");
						sControlAlias = _T("�̿����");
						break;
					case 4 :						
						sValue = _T("(��ǻ����������)");
						sControlAlias = _T("��ǻ����������");
						break;									
				}			
			}
		}

		if ( sValue.IsEmpty() ) {
			return FALSE;
		}

		//ȭ�� ����
		CDlgClassCode dlg;		
		dlg.InitData();
		//dlg.AddData( _T("a"), _T("b"));

		//�����о����
		FILE *fp;
		TCHAR cLine[256];
		fp = _tfopen( _T("c:\\kolas2up\\bin\\008TAG�ڵ�.txt") , _T("rb") );
		if ( fp == NULL ) {
			AfxMessageBox( _T("c:\\kolas2up\\bin\\008TAG�ڵ�.txt ������ �����ϴ�.") );
			return TRUE;
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
		
		//[CF]ã��
		BOOL bFormYn = FALSE;
		while( _fgetts( cLine, 256, fp) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			sLine.Replace( sLine.Right(1), NULL );
			//[CF]ã��						
			if ( sLine == _T("[CF]") ) {
				bFormYn = TRUE;
				break;
			}
		}
		if ( bFormYn == FALSE ) {
			AfxMessageBox( _T("[CF]�� �����ϴ�.") );
			return true;
		}
		//(���������)... ã��
		BOOL bControlYn = FALSE;
		while( _fgetts( cLine, 256, fp) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			if ( sLine.Left(1) == _T("[") ) {
				return true;
			}
			sLine.Replace( sLine.Right(1), NULL );
			if ( sLine == sValue ) {
				bControlYn = TRUE;
				break;
			}
		}
		//�� ��������
		while( _fgetts( cLine, 256, fp) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			if ( sLine.Left(1) == _T(".") ) {
				break;
			}
			sLine.Replace( sLine.Right(1), NULL );
			CString sCode, sDesc;
			sCode = sLine.Left(1);
			sDesc = sLine.Mid(2);			
			dlg.AddData( sCode, sDesc );
		}

		if (dlg.DoModal() == IDOK) {			
			CString sCode = dlg.m_strSelCode;
			SetControlData( _T("CM_BO_CAT_PROC_0321"), sControlAlias, sCode );
		}		

		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
