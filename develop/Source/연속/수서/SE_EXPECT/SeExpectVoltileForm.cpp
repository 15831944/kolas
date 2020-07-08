// SeExpectVoltileForm.cpp : implementation file
//

#include "stdafx.h"
#include "SeExpectVoltileForm.h"
#include "SeExpectGetPatternName.h"
#include "SeExpectPatternDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeExpectVoltileForm dialog


CSeExpectVoltileForm::CSeExpectVoltileForm(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeExpectVoltileForm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeExpectVoltileForm)
	m_bEXP_USE_PUB_DAY = FALSE;
	//}}AFX_DATA_INIT
	m_nOpenMode = 1;
	m_strFORMAT_TYPE = _T("1");
	m_pCM = NULL; 
	m_pDM = NULL;
	m_pDM_PATTERN = NULL;
	m_pGrid = NULL;
	m_pGridPattern = NULL;
	m_pEditGrid = NULL;
	m_pComboLevelSign = NULL;
	m_strACQ_KEY = _T("");
	m_bIsLevelSignFocused = FALSE;
	m_bTabUse = NULL;
	m_strPubDatePattern = _T("01");
	m_pComboPubDatePattern = NULL;

	m_bIsFirstCreated = TRUE;
	m_nCurrentIdx = -1;
}

CSeExpectVoltileForm::~CSeExpectVoltileForm()
{
	FreeGridEdit();
}

VOID CSeExpectVoltileForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeExpectVoltileForm)
	DDX_Check(pDX, IDC_chkEXP_USE_PUB_DAY_YN, m_bEXP_USE_PUB_DAY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeExpectVoltileForm, CDialog)
	//{{AFX_MSG_MAP(CSeExpectVoltileForm)
	ON_BN_CLICKED(IDC_btnEXP_LINE_INSERT, OnbtnEXPLINEINSERT)
	ON_BN_CLICKED(IDC_btnEXP_LINE_DELETE, OnbtnEXPLINEDELETE)
	ON_CBN_KILLFOCUS(IDC_cmbEXP_LEVEL_SIGN, OnKillfocuscmbEXPLEVELSIGN)
	ON_BN_CLICKED(IDC_chkEXP_USE_PUB_DAY_YN, OnchkEXPUSEPUBDAYYN)
	ON_BN_CLICKED(IDC_btnEXP_PATTERN_INSERT, OnbtnEXPPATTERNINSERT)
	ON_BN_CLICKED(IDC_btnEXP_PATTERN_SEARCH, OnbtnEXPPATTERNSEARCH)
	ON_BN_CLICKED(IDC_btnEXP_VOL_CLEAR_ALL, OnbtnEXPVOLCLEARALL)
	ON_BN_CLICKED(IDC_btnEXP_PATTERN_MANAGE, OnbtnEXPPATTERNMANAGE)
	ON_CBN_SELCHANGE(IDC_cmbEXP_PUB_PATTERN, OnSelchangecmbEXPPUBPATTERN)
	ON_MESSAGE(UPDATE_GRID_EDIT, OnUpdateGridText)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeExpectVoltileForm message handlers
BOOL CSeExpectVoltileForm::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}

BOOL CSeExpectVoltileForm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("����_����_����_��ȣ"))<0){
		AfxMessageBox( _T("InitESL_Mgr Error : ����_����_����_��ȣ") );
		return false;
	}

	m_pCM = FindCM(_T("CM_����_����_��ȣ"));
	
	CESL_Grid* pGrid;
	if(_T("1") == m_strFORMAT_TYPE) { //��ȣ

		m_pDM = FindDM(_T("DM_����_����_��ȣ"));
		m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("��ȣ�׸���"));
		pGrid = (CESL_Grid*)m_pCM->FindControl(_T("��������ȣ�׸���"));

		
	} else if(_T("2") == m_strFORMAT_TYPE) { //������ ��ȣ

		m_pDM = FindDM(_T("DM_����_����_��������ȣ"));
		m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("��������ȣ�׸���"));
		pGrid = (CESL_Grid*)m_pCM->FindControl(_T("��ȣ�׸���"));
	}
	
	m_pDM_PATTERN = FindDM(_T("DM_����_����_��ȣ����"));

	pGrid->ShowWindow(SW_HIDE);

	if(!m_pCM || !m_pDM || !m_pDM_PATTERN ) {
		AfxMessageBox( _T("CM,DM Error!") );
		return false;
	}

	m_pGridPattern = (CESL_Grid*)m_pCM->FindControl(_T("���ϱ׸���"));
	m_pComboLevelSign = (CComboBox*)GetDlgItem(IDC_cmbEXP_LEVEL_SIGN);
	m_pComboPubDatePattern = (CComboBox*)GetDlgItem(IDC_cmbEXP_PUB_PATTERN);
	InitLevelSignCombo();
	if(InitOpenMode()<0) return FALSE;

	ShowGrid();
	OnbtnEXPPATTERNSEARCH();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSeExpectVoltileForm::InitLevelSignCombo()
{
	
	m_pComboLevelSign->InsertString(0, _T(""));
	INT nCodeCount = 0;
	
	nCodeCount = m_pInfo->pCodeMgr->GetCodeCount( _T("�ܰ�ĺ���") );
	INT startrow = 0;
	CString code[ 6 ];
	for( INT i = 0; i < nCodeCount; i++ ) 
	{
		startrow = m_pInfo->pCodeMgr->GetCode( _T("�ܰ�ĺ���"), code, 6, startrow );
		startrow++;	

		m_pComboLevelSign->InsertString( i + 1, code[ 3 ] );
	}
	
	nCodeCount = 0;

	nCodeCount = m_pInfo->pCodeMgr->GetCodeCount( _T("�����Ͽ�������") );
	startrow = 0;
	CString code1[ 6 ];
	for( i = 0; i < nCodeCount; i++ ) 
	{
		startrow = m_pInfo->pCodeMgr->GetCode( _T("�����Ͽ�������"), code1, 6, startrow );
		startrow++;	

		m_pComboPubDatePattern->InsertString( i , code1[ 3 ] );
	}

	INT nCount = m_pComboLevelSign->GetCount();

	m_pComboLevelSign->ShowWindow(SW_HIDE);
	m_pComboPubDatePattern->SetCurSel(0);
	m_pComboPubDatePattern->ShowWindow(SW_HIDE);
}

