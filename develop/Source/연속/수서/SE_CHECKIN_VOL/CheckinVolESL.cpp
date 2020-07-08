// CheckinVolESL.cpp : implementation file
//

#include "stdafx.h"
#include "CheckinVolESL.h"
#include "..\SE_API\SeExpectMgr.h"
#include "..\SE_API\SeApi.h"
#include "..\SE_EXPECT\SeExpectForm.h"
#include "..\SE_VOL\SeVolDlg.h"
#include "CheckinVolMoreExp.h"
#include "CheckinVolSetMoreExp.h"
#include "..\..\..\����\�����ʷϰ���\ContentsMgr_Editor\CONTENTMGR_SE_EDIT.h"
#include "..\SE_API\CheckinReceipt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckinVolESL dialog


CCheckinVolESL::CCheckinVolESL(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckinVolESL)
	m_bIsAllDataShow = FALSE;
	//}}AFX_DATA_INIT
	
	//���� VIEW���� ������ �޽����� �����Ѵ�.
	pMain = NULL;

	m_pCM = NULL;
	m_pDM_IDX = NULL;
	m_pParentGrid = NULL;
	m_nACQ_TYPE = 1; //default
	m_nCurrentParentIdx = 0;
	m_nCurrentIdx = -1;
	m_pGrid = NULL;
	m_pDM_EXP = NULL;
	m_pDM_DB = NULL;
	m_pDM_WORK = NULL;
	m_pDM_ACQ = NULL;
	
	m_strSPECIES_KEY = _T("");
	m_strPUB_FREQ = _T("");
	m_strACQ_YEAR = _T("");
	m_strRECEIPT_NO = _T("");

	m_pProgressDlg = NULL;
	m_pResourceMgr = NULL;

	//color ref init
	CR_exp = RGB(255,255,255);
	CR_acq = RGB(255,255,255);
	CR_lack = RGB(255,255,255);
	CR_miss = RGB(255,255,255);
	CR_delay = RGB(255,255,255);
	CR_nonpub = RGB(255,255,255);

	CR_BLACK = RGB(0,0,0);

	m_bIsLightVersion = FALSE;

	m_bIsNeedReceiptNo = FALSE;

	m_bIsDepartMngUse = FALSE;
	m_pDM_DEPART = NULL;
}

CCheckinVolESL::~CCheckinVolESL()
{
	//FreeChild();
}

VOID CCheckinVolESL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckinVolESL)
	DDX_Control(pDX, IDC_tabCHECKIN_VOL, m_tabCTRL);
	DDX_Check(pDX, IDC_chkCHECKIN_VOL_ALLSHOW, m_bIsAllDataShow);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckinVolESL, CDialog)
	//{{AFX_MSG_MAP(CCheckinVolESL)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabCHECKIN_VOL, OnSelchangetabCHECKINVOL)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnCHECKIN_VOL_PREV, OnbtnCHECKINVOLPREV)
	ON_BN_CLICKED(IDC_btnCHECKIN_VOL_NEXT, OnbtnCHECKINVOLNEXT)
	ON_BN_CLICKED(IDC_chkCHECKIN_VOL_ALLSHOW, OnchkCHECKINVOLALLSHOW)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckinVolESL message handlers

BOOL CCheckinVolESL::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


INT CCheckinVolESL::SetParentInfo(CESL_DataMgr *pParentDM, 
									   CESL_Grid *pParentGrid,
									   INT nACQ_TYPE,
									   CString strACQ_YEAR,
									   CString strRECEIPT_NO,
									   BOOL bIsLightVersion)
{
	m_pDM_IDX = pParentDM;
	m_pParentGrid = pParentGrid;
	m_nACQ_TYPE = nACQ_TYPE;
	m_strACQ_YEAR = strACQ_YEAR;
	m_strRECEIPT_NO = strRECEIPT_NO;
	m_bIsLightVersion = bIsLightVersion;

	return 0;
}


BOOL CCheckinVolESL::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("����_����_üũ��_�Ǵ���"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error : ����_����_üũ��_�Ǵ���"));
		return FALSE;
	}
	
	m_tabCTRL.InsertItem(0,_T("����/��ȣ"));
	m_tabCTRL.InsertItem(1,_T("�Լ�"));
	m_tabCTRL.InsertItem(2,_T("�Լ�/��ȣ"));	
	m_tabCTRL.InsertItem(3,_T("��ȣ"));	
	
	
	m_pCM = FindCM(_T("CM_����_üũ��_�Ǵ���"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("�׸���"));

	
	m_pDM_EXP = FindDM(_T("DM_üũ��_����"));
	m_pDM_DB = FindDM(_T("DM_üũ��_DB"));
	m_pDM_WORK = FindDM(_T("DM_üũ��_�۾�"));

	//���� ���� ����
	if(1 == m_nACQ_TYPE) 
		m_pDM_ACQ = FindDM(_T("DM_üũ��_��������"));
	else
		m_pDM_ACQ = FindDM(_T("DM_üũ��_��������"));
	

	if(!m_pCM || !m_pGrid || !m_pDM_EXP || !m_pDM_DB || !m_pDM_WORK || !m_pDM_ACQ)
	{
		AfxMessageBox(_T("CM,DM,Grid Error : üũ��_�Ǵ���"));
		return FALSE;
	}
	
	//�׸����� �ڵ� setreverse ��� ����
	m_pGrid->m_bFroceReverse = FALSE; 

	//�׸����� �ڵ� sort ��� ����
//	m_pGrid->SetSort(FALSE);
		

	//�μ����� ��� ���θ� �����´�
	CSeApi api(this);
	m_bIsDepartMngUse = api.IsDepartMngUse(m_pDM_WORK);
	
	if(1 == m_nACQ_TYPE && m_bIsDepartMngUse)
	{
		m_pDM_DEPART = FindDM(_T("DM_üũ��_�μ������ڷ�"));
		if(!m_pDM_DEPART)
		{
			AfxMessageBox(_T("DM Error : DM_üũ��_�μ������ڷ�"));
		}
	}


	//ǥ�������� ������� �ʴ� ���� Ÿ�̸� ������ ���� �ش� ��ư�� �����
	if(!api.IsCoverImageUse(m_pDM_WORK))
	{
		SetTimer(0,100,NULL);
	}
	

	//������ �ʱ�ȭ 
	if(InitAllReference()<0)
	{
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CCheckinVolESL::FreeChild()
{
	if(m_pProgressDlg)	delete m_pProgressDlg;
	m_pProgressDlg = NULL;

}

VOID CCheckinVolESL::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridCHECKIN_VOL);
	if(pGrid->GetSafeHwnd() == NULL) return;
		
	/*
	CRect rect;
	GetClientRect(rect);
	rect.top = 25;
	rect.bottom -= 5;
	rect.left += 5;
	rect.right -= 5;
	
	pGrid->MoveWindow(rect);
	
	CWnd* pWnd = NULL;
	pWnd = GetDlgItem(IDC_tabCHECKIN_VOL);
	if(pWnd->GetSafeHwnd() == NULL) return;
	pWnd->MoveWindow(0, 0 , cx, cy);
	*/

	CRect rect;
	GetClientRect(rect);
	rect.top = 60;
	rect.bottom -= 5;
	rect.left += 5;
	rect.right -= 5;
	
	pGrid->MoveWindow(rect);
	
	CWnd* pWnd = NULL;
	pWnd = GetDlgItem(IDC_tabCHECKIN_VOL);
	if(pWnd->GetSafeHwnd() == NULL) return;
	pWnd->MoveWindow(0, 35 , cx+2, cy);
	pGrid->MoveWindow(-2, 55, cx+2, cy-55);


}

INT CCheckinVolESL::InitAllReference()
{
	
	//�θ� �׸��忡�� ���������� ���´�
	if( !m_pParentGrid )
	{
		AfxMessageBox(_T("�θ� �׸��带 ã�� �� �����ϴ�!"));
		return -1;
	}

	if( !m_pDM_IDX )
	{
		AfxMessageBox(_T("�θ� DM�� ã�� �� �����ϴ�!"));
		return -2;
	}


	m_pParentGrid->SelectMakeList();
	//===================================================
	//2010.01.11 ADD BY PJW : ���� index�� �����ش�.
//	m_nCurrentParentIdx = m_pParentGrid->SelectGetHeadPosition();
//	if( m_nCurrentParentIdx < 0 )
//	{
//		m_nCurrentParentIdx = m_pParentGrid->GetIdx();
//		//AfxMessageBox(_T("������ ������ �����ϴ�!"));
//		//return -3;
//	}
	m_nCurrentParentIdx = m_pParentGrid->GetIdx();
	//===================================================
	

	//���� ���Ͽ��� �׸��忡 ĥ�� ������ ������ �´� 
	SetColorRefFromFile();

	RefreshAll();

	return 0;
}

INT CCheckinVolESL::SetColorRefFromFile()
{
	
	LPCTSTR lpszFileName = CFG_FILE_PATH_COLOR;
	CStdioFile file;
	INT nSuccess = file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary);
	
	if(nSuccess)
	{
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
		
		CString strLine;
		CString tmp;
		CString strColor;
		INT nPos;
		INT nReadCnt = 0;
		
		while(file.ReadString(strLine))
		{
			
			strLine.TrimLeft();
			strLine.TrimRight();
			
			nPos = strLine.Find(_T(":"));

			if(nPos>0)
			{
				tmp = strLine.Left(nPos);
				tmp.TrimLeft();
				tmp.TrimRight();
				
				if(_T("����") == tmp)
				{
					strColor = strLine.Mid(nPos+1);
					strColor.TrimLeft();
					strColor.TrimRight();
					if(!strColor.IsEmpty()) SetColorRefByString(CR_exp,strColor);
				}
				else if(_T("�Լ�") == tmp)
				{
					strColor = strLine.Mid(nPos+1);
					strColor.TrimLeft();
					strColor.TrimRight();
					if(!strColor.IsEmpty()) SetColorRefByString(CR_acq,strColor);
				}
				else if(_T("�̴�") == tmp)
				{
					strColor = strLine.Mid(nPos+1);
					strColor.TrimLeft();
					strColor.TrimRight();
					if(!strColor.IsEmpty()) SetColorRefByString(CR_lack,strColor);
				}
				else if(_T("��ȣ") == tmp)
				{
					strColor = strLine.Mid(nPos+1);
					strColor.TrimLeft();
					strColor.TrimRight();
					if(!strColor.IsEmpty()) SetColorRefByString(CR_miss,strColor);

				}
				else if(_T("����") == tmp)
				{
					strColor = strLine.Mid(nPos+1);
					strColor.TrimLeft();
					strColor.TrimRight();
					if(!strColor.IsEmpty()) SetColorRefByString(CR_delay,strColor);

				}
				else if(_T("�̹���") == tmp)
				{
					strColor = strLine.Mid(nPos+1);
					strColor.TrimLeft();
					strColor.TrimRight();
					if(!strColor.IsEmpty()) SetColorRefByString(CR_nonpub,strColor);
				}		
			}
		}

		file.Close();
	}


	return 0;
}

INT CCheckinVolESL::WriteColorRefOnFile()
{
	
	LPCTSTR lpszFileName = CFG_FILE_PATH_COLOR;
	CStdioFile file;
	INT nSuccess = file.Open(lpszFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
	
	if(nSuccess<0) return -1;
	
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}
		
	CString strLine;

	strLine = GetStringByColorRef(_T("����"), CR_exp);
	file.WriteString(strLine);

	strLine = GetStringByColorRef(_T("�Լ�"), CR_acq);
	file.WriteString(strLine);

	strLine = GetStringByColorRef(_T("�̴�"), CR_lack);
	file.WriteString(strLine);
	
	strLine = GetStringByColorRef(_T("��ȣ"), CR_miss);
	file.WriteString(strLine);
	
	strLine = GetStringByColorRef(_T("����"), CR_delay);
	file.WriteString(strLine);
	
	strLine = GetStringByColorRef(_T("�̹���"), CR_nonpub);
	file.WriteString(strLine);

	file.Close();
	
	return 0;
}

CString CCheckinVolESL::GetStringByColorRef(CString strTitle, COLORREF& colorref)
{
	CString strResult;
	
	INT red,green,blue;
	
	red = GetRValue(colorref);
	green = GetGValue(colorref);
	blue = GetBValue(colorref);
	
	strResult.Format(_T("%s : %d/%d/%d\r\n"),strTitle,red,green,blue);

	return strResult;
}

VOID CCheckinVolESL::SetColorRefByString(COLORREF& colorref, CString strColor)
{
	INT red,green,blue;
	red = green = blue = 255;
	ExtractStringColorToRGB(strColor,&red,&green,&blue);
	colorref = RGB(red,green,blue);
}

VOID CCheckinVolESL::ExtractStringColorToRGB(CString strColor, INT *red, INT *green, INT *blue)
{
	if(strColor.GetLength() == 0) return ;
	INT idx, startidx, lastidx;
	idx = startidx = 0;
	lastidx = strColor.GetLength();
	idx = strColor.Find(_T("/"), startidx);
	if(idx > 0)
		*red = _ttoi((strColor.Left(idx)).GetBuffer(0));
	startidx = idx + 1;
	idx = strColor.Find(_T("/"), startidx);
	if(idx > 0)
		*green = _ttoi((strColor.Mid(startidx, idx - 1)).GetBuffer(0));
	startidx = idx + 1;
	*blue = _ttoi((strColor.Right(lastidx - startidx)).GetBuffer(0));
	return ;
}

