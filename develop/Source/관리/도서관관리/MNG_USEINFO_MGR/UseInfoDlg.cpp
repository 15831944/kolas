// UseInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UseInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUseInfoDlg dialog

CUseInfoDlg::CUseInfoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUseInfoDlg)
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_bInsert = TRUE;
	AfxInitRichEdit();
	m_HtmlEditor = NULL;

	if ( FAILED(CoInitialize(NULL) ) )
	{
		AfxAbort();
	}
}

CUseInfoDlg::~CUseInfoDlg()
{
	if ( m_HtmlEditor != NULL )
	{
		delete m_HtmlEditor;
	}
	CoUninitialize();
}

VOID CUseInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUseInfoDlg)
	DDX_Control(pDX, IDC_TGroup2, m_ctrlTab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUseInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CUseInfoDlg)
	ON_CBN_SELCHANGE(IDC_cManage, OnSelchangecManage)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TGroup2, OnSelchangeTGroup2)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()     
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUseInfoDlg message handlers

BOOL CUseInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_MNG_USEINFO_MGR")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Failed!!")); 
	}
	m_pCM = FindCM(_T("CM_MNG_USEINFO_MGR"));
	m_pDM = FindDM(_T("DM_MNG_USEINFO_MGR"));
	m_ctrlTab.InsertItem(0, _T("��������"));
	m_ctrlTab.InsertItem(1, _T("�̿�ȳ�"));
	InitManage();


	CRect rect;

	m_HtmlEditor = new CDlgHtmlEdit;
	CreateHtmlEditor(m_HtmlEditor, this);
	m_HtmlEditor->ShowWindow(true);	
	SetTimer(1, 100, NULL);
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CUseInfoDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd); 
}

