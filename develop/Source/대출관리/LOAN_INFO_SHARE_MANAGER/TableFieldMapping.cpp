// TableFieldMapping.cpp : implementation file
//

#include "stdafx.h"
#include "TableFieldMapping.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTableFieldMapping dialog


CTableFieldMapping::CTableFieldMapping(CESL_Mgr* pParent /*=NULL*/, CString sMappingType )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CTableFieldMapping)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_sMappingType = sMappingType;
	m_pMappingDM = NULL;

	InitMappingTable();
}

CTableFieldMapping::~CTableFieldMapping()
{
	delete m_pMappingDM;
}

VOID CTableFieldMapping::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTableFieldMapping)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CTableFieldMapping, CDialog)
	//{{AFX_MSG_MAP(CTableFieldMapping)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTableFieldMapping message handlers

INT CTableFieldMapping::InitMappingTable()
{
	EFS_BEGIN

	MakeMappingDM();
	
	MakeDMFromFile();

	return 0;


	EFS_END
	return -1;

}

INT CTableFieldMapping::MakeMappingDM()
{
	EFS_BEGIN

	/// ������ �޴��� ����
	m_pMappingDM = new CESL_DataMgr();
	
	// DB CONNECTION �ʱ�ȭ
	m_pMappingDM->SetCONNECTION_INFO(CLocCommonAPI::GetDBConnection());	
	
	// �÷� �����ϱ�
	m_pMappingDM->InitDataMgrRef( 3 );
	
	m_pMappingDM->SetDataMgrRef( 0 , _T("�ҽ��ʵ�") , _T("") , _T("") , _T("") , _T("") );
	m_pMappingDM->SetDataMgrRef( 1 , _T("�����ʵ�") , _T("") , _T("") , _T("") , _T("") );
	m_pMappingDM->SetDataMgrRef( 2 , _T("Ÿ��") , _T("") , _T("") , _T("") , _T("") );
	
	m_pMappingDM->FreeData();

	return 0;

	EFS_END
	return -1;

}

INT CTableFieldMapping::MakeDMFromFile()
{
	EFS_BEGIN

	INT ids;

	CString sFileName;
	sFileName = _MAPPING_FORDER _T("\\") + m_sMappingType + _T(".txt");

	CStdioFile fMappingFile;
	ids = fMappingFile.Open( sFileName , CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary);
	if( ids==0 )
	{
		CString sEMsg;
		sEMsg.Format( _T("[%s]���ϸ��� �߸��Ǿ����ϴ�.\r\n�ùٸ� ������� Ȯ���� �ֽʽÿ�.") , sFileName );
		AfxMessageBox( sEMsg );
		return 1;
	}
	TCHAR cUni;
	fMappingFile.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fMappingFile.SeekToBegin();
	}

	// DM�� �Է�
	CString sLine;
	CArray < CString , CString > asDataList;


	INT nInsertIndex;
	CString sData;
    while ( fMappingFile.ReadString( sLine ) ) 
	{
        sLine.TrimRight(); sLine.TrimLeft();
		if( sLine.IsEmpty() ) continue;
		if( sLine.GetAt(0) == '.' ) continue;
		
		ids = CLocCommonAPI::MakeArrayList( sLine , _T(">") , asDataList );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeStaticsMainInfoDM") );

		if( asDataList.GetSize() != 3 ) continue;

		m_pMappingDM->InsertRow(-1);

		nInsertIndex = m_pMappingDM->GetRowCount() - 1;

		sData = asDataList.GetAt(0); sData.TrimLeft(); sData.TrimRight();
		m_pMappingDM->SetCellData( _T("�ҽ��ʵ�")  , sData, nInsertIndex );
		sData = asDataList.GetAt(1); sData.TrimLeft(); sData.TrimRight();
		m_pMappingDM->SetCellData( _T("�����ʵ�")  , sData , nInsertIndex);		
		sData = asDataList.GetAt(2); sData.TrimLeft(); sData.TrimRight();
		m_pMappingDM->SetCellData( _T("Ÿ��")  , sData , nInsertIndex);		
		
	}
	
	fMappingFile.Close();

	return 0;


	EFS_END
	return -1;

}


CESL_DataMgr* CTableFieldMapping::GetMappingInfo()
{
	EFS_BEGIN

	return m_pMappingDM;

	EFS_END
	return NULL;

}