INT CCheckinVolESL::RefreshAll(INT nMoreExp)
{
	//�⺻ �߰� ���� ���
	if( 0 == nMoreExp )
	{
		nMoreExp = GetDefaultMoreExpCnt();
	}
	
	m_strSPECIES_KEY = m_pDM_IDX->GetCellData(_T("ISS_��KEY"),m_nCurrentParentIdx);
	m_strPUB_FREQ = m_pDM_IDX->GetCellData(_T("ISS_�����"),m_nCurrentParentIdx);
	
	if( m_strPUB_FREQ.IsEmpty() )
	{
		AfxMessageBox(_T("�ش� ���� ����󵵰� ���� ���������� ������ �� �����ϴ�!"));
		return -1;
	}

	if( m_strSPECIES_KEY.IsEmpty() )
	{
		AfxMessageBox(_T("�ش� ��KEY�� ã�� �� �����ϴ�!"));
		return -2;
	}
	
	m_pDM_EXP->FreeData(); 
	m_pDM_DB->FreeData(); 
	m_pDM_ACQ->FreeData();


	CString strWhere;
	strWhere.Format(_T("SPECIES_KEY = %s"),m_strSPECIES_KEY);
	m_pDM_ACQ->RefreshDataManager(strWhere);
	if( 0 == m_pDM_ACQ->GetRowCount() )
	{
		AfxMessageBox(_T("���Ա��������� ������ �� �����ϴ�"));
		return -3;
	}
	
	CString strACQ_KEY = _T("");
	if(1 == m_nACQ_TYPE) strACQ_KEY = m_pDM_ACQ->GetCellData(_T("SP_���԰���KEY"),0);
	else strACQ_KEY = m_pDM_ACQ->GetCellData(_T("SD_��������KEY"),0);
	
	
	m_pProgressDlg->SetProGressRange(100);
	//m_pProgressDlg->SetMessage(_T("���������� �����ϴ� ��..."));
	m_pProgressDlg->BringWindowToTop();
	m_pProgressDlg->ShowWindow(SW_SHOW);

	CProgressCtrl* pProgress = m_pProgressDlg->GetProgressCtrl();


	//���������� �����Ѵ� 
	//���� ������°� ��, Ȥ�� �ް��̸� ���� ���� �ʴ´� 
	m_arrayVolExpKeyList.RemoveAll();
	m_arrayOtherVolExpKeyList.RemoveAll();
	
	CString strPUB_STATUS;
	strPUB_STATUS = m_pDM_IDX->GetCellData(_T("ISS_�������"),m_nCurrentParentIdx);

	if(_T("d") != strPUB_STATUS && _T("s") !=  strPUB_STATUS)
	{	

		CSeExpectMgr mgr(this);
		mgr.InitMgr(m_pDM_EXP,
					m_pDM_ACQ, 
					m_strPUB_FREQ,
					m_nACQ_TYPE,
					m_arrayVolExpKeyList,
					m_arrayOtherVolExpKeyList,
					nMoreExp,
					pProgress);
	}
	else
	{
		if(_T("d") == strPUB_STATUS)
		{
			AfxMessageBox(_T("�� �ڷ�� �󰣵Ǿ����Ƿ� ���������� �������� �ʽ��ϴ�!"));
		}
		else if(_T("s") == strPUB_STATUS)
		{
			AfxMessageBox(_T("�� �ڷ�� �ް����̹Ƿ� ���������� �������� �ʽ��ϴ�!"));
		}

	}

	/*
	//DB���� �ش������� �����´�.
	strWhere.Format(_T("A.VOL_KEY = V.REC_KEY AND V.VOL_TYPE = 'V' AND A.ACQ_KEY = %s"),strACQ_KEY); 
	m_pDM_DB->RefreshDataManager(strWhere);
	*/

	//modified by loveisverb
	//2004-12-10
	//����⵵�� �������� ���Ѱ˻�
	UpdateData(TRUE);
	CString strPUBLISH_DATE;
	m_pCM->GetControlMgrData(_T("����⵵"),strPUBLISH_DATE);

	if(m_bIsAllDataShow || strPUBLISH_DATE.IsEmpty())
	{
		strWhere.Format(_T("A.VOL_KEY = V.REC_KEY AND V.VOL_TYPE = 'V' AND A.ACQ_KEY = %s"),strACQ_KEY); 
		m_pDM_DB->RefreshDataManager(strWhere);
	}
	else
	{
		
		CString strStartDate;
		CString strEndDate;
		strStartDate.Format(_T("%s/01/01"),strPUBLISH_DATE);
		strEndDate.Format(_T("%s/12/31"),strPUBLISH_DATE);
	
		strWhere.Format(_T("A.VOL_KEY = V.REC_KEY AND V.VOL_TYPE = 'V' AND A.ACQ_KEY = %s")
						_T(" AND V.PUBLISH_DATE BETWEEN '%s' AND '%s'"),
						strACQ_KEY,strStartDate,strEndDate); 
		m_pDM_DB->RefreshDataManager(strWhere);
	}
	
	m_pProgressDlg->SetProGressPos(100);
	m_pProgressDlg->ShowWindow(SW_HIDE);


	//���� ������ �����ؼ� �����ش�
	MakeWorkDM();
	
	//�θ� �׸��� ����
	m_pParentGrid->SetReverse(m_nCurrentParentIdx);
	
	//������ �����ֱ� 
	if(m_pResourceMgr) m_pResourceMgr->m_dlgSpeciesInfo.ShowSpeciesInfo(m_nCurrentParentIdx);
	
	//���� �ʱ�ȭ 
	m_bIsNeedReceiptNo = FALSE;

	//�μ���������
	if(1 == m_nACQ_TYPE && m_bIsDepartMngUse)
	{
		strWhere.Format(_T("PURCHASE_MANAGE_KEY = %s"),strACQ_KEY);
		m_pDM_DEPART->RefreshDataManager(strWhere);
		
	}

	return 0;
}
	
INT CCheckinVolESL::CopyExpToWork()
{
	INT nRow_Exp = m_pDM_EXP->GetRowCount();
	INT nRow_Work;
	CString strVoltitle;
	CSeApi api(this);
	CString strBeforeRow;

	for(INT i=0;i<nRow_Exp;i++)
	{
		m_pDM_WORK->InsertRow(-1);
		nRow_Work = m_pDM_WORK->GetRowCount()-1;
		CopyDMToDMByAlias(m_pDM_EXP,i,m_pDM_WORK,nRow_Work);
		

		//������ȣ�� ��ȣ�� �����ش�
		strVoltitle = _T("");
		strVoltitle = m_pDM_EXP->GetCellData(_T("SA_�Լ�������ȣ��"),i);
		m_pDM_WORK->SetCellData(_T("SV_��ȣ��"),strVoltitle,nRow_Work);
		m_pDM_WORK->SetCellData(_T("UDF_�Լ�����"),_T("����"),nRow_Work);
		m_pDM_WORK->SetCellData(_T("UDF_����"),_T("E"),nRow_Work);
		m_pDM_WORK->SetCellData(_T("SV_��ȣ����"),_T("1"),nRow_Work);
		m_pDM_WORK->SetCellData(_T("UDF_��ȣ����"),_T("�Ϲ�"),nRow_Work);
	
		strBeforeRow.Format(_T("%d"),i);
		m_pDM_WORK->SetCellData(_T("UDF_��"),strBeforeRow,nRow_Work);

		//������, ���Ա��������� ���� �������� ���Ա��к� �� ������ ����� ����.
		api.MAKE_EXP_WORK_by_IDX_ACQ(m_pDM_IDX,m_nCurrentParentIdx,m_pDM_WORK,nRow_Work,m_pDM_ACQ,m_nACQ_TYPE);
		

	}
	
	return 0;
}

INT CCheckinVolESL::CopyMissToWork()
{
	INT nRow_DB = m_pDM_DB->GetRowCount();
	INT nRow_Work;
	CString strCHECKIN_STATUS;
	CString strVOLUME_CLASS;
	CString strBeforeRow;
	CSeApi api(this);

	CString strLackBookCnt;
	INT nLackBook = 0;

	for(INT i=0;i<nRow_DB;i++)
	{
		strCHECKIN_STATUS = _T("");
		strCHECKIN_STATUS = m_pDM_DB->GetCellData(_T("SA_�Լ�����"),nRow_DB-i-1);
		
		if(_T("1") == strCHECKIN_STATUS) continue;
			
		m_pDM_WORK->InsertRow(-1);
		nRow_Work = m_pDM_WORK->GetRowCount()-1;
		CopyDMToDMByAlias(m_pDM_DB,nRow_DB-i-1,m_pDM_WORK,nRow_Work);

		if(_T("2") == strCHECKIN_STATUS)
			m_pDM_WORK->SetCellData(_T("UDF_�Լ�����"),_T("��ȣ"),nRow_Work);
		else if(_T("3") == strCHECKIN_STATUS)
			m_pDM_WORK->SetCellData(_T("UDF_�Լ�����"),_T("�̴�"),nRow_Work);
		

		strVOLUME_CLASS = _T("");
		strVOLUME_CLASS = m_pDM_WORK->GetCellData(_T("SV_��ȣ����"),nRow_Work);
		

		if(_T("1") == strVOLUME_CLASS)
			m_pDM_WORK->SetCellData(_T("UDF_��ȣ����"),_T("�Ϲ�"),nRow_Work);
		else if(_T("2") == strVOLUME_CLASS)
			m_pDM_WORK->SetCellData(_T("UDF_��ȣ����"),_T("�պ�ȣ"),nRow_Work);
		else if(_T("3") == strVOLUME_CLASS)
			m_pDM_WORK->SetCellData(_T("UDF_��ȣ����"),_T("�̹���"),nRow_Work);


		m_pDM_WORK->SetCellData(_T("UDF_����"),_T("D"),nRow_Work); //D: DB
		
		strBeforeRow.Format(_T("%d"),i);
		m_pDM_WORK->SetCellData(_T("UDF_��"),strBeforeRow,nRow_Work);
		
		//������, ���Ա��������� �߰��Ѵ�
		api.MAKE_DB_WORK_by_IDX_ACQ(m_pDM_IDX,m_nCurrentParentIdx,m_pDM_WORK,nRow_Work,m_pDM_ACQ,m_nACQ_TYPE);
		

		//���� ó��
		nLackBook = 0;
		strLackBookCnt = m_pDM_WORK->GetCellData(_T("UDF_������"),nRow_Work);
		if(!strLackBookCnt.IsEmpty()) nLackBook = _ttoi(strLackBookCnt.GetBuffer(0));
		
		if(nLackBook>0){
			m_pDM_WORK->SetCellData(_T("UDF_�Լ�����"),_T("����"),nRow_Work);
			//SetGridRowUniqueColor(INT nRowIdx)
		}
		

		
	}

	return 0;
}

INT CCheckinVolESL::CopyAcqToWork()
{
	INT nRow_DB = m_pDM_DB->GetRowCount();
	INT nRow_Work;
	CString strCHECKIN_STATUS;
	CString strVOLUME_CLASS;
	CString strBeforeRow;
	CSeApi api(this);

	for(INT i=0;i<nRow_DB;i++)
	{
		
		strCHECKIN_STATUS = _T("");
		strCHECKIN_STATUS = m_pDM_DB->GetCellData(_T("SA_�Լ�����"),nRow_DB-i-1);
		
		if(_T("2") == strCHECKIN_STATUS) continue;
		
		m_pDM_WORK->InsertRow(-1);
		nRow_Work = m_pDM_WORK->GetRowCount()-1;
		CopyDMToDMByAlias(m_pDM_DB,nRow_DB-i-1,m_pDM_WORK,nRow_Work);

		if(_T("1") == strCHECKIN_STATUS)
			m_pDM_WORK->SetCellData(_T("UDF_�Լ�����"),_T("�Լ�"),nRow_Work);
		else if(_T("3") == strCHECKIN_STATUS)
			m_pDM_WORK->SetCellData(_T("UDF_�Լ�����"),_T("�̴�"),nRow_Work);

		
		strVOLUME_CLASS = _T("");
		strVOLUME_CLASS = m_pDM_WORK->GetCellData(_T("SV_��ȣ����"),nRow_Work);
		

		if(_T("1") == strVOLUME_CLASS)
			m_pDM_WORK->SetCellData(_T("UDF_��ȣ����"),_T("�Ϲ�"),nRow_Work);
		else if(_T("2") == strVOLUME_CLASS)
			m_pDM_WORK->SetCellData(_T("UDF_��ȣ����"),_T("�պ�ȣ"),nRow_Work);
		else if(_T("3") == strVOLUME_CLASS)
			m_pDM_WORK->SetCellData(_T("UDF_��ȣ����"),_T("�̹���"),nRow_Work);
	

		m_pDM_WORK->SetCellData(_T("UDF_����"),_T("D"),nRow_Work); //D: DB

		strBeforeRow.Format(_T("%d"),i);
		m_pDM_WORK->SetCellData(_T("UDF_��"),strBeforeRow,nRow_Work);

		//������, ���Ա��������� �߰��Ѵ�
		api.MAKE_DB_WORK_by_IDX_ACQ(m_pDM_IDX,m_nCurrentParentIdx,m_pDM_WORK,nRow_Work,m_pDM_ACQ,m_nACQ_TYPE);
		
	}

	return 0;
}

