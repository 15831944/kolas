// MarcEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MarcEditDlg.h"
#include "efs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcEditDlg dialog


CMarcEditDlg::CMarcEditDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	AfxInitRichEdit();
}

CMarcEditDlg::~CMarcEditDlg()
{
}


BOOL CMarcEditDlg::Create(CWnd *pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}


void CMarcEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcEditDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarcEditDlg, CDialog)
	//{{AFX_MSG_MAP(CMarcEditDlg)
	ON_BN_CLICKED(IDC_bPREV_MEC, OnbPREV)
	ON_BN_CLICKED(IDC_bNEXT_MEC, OnbNEXT)
	ON_BN_CLICKED(IDC_bAPPLY_MARC, OnbAPPLYMARC)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcEditDlg message handlers
INT CMarcEditDlg::SetParentInfo(CESL_DataMgr *pDM_Parent, 
								CESL_DataMgr *pGridDm_Parent, 
								CESL_Grid *pGrid_Parent, 
								CString strMarcFieldAlias, 
								CString strMarcErrorFieldAlias,
								INT nFileType,
								INT nChkMultiBook,
								INT nChkGiveAutoRegno,
								INT nChkWorkStatus/*= _T("")*/)
{
	EFS_BEGIN

	if (pDM_Parent == NULL || pGridDm_Parent == NULL || pGrid_Parent == NULL || strMarcFieldAlias.IsEmpty())
		return -1;
	
	m_pDM_Parent = pDM_Parent;
	m_pGrid_Parent = pGrid_Parent;
	m_strMarcFieldAlias = strMarcFieldAlias;
	m_strMarcErrorFieldAlias = strMarcErrorFieldAlias;
	//===================================================
	//2009.03.11 ADD BY PJW : �׸����� DM�� �̿��ϱ� ���ؼ� �߰���
	m_pGridDm_Parent = pGridDm_Parent;
	m_nFileType = nFileType;
	m_nChkMultiBook = nChkMultiBook;
	m_nChkGiveAutoRegno = nChkGiveAutoRegno;
	m_nChkWorkStatus = nChkWorkStatus;	
	//===================================================

	return 0;

	EFS_END
	return -1;

}

INT CMarcEditDlg::Init()
{
	EFS_BEGIN

	if (m_pDM_Parent == NULL || m_pGridDm_Parent == NULL || 
		m_pGrid_Parent == NULL || m_strMarcFieldAlias.IsEmpty()) 
	{
		return -1;
	}

	INT ids = -1;
	// Subclass RichEdit
	if (!m_marcEditor.SubclassDlgItem(IDC_RICHEDIT_MARC, -1, this))
	{
		AfxMessageBox(_T("Subclass Marc Editor Failed"));
		return -1;
	}

	// Marc Manager Set
	m_marcEditor.SetMarcMgr(m_pInfo->pMarcMgr);

	// �θ�׸����� ���õ� �ο� ����
	ids = m_pGrid_Parent->SelectMakeList();
	if (ids < 0)
	{
		AfxMessageBox(_T("���ø���� ������ �� �����ϴ�."));
		return -1;
	}

	INT nIdx = m_pGrid_Parent->SelectGetHeadPosition();
	if (nIdx < 0)
	{
		AfxMessageBox(_T("���ø���� ������ �� �����ϴ�."));
		return -1;
	}

	m_pGrid_Parent->SetReverse(nIdx);

	return 0;

	EFS_END
	return -1;

}

INT CMarcEditDlg::Display()
{
	EFS_BEGIN

	INT ids = -1;

	CString strDMIdx = m_pGrid_Parent->GetTextMatrix(m_pGrid_Parent->SelectGetIdx() + 1, 0);
	m_nParentSelectedDMIdx = _ttoi(strDMIdx)-1;

	if (m_nParentSelectedDMIdx < 0) return -1;

	CString strStreamMarc = m_pDM_Parent->GetCellData(m_strMarcFieldAlias, m_nParentSelectedDMIdx);
	if ( strStreamMarc.IsEmpty()) return -1;


	ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &m_marc);
	if (ids < 0)
	{
		return -1;
	}

	m_marc.DeleteEmptySubfield();
	m_marc.DeleteEmptyField();
	
	m_str049.Empty();
