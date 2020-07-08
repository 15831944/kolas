// IndexEditorMain.cpp : implementation file
//

#include "stdafx.h"
#include "IndexEditorMain.h"
#include "IndexGroup.h"
#include "IndexIndicatorType.h"
#include "IndexSubfield.h"
#include "IndexTagType.h"
#include "IndexTagInsert.h"
#include "IndexInsert.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CIndexEditorMain dialog


CIndexEditorMain::CIndexEditorMain(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndexEditorMain)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCM = NULL;

	m_strIndexGroup = _T("");

	m_pCurrentIndex = NULL;

	m_pChildIndexInfo = NULL;

	m_nPopupMenu = -1;
}

CIndexEditorMain::~CIndexEditorMain()
{
	FreeAllData();
	FreeChildForm();
}


VOID CIndexEditorMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndexEditorMain)
	DDX_Control(pDX, IDC_treeIndexComponent, m_ctrlTree);
	DDX_Control(pDX, IDC_listIndexAlias, m_ctrlListBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndexEditorMain, CDialog)
	//{{AFX_MSG_MAP(CIndexEditorMain)
	ON_COMMAND_RANGE(4000,4003,PopupSelect)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_LBN_SELCHANGE(IDC_listIndexAlias, OnSelchangelistIndexAlias)
	ON_NOTIFY(TVN_SELCHANGED, IDC_treeIndexComponent, OnSelchangedtreeIndexComponent)
	ON_NOTIFY(NM_RCLICK, IDC_treeIndexComponent, OnRclicktreeIndexComponent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndexEditorMain message handlers
BOOL CIndexEditorMain::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CIndexEditorMain::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(InitESL_Mgr( _T("�ε���������_����") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : �ε���������_����") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_�ε���������_����"));
	if(!m_pCM)
	{
		AfxMessageBox( _T("reference Error : CM_�ε���������_����") );
		return FALSE;		
	}


	m_TreeImage.Create(IDB_TREETYPE, 16, 0, RGB(255, 255, 255));

	CreateChildForm();

	//Ÿ�̸� ������ ���� �׷��� �����ϰ� �Ѵ� 
	SetTimer(0,100,NULL);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CIndexEditorMain::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	CWnd* pWnd = NULL;


	//���� �׷� group box 
	pWnd = (CWnd*)GetDlgItem(IDC_groupIndexGroup);
	if(pWnd->GetSafeHwnd() == NULL) return;
	 
	pWnd->MoveWindow(2, 0, 200, 40);	

	
	//���� �׷� static
	pWnd = (CWnd*)GetDlgItem(IDC_stcIndexGroup);
	if(pWnd->GetSafeHwnd() == NULL) return;
	 
	pWnd->MoveWindow(5, 17, 50, 23);	


	//���� �׷� edit 
	pWnd = (CWnd*)GetDlgItem(IDC_edtIndexGroup);
	if(pWnd->GetSafeHwnd() == NULL) return;
	 
	pWnd->MoveWindow(55, 13, 140, 20);	


	//����Ʈ
	pWnd = (CWnd*)GetDlgItem(IDC_listIndexAlias);
	if(pWnd->GetSafeHwnd() == NULL) return;
	 
	pWnd->MoveWindow(0, 50, 200, cy);	


	//Ʈ��
	pWnd = (CWnd*)GetDlgItem(IDC_treeIndexComponent);
	if(pWnd->GetSafeHwnd() == NULL) return;


	pWnd->MoveWindow(210, 0, 240, cy);

	//child(index info)
	CRect rect;
	this->GetClientRect(rect);



	if(m_pChildIndexInfo) m_pChildIndexInfo->ResizeWindow(rect);


}

INT CIndexEditorMain::LoadReferenceFromFile()
{
	if(m_strIndexGroup.IsEmpty()) 
	{
		AfxMessageBox(_T("���� �׷��� ���õ��� �ʾҽ��ϴ�!"));
		return -1;
	}

	CString strCfgFilePath;
	strCfgFilePath.Format(_T("..\\Cfg\\IndexMgr\\Index_%s.txt"),m_strIndexGroup);

	//���۷��� ���Ϸ� ���� ���� ��Ģ�� �д´�
	if(LoadIndexReference(strCfgFilePath,&mapIndex)<0) return -1;


	return 0;
}

INT CIndexEditorMain::LoadIndexReference(LPCTSTR lpszFileName,CMapStringToOb* pMap)
{
	m_arrayIndexList.RemoveAll();
	CStdioFile file;
	INT nSuccess = file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary);
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	CString strLine;
	
	if(nSuccess)
	{
		
		CString strTag;
		CString strTagType;
		CString strSubfield;
		CString strIndicator;
		CString strIndicatorType;
		CString strIndicatorOption;
		CString strTmp;
		INT nPos = -1;

		structIndex* index = NULL;
		structIndexTag* tag = NULL;
		structIndexSubfield* subfield = NULL;
		structIndexIndicator* indicator = NULL;

		
		while(file.ReadString(strLine))
		{
			strLine.TrimLeft();
			strLine.TrimRight();
			if(strLine.IsEmpty()) continue;
			
			
			if(strLine.GetAt(0) == '=') //���ο� ���� 
			{
			
				strLine = strLine.Mid(1);
				strLine.TrimLeft();
				strLine.TrimRight();
				if(strLine.IsEmpty())
				{	
					AfxMessageBox(_T("ERROR!!! IndexMgr: ���� �˸��ƽ��� ���� ������ �����մϴ�!"));
					return -1;
				}
				
				index = new structIndex;
				if(!index) return -1;
				
				pMap->SetAt(strLine,(CObject*)index); 

				m_arrayIndexList.Add(strLine);
										
			}
			else if(strLine.GetAt(0) == '[') //�±�
			{
				strLine.Remove('[');
				strLine.Remove(']');
				if(strLine.IsEmpty()) return -1;
				
				nPos = strLine.Find(_T("|"));
				if(nPos>0)
				{
					strTag = strLine.Left(nPos);
					strTagType = strLine.Mid(nPos+1);
				}
				else
				{
					strTag = strLine;
					strTagType = _T("");
				}
				
				tag = new structIndexTag;
				if(!tag) return -1;
				tag->strTag = strTag;
				tag->strType = strTagType;

				if(index) index->IndexTagList.AddTail((CObject*)tag);

			}
			else if(strLine.GetAt(0) == '<') //�ĺ���ȣ
			{
				strLine.Remove('<');
				strLine.Remove('>');
				if(strLine.IsEmpty()) return -1;
				
				strSubfield = strLine;

				subfield = new structIndexSubfield;
				if(!subfield) return -1;
				subfield->strSubfield = strSubfield;
				
				if(tag) tag->IndexSubfieldList.AddTail((CObject*)subfield);

			}
			else //���ñ�ȣ
			{
					
				nPos = strLine.Find(_T("|"));
				
				if(nPos>0)
				{
					strIndicator = strLine.Left(nPos);

					//strIndicatorType = strLine.Mid(nPos+1);

					strTmp = strLine.Mid(nPos+1);

					nPos = strTmp.Find(_T("|"));

					if(nPos>0)
					{
						strIndicatorType = strTmp.Left(nPos);
						strIndicatorOption = strLine.Mid(nPos+1);
					}
					else
					{
						strIndicatorType = strTmp;
						strIndicatorOption = DEFAULT_INDICATOR_SINGLE_OPTION;
					}

				}
				else
				{
					strIndicator = strLine;
					strIndicatorType = DEFAULT_INDICATOR_TYPE;
					strIndicatorOption = DEFAULT_INDICATOR_SINGLE_OPTION;
				}


				indicator = new structIndexIndicator;
				if(!indicator) return -1;
				indicator->strIndicator = strIndicator;
				indicator->strType = strIndicatorType;
				indicator->strSingleOption = strIndicatorOption;

				if(subfield) subfield->IndexIndicatorList.AddTail((CObject*)indicator);

			}

		}
		

		file.Close();
	}
	else
	{
		AfxMessageBox(_T("�ش� ������ ������ ������ ���� �� �����ϴ�!"));
		return -1;
	}

	return 0;
}

