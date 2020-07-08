// ReadCfgFile.cpp: implementation of the CReadCfgFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AccessControl.h"
#include "ReadCfgFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReadCfgFile::CReadCfgFile()
{
	m_sUserID = _T("");
	m_sSequence = _T("");
	m_sConnection = _T("");
}

CReadCfgFile::~CReadCfgFile()
{

}

INT CReadCfgFile::ReadCfgFile(CString path)
{
	if (path.GetLength() == 0) {
#ifdef _UNICODE
		path = _T("..\\cfg\\RefEditor\\refeditor_uni.cfg");
#else
		path = _T("..\\cfg\\RefEditor\\refeditor_mbcs.cfg");
#endif
	}

	CFile file;
	if ( file.Open(path, CFile::modeRead | CFile::typeBinary) == NULL ) {
		path += _T(" ������ �������� �ʽ��ϴ�.");
		AfxMessageBox(path, MB_ICONSTOP);
		return -1;
	}

	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	_TCHAR szCfg[1000];

	INT nRead = file.Read(szCfg, 1000);

#ifdef _UNICODE
	nRead /= 2;
#endif 

	szCfg[nRead] = NULL;
	file.Close();

    if ( _tcslen(szCfg) == 0 ) {
        file.Close();
        return -1;
    }

	_TCHAR name[64];
	_TCHAR value[256];
	INT pos;

	const _TCHAR seps[] = _T("\n");
	_TCHAR* token;
	token = _tcstok(szCfg, seps);

	if ( token[0] == NULL ) return 0;

	while ( token != NULL ) 
	{
		name[0] = NULL;
		value[0] = NULL;
		pos = 0;

		if ( token[0] == _T('.') ) {
			token = _tcstok(NULL, seps);
			if ( token == NULL ) break;
			continue;

		} else if ( token[0] == _T('\r') ) {
			token = _tcstok(NULL, seps);
			if ( token == NULL ) break;
			continue;
		}

		pos = GetToken(token, pos, name);
		pos = GetToken(token, pos, value);

		if ( _tcscmp(name, _T("ID")) == 0 ) {
			m_sUserID = value;
		} else if ( _tcscmp(name, _T("HOST")) == 0 ) {
			m_sConnection = value;
		} else if ( _tcscmp(name, _T("SEQ")) == 0 ) {
			m_sSequence = value;
		} else if ( _tcscmp(name, _T("DB")) == 0 ) {
			m_sDBKIND = value;
		}

		token = _tcstok(NULL, seps);
	}
	return 0;
}

INT CReadCfgFile::GetToken(_TCHAR* line, INT pos, _TCHAR* token)
{
	if ( pos < 0 ) {
		token[0] = NULL;
		return -1;
	} else if ( line[pos] == _T('\n') ) {
		token[0] = NULL;
		return -1;
	}

	INT end;

	// ó���� ����� tab�� �����Ѵ�
	while ( true ) {
		if ( line[pos] == _T(' ') ) {

		} else if ( line[pos] == _T('\t') ) {

		} else if ( line[pos] == _T('\r') ) {
			token[0] = NULL;
			return -1;

		} else if ( line[pos] == NULL ) {
			token[0] = NULL;
			return -1;
		
		} else {
			break;
		}
		pos++;
	}

	// token�� ���� ã�´�
    bool bBrace;
    if ( line[pos] == _T('"') ) {
        bBrace = true;
	    pos++;
    } else {
        bBrace = false;
    }
    end = pos;


	while ( true ) {
        if ( bBrace ) {
            if ( line[end] == _T('"') ) {
                // Brace�϶��� ' ', '\t'��� _T("�� ���������̴�
			    _tcsncpy(token, line+pos, end-pos);
			    token[end-pos] = NULL;
			    break;

		    } else if ( line[end] == _T('\r') ) {
			    _tcsncpy(token, line+pos, end-pos);
			    token[end-pos] = NULL;
			    break;

		    } else if ( line[end] == NULL ) {
			    _tcsncpy(token, line+pos, end-pos);
			    token[end-pos] = NULL;
			    break;
		    }

        } else {
		    if ( line[end] == _T(' ') ) {
			    _tcsncpy(token, line+pos, end-pos);
			    token[end-pos] = NULL;
			    break;

		    } else if ( line[end] == _T('\t') ) {
			    _tcsncpy(token, line+pos, end-pos);
			    token[end-pos] = NULL;
			    break;

		    } else if ( line[end] == _T('\r') ) {
			    _tcsncpy(token, line+pos, end-pos);
			    token[end-pos] = NULL;
			    break;

		    } else if ( line[end] == NULL ) {
			    _tcsncpy(token, line+pos, end-pos);
			    token[end-pos] = NULL;
			    break;
		    }
        }
		end++;
	}

	return end+1;
}

