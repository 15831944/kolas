// SMS_LifeLong_Search.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_LifeLong_Search.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMS_LifeLong_Search dialog


CSMS_LifeLong_Search::CSMS_LifeLong_Search(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMS_LifeLong_Search)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sSMSType	   = _T("");
	m_nDisplayMode = -1;
	m_pCSMS_LifeLong = (CSMS_LifeLong*)pParent;
}


VOID CSMS_LifeLong_Search::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMS_LifeLong_Search)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMS_LifeLong_Search, CDialog)
	//{{AFX_MSG_MAP(CSMS_LifeLong_Search)
	ON_CBN_SELCHANGE(IDC_cSMS, OnSelchangecSMS)
	ON_CBN_SELCHANGE(IDC_cUSER_GROUP, OnSelchangecUSERGROUP)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_LifeLong_Search message handlers

BOOL CSMS_LifeLong_Search::Create(CWnd* pParentWnd)
{
EFS_BEGIN	
	return CDialog::Create(IDD, pParentWnd);
EFS_END
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// �ʱ�ȭ �۾�
///////////////////////////////////////////////////////////////////////////
BOOL CSMS_LifeLong_Search::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids;
	CString sErrMsg = _T("");

	//////////////////////////////////////////////////////////////////
	// 0. ������ �ڵ� ���߱� 
	//////////////////////////////////////////////////////////////////
	FILE* fp = _tfopen(_T("..\\Cfg\\_debug"),_T("rb"));
	
	if (fp == NULL)
	{
		CRect rc;
		this->GetClientRect(&rc);
		this->MoveWindow( rc.left, rc.top, rc.Width() - 305, rc.Height() + 23);
	}
	else
		fclose(fp);

	///////////////////////////////////////////////////////////////////////
	// 1. SM �ʱ�ȭ
	///////////////////////////////////////////////////////////////////////
	if( InitESL_Mgr( _T("SMS_DLG_LifeLong_Search") ) < 0 )
	{
		sErrMsg.Format( _T("InitESL_Mgr Error : SMS_DLG_LifeLong_Search") );
		AfxMessageBox( sErrMsg );
		return false;
	}
	
	///////////////////////////////////////////////////////////////////////
	// 2. SMS ���� �޺��ڽ� �ʱ�ȭ
	///////////////////////////////////////////////////////////////////////
	ids = InitSMSGroupCombo();
	if (ids < 0)	return FALSE;
	
	
	///////////////////////////////////////////////////////////////////////
	// 3. ���� ���� �޺��ڽ� �ʱ�ȭ
	///////////////////////////////////////////////////////////////////////
	ids = InitCategoryCombo();
	if (ids < 0)	return FALSE;
	

	///////////////////////////////////////////////////////////////////////
	// 4. �̿��� �׷� �޺��ڽ� �ʱ�ȭ
	///////////////////////////////////////////////////////////////////////
	ids = InitUserGroupCombo();
	if (ids < 0)	return FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// SMS���� �޺��ڽ��� �ʱ�ȭ �Ѵ�.
////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Search::InitSMSGroupCombo()
{
EFS_BEGIN

	INT ids;
	/////////////////////////////////////////////////////////////////////////
	// 1. SMS �⺻���� �ʱ�ȭ
	/////////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_SMSGroup") );
	if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Search::InitSMSGroupCombo") );
	pDM->RefreshDataManager( _T("") );
	
	/////////////////////////////////////////////////////////////////////////
	// 2. SMS���� �޺��ڽ��� �Է��Ѵ�.
	/////////////////////////////////////////////////////////////////////////
	INT nCnt = pDM->GetRowCount();
	if ( nCnt < 1 )
	{
		// �����Ҽ� �ִ� ������ ����.
		return 3;
	}
	
	CString sGetSMSGroup = _T("");
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_cSMS);
	pCB->AddString( _T("") );

	for (INT i = 0;i < nCnt;i++)
	{
		ids = pDM->GetCellData( _T("SMS��"), i, sGetSMSGroup);
		if (ids < 0) return -2;
		sGetSMSGroup.TrimLeft();sGetSMSGroup.TrimRight();
		pCB->AddString( sGetSMSGroup );
	}

	return 0;
EFS_END
	return -1;
}

