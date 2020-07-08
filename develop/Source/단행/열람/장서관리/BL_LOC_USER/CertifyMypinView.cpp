#include "Stdafx.h"
#include "CertifyMypinView.h"
#include "CertifyApi.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CERTIFY_RESULT_TYPE_SYS		0
#define CERTIFY_RESULT_TYPE_MSG		1
#define CERTIFY_RESULT_TYPE_CODE	2




CCertifyMypinView::CCertifyMypinView(CESL_Mgr* pParent /*=NULL*/)
	: CCertifyView(CCertifyMypinView::IDD, pParent)
{
	m_pBirthCtrl = NULL;
}

CCertifyMypinView::~CCertifyMypinView()
{
	if (m_pBirthCtrl)
	{
		delete m_pBirthCtrl;
		m_pBirthCtrl = NULL;
	}
}

void CCertifyMypinView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCertifyMypinView)
	DDX_Control(pDX, IDC_EDT_NAME, m_edtName);
	DDX_Control(pDX, IDC_RAD_MAN, m_radGenderMan);
	DDX_Control(pDX, IDC_RAD_WOMAN, m_radGenderWoman);
	DDX_Control(pDX, IDC_EDT_MYPIN_NUMBER, m_edtMypinNumber);
	 //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCertifyMypinView, CCertifyView)
	//{{AFX_MSG_MAP(CCertifyMypinView)
	//}}AFX_MSG_MAP		
END_MESSAGE_MAP()




BOOL CCertifyMypinView::PreTranslateMessage(MSG* pMsg) 
{	
	return CCertifyView::PreTranslateMessage(pMsg);
}

BOOL CCertifyMypinView::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	
	AddColorControl(IDC_EDT_NAME ,_T("") ,_T(""));
	AddColorControl(IDC_EDT_MYPIN_NUMBER ,_T("") ,_T(""));

	
	m_pBirthCtrl = new CECO_ControlDateEdit(NULL);
	m_pBirthCtrl->SubclassDlgItem(IDC_EDT_BIRTHDATE, this);
	m_pBirthCtrl->InitEdit();
	m_pBirthCtrl->ECO_SetReadOnly(FALSE);
	
	m_edtMypinNumber.SetLimitText(13);

	
	CString strName = GetUserName();
	if (!strName.IsEmpty())
	{
		m_edtName.SetWindowText(strName);
	}
	CString strBirthDay = GetBirthDay();
	if (!strBirthDay.IsEmpty())
	{
		m_pBirthCtrl->SetData(strBirthDay);
	}

	CString strGender = GetCertifyGender();
	(strGender.CompareNoCase(_T("1")) == 0) ? m_radGenderWoman.SetCheck(TRUE) : m_radGenderMan.SetCheck(TRUE);
	
	return TRUE; 
}

