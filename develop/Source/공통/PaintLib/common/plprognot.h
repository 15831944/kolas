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
|      $Id: plprognot.h,v 1.1 2009/12/09 05:33:56 ������ Exp $
|
|      Copyright (c) 1996-2002 Ulrich von Zadow
|
\--------------------------------------------------------------------
*/

#ifndef INCL_PLPROGNOT
#define INCL_PLPROGNOT

//! Defines an interface for progress notifications.
class PLIProgressNotification
{
public:
  //! Called during decoding as progress gets made.
  virtual void OnProgress 
    ( double Part
    ) = 0;
};

#endif
/*
/--------------------------------------------------------------------
|
|      $Log: plprognot.h,v $
|      Revision 1.1  2009/12/09 05:33:56  ������
|      *** empty log message ***
|
|      Revision 1.1  2008/06/27 14:30:45  �ڴ��
|      *** empty log message ***
|
|      Revision 1.1  2008/02/29 11:02:05  PDJ
|      *** empty log message ***
|
|      Revision 1.1  2007/10/19 02:09:58  �ڿ��
|      *** empty log message ***
|
|      Revision 1.1  2007/04/06 09:31:06  Ȳ����
|      *** empty log message ***
|
|      Revision 1.1  2006/11/29 03:34:26  Ȳ����
|      ****2006.11.29*****
|
|      Revision 1.1  2006/05/23 05:48:28  Ȳ����
|      2006.05.23 �ʱ�ȭ
|
|      Revision 1.1  2006/05/18 01:30:33  Ȳ����
|      2006.05.17
|
|      Revision 1.2  2002/03/31 13:36:42  uzadow
|      Updated copyright.
|
|      Revision 1.1  2001/09/16 19:03:22  uzadow
|      Added global name prefix PL, changed most filenames.
|
|
\--------------------------------------------------------------------
*/
