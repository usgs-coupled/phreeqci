// KeywordLoader.cpp: implementation of the CKeywordLoader class.
//
// $Date: 2004/10/23 01:35:36 $
// $Revision: 1.2 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "KeywordLoader.h"

extern "C"
{
#define EXTERNAL extern
#include "phreeqc/src/global.h"
	int clean_up();
// COMMENT: {10/20/2004 4:21:04 PM}	void load_keyword(GUICALLBACK gui);
}


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKeywordLoader::CKeywordLoader(CLogicalLineParser& rLogicalLineParser)
: m_rLogicalLineParser(rLogicalLineParser), m_pPhreeqcCallback(NULL)
{
	ASSERT(FALSE); // DEPRECATED

// COMMENT: {10/20/2004 4:20:52 PM}	ASSERT(sizeof(DWORD) >= sizeof(this));
// COMMENT: {10/20/2004 4:20:52 PM}	GUICALLBACK gui = {(DWORD)this, LineCallBack, NULL/*ErrorCallBack*/, NULL/*WarnCallBack*/};
// COMMENT: {10/20/2004 4:20:52 PM}
// COMMENT: {10/20/2004 4:20:52 PM}	// Need to create on the stack so that clean_up is called before the files
// COMMENT: {10/20/2004 4:20:52 PM}	// are closed in CPhreeqcCallback::~CPhreeqcCallback
// COMMENT: {10/20/2004 4:20:52 PM}	m_pPhreeqcCallback = new CPhreeqcCallback(gui);
// COMMENT: {10/20/2004 4:20:52 PM}	m_pPhreeqcCallback->Go(load_keyword);
}

CKeywordLoader::~CKeywordLoader()
{
	clean_up();
	ASSERT(m_pPhreeqcCallback != NULL);
	delete m_pPhreeqcCallback;
}

BOOL CALLBACK CKeywordLoader::LineCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	static CString strLine;

	ASSERT(dwCookie);
	CKeywordLoader* pCookie = (CKeywordLoader*) dwCookie;

	// strLine.IsEmpty() == FALSE signifies that the last call returned FALSE
	// and the strLine has not been copied yet
	if (!strLine.IsEmpty() || pCookie->m_rLogicalLineParser.GetNextLine(strLine))
	{
		*pcb = strLine.GetLength() + 1;			// GetLength does not include NULL terminator
		if (*pcb <= cb)
		{
			strncpy((LPSTR)pbBuff, (LPCSTR)strLine, *pcb);
			strLine = &afxChNil;    // empty string without deallocating
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		strLine.Empty();			// empty string and deallocate
		*pcb = 0;	// EOF
	}
	return TRUE;
}