VOID CUseInfoDlg::OnModify()
{
	INT type = m_ctrlTab.GetCurSel();
	// 0 : ��������
	// 1 : �̿�ȳ�

	INT inputMode = m_pDM->GetRowCount() ? 0 : 1;
	// 0 : UPDATE
	// 1 : INSERT

	const INT cnt = 4;
	CString alias[ cnt ] = {
		_T("ANNOUNCEMENT"),						// ��������
		_T("ANN_UPDATE_DATE"),					// �������� ��������
		_T("FIRST_WORK"),						// �����۾�
		_T("MANAGE_CODE") };					// ��������
	if( type )
	{
		alias[ 0 ] = _T("GUIDE");				// �̿�ȳ�
		alias[ 1 ] = _T("GUI_UPDATE_DATE");		// �̿�ȳ� ��������
	}
	if( inputMode )
		alias[ 2 ] = _T("LAST_WORK");

	CString data[ cnt ];
	// m_pCM->GetControlMgrData( _T("��������"), data[ 0 ] );
	data[0] = m_HtmlEditor->GetHtml();	

	m_pCM->GetControlMgrData( _T("�����"),   data[ 1 ] );
	data[ 2 ] = GetWorkLogMsg( _T("�������̿�ȳ�"), __WFILE__, __LINE__ );
	m_pCM->GetControlMgrData( _T("��������"), data[ 3 ] );// �迭�� �߸��� ���ڰ� �� ��������(M_2004-1006_HAN...)

	m_pDM->StartFrame();
	m_pDM->InitDBFieldData();

	CString recKey;
	m_pDM->MakeRecKey( recKey );
/*
	for( INT i = 0; i < cnt; i++ )
		if( !data[ i ].IsEmpty() )
			m_pDM->AddDBFieldData( alias[ i ], _T("STRING"), data[ i ], true );
*/
	// 2003�� 12�� 23�� ������ ����
	for( INT i = 0; i < cnt; i++ )
		if( !data[ i ].IsEmpty() )
		{
			m_pDM->AddDBFieldData( alias[ i ], _T("STRING"), data[ i ], true );
			m_pDM->SetCellData( alias[i] , data[i] , 0 );
		}


	if( inputMode )
	{
		m_pDM->AddDBFieldData( _T("REC_KEY"), _T("NUMERIC"), recKey );
		m_pDM->MakeInsertFrame( _T("MN_LIB_GUIDANCE_TBL") );

		m_pDM->SetCellData( alias, cnt, data, -1 );
		m_pDM->SetCellData( _T("REC_KEY"), recKey, 0 );
	}
	else
		m_pDM->MakeUpdateFrame( _T("MN_LIB_GUIDANCE_TBL"), _T("REC_KEY"), _T("NUMERIC"), m_pDM->GetCellData( _T("REC_KEY"), 0 ) );
	m_pDM->SendFrame();
	m_pDM->EndFrame();

	if( inputMode )
		AfxMessageBox( _T("����Ǿ����ϴ�.") );
	else
		AfxMessageBox( _T("�����Ǿ����ϴ�.") );
	


	/*
	CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)m_pCM->FindControl(_T("��������"));
	if (pCMB == NULL) return;

	CArray<CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();

	CString strManage;
//	CString strDate;
	CString strInfo;
	CString query;
	CString strRecKey;
	m_pCM->GetControlMgrData(_T("��������"), strInfo);
	if(strInfo.IsEmpty())
	{
		ESLAfxMessageBox(_T("������ �Է��� �ֽʽÿ�."));
		return ;
	}
	

	if(m_pDM->GetRowCount() == 0)
		m_bInsert = TRUE;
	else 
		m_bInsert = FALSE;


	INT num = m_ctrlTab.GetCurSel(); 
	CTime time = m_pDM->GetDBTime();
	CString date;
	date.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );
	CString workLog = GetWorkLogMsg( _T("�������̿�ȳ�"), __WFILE__, __LINE__ );
	if (num == 0)
	{	
		m_pCM->GetControlMgrData(_T("��������"), strManage);
//		m_pCM->GetControlMgrData(_T("�����"), strDate);
		m_pCM->GetControlMgrData(_T("��������"), strInfo);
	
		// �����Ͱ� ���� ��� �Է��ϴ� �κ�. 	m_bInsert�� pDM->GetRowCount() = 0 �ϰ�� TRUE	
		if(m_bInsert)   
		{
			m_pDM->InsertRow(-1);
			m_pDM->MakeRecKey(strRecKey);
			m_pDM->SetCellData(_T("REC_KEY"), strRecKey, 0);
			m_pDM->SetCellData(_T("FIRST_WORK"), this->GetWorkLogMsg( _T("����������"), __WFILE__, __LINE__ ), 0);
		}
		else
			strRecKey = m_pDM->GetCellData(_T("REC_KEY"), 0);
		m_pDM->SetCellData(_T("MANAGE_CODE"), strManage, 0);
//		m_pDM->SetCellData(_T("ANN_UPDATE_DATE"), strDate, 0);
		m_pDM->SetCellData(_T("ANNOUNCEMENT"), strInfo, 0);
		m_pDM->SetCellData(_T("LAST_WORK"), this->GetWorkLogMsg( _T("����������"), __WFILE__, __LINE__ ), 0);

		m_pDM->StartFrame(); 
				
		m_pDM->AddDBFieldData( _T("MANAGE_CODE"), _T("STRING"), strManage );
		m_pDM->AddDBFieldData( _T("REC_KEY"), _T("STRING"), strInfo );
		m_pDM->AddDBFieldData( _T("ANNOUNCEMENT"), _T("STRING"), strInfo );
		m_pDM->AddDBFieldData( _T("ANN_UPDATE_DATE"), _T("STRING"), date );

		if( m_bInsert )
		{
			m_pDM->AddDBFieldData( _T("REC_KEY"), _T("NUMERIC"), _T("ESL_SEQ.NEXTVAL") );
			m_pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), workLog );
			m_pDM->MakeInsertFrame( _T("MN_LIB_GUIDANCE_TBL") );
		}
		else
		{
			m_pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), workLog );
			m_pDM->MakeUpdateFrame( _T("MN_LIB_GUIDANCE_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey );
		}
		
		
		//if(m_bInsert)
			//m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, m_pDM->TBL_NAME, 0, m_pDM);
		//else
			//m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, m_pDM->TBL_NAME, 0, m_pDM, _T("REC_KEY"), _T("NUMERIC"), strRecKey);
		
	}
	else if (num == 1)
	{
		m_pCM->GetControlMgrData(_T("��������"), strManage);
//		m_pCM->GetControlMgrData(_T("�����"), strDate);
		m_pCM->GetControlMgrData(_T("��������"), strInfo);
		
        // �����Ͱ� ���� ��� �Է��ϴ� �κ� 
		if(m_bInsert)
		{
			m_pDM->InsertRow(-1);
			m_pDM->MakeRecKey(strRecKey);
			m_pDM->SetCellData(_T("REC_KEY"), strRecKey, 0);
			m_pDM->SetCellData(_T("FIRST_WORK"), this->GetWorkLogMsg( _T("����������"), __WFILE__, __LINE__ ), 0);
		}
		else 
			strRecKey = m_pDM->GetCellData(_T("REC_KEY"), 0);
		m_pDM->SetCellData(_T("MANAGE_CODE"), strManage, 0);
//		m_pDM->SetCellData(_T("GUI_UPDATE_DATE"), strDate, 0);
		m_pDM->SetCellData(_T("GUIDE"), strInfo, 0);
		m_pDM->SetCellData(_T("LAST_WORK"), this->GetWorkLogMsg( _T("����������"), __WFILE__, __LINE__ ), 0);

		m_pDM->StartFrame(); 
		
		m_pDM->AddDBFieldData( _T("MANAGE_CODE"), _T("STRING"), strManage );
		m_pDM->AddDBFieldData( _T("REC_KEY"), _T("STRING"), strInfo );
		m_pDM->AddDBFieldData( _T("GUIDE"), _T("STRING"), strInfo );
		m_pDM->AddDBFieldData( _T("GUI_UPDATE_DATE"), _T("STRING"), date );

		if( m_bInsert )
		{
			m_pDM->AddDBFieldData( _T("REC_KEY"), _T("NUMERIC"), _T("ESL_SEQ.NEXTVAL") );
			m_pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), workLog );
			m_pDM->MakeInsertFrame( _T("MN_LIB_GUIDANCE_TBL") );
		}
		else
		{
			m_pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), workLog );
			m_pDM->MakeUpdateFrame( _T("MN_LIB_GUIDANCE_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey );
		}
		
		
		//if(m_bInsert)
			//m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, m_pDM->TBL_NAME, 0, m_pDM);
		//else
			//m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, m_pDM->TBL_NAME, 0, m_pDM, _T("REC_KEY"), _T("NUMERIC"), strRecKey);	
		
	}

	INT ids = m_pDM->SendFrame();
	if(ids < 0) 
		ESLAfxMessageBox(_T("���泻���� ������ �� �����ϴ�."));
	ESLAfxMessageBox(_T("���泻���� �����Ͽ����ϴ�."));
	return;
	Display();
	*/
}