INT CSeExpectVoltileForm::InitOpenMode()
{
	CString strPubPattern;
	if(-2 == m_nOpenMode) //�Ϲݼ���
	{

		if(m_strACQ_KEY.IsEmpty())
		{
			AfxMessageBox(_T("[���Ա���KEY]�� ã�� �� �����ϴ�"));
			return -1;
		}
		
		CString strWhere;	
		strWhere.Format(_T("ACQ_KEY = %s AND FORMAT_TYPE = %s"),m_strACQ_KEY,m_strFORMAT_TYPE);
		m_pDM->RefreshDataManager(strWhere);

		//������ ���� ���� ��� ���� 
		if(1 == m_pDM->GetRowCount())
		{
			CString strEXP_LEVEL_NAME = _T("");
			strEXP_LEVEL_NAME =	m_pDM->GetCellData(_T("�ܰ��"),0);
			strEXP_LEVEL_NAME.TrimLeft();
			strEXP_LEVEL_NAME.TrimRight();
			
			if(_T("<������>") == strEXP_LEVEL_NAME)
			{
				m_bEXP_USE_PUB_DAY = TRUE;
				m_pDM->GetCellData(_T("�����Ͽ�������"),0,strPubPattern);
				
				if(strPubPattern == _T(""))
				{
					m_strPubDatePattern = _T("01");
				}
				else
				{
					m_strPubDatePattern = strPubPattern;
					INT nCode = _ttoi(strPubPattern);
					if(nCode > 1)
						m_pComboPubDatePattern->SetCurSel(nCode-1);
				}

				m_pGrid->EnableWindow(FALSE);
				m_pComboPubDatePattern->ShowWindow(SW_SHOW);
				UpdateData(FALSE);
			}

		}
		else
		{	
			for(INT i = 1; i< m_pDM->GetRowCount(); i++)
			{
				CString strINC_UNIT = m_pDM->GetCellData(_T("�ܰ�������"), i);
				CString strINC_LEVEL = m_pDM->GetCellData(_T("�ܰ���������"), i);
				if( strINC_LEVEL.IsEmpty() )
				{
					m_pDM->SetCellData(_T("UDF_�ܰ���������"),strINC_UNIT,i);
				}
				else
				{
					CString sLevel = strINC_UNIT;
					sLevel += _T(":");
					sLevel += strINC_LEVEL;
					m_pDM->SetCellData(_T("UDF_�ܰ���������"),sLevel,i);
				}


			}
		}
		//ȭ�� ����� ��ư ���߱� 
		GetDlgItem(IDC_btnEXP_VOL_CLEAR_ALL)->ShowWindow(SW_HIDE);

	}
	else if(-1 == m_nOpenMode) //��ȭ�� ����
	{
	
		if(m_bIsFirstCreated) //ó���ߴ� ���
		{
			if(m_strACQ_KEY.IsEmpty())
			{
				AfxMessageBox(_T("[���Ա���KEY]�� ã�� �� �����ϴ�"));
				return -1;
			}
			
			CString strWhere;	
			strWhere.Format(_T("ACQ_KEY = %s AND FORMAT_TYPE = %s"),m_strACQ_KEY,m_strFORMAT_TYPE);
			m_pDM->RefreshDataManager(strWhere);
			

			//������ ���� ���� ��� ���� 
			if(1 == m_pDM->GetRowCount())
			{
				CString strEXP_LEVEL_NAME = _T("");
				strEXP_LEVEL_NAME =	m_pDM->GetCellData(_T("�ܰ��"),0);
				strEXP_LEVEL_NAME.TrimLeft();
				strEXP_LEVEL_NAME.TrimRight();
				
				if(_T("<������>") == strEXP_LEVEL_NAME)
				{
					m_bEXP_USE_PUB_DAY = TRUE;
					m_pDM->GetCellData(_T("�����Ͽ�������"),0,strPubPattern);
					if(strPubPattern == _T(""))
					{
						m_strPubDatePattern = _T("01");
					}
					else
					{
						m_strPubDatePattern = strPubPattern;
						INT nCode = _ttoi(strPubPattern);
						if(nCode > 1)
							m_pComboPubDatePattern->SetCurSel(nCode-1);
					}
					m_pComboPubDatePattern->ShowWindow(SW_SHOW);
					m_pGrid->EnableWindow(FALSE);
					UpdateData(FALSE);
				}

			}
			else
			{	
				for(INT i = 1; i< m_pDM->GetRowCount(); i++)
				{
					CString strINC_UNIT = m_pDM->GetCellData(_T("�ܰ�������"), i);
					CString strINC_LEVEL = m_pDM->GetCellData(_T("�ܰ���������"), i);
					if( strINC_LEVEL.IsEmpty() )
					{
						m_pDM->SetCellData(_T("UDF_�ܰ���������"),strINC_UNIT,i);
					}
					else
					{
						CString sLevel = strINC_UNIT;
						sLevel += _T(":");
						sLevel += strINC_LEVEL;
						m_pDM->SetCellData(_T("UDF_�ܰ���������"),sLevel,i);
					}
					
					
				}
			}

			//ȭ�� ����� ��ư ���߱� 
			GetDlgItem(IDC_btnEXP_VOL_CLEAR_ALL)->ShowWindow(SW_HIDE);

		}
		else //�ι�° ���� �ߴ� ���
		{

			//������ ���� ���� ��� ���� 
			if(1 == m_pDM->GetRowCount())
			{
				CString strEXP_LEVEL_NAME = _T("");
				strEXP_LEVEL_NAME =	m_pDM->GetCellData(_T("�ܰ��"),0);
				strEXP_LEVEL_NAME.TrimLeft();
				strEXP_LEVEL_NAME.TrimRight();
				
				if(_T("<������>") == strEXP_LEVEL_NAME)
				{
					m_bEXP_USE_PUB_DAY = TRUE;
					m_pGrid->EnableWindow(FALSE);
					m_pDM->GetCellData(_T("�����Ͽ�������"),0,strPubPattern);
					if(strPubPattern == _T(""))
					{
						m_strPubDatePattern = _T("01");
					}
					else
					{
						m_strPubDatePattern = strPubPattern;
						INT nCode = _ttoi(strPubPattern);
						if(nCode > 1)
							m_pComboPubDatePattern->SetCurSel(nCode-1);
					}
					m_pComboPubDatePattern->ShowWindow(SW_SHOW);
					UpdateData(FALSE);
				}

			}
			else
			{	
				for(INT i = 1; i< m_pDM->GetRowCount(); i++)
				{
					CString strINC_UNIT = m_pDM->GetCellData(_T("�ܰ�������"), i);
					CString strINC_LEVEL = m_pDM->GetCellData(_T("�ܰ���������"), i);
					if( strINC_LEVEL.IsEmpty() )
					{
						m_pDM->SetCellData(_T("UDF_�ܰ���������"),strINC_UNIT,i);
					}
					else
					{
						CString sLevel = strINC_UNIT;
						sLevel += _T(":");
						sLevel += strINC_LEVEL;
						m_pDM->SetCellData(_T("UDF_�ܰ���������"),sLevel,i);
					}
					
					
				}
			}

			//ȭ�� ����� ��ư ���߱� 
			GetDlgItem(IDC_btnEXP_VOL_CLEAR_ALL)->ShowWindow(SW_HIDE);

		}


	}
	

	/*
	if(m_nOpenMode<0) //���� 
	{
		if(m_strACQ_KEY.IsEmpty())
		{
			AfxMessageBox(_T("[���Ա���KEY]�� ã�� �� �����ϴ�"));
			return -1;
		}
		
		CString strWhere;	
		strWhere.Format(_T("ACQ_KEY = %s AND FORMAT_TYPE = %s"),m_strACQ_KEY,m_strFORMAT_TYPE);
		m_pDM->RefreshDataManager(strWhere);
		

		//������ ���� ���� ��� ���� 
		if(1 == m_pDM->GetRowCount())
		{
			CString strEXP_LEVEL_NAME = _T("");
			strEXP_LEVEL_NAME =	m_pDM->GetCellData(_T("�ܰ��"),0);
			strEXP_LEVEL_NAME.TrimLeft();
			strEXP_LEVEL_NAME.TrimRight();
			
			if(_T("<������>") == strEXP_LEVEL_NAME)
			{
				m_bEXP_USE_PUB_DAY = TRUE;
				m_pGrid->EnableWindow(FALSE);
				UpdateData(FALSE);
			}

		}

		//ȭ�� ����� ��ư ���߱� 
		GetDlgItem(IDC_btnEXP_VOL_CLEAR_ALL)->ShowWindow(SW_HIDE);

	}
	*/
	

	return 0;
}


