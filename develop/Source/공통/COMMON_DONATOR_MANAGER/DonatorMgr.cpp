// DonatorMgr.cpp : implementation file
//

#include "stdafx.h"
#include "DonatorMgr.h"
#include "../SearchZipCodeApi/SearchZipCodeDlg.h"

// 14/11/27 �ڴ�� : ���θ��ּ� OpenAPI ����
/*//BEFORE-----------------------------------------------------------------------
//2012.11.23 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2��(���ּ� ��ȸ)
#include "..\\..\\..\\..\\����\\SearchZipCodeAPI\\SearchZipCodeDlgNew.h"
*///AFTER------------------------------------------------------------------------
#include "..\\..\\..\\..\\����\\SearchZipCodeAPI\\SearchZipCodeDlgOpenAPI.h"
/*//END------------------------------------------------------------------------*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WORK_LOG GetWorkLogMsg(_T("�������Է¼���"), _T(__FILE__), __LINE__)

/////////////////////////////////////////////////////////////////////////////
// CDonatorMgr dialog


CDonatorMgr::CDonatorMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDonatorMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDonatorMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pMainGrid = NULL;
	m_nOpenMode = OPERATION_TYPE_INVALID;
	m_nIdx = -1; 
	m_pComboMgr = new CMulitComboBoxManager(this);
	m_firstidx = -1;
	m_lastidx = -1;
	m_pLastNumber = new CLastNumberManager(this);
	m_strMainCMAlias = _T("CM_�����ڰ���");
	m_bGridSelectYN = FALSE;

	m_pCMakeIndex = new CMakeSearchData(pParent);
}

CDonatorMgr::~CDonatorMgr()
{

	if(m_pComboMgr)
	{
		delete m_pComboMgr;
		m_pComboMgr = NULL;
	}
	if(m_pLastNumber)
	{
		delete m_pLastNumber;
		m_pLastNumber = NULL;
	}
	if (m_pCMakeIndex)
	{
		delete m_pCMakeIndex;
		m_pCMakeIndex = NULL; 
	}
}

VOID CDonatorMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDonatorMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDonatorMgr, CDialog)
	//{{AFX_MSG_MAP(CDonatorMgr)
	ON_BN_CLICKED(IDC_btnDONATOR_FIND_ZIP_CODE, OnbtnDONATORFINDZIPCODE)
	ON_BN_CLICKED(IDC_btnDONATOR_PREV, OnbtnDONATORPREV)
	ON_BN_CLICKED(IDC_btnDONATOR_NEXT, OnbtnDONATORNEXT)
	ON_EN_KILLFOCUS(IDC_edtDONATOR_NAME, OnKillfocusedtDONATORNAME)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_cmbDONATOR_FLAG, OnSelchangecmbDONATORFLAG)
	ON_EN_KILLFOCUS(IDC_edtDONATOR_YEAR, OnKillfocusedtDONATORYEAR)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDonatorMgr message handlers

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - ������ �Է�â �ʱ�ȭ
//
// [ �̴Ͻ��� ]
/////////////////////////////////////////////////////////////////////////////
BOOL CDonatorMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//=======================================================================
	// 1. SM/CM/DM�� �����Ѵ�.
	//=======================================================================
	if( InitESL_Mgr( _T("K2UPGRADE_������") ) < 0 )
	{
		ESLAfxMessageBox( _T("InitESL_Mgr Error : MNG_USER_MANAGER_02") );
		return FALSE;
	}
	m_pCM = FindCM(_T("CM_������"));
	m_pDM = FindDM(_T("DM_�����ڰ���"));
	if( m_pCM == NULL || m_pDM == NULL )
	{
		ESLAfxMessageBox( _T("[CM_������/DM_�����ڰ���]�� ������ �� �����ϴ�.") );
		return FALSE;
	}

	m_pMainGrid = (CESL_Grid*)FindControl(m_strMainCMAlias, _T("MainGrid"));
	if( m_pMainGrid == NULL )
	{
		ESLAfxMessageBox( _T("[CM_�����ڰ���]�� ������ �� �����ϴ�.") );
		return FALSE;
	}

	//=======================================================================
	// 2. ���������� �����Ѵ�.
	//=======================================================================
	this->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);

	//CString strData[2] = {_T("PN"), _T("GR")};
	//m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T("PN"), strData, _T("�����ڱ���"), 2);

	//=======================================================================
	// 3. �ʼ� �Է��׸� ���� ��Ʈ�� ������ �����Ѵ�.
	//=======================================================================	
	AddColorControl(IDC_edtDONATOR_NAME, _T(""), _T(""), 'E');
	AddColorControl(IDC_edtDONATOR_YEAR, _T(""), _T(""), 'E');


	//=======================================================================
	// 4. ���¸�忡 ���� ���ҽ��� ǥ�ø� �����Ѵ�. (�Է�/����)
	//=======================================================================
	ShowCtrlByOpenMode();

	//=======================================================================
	// 5. ���¸�尡 �Է��� �ƴ϶�� 
	//=======================================================================
	if(m_nOpenMode == SINSERT)	OnSelchangecmbDONATORFLAG();
	else						Display(0);

	//=====================================================
	//2009.01.13 ADD BY PJW : ������ �����ڵ�� ������ �ϼ������� �Ѵ�.
	//2009.02.03 REM BY PJW : ������ �����ڵ�� MANAGE_CODE �� ������ �ּ�ó����
// 	CEdit* pEdit = (CEdit*) GetDlgItem(IDC_cmbDONATOR_FLAG);
// 	pEdit->EnableWindow(FALSE);
	//=====================================================	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CDonatorMgr::OnbtnDONATORFINDZIPCODE() 
{
	// 14/11/27 �ڴ�� : ���θ��ּ� OpenAPI ����
/*//BEFORE-----------------------------------------------------------------------
	//2012.11.23 ADD BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2��(���ּ� ��ȸ)
	CSearchZipCodeDlgNew SearchZipCodeDlgNew(this);
	if(IDOK == SearchZipCodeDlgNew.DoModal())
	{			
		m_pCM->SetControlMgrData(_T("�����ȣ"), SearchZipCodeDlgNew.m_ResultZipcode);
		m_pCM->SetControlMgrData(_T("�ּ�"), SearchZipCodeDlgNew.m_ResultAddress);
	}

	//2012.11.23 DEL BY BJY : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2��(���ּ� ��ȸ)
