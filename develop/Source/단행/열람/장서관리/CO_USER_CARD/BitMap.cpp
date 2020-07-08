// BitMap.cpp: implementation of the CBitMap class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BitMap.h"
#include "CharConvert.h"
#include "..\..\..\����\paintlib\common\Filter\plfilterresizebilinear.h"
#include "..\..\..\����\paintlib\common\Filter\PLFilterCrop.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBitMap::CBitMap()
{
	m_pBmp = NULL;
	m_pBmp = new PLWinBmp;
}

CBitMap::~CBitMap()
{
	delete m_pBmp;
}

INT CBitMap::InsertValue(  CESL_DataMgr* pCardDM , INT nIndex , INT nMode )
{
	const INT nCnt = 1;
	CString sAlias[nCnt] = 
	{
		_T("���ϰ��") 
	};
	
	CString sResult[nCnt];
	
	pCardDM->GetCellData( sAlias , nCnt , sResult , nIndex );
	
	sFilePath = sResult[0];

	CControlObject::InsertValue( pCardDM , nIndex  , nMode );
	
	
	return 0;
}

INT CBitMap::DrawDC( CDC* pDC )
{
	if( sFilePath.IsEmpty() ) return 0;

	INT ids;

	CFile fFile;
	ids = fFile.Open( sFilePath , CFile::modeRead | CFile::typeBinary );
	if( ids == 0 )
	{
		return 0;
	}

	DWORD nFLength = fFile.GetLength();

	
	fFile.Close();

	CHAR		*szFileName;	
	CharConvert cvt;
	INT			nLen;

	cvt.UnicodeToMultiByte(sFilePath.GetBuffer(0), &szFileName, &nLen );


	/// bmp ����
	//����m_Decoder.MakeBmpFromFile ((LPSTR)(LPCTSTR)sFilePath, m_pBmp);
	m_Decoder.MakeBmpFromFile (szFileName, m_pBmp);

	if( m_nMode == CUC_MANAGER_MODE_PREVIEW   )
	{
/*
		/// ���� ������ ��������
		CSize Size = m_pBmp->GetSize();

		/// TemDC�� ���� ����Ʈ�� ���� ������� ���
		CDC TmpDC;
		TmpDC.CreateCompatibleDC(pDC);
		
		TmpDC.SetBkMode(OPAQUE);
		TmpDC.SetBkColor(RGB(255,255,255) );

		/// ���� ������
		CRect rTotalLocation( 0 , 0 , Size.cx , Size.cy );

		/// ���� ������ ��� DC�� �°� ��ǥ ����
		CBitmap bmpBuffer;
		bmpBuffer.CreateCompatibleBitmap(pDC,rTotalLocation.Width(),rTotalLocation.Height() );
		CBitmap* pOldBitmap1=(CBitmap*)TmpDC.SelectObject(&bmpBuffer);

		/// Temp DC�� �ѷ��ֱ�
		rTotalLocation = ConvertRectMMToPixel( m_nMode , rTotalLocation );

		m_pBmp->StretchDraw( TmpDC.m_hDC , 0 , 0 , rTotalLocation.Width() , rTotalLocation.Height() );

		pDC->BitBlt( rLocation.left , rLocation.top , rLocation.Width() , rLocation.Height() , &TmpDC , 0 , 0 , SRCCOPY );

		TmpDC.SelectObject(pOldBitmap1);
*/
	
/*		
		m_pBmp->StretchDraw( pDC->m_hDC , rLocation.left , rLocation.top , rLocation.Width() , rLocation.Height() );
*/

/*		
		/// ���� ������ ��������
		CSize Size = m_pBmp->GetSize();
		
		HGLOBAL m_hDIB;
		m_hDIB = ::GlobalAlloc(GMEM_FIXED, nFLength );
		/// ���� �ѷ��ִ� DC�� �߶� �ѷ��ֱ�		
		BITMAPINFO &bmInfo = *(LPBITMAPINFO)m_hDIB ;
		
		CopyBitMapInfo( m_pBmp->GetBMI() , &(bmInfo.bmiHeader) );
		
		for( INT i=0;i<256;i++)
		{
			bmInfo.bmiColors[i].rgbRed=(BYTE)i;
			bmInfo.bmiColors[i].rgbGreen=(BYTE)i;
			bmInfo.bmiColors[i].rgbBlue=(BYTE)i;
		}
		
		double dRate;
		INT nRealX , nRealY;
		if(  double(rLocation.Width())/double(Size.cx) > double(rLocation.Height())/double(Size.cy) )
		{
			dRate = double(rLocation.Width())/double(Size.cx);
			nRealX = Size.cx;
			nRealY = rLocation.Height()/dRate;
			
		}
		else
		{
			dRate = double(rLocation.Height())/double(Size.cy);
			nRealY = Size.cy;
			nRealX = rLocation.Width()/dRate;
		}		
		
		::StretchDIBits( pDC->m_hDC , rLocation.left , rLocation.top  , rLocation.Width() , rLocation.Height(), 
			0 , 0 , nRealX, nRealY, m_pBmp->GetBits() , (LPBITMAPINFO)m_hDIB  , DIB_RGB_COLORS , SRCCOPY );

		
		::GlobalFree(m_hDIB);

*/

		/// ���� ������ ��������
		CSize Size = m_pBmp->GetSize();
		
		double dRate;
		INT nRealX , nRealY;
		if(  double(rLocation.Width())/double(Size.cx) > double(rLocation.Height())/double(Size.cy) )
		{
			dRate = double(rLocation.Width())/double(Size.cx);
			nRealX = Size.cx;
			nRealY = rLocation.Height()/dRate;
			
		}
		else
		{
			dRate = double(rLocation.Height())/double(Size.cy);
			nRealY = Size.cy;
			nRealX = rLocation.Width()/dRate;
		}		
		
		PLFilterCrop CropFilter( 0 , 0 , nRealX , nRealY );
		CropFilter.ApplyInPlace (m_pBmp);   
		
		PLFilterResizeBilinear ResizeFilter ( rLocation.Width() , rLocation.Height() );
		ResizeFilter.ApplyInPlace (m_pBmp);   
		
		
		m_pBmp->Draw( pDC->m_hDC  , rLocation.left , rLocation.top );
			
	}
	else
	{

/*
		/// ���� ������ ��������
		CSize Size = m_pBmp->GetSize();
		
		HGLOBAL m_hDIB;
		m_hDIB = ::GlobalAlloc(GMEM_FIXED, nFLength );
		/// ���� �ѷ��ִ� DC�� �߶� �ѷ��ֱ�		
		BITMAPINFO &bmInfo = *(LPBITMAPINFO)m_hDIB ;
	
		CopyBitMapInfo( m_pBmp->GetBMI() , &(bmInfo.bmiHeader) );
		
		for( INT i=0;i<256;i++)
		{
			bmInfo.bmiColors[i].rgbRed=(BYTE)i;
			bmInfo.bmiColors[i].rgbGreen=(BYTE)i;
			bmInfo.bmiColors[i].rgbBlue=(BYTE)i;
		}
		

		::StretchDIBits( pDC->m_hDC , rLocation.left , rLocation.top  , rLocation.Width() , rLocation.Height(), 
			0 , 0 , rRealLocation.Width(), rRealLocation.Height(), m_pBmp->GetBits() , (LPBITMAPINFO)m_hDIB  , DIB_RGB_COLORS , SRCCOPY );

		::GlobalFree(m_hDIB);
*/

		/// ���� ������ ��������
		CSize Size = m_pBmp->GetSize();
		
		double dRate;
		INT nRealX , nRealY;
		if(  double(rLocation.Width())/double(Size.cx) > double(rLocation.Height())/double(Size.cy) )
		{
			dRate = double(rLocation.Width())/double(Size.cx);
			nRealX = Size.cx;
			nRealY = rLocation.Height()/dRate;
			
		}
		else
		{
			dRate = double(rLocation.Height())/double(Size.cy);
			nRealY = Size.cy;
			nRealX = rLocation.Width()/dRate;
		}		
		
		PLFilterCrop CropFilter( 0 , 0 , nRealX , nRealY );
		CropFilter.ApplyInPlace (m_pBmp);   

		HGLOBAL m_hDIB;
		m_hDIB = ::GlobalAlloc(GMEM_FIXED, nFLength );
		/// ���� �ѷ��ִ� DC�� �߶� �ѷ��ֱ�		
		BITMAPINFO &bmInfo = *(LPBITMAPINFO)m_hDIB ;
		
		CopyBitMapInfo( m_pBmp->GetBMI() , &(bmInfo.bmiHeader) );
		
		for( INT i=0;i<256;i++)
		{
			bmInfo.bmiColors[i].rgbRed=(BYTE)i;
			bmInfo.bmiColors[i].rgbGreen=(BYTE)i;
			bmInfo.bmiColors[i].rgbBlue=(BYTE)i;
		}
		
				
		::StretchDIBits( pDC->m_hDC , rLocation.left , rLocation.top  , rLocation.Width() , rLocation.Height(), 
			0 , 0 , nRealX, nRealY, m_pBmp->GetBits() , (LPBITMAPINFO)m_hDIB  , DIB_RGB_COLORS , SRCCOPY );
		
		::GlobalFree(m_hDIB);

		
		
	}
/*
	/// ���� ������ ��������
	CSize Size = m_pBmp->GetSize();

	double dRate;
	INT nRealX , nRealY;
	if(  double(rLocation.Width())/double(Size.cx) > double(rLocation.Height())/double(Size.cy) )
	{
		dRate = double(rLocation.Width())/double(Size.cx);
		nRealX = Size.cx;
		nRealY = rLocation.Height()/dRate;
		
	}
	else
	{
		dRate = double(rLocation.Height())/double(Size.cy);
		nRealY = Size.cy;
		nRealX = rLocation.Width()/dRate;
	}		

	PLFilterCrop CropFilter( 0 , 0 , nRealX , nRealY );
	CropFilter.ApplyInPlace (m_pBmp);   

	PLFilterResizeBilinear ResizeFilter ( rLocation.Width() , rLocation.Height() );
	ResizeFilter.ApplyInPlace (m_pBmp);   
	
	
	m_pBmp->Draw( pDC->m_hDC  , rLocation.left , rLocation.top );
*/	
	


//	pDC->BitBlt( rLocation.left , rLocation.top  , rLocation.Width() , rLocation.Height() , &TmpDC , 0 , 0 ,	m_nSRCCOPY );
	

	CControlObject::DrawDC(pDC);
	
	return 0;
}



INT CBitMap::CopyBitMapInfo( BITMAPINFOHEADER* pSrcBitInfo , BITMAPINFOHEADER* pDstBitInfo )
{
	pDstBitInfo->biSize =              pSrcBitInfo->biSize          ;
	pDstBitInfo->biWidth =             pSrcBitInfo->biWidth         ;
	pDstBitInfo->biHeight=             pSrcBitInfo->biHeight        ;
	pDstBitInfo->biPlanes=             pSrcBitInfo->biPlanes        ;
	pDstBitInfo->biBitCount=           pSrcBitInfo->biBitCount      ;
	pDstBitInfo->biCompression=        pSrcBitInfo->biCompression   ;
	pDstBitInfo->biSizeImage=          pSrcBitInfo->biSizeImage     ;
	pDstBitInfo->biXPelsPerMeter=      pSrcBitInfo->biXPelsPerMeter ;
	pDstBitInfo->biYPelsPerMeter=      pSrcBitInfo->biYPelsPerMeter ;
	pDstBitInfo->biClrUsed=            pSrcBitInfo->biClrUsed       ;
	pDstBitInfo->biClrImportant=       pSrcBitInfo->biClrImportant  ; 
	

	return 0;

}