BEGIN_EVENTSINK_MAP(CSeExpectVoltileForm, CDialog)
    //{{AFX_EVENTSINK_MAP(CSeExpectVoltileForm)
	ON_EVENT(CSeExpectVoltileForm, IDC_gridEXP_VOLTITLE, -600 /* Click */, OnClickgridEXPVOLTITLE, VTS_NONE)
	ON_EVENT(CSeExpectVoltileForm, IDC_gridEXP_PATTERN_SEARCH, -601 /* DblClick */, OnDblClickgridEXPPATTERNSEARCH, VTS_NONE)
	ON_EVENT(CSeExpectVoltileForm, IDC_gridEXP_VOLTITLE, -601 /* DblClick */, OnDblClickgridEXPVOLTITLE, VTS_NONE)
	ON_EVENT(CSeExpectVoltileForm, IDC_gridEXP_SUPPORT_VOLTITLE, -601 /* DblClick */, OnDblClickgridEXPSUPPORTVOLTITLE, VTS_NONE)
	ON_EVENT(CSeExpectVoltileForm, IDC_gridEXP_SUPPORT_VOLTITLE, -600 /* Click */, OnClickgridEXPSUPPORTVOLTITLE, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSeExpectVoltileForm::MakeSubControl()
{
	if(0 == m_pDM->GetRowCount()) return; 

	INT nRow = m_pGrid->GetRow();
	INT nCol = m_pGrid->GetCol();
	
	CString tmp;
	tmp = m_pGrid->GetTextMatrix(nRow,nCol);
	m_pGrid->SetReverse(nRow-1);

	switch(nCol)
	{
	case 1:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
		CreateGridEdit(nRow,nCol,tmp);
		break;
	case 2:
		SetLevelSign(nRow,nCol,tmp);
		break;
	default:
		break;
	}

}

VOID CSeExpectVoltileForm::OnClickgridEXPVOLTITLE() 
{
	// TODO: Add your control notification handler code here
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		
		if(m_pDM->GetRowCount()>0)
		{
			  m_nCurrentIdx = row-1;
		}
	}	
}