//	CSearchZipCodeDlg SearchZipCodeDlg(this);
//	SearchZipCodeDlg.DoModal();
//
//	// �����
//	if(SearchZipCodeDlg.m_nCloseMode != 0)
//	{
//		m_pCM->SetControlMgrData(_T("�����ȣ"), SearchZipCodeDlg.m_sZipCode);
//		m_pCM->SetControlMgrData(_T("�ּ�"), SearchZipCodeDlg.m_sAddress);
//	}
*///AFTER------------------------------------------------------------------------
	CSearchZipCodeDlgOpenAPI dlg(this);
	if (dlg.DoModal() == IDOK)
	{
		m_pCM->SetControlMgrData(_T("�ּ�"), dlg.GetApplyAddress());
		m_pCM->SetControlMgrData(_T("�����ȣ"), dlg.GetApplyZipCode());		
	}
/*//END------------------------------------------------------------------------*/	
}

VOID CDonatorMgr::OnbtnDONATORPREV() 
{
	Display(-1);
}

VOID CDonatorMgr::OnbtnDONATORNEXT() 
{
	Display(1);
}

VOID CDonatorMgr::OnOK() 
{
	INT ids = -1;
	CString strMsg;

	if( m_nOpenMode == SINSERT )	ids = InsertDonator();
	else							ids = UpdateDonator();

	if( ids < 0 )
	{
		if( m_nOpenMode == SINSERT )	strMsg = _T("������ ������ �Է��� �� �����ϴ�.");
		else							strMsg = _T("������ ������ ������ �� �����ϴ�.");
	}
	else if( ids == 0 )
	{
		strMsg = _T("������ ������ �����Ͽ����ϴ�.");
		if( m_nOpenMode == SINSERT )	m_pCM->AllControlClear();
		else							Display(1);
	}
	else		return;
	ESLAfxMessageBox(strMsg);
	OnSelchangecmbDONATORFLAG();
}

