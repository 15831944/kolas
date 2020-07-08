// CatCodeGetterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "CatCodeGetterDlg.h"

#include "..\..\..\ESL\MARC\MarcToolKit\CreateAutorCode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCatCodeGetterDlg dialog


CCatCodeGetterDlg::CCatCodeGetterDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCatCodeGetterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCatCodeGetterDlg::~CCatCodeGetterDlg()
{
}

BOOL CCatCodeGetterDlg::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CCatCodeGetterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCatCodeGetterDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCatCodeGetterDlg, CDialog)
	//{{AFX_MSG_MAP(CCatCodeGetterDlg)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_bFIND, OnbFIND)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatCodeGetterDlg message handlers

BOOL CCatCodeGetterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("CatCodeGetterDlg") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : CatCodeGetterDlg") );
		return false;
	}	

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CCatCodeGetterDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	( (CEdit*) GetDlgItem (IDC_eAUTHOR_CCG) )->SetFocus();
	CDialog::OnShowWindow(bShow, nStatus);	
}

VOID CCatCodeGetterDlg::OnbFIND() 
{

	CString condition[ 2 ];
	if( GetSearchCondition( condition ) )
		return;
	
	CESL_DataMgr *pDM = FindDM( _T("DM_CatCodeGetterDlg") );
	pDM->FreeData();

	
//	if( SearchCatCode( MakeFilePath( condition[ 0 ] ), condition[ 1 ], pDM ) )
//	{
//		AfxMessageBox( _T("������ ã�� �� �����ϴ�.") );
//		return;
//	}
	
	CString sResult = SearchCatCode(condition[0], condition[1]);

	if( sResult == _T("") ) 
	{
		;
	}
	else
	{
		CString alias[ 2 ] = { _T("AUTHOR"), _T("SIGN") };
		CString data[ 2 ];

		data[0] = condition[1];
		data[1] = sResult;

		pDM->SetCellData( alias, 2, data, -1 );

		ShowGrid();
	}
}

INT CCatCodeGetterDlg::GetSearchCondition( CString condition[] )
{
	GetControlData( _T("CM_CatCodeGetterDlg"), _T("���ڱ�ȣ"), condition[ 0 ], -1, 1 );

	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_eAUTHOR_CCG );
	pEdt->GetWindowText( condition[ 1 ] );

	if( ConvertCondition( condition[ 1 ] ) )
	{
		AfxMessageBox( _T("�����ڸ� �ùٸ� ���·� �Է��Ͻʽÿ�") );
		return -1;
	}
	
	return 0;
}

CString CCatCodeGetterDlg::MakeFilePath( CString condition )
{
	CString path;
	path.Format( _T("..\\cfg\\%s.txt"), condition );

	return path;
}	

INT CCatCodeGetterDlg::SearchCatCode( CString path, CString condition, CESL_DataMgr *pDM )
{
	CStdioFile fp;
	if( !fp.Open( path, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary ) )
		return -1;
			TCHAR cUni;
			fp.Read( &cUni, sizeof(TCHAR) );
			if( 0xFEFF != cUni )
			{
				fp.SeekToBegin();
			}

	
	CString alias[ 2 ] = { _T("AUTHOR"), _T("SIGN") };
	CString data[ 2 ];
	
	CString row;
	INT locate = 0;
	INT cnt = 0;
	INT ids = 0;

	CString strPrevCode;
	while( fp.ReadString( row ) )
	{
		locate = row.Find( _T("|"), 0 );
		if( locate > 0 )
		{
			data[ 0 ] = row.Left( locate );
			data[ 0 ].Replace( _T(" "), _T("") );

			data[ 1 ] = row.Right( row.GetLength() - locate );
			data[ 1 ].Replace( _T(" "), _T("") );
			data[ 1 ].Replace( _T("|"), _T("") );

			ids = _tcscmp(condition, data[ 0 ]);
			if (ids == 0)
			{
				data[ 0 ] = m_author;
				pDM->SetCellData( alias, 2, data, -1 );
				break;
			}
			else if (ids < 0)
			{
				data[ 0 ] = m_author;
				if (!strPrevCode.IsEmpty())
					data[1] = strPrevCode;
				pDM->SetCellData( alias, 2, data, -1 );
				break;
			}

			strPrevCode = data[1];
		}
	}

	fp.Close();
	
	return 0;	
}

VOID CCatCodeGetterDlg::ShowGrid()
{
	ControlDisplay( _T("CM_CatCodeGetterDlg"), _T("�׸���") );
}

