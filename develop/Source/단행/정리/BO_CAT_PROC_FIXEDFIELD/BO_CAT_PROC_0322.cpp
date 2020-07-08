// BO_CAT_PROC_0322.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_PROC_0322.h"
#include "BO_CAT_PROC_0320_FORM.h"
#include "..\..\..\����\KOMARC\KOMarcGetter\KOMarcGetterDlg.h"
#include "..\..\..\ESL\ESL\DlgClassCode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0322 dialog


CBO_CAT_PROC_0322::CBO_CAT_PROC_0322(CESL_Mgr* pParent, CString sDm, CMarc &m_marc /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0322)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//pDM = FindDM(_T("DM_BO_CAT_PROC_0000"));
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

CBO_CAT_PROC_0322::~CBO_CAT_PROC_0322()
{
}

VOID CBO_CAT_PROC_0322::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0322)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0322, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0322)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bTAG_008_EDIT, OnbTAG008EDIT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0322 message handlers

BOOL CBO_CAT_PROC_0322::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0322::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0322")) < 0)
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
		SetControlData(_T("CM_BO_CAT_PROC_0322"), _T("�Է³�¥"), sItemData);
	}
	else {
		//sItemData = _T("");
		GetControlData( _T("CM_BO_CAT_PROC_0322"), _T("��¥") , sItemData );
		CString sDate;
		sDate.Format( _T("%s%s%s"), sItemData.Mid(2,2), sItemData.Mid(5,2), sItemData.Right(2) );
		SetControlData( _T("CM_BO_CAT_PROC_0322"), _T("�Է³�¥") , sDate );
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@6"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0322"), _T("���������"), sItemData);
	}
	else {
		SetControlData(_T("CM_BO_CAT_PROC_0322"), _T("���������"), _T("s"));
	}

	sItemData = _T("");
	/*
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@7-10"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0322"), _T("�����1"), sItemData);
	}
	*/
	INT iIntCh = 0;
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
			SetControlData(_T("CM_BO_CAT_PROC_0322"), _T("�����1"), sItemData);
		}
		else{
			SetControlData(_T("CM_BO_CAT_PROC_0322"), _T("�����1"), _T(""));
		}
		//SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("�����1"), sItemData);
	}
	else {
		CTime time = CTime::GetCurrentTime();
		m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@7-10"), sItemData);
		//m_pInfo->pMarcMgr->GetItem(&m_marc, _T("�����"), sItemData);
		if (sItemData.IsEmpty())
			sItemData = time.Format(_T("%Y"));
		SetControlData(_T("CM_BO_CAT_PROC_0322"), _T("�����1"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@11-14"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0322"), _T("�����2"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@15-17"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0322"), _T("���౹��ȣ"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@26-27"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0322"), _T("�ѱ����к�ȣ"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@28"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0322"), _T("����"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@32"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0322"), _T("�������"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@35-37"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0322"), _T("����"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@38-39"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0322"), _T("���α����ȣ"), sItemData);
	}


	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@18"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0322"), _T("������1"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@19"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0322"), _T("������2"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@20"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0322"), _T("������3"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@21"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0322"), _T("������4"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@22-23"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0322"), _T("����"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@24"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0322"), _T("�����ڿ���"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@25"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0322"), _T("�ڷ�������"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@31"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0322"), _T("����"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@33"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0322"), _T("���»���Ư¡1"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@34"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0322"), _T("���»���Ư¡2"), sItemData);
	}

	// ���� �о����
	CString sLeader;
	m_marc.GetLeader( sLeader );

	CString sStatus, sForm, sSeoji, sInputLevel;
	CString sMok, sRelation;
	sStatus = sLeader.Mid(5,1);
	sForm = sLeader.Mid(6,1);
	sSeoji = sLeader.Mid(7,1);
	sInputLevel = sLeader.Mid(17,1);
	sMok = sLeader.Mid(18,1);
	sRelation = sLeader.Mid(19,1);

	if ( sStatus != _T(" ") ) {
		SetControlData( _T("CM_BO_CAT_PROC_0322"), _T("����"), sStatus );
	}
	if ( sForm != _T(" ") ) {
		SetControlData( _T("CM_BO_CAT_PROC_0322"), _T("���ڵ�����"), sForm );
	}
	if ( sSeoji != _T(" ") ) {
		SetControlData( _T("CM_BO_CAT_PROC_0322"), _T("��������"), sSeoji );
	}
	if ( sInputLevel != _T(" ") ) {
		SetControlData( _T("CM_BO_CAT_PROC_0322"), _T("�Է¼���"), sInputLevel );
	}
	if ( sMok != _T(" ") ) {
		SetControlData( _T("CM_BO_CAT_PROC_0322"), _T("��ϱ������"), sMok );
	}
	if ( sRelation != _T(" ") ) {
		SetControlData( _T("CM_BO_CAT_PROC_0322"), _T("�������ڵ�����"), sRelation );
	}

	// ���������� ������ �����̸� s �ƴϸ� m�� �־��ش�.
	if (sSeoji == _T(" "))
	{
		if (m_pInfo->pMarcMgr->GetMarcGroup() == _T("����"))
			sSeoji = _T("s");
		else
			sSeoji = _T("m");
		
		SetControlData( _T("CM_BO_CAT_PROC_0320_01"), _T("��������"), sSeoji );
	}
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0322::OnOK() 
{
EFS_BEGIN

	// TODO: Add extra validation here
	CString sMessage;
	CString sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0322"), _T("�Է³�¥"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@0-5"), sData );
	if ( sData == _T("") ) {
		sMessage = _T(", �Է³�¥");
	}

	CString sYear1;
	GetControlData(_T("CM_BO_CAT_PROC_0322"), _T("�����1"), sYear1);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@7-10"), sYear1 );
	if ( sYear1 == _T("") ) {
		sMessage = sMessage + _T(", �����1");		
	}

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0322"), _T("�����2"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@11-14"), sData );
	/*
	if ( sData == _T("") ) {
		sMessage = sMessage + _T(", �����2");
	}
	*/

	sYear1 = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0322"), _T("���������"), sYear1);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@6"), sYear1 );
	if ( sYear1 == _T("") ) {
		sMessage = sMessage + _T(", ����");
	}

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0322"), _T("���౹��ȣ"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@15-17"), sData );
	/*
	if ( sData == _T("") ) {
		sMessage = sMessage + _T(", ���౹��");
	}
	*/

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0322"), _T("�ѱ����к�ȣ"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@26-27"), sData );
	/*
	if ( sData == _T("") ) {
		sMessage = sMessage + _T(", ���к�ȣ");
	}
	*/

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0322"), _T("����"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@28"), sData );
	/*
	if ( sData == _T("") ) {
		sMessage = sMessage + _T(", �������ڵ�");
	}
	*/

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0322"), _T("�������"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@32"), sData );
	/*
	if ( sData == _T("") ) {
		sMessage = sMessage + _T(", �������");
	}
	*/

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0322"), _T("����"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@35-37"), sData );
	/*
	if ( sData == _T("") ) {
		sMessage = sMessage + _T(", ���");
	}
	*/

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0322"), _T("���α����ȣ"), sData);
	if ( sData.IsEmpty() ) {
		/*
		if ( sData == _T("") ) {
			sMessage = sMessage + _T(", ���α��");
		}
		*/
		sData = _T("  ");
	}
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@38-39"), sData );
	
	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0322"), _T("������1"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@18"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0322"), _T("������2"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@19"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0322"), _T("������3"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@20"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0322"), _T("������4"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@21"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0322"), _T("����"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@22-23"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0322"), _T("�����ڿ���"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@24"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0322"), _T("�ڷ�������"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@25"), sData );
	if ( sData == _T("") ) {
		sMessage = sMessage + _T(", �ڷ�������");
	}

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0322"), _T("����"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@31"), sData );

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0322"), _T("���»���Ư¡1"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@33"), sData );
	/*
	if ( sData == _T("") ) {
		sMessage = sMessage + _T(", ���»���Ư¡1");
	}
	*/

	sData = _T("");
	GetControlData(_T("CM_BO_CAT_PROC_0322"), _T("���»���Ư¡2"), sData);
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("008@34"), sData );
	/*
	if ( sData == _T("") ) {
		sMessage = sMessage + _T(", ���»���Ư¡2");
	}
	*/

	//�޼��� �����ֱ�
	if ( sMessage.GetLength() > 3 ) {
		sMessage = sMessage.Mid(2) + _T("��(��) �Է��ؾ� �մϴ�.");
		AfxMessageBox( sMessage );
		return;
	}

	// ���� �Է��ϱ�
	CString sLeader;
	m_marc.GetLeader( sLeader );

	CString sStatus, sForm, sSeoji, sInputLevel, sMok, sRecord;
	GetControlData( _T("CM_BO_CAT_PROC_0322"), _T("���ڵ����"), sStatus );
	GetControlData( _T("CM_BO_CAT_PROC_0322"), _T("���ڵ�����"), sForm );
	GetControlData( _T("CM_BO_CAT_PROC_0322"), _T("��������"), sSeoji );
	GetControlData( _T("CM_BO_CAT_PROC_0322"), _T("�Է¼���"), sInputLevel );
	GetControlData( _T("CM_BO_CAT_PROC_0322"), _T("��ϱ������"), sMok );
	GetControlData( _T("CM_BO_CAT_PROC_0322"), _T("�������ڵ�����"), sRecord );

	if ( sInputLevel == _T("") ) {
		sInputLevel = _T(" ");
	}
	if ( sMok == _T("") ) {
		sMok = _T(" ");
	}
	if ( sRecord == _T("") ) {
		sRecord = _T(" ");
	}

	CString sTmp;
	//sTmp = sLeader.Left(5) + sStatus + sForm + sSeoji + sLeader.Mid(8, 9) + sInputLevel + sMok + sRecord + sLeader.Right(4);
	sTmp.Format(_T("%5s%1s%1s%1s%s%1s%1s%1s%s"), sLeader.Left(5), sStatus, sForm, sSeoji, sLeader.Mid(8, 9), sInputLevel, sMok, sRecord, sLeader.Right(4));
	//sTmp = sLeader.Left(5) + sStatus + sLeader.Right(18);
	m_marc.SetLeader( sTmp.GetBuffer(0) );


	CDialog::OnOK();