INT CCheckinVolESL::CopyAllDBToWork()
{
	INT nRow_DB = m_pDM_DB->GetRowCount();
	INT nRow_Work;
	CString strCHECKIN_STATUS;
	CString strBeforeRow;
	CSeApi api(this);
	CString strVOLUME_CLASS;
	
	CString strLackBookCnt;
	INT nLackBook = 0;

	for(INT i=0;i<nRow_DB;i++)
	{
		m_pDM_WORK->InsertRow(-1);
		nRow_Work = m_pDM_WORK->GetRowCount()-1;
		
		strCHECKIN_STATUS = _T("");
		strCHECKIN_STATUS = m_pDM_DB->GetCellData(_T("SA_�Լ�����"),nRow_DB-i-1);
		CopyDMToDMByAlias(m_pDM_DB,nRow_DB-i-1,m_pDM_WORK,nRow_Work);
		
		if(_T("1") == strCHECKIN_STATUS)
			m_pDM_WORK->SetCellData(_T("UDF_�Լ�����"),_T("�Լ�"),nRow_Work);
		else if(_T("2") == strCHECKIN_STATUS)
			m_pDM_WORK->SetCellData(_T("UDF_�Լ�����"),_T("��ȣ"),nRow_Work);
		else if(_T("3") == strCHECKIN_STATUS)
			m_pDM_WORK->SetCellData(_T("UDF_�Լ�����"),_T("�̴�"),nRow_Work);

		
		strVOLUME_CLASS = _T("");
		strVOLUME_CLASS = m_pDM_WORK->GetCellData(_T("SV_��ȣ����"),nRow_Work);
		

		if(_T("1") == strVOLUME_CLASS)
			m_pDM_WORK->SetCellData(_T("UDF_��ȣ����"),_T("�Ϲ�"),nRow_Work);
		else if(_T("2") == strVOLUME_CLASS)
			m_pDM_WORK->SetCellData(_T("UDF_��ȣ����"),_T("�պ�ȣ"),nRow_Work);
		else if(_T("3") == strVOLUME_CLASS)
			m_pDM_WORK->SetCellData(_T("UDF_��ȣ����"),_T("�̹���"),nRow_Work);



		m_pDM_WORK->SetCellData(_T("UDF_����"),_T("D"),nRow_Work); //D: DB
	
		strBeforeRow.Format(_T("%d"),i);
		m_pDM_WORK->SetCellData(_T("UDF_��"),strBeforeRow,nRow_Work);

		//������, ���Ա��������� �߰��Ѵ�
		api.MAKE_DB_WORK_by_IDX_ACQ(m_pDM_IDX,m_nCurrentParentIdx,m_pDM_WORK,nRow_Work,m_pDM_ACQ,m_nACQ_TYPE);
	

		if(_T("2") == strCHECKIN_STATUS) //��ȣ�̸� 
		{
			//���� ó��
			nLackBook = 0;
			strLackBookCnt = m_pDM_WORK->GetCellData(_T("UDF_������"),nRow_Work);
			if(!strLackBookCnt.IsEmpty()) nLackBook = _ttoi(strLackBookCnt.GetBuffer(0));
			
			if(nLackBook>0){
				m_pDM_WORK->SetCellData(_T("UDF_�Լ�����"),_T("����"),nRow_Work);
			}
		}

	}

	return 0;
}

	
INT CCheckinVolESL::CopyDMToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
{
	if (!pDM_SOURCE || !pDM_TARGET) return -1;

	INT nSourceColumnCount = pDM_SOURCE->GetRefColumnCount();
	CString field_alias, field_name, field_type, data_type, strData;
	INT ids;
	for (INT col = 0; col < nSourceColumnCount; col++) {
		strData = _T("");
		ids = pDM_SOURCE->FindColumnFieldName(col, field_alias, field_name, field_type, data_type, SourceRowIdx, strData);
		ids = pDM_TARGET->SetCellData(field_alias, strData, TargetRowIdx);
	}
	
	return 0;
}

VOID CCheckinVolESL::OnSelchangetabCHECKINVOL(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	MakeWorkDM();
	
	//test
	//m_nCurrentIdx = -1;

	*pResult = 0;
}

VOID CCheckinVolESL::MakeWorkDM()
{
	//�۾� DM�� data�� �����Ѵ� 
	m_pDM_WORK->FreeData(); 
	
	//test
	//DB DM�� ��ȣ�Ϸù�ȣ�� ���� sort�Ѵ�
	m_pDM_DB->SORT(_T("SV_��ȣ�Ϸù�ȣ"));

	//�� ���¿� ���� �� ������ ������ �����Ѵ�
	INT nCurSel = m_tabCTRL.GetCurSel();
	
	switch(nCurSel)
	{
	case 0:
		CopyExpToWork();
		CopyMissToWork();	
		break;
	case 1:
		CopyAcqToWork();
		break;
	case 2:
		CopyAllDBToWork();
		break;
	case 3:
		CopyMissToWork();
		break;
	}
	
	m_pGrid->Display();
	
	//color setting 
	SetAllGridColor();

}


VOID CCheckinVolESL::SetAllGridColor()
{
	//color setting 

	CString strUDF;
	CString strCHECKIN_STATUS;
	CString strLackBookCnt;
	CString strVOLUME_CLASS;
	INT nLackBook = 0;
	
	

	m_pGrid->ShowWindow(FALSE);
	for(INT i=0;i<m_pDM_WORK->GetRowCount();i++)
	{
		strUDF = _T("");
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_����"),i); 
		
		if(_T("E") == strUDF) //�����ڷ�
		{
			SetGridRowUniqueColor(CR_exp,i);
		}
		else if(_T("D") ==strUDF) //DB �ڷ�
		{
			strCHECKIN_STATUS = _T("");
			strCHECKIN_STATUS = m_pDM_WORK->GetCellData(_T("SA_�Լ�����"),i); 
			
			if(_T("1") == strCHECKIN_STATUS) //�Լ�
			{
				strVOLUME_CLASS = _T("");
				strVOLUME_CLASS = m_pDM_WORK->GetCellData(_T("SV_��ȣ����"),i); 
				
				if(_T("3") == strVOLUME_CLASS)
					SetGridRowUniqueColor(CR_nonpub,i); //�̹��� 
				else
					SetGridRowUniqueColor(CR_acq,i); //�Ϲ�,�պ�ȣ

			}
			else if(_T("2") == strCHECKIN_STATUS) //��ȣ 
			{
				nLackBook = 0;		
				strLackBookCnt = m_pDM_WORK->GetCellData(_T("UDF_������"),i);
				if(!strLackBookCnt.IsEmpty()) nLackBook = _ttoi(strLackBookCnt.GetBuffer(0));
					
				if(nLackBook>0){
					SetGridRowUniqueColor(CR_delay,i); //����
				}
				else
				{
					SetGridRowUniqueColor(CR_miss,i); //��ȣ 
				}
				
			}
			else if(_T("3") == strCHECKIN_STATUS) //�̴� 
			{
				
				SetGridRowUniqueColor(CR_lack,i);

			}
		}

	}
	m_pGrid->ShowWindow(TRUE);


}

INT CCheckinVolESL::CMD_CHECKIN()
{
	//----------------------------------------------------------------------
	// 2005-10-11 �Լ�ó�� ��ư�� ������ ����� �� �ֵ��� ����
	//----------------------------------------------------------------------
	//��ȣ�ڷ�� �����ڷῡ ���ؼ��� �Լ�ó�� �ϴ� �Լ��̴�.
	if( m_pGrid->SelectMakeList() < 0 )
		return 0;
	if( m_pGrid->SelectGetCount() < 1 )
	{ 
		AfxMessageBox(_T("������ ������ �����ϴ�."));
		return 0;
	}
	INT nIdx = m_pGrid->SelectGetHeadPosition();
	CString strUDF;
	CString strStatus;

	while(nIdx>=0)
	{
		strUDF = _T("");
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_����"),nIdx);
		strStatus = m_pDM_WORK->GetCellData(_T("UDF_�Լ�����"),nIdx);
		if(_T("E") == strUDF || _T("M") == strUDF) CheckinForUser(nIdx);
		if(_T("D") == strUDF || _T("U") == strUDF) 
		{
			if( strStatus != _T("�̴�") )
				CheckinDBForUser(nIdx);
			m_bIsNeedReceiptNo = TRUE;
		}
	
		nIdx = m_pGrid->SelectGetNext();
	}

	CMD_SAVE();

	return 0;
}

INT CCheckinVolESL::CMD_MISS()
{
	//----------------------------------------------------------------------
	// 2005-10-11 ��ȣó�� ��ư�� ������ ����� �� �ֵ��� ����
	//----------------------------------------------------------------------
	//�Լ��ڷ�� �����ڷῡ ���ؼ��� ��ȣó�� �ϴ� �Լ��̴�.
	if( m_pGrid->SelectMakeList() < 0 )
		return 0;
	if( m_pGrid->SelectGetCount() < 1 )
	{ 
		AfxMessageBox(_T("������ ������ �����ϴ�."));
		return 0;
	}
	INT nIdx = m_pGrid->SelectGetHeadPosition();
	CString strUDF;

	while(nIdx>=0)
	{
		strUDF = _T("");
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_����"),nIdx);
		if(_T("E") == strUDF || _T("C") == strUDF) MissbookForUser(nIdx); 
		if(_T("D") == strUDF || _T("U") == strUDF) MissbookDBForUser(nIdx);
		
		nIdx = m_pGrid->SelectGetNext();
	}
	CMD_SAVE();


	return 0;
}