VOID CUseInfoDlg::OnSelchangecManage() 
{
	CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)m_pCM->FindControl(_T("��������"));
	if (pCMB == NULL) return;

	CString strManage;
	m_pCM->GetControlMgrData(_T("��������"), strManage);
	if (strManage.IsEmpty()) 
	{
		pCMB->SetCurSel(m_nMANAGE_CODE);
		AfxMessageBox(_T("[�������к�����������]�� 'Y'�϶��� ��������� ������ �� �����ϴ�."));

		m_pCM->SetControlMgrData(_T("��������"), m_pDM->GetCellData(_T("MANAGE_CODE"), 0));
//		return;
	}
	CString strManageCode;
	m_pCM->GetControlMgrData(_T("��������"), strManageCode);
	RefreshDM(strManageCode);
	Display();
}

VOID CUseInfoDlg::InitManage()
{
	//++2008.10.22 UPDATE BY PWR {{++
	// ���սý��� ����
	m_cUseMANAGE_CODE = 'Y';
	m_strUSER_MANAGE_CODE = m_pInfo->MANAGE_CODE;
//	CString strTmp, strQuery;
//	strTmp = _T("");
//	strQuery.Format(_T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE GROUP_1='L' AND GROUP_2='��Ÿ' AND GROUP_3='����' AND CLASS_ALIAS='�������к�����������' "));
//	m_pDM->GetOneValueQuery(strQuery, strTmp);
//	strTmp.TrimLeft();				strTmp.TrimRight();
//	strTmp.MakeUpper();
//	if (strTmp == _T("N")) 
//		m_cUseMANAGE_CODE = 'N';
//	else 
//		m_cUseMANAGE_CODE = 'Y';   // m_cUseMANAGE_CODE : �������к����������� Y/N
//
//	strQuery.Format(_T("SELECT MANAGE_CODE FROM MN_USER_TBL WHERE ID='%s'"), GetUserID());
//	m_pDM->GetOneValueQuery(strQuery, m_strUSER_MANAGE_CODE);    // m_strUSER_MANAGE_CODE : �������к������� 
//	m_strUSER_MANAGE_CODE.TrimLeft();			m_strUSER_MANAGE_CODE.TrimRight();
	//--2008.10.22 UPDATE BY PWR --}}

	// ���������� �����´�.
	CString strManageCode;
	m_pCM->GetControlMgrData(_T("��������"), strManageCode);   // strManageCode : �������к���������ȣ
	RefreshDM(strManageCode);
	Display();	
}  