VOID CDonatorMgr::ShowCtrlByOpenMode()
{
	if(m_nOpenMode == SINSERT)
	{
		GetDlgItem(IDC_btnDONATOR_PREV)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnDONATOR_NEXT)->ShowWindow(SW_HIDE);		
		GetDlgItem(IDC_edtDONATOR_ID)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_edtDONATOR_SERIAL_NO)->ShowWindow(SW_HIDE);
		SetWindowText(_T("���������� �Է�"));
	}
	else
	{
		GetDlgItem(IDC_btnDONATOR_PREV)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnDONATOR_NEXT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cmbDONATOR_FLAG)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtDONATOR_YEAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtDONATOR_ID)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtDONATOR_SERIAL_NO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcDONATOR_SERIAL_NO)->ShowWindow(SW_HIDE);
		SetWindowText(_T("���������� ����"));
	}
}

VOID CDonatorMgr::Display(INT ids)
{
	INT nIdx;
	CString strID, strYear, strFlag, strSerialNo;

	switch( ids )
	{
	case 0:
		{
		InitFirstLastIndex();
		nIdx = GetFirstSelectIndex();
		ShowNextPrevButton(nIdx);
		}
		break;
	case -1:
		{
		nIdx = GetPrevSelectIndex();
		ShowNextPrevButton(nIdx);
		}
		break;
	case 1:
		{
		nIdx = GetNextSelectIndex();
		ShowNextPrevButton(nIdx);
		}
		break;
	default:		return ;
		break;
	}

	if( nIdx >= 0 )		 m_nIdx = nIdx;

	m_pDM->GetCellData(_T("���̵�"), m_nIdx, strID);
	strFlag = strID.Left(2);
	strYear = strID.Mid(2,4);
	strSerialNo = strID.Right(5);

	//CString strData[2] = {_T("PN"), _T("GR")};
	//m_pComboMgr->SetSingleComboBoxSelect(m_pCM->CM_ALIAS, _T("�����ڱ���"), strData, strFlag, 2);
	
	m_pCM->SetControlMgrData(_T("��ϳ⵵"), strYear);
	m_pCM->SetControlMgrData(_T("�Ϸù�ȣ"), strSerialNo);
	m_pCM->AllControlDisplay(m_nIdx);
	
	m_pMainGrid->SetReverse(m_nIdx);
}


//////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �׸��� ����üũ ���ο� ���� ��������� �����Ѵ�.
//
//////////////////////////////////////////////////////////////////////////////
VOID CDonatorMgr::InitFirstLastIndex()
{
	//=======================================================================
	// 1. �׸��� ����üũ�� Ȯ�� �Ѵ�.
	//=======================================================================
	INT ids = m_pMainGrid->SelectMakeList();

	//=======================================================================
	// 2. ����üũ�� �Ǿ� ���� �ʴٸ� if������ �����ϰ�, �Ǿ� �ִٸ� else ������ �����Ѵ�.
	//		- if : �׸��� ����üũ=FALSE, ù �ε���=0, ������ �ε���=�������ο�
	//		- else : �׸��� ����üũ=TRUE, ù�ε���=HeadPosition, �������ε���=TailPosition
	//=======================================================================
	ids = m_pMainGrid->SelectGetHeadPosition();
	if(ids < 0)
	{
		m_bGridSelectYN = FALSE;
		INT cnt = m_pDM->GetRowCount();
		m_firstidx = 0;
		m_lastidx = cnt-1;
	}
	else
	{
		m_bGridSelectYN = TRUE;
		m_lastidx = m_pMainGrid->SelectGetTailPosition();
		m_firstidx = m_pMainGrid->SelectGetHeadPosition();
	}
}

VOID CDonatorMgr::ShowNextPrevButton(INT idx)
{
	if(idx < 0) idx = m_nIdx;
	GetDlgItem(IDC_btnDONATOR_PREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnDONATOR_NEXT)->EnableWindow(TRUE);

	if(m_firstidx >= idx || idx < 0)
		GetDlgItem(IDC_btnDONATOR_PREV)->EnableWindow(FALSE);
	if(m_lastidx <= idx || idx < 0)
		GetDlgItem(IDC_btnDONATOR_NEXT)->EnableWindow(FALSE);
}

