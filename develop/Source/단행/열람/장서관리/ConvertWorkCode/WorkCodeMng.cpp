// WorkCodeMng.cpp: implementation of the CWorkCodeMng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WorkCodeMng.h"
#include <direct.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorkCodeMng::CWorkCodeMng()
{
	INT nIds = 0 ;

	nIds = initWorkCode ( ) ;
	if ( nIds < 0 ) return ;
}

CWorkCodeMng::~CWorkCodeMng()
{

}

/// ���Ϸκ��� �о�鿩 �۾����� �ڵ带 �迭�� �����Ѵ�.
INT CWorkCodeMng::initWorkCode()
{
	const CString FIELD_DELIMITER = _T(";") ;

	INT nIndex = 0 ;
	CString sFilePath = _T("..\\Cfg\\REDEFINE_WORKING_STATUS.txt") ;
	CString sLine = _T("") ;

EFS_BEGIN

	CStdioFile file ;
	if ( !file.Open ( sFilePath, CFile::modeRead | CFile::typeBinary ) ) return 1 ;
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
	
	while ( file.ReadString ( sLine ) )
	{
		if ( sLine.IsEmpty ( ) || sLine.Left ( 1 ) == _T( '.' ) ) 
			continue ;

		for ( INT i = 0 ; i < 3 ; i++ )
		{
			if ( i == 2 )
				m_saWorkingStatus [ i ].Add ( sLine ) ;
			else
			{
				nIndex = sLine.Find ( FIELD_DELIMITER, 0 ) ;
				m_saWorkingStatus [ i ].Add ( sLine.Mid ( 0, ( nIndex ) ) ) ;
				sLine.Delete ( 0, ( nIndex + 1 ) ) ;
			}
		}
	}

	return 0 ;

EFS_END
return -1;
}

/// K2UP �ڵ带 PDA �ڵ�� ����
INT CWorkCodeMng::convertPCToPDA(CString sPCCode, CString &sPDACode)
{
	INT nArraySize = 0 ; 

EFS_BEGIN

	nArraySize = m_saWorkingStatus [ PC_CODE_IDX ].GetSize ( ) ;
	if ( nArraySize < 1 ) return 1 ;

	for ( INT i = 0 ; i < nArraySize ; i++ )
	{
		if ( m_saWorkingStatus [ PC_CODE_IDX ].GetAt ( i ) == sPCCode )
			sPDACode = m_saWorkingStatus [ PDA_CODE_IDX ].GetAt ( i ) ;
	}

	return 0 ;

EFS_END
return -1;
}

/// PDA �ڵ带 K2UP �ڵ�� ����
INT CWorkCodeMng::convertPDAToPC(CString sPDACode, CString &sPCCode)
{
	INT nArraySize = 0 ; 

EFS_BEGIN

	nArraySize = m_saWorkingStatus [ PDA_CODE_IDX ].GetSize ( ) ;
	if ( nArraySize < 1 ) return 1 ;

	for ( INT i = 0 ; i < nArraySize ; i++ )
	{
		if ( m_saWorkingStatus [ PDA_CODE_IDX ].GetAt ( i ) == sPDACode )
			sPCCode = m_saWorkingStatus [ PC_CODE_IDX ].GetAt ( i ) ;
	}

	return 0 ;

EFS_END
return -1;
}

/// ���(nMode)�� ���� K2UP �ڵ�� PDA �ڵ带 �������� �ٲ۴�.
INT CWorkCodeMng::convertCodeToDesc(CString &sCode, INT nMode)
{
	INT nArraySize = 0 ; 

EFS_BEGIN

	nArraySize = m_saWorkingStatus [ nMode ].GetSize ( ) ;
	if ( nArraySize < 1 ) return 1 ;

	for ( INT i = 0 ; i < nArraySize ; i++ )
	{
		if ( m_saWorkingStatus [ nMode ].GetAt ( i ) == sCode )
			sCode = m_saWorkingStatus [ DESCRIPTION_IDX ].GetAt ( i ) ;
	}

	return 0 ;

EFS_END
return -1;
}
