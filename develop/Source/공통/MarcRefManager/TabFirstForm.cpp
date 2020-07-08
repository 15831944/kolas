// TabFirstForm.cpp : implementation file
//

#include "stdafx.h"
#include "TabFirstForm.h"
#include "IndicatorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabFirstForm dialog


CTabFirstForm::CTabFirstForm(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabFirstForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_DM_FIRST_IND = NULL;
	m_DM_SECOND_IND = NULL;
	m_pGridFirstInd = NULL;
	m_pGridSecondInd = NULL;

	m_strTagAlias = _T("");
	m_strCode = _T("");
	m_strName = _T("");
	m_strRepeat = _T("");
	m_strEssential = _T("");
}


VOID CTabFirstForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabFirstForm)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabFirstForm, CDialog)
	//{{AFX_MSG_MAP(CTabFirstForm)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabFirstForm message handlers

BOOL CTabFirstForm::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CTabFirstForm::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

VOID CTabFirstForm::ResizeWindow(CRect rectParent)
{
	//�ڱ� �ڽ�
	this->MoveWindow(rectParent);

	
	CWnd *pWnd = NULL;

	CRect rect;
	this->GetClientRect(rect);
	
	rect.top += 100;
	rect.bottom -= 10;
	rect.left += 10;
	rect.right -= 10;	
	
	CRect tmprect;
	tmprect = rect;
	tmprect.bottom -= rect.Height()/2 + 10;

	//��1 ���ñ�ȣ �׸��� static
	pWnd = (CWnd*)GetDlgItem(IDC_stcFirstIndicator);
	if(pWnd->GetSafeHwnd() == NULL) return;

	pWnd->MoveWindow(tmprect);	


	//�� 1 ���ñ�ȣ �׸���
	pWnd = (CWnd*)GetDlgItem(IDC_gridFirstIndicator);
	if(pWnd->GetSafeHwnd() == NULL) return;

	tmprect.top += 15;
	tmprect.bottom -= 15;
	tmprect.left += 15;
	tmprect.right -= 15;	

	pWnd->MoveWindow(tmprect);	


	tmprect = rect;
	tmprect.top += tmprect.Height()/2 + 10;

	//��2 ���ñ�ȣ �׸��� static
	pWnd = (CWnd*)GetDlgItem(IDC_stcSecondIndicator);
	if(pWnd->GetSafeHwnd() == NULL) return;

	pWnd->MoveWindow(tmprect);	



	//�� 2 ���ñ�ȣ �׸���
	pWnd = (CWnd*)GetDlgItem(IDC_gridSecondIndicator);
	if(pWnd->GetSafeHwnd() == NULL) return;

	tmprect.top += 15;
	tmprect.bottom -= 15;
	tmprect.left += 15;
	tmprect.right -= 15;	

	pWnd->MoveWindow(tmprect);	



}