//////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �������Է�
//
// [ �̴Ͻ��� ]
// 1. �����ڶ��� ����ְų� ������ �����ڰ� �����ϴ��� Ȯ���Ѵ�.
// 2. CM_�����ڸ� �̿��Ͽ� DM_�����ڰ����� �����Ѵ�.
// 3. DM�� �����Ѵ�.
// 4. ������ DM�� ������ �����Ͽ� ��� �����Ѵ�.
// 5. �Էµ� �����ڸ� �׸��忡 �߰��Ѵ�.
// 6. ��������ȣ�� ������Ʈ�Ѵ�.
//
//////////////////////////////////////////////////////////////////////////////
INT CDonatorMgr::InsertDonator()
{
	//========================================================================
	// ��������
	//========================================================================
	INT ids, nCurRow;
	CString strTmpData, strYear, strID, strFlag;
	CArray <CString, CString> RemoveAliasList;


	//========================================================================
	// 1. �����ڶ��� ����ְų� ������ �����ڰ� �����ϴ��� Ȯ���Ѵ�.
	//========================================================================
	ids = CheckValidateDonatorInfo();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�����ڸ��� �Է��ϼž� �մϴ�."));
		return -1;
	}
	else if(ids > 0) return ids;
	
	//========================================================================
	// 2. CM_�����ڸ� �̿��Ͽ� DM_�����ڰ����� �����Ѵ�.
	//========================================================================
	m_pDM->InsertRow(-1);
	nCurRow = m_pDM->GetRowCount() - 1;
	ids = SetDMRowbyCMData(m_pCM, m_pDM, nCurRow);
	if( ids < 0 )	return ids;

	//========================================================================
	// 3. DM�� �����Ѵ�.
	//========================================================================
	m_pDM->SetCellData(_T("�Է���"), GetTodayDate(), nCurRow);
	m_pDM->SetCellData(_T("�����۾�"), WORK_LOG, nCurRow);
	m_pDM->SetCellData(_T("�������۾�"), WORK_LOG, nCurRow);
	
	strTmpData = m_pDM->GetCellData(_T("������"), nCurRow);
	m_pCMakeIndex->GetKeyWord(strTmpData);	//���ξ� ����Ŭ���� - ���޵� ������ ��Ʈ���� ���ξ�� ��ȯ�Ѵ�.
	m_pDM->SetCellData(_T("�������ε���"), strTmpData, nCurRow);
	m_pDM->MakeRecKey(strTmpData);
	m_pDM->SetCellData(_T("����������KEY"), strTmpData, nCurRow);
	//strType, _T("CO"), strAcqYear, strDesc, strLastNo);

	// ��� ���̵� �����. = ��ϳ⵵+�����ڱ���+�Ϸù�ȣ(5�ڸ�)
	m_pCM->GetControlMgrData(_T("��ϳ⵵"), strYear);
	m_pCM->GetControlMgrData(_T("�����ڱ���"), strFlag);
	m_pCM->GetControlMgrData(_T("�Ϸù�ȣ"), strTmpData);
	INT nTmpData = _ttoi(strTmpData);
	strTmpData.Format(_T("%05d"), nTmpData);
	strID = strFlag+strYear+strTmpData;
	m_pDM->SetCellData(_T("���̵�"), strID, nCurRow);

	//=====================================================
	//2008.12.12 ADD BY PJW : �������Է��Ұ�쵵 MANAGE_CODE�� �߰��Ѵ�.
	m_pDM->SetCellData(_T("��������"), m_pInfo->MANAGE_CODE, nCurRow);
	//=====================================================

	
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("����å��"));
	RemoveAliasList.Add(_T("����å��"));

	//========================================================================
	// 4. ������ DM�� ������ �����Ͽ� ��� �����Ѵ�.
	//========================================================================
	m_pDM->StartFrame();
	m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, m_pDM->TBL_NAME, nCurRow, m_pDM);
	ids = m_pDM->SendFrame();
	if( ids < 0 ) return -1;
	ids = m_pDM->EndFrame();
	if( ids < 0 ) return -1;
	
	//========================================================================
	// 5. �Էµ� �����ڸ� �׸��忡 �߰��Ѵ�.
	//========================================================================
	m_pMainGrid->DisplayLine(nCurRow);

	//========================================================================
	// 6. ��������ȣ�� ������Ʈ�Ѵ�.
	//========================================================================
	strTmpData.Format(_T("%d"), nTmpData);
	ids = m_pLastNumber->SetLastNumber(_T("DON_ID"), _T("CO"), strYear, strFlag, strTmpData);
	if( ids < 0 ) return -1;

	return 0;
}