INT CCheckinVolESL::CMD_SAVE()
{
	//���� �׸����� ��� ������ ó���ϴ� �Լ� 
	//DB ������ ��쿡�� ���� �Ǿ��� ���� ó���ϰ� 
	//���������� ó���� ������ �ڵ� ��ȣó���� ����Ѵ�. 
	//�۾� ó���� �����͸� �ش� DM���� ������ 
	
	//���Գ⵵�� ������ȣ�� �ԷµǾ� �ִ����� Ȯ���Ѵ�.
	m_strRECEIPT_NO.TrimLeft();
	m_strRECEIPT_NO.TrimRight();
	if(m_strRECEIPT_NO.IsEmpty())
	{
		//modified by loveisverb 2004-09-15
		//������ȣ �ڵ� �ο� ������� 
		if(IsNeedReceipNo())
		{
			CString strMsg;
			strMsg.Format(_T("������ȣ�� �Էµ��� �ʾҽ��ϴ�!\n�� ������ȣ�� �����Ͻðڽ��ϱ�?"));
			if(AfxMessageBox(strMsg,MB_YESNO) == IDYES)
			{
				if(GetNewReceiptNo()<0)
				{
					AfxMessageBox(_T("������ȣ �ڵ� �ο��� ���� �Ͽ����ϴ�!"));
					return 0;
				}
			}
			else
			{
				//����� �׸��带 �����Ѵ�. Modified by hjy.
				RefreshAll();
				return 0;
			}
		}

	}
	

	CESL_DataMgr* pDM_VOL		  = FindDM(_T("DM_üũ��_��"));
	CESL_DataMgr* pDM_ACQ_VOL	  = FindDM(_T("DM_üũ��_���Ա��к���"));
	CESL_DataMgr* pDM_BOOK		  = FindDM(_T("DM_üũ��_å"));
	CESL_DataMgr* pDM_BOOK_INSERT = FindDM(_T("DM_üũ��_å_�Է�"));
	CESL_DataMgr* pDM_APPENDIX	  = FindDM(_T("DM_üũ��_�η�"));

	CSeApi api(this);
	
	CString strUDF;
	CString msg;
	INT nRow = m_pDM_WORK->GetRowCount();
	INT ids;

	m_pGrid->UpdateWindow();
	
	INT nProcCnt = 0;
	CString strUDFforVerify;
	//���¸� �˻��Ѵ�.
	BOOL bIsAlert = FALSE;
	for(INT i=0;i<nRow;i++)
	{
		strUDF = _T("");
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_����"),nRow-i-1);	
		
		if(_T("E") == strUDF) bIsAlert = TRUE;
		if(_T("C") == strUDF || _T("M") == strUDF || _T("X") == strUDF)
		{
			
			nProcCnt++;
			if(bIsAlert) { //ó������ ���� ���� ���� ���� ���� 
				
				if(CheckUnprocessedData()<0) return 0;
			}
		}
		
		if(_T("U") == strUDF || _T("Z") == strUDF) nProcCnt++;
			
	}
	

	if(nProcCnt>0) {
		m_pProgressDlg->SetProGressRange(nProcCnt);
		m_pProgressDlg->ShowWindow(SW_SHOW);
		m_pProgressDlg->BringWindowToTop();
		m_pProgressDlg->CenterWindow();
		m_pProgressDlg->SetProGressPos(0);
	}
	
	CString strProgress;
	INT nProcCount = 1;

	for(i=0;i<nRow;i++)
	{
		strUDF = _T("");
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_����"),nRow-i-1);	
		if(_T("E") == strUDF) break;
		if(_T("D") == strUDF) continue;
		
		strProgress.Format(_T("[ %d ] ���� �ڷḦ ó���ϴ� ���Դϴ�"),nProcCount);
		m_pProgressDlg->SetMessage(strProgress);
		nProcCount++;

		if(_T("C") == strUDF || _T("M") == strUDF) //insert
		{

			ids = api.CHECK_IN(m_pDM_IDX,
							   m_nCurrentParentIdx,
							   m_pDM_WORK,nRow-i-1,
						       m_pDM_ACQ,
						       m_nACQ_TYPE,
						       pDM_VOL,
						       pDM_ACQ_VOL,
						       pDM_BOOK,
						       m_arrayVolExpKeyList,
						       m_arrayOtherVolExpKeyList,
						       pDM_BOOK_INSERT,
						       pDM_APPENDIX,
							   m_strACQ_YEAR,
							   m_strRECEIPT_NO,
							   m_bIsDepartMngUse,
							   m_pDM_DEPART);
		
		}
		else if(_T("U") == strUDF) //update
		{
			ids = api.VOL_UPDATE(m_pDM_IDX,
							     m_nCurrentParentIdx,
							     m_pDM_WORK,nRow-i-1,
							     m_pDM_ACQ,
						         m_nACQ_TYPE,
						         pDM_VOL,
						         pDM_ACQ_VOL,
						         pDM_BOOK,
								 pDM_BOOK_INSERT,
								 m_strACQ_YEAR,
								 m_strRECEIPT_NO,
								 m_bIsLightVersion,
								 m_bIsDepartMngUse,
								 m_pDM_DEPART);
		}
		else if(_T("X") == strUDF) //Exp delete
		{
			
			ids = api.VOL_DELETE_EXP(m_pDM_WORK,
									 nRow-i-1,
									 m_pDM_ACQ,
									 m_nACQ_TYPE,
									 m_arrayVolExpKeyList,
									 m_arrayOtherVolExpKeyList);

		}
		else if(_T("Z") == strUDF) //DB delete 
		{

			ids = api.VOL_DELETE(m_pDM_WORK,
								 nRow-i-1,
								 m_nACQ_TYPE);

		}


		m_pProgressDlg->SetProGressPos(i+1);
		
		
		if(ids<0) //����ó�� 
		{
			
			if(_T("Z") == strUDF)
			{
				if(1002 == ids) AfxMessageBox(_T("�ش� ��ȣ�� ������ �� ���� å�� �ֽ��ϴ�!\n Ȯ���� �ֽʽÿ�."));

			}


			m_pProgressDlg->ShowWindow(SW_HIDE);
			msg.Format(_T("[ %d ] ��° �� �۾����� ������ ���� �۾��� �ߴ��մϴ�"),nRow-i);
			AfxMessageBox(msg);
			return -1;

		}
		else //���� �� ���� ���� ���� ó�� 
		{
			if(_T("C") == strUDF || _T("M") == strUDF) //���������� EXP --> DB�� MOVE
			{
				
				CString strUDFRow = m_pDM_WORK->GetCellData(_T("UDF_��"),nRow-i-1);
				INT nUDFRow = _ttoi(strUDFRow.GetBuffer(0));
				if(nUDFRow>=0) 
				{
					INT nBeforeRow = m_pDM_EXP->GetRowCount()-1;
					
					//���� ���� ���������� ����
					CString strACQ_EXP_DATE = _T("");
					strACQ_EXP_DATE = m_pDM_EXP->GetCellData(_T("SA_�Լ�������"),nBeforeRow);
					if(!strACQ_EXP_DATE.IsEmpty()) m_pDM_EXP->DeleteRow(nBeforeRow);
				}


				INT nDBrow;
				m_pDM_DB->InsertRow(-1);
				nDBrow = m_pDM_DB->GetRowCount()-1;
				CopyDMToDMByAlias(m_pDM_WORK,nRow-i-1,m_pDM_DB,nDBrow);
				
			}
			else if(_T("U") == strUDF) //update�� refresh(copy)
			{
				
				CString strACQ_VOL_KEY_WORK;
				CString strACQ_VOL_KEY_DB;
				CString strWORKrow;
				strWORKrow = m_pDM_WORK->GetCellData(_T("UDF_��"),nRow-i-1);
				strACQ_VOL_KEY_WORK = m_pDM_WORK->GetCellData(_T("SA_���Ա��к���KEY"),nRow-i-1);
				

				//�ϴ��� �˻� ������� 
				for(INT j=0;j<m_pDM_DB->GetRowCount();j++)
				{
					strACQ_VOL_KEY_DB = _T("");
					strACQ_VOL_KEY_DB = m_pDM_DB->GetCellData(_T("SA_���Ա��к���KEY"),j);
					
					if(strACQ_VOL_KEY_WORK == strACQ_VOL_KEY_DB)
						CopyDMToDMByAlias(m_pDM_WORK,nRow-i-1,m_pDM_DB,j);

				}

			}
			else if(_T("X") == strUDF) //Exp delete Exp DM�� ���� 
			{
				
				//������ ���� ������ ã�Ƽ� ����(�׽�Ʈ) 
				CString strBeforeRow;
				INT nBeforeRow;
				strBeforeRow = m_pDM_WORK->GetCellData(_T("UDF_��"),nRow-i-1);
				nBeforeRow = _ttoi(strBeforeRow.GetBuffer(0));

				if(nBeforeRow>=0) 
				{
					//���� ���� ���������� ����
					CString strACQ_EXP_DATE = _T("");
					strACQ_EXP_DATE = m_pDM_EXP->GetCellData(_T("SA_�Լ�������"),nBeforeRow);
					if(!strACQ_EXP_DATE.IsEmpty()) m_pDM_EXP->DeleteRow(nBeforeRow);
				}
			
			}
			else if(_T("Z") == strUDF) //DB delete DB DM�� ���� 
			{
				CString strACQ_VOL_KEY_WORK;
				CString strACQ_VOL_KEY_DB;
				CString strWORKrow;
				strWORKrow = m_pDM_WORK->GetCellData(_T("UDF_��"),nRow-i-1);
				strACQ_VOL_KEY_WORK = m_pDM_WORK->GetCellData(_T("SA_���Ա��к���KEY"),nRow-i-1);
				

				//�ϴ��� �˻� ������� 
				for(INT j=0;j<m_pDM_DB->GetRowCount();j++)
				{
					strACQ_VOL_KEY_DB = _T("");
					strACQ_VOL_KEY_DB = m_pDM_DB->GetCellData(_T("SA_���Ա��к���KEY"),j);
					
					if(strACQ_VOL_KEY_WORK == strACQ_VOL_KEY_DB) m_pDM_DB->DeleteRow(j);
				}

			}

		}

	
	}	
	
	m_pProgressDlg->ShowWindow(SW_HIDE);
	AfxMessageBox(_T("�����Ͽ����ϴ�!"));
	m_nCurrentIdx = -1;
	MakeWorkDM();

	return 0;
}

INT CCheckinVolESL::CMD_PRINT()
{

	INT nRowCnt = m_pDM_WORK->GetRowCount();
	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("����� �ڷᰡ �����ϴ�!"));
		return 0;
	}

	//print
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	

	INT ids;
	pSIReportManager->SetCONNECTION_INFO(m_pDM_WORK->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("����üũ�αǺ����"));
	
	if (ids < 0) {
		ESLAfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return 0;
	}
	
	pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;
	pSIReportManager->SetDataMgr(0, m_pDM_WORK, 0);
	
	
	CString strtitle = _T("");
	strtitle = m_pDM_IDX->GetCellData(_T("ISS_ǥ������"),m_nCurrentParentIdx);
	CString strPublisher = _T("");
	strPublisher = m_pDM_IDX->GetCellData(_T("ISS_�����ڻ���"),m_nCurrentParentIdx);

	
	pSIReportManager->SetTempVariable(_T("����"),strtitle);
	pSIReportManager->SetTempVariable(_T("������"),strPublisher);
	
	INT nCurSel = m_tabCTRL.GetCurSel();
	CString strTabName = _T("");
	
	switch(nCurSel)
	{
	case 0:
		strTabName = _T("����/��ȣ�ڷ��� ");
		break;
	case 1:
		strTabName = _T("�Լ��ڷ���");
		break;
	case 2:
		strTabName = _T("�Լ�/��ȣ�ڷ���");
		break;
	case 3:
		strTabName = _T("��ȣ�ڷ���");
		break;
	default:
		break;
	}

	
	pSIReportManager->SetTempVariable(_T("���̸�"),strTabName);


	ids = pSIReportManager->Print();

	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}

	return 0;
}

INT	CCheckinVolESL::CMD_VOL_INSERT()
{

	INT nCurSel = m_tabCTRL.GetCurSel();
	if(0 != nCurSel)
	{
		AfxMessageBox(_T("�� �Է��� [����/��ȣ] �ڷ� ������ �����մϴ�!"));
		return 0;	
	}

	//������, ���Ա��������� ���� �������� ���Ա��к� �� ������ ����� ����.
	m_pDM_WORK->InsertRow(-1);
	INT nRow_Work = m_pDM_WORK->GetRowCount()-1;
	
	CSeApi api(this);
	api.MAKE_EXP_WORK_by_IDX_ACQ(m_pDM_IDX,m_nCurrentParentIdx,m_pDM_WORK,nRow_Work,m_pDM_ACQ,m_nACQ_TYPE);
	m_pDM_WORK->SetCellData(_T("UDF_�Լ�����"),_T("����"),nRow_Work);
	m_pDM_WORK->SetCellData(_T("UDF_����"),_T("E"),nRow_Work);
	m_pDM_WORK->SetCellData(_T("UDF_��"),_T("-1"),nRow_Work); 
	m_pDM_WORK->SetCellData(_T("UDF_��ȣ����"),_T("�Ϲ�"),nRow_Work);
	m_pDM_WORK->SetCellData(_T("SV_��ȣ����"),_T("1"),nRow_Work); //�Ϲ�

	CSeVolDlg dlg(this);
	dlg.m_pParentGrid = m_pGrid;
	dlg.m_nOpenMode = 1; //�Է�
	dlg.m_nACQ_TYPE = m_nACQ_TYPE;
	dlg.m_bIsLightVersion = m_bIsLightVersion;

	if(dlg.DoModal() == IDOK) 
	{
	//----------------------------------------------------------------------
	// 2005-10-11 ��ȣ�Է� ��ư�� ������ ����� �� �ֵ��� ����
	//----------------------------------------------------------------------
/*		AfxMessageBox(_T("�Էµ� ��ȣ�� �ڵ� �Լ�ó�� ���� �˴ϴ�."));

		CString strUDF = _T("");
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_����"),nRow_Work);
		if(_T("E") == strUDF || _T("M") == strUDF) CheckinForUser(nRow_Work);
		if(_T("D") == strUDF || _T("U") == strUDF) 
		{
			CheckinDBForUser(nRow_Work);
			m_bIsNeedReceiptNo = TRUE;
		}
		CMD_SAVE();*/
	}
	else
	{
		m_pDM_WORK->DeleteRow(nRow_Work);
	}

	return 0;	
}

INT	CCheckinVolESL::CMD_VOL_MODIFY()
{
	INT nCurSel = m_tabCTRL.GetCurSel();
/*	if( 0 == nCurSel || 3 == nCurSel )
	{
		AfxMessageBox(_T("�Ǽ����� [�Լ�] �Ǵ� [�Լ�/��ȣ] �ڷ� ������ �����մϴ�!"));
		return 0;	
	}
*/
	if(m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("������ ������ �����ϴ�!"));
		return 0;
	}

	if(m_nCurrentIdx>=0)
	{
		SetGridRowBoldFont(m_nCurrentIdx, FALSE);
	}
	
	CString strTitleInfo,strPubInfo,strAuthorInfo;
	strTitleInfo = m_pDM_IDX->GetCellData(_T("ISS_ǥ������"),m_nCurrentParentIdx);
	strPubInfo = m_pDM_IDX->GetCellData(_T("ISS_�����ڻ���"),m_nCurrentParentIdx);
	strAuthorInfo = m_pDM_IDX->GetCellData(_T("ISS_�����ڻ���"),m_nCurrentParentIdx);

	CSeVolDlg dlg(this);
	
	m_pGrid->SelectMakeList();
	INT cnt = m_pGrid->SelectGetCount();

	if(cnt>0) dlg.m_nMoveOption = 2;
	else dlg.m_nMoveOption = 1;
	
	dlg.m_nCurrentParentIdx = m_nCurrentIdx;
	dlg.m_pParentGrid = m_pGrid;
	dlg.m_nOpenMode = -1; //����
	dlg.m_nACQ_TYPE = m_nACQ_TYPE;
	dlg.m_strTITLE_INFO = strTitleInfo;
	dlg.m_strAUTHOR_INFO = strAuthorInfo;
	dlg.m_strPUB_INFO = strPubInfo;
	dlg.m_bIsLightVersion = m_bIsLightVersion;

	//----------------------------------------------------------------------
	// 2005-10-11 ��ȣ���� ��ư�� ������ ����� �� �ֵ��� ����
	//----------------------------------------------------------------------
/*	INT ids = dlg.DoModal();
	
	if(dlg.m_nCurrentParentIdx>=0)
	{
		SetGridRowBoldFont(dlg.m_nCurrentParentIdx, FALSE);
	}

	//å���� operation ����
	if(11 == ids)
	{
		RefreshAll();
	}
	
	//å���� ���� �� �۾��� ����Ǿ������� �ľ��� 
	//������ȣ �ʿ� ������ Ȯ���Ѵ�.
	m_bIsNeedReceiptNo = dlg.m_bIsBookCntIncreased;
*/
	INT ids = dlg.DoModal();
	if( IDOK == ids )
	{
		if(dlg.m_nCurrentParentIdx>=0)
		{
			SetGridRowBoldFont(dlg.m_nCurrentParentIdx, FALSE);
		}
		m_bIsNeedReceiptNo = dlg.m_bIsBookCntIncreased;
		
		if(nCurSel == 2 || nCurSel == 1)
			CMD_SAVE();
	}
	else if(11 == ids)
	{
		RefreshAll();
	}
	else
	{
		;
	}
	return 0;	
}

