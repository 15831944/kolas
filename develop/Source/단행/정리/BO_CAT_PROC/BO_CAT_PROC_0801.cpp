// BO_CAT_PROC_0801.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROC_0801.h"
#include "..\..\..\ESL\ESL\DlgClassCode.h"
#include "..\..\..\����\KOMARC\KOMarcGetter\KOMarcGetterDlg.h"
#include "..\..\..\����\BO_VOLUME_FORM\BOVolumeDlg.h"

#include "..\BO_CAT_PROC_FIXEDFIELD\BO_CAT_PROC_0320_01.h"
#include "..\BO_CAT_PROC_FIXEDFIELD\BO_CAT_PROC_0321.h"
#include "..\BO_CAT_PROC_FIXEDFIELD\BO_CAT_PROC_0322.h"
#include "..\BO_CAT_PROC_FIXEDFIELD\BO_CAT_PROC_0323.h"
#include "..\BO_CAT_PROC_FIXEDFIELD\BO_CAT_PROC_0324.h"
#include "..\\..\\..\\����\\MarcAdjustApi\\CatApi_BO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0801 dialog


CBO_CAT_PROC_0801::CBO_CAT_PROC_0801(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_bInitDlg = TRUE;

	//{{AFX_DATA_INIT(CBO_CAT_PROC_0801)
	m_nClass = -1;
	m_nMode = -1;
	//}}AFX_DATA_INIT

	m_nModifyMode = -1;
	m_nAddMode = -1;
	m_nDeleteMode = -1;

	m_idx = -1;

	m_iPreModifyCount = 0;

	m_iPostModifyCount = 0;

	pCharSetMgr_RoleWord = NULL;

	m_pIndexTool = NULL;

	AfxInitRichEdit();
}

VOID CBO_CAT_PROC_0801::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0801)
	DDX_Radio(pDX, IDC_rNORMAL_MARC, m_nClass);
	DDX_Radio(pDX, IDC_rUPDATE_MODE, m_nMode);
	//}}AFX_DATA_MAP

EFS_END
}

BOOL CBO_CAT_PROC_0801::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0801, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0801)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bCat0801START, OnbCat0801START)
	ON_BN_CLICKED(IDC_bAPPLY, OnbAPPLY)
	ON_BN_CLICKED(IDC_bAPPLY_ALL, OnbAPPLYALL)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MODIFY, OnSelchangeTabModify)
	ON_BN_CLICKED(IDC_bCat0801PREV, OnbCat0801PREV)
	ON_BN_CLICKED(IDC_bCat0801NEXT, OnbCat0801NEXT)
	ON_BN_CLICKED(IDC_r008_MARC, Onr008MARC)
	ON_BN_CLICKED(IDC_rNORMAL_MARC, OnrNORMALMARC)
	ON_CBN_SELCHANGE(IDC_cFORM, OnSelchangecFORM)
	ON_BN_CLICKED(IDC_bFIND_CODE_FROM, OnbFINDCODEFROM)
	ON_BN_CLICKED(IDC_bSTATIC, OnbSTATIC)
	ON_BN_CLICKED(IDC_rINSERT_MODE, OnrINSERTMODE)
	ON_BN_CLICKED(IDC_rDELETE_MODE, OnrDELETEMODE)
	ON_BN_CLICKED(IDC_rUPDATE_MODE, OnrUPDATEMODE)
	ON_BN_CLICKED(IDC_bFIND_CODE_FROM_TO, OnbFINDCODEFROMTO)
	ON_CBN_SELCHANGE(IDC_c0300FORM_CLASS, OnSelchangec0300FORMCLASS)
	ON_EN_KILLFOCUS(IDC_eTAGNO, OnKillfocuseTAGNO)
	ON_EN_KILLFOCUS(IDC_eNEW_DATA, OnKillfocuseNEWDATA)
	ON_EN_KILLFOCUS(IDC_eOLD_DATA, OnKillfocuseOLDDATA)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0801 Operations

INT CBO_CAT_PROC_0801::CheckMode(CString strTag, CString strInd, CString strCode, CString strData)
{
EFS_BEGIN

	if (m_nMode == MODE_MODIFY)
	{
		if (!strTag.IsEmpty() && !strCode.IsEmpty() && !strData.IsEmpty())
		{
			m_nModifyMode = MODIFY_TAG_SUBFIELD_DATA;
		}
		else if (!strTag.IsEmpty() && !strCode.IsEmpty())
		{
			m_nModifyMode = MODIFY_TAG_SUBFIELD;
		}
		else if (!strTag.IsEmpty() && !strInd.IsEmpty())
		{
			m_nModifyMode = MODIFY_TAG_IND;
		}
		else if (!strTag.IsEmpty())
		{
			m_nModifyMode = MODIFY_TAG;
		}
		else
		{	m_nModifyMode = -1;
			return -1;
		}

	}
	else if (m_nMode == MODE_ADD)
	{
		
		if (!strTag.IsEmpty() && !strInd.IsEmpty() && !strCode.IsEmpty() && !strData.IsEmpty())
		{
			m_nAddMode = ADD_ALL;
		}
		else 
		{
			m_nAddMode = -1;
			return -1;
		}
	}
	else if (m_nMode == MODE_DELETE)
	{

		if (!strTag.IsEmpty() && !strCode.IsEmpty())
		{
			m_nDeleteMode = DELETE_TAG_SUBFIELD;
		}
		else if (!strTag.IsEmpty())
		{
			m_nDeleteMode = DELETE_TAG;
		}
		else 
		{
			m_nDeleteMode = -1;
			return -1;
		}
	}
	else
	{
		return -1;
	}

	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_0801::VerifyData(CString strTag, CString strInd, CString strCode, CString strData, CString strNewData)
{
EFS_BEGIN
	
	if (m_nMode == MODE_ADD && m_nAddMode == ADD_ALL)
	{
		if (strTag.IsEmpty() || strInd.IsEmpty() || strCode.IsEmpty() || strData.IsEmpty())
			{
				AfxMessageBox ( _T("������ �׸��� Ʋ�Ƚ��ϴ�.") );
				return -1;
			}
	}
	else if (m_nMode == MODE_DELETE)
	{
		if (m_nDeleteMode == DELETE_TAG_SUBFIELD)
		{
			if (strTag.IsEmpty() || strCode.IsEmpty())
			{
				AfxMessageBox ( _T("������ �׸��� Ʋ�Ƚ��ϴ�.") );
				return -1;
			}
		}		
		else if (m_nDeleteMode == DELETE_TAG)
		{
			if (strTag.IsEmpty())
			{
				AfxMessageBox ( _T("������ �׸��� Ʋ�Ƚ��ϴ�.") );
				return -1;
			}
		}		
		else 
		{		
			return -1;
		}
	}
	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_0801::MakeList()
{
EFS_BEGIN

	INT ids = -1;

	CMarc marc;
	CString strStreamMarc;
	CString strReckey;
	CArray<CString, CString> arrayErrorReckey;
	arrayErrorReckey.RemoveAll();
	
	CArray<CString, CString> arrayModifyReckey;
	arrayModifyReckey.RemoveAll();

	SelectMakeList(m_strParentCMAlias, m_strParentGridAlias);		
	INT idx = SelectGetHeadPosition(m_strParentCMAlias, m_strParentGridAlias);
	
	while ( idx != -1 ) {
		strStreamMarc = m_pDM_Main->GetCellData( _T("BS_MARC") , idx );
	
		ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &m_marc);
		ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
		if (ids >= 0)
		{
			ids = CheckMatchCondition(&marc);
			if (ids == 0)
			{
				strReckey = m_pDM_Main->GetCellData(_T("BS_��KEY"), idx);				
				if (!strReckey.IsEmpty()) {
					arrayModifyReckey.Add(strReckey);
					m_aIdx.Add( idx );
				}
			}
			else if (ids == -200)
			{
				strReckey = m_pDM_Main->GetCellData(_T("BS_��KEY"), idx);
				if (!strReckey.IsEmpty()) {
					arrayErrorReckey.Add(strReckey);
					m_aIdx.Add( idx );
				}
			}
		}

		idx = SelectGetNext(m_strParentCMAlias, m_strParentGridAlias);
	}

	
	if ( arrayModifyReckey.GetSize() > 0)
	{
		ShowModifyList(arrayModifyReckey);
	}
	else if ( arrayErrorReckey.GetSize() > 0)
	{
		ShowErrorList(arrayErrorReckey);
	}

	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_0801::CheckMatchCondition(CMarc *pMarc)
{
EFS_BEGIN

	if (pMarc == NULL) return -1;
	
	CString strTag;
	CString strInd;
	CString strCode;
	CString strData;

	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("TAG"), strTag);
	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("���ñ�ȣ"), strInd);
	if (m_nClass == GENERIC)
	{
		GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("�ĺ���ȣ"), strCode);
	}
	else if (m_nClass == STATIC)
	{
		GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("�ĺ���ȣ�޺�"), strCode);		
	}
	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("������DATA"), strData);

	CString strNewData;
	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("������DATA"), strNewData);

	INT ids = -1;
	CString strResult;

	CString sFormCode;
	GetControlData( _T("CM_BO_CAT_PROC_0800"), _T("���ı���_�ڵ�"), sFormCode );

	if ( m_nMode == MODE_MODIFY )
	{
		CString sData;
		if (m_nClass == STATIC)	{

			CString sData;
			CString s008StaticLength =  Get008StaticLengthRange(strCode , sFormCode, strNewData);

			m_pInfo->pMarcMgr->GetItem( pMarc, s008StaticLength, sData );

			if ( sData == _T("") ) {
				return -200;
			}

			sData.TrimRight();
			if ( sData == strData ) {
				return 0;
			}
			else {
				return -100;
			}			

		}

		if ( m_nModifyMode == MODIFY_TAG )
		{
			if (!CheckMarcData(pMarc, strTag, strInd))
				return -100;

			return 0;
		}
		else if (m_nModifyMode == MODIFY_TAG_SUBFIELD)
		{
			if (!CheckMarcData(pMarc, strTag, strInd, strCode))
				return -100;

			return 0;
		}	
		else if (m_nModifyMode == MODIFY_TAG_IND)
		{
			if (!CheckMarcData(pMarc, strTag, strInd, strCode))
				return -100;
			
			return 0;
		}
		else if (m_nModifyMode == MODIFY_TAG_SUBFIELD_DATA)
		{
			if (!CheckMarcData(pMarc, strTag, strInd, strCode, strData))
				return -100;
			
			return 0;
		}
		else 
		{
			return -1;
		}
	}
	else if (m_nMode == MODE_ADD)
	{
		CString sData;
		if (m_nClass == STATIC)	{			
			return 0;
		}
		
		if (m_nAddMode == ADD_ALL)
		{
			if (CheckMarcData(pMarc, strTag, _T(""), strCode, _T("")))
			{	
				strCode.Replace(SUBFIELD_CODE, '$');
				ids = m_pInfo->pMarcMgr->CheckSimpleSyntax(strTag + strCode, CHK_REPEAT_SUBFIELD, strResult, FALSE);
				
				if (ids < 0)
				{
					ids = m_pInfo->pMarcMgr->CheckSimpleSyntax(strTag, CHK_REPEAT_TAG, strResult, FALSE);
					if (ids < 0)
					{
						return -200;
					}
				}
			}
			return 0;
		}
		else 
		{			
			return -1;
		}
	}
	else if (m_nMode == MODE_DELETE)
	{
		CString sData;
		if (m_nClass == STATIC)	{					
			return 0;
		}

		if (m_nDeleteMode == DELETE_TAG_SUBFIELD)
		{
			if (!CheckMarcData(pMarc, strTag, strInd, strCode))
			{
				return -100;
			}
		}		
		else if (m_nDeleteMode == DELETE_TAG)
		{
			if (!CheckMarcData(pMarc, strTag, strInd, strCode, strData))
			{
				return -100;
			}
		}		
		else 
		{		
			return -1;
		}
	}
	else
	{
		return -1;
	}

	return 0;

EFS_END
return -1;

}

BOOL CBO_CAT_PROC_0801::CheckMarcData(CMarc *pMarc, CString strTag /*= _T("")*/, CString strInd /*= _T("")*/, CString strSubfieldCode /*= _T("")*/, CString strData /*= _T("")*/)
{
EFS_BEGIN

	if (pMarc == NULL) return FALSE;

	CString strTemp = strSubfieldCode;
	if (!strTemp.IsEmpty())
		strSubfieldCode.Format(_T("%c%s"), SUBFIELD_CODE, strTemp);

	INT nRequestCount = 0;
	INT nResultCount = 0;

	if (!strTag.IsEmpty())
		nRequestCount ++;
	if (!strInd.IsEmpty())
		nRequestCount ++;
	if (!strSubfieldCode.IsEmpty())
		nRequestCount ++;
	if (!strData.IsEmpty())
		nRequestCount ++;
	

	POSITION posTag = NULL;
	POSITION posSubfield = NULL;
	
	CTag *pTag = NULL;
	CSubfield *pSubfield = NULL;

	posTag = pMarc->m_listTag.GetHeadPosition();
	while (posTag)
	{
		pTag = (CTag*)pMarc->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;

		nResultCount = 0;

		if (!strTag.IsEmpty())
		{
			if (pTag->GetTagCode() == strTag)
				nResultCount++;
			else
				continue;
		}

		if (!strInd.IsEmpty())
		{
			strTemp = pTag->GetFirstIndicator() + pTag->GetSecondIndicator();
			if (strInd == strTemp) 
				nResultCount++;
			else 
				continue;
		}

		if (nRequestCount == nResultCount)
		{
			return TRUE;
		}

		if (strSubfieldCode.IsEmpty())
			continue;

		posSubfield = pTag->m_listSubfield.GetHeadPosition();
		while (posSubfield)
		{
			pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
			if (pSubfield == NULL) continue;

			INT nCount = 0;
			if (strSubfieldCode == pSubfield->GetSubfieldCode())
			{
				nCount++;
				if (!strData.IsEmpty())
				{
					if (pSubfield->GetItemData().Find(strData) >= 0)
					{
						nCount ++;
					}
				}
			}

			nResultCount += nCount;
			
			if (nRequestCount == nResultCount)
			{
				return TRUE;
			}
			else
			{
				nResultCount -= nCount;
			}
		}
	}

	return FALSE;

EFS_END
return FALSE;

}