VOID CIndexEditorMain::FreeAllData()
{
	CString strKey;

	CObject* pObject = NULL;
	structIndex* index = NULL;
	structIndexTag* tag = NULL;
	structIndexSubfield* subfield = NULL;
	structIndexIndicator* indicator = NULL;

	
	POSITION pos = mapIndex.GetStartPosition();
	POSITION posTag;
	POSITION posSubfield;
	POSITION posIndicator;

	while(pos) //����
	{
		mapIndex.GetNextAssoc(pos, strKey, pObject);
		index = (structIndex*)pObject;
		
		posTag = index->IndexTagList.GetHeadPosition();
		                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
		while(posTag) //�±�
		{
			pObject = index->IndexTagList.GetNext(posTag);
			tag = (structIndexTag*)pObject;
			
			posSubfield = tag->IndexSubfieldList.GetHeadPosition();

			while(posSubfield) //�ĺ���ȣ
			{
				pObject = tag->IndexSubfieldList.GetNext(posSubfield);
				subfield = (structIndexSubfield*)pObject;
				
				posIndicator = subfield->IndexIndicatorList.GetHeadPosition();

				while(posIndicator) //���ñ�ȣ
				{
					pObject = subfield->IndexIndicatorList.GetNext(posIndicator);
					indicator = (structIndexIndicator*)pObject;
					if(indicator) 
					{
						delete indicator;
						indicator = NULL;
					}
				}

				if(subfield)
				{
					subfield->IndexIndicatorList.RemoveAll();
					delete subfield;
					subfield = NULL;
				}
			}
				
			if(tag)
			{
				tag->IndexSubfieldList.RemoveAll();
				delete tag;
				tag = NULL;
			}

		}
		
		if(index)
		{
			index->IndexTagList.RemoveAll();
			delete index;
			index = NULL;

		}
	}
	
	mapIndex.RemoveAll();


}

VOID CIndexEditorMain::CreatePopUpButton( INT nLevel )
{

    CMenu   menuPopup;
	CPoint	point;

	::GetCursorPos( &point );
	
	menuPopup.CreatePopupMenu();
	switch( nLevel ) {
		case 0:	//�ε���
			menuPopup.AppendMenu		( MF_STRING  , 4000, _T("�±��߰�") );
			menuPopup.TrackPopupMenu	( TPM_LEFTALIGN, point.x, point.y, this);
			break;
		case 1:	//�±�
			menuPopup.AppendMenu		( MF_STRING, 4000, _T("�ĺ���ȣ�߰�") );
			menuPopup.AppendMenu		( MF_SEPARATOR);
			menuPopup.AppendMenu		( MF_STRING, 4001, _T("�±׻���") );
			menuPopup.AppendMenu		( MF_STRING, 4002, _T("�±���������") );
			menuPopup.TrackPopupMenu	( TPM_LEFTALIGN, point.x, point.y, this);
			break;
		case 2:	//�ĺ���ȣ
			menuPopup.AppendMenu		( MF_STRING, 4000, _T("���ñ�ȣ�߰�") );
			menuPopup.AppendMenu		( MF_SEPARATOR);
			menuPopup.AppendMenu		( MF_STRING, 4001, _T("�ĺ���ȣ����") );
			menuPopup.TrackPopupMenu	( TPM_LEFTALIGN, point.x, point.y, this);
			break;
		case 3:	//���ñ�ȣ
			menuPopup.AppendMenu		( MF_STRING, 4000, _T("���ñ�ȣ����") );
			menuPopup.AppendMenu		( MF_SEPARATOR);
			menuPopup.AppendMenu		( MF_STRING, 4001, _T("���ñ�ȣ����") );
			menuPopup.TrackPopupMenu	( TPM_LEFTALIGN, point.x, point.y, this);
			break;
		default:
			break;
	}

	//���� �˾�â ���� ����
	if(nLevel>=0) m_nPopupMenu = nLevel;

	return;
}

INT CIndexEditorMain::SetTreeAll(CString strIndexAlias,CMapStringToOb* pMap)
{
	
	CObject* pObject = NULL;
	structIndex* pIndex = NULL;
	structIndexTag* pTag = NULL;
	structIndexSubfield* pSubfield = NULL;
	structIndexIndicator* pIndicator = NULL;


	INT ids = pMap->Lookup(strIndexAlias,pObject);
	
	if(ids<0) return ids;
	pIndex = (structIndex*)pObject;

	//���� ���õ� �������� ���� 
	m_pCurrentIndex = pIndex;

	m_ctrlTree.SetImageList( &m_TreeImage , TVSIL_NORMAL );

	m_ctrlTree.DeleteAllItems();

	TVINSERTSTRUCT tvinsert;

	// �⺻�� ����
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE;
	tvinsert.item.state = 0;
	tvinsert.item.stateMask = 0;
	tvinsert.item.cchTextMax = 60;
	tvinsert.item.cChildren = 0;

	
	//level 1 (index)

	tvinsert.hParent = NULL;
	tvinsert.item.pszText = strIndexAlias.GetBuffer(0);
	
	HTREEITEM hRoot = m_ctrlTree.InsertItem(&tvinsert);
	m_ctrlTree.SetItemImage(hRoot,0,0);

	HTREEITEM hLevel2;
	HTREEITEM hLevel3;
	HTREEITEM hLevel4;

	POSITION posTag = pIndex->IndexTagList.GetHeadPosition();
	POSITION posSubfield;
	POSITION posIndicator;
	INT nImage = -1;

	while(posTag)
	{
		//level 2 (tag)
		pTag = (structIndexTag*)pIndex->IndexTagList.GetNext(posTag);
		
		if(pTag)
		{
			
			tvinsert.hParent = hRoot;
			tvinsert.item.pszText = (TCHAR*)pTag->strTag.GetBuffer(0);
			hLevel2 = m_ctrlTree.InsertItem(&tvinsert);


			if(pTag->strType.IsEmpty())
			{
				m_ctrlTree.SetItemImage(hLevel2,1,1);
			}
			else
			{
				nImage = GetImageNumber(pTag->strType);
				m_ctrlTree.SetItemImage(hLevel2,nImage,nImage);
				
			}

			posSubfield = pTag->IndexSubfieldList.GetHeadPosition();

			while(posSubfield)
			{
				// level 3 (subfield)
				pSubfield = (structIndexSubfield*)pTag->IndexSubfieldList.GetNext(posSubfield);

				if(pSubfield)
				{
					tvinsert.item.mask = TVIF_TEXT;
					tvinsert.hParent = hLevel2;
					tvinsert.item.pszText = (TCHAR*)pSubfield->strSubfield.GetBuffer(0);
					hLevel3 = m_ctrlTree.InsertItem(&tvinsert);
					m_ctrlTree.SetItemImage(hLevel3,2,2);
	

					posIndicator = pSubfield->IndexIndicatorList.GetHeadPosition();

					while(posIndicator)
					{
						// level 4 (indicator)
						pIndicator = (structIndexIndicator*)pSubfield->IndexIndicatorList.GetNext(posIndicator);
						
						if(pIndicator)
						{
							tvinsert.hParent = hLevel3;
							tvinsert.item.pszText = (TCHAR*)pIndicator->strIndicator.GetBuffer(0);
							hLevel4 = m_ctrlTree.InsertItem(&tvinsert);
	
							nImage = GetImageNumber(pIndicator->strType);
							m_ctrlTree.SetItemImage(hLevel4,nImage,nImage);
						}

					}

					m_ctrlTree.Expand(hLevel3, TVE_EXPAND);

				}
			}

			//m_ctrlTree.Expand(hLevel2, TVE_EXPAND);

		}
	
	}

	m_ctrlTree.Expand(hRoot, TVE_EXPAND);

	
	return 0;
}

VOID CIndexEditorMain::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 0)
	{
        KillTimer(0);
		ProcShowIndexGroup();
	}

	CDialog::OnTimer(nIDEvent);
}