INT	CCheckinVolESL::CMD_VOL_DELETE()
{
	//���� �Լ��� �����ڷῡ���� ������ ���� �ڷῡ ���� ������ ó���Ѵ�.
	m_pGrid->SelectMakeList();
	INT nIdx = m_pGrid->SelectGetHeadPosition();
	CString strUDF;
	CString strVOL_TITLE;
	CString msg;
	
	INT nCnt = m_pGrid->SelectGetCount();
	if(0 == nCnt)
	{
		AfxMessageBox(_T("������ �ڷᰡ �����ϴ�!"));
		return 0;
	}
	
	msg.Format(_T("[ %d ] ���� ��ȣ�� ���� �Ͻðڽ��ϱ�?"),nCnt);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return 0;
	
	CSeApi api(this);
	INT ids = 0;

	while(nIdx>=0)
	{
		strUDF = _T("");
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_����"),nIdx);
		strVOL_TITLE = m_pDM_WORK->GetCellData(_T("SV_��ȣ��"),nIdx);
		
	
		if(_T("E") == strUDF || _T("M") == strUDF || _T("C") == strUDF) DeleteForUser(nIdx); 
		if(_T("D") == strUDF || _T("U") == strUDF) {
			
			//DB �����ʹ� ���� �������� �����Ѵ�.
			ids = api.VOL_DELETE_VERIFY(m_pDM_WORK,nIdx);
			if(ids<0)
			{
				
				if(-1001 == ids) //��Ȯ�� ���� �Ұ�å ����
				{
				
					if(m_bIsLightVersion)
					{
						
						msg.Format(_T("[ %s ] ��ȣ�� �̹� �ΰ�� å �Ǵ� �η��� �����մϴ�.\n")
								   _T("�̷��� ��ȣ�� å���� Ȥ�� �η� ȭ�鿡�� Ȯ���� å������ ����\n")
								   _T("�� �� �ֽ��ϴ�. Ȯ���� �ֽʽÿ�"),strVOL_TITLE);

					}
					else
					{
						msg.Format(_T("[ %s ] ��ȣ�� \n")
								   _T("<�ǽð��ΰ��ڷ�,�����ΰ��ڷ�,����ΰ��ڷ�>\n")
								   _T("�̿��� å �Ǵ� �η��� �����մϴ�.\n")
								   _T("�̷��� ��ȣ�� å���� ȭ�鿡�� Ȯ���� å������ ����\n")
								   _T("�� �� �ֽ��ϴ�. Ȯ���� �ֽʽÿ�"),strVOL_TITLE);
					}

					AfxMessageBox(msg);

				}
				else if(-1002 == ids) //��� ���� ���� 
				{
						
						msg.Format(_T("[ %s ] ��ȣ�� ��������� �־� \n")
								   _T("�ش� ��ȣ ������ ������ �����˴ϴ�!\n")
							       _T("�׷��� ���� �Ͻðڽ��ϱ�?"),strVOL_TITLE);
						
						if(AfxMessageBox(msg,MB_YESNO) == IDYES) DeleteDBForUser(nIdx);
	
								   
						
				}
				else if(-1003 == ids) //�η� ���� ���� 
				{

										
						msg.Format(_T("[ %s ] ��ȣ�� �η��� �־� \n")
							       _T("�ش� ��ȣ ������ �ηϱ��� ������ �� �ֽ��ϴ�!\n")
							       _T("�׷��� ���� �Ͻðڽ��ϱ�?"),strVOL_TITLE);
						
						if(AfxMessageBox(msg,MB_YESNO) == IDYES) DeleteDBForUser(nIdx);

				}
				else if(-1004 == ids) //��� �� �η� ���� ���� 
				{
						
						msg.Format(_T("[ %s ] ��ȣ�� ���� �η� �־�\n")
							       _T("�ش� ��ȣ ������ �Բ� ������ �� �ֽ��ϴ�!\n")
							       _T("�׷��� ���� �Ͻðڽ��ϱ�?"),strVOL_TITLE);
						
						if(AfxMessageBox(msg,MB_YESNO) == IDYES) DeleteDBForUser(nIdx);				
				}
			

			}
			else 
			{
				DeleteDBForUser(nIdx);
			}
		}

		nIdx = m_pGrid->SelectGetNext();
	}	
	//----------------------------------------------------------------------
	// 2005-10-11 ��ȣ ���� ��ư�� ������ ����� �� �ֵ��� ����
	//----------------------------------------------------------------------
	CMD_SAVE();

	return 0;	
}

INT	CCheckinVolESL::CMD_VOL_MERGE()
{
	//�պ�ȣ �Լ� 
	//���� tab�� ���ؼ��� ������
	INT nCurSel = m_tabCTRL.GetCurSel();
	if(0 != nCurSel)
	{
		AfxMessageBox(_T("�պ�ȣ ó���� [����/��ȣ] �ڷ� ������ �����մϴ�!"));
		return 0;	
	}
	
	INT nSelected = 0;
	m_pGrid->SelectMakeList();
	nSelected = m_pGrid->SelectGetCount();

	if(nSelected<2) 
	{
		AfxMessageBox(_T("�� �� �̻��� �׸��� ���õ��� �ʾҽ��ϴ�!"));
		return 0;
	}

	INT idx = m_pGrid->SelectGetHeadPosition();
	INT nGap;
		
	while(idx>=0)
	{
		
		nGap = idx;
	
		idx =  m_pGrid->SelectGetNext();
		
		nGap = idx - nGap;

		if(nGap>1)
		{
			AfxMessageBox(_T("�պ�ȣó���� ����� ��ȣ�����ؼ��� �����մϴ�!"));
			return 0;
		}
	}

	CString msg;
	msg.Format(_T("���õ� %d ���� ��ȣ�� �պ�ȣ ó�� �Ͻðڽ��ϱ�?"),nSelected);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return 0;


	CSeVolDlg dlg(this);
	dlg.m_pParentGrid = m_pGrid;
	dlg.m_nOpenMode = -2; //�պ�ȣ
	dlg.m_nACQ_TYPE = m_nACQ_TYPE;
	dlg.m_bIsLightVersion = m_bIsLightVersion;

	if(dlg.DoModal() != IDOK) return 0;

	//�պ�ȣ�� ���� ���� �Ϳ��� ��ȣ������ �պ�ȣ�� ����� �������� ���� ó���Ѵ�.
	m_pGrid->SelectMakeList();
	idx = m_pGrid->SelectGetHeadPosition();
	m_pDM_WORK->SetCellData(_T("UDF_��ȣ����"),_T("�պ�ȣ"),idx);
	m_pDM_WORK->SetCellData(_T("SV_��ȣ����"),_T("2"),idx); //�պ�ȣ
	m_pGrid->DisplayLine(idx);
	INT idx_Merge = idx;

	
	//������ �ڷ�� ������ �����Ѵ�.
	CString strUDF; 

	while(idx>=0)
	{
		idx =  m_pGrid->SelectGetNext();
		
		strUDF = _T("");
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_����"),idx);
		
		if(idx>=0)
		{
			if(_T("E") == strUDF || _T("C") == strUDF || _T("M") == strUDF)
			{
				DeleteForUser(idx);
			}
			else
			{
				DeleteDBForUser(idx);
			}
			
			m_pGrid->SetAt(idx, m_pGrid->m_nSelectIdx, _T(""));
		}
	}

	strUDF = _T("");
	strUDF = m_pDM_WORK->GetCellData(_T("UDF_����"),idx_Merge);
	if(_T("E") == strUDF || _T("M") == strUDF) CheckinForUser(idx_Merge);
	if(_T("D") == strUDF || _T("U") == strUDF) 
	{
		CheckinDBForUser(idx_Merge);
		m_bIsNeedReceiptNo = TRUE;
	}	
	//----------------------------------------------------------------------
	// 2005-10-11 �պ�ȣó�� ��ư�� ������ ����� �� �ֵ��� ����
	//----------------------------------------------------------------------
	CMD_SAVE();

	return 0;
}

INT	CCheckinVolESL::CMD_VOL_NON_PUBLISH()
{
	//�̹��� ó��
	//��� ��ȣ�� ���ؼ� ���� 
	m_pGrid->SelectMakeList();
	INT nSelected = m_pGrid->SelectGetCount();

	if(nSelected<1) 
	{
		AfxMessageBox(_T("���õ� �׸��� �����ϴ�!"));
		return 0;
	}

	INT idx = m_pGrid->SelectGetHeadPosition();
	CString strVOLUME_CLASS;
	CString strVOL_TITLE;
	CString strUDF;
	CString msg;
	INT nProcess = 0;


	while(idx>=0)
	{
		strVOLUME_CLASS = _T("");
		strVOLUME_CLASS = m_pDM_WORK->GetCellData(_T("SV_��ȣ����"),idx);
		if(_T("3") == strVOLUME_CLASS) 
		{
			AfxMessageBox(_T("������ �����߿� �̹� �̹���ó���� �ڷᰡ �ֽ��ϴ�! \n Ȯ���Ͽ� �ֽʽÿ�."));
			return 0;
		}
		
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_����"),idx);
		if(_T("X") == strUDF || _T("Z") == strUDF)
		{
			AfxMessageBox(_T("������ �����߿� ���� ���� �ڷᰡ �ֽ��ϴ�! \n Ȯ���Ͽ� �ֽʽÿ�."));
			return 0;
		}

		idx =  m_pGrid->SelectGetNext();
	}

	
	idx = m_pGrid->SelectGetHeadPosition();

	while(idx>=0)
	{
		strVOL_TITLE = _T("");
		strUDF = _T("");
		strVOL_TITLE = m_pDM_WORK->GetCellData(_T("SV_��ȣ��"),idx);
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_����"),idx);


		msg.Format(_T("[ %s ] ��ȣ�� �̹��� ó�� �Ͻðڽ��ϱ�?"),strVOL_TITLE);
		if(AfxMessageBox(msg,MB_YESNO) == IDYES)
		{

			if(_T("D") == strUDF || _T("U") == strUDF) 
			{
				m_pDM_WORK->SetCellData(_T("UDF_�Լ�����"),_T("�Լ�"),idx);
				m_pDM_WORK->SetCellData(_T("UDF_����"),_T("U"),idx);
				m_pDM_WORK->SetCellData(_T("UDF_��ȣ����"),_T("�̹���"),idx);
				m_pDM_WORK->SetCellData(_T("SV_��ȣ����"),_T("3"),idx); //�̹���
				m_pDM_WORK->SetCellData(_T("SA_�Լ�å��"),_T("0"),idx);
				m_pDM_WORK->SetCellData(_T("SA_��ȣå��"),_T("0"),idx);
				m_pGrid->DisplayLine(idx);
				nProcess++;
			}
			else if(_T("X") == strUDF || _T("E") == strUDF || _T("C") == strUDF || _T("M") == strUDF)
			{

				m_pDM_WORK->SetCellData(_T("UDF_�Լ�����"),_T("�Լ�(����)"),idx);
				m_pDM_WORK->SetCellData(_T("UDF_����"),_T("C"),idx);
				m_pDM_WORK->SetCellData(_T("UDF_��ȣ����"),_T("�̹���"),idx);
				m_pDM_WORK->SetCellData(_T("SV_��ȣ����"),_T("3"),idx); //�̹���
				m_pDM_WORK->SetCellData(_T("SA_�Լ�å��"),_T("0"),idx);
				m_pDM_WORK->SetCellData(_T("SA_��ȣå��"),_T("0"),idx);
				m_pGrid->DisplayLine(idx);
				nProcess++;

			}

		}

		idx =  m_pGrid->SelectGetNext();
	}
	
	if(nProcess)
	{
	//----------------------------------------------------------------------
	// 2005-10-11 �̹���ó�� ��ư�� ������ ����� �� �ֵ��� ����
	//----------------------------------------------------------------------
		//msg.Format(_T(" %d ���� �ڷḦ �̹��� ó�� �Ͽ����ϴ�!"),nProcess);
		//AfxMessageBox(msg);
		CMD_SAVE();
	}

	return 0;
}

INT	CCheckinVolESL::CMD_EXP_INFO()
{
	CSeExpectForm  dlg(this);
	dlg.m_nOpenMode = -2; //�Ϲ� ����
	dlg.m_nACQ_TYPE = m_nACQ_TYPE;
	dlg.m_strSPECIES_KEY = m_strSPECIES_KEY;
	dlg.m_strPUB_FREQ = m_strPUB_FREQ;
	dlg.m_strTITLE = m_pDM_IDX->GetCellData(_T("ISS_��ǥ��"),m_nCurrentParentIdx);	 

	if(dlg.DoModal() == IDOK)
	{
		RefreshAll();
	}
	
	return 0;	
}

INT	CCheckinVolESL::CMD_CONTENTS_INFO()
{
	//��������
	//���� tab������ �� �� ���� 
	INT nCurSel = m_tabCTRL.GetCurSel();
	if(0 == nCurSel)
	{
		AfxMessageBox(_T("���������� [����/��ȣ] ���� �� �� �����ϴ�!"));
		return 0;	
	}
	
	if(m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("���õ� �׸��� �����ϴ�!"));
		return 0;
	}

	CCONTENTMGR_SE_EDIT dlg(this);
	dlg.Set_Mgr(_T(""),_T("DM_üũ��_�۾�"),_T(""),_T("SV_��KEY"),m_nCurrentIdx);
	dlg.DoModal();
	
	
	//��簹�� update
	CString strArticleCnt;
	strArticleCnt.Format(_T("%d"),dlg.m_KisaCount);
	m_pDM_WORK->SetCellData(_T("UDF_����"),strArticleCnt,m_nCurrentIdx);

	CString strACQ_VOL_KEY_WORK;
	CString strACQ_VOL_KEY_DB;
	strACQ_VOL_KEY_WORK = m_pDM_WORK->GetCellData(_T("SA_���Ա��к���KEY"),m_nCurrentIdx);
	

	//�ϴ��� �˻� ������� 
	for(INT i=0;i<m_pDM_DB->GetRowCount();i++)
	{
		strACQ_VOL_KEY_DB = _T("");
		strACQ_VOL_KEY_DB = m_pDM_DB->GetCellData(_T("SA_���Ա��к���KEY"),i);
					
		if(strACQ_VOL_KEY_WORK == strACQ_VOL_KEY_DB) {
			m_pDM_DB->SetCellData(_T("UDF_����"),strArticleCnt,i);
		}
	}
	
	//�׸��� refresh
	m_pGrid->DisplayLine(m_nCurrentIdx);
	

	return 0;
}

INT	CCheckinVolESL::CMD_REFRESH()
{
	RefreshAll();
	return 0;	
}

