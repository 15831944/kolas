// ControlObject.cpp: implementation of the CControlObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ControlObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CControlObject::CControlObject()
{

}

CControlObject::~CControlObject()
{

}


INT CControlObject::InsertValue(  CESL_DataMgr* pCardDM , INT nIndex , INT nMode )
{
	const INT nCnt = 9;
	CString sAlias[nCnt] = 
	{
		_T("�˸��ƽ���") , _T("Ÿ��") , _T("ZINDEX") , _T("������ġ_X") , _T("������ġ_Y") , 
		_T("����") , _T("����") , _T("��ũ����") , _T("��������")
	};
	
	CString sResult[nCnt];
	
	pCardDM->GetCellData( sAlias , nCnt , sResult , nIndex );
	
	
	sObjectAlias = sResult[0];
	sType = sResult[1];
	nZIndex = _ttoi(sResult[2]);
	rLocation = CRect( _ttoi(sResult[3]) , _ttoi(sResult[4]) , _ttoi(sResult[3])+_ttoi(sResult[5]) , _ttoi(sResult[4])+_ttoi(sResult[6]) );

	rRealLocation = rLocation;
	
	rLocation = ConvertRectMMToPixel( nMode , rLocation );


	if( sResult[7].Compare(_T("Y")) == 0 )
		bLink = TRUE;
	else
		bLink = FALSE;
	
	if( sResult[8].Compare(_T("Y")) == 0 )
		bModify = TRUE;
	else
		bModify = FALSE;

	m_nMode = nMode;
	
	return 0;
}

INT CControlObject::DrawDC( CDC* pDC )
{
	/// �̸������� ��� �ڽ� �׷��ֱ�
	if( m_nMode == CUC_MANAGER_MODE_PREVIEW  )
	{
		pDC->SetROP2(R2_COPYPEN);
		CPen   pen;
		pen.CreatePen( PS_DOT, 1 , RGB( 255, 0 , 0 ) );
		CPen*   pOldPen   = (CPen*)pDC->SelectObject(&pen);
		
		pDC->MoveTo(rLocation.left, rLocation.bottom);
		pDC->LineTo(rLocation.left, rLocation.top);
		pDC->MoveTo(rLocation.left, rLocation.top);
		pDC->LineTo(rLocation.right, rLocation.top);
		pDC->MoveTo(rLocation.right, rLocation.top);
		pDC->LineTo(rLocation.right, rLocation.bottom);
		pDC->MoveTo(rLocation.right, rLocation.bottom);
		pDC->LineTo(rLocation.left, rLocation.bottom);
		
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
	}

	return 0;
}

CRect CControlObject::ConvertRectMMToPixel(INT mode, CRect rect)
{
	CRect result;
	result.left = DrawGetMMToPixel(mode, (double)rect.left);
	result.top = DrawGetMMToPixel(mode, (double)rect.top);
	result.right = DrawGetMMToPixel(mode, (double)rect.right);
	result.bottom = DrawGetMMToPixel(mode, (double)rect.bottom);
	return result;
}



INT CControlObject::DrawGetMMToPixel(INT mode, double mm)
{
	
	if (mode == CUC_MANAGER_MODE_PRINT) {
		mm = 297.0 * mm / 980.0;
		return ((INT)((double)(1440*0.03937) * mm));	
	} else {
		return (INT)mm;
	}		
	
}
