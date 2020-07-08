// BO_Request_Duplicate_DetailInfo_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_Request_Duplicate_DetailInfo_Dlg.h"
#include "BO_HOLDING_ALL_DUP_SEARCH.h"
#include "../BO_VOLUME_FORM/BOVolumeDlg.h"
#include "../K2UP_Common/MulitComboBoxManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_Request_Duplicate_DetailInfo_Dlg dialog


CBO_Request_Duplicate_DetailInfo_Dlg::CBO_Request_Duplicate_DetailInfo_Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_Request_Duplicate_DetailInfo_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_Request_Duplicate_DetailInfo_Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	AfxInitRichEdit();	
	m_pFurnishCM = NULL;
	m_pSrcCM = NULL;
	m_pDupCM = NULL;
	m_pRequestDM = NULL;
	m_pMainDM = NULL;
	m_pMainGrid = NULL;
	m_nSrcIdx = -1;
	m_nDupIdx = -1;
	m_nSrcFirstIdx = -1;
	m_nDupFirstIdx = -1;
	m_nSrcLastIdx = -1;
	m_nDupLastIdx = -1;
	m_bGridSelectYN = FALSE;
	m_nOpenMode = OPERATION_TYPE_INVALID;
	m_bFurnishDataChange = FALSE;
}

CBO_Request_Duplicate_DetailInfo_Dlg::~CBO_Request_Duplicate_DetailInfo_Dlg()
{
}


VOID CBO_Request_Duplicate_DetailInfo_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_Request_Duplicate_DetailInfo_Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_Request_Duplicate_DetailInfo_Dlg, CDialog)
	//{{AFX_MSG_MAP(CBO_Request_Duplicate_DetailInfo_Dlg)
	ON_BN_CLICKED(IDC_btnNEXT_SRC, OnbtnNEXTSRC)
	ON_BN_CLICKED(IDC_btnPREV_SRC, OnbtnPREVSRC)
	ON_BN_CLICKED(IDC_btnPRE_SEL, OnbtnPRESEL)
	ON_BN_CLICKED(IDC_btnNEXT_SEL, OnbtnNEXTSEL)
	ON_BN_CLICKED(IDC_btnREQDATA_SAVE, OnbtnREQDATASAVE)
	ON_BN_CLICKED(IDC_btnSHOW_DUPVOL, OnbtnSHOWDUPVOL)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_Request_Duplicate_DetailInfo_Dlg message handlers