////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// ���±��� �޺��ڽ� �ʱ�ȭ
////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Search::InitCategoryCombo()
{
EFS_BEGIN

	INT ids;
	/////////////////////////////////////////////////////////////////////////
	// 1. ���±��� �⺻���� �ʱ�ȭ
	/////////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_PGCategory") );
	if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Search::InitCategoryCombo") );
	pDM->RefreshDataManager( _T("") );

	/////////////////////////////////////////////////////////////////////////
	// 2. ���±��� �޺��ڽ��� �Է��Ѵ�.
	/////////////////////////////////////////////////////////////////////////
	INT nCnt = pDM->GetRowCount();
	if ( nCnt < 1 )
	{
		// �����Ҽ� �ִ� ���±����� ����.
		return 3;
	}
	
	CString sGetSMSGroup = _T("");
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_cPROGRAM_CODE);
	pCB->AddString( _T("") );

	for (INT i = 0;i < nCnt;i++)
	{
		ids = pDM->GetCellData( _T("���±��и�"), i, sGetSMSGroup);
		if (ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Search::InitCategoryCombo") );
		sGetSMSGroup.TrimLeft();sGetSMSGroup.TrimRight();
		pCB->AddString( sGetSMSGroup );
	}

	return 0;
EFS_END
	return -1;
}

////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// �̿��ڱ׷� �޺��ڽ� �ʱ�ȭ
////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Search::InitUserGroupCombo()
{
EFS_BEGIN

	INT ids;
	/////////////////////////////////////////////////////////////////////////
	// 1. �̿��ڱ׷� �⺻���� �ʱ�ȭ
	/////////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_UserGroup") );
	if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Search::InitUserGroupCombo") );
	pDM->RefreshDataManager( _T("") );

	///////////////////////////////////////////////////////////////
	// 2. �׷��޺��ڽ��� ��Ƽ��Ÿ�Ϸ� �����Ѵ�.
	///////////////////////////////////////////////////////////////
	CESL_ControlMgr* pCM = FindCM(_T("CM_SMS_LifeLong_Search"));
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Search::InitUserGroupCombo") );

	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("ȸ���׷�")));
	if( pCMCB == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CSMS_LifeLong_Search::InitUserGroupCombo") );

	///////////////////////////////////////////////////////////////
	// 3. �׷��޺��ڽ� ������ �����Ѵ�.
	///////////////////////////////////////////////////////////////
	CString sInsertCode[2];

	pCMCB->InitMultiComboBox(2);
		
	sInsertCode[0] = _T("   ");
	sInsertCode[1] = _T("�������");
	ids = pCMCB->SetData( sInsertCode , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CSMS_LifeLong_Search::InitUserGroupCombo") );

	INT cnt = pDM->GetRowCount();
	if (cnt != 0)
	{
		for (INT i = 0;i < cnt;i++)
		{
			CString sTemp =_T("");

			pDM->GetCellData(_T("�׷��ڵ�"), i, sTemp);
			sInsertCode[0] = sTemp;
			pDM->GetCellData(_T("�׷��̸�"), i, sTemp);
			sInsertCode[1] = sTemp;
			ids = pCMCB->SetData( sInsertCode , 2 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CSMS_LifeLong_Search::InitUserGroupCombo") );
		}	
	}
	
	pCMCB->SetCurSel(0);

	return 0;
EFS_END
	return -1;
}

////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// ��¥ ������ �ڽ��� ��¥�� ��ȯ�����ش�.
// (I N) ��¥ ������ �ڽ� ���ҽ� ID
// (OUT) ��¥ 'YYYY/MM/DD'
////////////////////////////////////////////////////////////////////////////
CString CSMS_LifeLong_Search::GetCtrlDate(UINT id)
{
	CDateTimeCtrl *pDate = ( CDateTimeCtrl * )GetDlgItem( id );
	if(!pDate->IsWindowEnabled()) return _T("");
	CTime time;
	if( GDT_VALID != pDate->GetTime( time ) )
		return _T("");

	CString date;
	date.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );
	return date;
}

////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// SMS ���� �޺��ڽ� ���� �̺�Ʈ
////////////////////////////////////////////////////////////////////////////
VOID CSMS_LifeLong_Search::OnSelchangecSMS() 
{
	///////////////////////////////////////////////////////////////////////
	// 1. ���õ� �޺� �ڽ��� SMS �̸��� ���Ѵ�.
	///////////////////////////////////////////////////////////////////////
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_cSMS);
	INT nIndex = pCB->GetCurSel();
	CString sGetValue = _T("");
	pCB->GetLBText(nIndex, sGetValue);

	///////////////////////////////////////////////////////////////////////
	// 2. �ش� SMS�� Ű�� �����Ѵ�.
	///////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_SMSGroup") );
	if (pDM == NULL) return;
	pDM->RefreshDataManager( _T("") );

	INT nCnt = pDM->GetRowCount();
	CString sGetSMSGroup = _T("");

	for (INT i = 0;i < nCnt;i++)
	{
		pDM->GetCellData( _T("SMS��"), i, sGetSMSGroup);
		if ( _tcscmp(sGetSMSGroup, sGetValue) == 0 )
		{
			pDM->GetCellData( _T("Ÿ��"), i, m_sSMSType);
			break;
		}
	}

	///////////////////////////////////////////////////////////////////////
	// 3. ������ ����
	///////////////////////////////////////////////////////////////////////
	SetDlgItemText(IDC_eTest6, m_sSMSType);
}