VOID CIndexEditorMain::ProcShowIndexGroup()
{
	CIndexGroup dlg(this);
	if(dlg.DoModal() != IDOK) return;
	
	m_strIndexGroup = dlg.m_strGroupCode;

	m_pCM->SetControlMgrData(_T("���α׷�"),m_strIndexGroup);

	RefreshAllData();
}

VOID CIndexEditorMain::ProcInsertIndex()
{
	CIndexInsert dlg(this);
	if(dlg.DoModal() != IDOK) return;
	
	CString strKey = dlg.m_strIndexName;

	CObject* pObject = NULL;
	INT ids = mapIndex.Lookup(strKey,pObject);
	if(pObject) 
	{
		AfxMessageBox(_T("�ش� �׷쳻�� �̹� ������ ������ �ֽ��ϴ�!"));
		return;

	}

	structIndex* pIndex = NULL;

	pIndex = new structIndex;
	if(!pIndex) return;
	
	mapIndex.SetAt(strKey,(CObject*)pIndex); 
	INT nCurSel = m_ctrlListBox.GetCount();
	m_ctrlListBox.InsertString(nCurSel,strKey);
	m_ctrlListBox.SetCurSel(nCurSel);

	OnSelchangelistIndexAlias();

}

VOID CIndexEditorMain::ProcDeleteIndex()
{
	UpdateData(TRUE);
	
	INT nSelectedRow = m_ctrlListBox.GetCurSel();

	CString strSelectedIndex;
	m_ctrlListBox.GetText(nSelectedRow,strSelectedIndex);

	CString strMsg;
	strMsg.Format(_T(" [ %s ] �׷쿡�� [ %s ] ������ ���� �Ͻðڽ��ϱ�?"),m_strIndexGroup,strSelectedIndex);

	if(AfxMessageBox(strMsg,MB_YESNO) != IDYES) return;

	

	CObject* pObject = NULL;
	INT ids = mapIndex.Lookup(strSelectedIndex,pObject);
	if(ids<0) 
	{
		AfxMessageBox(_T("���� �Ϸ��� ����� ã�� �� �����ϴ�!"));
		return;

	}

	
	//������ ���� 
	structIndex* index = NULL;
	structIndexTag* tag = NULL;
	structIndexSubfield* subfield = NULL;
	structIndexIndicator* indicator = NULL;

	index = (structIndex*)pObject;
	
	POSITION pos = mapIndex.GetStartPosition();
	POSITION posTag;
	POSITION posSubfield;
	POSITION posIndicator;

	posTag = index->IndexTagList.GetHeadPosition();
		                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
	while(posTag) //�±�
	{
		pObject = index->IndexTagList.GetNext(posTag);
		tag = (structIndexTag*)pObject;
			
		posSubfield = tag->IndexSubfieldList.GetHeadPosition();

		while(posSubfield) //�ĺ���ȣ
		{
			pObject = tag->IndexSubfieldList.GetNext(posSubfield);
			subfield = (structIndexSubfield*)pObject;
				
			posIndicator = subfield->IndexIndicatorList.GetHeadPosition();

			while(posIndicator) //���ñ�ȣ
			{
				pObject = subfield->IndexIndicatorList.GetNext(posIndicator);
				indicator = (structIndexIndicator*)pObject;
				if(indicator) 
				{
					delete indicator;
					indicator = NULL;
				}
			}

			if(subfield)
			{
				subfield->IndexIndicatorList.RemoveAll();
				delete subfield;
				subfield = NULL;
			}
		}
				
		if(tag)
		{
			tag->IndexSubfieldList.RemoveAll();
			delete tag;
			tag = NULL;
		}

	}
		
	if(index)
	{
		index->IndexTagList.RemoveAll();
		delete index;
		index = NULL;

	}

	//����Ʈ ���� 
	m_ctrlListBox.DeleteString(nSelectedRow);
	
	INT nCnt = m_ctrlListBox.GetCount();
	INT nSetCursel = -1;

	//Refresh
	if(nCnt>0) 
	{
		if(nSelectedRow>0) nSetCursel = nSelectedRow - 1;
		if(0 == nSelectedRow) nSetCursel = 0;

		m_ctrlListBox.SetCurSel(nSetCursel);
		
		OnSelchangelistIndexAlias();
	}



}

VOID CIndexEditorMain::ProcSave()
{
	
	CString strCfgFilePath;
	strCfgFilePath.Format(_T("..\\Cfg\\IndexMgr\\Index_%s.txt"),m_strIndexGroup);
	
	CStdioFile file;
	INT nSuccess = file.Open(strCfgFilePath, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
	if(!nSuccess) 
	{
		AfxMessageBox(_T("���۷��� ������ ã�� �� �����ϴ�!"));
		return;
	}
	
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}

	CString strKey;

	CObject* pObject = NULL;
	structIndex* index = NULL;
	structIndexTag* tag = NULL;
	structIndexSubfield* subfield = NULL;
	structIndexIndicator* indicator = NULL;

	
	POSITION pos = mapIndex.GetStartPosition();
	POSITION posTag;
	POSITION posSubfield;
	POSITION posIndicator;
	CString strLine;

	while(pos) //����
	{
		mapIndex.GetNextAssoc(pos, strKey, pObject);
		index = (structIndex*)pObject;
		
		if(strKey.IsEmpty()) continue;
		
		strLine.Format(_T("=%s\r\n"),strKey);
		file.WriteString(strLine);

		posTag = index->IndexTagList.GetHeadPosition();
		                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
		while(posTag) //�±�
		{
			pObject = index->IndexTagList.GetNext(posTag);
			tag = (structIndexTag*)pObject;
			
			if(tag)
			{
				if(tag->strType.IsEmpty()) strLine.Format(_T("[%s]\r\n"),tag->strTag);
				else strLine.Format(_T("[%s|%s]\r\n"),tag->strTag,tag->strType);

				file.WriteString(strLine);
			}

			posSubfield = tag->IndexSubfieldList.GetHeadPosition();

			while(posSubfield) //�ĺ���ȣ
			{
				pObject = tag->IndexSubfieldList.GetNext(posSubfield);
				subfield = (structIndexSubfield*)pObject;
					
				if(subfield)
				{
					strLine.Format(_T("<%s>\r\n"),subfield->strSubfield);
					file.WriteString(strLine);

				}


				posIndicator = subfield->IndexIndicatorList.GetHeadPosition();

				while(posIndicator) //���ñ�ȣ
				{
					pObject = subfield->IndexIndicatorList.GetNext(posIndicator);
					indicator = (structIndexIndicator*)pObject;
					
					if(indicator) 
					{
						
						if(indicator->strSingleOption.IsEmpty())
							strLine.Format(_T("%s|%s\r\n"),indicator->strIndicator,indicator->strType);
						else 
							strLine.Format(_T("%s|%s|%s\r\n"),indicator->strIndicator,indicator->strType,indicator->strSingleOption);
						
						file.WriteString(strLine);
					}
				}

			}
				
		}
	}
	
	file.Close();

	AfxMessageBox(_T("�����Ͽ����ϴ�!"));
}

INT CIndexEditorMain::RefreshAllData()
{
	if(LoadReferenceFromFile()<0) return -1;
		
	ShowIndexList();

	return 0;
}

VOID CIndexEditorMain::ShowIndexList()
{

	m_ctrlListBox.ResetContent();

	INT nSize = m_arrayIndexList.GetSize();
	CString strIndex;

	for(INT i=0;i<nSize;i++)
	{
		strIndex = m_arrayIndexList.GetAt(i);
		m_ctrlListBox.InsertString(i,strIndex);
		
	}

}

VOID CIndexEditorMain::OnSelchangelistIndexAlias() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	INT nSelectedRow = m_ctrlListBox.GetCurSel();

	CString strSelectedIndex;
	m_ctrlListBox.GetText(nSelectedRow,strSelectedIndex);

	SetTreeAll(strSelectedIndex,&mapIndex);

}