BOOL CTabFirstForm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("��ũ����_��1") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : ��ũ����_��1") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_��ũ����_��1"));
	m_DM_FIRST_IND = FindDM(_T("DM_��ũ����_ù��°���ñ�ȣ"));
	m_DM_SECOND_IND = FindDM(_T("DM_��ũ����_�ι�°���ñ�ȣ"));
	m_pGridFirstInd = (CESL_Grid*)m_pCM->FindControl(_T("ù��°���ñ�ȣ�׸���"));
	m_pGridSecondInd = (CESL_Grid*)m_pCM->FindControl(_T("�ι�°���ñ�ȣ�׸���"));

	if(!m_pCM || !m_DM_FIRST_IND || !m_DM_SECOND_IND || !m_pGridFirstInd || !m_pGridSecondInd)
	{
		AfxMessageBox( _T("reference Error : ��ũ����_��1") );
		return FALSE;		
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CTabFirstForm::SetAllData(CString strTagAlias,
							   CString strCode,
							   CString strName,
							   CString strRepeat,
							   CString strEssential)
{

	m_strTagAlias = strTagAlias;
	m_strCode = strCode;
	m_strName = strName;
	m_strRepeat = strRepeat;
	m_strEssential = strEssential;
}

VOID CTabFirstForm::ShowAllData()
{
	//�±� �⺻ ����
	m_pCM->SetControlMgrData(_T("�ڵ�"),m_strCode);
	m_pCM->SetControlMgrData(_T("�̸�"),m_strName);

	CString strTmp;

	if(_T("Y") == m_strRepeat)
	{
		strTmp  = _T("�ݺ�");
	}
	else if(_T("N") == m_strRepeat)
	{
		strTmp  = _T("�ݺ��Ұ�");
	}

	m_pCM->SetControlMgrData(_T("�ݺ�����"),strTmp);


	if(_T("M") == m_strEssential)
	{
        strTmp = _T("�ʼ�");
	}
	else if(_T("A") == m_strEssential)
	{
        strTmp = _T("�ش���ʼ�");
	}
	else if(_T("O") == m_strEssential)
	{
        strTmp = _T("�緮");
	}
	else if(_T("B") == m_strEssential)
	{
        strTmp = _T("����");
	}

	m_pCM->SetControlMgrData(_T("�ʼ�����"),strTmp);


	//���ñ�ȣ
	CString strWhere;
	strWhere.Format(_T("MARC_TAG_ALIAS = '%s'"),m_strTagAlias);
	
	m_DM_FIRST_IND->RefreshDataManager(strWhere);
	m_DM_SECOND_IND->RefreshDataManager(strWhere);

	m_pGridFirstInd->Display();
	m_pGridSecondInd->Display();

}

VOID CTabFirstForm::InsertIndicator()
{

	CIndicatorDlg dlg(this);
	dlg.m_nOpenMode = 1;
	dlg.m_pParentDM1 = m_DM_FIRST_IND;
	dlg.m_pParentDM2 = m_DM_SECOND_IND;
	dlg.m_pParentGrid1 = m_pGridFirstInd;
	dlg.m_pParentGrid2 = m_pGridSecondInd;
	dlg.m_strMARC_TAG_ALIAS = m_strTagAlias;

	dlg.DoModal();
}

VOID CTabFirstForm::ModifyIndicator()
{
	INT nIdx,nOrder;
	if(GetSelectedIdx(&nOrder,&nIdx)<0) return;

	CIndicatorDlg dlg(this);
	dlg.m_nOpenMode = -1;
	dlg.m_pParentDM1 = m_DM_FIRST_IND;
	dlg.m_pParentDM2 = m_DM_SECOND_IND;
	dlg.m_pParentGrid1 = m_pGridFirstInd;
	dlg.m_pParentGrid2 = m_pGridSecondInd;
	dlg.m_strMARC_TAG_ALIAS = m_strTagAlias;
	dlg.m_nIndOrder = nOrder;
	dlg.m_nCurrentParentIdx = nIdx;

	dlg.DoModal();
}

VOID CTabFirstForm::DeleteIndicator()
{
	INT nIdx,nOrder;
	if(GetSelectedIdx(&nOrder,&nIdx)<0) return;

	CString strPK;
	CString strIndCode;
	CString strMsg;
	CString strQuery;
	INT ids = -1;
	

	if(1 == nOrder)
	{
		
		strIndCode = m_DM_FIRST_IND->GetCellData(_T("IND_CODE"),nIdx);
		strMsg.Format(_T("�� 1 ���ñ�ȣ���� [ %s ] �ڵ带 ���� �Ͻðڽ��ϱ�?"),strIndCode);
		if(AfxMessageBox(strMsg,MB_YESNO) != IDYES) return;

		strPK = m_DM_FIRST_IND->GetCellData(_T("PK"),nIdx);
		
		m_DM_FIRST_IND->StartFrame();
		strQuery.Format(_T("DELETE FROM MARC_FIRST_INDICATOR_TBL WHERE PK = %s;"),strPK);
		m_DM_FIRST_IND->AddFrame(strQuery);
		ids = m_DM_FIRST_IND->SendFrame();
		m_DM_FIRST_IND->EndFrame();

	}
	else if(2 == nOrder)
	{
		strIndCode = m_DM_SECOND_IND->GetCellData(_T("IND_CODE"),nIdx);
		strMsg.Format(_T("�� 2 ���ñ�ȣ���� [ %s ] �ڵ带 ���� �Ͻðڽ��ϱ�?"),strIndCode);
		if(AfxMessageBox(strMsg,MB_YESNO) != IDYES) return;
		
		strPK = m_DM_SECOND_IND->GetCellData(_T("PK"),nIdx);
		
		m_DM_SECOND_IND->StartFrame();
		strQuery.Format(_T("DELETE FROM MARC_SECOND_INDICATOR_TBL WHERE PK = %s;"),strPK);
		m_DM_SECOND_IND->AddFrame(strQuery);
		ids = m_DM_SECOND_IND->SendFrame();
		m_DM_SECOND_IND->EndFrame();
	}

	if(ids<0) 
	{
		AfxMessageBox(_T("�����ϴ� �� ������ �߻��Ͽ����ϴ�"));
	}
	else
	{
		AfxMessageBox(_T("�����Ͽ����ϴ�!"));
		
		if(1 == nOrder)
		{
			m_DM_FIRST_IND->DeleteRow(nIdx);
			m_pGridFirstInd->Display();
		}
		else if(2 == nOrder)
		{
			m_DM_SECOND_IND->DeleteRow(nIdx);
			m_pGridSecondInd->Display();
		}
	}

}

INT CTabFirstForm::GetSelectedIdx(INT* nOrder,INT* nIdx)
{
	if(0 == m_DM_FIRST_IND->GetRowCount() && 0 == m_DM_SECOND_IND->GetRowCount())
	{
		AfxMessageBox(_T("���� �Էµ� ���ñ�ȣ�� �����ϴ�!"));
	    return -1;
	}

	m_pGridFirstInd->SelectMakeList();
	m_pGridSecondInd->SelectMakeList();

	INT nFirstCnt,nSecondCnt;
	nFirstCnt = nSecondCnt = -1;
	
	nFirstCnt  = m_pGridFirstInd->SelectGetCount();
	nSecondCnt = m_pGridSecondInd->SelectGetCount();

	if(0 == nFirstCnt && 0 == nSecondCnt)
	{
		AfxMessageBox(_T("������ ���ñ�ȣ�� �����ϴ�!"));
	    return -1;
	}

	if(nFirstCnt>0 && nSecondCnt>0)
	{
		AfxMessageBox(_T("ù��° �����ȣ�� �ι�° ���ñ�ȣ ��� �����Ǿ����ϴ�!\n")
					  _T("���ʸ� ������ �ֽʽÿ�!"));
	    return -1;
	}

	if(nFirstCnt>1 || nSecondCnt>1)
	{
		AfxMessageBox(_T("�ѹ��� �ϳ��� ���ñ�ȣ�� ������ �� �ֽ��ϴ�!"));
	    return -1;
	}
	
	if(nFirstCnt>0)
	{
		*nOrder = 1;
		*nIdx = m_pGridFirstInd->SelectGetHeadPosition();
	}

	if(nSecondCnt>0)
	{
		*nOrder = 2;
		*nIdx = m_pGridSecondInd->SelectGetHeadPosition();
	}

	return 0;
}