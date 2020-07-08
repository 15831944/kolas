// BO_CAT_PROC_0320_01.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_PROC_0320_01.h"
#include "BO_CAT_PROC_0320_FORM.h"

//#include _T("..\..\..\esl\marc\Marcedit\MarcEditor.h")
#include "..\..\..\����\KOMARC\KOMarcGetter\KOMarcGetterDlg.h"

#include "..\..\..\ESL\ESL\DlgClassCode.h"

#include <direct.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0320_01 dialog


CBO_CAT_PROC_0320_01::CBO_CAT_PROC_0320_01(CESL_Mgr* pParent, CMarc &m_marc /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{

//	m_marc = m_marc_in;
	//CMarc m_marc;

	
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

	//{{AFX_DATA_INIT(CBO_CAT_PROC_0320_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CBO_CAT_PROC_0320_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0320_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0320_01, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0320_01)
	ON_BN_CLICKED(IDC_bVARIABLE, OnbVARIABLE)
	ON_BN_CLICKED(IDC_bTAG_008_EDIT, OnbTAG008EDIT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0320_01 message handlers

BOOL CBO_CAT_PROC_0320_01::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_CAT_PROC_0320_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0320_01")) < 0)
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
	
	/*
	if ( sItemData != _T("") ) {
		m_pInfo->pMarcMgr->SetItem(&m_marc, _T("û����ȣ_��å��ȣ"), sItemData);
	}
	*/
	INT iIntCh = 0;
	if ( sItemData != _T("") ) {
		for ( INT i = 0 ; i < sItemData.GetLength() ; i ++ ) {
			CString sCh = sItemData.Mid( i, 1 );
			//TCHAR ch = sCh.GetBuffer(0);
			TCHAR ch = sCh.GetAt(0);
			if(!((ch >= '0')&&(ch <= '9'))) {			
				iIntCh = 1;
				break;
			}
		}
		if ( iIntCh == 0 ) {
			SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("�Է³�¥"), sItemData);
		}
		else{
			SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("�Է³�¥"), _T(""));
		}
	}
	else {
		//sItemData = _T("");
		GetControlData( _T("CM_BO_CAT_PROC_0320_01"), _T("��¥") , sItemData );
		CString sDate;
		sDate.Format( _T("%s%s%s"), sItemData.Mid(2,2), sItemData.Mid(5,2), sItemData.Right(2) );
		SetControlData( _T("CM_BO_CAT_PROC_0320_01"), _T("�Է³�¥") , sDate );
	}
	
	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@6"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("���������"), sItemData);
	}
	else {
		SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("���������"), _T("s"));
	}


	sItemData = _T("");
	//m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@7-10"), sItemData);
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("�����"), sItemData);
	if ( sItemData != _T("") ) {
		for ( INT i = 0 ; i < sItemData.GetLength() ; i ++ ) {
			CString sCh = sItemData.Mid( i, 1 );
			//TCHAR ch = sCh.GetBuffer(0);
			TCHAR ch = sCh.GetAt(0);
			if(!((ch >= '0')&&(ch <= '9'))) {			
				iIntCh = 1;
				break;
			}
		}
		if ( iIntCh == 0 ) {
			SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("�����1"), sItemData);
		}
		else{
			SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("�����1"), _T(""));
		}
		//SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("�����1"), sItemData);
	}
	else {
		m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@7-10"), sItemData);
		//m_pInfo->pMarcMgr->GetItem(&m_marc, _T("�����"), sItemData);
		SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("�����1"), sItemData);
	}


	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@11-14"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("�����2"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@15-17"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("���౹��ȣ"), sItemData);
	}

	
	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@18"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("��ǥ��1"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@19"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("��ǥ��2"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@20"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("��ǥ��3"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@21"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("��ǥ��3"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@22"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("�̿����"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@23"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("�ڷ�ĺ�"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@24"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("��������1"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@25"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("��������2"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@26-27"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("�ѱ����к�ȣ"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@28"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("����"), sItemData);
	}


	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@29"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("ȸ��"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@30"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("����"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@31"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("����"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@32"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("�������"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@33"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("��������"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@34"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("����"), sItemData);
	}


	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@35-37"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("���"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@38-39"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("���α����ȣ"), sItemData);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_CAT_PROC_0320_01::OnbVARIABLE() 
{
	// TODO: Add your control notification handler code here
	//m_BO_CAT_PROC_0300->m_marcEditor.ApplyEdit();

	CString sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("�Է³�¥"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@0-5"), sData );

	CString sYear1;
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("�����1"), sYear1);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@7-10"), sYear1 );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("�����2"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@11-14"), sData );	

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("���������"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@6"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("���౹��ȣ"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@15-17"), sData );

	CString tmp = strStreamMarc;
	
	//CDialog::OnOK();
}