INT CDonatorMgr::UpdateDonator()
{
	INT ids;
	CString strTmpData, strYear, strID, strFlag, strKey, strSerialNo;
	CArray <CString, CString> RemoveAliasList;

	//========================================================================
	// 1. �����ڶ��� ����ְų� ������ �����ڰ� �����ϴ��� Ȯ���Ѵ�.
	//========================================================================
	ids = CheckValidateDonatorInfo();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("�����ڸ��� �Է��ϼž� �մϴ�."));
		return -1;
	}
	else if(ids > 0) return ids;

	//========================================================================
	// 2. CM_�����ڸ� �̿��Ͽ� DM_�����ڰ����� �����Ѵ�.
	//========================================================================
	ids = SetDMRowbyCMData(m_pCM, m_pDM, m_nIdx);


	//========================================================================
	// 3. DM�� �����Ѵ�.
	//========================================================================
	m_pDM->SetCellData(_T("�������۾�"), WORK_LOG, m_nIdx);
	
	strTmpData = m_pDM->GetCellData(_T("������"), m_nIdx);
	m_pCMakeIndex->GetKeyWord(strTmpData);
	m_pDM->SetCellData(_T("�������ε���"), strTmpData, m_nIdx);
	strKey = m_pDM->GetCellData(_T("����������KEY"), m_nIdx);
	
	/*
	m_pCM->GetControlMgrData(_T("��ϳ⵵"), strYear);
	m_pCM->GetControlMgrData(_T("�����ڱ���"), strFlag);
	m_pCM->GetControlMgrData(_T("�Ϸù�ȣ"), strSerialNo);
	strID = strFlag+strYear+strSerialNo;
	m_pDM->SetCellData(_T("���̵�"), strID, m_nIdx);
	*/

	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("����������KEY"));
	RemoveAliasList.Add(_T("���̵�"));
	RemoveAliasList.Add(_T("�����ڱ���"));
	RemoveAliasList.Add(_T("�Ϸù�ȣ"));
	RemoveAliasList.Add(_T("��ϳ⵵"));
	RemoveAliasList.Add(_T("����å��"));
	RemoveAliasList.Add(_T("����å��"));

	//========================================================================
	// 4. ������ DM�� ������ �����Ͽ� ��� �����Ѵ�.
	//========================================================================
	m_pDM->StartFrame();
	m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, m_pDM->TBL_NAME, m_nIdx, m_pDM, _T("REC_KEY"), _T("NUMERIC"), strKey);
	ids = m_pDM->SendFrame();
	if( ids < 0 )	return -1;
	ids = m_pDM->EndFrame();
	if( ids < 0 )	return -1;

	//========================================================================
	// 5. �׸��带 �ٽ� �׸���.
	//========================================================================
	m_pMainGrid->DisplayLine(m_nIdx);
	return 0;	
}