INT CIndexEditorMain::GetImageNumber(CString strType)
{
	if(_T("A") == strType) return 3;
	if(_T("B") == strType) return 4;
	if(_T("C") == strType) return 5;
	if(_T("D") == strType) return 6;
	if(_T("E") == strType) return 7;
	if(_T("F") == strType) return 8;
	if(_T("G") == strType) return 9;
	if(_T("H") == strType) return 10;
	if(_T("I") == strType) return 11;
	if(_T("J") == strType) return 12;

	return 0;
}

VOID CIndexEditorMain::OnSelchangedtreeIndexComponent(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	
	TCHAR text[20];
	TV_ITEM item;
	
	wmemset(text,0,20);

	item.mask = TVIF_HANDLE | TVIF_TEXT;
	//item.hItem = hItem;
	item.pszText = text;
	item.cchTextMax = 30;


	//���õ� �׸��� ������ ����â�� ������
	//���ñ�ȣ �ĺ���ȣ�� ��쿡�� parent �±׸� ã�´�.
	INT nLevel = GetIndentLevel(hItem);
	
	if(0 == nLevel) {
		
		m_pChildIndexInfo->ShowIndexTypeDesc(0);
		*pResult = 0;
		return;
	}
	else if(1 == nLevel) //�±� ����
	{
		m_pChildIndexInfo->ShowIndexTypeDesc(0);
	}
	else if(2 == nLevel) //�ĺ���ȣ ����
	{
		hItem = m_ctrlTree.GetParentItem(hItem);

		m_pChildIndexInfo->ShowIndexTypeDesc(1);
	}
	else if(3 == nLevel) //���ñ�ȣ ����
	{
		hItem = m_ctrlTree.GetParentItem(hItem);
		hItem = m_ctrlTree.GetParentItem(hItem);
	
		m_pChildIndexInfo->ShowIndexTypeDesc(1);
	}

	

	item.hItem = hItem;
	m_ctrlTree.GetItem(&item);

	CString strMsg;
	strMsg.Format(_T("%s"),(LPCTSTR)text);
	

	m_pChildIndexInfo->ShowTagDesc(m_strIndexGroup, strMsg);


	*pResult = 0;
}

VOID CIndexEditorMain::OnRclicktreeIndexComponent(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	CPoint pt;
	GetCursorPos(&pt);
	m_ctrlTree.ScreenToClient(&pt);
	HTREEITEM hItem = m_ctrlTree.HitTest(pt);
	m_ctrlTree.SelectItem(hItem);

	TCHAR text[20];
	TV_ITEM item;
	
	wmemset(text,0,20);

	item.mask = TVIF_HANDLE | TVIF_TEXT;
	item.hItem = hItem;
	item.pszText = text;
	item.cchTextMax = 30;
	
	m_ctrlTree.GetItem(&item);
	
	CString strMsg;
	strMsg.Format(_T("%s"),(LPCTSTR)text);

	
	if(!strMsg.IsEmpty())
	{
		INT nLevel = GetIndentLevel(hItem);
		CreatePopUpButton(nLevel);
	}

	*pResult = 0;
}

INT CIndexEditorMain::GetIndentLevel(HTREEITEM hItem)
{
    INT iLevel = 0;

    while((hItem = m_ctrlTree.GetParentItem(hItem)) != NULL )
        iLevel++;

    return iLevel;
}

VOID CIndexEditorMain::CreateChildForm()
{
	m_pChildIndexInfo = new CIndexInfo(this);
	m_pChildIndexInfo->Create(this);

	CRect rect;
	this->GetClientRect(rect);
	
	
	rect.left = rect.left + 455;
	
	//rect.left = rect.right - 450;
	rect.right -= 5;
	
	this->MoveWindow(rect);
	
	m_pChildIndexInfo->MoveWindow(rect);
	m_pChildIndexInfo->ShowWindow(TRUE);
}

VOID CIndexEditorMain::FreeChildForm()
{
	if(m_pChildIndexInfo) delete m_pChildIndexInfo;
	m_pChildIndexInfo = NULL;
}

VOID CIndexEditorMain::PopupSelect(UINT nIDC)
{
	if(m_nPopupMenu<0) return;

	INT nCommandRange = (INT)nIDC;
	nCommandRange -= 4000; //command range�� 0,1,2�� �ܼ�ȭ �Ѵ�

	INT ids = -1;

	switch(m_nPopupMenu)
	{
		case 0: 
			ids = CommandIndex(nCommandRange);
			break;
		case 1: 
			ids = CommandTag(nCommandRange);
			break;
		case 2: 
			ids = CommandSubfield(nCommandRange);
			break;
		case 3: 
			ids = CommandIndicator(nCommandRange);
			break;
		default:
			break;

	}

	//if(ids<0) AfxMessageBox(_T("��� ó���� �����߽��ϴ�!"));

}

INT CIndexEditorMain::CommandIndex(INT nCommand)
{
	//nCommand : 0 -> �±��߰�
	structIndexTag* pIndexTag = NULL;

	if(0 == nCommand)
	{
		CIndexTagInsert dlg(this);
		dlg.m_strIndexGroup = m_strIndexGroup;

		if(dlg.DoModal() == IDOK) 
		{

			//�ߺ��˻� 
			if(CheckDuplicatedTag(dlg.m_strTag)<0) 
			{
				AfxMessageBox(_T("�ش� ���� ���� ������ �±װ� �ֽ��ϴ�!"));
				return -1;
			}
			
			//������ �߰�
			pIndexTag = new structIndexTag;
			if(!pIndexTag) return -1;
		
			pIndexTag->strTag = dlg.m_strTag;
			pIndexTag->strType = dlg.m_strTagType;
		
			m_pCurrentIndex->IndexTagList.AddTail((CObject*)pIndexTag);

			//Ʈ�� ��� �߰�
			SetTreeNodeTag(pIndexTag->strTag,pIndexTag->strType,1);

		}

	}

	return 0;
}

INT CIndexEditorMain::CommandTag(INT nCommand)
{
	//nCommand : 0 -> �ĺ���ȣ�߰�(�ĺ���ȣ �߰��ÿ��� �⺻ ���ñ�ȣ(AL|A)�� ������ �ش�)
	//nCommand : 1 -> �±׻���
	//nCommand : 2 -> �±���������
	

	structIndexTag* pIndexTag = NULL;
	structIndexSubfield* pIndexSubfield = NULL;
	structIndexIndicator* pIndexIndicator = NULL;

	CString strTag;
	
	pIndexTag = GetCurrentTag(strTag);
	
	if(!pIndexTag) return -1;

	if(0 == nCommand)
	{
		CIndexSubfield dlg(this);
			
		dlg.m_nOpenMode = 1; 
		dlg.m_strIndexGroup = m_strIndexGroup;
		dlg.m_strTag = strTag;
		
		if(dlg.DoModal() == IDOK) 
		{
			//������ �߰�
			pIndexSubfield = new structIndexSubfield;
			if(!pIndexSubfield) return -1;
		
			pIndexSubfield->strSubfield = dlg.m_strSubfield;
			
			//�ߺ��˻� 
			if(CheckDuplicatedSubfield(strTag,dlg.m_strSubfield)<0) 
			{
				AfxMessageBox(_T("�ش� �±� ���� ������ �ĺ���ȣ�� �ֽ��ϴ�!"));
				return -1;
			}

			pIndexIndicator = new structIndexIndicator;
			if(!pIndexIndicator) return -1;

			pIndexIndicator->strIndicator = _T("AL");
			pIndexIndicator->strType = _T("A");
			
			pIndexSubfield->IndexIndicatorList.AddTail((CObject*)pIndexIndicator);
			
			pIndexTag->IndexSubfieldList.AddTail((CObject*)pIndexSubfield);

			//Ʈ����� �߰�
			SetTreeNodeSubfield(dlg.m_strSubfield);
		}
	}
	else if(1 == nCommand)
	{
		CString strMsg;

		strMsg.Format(_T("%s �±׸� ���� �Ͻðڽ��ϱ�?"),
					  strTag);
			
		if(AfxMessageBox(strMsg,MB_YESNO) != IDYES) return 0;

		
		//������ ���� 
		if(DeleteCurrentTag()<0)
		{
			AfxMessageBox(_T("������ �����Ͽ����ϴ�!"));
		}
		else
		{
			//��� ���� 
			HTREEITEM hItemTag = m_ctrlTree.GetSelectedItem();
			m_ctrlTree.DeleteItem(hItemTag);
			
			AfxMessageBox(_T("�����Ͽ����ϴ�!"));
		}

	}
	else if(2 == nCommand)
	{

		CIndexTagType dlg(this);
		
		if(_T("U") == pIndexTag->strType)
		{
			AfxMessageBox(_T("�ش� �±״� ����� ���� �±װ� �ƴϹǷ� \n")
						  _T("���� ������ �� �� �����ϴ�!"));
			return -1;
		}

		dlg.m_strTagType = pIndexTag->strType;
		

		if(dlg.DoModal() == IDOK) 
		{
			if(dlg.m_strTagType != pIndexTag->strType)
			{
				
				//������ ����
				pIndexTag->strType = dlg.m_strTagType;

					
				//Ʈ�� ��� ����
				SetTreeNodeTag(pIndexTag->strTag,pIndexTag->strType,-1);
			}

		}


	}

		
	return 0;
}

