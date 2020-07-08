
///////////////////////////////////////////////////////////////////////////////
// Mail Manger Dialog�� ���� ���� ������ üũ�Ͽ� ���� ��
///////////////////////////////////////////////////////////////////////////////

VOID CMailMgrTestDlg::OnbMailTest() 
{
	///Mail Manager ��ü ����
	CSendMailMgr MailMgr(this);

	///Mail Record ��ü ����
	SendMailRecord MailRecord;

	///Mail Record ��ü ����
	MailRecord.m_nFileAttachCnt = 0;
	MailRecord.m_pFileAttach = _T("");
	MailRecord.m_sBody = _T("<head><title>ù��°</title></head><body>ù��° ����</body>");
	MailRecord.m_sEncoding = _T("text/html");
	MailRecord.m_sRecipientAddress = _T("gjpark@eco.co.kr");	//���ڿ���
	MailRecord.m_sRecipientName	 = _T("�ڱ���");	//��û���̸�
	MailRecord.m_sSubject = _T("CIP �ݼ� - ù��°");
	MailRecord.m_sCCName    = _T("trzeze");
	MailRecord.m_sCCAddress = _T("trzeze@eco.co.kr");
	
	///Mail Record ��ü �߰�
	MailMgr.AddMailRecord(&MailRecord);

	///Mail Record ��ü ����
	MailRecord.m_nFileAttachCnt = 0;
	MailRecord.m_pFileAttach = _T("");
	MailRecord.m_sBody = _T("<head><title>�ι�°</title></head><body>�ι�° ����</body>");
	MailRecord.m_sEncoding = _T("text/html");
	MailRecord.m_sRecipientAddress = _T("gjpark@eco.co.kr");	//���ڿ���
	MailRecord.m_sRecipientName	 = _T("�ڱ���");	//��û���̸�
	MailRecord.m_sSubject = _T("CIP �ݼ� - �ι�°");
	MailRecord.m_sCCName    = _T("trzeze");
	MailRecord.m_sCCAddress = _T("trzeze@eco.co.kr");
	
	///Mail Record ��ü �߰�
	MailMgr.AddMailRecord(&MailRecord);

	///Mail Manager Dialog �ٿ��
	MailMgr.DoModal();

	///��� Ȯ��
	///���� ������ ������ ����
	INT nCntSended = MailMgr.GetCountMailSended();
	///���� ������ ������ ����
	INT nCntFailed = MailMgr.GetCountMailFailed();
}


///////////////////////////////////////////////////////////////////////////////
// Mail Manger Dialog�� ����� �ʰ� Background�� ������ ���� ��
///////////////////////////////////////////////////////////////////////////////

VOID CMailMgrTestDlg::OnbMailTestBackground() 
{
	///Mail Manager ��ü ����
	CSendMailMgr MailMgr(this);
	
	///Mail Record ��ü ����
	SendMailRecord MailRecord;

	///Mail Record ��ü ����
	MailRecord.m_nFileAttachCnt = 0;
	MailRecord.m_pFileAttach = _T("");
	MailRecord.m_sBody = _T("<head><title>ù��°</title></head><body>ù��° ����</body>");
	MailRecord.m_sEncoding = _T("text/html");
	MailRecord.m_sRecipientAddress = _T("gjpark@eco.co.kr");	//���ڿ���
	MailRecord.m_sRecipientName	 = _T("�ڱ���");	//��û���̸�
	MailRecord.m_sSubject = _T("CIP �ݼ� - ù��°");
	MailRecord.m_sCCName    = _T("trzeze");
	MailRecord.m_sCCAddress = _T("trzeze@eco.co.kr");
	
	///Mail Record ��ü �߰�
	MailMgr.AddMailRecord(&MailRecord);
	
	///Mail Record ��ü ����
	MailRecord.m_nFileAttachCnt = 0;
	MailRecord.m_pFileAttach = _T("");
	MailRecord.m_sBody = _T("<head><title>�ι�°</title></head><body>�ι�° ����</body>");
	MailRecord.m_sEncoding = _T("text/html");
	MailRecord.m_sRecipientAddress = _T("gjpark@eco.co.kr");	//���ڿ���
	MailRecord.m_sRecipientName	 = _T("�ڱ���");	//��û���̸�
	MailRecord.m_sSubject = _T("CIP �ݼ� - �ι�°");
	MailRecord.m_sCCName    = _T("trzeze");
	MailRecord.m_sCCAddress = _T("trzeze@eco.co.kr");
	
	///Mail Record ��ü �߰�
	MailMgr.AddMailRecord(&MailRecord);

	INT ids;

	///���� ������ ������ ��� ���� �ּ� ���� >>> �Ʒ� �ΰ��� ���
	{
		///���� ���ϼ����� ������ ����� �����ϴ� ���
		MailMgr.SetDefaultMailServer(_T("mail.eco.co.kr"), _T("gjpark@eco.co.kr"));
		
		///Registry �������� ���ϼ����� ������ ����� �����ϴ� ���
		//ids = MailMgr.GetDefaultMailServerInfo(TRUE);	// <<< �ݵ�� FALSE�� �Ű������� �Ѱܾ� �Ѵ�.
		//if (0>ids)
		//{
		//	AfxMessageBox(_T("���ϼ����� ������ �� �����ϴ�."));
		//	return;
		//}
	}
	
	///���� ����
	ids = MailMgr.SendMail(FALSE, TRUE);	// <<< �ݵ�� FALSE, TRUE�� �Ű������� �Ѱܾ� �Ѵ�.
	if (0>ids)
	{
		AfxMessageBox(_T("�������� �� ���ο����� �߻��Ͽ����ϴ�."));
		return;
	}
	
	///��� Ȯ��
	CString sTemp;
	sTemp.Format(_T("���������� �Ϸ�Ǿ����ϴ�. [����:%d / ����:%d]"), MailMgr.GetCountMailSended(), MailMgr.GetCountMailFailed());
	MessageBox(sTemp, _T("Ȯ��"), MB_ICONINFORMATION);
}
