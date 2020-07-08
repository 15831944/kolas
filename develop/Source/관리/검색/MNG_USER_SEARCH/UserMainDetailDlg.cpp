// UserMainDetailDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserMainDetailDlg.h"
#include "UserMainMarcDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserMainDetailDlg dialog


CUserMainDetailDlg::CUserMainDetailDlg(CESL_Mgr* pParent /*=NULL*/, CCommonProc *pCmdProc)
	: CESL_Mgr(CUserMainDetailDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserMainDetailDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bkR = 205;
	m_bkG = 229;
	m_bkB = 255;
	m_BkBrush.CreateSolidBrush(RGB(m_bkR, m_bkG, m_bkB));

	m_pCmdProc = pCmdProc;

	m_pkCnt = 0;

	// KOL.UDF.022 ��ť���ڵ� ����
	m_eslGrid = NULL;
	m_idx = -1;
}

CUserMainDetailDlg::~CUserMainDetailDlg()
{
	m_BkBrush.DeleteObject();
}

void CUserMainDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserMainDetailDlg)
	DDX_Control(pDX, IDC_DETAIL_stcTitle, m_MarkStaticTitle);
	DDX_Control(pDX, IDC_DETAIL_stcAuthor, m_MarkStaticAuthor);
	DDX_Control(pDX, IDC_DETAIL_stcPublisher, m_MarkStaticPublisher);
	DDX_Control(pDX, IDC_DETAIL_stcForm, m_MarkStaticForm);
	DDX_Control(pDX, IDC_DETAIL_stcMedia, m_MarkStaticMedia);
	DDX_Control(pDX, IDC_DETAIL_stcClassNo, m_MarkStaticClassNo);
	DDX_Control(pDX, IDC_DETAIL_stcManageCode, m_MarkStaticManageCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserMainDetailDlg, CDialog)
	//{{AFX_MSG_MAP(CUserMainDetailDlg)
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_DETAIL_btnClose, OnDETAILbtnClose)
	ON_BN_CLICKED(IDC_DETAIL_btnBefore, OnDETAILbtnBefore)
	ON_BN_CLICKED(IDC_DETAIL_btnAfter, OnDETAILbtnAfter)
	ON_BN_CLICKED(IDC_DETAIL_btnMarcView, OnDETAILbtnMarcView)	
	ON_BN_CLICKED(IDC_DETAIL_btnCallNoPrint, OnDETAILbtnCallNoPrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserMainDetailDlg message handlers

BOOL CUserMainDetailDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
 	{
		if(VK_RETURN == pMsg->wParam)
		{
			return TRUE;
		}
 	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CUserMainDetailDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if( IDC_DETAIL_edtTitle == pWnd->GetDlgCtrlID() ||
		IDC_DETAIL_edtAuthor == pWnd->GetDlgCtrlID() ||
		IDC_DETAIL_edtPublisher == pWnd->GetDlgCtrlID() ||
		IDC_DETAIL_edtForm == pWnd->GetDlgCtrlID() ||
		IDC_DETAIL_edtMedia == pWnd->GetDlgCtrlID() ||
		IDC_DETAIL_edtClassNo == pWnd->GetDlgCtrlID() ||
		IDC_DETAIL_edtManageCode == pWnd->GetDlgCtrlID() )
	{
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(RGB(m_bkR, m_bkG, m_bkB));
		return m_BkBrush;
	}

	return hbr;
}

BOOL CUserMainDetailDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	if(InitESL_Mgr(_T("SM_USERMAIN_DETAIL")) < 0)
 	{
 		AfxMessageBox(_T("InitESL_Mgr Error!"));
 		return FALSE;
 	}

	// û����ȣ��� ��뿩�ο����� ó��
	CString strReceiptUseYN;
	strReceiptUseYN = m_pCmdProc->m_strReceiptUseYN;
	if( strReceiptUseYN == _T("Y") )
	{
		GetDlgItem(IDC_DETAIL_btnCallNoPrint)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_DETAIL_btnCallNoPrint)->ShowWindow(SW_HIDE);
	}

	

	return TRUE;
}

void CUserMainDetailDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if( !m_pk.GetLength() )
	{
		PostMessage( WM_CLOSE );
		return;
	}
	else
	{
		ShowDetailView();
	}

	if(1 == m_pkCnt)
	{
		((CButton*)GetDlgItem(IDC_DETAIL_btnBefore))->ShowWindow(FALSE);
		((CButton*)GetDlgItem(IDC_DETAIL_btnAfter))->ShowWindow(FALSE);
	}
}

void CUserMainDetailDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

void CUserMainDetailDlg::OnDETAILbtnClose() 
{
	OnCancel();	
}

VOID CUserMainDetailDlg::SetPKList(CString sDMAlias, CStringList &pkList, CESL_Grid *grid, CArray< INT, INT > &gridRow, INT idx)
{
	m_pkList.RemoveAll();
	POSITION pos = pkList.GetHeadPosition();
	while(pos)
	{
		m_pkList.AddTail(pkList.GetNext(pos));		
	}

	m_pkCnt = m_pkList.GetCount();

	m_idx = idx;
	pos = m_pkList.FindIndex(m_idx);
	m_pk = m_pkList.GetNext(pos);

	m_sDMAlias = sDMAlias;
	m_eslGrid = grid;

	SetMode(m_idx);

	for(INT i = 0; i < gridRow.GetSize(); i++)
	{
		m_selectedGridRow.Add(gridRow.GetAt(i));
	}

	m_eslGrid->SetReverse(m_selectedGridRow.GetAt(m_idx));
}

VOID CUserMainDetailDlg::SetMode(INT nIndex)
{
	CESL_DataMgr *pDM = FindDM(m_sDMAlias);
	pDM->GetCellData(_T("����/���ӱ���"), nIndex, m_sPublishFormCode);
}

INT CUserMainDetailDlg::ShowDetailView()
{
	CString sData, sData2;
	INT ids;

	CESL_DataMgr pRSDM;	
	pRSDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	CString where;
	where.Format(_T("I.REC_KEY='%s'"), m_pk);

	CString sDMAlias, sDMAlias_BOOK;
	CESL_DataMgr *pDM;
	if(_T("MO") == m_sPublishFormCode)
	{
		sDMAlias = _T("DM_USERMAIN_DETAIL_BO");
		sDMAlias_BOOK = _T("DM_USERMAIN_DETAIL_BO_BOOK");
	}
	else
	{
		sDMAlias = _T("DM_USERMAIN_DETAIL_SE");
		sDMAlias_BOOK = _T("DM_USERMAIN_DETAIL_SE_BOOK");
	}
	pDM = FindDM(sDMAlias);
	pDM->FreeData();
	pRSDM.InitDMPKFromFile(sDMAlias);
	pRSDM.RefreshDataManager(where);
	m_pCmdProc->CopyDM2DM(&pRSDM, pDM);
	pRSDM.FreeData();

	pDM = FindDM(sDMAlias_BOOK);
	pDM->FreeData();
	pRSDM.InitDMPKFromFile(sDMAlias_BOOK);

	// 2009.11.26 UPDATE BY PWR : �˻��뿡���� �󼼺���� �̿��ڿ� �ڷḸ �˻��ǵ��� �Ѵ�.
	// 2009.12.21 UPDATE BY PWR : �ηϰ˻��ȵǵ���, ������ ��Ϲ�ȣ ���°� �˻��ȵǵ��� ����
	if(_T("MO") == m_sPublishFormCode)
	{
		// 17/07/20 ������ : å���������ڷ� ���� �߰� 
/*//BEFORE-----------------------------------------------------------------------------
		// 2013.12.01 UPDATE BY BJS : 2013���������ռ���Ȯ�뱸��װ��� (���յ���Ÿ������ ���� �߰�)
		where += _T(" AND B.WORKING_STATUS IN ('BOL112N','BOL113O','BOL114O','BOL115O','BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL217O','BOL218O','BOL511O','BOL511O')");
		//where += _T(" AND B.WORKING_STATUS IN ('BOL112N','BOL113O','BOL114O','BOL115O','BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL217O','BOL218O','BOL511O')");
*///AFTER------------------------------------------------------------------------------
		where += _T(" AND B.WORKING_STATUS IN ('BOL112N','BOL113O','BOL114O','BOL115O','BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL217O','BOL218O','BOL219O','BOL511O','BOL611O')");
/*//END-------------------------------------------------------------------------------*/
		where += _T(" AND B.BOOK_APPENDIX_FLAG='B' AND B.REG_NO IS NOT NULL ");
	}
	else
	{
		where += _T(" AND B.WORKING_STATUS IN ('SEL112N','SEL212N','SEL213O','SEL214O','SEL215O','SEL311O','SEL312O','SEL313O','SEL314O','SEL315O','SEL317O','SEL318O','SEL511O')");
		where += _T(" AND B.BOOK_TYPE IN ('B','C') ");
	}

	// 2009.12.14 ADD BY PWR : �缭���� �˻��ȵǵ��� ����
	where += _T(" AND B.USE_LIMIT_CODE != 'CL'");

	pRSDM.RefreshDataManager(where);
	m_pCmdProc->CopyDM2DM(&pRSDM, pDM);
	pRSDM.FreeData();

//+++++ 2011.9.16 ä��ȣ [�߰�]
// ���� : ����ñ���û ��ɰ������
// �䱸 : �ڷ�ã�⿡�� �����ڷ��� ��� �������� ǥ�õǵ��� ����
	for(INT i = 0; i < pDM->GetRowCount(); i++)
	{
		ids = pDM->GetCellData(_T("���������̷°Ǽ�"), i, sData);
		if(0 > ids) { return -12; }

		if ( sData > _T("0") ) {
			sData2.Format(_T("X(������(%s��))"), sData);
			ids = pDM->SetCellData(_T("���Ⱑ�ɿ���"), sData2, i);
			if(0 > ids) { return -15; }

			ids = pDM->GetCellData(_T("�ڷ���¼���"), i, sData);
			if(0 > ids) { return -12; }

			if ( sData != _T("������") ) {
				ids = pDM->SetCellData(_T("�ڷ���¼���"), _T("������"), i);
				if(0 > ids) { return -15; }
			} 
		}
	}
//-----

	ShowControls();
	
	return 0;
}