INT CIndexEditorMain::CommandSubfield(INT nCommand)
{
	//nCommand : 0 -> ���ñ�ȣ�߰�
	//nCommand : 1 -> �ĺ���ȣ����
	
	structIndexIndicator* pIndexIndicator = NULL;
	structIndexSubfield* pIndexSubfield = NULL;

	CString strTag,strSubfield;
	
	pIndexSubfield = GetCurrentSubfield(strTag,strSubfield);

	if(!pIndexSubfield) return -1;

	if(0 == nCommand)
	{
		
		//RULE 1: AL �� �ƴ� ���ñ�ȣ ������ �Էµ� ���, 
		//sibling�߿� AL���ñ�ȣ�� ������ �̸� �����Ѵ�
		
		//RULE 2: AL ���ñ�ȣ ������ �Էµ� ���,
		//��Ÿ�� ��� sibling�� �����Ѵ�

		
		CIndexIndicatorType dlg(this);
			
		dlg.m_nOpenMode = 1; 
		dlg.m_strIndexGroup = m_strIndexGroup;
		dlg.m_strTag = strTag;

		if(dlg.DoModal() == IDOK) 
		{
			
			//������ �߰�
			pIndexIndicator = new structIndexIndicator;
			if(!pIndexIndicator) return -1;
			
			pIndexIndicator->strIndicator = dlg.m_strIndicator;
			pIndexIndicator->strType = dlg.m_strIndicatorType;
			pIndexIndicator->strSingleOption = dlg.m_strSingleOption;

			if(CheckDuplicatedIndicator(pIndexSubfield,dlg.m_strIndicator)<0) 
			{
				AfxMessageBox(_T("�ش� �ĺ���ȣ ���� ������ ���ñ�ȣ�� �ֽ��ϴ�!"));
				return -1;			
			}

			if(_T("AL") == pIndexIndicator->strIndicator)
			{
				POSITION pos = pIndexSubfield->IndexIndicatorList.GetHeadPosition();
				
				structIndexIndicator* pSiblingIndicator = NULL;

				while(pos)
				{
					pSiblingIndicator = (structIndexIndicator*)pIndexSubfield->IndexIndicatorList.GetNext(pos);
					delete pSiblingIndicator;
					pSiblingIndicator = NULL;
				}

				pIndexSubfield->IndexIndicatorList.RemoveAll();

			}
			else
			{

				POSITION pos = pIndexSubfield->IndexIndicatorList.GetHeadPosition();
				POSITION posOld;

				structIndexIndicator* pSiblingIndicator = NULL;

				while(pos)
				{
					posOld = pos;
					pSiblingIndicator = (structIndexIndicator*)pIndexSubfield->IndexIndicatorList.GetNext(pos);
					
					if(_T("AL") == pSiblingIndicator->strIndicator)
					{
						delete pSiblingIndicator;
						pSiblingIndicator = NULL;
						pIndexSubfield->IndexIndicatorList.RemoveAt(posOld);
						break;
					}
				}
				
			}


			pIndexSubfield->IndexIndicatorList.AddTail((CObject*)pIndexIndicator);
			
			//Ʈ�� ��� �߰�
			SetTreeNodeIndicator(dlg.m_strIndicator,dlg.m_strIndicatorType);

		}
	}
	else if(1 == nCommand)
	{

		CString strMsg;

		strMsg.Format(_T("%s �ĺ���ȣ�� ���� �Ͻðڽ��ϱ�?"),
					  pIndexSubfield->strSubfield);
			
		if(AfxMessageBox(strMsg,MB_YESNO) != IDYES) return 0;

		
		//������ ���� 
		if(DeleteCurrentSubfield()<0)
		{
			AfxMessageBox(_T("������ �����Ͽ����ϴ�!"));
		}
		else
		{
			//��� ���� 
			HTREEITEM hItemSubfield = m_ctrlTree.GetSelectedItem();
			m_ctrlTree.DeleteItem(hItemSubfield);
			
			AfxMessageBox(_T("�����Ͽ����ϴ�!"));
		}


	}

	return 0;
}

INT CIndexEditorMain::CommandIndicator(INT nCommand)
{
	//nCommand : 0 -> ���ñ�ȣ����
	//nCommand : 1 -> ���ñ�ȣ����

	structIndexIndicator* pIndexIndicator = NULL;
	structIndexSubfield* pIndexSubfield = NULL;

	CString strTag,strSubfield,strIndicator;
	
	pIndexIndicator = GetCurrentIndicator(strTag,strSubfield,strIndicator);
	pIndexSubfield = FindSubfield(strTag,strSubfield);


	if(!pIndexIndicator|| !pIndexSubfield) return -1;

	
	if(0 == nCommand)
	{
		//RULE 1: AL �� �ƴ� ���ñ�ȣ�� ������ ���, 
		//�ڱ� �ڽ��� ������ sibling�߿� AL���ñ�ȣ�� ������ �̸� �����Ѵ�
		
		//RULE 2: AL ���ñ�ȣ�� ������ ������ ���, ,
		//�ڱ� �ڽ��� ������ ��� sibling�� �����Ѵ�

		
		CIndexIndicatorType dlg(this);
		
		dlg.m_nOpenMode = -1; 
		dlg.m_strIndexGroup = m_strIndexGroup;
		dlg.m_strTag = strTag;
		dlg.m_strIndicator = strIndicator;
		dlg.m_strIndicatorType = pIndexIndicator->strType;
		dlg.m_strSingleOption = pIndexIndicator->strSingleOption;

		if(dlg.DoModal() == IDOK) 
		{
			//�ߺ��˻�
			if(strIndicator != dlg.m_strIndicator)
			{
				if(CheckDuplicatedIndicator(pIndexSubfield,
											dlg.m_strIndicator,
											pIndexIndicator,
											-1)<0) 
				{
					AfxMessageBox(_T("�ش� �ĺ���ȣ ���� ������ ���ñ�ȣ�� �ֽ��ϴ�!"));
					return -1;	
				}
			}

			pIndexIndicator->strIndicator = dlg.m_strIndicator;
			pIndexIndicator->strType = dlg.m_strIndicatorType;
			pIndexIndicator->strSingleOption = dlg.m_strSingleOption;
		
			if(_T("AL") == pIndexIndicator->strIndicator) //RULE 2
			{
				
				POSITION pos = pIndexSubfield->IndexIndicatorList.GetHeadPosition();
				POSITION posOld;

				structIndexIndicator* pSiblingIndicator = NULL;

				while(pos)
				{
					
					posOld = pos;
					pSiblingIndicator = (structIndexIndicator*)pIndexSubfield->IndexIndicatorList.GetNext(pos);
					if(pSiblingIndicator == pIndexIndicator) continue;
					
					delete pSiblingIndicator;
					pSiblingIndicator = NULL;
					pIndexSubfield->IndexIndicatorList.RemoveAt(posOld);

				}

			}
			else //RULE 1
			{

				POSITION pos = pIndexSubfield->IndexIndicatorList.GetHeadPosition();
				POSITION posOld;

				structIndexIndicator* pSiblingIndicator = NULL;

				while(pos)
				{
					posOld = pos;
					pSiblingIndicator = (structIndexIndicator*)pIndexSubfield->IndexIndicatorList.GetNext(pos);
					
					if(_T("AL") == pSiblingIndicator->strIndicator)
					{
						delete pSiblingIndicator;
						pSiblingIndicator = NULL;
						pIndexSubfield->IndexIndicatorList.RemoveAt(posOld);
						break;
					}
				}
				
			}

			SetTreeNodeIndicator(dlg.m_strIndicator,dlg.m_strIndicatorType,-1);
		}

	}
	else if(1 == nCommand)
	{
		
		CString strMsg;

		strMsg.Format(_T("%s ���ñ�ȣ�� ���� �Ͻðڽ��ϱ�?"),
					  pIndexIndicator->strIndicator);
			
		if(AfxMessageBox(strMsg,MB_YESNO) != IDYES) return 0;


		//������ ���ñ�ȣ�� ������ ���ñ�ȣ�� ������ ���´�
		INT nCount = pIndexSubfield->IndexIndicatorList.GetCount();
		if(1 == nCount) 
		{
			strMsg.Format(_T("�ش� ���ñ�ȣ�� �ĺ���ȣ�� ������ ���ñ�ȣ�̹Ƿ�\n")
						  _T("������ �� �����ϴ�!"));
					
			AfxMessageBox(strMsg);
			return 0;
		}
		
		//������ ���� 
		if(DeleteCurrentIndicator()<0)
		{
			AfxMessageBox(_T("������ �����Ͽ����ϴ�!"));
		}
		else
		{
			//��� ���� 
			HTREEITEM hItemIndicator = m_ctrlTree.GetSelectedItem();
			m_ctrlTree.DeleteItem(hItemIndicator);
			
			AfxMessageBox(_T("�����Ͽ����ϴ�!"));
		}

	}


	return 0;
}



