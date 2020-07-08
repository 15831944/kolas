// BO_LOC_3823.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3823.h"
#include "BO_LOC_38201.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3823 dialog


CBO_LOC_3823::CBO_LOC_3823(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_pCBOLOC3820 = pParent;

	//{{AFX_DATA_INIT(CBO_LOC_3823)
	m_sBatchWorkName = _T("");
	m_sMailClass = _T("");
	m_strSMS = _T("");
	m_bSMS = FALSE;
	// KOL.UDF.022	
	m_nMailClass = 0;
	m_nMailKey = 0;
	m_pCBOLOC3820 = NULL;
	m_sTime = _T("");
	m_sLibName = _T("");
	
	//}}AFX_DATA_INIT
}

// �߰� : DeleteObject���� �Ҹ��� �߰� ��� [2008-05-15]
CBO_LOC_3823::~CBO_LOC_3823()
{
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]
}

VOID CBO_LOC_3823::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3823)
	DDX_Text(pDX, IDC_eBATCH_WORK_NAME, m_sBatchWorkName);
	DDX_Text(pDX, IDC_eMAILNAME, m_sMailClass);
	DDX_Text(pDX, IDC_eSMS, m_strSMS);
	DDV_MaxChars(pDX, m_strSMS, 80);
	DDX_Check(pDX, IDC_cSMS, m_bSMS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3823, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3823)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3823 message handlers

BOOL CBO_LOC_3823::Create(CWnd* pParentWnd)
{
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_LOC_3823::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// �߰� : ���̾�α�&����ƽ ������ ��� [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	INT ids;

	ids = InitESL_Mgr(_T("SM_BO_LOC_3823"));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitESL_Mgr") );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_LOC_3823::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
}

BOOL CBO_LOC_3823::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		OnOK();
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CBO_LOC_3823::OnOK() 
{
	MailSendProc();
	
	//CDialog::OnOK();
}