INT CBO_CAT_PROC_0801::Display(INT idx)
{
EFS_BEGIN	

	INT ids;

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_PROC_0800_MODIFY"));
	if (pDM == NULL) return -1;

	INT nRowCount = pDM->GetRowCount();

	if (idx < 0 && nRowCount != 0)
	{
		AfxMessageBox(_T("ó�� �ڷ��Դϴ�."));
		m_idx = 0;
		return -1;
	}


	if (idx >= nRowCount)
	{
		if ( idx != 0 ) {
			AfxMessageBox(_T("������ �ڷ��Դϴ�."));
		}
		m_idx = nRowCount - 1;
		return -1;
	}
	

	CString strMarc;
	ids = pDM->GetCellData(_T("BS_MARC"), idx, strMarc);
	if (ids < 0)
		return -1;

	CMarc marc;
	m_pInfo->pMarcMgr->Decoding(strMarc, &marc);	
	m_marcEditor.Init(m_pInfo->pMarcMgr, &marc);	

	ids = m_pInfo->pMarcMgr->CheckDeepSyntax(&marc);
	if ( ids < 0 ) return FALSE;
	
	m_marcEditor.Display();

	CString strLeader;
	marc.GetLeader(strLeader);
	SetControlData(_T("CM_BO_CAT_PROC_0800"), _T("����"), strLeader);

	CString strRegNo;
	pDM->GetCellData(_T("���۵�Ϲ�ȣ"), idx, strRegNo);
	SetControlData(_T("CM_BO_CAT_PROC_0800"), _T("��Ϲ�ȣ"), strRegNo);	

	GetDlgItem(IDC_bAPPLY)->EnableWindow(true);

	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_0801::Update(INT idx)
{
EFS_BEGIN

	if (idx < 0)
		return -1;

	INT ids;

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_PROC_0800_MODIFY"));
	if (pDM == NULL) return -1;	

	if (idx >= pDM->GetRowCount())
		return -1;

	CString strMarc;	
	ids = pDM->GetCellData(_T("BS_MARC"), idx, strMarc);
	if (ids < 0) return -1;	
	
	CMarc marc;
	m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	
	CString strTag;
	CString strInd;
	CString strCode;
	CString strData;

	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("TAG"), strTag);
	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("���ñ�ȣ"), strInd);

	if (m_nClass == GENERIC)
	{
		GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("�ĺ���ȣ"), strCode);
	}
	else if (m_nClass == STATIC)
	{
		GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("�������"), strCode);
	}
	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("������DATA"), strData);

	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cFORM);
	INT nCurSel = pComboBox->GetCurSel();

	CString sComboAlias;
	if (nCurSel >= 0) {		
		pComboBox->GetLBText(nCurSel, sComboAlias);
	}
	
	CString strNewData;

	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("������DATA"), strNewData);

	CString strResult;

	CString sFormCode;
	GetControlData( _T("CM_BO_CAT_PROC_0800"), _T("���ı���_�ڵ�"), sFormCode );

	if ( m_nMode == MODE_MODIFY )
	{
		
		CString sData;
		if (m_nClass == STATIC)	{	

			CString s008StaticLength = Get008StaticLengthRange(sComboAlias, sFormCode, strNewData);

			if ( !Update008ToMarcAndDB( &marc, pDM, s008StaticLength, strNewData, idx ) ) return -1;

			return 0;
		}
		if ( m_nModifyMode == MODIFY_TAG )
		{	
			if ( !ExecuteModifyTag( &marc, pDM, strTag, strInd, strNewData, idx ) ) 
			{				
				return -1;
			}
		}
		else if (m_nModifyMode == MODIFY_TAG_SUBFIELD)
		{	
			if ( !ExecuteModifyTagSubfield( &marc, pDM, strTag, strCode, strNewData, idx) ) 
			{				
				return -1;
			}
		}	
		else if (m_nModifyMode == MODIFY_TAG_SUBFIELD_DATA)
		{				
			if ( ExecuteModifyTagSubfieldData( &marc, pDM, strTag, strCode, strData, strNewData, idx) ) 
			{
				UpdateIndex(idx);
				return 0;
			}
			else
			{
				return 0;
			}
		}
		else if (m_nModifyMode == MODIFY_TAG_IND)
		{	
			if (!ExecuteModifyTagInd( &marc, pDM, strTag, strInd, idx )) return -1;
		}
		else 
		{
			return -1;
		}

	}
	else if (m_nMode == MODE_ADD)
	{
		
		CString sData;
		if (m_nClass == STATIC)	{
			CString s008StaticLength = Get008StaticLengthRange(sComboAlias, sFormCode, strNewData);
			
			if ( !Insert008ToMarcAndDB( &marc, pDM, s008StaticLength, strData, idx ) ) return -1;
		
			return 0;

		}
		
		if (m_nAddMode == ADD_ALL)
		{
			if ( ExecuteAddAll( &marc, pDM, strTag, strCode, strInd, strData, strNewData, idx ) ) 
			{
				UpdateIndex(idx);
				return 0;
			}
			else
			{
				return 0;
			}
		}
		else 
		{			
			return -1;
		}
	}
	else if (m_nMode == MODE_DELETE)
	{
		
		CString sData;
		if (m_nClass == STATIC)	{
			CString s008StaticLength = Get008StaticLengthRange(sComboAlias, sFormCode, strNewData);

			if ( !Delete008FromMarcAndDB( &marc, pDM, s008StaticLength, sComboAlias, strData, idx ) ) return -1;

			return 0;
		}
		
		if (m_nDeleteMode == DELETE_TAG)
		{
			if ( !ExecuteDeleteTag(&marc, pDM, strTag, idx) ) return -1;
		}		
		else if (m_nDeleteMode == DELETE_TAG_SUBFIELD)	
		{
			if ( !ExecuteDeleteTagSubfield(&marc, pDM, strTag, strCode, idx) ) return -1;
		}
		else 
		{		
			return -1;
		}
	}
	else
	{
		return -1;
	}	

	GetDlgItem(IDC_bAPPLY)->EnableWindow(false);

	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_0801::ShowFormCodeStatus(CComboBox *pComboBox)
{
EFS_BEGIN

	if (pComboBox == NULL) return -1;

	const INT nReturn100Cnt = 9;
	const INT nReturn200Cnt = 29;
	const INT nReturn300Cnt = 4;

	CString strReturn100Data[nReturn100Cnt] = {
		_T("�Է�����"), _T("�����1"), _T("�����2"), _T("��ǥ��"),
		_T("�󿵽ð�"),
		_T("������")
	};

	CString strReturn200Data[nReturn200Cnt] = {
		_T("���������"),
		_T("�̿����"), _T("�ڷ�ĺ�"),
		_T("��������"),
		_T("����"), _T("ȸ��"),
		_T("����"), _T("����"),
		_T("�������"), _T("��������"),
		_T("����"),
		_T("�����ڷ�1"), _T("�����ڷ�2"), _T("�����ڷ�3"), _T("�����ڷ�4"),
		_T("�ڷ�����"), _T("�������"),
		_T("�����ڷ��ǳ���"),
		_T("��������"), _T("�Ǻ�����"), _T("�ڷ�����"),
		_T("����"), _T("�����ڿ���"), _T("�����ڷ�����"), _T("���»���Ư¡1"), _T("���»���Ư¡2"),
		_T("�̿����ڼ���"), _T("��ǻ����������"), _T("�������ڵ�")
	};

	CString strReturn300Data[nReturn300Cnt] = {
		_T("���౹��ȣ"), 
		_T("�ѱ����к�ȣ"),
		_T("���к�ȣ"),
		_T("�ѱ����α����ȣ")
	};

	INT nCurSel = pComboBox->GetCurSel();
	if (nCurSel < 0) return -1;

	CString strData;
	pComboBox->GetLBText(nCurSel, strData);
	if (strData.IsEmpty()) return -1;

	INT i = 0;
	for (i = 0; i <nReturn100Cnt; i++)
	{
		if (strData == strReturn100Data[i])
			return 100;
	}

	for (i = 0; i <nReturn200Cnt; i++)
	{
		if (strData == strReturn200Data[i])
			return 200;
	}

	for (i = 0; i <nReturn300Cnt; i++)
	{
		if (strData == strReturn300Data[i])
			return 300;
	}
		
	return -1;

EFS_END
return -1;

}

BOOL CBO_CAT_PROC_0801::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	m_idx = 0;

	m_bReSearch = FALSE;

	m_pDM_Main = NULL;

	m_strInd = _T("");

	if (InitESL_Mgr(_T("BO_CAT_PROC_0800")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pDM_Main = FindDM( m_strParentDMAlias );
	if ( m_pDM_Main == NULL ) return -1;
	
	if (m_strParentCMAlias.IsEmpty())
		m_strParentCMAlias = _T("CM_BO_CAT_PROC_0000");
	if (m_strParentDMAlias.IsEmpty())
		m_strParentDMAlias = _T("DM_BO_CAT_PROC_0000");
	if (m_strParentGridAlias.IsEmpty())
		m_strParentGridAlias = _T("ProcGrid");

	INT ids = SelectMakeList(m_strParentCMAlias, m_strParentGridAlias);
	if ( 0 != ids ) {
		AfxMessageBox ( _T("SelectMakeList error") );
	}
	INT idx = SelectGetHeadPosition(m_strParentCMAlias, m_strParentGridAlias);
	if (idx < 0)
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�."));
		EndDialog(IDCANCEL);
		m_bInitDlg = FALSE;
		return -1;
	}

	CheckDlgButton(IDC_rNORMAL_MARC, 1);
	CheckDlgButton(IDC_rUPDATE_MODE, 1);

	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_MODIFY);
	if (pTabCtrl)
	{
		pTabCtrl->InsertItem(0, _T("��������Ʈ"));
		pTabCtrl->InsertItem(1, _T("��ũ"));
		pTabCtrl->InsertItem(2, _T("��������Ʈ"));

		pTabCtrl->SetCurFocus(1);
		pTabCtrl->SetCurFocus(0);
	}

	CString sFormCode;
	GetControlData( _T("CM_BO_CAT_PROC_0800"), _T("���ı���_�ڵ�"), sFormCode );

	const INT nComboData = 19;	
	CString strComboData[nComboData] = {
		_T("�Է�����"), _T("���������"),
		_T("�����1"), _T("�����2"),
		_T("���౹��ȣ"), _T("��ǥ��"),
		_T("�̿����"), _T("�ڷ�ĺ�"),
		_T("��������"), _T("�ѱ����к�ȣ"),
		_T("����"), _T("ȸ��"),
		_T("����"), _T("����"),
		_T("�������"), _T("��������"),
		_T("����"), _T("���к�ȣ"),
		_T("�ѱ����α����ȣ")
	};
	CComboBox *pCombo = NULL;

	
	pCombo = (CComboBox*)GetDlgItem(IDC_cFORM);
	if (pCombo == NULL) return FALSE;

	for (INT j = 0; j < nComboData; j++)
	{
		pCombo->InsertString(j, strComboData[j]);
	}

	UpdateData();
	if (m_nClass == 0)
	{
		GetDlgItem(IDC_cFORM)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_eSUBFIELDCODE)->ShowWindow(SW_SHOW);
	}

	m_marcEditor.SubclassDlgItem(IDC_REDIT_0800, -1, this);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));

	InitMemberData();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0801::OnbCat0801START() 
{
EFS_BEGIN

	INT ids;

	UpdateData();
	
	CString strTag, strCode, strData;
	
	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("TAG"), strTag);
	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("���ñ�ȣ"), m_strInd);
	
	if (m_nClass == GENERIC)
	{
		GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("�ĺ���ȣ"), strCode);
	}
	else if (m_nClass == STATIC)
	{
		GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("�������"), strCode);
	}
	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("������DATA"), strData);
	
	if (strTag.IsEmpty()) return;
	
	CString strNewData;
	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("������DATA"), strNewData);
	
	if ( !CheckNoChangeTag(strTag, strCode) ) return;

	ids = CheckMode(strTag, m_strInd, strCode, strData);
	if (ids < 0) {
		AfxMessageBox ( _T("�Է»����� Ʋ���ϴ�.") );
		return;
	}

	ids = VerifyData(strTag, m_strInd, strCode, strData, strNewData);
	if (ids < 0) {
		AfxMessageBox ( _T("������ ���� ����") );
		return;
	}
	if ( ids == 100 ) 
	{
		return;
	}

	if ( !InitModifyErrorList() ) return ;

	ids = MakeList();
	if (ids < 0) {
		AfxMessageBox ( _T("MakeList ����") );
		return;
	}

	if ( !ShowFirstData() ) return;

EFS_END
}

VOID CBO_CAT_PROC_0801::OnbAPPLY() 
{
EFS_BEGIN

	INT ids = Update(m_idx);

	if ( ids < 0 ) return;

	AllControlDisplay( _T("CM_BO_CAT_PROC_0800") );
	AllControlDisplay( m_strParentCMAlias );

	AfxMessageBox( _T("�����߽��ϴ�.") );


EFS_END
}

VOID CBO_CAT_PROC_0801::OnbAPPLYALL() 
{
EFS_BEGIN

	if ( CheckSeprateShelfCode() == FALSE) return;

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_PROC_0800_MODIFY"));
	if (pDM == NULL) return;
	INT ids;

	INT nRowCount = pDM->GetRowCount();
	for ( INT i = 0 ; i < nRowCount ; i++ ) {
		ids = Update(i);
	}

	AllControlDisplay( _T("CM_BO_CAT_PROC_0800") );

	CString sMessageFormat;
	CString sPreModifyCount;

	sPreModifyCount.Format(_T("%d"), m_iPreModifyCount);

	CString sPostModifyCount;
	sPostModifyCount.Format(_T("%d"), m_iPostModifyCount);
	

		
	sMessageFormat =  _T("������ �ڷ� �Ǽ� : ") + sPreModifyCount;
	sMessageFormat = sMessageFormat + _T(" ���� \n������ �ڷ� �Ǽ� : ");
	sMessageFormat = sMessageFormat + sPostModifyCount;
	sMessageFormat = sMessageFormat + _T(" ���� \n �ϰ� ���� �߽��ϴ�.");

	AfxMessageBox( sMessageFormat );
	
	m_bReSearch = TRUE;

	m_iPostModifyCount = 0;

	GetDlgItem(IDC_bAPPLY)->EnableWindow(false);

EFS_END
}

VOID CBO_CAT_PROC_0801::OnSelchangeTabModify(NMHDR* pNMHDR, LRESULT* pResult) 
{
EFS_BEGIN

	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_MODIFY);
	if (pTabCtrl == NULL) return;

	const INT nMarcResource = 7;
	INT nArrMarcResource[nMarcResource] = {
		IDC_STATIC_REG, IDC_eACCESSION_NO,
		IDC_eLEADER, IDC_REDIT_0800,
		IDC_bCat0801PREV, IDC_bCat0801NEXT,
		IDC_bAPPLY 
	};

	INT i = 0;
	CMSHFlexGrid *pGrid_Modify = (CMSHFlexGrid*)GetDlgItem(IDC_GRD_LIST);
	CMSHFlexGrid *pGrid_Error = (CMSHFlexGrid*)GetDlgItem(IDC_GRD_ERROR);
	if (pGrid_Error == NULL || pGrid_Modify == NULL) return;
	
	INT nCurSel = pTabCtrl->GetCurSel();
	if (nCurSel == 0)
	{
		pGrid_Modify->ShowWindow(SW_SHOW);
		pGrid_Error->ShowWindow(SW_HIDE);

		for (i = 0; i < nMarcResource; i++)
			GetDlgItem(nArrMarcResource[i])->ShowWindow(SW_HIDE);
	}
	else if (nCurSel == 1)
	{
		pGrid_Modify->ShowWindow(SW_HIDE);
		pGrid_Error->ShowWindow(SW_HIDE);

		for (i = 0; i < nMarcResource; i++)
			GetDlgItem(nArrMarcResource[i])->ShowWindow(SW_SHOW);
	}
	else if (nCurSel == 2)
	{
		pGrid_Modify->ShowWindow(SW_HIDE);
		pGrid_Error->ShowWindow(SW_SHOW);

		for (i = 0; i < nMarcResource; i++)
			GetDlgItem(nArrMarcResource[i])->ShowWindow(SW_HIDE);
	}

	*pResult = 0;

EFS_END
}

VOID CBO_CAT_PROC_0801::OnbCat0801PREV() 
{
EFS_BEGIN

	Display(--m_idx);
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_PROC_0800_MODIFY"));
	if (pDM == NULL) return;
	INT nRowCount = pDM->GetRowCount();
	CString sRowCount;
	sRowCount.Format( _T("%d / %d") , m_idx+1, nRowCount );
	SetControlData( _T("CM_BO_CAT_PROC_0800") , _T("�����Ȳ"), sRowCount );

	CString sFormCode = pDM->GetCellData( _T("BS_���ı���_�ڵ�"), m_idx );
	SetControlData( _T("CM_BO_CAT_PROC_0800"), _T("���ı���_�ڵ�"), sFormCode );

EFS_END
}

VOID CBO_CAT_PROC_0801::OnbCat0801NEXT() 
{
EFS_BEGIN

	Display(++m_idx);

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_PROC_0800_MODIFY"));
	if (pDM == NULL) return;
	INT nRowCount = pDM->GetRowCount();
	CString sRowCount;
	sRowCount.Format( _T("%d / %d") , m_idx+1, nRowCount );
	SetControlData( _T("CM_BO_CAT_PROC_0800") , _T("�����Ȳ"), sRowCount );

	CString sFormCode = pDM->GetCellData( _T("BS_���ı���_�ڵ�"), m_idx );
	SetControlData( _T("CM_BO_CAT_PROC_0800"), _T("���ı���_�ڵ�"), sFormCode );	

EFS_END
}

VOID CBO_CAT_PROC_0801::OnrNORMALMARC() 
{
EFS_BEGIN

	UpdateData();
	if (m_nClass == 0)
	{
		GetDlgItem(IDC_cFORM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_eSUBFIELDCODE)->ShowWindow(SW_SHOW);
	}
	SetControlData( _T("CM_BO_CAT_PROC_0800"), _T("TAG") , _T("") );
	GetDlgItem(IDC_eTAGNO)->EnableWindow(TRUE);
	GetDlgItem(IDC_eJISI_CODE)->EnableWindow(TRUE);

	GetDlgItem(IDC_rINSERT_MODE)->EnableWindow(TRUE);
	GetDlgItem(IDC_rDELETE_MODE)->EnableWindow(TRUE);



	if ( m_nMode == MODE_MODIFY ) {		
		GetDlgItem(IDC_eOLD_DATA)->EnableWindow(TRUE);
		GetDlgItem(IDC_bFIND_CODE_FROM)->EnableWindow(FALSE);

		GetDlgItem(IDC_eNEW_DATA)->EnableWindow(true);
	}

	GetDlgItem(IDC_STATIC1_BCP)->EnableWindow(true);
	
	GetDlgItem(IDC_bFIND_CODE_FROM)->ShowWindow(SW_HIDE);

EFS_END
}