INT	CCheckinVolESL::CMD_MORE_EXP()
{
	CString strAutoUseYN = _T("N");
	CString strAutoCount = _T("0");
	
	LPCTSTR lpszFileName = CFG_FILE_PATH_MORE_EXP;

	CStdioFile file;

	INT nSuccess = file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary);
	
	if(nSuccess)
	{
		CString strLine;
		CString tmp;
		INT nPos;
		INT nReadCnt = 0;

		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
				
		while (file.ReadString(strLine))
		{
			strLine.TrimLeft();
			strLine.TrimRight();
			
			nPos = strLine.Find(_T(":"));

			if(nPos>0)
			{
				tmp = strLine.Left(nPos);
				tmp.TrimLeft();
				tmp.TrimRight();
				if(_T("AUTO_USE") == tmp)
				{
					strAutoUseYN = strLine.Mid(nPos+1);
					strAutoUseYN.TrimLeft();
					strAutoUseYN.TrimRight();
				}
				else if(_T("AUTO_COUNT") == tmp)
				{
					strAutoCount = strLine.Mid(nPos+1);
					strAutoCount.TrimLeft();
					strAutoCount.TrimRight();
				}

			}
		}

		file.Close();
	}
	
	INT nMoreExpCnt;

	if(_T("Y") == strAutoUseYN)
	{
		nMoreExpCnt = _ttoi(strAutoCount.GetBuffer(0));
	}
	else if(_T("N") == strAutoUseYN)
	{
		CCheckinVolMoreExp dlg(this);
		if(dlg.DoModal() != IDOK) return 0;
		nMoreExpCnt = _ttoi(dlg.m_strMoreExpCnt.GetBuffer(0));
	}
	
	RefreshAll(nMoreExpCnt);


	return 0;
}

INT	CCheckinVolESL::CMD_MORE_EXP_SETTING()
{
	//��ȣ�� üũ�� ȭ�� ����(�߰�����, �׸��� ����)

	CString strDefaultUseYN = _T("N");
	CString strDefaultCount = _T("0");
	CString strAutoUseYN = _T("N");
	CString strAutoCount = _T("0");
	
	LPCTSTR lpszFileName = CFG_FILE_PATH_MORE_EXP;

	CStdioFile file;

	INT nSuccess = file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary);
	
	if(nSuccess)
	{
		CString strLine;
		CString tmp;
		INT nPos;
		INT nReadCnt = 0;

		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
				
		while (file.ReadString(strLine))
		{
			strLine.TrimLeft();
			strLine.TrimRight();
			
			nPos = strLine.Find(_T(":"));

			if(nPos>0)
			{
				tmp = strLine.Left(nPos);
				tmp.TrimLeft();
				tmp.TrimRight();

				if(_T("DEFAULT_USE") == tmp)
				{
					strDefaultUseYN = strLine.Mid(nPos+1);
					strDefaultUseYN.TrimLeft();
					strDefaultUseYN.TrimRight();
				}
				else if(_T("DEFAULT_COUNT") == tmp)
				{
					strDefaultCount = strLine.Mid(nPos+1);
					strDefaultCount.TrimLeft();
					strDefaultCount.TrimRight();
				}
				else if(_T("AUTO_USE") == tmp)
				{
					strAutoUseYN = strLine.Mid(nPos+1);
					strAutoUseYN.TrimLeft();
					strAutoUseYN.TrimRight();
				}
				else if(_T("AUTO_COUNT") == tmp)
				{
					strAutoCount = strLine.Mid(nPos+1);
					strAutoCount.TrimLeft();
					strAutoCount.TrimRight();
				}

			}
		}

		file.Close();
	}
	
	CCheckinVolSetMoreExp dlg(this);
	

	//�⺻�߰�����
	if(_T("Y") == strDefaultUseYN)
	{
		dlg.m_bDEFAULT_USE_YN = TRUE;
	}
	else if(_T("N") == strDefaultUseYN)
	{
		dlg.m_bDEFAULT_USE_YN = FALSE;
	}

	dlg.m_strDEFAULT_EXP_NO = strDefaultCount;
	

	//�ڵ��߰�����
	if(_T("Y") == strAutoUseYN)
	{
		dlg.m_bAUTO_USE_YN = TRUE;
	}
	else if(_T("N") == strAutoUseYN)
	{
		dlg.m_bAUTO_USE_YN = FALSE;
	}
	
	dlg.m_strMORE_EXP_NO = strAutoCount;

	//�׸��� ����
	dlg.CR_exp = CR_exp; //���� 
	dlg.CR_acq = CR_acq; //�Լ� 
	dlg.CR_lack = CR_lack; //�̴� 
	dlg.CR_miss = CR_miss; //��ȣ 
	dlg.CR_delay = CR_delay; //���� 
	dlg.CR_nonpub = CR_nonpub; //�̹���

	if(dlg.DoModal() != IDOK) return 0;
	

	//�⺻ �߰� ���� ���� ����
	if(dlg.m_bDEFAULT_USE_YN)
		strDefaultUseYN = _T("Y");
	else
		strDefaultUseYN = _T("N");
	

	strDefaultCount = dlg.m_strDEFAULT_EXP_NO;

	
	//�߰� ���� ���� ����
	if(dlg.m_bAUTO_USE_YN)
		strAutoUseYN = _T("Y");
	else
		strAutoUseYN = _T("N");
	
	strAutoCount = dlg.m_strMORE_EXP_NO;
	
	CStdioFile fileWrite;
	CString tmp;
	nSuccess = fileWrite.Open(lpszFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);

	if(nSuccess)
	{
		if( 0 == fileWrite.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			fileWrite.Write( &cUni, sizeof(TCHAR));
		}
		
		tmp.Format(_T("DEFAULT_USE : %s\r\n"),strDefaultUseYN);
		fileWrite.WriteString(tmp);
	
		tmp.Format(_T("DEFAULT_COUNT : %s\r\n"),strDefaultCount);
		fileWrite.WriteString(tmp);

		tmp.Format(_T("AUTO_USE : %s\r\n"),strAutoUseYN);
		fileWrite.WriteString(tmp);
	
		tmp.Format(_T("AUTO_COUNT : %s\r\n"),strAutoCount);
		fileWrite.WriteString(tmp);
	
		fileWrite.Close();
	}

	//�׸��� ���� ����
	CR_exp = dlg.CR_exp; //���� 
	CR_acq = dlg.CR_acq; //�Լ� 
	CR_lack = dlg.CR_lack; //�̴� 
	CR_miss = dlg.CR_miss; //��ȣ 
	CR_delay = dlg.CR_delay; //���� 
	CR_nonpub = dlg.CR_nonpub; //�̹���

	//�׸��� ���� ���� ����
	WriteColorRefOnFile();

	AfxMessageBox(_T("�����Ǿ����ϴ�!"));

	return 0;
}

INT CCheckinVolESL::CMD_RECEIPT_NO()
{
	//������ȣ ����
	CCheckinReceipt dlg(this);
	dlg.m_strACQ_YEAR = m_strACQ_YEAR;
	dlg.m_nACQ_TYPE = m_nACQ_TYPE;

	if(dlg.DoModal() == IDOK)
	{
		m_strACQ_YEAR = dlg.m_strACQ_YEAR;
		m_strRECEIPT_NO = dlg.m_strRECEIPT_NO;

		//������ ���̾�α� refresh 
		if(m_pResourceMgr) {

			m_pResourceMgr->m_dlgSpeciesInfo.SetReceiptNo(m_strACQ_YEAR,m_strRECEIPT_NO);
			m_pResourceMgr->m_dlgSpeciesInfo.ShowSpeciesInfo(m_nCurrentParentIdx);
		
		}	 
	}

	return 0;
}

INT	CCheckinVolESL::CMD_PREV()
{
	//===================================================
	//2010.01.11 UPDATE BY PJW : INDEX�̵� ������
//	INT nPos = m_pParentGrid->SelectGetPrev();
//	if(nPos<0) {
//
//		m_pParentGrid->SelectMakeList();
//		m_nCurrentParentIdx = m_pParentGrid->SelectGetHeadPosition();
//		if(m_nCurrentParentIdx<0) 
//		{	
//			nPos = m_pParentGrid->GetIdx() - 1;
//			if ( nPos == -1 ) nPos = 0;
//		}
//		else
//		{
//
//			m_pParentGrid->SelectGetHeadPosition();
//			AfxMessageBox(_T("�� ó�� ���� �Դϴ�!"));
//			return 0;
//		}
//	}
	INT nPos;
	INT nHeadIdx;
	m_pParentGrid->SelectMakeList();
	nHeadIdx = m_pParentGrid->SelectGetHeadPosition();
	nPos = nHeadIdx;
	while( 1 )
	{
		if( nPos == m_nCurrentParentIdx )
		{
			if( m_nCurrentParentIdx == nHeadIdx )
			{
				AfxMessageBox(_T("�� ó�� ���� �Դϴ�!"));
				return 0;
			}
			else
			{
				nPos = m_pParentGrid->SelectGetPrev();
			}
			break;
		}
		nPos = m_pParentGrid->SelectGetNext();
	}

	//===================================================
	
	m_nCurrentParentIdx = nPos;
	RefreshAll();

	return 0;
}

INT	CCheckinVolESL::CMD_NEXT()
{
	//===================================================
	//2010.01.11 UPDATE BY PJW : INDEX�̵� ������
//	INT nPos = m_pParentGrid->SelectGetNext();
//	if(nPos<0) {
//
//		m_pParentGrid->SelectMakeList();
//		m_nCurrentParentIdx = m_pParentGrid->SelectGetHeadPosition();
//		if(m_nCurrentParentIdx<0) 
//		{	
//			nPos = m_pParentGrid->GetIdx() + 1;
//		}
//		else
//		{
//			m_pParentGrid->SelectGetTailPosition();
//			AfxMessageBox(_T("�� ������ ���� �Դϴ�!"));
//			return 0;
//		}
//	}
	INT nPos;
	INT nTailIdx;
	m_pParentGrid->SelectMakeList();
	nTailIdx = m_pParentGrid->SelectGetTailPosition();
	nPos = nTailIdx;
	while( 1 )
	{
		if( nPos == m_nCurrentParentIdx )
		{
			if( m_nCurrentParentIdx == nTailIdx )
			{
				AfxMessageBox(_T("�� ������ ���� �Դϴ�!"));
				return 0;
			}
			else
			{
				nPos = m_pParentGrid->SelectGetNext();
			}
			break;
		}
		nPos = m_pParentGrid->SelectGetPrev();
	}

	//===================================================
	
	m_nCurrentParentIdx = nPos;
	RefreshAll();
	
	return 0;	
}

INT CCheckinVolESL::CMD_COVER_INFO()
{
	//ǥ������ 
	//���� tab������ �� �� ���� 
	
	INT nCurSel = m_tabCTRL.GetCurSel();
	if(0 == nCurSel)
	{
		AfxMessageBox(_T("ǥ������ ������ [����/��ȣ] ���� �� �� �����ϴ�!"));
		return 0;	
	}
	
	if(m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("���õ� �׸��� �����ϴ�!"));
		return 0;
	}

	CString strVOL_KEY;
	strVOL_KEY = m_pDM_WORK->GetCellData(_T("SV_��KEY"),m_nCurrentIdx);

	if(strVOL_KEY.IsEmpty())
	{
		AfxMessageBox(_T("�ش� ��ȣŰ�� ã�� �� �����ϴ�!"));
		return 0;
	}

	CCOVER_MAIN dlg(this);
	dlg.SetInsertMode(FALSE);
	dlg.SetPubType(_SERIAL_);
	dlg.SetLocalPath(_T("..\\bmp\\cover_info\\"));
	dlg.SetBasicName(strVOL_KEY.GetBuffer(0));
	dlg.SetSpeciesKey(strVOL_KEY.GetBuffer(0));
	dlg.DoModal();


	return 0;
}