BOOL CBO_Request_Duplicate_DetailInfo_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_��ġ�����������󼼺���")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pFurnishCM = FindCM(_T("CM_��ġ�����������󼼺���"));
	m_pSrcCM = FindCM(_T("CM_��ġ��������������ڷ�"));
	m_pDupCM = FindCM(_T("CM_��ġ����������纹���ڷ�"));
	m_pRequestDM = FindDM(_T("DM_��ġ����ڷ����"));

	if(m_pInfo->GetSMSUseMode() < 1)
	{
		SetUDFCheck(m_pRequestDM, _T("SMS���ſ���"));
	}

	if(!Init())
	{
		ESLAfxMessageBox(_T("��ġ����������� �󼼺��� ȭ���� �ʱ�ȭ �� �� �����ϴ�."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CBO_Request_Duplicate_DetailInfo_Dlg::SetUDFCheck(CESL_DataMgr *pDM, CString strAlias)
{
	if(pDM == NULL || strAlias.IsEmpty()) return -1;
	
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	
	INT col, nCols;
	nCols = pDM->m_nCols;
	if(nCols < 0) nCols = pDM->GetRefColumnCount();
	for (col = 0; col < nCols; col++) {
		if (pDM->GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE)!= 0) continue;
		if(FIELD_ALIAS == strAlias)
		{
			pDM->SetDataMgrRef(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, _T("udf"));
		}
	}
	return 0;
}	

BOOL CBO_Request_Duplicate_DetailInfo_Dlg::Init()
{
	if(m_pMainDM == NULL || m_pMainGrid == NULL) return FALSE;
	m_Marceditor_dup.SubclassDlgItem(IDC_richHOLDING_DUPLICATE, -1, this);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
	//======================================================
	//20080522 ADD BY PJW : ��� ���ڼ� ����
	( (CEdit*) GetDlgItem(IDC_richREQDUP_CANCEL_REASON) )->SetLimitText(200);
	//======================================================
	SetFurnishStatusCombo();
	INT ids = Display(0);
	if(ids < 0) return FALSE;
	return TRUE;
}

INT CBO_Request_Duplicate_DetailInfo_Dlg::Display(INT nIdx)
{
	BOOL bSrcChange = FALSE;
	switch(nIdx)
	{
	case 0:
		InitFirstLastIndex();
		GetFirstSelectIndex(m_nSrcIdx, m_nDupIdx);
		ShowSrcNextPrevButton(m_nSrcIdx);
		ShowDupNextPrevButton(m_nDupIdx);
		bSrcChange = TRUE;
		break;
	case -1:
		GetPrevSelectIndex(m_nSrcIdx, m_nDupIdx);
		ShowSrcNextPrevButton(m_nSrcIdx);
		ShowDupNextPrevButton(m_nDupIdx);
		bSrcChange = TRUE;
		break;
	case 1:
		GetNextSelectIndex(m_nSrcIdx, m_nDupIdx);
		ShowSrcNextPrevButton(m_nSrcIdx);
		ShowDupNextPrevButton(m_nDupIdx);
		bSrcChange = TRUE;
		break;
	case 1000:
		GetNextDupIndex(m_nDupIdx);
		ShowDupNextPrevButton(m_nDupIdx);
		bSrcChange = FALSE;
		break;
	case -1000:
		GetPrevDupIndex(m_nDupIdx);
		ShowDupNextPrevButton(m_nDupIdx);
		bSrcChange = FALSE;
		break;
	default:
		return 0;
	}
	
	CString dupmarc, strQuery, strKey;
	
	strKey = m_pMainDM->GetCellData(_T("IBS_��KEY"), m_nDupIdx);
	
	strQuery.Format(_T("SELECT MARC FROM BO_SPECIES_TBL WHERE REC_KEY=%s"), strKey);
	m_pMainDM->GetOneValueQuery(strQuery, dupmarc);
	m_pMainDM->SetCellData(_T("BS_MARC"), dupmarc ,m_nDupIdx);	
	if(bSrcChange)
	{
		strKey = m_pMainDM->GetCellData(_T("IBS_��KEY"), m_nSrcIdx); 
		strQuery.Format(_T("REC_KEY=%s"), strKey);
		m_pRequestDM->RefreshDataManager(strQuery);
		m_pFurnishCM->AllControlDisplay(0);
	}
	
	//decoding
	CString strTagCode, strErrMsg, strHelpMsg;
	
	if(m_pInfo->pMarcMgr->Decoding(dupmarc, &m_Marc_dup))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}	
	}

	m_Marceditor_dup.InitUserAlias();
	m_Marceditor_dup.Init(m_pInfo->pMarcMgr, &m_Marc_dup);
	m_Marceditor_dup.Display();
	
	m_pSrcCM->AllControlDisplay(m_nSrcIdx);
	m_pDupCM->AllControlDisplay(m_nDupIdx);
	m_pMainGrid->SetReverse(m_nDupIdx);
	//20070813 ADD PJW : ��û���� ����¸��� 
	GetDlgItem(IDC_edtREQDUP_APPLICATION_DATE)->EnableWindow(FALSE);
	return 0;
}

VOID CBO_Request_Duplicate_DetailInfo_Dlg::InitFirstLastIndex()
{
	CString strTmpData;
	INT ids = m_pMainGrid->SelectMakeList();
	ids = m_pMainGrid->SelectGetHeadPosition();
	if(ids < 0)
	{
		m_bGridSelectYN = FALSE;
		INT cnt = m_pMainDM->GetRowCount();
		m_nSrcFirstIdx = 0;
		for(INT i=cnt-1 ; i>=0 ; i--)
		{
			strTmpData = m_pMainDM->GetCellData(_T("�ĺ���ȣ"), i);
			if(strTmpData != _T("**"))
				break;
		}
		m_nSrcLastIdx = i;
	}
	else
	{
		m_bGridSelectYN = TRUE;
		m_nSrcLastIdx = m_pMainGrid->SelectGetTailPosition();
		m_nSrcFirstIdx = m_pMainGrid->SelectGetHeadPosition();
	}
}

