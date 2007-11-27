// RichEditLineParserSafe.cpp: implementation of the CRichEditLineParserSafe class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "RichEditLineParserSafe.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//
// throws CRichEditLineParser::CNotWindowException
//
CRichEditLineParserSafe::CRichEditLineParserSafe(CRichViewOut::CThreadParam* p)
: CRichEditLineParser(p->m_hwndRichEdit)
{
	// Note: This will work as long as this object goes out of scope
	// before p->m_hAbortEvent does
	m_AbortEvent = p->m_hAbortEvent;
}

CRichEditLineParserSafe::~CRichEditLineParserSafe()
{

}

//
// throws CRichEditLineParserSafe::CSignaledException
//
bool CRichEditLineParserSafe::GetNextLine(CString &rStr)
{
	// Don't allow access to m_hwndRichEdit if abort event is signaled
	DWORD dw = ::WaitForSingleObject(m_AbortEvent, 0);
	ASSERT( dw != WAIT_FAILED );

	if (dw == WAIT_OBJECT_0)	//	Abort signaled
	{
		rStr.Empty();
		throw new CSignaledException();
		return false;			// never called
	}
	return CRichEditLineParser::GetNextLine(rStr);
}

//
// throws CRichEditLineParserSafe::CSignaledException
//
void CRichEditLineParserSafe::FillBuffer()
{
	// Don't allow access to m_hwndRichEdit if abort event is signaled
	DWORD dw = ::WaitForSingleObject(m_AbortEvent, 0);
	ASSERT( dw != WAIT_FAILED );

	if (dw == WAIT_OBJECT_0)
	{
		throw new CSignaledException();
	}

	CRichEditLineParser::FillBuffer();
}

//////////////////////////////////////////////////////////////////////
// CSignaledException Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRichEditLineParserSafe::CSignaledException::CSignaledException()
{
	// thrown when m_AbortEvent is signaled
}

CRichEditLineParserSafe::CSignaledException::~CSignaledException()
{
}