////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// m_nDisplayMode�� ���� �Է��� �� �ִ� �׸��� Ʋ����.
////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Search::DisplayCtrlMode()
{
	return 0;	
}

VOID CSMS_LifeLong_Search::OnOK() 
{
	INT ids;
	CString sErrMsg = _T("");

	///////////////////////////////////////////////////////////////////////
	// 1. �Է� üũ
	///////////////////////////////////////////////////////////////////////
	if ( m_sSMSType.IsEmpty() )
	{
		sErrMsg.Format(_T("SMS������ �������� �ʾҽ��ϴ�."));
		AfxMessageBox(sErrMsg);
		return;
	}

	m_pCSMS_LifeLong->m_sSMSType = m_sSMSType;

	///////////////////////////////////////////////////////////////////////
	// 2. ������
	///////////////////////////////////////////////////////////////////////
	ids = SetCurMode();
	if (ids < 0)
	{
		sErrMsg.Format( _T(" ERROR Position :: SetCurMode\r\n ERROR CODE :: %d "), ids);
		AfxMessageBox(sErrMsg);
		return;
	}
	// 2.1) �θ��� ��带 SET�Ѵ�.
	m_pCSMS_LifeLong->SetDisplayMode(m_nDisplayMode);

	///////////////////////////////////////////////////////////////////////
	// 3. ��忡 ���� ���� ����
	///////////////////////////////////////////////////////////////////////
	ids = MakeQuery();
	if (ids < 0)
	{
		sErrMsg.Format( _T(" ERROR Position :: MakeQuery\r\n ERROR CODE :: %d "), ids);
		AfxMessageBox(sErrMsg);
		return;
	}

//	CDialog::OnOK();
}