INT CCheckinVolESL::CMD_PROCESSPRINT()
{
	INT ids = -1;
	CString strWhere;
	//����/������/����ó/���Ա���/�����/�����Ⱓ/��������/���Լ�å��/�����켱
	CString strtitle = _T("");
	CString strPublisher = _T("");
	CString strPurPlace = _T("");
	CString strAcqCode = _T("");
	CString strFreq = _T("");
	CString strPurStart = _T("");
	CString strPurEnd = _T("");
	CString strPurPrice = _T("");
	CString strTotalBookCnt = _T("");
	CString strLateBookCnt =_T("");
	CString strMissBookCnt =_T("");
	CString strAppendBookCnt=_T("");	
	CString strIsLocFirst = _T("");
	CString strISSN = _T("");
	

	m_pDM_DB->FreeData();

	CString strACQ_KEY = _T("");
	CString strSpeciesKey = _T("");
	if(1 == m_nACQ_TYPE)
	{
		strACQ_KEY = m_pDM_ACQ->GetCellData(_T("SP_���԰���KEY"),0);
		//strTotalBook = m_pDM_ACQ->GetCellData(_T("SP_�Լ�å��"),0);
		strIsLocFirst = m_pDM_ACQ->GetCellData(_T("SP_�����켱����"),0);
		strPurStart = m_pDM_ACQ->GetCellData(_T("SP_����������"),0);
		strPurEnd = m_pDM_ACQ->GetCellData(_T("SP_����������"),0);
		strPurPrice = m_pDM_ACQ->GetCellData(_T("SP_��������"),0);
	}
	else
	{
		strACQ_KEY = m_pDM_ACQ->GetCellData(_T("SD_��������KEY"),0);
		//strTotalBook = m_pDM_ACQ->GetCellData(_T("SD_�Լ�å��"),0);
		strIsLocFirst = m_pDM_ACQ->GetCellData(_T("SD_�����켱����"),0);
		strPurStart = m_pDM_ACQ->GetCellData(_T("SD_����������"),0);
		//strPurEnd = m_pDM_ACQ->GetCellData(_T("SD_����������"),0);
		//strPurPrice = m_pDM_ACQ->GetCellData(_T("SD_��������"),0);
	}

	UpdateData(TRUE);
	CString strPUBLISH_DATE;
	m_pCM->GetControlMgrData(_T("����⵵"),strPUBLISH_DATE);

	strWhere.Format(_T("A.VOL_KEY = V.REC_KEY AND V.VOL_TYPE = 'V' AND A.ACQ_KEY = %s"),strACQ_KEY); 
	//	m_pDM_DB->RefreshDataManager(strWhere);
	
	m_pDM_DB->RefreshDataManager(strWhere);
	
	m_pDM_WORK->FreeData();

	CopyAllDBToWork();

	INT nRowCnt = m_pDM_WORK->GetRowCount();
	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("����� �ڷᰡ �����ϴ�!"));
		return 0;
	}
	
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	
	pSIReportManager->SetCONNECTION_INFO(m_pDM_WORK->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("����_üũ��_��Ȳǥ"));
	
	if (ids < 0) {
		ESLAfxMessageBox(_T("DB�� ���� SI Report ������ �������� ���Ͽ����ϴ�."));
		return 0;
	}
	pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;

	strSpeciesKey = m_pDM_IDX->GetCellData(_T("ISS_��KEY"),m_nCurrentParentIdx);

	strWhere.Format(_T("select count(*) from  se_book_tbl where book_type = 'C' and species_key = %s"),strSpeciesKey);
	m_pDM_WORK->GetOneValueQuery(strWhere,strTotalBookCnt);
	strWhere.Format(_T("select count(*) from  se_book_tbl where book_type = 'M' and species_key = %s"),strSpeciesKey);
	m_pDM_WORK->GetOneValueQuery(strWhere,strMissBookCnt);
	strWhere.Format(_T("select count(*) from  se_book_tbl where book_type = 'L' and species_key = %s"),strSpeciesKey);
	m_pDM_WORK->GetOneValueQuery(strWhere,strLateBookCnt);
	strWhere.Format(_T("select count(*) from  se_book_tbl where book_type = 'A' and species_key = %s"),strSpeciesKey);
	m_pDM_WORK->GetOneValueQuery(strWhere,strAppendBookCnt);
	
	strtitle = m_pDM_IDX->GetCellData(_T("ISS_ǥ������"),m_nCurrentParentIdx);
	strPublisher = m_pDM_IDX->GetCellData(_T("ISS_�����ڻ���"),m_nCurrentParentIdx);
	strISSN = m_pDM_IDX->GetCellData(_T("ISS_ISSN"),m_nCurrentParentIdx);
	strFreq = m_pDM_IDX->GetCellData(_T("UDF_�����"),m_nCurrentParentIdx);
	strAcqCode = m_pDM_IDX->GetCellData(_T("UDF_���Ա���"),m_nCurrentParentIdx);

	pSIReportManager->SetTempVariable(_T("����"),strtitle);
	pSIReportManager->SetTempVariable(_T("������"),strPublisher);
	pSIReportManager->SetTempVariable(_T("����ó"),strPurPlace);
	pSIReportManager->SetTempVariable(_T("���Ա���"),strAcqCode);
	pSIReportManager->SetTempVariable(_T("�����"),strFreq);
	strPurStart = strPurStart + _T(" ~ ") + strPurEnd;
	pSIReportManager->SetTempVariable(_T("�����Ⱓ"),strPurStart);
	pSIReportManager->SetTempVariable(_T("��������"),strPurPrice);
	pSIReportManager->SetTempVariable(_T("���Լ�å��"),strTotalBookCnt);
	pSIReportManager->SetTempVariable(_T("�����켱"),strIsLocFirst);
	pSIReportManager->SetTempVariable(_T("ISSN"),strISSN);
	pSIReportManager->SetTempVariable(_T("��ȣå��"),strMissBookCnt);
	pSIReportManager->SetTempVariable(_T("����å��"),strLateBookCnt);
	pSIReportManager->SetTempVariable(_T("�η�å��"),strAppendBookCnt);

	pSIReportManager->SetDataMgr(0, m_pDM_WORK);
	ids = pSIReportManager->MakeXMLPageFiles();
	// ������ ������ �׸�
	Print_SetLastPageLine();
	ids = pSIReportManager->Print2();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}

	//RefreshAll();
	return 0;
}

VOID CCheckinVolESL::Print_SetLastPageLine()
{
	INT ids;

	// XML �� WorkForm��ü�� �����Ѵ�.
	CSIReportXMLIOMgr *pXMLIOMgr = new CSIReportXMLIOMgr(MASTER_IS_MANAGER);
	CSIReportWorkForm *pPageWorkForm = new CSIReportWorkForm(MASTER_IS_MANAGER);
	
	if (pXMLIOMgr == NULL) {
		AfxMessageBox(_T("XML ó���� ������ �����Ͽ����ϴ�."));
		return;
	}
	if (pPageWorkForm == NULL) {
		AfxMessageBox(_T("�߰� XML ó���� ������ �����Ͽ����ϴ�."));
		return;
	}

	// ���� ������ ������ ��ȣ�� �����´�.
	INT nMaxTmpPageNum = pXMLIOMgr->GetMaxTmpPageNum();
	if (nMaxTmpPageNum <= 0) {
		AfxMessageBox(_T("������ ������ ã�� �� �����ϴ�."));
		return;
	}
	CString strFileName;
	strFileName.Format(_T("rpt_tmp\\page%04d.xml"), nMaxTmpPageNum);
	// ���� ������ ������ ������ �ҷ��´�.
	ids = pPageWorkForm->LoadWorkForm(strFileName);
	if (ids != 0) {
		AfxMessageBox(_T("�������� �ε� ���� ���Ͽ����ϴ�."));
		return;
	}
	// �ҷ��� WorkForm�߿��� First Sheet�� �����´�.(�߰� ������ ���Ͽ��� Sheet�� �Ѱ����̴�.)
	CSIReportWorkSheet *pSheet = pPageWorkForm->FindSheet(0);
	if (pSheet == NULL) {
		AfxMessageBox(_T("�������� ã�� �� �����ϴ�."));
		return;
	}
	// ������ ������ ó���� �̹� �Ǿ� �ִ��� ����. �̹� ó���Ǿ� �ִٸ� �ι� �� �ʿ� ����.
	CSIReportObject * pObject = pSheet->FindObject(_T("�߰��ȸ�����������"));
	if (pObject) return;
	// �ܺ����̺� �� �ݺ� ���̺��� �����´�.(���̺� �̸��� ��Ȳ�� ���� ������ ��)
	CSIReportObject_Table * pIn = (CSIReportObject_Table*)pSheet->FindObject(_T("���̺�_1"));
	CSIReportObject_Table * pOut = (CSIReportObject_Table*)pSheet->FindObject(_T("���̺�_19"));
	if (pIn == NULL) return;
	if (pOut == NULL) return;

	CRect InLeft, InRight, OutLeft, OutRight, NewRect;

	CSIReportObject_Table_Cell *pCell;

	// �ݺ����̺��� ������ ������ ���� ó���� ������ ���������� Rect�� ���ϰ�
	pCell = NULL;
	pCell = pIn->GetCell(pIn->row_count-1, 0);
	if (pCell) InLeft = pCell->GetRect();
	
	pCell = NULL;
	pCell = pIn->GetCell(pIn->row_count-1, pIn->col_count-1);
	if (pCell) InRight = pCell->GetRect();

	// �ܺ����̺��� ������ ������ ���� ó�� ���� Rect�� ���Ѵ�.
	pCell = NULL;
	pCell = pOut->GetCell(pOut->row_count-1, 0);
	if (pCell) OutLeft = pCell->GetRect();

	// �ܺ����̺��� �ݺ����̺��� ������ ������ �������� �߰����ش�.
	if (InLeft.bottom < OutLeft.bottom) {
		// ������ �������� Rect�� ����Ѵ�.
		NewRect.top = InLeft.bottom;
		NewRect.bottom = InLeft.bottom;
		NewRect.left = InLeft.left;
		NewRect.right = InRight.right;

		// ������ �������� �߰� ������ ���Ͽ� �߰��Ѵ�.
		CSIReportObject_Line *pLine = (CSIReportObject_Line *)pSheet->AddNewObject(OBJECT_LINE);
		if (pLine) {
			pLine->SetName(_T("�߰��ȸ�����������"));
			pLine->SetRect(NewRect);
		}

		// ����� �߰������� ������ �����Ѵ�.
		ids = pXMLIOMgr->SaveToXML(pPageWorkForm, strFileName);
	}

	// ������ Object�� ����
	if (pXMLIOMgr) {
		delete pXMLIOMgr;
		pXMLIOMgr = NULL;
	}
	if (pPageWorkForm) {
		delete pPageWorkForm;
		pPageWorkForm = NULL;
	}
	return;
}


INT CCheckinVolESL::CheckUnprocessedData()
{
	//����/��ȣ �ǿ����� ó������ ���� ���� �����ʹ� �ڵ� ��ȣó�� ���θ� ���� 
	//�ƴϸ� return -1;
	
	INT nRow = m_pDM_WORK->GetRowCount();
	CString strUDF;
	BOOL bChecked = FALSE;	
	CArray<INT,INT> arrayUnprocessedIdx;
	

	for(INT i=0;i<nRow;i++)
	{
		strUDF = _T("");
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_����"),i);
		
		if(!bChecked)
		{
			if(_T("C") == strUDF || _T("M") == strUDF || _T("X") == strUDF) bChecked = TRUE;
		}
		else 
		{
			if(_T("E") == strUDF) {
				arrayUnprocessedIdx.Add(i);
			}
		}
	
	}
	
	CString msg;
	INT nSize = arrayUnprocessedIdx.GetSize();
	if( nSize > 0)
	{
		msg.Format(_T("%d ���� ó������ ���� ���� ���� �ڷᰡ ���� �մϴ� \n ��� ������ ��� �̵��� �ڵ� ��ȣ ó�� �˴ϴ�. �����Ͻ� �ڽ��ϱ�?"), nSize);
		if(AfxMessageBox(msg,MB_YESNO) != IDYES)
		{
			CMD_REFRESH();
			return -1;
		}
	}

	INT nRowIdx;
	for(i=0;i<nSize;i++)
	{
		nRowIdx = arrayUnprocessedIdx.GetAt(i);
		MissbookForUser(nRowIdx);
	}

	return 0;
}

INT CCheckinVolESL::CheckinForUser(INT nRowIdx)
{
	CString strACQ_EXP_BOOK_CNT;
	strACQ_EXP_BOOK_CNT = m_pDM_WORK->GetCellData(_T("SA_�Լ�����å��"),nRowIdx);
	if(!strACQ_EXP_BOOK_CNT.IsEmpty()) m_pDM_WORK->SetCellData(_T("SA_�Լ�å��"),strACQ_EXP_BOOK_CNT,nRowIdx);
	m_pDM_WORK->SetCellData(_T("SA_��ȣå��"),_T("0"),nRowIdx);
	m_pDM_WORK->SetCellData(_T("UDF_�Լ�����"),_T("�Լ�(����)"),nRowIdx);
	m_pDM_WORK->SetCellData(_T("UDF_����"),_T("C"),nRowIdx);
	m_pGrid->DisplayLine(nRowIdx);
	
	return 0;
}

INT CCheckinVolESL::CheckinDBForUser(INT nRowIdx)
{
	//��ȣ å���� Ȯ������ �̸� �Լ�å���� ������ ���¸� U �� �ٲپ� �ش�. 
	CString strMISS_BOOK_CNT = _T("");
	strMISS_BOOK_CNT =  m_pDM_WORK->GetCellData(_T("SA_��ȣå��"),nRowIdx);
	INT nMISS_BOOK_CNT = 0;
	nMISS_BOOK_CNT = _ttoi(strMISS_BOOK_CNT.GetBuffer(0));

	if(0 == nMISS_BOOK_CNT) return 0;
	
	CString strACQ_BOOK_CNT = _T("");
	strACQ_BOOK_CNT =  m_pDM_WORK->GetCellData(_T("SA_�Լ�å��"),nRowIdx);
	INT nACQ_BOOK_CNT = _ttoi(strACQ_BOOK_CNT.GetBuffer(0));
	
	CString strNewBookCnt;
	INT nNewBookCnt = nACQ_BOOK_CNT + nMISS_BOOK_CNT;
	strNewBookCnt.Format(_T("%d"),nNewBookCnt);

	m_pDM_WORK->SetCellData(_T("SA_�Լ�å��"),strNewBookCnt,nRowIdx);
	m_pDM_WORK->SetCellData(_T("SA_��ȣå��"),_T("0"),nRowIdx);
	m_pDM_WORK->SetCellData(_T("UDF_�Լ�����"),_T("�Լ�"),nRowIdx);
	m_pDM_WORK->SetCellData(_T("UDF_����"),_T("U"),nRowIdx);
	m_pGrid->DisplayLine(nRowIdx);
	
	return 0;
}


INT CCheckinVolESL::MissbookForUser(INT nRowIdx)
{
	CString strACQ_EXP_BOOK_CNT;
	strACQ_EXP_BOOK_CNT = m_pDM_WORK->GetCellData(_T("SA_�Լ�����å��"),nRowIdx);
	m_pDM_WORK->SetCellData(_T("SA_�Լ�å��"),_T("0"),nRowIdx);
	if(!strACQ_EXP_BOOK_CNT.IsEmpty()) m_pDM_WORK->SetCellData(_T("SA_��ȣå��"),strACQ_EXP_BOOK_CNT,nRowIdx);
	m_pDM_WORK->SetCellData(_T("UDF_�Լ�����"),_T("��ȣ(����)"),nRowIdx);
	m_pDM_WORK->SetCellData(_T("UDF_����"),_T("M"),nRowIdx);
	m_pGrid->DisplayLine(nRowIdx);

	return 0;
}

