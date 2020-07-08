#include "stdafx.h"
#include "resource.h"
#include "BOOK_COOPERATIVE_GROUP_MAIN.h"
#include "ILL_MODIFY.h"
#include "ILL_SEARCH.h"
#include "..\\BL_LOC_LOAN\\CfgFileApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBOOK_COOPERATIVE_GROUP_MAIN::CBOOK_COOPERATIVE_GROUP_MAIN(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{	
	m_nMode			= -1;
	mCtrl_pDlg_Offer	= NULL;
	mCtrl_pDlg_Request	= NULL;

	m_pLoanShareManager = NULL;
	m_pLoanMangeValue = NULL;

}
CBOOK_COOPERATIVE_GROUP_MAIN::~CBOOK_COOPERATIVE_GROUP_MAIN()
{
	if(NULL != mCtrl_pDlg_Offer)
	{
		delete mCtrl_pDlg_Offer;
		mCtrl_pDlg_Offer = NULL;
	}
	if(NULL != mCtrl_pDlg_Request)
	{
		delete mCtrl_pDlg_Request;
		mCtrl_pDlg_Request = NULL;
	}
 	if(NULL != m_pLoanShareManager)
 	{
 		delete m_pLoanShareManager;
 		m_pLoanShareManager = NULL;
 	}
 	if(NULL != m_pLoanMangeValue)
 	{
 		delete m_pLoanMangeValue;
 		m_pLoanMangeValue = NULL;
 	}
}

void CBOOK_COOPERATIVE_GROUP_MAIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBOOK_COOPERATIVE_GROUP_MAIN) //}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TAB_MAIN, mCtrl_TabMain);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBOOK_COOPERATIVE_GROUP_MAIN, CDialog)
	//{{AFX_MSG_MAP(CBOOK_COOPERATIVE_GROUP_MAIN)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, OnSelchangeTabMain)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CBOOK_COOPERATIVE_GROUP_MAIN::Create(CWnd *pParent)
{
	return CDialog::Create(IDD, pParent);
}

void CBOOK_COOPERATIVE_GROUP_MAIN::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);	
	
	ResizeControl();
}