VOID CSeExpectVoltileForm::OnDblClickgridEXPVOLTITLE() 
{
	// TODO: Add your control notification handler code here
	MakeSubControl();
	
}

VOID CSeExpectVoltileForm::OnDblClickgridEXPSUPPORTVOLTITLE() 
{
	// TODO: Add your control notification handler code here
	MakeSubControl();
}


VOID CSeExpectVoltileForm::OnDblClickgridEXPPATTERNSEARCH() 
{
	// TODO: Add your control notification handler code here
	//���ϸ��� �������� ��ȣ ���� ������ ������ �ش�

	CESL_Grid* pGrid = (CESL_Grid*)m_pCM->FindControl(_T("���ϱ׸���"));
	INT nRow = pGrid->GetRow();
	
	if(nRow>0)
	{
		

		CESL_DataMgr* pDM = FindDM(_T("DM_����_����_�����̸�"));
		if(!pDM) {
			AfxMessageBox(_T("���� �̸� ������ ã�� �� �����ϴ�!"));
		}
		

		CString strPATTERN_NAME,tmp;
		strPATTERN_NAME = pDM->GetCellData(_T("UDF_�����̸�"),nRow-1);
		BOOL Isfreed = FALSE;
		INT nRow;

		for(INT i=0;i<m_pDM_PATTERN->GetRowCount();i++)
		{
			tmp = m_pDM_PATTERN->GetCellData(_T("�����̸�"),i);
		
			if(tmp == strPATTERN_NAME)
			{
				if(!Isfreed)
				{

					if(m_nOpenMode<0)
					{
						INT nRowCnt = m_pDM->GetRowCount();
						CString strREC_KEY;
						for(INT i=0;i<nRowCnt;i++)
						{
							strREC_KEY = m_pDM->GetCellData(_T("��ȣ����KEY"),nRowCnt-1-i);
							if(!strREC_KEY.IsEmpty()) m_arrayDeleteKeyList.Add(strREC_KEY);
						}
					}

					m_pDM->FreeData();
					Isfreed = TRUE;
				}

				m_pDM->InsertRow(-1);
				nRow = m_pDM->GetRowCount()-1;
				CopyDMToDMByAlias(m_pDM_PATTERN,i,m_pDM,nRow);	
				m_pDM->SetCellData(_T("�����÷���"),_T("I"),nRow);
				m_pDM->SetCellData(_T("������ȣ"),m_strFORMAT_TYPE,nRow);

			}

			m_bEXP_USE_PUB_DAY = FALSE;
		}
	}
	
	UpdateData(FALSE);
	ShowGrid();
	
}

VOID CSeExpectVoltileForm::OnUpdateGridText(WPARAM w, LPARAM l)
{
	
	CString tmp;
	INT row,col;
	BOOL bTab = FALSE;
	
	row = m_pEditGrid->m_nRow;
	col = m_pEditGrid->m_nCol;
	bTab = m_pEditGrid->m_bTab;


	if(w) //���Ϳ� ��Ű�� ������� ��� 
	{
		INT nCurRow = m_pGrid->GetRow();
		INT nCurCol = m_pGrid->GetCol();
		
		m_pEditGrid->GetWindowText(tmp);
		
		m_pGrid->SetRow(row);
		m_pGrid->SetCol(col);
		m_pGrid->SetText(tmp);
		
		m_pGrid->SetRow(nCurRow);
		m_pGrid->SetCol(nCurCol);

		CString strAlias = _T("");

		switch(col)
		{
		case 1:
			strAlias = _T("�ܰ��"); 
			break;
		case 3:
			strAlias = _T("���ۼ�"); 
			break;
		case 4:
			strAlias = _T("����"); 
			break;
		case 5:
			//strAlias = _T("�ܰ�������"); 
			strAlias = _T("UDF_�ܰ���������"); 
			break;
		case 6:
			strAlias = _T("��������"); 
			break;
		case 7:
			strAlias = _T("�������ۼ�"); 
			break;
		case 8:
			strAlias = _T("����"); 
			break;
		default:
			break;

		}

		m_pDM->SetCellData(strAlias,tmp,row-1);
	}
	
	FreeGridEdit();
		
	if(bTab)  //��Ű�� ��������� ������ ���ο� ����Ʈ ���� ����� �ش�
	{
		MakeNextSubControl(row, col);
	}

}