VOID CUserMainDetailDlg::ShowControls()
{
	//SetCodeToDesc();

	if(_T("MO") == m_sPublishFormCode)
	{
		GetDlgItem(IDC_DETAIL_gridBook_BO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DETAIL_gridBook_SE)->ShowWindow(SW_HIDE);
		
		LockWindowUpdate();
		CESL_Grid *pGrid1 = (CESL_Grid*)GetDlgItem(IDC_DETAIL_gridBook_BO);
		pGrid1->m_nHeight = 30;
		ControlDisplay(_T("CM_USERMAIN_DETAIL_BO"), _T("BoGrid"));
		UnlockWindowUpdate();
	}
	else
	{
		GetDlgItem(IDC_DETAIL_gridBook_SE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DETAIL_gridBook_BO)->ShowWindow(SW_HIDE);
		
		LockWindowUpdate();
		CESL_Grid *pGrid2 = (CESL_Grid*)GetDlgItem(IDC_DETAIL_gridBook_SE);
		pGrid2->m_nHeight = 30;
		ControlDisplay(_T("CM_USERMAIN_DETAIL_SE"), _T("SeGrid"));
		UnlockWindowUpdate();
	}

 	DisplaySpecies();
}

INT CUserMainDetailDlg::DisplaySpecies()
{
	INT ids;
	CString sDMAlias;
	if(_T("MO") == m_sPublishFormCode)
	{
		sDMAlias = _T("DM_USERMAIN_DETAIL_BO");
	}
	else
	{
		sDMAlias = _T("DM_USERMAIN_DETAIL_SE");
	}

	CESL_DataMgr* pDM = FindDM(sDMAlias);
	if(NULL == pDM)
	{
		return -1;
	}
	
	const INT nFieldCnt = 7;
	TCHAR* sDMFieldName[nFieldCnt] =
	{
		_T("�������"), _T("�����ڻ���"), _T("�����ڻ���"), 
		_T("S_MARC"), _T("S_MEDIA_CODE"), _T("�з���ȣ"), 
		_T("S_MANAGE_CODE") 
	};
	
	const int nContolName[nFieldCnt] =
	{
		IDC_DETAIL_edtTitle, IDC_DETAIL_edtAuthor, IDC_DETAIL_edtPublisher,
		IDC_DETAIL_edtForm, IDC_DETAIL_edtMedia, IDC_DETAIL_edtClassNo,
		IDC_DETAIL_edtManageCode,
	};
	
	CMarcMgr* pMarcMgr = m_pInfo->pMarcMgr;
	CMarc Marc;
	
	CString sData;
	for(INT i = 0; i < nFieldCnt; i++)
	{
		ids = pDM->GetCellData(sDMFieldName[i], 0, sData);
		if(0 > ids)
		{
			return -2;
		}
		if(3 == i)
		{ // ���»���
			pMarcMgr->Decoding( sData , &Marc );
			sData.Empty();
			Marc.GetField( _T("300"), sData );
			sData.Replace( CString( 31 ) + _T("a") , _T("") );
			sData.Replace( CString( 31 ) + _T("b") , _T("") );
			sData.Replace( CString( 31 ) + _T("c") , _T("") );
			sData.Replace( CString( 31 ) + _T("d") , _T("") );
			sData.Replace( CString( 31 ) + _T("e") , _T("") );
			sData.Replace( CString( 30 ) , _T("") );
			sData.TrimLeft();
			sData.TrimRight();
		}
		
		//2012.11.05 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2��
 		if(4 == i)
 		{ // ��ü����
 			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("��ü����"), sData, sData);
 		}
 		if(6 == i)
 		{ // ��������
 			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("��������"), sData, sData);
 		}
		GetDlgItem(nContolName[i])->SetWindowText(sData);
	}
	
	return 0;
}

