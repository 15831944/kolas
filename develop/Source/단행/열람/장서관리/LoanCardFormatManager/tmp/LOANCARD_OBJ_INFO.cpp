// LOANCARD_OBJ_INFO.cpp: implementation of the LOANCARD_OBJ_INFO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LOANCARD_OBJ_INFO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LOANCARD_OBJ_INFO::LOANCARD_OBJ_INFO()
{
	// KOL.UDF.022
		REC_KEY = _T("");
		ALIAS = _T("");
		BOX_SIZE = _T("");
		TYPE = _T("");
		USE_YN = FALSE;
		FIX = FALSE;
		CONTENT = _T("");
		FILL_COLOR = _T("");
		STRING_LEN = 0;
		STRING_MAX = 0;
		HORZ_ALIGN = _T("");	// ����( ����:ALIGN_LEFT, �߾�:ALIGN_CENTER, ������:ALIGN_RIGHT )
		VERT_ALIGN = _T("");	// ����( ��:ALIGN_TOP, ���:ALIGN_VCENTER, �Ʒ�:ALIGN_BOTTOM )
		HORZ_LINE = FALSE;
		VERT_LINE = FALSE;
		FONT = _T("");
		FONT_SIZE = 0;
}

LOANCARD_OBJ_INFO::~LOANCARD_OBJ_INFO()
{

}
