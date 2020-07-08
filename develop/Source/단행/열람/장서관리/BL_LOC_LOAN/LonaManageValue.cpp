// LonaManageValue.cpp : implementation file
//

#include "stdafx.h"
#include "LonaManageValue.h"
#include "..\..\..\..\����\����������\MNG_LIB_HOLIDAY_MGR\MNG_LIB_HOLIDAY_MGR_01.h"
#include "..\..\..\..\����\����������\FileManager\LibFileManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLonaManageValue dialog


CLonaManageValue::CLonaManageValue(CESL_Mgr* pParent,INT nMode)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CLonaManageValue)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bLoad = FALSE;
	m_nMode = nMode;

	if(InitESL_Mgr(_T("SM_LOAN_MANAGE_VALUE")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return ;
	}

	m_sRegCodeDMAlias = _T("DM_��ϱ���");
	m_sSeCodeDMAlias = _T("DM_��ġ��ȣ");
	m_sShelfLocCodeDMAlias = _T("DM_�ڷ��");
	m_sUseLimitCodeDMAlias = _T("DM_�̿����ѱ���");
	m_sUserPositionDMAlias = _T("DM_���޺��������а�");
	m_sIndividualUserPositionDMAlias = _T("DM_���޺��������а�_��������");
	m_sAllSettingValueDMAlias = _T("DM_��ٱ���������");
	m_sAttestation = _T("N");
	m_strLibCodeList = _T("");

	m_sUnionLibInfoDMAlias    = _T("DM_BO_LOC_3100_LIB_INFO");
	m_pUnionLibRegLngDMAlias  = _T("DM_BO_LOC_3100_LIB_REGCODE_LENGTH");

	m_bIsLoanReturnMsgDisp = TRUE;
}

CLonaManageValue::~CLonaManageValue()
{
}	

VOID CLonaManageValue::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLonaManageValue)
	DDX_Control(pDX, IDC_LOAN_MANAGE_VALUE_PROGRESS, m_ctrlProgress);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CLonaManageValue, CDialog)
	//{{AFX_MSG_MAP(CLonaManageValue)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLonaManageValue message handlers