VOID CBO_CAT_PROC_0801::Onr008MARC() 
{
	EFS_BEGIN

	UpdateData();
	if (m_nClass == 1)
	{
		GetDlgItem(IDC_cFORM)->ShowWindow(SW_SHOW);	
		GetDlgItem(IDC_eSUBFIELDCODE)->ShowWindow(SW_HIDE);
	}
	SetControlData( _T("CM_BO_CAT_PROC_0800"), _T("TAG") , _T("008") );

	GetDlgItem(IDC_eTAGNO)->EnableWindow(FALSE);
	GetDlgItem(IDC_eJISI_CODE)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC1_BCP)->EnableWindow(FALSE);
	GetDlgItem(IDC_rINSERT_MODE)->EnableWindow(FALSE);
	GetDlgItem(IDC_rDELETE_MODE)->EnableWindow(FALSE);

	GetDlgItem(IDC_bFIND_CODE_FROM)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_bFIND_CODE_FROM)->ShowWindow(SW_SHOW);

	EFS_END
}

VOID CBO_CAT_PROC_0801::OnSelchangecFORM() 
{
EFS_BEGIN

	INT ids;

	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cFORM);
	if (pComboBox == NULL) return;	

	ids = ShowFormCodeStatus(pComboBox);
	if (ids == 100)
	{
		GetDlgItem(IDC_eOLD_DATA)->EnableWindow(TRUE);
		GetDlgItem(IDC_bFIND_CODE_FROM)->EnableWindow(FALSE);

		GetDlgItem(IDC_eNEW_DATA)->EnableWindow(TRUE);
	}
	else if (ids == 200)
	{
		GetDlgItem(IDC_eOLD_DATA)->EnableWindow(FALSE);
		GetDlgItem(IDC_bFIND_CODE_FROM)->EnableWindow(TRUE);
	}
	else if (ids == 300)
	{
		GetDlgItem(IDC_eOLD_DATA)->EnableWindow(FALSE);
		GetDlgItem(IDC_bFIND_CODE_FROM)->EnableWindow(TRUE);
	}
	
	CString sData;
	GetControlData( _T("CM_BO_CAT_PROC_0800"), _T("�ĺ���ȣ�޺�"), sData );

EFS_END
}


VOID CBO_CAT_PROC_0801::OnbFINDCODEFROM() 
{
EFS_BEGIN

	INT ids;
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cFORM);
	if (pComboBox == NULL) return;

	CString sFormValue;
	pComboBox->GetWindowText(sFormValue);
	
	CString sFormCode;
	GetControlData( _T("CM_BO_CAT_PROC_0800"), _T("���ı���_�ڵ�"), sFormCode );

	ids = ShowFormCodeStatus(pComboBox);
	if (ids == 200)
	{
		CDlgClassCode dlg;

		TCHAR sCurPath[1000];
		GetCurrentDirectory( _MAX_PATH , sCurPath );

		CString sPath = _T("..\\CFG\\008TAG�ڵ�.txt");

		FILE *fp;
		TCHAR cLine[256];
		fp = _tfopen( sPath , _T("rb") );
		if ( fp == NULL ) {
			AfxMessageBox( _T("008TAG�ڵ� ������ �����ϴ�.") );
			return;
		}
		
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
		BOOL i = SetCurrentDirectory( sCurPath );
		
		BOOL bFormYn = FALSE;
		while( _fgetts( cLine, 256, fp) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			sLine.Replace( sLine.Right(1), NULL );

			CString sFormCodeCom = _T("[") + sFormCode + _T("]");
			if ( sLine == sFormCodeCom ) {
				bFormYn = TRUE;
				break;
			}
		}
		if ( bFormYn == FALSE ) {
			AfxMessageBox( _T("���ı����ڵ尡 �����ϴ�.") );
			return;
		}

		CString sValue;
		BOOL bControlYn = FALSE;
		while( _fgetts( cLine, 256, fp) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			if ( sLine.Left(1) == _T("[") ) {
				AfxMessageBox( _T("����������� �����ϴ�.") );
				return;
			}
			sLine.Replace( sLine.Right(1), NULL );

			if ( sFormValue == _T("����") ) {
				sFormValue = _T("�������ڵ�");
			}
			else if ( sFormValue == _T("�����ڷ�1") || sFormValue == _T("�����ڷ�2") || sFormValue == _T("�����ڷ�3") || sFormValue == _T("�����ڷ�4") ) {
				sFormValue = _T("008�����ڷ�");
			}			

			if ( sFormCode == _T("MP") ) {
				if ( sFormValue == _T("���»���Ư¡1") || sFormValue == _T("���»���Ư¡2") ) {
					sFormValue = _T("���»�Ư¡");
				}
			}
			if ( sFormCode == _T("MU") ) {
				if ( sFormValue == _T("�����ڷ��ǳ���") ) {
					sFormValue = _T("�����ڷ᳻��");
				}
			}
			if ( sFormCode == _T("VM") ) {
				if ( sFormValue == _T("�̿����") ) {
					sFormValue = _T("�̿����ڼ���");
				}
			}
			if ( sFormValue == _T("�ڷ�����") ) {
				sFormValue = _T("��û���ڷ�����");
			}
			sValue = _T("(���������)");
			sValue = _T("(") + sFormValue + _T(")");
			if ( sLine == sValue ) {
				bControlYn = TRUE;
				break;
			}
		}
		if ( bControlYn == FALSE ) {
			AfxMessageBox( _T("����������� �����ϴ�.") );
			return;
		}
		
		while( _fgetts( cLine, 256, fp) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			if ( sLine.Left(1) == _T(".") ) {
				break;
			}
			sLine.Replace( sLine.Right(1), NULL );

			if ( sValue == _T("(��������)") || sValue == _T("(����)") ) {
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

		if (dlg.DoModal() == IDOK)
		{
			if ( m_iUpdateLater == 1 ) {
				SetControlData( _T("CM_BO_CAT_PROC_0800"), _T("������DATA"), dlg.m_strSelCode );
				fclose(fp);
			}
			else {
				SetControlData( _T("CM_BO_CAT_PROC_0800"), _T("������DATA"), dlg.m_strSelCode );
				fclose(fp);
			}
		}
	}
	else if (ids == 300) {
		CKOMarcGetterDlg dlg( this );
		INT iMode;
		if ( sFormValue == _T("���౹��ȣ") ) {
			iMode = 0;
		}
		else if ( sFormValue == _T("�ѱ����к�ȣ") ) {
			iMode = 1;
		}
		else if ( sFormValue == _T("���к�ȣ") ) {
			iMode = 2;
		}
		else if ( sFormValue == _T("�ѱ����α����ȣ") ) {
			iMode = 3;
		}
		dlg.InitSetProperty( iMode );
		if ( dlg.DoModal() == IDOK ) {
			CString sCode = dlg.m_code;
			if ( m_iUpdateLater == 1 ) {
				SetControlData( _T("CM_BO_CAT_PROC_0800"), _T("������DATA"), sCode );
			}
			else {
				SetControlData( _T("CM_BO_CAT_PROC_0800"), _T("������DATA"), sCode );
			}
			return;
		}
	}

EFS_END
}

VOID CBO_CAT_PROC_0801::OnbSTATIC() 
{
EFS_BEGIN

	CESL_DataMgr *pDM_Modify = FindDM(_T("DM_BO_CAT_PROC_0800_MODIFY"));
	CESL_DataMgr *pDM_Error = FindDM(_T("DM_BO_CAT_PROC_0800_ERROR"));
	if (pDM_Modify == NULL || pDM_Error == NULL)
		return;

	m_strStreamMarc = pDM_Modify->GetCellData( _T("BS_MARC"), m_idx );
	m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc);

	CString sFormCode;
	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("���ı���_�ڵ�"), sFormCode);

	if ( sFormCode == _T("BK") ) {
		CBO_CAT_PROC_0320_01 dlg(this, m_strParentDMAlias, m_marc);

		if( IDOK == dlg.DoModal() ) {
			CString strStreamMarc = m_strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);

			CString strTagCode, strErrMsg, strHelpMsg;
		
			if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
			{
				POSITION pos;
				pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
				while (pos)
				{
					m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
					UpdateData(FALSE);
				}	
			}

			m_pInfo->pMarcMgr->SortMarc( &m_marc );

			m_marcEditor.InitLimitTag();

			m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
			INT ids = m_pInfo->pMarcMgr->CheckDeepSyntax(&m_marc);
			if ( ids < 0 ) return;
			m_marcEditor.Display();

			TCHAR szLeader[25];
			m_marc.GetLeader(szLeader);
			CString sLeader;
			sLeader.Format( _T("%s") , szLeader );
			SetControlData(_T("CM_BO_CAT_PROC_0800"), _T("����"), sLeader);
		}
	}
	else if ( sFormCode == _T("CF") ) {
		CBO_CAT_PROC_0321 dlg(this, m_strParentDMAlias, m_marc);

		if( IDOK == dlg.DoModal() ) {			
			CString strStreamMarc = m_strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);

			CString strTagCode, strErrMsg, strHelpMsg;
		
			if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
			{
				POSITION pos;
				pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
				while (pos)
				{
					m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);		
					UpdateData(FALSE);
				}	
			}

			m_pInfo->pMarcMgr->SortMarc( &m_marc );
			m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
			INT ids = m_pInfo->pMarcMgr->CheckDeepSyntax(&m_marc);
			if ( ids < 0 ) return;
			m_marcEditor.Display();

			TCHAR szLeader[25];
			m_marc.GetLeader(szLeader);
			CString sLeader;
			sLeader.Format( _T("%s") , szLeader );
			SetControlData(_T("CM_BO_CAT_PROC_0800"), _T("����"), sLeader);
		}
	}
	else if ( sFormCode == _T("MP") ) {
		CBO_CAT_PROC_0322 dlg(this, m_strParentDMAlias, m_marc);

		if( IDOK == dlg.DoModal() ) {			
			CString strStreamMarc = m_strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);

			CString strTagCode, strErrMsg, strHelpMsg;
		
			if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
			{
				POSITION pos;
				pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
				while (pos)
				{
					m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);		
					UpdateData(FALSE);
				}	
			}

			m_pInfo->pMarcMgr->SortMarc( &m_marc );
			m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
			INT ids = m_pInfo->pMarcMgr->CheckDeepSyntax(&m_marc);
			if ( ids < 0 ) return ;
			m_marcEditor.Display();

			TCHAR szLeader[25];
			m_marc.GetLeader(szLeader);
			CString sLeader;
			sLeader.Format( _T("%s") , szLeader );
			SetControlData(_T("CM_BO_CAT_PROC_0800"), _T("����"), sLeader);
		}
	}	
	else if ( sFormCode == _T("MU") ) {
		CBO_CAT_PROC_0323 dlg(this, m_strParentDMAlias, m_marc);
		if( IDOK == dlg.DoModal() ) {			
			CString strStreamMarc = m_strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);

			CString strTagCode, strErrMsg, strHelpMsg;
		
			if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
			{
				POSITION pos;
				pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
				while (pos)
				{
					m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);		
					UpdateData(FALSE);
				}	
			}

			m_pInfo->pMarcMgr->SortMarc( &m_marc );
			m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
			INT ids = m_pInfo->pMarcMgr->CheckDeepSyntax(&m_marc);
			if ( ids < 0 ) return ;
			m_marcEditor.Display();

			TCHAR szLeader[25];
			m_marc.GetLeader(szLeader);
			CString sLeader;
			sLeader.Format( _T("%s") , szLeader );
			SetControlData(_T("CM_BO_CAT_PROC_0800"), _T("����"), sLeader);
		}
	}
	else if ( sFormCode == _T("VM") ) {
		CBO_CAT_PROC_0324 dlg(this, m_strParentDMAlias, m_marc);
		if( IDOK == dlg.DoModal() ) {			
			CString strStreamMarc = m_strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);

			CString strTagCode, strErrMsg, strHelpMsg;
		
			if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
			{
				POSITION pos;
				pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
				while (pos)
				{
					m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);		
					UpdateData(FALSE);
				}	
			}

			m_pInfo->pMarcMgr->SortMarc( &m_marc );
			m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
			INT ids = m_pInfo->pMarcMgr->CheckDeepSyntax(&m_marc);
			if ( ids < 0 ) return ;
			m_marcEditor.Display();

			TCHAR szLeader[25];
			m_marc.GetLeader(szLeader);
			CString sLeader;
			sLeader.Format( _T("%s") , szLeader );
			SetControlData(_T("CM_BO_CAT_PROC_0800"), _T("����"), sLeader);
		}
	}
	else {

		CBO_CAT_PROC_0320_01 dlg(this, m_strParentDMAlias, m_marc);
		if( IDOK == dlg.DoModal() ) {			
			CString strStreamMarc = m_strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);

			CString strTagCode, strErrMsg, strHelpMsg;
		
			if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
			{
				POSITION pos;
				pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
				while (pos)
				{
					m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);		
					UpdateData(FALSE);
				}	
			}

			m_pInfo->pMarcMgr->SortMarc( &m_marc );
			m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
			INT ids = m_pInfo->pMarcMgr->CheckDeepSyntax(&m_marc);
			if ( ids < 0 ) return ;
			m_marcEditor.Display();

			TCHAR szLeader[25];
			m_marc.GetLeader(szLeader);
			CString sLeader;
			sLeader.Format( _T("%s") , szLeader );
			SetControlData(_T("CM_BO_CAT_PROC_0800"), _T("����"), sLeader);
		}
	}
	

EFS_END
}

VOID CBO_CAT_PROC_0801::OnrINSERTMODE() 
{
EFS_BEGIN

	const INT nMarcResource = 2;
	INT nArrMarcResource[nMarcResource] = {
		IDC_STATIC_DATA,
		IDC_eNEW_DATA
	};
	
	GetDlgItem(IDC_STATIC6_BCP)->EnableWindow(TRUE);
	GetDlgItem(IDC_eJISI_CODE)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC1_BCP)->EnableWindow(TRUE);

	for ( INT i = 0; i < nMarcResource; i++) {
		GetDlgItem(nArrMarcResource[i])->EnableWindow(false);
	}

EFS_END
}

VOID CBO_CAT_PROC_0801::OnrDELETEMODE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	const INT nMarcResource = 4;
	INT nArrMarcResource[nMarcResource] = {
		IDC_STATIC6_BCP,
		IDC_eJISI_CODE,
		IDC_STATIC_DATA,
		IDC_eNEW_DATA
	};


	GetDlgItem(IDC_STATIC1_BCP)->EnableWindow(TRUE);

	for ( INT i = 0; i < nMarcResource; i++) {
		GetDlgItem(nArrMarcResource[i])->EnableWindow(false);
	}	

EFS_END
}

VOID CBO_CAT_PROC_0801::OnrUPDATEMODE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	const INT nMarcResource = 5;
	INT nArrMarcResource[nMarcResource] = {
			IDC_STATIC_DATA,
			IDC_eNEW_DATA, 
			IDC_STATIC1_BCP, 
			IDC_eJISI_CODE, 
			IDC_STATIC6_BCP
		};

	for ( INT i = 0; i < nMarcResource; i++) {
		GetDlgItem(nArrMarcResource[i])->EnableWindow(true);
	}

	CButton * pStartReg;
	pStartReg=(CButton *)GetDlgItem(IDC_r008_MARC);
	if ( pStartReg->GetCheck() == 1 ) {
		GetDlgItem(IDC_eNEW_DATA)->EnableWindow(FALSE);
		
	}
	else {
	}
	

EFS_END
}