CString CDonatorMgr::GetTodayDate(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

INT CDonatorMgr::SetDMRowbyCMData(CESL_ControlMgr * pCM, CESL_DataMgr * pDM, INT nCurRow)
{
	INT ids, controlCnt, i;
	CString strTmp, alias;
	POSITION pos;
	CESL_Control_Element* pCE = NULL;

	if( pCM == NULL || pDM == NULL )		return -1000;

	controlCnt = pCM->Control_List.GetCount();	
	pos = pCM->Control_List.GetHeadPosition();

	for( i=0 ; i<controlCnt && pos != NULL; i++ )
	{
		pCE = NULL;
		pCE = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if( pCE == NULL )	continue;

		alias = pCE->GetAlias();
		ids = pCM->GetControlMgrData(alias, strTmp);
		if( ids < 0 )		continue;

		pDM->SetCellData(alias, strTmp, nCurRow);
	}
	
	return 0;
}

INT CDonatorMgr::CheckValidateDonatorInfo()
{
	INT ids ;
	CString strTmpData, strMsg;

	m_pCM->GetControlMgrData(_T("������"), strTmpData);
	if( strTmpData.IsEmpty() == TRUE )		return -1;

	ids = CheckDuplicateDonator(strTmpData);
	if(ids < 0)
	{
		strMsg.Format(_T("���� �̸��� ��ϵ� �����ڰ� �����մϴ�. �����Ͻðڽ��ϱ�?"));
		if( IDNO == ESLAfxMessageBox(strMsg, MB_YESNO) )	 return 1000;
	}

	return 0;
}

INT CDonatorMgr::CheckDuplicateDonator(CString strDonator)
{
	INT nRowCnt;
	CString strQuery;
	CESL_DataMgr tmpDM;

	if( m_nOpenMode == SINSERT ) 
	{
		//=====================================================
		//2009.01.12 UPDATE BY PJW : ������ �ߺ��˻��Ұ�� MANAGE_CODE�� ���ǿ� �ִ´�.
// 		strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE DONATOR='%s'"), m_pDM->TBL_NAME, strDonator);
		strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE DONATOR='%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), m_pDM->TBL_NAME, strDonator);		
		//=====================================================
	}
	else
	{
		//=====================================================
		//2009.01.12 UPDATE BY PJW : ������ �ߺ��˻��Ұ�� MANAGE_CODE�� ���ǿ� �ִ´�.
// 		strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE DONATOR='%s' AND REC_KEY != %s"), 
// 						m_pDM->TBL_NAME, strDonator, m_pDM->GetCellData(_T("����������KEY"), m_nIdx));

		strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE DONATOR='%s' AND REC_KEY != %s AND MANAGE_CODE = UDF_MANAGE_CODE"), 
 						m_pDM->TBL_NAME, strDonator, m_pDM->GetCellData(_T("����������KEY"), m_nIdx));
		//=====================================================
		
	}
		
	
	tmpDM.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	tmpDM.RestructDataManager(strQuery);
	
	nRowCnt = tmpDM.GetRowCount();
	if( nRowCnt > 0 )	return -1;
	else return 0;
}

VOID CDonatorMgr::OnKillfocusedtDONATORNAME() 
{
	/*
	CString strDonator;
	m_pCM->GetControlMgrData(_T("������"), strDonator);
	INT ids = CheckDuplicateDonator(strDonator);
	if(ids < 0) 
	{
		ESLAfxMessageBox(_T("�ߺ��� �����ڰ� �����մϴ�."));
	}
	*/
}

HBRUSH CDonatorMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (IsColorControl(pWnd->GetDlgCtrlID())) {
		return SetColor(pDC);
	}

	return hbr;

}

INT CDonatorMgr::GetFirstSelectIndex()
{
	INT nFirstSelIdx = -1;
	if(m_nOpenMode == GUPDATE)
	{
		nFirstSelIdx = m_pMainGrid->GetRow() - 1;
	}
	else if(m_nOpenMode == SUPDATE)
	{
		if(m_bGridSelectYN)
			nFirstSelIdx = m_firstidx;
		else
			nFirstSelIdx = m_pMainGrid->GetRow() - 1;
	}
	if(nFirstSelIdx < 0) nFirstSelIdx = 0;
	return nFirstSelIdx;
}