INT CCatCodeGetterDlg::ConvertCondition( CString &condition )
{
	//RemoveEndglish( condition );
	m_author = condition;
	
	return 0;

	INT length = condition.GetLength();
	if( length < 3 )
		return -1;

	condition = condition.Right( length - 2 );
	if( _T(",") == condition.Left( 1 ) )
		condition = condition.Left( 3 );
	else
		condition = condition.Left( 2 );

	return 0;
}

VOID CCatCodeGetterDlg::RemoveEndglish( CString &string )
{
	string.Replace( _T(" "), _T("") );
	
	CString tmp;
	for( INT i = 33; i < 127; i++ )
		if( 44 == i )
			continue;
		else
		{
			tmp.Format( _T("%c"), i );
			string.Replace( tmp, _T("") );
		}
}

CString CCatCodeGetterDlg::SearchCatCode(CString strRuleType, CString strItemData)
{
	INT ids = -1;

	CCreateAutorCode *pCreateAuthorCode = NULL;

	CString strTagCount;
	CString strTag;
	CString strSubfield;
	//CString strRuleType;

	//===================================================
	//2009.05.07 UPDATE BY PJW : ��� ���ð�� ����ؼ� UPPERCASE�Ѵ�.
	strRuleType.MakeUpper();
	//===================================================
	

	// �����Ͱ� ���̸�
		if (strItemData.IsEmpty())
		{
			AfxMessageBox (_T("���ڱ�ȣ������ �߸��Ǿ����ϴ�."));
			return _T("");
		}

		// ���ڱ�ȣ ������ ���� strItemData : result�Դϴ�.
		
		// ���ϼ�
		if (strRuleType == _T("���ϼ�"))
		{
			if (!pCreateAuthorCode)
				pCreateAuthorCode = new CCreateAutorCode;

			CString strAuthorAddData;
			CString strAuthorCode;

			ids = pCreateAuthorCode->GetJangIlSeaAuthorData(strItemData.GetBuffer(0), strAuthorAddData.GetBuffer(10), strAuthorCode.GetBuffer(10));
			strAuthorAddData.ReleaseBuffer();
			strAuthorCode.ReleaseBuffer();

			//if (ids < 0) return _T("");

			strItemData = strAuthorCode;
		}
		//���ʸ�
		else if( strRuleType == _T("���ʸ�") )
		{
			if (!pCreateAuthorCode)
				pCreateAuthorCode = new CCreateAutorCode;
			
			CString strAuthorAddData;
			CString strAuthorCode;
			
			pCreateAuthorCode->GetJungPilMoAuthorData(strItemData.GetBuffer(0), strAuthorAddData.GetBuffer(10), strAuthorCode.GetBuffer(10));
			strAuthorAddData.ReleaseBuffer();
			strAuthorCode.ReleaseBuffer();
			
			//if (ids < 0) return _T("");
			
			strItemData = strAuthorCode;
			
		}
		//���缭���ڱ�ȣ
		else if( strRuleType == _T("���缭���ڱ�ȣ") )
		{
			if (!pCreateAuthorCode)
				pCreateAuthorCode = new CCreateAutorCode;

			CString strAuthorAddData;
			CString strAuthorCode;

			strAuthorCode = pCreateAuthorCode->GetOrientBookData(strItemData.GetBuffer(0), strAuthorAddData.GetBuffer(10), strAuthorCode.GetBuffer(10));
			strAuthorAddData.ReleaseBuffer();
			strAuthorCode.ReleaseBuffer();

			//if (ids < 0) return _T("");

			strItemData = strAuthorCode;

		}
		else if ( strRuleType == _T("ī�ͼ���") )
		{
			if (!pCreateAuthorCode)
				pCreateAuthorCode = new CCreateAutorCode;
			
			CString strAuthorAddData;
			CString strAuthorCode;
			
			strAuthorCode = pCreateAuthorCode->GetCutterSanbornAuthorData(strItemData.GetBuffer(0), strAuthorAddData.GetBuffer(10), strAuthorCode.GetBuffer(10));
			strAuthorAddData.ReleaseBuffer();
			strAuthorCode.ReleaseBuffer();
			
			//if (ids < 0) return _T("");
			
			strItemData = strAuthorCode;
		
		}
		else if ( strRuleType == _T("ELORD") ) 
		{
			//===================================================
			//2009.10.07 ADD BY PJW : ELORD�ϰ�� �ѹ��� �ѱ۷� �ٲ��ش�.
			//���� �ѱ� ��ȯ
			INT ids = ((CIndexTool*)this->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(strItemData);
			if(ids < 0) 
			{
				AfxMessageBox( _T("���ڿ��� �ѱ� ��ȯ�� �����߽��ϴ�.") );
				return -1;
			}
			//===================================================
			

			if (!pCreateAuthorCode)
				pCreateAuthorCode = new CCreateAutorCode;
			
			CString strAuthorAddData;
			CString strAuthorCode;
			
			//Elord ���ڱ�ȣ ������ ���ڿ� ������ ������� ������������ ©�� ó���Ѵ�.
			INT iTagCnt = _ttoi( strTagCount.GetBuffer(0) );
			for ( INT i = 1 ; i <= iTagCnt ; i++ ) {
				CString sTagSub;
				if ( i == 1 ) {
					sTagSub = strTag.Left(3) + _T("$") + strSubfield.Left(1);
				}
				else {
					INT iStart = (i-1)*3+1;
					INT iTmp = (i-1)*2 +1;
					sTagSub = strTag.Mid(iStart, 3) + _T("$") + strSubfield.Mid( (i-1)*2 , 1 );
				}
				
				//m_pCatESLMgr->GetMarcMgr().GetItem(&m_pCatESLMgr->GetWorkingMarc(), sTagSub, strItemData);
				if ( strItemData != _T("") ) {
					break;
				}
			}

			ids = pCreateAuthorCode->GetElordAuthorDate(strItemData.GetBuffer(0), strAuthorAddData.GetBuffer(10), strAuthorCode.GetBuffer(10));
			strAuthorAddData.ReleaseBuffer();
			strAuthorCode.ReleaseBuffer();
			
			//if (ids < 0) return _T("");
			
			strItemData = strAuthorCode;
			
		}
		else
		{
			//=====================================================
			//2009.01.20 UPDATE BY PJW : ���ڸ��� 1�ڸ��ϰ�� length�������ͼ� üũ�ϰ� mid�߶󳻴ºκ� ����
// 			CString sData = strItemData.Left(2);
// 			CString sAuthorMid, sAuthorNo;
// 			if ( strItemData.Mid(2,1) == _T(" ") ) 
// 				sAuthorMid = _T(",") + strItemData.Mid(3,2);
// 			else 
// 			{
// 				// 2005.09.30 UPDATE BY PDJ
// 				// ������ ������ �̻��ϰ� �����Ͽ� �ٽ� �ּ� ����
// 				sAuthorMid = strItemData.Mid(2,2);
// 				//sAuthorMid = strItemData;
// 			}
			CString sData;
			CString sAuthorMid, sAuthorNo;
			strItemData.TrimLeft();		strItemData.TrimRight();

			if( 2 > strItemData.GetLength() )
			{
				sData = strItemData;
				sAuthorMid = _T("");
			}
			else
			{
				sData = strItemData.Left(1);
				
				if ( strItemData.Mid(1,1) == _T(" ") ) 
					sAuthorMid = _T(",") + strItemData.Mid(2,1);
				else 
				{
					// 2005.09.30 UPDATE BY PDJ
					// ������ ������ �̻��ϰ� �����Ͽ� �ٽ� �ּ� ����
					sAuthorMid = strItemData.Mid(1,1);
					//sAuthorMid = strItemData;
				}
			}
			//=====================================================
			
			if ( sAuthorMid != _T("") ) 
			{			
				//���ϰ��
				CString strFilePath = _T("..\\CFG\\") + strRuleType + _T(".txt");
				
				FILE *fp;
				fp = _tfopen( strFilePath , _T("rb") );
				
				if ( fp == NULL ) {
					AfxMessageBox( _T("���ڱ�ȣ���� ������ �����ϴ�.") );
					return -1;
				}
				//++++++++++++++++++++++++++++++++++++++++++
				//Unicode�϶� 2byte�ǳʶ��
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
				//------------------------------------------
				
				TCHAR cLine[256];
				while( _fgetts( cLine, 256, fp ) ) {
					CString sLine;
					sLine.Format( _T("%s"), cLine );
					CString tnp = sLine.Mid(0,3);
					tnp.Replace( _T(" "), NULL );
					if ( sAuthorMid == tnp ) {
						INT iPos = sLine.Find(_T("|"));
						sAuthorNo = sLine.Mid( iPos+1, 3 );
						sAuthorNo.Replace( _T(" "), NULL );
						if ( sAuthorMid.Left(1) == _T(",") ) {
							sAuthorNo = _T(",") + sAuthorNo;						
						}
						break;
					}				
				}
			}
			
			if ( sAuthorNo.IsEmpty())
			{
				AfxMessageBox( _T("���ڱ�ȣ�� ã�� �� �����ϴ�. ���ڱ�ȣ���� ������ Ȯ���ϼ���.") );
			}
			
			strItemData = sData + sAuthorNo;
		}


		return strItemData;
}

BOOL CCatCodeGetterDlg::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		OnbFIND();
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CCatCodeGetterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