INT CCheckinVolESL::MissbookDBForUser(INT nRowIdx)
{
	
	//�Լ�å���� ��ȣå�� ������ ���¸� U �� �ٲپ� �ش�. 
	CString strACQ_BOOK_CNT = _T("");
	strACQ_BOOK_CNT =  m_pDM_WORK->GetCellData(_T("SA_�Լ�å��"),nRowIdx);
	INT nACQ_BOOK_CNT = 0;
	nACQ_BOOK_CNT = _ttoi(strACQ_BOOK_CNT.GetBuffer(0));
	
	if(0 == nACQ_BOOK_CNT) 
	{
		CString strUDF;
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_�Լ�����"),nRowIdx);
		if(_T("����") == strUDF) {
			m_pDM_WORK->SetCellData(_T("UDF_�Լ�����"),_T("��ȣ"),nRowIdx);
			m_pDM_WORK->SetCellData(_T("UDF_����"),_T("U"),nRowIdx);
			m_pGrid->DisplayLine(nRowIdx);
		}
		
		return 0;
	}

	CString strMISS_BOOK_CNT = _T("");
	strMISS_BOOK_CNT =  m_pDM_WORK->GetCellData(_T("SA_��ȣå��"),nRowIdx);
	INT nMISS_BOOK_CNT = _ttoi(strMISS_BOOK_CNT.GetBuffer(0));
	
	CString strNewBookCnt;
	INT nNewBookCnt = nACQ_BOOK_CNT + nMISS_BOOK_CNT;
	strNewBookCnt.Format(_T("%d"),nNewBookCnt);

	m_pDM_WORK->SetCellData(_T("SA_��ȣå��"),strNewBookCnt,nRowIdx);
	m_pDM_WORK->SetCellData(_T("SA_�Լ�å��"),_T("0"),nRowIdx);
	m_pDM_WORK->SetCellData(_T("UDF_�Լ�����"),_T("��ȣ"),nRowIdx);
	m_pDM_WORK->SetCellData(_T("UDF_����"),_T("U"),nRowIdx);
	m_pGrid->DisplayLine(nRowIdx);

	return 0;
}

INT CCheckinVolESL::DeleteForUser(INT nRowIdx)
{
	m_pDM_WORK->SetCellData(_T("UDF_�Լ�����"),_T("����(����)"),nRowIdx);
	m_pDM_WORK->SetCellData(_T("UDF_����"),_T("X"),nRowIdx);
	m_pGrid->DisplayLine(nRowIdx);

	return 0;
}

INT CCheckinVolESL::DeleteDBForUser(INT nRowIdx)
{

	m_pDM_WORK->SetCellData(_T("UDF_�Լ�����"),_T("����"),nRowIdx);
	m_pDM_WORK->SetCellData(_T("UDF_����"),_T("Z"),nRowIdx);
	m_pGrid->DisplayLine(nRowIdx);
	
	return 0;
}

BEGIN_EVENTSINK_MAP(CCheckinVolESL, CDialog)
    //{{AFX_EVENTSINK_MAP(CCheckinVolESL)
	ON_EVENT(CCheckinVolESL, IDC_gridCHECKIN_VOL, -600 /* Click */, OnClickgridCHECKINVOL, VTS_NONE)
	ON_EVENT(CCheckinVolESL, IDC_gridCHECKIN_VOL, -601 /* DblClick */, OnDblClickgridCHECKINVOL, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CCheckinVolESL::OnClickgridCHECKINVOL() 
{
	// TODO: Add your control notification handler code here
	INT nDMRow = m_pDM_WORK->GetRowCount();
	if(0 == nDMRow) return;

	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {

		if(m_nCurrentIdx>=0) {
			
			if(m_nCurrentIdx<nDMRow)
				SetGridRowBoldFont(m_nCurrentIdx,FALSE);
		}

		m_nCurrentIdx = row-1;
		SetGridRowBoldFont(m_nCurrentIdx,TRUE);
	
	}

}

VOID CCheckinVolESL::SetGridRowBoldFont(INT nRowIdx, BOOL bNewValue)
{
	INT nCol = m_pGrid->GetCol();
	INT nCols = m_pGrid->GetCols(0);
	
	if (nRowIdx >= 0)
	{
		m_pGrid->SetRow(nRowIdx + 1);
		
		for (INT i = 1 ; i < nCols; i++)
		{
			m_pGrid->SetCol(i);
			m_pGrid->SetCellFontBold(bNewValue);
		}

	}
	
	m_pGrid->SetCol(nCol);
}

INT CCheckinVolESL::SetGridRowUniqueColor(COLORREF& colorref, INT nRowIdx)
{

	INT nCol = m_pGrid->GetCol();
	INT nCols = m_pGrid->GetCols(0);
	
	if (nRowIdx >= 0)
	{
		m_pGrid->SetRow(nRowIdx + 1);
		
		for (INT i = 1 ; i < nCols; i++)
		{
			m_pGrid->SetCol(i);
			m_pGrid->SetCellBackColor(colorref);
			m_pGrid->SetCellForeColor(CR_BLACK);
		}
	}
	
	m_pGrid->SetCol(nCol);
	
	return 0;
}

VOID CCheckinVolESL::OnDblClickgridCHECKINVOL() 
{
	INT nRow = m_pGrid->GetMouseRow();
	
	if( nRow > 0)
	{
		CMD_VOL_MODIFY();
	}
	SetAllGridColor();
}


INT CCheckinVolESL::GetDefaultMoreExpCnt()
{

	CString strDefaultUseYN = _T("N");
	CString strDefaultCount = _T("0");
	
	LPCTSTR lpszFileName = CFG_FILE_PATH_MORE_EXP;

	CStdioFile file;

	INT nSuccess = file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary);
	
	if(nSuccess)
	{
		CString strLine;
		CString tmp;
		INT nPos;
		INT nReadCnt = 0;

		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
				
		while (file.ReadString(strLine))
		{
			strLine.TrimLeft();
			strLine.TrimRight();
			
			nPos = strLine.Find(_T(":"));

			if(nPos>0)
			{
				tmp = strLine.Left(nPos);
				tmp.TrimLeft();
				tmp.TrimRight();
				if(_T("DEFAULT_USE") == tmp)
				{
					strDefaultUseYN = strLine.Mid(nPos+1);
					strDefaultUseYN.TrimLeft();
					strDefaultUseYN.TrimRight();
				}
				else if(_T("DEFAULT_COUNT") == tmp)
				{
					strDefaultCount = strLine.Mid(nPos+1);
					strDefaultCount.TrimLeft();
					strDefaultCount.TrimRight();
				}

			}
		}

		file.Close();
	}
	
	INT nMoreExpCnt = 0;

	if(_T("Y") == strDefaultUseYN)
	{
		if(!strDefaultCount.IsEmpty()) nMoreExpCnt = _ttoi(strDefaultCount.GetBuffer(0));
	}
	
	return nMoreExpCnt;
}

INT CCheckinVolESL::GetNewReceiptNo()
{

	CSeApi api(this);
	
	if(m_strACQ_YEAR.IsEmpty())
	{
		m_strACQ_YEAR = api.GetCurrentYear();
	}

	CString strRECEIPT_NO;
	strRECEIPT_NO = api.GetLastReceiptNo(m_pDM_WORK,m_nACQ_TYPE,m_strACQ_YEAR);


	CString strDate;
	strDate = api.GetTodayDate();

	CString strWorkLog;
	strWorkLog = GetWorkLogMsg(WORK_LOG_CHECKIN);

	m_pDM_WORK->StartFrame();
	m_pDM_WORK->InitDBFieldData();
	// 2005-04-11 By SM5ong
	// ������ ������ȣ+1�� Update
	// BEGIN:
	CString strKindCode, strQuery;
  	if(1 == m_nACQ_TYPE)
		strKindCode = _T("CK1_NO"); //���� ������ȣ 
	else if(2 == m_nACQ_TYPE)
		strKindCode = _T("CK2_NO"); //���� ������ȣ 
  
	if( _T("1") == strRECEIPT_NO )	//�˻������ ������ INSERT 
	{
		strQuery.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL (REC_KEY, KIND_CODE, PUBLISH_FORM_CODE, MAKE_YEAR,LAST_NUMBER,CREATE_DATE,CREATE_WORKER,FIRST_WORK,MANAGE_CODE)")
						_T("VALUES	(%s.NEXTVAL,'%s','SE','%s',1,'%s','%s','%s',UDF_MANAGE_CODE);"), 
						DEFAULT_SEQ_NAME, strKindCode, m_strACQ_YEAR, strDate, m_pInfo->USER_ID, strWorkLog);
		m_pDM_WORK->AddFrame(strQuery); 
	}
	else	//������ UPDATE
	{
		//===================================================
		//2009.12.01 UPDATE BY PJW : MANAGE_CODE ���� �߰�
//		strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = %s, LAST_WORK = '%s' WHERE MAKE_YEAR = '%s' AND KIND_CODE = '%s';"),
		strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = %s, LAST_WORK = '%s' WHERE MAKE_YEAR = '%s' AND KIND_CODE = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE;"),
		//===================================================		
			            strRECEIPT_NO, strWorkLog, m_strACQ_YEAR, strKindCode);
		m_pDM_WORK->AddFrame(strQuery);
	}
	// END:
	strQuery.Format(_T("INSERT INTO SE_RECEIPT_TBL ")
					_T("(REC_KEY, ACQ_CODE, ACQ_YEAR, RECEIPT_NO, LAST_SERIAL_NO, ")
					_T("RECEIPT_NO_CREATE_DATE, RECEIPT_NO_CREATE_WORKER, FIRST_WORK, LAST_WORK,MANAGE_CODE) ")
					_T("VALUES (%s.NEXTVAL,'%d','%s','%s',0,'%s','%s','%s','%s',UDF_MANAGE_CODE);")
					,DEFAULT_SEQ_NAME,m_nACQ_TYPE,m_strACQ_YEAR,
					strRECEIPT_NO, strDate, m_pInfo->USER_ID, strWorkLog, strWorkLog);
	m_pDM_WORK->AddFrame(strQuery);
					
	INT ids = m_pDM_WORK->SendFrame();
	m_pDM_WORK->EndFrame();
	
	if(ids<0) return -1;

	m_strRECEIPT_NO = strRECEIPT_NO;

	//������ ���̾�α� refresh 
	if(m_pResourceMgr) {

		m_pResourceMgr->m_dlgSpeciesInfo.SetReceiptNo(m_strACQ_YEAR,m_strRECEIPT_NO);
		m_pResourceMgr->m_dlgSpeciesInfo.ShowSpeciesInfo(m_nCurrentParentIdx);
		
	}	 

	return 0;
}

BOOL CCheckinVolESL::IsNeedReceipNo()
{
	//������ å���� ������ ��쳪 ��ȣ å�� �Լ��� ��� 
	if(m_bIsNeedReceiptNo) return TRUE;

	INT nRow = m_pDM_WORK->GetRowCount();
	

	//���� �Լ�ó���� �Ȱ�� 
	CString strUDF;

	for(INT i=0;i<nRow;i++)
	{
		strUDF = _T("");
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_����"),nRow-i-1);	

		if(_T("C") == strUDF) 
		{
			return TRUE;
		}
	}

	return FALSE;
}

VOID CCheckinVolESL::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(0);
	if(m_pResourceMgr) m_pResourceMgr->HideToolBarButton(_T("ǥ������"));

	CDialog::OnTimer(nIDEvent);
}

VOID CCheckinVolESL::OnbtnCHECKINVOLPREV() 
{
	// TODO: Add your control notification handler code here
	CString strYear;
	m_pCM->GetControlMgrData(_T("����⵵"),strYear);

	if(strYear.IsEmpty()) return;

	INT nYear = 0;
	nYear = _ttoi(strYear.GetBuffer(0));
	nYear--;
	
	strYear.Format(_T("%d"),nYear);
	m_pCM->SetControlMgrData(_T("����⵵"),strYear);
}

VOID CCheckinVolESL::OnbtnCHECKINVOLNEXT() 
{
	// TODO: Add your control notification handler code here
	CString strYear;
	m_pCM->GetControlMgrData(_T("����⵵"),strYear);

	if(strYear.IsEmpty()) return;

	INT nYear = 0;
	nYear = _ttoi(strYear.GetBuffer(0));
	nYear++;
	
	strYear.Format(_T("%d"),nYear);
	m_pCM->SetControlMgrData(_T("����⵵"),strYear);
}

VOID CCheckinVolESL::OnchkCHECKINVOLALLSHOW() 
{
	// TODO: Add your control notification handler code here
	
	//��ü ���Ⱑ Ŭ���Ǹ� ����� ��Ʈ���� ��Ȱ��ȭ
	UpdateData(TRUE);
	
	CWnd* pWnd = NULL;


	if(m_bIsAllDataShow)
	{
		pWnd = GetDlgItem(IDC_btnCHECKIN_VOL_PREV);
		if(pWnd) pWnd->EnableWindow(FALSE);

		pWnd = GetDlgItem(IDC_btnCHECKIN_VOL_NEXT);
		if(pWnd) pWnd->EnableWindow(FALSE);

		pWnd = GetDlgItem(IDC_edtCHECKIN_VOL_YEAR);
		if(pWnd) pWnd->EnableWindow(FALSE);

	}
	else
	{
		pWnd = GetDlgItem(IDC_btnCHECKIN_VOL_PREV);
		if(pWnd) pWnd->EnableWindow(TRUE);

		pWnd = GetDlgItem(IDC_btnCHECKIN_VOL_NEXT);
		if(pWnd) pWnd->EnableWindow(TRUE);

		pWnd = GetDlgItem(IDC_edtCHECKIN_VOL_YEAR);
		if(pWnd) pWnd->EnableWindow(TRUE);
	}
	

}

BOOL CCheckinVolESL::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN )	return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CCheckinVolESL::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
