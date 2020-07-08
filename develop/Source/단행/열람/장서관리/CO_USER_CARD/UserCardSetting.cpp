// UserCardSetting.cpp : implementation file
//

#include "stdafx.h"
#include "UserCardSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserCardSetting dialog


CUserCardSetting::CUserCardSetting(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserCardSetting)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
// �߰� : DeleteObject���� �Ҹ��� �߰� ��� [2008-05-15]
CUserCardSetting::~CUserCardSetting()
{
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]
}

VOID CUserCardSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserCardSetting)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserCardSetting, CDialog)
	//{{AFX_MSG_MAP(CUserCardSetting)
	ON_CBN_SELCHANGE(IDC_cmbSFORMAT, OnSelchangecmbSFORMAT)
	ON_BN_CLICKED(IDC_btnSPREVIEW_CLOSE, OnbtnSPREVIEWCLOSE)
	ON_BN_CLICKED(IDC_btnSFORMSAVE, OnbtnSFORMSAVE)
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserCardSetting message handlers

BOOL CUserCardSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	InitESL_Mgr( _T("SM_ȸ����ī�弳��") );

	InitFormatList();

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_ȸ����ī���������") , _T("ȸ����") );
	pGrid->m_bGridEdit = TRUE;
	for( INT i = 2 ; i < 13 ; i ++ )
		pGrid->SELEDITOR_AddColumn(i);
	

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