BOOL CBOOK_COOPERATIVE_GROUP_MAIN::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	
  	m_pLoanShareManager = new CLoanShareManager(this);
 
 	m_pLoanMangeValue = new CLonaManageValue(this);
 	m_pLoanMangeValue->pMain = pMain;
 	m_pLoanMangeValue->pParentMgr = (CESL_Mgr*)this;
 	m_pLoanMangeValue->SM_STYLE = 'T';
	m_pLoanMangeValue->m_pInfo = this->m_pInfo;
 
 	m_pLoanMangeValue->DoModal();

	mCtrl_TabMain.InsertItem(IDX_REQUEST,	_T("��û�ڷ�"));
	mCtrl_TabMain.InsertItem(IDX_OFFER,	_T("�����ڷ�"));

	mCtrl_pDlg_Offer = new CBOOK_COOPERATIVE_GROUP_OFFER(this);
	mCtrl_pDlg_Request = new CBOOK_COOPERATIVE_GROUP_REQUEST(this);

	mCtrl_pDlg_Offer->m_pInfo = m_pInfo;
	mCtrl_pDlg_Request->m_pInfo = m_pInfo;

	mCtrl_pDlg_Offer->m_pLoanShareManager = m_pLoanShareManager;
	mCtrl_pDlg_Offer->m_pLoanMangeValue = m_pLoanMangeValue;

	mCtrl_pDlg_Request->m_pLoanShareManager = m_pLoanShareManager;
	mCtrl_pDlg_Request->m_pLoanMangeValue = m_pLoanMangeValue;


	{ 
	CString strLibCode = _T("");
	CString strLibTel = _T("");
	CString strLibName = _T("");

	CESL_DataMgr pTmpDM;
	pTmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	CString strQuery;
	strQuery.Format(
		_T("SELECT LIB_CODE, SMS_CALLBACK_PHONE, LIB_NAME FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE='%s'"),
		m_pInfo->MANAGE_CODE
		);

	pTmpDM.RestructDataManager(strQuery);

	pTmpDM.GetCellData(0, 0, strLibCode);
	pTmpDM.GetCellData(0, 1, strLibTel);
	pTmpDM.GetCellData(0, 2, strLibName);

	if(_T("") == strLibCode)
	{
		AfxMessageBox(_T("������ �ڵ带 �������� ��, �����Ͽ����ϴ�."));
		CDialog::OnCancel();
	}	

	
	
	m_ManageCodeListDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	strQuery.Format(_T("SELECT MANAGE_CODE FROM ESL_MANAGE_TBL WHERE VALUE_NAME = 'å�η���뿩��' AND VALUE_1 = 'Y'"));
	m_ManageCodeListDM.RestructDataManager(strQuery);
	mCtrl_pDlg_Offer->m_pManageCodeList = &m_ManageCodeListDM;
	mCtrl_pDlg_Request->m_pManageCodeList = &m_ManageCodeListDM;
	CString strTmp = m_ManageCodeListDM.GetCellData(0, 0);


	
	CCfgFileApi cfgFileApi;
	CString strCfgFilePath = _T("..\\cfg\\SMS_Callback.cfg");
	cfgFileApi.SetFile(strCfgFilePath);
	CFileFind ff;
	if(ff.FindFile(strCfgFilePath))
	{
		CString strCallbackTarget;
		cfgFileApi.GetData(_T("ȸ�Ź�ȣ����������"), strCallbackTarget);
		CString strValue;
		if(_T("1") == strCallbackTarget)
		{
			cfgFileApi.GetData(_T("ȸ�Ź�ȣ"), strValue);
			if(!strValue.IsEmpty()) strLibTel = strValue;
		}
		CString strLibNameTarget;
		cfgFileApi.GetData(_T("����������������"), strLibNameTarget);
		if(_T("1") == strLibNameTarget)
		{
			cfgFileApi.GetData(_T("��������"), strValue);
			if(!strValue.IsEmpty()) strLibName = strValue;
		}
	}

	m_strLibCode = strLibCode;
	mCtrl_pDlg_Request->m_strLibName = strLibName;
	mCtrl_pDlg_Request->m_strLibTel = strLibTel;
	mCtrl_pDlg_Request->m_strLibCode = strLibCode;
	mCtrl_pDlg_Offer->m_strLibName = strLibName;
	mCtrl_pDlg_Offer->m_strLibTel = strLibTel;
	mCtrl_pDlg_Offer->m_strLibCode = strLibCode;	
	
	} 
	
	CString strValue;
	INT ids = m_pLoanMangeValue->GetManageValue(_T("����"), _T("����"), _T("å�η��ý���"), _T("å�η�����"), strValue);
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1210, _T("MakeManageValue"));
	}
	if(strValue.IsEmpty()) 
	{
		mCtrl_pDlg_Request->m_sBookCooperativeMode = _T("I");
		mCtrl_pDlg_Offer->m_sBookCooperativeMode = _T("I");
		m_sBookCooperativeMode = _T("I");
	}
	else
	{
		m_sBookCooperativeMode = strValue;
		mCtrl_pDlg_Offer->m_sBookCooperativeMode = strValue;
		mCtrl_pDlg_Request->m_sBookCooperativeMode = strValue;
	}

	mCtrl_pDlg_Offer->Create(this);
	mCtrl_pDlg_Request->Create(this);

	SetMode(IDX_REQUEST);

	return TRUE;  
	              

	EFS_END

	PostMessage(WM_QUIT);

	return FALSE;
}