VOID CBO_Request_Duplicate_DetailInfo_Dlg::ShowSrcNextPrevButton(INT idx)
{
	GetDlgItem(IDC_btnPREV_SRC)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnNEXT_SRC)->EnableWindow(TRUE);

	if(m_nSrcFirstIdx >= idx || idx < 0)
		GetDlgItem(IDC_btnPREV_SRC)->EnableWindow(FALSE);
	if(m_nSrcLastIdx <= idx || idx < 0)
		GetDlgItem(IDC_btnNEXT_SRC)->EnableWindow(FALSE);
}

VOID CBO_Request_Duplicate_DetailInfo_Dlg::ShowDupNextPrevButton(INT idx)
{
	GetDlgItem(IDC_btnPRE_SEL)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnNEXT_SEL)->EnableWindow(TRUE);

	if(m_nDupFirstIdx >= idx || idx < 0)
		GetDlgItem(IDC_btnPRE_SEL)->EnableWindow(FALSE);
	if(m_nDupLastIdx <= idx || idx < 0)
		GetDlgItem(IDC_btnNEXT_SEL)->EnableWindow(FALSE);
}

INT CBO_Request_Duplicate_DetailInfo_Dlg::GetFirstSelectIndex(INT &nSrcIdx, INT &nDupIdx)
{
	INT nFirstSelIdx = -1;
	if(m_nOpenMode == GUPDATE)
	{
		nFirstSelIdx = m_pMainGrid->GetRow() - 1;
	}
	else if(m_nOpenMode == SUPDATE)
	{
		if(m_bGridSelectYN)
			nFirstSelIdx = m_nSrcFirstIdx;
		else
			nFirstSelIdx = m_pMainGrid->GetRow() - 1;
	}
	if(nFirstSelIdx < 0) nFirstSelIdx = 0;
	CString strTmpData;
	strTmpData = m_pMainDM->GetCellData(_T("�ĺ���ȣ"), nFirstSelIdx);
	if(strTmpData != _T("**"))
	{
		nSrcIdx = nFirstSelIdx;
		nDupIdx = nFirstSelIdx+1;
	}
	else
	{
		for(INT i=nFirstSelIdx ; i>=0 ; i--)
		{
			strTmpData = m_pMainDM->GetCellData(_T("�ĺ���ȣ"), i);
			if(strTmpData != _T("**"))
				break;
		}
		nSrcIdx = i;
		nDupIdx = nFirstSelIdx;
	}
	//�����ڷ��� ó�� ������ �ε����� �����Ѵ�.
	SetDupIndexInfo(nSrcIdx);
	return nFirstSelIdx;
}

INT CBO_Request_Duplicate_DetailInfo_Dlg::GetPrevSelectIndex(INT &nSrcIdx, INT &nDupIdx)
{
	INT nPrevSelIndex = -1;
	if(m_bGridSelectYN)
	{
		nPrevSelIndex = m_pMainGrid->SelectGetPrev();
		if(nPrevSelIndex < 0)
		{
			nPrevSelIndex = m_pMainGrid->SelectGetHeadPosition();
		}
	}
	else
	{
		if(m_nSrcIdx != m_nSrcFirstIdx)
		{
			CString strTmpData;
			for(INT i=m_nSrcIdx-1 ; i>=0 ; i--)
			{
				strTmpData = m_pMainDM->GetCellData(_T("�ĺ���ȣ"), i);
				if(strTmpData != _T("**")) break;
			}
			nPrevSelIndex = i;
		}
	}
	nSrcIdx = nPrevSelIndex;
	nDupIdx = nPrevSelIndex + 1;
	//�����ڷ��� ó�� ������ �ε����� �����Ѵ�.
	SetDupIndexInfo(nSrcIdx);
	return nPrevSelIndex;
}