EFS_END
}

VOID CBO_CAT_PROC_0322::OnbTAG008EDIT() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CBO_CAT_PROC_0320_FORM dlg(this);
	CString s007;
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("007*"), s007);
	dlg.SetType007( _T("MP"), s007 );
	//dlg.DoModal();
	if( IDOK == dlg.DoModal() ) {
		m_pInfo->pMarcMgr->DeleteField(&m_marc,_T("007"));
		m_pInfo->pMarcMgr->SetItem(&m_marc, _T("007*"), dlg.s007);
		m_pInfo->pMarcMgr->SortMarc( &m_marc );
	}

EFS_END
}

BOOL CBO_CAT_PROC_0322::PreTranslateMessage(MSG* pMsg) 
{
EFS_BEGIN

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
					SetControlData( _T("CM_BO_CAT_PROC_0322"), sControlAlias, sCode );
				}
				bScreenYn = TRUE;
			}
		}
		if ( bScreenYn == TRUE ) {
			return true;
		}



		//���� ��ȣ�ֱ�
		UINT wndCodeIdLeader[6] = {
			IDC_cSTATUS, IDC_cFORM, IDC_cSEOJI, IDC_cINPUT_LEVEL,
			IDC_cSTATUS2, IDC_cFORM2
		};
		CString sValueLeader;
		CString sControlAliasLeader;
		for ( i = 0 ; i < 6 ; i++ ) {
			pWnd = ( CWnd * )GetDlgItem( wndCodeIdLeader[ i ] );
			if ( pWnd->m_hWnd == pMsg->hwnd ) {
				CKOMarcGetterDlg dlg( this );				
				switch( i ) {
					case 0 : 						
						sValueLeader = _T("(���ڵ����)");
						sControlAliasLeader = _T("���ڵ����");
						break;
					case 1 :
						sValueLeader = _T("(���ڵ�����)");
						sControlAliasLeader = _T("���ڵ�����");
						break;
					case 2 :
						if (m_pInfo->pMarcMgr->GetMarcGroup() == _T("����"))
						{
							sValueLeader = _T("(��������)����");
						}
						else
							sValueLeader = _T("(��������)����");
						sControlAliasLeader = _T("��������");
						break;
					case 3 :
						sValueLeader = _T("(�Է¼���)");
						sControlAliasLeader = _T("�Է¼���");
						break;
					case 4 :
						sValueLeader = _T("(��ϱ������)");
						sControlAliasLeader = _T("��ϱ������");
						break;
					case 5 :
						sValueLeader = _T("(�������ڵ�����)");
						sControlAliasLeader = _T("�������ڵ�����");
						break;
				}
			}
		}

		INT iLeaderYes = 1;
		if ( sValueLeader.IsEmpty() ) {
			iLeaderYes = 0;
			//return FALSE;
		}
		if ( iLeaderYes == 1 ) {
			//ȭ�� ����
			CDlgClassCode dlgLeader(this);		
			dlgLeader.InitData();

			CString sPathLeader = _T("..\\CFG\\�����ڵ�.txt");
			//�����о����
			FILE *fpLeader;
			TCHAR cLineLeader[256];
			fpLeader = _tfopen( sPathLeader , _T("rb") );
			if ( fpLeader == NULL ) {
				AfxMessageBox( _T("007TAG�ڵ� ������ �����ϴ�.") );
				return TRUE;
			}
			
			//++++++++++++++++++++++++++++++++++++++++++
			//Unicode�϶� 2byte�ǳʶ��
			FILE* fpUni = fpLeader;
		
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
			BOOL bFormYnLeader = FALSE;
			while( _fgetts( cLineLeader, 256, fpLeader) ) {
				CString sLine;
				sLine.Format( _T("%s"), cLineLeader );
				sLine.Replace( sLine.Right(1), NULL );
				//[TX]ã��						
				if ( sLine == _T("[AV]") ) {
					bFormYnLeader = TRUE;
					break;
				}
			}
			if ( bFormYnLeader == FALSE ) {
				AfxMessageBox( _T("[TX]�� �����ϴ�.") );
				return true;
			}
			//(���������)... ã��
			BOOL bControlYnLeader = FALSE;
			while( _fgetts( cLineLeader, 256, fpLeader) ) {
				CString sLine;
				sLine.Format( _T("%s"), cLineLeader );
				if ( sLine.Left(1) == _T("[") ) {
					return true;
				}
				sLine.Replace( sLine.Right(1), NULL );
				if ( sLine == sValueLeader ) {
					bControlYnLeader = TRUE;
					break;
				}
			}
			//�� ��������
			while( _fgetts( cLineLeader, 256, fpLeader) ) {
				CString sLine;
				sLine.Format( _T("%s"), cLineLeader );
				//if ( sLine.Left(1) == _T(".") ) {
				if ( sLine.Left(1) == _T("(") || sLine.Left(1) == _T("[") ) {
					break;
				}
				sLine.Replace( sLine.Right(1), NULL );
				CString sCode, sDesc;
				sCode = sLine.Left(1);
				sDesc = sLine.Mid(2);			
				dlgLeader.AddData( sCode, sDesc );
			}

			if (dlgLeader.DoModal() == IDOK) {			
				CString sCode = dlgLeader.m_strSelCode;
				SetControlData( _T("CM_BO_CAT_PROC_0322"), sControlAliasLeader, sCode );
				//SetControlData(_T("CM_BO_CAT_PROC_0800"), _T("������DATA"), dlg.m_strSelCode);
			}

			return true;
		}


		//��ȣ�ֱ�
		//CWnd *pWnd;
		UINT wndCodeID[ 13 ] = {		
			IDC_cSTYLE, IDC_cUPDATE, IDC_cLIST_JUNGEO, IDC_eMODEL_MAP1,
			IDC_eMODEL_MAP2, IDC_eMODEL_MAP3, IDC_eMODEL_MAP4,
			IDC_cDRAWING, IDC_cPRIME_MERIAIAN, IDC_cMP_FROM, IDC_cMP_INDEX,
			IDC_cFORM_MARK1, IDC_cFORM_MARK2
		};				
		CString sValue;		//����, ��ǥ��...
		CString sControlAlias;
		for ( i = 0 ; i < 13 ; i++ ) {
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
						sValue = _T("(������)");
						sControlAlias = _T("������1");
						break;
					case 4 :
						sValue = _T("(������)");
						sControlAlias = _T("������2");
						break;
					case 5 :
						sValue = _T("(������)");
						sControlAlias = _T("������3");
						break;
					case 6 :
						sValue = _T("(������)");
						sControlAlias = _T("������4");
						break;
					case 7 :		
						sValue = _T("(����)");
						sControlAlias = _T("����");
						break;
					case 8 :
						sValue = _T("(�����ڿ���)");
						sControlAlias = _T("�����ڿ���");
						break;
					case 9 :
						sValue = _T("(�����ڷ�����)");
						sControlAlias = _T("�ڷ�������");
						break;
					case 10 :
						sValue = _T("(����)");
						sControlAlias = _T("����");
						break;
					case 11 :
						sValue = _T("(���»�Ư¡)");
						sControlAlias = _T("���»���Ư¡1");
						break;
					case 12 :
						sValue = _T("(���»�Ư¡)");
						sControlAlias = _T("���»���Ư¡2");
						break;
				}			
			}
		}

		if ( sValue.IsEmpty() ) {
			return FALSE;
		}

		//ȭ�� ����
		CDlgClassCode dlg(this);		
		dlg.InitData();
		//dlg.AddData( _T("a"), _T("b"));

		//�����о����
		//sPath = _T("..\\CFG\\008TAG�ڵ�.txt");

		FILE *fp;
		TCHAR cLine[256];
		fp = _tfopen( _T("..\\CFG\\008TAG�ڵ�.txt") , _T("rb") );
		if ( fp == NULL ) {
			AfxMessageBox( _T("008TAG�ڵ�.txt ������ �����ϴ�.") );
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
			if ( sLine == _T("[MP]") ) {
				bFormYn = TRUE;
				break;
			}
		}
		if ( bFormYn == FALSE ) {
			AfxMessageBox( _T("[MP]�� �����ϴ�.") );
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
			if ( sValue == _T("(����)") ) {
				CString sCode, sDesc;
				sCode = sLine.Left(2);
				sDesc = sLine.Mid(3);			
				dlg.AddData( sCode, sDesc );
			}
			else {
				CString sCode, sDesc;
				sCode = sLine.Left(1);
				sDesc = sLine.Mid(2);			
				dlg.AddData( sCode, sDesc );
			}
		}

		if (dlg.DoModal() == IDOK) {			
			CString sCode = dlg.m_strSelCode;
			SetControlData( _T("CM_BO_CAT_PROC_0322"), sControlAlias, sCode );
		}		

		return true;
	}
	else if ( pMsg->message == WM_KEYUP ) {
		//���� ��ȣ�ֱ�
		CWnd *pWnd;
		UINT wndCodeIdLeader[6] = {
			IDC_cSTATUS, IDC_cFORM, IDC_cSEOJI, IDC_cINPUT_LEVEL,
			IDC_cSTATUS2, IDC_cFORM2
		};
		CString sValueLeader;
		CString sControlAliasLeader;
		for ( INT i = 0 ; i < 6 ; i++ ) {
			pWnd = ( CWnd * )GetDlgItem( wndCodeIdLeader[ i ] );
			if ( pWnd->m_hWnd == pMsg->hwnd ) {
				CKOMarcGetterDlg dlg( this );				
				switch( i ) {
					case 0 : 						
						sValueLeader = _T("(���ڵ����)");
						sControlAliasLeader = _T("���ڵ����");
						break;
					case 1 :
						sValueLeader = _T("(���ڵ�����)");
						sControlAliasLeader = _T("���ڵ�����");
						break;
					case 2 :
						sValueLeader = _T("(��������)");
						sControlAliasLeader = _T("��������");
						break;
					case 3 :
						sValueLeader = _T("(�Է¼���)");
						sControlAliasLeader = _T("�Է¼���");
						break;
					case 4 :
						sValueLeader = _T("(��ϱ������)");
						sControlAliasLeader = _T("��ϱ������");
						break;
					case 5 :
						sValueLeader = _T("(�������ڵ�����)");
						sControlAliasLeader = _T("�������ڵ�����");
						break;
				}
			}
		}

		CString sData;
		GetControlData( _T("CM_BO_CAT_PROC_0322"), sControlAliasLeader, sData );

		INT iLeaderYes = 1;
		if ( sValueLeader.IsEmpty() ) {
			iLeaderYes = 0;
			//return FALSE;
		}
		if ( iLeaderYes == 1 ) {
			//ȭ�� ����
			/*
			CDlgClassCode dlgLeader;		
			dlgLeader.InitData();
			*/

			CString sPathLeader = _T("..\\CFG\\�����ڵ�.txt");
			//�����о����
			FILE *fpLeader;
			TCHAR cLineLeader[256];
			fpLeader = _tfopen( sPathLeader , _T("rb") );
			if ( fpLeader == NULL ) {
				AfxMessageBox( _T("007TAG�ڵ� ������ �����ϴ�.") );
				return TRUE;
			}
			
			//++++++++++++++++++++++++++++++++++++++++++
			//Unicode�϶� 2byte�ǳʶ��
			FILE* fpUni = fpLeader;
		
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
			BOOL bFormYnLeader = FALSE;
			while( _fgetts( cLineLeader, 256, fpLeader) ) {
				CString sLine;
				sLine.Format( _T("%s"), cLineLeader );
				sLine.Replace( sLine.Right(1), NULL );
				//[TX]ã��						
				if ( sLine == _T("[AV]") ) {
					bFormYnLeader = TRUE;
					break;
				}
			}
			if ( bFormYnLeader == FALSE ) {
				AfxMessageBox( _T("[TX]�� �����ϴ�.") );
				return true;
			}
			//(���������)... ã��
			BOOL bControlYnLeader = FALSE;
			while( _fgetts( cLineLeader, 256, fpLeader) ) {
				CString sLine;
				sLine.Format( _T("%s"), cLineLeader );
				if ( sLine.Left(1) == _T("[") ) {
					return true;
				}
				sLine.Replace( sLine.Right(1), NULL );
				if ( sLine == sValueLeader ) {
					bControlYnLeader = TRUE;
					break;
				}
			}
			//�� ��������
			bool bFind = FALSE;
			while( _fgetts( cLineLeader, 256, fpLeader) ) {
				CString sLine;
				sLine.Format( _T("%s"), cLineLeader );
				//if ( sLine.Left(1) == _T(".") ) {
				if ( sLine.Left(1) == _T("(") || sLine.Left(1) == _T("[") ) {
					break;
				}
				sLine.Replace( sLine.Right(1), NULL );
				CString sCode, sDesc;
				sCode = sLine.Left(1);
				//sDesc = sLine.Mid(2);			
				//dlgLeader.AddData( sCode, sDesc );
				if ( sCode == sData ) {
					bFind = true;
					break;
					//return true;
				}
			}
			if ( bFind == FALSE && sData != _T("") ) {
				AfxMessageBox ( _T("�߸��� ���Դϴ�.") );
				SetControlData( _T("CM_BO_CAT_PROC_0322"), sControlAliasLeader, _T("") );
				//return true;
				return CDialog::PreTranslateMessage(pMsg);
			}
			/*
			if (dlgLeader.DoModal() == IDOK) {			
				CString sCode = dlgLeader.m_strSelCode;
				SetControlData( _T("CM_BO_CAT_PROC_0322"), sControlAliasLeader, sCode );			
			}
			return true;
			*/
		}

		//��ȣ�ֱ�
		//CWnd *pWnd;
		UINT wndCodeID[ 13 ] = {		
			IDC_cSTYLE, IDC_cUPDATE, IDC_cLIST_JUNGEO, IDC_eMODEL_MAP1,
			IDC_eMODEL_MAP2, IDC_eMODEL_MAP3, IDC_eMODEL_MAP4,
			IDC_cDRAWING, IDC_cPRIME_MERIAIAN, IDC_cMP_FROM, IDC_cMP_INDEX,
			IDC_cFORM_MARK1, IDC_cFORM_MARK2
		};				
		CString sValue;		//����, ��ǥ��...
		CString sControlAlias;
		for ( i = 0 ; i < 13 ; i++ ) {
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
						sValue = _T("(������)");
						sControlAlias = _T("������1");
						break;
					case 4 :
						sValue = _T("(������)");
						sControlAlias = _T("������2");
						break;
					case 5 :
						sValue = _T("(������)");
						sControlAlias = _T("������3");
						break;
					case 6 :
						sValue = _T("(������)");
						sControlAlias = _T("������4");
						break;
					case 7 :		
						sValue = _T("(����)");
						sControlAlias = _T("����");
						break;
					case 8 :
						sValue = _T("(�����ڿ���)");
						sControlAlias = _T("�����ڿ���");
						break;
					case 9 :
						sValue = _T("(�����ڷ�����)");
						sControlAlias = _T("�ڷ�������");
						break;
					case 10 :
						sValue = _T("(����)");
						sControlAlias = _T("����");
						break;
					case 11 :
						sValue = _T("(���»�Ư¡)");
						sControlAlias = _T("���»���Ư¡1");
						break;
					case 12 :
						sValue = _T("(���»�Ư¡)");
						sControlAlias = _T("���»���Ư¡2");
						break;
				}			
			}
		}

		if ( sValue.IsEmpty() ) {
			return FALSE;
		}

		sData = _T("");
		GetControlData( _T("CM_BO_CAT_PROC_0322"), sControlAlias, sData );

		//ȭ�� ����
		//CDlgClassCode dlg;		
		//dlg.InitData();		

		//�����о����
		//sPath = _T("..\\CFG\\008TAG�ڵ�.txt");

		FILE *fp;
		TCHAR cLine[256];
		fp = _tfopen( _T("..\\CFG\\008TAG�ڵ�.txt") , _T("rb") );
		if ( fp == NULL ) {
			AfxMessageBox( _T("008TAG�ڵ�.txt ������ �����ϴ�.") );
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
			if ( sLine == _T("[MP]") ) {
				bFormYn = TRUE;
				break;
			}
		}
		if ( bFormYn == FALSE ) {
			AfxMessageBox( _T("[MP]�� �����ϴ�.") );
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
		bool bFind = FALSE;
		while( _fgetts( cLine, 256, fp) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			if ( sLine.Left(1) == _T(".") ) {
				break;
			}
			sLine.Replace( sLine.Right(1), NULL );
			if ( sValue == _T("(����)") ) {
				CString sCode, sDesc;
				sCode = sLine.Left(2);
				//sDesc = sLine.Mid(3);			
				//dlg.AddData( sCode, sDesc );
				if ( sCode == sData ) {
					bFind = true;
					break;
				}
			}
			else {
				CString sCode, sDesc;
				sCode = sLine.Left(1);
				//sDesc = sLine.Mid(2);			
				//dlg.AddData( sCode, sDesc );
				if ( sCode == sData ) {
					bFind = true;
					break;
				}
			}
		}
		if ( bFind == FALSE && sData != _T("") && sValue != _T("(����)") ) {
				AfxMessageBox ( _T("�߸��� ���Դϴ�.") );
				SetControlData( _T("CM_BO_CAT_PROC_0322"), sControlAlias, _T("") );
				//return true;
				return CDialog::PreTranslateMessage(pMsg);
		}
	}

	return CDialog::PreTranslateMessage(pMsg);

EFS_END
return FALSE;

}

HBRUSH CBO_CAT_PROC_0322::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}