INT CLonaManageValue::MakeManageValue()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("MakeManageValue");
	CString strValue;
	
	SetProgress(0, _T("�۾�����"));

	m_pAllSettingValueDM = FindDM(m_sAllSettingValueDMAlias);
	if(m_pAllSettingValueDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	ids = CLocCommonAPI::RefreshDataManager(this, m_pAllSettingValueDM, _T(""));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	m_pRegClassDM = FindDM(_T("DM_������޼�����"));
	if(m_pRegClassDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
	ids = CLocCommonAPI::RefreshDataManager(this, m_pRegClassDM, _T(""));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(5, _T("��ϱ��к� ���� ����"));
	m_pRegCodeDM = FindDM(m_sRegCodeDMAlias);
	if(m_pRegCodeDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2(-1040, strFuncName);
	ids = CLocCommonAPI::RefreshDataManager(this, m_pRegCodeDM, _T(""));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	SetProgress(10, _T("��ġ��ȣ�� ���� ����"));
	m_pSeCodeDM = FindDM(m_sSeCodeDMAlias);
	if(m_pSeCodeDM == NULL)ERROR_MSG_RETURN_ERROR_CODE2(-1060, strFuncName);
	ids = CLocCommonAPI::RefreshDataManager(this, m_pSeCodeDM, _T(""));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(15, _T("�̿����ѱ��к� ���� ����"));
	m_pUseLimitCodeDM = FindDM(m_sUseLimitCodeDMAlias);
	if(m_pUseLimitCodeDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2(-1080, strFuncName);
	ids = CLocCommonAPI::RefreshDataManager(this, m_pUseLimitCodeDM, _T(""));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(16, _T("�ڷ�Ǻ� ���� ����"));
	m_pUseShelfLocCodeDM = FindDM(m_sShelfLocCodeDMAlias);
	if(m_pUseShelfLocCodeDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2(-1100, strFuncName);
	ids = CLocCommonAPI::RefreshDataManager(this, m_pUseShelfLocCodeDM, _T(""));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	SetProgress(20, _T("���޺� �������а�"));
	m_pUserPositionDM = FindDM(m_sUserPositionDMAlias);
	if(m_pUserPositionDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2(-1120, strFuncName);
	ids = CLocCommonAPI::RefreshDataManager(this, m_pUserPositionDM, _T(""));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	SetProgress(21, _T("�������� ��ϱ����ڸ���"));
	m_pUnionLibRegLngDM = FindDM(m_pUnionLibRegLngDMAlias);
	if(m_pUnionLibRegLngDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2(-1125, strFuncName);
	ids = CLocCommonAPI::RefreshDataManager(this, m_pUnionLibRegLngDM, _T(" VALUE_NAME='��ϱ����ڸ���'"));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	SetProgress(22, _T("����������"));
	m_pUnionLibInfoDM = FindDM(m_sUnionLibInfoDMAlias);
	if(m_pUnionLibInfoDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2(-1130, strFuncName);
	ids = CLocCommonAPI::RefreshDataManager(this, m_pUnionLibInfoDM, _T(""));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	m_pIndividualUserPositionDM = FindDM(m_sIndividualUserPositionDMAlias);
	if(m_pIndividualUserPositionDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2(-1140, strFuncName);
	ids = CLocCommonAPI::RefreshDataManager(this, m_pIndividualUserPositionDM, _T(""));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	CString sSql;
	sSql.Format(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE='ONELNERESERV'"));	
	m_pAllSettingValueDM->GetOneValueQuery(sSql, strValue);
	strValue.TrimLeft();	strValue.TrimRight();
	if(_T("Y") == strValue)	
	{
		m_bVolReserve = TRUE;
	}
	else
	{
		m_bVolReserve = FALSE;
	}

	m_strEachManageUseYN = _T("Y");

	ids = GetManageValue(_T("����"), _T("����"), _T("�������Ļ�뿩��"), _T("����"), m_sInLibLoanTpyeUse  );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	ids = GetManageValue(_T("����"), _T("����"), _T("�������Ļ�뿩��"), _T("Ư��"), m_sSpeciealLoanTpyeUse);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	ids = GetManageValue(_T("����"), _T("����"), _T("�������Ļ�뿩��"), _T("���"), m_sLongLoanTpyeUse);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	ids = GetManageValue(_T("����"), _T("����"), _T("�ݳ����Ļ�뿩��"), _T("�ϰ�"), m_sAllLoanTpyeUse);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	ids = GetManageValue(_T("����"), _T("����"), _T("�ݳ����Ļ�뿩��"), _T("�ұ�"), m_sModifyLoanTpyeUse);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	ids = GetManageValue(_T("����"), _T("����"), _T("å�η��ý���"), _T("å�η���뿩��"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_sBookCooperativeSys = _T("N");
	}
	else
	{
		m_sBookCooperativeSys = strValue;
	}
	
	if(m_sBookCooperativeSys == _T("Y"))
	{
		m_sIsLoanManageCode = _T("N");

		m_sIsReturnManageCode = _T("N");

		m_sIsOtherManageReturn = _T("N");
		
		ids = GetManageValue(_T("����"), _T("����"), _T("å�η��ý���"), _T("å�η�����"), strValue);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
		if(strValue.IsEmpty()) 
		{
			m_sBookCooperativeMode = _T("I");
		}
		else
		{
			m_sBookCooperativeMode = strValue;
		}
	}
	else
	{
		m_sBookCooperativeMode = _T("I");

		ids = GetManageValue(_T("����"), _T("����"), _T("�������"), _T("�ٸ��������а����⿩��"), strValue);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
		if(strValue.IsEmpty())
		{
			m_sIsLoanManageCode = _T("N");
		}
		else
		{
			m_sIsLoanManageCode = strValue;
		}
		ids = GetManageValue(_T("����"), _T("����"), _T("�������"), _T("�ٸ��������а��ݳ�����"), strValue);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
		if(strValue.IsEmpty())
		{
			m_sIsReturnManageCode = _T("N");
		}
		else
		{
			m_sIsReturnManageCode = strValue;
		}
	
		ids = GetManageValue( _T("��Ÿ"), _T("����"), _T("�������к��ݳ�����"), _T(""), strValue);
		if(strValue.IsEmpty())
		{
			m_sIsOtherManageReturn = _T("N");
		}
		else
		{
			m_sIsOtherManageReturn = strValue;
		}
	}

	SetProgress(24, _T("���� ����� ����"));
	ids = GetManageValue(_T("����"), _T("����"), _T("�������"), _T("��������⿩��"), m_sIsOneManReLoan );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(26, _T("���� ����� ����"));
	ids = GetManageValue(_T("����"), _T("����"), _T("�������"), _T("��������⿩��"), m_sIsFamilyReLoan );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(26, _T("���� �̷� �����ֱ� ����"));
	ids = GetManageValue(_T("����"), _T("����"), _T("�������"), _T("������̷º����ֱ⿩��"), m_sIsViewLoanHistory );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(27, _T("�����������"));
	ids = GetManageValue(_T("����"), _T("����"), _T("�������"), _T("�����������"), m_sReLoanLimitDay );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	SetProgress(28, _T("��ü��"));
	ids = GetManageValue(_T("����"), _T("����"), _T("��ü����"), _T("��ü��"), m_sDelayMoney );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(30, _T("��ü�� ����ġ"));
	ids = GetManageValue(_T("����"), _T("����"), _T("��ü����"), _T("��ü�ϰ���ġ"), m_sAddDelayDay );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(34, _T("��ü�ϸ��"));
	ids = GetManageValue(_T("����"), _T("����"), _T("��ü����"), _T("��ü�ϸ��"), m_sLoanStopMode);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(36, _T("���� ������ �ο� ����"));
	ids = GetManageValue(_T("����"), _T("����"), _T("��ü����"), _T("���������Ϻο�����"), m_sIsLoanStop );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(40, _T("��ü�� �ο� ����"));
	ids = GetManageValue(_T("����"), _T("����"), _T("��ü����"), _T("��ü��ο�����"), m_sIsDelayMoney );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(44, _T("�ݳ����⿹��켱����"));
	ids = GetManageValue(_T("����"), _T("����"), _T("�ݳ��������"), _T("�ݳ����⿹��켱����"), m_sDelayBook );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(48, _T("�ݳ������ڵ������ο�"));
	ids = GetManageValue(_T("����"), _T("����"), _T("�ݳ��������"), _T("�ݳ������ڵ������ο�"), m_sAutoDelayFunc );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(52, _T("�ݳ������ɻ�뿩��"));
	ids = GetManageValue(_T("����"), _T("����"), _T("�ݳ��������"), _T("�ݳ������ɻ�뿩��"), m_sIsDelayFunc );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(56, _T("�ݳ����Ⱑ��Ƚ��"));
	ids = GetManageValue(_T("����"), _T("����"), _T("�ݳ��������"), _T("�ݳ����Ⱑ��Ƚ��"), m_sDelayCnt );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(60, _T("���������ϰ���������"));
	ids = GetManageValue(_T("����"), _T("����"), _T("�̿��ڰ���"), _T("���������ϰ���������"), m_sIsAutoUnLoanStop );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(64, _T("�����ڵ���ҿ���"));
	ids = GetManageValue(_T("����"), _T("����"), _T("�������"), _T("�����ڵ���ҿ���"), m_sIsAutoCancelReserve);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(68, _T("��������ο�"));
	ids = GetManageValue(_T("����"), _T("����"), _T("�������"), _T("��������ο�"), m_sReserveUserCnt );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(70, _T("���ุ�����ڵ��ο�����"));
	ids = GetManageValue(_T("����"), _T("����"), _T("�������"), _T("���ุ�����ڵ��ο�����"), m_sIsAutoSetReservationDay );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(72, _T("���������ٱ���"));
	ids = GetManageValue(_T("����"), _T("����"), _T("���������ٱ���"), m_pInfo->USER_ID, m_sLoanUserAccessMode);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	ids = GetManageValue(_T("����"), _T("����"), _T("�̿��ڰ���"), _T("����ȸ����ɻ�뿩��"), m_sIsFamilyFunc );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(72, _T("�ηϰ�����"));
	ids = GetManageValue(_T("����"), _T("����"), _T("�ηϰ���"), _T("�ηϴ��⿩��"), m_sIsLoanAppendix);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	ids = GetManageValue(_T("����"), _T("����"), _T("�ηϰ���"), _T("�ηϹݳ�����"), m_sIsReturnAppendix);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	ids = GetManageValue(_T("����"), _T("����"), _T("�ηϰ���"), _T("�ηϴ���Ǽ����Կ���"), m_sIsIncludeLoanCnt);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	ids = GetManageValue(_T("����"), _T("����"), _T("�ηϰ���"), _T("�ηϿ�ü���뿩��"), m_sIsAppendixLoanStop);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	ids = GetManageValue(_T("����"), _T("����"), _T("�ݳ��������"), _T("�ݳ�����ο�����"), m_sDelayOption);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(90, _T("�����ɻ�뿩��"));
	ids = GetManageValue(_T("����"), _T("����"), _T("�������"), _T("�����ɻ�뿩��"), m_sReserveMode);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(95, _T("�ް���"));
	CLibFileManager HolyDayList;
	HolyDayList.SaveDataToFile(m_pRegClassDM, GetUserID());		
	
	SetProgress(96, _T("��ü�Ͽ� +/- �� ��¥ ��"));
	ids = GetManageValue(_T("����"), _T("����"), _T("��ü����"), _T("��ü�Ϻο��ϼ�"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	m_nDelayDayCnt = _ttoi(strValue);
	
	SetProgress(96, _T("���������Ͽ� +/- �� ��¥ ��"));
	ids = GetManageValue(_T("����"), _T("����"), _T("��ü����"), _T("���������Ϻο��ϼ�"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	m_nLoanStopDayCnt = _ttoi(strValue);

	ids = GetManageValue(_T("��Ÿ"), _T("����"), _T("�̵�������"), _T("������å��������"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty())	
	{
		m_sLoanPS = _T("Y");
	}
	else
	{
		m_sLoanPS = strValue;
	}

	ids = GetManageValue(_T("����"), _T("����"), _T("�������"), _T("������ҽø��Ϲ߼ۿ���"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_sIsSendCancelMail = _T("N");
	}
	else
	{
		m_sIsSendCancelMail = strValue;
	}

	ids = GetManageValue(_T("��Ÿ"), _T("����"), _T("��â������"), _T("��â������"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty())	
	{
		m_sIsSunchanglib = _T("N");
	}
	else
	{
		m_sIsSunchanglib = strValue;
	}	

	ids = GetManageValue(_T("��Ÿ"), _T("����"), _T("5163"), _T("5163"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty())	
	{
		m_sIs5163lib = _T("N");
	}
	else
	{
		m_sIs5163lib = strValue;
	}

	ids = GetManageValue(_T("��Ÿ"), _T("����"), _T("����"), _T("����"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty())
	{
		m_sIs64lib = _T("N");
	}
	else
	{
		m_sIs64lib = strValue;
	}

	ids = GetManageValue(_T("��Ÿ"), _T("����"), _T("�����"), _T("�����"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty())
	{
		m_sIs90lib = _T("N");
	}
	else
	{
		m_sIs90lib = strValue;
	}

	ids = GetManageValue(_T("����"), _T("����"), _T("��ü����"), _T("������üȽ���ο����"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	m_sMethodDelayCntPlus = strValue;
	
	strValue = m_pInfo->MANAGE_CODE;
	m_sUserManageCode = strValue;

	ids = GetManageValue(_T("����"), _T("����"), _T("���������"), _T("��������»�뿩��"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty())
	{
		m_sUseReceipt = _T("N");
	}
	else
	{
		m_sUseReceipt = strValue;
	}

	ids = GetManageValue(_T("����"), _T("����"), _T("���������"), _T("��������»������"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty())	
	{
		m_sUseReceiptType = _T("NN");
	}
	else
	{
		m_sUseReceiptType = strValue;
	}

	GetReceiptAttestation(m_sAttestation);
	if(m_sAttestation == _T("N"))
	{		
		m_sUseReceipt = _T("N");
		m_sUseReceiptType = _T("NN");
	}

	ids = GetManageValue(_T("��Ÿ"), _T("����"), _T("�����ڰ�������"), _T("��������ȸ���޽������"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue == _T("2"))	
	{
		m_sIsUnionUserStatus = _T("Y");
	}
	else
	{
		m_sIsUnionUserStatus = _T("N");
	}

	ids = GetManageValue(_T("��Ÿ"), _T("����"), _T("�����ڰ�������"), _T("���մ���å������"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_sIsUnionLoanCnt = _T("N");
	}
	else
	{
		m_sIsUnionLoanCnt = strValue;
	}

	strValue.Empty();
	ids = GetManageValue(_T("��Ÿ"), _T("����"), _T("�����ڰ�������"), _T("����ȸ���޽������"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	strValue.TrimLeft( ); 
	strValue.TrimRight( );
	if ( strValue == _T("2") )
	{
		m_sIsUnionUserRemoveStatus = _T("Y");
	}
	else
	{
		m_sIsUnionUserRemoveStatus = _T("N");
	}

	ids = GetManageValue(_T("����"), _T("����"), _T("�������"), _T("���ุ���ް�����������"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_sIsAddHolyDayReservation = _T("N");
	}
	else
	{
		m_sIsAddHolyDayReservation = strValue;
	}

	ids = GetManageValue(_T("����"), _T("����"), _T("��ü����"), _T("���������ް�����������"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_sIsAddHolyDayLoanStop = _T("N");
	}
	else
	{
		m_sIsAddHolyDayLoanStop = strValue;
	}

	ids = GetManageValue(_T("����"), _T("����"), _T("�������"), _T("ȸ�����нǽô���Ұ�����"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_sLoanIsLostUserCard = _T("N");
	}
	else
	{
		m_sLoanIsLostUserCard = strValue;
	}

	ids = GetManageValue(_T("����"), _T("����"), _T("���������"), _T("����������ý��ۻ������"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_sFeeMngSys = _T("N");
	}
	else
	{
		m_sFeeMngSys = strValue;
	}

	ids = GetManageValue(_T("����"), _T("����"), _T("�����ڷ����"), _T("���δ��⿹��������"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_sUnmannedReserveSys = _T("N");
	}
	else
	{
		m_sUnmannedReserveSys = strValue;
	}

	ids = GetManageValue(_T("����"), _T("����"), _T("�����ڷ����"), _T("�߰����⿹��������"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_sNightReserveSys = _T("N");
	}
	else
	{
		m_sNightReserveSys = strValue;
	}

	ids = GetManageValue(_T("����"), _T("����"), _T("���մ�����"), _T("���մ���Ǽ���뿩��"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_sUnityLoanCntYN = _T("N");
	}
	else
	{
		m_sUnityLoanCntYN = strValue;
	}

	ids = GetManageValue(_T("����"), _T("����"), _T("�������"), _T("���ุ���Ϻο���SMS�߼ۿ���"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_sReserveExpireSMS = _T("N");
	}
	else
	{
		m_sReserveExpireSMS = strValue;
	}
	
	ids = GetManageValue(_T("��Ÿ"), _T("����"), _T("���ϸ��ý���"), _T("��뿩��"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_sMailingSystemYN = _T("N");
	}
	else
	{
		m_sMailingSystemYN = strValue;
	}

	ids = GetManageValue(_T("����"), _T("����"), _T("��ü����"), _T("�ִ���������ο��ϼ�"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_strMaxLoanStopDay = _T("N");
	}
	else
	{
		m_strMaxLoanStopDay = strValue;
	}

	m_nUserBasketCnt = 100;

	ids = GetManageValue(_T("����"), _T("����"), _T("�̿��ڰ���"), _T("�����ڹ�ȣ�����ID���ڵ��Է�"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if( TRUE == strValue.IsEmpty() ) 
	{
		m_strIsAutoSetUsernoToUserid = _T("N");
	}
	else
	{
		m_strIsAutoSetUsernoToUserid = strValue;
	}

	ids = GetManageValue(_T("����"), _T("����"), _T("�̿��ڰ���"), _T("�ֹε�Ϲ�ȣ���ڸ���й�ȣ���ڵ��Է�"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if( TRUE == strValue.IsEmpty() ) 
	{
		m_strIsAutoSetCivilnoToPassword = _T("N");
	}
	else
	{
		m_strIsAutoSetCivilnoToPassword = strValue;
	}

	ids = GetManageValue(_T("��Ÿ"), _T("����"), _T("�������"), _T("����ݳ���������"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(TRUE == strValue.IsEmpty()) 
	{
		m_strLoanShare = _T("0");
	}
	else
	{
		m_strLoanShare = strValue;
	}
	ids = GetManageValue(_T("��Ÿ"), _T("����"), _T("�������"), _T("�����ڰ�������"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(TRUE == strValue.IsEmpty()) 
	{
		m_strUserShare = _T("0");
	}
	else
	{
		m_strUserShare = strValue;
	}

	if(_T("0") == m_strUserShare)
	{
		m_sIsUnionUserStatus = _T("N");
		m_sIsUnionLoanCnt = _T("N");
		m_sIsUnionUserRemoveStatus = _T("N");
	}

	ids = GetManageValue(_T("��Ÿ"), _T("����"), _T("���յ�������"), _T("���յ�������"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(_T("Y") != strValue) 
	{
		m_sIsUnityLoanService = _T("N");
	}
	else
	{
		m_sIsUnityLoanService = strValue;
	}
	m_sIsShareInsert = _T("N");

	if(_T("Y") == m_sIsUnityLoanService)
	{
		// ���մ���Ǽ���뿩��
		m_sUnityLoanCntYN = _T("Y");
 		m_sIsUnionLoanCnt = _T("N");
		ids = GetManageValue(_T("��Ÿ"), _T("����"), _T("���յ�������"), _T("����������"), strValue);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
		if(_T("Y") != strValue)
		{
			m_sIsShareInsert = _T("N");
		}
		
		CESL_DataMgr tempDM;
		tempDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		sSql = _T("SELECT DISTINCT LIB_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE IS NOT NULL ORDER BY LIB_CODE");
		tempDM.RestructDataManager(sSql);
		m_strLibCodeList = _T("");
		for(INT i = 0; i < tempDM.GetRowCount(); i++)
		{
			tempDM.GetCellData(i, 0, strValue);
			if(!strValue.IsEmpty())
			{
				if(!m_strLibCodeList.IsEmpty())
				{
					m_strLibCodeList += _T(",");
				}
				m_strLibCodeList += strValue;
			}
		}
	}

	ids = GetManageValue(_T("��Ÿ"), _T("����"), _T("���յ�������"), _T("��������������å"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_strUserClassShare = _T("1");
	}
	else
	{
		m_strUserClassShare = strValue;
	}

	ids = GetManageValue(_T("����"), _T("����"), _T("��ü����"), _T("��ü��̳��ô���Ұ�����"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_ArrearCheckYN = _T("N");
	}
	else
	{
		m_ArrearCheckYN = strValue;
	}

	ids = GetManageValue(_T("��Ÿ"), _T("����"), _T("���յ�������"), _T("�ֹι�ȣ���ȼ�����뿩��"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(_T("N")!=strValue)
	{
		m_strIsKLSecurityCivilNo = _T("Y");
	}
	else
	{
		m_strIsKLSecurityCivilNo = strValue;
	}

	ids = GetManageValue(_T("��Ÿ"), _T("����"), _T("����ݳ�����"), _T("����ݳ������뿩��"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(_T("Y") == strValue) {
		m_str3100ControlYN = strValue;
	} else {
		m_str3100ControlYN = _T("N");
	}

	ids = GetManageValue(_T("��Ÿ"), _T("����"), _T("RFIDå�η��ɼ�"), _T("å�η�Ÿ���ݳ������ȼ�������"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
	if(_T("Y")!=strValue) {
		m_strSetRFIDSecuReturnLibYN = _T("N");
	}
	else {
		m_strSetRFIDSecuReturnLibYN = strValue;
	}

	ids = GetManageValue(_T("����"), _T("����"), _T("�������"), _T("Ÿ�ý���ī���ȣ��뿩��"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
	if(_T("Y")!=strValue) {
		m_strScccardNoYN = _T("N");
	}
	else {
		m_strScccardNoYN = strValue;
	}

	SetProgress(100, _T("�Ϸ�"));

	return 0;

EFS_END
return -1;
}

INT CLonaManageValue::IsLoanCodeYN(CString sCodeAlias, CString sCode, CString &sValue)
{
EFS_BEGIN

	INT ids;

	if(sCode.IsEmpty())
	{
		sValue = _T("Y");
		return 0;
	}

	CESL_DataMgr *pCodeMgr;

	if(sCodeAlias.Compare(_T("��ϱ���"))==0)
	{
		pCodeMgr = m_pRegCodeDM;
	}
	else if(sCodeAlias.Compare(_T("��ġ��ȣ"))==0)
	{
		pCodeMgr = m_pSeCodeDM;
	}
	else if(sCodeAlias.Compare(_T("�̿����ѱ���"))==0)
	{
		pCodeMgr = m_pUseLimitCodeDM;
	}
	else if(sCodeAlias.Compare(_T("�ڷ�Ǳ���"))==0)
	{
		pCodeMgr = m_pUseShelfLocCodeDM;
	}
	else 
	{
		sValue = _T("Y");
		return 0;
	}

	CString sLoanYN;
	CString sCodes;

	for(INT i = 0 ; i < pCodeMgr->GetRowCount() ; i++ )
	{
		ids = pCodeMgr->GetCellData(_T("�ڵ�"), i, sCodes);
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("IsLoanCodeYN()")); 
		}
		sCodes.TrimLeft(); 
		sCodes.TrimRight();

		if(sCodes.Compare(sCode) == 0)
		{
			ids = pCodeMgr->GetCellData(_T("���⿩��"), i, sLoanYN );
			if(0 > ids) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("IsLoanCodeYN()")); 
			}
			sLoanYN.TrimLeft(); 
			sLoanYN.TrimRight();
			sValue = sLoanYN;

			return 0;
		}
	}

	return 0;

EFS_END
return -1;
}

INT CLonaManageValue::GetUserPosition(CString sPosition, CString sAlias, CString &sValue)
{
EFS_BEGIN

	INT ids;

	CString sPositions;
	for(INT i = 0 ; i < m_pUserPositionDM->GetRowCount() ; i++ )
	{
		ids = m_pUserPositionDM->GetCellData(_T("�ڵ�"), i, sPositions);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("GetUserPosition"));
		}

		sPositions.TrimLeft(); 
		sPositions.TrimRight();

		if(sPosition.Compare(sPositions)==0)
		{
			ids = m_pUserPositionDM->GetCellData(sAlias, i, sValue);
			if(0 > ids)
			{
				AfxMessageBox(sAlias);
			}
			if(0 > ids)
			{
				ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("GetUserPosition"));
			}

			return 0;
		}
	}
	
	return 0;

EFS_END
return -1;
}

INT CLonaManageValue::GetIndividualUserPosition(CString sPosition, CString sAlias, CString &sValue)
{
EFS_BEGIN

	INT ids;

	CString sPositions;
	for(INT i = 0 ; i < m_pIndividualUserPositionDM->GetRowCount() ; i++ )
	{
		ids = m_pIndividualUserPositionDM->GetCellData(_T("�ڵ�"), i, sPositions);
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("GetIndividualUserPosition"));
		}

		sPositions.TrimLeft(); 
		sPositions.TrimRight();

		if(sPosition.Compare(sPositions)==0)
		{
			ids = m_pIndividualUserPositionDM->GetCellData(sAlias, i, sValue);
			if(0 > ids) 
			{
				AfxMessageBox(sAlias);
			}
			if(0 > ids) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("GetIndividualUserPosition"));
			}

			return 0;
		}
	}
	
	return 0;

EFS_END
return -1;
}

BOOL CLonaManageValue::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd()); 	

	SetTimer(0, 100, NULL);
	SetTimer(1, 100, NULL);

	return TRUE;  

EFS_END
return FALSE;
}

BOOL CLonaManageValue::Create(CWnd* pParentWnd) 
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;
}

// ���� ��Ȳ�� ǥ�����ִ� �Լ�
INT CLonaManageValue::SetProgress(INT nPos, CString sMsg )
{
EFS_BEGIN

	CString sPMsg;
	if(nPos == 0)
	{
		m_ctrlProgress.SetRange32(0, 100);
		GetDlgItem(IDC_staMSG)->SetWindowText(_T("����"));

		return 0;
	}
	else if(nPos == 100)
	{
		sPMsg = _T("100%% \r\n�Ϸ�");
	}
	else
	{
		sPMsg.Format(_T("%d%% ������... \r\n%s ������"), nPos, sMsg );
	}

	GetDlgItem(IDC_staMSG)->SetWindowText(sPMsg );

	m_ctrlProgress.SetPos(nPos);

	return 0;

EFS_END
return -1;
}

VOID CLonaManageValue::OnTimer(UINT nIDEvent) 
{
EFS_BEGIN

	if(nIDEvent == 0)
	{
		KillTimer(0);
		if(m_ctrlProgress.GetPos() == 100)
		{
			m_ctrlProgress.SetPos(0);
			OnCancel();
			m_bLoad = TRUE;
		}
		else
		{
			SetTimer(0, 100, NULL);
		}
	}
	else if(nIDEvent == 1 )
	{
		KillTimer(1);
		if(GetSafeHwnd() != NULL) 
		{
			MakeManageValue();
		}
		else
		{
			SetTimer(1, 100, NULL);
		}
	}
	CDialog::OnTimer(nIDEvent);

EFS_END
}

INT CLonaManageValue::ReLoad()
{
EFS_BEGIN

	ShowWindow(SW_SHOW);
	SetTimer(1, 50, NULL);
	SetTimer(0, 100, NULL);

	return 0;

EFS_END
return -1;
}

/// ��ٱ��� DM���� VALUE_1�� �̾Ƴ��� �Լ�
INT CLonaManageValue::GetManageValue(CString sGroup2, CString sGroup3, CString sAlias, CString sValueName, CString &sValue)
{
EFS_BEGIN

	INT ids;

	ids = CLocCommonAPI::GetManageValue(this, m_pAllSettingValueDM, sGroup2, sGroup3, sAlias, sValueName, sValue);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("GetManageValue"));
	}

	return 0;

EFS_END
return -1;
}

//2009.02.02 ADD BY PWR 
/// ��ٱ��� DM���� VALUE_NAME�� �̾Ƴ��� �Լ�
INT CLonaManageValue::GetManageValue_Name(CString sGroup2, CString sGroup3, CString sAlias, CString &sValueName)
{
EFS_BEGIN

	INT ids;

	ids = CLocCommonAPI::GetManageValue_Name(this, m_pAllSettingValueDM, sGroup2, sGroup3, sAlias, sValueName);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("GetManageValue_Name"));
	}

	return 0;

EFS_END
return -1;
}

/// ��ϱ��� ���޺� ȯ�氪 �����ϴ� �Լ�
INT CLonaManageValue::GetRegClass(CString sUserClass, CString sRegCode, CString sFieldName, CString &sValue)
{
EFS_BEGIN

	INT ids;

	sUserClass.TrimLeft(); 	
	sUserClass.TrimRight();
	sRegCode.TrimLeft();	
	sRegCode.TrimRight();

	CString sTmpUserClass, sTmpRegCode;
	for(INT i = 0 ; i < m_pRegClassDM->GetRowCount() ; i++ )
	{
		ids = m_pRegClassDM->GetCellData(_T("�����ڵ�"), i, sTmpUserClass);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("GetRegClass"));
		}

		ids = m_pRegClassDM->GetCellData(_T("��ϱ���"), i, sTmpRegCode);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("GetRegClass"));
		}

		sTmpUserClass.TrimLeft(); 	
		sTmpUserClass.TrimRight();
		sTmpRegCode.TrimLeft();	
		sTmpRegCode.TrimRight();

		if(sTmpUserClass.Compare(sUserClass)==0 && (sTmpRegCode.Compare(sRegCode)==0))
		{
			ids = m_pRegClassDM->GetCellData(sFieldName, i, sValue);
			if(0 > ids)
			{
				ERROR_MSG_RETURN_ERROR_CODE2(-1020, _T("GetRegClass"));
			}
		}
	}

	return 0;

EFS_END
return -1;
}

INT CLonaManageValue::GetReceiptManageValue(CString &f_strPrINTBarcode, CString &f_strPrINTBarcodeAll, CString &f_strPrINTItem, CString &f_strPrINTMode, CString &f_strDeviceType, CString &f_strPreview, CString &f_strNotify )
{	
	// 2008.04.14 ADD BY PWR 
	// �������� : PRINT_NOTIFY �߰�
	CStdioFile file ;	
	CString	strPrINTBarcode,strPrINTBarcodeAll;
	CString	strPrINTItem;
	CString	strPrINTMode;
	CString strTemp;
	CString strDeviceType;
	CString strPreview;
	CString strNotify;
	INT		idx;

	strTemp = _T("..\\cfg\\ReceiptPrINT.cfg") ;	
	if(!file.Open (strTemp,CFile::modeRead | CFile::typeBinary)) 
	{
		return FALSE ;
	}
	TCHAR cUni;
	file.Read(&cUni, sizeof(TCHAR));
	if(0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	CString strLine ;
	CString sTemp ;
	while(file.ReadString (strLine))
	{
		strLine.TrimLeft();
		if(strLine.Left(1) == _T("#") || strLine.IsEmpty()) 
		{
			continue;		
		}
		if(strLine.Left(1) == _T("[") || strLine.IsEmpty())
		{
			continue;		
		}

		idx = strLine.Find(_T("="));
		if(0 <= idx)
		{
			sTemp = strLine.Left(idx);
			sTemp.TrimLeft();
			sTemp.TrimRight();
			if(sTemp == _T("PRINT_BARCODE")) 
			{					
				strPrINTBarcode = strLine.Mid(idx+1);
				strPrINTBarcode.TrimLeft();
				strPrINTBarcode.TrimRight();
			} 
			if(sTemp == _T("PRINT_BARCODE_ALL")) 
			{					
				strPrINTBarcodeAll = strLine.Mid(idx+1);
				strPrINTBarcodeAll.TrimLeft();
				strPrINTBarcodeAll.TrimRight();
			} 
			if(sTemp == _T("PRINT_ITEM")) 
			{	
				strPrINTItem = strLine.Mid(idx+1);
				strPrINTItem.TrimLeft();
				strPrINTItem.TrimRight();
			} 
			if(sTemp == _T("PRINT_MODE")) 
			{
				strPrINTMode = strLine.Mid(idx+1);
				strPrINTMode.TrimLeft();
				strPrINTMode.TrimRight();
			} 
			if(sTemp == _T("PRINT_DEVICE")) 
			{
				strDeviceType = strLine.Mid(idx+1);
				strDeviceType.TrimLeft();
				strDeviceType.TrimRight();
			} 
			if(sTemp == _T("PRINT_PREVIEW")) 
			{
				strPreview = strLine.Mid(idx+1);
				strPreview.TrimLeft();
				strPreview.TrimRight();
			}
			if(sTemp == _T("PRINT_NOTIFY")) 
			{
				strNotify = strLine.Mid(idx+1);
				strNotify.TrimLeft();
				strNotify.TrimRight();
			}
		}
	}
	file.Close () ;

	f_strPrINTBarcode	= strPrINTBarcode;
	f_strPrINTBarcodeAll= strPrINTBarcodeAll;
	f_strPrINTItem		= strPrINTItem;
	f_strPrINTMode		= strPrINTMode;
	f_strDeviceType		= strDeviceType;
	f_strPreview		= strPreview;
	f_strNotify			= strNotify;

	GetReceiptAttestation(m_sAttestation);
	if(m_sAttestation == _T("N"))
	{
		f_strPrINTMode = _T("2");
	}
	
	return 0;
}

INT CLonaManageValue::SetReceiptManageValue(CString f_strPrINTBarcode, CString f_strPrINTBarcodeAll, CString f_strPrINTItem, CString f_strPrINTMode, CString f_strDeviceType, CString f_strPreview, CString f_strNotify )
{
	CStdioFile file;
	CString strFilePath;
	CString	strConfig;

	strFilePath = _T("..\\cfg\\ReceiptPrINT.cfg") ;	
	if(!file.Open (strFilePath, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary ))
	{
		return FALSE;
	}

	if(0 == file.GetLength())
	{			
		TCHAR cUni = 0xFEFF;
		file.Write(&cUni, sizeof(TCHAR));
	}
	
	// 2008.04.14 ADD BY PWR 
	// �������� : PRINT_NOTIFY �߰�

	strConfig.Format(_T("[RECEIPT_PRINT]\r\n")
		             _T("PRINT_BARCODE = %s\r\n")
					 _T("PRINT_BARCODE_ALL = %s\r\n")
					 _T("PRINT_ITEM = %s\r\n")
					 _T("PRINT_MODE = %s\r\n")
					 _T("PRINT_DEVICE = %s\r\n")
					 _T("PRINT_PREVIEW = %s\r\n")
					 _T("PRINT_NOTIFY = %s\r\n"),
					 f_strPrINTBarcode, 
					 f_strPrINTBarcodeAll, 
					 f_strPrINTItem, 
					 f_strPrINTMode,
					 f_strDeviceType,
					 f_strPreview,
					 f_strNotify );
	file.WriteString(strConfig);
	file.Close();

	return 0;
}

VOID CLonaManageValue::GetReceiptAttestation(CString &Value)
{
	// �����ڵ� 
	CStdioFile file ;		
	CString strTemp;		
	CString strLine;
	INT		ids;

	m_sAttestation = _T("");
	strTemp = _T("..\\cfg\\ReceiptPrINTCode.cfg") ;	
	if(!file.Open (strTemp,CFile::modeRead | CFile::typeBinary)) 
	{
		m_sAttestation = _T("N");
		return;
	}
		TCHAR cUni;
		file.Read(&cUni, sizeof(TCHAR));
		if(0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
	
	ids = file.ReadString (strLine);
	if(ids) m_sAttestation = strLine;
	if(m_sAttestation == _T("N") || m_sAttestation.IsEmpty())
	{
		m_sAttestation = _T("N");
		Value = _T("N");
	}
	file.Close();
}


HBRUSH CLonaManageValue::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	
	return hbr; 
}

BOOL CLonaManageValue::GetLibDMValue(CString strLibCode, CString strAlias)
{
	EFS_BEGIN
	
	INT i = 0, nRowCnt = 0;
	CString strLibCodeTemp, strValue;
	CESL_DataMgr* pLibDM = FindDM(_T("DM_BO_LOC_3100_LIB_INFO"));
	
	nRowCnt = pLibDM->GetRowCount();
	for ( i=0; i<nRowCnt; i++ ) {
		pLibDM->GetCellData( _T("��������ȣ") , i , strLibCodeTemp );		
		
		if ( strLibCode.Compare(strLibCodeTemp) == 0 ) {
			if(strAlias.IsEmpty() == TRUE){
				return TRUE;
			}
			strValue = pLibDM->GetCellData(strAlias, i);
			if(strValue.Compare(_T("Y")) == 0)
				return TRUE;
			break;
		}
	}	

	return FALSE;
	EFS_END
		return FALSE;
}

VOID CLonaManageValue::GetLibDMValue(CString strLibCode, CString strAlias, CString &strValue)
{
	EFS_BEGIN
	
	INT i = 0, nRowCnt = 0;
	CString strLibCodeTemp;
	CESL_DataMgr* pLibDM = FindDM(_T("DM_BO_LOC_3100_LIB_INFO"));
	
	nRowCnt = pLibDM->GetRowCount();
	for ( i=0; i<nRowCnt; i++ ) {
		pLibDM->GetCellData( _T("��������ȣ") , i , strLibCodeTemp );		
		
		if ( strLibCode.Compare(strLibCodeTemp) == 0 ) {
			if(strAlias.IsEmpty() == TRUE){
				return ;
			}
			strValue = pLibDM->GetCellData(strAlias, i);
			break;
		}
	}	

	return ;
	EFS_END
	return;
}