BOOL CBOOK_COOPERATIVE_GROUP_MAIN::ResizeControl()
{
	CRect rDlg = CRect(0, 0, 0, 0);
	GetClientRect(rDlg);

	if(NULL == mCtrl_TabMain.GetSafeHwnd()) return FALSE;
	mCtrl_TabMain.MoveWindow(0, 0, rDlg.Width(), 20);
	CRect rSubDlg = rDlg;
	rSubDlg.right -= 4;
	rSubDlg.bottom -= 20;
	rSubDlg.OffsetRect(2, 20);

	if(NULL == mCtrl_pDlg_Offer || NULL == mCtrl_pDlg_Offer->GetSafeHwnd()) return FALSE;
	mCtrl_pDlg_Offer->MoveWindow(rSubDlg);

	if(NULL == mCtrl_pDlg_Request || NULL == mCtrl_pDlg_Request->GetSafeHwnd()) return FALSE;
	mCtrl_pDlg_Request->MoveWindow(rSubDlg);
	
	return TRUE;
}

BOOL CBOOK_COOPERATIVE_GROUP_MAIN::DestroyWindow() 
{
	
	if(NULL != mCtrl_pDlg_Offer && NULL != mCtrl_pDlg_Offer->GetSafeHwnd())
		mCtrl_pDlg_Offer->DestroyWindow();
	if(NULL != mCtrl_pDlg_Request && NULL != mCtrl_pDlg_Request->GetSafeHwnd())
		mCtrl_pDlg_Request->DestroyWindow();

	return CDialog::DestroyWindow();
}

int CBOOK_COOPERATIVE_GROUP_MAIN::ShowDlg(int nIdx)
{
	if(IDX_OFFER == nIdx)
	{		
		mCtrl_pDlg_Request->ShowWindow(SW_HIDE);
		mCtrl_pDlg_Offer->ShowWindow(SW_SHOW);
	}
	else if(IDX_REQUEST == nIdx)
	{
		mCtrl_pDlg_Request->ShowWindow(SW_SHOW);
		mCtrl_pDlg_Offer->ShowWindow(SW_HIDE);
	}
	else
		return -1;
	return 0;
}

void CBOOK_COOPERATIVE_GROUP_MAIN::OnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	SetMode(mCtrl_TabMain.GetCurSel());
	*pResult = 0;
}

BOOL CBOOK_COOPERATIVE_GROUP_MAIN::SetMode(int nMode)
{
	if(IDX_OFFER != nMode && IDX_REQUEST != nMode) return FALSE;

	mCtrl_TabMain.SetCurSel(nMode);
	ShowDlg(nMode);
	ChangeToolBarButton(nMode);
	m_nMode = nMode;

	return 0;
}