INT CBO_Request_Duplicate_DetailInfo_Dlg::GetNextDupIndex(INT &nDupIdx)
{
	INT nNextSelIndex = -1;
	if(m_nDupIdx != m_nDupLastIdx)
	{
		nNextSelIndex = m_nDupIdx + 1;
	}
	nDupIdx = nNextSelIndex;
	return nNextSelIndex;
}

INT CBO_Request_Duplicate_DetailInfo_Dlg::GetPrevDupIndex(INT &nDupIdx)
{
	INT nPrevSelIndex = -1;
	if(m_nDupIdx != m_nDupFirstIdx)
	{
		nPrevSelIndex = m_nDupIdx - 1;
	}
	nDupIdx = nPrevSelIndex;
	return nPrevSelIndex;
}

INT CBO_Request_Duplicate_DetailInfo_Dlg::GetNextSelectIndex(INT &nSrcIdx, INT &nDupIdx)
{
	INT nNextSelIndex = -1;
	if(m_bGridSelectYN)
	{
		nNextSelIndex = m_pMainGrid->SelectGetNext();
		if(nNextSelIndex < 0)
		{
			nNextSelIndex = m_pMainGrid->SelectGetTailPosition();
		}
	}
	else
	{
		if(m_nSrcIdx != m_nSrcLastIdx)
		{
			CString strTmpData;
			for(INT i=m_nSrcIdx+1 ; i<=m_nSrcLastIdx ; i++)
			{
				strTmpData = m_pMainDM->GetCellData(_T("�ĺ���ȣ"), i);
				if(strTmpData != _T("**")) break;
			}
			nNextSelIndex = i;			
		}
	}
	nSrcIdx = nNextSelIndex;
	nDupIdx = nNextSelIndex + 1;
	//�����ڷ��� ó�� ������ �ε����� �����Ѵ�.
	SetDupIndexInfo(nSrcIdx);
	return nNextSelIndex;
}

INT CBO_Request_Duplicate_DetailInfo_Dlg::SetDupIndexInfo(INT nSrcIdx)
{
	CString strTmpData;
	m_nDupFirstIdx = nSrcIdx + 1;
	for(INT i=m_nDupFirstIdx ; i<m_pMainDM->GetRowCount() ; i++)
	{
		strTmpData = m_pMainDM->GetCellData(_T("�ĺ���ȣ"), i);
		if(strTmpData != _T("**")) break;
	}
	m_nDupLastIdx = i - 1;
	return 0;
}

VOID CBO_Request_Duplicate_DetailInfo_Dlg::OnbtnNEXTSRC() 
{
	// TODO: Add your control notification handler code here
	Display(1);
}

VOID CBO_Request_Duplicate_DetailInfo_Dlg::OnbtnPREVSRC() 
{
	// TODO: Add your control notification handler code here
	Display(-1);
}

VOID CBO_Request_Duplicate_DetailInfo_Dlg::OnbtnPRESEL() 
{
	// TODO: Add your control notification handler code here
	Display(-1000);
}

VOID CBO_Request_Duplicate_DetailInfo_Dlg::OnbtnNEXTSEL() 
{
	// TODO: Add your control notification handler code here
	Display(1000);
}

VOID CBO_Request_Duplicate_DetailInfo_Dlg::OnbtnREQDATASAVE() 
{
	// TODO: Add your control notification handler code here
	ChangeFurnishData();
}

