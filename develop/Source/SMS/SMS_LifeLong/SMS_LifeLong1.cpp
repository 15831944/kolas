// SMS_LifeLong1.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_LifeLong1.h"
#include "SMS_LifeLong_Sender.h"
#include "..\\SMS_06\\SMS_061.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMS_LifeLong dialog


CSMS_LifeLong::CSMS_LifeLong(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMS_LifeLong)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nDisplayMode = 0;
	m_sSMSType = _T("");
	m_sDMAlias = _T("DM_SMS_LifeLong");
	m_sCMAlias = _T("MainGrid");	
}

CSMS_LifeLong::~CSMS_LifeLong()
{
	FreeAllDM();
}

VOID CSMS_LifeLong::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMS_LifeLong)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMS_LifeLong, CDialog)
	//{{AFX_MSG_MAP(CSMS_LifeLong)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_LifeLong message handlers

BOOL CSMS_LifeLong::Create(CWnd* pParentWnd) 
{
EFS_BEGIN
	
	return CDialog::Create(IDD, pParentWnd);
EFS_END
	return -1;
}

VOID CSMS_LifeLong::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);

	/////////////////////////////////////////////////////////////////
	// 1. ȸ���� �׸���	
	/////////////////////////////////////////////////////////////////
	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gMAIN_GRID);
	if( pGrid->GetSafeHwnd() == NULL )
		return;
	//pGrid->MoveWindow(0, 0, cx+4, cy+4);
	pGrid->MoveWindow(0, 0, cx, cy);
	
	/////////////////////////////////////////////////////////////////
	// 2. ��ȸ���� �׸���	
	/////////////////////////////////////////////////////////////////
	pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gMAIN_GRID2);
	if( pGrid->GetSafeHwnd() == NULL )
		return;
	//pGrid->MoveWindow(0, 0, cx+4, cy+4);
	pGrid->MoveWindow(0, 0, cx, cy);

	/////////////////////////////////////////////////////////////////
	// 3. ȸ���� �׸���	
	/////////////////////////////////////////////////////////////////
	pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gMAIN_GRID3);
	if( pGrid->GetSafeHwnd() == NULL )
		return;
	//pGrid->MoveWindow(0, 0, cx+4, cy+4);
	pGrid->MoveWindow(0, 0, cx, cy);

	EFS_END	
}