BOOL CCertifyMypinView::Certify(CString &strMsg)
{
	CString strName;
	CString strGender;
	CString strBirthDay;
	CString strMypinNumber;
	
	
	m_edtName.GetWindowText(strName);
	m_pBirthCtrl->GetData(strBirthDay);
	strGender = m_radGenderMan.GetCheck() ? _T("0") : _T("1");
	m_edtMypinNumber.GetWindowText(strMypinNumber);

	if (strName.IsEmpty())
	{
		strMsg.Format(_T("�̸��� �Է����ּ���."));
		m_edtName.SetFocus();
		return FALSE;
	}
	if (strMypinNumber.IsEmpty())
	{
		strMsg.Format(_T("My-PIN��ȣ�� �Է����ּ���."));
		m_edtMypinNumber.SetFocus();
		return FALSE;
	}

	
	CCertifyFrameWriter Writer(m_pInfo);
	
	Writer.setCommand(_T("MW_SERVICE_B02"));
	Writer.addRecord();
	Writer.addElement(_T("��������ȣ"), m_pInfo->LIB_CODE);
	Writer.addElement(_T("�̸�"), strName);
	Writer.addElement(_T("MYPIN��ȣ"), strMypinNumber);

	
	if (!strBirthDay.IsEmpty())
	{
		CString strBirthDayReplace = strBirthDay;
		strBirthDayReplace.Replace(_T("/"), _T(""));
		Writer.addElement(_T("�������"), strBirthDayReplace);
	}

	CCertifyFrameReader Reader;
	CCertifyFrameSender Sender(m_pInfo);
	INT ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids)
	{
		strMsg.Format(_T("%s"), MakeResultMsg(CERTIFY_RESULT_TYPE_SYS, ids, _T("")));
		return FALSE;
	}

	
	CESL_DataMgr TempDM;
	TempDM.SetCONNECTION_INFO(_T(""));

	Reader.makeDM(&TempDM);

	CString strResult = Reader.getCommand();
	if (strResult.CompareNoCase(_T("ERROR")) == 0)
	{
		strMsg.Format(_T("%s"), MakeResultMsg(CERTIFY_RESULT_TYPE_MSG, 0, TempDM.GetCellData(_T("MSG"), 0)));
		return FALSE;
	}

	
	SetUserName(strName);
	SetCertifyGender(strGender);
	SetBirthDay(strBirthDay);

	TempDM.GetCellData(_T("ó�����"), 0, strResult);
	if (strResult.CompareNoCase(_T("OK")) != 0)
	{
		strMsg.Format(_T("%s"), MakeResultMsg(CERTIFY_RESULT_TYPE_CODE, _ttoi(strResult), _T("")));

		if (GetUserMode() == 1 && m_pInfo->m_bKLUse && GetKLMemberYN() && !GetCertifyCI().IsEmpty())
		{
			
		}
		else
		{
			SetCertifyFailCode(strResult);
		}

		return FALSE;
	}

	CString strCI;
	TempDM.GetCellData(_T("�����ڵ�"), 0, strCI);
	if (strCI.IsEmpty())
	{
		strMsg.Format(_T("%s"), MakeResultMsg(CERTIFY_RESULT_TYPE_MSG, 0, _T("���������ڵ� ����")));		
		return FALSE;
	}

	
	
	SetCertifyCI(strCI);
	
	
	CString strCertifyDate;
	CLocCommonAPI::GetCurrentDateTime(this, strCertifyDate);
	SetCertifyDate(strCertifyDate);
	
	SetCertifyWorker(m_pInfo->USER_ID);
	

	return TRUE;
}

CString CCertifyMypinView::MakeResultMsg(INT nMode, INT nCode, CString strExtraMsg)
{
	CString strMsg;

	switch (nMode)
	{
	case CERTIFY_RESULT_TYPE_SYS:
		strMsg.Format(_T("�̵������� ��ſ� �����Ͽ����ϴ�.[%d]"), nCode);
		break;
	case CERTIFY_RESULT_TYPE_MSG:
		strMsg.Format(_T("%s"), strExtraMsg);
		break;
	case CERTIFY_RESULT_TYPE_CODE:
		{
			switch (nCode)
			{

			case 1:		strMsg = _T("I-PIN�� �߱��� �޾����� My-PIN�� ������ ����");	break;
			case 2:		strMsg = _T("���Ե��� ���� �̿����Դϴ�");	break;
			case 3:		strMsg = _T("I-PIN�� �����Ǿ� ����� �� ���� ����");	break;
			case 4:		strMsg = _T("�Է��� ����(�̸�, �������)�� ���� ���� ����");	break;
			case 5:		strMsg = _T("My-PIN�� �����ϳ� ��ȿ�Ⱓ�� ���� ����");	break;
			case 6:		strMsg = _T("�ý��� �����Դϴ�");	break;
			case 11:	strMsg = _T("��ſ��� �����Դϴ�");	break;
			case 12:	strMsg = _T("�ɹ�ȣ ���� �����Դϴ�");	break;
			case 13:	strMsg = _T("���� ������ �����Դϴ�");	break;
			case 14:	strMsg = _T("����Ʈ ID �����Դϴ�");	break;
			case 15:	strMsg = _T("SEED ��,��ũ��Ʈ �����Դϴ�");	break;
			case 16:	strMsg = _T("BASE64 ��,��ũ��Ʈ �����Դϴ�");	break;
			case 17:	strMsg = _T("�Է°� �����Դϴ�");	break;
			case 18:	strMsg = _T("��ϵ��� ���� IP�Դϴ�");	break;
			default:	strMsg = _T("�˼� ���� �����Դϴ�");	break;
			}
		}
		break;
	}

	return strMsg;
}