structIndexIndicator* CIndexEditorMain::GetCurrentIndicator(CString& strCurrenTag, 
															CString& strCurrentSubfield,
															CString& strCurrentIndicator)
{
	structIndexTag* pTag = NULL;
	structIndexSubfield* pSubfield = NULL;
	structIndexIndicator* pIndicator = NULL;

	
	HTREEITEM hItemIndicator = m_ctrlTree.GetSelectedItem();
	HTREEITEM hItemSubfield = m_ctrlTree.GetParentItem(hItemIndicator);
	HTREEITEM hItemTag = m_ctrlTree.GetParentItem(hItemSubfield);

	CString strTag = m_ctrlTree.GetItemText(hItemTag);
	CString strSubfield = m_ctrlTree.GetItemText(hItemSubfield);
	CString strIndicator = m_ctrlTree.GetItemText(hItemIndicator);
	
	if(strTag.IsEmpty() || strSubfield.IsEmpty() || strIndicator.IsEmpty()) return NULL;
	
	strCurrenTag = strTag;
	strCurrentSubfield = strSubfield;
	strCurrentIndicator = strIndicator;

	POSITION posTag = m_pCurrentIndex->IndexTagList.GetHeadPosition();
	POSITION posSubfield;
	POSITION posIndicator;

	while(posTag)
	{
		pTag = (structIndexTag*)m_pCurrentIndex->IndexTagList.GetNext(posTag);
		
		if(pTag)
		{
			if(strTag == pTag->strTag)
			{
				posSubfield = pTag->IndexSubfieldList.GetHeadPosition();
				
				while(posSubfield)
				{
					pSubfield = (structIndexSubfield*)pTag->IndexSubfieldList.GetNext(posSubfield);
					
					if(pSubfield)
					{
						if(strSubfield == pSubfield->strSubfield)
						{
							posIndicator = pSubfield->IndexIndicatorList.GetHeadPosition();
							
							while(posIndicator)
							{
								pIndicator = (structIndexIndicator*)pSubfield->IndexIndicatorList.GetNext(posIndicator);
								
								if(strIndicator == pIndicator->strIndicator) return pIndicator;
		
							}

						}


					}
					

				}

			}
		
		}
	}

	return NULL;
}

structIndexTag* CIndexEditorMain::GetCurrentTag(CString& strCurrenTag)
{
	structIndexTag* pTag = NULL;
	
	HTREEITEM hItemTag = m_ctrlTree.GetSelectedItem();

	CString strTag = m_ctrlTree.GetItemText(hItemTag);

	if(strTag.IsEmpty()) return NULL;
	
	strCurrenTag = strTag;

	POSITION posTag = m_pCurrentIndex->IndexTagList.GetHeadPosition();

	while(posTag)
	{
		pTag = (structIndexTag*)m_pCurrentIndex->IndexTagList.GetNext(posTag);
		
		if(pTag)
		{
			if(strTag == pTag->strTag) return pTag;
		}
	}

	return NULL;
}

structIndexSubfield* CIndexEditorMain::GetCurrentSubfield(CString& strCurrenTag, 
														  CString& strCurrentSubfield)
															
{
	structIndexTag* pTag = NULL;
	structIndexSubfield* pSubfield = NULL;

	
	HTREEITEM hItemSubfield = m_ctrlTree.GetSelectedItem();
	HTREEITEM hItemTag = m_ctrlTree.GetParentItem(hItemSubfield);

	CString strTag = m_ctrlTree.GetItemText(hItemTag);
	CString strSubfield = m_ctrlTree.GetItemText(hItemSubfield);
	
	if(strTag.IsEmpty() || strSubfield.IsEmpty()) return NULL;
	
	strCurrenTag = strTag;
	strCurrentSubfield = strSubfield;

	POSITION posTag = m_pCurrentIndex->IndexTagList.GetHeadPosition();
	POSITION posSubfield;


	while(posTag)
	{
		pTag = (structIndexTag*)m_pCurrentIndex->IndexTagList.GetNext(posTag);
		
		if(pTag)
		{
			if(strTag == pTag->strTag)
			{
				posSubfield = pTag->IndexSubfieldList.GetHeadPosition();
				
				while(posSubfield)
				{
					pSubfield = (structIndexSubfield*)pTag->IndexSubfieldList.GetNext(posSubfield);
					
					if(strSubfield == pSubfield->strSubfield) return pSubfield;

				}

			}
		
		}
	}

	return NULL;
}


structIndexSubfield* CIndexEditorMain::FindSubfield(CString strTag,CString strSubfield)
{
	CObject* pObject = NULL;
	structIndex* pIndex = NULL;
	structIndexTag* pTag = NULL;
	structIndexSubfield* pSubfield = NULL;
	
	if(strTag.IsEmpty() || strSubfield.IsEmpty()) return NULL;

	POSITION posTag = m_pCurrentIndex->IndexTagList.GetHeadPosition();
	POSITION posSubfield;


	while(posTag)
	{
		pTag = (structIndexTag*)m_pCurrentIndex->IndexTagList.GetNext(posTag);
		
		if(pTag)
		{
			if(strTag == pTag->strTag)
			{
				posSubfield = pTag->IndexSubfieldList.GetHeadPosition();
				
				while(posSubfield)
				{
					pSubfield = (structIndexSubfield*)pTag->IndexSubfieldList.GetNext(posSubfield);
					
					if(strSubfield == pSubfield->strSubfield) return pSubfield;

				}

			}
		
		}
	}

	return NULL;

}