INT CBO_Request_Duplicate_DetailInfo_Dlg::ChangeFurnishData()
{
	CString strFurnishStatus, strCancelReason, strDate, strKey, strCallNumber;
	m_pFurnishCM->GetControlMgrData(_T("��ġ����"), strFurnishStatus);	
	m_pFurnishCM->GetControlMgrData(_T("��һ���"), strCancelReason);
	
	if(strFurnishStatus == _T("3"))
	{
		strDate = m_pRequestDM->GetDate();
		INT pos = strDate.Find(_T(" "));
		strDate = strDate.Mid(0, pos);
		m_pRequestDM->SetCellData(_T("��ġ����"), strDate, 0);
		strCallNumber = ((CBO_HOLDING_ALL_DUP_SEARCH*)pParentMgr)->MakeCallNumber(m_pMainDM, m_nDupIdx);
		if(!strCallNumber.IsEmpty())
		{
			//=====================================================================================
			//2008.07.16 UPDATE BY PJW : \n���� ���๮�ڸ� �ڵ尪���� �߰��Ѵ�.
			//strCancelReason += _T("\nû����ȣ : ") + strCallNumber;
			TCHAR cCode1;
			TCHAR cCode2;
			CString strTemp;
			
			cCode1 = 13;
			cCode2 = 10;
			
			strTemp.Format(_T("%c%cû����ȣ : %s"), cCode1, cCode2, strCallNumber);

			//===================================================
			//2009.05.13 UPDATE BY LKS : �ش� û����ȣ�� ��ϵǾ� �ִٸ� Skip

			//strCancelReason += strTemp;
			if(strCancelReason.Find(strCallNumber) == -1)
			{
				strCancelReason += strTemp;
			}
			//===================================================
			//=====================================================================================
		}
		m_pFurnishCM->SetControlMgrData(_T("��һ���"), strCancelReason);
	}
	else
		m_pRequestDM->SetCellData(_T("��ġ����"), _T(""), 0);
	m_pRequestDM->SetCellData(_T("��ġ����"), strFurnishStatus, 0);
	m_pRequestDM->SetCellData(_T("��һ���"), strCancelReason, 0);


	//Ȯ�尳�����.. ����
	//������, ������, ���ǻ�, ���ǳ�, ���� ������ �� �ֵ��� ����
	//������, ������, ���ǻ�, ���ǳ�, ���� ������ �� �ֵ��� ����
	//20070813 ADD PJW : ��û������ ���������� ���߱� ���� strApplicantDate �߰�
	CString  strTitle, strAuthor, strPub, strPubYear, strPrice;
	CString strTempApplicantDate, strApplicantDate;
	m_pFurnishCM->GetControlMgrData(_T("������"), strTitle);
	m_pFurnishCM->GetControlMgrData(_T("������"), strAuthor);
	m_pFurnishCM->GetControlMgrData(_T("���ǻ�"), strPub);
	m_pFurnishCM->GetControlMgrData(_T("���ǳ�"), strPubYear);
	m_pFurnishCM->GetControlMgrData(_T("����"), strPrice);
	//20070813 ADD PJW : ��û���� �������� ���� 
	strApplicantDate = m_pRequestDM->GetCellData(_T("��û����"), 0);
	strTempApplicantDate.Format(_T("TO_DATE('%s','YYYY/MM/DD HH24:MI:SS')"), strApplicantDate);
	ChangeFieldAlias(m_pRequestDM, _T("��û����"), _T("APPLICANT_DATE"), _T("int"));


	m_pRequestDM->SetCellData(_T("��ǥ��"), strTitle, 0);
	m_pRequestDM->SetCellData(_T("������"), strAuthor, 0);
	m_pRequestDM->SetCellData(_T("������"), strPub, 0);
	m_pRequestDM->SetCellData(_T("�����"), strPubYear, 0);
	m_pRequestDM->SetCellData(_T("����"), strPrice, 0);
	//20070813 ADD PJW : DM�� ������ ��û���� ����
	m_pRequestDM->SetCellData(_T("��û����"), strTempApplicantDate, 0);

	strKey = m_pRequestDM->GetCellData(_T("��ġ���KEY"), 0);
	CArray<CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	// 2005-03-15
	RemoveAliasList.Add(_T("UDF_�켱�����ڷ�"));
	m_pRequestDM->StartFrame();
	m_pRequestDM->MakeQueryByDM(RemoveAliasList, m_pRequestDM, m_pRequestDM->TBL_NAME, 0, m_pRequestDM, _T("REC_KEY"), _T("NUMERIC"), strKey);
	INT ids = m_pRequestDM->SendFrame();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("��ġ��������� ������ �� �����ϴ�."));
		return ids;
	}
	ids = m_pRequestDM->EndFrame();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("��ġ��������� ������ �� �����ϴ�."));
		return ids;
	}
	ESLAfxMessageBox(_T("��ġ��������� �����Ͽ����ϴ�."));
	

	//20070813 ADD PJW : ������ DM�� �˸��ƽ��� ������� ������.
	ChangeFieldAlias(m_pRequestDM, _T("��û����"), _T("TO_CHAR(APPLICANT_DATE, 'YYYY/MM/DD HH24:MI:SS')"), _T("str"));
	m_pRequestDM->SetCellData(_T("��û����"), strApplicantDate, 0);
	
	//==================================================
	//20080618 ADD BY PJW : �������� ���泻�������Ұ�� ������ DM�� �����Ѵ�.
	INT nMainSelectCnt;
	INT nMainDmColCnt;
	INT nMainGridIdx;
	CString strRecKey;
	CString strMainDmRecKey;
	CString strTempData;

	CESL_DataMgr * pMainDM = pParentMgr->FindDM(_T("DM_��ġ����ڷ����"));
	CESL_ControlMgr * pMainCM = pParentMgr->FindCM(_T("CM_��ġ����ڷ����"));
	CESL_Grid * pMainGrid = (CESL_Grid*)pMainCM->FindControl(_T("MainGrid"));

	pMainGrid->SelectMakeList();
	nMainSelectCnt = pMainGrid->SelectGetCount();
	nMainGridIdx = pMainGrid->SelectGetHeadPosition();
	nMainDmColCnt = pMainDM->GetColCount();
	m_pRequestDM->GetCellData(_T("��ġ���KEY"), 0, strRecKey);
	

	


	//===================================================
	//2010.02.05 ADD BY PJW : 
	CString strFieldAlias;
	CString strFieldName;
	CString strFieldType;
	CString strInitValue;
	CString strDataType;
	//===================================================

	for( INT i = 0 ; i < nMainSelectCnt; i++ )
	{
		pMainDM->GetCellData(_T("��ġ���KEY"), nMainGridIdx, strMainDmRecKey);
		if( strRecKey == strMainDmRecKey )
		{
			for(INT j = 0 ; j < nMainDmColCnt ; j++ )
			{
				//===================================================
				//2010.04.05 UPDATE BY PJW : �׸����� ������ ��ȣ�� ������� �ʵ��� �Ѵ�.
//				m_pRequestDM->GetCellData( 0, j, strTempData );
//				pMainDM->SetCellData( nMainGridIdx, j, strTempData );
				m_pRequestDM->GetColumnRefInfo(j, strFieldAlias, strFieldName, strFieldType, strInitValue, strDataType);
				if( _T("�����ڹ�ȣ") !=strFieldAlias )
				{
					m_pRequestDM->GetCellData( 0, j, strTempData );
					pMainDM->SetCellData(strFieldAlias, strTempData, j );
				}
				//===================================================
			}

			pMainGrid->SetProtectSelectColumn(TRUE);
			
		
			pMainGrid->DisplayLine(nMainGridIdx);
			pMainGrid->SetProtectSelectColumn(FALSE);

			//2012.11.02 ADD BY BJS : 2012�⵵ ������ ���ռ��� Ȯ�뱸�� ��� 2��(�׸��忡 ��ġ���� ����)
			//�������� �󼼺��⸦ ���� ��ġ���� ���� �� ó�����°� �ٷ� ����� �� �ְ� �Ѵ�
			if(strFurnishStatus==_T("1")) strFurnishStatus=_T("��û��");
			else if(strFurnishStatus==_T("2")) strFurnishStatus=_T("ó����");
			else if(strFurnishStatus==_T("3")) strFurnishStatus=_T("������");
			else if(strFurnishStatus==_T("4")) strFurnishStatus=_T("��ҵ�");
		
			pMainGrid->SetAt(nMainGridIdx,7,strFurnishStatus);   
			pMainGrid->Refresh();
			///////////////////////////////////////////////////
			break;
		}
		nMainGridIdx = pMainGrid->SelectGetNext();
	}
	//==================================================

	//=====================================================
	//2009.01.09 ADD BY PJW : ��ġ��� �󼼺��� �����ϸ� �������� �׸��嵵 �������ش�.
	INT nDupSelectCnt;
	INT nDupDmColCnt;
	INT nDupGridIdx;
	CString strDupDmRecKey;

	CESL_DataMgr * pDupDM = pParentMgr->FindDM(_T("DM_�ߺ�������"));
	CESL_ControlMgr * pDupCM = pParentMgr->FindCM(_T("CM_��������"));
	CESL_Grid * pDupGrid = (CESL_Grid*)pDupCM->FindControl(_T("MainGrid2"));
	
	pDupGrid->SelectMakeList();
	nDupSelectCnt = pDupGrid->SelectGetCount();
	nDupGridIdx = pDupGrid->SelectGetHeadPosition();
	nDupDmColCnt = pDupGrid->GetColCount();

	for( i = 0 ; i < nDupSelectCnt; i++ )
	{
		pDupDM->GetCellData(_T("IBS_��KEY"), nDupGridIdx, strDupDmRecKey);
		if( strRecKey == strDupDmRecKey )
		{
			pDupDM->SetCellData(_T("IBS_��ǥ��"), strTitle, nDupGridIdx);
			pDupDM->SetCellData(_T("IBS_������"), strAuthor, nDupGridIdx);
			pDupDM->SetCellData(_T("IBS_������"), strPub, nDupGridIdx);
			pDupDM->SetCellData(_T("IBS_�����"), strPubYear, nDupGridIdx);
			pDupGrid->SetProtectSelectColumn(TRUE);
			pDupGrid->DisplayLine(nDupGridIdx);
			pDupGrid->SetProtectSelectColumn(FALSE);
			break;
		}
		nDupGridIdx = pDupGrid->SelectGetNext();
		


	}
	//=====================================================
	
	//==================================================
	//20080618 ADD BY PJW : MAIN�� DM�� CM�� �̹� �����Ͽ��⿡ ��������� FALSE�� �ִ´�.
	//m_bFurnishDataChange = TRUE;
	m_bFurnishDataChange = FALSE;
	//==================================================
	return 0;
}