VOID CUseInfoDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CStatic *pStatic;

	pStatic = ( CStatic * )GetDlgItem( IDC_SGroup2 );
	if( NULL == pStatic->GetSafeHwnd() )
		return;
	else
		pStatic->MoveWindow( 9, 3, cx- 16, 45 );

	pStatic = ( CStatic * )GetDlgItem( IDC_TGroup2 );
	if( NULL == pStatic->GetSafeHwnd() )
		return;
	else
		pStatic->MoveWindow( 9, 53, cx - 16, cy - 60);

	CRect rect;

	rect.left = 17;
	rect.top = 80;
	rect.right = cx-17;
	rect.bottom = cy-17;
	
	if ( m_HtmlEditor->GetSafeHwnd() != NULL ) m_HtmlEditor->MoveWindow(rect);	
}

INT CUseInfoDlg::RefreshDM(CString strManageCode)
{
	CString strWhere;
	if(m_cUseMANAGE_CODE == _T("Y"))
	{
		strWhere.Format(_T("MANAGE_CODE = '%s'"), strManageCode);
	}
	else
	{
		strWhere.Format(_T("MANAGE_CODE IS NULL"));
	}

	if(m_pDM == NULL) return -1;
	INT ids = m_pDM->RefreshDataManager(strWhere);
	if(ids < 0) return -1;
	
	CString strCheck;
//	m_pDM->GetCellData(_T("��������"), strCheck);
//	if (strCheck.IsEmpty())
	if(m_pDM->GetRowCount() == 0)
		m_bInsert = TRUE;
	else 
		m_bInsert = FALSE;
	return 0;
}

VOID CUseInfoDlg::Display()
{	
	if ( m_HtmlEditor == NULL ) return;

	if(m_cUseMANAGE_CODE == _T("Y"))
		GetDlgItem(IDC_cManage)->EnableWindow(TRUE);
	else 
	{
		GetDlgItem(IDC_cManage)->EnableWindow(FALSE);
		m_pCM->SetControlMgrData(_T("��������"), _T(""));
	}
	CString strDate; 
	CTime t = m_pDM->GetDBTime();
	strDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	CString strHtml;

	INT nCurSel = m_ctrlTab.GetCurSel();
	if(m_bInsert) 
	{
		if(nCurSel == 0)
		{
			//m_pCM->SetControlMgrData(_T("��������"), m_pDM->GetCellData(_T("ANNOUNCEMENT"), 0));
			strHtml = m_pDM->GetCellData(_T("ANNOUNCEMENT"), 0);
			if ( m_HtmlEditor->GetSafeHwnd() != NULL )
			{
				if(_T("") == strHtml) strHtml = _T("<HTML></HTML>");
				m_HtmlEditor->SetHtml(strHtml);
			}
			m_pCM->SetControlMgrData(_T("�����"), strDate, 0);			
		}
		else
		{
			//m_pCM->SetControlMgrData(_T("��������"), m_pDM->GetCellData(_T("GUIDE"), 0));
			strHtml = m_pDM->GetCellData(_T("GUIDE"), 0);
			if ( m_HtmlEditor->GetSafeHwnd() != NULL )
			{
				if(_T("") == strHtml) strHtml = _T("<HTML></HTML>");
				m_HtmlEditor->SetHtml(strHtml);
			}
			m_pCM->SetControlMgrData(_T("�����"), strDate, 0);			
		}
		return;
	}
	if(nCurSel == 0)
	{
		//m_pCM->SetControlMgrData(_T("��������"), m_pDM->GetCellData(_T("ANNOUNCEMENT"), 0));
		strHtml = m_pDM->GetCellData(_T("ANNOUNCEMENT"), 0);
		if ( m_HtmlEditor->GetSafeHwnd() != NULL )
		{
			if(_T("") == strHtml) strHtml = _T("<HTML></HTML>");
			m_HtmlEditor->SetHtml(strHtml);
		}
		m_pCM->SetControlMgrData(_T("�����"), strDate, 0);
		m_pCM->SetControlMgrData(_T("��������"), m_pDM->GetCellData(_T("MANAGE_CODE"), 0));
	}
	else
	{
		//m_pCM->SetControlMgrData(_T("��������"), m_pDM->GetCellData(_T("GUIDE"), 0));
		strHtml = m_pDM->GetCellData(_T("GUIDE"), 0);
		if ( m_HtmlEditor->GetSafeHwnd() != NULL )
		{
			if(_T("") == strHtml) strHtml = _T("<HTML></HTML>");
			m_HtmlEditor->SetHtml(strHtml);
		}
		m_pCM->SetControlMgrData(_T("�����"), strDate, 0);
		m_pCM->SetControlMgrData(_T("��������"), m_pDM->GetCellData(_T("MANAGE_CODE"), 0));
	}
}	

