#include "Stdafx.h"
#include "SearchZipCodeDlgOpenAPI.h"
#include "SearchZipCodeDlgAuth.h"
#include "OpenAPI.h"
#include "CfgFileApi.h"

#include "..\\..\\����\\����\\�弭����\\LOC_CommonAPI\\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define OPENAPI_TAG_RESULTS			_T("results")
#define OPENAPI_TAG_COMMON			_T("common")
#define OPENAPI_TAG_TOTALCOUNT		_T("totalCount")
#define OPENAPI_TAG_CURRENTPAGE		_T("currentPage")
#define OPENAPI_TAG_COUNTPERPAGE	_T("countPerPage")
#define OPENAPI_TAG_ERRORCODE		_T("errorCode")
#define OPENAPI_TAG_ERRORMESSAGE	_T("errorMessage")
#define OPENAPI_TAG_JUSO			_T("juso")
#define OPENAPI_TAG_ROADADDR		_T("roadAddr")
#define OPENAPI_TAG_ROADADDRPART1	_T("roadAddrPart1")
#define OPENAPI_TAG_ROADADDRPART2	_T("roadAddrPart2")
#define OPENAPI_TAG_JIBUNADDR		_T("jibunAddr")
#define OPENAPI_TAG_ENGADDR			_T("engAddr")
#define OPENAPI_TAG_ZIPNO			_T("zipNo")
#define OPENAPI_TAG_ADMCD			_T("admCd")
#define OPENAPI_TAG_RNMGTSN			_T("rnMgtSn")
#define OPENAPI_TAG_BDMGTSN			_T("bdMgtSn")

#define OPENAPI_RESULT_FILE			_T("..\\cfg\\SearchRoadResult.xml")
#define OPENAPI_CONFIG_FILE			_T("..\\cfg\\LoanReturn\\newAddress_mode.txt")

#define OPENAPI_CONFIG_TAG_REGION		_T("��������")
#define OPENAPI_CONFIG_TAG_APPLYTYPE	_T("�ּ�����")

/////////////////////////////////////////////////////////////////////////////
// CSearchZipCodeDlgOpenAPI Dialog

CSearchZipCodeDlgOpenAPI::CSearchZipCodeDlgOpenAPI(CESL_Mgr* pParent)
	: CESL_Mgr(IDD, pParent)
{
	ASSERT(m_pInfo);

	CoInitialize(NULL);
}

CSearchZipCodeDlgOpenAPI::~CSearchZipCodeDlgOpenAPI()
{
	CoUninitialize();
}