VOID CUserMainDetailDlg::SetCodeToDesc()
{
	CString sDMAlias;
	if(_T("MO") == m_sPublishFormCode)
	{
		sDMAlias = _T("DM_USERMAIN_DETAIL_BO_BOOK");
	}
	else
	{
		sDMAlias = _T("DM_USERMAIN_DETAIL_SE_BOOK");
	}
	CESL_DataMgr *pDM = FindDM(sDMAlias);

	const INT nCnt = 2;
	CString sCodeName[nCnt] =
	{
		_T("�ڷ�Ǳ���") , _T("��ü����")
	};
	CString sDMFieldName[nCnt] =
	{
		_T("�ڷ�Ǳ���") , _T("��ü����")
	};
	
	for(INT i = 0; i < nCnt; i++)
	{
		m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, sCodeName[i] , sDMFieldName[i]);
	}

	// �ڷ���� �ڵ� -> ����
	// û����ȣ ����
	m_pCmdProc->m_pLocCommonAPI->MakeShelfCode(this, pDM->DB_MGR_ALIAS);

	// �ڷ����
	// �Թ�ȣ
	// ���� ���� ���θ� �Ǵ��Ѵ�.
	CString sShelfLocCode, sBoxName;
	const INT nCheckIsLoanCnt = 7;
	CString sCheckIsLoan[nCheckIsLoanCnt][2] =
	{
		{ _T("B_WORKING_STATUS"), _T("") } ,
		{ _T("��������"), _T("") } ,
		{ _T("���������̷°Ǽ�"), _T("") } ,
		{ _T("�̿����ѱ���"), _T("") } ,
		{ _T("IBS_û��_��ġ��ȣ_�ڵ�"), _T("") } ,
		{ _T("��ϱ���"), _T("") } ,
		{ _T("�ڷ�Ǳ���"), _T("") } 
	};
	
	for( i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		// ���� ���ɿ��� �Ǵ�
		// 1. �ڷ����
		// 2. ��������
		// 3. �ηϿ��� <- �η��� �ƿ� �˻� ����
		// 4. ���࿩��
		// 5. �̿����ѱ���
		// 6. ��ġ��ȣ
		// 7. ��ϱ���
		// 8. �ڷ��
		CString sIsLoan = _T("O");
		for(INT j = 0; j < nCheckIsLoanCnt; j++)
		{
			pDM->GetCellData(sCheckIsLoan[j][0], i, sCheckIsLoan[j][1]);
		}
		// 1
		if(		_T("BOL112N") == sCheckIsLoan[0][1] 
			||	_T("��ġ�ڷ�") == sCheckIsLoan[0][1]
			||	_T("SEL112N") == sCheckIsLoan[0][1]
			||	_T("���Ӻ�ġ�ڷ�") == sCheckIsLoan[0][1]
			||	_T("SEL212N") == sCheckIsLoan[0][1]
			||	_T("�ǽð��谡�ڷ�") == sCheckIsLoan[0][1]	)
		{				
		}
		else
		{
			sIsLoan = _T("X");
		}
		// 2
		if(sCheckIsLoan[1][1] != m_pCmdProc->m_strManageCode)
		{
			sIsLoan = _T("X(Ÿ��å)");
		}
		// 4
		if(_ttoi(sCheckIsLoan[2][1]) > 0 )
		{
			sIsLoan = _T("X(������)");
		}
		// 5
		CString sValue;
		m_pCmdProc->IsLoanCodeYN(_T("�̿����ѱ���"), sCheckIsLoan[3][1], sValue);
		if(_T("Y") != sValue)			
		{
			sIsLoan = _T("X(�̿�����)");
		}
		// 6
		m_pCmdProc->IsLoanCodeYN(_T("��ġ��ȣ"), sCheckIsLoan[4][1], sValue);
		if(_T("Y") != sValue)			
		{
			sIsLoan = _T("X(��ġ��ȣ����)");
		}
		// 7
		m_pCmdProc->IsLoanCodeYN(_T("��ϱ���"), sCheckIsLoan[5][1], sValue);
		if(_T("Y") != sValue)			
		{
			sIsLoan = _T("X(��ϱ�������)");
		}
		// 8
		m_pCmdProc->IsLoanCodeYN(_T("�ڷ�Ǳ���"), sCheckIsLoan[6][1], sValue);
		if(_T("Y") != sValue)			
		{
			sIsLoan = _T("X(�ڷ������)");
		}
		pDM->SetCellData(_T("���Ⱑ�ɿ���"), sIsLoan, i);
		// �ڷ����
		CString sWorkingStatus;
		pDM->GetCellData( _T("B_WORKING_STATUS") , i , sWorkingStatus );
		m_pCmdProc->m_pLocCommonAPI->GetWorkingStatusDescToCode( sWorkingStatus , 1);
		pDM->SetCellData( _T("B_WORKING_STATUS") , sWorkingStatus , i );
	}

	return;
}