////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// ������ ������ ������ �����Ѵ�.
////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Search::MakeQuery()
{
	CString str = _T("");

	///////////////////////////////////////////////////////////////////////
	// 1. ������ ����
	///////////////////////////////////////////////////////////////////////
	str.Format(_T("%d"), m_nDisplayMode);
	SetDlgItemText(IDC_eTest4, str);
	str = _T("");

	///////////////////////////////////////////////////////////////////////
	// 2. ��忡 ���� �ٸ���.
	///////////////////////////////////////////////////////////////////////
	if (m_nDisplayMode == 0)
	{
		MakeQuery_Program(&str);
		MakeQuery_Member(&str);
	}
	else if (m_nDisplayMode == 1)
		MakeQuery_NonMember(&str);
	else if (m_nDisplayMode == 2)
		MakeQuery_Member(&str);
	
	m_pCSMS_LifeLong->SearchUSER(str);

	///////////////////////////////////////////////////////////////////////
	// 3. ������ ����
	///////////////////////////////////////////////////////////////////////
	GetUserGroup(&str);
	SetDlgItemText(IDC_eTest5, str);

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// ȸ���� ������ �����.
////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Search::MakeQuery_Member(CString *psQuery)
{
	EFS_BEGIN

	INT ids;
	CString sTemp = _T("");
	
	///////////////////////////////////////////////////////////////
	// 1. ��Ʈ�� �Է°��� �����Ѵ�.
	///////////////////////////////////////////////////////////////	
	const INT nCount = 9;
 
	INT nCtrlID[nCount] = { 	IDC_cUSER_SEX	   , IDC_eUSER_ID		, 
								IDC_eUSER_NAME1	   , IDC_eUSER_NAME2	,
								IDC_eUSER_AGE1	   , IDC_eUSER_AGE2	    ,
								IDC_eUSER_ADDRESS  , IDC_eUSER_PHONE	,   // ��ȭ
								IDC_eUSER_PHONE2   };		
 		
	CString sCtrlData[nCount];

	for (INT i = 0;i < nCount; i++)
	{
		GetDlgItemText(nCtrlID[i] , sCtrlData[i]);
		sCtrlData[i].TrimLeft();sCtrlData[i].TrimRight();
	}

	///////////////////////////////////////////////////////////////
	// 2. ȸ�� ���� ������ �Է��Ѵ�.
	///////////////////////////////////////////////////////////////
	if ( !sCtrlData[0].IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		if ( sCtrlData[0].Compare(_T("����")) == 0 )
			sTemp = _T(" M.MB_SEX = 'M' ");
		else
			sTemp = _T(" M.MB_SEX = 'F' ");

		*psQuery += sTemp;
	}
	
	///////////////////////////////////////////////////////////////
	// 3. ȸ�� ���̵� ������ �Է��Ѵ�.
	///////////////////////////////////////////////////////////////
	if ( !sCtrlData[1].IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		sTemp = _T(" M.MB_ID LIKE '%") + sCtrlData[1] + _T("%' ");
		*psQuery += sTemp;
	}
	
	///////////////////////////////////////////////////////////////
	// 4. ȸ�� �̸� ������ �Է��Ѵ�.
	///////////////////////////////////////////////////////////////
	if ( !sCtrlData[2].IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		if ( !sCtrlData[3].IsEmpty() )
			sTemp = _T(" M.MB_NAME BETWEEN '") + sCtrlData[2] + _T("' AND '") + sCtrlData[3] + _T("��' ");
		else
			sTemp = _T(" M.MB_NAME BETWEEN '") + sCtrlData[2] + _T("' AND '") + sCtrlData[2] + _T("��' ");

		*psQuery += sTemp;
	}

	///////////////////////////////////////////////////////////////
	// 4. ȸ�� ���� ������ �Է��Ѵ�.
	///////////////////////////////////////////////////////////////
	if ( !sCtrlData[4].IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		///////////////////////////////////////////////////////////
		// 4 - 1. ���� �⵵�� ���Ѵ�.
		///////////////////////////////////////////////////////////
		CString query = _T("select sysdate from dual");
		sTemp = _T("");

		CESL_DataMgr *pDM = FindDM( _T("DM_SMS_LifeLong") );
		if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Search::MakeQuery_Member") );

		ids = pDM->GetOneValueQuery(query, sTemp);
		if (ids < 0)
		{
			CString error;
			error.Format(_T("DM_SMS_LifeLong_PGCategory::MakeQuery_User \r\n")
						 _T("		GetBirth error		"));
			return -1;
		}

		sTemp = sTemp.Mid(2,4);
		
		///////////////////////////////////////////////////////////
		// 4 - 2. ���۳⵵�� �������⵵ ������ �Է��Ѵ�.
		///////////////////////////////////////////////////////////
		INT nStart = _ttoi(sTemp) - _ttoi(sCtrlData[4]);

		while (nStart < 0)
			nStart = 100 + nStart;

		CString sStart, sEnd;


		if ( !sCtrlData[5].IsEmpty() )
		{
			INT nEnd = _ttoi(sTemp) - _ttoi(sCtrlData[5]);
			
			while (nEnd < 0)
				nEnd = 100 + nEnd;
		
			sStart.Format(_T("%02d/01/01"), nStart);
			sEnd.Format(_T("%02d/12/31"), nEnd);			
		}
		else 
		{		
			sStart.Format(_T("%02d/01/01"), nStart);
			sEnd.Format(_T("%02d/12/31"), nStart);		
		}

		sTemp = _T(" M.MB_BIRTH BETWEEN '") + sStart + _T("' AND '") + sEnd + _T("' ");
		*psQuery += sTemp;
	}

	///////////////////////////////////////////////////////////////
	// 5. ȸ�� ���� ������ �Է��Ѵ�.
	///////////////////////////////////////////////////////////////
	if ( !sCtrlData[6].IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		sTemp = _T(" M.MB_ADDR1 LIKE '%") + sCtrlData[6] + _T("%' ");
		*psQuery += sTemp;
	}

	///////////////////////////////////////////////////////////////
	// 6. ȸ�� ��ȭ��ȣ ������ �Է��Ѵ�.
	///////////////////////////////////////////////////////////////
	if ( !sCtrlData[7].IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		sTemp = _T(" M.MB_TEL LIKE '%") + sCtrlData[7] + _T("%' ");
		*psQuery += sTemp;
	}

	///////////////////////////////////////////////////////////////
	// 7. ȸ�� �ڵ��� ������ �Է��Ѵ�.
	///////////////////////////////////////////////////////////////
	if ( !sCtrlData[8].IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		sTemp = _T(" M.MB_HP LIKE '%") + sCtrlData[8] + _T("%' ");
		*psQuery += sTemp;
	}

	///////////////////////////////////////////////////////////////
	// 8. �׷� ������ �Է��Ѵ�.
	///////////////////////////////////////////////////////////////
	ids = GetUserGroup(&sTemp);

	if ( !sTemp.IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		CString query;
		
		query.Format( _T(" CLU.REC_KEY IN (Select USER_KEY FROM MN_LOAN_USER_GROUP_LIST_TBL WHERE GROUP_CODE = '%s' )"), sTemp );
		*psQuery += query;
	}
	///////////////////////////////////////////////////////////////
	// 9. �Է� �׽�Ʈ
	///////////////////////////////////////////////////////////////
	SetDlgItemText(IDC_eTest2, *psQuery);
	return 0;

	EFS_END
	return -1;
}

////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// ��ȸ���� ������ �����.
////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Search::MakeQuery_NonMember(CString *psQuery)
{	
	EFS_BEGIN

	INT ids;
	CString sTemp = _T("");
	
	///////////////////////////////////////////////////////////////
	// 1. ��Ʈ�� �Է°��� �����Ѵ�.
	///////////////////////////////////////////////////////////////	
	const INT nCount = 3;
 
	INT nCtrlID[nCount] = { 	IDC_eUSER_NAME1	   , IDC_eUSER_NAME2	,
								IDC_eUSER_PHONE2   };		
 		
	CString sCtrlData[nCount];

	for (INT i = 0;i < nCount; i++)
	{
		GetDlgItemText(nCtrlID[i] , sCtrlData[i]);
		sCtrlData[i].TrimLeft();sCtrlData[i].TrimRight();
	}
	
	///////////////////////////////////////////////////////////////
	// 2. ȸ�� �̸� ������ �Է��Ѵ�.
	///////////////////////////////////////////////////////////////
	if ( !sCtrlData[0].IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		if ( !sCtrlData[1].IsEmpty() )
			sTemp = _T(" CNM.NAME BETWEEN '") + sCtrlData[0] + _T("' AND '") + sCtrlData[1] + _T("��' ");
		else
			sTemp = _T(" CNM.NAME BETWEEN '") + sCtrlData[0] + _T("' AND '") + sCtrlData[0] + _T("��' ");

		*psQuery += sTemp;
	}
	
	///////////////////////////////////////////////////////////////
	// 3. ȸ�� �ڵ��� ������ �Է��Ѵ�.
	///////////////////////////////////////////////////////////////
	if ( !sCtrlData[2].IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		sTemp = _T(" CNM.HANDPHONE LIKE '") + sCtrlData[2] + _T("%' ");
		*psQuery += sTemp;
	}

	///////////////////////////////////////////////////////////////
	// 4. �׷� ������ �Է��Ѵ�.
	///////////////////////////////////////////////////////////////
	ids = GetUserGroup(&sTemp);

	if ( !sTemp.IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		CString query;
		query.Format( _T(" CNM.GROUP_CODE = '%s'"), sTemp);
		*psQuery += query;
	}

	///////////////////////////////////////////////////////////////
	// 5. �Է� �׽�Ʈ
	///////////////////////////////////////////////////////////////
	SetDlgItemText(IDC_eTest3, *psQuery);
	return 0;
	EFS_END
	return -1;
}

////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// ���� ������ �����.
////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Search::MakeQuery_Program(CString *psQuery)
{
EFS_BEGIN
	INT ids;
	CString sTemp = _T("");
	CString sWhere = _T("");
	
	///////////////////////////////////////////////////////////////
	// 1. ��Ʈ�� �Է°��� �����Ѵ�.
	///////////////////////////////////////////////////////////////	
	const INT nCount = 7;
 
	INT nCtrlID[nCount] = { IDC_cPROGRAM_CODE  , IDC_ePROGRAM_NAME  ,
						    IDC_dPROGRAM_OPEN1 , IDC_dPROGRAM_OPEN2 ,
 							IDC_dPROGRAM_END1  , IDC_dPROGRAM_END2  ,
							IDC_cPROGRAM_COM	   }; 
 		
	CString sCtrlData[nCount];

	for (INT i = 0;i < nCount; i++)
	{
		GetDlgItemText(nCtrlID[i] , sCtrlData[i]);
		sCtrlData[i].TrimLeft();sCtrlData[i].TrimRight();
	}
	
	///////////////////////////////////////////////////////////////
	// 2. ���±��� ������ �Է��Ѵ�.
	///////////////////////////////////////////////////////////////
	if ( sCtrlData[0].Compare(_T("�������")) != 0 )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		///////////////////////////////////////////////////////////
		// 2 - 1. ���� �˸��ƽ��� �ڵ�� �����Ѵ�.
		///////////////////////////////////////////////////////////
		CESL_DataMgr *pDM = FindDM( _T("DM_SMS_LifeLong_PGCategory") );
		if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Search::MakeQuery_Program") );

		ids = pDM->RefreshDataManager(_T(""));
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Search::MakeQuery_Program") );

		INT nCnt = pDM->GetRowCount();

		for (INT i = 0;i < nCnt;i++)
		{
			if (sCtrlData[0].Compare( pDM->GetCellData(_T("���±��и�"), i) ) == 0)
			{
				sTemp = pDM->GetCellData(_T("�ڵ�"), i);
				sTemp = _T(" P.CATEGORY = '") + sTemp + _T("' ");
				*psQuery += sTemp;
				break;
			}
		}
	}

	///////////////////////////////////////////////////////////////
	// 3. ���¸� ������ �Է��Ѵ�.
	///////////////////////////////////////////////////////////////
	if ( !sCtrlData[1].IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		sTemp = _T(" P.PG_NAME LIKE '%") + sCtrlData[1] + _T("%' ");
		*psQuery += sTemp;
	}

	///////////////////////////////////////////////////////////////
	// 4. ���°����� ������ �Է��Ѵ�.
	///////////////////////////////////////////////////////////////
	CString sStartDay=_T(""), sEndDay=_T("");

	sStartDay = GetCtrlDate( IDC_dPROGRAM_OPEN1 );
	if ( !sStartDay.IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");
	
		sEndDay = GetCtrlDate( IDC_dPROGRAM_OPEN2 );
		if ( !sEndDay.IsEmpty() )
		{
			sTemp = _T(" P.OPERATION_PERIOD1 BETWEEN '") + sStartDay + _T("' AND '") + sEndDay + _T("' ");
		}
		else
			sTemp = _T(" P.OPERATION_PERIOD1 = '") + sStartDay + _T("' ");

		*psQuery += sTemp;
	}

	///////////////////////////////////////////////////////////////
	// 5. ���¿Ϸ��� ������ �Է��Ѵ�.
	///////////////////////////////////////////////////////////////
	sStartDay = GetCtrlDate( IDC_dPROGRAM_END1 );
	if ( !sStartDay.IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

	
		sEndDay = GetCtrlDate( IDC_dPROGRAM_END2 );
		if ( !sEndDay.IsEmpty() )
		{
			sTemp = _T(" P.OPERATION_PERIOD1 BETWEEN '") + sStartDay + _T("' AND '") + sEndDay + _T("' ");
		}
		else
			sTemp = _T(" P.OPERATION_PERIOD1 = '") + sStartDay + _T("' ");

		*psQuery += sTemp;
	}

	///////////////////////////////////////////////////////////////
	// 6. ���Ῡ�� ������ �Է��Ѵ�.
	///////////////////////////////////////////////////////////////
	if ( !sCtrlData[6].IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		sTemp = _T(" PM.GRADUATION = '") + sCtrlData[6] + _T("' ");
		*psQuery += sTemp;
	}

	///////////////////////////////////////////////////////////////
	// 7. �Է� �׽�Ʈ
	///////////////////////////////////////////////////////////////
	SetDlgItemText(IDC_eTest1, *psQuery);

	return 0;
EFS_END
	return -1;
}