VOID CBO_CAT_PROC_0801::OnCancel() 
{
EFS_BEGIN

	AfxMessageBox(_T("����� ��ũ�� �����ϱ� ���Ͽ� ��˻��� �մϴ�. \n�˻��� �׸��� ����� ��� �˻��� �ȵ� �� �ֽ��ϴ�."));
	
	CESL_DataMgr *pDM_PROC_0000;
	pDM_PROC_0000 = FindDM(m_strParentDMAlias);
	if ( pDM_PROC_0000 == NULL ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return;
	}


	if ( m_bReSearch == TRUE ) 
	{
		CESL_DataMgr *pWhere;
		pWhere = FindDM( _T("DM_BO_CAT_PROC_WHERE") );
		if ( pWhere == NULL ) {
			AfxMessageBox( _T("DM_BO_CAT_PROC_WHERE ERROR") );
			return;
		}
		CString sWhere;
		pWhere->GetCellData( _T("������"), 0, sWhere );

		if (pDM_PROC_0000->RefreshDataManager(sWhere) < 0)	return;
		pDM_PROC_0000->SORT( _T("���۵�Ϲ�ȣ"), FALSE );
	}



	CESL_ControlMgr *pCM;
	pCM = FindCM(m_strParentCMAlias);
	if ( pCM == NULL ) {
		AfxMessageBox ( _T("CM_BO_CAT_PROC_0000 error") );
		return;
	}
	for ( INT i = 0 ; i < pDM_PROC_0000->GetRowCount() ; i++ ) {
		CString sCatalogStatus = pDM_PROC_0000->GetCellData( _T("BS_��������"), i );
		if ( sCatalogStatus == _T("0") ) {
			pDM_PROC_0000->SetCellData( _T("BS_��������"), _T("�����ȵ�"), i );
		}
		else if ( sCatalogStatus == _T("1") ) {
			pDM_PROC_0000->SetCellData( _T("BS_��������"), _T("������"), i );
		}
	}
	pCM->AllControlDisplay();

	CDialog::OnCancel();

EFS_END
}

VOID CBO_CAT_PROC_0801::OnbFINDCODEFROMTO() 
{
	// TODO: Add your control notification handler code here
	m_iUpdateLater = 1;
	OnbFINDCODEFROM();
	m_iUpdateLater = 0;
	return;

	INT ids;

	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cFORM);
	if (pComboBox == NULL) return;

	CString sFormValue;
	pComboBox->GetWindowText(sFormValue);
	
	ids = ShowFormCodeStatus(pComboBox);
	if (ids == 200)
	{
		CDlgClassCode dlg;
		TCHAR sCurPath[1000];
		GetCurrentDirectory( _MAX_PATH , sCurPath );

		CString sPath = _T("..\\CFG\\008TAG�ڵ�.txt");
		FILE *fp;
		TCHAR cLine[256];
		fp = _tfopen( sPath , _T("rb") );
		if ( fp == NULL ) {
			AfxMessageBox( _T("008TAG�ڵ� ������ �����ϴ�.") );
			return;
		}
		
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
		SetCurrentDirectory( sCurPath );

		BOOL bFormYn = FALSE;
		while( _fgetts( cLine, 256, fp) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			sLine.Replace( sLine.Right(1), NULL );					
			if ( sLine == _T("[TX]") ) {
				bFormYn = TRUE;
				break;
			}
		}
		if ( bFormYn == FALSE ) {
			AfxMessageBox( _T("[TX]�� �����ϴ�.") );
			return;
		}
		
		BOOL bControlYn = FALSE;
		while( _fgetts( cLine, 256, fp) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			if ( sLine.Left(1) == _T("[") ) {
				return;
			}
			sLine.Replace( sLine.Right(1), NULL );
			CString sValue = _T("(���������)");
			if ( sLine == sValue ) {
				bControlYn = TRUE;
				break;
			}
		}

		while( _fgetts( cLine, 256, fp) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			if ( sLine.Left(1) == _T(".") ) {
				break;
			}
			sLine.Replace( sLine.Right(1), NULL );
			CString sCode, sDesc;
			sCode = sLine.Left(1);
			sDesc = sLine.Mid(2);			
			dlg.AddData( sCode, sDesc );
		}

		if (dlg.DoModal() == IDOK)
		{
			SetControlData( _T("CM_BO_CAT_PROC_0800"), _T("������DATA"), dlg.m_strSelCode );
		}
	}
	else if (ids == 300) {
		CKOMarcGetterDlg dlg( this );
		INT iMode;
		if ( sFormValue == _T("���౹��ȣ") ) {
			iMode = 0;
		}
		else if ( sFormValue == _T("�ѱ����к�ȣ") ) {
			iMode = 1;
		}
		else if ( sFormValue == _T("���к�ȣ") ) {
			iMode = 2;
		}
		else if ( sFormValue == _T("�ѱ����α����ȣ") ) {
			iMode = 3;
		}
		dlg.InitSetProperty( iMode );
		if ( dlg.DoModal() == IDOK ) {
			CString sCode = dlg.m_code;
			SetControlData( _T("CM_BO_CAT_PROC_0800"), _T("������DATA"), sCode );
			return;
		}
	}
}

VOID CBO_CAT_PROC_0801::OnSelchangec0300FORMCLASS() 
{
EFS_BEGIN


	CString sFormCode;
	GetControlData( _T("CM_BO_CAT_PROC_0800"), _T("���ı���_�ڵ�"), sFormCode );

	SetInsertStringToCombo(sFormCode);

EFS_END	
}

CString CBO_CAT_PROC_0801::Get008StaticLengthRange(CString sComboAlias, CString sFormCode, CString strNewData)
{
EFS_BEGIN

	if ( sComboAlias == _T("") ) return _T("");

	CString sStaticLengthRange;

	if ( sComboAlias == _T("�Է�����") ) {
			sStaticLengthRange= _T("008@0-5");
		}
		else if ( sComboAlias == _T("���������") ) {
			sStaticLengthRange= _T("008@6");
		}
		else if ( sComboAlias == _T("�����1") ) {
			sStaticLengthRange= _T("008@7-10");
		}
		else if ( sComboAlias == _T("�����2") ) {
			sStaticLengthRange= _T("008@11-14");
		}
		else if ( sComboAlias == _T("���౹��ȣ") ) {
			sStaticLengthRange= _T("008@15-17");
			if ( strNewData.GetLength() == 2 ) {
				strNewData = strNewData + _T(" ");
			}
		}
		else if ( sComboAlias == _T("��ǥ��") ) {
			sStaticLengthRange= _T("008@18-21");
		}
		else if ( sComboAlias == _T("��ǻ����������") ) {
			sStaticLengthRange= _T("008@25");
		}
		else if ( sComboAlias == _T("��������") ) {
			sStaticLengthRange= _T("008@18-19");
		}
		else if ( sComboAlias == _T("�Ǻ�����") ) {
			sStaticLengthRange= _T("008@20");
		}
		else if ( sComboAlias == _T("�ڷ�����") ) {
			sStaticLengthRange= _T("008@23");
		}			
		else if ( sComboAlias == _T("�����ڷ�1") && sFormCode == _T("MU") ) {
			sStaticLengthRange= _T("008@24");
		}
		else if ( sComboAlias == _T("�����ڷ�2") && sFormCode == _T("MU") ) {
			sStaticLengthRange= _T("008@25");
		}
		else if ( sComboAlias == _T("�����ڷ��ǳ���1") ) {
			sStaticLengthRange= _T("008@30");
		}
		else if ( sComboAlias == _T("�����ڷ��ǳ���2") ) {
			sStaticLengthRange= _T("008@31");
		}

		else if ( sComboAlias == _T("�󿵽ð�") ) {
			sStaticLengthRange= _T("008@18-20");
		}
		else if ( sComboAlias == _T("�̿����") || sComboAlias == _T("�̿����ڼ���") ) {
			sStaticLengthRange= _T("008@22");
		}
		else if ( sComboAlias == _T("�����ڷ�1") ) {
			sStaticLengthRange= _T("008@23");
		}
		else if ( sComboAlias == _T("�����ڷ�2") ) {
			sStaticLengthRange= _T("008@24");
		}
		else if ( sComboAlias == _T("�����ڷ�3") ) {
			sStaticLengthRange= _T("008@25");
		}
		else if ( sComboAlias == _T("�ڷ�����") ) {
			sStaticLengthRange= _T("008@33");
		}
		else if ( sComboAlias == _T("�������") ) {
			sStaticLengthRange= _T("008@34");
		}

		
		else if ( sComboAlias == _T("�ڷ�ĺ�") ) {
			sStaticLengthRange= _T("008@23");
		}
		else if ( sComboAlias == _T("��������") ) {
			sStaticLengthRange= _T("008@24-25");
		}
		else if ( sComboAlias == _T("�ѱ����к�ȣ") ) {
			sStaticLengthRange= _T("008@26-27");				
		}
		else if ( sComboAlias == _T("����") ) {
			sStaticLengthRange= _T("008@28");
		}
		else if ( sComboAlias == _T("ȸ��") ) {
			sStaticLengthRange= _T("008@29");
		}
		else if ( sComboAlias == _T("����") ) {
			sStaticLengthRange= _T("008@30");
		}
		else if ( sComboAlias == _T("����") ) {
			sStaticLengthRange= _T("008@31");
		}
		else if ( sComboAlias == _T("�������") ) {
			sStaticLengthRange= _T("008@32");
		}
		else if ( sComboAlias == _T("��������") ) {
			sStaticLengthRange= _T("008@33");
		}
		else if ( sComboAlias == _T("������") ) {
			sStaticLengthRange= _T("008@18-21");
		}
		else if ( sComboAlias == _T("����") ) {
			sStaticLengthRange= _T("008@22-23");
		}
		else if ( sComboAlias == _T("�����ڿ���") ) {
			sStaticLengthRange= _T("008@24");
		}
		else if ( sComboAlias == _T("�����ڷ�����") ) {
			sStaticLengthRange= _T("008@25");
		}
		else if ( sComboAlias == _T("���»���Ư¡1") ) {
			sStaticLengthRange= _T("008@33");
		}
		else if ( sComboAlias == _T("���»���Ư¡2") ) {
			sStaticLengthRange= _T("008@34");
		}

		else if ( sComboAlias == _T("���к�ȣ") ) {
			sStaticLengthRange= _T("008@35-37");
		}
		else if ( sComboAlias == _T("�ѱ����α����ȣ") ) {
			sStaticLengthRange= _T("008@38-39");
		}

		return sStaticLengthRange;

EFS_END
return _T("");
}

VOID CBO_CAT_PROC_0801::SetInsertStringToCombo(CString sFormCode)
{
EFS_BEGIN

	if ( sFormCode == _T("") ) return;	

	CComboBox *pComboPre = NULL;
	pComboPre = (CComboBox*)GetDlgItem(IDC_cFORM);

	INT iCount = pComboPre->GetCount();

	for ( INT i = iCount-1 ; i >= 0 ; i-- ) {
		pComboPre->DeleteString(i);		
	}

	if ( sFormCode == _T("BK") ) {
		const INT nComboData = 19;	
		CString strComboData[nComboData] = {
			_T("�Է�����"), _T("���������"),
			_T("�����1"), _T("�����2"),
			_T("���౹��ȣ"), _T("��ǥ��"),
			_T("�̿����"), _T("�ڷ�ĺ�"),
			_T("��������"), _T("�ѱ����к�ȣ"),
			_T("����"), _T("ȸ��"),
			_T("����"), _T("����"),
			_T("�������"), _T("��������"),
			_T("����"), _T("���к�ȣ"),
			_T("�ѱ����α����ȣ")
		};
		CComboBox *pCombo = NULL;
		
		pCombo = (CComboBox*)GetDlgItem(IDC_cFORM);
		if (pCombo == NULL) return;

		for (INT j = 0; j < nComboData; j++)
		{
			pCombo->InsertString(j, strComboData[j]);
		}
	
	}	
	else if ( sFormCode == _T("VM") ) {
		const INT nComboData = 17;
		CString strComboData[nComboData] = {
			_T("�Է�����"), _T("���������"),
			_T("�����1"), _T("�����2"),
			_T("���౹��ȣ"), _T("�󿵽ð�"),
			_T("�̿����"), _T("�����ڷ�1"), _T("�����ڷ�2"),
			_T("�����ڷ�3"),
			_T("�ѱ����к�ȣ"), _T("����"), 
			_T("�������"), _T("�ڷ�����"), _T("�������"),
			_T("���к�ȣ"),	_T("�ѱ����α����ȣ")
		};
		CComboBox *pCombo = NULL;
		
		pCombo = (CComboBox*)GetDlgItem(IDC_cFORM);
		if (pCombo == NULL) return;
		
		for (INT j = 0; j < nComboData; j++)
		{
			pCombo->InsertString(j, strComboData[j]);
		}

	}
	else if ( sFormCode == _T("MU") ) {
		const INT nComboData = 18;	
		CString strComboData[nComboData] = {
			_T("�Է�����"), _T("���������"),
			_T("�����1"), _T("�����2"),
			_T("���౹��ȣ"), _T("��������"),
			_T("�Ǻ�����"), _T("�̿����ڼ���"), _T("�ڷ�����"),
			_T("�����ڷ�1"), _T("�����ڷ�2"),
			_T("�ѱ����к�ȣ"), _T("����"),
			_T("�����ڷ��ǳ���1"), _T("�����ڷ��ǳ���2"), _T("�������"),
			_T("���к�ȣ"),	_T("�ѱ����α����ȣ")
		};
		CComboBox *pCombo = NULL;
		
		pCombo = (CComboBox*)GetDlgItem(IDC_cFORM);
		if (pCombo == NULL) return;

		for (INT j = 0; j < nComboData; j++)
		{
			pCombo->InsertString(j, strComboData[j]);
		}
	
	}
	else if ( sFormCode == _T("MP") ) {
		const INT nComboData = 17;	
		CString strComboData[nComboData] = {
			_T("�Է�����"), _T("���������"),
			_T("�����1"), _T("�����2"),
			_T("���౹��ȣ"), _T("������"),
			_T("����"), _T("�����ڿ���"), _T("�����ڷ�����"),			
			_T("�ѱ����к�ȣ"), _T("����"), _T("����"),
			_T("�������"), _T("���»���Ư¡1"), _T("���»���Ư¡2"),
			_T("���к�ȣ"),	_T("�ѱ����α����ȣ")
		};
		CComboBox *pCombo = NULL;
		
		pCombo = (CComboBox*)GetDlgItem(IDC_cFORM);
		if (pCombo == NULL) return;

		for (INT j = 0; j < nComboData; j++)
		{
			pCombo->InsertString(j, strComboData[j]);
		}

	}
	else if ( sFormCode == _T("CF") ) {
		const INT nComboData = 12;	
		CString strComboData[nComboData] = {
			_T("�Է�����"), _T("���������"),
			_T("�����1"), _T("�����2"),
			_T("���౹��ȣ"), _T("�̿����ڼ���"),
			_T("��ǻ����������"),
			_T("�ѱ����к�ȣ"), _T("����"),
			_T("�������"),
			_T("���к�ȣ"),	_T("�ѱ����α����ȣ")
		};
		CComboBox *pCombo = NULL;
		
		pCombo = (CComboBox*)GetDlgItem(IDC_cFORM);
		if (pCombo == NULL) return ;

		for (INT j = 0; j < nComboData; j++)
		{
			pCombo->InsertString(j, strComboData[j]);
		}
	
	}


EFS_END
return;
}