void CUserMainDetailDlg::OnDETAILbtnBefore() 
{
	BeginWaitCursor();

	if(!m_idx)
	{
		EndWaitCursor();
		return;
	}
	
	m_idx--;
	POSITION pos = m_pkList.FindIndex( m_idx );
	m_pk = m_pkList.GetPrev( pos );
	
	SetMode(m_idx);

	m_eslGrid->SetReverse( m_selectedGridRow.GetAt( m_idx ) );
	ShowDetailView();

	EndWaitCursor();
}

void CUserMainDetailDlg::OnDETAILbtnAfter() 
{
	BeginWaitCursor();

	if(m_idx == m_pkCnt - 1)
	{
		EndWaitCursor();
		return;
	}
	
	m_idx++;
	POSITION pos = m_pkList.FindIndex( m_idx );
	m_pk = m_pkList.GetNext( pos );

	SetMode(m_idx);

	m_eslGrid->SetReverse( m_selectedGridRow.GetAt( m_idx ) );
	ShowDetailView();

	EndWaitCursor();
}

void CUserMainDetailDlg::OnDETAILbtnMarcView() 
{
	CString marc;
	CString sDMAlias;
	if(_T("MO") == m_sPublishFormCode)
	{
		sDMAlias = _T("DM_USERMAIN_DETAIL_BO");
	}
	else
	{
		sDMAlias = _T("DM_USERMAIN_DETAIL_SE");
	}

	CESL_DataMgr *pDM = FindDM(sDMAlias);
	pDM->GetCellData(_T("S_MARC"), 0, marc);

	marc.TrimLeft(); marc.TrimRight();
	if( marc.IsEmpty() ) 
	{
		MessageBox(_T("��ũ�� �������� �ʴ� �ڷ��Դϴ�."), _T("�󼼺���"), MB_ICONEXCLAMATION);
		return ;
	}

	
	CUserMainMarcDlg dlg( this );
	dlg.m_sPublishFormCode = m_sPublishFormCode;
	dlg.DoModal();
}


