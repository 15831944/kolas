// TextBox.cpp: implementation of the CTextBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TextBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTextBox::CTextBox()
{

}

CTextBox::~CTextBox()
{

}




INT CTextBox::InsertValue(  CESL_DataMgr* pCardDM , INT nIndex , INT nMode  )
{
	const INT nCnt = 17;
	CString sAlias[nCnt] = 
	{
		_T("���򼱺����ֱ�") , _T("�����������ֱ�") , _T("����") , _T("���θ���") , _T("���θ���") , 
		_T("��׶����÷�") , _T("����") , _T("�۲�") , _T("�۲ý�Ÿ��") , _T("ũ��") , 
		_T("��Ҽ�") , _T("����") , _T("�۲û�") , _T("���̴¹��ڼ�") , _T("��ü����") , 
		_T("���̴¹��ڽ�����ġ") , _T("�ڰ�")
	};
	
	CString sResult[nCnt];
	
	pCardDM->GetCellData( sAlias , nCnt , sResult , nIndex );
	
	if( sResult[0].Compare(_T("Y")) == 0 )
		bShowWLine = TRUE;
	else
		bShowWLine = FALSE;
	
	if( sResult[1].Compare(_T("Y")) == 0 )
		bShowVLine = TRUE;
	else
		bShowVLine = FALSE;
	
	
	nSpace = _ttoi(sResult[2]);

	nWAlign = _ttoi(sResult[3]);

	if( nWAlign == 0 )
		nFormat = TA_LEFT;
	else if( nWAlign == 1 )
		nFormat = TA_CENTER;
	else if( nWAlign = 2 )
		nFormat = TA_RIGHT;

	nVAlign = _ttoi(sResult[4]);

	if( nVAlign == 0 )
		nFormat = nFormat | TA_TOP;
	else if( nVAlign == 1 )
		nFormat = nFormat | TA_BASELINE;
	else if( nVAlign = 2 )
		nFormat = nFormat | TA_BOTTOM;
	
	

	dBackGroundColor = _ttoi(sResult[5]);

	sText = sResult[6];

	/// ũ��� �۰� ����
	CFont tmpfont;
	tmpfont.CreatePointFont( (_ttoi(sResult[9])-2)*10 , sResult[7] );
	
	LOGFONT lf;
	tmpfont.GetLogFont(&lf);

	// �۲� ��Ÿ��
	// 0 :���� , 1:����Ӳ� , 2:���� , 3:���� ����Ӳ�
	
	if( _ttoi(sResult[8]) == 0 )
	{
		lf.lfItalic = FALSE;
		lf.lfWeight = FW_NORMAL;
	}
	else if( _ttoi(sResult[8]) == 1 )
	{
		lf.lfItalic = TRUE;
		lf.lfWeight = FW_NORMAL;
	}
	else if( _ttoi(sResult[8]) == 2 )
	{
		lf.lfItalic = FALSE;
		lf.lfWeight = FW_BOLD;
	}
	else if( _ttoi(sResult[8]) == 3 )
	{
		lf.lfItalic = TRUE;
		lf.lfWeight = FW_BOLD;		
	}

	if( sResult[10].Compare(_T("Y")) == 0 )
		lf.lfStrikeOut = TRUE;
	else
		lf.lfStrikeOut = FALSE;
	
	if( sResult[11].Compare(_T("Y")) == 0 )
		lf.lfUnderline = TRUE;
	else
		lf.lfUnderline = FALSE;

	if (nMode == CUC_MANAGER_MODE_PRINT) {		// �μ����϶� ������ ���
		lf.lfHeight = -MulDiv(_ttoi(sResult[9]), 1440, 72);									
	}
	
	fFontInfo.CreateFontIndirect(&lf);

	dFontColor =  _ttoi(sResult[12]);

	nShowTextCnt = _ttoi(sResult[13]);

	sReplaceChar = sResult[14];

	nShowTextStartPos = _ttoi(sResult[15]);

	if( nShowTextStartPos <= 0 )
		nShowTextStartPos = 0;
	else
		nShowTextStartPos--;

	nSpaceChar = _ttoi(sResult[16]);

	CRect rTmpRect( 0 , 0 , nSpaceChar , 0 );
	ConvertRectMMToPixel( nMode , rTmpRect );
	if( nMode == CUC_MANAGER_MODE_PRINT )
		nSpaceChar = rTmpRect.Width()*10;

	CControlObject::InsertValue( pCardDM , nIndex , nMode );
	
	return 0;
}

INT CTextBox::DrawDC( CDC* pDC )
{
	
	CFont *pOldFont = NULL;

	
	/// ����

	/// ���� �׸���
	/// ������ �׸���
	
	
	/// TextBox �׸���~~
	CString sReplaceText;
	INT nDeleteCnt = 0;
	if( nShowTextCnt > 0 )
	{
		if( sText.GetLength() < nShowTextStartPos )
			nShowTextStartPos = sText.GetLength();

		for( INT i = 0 ; i < sText.GetLength() ; i++ )
		{
			if( i >= nShowTextStartPos && i <= nShowTextStartPos+nShowTextCnt-1 )
				sReplaceText += CString(sText.GetAt(i));
			else
				if( !sReplaceChar.IsEmpty() )
					sReplaceText += sReplaceChar;
		}
	}

	/// ��Ʈ��
//	pDC->SetTextColor( COLORREF(dFontColor));

	/// ��Ʈ ���� 
	pOldFont = pDC->SelectObject(&fFontInfo);

	if( nShowTextCnt == 0 ) sReplaceText = sText;

	INT nCharCnt;
	if( nSpaceChar != 0 )
	{
		CSize siTextSize;
		CRect rTmpLocation = rLocation;
		for( INT i = 0 ; i < sReplaceText.GetLength() ; i++ )
		{
			nCharCnt = 1;
			if( sReplaceText.GetAt(i) < 0 )
				nCharCnt = 2;

			siTextSize = pDC->GetTextExtent(sReplaceText.Mid(i,nCharCnt) );
			rTmpLocation.right = rTmpLocation.left + siTextSize.cx ;
			
			pDC->DrawText( sReplaceText.Mid(i,nCharCnt) , rTmpLocation , nFormat );		
			
			rTmpLocation.left += siTextSize.cx + nSpaceChar ;			

			if( nCharCnt == 2 ) i++;
		}
	}
	else
		pDC->DrawText( sReplaceText , rLocation , nFormat );		


	pDC->SelectObject(pOldFont);

	CControlObject::DrawDC(pDC);

	return 0;
}