void CSearchZipCodeDlgOpenAPI::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchZipCodeDlgOpenAPI)
	DDX_Control(pDX, IDC_CMB_REGION, m_cmbRegion);
	DDX_Control(pDX, IDC_CMB_APPLY_TYPE, m_cmbApplyType);
	DDX_Control(pDX, IDC_EDT_SEARCH, m_edtSearch);	
	DDX_Control(pDX, IDC_EDT_CURRENT_PAGE, m_edtCurrentPage);
	DDX_Control(pDX, IDC_EDT_TOTAL_PAGE, m_edtTotalPage);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSearchZipCodeDlgOpenAPI, CDialog)
	//{{AFX_MSG_MAP(CSearchZipCodeDlgOpenAPI)
	ON_CBN_SELCHANGE(IDC_CMB_REGION, OnSelchangeRegion)
	ON_CBN_SELCHANGE(IDC_CMB_APPLY_TYPE, OnSelchangeApplyType)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnbtnSearch)
	ON_BN_CLICKED(IDC_BTN_PREV_PAGE, OnbtnPrevPage)
	ON_BN_CLICKED(IDC_BTN_NEXT_PAGE, OnbtnNextPage)
	ON_BN_CLICKED(IDC_BTN_SETUP, OnbtnSetup)
	ON_BN_CLICKED(IDC_BTN_APPLY, OnbtnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CSearchZipCodeDlgOpenAPI, CDialog)
    //{{AFX_EVENTSINK_MAP(CSearchZipCodeDlgOpenAPI)
	ON_EVENT(CSearchZipCodeDlgOpenAPI, IDC_GRID_RESULT, -601 /* DblClick */, OnDblClickGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchZipCodeDlgOpenAPI message handlers


BOOL CSearchZipCodeDlgOpenAPI::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		if (GetDlgItem(IDC_BTN_SEARCH)->IsWindowEnabled())
		{
			OnbtnSearch();
		}
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CSearchZipCodeDlgOpenAPI::OnInitDialog() 
{
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd());

	if(InitESL_Mgr(_T("SM_���ּ�OPENAPI�˻�")) < 0)
	{
		MessageBox(_T("InitESL_Mgr Error!"), _T("ERROR"));
		return FALSE;
	}
	
	// ����Ű Ȯ��
	InitAuthKey();
	// ��Ʈ�� �ʱ�ȭ
	InitControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSearchZipCodeDlgOpenAPI::OnSelchangeRegion()
{
	CCfgFileApi FileApi;
	FileApi.SetFile(OPENAPI_CONFIG_FILE);

	CString strData;
	strData.Format(_T("%d"), m_cmbRegion.GetCurSel());

	FileApi.SetData(OPENAPI_CONFIG_TAG_REGION, strData);
}

VOID CSearchZipCodeDlgOpenAPI::OnSelchangeApplyType()
{
	CCfgFileApi FileApi;
	FileApi.SetFile(OPENAPI_CONFIG_FILE);

	CString strData;
	strData.Format(_T("%d"), m_cmbApplyType.GetCurSel());

	FileApi.SetData(OPENAPI_CONFIG_TAG_APPLYTYPE, strData);
}

VOID CSearchZipCodeDlgOpenAPI::OnbtnSearch()
{
	CString strText;
	m_edtSearch.GetWindowText(strText);
	strText.TrimLeft();strText.TrimRight();

	if (strText.IsEmpty())
	{
		AfxMessageBox(_T("�˻�� �Է����ֽñ� �ٶ��ϴ�"), MB_ICONINFORMATION);
		m_edtSearch.SetFocus();
		return;
	}
	
	// ���������� �߰��Ѵ�
	INT idx = m_cmbRegion.GetCurSel();
	if (idx > 0)
	{
		CString strRegion;
		m_cmbRegion.GetLBText(idx, strRegion);
		strText = (strRegion + _T(" ") + strText);
	}

	SearchBeforeAction();

	SearchOpenAPI(strText, 1);

	SearchAfterAction();
}

VOID CSearchZipCodeDlgOpenAPI::OnbtnPrevPage()
{
	INT nCurrentPage = m_CommonInfo.CURRENTPAGE;

	CString strKeyWord = m_CommonInfo.LASTKEYWORD;

	SearchBeforeAction();

	SearchOpenAPI(strKeyWord, nCurrentPage - 1);

	SearchAfterAction();
}

VOID CSearchZipCodeDlgOpenAPI::OnbtnNextPage()
{
	INT nCurrentPage = m_CommonInfo.CURRENTPAGE;

	CString strKeyWord = m_CommonInfo.LASTKEYWORD;

	SearchBeforeAction();

	SearchOpenAPI(strKeyWord, nCurrentPage + 1);

	SearchAfterAction();
}

VOID CSearchZipCodeDlgOpenAPI::OnbtnSetup()
{
	// �׽�Ʈ�� ����
	CString strBeforeKey = m_strAuthKey;

	while (TRUE)
	{
		CSearchZipCodeDlgAuth dlg;
		if (dlg.DoModal() == IDOK)
		{			
			m_strAuthKey = dlg.GetAuthKey();

			// �˻��� �õ��Ͽ� ��ȿ�� ����Ű���� Ȯ���Ѵ�
			SearchOpenAPI(_T("�����߾ӵ�����"), 1, TRUE);

			if (m_CommonInfo.ERRORCODE != _T("0"))
			{
				if (IDYES == AfxMessageBox(_T("��ȿ���� �ʴ� ����Ű�Դϴ�. �ٽ� �Է��Ͻðڽ��ϱ�?"), MB_YESNO|MB_ICONQUESTION))
				{
					continue;
				}
			}
			else
			{
				if (strBeforeKey.IsEmpty())
				{
					strBeforeKey = m_strAuthKey;
				}

				CESL_DataMgr* pApiDM = FindDM(_T("DM_���ּ�OPENAPI_�˻����"));
				ASSERT(pApiDM);
				pApiDM->StartFrame();

				// DB UPDATE
				if (m_strAuthRecKey.IsEmpty())
				{
					// INSERT
					CString strQuery;
					strQuery.Format(_T("INSERT INTO ESL_MANAGE_TBL(REC_KEY, GROUP_1, GROUP_2, GROUP_3, CLASS_ALIAS, CLASS_DESC, VALUE_NAME, VALUE_1, SYS_SETUP_YN, MANAGE_CODE) ")
									_T("VALUES (ESL_SEQ.NEXTVAL, 'L', '��Ÿ', '����', '���θ��ּҰ˻�', '���������� ���θ��ּ� �˻��� ����Ű', '����Ű', '%s', 'Y', '%s');"),
									m_strAuthKey, m_pInfo->MANAGE_CODE );

					pApiDM->AddFrame(strQuery);
				}
				else
				{
					// UPDATE
					CString strQuery;
					strQuery.Format(_T("UPDATE ESL_MANAGE_TBL SET VALUE_1='%s' WHERE REC_KEY=%s;"), m_strAuthKey, m_strAuthRecKey);

					pApiDM->AddFrame(strQuery);
				}

				if (pApiDM->SendFrame() < 0)
				{
					AfxMessageBox(_T("[���θ��ּ� �ȳ��ý���] ����Ű ���忡 �����Ͽ����ϴ�"), MB_ICONSTOP);
				}

				pApiDM->EndFrame();
			}
		}

		break;
	}

	// ���������� ����
	m_strAuthKey = strBeforeKey;

	InitControl();
}

VOID CSearchZipCodeDlgOpenAPI::OnbtnApply()
{
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_���ּ�OPENAPI_�˻�"), _T("MAINGRID"));
	ASSERT(pGrid);

	INT idx = pGrid->GetIdx();
	if (idx < 0)
	{
		AfxMessageBox(_T("���õ� �����Ͱ� �����ϴ�"));
		return;
	}

	CESL_DataMgr* pApiDM = FindDM(_T("DM_���ּ�OPENAPI_�˻����"));
	ASSERT(pApiDM);

	CString strApplyType;
	m_cmbApplyType.GetLBText(m_cmbApplyType.GetCurSel(), strApplyType);

	if (strApplyType.CompareNoCase(_T("���θ��ּ�")) == 0)
	{
		// 17/09/19 ������ : ���θ� �ּҰ˻��� �����ּҰ� ��ȸ���� �ʴ� ���� ����
/*//BEFORE-----------------------------------------------------------------------------
		pApiDM->GetCellData(_T("roadAddrPart1"), idx, m_strAddress);
*///AFTER------------------------------------------------------------------------------
		pApiDM->GetCellData(_T("roadAddr"), idx, m_strAddress);
/*//END-------------------------------------------------------------------------------*/
	}
	else
	{
		pApiDM->GetCellData(_T("jibunAddr"), idx, m_strAddress);
	}

	while (m_strAddress.Replace(_T("  "), _T(" ")) > 0)
	{
		// ���ڿ� ���� ó��
	}

	pApiDM->GetCellData(_T("zipNo"), idx, m_strZipCode);

	CDialog::OnOK();	
}

VOID CSearchZipCodeDlgOpenAPI::OnDblClickGrid()
{
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_���ּ�OPENAPI_�˻�"), _T("MAINGRID"));
	ASSERT(pGrid);

	if (pGrid->GetMouseRow() == 0)
	{
		return;
	}

	CESL_DataMgr* pApiDM = FindDM(_T("DM_���ּ�OPENAPI_�˻����"));
	ASSERT(pApiDM);

	if (pApiDM->GetRowCount() == 0)
	{
		return;
	}

	OnbtnApply();
}

VOID CSearchZipCodeDlgOpenAPI::InitAuthKey()
{
	//rm.SetCONNECTION_INFO(m_DM.CONNECTION_INFO);
	CESL_DataMgr* pApiDM = FindDM(_T("DM_���ּ�OPENAPI_�˻����"));
	ASSERT(pApiDM);

	CESL_DataMgr DM;
	DM.SetCONNECTION_INFO(pApiDM->CONNECTION_INFO);

	CString strQuery;
	strQuery.Format(_T("SELECT REC_KEY, VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_2='��Ÿ' AND GROUP_3='����' AND CLASS_ALIAS='���θ��ּҰ˻�' AND VALUE_NAME='����Ű' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	
	DM.RestructDataManager(strQuery);
	if (DM.GetRowCount() < 1)	return;

	m_strAuthRecKey = DM.GetCellData(0, 0);
	m_strAuthKey = DM.GetCellData(0, 1); //m_strAuthKey = _T("bnVsbDIwMTQxMTE5MTQyODUx"); // DB���� �����ϵ��� �ϴ� �׽�Ʈ����
}

VOID CSearchZipCodeDlgOpenAPI::InitControl()
{
	// �ʱ�ȭ
	{
		INT nCount = m_cmbRegion.GetCount();
		for (INT i=nCount-1; i>=0; i--)
		{
			m_cmbRegion.DeleteString(i);
		}
	}
	// ����
	m_cmbRegion.AddString(_T("���þ���"));
	m_cmbRegion.AddString(_T("����Ư����"));
	m_cmbRegion.AddString(_T("�λ걤����"));
	m_cmbRegion.AddString(_T("�뱸������"));
	m_cmbRegion.AddString(_T("��õ������"));
	m_cmbRegion.AddString(_T("���ֱ�����"));
	m_cmbRegion.AddString(_T("����������"));
	m_cmbRegion.AddString(_T("��걤����"));
	m_cmbRegion.AddString(_T("����Ư����ġ��"));
	m_cmbRegion.AddString(_T("��⵵"));
	m_cmbRegion.AddString(_T("������"));
	m_cmbRegion.AddString(_T("��û�ϵ�"));
	m_cmbRegion.AddString(_T("��û����"));
	m_cmbRegion.AddString(_T("����ϵ�"));
	m_cmbRegion.AddString(_T("���󳲵�"));
	m_cmbRegion.AddString(_T("���ϵ�"));
	m_cmbRegion.AddString(_T("��󳲵�"));
	m_cmbRegion.AddString(_T("����Ư����ġ��"));
	m_cmbRegion.SetCurSel(0);

	// �ʱ�ȭ
	{
		INT nCount = m_cmbApplyType.GetCount();
		for (INT i=nCount-1; i>=0; i--)
		{
			m_cmbApplyType.DeleteString(i);
		}
	}
	// ������� �޺� �ڽ�
	m_cmbApplyType.AddString(_T("���θ��ּ�"));
	m_cmbApplyType.AddString(_T("�����ּ�"));
	m_cmbApplyType.SetCurSel(0);
	
	// ����ȯ�� �ε�
	CCfgFileApi FileApi;
	FileApi.SetFile(OPENAPI_CONFIG_FILE);

	CString strData;
	FileApi.GetData(OPENAPI_CONFIG_TAG_REGION, strData);
	if (!strData.IsEmpty())
	{
		m_cmbRegion.SetCurSel(_ttoi(strData));
	}

	FileApi.GetData(OPENAPI_CONFIG_TAG_APPLYTYPE, strData);
	if (!strData.IsEmpty())
	{
		m_cmbApplyType.SetCurSel(_ttoi(strData));
	}

	GetDlgItem(IDC_BTN_SEARCH)->EnableWindow(!m_strAuthKey.IsEmpty());
	GetDlgItem(IDC_BTN_SETUP)->EnableWindow(m_strAuthKey.IsEmpty());

	SearchBeforeAction();
}

BOOL CSearchZipCodeDlgOpenAPI::SearchOpenAPI(CString strKeyword, INT nCurPage/*=0*/, BOOL bTest/*=FALSE*/)
{
	if (!bTest) m_CommonInfo.LASTKEYWORD.Empty();

	//�˻��� URL ���ڵ�
	CString strEncodingKeyword = CLocCommonAPI::UrlEncode((LPCTSTR)strKeyword);

	// ��û URL
	// TODO: ������ �����ؾ��ϴ°�??
	CString strRequestURL = _T("http://www.juso.go.kr/addrlink/addrLinkApi.do");
	
	// ��û���ڿ�
	CString strReqStream;
	strReqStream.Format(_T("%s?confmKey=%s&currentPage=%d&countPerPage=10&keyword=%s"), strRequestURL, m_strAuthKey, nCurPage, strEncodingKeyword);

	TRACE(_T("���ּҿ�û���ڿ�------------------------------------------------------\n"));
	TRACE(_T("%s\n"), strReqStream);
	TRACE(_T("���ּҿ�û���ڿ�------------------------------------------------------\n"));

	COpenAPI API;
	API.RequestURL(strReqStream);
	if (!API.GetErrorMessage().IsEmpty())
	{
		CString strMsg;
		strMsg.Format(_T("[���θ��ּ� �ȳ��ý���] �˻���û�� �����Ͽ����ϴ� : %s"), API.GetErrorMessage());
		if (!bTest) AfxMessageBox(strMsg, MB_ICONSTOP);

		return FALSE;
	}

	// ���ڵ� ������ ������ UTF-16����
	API.m_strSearchResult.Replace(_T("encoding=\"UTF-8\""), _T("encoding=\"UTF-16\""));

	if (!WriteXML(API.m_strSearchResult))
	{
		CString strMsg;
		strMsg.Format(_T("[���θ��ּ� �ȳ��ý���] �˻���� ���ϻ����� �����Ͽ����ϴ�!!!"));
		if (!bTest) AfxMessageBox(strMsg, MB_ICONSTOP);

		return FALSE;
	}

	CString strFileName = OPENAPI_RESULT_FILE;
	CComVariant varFileName = (LPCTSTR)strFileName;

	IXMLDOMDocumentPtr XmlDom (_T("Microsoft.XMLDOM"), NULL, CLSCTX_INPROC_SERVER);
	if (!XmlDom->load(varFileName))
	{
		CString strMsg;
		strMsg.Format(_T("[���θ��ּ� �ȳ��ý���] �˻���� �ؼ��� �����Ͽ����ϴ�!!"));
		if (!bTest) AfxMessageBox(strMsg, MB_ICONSTOP);

		XmlDom.Release();
		return FALSE;
	}

	IXMLDOMNodePtr NodeRoot;
	{
		IXMLDOMNodeListPtr NodeList = XmlDom->childNodes;

		LONG lLength = NodeList->Getlength();
		for (LONG idx=0; idx<lLength; idx++)
		{
			IXMLDOMNodePtr Node = NodeList->item[idx];

			CComBSTR btrName;
			Node->get_baseName(&btrName);

			CString strName(btrName);
			if (strName.CompareNoCase(OPENAPI_TAG_RESULTS) == 0)
			{
				NodeRoot = Node;
			}

			Node.Release();
		}

		NodeList.Release();
	}

	// ROOT ��� �м�
	{
		IXMLDOMNodeListPtr NodeList = NodeRoot->childNodes;

		LONG lLength = NodeList->Getlength();
		for (LONG idx=0; idx<lLength; idx++)
		{
			IXMLDOMNodePtr Node = NodeList->item[idx];

			CComBSTR btrName;
			Node->get_baseName(&btrName);

			CString strName(btrName);
			if		(strName.CompareNoCase(OPENAPI_TAG_COMMON) == 0)
			{
				FetchCommonInfo(Node);		// ���� ���� ó���Ǿ�� �Ѵ�
			}
			else if (strName.CompareNoCase(OPENAPI_TAG_JUSO) == 0)
			{
				if (!bTest) FetchJuso(Node);
			}

			Node.Release();
		}


		NodeList.Release();
	}

	NodeRoot.Release();

	XmlDom.Release();

	// ��˻��� Ű���� ����
	if (!bTest) m_CommonInfo.LASTKEYWORD = strKeyword;

	// 17/09/18 ������ : �ּ���ȸ ����� ���� ��� �޽��� ó��
//*/ ADD ---------------------------------------------------------------------------
	if(m_CommonInfo.TOTALCOUNT <= 0)
	{
		MessageBox(_T("�˻��� ����� �����ϴ�."), _T("�ּ���ȸ"), MB_ICONEXCLAMATION);
	}
//*/ END ---------------------------------------------------------------------------
	
	return TRUE;
}

BOOL CSearchZipCodeDlgOpenAPI::SearchBeforeAction()
{
	m_CommonInfo.TOTALCOUNT   = 0;
	m_CommonInfo.CURRENTPAGE  = 0;
	m_CommonInfo.COUNTPERPAGE = 0;
	m_CommonInfo.ERRORCODE.Empty();
	m_CommonInfo.ERRORMESSAGE.Empty();
	m_CommonInfo.LASTKEYWORD.Empty();

	GetDlgItem(IDC_BTN_PREV_PAGE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_NEXT_PAGE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_APPLY)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDT_CURRENT_PAGE)->SetWindowText(_T("0"));
	GetDlgItem(IDC_EDT_TOTAL_PAGE)->SetWindowText(_T("0"));

	CESL_DataMgr* pDM = FindDM(_T("DM_���ּ�OPENAPI_�˻����"));
	ASSERT(pDM);
	pDM->FreeData();

	return TRUE;
}

BOOL CSearchZipCodeDlgOpenAPI::SearchAfterAction()
{
	if (m_CommonInfo.ERRORCODE != _T("0"))
	{
		CString strMsg;
		if		(m_CommonInfo.ERRORCODE.CompareNoCase(_T("-999")) == 0)
		{
			strMsg.Format(_T("[���θ��ּ� �ȳ��ý���] ����Դϴ�!!! �����Ŀ� �ٽ� �õ��Ͻñ� �ٶ��ϴ�"));
		}
		else if (m_CommonInfo.ERRORCODE.CompareNoCase(_T("E0001")) == 0)
		{
			strMsg.Format(_T("[���θ��ּ� �ȳ��ý���]���� ���ε��� ���� ����Ű�� ������Դϴ�!!!"));
			GetDlgItem(IDC_BTN_SEARCH)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_SETUP)->EnableWindow(TRUE);
		}
		else if (m_CommonInfo.ERRORCODE.CompareNoCase(_T("E0002")) == 0)
		{
			strMsg.Format(_T("[���θ��ּ� �ȳ��ý���]���� ���ε��� ���� SITE�Դϴ�!!!"));
		}
		else if (m_CommonInfo.ERRORCODE.CompareNoCase(_T("E0003")) == 0)
		{
			strMsg.Format(_T("�������� ��η� �����Ͻñ� �ٶ��ϴ�."));
		}
		else if (m_CommonInfo.ERRORCODE.CompareNoCase(_T("E0004")) == 0)
		{
			strMsg.Format(_T("�ִ� �˻������ 1000������ ���ѵǾ� �ֽ��ϴ�."));
		}
		else if (m_CommonInfo.ERRORCODE.CompareNoCase(_T("P0001")) == 0)
		{
			strMsg.Format(_T("�˻������ �ʹ� �����ϴ�. ���θ��ּ� �Ǵ� ������ �Է��Ͻñ� �ٶ��ϴ�."));
		}

		SearchBeforeAction();

		AfxMessageBox(strMsg);
	}
	else
	{
		// ������������ ù�������� �ƴ϶��
		if (m_CommonInfo.CURRENTPAGE > 1)
		{
			GetDlgItem(IDC_BTN_PREV_PAGE)->EnableWindow(TRUE);
		}
		// ��ü �������� ���������������� ������ �۴ٸ�
		if ((m_CommonInfo.CURRENTPAGE*m_CommonInfo.COUNTPERPAGE) < m_CommonInfo.TOTALCOUNT)
		{
			GetDlgItem(IDC_BTN_NEXT_PAGE)->EnableWindow(TRUE);
		}
		// �˻������ �ִٸ�
		if (m_CommonInfo.TOTALCOUNT > 0)
		{
			GetDlgItem(IDC_BTN_APPLY)->EnableWindow(TRUE);

			CString strCurPage;
			strCurPage.Format(_T("%d"), m_CommonInfo.CURRENTPAGE);
			GetDlgItem(IDC_EDT_CURRENT_PAGE)->SetWindowText(strCurPage);

			INT nTotalPage = (m_CommonInfo.TOTALCOUNT / m_CommonInfo.COUNTPERPAGE) + 1;

			CString strTotalPage;
			strTotalPage.Format(_T("%d"), nTotalPage);
			GetDlgItem(IDC_EDT_TOTAL_PAGE)->SetWindowText(strTotalPage);
		}
	}

	ControlDisplay(_T("CM_���ּ�OPENAPI_�˻�"), _T("MAINGRID"));

	return TRUE;
}

BOOL CSearchZipCodeDlgOpenAPI::FetchCommonInfo(IXMLDOMNodePtr NodeInfo)
{
	IXMLDOMNodeListPtr NodeList = NodeInfo->childNodes;

	LONG lLength = NodeList->Getlength();
	for (LONG idx=0; idx<lLength; idx++)
	{
		IXMLDOMNodePtr Node = NodeList->item[idx];

		CComBSTR btrName;
		CComBSTR btrText;
		Node->get_baseName(&btrName);
		Node->get_text(&btrText);

		CString strName(btrName);
		CString strText(btrText);
		if		(strName.CompareNoCase(OPENAPI_TAG_TOTALCOUNT) == 0)
		{
			m_CommonInfo.TOTALCOUNT = _ttoi(strText);
		}
		else if (strName.CompareNoCase(OPENAPI_TAG_CURRENTPAGE) == 0)
		{
			m_CommonInfo.CURRENTPAGE = _ttoi(strText);
		}
		else if (strName.CompareNoCase(OPENAPI_TAG_COUNTPERPAGE) == 0)
		{
			m_CommonInfo.COUNTPERPAGE = _ttoi(strText);
		}
		else if (strName.CompareNoCase(OPENAPI_TAG_ERRORCODE) == 0)
		{
			m_CommonInfo.ERRORCODE = strText;
		}
		else if (strName.CompareNoCase(OPENAPI_TAG_ERRORMESSAGE) == 0)
		{
			m_CommonInfo.ERRORMESSAGE = strText;
		}
		else
		{
			// I DON'T KNOW
		}

		Node.Release();
	}

	NodeList.Release();

	// ANALZE INFO

	return TRUE;
}

BOOL CSearchZipCodeDlgOpenAPI::FetchJuso(IXMLDOMNodePtr NodeJuso)
{
	CESL_DataMgr* pDM = FindDM(_T("DM_���ּ�OPENAPI_�˻����"));
	ASSERT(pDM);

	pDM->InsertRow(-1);
	INT nIdx = pDM->GetRowCount() - 1;

	IXMLDOMNodeListPtr NodeList = NodeJuso->childNodes;
	LONG lLength = NodeList->Getlength();
	for (LONG idx=0; idx<lLength; idx++)
	{
		IXMLDOMNodePtr Node = NodeList->item[idx];

		CComBSTR btrName;
		CComBSTR btrText;
		Node->get_baseName(&btrName);
		Node->get_text(&btrText);

		CString strName(btrName);
		CString strText(btrText);

		pDM->SetCellData(strName, strText, nIdx);

		Node.Release();
	}

	NodeList.Release();

	return TRUE;
}

BOOL CSearchZipCodeDlgOpenAPI::WriteXML(CString strResultXML)
{
	FILE* fp = _tfopen(OPENAPI_RESULT_FILE, _T("wb"));
	if (fp == NULL)
	{
		return FALSE;
	}

	fputc(0xFF, fp);
	fputc(0xFE, fp);

	// WRITE RESULT
	_ftprintf (fp, _T("%s"), strResultXML);
		
	fclose(fp);

	return TRUE;
}