INT CDonatorMgr::GetPrevSelectIndex()
{
	//===============================
	//2009.04.08 UPDATE BY PJW : �񱳸� ���ؼ� �ʱⰪ�� 0���� �����Ѵ�.
// 	INT nPrevSelIndex = -1;
 	INT nPrevSelIndex = 0;
	//===============================
	
	if(m_bGridSelectYN)
	{
		//================================================
		//2009.04.08 ADD BY PJW : �������� ������ ����,���� �Ұ�� �ε����� ���� ����
		while( 1 )
		{
			if(nPrevSelIndex == m_nIdx || nPrevSelIndex < 0 )
			{
				break;
			}
			else if( nPrevSelIndex < m_nIdx )
			{
				nPrevSelIndex = m_pMainGrid->SelectGetNext();
				if( nPrevSelIndex < 0 )
				{
					nPrevSelIndex = m_pMainGrid->SelectGetTailPosition();
					break;
				}
			}
			else if( nPrevSelIndex > m_nIdx )
			{
				nPrevSelIndex = m_pMainGrid->SelectGetPrev();
			}
		}
		//================================================

		nPrevSelIndex = m_pMainGrid->SelectGetPrev();
		if(nPrevSelIndex < 0)
		{
			nPrevSelIndex = m_pMainGrid->SelectGetHeadPosition();
		}
	}
	else
	{
		if(m_nIdx != m_firstidx)
			nPrevSelIndex = m_nIdx - 1;
	}
	return nPrevSelIndex;
}

INT CDonatorMgr::GetNextSelectIndex()
{
	//===============================
	//2009.04.08 UPDATE BY PJW : �񱳸� ���ؼ� �ʱⰪ�� 0���� �����Ѵ�.
// 	INT nNextSelIndex = -1;
 	INT nNextSelIndex = 0;
	//===============================
	
	if(m_bGridSelectYN)
	{
		//================================================
		//2009.04.08 ADD BY PJW : �������� ������ ����,���� �Ұ�� �ε����� ���� ����
		while( 1 )
		{
			if(nNextSelIndex == m_nIdx || nNextSelIndex < 0 )
			{
				break;
			}
			else if( nNextSelIndex < m_nIdx )
			{
				nNextSelIndex = m_pMainGrid->SelectGetNext();
			}
			else if( nNextSelIndex > m_nIdx )
			{
				nNextSelIndex = m_pMainGrid->SelectGetPrev();
			}
		}
		//================================================	

		nNextSelIndex = m_pMainGrid->SelectGetNext();
		if(nNextSelIndex < 0)
		{
			nNextSelIndex = m_pMainGrid->SelectGetTailPosition();
		}
	}
	else
	{
		if(m_nIdx != m_lastidx)
			nNextSelIndex = m_nIdx + 1;
	}
	return nNextSelIndex;
}

VOID CDonatorMgr::OnKillfocusedtDONATORYEAR() 
{
	OnSelchangecmbDONATORFLAG();	
}

/////////////////////////////////////////////////////////////////////////////
// [ �Լ����� ]
// - �Է½� �ο��� �Ϸù�ȣ�� ���� ���Ѵ�.
//
// [ �̴Ͻ��� ]
// 1. ��ϳ⵵, �����ڱ����� ������ �Ϸù�ȣ�� ���������� ���Ѵ�.
// 2. ���ؿ� �Ϸù�ȣ�� ���� +1�� �Ͽ� ���ο� �Ϸù�ȣ�� �ο��Ѵ�.
//
// 2005.10.19 ������ �߰�
/////////////////////////////////////////////////////////////////////////////
VOID CDonatorMgr::OnSelchangecmbDONATORFLAG() 
{
	INT nSerialNo;
	CString strYear, strFlag, strSerialNo;

	//=======================================================================
	// 1. ��ϳ⵵, �����ڱ����� ������ �Ϸù�ȣ�� ���������� ���Ѵ�.
	//=======================================================================
	m_pCM->GetControlMgrData(_T("��ϳ⵵"), strYear);
	m_pCM->GetControlMgrData(_T("�����ڱ���"), strFlag);	
	m_pLastNumber->GetLastNumber(_T("DON_ID"), _T("CO"), strYear, strFlag, strSerialNo);
	
	//=======================================================================
	// 2. ���ؿ� �Ϸù�ȣ�� ���� +1�� �Ͽ� ���ο� �Ϸù�ȣ�� �ο��Ѵ�.
	//=======================================================================
	nSerialNo = _ttoi(strSerialNo);
	nSerialNo++;
	strSerialNo.Format(_T("%d"), nSerialNo);
	m_pCM->SetControlMgrData(_T("�Ϸù�ȣ"), strSerialNo);
}