VOID CUseInfoDlg::OnSelchangeTGroup2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	Display();
	*pResult = 0;
}


void CUseInfoDlg::OnInsertObject()
{
	BeginWaitCursor();	
		// Word document�� ���� Ŭ���� ID�� ��´�.
		// �����Ҷ� ���ȴ�. 

		//if(FAILED(::CLSIDFromProgID(OLESTR("ecom.HtmlEditorControl"),&clsid)))        		

		/*
		// Word �� ���Ե� item �����ϱ�
		if(!pItem->CreateNewItem(clsid))
		AfxThrowMemoryException(); // � ���ܰ� �߻��ϸ� TRY ���� Ż���ϱ⸸ �ϸ� �ȴ�.

		// ���ο� CContainerItem�� ��ȿ�� ���� Ȯ���϶�.
		ASSERT_VALID(pItem);

		// �������� �����ϱ� ���� ������ �����Ѵ�.
		pItem->DoVerb(OLEIVERB_SHOW, this);

		// ������ UI ������ ó�� ���������� ���Ե� �������� �����ϵ��� �����Ѵ�.
		m_pSelection = pItem;  // ���������� ���Ե� �������� �����ϵ��� �����Ѵ�.

		pDoc->UpdateAllViews(NULL);

		// embedded object�� ���� �����͸� diapatch�ϴ� ���Ǹ� �Ѵ�. �̰��, Word
		// document�̴�.
		LPDISPATCH lpDisp;
		lpDisp = pItem->GetDispatch();
		
		// lpDisp�� ����ϵ��� _Document doc�� �����Ѵ�, ���� document�� IDispatch*		
		m_html.AttachDispatch(lpDisp);
		m_html.setHtmlDoc(_T("Test html...."));	
		m_html.SetLeft(100);
		m_html.SetTop(100);
		m_html.SetWidth(400);
		m_html.SetHeight(400);		
	}  
	CATCH(CException, e)// ���ܰ� �߻��ϸ� clean up�� ���ش�.
	{
		if (pItem != NULL) {
			ASSERT_VALID(pItem);
			pItem->Delete();
		}
		AfxMessageBox(IDP_FAILED_TO_CREATE);
	}
	END_CATCH
	*/

	// �۾��� �����ٴ� ���� ����ڰ� �� �� �ֵ��� Ŀ���� ���� ���·� �����Ѵ�
	EndWaitCursor();
}

void CUseInfoDlg::OnTimer(UINT nIDEvent) 
{
	if ( nIDEvent == 1 )
	{
		KillTimer(1);
		Display();
	}
	
	CDialog::OnTimer(nIDEvent);
}

HBRUSH CUseInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