VOID CSeExpectVoltileForm::MakeNextSubControl(INT row, INT col)
{
	INT nRowCnt = m_pDM->GetRowCount();

	if(0 == nRowCnt) return; 

	if(col<7)
	{
		col++;
	}
	else if(7 == col) //������ �÷� 
	{
		row++;
		col = 1;
		if(row > nRowCnt) return;
	}
	
	m_pGrid->SetCol(col);
	m_pGrid->SetRow(row);

	MakeSubControl();

}

VOID CSeExpectVoltileForm::CreateGridEdit(INT row, INT col, CString strInitValue)
{
	FreeGridEdit();
	m_pEditGrid = new CEditGrid;
	
	m_pEditGrid->m_nRow = row;
	m_pEditGrid->m_nCol = col;

	CWnd *pWnd = ( CWnd * )m_pGrid;

	m_pEditGrid->parent = ( CWnd * )this;
	
	m_pEditGrid->Create( ES_LEFT|ES_MULTILINE|ES_AUTOVSCROLL|WS_CHILD, CRect( 0 , 0, 10, 20 ), pWnd->GetParent(), 645 );
	
	CRect rect = GetGridCellRect();
	m_pEditGrid->MoveWindow( rect.left - 1, rect.top - 1, rect.Width(), rect.Height() );

	m_pEditGrid->ShowWindow( true );
	m_pEditGrid->BringWindowToTop();
	m_pEditGrid->SetFocus();

	m_pEditGrid->SetWindowText( strInitValue );
    m_pEditGrid->SetSel( 0, -1 );

}

CRect CSeExpectVoltileForm::GetGridCellRect()
{
	CWnd *pWnd = ( CWnd * )m_pGrid;
	
	CRect rect,gridRect;
	RECT parentRect;
	pWnd->GetWindowRect( &gridRect );
	pWnd->GetParent()->GetClientRect( &parentRect );

	::ClientToScreen( pWnd->GetParent()->m_hWnd, ( POINT * )&( parentRect.left ) );

	gridRect.top -= parentRect.top;
	gridRect.left -= parentRect.left;

	rect.left   = gridRect.left + ( long )( ( float )m_pGrid->GetCellLeft()   / 15.0f );
	rect.top    = gridRect.top  + ( long )( ( float )m_pGrid->GetCellTop()    / 15.0f );
	rect.right  = rect.left     + ( long )( ( float )m_pGrid->GetCellWidth()  / 15.0f );
	rect.bottom = rect.top      + ( long )( ( float )m_pGrid->GetCellHeight() / 15.0f );
	
	return rect;
}

VOID CSeExpectVoltileForm::SetLevelSign(INT nRow,INT nCol,CString tmp)
{
	CRect rect = GetGridCellRect();
	m_pComboLevelSign->MoveWindow( rect.left - 2, rect.top - 3, rect.Width() + 2, rect.Height()  );

	INT idx = -1;
	CString strCurrentText;
	INT nCount = m_pComboLevelSign->GetCount();

	for(INT i=0; i<nCount;i++)
	{
		m_pComboLevelSign->SetCurSel(i);
		m_pComboLevelSign->GetWindowText(strCurrentText);
		if( tmp == strCurrentText ) {
			idx = i;
			break;
		}		
	}

	if( -1 == idx ) {
		m_pComboLevelSign->SetCurSel(0);
		m_pComboLevelSign->SetWindowText(tmp);
	}
	else
		m_pComboLevelSign->SetCurSel(idx);
	
	m_pComboLevelSign->BringWindowToTop();
	m_pComboLevelSign->ShowWindow(SW_SHOW);
	m_pComboLevelSign->SetFocus();

	m_bIsLevelSignFocused = TRUE;
	m_bTabUse = FALSE;

	
}

VOID CSeExpectVoltileForm::FreeGridEdit()
{
	if(m_pEditGrid)
	{
		delete m_pEditGrid;
		m_pEditGrid = NULL;
	}
}

VOID CSeExpectVoltileForm::OnbtnEXPLINEINSERT() 
{
	// TODO: Add your control notification handler code here
	INT nRowCnt =  m_pDM->GetRowCount();

	if(0 == nRowCnt)
	{
		m_pDM->InsertRow(-1);
		ShowGrid();
		m_pDM->SetCellData(_T("�����÷���"),_T("I"),m_pDM->GetRowCount()-1);
		m_pDM->SetCellData(_T("������ȣ"),m_strFORMAT_TYPE,m_pDM->GetRowCount()-1);
		m_pGrid->BringWindowToTop();
		m_pGrid->SetFocus();
		return;
	}
	
	//���������� �ܰ���� ��� �ִ� ���� �˻� �Ѵ�. 
	CString strLEVEL_NAME;
	strLEVEL_NAME = m_pDM->GetCellData(_T("�ܰ��"),nRowCnt-1);
	if(strLEVEL_NAME.IsEmpty())
	{
		AfxMessageBox(_T("���� ������ �ܰ�� �Էµ��� �ʾҽ��ϴ�"));
		return;
	}
		

	INT nRows =  m_pGrid->GetRows();  
	m_pGrid->SetRows( nRows + 1 );
	m_pDM->InsertRow(-1);
	m_pDM->SetCellData(_T("������ȣ"),m_strFORMAT_TYPE,m_pDM->GetRowCount()-1);
	m_pDM->SetCellData(_T("�����÷���"),_T("I"),m_pDM->GetRowCount()-1);
	m_pGrid->BringWindowToTop();
	m_pGrid->SetFocus();


}