int CBOOK_COOPERATIVE_GROUP_MAIN::ChangeToolBarButton(int nIdx)
{
	int ids = 0;
	CString strFuncName = _T("CBOOK_COOPERATIVE_GROUP_MAIN::ChangeToolBarButton(int nIdx)");

	const int OfferBtnCnt = 14;

	const int RequestBtnCnt = 18;

	CString strBtnAlias_Request[RequestBtnCnt] = 
	{
		_T("��û_��û"),
		_T("��û_��û����"),
		_T("��û_��û���"),
		_T("��û_�Լ�"),
		_T("��û_�Լ����"),
		_T("��û_����"),
		_T("��û_�ݳ�"),
		_T("��û_����"),
		_T("��û_�߼۰���Ȯ��"),
		_T("��û_���"),
		_T("��û_��������"),
		_T("��û_�̿�������"),
		_T("��û_���������"),
		_T("��û_��������"),
		_T("��û_���"),
		_T("������"),
		_T("��û_�˻�")		
	};
	CString strBtnAlias_Offer[OfferBtnCnt] = 
	{
		_T("����_�߼�"),
		_T("����_�߼۰���"),
		_T("����_�߼����"),
		_T("����_����(�Ϸ�)"),
		_T("����_���"),
		_T("����_��������"),
		_T("����_�̿�������"),
		_T("����_���������"),
		_T("����_��������"),
		_T("����_�˻�"),
		_T("������"),
		_T("RFID���ȼ���"),
		_T("����_�߼۰������"),
		_T("����_�����߼��������")
	};

	CString* strHideAlias = (IDX_REQUEST == nIdx) ? strBtnAlias_Offer : strBtnAlias_Request;
	CString* strShowAlias = (IDX_REQUEST == nIdx) ? strBtnAlias_Request : strBtnAlias_Offer;

	int nHideCount = (IDX_REQUEST == nIdx) ? OfferBtnCnt : RequestBtnCnt;
	int nShowCount = (IDX_REQUEST == nIdx) ? RequestBtnCnt : OfferBtnCnt;

	{ 
	CString strAlias = _T("");
	for(int nCnt = 0; nCnt < nHideCount; nCnt++)
	{
		strAlias = strHideAlias[nCnt];
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strAlias, TRUE);
	}
	} 

	{ 
	CString strAlias = _T("");
	for(int nCnt = 0; nCnt < nShowCount; nCnt++)
	{
		strAlias = strShowAlias[nCnt];
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strAlias, FALSE);
	}
	} 
	
	{
	BOOL bHideToolbar = TRUE;

	if(IDX_REQUEST == nIdx)
	{
		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

		CString strQuery;
		CString strValue;
		strQuery.Format(
			_T("SELECT VALUE_1 ")
			_T("FROM ILL_MANAGE_TBL ")
			_T("WHERE VALUE_NAME = '��û��ɻ������' AND MANAGE_CODE = '%s'"),
			m_pInfo->MANAGE_CODE
			);

		ids = TmpDM.GetOneValueQuery(strQuery, strValue);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);
	
		bHideToolbar =  (_T("Y") == strValue) ? FALSE : TRUE;
	}

	CString strAlias = _T("��û_��û");
	pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strAlias, bHideToolbar);
	
	}

#ifdef _RFID
	if(!m_pInfo->GetRFIDUseMode())
#endif
	{
		CString strAlias = _T("RFID���ȼ���");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strAlias, TRUE);
	}

	
	if(_T("D") == m_sBookCooperativeMode)
	{
		CString strAlias = _T("���忩������");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strAlias, TRUE);
	}

	return 0;
}

int CBOOK_COOPERATIVE_GROUP_MAIN::GetMode()
{
	return m_nMode;
}

int CBOOK_COOPERATIVE_GROUP_MAIN::ExcutePath(CString strSM)
{
	EFS_BEGIN

	if(_T("�ɼǼ���") == strSM)
	{
		SetOption();
		return 0;
	}
	
	if(IDX_OFFER == GetMode())
	{
		return mCtrl_pDlg_Offer->ExcutePath(strSM);
	}
	
	else if(IDX_REQUEST == GetMode())
	{
		return mCtrl_pDlg_Request->ExcutePath(strSM);
	}

	return 0;

	EFS_END

	return -1;
}

void CBOOK_COOPERATIVE_GROUP_MAIN::SetOption()
{

}

CESL_Mgr* CBOOK_COOPERATIVE_GROUP_MAIN::CreateSearchDlg(CString strSM)
{
	CILL_SEARCH::MODE Mode;
	CESL_Mgr* pParentMgr = NULL;

	if(_T("��û_�˻�") == strSM)
	{
		pParentMgr = (CESL_Mgr*)mCtrl_pDlg_Request;
		Mode = CILL_SEARCH::MODE_REQUEST;
	}
	else if(_T("����_�˻�") == strSM)
	{
		pParentMgr = (CESL_Mgr*)mCtrl_pDlg_Offer;
		Mode = CILL_SEARCH::MODE_OFFER;
	}
	else return NULL;

	CILL_SEARCH* pDlg = new CILL_SEARCH(pParentMgr);
	pDlg->m_pInfo = m_pInfo;
		
	pDlg->SetMode(Mode, m_strLibCode);
	pDlg->CDialog::Create(CILL_SEARCH::IDD, pParentMgr);
	pDlg->CenterWindow();
	pDlg->ShowWindow(SW_SHOW);

	return pDlg;
}