BOOL CBO_CAT_PROC_0801::ShowModifyList(CArray<CString, CString> &arrayModifyReckey)
{
EFS_BEGIN

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_PROC_0800_MODIFY"));
	if ( pDM == NULL ) return FALSE;

	CString strOption;
	CString sBookKeyList;
	CStringArray aBookKeyList;

	for ( INT i = 0 ; i < arrayModifyReckey.GetSize() ; i++ ) {
		sBookKeyList = sBookKeyList + _T(", ") + arrayModifyReckey.GetAt(i);

		if ( ((i+1) % 999 ) == 0 ) {
			aBookKeyList.Add( sBookKeyList );
			sBookKeyList = _T("");
		}			
	}

	aBookKeyList.Add( sBookKeyList );
	sBookKeyList = aBookKeyList.GetAt(0).Mid(2);
	
	strOption = _T(" I.REC_KEY = S.REC_KEY AND I.REC_KEY IN ( ") + sBookKeyList + _T(" )");
	
	INT ids = pDM->RefreshDataManager(strOption);
	if (ids < 0 )
	{
		AfxMessageBox(_T("RefreshDataManager �� Ȯ���� �ּ���"));
		return FALSE;
	}
	
	for ( i = 1 ; i < aBookKeyList.GetSize(); i++ ) {
		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
		sBookKeyList = aBookKeyList.GetAt(i).Mid(2);
		strOption = _T("SELECT I.AUTHOR, I.FORM_CODE, I.MEDIA_CODE, I.PUBLISHER, I.PUB_YEAR, I.REC_KEY, I.TITLE, S.ACQ_YEAR,S.DUP_FLAG,S.MARC,bo_get_max_reg_no(S.rec_key),bo_get_min_reg_no(S.rec_key) FROM IDX_BO_TBL I, BO_SPECIES_TBL S WHERE I.REC_KEY = S.REC_KEY AND I.MANAGE_CODE = UDF_MANAGE_CODE AND I.REC_KEY IN ( ") + sBookKeyList + _T(" )");
		dm.RestructDataManager(strOption);
		for ( INT j = 0 ; j < dm.GetRowCount() ; j++ ) {
			INT nModifyCount = pDM->GetRowCount();
			pDM->InsertRow(-1);
			
			CString sItem = dm.GetCellData( j, 0 );
			pDM->SetCellData( _T("IBS_������"), sItem, nModifyCount );
			sItem = dm.GetCellData( j, 1 );				
			pDM->SetCellData( _T("BS_���ı���_�ڵ�"), sItem, nModifyCount );
			sItem = dm.GetCellData( j, 2 );
			pDM->SetCellData( _T("BS_��ü����_�ڵ�"), sItem, nModifyCount );
			sItem = dm.GetCellData( j, 3 );
			pDM->SetCellData( _T("IBS_������"), sItem, nModifyCount );
			sItem = dm.GetCellData( j, 4 );
			pDM->SetCellData( _T("IBS_����⵵2"), sItem, nModifyCount );
			sItem = dm.GetCellData( j, 5 );
			pDM->SetCellData( _T("BS_��KEY"), sItem, nModifyCount );
			sItem = dm.GetCellData( j, 6 );
			pDM->SetCellData( _T("IBS_��ǥ��"), sItem, nModifyCount );
			sItem = dm.GetCellData( j, 7 );
			pDM->SetCellData( _T("BS_���Գ⵵"), sItem, nModifyCount );
			sItem = dm.GetCellData( j, 8 );
			pDM->SetCellData( _T("BS_��������"), sItem, nModifyCount );
			sItem = dm.GetCellData( j, 9 );
			pDM->SetCellData( _T("BS_MARC"), sItem, nModifyCount );
			sItem = dm.GetCellData( j, 10 );
			pDM->SetCellData( _T("����Ϲ�ȣ"), sItem, nModifyCount );
			sItem = dm.GetCellData( j, 11 );
			pDM->SetCellData( _T("���۵�Ϲ�ȣ"), sItem, nModifyCount );

			
		}
	}
		
	m_iPreModifyCount = pDM->GetRowCount();

	pDM->SORT( _T("���۵�Ϲ�ȣ"), FALSE );

	AllControlDisplay(_T("CM_BO_CAT_PROC_0800"));

	return TRUE;

EFS_END
return FALSE;
}

BOOL CBO_CAT_PROC_0801::ShowErrorList(CArray<CString, CString> &arrayErrorReckey)
{
EFS_BEGIN

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_PROC_0800_ERROR"));
	if ( pDM == NULL ) return FALSE;

	CString strOption;
	CString sBookKeyList;
	CStringArray aBookKeyList;

	
	for ( INT i = 0 ; i < arrayErrorReckey.GetSize() ; i++ ) {
		sBookKeyList = sBookKeyList + _T(", ") + arrayErrorReckey.GetAt(i);
		if ( ((i+1) % 999 ) == 0 ) {
			aBookKeyList.Add( sBookKeyList );
			sBookKeyList = _T("");
		}			
	}
	aBookKeyList.Add( sBookKeyList );
	sBookKeyList = aBookKeyList.GetAt(0).Mid(2);

	strOption = _T(" I.REC_KEY = S.REC_KEY AND I.REC_KEY IN ( ") + sBookKeyList + _T(" )");

	INT ids = pDM->RefreshDataManager(strOption);
	if (ids < 0 )
	{
		AfxMessageBox(_T("RefreshDataManager �� Ȯ���� �ּ���"));
		return FALSE;
	}		

	for ( i = 1 ; i < aBookKeyList.GetSize(); i++ ) {
		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
		sBookKeyList = aBookKeyList.GetAt(i).Mid(2);
		strOption = _T("SELECT I.AUTHOR, I.FORM_CODE, I.MEDIA_CODE, I.PUBLISHER, I.PUB_YEAR, I.REC_KEY, I.TITLE, S.ACQ_YEAR,S.DUP_FLAG,S.MARC,bo_get_max_reg_no(S.rec_key),bo_get_min_reg_no(S.rec_key) FROM IDX_BO_TBL I, BO_SPECIES_TBL S WHERE I.REC_KEY = S.REC_KEY AND I.MANAGE_CODE = UDF_MANAGE_CODE AND I.REC_KEY IN ( ") + sBookKeyList + _T(" )");
		dm.RestructDataManager(strOption);
		for ( INT j = 0 ; j < dm.GetRowCount() ; j++ ) {
			INT nErrorCount = pDM->GetRowCount();
			pDM->InsertRow(-1);
			
			CString sItem = dm.GetCellData( j, 0 );
			pDM->SetCellData( _T("IBS_������"), sItem, nErrorCount );
			sItem = dm.GetCellData( j, 1 );				
			pDM->SetCellData( _T("BS_���ı���_�ڵ�"), sItem, nErrorCount );
			sItem = dm.GetCellData( j, 2 );
			pDM->SetCellData( _T("BS_��ü����_�ڵ�"), sItem, nErrorCount );
			sItem = dm.GetCellData( j, 3 );
			pDM->SetCellData( _T("IBS_������"), sItem, nErrorCount );
			sItem = dm.GetCellData( j, 4 );
			pDM->SetCellData( _T("IBS_����⵵2"), sItem, nErrorCount );
			sItem = dm.GetCellData( j, 5 );
			pDM->SetCellData( _T("BS_��KEY"), sItem, nErrorCount );
			sItem = dm.GetCellData( j, 6 );
			pDM->SetCellData( _T("IBS_��ǥ��"), sItem, nErrorCount );
			sItem = dm.GetCellData( j, 7 );
			pDM->SetCellData( _T("BS_���Գ⵵"), sItem, nErrorCount );
			sItem = dm.GetCellData( j, 8 );
			pDM->SetCellData( _T("BS_��������"), sItem, nErrorCount );
			sItem = dm.GetCellData( j, 9 );
			pDM->SetCellData( _T("BS_MARC"), sItem, nErrorCount );
			sItem = dm.GetCellData( j, 10 );
			pDM->SetCellData( _T("����Ϲ�ȣ"), sItem, nErrorCount );
			sItem = dm.GetCellData( j, 11 );
			pDM->SetCellData( _T("���۵�Ϲ�ȣ"), sItem, nErrorCount );
			
		}
	}

	pDM->SORT( _T("���۵�Ϲ�ȣ"), FALSE );
	
	AllControlDisplay(_T("CM_BO_CAT_PROC_0800"));
	return TRUE;
	
EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0801::Update008ToMarcAndDB(CMarc *marc, CESL_DataMgr *pDM, CString s008StaticLength, CString strNewData, INT idx)
{
EFS_BEGIN

	m_pInfo->pMarcMgr->SetItem( marc, s008StaticLength, strNewData );
	m_marcEditor.Init(m_pInfo->pMarcMgr, marc);	

	INT ids = m_pInfo->pMarcMgr->CheckDeepSyntax(marc);
	if ( ids < 0 ) return FALSE;

	m_marcEditor.Display();

	CString strMarc;
	m_pInfo->pMarcMgr->Encoding(marc, strMarc);
	m_strStreamMarc = strMarc;
	m_pInfo->pMarcMgr->Decoding( strMarc, &m_marc );

	CString sRecKey = pDM->GetCellData( _T("BS_��KEY") , idx );
	INT iCh;	
	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return FALSE;

	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc, 1);
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("�������ϰ�����"),WORK_LOG), 1);
	INT idsTmp = pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( 0 > idsTmp ) return FALSE;

	iCh = pDM->SendFrame();
	m_iPostModifyCount++;

	if ( iCh == -3 || iCh == -1) {
		AfxMessageBox(_T("SendFrame Query Error!!"));
		return FALSE;
	}
	iCh = pDM->EndFrame();	
	if ( 0 > iCh ) return FALSE;

	pDM->SetCellData( _T("BS_MARC") , strMarc, idx );
	INT iMainIdx = m_aIdx.GetAt(idx);
	m_pDM_Main->SetCellData( _T("BS_MARC") , strMarc, iMainIdx );

	GetDlgItem(IDC_bAPPLY)->EnableWindow(false);



EFS_END
return FALSE;
}

BOOL CBO_CAT_PROC_0801::Insert008ToMarcAndDB(CMarc *marc, CESL_DataMgr *pDM, CString s008StaticLength, CString strData, INT idx)
{
EFS_BEGIN

	m_pInfo->pMarcMgr->SetItem( marc, s008StaticLength, strData );
	m_marcEditor.Init(m_pInfo->pMarcMgr, marc);	
	INT ids = m_pInfo->pMarcMgr->CheckDeepSyntax(marc);
	if ( ids < 0 ) return FALSE;
	m_marcEditor.Display();
		
	CString strMarc;

	m_pInfo->pMarcMgr->Encoding(marc, strMarc);
	CString sRecKey = pDM->GetCellData( _T("BS_��KEY") , idx );
	INT iCh;	
	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return FALSE;

	pDM->InitDBFieldData();				
	pDM->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc, 1);
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("�������ϰ�����"),WORK_LOG), 1);

	INT idsTmp = pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( 0 > idsTmp ) return FALSE;

	iCh = pDM->SendFrame();
	m_iPostModifyCount++;

	if ( iCh == -3 || iCh == -1) {
		AfxMessageBox(_T("SendFrame Query Error!!"));
		return -1;
	}
	iCh = pDM->EndFrame();	
	if ( 0 > iCh ) return FALSE;

	pDM->SetCellData( _T("BS_MARC") , strMarc, idx );
	INT iMainIdx = m_aIdx.GetAt(idx);
	m_pDM_Main->SetCellData( _T("BS_MARC") , strMarc, iMainIdx );

	return TRUE;

EFS_END
return FALSE;
}

BOOL CBO_CAT_PROC_0801::ExecuteDeleteTag(CMarc *marc, CESL_DataMgr *pDM, CString strTag, INT idx)
{
EFS_BEGIN

	if ( strTag == _T("049") || strTag == _T("300") || strTag == _T("653") || strTag == _T("008") || strTag == _T("260")) 
	{
		AfxMessageBox(_T("049, 300, 653, 008, 260 �� ���� �Ͻ� �� �����ϴ�."))	;
		return FALSE;
	}

	m_pInfo->pMarcMgr->DeleteField(marc, strTag);
	m_marcEditor.Init(m_pInfo->pMarcMgr, marc);	
	INT ids = m_pInfo->pMarcMgr->CheckDeepSyntax(marc);
	if ( ids < 0 ) return FALSE;
	
	m_pInfo->pMarcMgr->DeleteEmptySubfield(marc);
	m_pInfo->pMarcMgr->DeleteEmptyField(marc);

	m_marcEditor.Display();

	CString strMarc;

	m_pInfo->pMarcMgr->Encoding(marc, strMarc);
	CString sRecKey = pDM->GetCellData( _T("BS_��KEY") , idx );
	INT iCh;	
	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return -1;

	pDM->InitDBFieldData();				
	pDM->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc, 1);
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("�������ϰ�����"),WORK_LOG), 1);
	INT idsTmp = pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( 0 > idsTmp ) return idsTmp;

	iCh = pDM->SendFrame();
	m_iPostModifyCount++;

	if ( iCh == -3 || iCh == -1) {
					AfxMessageBox(_T("SendFrame Query Error!!"));
					return -1;
	}
	iCh = pDM->EndFrame();	
	if ( 0 > iCh ) return -1;

	pDM->SetCellData( _T("BS_MARC") , strMarc, idx );
	INT iMainIdx = m_aIdx.GetAt(idx);
	m_pDM_Main->SetCellData( _T("BS_MARC") , strMarc, iMainIdx );

	return TRUE;

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0801::ExecuteDeleteTagSubfield(CMarc *marc, CESL_DataMgr *pDM, CString strTag, CString strCode, INT idx)
{
EFS_BEGIN

	if ( strTag == _T("049") || strTag == _T("300") || strTag == _T("653") || strTag == _T("008") || strTag == _T("260")) 
	{
		AfxMessageBox(_T("049, 300, 653, 008, 260 �� ���� �Ͻ� �� �����ϴ�."))	;
		return FALSE;
	}

	CESL_DataMgr *pDmBook = FindDM(_T("DM_BO_CAT_PROC_0800_å"));
	if (pDmBook == NULL) return -1;

	m_pInfo->pMarcMgr->DeleteItem(marc, strTag + _T("$") + strCode);
	m_marcEditor.Init(m_pInfo->pMarcMgr, marc);	
	INT ids = m_pInfo->pMarcMgr->CheckDeepSyntax(marc);
	if ( ids < 0 ) return FALSE;

	m_pInfo->pMarcMgr->DeleteEmptySubfield(marc);
	m_pInfo->pMarcMgr->DeleteEmptyField(marc);
	m_marcEditor.Display();

	CString strMarc;
	m_pInfo->pMarcMgr->Encoding(marc, strMarc);
	
	CString sRecKey = pDM->GetCellData( _T("BS_��KEY") , idx );

	if ( strTag == _T("049") && strCode == _T("f") ) {
		CString strWhere = _T("species_key = '") + sRecKey + _T("'");
		if ( pDmBook->RefreshDataManager(strWhere) < 0 ) return -1;

		CString sLocCode;
		CArray<CString, CString&> pArray049;
		m_pInfo->pMarcMgr->GetField( marc, _T("049"), sLocCode, &pArray049 );
		CString sGt, sFt;
		sGt.Format( _T("%c"), 31 );
		sFt.Format( _T("%c"), 30 );
		CString sIndL, sIndF;
		sIndL.Format( _T("%cl"), 31 );
		sIndF.Format( _T("%cf"), 31 );

		INT iPosL = sLocCode.Find( sIndL );

		for ( INT i = 0 ; i < pDmBook->GetRowCount() ; i++ ) {
			pDmBook->SetCellData( _T("BB_��ġ��ȣ_�ڵ�"), _T(""), i );
			pDmBook->SetCellData( _T("BB_DB_FLAG"), _T("U"), i );
		}
	}
	else if ( strTag == _T("090") )
	{
		for ( INT i = 0 ; i < pDmBook->GetRowCount(); i++ )
		{
			pDmBook->SetCellData(_T("BB_������ȣ"), _T("") ,i);
			pDmBook->SetCellData(_T("BB_�з���ȣ"), _T("") ,i);
			pDmBook->SetCellData( _T("BB_DB_FLAG"), _T("U"), i );
		}
		
	}

	INT iCh;	
	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return -1;
	CString sQuery;

	pDM->InitDBFieldData();				
	pDM->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc, 1);
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("�������ϰ�����"),WORK_LOG), 1);

	INT idsTmp = pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( 0 > idsTmp ) return idsTmp;

	pDM->SetCellData( _T("BS_MARC") , strMarc, idx );
	INT iMainIdx = m_aIdx.GetAt(idx);
	m_pDM_Main->SetCellData( _T("BS_MARC") , strMarc, iMainIdx );

	CBO_SPECIES API(this);
	API.MakeBookQueryFrame(pDmBook, sRecKey, _T("0"), pDM );

	iCh = pDM->SendFrame();
	m_iPostModifyCount++;

	if ( iCh == -3 || iCh == -1) {
		AfxMessageBox(_T("SendFrame Query Error!!"));
		return -1;
	}
	iCh = pDM->EndFrame();	
	if ( 0 > iCh ) return -1;

	return 0;

EFS_END
return -1;
}