VOID CSeExpectVoltileForm::OnbtnEXPLINEDELETE() 
{
	// TODO: Add your control notification handler code here
	// �ش� ���� ���� 
	// �Է¸�忡���� �׳� �����ϰ� ������忡�� �����Ͱ� �ִ� ���� detete key list�� �߰�
	INT nRowCnt = m_pDM->GetRowCount();
	
	// Ȳ���� ����
	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("������ ������ �����ϴ�!"));
		return;
	}
	else if(m_nCurrentIdx < 0)
	{
		AfxMessageBox(_T("���õ� ������ �����ϴ�."));
		return;
	}
	else
	{
		if(m_nOpenMode<0) { //������
			CString strREC_KEY;
			strREC_KEY = m_pDM->GetCellData(_T("��ȣ����KEY"),m_nCurrentIdx);
			if(!strREC_KEY.IsEmpty()) m_arrayDeleteKeyList.Add(strREC_KEY);
		}

		m_pDM->DeleteRow(m_nCurrentIdx);
		
		if(1 ==	nRowCnt)
			m_pGrid->SetRowHeight( 1, 0 );	
		else 
			ShowGrid();
	}
	/*else 
	{
		if(m_nOpenMode<0) { //������
			CString strREC_KEY;
			strREC_KEY = m_pDM->GetCellData(_T("��ȣ����KEY"),nRowCnt-1);
			if(!strREC_KEY.IsEmpty()) m_arrayDeleteKeyList.Add(strREC_KEY);
		}

		m_pDM->DeleteRow(nRowCnt-1);
		
		if(1 ==	nRowCnt)
			m_pGrid->SetRowHeight( 1, 0 );	
		else 
			ShowGrid();
	}
	*/
	m_pGrid->BringWindowToTop();
	m_pGrid->SetFocus();

}

VOID CSeExpectVoltileForm::OnKillfocuscmbEXPLEVELSIGN() 
{
	// TODO: Add your control notification handler code here
	
	CString strLEVEL_SIGN;
	m_pComboLevelSign->GetWindowText(strLEVEL_SIGN);

	INT nRow = m_pGrid->GetRow();
	INT nCol = m_pGrid->GetCol();
	
	m_pGrid->SetTextMatrix(nRow,nCol,strLEVEL_SIGN);
	m_pComboLevelSign->ShowWindow(SW_HIDE);
	m_pGrid->BringWindowToTop();	
	m_pGrid->SetFocus();
	m_pDM->SetCellData(_T("�ܰ�ĺ���"),strLEVEL_SIGN,nRow-1);

	m_bIsLevelSignFocused = FALSE;

	if(m_bTabUse) MakeNextSubControl(nRow,nCol);

}

VOID CSeExpectVoltileForm::ShowGrid()
{
	m_pGrid->Display();
	if(0 == m_pDM->GetRowCount())  
		m_pGrid->SetRowHeight( 1, 0 );	
	
	m_pGrid->BringWindowToTop();
	m_pGrid->SetFocus();
}


INT CSeExpectVoltileForm::SaveAllData(CESL_DataMgr* pDM_OUT,CString strACQ_TYPE,CString strACQ_KEY)
{
	CArray<CString,CString> RemoveAliasList;
	pDM_OUT->InitDBFieldData();
	CString strREC_KEY = _T("");

	if(m_nOpenMode>0) //�Է� (INSERT)
	{ 

		for(INT i=0;i<m_pDM->GetRowCount();i++)
		{
			m_pDM->MakeRecKey(strREC_KEY);
			m_pDM->SetCellData(_T("��ȣ����KEY"),strREC_KEY,i);
			m_pDM->SetCellData(_T("���Ա���"),strACQ_TYPE,i);
			m_pDM->SetCellData(_T("���Ա���KEY"),strACQ_KEY,i);
			m_pDM->SetCellData(_T("������ȣ"),m_strFORMAT_TYPE,i);
			m_pDM->SetCellData(_T("�����Ͽ�������"),m_strPubDatePattern,i);

			RemoveAliasList.RemoveAll();
			pDM_OUT->MakeQueryByDM(RemoveAliasList, m_pDM , _T("SE_VOL_EXP_TBL"), i , pDM_OUT);
			strREC_KEY = _T("");
		}

	}
	else // ����( INSERT , UPDATE , DELETE)
	{
		CString strQueryFlag = _T("");
		CString strQuery;
		
		for(INT i=0;i<m_pDM->GetRowCount();i++)
		{
			strQueryFlag = m_pDM->GetCellData(_T("�����÷���"),i);
			
			CString strEXP_LEVEL_NO;
			strEXP_LEVEL_NO.Format(_T("%d"),i+1);
			m_pDM->SetCellData(_T("�ܰ��ȣ"),strEXP_LEVEL_NO,i);
			RemoveAliasList.RemoveAll();

			if(_T("I") == strQueryFlag) //INSERT
			{
				m_pDM->MakeRecKey(strREC_KEY);
				m_pDM->SetCellData(_T("��ȣ����KEY"),strREC_KEY,i);
				m_pDM->SetCellData(_T("���Ա���"),strACQ_TYPE,i);
				m_pDM->SetCellData(_T("���Ա���KEY"),strACQ_KEY,i);
				m_pDM->SetCellData(_T("������ȣ"),m_strFORMAT_TYPE,i);
				m_pDM->SetCellData(_T("�����Ͽ�������"),m_strPubDatePattern,i);
				pDM_OUT->MakeQueryByDM(RemoveAliasList, m_pDM , _T("SE_VOL_EXP_TBL"), i , pDM_OUT);
			}
			else //UPDATE
			{
				strREC_KEY = m_pDM->GetCellData(_T("��ȣ����KEY"),i);
				RemoveAliasList.Add(_T("��ȣ����KEY"));
				m_pDM->SetCellData(_T("�����Ͽ�������"),m_strPubDatePattern,i);
				pDM_OUT->MakeQueryByDM(RemoveAliasList, m_pDM,_T("SE_VOL_EXP_TBL"), i , pDM_OUT,_T("REC_KEY"),_T("NUMERIC"),strREC_KEY);
			}

			strQueryFlag = _T("");
			strREC_KEY = _T("");
		}

		//DELETE 
		for(INT j=0;j<m_arrayDeleteKeyList.GetSize();j++)
		{
			strREC_KEY = m_arrayDeleteKeyList.GetAt(j);
			if(strREC_KEY.IsEmpty()) continue;		
			strQuery.Format(_T("DELETE FROM SE_VOL_EXP_TBL WHERE REC_KEY = %s;"),strREC_KEY);
			pDM_OUT->AddFrame(strQuery);
		}
			
	}

	return 0;
}