////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// �̿��� �׷��޺� �ڽ� ���� �̺�Ʈ
////////////////////////////////////////////////////////////////////////////
VOID CSMS_LifeLong_Search::OnSelchangecUSERGROUP() 
{
/*	///////////////////////////////////////////////////////////////////////
	// 1. ���õ� �޺� �ڽ��� �׷���� ���Ѵ�.
	///////////////////////////////////////////////////////////////////////
	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_3822_LifeLong"));
	if( pCM == NULL ) return;

	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("ȸ���׷�")));
	if( pCMCB == NULL ) return;
	
	INT nIndex = pCMCB->GetCurSel();
	CString sGetValue = _T("");
	pCMCB->GetData(sGetValue, nIndex, 1);

	///////////////////////////////////////////////////////////////////////
	// 2. �ش� SMS�� Ű�� �����Ѵ�.
	///////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_UserGroup") );
	if (pDM == NULL) return;
	pDM->RefreshDataManager( _T("") );

	INT nCnt = pDM->GetRowCount();
	CString sGetUserGroup = _T("");

	for (INT i = 0;i < nCnt;i++)
	{
		pDM->GetCellData( _T("�׷��̸�"), i, sGetUserGroup);
		if ( _tcscmp(sGetUserGroup, sGetValue) == 0 )
		{
			pDM->GetCellData( _T("�׷��ڵ�"), i, m_sUserGroup);
			pDM->GetCellData( _T("ȸ������"), i, sGetValue);
			
			if ( _tcscmp(sGetValue, _T("Y")) )
				m_nDisplayMode = 0;
			else if ( _tcscmp(sGetValue, _T("N")) )
				m_nDisplayMode = 1;

			break;
		}
	}
*/
}