BOOL CBO_CAT_PROC_0801::ExecuteModifyTag(CMarc *marc, CESL_DataMgr *pDM, CString strTag, CString strInd, CString strNewData, INT idx)
{
EFS_BEGIN

	CString strField;
	m_pInfo->pMarcMgr->GetField( marc, strTag, strField);
	if ( strField.Left(2) != m_strInd ) return FALSE;

	CArray<CString, CString&> array;
	CString strDataFrom;
	CString strDataTo;
	m_pInfo->pMarcMgr->GetField(marc, strTag, strDataTo, &array);


	INT nCount = array.GetSize();
	for (INT i = 0; i < nCount; i++)
	{
		strDataFrom = array.GetAt(i);
		if (!strInd.IsEmpty())
		{
			if (strInd != strDataFrom.Mid(0, 2))
				continue;
			
			strDataTo = strDataFrom.Mid(2);
		}
		else
		{
			strDataTo = strDataFrom;
		}
		
		CArray<CString, CString&> arrayGetItem;
		CString sItem = strTag + _T("$") + strDataFrom.Mid(3,1);
		CString sData;
		m_pInfo->pMarcMgr->GetItem(marc, sItem, sData, &arrayGetItem);
		
		if ( arrayGetItem.GetSize() >= i ) {
			CString sGetItem = arrayGetItem.GetAt(i);
			m_pInfo->pMarcMgr->DeleteItem(marc, sItem, sData);
		}
		
		m_pInfo->pMarcMgr->InsertField(marc, strDataTo);
					
	}
	m_pInfo->pMarcMgr->DeleteEmptySubfield(marc);
	m_pInfo->pMarcMgr->DeleteEmptyField(marc);

	m_marcEditor.Init(m_pInfo->pMarcMgr, marc);	
	INT ids = m_pInfo->pMarcMgr->CheckDeepSyntax(marc);
	if ( ids < 0 ) return FALSE;
	m_pInfo->pMarcMgr->CheckSimpleSyntax(marc);
	m_marcEditor.Display();

	CString strMarc;
	m_pInfo->pMarcMgr->Encoding(marc, strMarc);
	CString sRecKey = pDM->GetCellData( _T("BS_��KEY") , idx );
	INT iCh;	
	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return -1;

	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc, 1);
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("�������ϰ�����"),WORK_LOG), 1);

	INT idsTmp = pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( 0 > idsTmp ) return -1;

	iCh = pDM->SendFrame();
	m_iPostModifyCount++;

	if ( iCh == -3 || iCh == -1) {
					AfxMessageBox(_T("SendFrame Query Error!!"));
					return -1;
	}
	iCh = pDM->EndFrame();	
	if ( 0 > iCh ) return -1;

	pDM->SetCellData( _T("BS_MARC") , strMarc, idx );
	INT iMainIdx = m_aIdx.GetAt(idx);
	m_pDM_Main->SetCellData( _T("BS_MARC") , strMarc, iMainIdx );

	return 0;

EFS_END
return -1;
}

BOOL CBO_CAT_PROC_0801::ExecuteModifyTagSubfield(CMarc *marc, 
												 CESL_DataMgr *pDM, 
												 CString strTag, CString strCode, CString strNewData, INT idx)
{
EFS_BEGIN

	CString strField;
	m_pInfo->pMarcMgr->GetField( marc, strTag, strField);
	if ( strField.Left(2) != m_strInd ) return FALSE;

	CString strDataTo;
	m_pInfo->pMarcMgr->GetItem(marc, strTag+_T("$")+strCode, strDataTo);

	m_pInfo->pMarcMgr->DeleteEmptySubfield( marc );
	m_pInfo->pMarcMgr->DeleteEmptyField( marc );

	m_marcEditor.Init(m_pInfo->pMarcMgr, marc);	
	INT ids = m_pInfo->pMarcMgr->CheckDeepSyntax(marc);
	if ( ids < 0 ) return FALSE;
	m_pInfo->pMarcMgr->CheckSimpleSyntax(marc);
	m_marcEditor.Display();
				
	CString strMarc;
	m_pInfo->pMarcMgr->Encoding(marc, strMarc);
	CString sRecKey = pDM->GetCellData( _T("BS_��KEY") , idx );
	INT iCh;	
	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return -1;

	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc, 1);
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("�������ϰ�����"),WORK_LOG), 1);

	INT idsTmp = pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( 0 > idsTmp ) return -1;

	iCh = pDM->SendFrame();
	m_iPostModifyCount++;

	if ( iCh == -3 || iCh == -1) {
					AfxMessageBox(_T("SendFrame Query Error!!"));
					return -1;
	}
	iCh = pDM->EndFrame();	
	if ( 0 > iCh ) return -1;

	pDM->SetCellData( _T("BS_MARC") , strMarc, idx );
	INT iMainIdx = m_aIdx.GetAt(idx);
	m_pDM_Main->SetCellData( _T("BS_MARC") , strMarc, iMainIdx );

	return 0;

EFS_END
return -1;
}

BOOL CBO_CAT_PROC_0801::ExecuteModifyTagSubfieldData(CMarc *marc, CESL_DataMgr *pDM, CString strTag, CString strCode, CString strData, CString strNewData, INT idx)
{
EFS_BEGIN

	CString strField;
	CString strVol;
	CString strTempVol;
	BOOL	bMultiVol;
	INT		i;

	m_pInfo->pMarcMgr->GetField( marc, strTag, strField);
	
	CString strDataTo;
	if( _T("020") ==strTag )
	{
		CArray<CString, CString&> array;
		CString strDataFrom;

		m_pInfo->pMarcMgr->GetField(marc, strTag, strDataTo, &array);

		INT nCount = 0;
		for ( i = 0; i < array.GetSize(); i++)
		{	
			strDataFrom = array.GetAt(i);
			if ( strDataFrom.Left(2) == m_strInd ) nCount++;
		}
		if ( nCount <= 0 ) return FALSE;
	}
	else
	{
		m_pInfo->pMarcMgr->GetItem(marc, strTag+_T("$")+strCode, strDataTo);
	}
	CESL_DataMgr *pDmBook = FindDM(_T("DM_BO_CAT_PROC_0800_å"));
	if (pDmBook == NULL) return FALSE;

	
	if ( (_T("049") == strTag)  )
	{
		if ( (_T("f") == strCode) && (_T("1 ") == m_strInd) )
		{
			CString str049Field = strField;
			str049Field.Replace( strCode + strData, strCode + strNewData );
			str049Field.Replace('$', SUBFIELD_CODE);

			str049Field = _T("049") + str049Field;

			m_pInfo->pMarcMgr->DeleteField(marc, strTag);
			m_pInfo->pMarcMgr->InsertField(marc, str049Field);

		}
		else if ( (_T("f") == strCode) && (_T("0 ") == m_strInd) )
		{
			CString strDataFrom = strDataTo;
			strDataFrom.Replace(strData, strNewData);

			m_pInfo->pMarcMgr->DeleteItem(marc, strTag+_T("$")+strCode);
			m_pInfo->pMarcMgr->SetItem(marc, strTag+_T("$")+strCode, strDataFrom );	
		}
		else 
		{
			AfxMessageBox(_T("049 TAG�� ��ġ��ȣ($f)�� ������ �� �ֽ��ϴ�."));
			return FALSE;
		}
		
	}
	else if( strTag == _T("020") )
	{	
		CString strDataFrom = strDataTo;
		CString strFieldDataFrom;
		CString strFieldDataTo;
		CArray<CString, CString&> astrFieldDataFrom;
		
		m_pInfo->pMarcMgr->GetField(marc,strTag, strFieldDataFrom,&astrFieldDataFrom);

		for( INT i = 0 ; i < astrFieldDataFrom.GetSize() ; ++i )
		{
			strFieldDataFrom = astrFieldDataFrom.GetAt( i );
			if ( strFieldDataFrom.Left(2) == m_strInd )
			{
				strFieldDataTo = astrFieldDataFrom.GetAt( i );
				strFieldDataTo.Replace(strData, strNewData);
				strFieldDataFrom = strTag + strFieldDataFrom;
				strFieldDataTo = strTag + strFieldDataTo;
				m_pInfo->pMarcMgr->UpdateField(marc,strFieldDataFrom, strFieldDataTo);
			}
		}
	}
	else 
	{
		if( strTag == _T("653") )
		{
			CString strDataFrom = strDataTo;
			CString strFieldDataFrom;
			CString strFieldDataTo;
			CArray<CString, CString&> astrFieldDataFrom;
			
			m_pInfo->pMarcMgr->GetField(marc,strTag, strFieldDataFrom,&astrFieldDataFrom);

			for( INT i = 0 ; i < astrFieldDataFrom.GetSize() ; ++i )
			{
				strFieldDataFrom = astrFieldDataFrom.GetAt( i );
				strFieldDataTo = astrFieldDataFrom.GetAt( i );
				strFieldDataTo.Replace(strData, strNewData);
				strFieldDataFrom = strTag + strFieldDataFrom;
				strFieldDataTo = strTag + strFieldDataTo;
				m_pInfo->pMarcMgr->UpdateField(marc,strFieldDataFrom, strFieldDataTo);
			}
		}
		else
		{
			CString strDataFrom = strDataTo;
			strDataFrom.Replace(strData, strNewData);

			m_pInfo->pMarcMgr->DeleteItem(marc, strTag+_T("$")+strCode);
			m_pInfo->pMarcMgr->SetItem(marc, strTag+_T("$")+strCode, strDataFrom );
		}
	}
	
	m_marcEditor.Init(m_pInfo->pMarcMgr, marc);	

	m_pInfo->pMarcMgr->DeleteEmptySubfield( marc );
	m_pInfo->pMarcMgr->DeleteEmptyField( marc );
	INT ids = m_pInfo->pMarcMgr->CheckDeepSyntax(marc);
	if ( ids < 0 ) return FALSE;
	m_pInfo->pMarcMgr->CheckSimpleSyntax(marc);
	m_marcEditor.Display();
						
	CString strMarc;
	m_pInfo->pMarcMgr->Encoding(marc, strMarc);
	CString sTmp;
	m_pInfo->pMarcMgr->GetItem(marc, strTag+_T("$")+strCode, sTmp );

	CString sRecKey = pDM->GetCellData( _T("BS_��KEY") , idx );

	CString strWhere = _T("species_key = '") + sRecKey + _T("'");
	if ( pDmBook->RefreshDataManager(strWhere) < 0 ) return FALSE;
	
	if ( strTag == _T("049") && strCode == _T("f") ) {
		
		CString sLocCode;
		CArray<CString, CString&> pArray049;
		pArray049.RemoveAll();
		m_pInfo->pMarcMgr->GetField( marc, _T("049"), sLocCode, &pArray049 );
		CString sGt, sFt;
		sGt.Format( _T("%c"), 31 );
		sFt.Format( _T("%c"), 30 );
		CString sIndL, sIndF;
		sIndL.Format( _T("%cl"), 31 );
		sIndF.Format( _T("%cf"), 31 );

		INT iPosL = sLocCode.Find( sIndL );

		if( strNewData.IsEmpty() )
		{
			CString strShelf;
			
			for ( INT i = 0 ; i < pDmBook->GetRowCount() ; i++ ) 
			{
				strShelf = pDmBook->GetCellData(_T("BB_��ġ��ȣ_�ڵ�"),i);
				if( strData == strShelf )
				{
					pDmBook->SetCellData( _T("BB_��ġ��ȣ_�ڵ�"), _T(""), i );
					pDmBook->SetCellData( _T("BB_DB_FLAG"), _T("U"), i );
				}			
			}
		}
		else
		{
			for ( INT i = 0 ; i < pDmBook->GetRowCount() ; i++ ) {
				pDmBook->SetCellData( _T("BB_��ġ��ȣ_�ڵ�"), _T(""), i );
			}
		}

		INT iPosF = sLocCode.Find( sIndF );

		CString sOriLocCode = sLocCode;

		while ( iPosF != -1 ) {
			iPosL = sLocCode.Find( sIndL );

			CString sRegNo;
			sRegNo = sLocCode.Mid( iPosL+2, 12 );

			while ( iPosL != -1 && iPosL < iPosF ) {
				sRegNo = sLocCode.Mid( iPosL+2, 12 );
				sLocCode = sLocCode.Mid( iPosL+1 );
 				iPosL = sLocCode.Find( sIndL );
			}
			iPosF = sLocCode.Find( sIndF );

			INT iPosGT = sLocCode.Find( sGt, iPosF+1 );
			if ( iPosGT == -1 ) {
				iPosGT = sLocCode.Find( sFt, iPosF+1 );
			}
			CString sLoc = sLocCode.Mid( iPosF+2, iPosGT-iPosF-2 );
			
			for ( INT i = 0 ; i < pDmBook->GetRowCount() ; i++ ) {
				CString sRegNoDm = pDmBook->GetCellData( _T("BB_��Ϲ�ȣ"), i );

				if ( (sRegNoDm == sRegNo) || (strTag == _T("049") && strCode == _T("f") && _T("0 ") == m_strInd) ) {				
					pDmBook->SetCellData( _T("BB_��ġ��ȣ_�ڵ�"), sLoc, i );
					pDmBook->SetCellData( _T("BB_DB_FLAG"), _T("U"), i );
				}

			}
			sLocCode = sLocCode.Mid( iPosF+1 );
			iPosF = sLocCode.Find( sIndF );
		}				
	}
	else if ( strTag == _T("090") )
	{
		CString sClass_No = _T("") ;
		CString sBook_Code = _T("") ;
		
		m_pInfo->pMarcMgr->GetItem(marc, _T("090$a"), sClass_No );
		m_pInfo->pMarcMgr->GetItem(marc, _T("090$b"), sBook_Code );

		for ( i = 0 ; i < pDmBook->GetRowCount(); i++ )
		{
			pDmBook->SetCellData(_T("BB_�з���ȣ"), sClass_No ,i);
			pDmBook->SetCellData(_T("BB_������ȣ"), sBook_Code ,i);
			pDmBook->SetCellData( _T("BB_DB_FLAG"), _T("U"), i );
		}

	}
	else if ( strTag == _T("950") )
	{		
		bMultiVol = FALSE;
		for ( i=0 ; i<pDmBook->GetRowCount() ; i++ )
		{
			pDmBook->GetCellData(_T("BB_�����"), i, strVol);
			if ( i>0 && strVol != strTempVol ) 
			{
				bMultiVol = TRUE;
				break;
			}
		}

		if ( bMultiVol == FALSE)
		{
			CString strMarcAlias, strItemData;
			m_mapMarcAlias.Lookup(_T("����"), strMarcAlias);
			m_pInfo->pMarcMgr->GetItem(marc, strMarcAlias, strItemData);
			CString strPrice;
			GetPriceCode(strItemData, strPrice);			
			for ( i=0 ; i<pDmBook->GetRowCount() ; i++ )
			{
				ids = pDmBook->SetCellData(_T("BB_����"), strPrice ,i);
				ids = pDmBook->SetCellData( _T("BB_DB_FLAG"), _T("U"), i );
			}	
			
		}
		

	}


	INT iCh;	
	iCh = pDM->StartFrame();			
	if ( 0 > iCh ) return FALSE;
	
	CString sQuery;

	pDM->InitDBFieldData();				
	pDM->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc, 1);
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("�������ϰ�����"),WORK_LOG), 1);
	INT idsTmp = pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( 0 > idsTmp ) return idsTmp;


	pDM->SetCellData( _T("BS_MARC") , strMarc, idx );
	
	INT iMainIdx = m_aIdx.GetAt(idx);
	m_pDM_Main->SetCellData( _T("BS_MARC") , strMarc, iMainIdx );

	if ( strTag == _T("090") )
	{
		CString strClassNo = _T("") ;
		CString strBookCode = _T("") ;
		
		m_pInfo->pMarcMgr->GetItem(marc, _T("090$a"), strClassNo );
		m_pInfo->pMarcMgr->GetItem(marc, _T("090$b"), strBookCode );

		pDM->InitDBFieldData();				
		pDM->AddDBFieldData(_T("CLASS_NO"), _T("STRING"), strClassNo, 1);
		pDM->AddDBFieldData(_T("BOOK_CODE"), _T("STRING"), strBookCode, 1);
		INT idsTmp = pDM->MakeUpdateFrame( _T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
		if ( 0 > idsTmp ) return idsTmp;
	}

	CBO_SPECIES API(this);
	API.MakeBookQueryFrame(pDmBook, sRecKey, _T("0"), pDM );

	iCh = pDM->SendFrame();
	m_iPostModifyCount++;

	if ( iCh == -3 || iCh == -1) {
		AfxMessageBox(_T("SendFrame Query Error!!"));
		return -1;
	}
	iCh = pDM->EndFrame();
	if ( 0 > iCh ) return FALSE;

EFS_END
return TRUE;
}

