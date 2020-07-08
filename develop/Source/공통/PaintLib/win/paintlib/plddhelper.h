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
|      $Id: plddhelper.h,v 1.1 2009/12/09 05:34:46 ������ Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

char * GetDDErrorText
    (HRESULT hr
    );

void CheckForDDError 
    ( HRESULT hr
    );

/*
/--------------------------------------------------------------------
|
|      $Log: plddhelper.h,v $
|      Revision 1.1  2009/12/09 05:34:46  ������
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:31:06  �ڴ��
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:43  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:10:23  �ڿ��
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:28  Ȳ����
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:45  Ȳ����
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:58  Ȳ����
|      2006.05.23 �ʱ�ȭ
|
|      Revision 1.1  2006/05/18 01:31:08  Ȳ����
|      2006.05.17
|
|      Revision 1.2  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.1  2001/09/16 19:03:23  uzadow
|      Added global name prefix PL, changed most filenames.
|
|      Revision 1.1  2000/08/13 12:11:43  Administrator
|      Added experimental DirectDraw-Support
|
|
\--------------------------------------------------------------------
*/