VOID CSeExpectVoltileForm::OnchkEXPUSEPUBDAYYN() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);
	
	if(m_bEXP_USE_PUB_DAY) //������ �������� ��� 
	{
		m_pComboPubDatePattern->ShowWindow(SW_SHOW);
		if(m_nOpenMode<0) // �����ÿ��� ������ REC_KEY�� �����Ѵ� 
		{
			
			CString strREC_KEY;
			for(INT i=0;i<m_pDM->GetRowCount();i++)
			{
				strREC_KEY = m_pDM->GetCellData(_T("��ȣ����KEY"),i);
				if(!strREC_KEY.IsEmpty()) m_arrayDeleteKeyList.Add(strREC_KEY);
			}

		}

		m_pDM->FreeData();
		m_pDM->InsertRow(-1);
		m_pDM->SetCellData(_T("�ܰ��"),_T("<������>"),0);
		m_pDM->SetCellData(_T("�ܰ�ĺ���"),_T(""),0);
		m_pDM->SetCellData(_T("���ۼ�"),_T(""),0);
		m_pDM->SetCellData(_T("����"),_T(""),0);
		m_pDM->SetCellData(_T("�ܰ�������"),_T(""),0);
		m_pDM->SetCellData(_T("��������"),_T(""),0);
		m_pDM->SetCellData(_T("�������ۼ�"),_T(""),0);
		m_pDM->SetCellData(_T("�����÷���"),_T("I"),0);
		m_pDM->SetCellData(_T("������ȣ"),m_strFORMAT_TYPE,0);	

		m_pGrid->EnableWindow(FALSE);
	}
	else
	{
		m_pComboPubDatePattern->ShowWindow(SW_HIDE);
		if(m_nOpenMode<0) // �����ÿ��� ������ REC_KEY�� �����Ѵ� 
		{
			
			CString strREC_KEY;
			for(INT i=0;i<m_pDM->GetRowCount();i++)
			{
				strREC_KEY = m_pDM->GetCellData(_T("��ȣ����KEY"),i);
				if(!strREC_KEY.IsEmpty()) m_arrayDeleteKeyList.Add(strREC_KEY);
			}

		}
		
		m_pDM->FreeData();
		m_pGrid->EnableWindow(TRUE);
	}

	ShowGrid();
	
}

VOID CSeExpectVoltileForm::OnbtnEXPPATTERNINSERT() 
{
	// TODO: Add your control notification handler code here
	// �����Է��� ������ �������̺� �Է��Ѵ� 

	// ���� ������ �Է¹ް� 
	CSeExpectGetPatternName dlg(this);
	if(dlg.DoModal() != IDOK) return;

	CString strREC_KEY;
	CString strEXP_LEVEL_NO;

	m_pDM_PATTERN->StartFrame();
	CArray<CString,CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	INT nEXP_LEVEL_NO = 1;

	for(INT i=0;i<m_pDM->GetRowCount();i++)
	{
		m_pDM_PATTERN->InsertRow(-1); //���� ���� ������ �߰�
		INT nRow = m_pDM_PATTERN->GetRowCount()-1;

		CopyDMToDMByAlias(m_pDM,i,m_pDM_PATTERN,nRow);
		m_pDM->MakeRecKey(strREC_KEY);
		m_pDM_PATTERN->SetCellData(_T("��ȣ����KEY"),strREC_KEY,nRow);
		m_pDM_PATTERN->SetCellData(_T("�����̸�"),dlg.m_strPATTERN_NAME,nRow);
			 
		strEXP_LEVEL_NO.Format(_T("%d"),nEXP_LEVEL_NO);
		m_pDM_PATTERN->SetCellData(_T("�ܰ��ȣ"),strEXP_LEVEL_NO,nRow);
		m_pDM_PATTERN->SetCellData(_T("�����۾�"),GetWorkLogMsg(WORK_LOG_EXP2),nRow);
		m_pDM_PATTERN->SetCellData(_T("�������۾�"),GetWorkLogMsg(WORK_LOG_EXP2),nRow);
		
		m_pDM_PATTERN->InitDBFieldData();
		m_pDM_PATTERN->MakeQueryByDM(RemoveAliasList, m_pDM_PATTERN , _T("SE_VOL_EXP_PATTERN_TBL"),nRow, m_pDM_PATTERN);
		nEXP_LEVEL_NO++;
	}
	
	INT ids = m_pDM_PATTERN->SendFrame();
	m_pDM_PATTERN->EndFrame();
	
	
	if(ids<0)
	{
		AfxMessageBox(_T("���� ���忡 �����Ͽ����ϴ�"));
	}
	else
	{
		CString msg;
		msg.Format(_T("[ %s ] �̸����� ���Ͽ� �����Ͽ����ϴ�"),dlg.m_strPATTERN_NAME);
		AfxMessageBox(msg);
	}

}