BOOL CBO_CAT_PROC_0801::ExecuteAddAll(CMarc *marc, CESL_DataMgr *pDM, CString strTag, CString strCode, CString strInd, CString strData, CString strNewData, INT idx)
{
	EFS_BEGIN
	
	if ( strTag == _T("049") && strCode == _T("f") )
	{
		CString sTmp049Check = _T("");
		m_pInfo->pMarcMgr->GetItem(marc, _T("049$f") , sTmp049Check);
		if ( !sTmp049Check.IsEmpty() ) return FALSE;
	}
	
	m_pInfo->pMarcMgr->SetItem(marc, strTag + _T("$") + strCode + _T("!") +  strInd, strData);		
	INT ids = m_pInfo->pMarcMgr->CheckDeepSyntax(marc);
	if ( ids < 0 ) return FALSE;
	m_marcEditor.Init(m_pInfo->pMarcMgr, marc);	
	m_marcEditor.Display();

	m_pInfo->pMarcMgr->CheckSimpleSyntax(marc);
		
	CString strMarc;
	m_pInfo->pMarcMgr->Encoding(marc, strMarc);
	CString sRecKey = pDM->GetCellData( _T("BS_��KEY") , idx );	
	
	if ( strTag == _T("049") && strCode == _T("f") ) {
		
		CString sLocCode;
		CArray<CString, CString&> pArray049;
		m_pInfo->pMarcMgr->GetField( marc, _T("049"), sLocCode, &pArray049 );
		
		CString s049Ind;
		s049Ind = sLocCode.Left(2);
		
		if ( s049Ind == _T("0 ") )
		{
			if ( !Insert049ValueToBook_Ind0(marc, sRecKey ) )  return FALSE;
		}
		else if ( s049Ind == _T("1 ") )
		{
			if ( !Insert049ValueToBook_Ind1( sRecKey, strData ) ) return FALSE;

			CESL_DataMgr *pDmBook = FindDM(_T("DM_BO_CAT_PROC_0800_å"));
			if (pDmBook == NULL) return -1;
			CCatApi_BO CommonCatApi(this);
			CommonCatApi.ConvertBookTo049(marc, pDmBook );

			m_pInfo->pMarcMgr->Encoding(marc, strMarc);
		}
		else
			return -1;
		
	}
	
	INT iCh;
	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return FALSE;
	
	CString sQuery;
	
	pDM->InitDBFieldData();				
	pDM->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc, 1);
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("�������ϰ�����"),WORK_LOG), 1);
	INT idsTmp = pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( 0 > idsTmp ) return idsTmp;
	
	pDM->SetCellData( _T("BS_MARC") , strMarc, idx );
	INT iMainIdx = m_aIdx.GetAt(idx);
	m_pDM_Main->SetCellData( _T("BS_MARC") , strMarc, iMainIdx );
	
	iCh = pDM->SendFrame();
	m_iPostModifyCount++;
	
	
	if ( iCh == -3 || iCh == -1) {
		AfxMessageBox(_T("SendFrame Query Error!!"));
		return FALSE;
	}
	iCh = pDM->EndFrame();
	if ( 0 > iCh ) return FALSE;
		
	EFS_END
	return TRUE;
}

BOOL CBO_CAT_PROC_0801::ShowFirstData()
{
	EFS_BEGIN
		
		Display(m_idx);
	
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_PROC_0800_MODIFY"));
	if (pDM == NULL) return FALSE;
	
	
	INT nRowCount = pDM->GetRowCount();
	if ( nRowCount < 1 ) {
		AfxMessageBox( _T("������ �ڷᰡ �����ϴ�.") );
		GetDlgItem(IDC_bAPPLY_ALL)->EnableWindow(false);
		GetDlgItem(IDC_bAPPLY)->EnableWindow(false);
		GetDlgItem(IDC_bCat0801PREV)->EnableWindow(false);
		GetDlgItem(IDC_bCat0801NEXT)->EnableWindow(false);
		return FALSE;
	}
	CString sRowCount;
	sRowCount.Format( _T("1 / %d") , nRowCount );
	SetControlData( _T("CM_BO_CAT_PROC_0800") , _T("�����Ȳ"), sRowCount );
	
	if ( nRowCount > 0 ) {
		GetDlgItem(IDC_bAPPLY_ALL)->EnableWindow(TRUE);
		GetDlgItem(IDC_bAPPLY)->EnableWindow(TRUE);
		GetDlgItem(IDC_bCat0801PREV)->EnableWindow(TRUE);
		GetDlgItem(IDC_bCat0801NEXT)->EnableWindow(TRUE);		
	}
	else {
		AfxMessageBox( _T("������ �ڷᰡ �����ϴ�.") );
		return FALSE;
	}
	
	CString sFormCode = pDM->GetCellData( _T("BS_���ı���_�ڵ�"), m_idx );
	SetControlData( _T("CM_BO_CAT_PROC_0800"), _T("���ı���_�ڵ�"), sFormCode );
	
	return TRUE;
	
	EFS_END
		return FALSE;
}

BOOL CBO_CAT_PROC_0801::Delete008FromMarcAndDB(CMarc *marc, CESL_DataMgr *pDM, CString s008StaticLength, CString sComboAlias, CString strData, INT idx)
{
EFS_BEGIN

	m_pInfo->pMarcMgr->SetItem( marc, sComboAlias, strData );
	m_marcEditor.Init(m_pInfo->pMarcMgr, marc);	
	INT ids = m_pInfo->pMarcMgr->CheckDeepSyntax(marc);
	if ( ids < 0 ) return FALSE;
	m_marcEditor.Display();
	
	CString strMarc;
	m_pInfo->pMarcMgr->Encoding(marc, strMarc);
	CString sRecKey = pDM->GetCellData( _T("BS_��KEY") , idx );
	INT iCh;	
	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return -1;

	pDM->InitDBFieldData();				
	pDM->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc, 1);
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("�������ϰ�����"),WORK_LOG), 1);
	INT idsTmp = pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( 0 > idsTmp ) return idsTmp;

	iCh = pDM->SendFrame();
	m_iPostModifyCount++;


	if ( iCh == -3 || iCh == -1) {
					AfxMessageBox(_T("SendFrame Query Error!!"));
					return -1;
	}
	iCh = pDM->EndFrame();
	if ( 0 > iCh ) return -1;

	pDM->SetCellData( _T("BS_MARC") , strMarc, idx );
	INT iMainIdx = m_aIdx.GetAt(idx);
	m_pDM_Main->SetCellData( _T("BS_MARC") , strMarc, iMainIdx );

EFS_END
return FALSE;
}

BOOL CBO_CAT_PROC_0801::CheckNoChangeTag(CString strTag, CString strCode)
{
EFS_BEGIN

	strTag.TrimLeft();
	strTag.TrimRight();
	
	if ( strTag == _T("049") && strCode == _T("l")) 	
	{
		AfxMessageBox(_T("049$l�� �ϰ� ���� �Ͻ� �� �����ϴ�."))	;
		return FALSE;
	}
	
	if ( strTag == _T("020") && m_strInd != _T("1 ") )
	{
		AfxMessageBox(_T("020 Tag�� ���ñ�ȣ 1_(Set ISBN) �ܿ��� �ϰ� ���� �Ͻ� �� �����ϴ�.\n �Ǻ��������� ������ �ּ���."))	;
		return FALSE;
	}

	return TRUE;
	
EFS_END
return FALSE;
}

CBO_CAT_PROC_0801::~CBO_CAT_PROC_0801()
{
EFS_BEGIN

	m_nModifyMode = NULL;
	m_nAddMode = NULL;
	m_nDeleteMode = NULL;
	m_marcEditor = NULL;
	m_strStreamMarc = _T("");
	m_bInitDlg = NULL;
	m_iPreModifyCount = 0;
	m_iPostModifyCount = 0;
	

EFS_END
}

BOOL CBO_CAT_PROC_0801::Insert049ValueToBook_Ind0(CMarc *pMarc, CString sSpeciesKey)
{
EFS_BEGIN
	INT ids;

	CESL_DataMgr *pDmBook = FindDM(_T("DM_BO_CAT_PROC_0800_å"));
	if (pDmBook == NULL) return -1;

	CString strWhere = _T("species_key = '") + sSpeciesKey + _T("'");
	if ( pDmBook->RefreshDataManager(strWhere) < 0 ) return FALSE;

	CString s049ShelfCode;
	ids = m_pInfo->pMarcMgr->GetItem(pMarc, _T("049$f") , s049ShelfCode);
	if ( ids < 0 ) return FALSE;

	for ( INT j = 0 ; j < pDmBook->GetRowCount() ; j++ ) {
		ids = pDmBook->SetCellData( _T("BB_��ġ��ȣ_�ڵ�"), _T(""), j );
		if ( ids < 0 ) return FALSE;
	}

	for ( INT i = 0 ; i < pDmBook->GetRowCount() ; i++ ) {
					
		CString sBookKey, sQuery;
		pDmBook->GetCellData( _T("BB_åKEY"), i, sBookKey );
		
		sQuery.Format(_T("UPDATE BO_BOOK_TBL SET SEPARATE_SHELF_CODE = '%s' WHERE rec_key = '%s';"), s049ShelfCode, sBookKey );
		ids = pDmBook->ExecuteQuery(sQuery, 1);
		if ( ids < 0 ) return FALSE;
	}

	return TRUE;

EFS_END
return FALSE;
}

BOOL CBO_CAT_PROC_0801::Insert049ValueToBook_Ind1(CString sSpeciesKey, CString sLocCode)
{
EFS_BEGIN	
	CString strSepShelfCode;
	CString sQuery;
	INT ids;

	CESL_DataMgr *pDmBook = FindDM(_T("DM_BO_CAT_PROC_0800_å"));
	if (pDmBook == NULL) return -1;

	CString strWhere = _T("species_key = '") + sSpeciesKey + _T("'");
	if ( pDmBook->RefreshDataManager(strWhere) < 0 ) return -1;

	for ( int i=0 ; i<pDmBook->GetRowCount() ; i++ )
	{
		pDmBook->GetCellData(_T("BB_��ġ��ȣ_�ڵ�"), i, strSepShelfCode);
		if ( strSepShelfCode == _T("") )
		{
			pDmBook->SetCellData(_T("BB_��ġ��ȣ_�ڵ�"), sLocCode, i );
		}
	}

	sQuery.Format(_T("UPDATE BO_BOOK_TBL SET SEPARATE_SHELF_CODE = '%s' WHERE SPECIES_KEY = '%s' AND SEPARATE_SHELF_CODE IS NULL;"), sLocCode, sSpeciesKey );
				ids = pDmBook->ExecuteQuery(sQuery, 1);
				if ( ids < 0 ) return FALSE;

	return TRUE;

EFS_END
return FALSE;
}

BOOL CBO_CAT_PROC_0801::InitModifyErrorList()
{
EFS_BEGIN

	CESL_DataMgr *pDmBookModify = FindDM(_T("DM_BO_CAT_PROC_0800_MODIFY"));
	CESL_DataMgr *pDmBookError = FindDM(_T("DM_BO_CAT_PROC_0800_ERROR"));
	pDmBookModify->FreeData();
	pDmBookError->FreeData();
	AllControlDisplay( _T("CM_BO_CAT_PROC_0800"));
	return TRUE;

EFS_END
return FALSE;
}

BOOL CBO_CAT_PROC_0801::ExecuteModifyTagInd( CMarc *marc, CESL_DataMgr *pDM, CString strTag, CString strInd,  INT idx )
{
EFS_BEGIN

	CString strField;
	m_pInfo->pMarcMgr->GetField( marc, strTag, strField);
	if ( strField.Left(2) != m_strInd ) return FALSE;

	CArray<CString, CString&> array;
	CString strDataFrom;
	CString strDataTo;
	m_pInfo->pMarcMgr->GetField(marc, strTag, strDataTo, &array);


	INT nCount = array.GetSize();
	for (INT i = 0; i < nCount; i++)
	{
		strDataFrom = array.GetAt(i);
		if (!strInd.IsEmpty())
		{			
			strDataTo = strTag + strInd + strDataFrom.Mid(2);
		}
		else
		{
			strDataTo = strDataFrom;
		}
		
		CArray<CString, CString&> arrayGetItem;
		CString sItem = strTag + _T("$") + strDataFrom.Mid(3,1);
		CString sData;
		m_pInfo->pMarcMgr->GetItem(marc, sItem, sData, &arrayGetItem);
		
		if ( arrayGetItem.GetSize() >= i ) {
			CString sGetItem = arrayGetItem.GetAt(i);
			m_pInfo->pMarcMgr->DeleteItem(marc, sItem, sData);
		}
		
		m_pInfo->pMarcMgr->InsertField(marc, strDataTo);
		
	}
	m_pInfo->pMarcMgr->DeleteEmptySubfield(marc);
	m_pInfo->pMarcMgr->DeleteEmptyField(marc);

	m_marcEditor.Init(m_pInfo->pMarcMgr, marc);	
	INT ids = m_pInfo->pMarcMgr->CheckDeepSyntax(marc);
	if ( ids < 0 ) return FALSE;
	m_marcEditor.Display();
	
	CString strMarc;
	m_pInfo->pMarcMgr->Encoding(marc, strMarc);
	CString sRecKey = pDM->GetCellData( _T("BS_��KEY") , idx );
	INT iCh;	
	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return -1;

	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc, 1);
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("�������ϰ�����"),WORK_LOG), 1);

	INT idsTmp = pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( 0 > idsTmp ) return -1;

	iCh = pDM->SendFrame();
	m_iPostModifyCount++;

	if ( iCh == -3 || iCh == -1) {
		AfxMessageBox(_T("SendFrame Query Error!!"));
		return -1;
	}
	iCh = pDM->EndFrame();	
	if ( 0 > iCh ) return -1;

	pDM->SetCellData( _T("BS_MARC") , strMarc, idx );
	INT iMainIdx = m_aIdx.GetAt(idx);
	m_pDM_Main->SetCellData( _T("BS_MARC") , strMarc, iMainIdx );

	return 0;

EFS_END
return -1;
}