INT CIndexEditorMain::DeleteCurrentTag()
{
	
	structIndexTag* pTag = NULL;
	structIndexSubfield* pSubfield = NULL;
	structIndexIndicator* pIndicator = NULL;
	
	HTREEITEM hItemTag = m_ctrlTree.GetSelectedItem();

	CString strTag = m_ctrlTree.GetItemText(hItemTag);


	if(strTag.IsEmpty()) return -1;
	
	POSITION posTag = m_pCurrentIndex->IndexTagList.GetHeadPosition();
	POSITION posSubfield;
	POSITION posIndicator;
	POSITION posOld;

	while(posTag)
	{
		posOld = posTag;
		pTag = (structIndexTag*)m_pCurrentIndex->IndexTagList.GetNext(posTag);
		
		if(pTag)
		{
			if(strTag == pTag->strTag)
			{
				if(_T("U") == pTag->strType)
				{
					AfxMessageBox(_T("�ش� �±״� ����� ���� �±װ� �ƴϹǷ� \n")
								  _T("���� �� �� �����ϴ�!"));
					return -1;

				}

				posSubfield = pTag->IndexSubfieldList.GetHeadPosition();
				

				while(posSubfield)
				{
					pSubfield = (structIndexSubfield*)pTag->IndexSubfieldList.GetNext(posSubfield);
					
					if(pSubfield)
					{
						posIndicator = pSubfield->IndexIndicatorList.GetHeadPosition();
							
						while(posIndicator)
						{
							pIndicator = (structIndexIndicator*)pSubfield->IndexIndicatorList.GetNext(posIndicator);
								
							if(pIndicator) 
							{
								delete pIndicator;
								pIndicator = NULL;
							}
						}
	
						pSubfield->IndexIndicatorList.RemoveAll();	
						delete pSubfield;
						pSubfield = NULL;
					}
				
				}

				pTag->IndexSubfieldList.RemoveAll();	
				
				delete pTag;
				pTag = NULL;
				m_pCurrentIndex->IndexTagList.RemoveAt(posOld);
				return 0;
			}
		}
	}


	return -1;

}

INT CIndexEditorMain::DeleteCurrentSubfield()
{
	
	structIndexTag* pTag = NULL;
	structIndexSubfield* pSubfield = NULL;
	structIndexIndicator* pIndicator = NULL;
	

	HTREEITEM hItemSubfield = m_ctrlTree.GetSelectedItem();
	HTREEITEM hItemTag = m_ctrlTree.GetParentItem(hItemSubfield);

	CString strTag = m_ctrlTree.GetItemText(hItemTag);
	CString strSubfield = m_ctrlTree.GetItemText(hItemSubfield);

	if(strTag.IsEmpty() || strSubfield.IsEmpty()) return -1;
	
	POSITION posTag = m_pCurrentIndex->IndexTagList.GetHeadPosition();
	POSITION posSubfield;
	POSITION posIndicator;
	POSITION posOld;

	while(posTag)
	{
		pTag = (structIndexTag*)m_pCurrentIndex->IndexTagList.GetNext(posTag);
		
		if(pTag)
		{
			if(strTag == pTag->strTag)
			{
				posSubfield = pTag->IndexSubfieldList.GetHeadPosition();
				
				while(posSubfield)
				{
					posOld = posSubfield;
					pSubfield = (structIndexSubfield*)pTag->IndexSubfieldList.GetNext(posSubfield);
					
					if(pSubfield)
					{
						if(strSubfield == pSubfield->strSubfield)
						{
							posIndicator = pSubfield->IndexIndicatorList.GetHeadPosition();
							
							while(posIndicator)
							{
								pIndicator = (structIndexIndicator*)pSubfield->IndexIndicatorList.GetNext(posIndicator);
								
								if(pIndicator) 
								{
									delete pIndicator;
									pIndicator = NULL;
								}
							}
	
							pSubfield->IndexIndicatorList.RemoveAll();	
						
							delete pSubfield;
							pSubfield = NULL;
							pTag->IndexSubfieldList.RemoveAt(posOld);
							return 0;						
	
						}

					}
				}

			}
		
		}
	}
	

	return -1;
}

INT CIndexEditorMain::DeleteCurrentIndicator()
{

	structIndexTag* pTag = NULL;
	structIndexSubfield* pSubfield = NULL;
	structIndexIndicator* pIndicator = NULL;
	
	HTREEITEM hItemIndicator = m_ctrlTree.GetSelectedItem();
	HTREEITEM hItemSubfield = m_ctrlTree.GetParentItem(hItemIndicator);
	HTREEITEM hItemTag = m_ctrlTree.GetParentItem(hItemSubfield);

	CString strTag = m_ctrlTree.GetItemText(hItemTag);
	CString strSubfield = m_ctrlTree.GetItemText(hItemSubfield);
	CString strIndicator = m_ctrlTree.GetItemText(hItemIndicator);
	
	if(strTag.IsEmpty() || strSubfield.IsEmpty() || strIndicator.IsEmpty()) return -1;
	
	POSITION posTag = m_pCurrentIndex->IndexTagList.GetHeadPosition();
	POSITION posSubfield;
	POSITION posIndicator;
	POSITION posOld;

	while(posTag)
	{
		pTag = (structIndexTag*)m_pCurrentIndex->IndexTagList.GetNext(posTag);
		
		if(pTag)
		{
			if(strTag == pTag->strTag)
			{
				posSubfield = pTag->IndexSubfieldList.GetHeadPosition();
				
				while(posSubfield)
				{
					pSubfield = (structIndexSubfield*)pTag->IndexSubfieldList.GetNext(posSubfield);
					
					if(pSubfield)
					{
						if(strSubfield == pSubfield->strSubfield)
						{
							posIndicator = pSubfield->IndexIndicatorList.GetHeadPosition();
							
							while(posIndicator)
							{
								posOld = posIndicator;
								pIndicator = (structIndexIndicator*)pSubfield->IndexIndicatorList.GetNext(posIndicator);
								
								if(strIndicator == pIndicator->strIndicator) 
								{
									// KOL.UDF.022 ��ť���ڵ� ����
									if(&pIndicator) 
									//if(pIndicator) 
									{
										delete pIndicator;
										pIndicator = NULL;
										pSubfield->IndexIndicatorList.RemoveAt(posOld);
				
										return 0;
										
									}				
															
								}
							}

						}


					}
					

				}

			}
		
		}
	}

	return -1;
}



INT CIndexEditorMain::CheckDuplicatedTag(CString strTag)
{
	if(strTag.IsEmpty()) return -1;

	structIndexTag* pTag = NULL;

	POSITION posTag = m_pCurrentIndex->IndexTagList.GetHeadPosition();

	while(posTag)
	{
		pTag = (structIndexTag*)m_pCurrentIndex->IndexTagList.GetNext(posTag);
		
		if(pTag)
		{
			if(strTag == pTag->strTag)
			{
				return -1;

			}
		
		}
	}

	return 0;
}


INT CIndexEditorMain::CheckDuplicatedSubfield(CString strTag,CString strSubfield)
{
	if(strTag.IsEmpty() || strSubfield.IsEmpty()) return -1;

	
	structIndexTag* pTag = NULL;
	structIndexSubfield* pSubfield = NULL;
	
	POSITION posTag = m_pCurrentIndex->IndexTagList.GetHeadPosition();
	POSITION posSubfield;


	while(posTag)
	{
		pTag = (structIndexTag*)m_pCurrentIndex->IndexTagList.GetNext(posTag);
		
		if(pTag)
		{
			if(strTag == pTag->strTag)
			{
				posSubfield = pTag->IndexSubfieldList.GetHeadPosition();
				
				while(posSubfield)
				{
					pSubfield = (structIndexSubfield*)pTag->IndexSubfieldList.GetNext(posSubfield);
					
					if(strSubfield == pSubfield->strSubfield) return -1;

				}
				
				return 0;
			}
		
		}
	}

	  
	return 0;

}

INT CIndexEditorMain::CheckDuplicatedIndicator(structIndexSubfield* pSubfield,
											   CString strIndicator,
											   structIndexIndicator* pIndicator,
											   INT nCheckType)
{
	//sibling �߿� ������ indicator�� �ִ����� �����Ѵ�
	//nCheckType : 1 �Է½�, nCheckType : -1 ������
	
	structIndexIndicator* pIndexIndicator = NULL;

	if(nCheckType>0)
	{
		POSITION pos = pSubfield->IndexIndicatorList.GetHeadPosition();

		while(pos)
		{
			pIndexIndicator = (structIndexIndicator*)pSubfield->IndexIndicatorList.GetNext(pos);
			
			if(pIndexIndicator) 
			{
				if(strIndicator == pIndexIndicator->strIndicator) return -1;
			}

		}
	}
	else
	{
		POSITION pos = pSubfield->IndexIndicatorList.GetHeadPosition();

		while(pos)
		{
			pIndexIndicator = (structIndexIndicator*)pSubfield->IndexIndicatorList.GetNext(pos);
			
			if(pIndexIndicator) 
			{
				if(pIndicator == pIndexIndicator) continue; //�ڱ� �ڽ��� ����
				if(strIndicator == pIndexIndicator->strIndicator) return -1;
			}

		}


	}


	return 0;
}


