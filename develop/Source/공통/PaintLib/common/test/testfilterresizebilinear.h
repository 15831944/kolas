/*The zlib/libpng License

Copyright (c) <year> <copyright holders>

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.*/
/*
/--------------------------------------------------------------------
|
|      $Id: testfilterresizebilinear.h,v 1.1 2009/12/09 05:34:02 ������ Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLTESTFILTERRESIZEBILINEAR
#define INCL_PLTESTFILTERRESIZEBILINEAR

#include "pltest.h"

class PLBmp;

class PLTestFilterResizeBilinear: public PLTest
{

public:
  PLTestFilterResizeBilinear ();

  virtual ~PLTestFilterResizeBilinear();

  virtual void RunTests ();

private:
  void createBmp(PLBmp& Bmp);
};

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: testfilterresizebilinear.h,v $
|      Revision 1.1  2009/12/09 05:34:02  ������
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:48  �ڴ��
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:11  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:03  �ڿ��
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:08  Ȳ����
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:29  Ȳ����
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:45  Ȳ����
|      2006.05.23 �ʱ�ȭ
|
|      Revision 1.1  2006/05/18 01:30:34  Ȳ����
|      2006.05.17
|
|      Revision 1.5  2002/08/04 20:08:01  uzadow
|      Added PLBmpInfo class, ability to extract metainformation from images without loading the whole image and proper greyscale support.
|
|      Revision 1.4  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.3  2001/10/21 17:08:41  uzadow
|      no message
|
|      Revision 1.2  2001/10/06 20:44:45  uzadow
|      Linux compatibility
|
|      Revision 1.1  2001/10/03 14:00:29  uzadow
|      Much improved quality in FilterResizeBilinear.
|
|
\--------------------------------------------------------------------
*/