INT CUserCardSetting::InitFormatList()
{
	INT ids;
	
	CESL_DataMgr* pFormatDM = FindDM( _T("DM_ȸ�����������") );
	
	pFormatDM->RefreshDataManager(_T(""));
	
	CString sInsertCode[2];
	
	CESL_ControlMgr* pCM = FindCM( _T("CM_ȸ�����������") );
	
	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("��ĸ���Ʈ")));
	if( pCMCB == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ChangeScreenFromRFID") );
	
	pCMCB->InitMultiComboBox(2);
		
	for( INT i = 0 ; i < pFormatDM->GetRowCount() ; i++ )
	{
		pFormatDM->GetCellData( _T("����̸�") , i , sInsertCode[0] );
		pFormatDM->GetCellData( _T("�ո鿩��") , i , sInsertCode[1] );
		
		if( sInsertCode[1].Compare(_T("0")) == 0 )
			sInsertCode[1] = _T("�ո�");
		else
			sInsertCode[1] = _T("�޸�");
		
		ids = pCMCB->SetData( sInsertCode , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ChangeScreenFromRFID") );
	}
	
	pCMCB->SetCurSel(0);
	
	//MakeCardFormatInfo();	
	
	return 0;
}


VOID CUserCardSetting::OnSelchangecmbSFORMAT() 
{
	MakeCardFormatInfo();	
}


INT CUserCardSetting::MakeCardFormatInfo()
{
	/// ��� ������ ����
	INT nIndex;

	nIndex = ((CComboBox*)GetDlgItem(IDC_cmbSFORMAT))->GetCurSel();

	CESL_DataMgr* pFormatDM = FindDM( _T("DM_ȸ�����������") );
	
	CString sFormKey;
	pFormatDM->GetCellData( _T("������KEY") , nIndex , sFormKey );
	
	CString sWhere;
	sWhere.Format( _T("CUCF.REC_KEY = %s ") , sFormKey );
	
	/// DB���� ���ĺ� ī�� ��� ���� ��������
	CESL_DataMgr* pCardDM = FindDM(_T("DM_������ī���������"));
	pCardDM->FreeData();
	pCardDM->RefreshDataManager(sWhere);
	
	AllControlDisplay(_T("CM_ȸ����ī���������"));

	return 0;
}

BOOL CUserCardSetting::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CUserCardSetting::OnbtnSPREVIEWCLOSE() 
{
	OnCancel();	
}

VOID CUserCardSetting::OnbtnSFORMSAVE() 
{
	SaveSetting();	
}

INT CUserCardSetting::GetFormat()
{
	CString sFileName = _T("..\\cfg\\UserCardFormat.cfg");
	CFileFind ff;
	if(!ff.FindFile(sFileName))
	{ // ���� ���ٸ� 0
		return 0;
	}
	else
	{ // �����ִٸ� ���Ͽ� ������ ������ ����
		CString sValue;
		CStdioFile fp;
		if(!fp.Open(sFileName, CFile::modeRead | CFile::typeBinary))
		{
			return 0;
		}
		TCHAR cUni;
		fp.Read(&cUni, sizeof(TCHAR));
		if(0xFEFF != cUni)
		{
			fp.SeekToBegin();
		}
		
		fp.ReadString(sValue);
		fp.Close();
		sValue.TrimLeft();	sValue.TrimRight();
		
		return _ttoi(sValue)-1;
	}

	return 0;
}

INT CUserCardSetting::SaveSetting()
{
	CESL_DataMgr* pDM = FindDM(_T("DM_������ī���������"));

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_ȸ����ī���������") , _T("ȸ����") );

	const INT nCnt = 11;
	CString sDBField[nCnt] = 
	{
		_T("START_POINT_X") , _T("START_POINT_Y") , _T("WIDTH") , _T("HEIGHT") , _T("FONT_NAME") , _T("FONT_SIZE") , _T("SHOW_TEXT_START_POS") , _T("SHOW_TEXT_CNT") , _T("REPLACE_CHAR") , _T("SPACE_CHAR") , _T("TEXT")
	};
	CString sType[nCnt] = 
	{
		_T("NUMERIC") , _T("NUMERIC") , _T("NUMERIC") , _T("NUMERIC") , _T("STRING") , _T("NUMERIC") , _T("NUMERIC") , _T("NUMERIC") , _T("STRING") , _T("NUMERIC") , _T("STRING")
	};
	CString sDMField[nCnt] = 
	{
		_T("������ġ_X") , _T("������ġ_Y") , _T("����") , _T("����") , _T("�۲�") , _T("ũ��") , _T("���̴¹��ڽ�����ġ") , _T("���̴¹��ڼ�") , _T("��ü����") , _T("�ڰ�") , _T("����")
	};
	
	CString sData[nCnt];

	pDM->StartFrame();

	CString sRecKey;
	for( INT i = 0 ; i < pGrid->GetCount() ; i++ )
	{
		pDM->GetCellData( _T("����������KEY") , i , sRecKey );

		pDM->InitDBFieldData();

		for( INT j = 0 ; j < nCnt ; j++ )
		{
			sData[j] = _T("");
			pGrid->GetAt( i , j+2 , sData[j] );

			pDM->SetCellData( sDMField[j] , sData[j] , i );

			if( sData[j].IsEmpty() && (sType[j].Compare(_T("NUMERIC"))==0) ) 
				sData[j] = _T("0");

			pDM->AddDBFieldData( sDBField[j] , sType[j] , sData[j] );
		}

		pDM->MakeUpdateFrame(_T("CO_USER_CARD_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	}

	INT ids = pDM->SendFrame();
	pDM->EndFrame();
	if( ids != 0 )
	{
		AfxMessageBox( _T("����� ���������� �������� ���Ͽ����ϴ�.") );
		return 1;
	}

	AfxMessageBox( _T("�����Ͽ����ϴ�") );

	return 0;
}

HBRUSH CUserCardSetting::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 ��� [2008-05-15]
	���̾�α� & ����ƽ ������
	 ************************************/
	// ���⼭���� 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

void CUserCardSetting::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if(TRUE == bShow)
	{
		// 2009.10.15 UPDATE BY PWR : ������ ������ ������� ����
		CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_cmbSFORMAT);	
		pCmb->SetCurSel(GetFormat());

		MakeCardFormatInfo();
	}
}