INT CIndexEditorMain::SetTreeNodeTag(CString strTag,
									 CString strType,
								     INT nSetType)
{
	//�±� ��带 �Է�,�����Ѵ�. (nSetType : 1 �Է�,  nSetType : -1 ����)
		
	INT nImage = -1;

	if(nSetType>0) //�Է½�
	{
		
		HTREEITEM hItemIndex = m_ctrlTree.GetSelectedItem();
		
		TVINSERTSTRUCT tvinsert;
		tvinsert.hInsertAfter = TVI_LAST;
		tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE;
		tvinsert.item.state = 0;
		tvinsert.item.stateMask = 0;
		tvinsert.item.cchTextMax = 60;
		tvinsert.item.cChildren = 0;
		tvinsert.item.iSelectedImage = 0;
	
		if(strType.IsEmpty()) nImage = 1;
		else nImage = GetImageNumber(strType);

		tvinsert.hParent = hItemIndex;
		tvinsert.item.pszText = strTag.GetBuffer(0);
		
		HTREEITEM hItemTag = m_ctrlTree.InsertItem(&tvinsert);
		m_ctrlTree.SetItemImage(hItemTag,nImage,nImage);

		
	}
	else //������
	{
		
		HTREEITEM hItemTag = m_ctrlTree.GetSelectedItem();
		
	
		if(strType.IsEmpty()) nImage = 1;
		else nImage = GetImageNumber(strType);

		m_ctrlTree.SetItemImage(hItemTag,nImage,nImage);
	}



	return 0;
}


INT CIndexEditorMain::SetTreeNodeIndicator(CString strIndicator,
										   CString strType,
										   INT nSetType)
{
	//���ñ�ȣ ��带 �Է�,�����Ѵ�. (nSetType : 1 �Է�,  nSetType : -1 ����)
		
	if(nSetType>0) //�Է½�
	{
		//RULE 1: AL �� �ƴ� ���ñ�ȣ ������ �Էµ� ���, 
		//sibling�߿� AL���ñ�ȣ�� ������ �̸� �����Ѵ�
		
		//RULE 2: AL ���ñ�ȣ ������ �Էµ� ���,
		//��Ÿ�� ��� sibling�� �����Ѵ�
		
	
		if(_T("AL") == strIndicator) //RULE 2
		{
			HTREEITEM hItemSubfield = m_ctrlTree.GetSelectedItem();
			HTREEITEM hItem = m_ctrlTree.GetChildItem(hItemSubfield);
			HTREEITEM hItemOld = hItem;
			
		   while (hItem != NULL)
		   {
			   hItem = m_ctrlTree.GetNextSiblingItem(hItem);
		   
			   m_ctrlTree.DeleteItem(hItemOld);
		
			   hItemOld = hItem;
		   }
		}
		else //RULE 1
		{

			HTREEITEM hItemSubfield = m_ctrlTree.GetSelectedItem();
			HTREEITEM hItem = m_ctrlTree.GetChildItem(hItemSubfield);
			CString strText;
			
		   while (hItem != NULL)
		   {
			  strText = m_ctrlTree.GetItemText(hItem);
			 
			  if(_T("AL") == strText)  {
				  m_ctrlTree.DeleteItem(hItem);
				  break;
			  }

			   hItem = m_ctrlTree.GetNextSiblingItem(hItem);
		   }

		}

	
		HTREEITEM hItemSubfield = m_ctrlTree.GetSelectedItem();
		
		TVINSERTSTRUCT tvinsert;
		tvinsert.hInsertAfter = TVI_LAST;
		tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE;
		tvinsert.item.state = 0;
		tvinsert.item.stateMask = 0;
		tvinsert.item.cchTextMax = 60;
		tvinsert.item.cChildren = 0;
		tvinsert.item.iSelectedImage = 0;
	
		INT nImage = GetImageNumber(strType);
		tvinsert.hParent = hItemSubfield;
		tvinsert.item.pszText = (TCHAR*)strIndicator.GetBuffer(0);
		
		HTREEITEM hItemIndicator = m_ctrlTree.InsertItem(&tvinsert);
		m_ctrlTree.SetItemImage(hItemIndicator,nImage,nImage);
		
	}
	else //������
	{
		//RULE 1: AL �� �ƴ� ���ñ�ȣ�� ������ ���, 
		//�ڱ� �ڽ��� ������ sibling�߿� AL���ñ�ȣ�� ������ �̸� �����Ѵ�
		
		//RULE 2: AL ���ñ�ȣ�� ������ ������ ���,
		//�ڱ� �ڽ��� ������ ��� sibling�� �����Ѵ�

		
		if(_T("AL") == strIndicator) //RULE 2
		{
			HTREEITEM hItemIndicator = m_ctrlTree.GetSelectedItem();
			HTREEITEM hItemSubfield = m_ctrlTree.GetParentItem(hItemIndicator);
			HTREEITEM hItem = m_ctrlTree.GetChildItem(hItemSubfield);
			HTREEITEM hItemOld = hItem;
			
		   while (hItem != NULL)
		   {
			   hItem = m_ctrlTree.GetNextSiblingItem(hItem);
		   
			   if(hItemIndicator != hItemOld) m_ctrlTree.DeleteItem(hItemOld);
		
			   hItemOld = hItem;
		   }
		}
		else //RULE 1
		{
			
			HTREEITEM hItemIndicator = m_ctrlTree.GetSelectedItem();
			HTREEITEM hItemSubfield = m_ctrlTree.GetParentItem(hItemIndicator);
			HTREEITEM hItem = m_ctrlTree.GetChildItem(hItemSubfield);
			CString strText;
			
		   while (hItem != NULL)
		   {
			  strText = m_ctrlTree.GetItemText(hItem);
			 
			  if(_T("AL") == strText)  {
				  m_ctrlTree.DeleteItem(hItem);
				  break;
			  }

			   hItem = m_ctrlTree.GetNextSiblingItem(hItem);
		   }

		}

		
		HTREEITEM hItemIndicator = m_ctrlTree.GetSelectedItem();
		INT nImage = GetImageNumber(strType);
		m_ctrlTree.SetItemImage(hItemIndicator,nImage,nImage);
		m_ctrlTree.SetItemText(hItemIndicator,strIndicator);
	}



	return 0;
}

INT CIndexEditorMain::SetTreeNodeSubfield(CString strSubfield)
{
	
	HTREEITEM hItemTag = m_ctrlTree.GetSelectedItem();
	
	TVINSERTSTRUCT tvinsert;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE;
	tvinsert.item.state = 0;
	tvinsert.item.stateMask = 0;
	tvinsert.item.cchTextMax = 60;
	tvinsert.item.cChildren = 0;
	tvinsert.item.iSelectedImage = 0;
	

	//�߰��� �ĺ���ȣ 
	tvinsert.hParent = hItemTag;
	tvinsert.item.pszText = (TCHAR*)strSubfield.GetBuffer(0);
		
	HTREEITEM hItemSubfield = m_ctrlTree.InsertItem(&tvinsert);
	m_ctrlTree.SetItemImage(hItemSubfield,0,0);

	//�⺻�� ���ñ�ȣ
	CString strIndicator = _T("AL");
	INT nImage = GetImageNumber(_T("A"));
	tvinsert.hParent = hItemSubfield;
	tvinsert.item.pszText = (TCHAR*)strIndicator.GetBuffer(0);
	HTREEITEM hItemIndicator = m_ctrlTree.InsertItem(&tvinsert);
	
	m_ctrlTree.SetItemImage(hItemIndicator,nImage,nImage);

	return 0;
}