////////////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// �� �ʱ�ȭ �� �⺻���� �ʱ�ȭ
////////////////////////////////////////////////////////////////////////////////////
BOOL CSMS_LifeLong::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//////////////////////////////////////////////////////////////////
	// 1. SM �ʱ�ȭ
	//////////////////////////////////////////////////////////////////
	if( InitESL_Mgr( _T("SMS_DLG_LifeLong") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : SMS_DLG_LifeLong") );
		return false;
	}

	//////////////////////////////////////////////////////////////////
	// 2. ���� �׸��� �ʱ�ȭ
	//////////////////////////////////////////////////////////////////
	InitDisplayGrid();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

////////////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// ���� �׸��� ����
////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong::InitDisplayGrid()
{
	if (m_nDisplayMode == 0)		// ���α׷�&&ȸ��
	{
		GetDlgItem(IDC_gMAIN_GRID)->ShowWindow(TRUE);
		GetDlgItem(IDC_gMAIN_GRID2)->ShowWindow(FALSE);		
		GetDlgItem(IDC_gMAIN_GRID3)->ShowWindow(FALSE);
	}
	else if (m_nDisplayMode == 1)	// ��ȸ��
	{
		GetDlgItem(IDC_gMAIN_GRID)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID2)->ShowWindow(TRUE);		
		GetDlgItem(IDC_gMAIN_GRID3)->ShowWindow(FALSE);
	}	
	else if (m_nDisplayMode == 2)	// ȸ��
	{
		GetDlgItem(IDC_gMAIN_GRID)->ShowWindow(FALSE);
		GetDlgItem(IDC_gMAIN_GRID2)->ShowWindow(FALSE);		
		GetDlgItem(IDC_gMAIN_GRID3)->ShowWindow(TRUE);
	}	

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// ��带 SET 
// ���� �˻� ��İ� �׸��忡 ������ ��ģ��.
////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong::SetDisplayMode(INT nMode)
{
	if ( m_nDisplayMode != nMode )
	{
		FreeAllDM();	
	}

	m_nDisplayMode = nMode;

	if (m_nDisplayMode == 0)
	{
		m_sDMAlias = _T("DM_SMS_LifeLong");
		m_sCMAlias = _T("MainGrid");
	}
	else if (m_nDisplayMode == 1)
	{
		m_sDMAlias = _T("DM_SMS_LifeLong_NonMember");
		m_sCMAlias = _T("NonMemberGrid");
	}
	else if (m_nDisplayMode == 2)
	{
		m_sDMAlias = _T("DM_SMS_LifeLong_Member");
		m_sCMAlias = _T("MemberGrid");
	}

	InitDisplayGrid();
	ControlDisplay( _T("CM_SMS_LifeLong"), m_sCMAlias );

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// �˻���� DM �ʱ�ȭ
////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong::FreeAllDM()
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_SMS_LifeLong") );
	pDM->FreeData();
	
	pDM = FindDM( _T("DM_SMS_LifeLong_NonMember") );
	pDM->FreeData();

	pDM = FindDM( _T("DM_SMS_LifeLong_Member") );
	pDM->FreeData();

	return 0;
	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// �˻�â���� ������ WHERE�� ����Ͽ� ȸ���� �˻��Ѵ�.
/////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong::SearchUSER(CString sQuery)
{
	EFS_BEGIN

	INT ids=0, cnt=0;

	//////////////////////////////////////////////////////////////////
	// 1. ȸ�� DM�� �ʱ�ȭ �Ѵ�.
	//////////////////////////////////////////////////////////////////
	CESL_DataMgr *pDM = FindDM( m_sDMAlias );
	if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong::SearchUSER") );
	pDM->FreeData();

	ids = pDM->RefreshDataManager(sQuery);
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong::SearchUSER") );
	
	cnt = pDM->GetRowCount();
	if(cnt < 1)		
	{
		AfxMessageBox(_T("�˻� ����� �����ϴ�."));
		ControlDisplay( _T("CM_SMS_LifeLong"), m_sCMAlias );
		return 0;
	}

	//////////////////////////////////////////////////////////////////
	// 2. ȸ�� �ּҸ� �Է� �Ѵ�.
	//    ��ȸ�� ��� ����
	//////////////////////////////////////////////////////////////////
	for (INT i = 0;i < cnt;i++)
	{
		if (m_nDisplayMode == 1) break;
		ids = ViewAddress(i);
		if(ids<0) return ids;
	}

	ControlDisplay( _T("CM_SMS_LifeLong"), m_sCMAlias );	
	AfxMessageBox(_T("�˻��� �Ϸ� �Ǿ����ϴ�"));

	return 0;
	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// �ּҿ� ���ּ� ������ ���ļ� �׸���� ȸ���ּҸ� �����.
/////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong::ViewAddress(INT nRow)
{
	EFS_BEGIN
	
	CESL_DataMgr *pDM = FindDM( m_sDMAlias );
	if(pDM==NULL) if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong::ViewAddress") );

	CString Address=_T(""), sTemp=_T("");
	
	pDM->GetCellData(_T("�ּ�"), nRow, sTemp);
	Address = sTemp + _T(" ");
	pDM->GetCellData(_T("���ּ�"), nRow, sTemp);
	Address += sTemp;
	pDM->SetCellData(_T("ȸ���ּ�"), Address, nRow);

	return 0;
	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// ���õ� ������� ����� ����� �߼��Ѵ�.
/////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong::SendSMS()
{
EFS_BEGIN
	
	INT ids;
	INT nCnt = 0, nRowCnt = 0;
	/////////////////////////////////////////////////////////////////////////
	// 1. ���� DM�� ã�´�.
	/////////////////////////////////////////////////////////////////////////
	CESL_DataMgr *pDMSrc = FindDM( m_sDMAlias );
	if(pDMSrc==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong::SendSMS") );

	nRowCnt = pDMSrc->GetRowCount();
	if(nRowCnt < 1){
		AfxMessageBox(_T("���Ϲ߼� ����ڰ� �����ϴ�"));
		return 0;
	}

	/////////////////////////////////////////////////////////////////////////
	// 2. ������ ����ڸ� Ȯ���Ѵ�.
	/////////////////////////////////////////////////////////////////////////
	CESL_ControlMgr *pCM = FindCM(_T("CM_SMS_LifeLong"));
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong::SendSMS") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( m_sCMAlias ));
	if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CSMS_LifeLong::SendSMS") );
	
	pGrid->SelectMakeList();
	nCnt = pGrid->SelectGetCount();
	if(nCnt < 1){
		AfxMessageBox(_T("������ ����ڰ� �����ϴ�."));
		return 0;
	}
	
	/////////////////////////////////////////////////////////////////////////
	// 3. ������ ����ڸ� �߼ۿ� DM���� �����Ѵ�.
	/////////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pDMSend = new CESL_DataMgr;
	ids = CLocCommonAPI::MakeCopyDM( this , pDMSrc , pDMSend );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CSMS_LifeLong::SendSMS") );

	pDMSend->FreeData();
	nCnt = 0;

	CString sTemp =_T("");
	for(INT i=0;i<nRowCnt;i++){
		sTemp = pGrid->GetTextMatrix(i+1, 1);
		if(sTemp == _T("V")){
			pDMSend->InsertRow(-1);
			ids = CLocCommonAPI::AddDM(pDMSrc, pDMSend, i, nCnt, this );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CSMS_LifeLong::SendSMS") );
			nCnt++;
		}		
	}

	if(nCnt < 1){
		AfxMessageBox(_T("������ ����ڰ� �����ϴ�."));
		return 0;
	}

	/////////////////////////////////////////////////////////////////////////
	// 4. �߼� ���̾�α׸� ����.
	/////////////////////////////////////////////////////////////////////////
	CSMS_LifeLong_Sender SenderDlg(this);
	SenderDlg.SetInfo(pDMSend);
	SenderDlg.DoModal();
	
	delete pDMSend;
	return 0;
EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// ���� �߼�
/////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong::SendOneSMS()
{
	CSMS_06 dlg(this);
	dlg.DoModal();
	return 0;
}	