VOID CBO_CAT_PROC_0320_01::OnOK() 
{
	// TODO: Add extra validation here	

	CString sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("�Է³�¥"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@0-5"), sData );

	CString sYear1;
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("�����1"), sYear1);
	sYear1.Replace( _T(" ") , NULL );
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@7-10"), sYear1 );
	if ( sYear1 != _T("") ) {
		CString sTmp;
		//m_pInfo->pMarcMgr->GetItem(&m_marc, _T("�����"), sTmp );
		m_pInfo->pMarcMgr->DeleteItem(&m_marc, _T("�����") );
		m_pInfo->pMarcMgr->SetItem(&m_marc, _T("�����"), sYear1 );
	}

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("�����2"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@11-14"), sData );

	sYear1 = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("���������"), sYear1);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@6"), sYear1 );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("���౹��ȣ"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@15-17"), sData );

/*
	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("���౹��ȣ"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("��������"), sData );
*/
	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("��ǥ��1"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@18"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("��ǥ��2"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@19"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("��ǥ��3"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@20"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("��ǥ��4"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@21"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("�̿����"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@22"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("�ڷ�ĺ�"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@23"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("��������1"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@24"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("��������2"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@25"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("�ѱ����к�ȣ"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@26-27"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("����"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@28"), sData );


	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("ȸ��"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@29"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("����"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@30"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("����"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@31"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("�������"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@32"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("��������"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@33"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("����"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@34"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("����"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@35-37"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("���α����ȣ"), sData);
	if ( sData.IsEmpty() ) {
		sData = _T("  ");
	}
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@38-39"), sData );
	
	CString tmp = strStreamMarc;
	
	/*
	// ���� �Է��ϱ�
	CString sLeader;
	m_marc.GetLeader( sLeader );

	CString sStatus, sForm, sSeoji, sInputLevel;
	GetControlData( _T("CM_BO_CAT_PROC_0320_01"), _T("���ڵ����"), sStatus );
	GetControlData( _T("CM_BO_CAT_PROC_0320_01"), _T("���ڵ�����"), sForm );
	GetControlData( _T("CM_BO_CAT_PROC_0320_01"), _T("��������"), sSeoji );
	GetControlData( _T("CM_BO_CAT_PROC_0320_01"), _T("�Է¼���"), sInputLevel );

	CString sTmp;
	sTmp = sLeader.Left(5) + sStatus + sForm + sSeoji + sLeader.Mid(8, 9) + sInputLevel + sLeader.Right(6);
	//sTmp = sLeader.Left(5) + sStatus + sLeader.Right(18);
	m_marc.SetLeader( sTmp.GetBuffer(0) );
	*/
	
	CDialog::OnOK();
}

VOID CBO_CAT_PROC_0320_01::OnbTAG008EDIT() 
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

BOOL CBO_CAT_PROC_0320_01::PreTranslateMessage(MSG* pMsg) 
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
				/*
				iMode = i;
				if ( 2 == i ) {
					iMode = 3;
				}
				*/
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
					SetControlData( _T("CM_BO_CAT_PROC_0320_01"), sControlAlias, sCode );
					return true;
				}
				bScreenYn = TRUE;
			}
		}
		if ( bScreenYn == TRUE ) {
			return true;
		}
/*
		//if ( GetDlgItem(IDC_cUNIVERCITY_CODE)->GetFocus() ) {			
		if (GetDlgItem(IDC_cUNIVERCITY_CODE)->m_hWnd == pMsg->hwnd) {
			CKOMarcGetterDlg dlg( this );
			dlg.InitSetProperty( 4, _T("") );
			dlg.DoModal();
			CString sCode = dlg.m_code;			
		}		
		*/


		//��ȣ�ֱ�
		//CWnd *pWnd;
		UINT wndCodeID[ 16 ] = {		
			IDC_cSTYLE, IDC_eILLUST1, IDC_eILLUST2, IDC_eILLUST3, IDC_eILLUST4,	IDC_cUSE, 
			IDC_cDISCERN, IDC_cDATA_TYPE1, IDC_cDATA_TYPE2, IDC_cUPDATE, IDC_cCONFERENCE,
			IDC_cTHESIS, IDC_cINDEX, IDC_cLIST_JUNGEO, IDC_cLITERATURE, IDC_cJUNGEO };
			//IDC_cSTATUS, IDC_cFORM, IDC_cSEOJI, IDC_cINPUT_LEVEL, IDC_cSTYLE };

		CString sValue;		//����, ��ǥ��...
		CString sControlAlias;
		for ( i = 0 ; i < 16 ; i++ ) {
			pWnd = ( CWnd * )GetDlgItem( wndCodeID[ i ] );
			if ( pWnd->m_hWnd == pMsg->hwnd ) {
				CKOMarcGetterDlg dlg( this );
				//INT iMode;
				/*
				iMode = i;
				if ( 2 == i ) {
					iMode = 3;
				}
				*/
				//CString sControlAlias;
				switch( i ) {
					case 0 : 						
						sValue = _T("(���������)");
						sControlAlias = _T("���������");
						break;
					case 1 :
						sValue = _T("(��ǥ��)");
						sControlAlias = _T("��ǥ��1");
						break;
					case 2 :
						sValue = _T("(��ǥ��)");
						sControlAlias = _T("��ǥ��2");
						break;
					case 3 :
						sValue = _T("(��ǥ��)");
						sControlAlias = _T("��ǥ��3");
						break;
					case 4 :						
						sValue = _T("(��ǥ��)");
						sControlAlias = _T("��ǥ��4");
						break;					
					case 5 :						
						sValue = _T("(�̿����)");
						sControlAlias = _T("�̿����");
						break;
					case 6 : 
						sValue = _T("(�ڷ�ĺ�)");
						sControlAlias = _T("�ڷ�ĺ�");
						break;
					case 7 :
						sValue = _T("(��������)");
						sControlAlias = _T("��������1");
						break;
					case 8 :
						sValue = _T("(��������)");
						sControlAlias = _T("��������2");
						break;
					case 9 :
						sValue = _T("(����)");
						sControlAlias = _T("����");
						break;
					case 10 :
						sValue = _T("(ȸ��)");
						sControlAlias = _T("ȸ��");
						break;
					case 11 :
						sValue = _T("(����)");
						sControlAlias = _T("����");
						break;
					case 12 :
						sValue = _T("(����)");
						sControlAlias = _T("����");
						break;
					case 13 :
						sValue = _T("(�������)");
						sControlAlias = _T("�������");
						break;
					case 14 :
						sValue = _T("(��������)");
						sControlAlias = _T("��������");
						break;
					case 15 :
						sValue = _T("(����)");
						sControlAlias = _T("����");
						break;					
				}					
				
				//CString sValue;
				//pWnd->GetWindowText( sValue );
				//dlg.InitSetProperty( iMode, sValue );
				//if ( dlg.DoModal() == IDOK ) {
				//	CString sCode = dlg.m_code;
				//	SetControlData( _T("CM_BO_CAT_PROC_0320_01"), sControlAlias, sCode );
				//}
				
			}
		}

		if ( sValue.IsEmpty() ) {
			return FALSE;
		}

		//ȭ�� ����
		CDlgClassCode dlg;		
		dlg.InitData();
		//dlg.AddData( _T("a"), _T("b"));

		//����̺� ���ؿ���
		INT curdrive;	
		curdrive = _getdrive();
		CString sPath;
		if(curdrive==3){
			sPath=_T("C:\\KOLAS2UP\\CFG\\008TAG�ڵ�.txt");
		}
		if(curdrive==4){
			sPath=_T("D:\\KOLAS2UP\\CFG\\008TAG�ڵ�.txt");
		}
		if(curdrive==5){
			sPath=_T("E:\\KOLAS2UP\\CFG\\008TAG�ڵ�.txt");
		}

		//�����о����
		FILE *fp;
		TCHAR cLine[256];
		fp = _tfopen( sPath , _T("rb") );
		if ( fp == NULL ) {
			AfxMessageBox( _T("008TAG�ڵ� ������ �����ϴ�.") );
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
		
		//[TX]ã��
		BOOL bFormYn = FALSE;
		while( _fgetts( cLine, 256, fp) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			sLine.Replace( sLine.Right(1), NULL );
			//[TX]ã��						
			if ( sLine == _T("[TX]") ) {
				bFormYn = TRUE;
				break;
			}
		}
		if ( bFormYn == FALSE ) {
			AfxMessageBox( _T("[TX]�� �����ϴ�.") );
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
		/*
		while( _fgetts( cLine, 256, fp) ){
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			sLine.Replace( sLine.Right(1), NULL );
			//[TX]ã��
			//INT iFormYn = 0; //0:����, 1:����
			BOOL bFormYn = FALSE;
			if ( sLine == _T("[TX]") ) {
				bFormYn = TRUE;
			}

			if ( sLine == _T("[TX]") ) {
				_fgetts( cLine, 256, fp );
				sLine.Format( _T("%s"), cLine );
				sLine.Replace( sLine.Right(1), NULL );
				//'(' ã��
				while ( sLine.Left(1) != _T("(") ) {
					_fgetts( cLine, 256, fp );
					sLine.Format( _T("%s"), cLine );
					sLine.Replace( sLine.Right(1), NULL );
				}

				if ( sLine == sValue ) {	// (���������)...
					_fgetts( cLine, 256, fp );
					sLine.Format( _T("%s"), cLine );
					while ( sLine.Left(1) != _T(".") ) {
						CString sCode, sDesc;
						sCode = sLine.Left(1);
						sDesc = sLine.Mid(2);
						_fgetts( cLine, 256, fp );
						sLine.Format( _T("%s"), cLine );
						sLine.Replace( sLine.Right(1), NULL );
						dlg.AddData( sCode, sDesc );
					}
				}
			}
		}		
		*/
		if (dlg.DoModal() == IDOK) {			
			CString sCode = dlg.m_strSelCode;
			SetControlData( _T("CM_BO_CAT_PROC_0320_01"), sControlAlias, sCode );
			//SetControlData(_T("CM_BO_CAT_PROC_0800"), _T("������DATA"), dlg.m_strSelCode);
		}		

		return true;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
