// SortElement.cpp: implementation of the CSortElement class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SortElement.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSortElement::CSortElement()
{
	// KOL.UDF.022 ��ť���ڵ� ����
	m_nPriority = 0;
	m_nSortDirection = 0;

}

CSortElement::~CSortElement()
{

}