void CUserMainDetailDlg::OnDETAILbtnCallNoPrint() 
{
	// 2010.05.14 UPDATE BY PWR : �������˻������� �����ϵ���
// 	if(_T("0") != m_pCmdProc->m_strSearchMode)
// 	{
// 		return;
// 	}
	INT ids, nIdx;
	CESL_ControlMgr *pCM;
	CESL_Grid *pGrid;
	if(_T("MO") == m_sPublishFormCode)
	{
		pCM = FindCM(_T("CM_USERMAIN_DETAIL_BO"));
		if(NULL == pCM) return;
		pGrid = (CESL_Grid*)(pCM->FindControl(_T("BoGrid")));
		if(NULL == pGrid) return;
	}
	else
	{
		pCM = FindCM(_T("CM_USERMAIN_DETAIL_SE"));
		if(NULL == pCM) return;
		pGrid = (CESL_Grid*)(pCM->FindControl(_T("SeGrid")));
		if(NULL == pGrid) return;
	}
	nIdx = pGrid->GetIdx();
	if(0 > nIdx)
	{
		return;
	}
	CString sDMAlias;
	if(_T("MO") == m_sPublishFormCode)
	{
		sDMAlias = _T("DM_USERMAIN_DETAIL_BO_BOOK");
	}
	else
	{
		sDMAlias = _T("DM_USERMAIN_DETAIL_SE_BOOK");
	}
	CESL_DataMgr *pDM = FindDM(sDMAlias);
	if(NULL == pDM)
	{
		return;
	}
	if(1 > pDM->GetRowCount())
	{
		MessageBox(_T("�˻��� �ڷᰡ �����ϴ�."), _T("�ڷ�˻�"), MB_ICONEXCLAMATION);
		return;
	}
// 	CString strLoanPossible;
// 	pDM->GetCellData(_T("���Ⱑ�ɿ���"), nIdx, strLoanPossible);
// 	if(_T("O") != strLoanPossible)
// 	{
// 		MessageBox(_T("���Ⱑ���� å�� ����� �� �ֽ��ϴ�."), _T("�ڷ�˻�"), MB_ICONEXCLAMATION);
// 		return;
// 	}
 	CString strTitle;
 	CString strAuthor;
 	CString strCallNo;
 	CString strShelfLocCode;
	CString strRegNo;
	pDM->GetCellData(_T("����"), nIdx, strTitle);
	pDM->GetCellData(_T("������"), nIdx, strAuthor);
	pDM->GetCellData(_T("û����ȣ"), nIdx, strCallNo);		
	pDM->GetCellData(_T("�ڷ���ڵ弳��"), nIdx, strShelfLocCode);
	pDM->GetCellData(_T("B_REG_NO"), nIdx, strRegNo);
// 	if(strCallNo.IsEmpty())
// 	{
// 		MessageBox(_T("û����ȣ�� ���� �ڷ��Դϴ�."), _T("�ڷ�˻�"), MB_ICONEXCLAMATION);
// 		return;
// 	}

	ids = m_pCmdProc->m_CallNoPrnMgr.GetReady();
	if(0 != ids)
	{
		MessageBox(_T("������ ��±���� ���ῡ �����Ͽ����ϴ�."), _T("�ڷ�˻�"), MB_ICONEXCLAMATION);
		return;
	}
	m_pCmdProc->m_CallNoPrnMgr.Init();
	m_pCmdProc->m_CallNoPrnMgr.Add(strTitle, strAuthor,  strCallNo, strShelfLocCode, strRegNo);
	m_pCmdProc->m_CallNoPrnMgr.Print();
}
