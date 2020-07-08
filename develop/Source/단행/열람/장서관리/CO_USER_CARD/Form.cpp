// Form.cpp: implementation of the CForm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Form.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CForm::CForm()
{

}

CForm::~CForm()
{

}




INT CForm::InsertValue(  CESL_DataMgr* pCardDM , INT nIndex  )
{
	const INT nCnt = 6;
	CString sAlias[nCnt] = 
	{
		_T("������KEY") , _T("��������ȣ") , _T("����̸�") , _T("�ո鿩��") , _T("WIDTH") , 
		_T("HEIGHT") 
	};
	
	CString sResult[nCnt];
	
	pCardDM->GetCellData( sAlias , nCnt , sResult , 0 );
	
	sFormKey = sResult[0];
	sLibCode = sResult[1];
	sFormName = sResult[2];
	nPageNum = _ttoi(sResult[3]);
	nWidth = _ttoi( sResult[4] );
	nHeight = _ttoi( sResult[5] );
	
	
	return 0;
}