VOID CSeExpectVoltileForm::OnbtnEXPPATTERNSEARCH() 
{
	// TODO: Add your control notification handler code here
	// ���ϸ����� LIKE �˻��� �Ѵ�.
	CString strPATTERN_NAME = _T("");
	m_pCM->GetControlMgrData(_T("�����̸�"),strPATTERN_NAME);	
	CString strWhere;
	if(strPATTERN_NAME.IsEmpty())
		strWhere = _T("");
	else 
		strWhere.Format(_T("PATTERN_NAME LIKE '%%%s%%'"),strPATTERN_NAME);
	
	m_pDM_PATTERN->RefreshDataManager(strWhere);
	
	CESL_DataMgr* pDM = FindDM(_T("DM_����_����_�����̸�"));
	if(!pDM) {
		AfxMessageBox(_T("���� �̸� ������ ã�� �� �����ϴ�!"));
	}
	
	pDM->FreeData();
	CString tmp;
	BOOL IsLegal = TRUE;
	INT nRow;

	for(INT i=0;i<m_pDM_PATTERN->GetRowCount();i++)
	{
		strPATTERN_NAME = m_pDM_PATTERN->GetCellData(_T("�����̸�"),i);
		
		for(INT j=0;j<pDM->GetRowCount();j++)
		{
			tmp = pDM->GetCellData(_T("UDF_�����̸�"),j);
			if(tmp == strPATTERN_NAME) IsLegal = FALSE;
		}

		if(IsLegal)
		{
			pDM->InsertRow(-1);
			nRow = pDM->GetRowCount()-1;
			pDM->SetCellData(_T("UDF_�����̸�"),strPATTERN_NAME,nRow);
		}

		IsLegal = TRUE;
	}
	
	CESL_Grid* pGrid = (CESL_Grid*)m_pCM->FindControl(_T("���ϱ׸���"));
	pGrid->Display();

}

INT CSeExpectVoltileForm::CopyDMToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
{
	if (!pDM_SOURCE || !pDM_TARGET) return -1;

	INT nSourceColumnCount = pDM_SOURCE->GetRefColumnCount();
	CString field_alias, field_name, field_type, data_type, strData;
	INT ids;
	for (INT col = 0; col < nSourceColumnCount; col++) {
		strData = _T("");
		ids = pDM_SOURCE->FindColumnFieldName(col, field_alias, field_name, field_type, data_type, SourceRowIdx, strData);
		ids = pDM_TARGET->SetCellData(field_alias, strData, TargetRowIdx);
	}
	
	return 0;
}

VOID CSeExpectVoltileForm::OnbtnEXPVOLCLEARALL() 
{
	// TODO: Add your control notification handler code here
	// �Է½ÿ��� ����� �� �ִ� ȭ�� ����� �Լ��̴�
	
	m_bEXP_USE_PUB_DAY = FALSE;
	m_pDM->FreeData();
	UpdateData(FALSE);
	ShowGrid();
	
}

VOID CSeExpectVoltileForm::OnbtnEXPPATTERNMANAGE() 
{
	// TODO: Add your control notification handler code here
	CSeExpectPatternDlg dlg(this);
	dlg.DoModal();
	
	
	CESL_Grid* pGrid = (CESL_Grid*)m_pCM->FindControl(_T("���ϱ׸���"));
	pGrid->ShowWindow(FALSE);

	OnbtnEXPPATTERNSEARCH();

	pGrid->ShowWindow(TRUE);


	
}

BOOL CSeExpectVoltileForm::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_RETURN )
		{
			PostMessage(WM_LBUTTONDBLCLK, NULL, NULL);
			
			if(m_bIsLevelSignFocused)
			{
				OnKillfocuscmbEXPLEVELSIGN();
			}

			return TRUE;
		}
		else if( pMsg->wParam == VK_TAB)
		{
			PostMessage(WM_LBUTTONDBLCLK, NULL, NULL);
			
			if(m_bIsLevelSignFocused)
			{
				m_bTabUse = TRUE;
				OnKillfocuscmbEXPLEVELSIGN();
			}

			return TRUE;
		}
		else if( pMsg->wParam == VK_ESCAPE)
		{
			if(m_bIsLevelSignFocused)
			{
				OnKillfocuscmbEXPLEVELSIGN();
			}

			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

VOID CSeExpectVoltileForm::OnClickgridEXPSUPPORTVOLTITLE() 
{
	// TODO: Add your control notification handler code here
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		
		if(m_pDM->GetRowCount()>0)
		{
			  m_nCurrentIdx = row-1;
		}
	}
}

VOID CSeExpectVoltileForm::OnSelchangecmbEXPPUBPATTERN() 
{
	// TODO: Add your control notification handler code here
	INT nCurSel = m_pComboPubDatePattern->GetCurSel();
	m_strPubDatePattern.Format(_T("%02d"), nCurSel+1);
}

HBRUSH CSeExpectVoltileForm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