INT CBO_CAT_PROC_0801::UpdateIndex(INT idx)
{
	if (idx < 0)
		return -1;

	INT ids;

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_PROC_0800_MODIFY"));
	if (pDM == NULL) return -1;

	CESL_DataMgr *pDM_Index = FindDM(_T("DM_����_����DM_��������"));
	if (pDM == NULL) return -1;

	if (idx >= pDM->GetRowCount())
		return -1;

	CString strMarc;
	CString strSpeciesKey;
	CString strWhere;
	CString strValue;
	ids = pDM->GetCellData(_T("BS_MARC"), idx, strMarc);
	if (ids < 0) return -1;
	ids = pDM->GetCellData(_T("BS_��KEY"), idx, strSpeciesKey);
	if (ids < 0) return -1;
	
	CMarc pMarc;
	m_pInfo->pMarcMgr->Decoding(strMarc, &pMarc);

	strWhere.Format(_T("REC_KEY = %s"), strSpeciesKey );
	pDM_Index->RefreshDataManager(strWhere);

	ids = -1;
	INT nVolCount = -1;

	CString strDMFieldAlias;
	CString strMarcAlias;
	CString strItemData;
	CArray<CString, CString&> arrItemData;
	
	POSITION pos = m_mapCatDMtoMarc.GetStartPosition();
	while (pos)
	{
		m_mapCatDMtoMarc.GetNextAssoc(pos, strDMFieldAlias, strMarcAlias);

		ids = m_pInfo->pMarcMgr->GetItem(&pMarc, strMarcAlias, strItemData);
		if (ids < 0) continue;

		if (strDMFieldAlias.Find(_T("IBS")) >= 0)
		{
			pDM_Index->SetCellData(strDMFieldAlias, strItemData, 0);
			if (strDMFieldAlias==_T("IBS_�����") )			
			{
				pDM->SetCellData( _T("IBS_����⵵2"), strItemData, idx);
			}
			else
			{
				pDM->SetCellData(strDMFieldAlias, strItemData, idx);
			}


		}
	}
	
	POSITION posTag = NULL;
	POSITION posSubfield = NULL;

	CString strTitle;
	CTag *pTag = NULL;
	CSubfield *pSubfield = NULL;
	CString strSubfieldCode;
	posTag = pMarc.m_listTag.GetHeadPosition();
	while (posTag)
	{
		pTag = (CTag*)pMarc.m_listTag.GetNext(posTag);
		if (pTag && pTag->GetTagCode() == _T("245"))
		{
			posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield)
			{
				pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (!pSubfield) continue;

				strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$a") ||
					strSubfieldCode == _T("$x") ||
					strSubfieldCode == _T("$b"))
				{
					strTitle += pSubfield->GetPunctuationMark() + pSubfield->GetItemData();
				}
			}
		}
	}

	pDM_Index->SetCellData(_T("IBS_��ǥ��"), strTitle, 0);
	pDM->SetCellData(_T("IBS_��ǥ��"), strTitle, idx);
	
	m_mapMarcAlias.Lookup(_T("����"), strMarcAlias);
	m_pInfo->pMarcMgr->GetItem(&pMarc, strMarcAlias, strItemData);
	{
		CString strAuthor = strItemData;
		arrItemData.RemoveAll();
		m_mapMarcAlias.Lookup(_T("������"), strMarcAlias);
		ids = m_pInfo->pMarcMgr->GetItem(&pMarc, strMarcAlias, strItemData, &arrItemData);
		if (ids >= 0)
		{
			INT nCount = arrItemData.GetSize();
			for (INT idx = 0; idx < nCount; idx++)
			{
				strItemData = arrItemData.GetAt(idx);
				if (strItemData.IsEmpty()) continue;

				strAuthor += _T(";") + strItemData;
			}
		}

		pDM_Index->SetCellData(_T("IBS_������"), strAuthor, 0);
		pDM->SetCellData(_T("IBS_������"), strAuthor, idx);
	}

	CString strPublisher;
	arrItemData.RemoveAll();

	m_mapMarcAlias.Lookup(_T("������"), strMarcAlias);
	m_pInfo->pMarcMgr->GetItem(&pMarc, strMarcAlias, strItemData, &arrItemData);
	{
		strPublisher = strItemData;
		INT nCount = arrItemData.GetSize();
		for (INT idx = 1; idx < nCount; idx++)
		{
			strItemData = arrItemData.GetAt(idx);
			if (strItemData.IsEmpty()) continue;

			strPublisher += _T(":") + strItemData;
		}

		pDM_Index->SetCellData(_T("IBS_������"), strPublisher, 0);		
	}

	if (m_pIndexTool == NULL)
		m_pIndexTool = new CIndexTool;
	m_pInfo->pMarcMgr->SetMarcGroup(_T("���ո�ũ"));
	m_pMarcMgr = m_pInfo->pMarcMgr;
	m_pIndexTool->Init(m_pMarcMgr, &pMarc);

	CString strIndexItem;
	CString strIndexDMFieldAlias;
	CString strIndexToolKitAlias;
	pos = m_mapIndexDMtoIndexToolKit.GetStartPosition();
	while (pos)
	{
		strIndexItem.Empty();
		m_mapIndexDMtoIndexToolKit.GetNextAssoc(pos, strIndexDMFieldAlias, strIndexToolKitAlias);
		
		ids = m_pIndexTool->GetIndex(strIndexToolKitAlias, strIndexItem);
		if (ids < 0) continue;

		pDM_Index->SetCellData(strIndexDMFieldAlias, strIndexItem, 0);
	}

	strItemData.Empty();
	ids = pDM_Index->GetCellData( _T("IBS_������"), 0, strItemData);
	if (strItemData.IsEmpty())
	{
		m_pInfo->pMarcMgr->GetItem(&pMarc, _T("245$d"), strItemData);
		DeleteRoleWord(strItemData);
		pDM_Index->SetCellData( _T("IBS_������"), strItemData, 0);

		if (strItemData.IsEmpty()) 
		{
			CString strFormCode;
			pDM_Index->GetCellData( _T("IBS_���ı���_�ڵ�"), 0, strFormCode);
			if (strFormCode == _T("BK")) 
			{
				m_pInfo->pMarcMgr->GetItem(&pMarc, _T("100$a"), strItemData);
				pDM_Index->SetCellData( _T("IBS_������"), strItemData, 0);
			} 
			else 
			{
				m_pInfo->pMarcMgr->GetItem(&pMarc, _T("700$a"), strItemData);
				pDM_Index->SetCellData( _T("IBS_������"), strItemData, 0);
			}
		}
	}

	strItemData.Empty();
	ids = pDM_Index->GetCellData( _T("IBS_��ǥ��"), 0, strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		pDM_Index->GetCellData( _T("IBS_�˻��뺻ǥ��"), 0, strItemData);
		pDM_Index->SetCellData( _T("IBS_��ǥ��"), strItemData, 0);
	}

	strItemData.Empty();
	ids = pDM_Index->GetCellData( _T("IBS_�˻�������˻�������"), 0, strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		m_pIndexTool->GetIndex(_T("������"), strItemData);
		pDM_Index->SetCellData( _T("IBS_�˻�������˻�������"), strItemData, 0);
	}

	strItemData.Empty();
	ids = pDM_Index->GetCellData( _T("IBS_������"), 0, strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		m_pIndexTool->GetIndex(_T("������"), strItemData);
		pDM_Index->SetCellData( _T("IBS_������"), strItemData, 0);
	}

	strItemData.Empty();
	ids = pDM_Index->GetCellData( _T("IBS_�����"), 0, strItemData);	
	if (ids < 0 || strItemData.IsEmpty())
	{
		m_pIndexTool->GetIndex(_T("�����˻������⵵"), strItemData);
		pDM_Index->SetCellData( _T("IBS_�����"), strItemData, 0);		
	}

	strItemData.Empty();
	m_pInfo->pMarcMgr->GetItem( &pMarc , _T("008@15-17"), strItemData );
	pDM_Index->SetCellData( _T("IBS_���౹�ڵ�"), strItemData, 0);

	CString strIdxTitle;
	CString strIdxAuthor;
	CString strIdxPublisher;
	CString strIdxKeyword;
	CString strIdxSubject;

	ids = pDM_Index->GetCellData( _T("IBS_�˻��뺻ǥ��"), 0, strIdxTitle);
	ids = pDM_Index->GetCellData( _T("IBS_�˻���������"), 0, strIdxAuthor);
	ids = pDM_Index->GetCellData( _T("IBS_�˻��������"), 0, strIdxPublisher);
	ids = pDM_Index->GetCellData( _T("IBS_�˻���Ű����"), 0, strIdxKeyword);	
	
	ids = pDM_Index->GetCellData( _T("IBS_����������"), 0, strIdxSubject);	

	CString strIdxAllItem = strIdxTitle + _T(" ") + strIdxAuthor + _T(" ") + strIdxPublisher + _T(" ") + strIdxKeyword + _T(" ") +strIdxSubject;
	CStringArray arrStrTarget;
	MakeIndexArray(&arrStrTarget, strIdxAllItem,_T(" "));
	
	INT nCount = arrStrTarget.GetSize();

	strIdxAllItem.Empty();
	for (idx = 0 ; idx < nCount ; idx++)
	{
		strIndexItem = arrStrTarget.GetAt(idx);

		if(strIdxAllItem.IsEmpty()) 
			strIdxAllItem = strIndexItem;
		else 
			strIdxAllItem += _T(" ") + strIndexItem;
	}
	pDM_Index->SetCellData( _T("IBS_��ü�˻�����ξ�"), strIdxAllItem, 0);	

	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add( _T("IBS_REC_KEY"));
	
	pDM->StartFrame();
	pDM->InitDBFieldData();	
	pDM->MakeQueryByDM(RemoveAliasList, pDM_Index, pDM_Index->TBL_NAME, 0, pDM, _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);	
	
	CString strTempData;
	strTempData.Format(_T("GET_BO_BOOK_INFO( %s, 'SEPARATE_SHELF_CODE' )"), strSpeciesKey);
	pDM->AddDBFieldData(_T("SE_SHELF_CODE"), _T("NUMERIC"), strTempData );
	ids = pDM->MakeUpdateFrame(_T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);					

	CString strQuery;
	CString strRecKey;
	ids = pDM->MakeRecKey(strRecKey);
	if (ids < 0) return ids;
	
	pDM->SendFrame(TRUE);

	pDM->EndFrame();

	return 0;
}

INT CBO_CAT_PROC_0801::InitMemberData()
{	
	EFS_BEGIN

	m_mapMarcAlias.RemoveAll();
	m_mapCatDMtoMarc.RemoveAll();
	m_mapIndexDMtoIndexToolKit.RemoveAll();

	m_mapMarcAlias.SetAt(_T("�����"), _T("245$n"));
	m_mapMarcAlias.SetAt(_T("����"), _T("245$p"));
	m_mapMarcAlias.SetAt(_T("����"), _T("245$a"));
	m_mapMarcAlias.SetAt(_T("�μ���"), _T("245$b"));
	m_mapMarcAlias.SetAt(_T("����"), _T("245$x"));
	m_mapMarcAlias.SetAt(_T("����"), _T("245$d"));
	m_mapMarcAlias.SetAt(_T("������"), _T("245$e"));
	m_mapMarcAlias.SetAt(_T("������"), _T("260$b"));
	m_mapMarcAlias.SetAt(_T("�Ѽ�ǥ��"), _T("440$a"));
	m_mapMarcAlias.SetAt(_T("�Ѽ�����"), _T("490$a"));
	m_mapMarcAlias.SetAt(_T("����"), _T("950$b"));
	m_mapMarcAlias.SetAt(_T("����������"), _T("950$c"));
	m_mapMarcAlias.SetAt(_T("�����"), _T("300$a"));
	m_mapMarcAlias.SetAt(_T("������Ư��"), _T("300$b"));
	m_mapMarcAlias.SetAt(_T("ũ��"), _T("300$c"));
	m_mapMarcAlias.SetAt(_T("�����ڷ�"), _T("300$e"));


	m_mapCatDMtoMarc.SetAt(_T("IBS_�ǻ���"), _T("250$a"));
	m_mapCatDMtoMarc.SetAt(_T("IBS_������"), _T("260$a"));
	m_mapCatDMtoMarc.SetAt(_T("IBS_������"), _T("260$b"));
	m_mapCatDMtoMarc.SetAt(_T("IBS_�����"), _T("260$c"));

	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_�˻���������"), _T("������"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_�����˻������⵵"), _T("�����˻������⵵"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_�˻�������˻�������"), _T("�����˻��������"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_�˻������Ŀ뺻ǥ��"), _T("�����˻��뺻ǥ��"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_�˻���Ű����"), _T("Ű��������"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_�˻��������"), _T("������"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_�˻��뺻ǥ��"), _T("��ǥ��"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_ǥ������"), _T("ǥ������"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_�����ڻ���"), _T("�����ڻ���"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_�����ڻ���"), _T("�����ڻ���"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_����⵵����"), _T("����⵵����"));


	return 0;

	EFS_END
	return -1;
}

INT CBO_CAT_PROC_0801::DeleteRoleWord(CString &strSrc)
{
	EFS_BEGIN
	
	if (pCharSetMgr_RoleWord == NULL)
	{
		pCharSetMgr_RoleWord = new CESL_CharSetMgr;
		pCharSetMgr_RoleWord->InitData(_T("���Ҿ�"), _T("���Ҿ�"));
	}

	INT nFind = strSrc.ReverseFind(' ');
	if (nFind > 0)
	{
		CString str1 = strSrc.Mid(nFind);
		CString str2 = str1;
		str1.TrimLeft();
		str1.TrimRight();
		
		if (pCharSetMgr_RoleWord->Find(str1, str2) == 0)
		{
			strSrc.Delete(nFind, strSrc.GetLength() - nFind);
		}
	}

	return 0;

	EFS_END
	return -1;
}


INT CBO_CAT_PROC_0801::MakeIndexArray(CStringArray *pArrStr, 
							LPCTSTR strSrcIndexField, 
							LPCTSTR lpszDelimit /*= _T(" ")*/)
{
	EFS_BEGIN

	CIndex *pIndex = m_pIndexTool->GetIndex();
	CString strIndexItem;	
	
	LPTSTR lpszToken = _tcstok(const_cast<LPTSTR>(strSrcIndexField), lpszDelimit);
	while (lpszToken)
	{
		strIndexItem.Format(_T("%s"), lpszToken);
		pIndex->MakeIndex(strIndexItem);
		if (!IsStringInArray(pArrStr, strIndexItem))
		{
			pArrStr->Add(strIndexItem);
		}

		lpszToken = _tcstok(NULL, lpszDelimit);
	}

	return 0;

	EFS_END
	return -1;
}

BOOL CBO_CAT_PROC_0801::IsStringInArray(CStringArray *pArr, 
							  CString str)
{
	EFS_BEGIN

	if (pArr == NULL || str.IsEmpty()) return FALSE;

	INT nRowCount = pArr->GetSize();
	if (nRowCount == 0) return FALSE;

	for (INT i = 0; i < nRowCount; i++)
	{
		if (pArr->GetAt(i) == str) return TRUE;
	}

	return FALSE;

	EFS_END
	return FALSE;
}

CString CBO_CAT_PROC_0801::GetPriceCode(CString strPrice, 
							  CString &strRealPrice)
{
	EFS_BEGIN

	CString strResult;

	INT nStartPrice = -1;

	TCHAR ch = 0;
	INT nLength = strPrice.GetLength();
	for (INT idx = 0; idx < nLength; idx++)
	{
		ch = strPrice.GetAt(idx);
		if (_istdigit(ch))
		{
			nStartPrice = idx;
			break;
		}
		
		strResult += ch;
	}

	if (nStartPrice >= 0)
		strRealPrice = strPrice.Mid(nStartPrice);

	return strResult;

	return _T("");

	EFS_END
	return _T("");
}
HBRUSH CBO_CAT_PROC_0801::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

void CBO_CAT_PROC_0801::OnKillfocuseTAGNO() 
{
	CString strTagNo;
	CEdit* pEditTagNo = (CEdit*) GetDlgItem(IDC_eTAGNO);
	pEditTagNo->GetWindowText( strTagNo );
	CButton * pBtnUpdate = (CButton *)GetDlgItem(IDC_rUPDATE_MODE);
	CButton * pBtnInsert = (CButton *)GetDlgItem(IDC_rINSERT_MODE);
	CButton * pBtnDelete = (CButton *)GetDlgItem(IDC_rDELETE_MODE);
	if( strTagNo == _T("001") || strTagNo == _T("049") || strTagNo == _T("008") || strTagNo == _T("020") )
	{		
		if( 1 == pBtnDelete->GetCheck() )
		{
			AfxMessageBox(_T("(001, 049, 008, 020)TAG�� ���� �Ҽ� �����ϴ�."));
			pEditTagNo->SetWindowText(_T(""));
		}
	}
}

BOOL CBO_CAT_PROC_0801::CheckSeprateShelfCode()
{
	// ������ ��������
	CString strTag, strCode, strData, strNewData;

	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("TAG"), strTag);
	if (m_nClass == GENERIC)
	{
		GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("�ĺ���ȣ"), strCode);
	}
	else
	{
		return TRUE;
	}

	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("������DATA"), strData);
	
	if (strTag.IsEmpty()) return TRUE;
	
	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("������DATA"), strNewData);

	CString strCheckQuery;
	CString strValue;	
	if ( strTag == _T("049") && (strCode == _T("f") || strCode == _T("$f") ))
	{
		if ( !strData.IsEmpty() )
		{
			strCheckQuery.Format(_T("SELECT COUNT(1) FROM CD_CODE_TBL WHERE CLASS='11' AND CODE='%s'"), strData );
			m_pDM_Main->GetOneValueQuery(strCheckQuery, strValue);
			if ( _ttoi(strValue.GetBuffer(0)) <= 0 ) 
			{
				AfxMessageBox(_T("�������� �ʴ� ��ġ��ȣ�� �Է��Ͽ����ϴ�."));
				return FALSE;
			}
		}		
		if ( !strNewData.IsEmpty() )
		{
			strCheckQuery.Format(_T("SELECT COUNT(1) FROM CD_CODE_TBL WHERE CLASS='11' AND CODE='%s'"), strNewData );
			m_pDM_Main->GetOneValueQuery(strCheckQuery, strValue);
			if ( _ttoi(strValue.GetBuffer(0)) <= 0 ) 
			{
				AfxMessageBox(_T("�������� �ʴ� ��ġ��ȣ�� �Է��Ͽ����ϴ�."));
				return FALSE;
			}
		}		
	}	

	return TRUE;
}

void CBO_CAT_PROC_0801::OnKillfocuseNEWDATA() 
{
	CheckSeprateShelfCode();		
}

void CBO_CAT_PROC_0801::OnKillfocuseOLDDATA() 
{
	CheckSeprateShelfCode();		
}
