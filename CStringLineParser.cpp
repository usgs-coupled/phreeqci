// CStringLineParser.cpp: implementation of the CCStringLineParser class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "CStringLineParser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CCStringLineParser, CLineParser)

CCStringLineParser::CCStringLineParser(const CString& rString)
: m_rString(rString), m_nLineNumber(0), m_nextChar(0)
{
	m_lastChar = m_rString.GetLength();
}

CCStringLineParser::~CCStringLineParser()
{
}

//
// returns:
//		false if end-of-string was reached without reading any data.
//		true  otherwise
//
bool CCStringLineParser::GetNextLine(CString& rStr)
{
	rStr = &afxChNil;    // empty string without deallocating


	if (m_nextChar >= m_lastChar)
	{
		return false;
	}

	int i;
	bool bEol = false;
	TCHAR c = 0;

	for (i = m_nextChar; i < m_lastChar; ++i)
	{
		c = m_rString[i];
		if ((c == _T('\n')) || (c == _T('\r')))
		{
			m_nLineNumber++;
			bEol = true;
			break;
		}
	}

	rStr = CString((LPCTSTR)m_rString + m_nextChar, i - m_nextChar);
	m_nextChar = i;

	if (bEol)
	{
		// skip CR or LF
		m_nextChar++;
		if (c == _T('\r'))
		{
			if ((m_nextChar < m_lastChar) && (m_rString[m_nextChar] == '\n'))
			{
				// skip LF
				m_nextChar++;
			}
		}
	}
#ifdef _DEBUG
	else
	{
		ASSERT(m_nextChar == m_lastChar);
	}
#endif
	return true;
}

int CCStringLineParser::GetLineNumber()
{
	return m_nLineNumber;
}