//	if(m_nFileType == 0)	
//	{
//		m_pInfo->pMarcMgr->GetField(&m_marc, _T("049"), m_str049);
////  		m_pInfo->pMarcMgr->DeleteField(&m_marc, _T("049"));
//	}
	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
	
	m_marcEditor.Display();

	return 0;

	EFS_END
	return -1;

}

/////////////////////////////////////////////////////////////////////////////
// CMarcEditDlg message handlers

BOOL CMarcEditDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (Init() < 0)
	{
		//AfxMessageBox(_T("Marc Editor Init Failed!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	Display();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CMarcEditDlg::OnbPREV() 
{
	EFS_BEGIN

	INT nIdx = m_pGrid_Parent->SelectGetPrev();
	if (nIdx < 0)
		nIdx = m_pGrid_Parent->SelectGetHeadPosition();
	
	m_pGrid_Parent->SetReverse(nIdx);
	Display();

	EFS_END
}

VOID CMarcEditDlg::OnbNEXT() 
{
	EFS_BEGIN

	INT nIdx = m_pGrid_Parent->SelectGetNext();
	if (nIdx < 0)
		nIdx = m_pGrid_Parent->SelectGetTailPosition();

	m_pGrid_Parent->SetReverse(nIdx);
	Display();

	EFS_END
}

VOID CMarcEditDlg::OnbAPPLYMARC() 
{
	EFS_BEGIN

	INT ids = -1;
	INT nIdx;

	CESL_DataMgr* pDM = NULL;
	pDM = m_pGridDm_Parent;
	
	if (m_bShowErrorDlg)
		ids = m_marcEditor.ApplyEdit(0);
	else 
		ids = m_marcEditor.ApplyEdit(1);
	
	CString strMarcCheckResult;
	if(m_pMarcImpExpMgr)
		m_pMarcImpExpMgr->CheckMarcSyntax(&m_marc, strMarcCheckResult);
	else
	{
		ESLAfxMessageBox(_T("��ũ ��ȿ�� �˻縦 ������ �� �����ϴ�."));
		return;
	}

	nIdx = m_pGrid_Parent->GetRow() - 1;

	CString strErrMsg;
	CString strField;
	
	strErrMsg.Empty();
	strField.Empty();

	if ( m_nChkWorkStatus == 0 ) 
	{
 		CheckRequiredTag( _T("008"), strErrMsg);
		CheckRequiredTag( _T("260"), strErrMsg);
		CheckRequiredTag( _T("300"), strErrMsg);
	}
	
	CString strRegNo;
 	CArray<CString, CString&> astrRegNo;
	INT nRegNoCnt;
	//�ڵ��ο� üũ�� ��� �׸��忡 �ڵ��ο��� �ѷ��ֱ�
	if ( m_nChkGiveAutoRegno == 1 ) 
	{
 		m_pInfo->pMarcMgr->DeleteItem(&m_marc, _T("049$l"));
	}
	else
	{
		CheckRequiredTag( _T("049"), strErrMsg);
		m_pInfo->pMarcMgr->GetItem(&m_marc, _T("049$l"), strRegNo, &astrRegNo);
		nRegNoCnt = astrRegNo.GetSize();
		if( 0 >= nRegNoCnt )
		{
			if( strErrMsg.IsEmpty() )
			{
				strErrMsg.Format( _T("��Ϲ�ȣ�� �Է����ּ���") );
			}
			else
			{
				strErrMsg = strErrMsg + _T("\r\n��Ϲ�ȣ�� �Է����ּ���");
			}
		}
	}

	//===================================================
	//2010.05.02 ADD BY PJW : ���ؽ� üũ���� ������ �κе� �־��ش�.
	strErrMsg = strMarcCheckResult + strErrMsg;
	//===================================================
	
	if( !strErrMsg.IsEmpty() )
	{
		m_pDM_Parent->SetCellData(m_strMarcErrorFieldAlias, strErrMsg, m_nParentSelectedDMIdx);
		pDM->SetCellData( m_strMarcErrorFieldAlias, strErrMsg, nIdx );	
		AfxMessageBox(strErrMsg);
		return;
	}

	if (ids < 0)
	{
		// ���� �÷��� �ޱ�
		if (!m_strMarcErrorFieldAlias.IsEmpty())
		{
			m_pDM_Parent->SetCellData(m_strMarcErrorFieldAlias, _T("��ũ ���� ����"), m_nParentSelectedDMIdx);
		}

		if (MessageBox(_T("���� ������ �ֽ��ϴ�. �׷��� �����Ͻðڽ��ϱ�?"), NULL, MB_YESNO | MB_ICONQUESTION) == IDNO)
		{
			return;
		}
	}
	else
	{
		// ���� �÷��� �ޱ�
		if (!m_strMarcErrorFieldAlias.IsEmpty())
		{
			m_pDM_Parent->SetCellData(m_strMarcErrorFieldAlias, _T(""), m_nParentSelectedDMIdx);		
		}	
	}

	CString strStremMarc;

	ids = m_pInfo->pMarcMgr->Encoding(&m_marc, strStremMarc);	

	if (ids < 0)
	{
		AfxMessageBox(_T("�������"));
	}
	else
	{
		m_pDM_Parent->SetCellData(m_strMarcFieldAlias, strStremMarc, m_nParentSelectedDMIdx);
		ApplySpeciesDmToImportDm( m_nParentSelectedDMIdx, nIdx );

		AfxMessageBox(_T("�����Ͽ����ϴ�"));

		OnbNEXT();
	}	

	EFS_END
}

CMarcEditDlg::ApplySpeciesDmToImportDm( INT nDmIdx, INT nCridIdx )
{
	CESL_DataMgr* pDM = NULL;

	INT nRegCodeLength;
	INT i;
	INT nColCnt;

	CString strAlias;
	CStringArray astrAlias;
	CString strData;

	pDM = m_pGridDm_Parent;	

	nColCnt = pDM->GetRefColumnCount();
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;

	for( i = 0 ; i < nColCnt ; i++ )
	{
		CString strFieldName;
		CString strFieldType;
		CString strDataType;

		pDM->FindColumnFieldName( i , strAlias , strFieldName ,strFieldType,strDataType,0,strData);
		astrAlias.Add(strAlias);
	}

	for( i = 0 ; i < nColCnt ; i++ )
	{
		strAlias = astrAlias.GetAt(i);
		if( _T("����") == strAlias || _T("����") == strAlias) 
		{
			continue;
		}
		else if(  _T("���۵�Ϲ�ȣ") == strAlias )
		{
			//�ڵ��ο� üũ�� ��� �׸��忡 �ڵ��ο��� �ѷ��ֱ�
			if ( m_nChkGiveAutoRegno == 1 ) 
			{
				strData = _T("�ڵ��ο�");
			}
			else
			{
				strData = m_pDM_Parent->GetCellData(_T("BS_MARC.049$l"), nDmIdx);
				if ( strData != _T("") && strData.GetLength() < 12 ) 
				{
					if ( strData.GetLength() > nRegCodeLength ) 
					{
						strData =  m_pInfo->MakeRegNo( strData.Left(nRegCodeLength) , strData.Mid(nRegCodeLength) );
					}						
				}
			}
		}
		else if( _T("BS_���Ϲ���å��") == strAlias )
		{
			if( m_nChkMultiBook == 1)
			{
				strData = m_pDM_Parent->GetCellData(_T("UDF_å��"),nDmIdx);
			}
			else
			{
				strData = _T("1");
			}
		}
		else if( _T("BS_���Ϲ��Կ���") == strAlias )
		{
			strData = m_pDM_Parent->GetCellData(_T("BS_���Ϲ��Կ���"),nDmIdx);
		}
		else if( _T("����ISBN") == strAlias )
		{
			strData = m_pDM_Parent->GetCellData(_T("BS_MARC.020$a"),nDmIdx);
		}
		else
		{
			m_pDM_Parent->GetCellData(("BS_MARC.") + strAlias, nDmIdx, strData);
		}

		pDM->SetCellData( nCridIdx, i, strData);
	}
	return 0 ;
}

HBRUSH CMarcEditDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

void CMarcEditDlg::OnCancel() 
{

	CDialog::OnCancel();
}

INT CMarcEditDlg::CheckRequiredTag( CString strTag, CString &strErrMsg)
{
	CString strTemp;
	CString strField;
	
	strTemp = strErrMsg;

	m_pInfo->pMarcMgr->GetField( &m_marc, strTag, strField );

	if( strField.IsEmpty() )
	{
		if( strTemp.IsEmpty() )
		{
			strErrMsg.Format( _T("[%s] Tag�� �Է����ּ���"), strTag );			
		}
		else
		{			
			strErrMsg.Format( _T("%s\r\n[%s] Tag�� �Է����ּ���"), strTemp, strTag );
		}		
	}
	return 0 ;
}