////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// �ԷµǾ� �ִ� ������ ���� ��尡 ����ȴ�.
////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Search::SetCurMode()
{
	INT ids;
	CString strTemp = _T("");

	///////////////////////////////////////////////////////////////////////
	// 1. ���α׷� ������ �ִٸ� m_nDisplayMode = 0
	//    ���� ���а� ���¸����� �Ǵ��Ѵ�.
	///////////////////////////////////////////////////////////////////////
	const INT nCount = 2;
 
	INT nCtrlID[nCount] = { 	IDC_cPROGRAM_CODE  , IDC_ePROGRAM_NAME	};
	for (INT i = 0;i < nCount;i++)
	{
		GetDlgItemText(nCtrlID[i], strTemp);
		// ���� �ִٸ� ���α׷� ���̺��� �˻��ؾ��Ѵ�.
		if ( !strTemp.IsEmpty() )
		{
			m_nDisplayMode = 0;
			return 0;
		}
	}

	///////////////////////////////////////////////////////////////////////
	// 3. ȸ�������� �ִٸ� ��ȸ������ ��ȸ������ Ȯ���ؾ��Ѵ�.
	// ��ȸ���� m_nDisplayMode = 1
	// ȸ����   m_nDisplayMode = 2
	///////////////////////////////////////////////////////////////////////
	ids = GetUserGroup( &strTemp );
	if (ids < 0)	ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Search::SetCurMode") );

	
	if ( !strTemp.IsEmpty() )
	{
		CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_UserGroup") );
		if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Search::SetCurMode") );
		pDM->RefreshDataManager( _T("") );

		INT nCnt = pDM->GetRowCount();
		CString sCode = _T("");

		for (i = 0;i < nCnt;i++)
		{
			pDM->GetCellData( _T("�׷��ڵ�"), i, sCode);
			if ( _tcscmp(strTemp, sCode) == 0 )
			{
				pDM->GetCellData( _T("ȸ������"), i, sCode);

				// ��ȸ�� ���
				if ( _tcscmp(sCode, _T("N")) == 0 )
				{
					m_nDisplayMode = 1;
					return 0;
				}

				break;
			}
		}		
	}

	// ������� �Դٸ� ȸ�� ���
	m_nDisplayMode = 2;

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// [�Լ�����]
// �̿��� �׷�ڽ��� �ڵ带 �����Ѵ�.
////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Search::GetUserGroup(CString *pGetGroup)
{
	EFS_BEGIN

	///////////////////////////////////////////////////////////////
	// 1. �׷��޺��ڽ��� �����͸� ���Ѵ�.
	///////////////////////////////////////////////////////////////
	CESL_ControlMgr* pCM = FindCM(_T("CM_SMS_LifeLong_Search"));
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Search::GetUserGroup") );

	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("ȸ���׷�")));
	if( pCMCB == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Search::GetUserGroup") );
	
	///////////////////////////////////////////////////////////////
	// 2. �׷� �ڵ带 ����
	///////////////////////////////////////////////////////////////
	CString sTemp = _T("");
	pCMCB->GetData( sTemp, pCMCB->GetCurSel() );
	*pGetGroup = sTemp;

	return 0;
	EFS_END
	return -1;
}

VOID CSMS_LifeLong_Search::OnCancel() 
{
	// TODO: Add extra cleanup here
//	this->DestroyWindow();	
	CDialog::OnCancel();
}