VOID CBO_Request_Duplicate_DetailInfo_Dlg::OnbtnSHOWDUPVOL() 
{
	// TODO: Add your control notification handler code here
	CString strKey = m_pMainDM->GetCellData(_T("IBS_��KEY"), m_nDupIdx);
	CString strAcqKey = m_pMainDM->GetCellData(_T("BP_��������KEY"), m_nDupIdx);
	if(strKey.IsEmpty()) return ;

	CBOVolumeDlg dlg(this);
	dlg.SetSpeciesKey(strKey);
	if(!strAcqKey.IsEmpty() && strAcqKey != _T("-1"))
		dlg.SetPurchaseKey(strAcqKey);
	dlg.SetOpenMode(8000);
	dlg.DoModal();	
}

INT CBO_Request_Duplicate_DetailInfo_Dlg::SetFurnishStatusCombo()
{
	CMulitComboBoxManager mgr(this);
	CString strData[4][2] = {
		_T("1"), _T("��û��"),
		_T("2"), _T("ó����"),
		_T("3"), _T("������"),
		_T("4"), _T("��ҵ�")
	};
	mgr.SetMulTiComboBoxStrData(m_pFurnishCM->CM_ALIAS, _T(""), strData, _T("��ġ����"), 4);
	return 0;
}


INT CBO_Request_Duplicate_DetailInfo_Dlg::ChangeFieldAlias(CESL_DataMgr *pDM, CString strFieldAlias, CString strFieldName, CString strFieldType)
{
	INT col, nCols;
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;

	if( pDM == NULL || strFieldAlias.IsEmpty() == TRUE )		return -1;
	
	nCols = pDM->m_nCols;
	if( nCols < 0 )		 nCols = pDM->GetRefColumnCount();
	for( col = 0; col < nCols; col++ ) 
	{
		if( pDM->GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE) != 0 )
			continue;
		if( FIELD_ALIAS == strFieldAlias )
			pDM->SetDataMgrRef(col, FIELD_ALIAS, strFieldName, strFieldType, INIT_VALUE, DATA_TYPE);
	}

	return 0;
}	

HBRUSH CBO_Request_Duplicate_DetailInfo_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