INT CBO_LOC_3823::MailSendProc()
{
	EFS_BEGIN

	INT ids=0;

	// 1. ������ �̸� ��������.
	ids = GetLibName();
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MailSendProc()") );

	// 2. 
	UpdateData(TRUE);
	if(m_nMailClass == 10 || m_nMailClass == 11 || ( m_nMailClass >= 0 && m_nMailClass <= 7)){
		MakeDM_LOAN();
	}else{
		if(((CBO_LOC_3820*)m_pCBOLOC3820)->nDirectCount > 0 && m_nMailClass!=13){
			MakeDM_USER_DIRECT();
		}else{
			MakeDM_USER();
		}
	}

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3823::GetLibName()
{
	EFS_BEGIN

	CString sLibInfo[1] =
	{
		_T("LIB_NAME")
	};
	CLocCommonAPI::GetLibInfo( sLibInfo , sLibInfo , 1 , this);
	m_sLibName = sLibInfo[0];

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3823::MakeDM_USER()
{
	EFS_BEGIN
	INT ids=0, i=0;
	INT nRowCnt=0, nRowCntDst=0;
	CString sTemp, sTemp2;
	CString pk;
	INT nGridPollIndex=0;

	CESL_DataMgr *pDMDst = FindDM( _T("DM_BO_LOC_3823") );
	if(pDMDst==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3823::MakeDM_USER()") );
	
	CESL_DataMgr *pDMUser = FindDM( _T("DM_BO_LOC_3820_USER") );
	if(pDMUser==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3823::MakeDM_USER()") );

	CESL_DataMgr *pDMBO = FindDM( _T("DM_BO_LOC_3820_BO_BOOK") );
	if(pDMBO==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3823::MakeDM_USER()") );

	CESL_DataMgr *pDMSE = FindDM( _T("DM_BO_LOC_3820_SE_BOOK") );
	if(pDMSE==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CBO_LOC_3823::MakeDM_USER()") );

	CESL_DataMgr *pDMFurnish = FindDM( _T("DM_BO_LOC_3820_FURNISH") );
	if(pDMFurnish==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBO_LOC_3823::MakeDM_USER()") );

	CESL_DataMgr *pDMPoll = FindDM( _T("DM_BO_LOC_3820_POLL") );
	if(pDMPoll==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -12 , _T("CBO_LOC_3823::MakeDM_USER()") );
	
	CESL_DataMgr *pDMNonUser = FindDM( _T("DM_BO_LOC_3820_NONUSER") );
	if(pDMNonUser==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -12 , _T("CBO_LOC_3823::MakeDM_USER()") );

	CESL_ControlMgr *pCM = FindCM(_T("CM_BO_LOC_3820"));
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CBO_LOC_3823::MakeDM_USER()") );

	CESL_Grid *pGridUser = (CESL_Grid*)(pCM->FindControl(_T("UserGrid")));
	if(pGridUser == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CBO_LOC_3823::MakeDM_USER()") );

	CESL_Grid *pGridBO = (CESL_Grid*)(pCM->FindControl(_T("BoBookGrid")));
	if(pGridBO == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CBO_LOC_3823::MakeDM_USER()") );

	CESL_Grid *pGridSE = (CESL_Grid*)(pCM->FindControl(_T("SeBookGrid")));
	if(pGridSE == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -19 , _T("CBO_LOC_3823::MakeDM_USER()") );

	CESL_Grid *pGridFurnish = (CESL_Grid*)(pCM->FindControl(_T("FurnishGrid")));
	if(pGridFurnish == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("CBO_LOC_3823::MakeDM_USER()") );

	CESL_Grid *pGridPoll = (CESL_Grid*)(pCM->FindControl(_T("PollGrid")));
	if(pGridPoll == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -22 , _T("CBO_LOC_3823::MakeDM_USER()") );

	CESL_Grid *pGridNonUser = (CESL_Grid*)(pCM->FindControl(_T("NonUserGrid")));
	if(pGridNonUser == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -23 , _T("CBO_LOC_3823::MakeDM_USER()") );


	// 1. �߼۴���ڰ� �׸��忡 �˻��Ǿ����� �˻�.
	if(m_nMailClass==13){//��ġ���
		nRowCnt = pDMFurnish->GetRowCount();
	}else if(m_nMailClass==17){//��ȸ������
		nRowCnt = pDMNonUser->GetRowCount();
	}else{
		nRowCnt = pDMUser->GetRowCount();
	}
	if(nRowCnt < 1){
		AfxMessageBox(_T("���Ϲ߼� ����ڰ� �����ϴ�."));
		return 0;
	}

	// 2. ������ ����� �ִ��� �˻�.
	if(m_nMailClass==13){
		pGridFurnish->SelectMakeList();
		nRowCntDst = pGridFurnish->SelectGetCount();
	}else if(m_nMailClass==17){
		pGridNonUser->SelectMakeList();
		nRowCntDst = pGridNonUser->SelectGetCount();
	}else{
		pGridUser->SelectMakeList();
		nRowCntDst = pGridUser->SelectGetCount();
	}	
	if(nRowCntDst < 1){
		AfxMessageBox(_T("������ ����ڰ� �����ϴ�."));
		return 0;
	}

	CString sBookKeyList=_T("");
	if(m_nMailClass == 12){//���� �Ǵ� ��õ ����
		// 3. ���� BOOK_KEY_LIST �� ��´�.	
		pGridBO->SelectMakeList();
		for(i=0;i<pDMBO->GetRowCount();i++){
			sTemp = pGridBO->GetTextMatrix(i+1, 1);
			if(sTemp == _T("V")){
				pDMBO->GetCellData(_T("åKEY"), i, sTemp);
				if(sBookKeyList.IsEmpty()){
					sBookKeyList = sTemp;
				}else{
					sBookKeyList += _T(",");
					sBookKeyList += sTemp;
				}
			}
		}

		// 3. ���� BOOK_KEY_LIST �� ��´�.
		pGridSE->SelectMakeList();
		for(i=0;i<pDMSE->GetRowCount();i++){
			sTemp = pGridSE->GetTextMatrix(i+1, 1);
			if(sTemp == _T("V")){
				pDMSE->GetCellData(_T("åKEY"), i, sTemp);
				if(sBookKeyList.IsEmpty()){
					sBookKeyList = sTemp;
				}else{
					sBookKeyList += _T(",");
					sBookKeyList += sTemp;
				}
			}
		}

		if(sBookKeyList.IsEmpty()){
			AfxMessageBox(_T("�˻��� å������ �����ϴ�."));
			return 0;
		}

		if(sBookKeyList.GetLength() > 3000){
			AfxMessageBox(_T("�����Ͻ� å�� ���� �ʹ� �����ϴ�."));
			return 0;
		}
	}


	// 4. ��������
	CString sPollKey=_T("");
	if(m_nMailClass == 16){//��������
		pGridPoll->SelectMakeList();
		if(pGridPoll->SelectGetCount() < 1 ){
			AfxMessageBox(_T("�����Ͻ� �������� �����ϴ�."));
			return 0;
		}else if(pGridPoll->SelectGetCount() > 1 ){
			AfxMessageBox(_T("�������� �Ѱ��� ���� �� �ֽ��ϴ�."));
			return 0;
		}

		for(i=0;i<pDMPoll->GetRowCount();i++){
			sTemp = pGridPoll->GetTextMatrix(i+1, 1);
			if(sTemp == _T("V")){
				pDMPoll->GetCellData(_T("�����߼���"), i, sTemp);
				if(!sTemp.IsEmpty()){
					sTemp2.Format(_T("�����Ͻ� �������� %s �Ͽ� �̹� �߼��Ͽ����ϴ�. ��߼��Ͻðڽ��ϱ�?"), sTemp.GetBuffer(0));
					if(MessageBox(sTemp2, _T("Ȯ��"), MB_YESNO|MB_ICONQUESTION ) == IDNO ){
						return 0;
					}
				}
				nGridPollIndex = i;
				pDMPoll->GetCellData(_T("REC_KEY"), i, sTemp);
				sPollKey = sTemp;
				break;
			}
		}
	}


	// 5. ���α׷����� ����
	CProgressCtrl *pProgressCtrl;
	pProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_pProgress);
	if(pProgressCtrl != NULL){
		pProgressCtrl->SetRange(0,nRowCntDst+3);
	}
	pProgressCtrl->SetPos(0);


	// 6. insert into mn_mail_temp_tbl
	pDMDst->FreeData();
	pDMDst->StartFrame();
	nRowCntDst = 0;
	for(i=0;i<nRowCnt;i++){
		if(m_nMailClass == 13){
			sTemp = pGridFurnish->GetTextMatrix(i+1, 1);
		}else if(m_nMailClass == 17){
			sTemp = pGridNonUser->GetTextMatrix(i+1, 1);
		}else{
			sTemp = pGridUser->GetTextMatrix(i+1, 1);
		}
		
		if(sTemp == _T("V")){
			pDMDst->MakeRecKey(pk , _T("ESL_MAIL_SEQ"));
			pDMDst->InitDBFieldData();

			// 1. REC_KEY
			pDMDst->AddDBFieldData( _T("REC_KEY"), _T("NUMERIC"), pk );

			// 2. USER_KEY
			if(m_nMailClass!=13 && m_nMailClass!=17){
				pDMUser->GetCellData(_T("������KEY"), i, sTemp);
				pDMDst->AddDBFieldData( _T("USER_KEY"), _T("NUMERIC"), sTemp );
			}			

			// 3. LIB_NAME
			pDMDst->AddDBFieldData( _T("LIB_NAME"), _T("STRING"), m_sLibName );

			// 4. BATCH_WORK_NAME
			pDMDst->AddDBFieldData( _T("BATCH_WORK_NAME"), _T("STRING"), m_sBatchWorkName );

			// 5. MAIL_CLASS
			sTemp.Format(_T("%d"), m_nMailClass);
			pDMDst->AddDBFieldData( _T("MAIL_CLASS"), _T("STRING"), sTemp );

			// 6. MAIL_KEY
			sTemp.Format(_T("%d"), m_nMailKey);
			pDMDst->AddDBFieldData( _T("MAIL_KEY"), _T("NUMERIC"), sTemp );

			// 7. BOOK_KEY_LIST
			if(m_nMailClass==13){
				pDMFurnish->GetCellData(_T("åKEY"), i, sTemp);
				pDMDst->AddDBFieldData( _T("BOOK_KEY_LIST"), _T("STRING"), sTemp );
			}else if(m_nMailClass==12){
				pDMDst->AddDBFieldData( _T("BOOK_KEY_LIST"), _T("STRING"), sBookKeyList );
			}

			// 8. FURNISH_KEY
			if(m_nMailClass==13){
				pDMFurnish->GetCellData(_T("REC_KEY"), i, sTemp);
				pDMDst->AddDBFieldData( _T("FURNISH_KEY"), _T("NUMERIC"), sTemp );
			}

			// 9. POLL_KEY
			if(m_nMailClass==16){
				pDMDst->AddDBFieldData( _T("POLL_KEY"), _T("NUMERIC"), sPollKey );
			}

			// 10. NONUSER_KEY
			if(m_nMailClass==17){
				pDMNonUser->GetCellData(_T("��ȸ��Ű"), i, sTemp);
				pDMDst->AddDBFieldData( _T("NONUSER_KEY"), _T("NUMERIC"), sTemp );
			}

			// 2009.03.09 ADD BY CJY : �������۽� �̷°����� ���� �������� �׸� �߰�
			pDMDst->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), m_pInfo->MANAGE_CODE);

			pDMDst->MakeInsertFrame(_T("MN_MAIL_TEMP_TBL"));

			pProgressCtrl->SetPos(nRowCntDst);
			nRowCntDst++;
		}
	}

	ids = pDMDst->SendFrame();
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -33 , _T("CBO_LOC_3823::MakeDM_USER()") );
	pDMDst->EndFrame();

	// �����߼��� ������Ʈ�ϱ�
	if(m_nMailClass==16 && !sPollKey.IsEmpty()){
		sTemp.Format(_T("update research_info_tbl set send_date = to_char(sysdate, 'YYYY/MM/DD') where rec_key = %s;"), sPollKey);
		pDMDst->ExecuteQuery(sTemp , TRUE );
		if(!m_sTime.IsEmpty()){
			sTemp.Format(_T("%s"), m_sTime.GetBufferSetLength(10));
			pDMPoll->SetCellData(_T("�����߼���"), sTemp, nGridPollIndex );
			pGridPoll->DisplayLine(nGridPollIndex);
			pGridPoll->SetTextMatrix(nGridPollIndex+1 , 1 , _T("V"));
		}
	}

	// SMS �߼��ϱ�.
	CString sTable;
	if(m_bSMS && !m_strSMS.IsEmpty()){
		INT cnt=0;
		CString sql=_T("");
		CString user_key;
		CString sms_type;
		CString handphone;
		for(i=0;i<nRowCnt;i++){
			if(m_nMailClass == 13){/* ��ġ����ȳ� */
				sTemp = pGridFurnish->GetTextMatrix(i+1, 1);
			}else if(m_nMailClass == 17){/* ��ġ����ȳ� */
				sTemp = pGridNonUser->GetTextMatrix(i+1, 1);
			}else{
				sTemp = pGridUser->GetTextMatrix(i+1, 1);
			}

			if(sTemp == _T("V")){
				if(m_nMailClass == 8){/* �ű�ȸ���ȳ� */
					pDMUser->GetCellData(_T("������KEY"), i, user_key);
					pDMUser->GetCellData(_T("�ڵ���"), i, handphone);
					sms_type = _T("SMS09");
				}else if(m_nMailClass == 12){/* �����ڷ�ȳ� */
					pDMUser->GetCellData(_T("������KEY"), i, user_key);
					pDMUser->GetCellData(_T("�ڵ���"), i, handphone);
					sms_type = _T("SMS07");
				}else if(m_nMailClass == 13){/* ��ġ����ȳ� */
					pDMFurnish->GetCellData(_T("REC_KEY"), i, user_key);
					pDMFurnish->GetCellData(_T("�ڵ���"), i, handphone);
					sms_type = _T("SMS04");
				}else if(m_nMailClass == 14){/* �������ϸ��� */
					pDMUser->GetCellData(_T("������KEY"), i, user_key);
					pDMUser->GetCellData(_T("�ڵ���"), i, handphone);
					sms_type = _T("SMS09");
				}else if(m_nMailClass == 15){/* �������� */
					pDMUser->GetCellData(_T("������KEY"), i, user_key);
					pDMUser->GetCellData(_T("�ڵ���"), i, handphone);
					sms_type = _T("SMS05");
				}else if(m_nMailClass == 16){/* �������� */
					pDMUser->GetCellData(_T("������KEY"), i, user_key);
					pDMUser->GetCellData(_T("�ڵ���"), i, handphone);
					sms_type = _T("SMS09");
				}else if(m_nMailClass == 17){/* ��ȸ������ */
					pDMNonUser->GetCellData(_T("��ȸ��Ű"), i, user_key);
					pDMNonUser->GetCellData(_T("�ڵ���"), i, handphone);
					sms_type = _T("SMS09");
				}else{
					pDMUser->GetCellData(_T("������KEY"), i, user_key);
					pDMUser->GetCellData(_T("�ڵ���"), i, handphone);
					sms_type = _T("SMS09");
				}

				if(handphone.IsEmpty()) continue;
			
				
				if(m_nMailClass == 13){
					sTable = _T("BO_FURNISH_TBL");
				}else if(m_nMailClass == 17){
					sTable = _T("CO_NONMEMBER_TBL");
				}else{
					sTable = _T("CO_LOAN_USER_TBL");
				}

				// 2011.09.07 ADD : 3000Api
				if(TRUE == m_pInfo->m_bLibSecuUse)
				{
					if(_T("BO_FURNISH_TBL") == sTable || _T("CO_LOAN_USER_TBL") == sTable)
					{
						sTemp.Format(_T("insert into sms_send_temp_tbl ( rec_key, name, handphone, msg, user_key, title, sms_type, mail_class ) select esl_mail_seq.nextval, name, '%s', '%s', rec_key, '%s', '%s' , '%d' from %s where handphone is not null and rec_key = %s ;"), 
							handphone, m_strSMS.GetBuffer(0), m_sBatchWorkName.GetBuffer(0), sms_type.GetBuffer(0), m_nMailClass , sTable.GetBuffer(0), user_key.GetBuffer(0));
					}
					else
					{
						sTemp.Format(_T("insert into sms_send_temp_tbl ( rec_key, name, handphone, msg, user_key, title, sms_type, mail_class ) select esl_mail_seq.nextval, name, handphone, '%s', rec_key, '%s', '%s' , '%d' from %s where handphone is not null and rec_key = %s ;"), 
							m_strSMS.GetBuffer(0), m_sBatchWorkName.GetBuffer(0), sms_type.GetBuffer(0), m_nMailClass , sTable.GetBuffer(0), user_key.GetBuffer(0));
					}
				}
				else
				{
					sTemp.Format(_T("insert into sms_send_temp_tbl ( rec_key, name, handphone, msg, user_key, title, sms_type, mail_class ) select esl_mail_seq.nextval, name, handphone, '%s', rec_key, '%s', '%s' , '%d' from %s where handphone is not null and rec_key = %s ;"), 
						m_strSMS.GetBuffer(0), m_sBatchWorkName.GetBuffer(0), sms_type.GetBuffer(0), m_nMailClass , sTable.GetBuffer(0), user_key.GetBuffer(0));
				}
// 				sTemp.Format(_T("insert into sms_send_temp_tbl ( rec_key, name, handphone, msg, user_key, title, sms_type, mail_class ) select esl_mail_seq.nextval, name, handphone, '%s', rec_key, '%s', '%s' , '%d' from %s where handphone is not null and rec_key = %s ;"), 
// 					m_strSMS.GetBuffer(0), m_sBatchWorkName.GetBuffer(0), sms_type.GetBuffer(0), m_nMailClass , sTable.GetBuffer(0), user_key.GetBuffer(0));
				

				sql += sTemp;
				cnt++;

				if(cnt == 20){
					ids = pDMDst->ExecuteQuery(sql , TRUE );
					sql.Empty();
					cnt = 0;
				}
			}
		}
		if(cnt > 0) ids = pDMDst->ExecuteQuery(sql , TRUE );
	}

	pProgressCtrl->SetPos(nRowCntDst+3);

	sTemp.Format(_T("%d ���� ������ ���������Կ� �־����ϴ�."), nRowCntDst);
	AfxMessageBox(sTemp);

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3823::MakeDM_LOAN()
{
	EFS_BEGIN

	INT ids=0, i;
	INT nRowCnt=0,nRowCntDst=0;
	CString sTemp, sTemp2;
	CString pk;

	CESL_DataMgr *pDMDst = FindDM( _T("DM_BO_LOC_3823") );
	if(pDMDst==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3823::MakeDM_LOAN()") );
	
	CESL_ControlMgr *pCM = FindCM(_T("CM_BO_LOC_3820"));
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3823::MakeDM_LOAN()") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(_T("LoanGrid")));
	if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3823::MakeDM_LOAN()") );
	
	CESL_DataMgr *pDMSrc = FindDM( _T("DM_BO_LOC_3820") );
	if(pDMSrc==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CBO_LOC_3823::MakeDM_LOAN()") );
	
	nRowCnt = pDMSrc->GetRowCount();
	if(nRowCnt < 1){
		AfxMessageBox(_T("���Ϲ߼� ����ڰ� �����ϴ�."));
		return 0;
	}

	pDMDst->StartFrame();

	// 1. ��������Ʈ���� ������ �Ǽ� �����´�.
	pGrid->SelectMakeList();
	nRowCntDst = pGrid->SelectGetCount();
	if(nRowCntDst < 1){
		AfxMessageBox(_T("������ ����ڰ� �����ϴ�."));
		return 0;
	}


	// 2. �����ϱ� ���� DM �� ����� ������ �͸� DM�� �����Ѵ�.
	CESL_DataMgr TmpDM;
	ids = CLocCommonAPI::MakeCopyDM( this , pDMSrc , &TmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CBO_LOC_3823::MakeDM_LOAN()") );

	TmpDM.FreeData();
	nRowCntDst = 0;

	for(i=0;i<nRowCnt;i++){
		sTemp = pGrid->GetTextMatrix(i+1, 1);
		if(sTemp == _T("V")){
			TmpDM.InsertRow(-1);
			ids = CLocCommonAPI::AddDM(pDMSrc, &TmpDM, i, nRowCntDst, this );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CBO_LOC_3823::MakeDM_LOAN()") );
			nRowCntDst++;
		}		
	}

	if(nRowCntDst < 1){
		AfxMessageBox(_T("���Ϲ߼� ����ڰ� �����ϴ�."));
		return 0;
	}

	// 3. �����Ѵ�.
	CString sDateAlias=_T("");
	if(m_nMailClass==6 ){
		sDateAlias = _T("���������");
		CString sDMAlias[2] = {_T("�����ڹ�ȣ"), _T("���������")};
		INT nSortCond[2] = { ESL_DATAMGR_SORT_ASC , ESL_DATAMGR_SORT_ASC };
		TmpDM.SORT(2, sDMAlias, nSortCond, ESL_DATAMGR_SORT_ASC );
	}else if( m_nMailClass==2 || m_nMailClass==7 || m_nMailClass==10 || m_nMailClass==11){
		sDateAlias = _T("�ݳ�������");
		CString sDMAlias[2] = {_T("�����ڹ�ȣ"), _T("�ݳ�������")};
		INT nSortCond[2] = { ESL_DATAMGR_SORT_ASC , ESL_DATAMGR_SORT_ASC };
		TmpDM.SORT(2, sDMAlias, nSortCond, ESL_DATAMGR_SORT_ASC );
	}else if(m_nMailClass==3){
		sDateAlias = _T("������");
		CString sDMAlias[2] = {_T("�����ڹ�ȣ"), _T("������")};
		INT nSortCond[2] = { ESL_DATAMGR_SORT_ASC , ESL_DATAMGR_SORT_ASC };
		TmpDM.SORT(2, sDMAlias, nSortCond, ESL_DATAMGR_SORT_ASC );
	}else if(m_nMailClass==4 || m_nMailClass==5){
		sDateAlias = _T("�ݳ���");
		CString sDMAlias[2] = {_T("�����ڹ�ȣ"), _T("�ݳ���")};
		INT nSortCond[2] = { ESL_DATAMGR_SORT_ASC , ESL_DATAMGR_SORT_ASC };
		TmpDM.SORT(2, sDMAlias, nSortCond, ESL_DATAMGR_SORT_ASC );
	}

	INT nRowSendCnt = 0;

	CString sPreUserNo = _T("");
	CString sCurrUserNo = _T("");
	CString sPreDate = _T("");
	CString sCurrDate = _T("");

	pDMDst->FreeData();

	// 4. �׷����ϸ鼭 DM�� �ִ´�.
	for(i=0;i<nRowCntDst;i++){

		if(m_nMailClass==0 || m_nMailClass==1 ){
			pDMDst->InsertRow(-1);
				
			TmpDM.GetCellData(_T("������KEY"), i, sTemp);
			pDMDst->SetCellData(_T("USER_KEY"), sTemp, nRowSendCnt);

			TmpDM.GetCellData(_T("�ڵ���"), i, sTemp);
			pDMDst->SetCellData(_T("HANDPHONE"), sTemp, nRowSendCnt);

			TmpDM.GetCellData(_T("����ݳ�KEY"), i, sTemp);
			pDMDst->SetCellData(_T("LOAN_KEY_LIST"), sTemp, nRowSendCnt);

			nRowSendCnt++;
		}else{
			TmpDM.GetCellData(_T("�����ڹ�ȣ"), i, sCurrUserNo);
			TmpDM.GetCellData(sDateAlias, i, sCurrDate);

			if(sCurrUserNo != sPreUserNo){
				pDMDst->InsertRow(-1);
				
				TmpDM.GetCellData(_T("������KEY"), i, sTemp);
				pDMDst->SetCellData(_T("USER_KEY"), sTemp, nRowSendCnt);

				TmpDM.GetCellData(_T("�ڵ���"), i, sTemp);
				pDMDst->SetCellData(_T("HANDPHONE"), sTemp, nRowSendCnt);

				TmpDM.GetCellData(_T("����ݳ�KEY"), i, sTemp);
				pDMDst->SetCellData(_T("LOAN_KEY_LIST"), sTemp, nRowSendCnt);

				nRowSendCnt++;
			}else{
				if(sCurrDate != sPreDate){				
					pDMDst->InsertRow(-1);
					
					TmpDM.GetCellData(_T("������KEY"), i, sTemp);
					pDMDst->SetCellData(_T("USER_KEY"), sTemp, nRowSendCnt);

					TmpDM.GetCellData(_T("�ڵ���"), i, sTemp);
					pDMDst->SetCellData(_T("HANDPHONE"), sTemp, nRowSendCnt);

					TmpDM.GetCellData(_T("����ݳ�KEY"), i, sTemp);
					pDMDst->SetCellData(_T("LOAN_KEY_LIST"), sTemp, nRowSendCnt);

					nRowSendCnt++;
				}else{
					pDMDst->GetCellData(_T("LOAN_KEY_LIST"), nRowSendCnt-1, sTemp2);
					TmpDM.GetCellData(_T("����ݳ�KEY"), i, sTemp);
					if(sTemp2.IsEmpty()){
						pDMDst->SetCellData(_T("LOAN_KEY_LIST"), sTemp, nRowSendCnt-1);
					}else{
						sTemp2 += _T(",");
						sTemp2 += sTemp;
						pDMDst->SetCellData(_T("LOAN_KEY_LIST"), sTemp2, nRowSendCnt-1);
					}
				}
			}
			sPreUserNo = sCurrUserNo;
			sPreDate = sCurrDate;
		}
	}

	if(nRowSendCnt < 1){
		AfxMessageBox(_T("���Ϲ߼� ����ڰ� �����ϴ�."));
		return 0;
	}

	// 5. ���α׷����� ����
	CProgressCtrl *pProgressCtrl;
	pProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_pProgress);
	if(pProgressCtrl != NULL){
		pProgressCtrl->SetRange(0,nRowSendCnt+3);
	}
	pProgressCtrl->SetPos(0);
	

	// 6. mn_mail_temp_tbl �� insert �Ѵ�.
	for(i=0;i<nRowSendCnt;i++){

		pDMDst->MakeRecKey(pk , _T("ESL_MAIL_SEQ"));
		pDMDst->InitDBFieldData();

		pDMDst->AddDBFieldData( _T("REC_KEY"), _T("NUMERIC"), pk );

		pDMDst->GetCellData(_T("USER_KEY"), i , sTemp);
		pDMDst->AddDBFieldData( _T("USER_KEY"), _T("NUMERIC"), sTemp );

		pDMDst->GetCellData(_T("LOAN_KEY_LIST"), i , sTemp);
		pDMDst->AddDBFieldData( _T("LOAN_KEY_LIST"), _T("STRING"), sTemp );

		pDMDst->AddDBFieldData( _T("LIB_NAME"), _T("STRING"), m_sLibName );

		pDMDst->AddDBFieldData( _T("BATCH_WORK_NAME"), _T("STRING"), m_sBatchWorkName );

		sTemp.Format(_T("%d"), m_nMailClass);
		pDMDst->AddDBFieldData( _T("MAIL_CLASS"), _T("STRING"), sTemp );

		sTemp.Format(_T("%d"), m_nMailKey);
		pDMDst->AddDBFieldData( _T("MAIL_KEY"), _T("NUMERIC"), sTemp );

		// 2009.03.09 ADD BY CJY : �������۽� �̷°����� ���� �������� �׸� �߰�
		pDMDst->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), m_pInfo->MANAGE_CODE);

		pDMDst->MakeInsertFrame(_T("MN_MAIL_TEMP_TBL"));

		pProgressCtrl->SetPos(i);
	}

	ids = pDMDst->SendFrame();
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -33 , _T("CBO_LOC_3823::MakeDM_LOAN()") );
	pDMDst->EndFrame();


	// SMS �߼��ϱ�.
	if(m_bSMS && !m_strSMS.IsEmpty()){
		INT cnt = 0;
		CString sql=_T("");
		CString user_key;
		CString sms_type;
		CString handphone;
		for(i=0;i<nRowSendCnt;i++){
			pDMDst->GetCellData(_T("USER_KEY"), i , user_key);
			pDMDst->GetCellData(_T("HANDPHONE"), i , handphone);
			if(handphone.IsEmpty()) continue;

			if(m_nMailClass == 0 || m_nMailClass == 1){
				sms_type = _T("SMS03");
			}else if(m_nMailClass == 10){
				sms_type = _T("SMS01");
			}else if(m_nMailClass == 2 || m_nMailClass == 11){
				sms_type = _T("SMS02");
			}else{
				sms_type = _T("SMS09");
			}
			
			sTemp.Format(_T("insert into sms_send_temp_tbl ( rec_key, name, handphone, msg, user_key, title, sms_type ) select esl_mail_seq.nextval, name, handphone, '%s', rec_key, '%s', '%s' from co_loan_user_tbl where handphone is not null and rec_key = %s ;"), 
				m_strSMS.GetBuffer(0), m_sBatchWorkName.GetBuffer(0), sms_type.GetBuffer(0), user_key.GetBuffer(0));

			sql += sTemp;
			cnt++;

			if(cnt == 20){
				ids = pDMDst->ExecuteQuery(sql , TRUE );
				sql.Empty();
				cnt = 0;
			}
		}
		if(cnt > 0) ids = pDMDst->ExecuteQuery(sql , TRUE );
	}

	pProgressCtrl->SetPos(nRowSendCnt+3);

	sTemp.Format(_T("%d ���� ������ ���������Կ� �־����ϴ�."), nRowSendCnt);
	AfxMessageBox(sTemp);

	return 0;
	EFS_END
	return -1;
}

VOID CBO_LOC_3823::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	ShowInit();
}

INT CBO_LOC_3823::ShowInit()
{
	INT ids=0;

	m_sMailClass = ((CBO_LOC_3820*)m_pCBOLOC3820)->m_sMailClass;
	m_nMailClass = ((CBO_LOC_3820*)m_pCBOLOC3820)->m_nMailClass;
	m_nMailKey   = ((CBO_LOC_3820*)m_pCBOLOC3820)->m_nMailKey;
	
	m_strSMS = _T("");
	m_bSMS = FALSE;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3823") );
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3823::ShowInit()") );

	pDM->GetOneValueQuery(_T("select to_char(sysdate, 'YYYY/MM/DD-HH24:MI:SS') from dual"), m_sTime);

	m_sBatchWorkName = m_sMailClass + _T("-") + m_sTime;

	CProgressCtrl *pProgressCtrl;
	pProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_pProgress);
	if(pProgressCtrl != NULL){
		pProgressCtrl->SetRange(0,100);
	}
	pProgressCtrl->SetPos(0);

	UpdateData(FALSE);

	return 0;
}

INT CBO_LOC_3823::MakeDM_USER_DIRECT()
{
	EFS_BEGIN
	INT ids=0, i=0;
	CString sTemp, sTemp2;
	INT nGridPollIndex=0;

	CESL_DataMgr *pDMBO = FindDM( _T("DM_BO_LOC_3820_BO_BOOK") );
	if(pDMBO==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3823::MakeDM_USER()") );

	CESL_DataMgr *pDMSE = FindDM( _T("DM_BO_LOC_3820_SE_BOOK") );
	if(pDMSE==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CBO_LOC_3823::MakeDM_USER()") );

	CESL_DataMgr *pDMPoll = FindDM( _T("DM_BO_LOC_3820_POLL") );
	if(pDMPoll==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBO_LOC_3823::MakeDM_USER()") );

	CESL_ControlMgr *pCM = FindCM(_T("CM_BO_LOC_3820"));
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CBO_LOC_3823::MakeDM_USER()") );

	CESL_Grid *pGridBO = (CESL_Grid*)(pCM->FindControl(_T("BoBookGrid")));
	if(pGridBO == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CBO_LOC_3823::MakeDM_USER()") );

	CESL_Grid *pGridSE = (CESL_Grid*)(pCM->FindControl(_T("SeBookGrid")));
	if(pGridSE == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -19 , _T("CBO_LOC_3823::MakeDM_USER()") );

	CESL_Grid *pGridPoll = (CESL_Grid*)(pCM->FindControl(_T("PollGrid")));
	if(pGridPoll == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("CBO_LOC_3823::MakeDM_USER()") );



	// 1. �߼۴���ڰ� �׸��忡 �˻��Ǿ����� �˻�.

	// 2. ������ ����� �ִ��� �˻�.


	CString sBookKeyList=_T("");
	if(m_nMailClass == 12){//���� �Ǵ� ��õ ����
		// 3. ���� BOOK_KEY_LIST �� ��´�.	
		pGridBO->SelectMakeList();
		for(i=0;i<pDMBO->GetRowCount();i++){
			sTemp = pGridBO->GetTextMatrix(i+1, 1);
			if(sTemp == _T("V")){
				pDMBO->GetCellData(_T("åKEY"), i, sTemp);
				if(sBookKeyList.IsEmpty()){
					sBookKeyList = sTemp;
				}else{
					sBookKeyList += _T(",");
					sBookKeyList += sTemp;
				}
			}
		}

		// 3. ���� BOOK_KEY_LIST �� ��´�.
		pGridSE->SelectMakeList();
		for(i=0;i<pDMSE->GetRowCount();i++){
			sTemp = pGridSE->GetTextMatrix(i+1, 1);
			if(sTemp == _T("V")){
				pDMSE->GetCellData(_T("åKEY"), i, sTemp);
				if(sBookKeyList.IsEmpty()){
					sBookKeyList = sTemp;
				}else{
					sBookKeyList += _T(",");
					sBookKeyList += sTemp;
				}
			}
		}

		if(sBookKeyList.IsEmpty()){
			AfxMessageBox(_T("�˻��� å������ �����ϴ�."));
			return 0;
		}

		if(sBookKeyList.GetLength() > 3000){
			AfxMessageBox(_T("�����Ͻ� å�� ���� �ʹ� �����ϴ�."));
			return 0;
		}
	}

	// 4. ��������
	CString sPollKey=_T("");
	if(m_nMailClass == 16){//��������
		pGridPoll->SelectMakeList();
		if(pGridPoll->SelectGetCount() < 1 ){
			AfxMessageBox(_T("�����Ͻ� �������� �����ϴ�."));
			return 0;
		}else if(pGridPoll->SelectGetCount() > 1 ){
			AfxMessageBox(_T("�������� �Ѱ��� ���� �� �ֽ��ϴ�."));
			return 0;
		}

		for(i=0;i<pDMPoll->GetRowCount();i++){
			sTemp = pGridPoll->GetTextMatrix(i+1, 1);
			if(sTemp == _T("V")){
				pDMPoll->GetCellData(_T("�����߼���"), i, sTemp);
				if(!sTemp.IsEmpty()){
					sTemp2.Format(_T("�����Ͻ� �������� %s �Ͽ� �̹� �߼��Ͽ����ϴ�. ��߼��Ͻðڽ��ϱ�?"), sTemp.GetBuffer(0));
					if(MessageBox(sTemp2, _T("Ȯ��"), MB_YESNO|MB_ICONQUESTION ) == IDNO ){
						return 0;
					}
				}
				nGridPollIndex = i;
				pDMPoll->GetCellData(_T("REC_KEY"), i, sTemp);
				sPollKey = sTemp;
				break;
			}
		}
	}

	// 5. ���α׷����� ����
	CProgressCtrl *pProgressCtrl;
	pProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_pProgress);
	if(pProgressCtrl != NULL){
		pProgressCtrl->SetRange(0,100);
	}
	pProgressCtrl->SetPos(0);


	// 6. insert into mn_mail_temp_tbl
	// 2009.03.09 UPDATE BY CJY : �������۽� �̷°����� ���� �������� �׸� �߰�

	if(m_nMailClass == 16){
		sTemp.Format(_T("INSERT INTO MN_MAIL_TEMP_TBL (REC_KEY, USER_KEY, LIB_NAME, BATCH_WORK_NAME, MAIL_CLASS, MAIL_KEY, POLL_KEY, MANAGE_CODE) SELECT ESL_MAIL_SEQ.NEXTVAL, REC_KEY, '%s', '%s', '%d', '%d', %s, '%s' FROM CO_LOAN_USER_TBL U WHERE 1>0 %s; "),
				m_sLibName, m_sBatchWorkName, m_nMailClass , m_nMailKey , sPollKey , m_pInfo->MANAGE_CODE , ((CBO_LOC_3820*)m_pCBOLOC3820)->sDirectQuery);
	}else{
		sTemp.Format(_T("INSERT INTO MN_MAIL_TEMP_TBL (REC_KEY, USER_KEY, LIB_NAME, BATCH_WORK_NAME, MAIL_CLASS, MAIL_KEY, BOOK_KEY_LIST, MANAGE_CODE) SELECT ESL_MAIL_SEQ.NEXTVAL, REC_KEY, '%s', '%s', '%d', '%d', '%s', '%s' FROM CO_LOAN_USER_TBL U WHERE 1>0 %s; "),
				m_sLibName, m_sBatchWorkName, m_nMailClass , m_nMailKey , sBookKeyList , m_pInfo->MANAGE_CODE , ((CBO_LOC_3820*)m_pCBOLOC3820)->sDirectQuery);
	}
// 	if(m_nMailClass == 16){
// 		sTemp.Format(_T("insert into mn_mail_temp_tbl (rec_key, user_key, lib_name, batch_work_name, mail_class, mail_key, poll_key) select esl_mail_seq.nextval, rec_key, '%s', '%s', '%d', '%d', %s from co_loan_user_tbl U where 1>0 %s; "),
// 				m_sLibName, m_sBatchWorkName, m_nMailClass , m_nMailKey , sPollKey , ((CBO_LOC_3820*)m_pCBOLOC3820)->sDirectQuery);
// 	}else{
// 		sTemp.Format(_T("insert into mn_mail_temp_tbl (rec_key, user_key, lib_name, batch_work_name, mail_class, mail_key, book_key_list) select esl_mail_seq.nextval, rec_key, '%s', '%s', '%d', '%d', '%s' from co_loan_user_tbl U where 1>0 %s; "),
// 				m_sLibName, m_sBatchWorkName, m_nMailClass , m_nMailKey , sBookKeyList , ((CBO_LOC_3820*)m_pCBOLOC3820)->sDirectQuery);
// 	}


	pProgressCtrl->SetPos(30);

	ids = pDMBO->ExecuteQuery(sTemp , TRUE );
	if(ids){
		sTemp.Format(_T("������ ���������Կ� �ִµ� �����Ͽ����ϴ�."));
		AfxMessageBox(sTemp);
		return 0;
	}

	pProgressCtrl->SetPos(70);

	// �����߼��� ������Ʈ�ϱ�
	if(m_nMailClass==16 && !sPollKey.IsEmpty()){
		sTemp.Format(_T("update research_info_tbl set send_date = to_char(sysdate, 'YYYY/MM/DD') where rec_key = %s;"), sPollKey);
		pDMBO->ExecuteQuery(sTemp , TRUE );
		if(!m_sTime.IsEmpty()){
			sTemp.Format(_T("%s"), m_sTime.GetBufferSetLength(10));
			pDMPoll->SetCellData(_T("�����߼���"), sTemp, nGridPollIndex );
			pGridPoll->DisplayLine(nGridPollIndex);
			pGridPoll->SetTextMatrix(nGridPollIndex+1 , 1 , _T("V"));
		}
	}

	// SMS �߼��ϱ�.
	if(m_bSMS && !m_strSMS.IsEmpty()){
		CString sql;
		CString sms_type;

		if(m_nMailClass == 8){/* �ű�ȸ���ȳ� */
			sms_type = _T("SMS09");
		}else if(m_nMailClass == 12){/* �����ڷ�ȳ� */
			sms_type = _T("SMS07");
		}else if(m_nMailClass == 13){/* ��ġ����ȳ� */
			sms_type = _T("SMS04");
		}else if(m_nMailClass == 14){/* �������ϸ��� */
			sms_type = _T("SMS09");
		}else if(m_nMailClass == 15){/* �������� */
			sms_type = _T("SMS05");
		}else if(m_nMailClass == 16){/* �������� */
			sms_type = _T("SMS09");
		}else{
			sms_type = _T("SMS09");
		}
	
		sql.Format(_T("insert into sms_send_temp_tbl ( rec_key, name, handphone, msg, user_key, title, sms_type ) \
		select esl_mail_seq.nextval, U.name, U.handphone, '%s', U.rec_key, '%s', '%s' from co_loan_user_tbl U \
		where U.handphone is not null %s ;_T("), m_strSMS, m_sBatchWorkName, sms_type, ((CBO_LOC_3820*)m_pCBOLOC3820)->sDirectQuery);

		ids = pDMBO->ExecuteQuery(sql , TRUE );
	}

	pProgressCtrl->SetPos(100);

	sTemp.Format(_T("%d ���� ������ ���������Կ� �־����ϴ�."), ((CBO_LOC_3820*)m_pCBOLOC3820)->nDirectCount);
	AfxMessageBox(sTemp);

	return 0;
	EFS_END
	return -1;
}

HBRUSH CBO_LOC_3823::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
