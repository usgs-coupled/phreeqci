// LogicalLineParser.cpp: implementation of the CLogicalLineParser class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "LogicalLineParser.h"

#include "LineParser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CLogicalLineParser, CLineParser)

CLogicalLineParser::CLogicalLineParser(CLineParser& rLineParser)
: m_rLineParser(rLineParser)
{
	m_bEof = !m_rLineParser.GetNextLine(m_strLine);
}

CLogicalLineParser::~CLogicalLineParser()
{
}


//
// returns
//		false
//			if EOF
//		true
//			otherwise
//
// throws
//		CMemoryException
//
bool CLogicalLineParser::GetNextLine(CString& rStr)
{
	rStr.Empty();

	if (!GetLine(m_strTemp))
	{
#ifdef _DEBUG
		/// TRACE("%d : %s\n", GetLineNumber(), (LPCTSTR)rStr);
#endif
		return false;
	}

	// Note: Comment overrides line continuation
	int nComment = m_strTemp.Find(_T('#'));

	while (nComment == -1 && m_strTemp.GetLength() && m_strTemp[m_strTemp.GetLength() - 1] == _T('\\'))
	{
		// extract line w/o line continuation
		rStr += m_strTemp.Mid(0, m_strTemp.GetLength() - 1);
		GetLine(m_strTemp);
		nComment = m_strTemp.Find(_T('#'));
	}

	// Remove semicolon if present
	// the semicolon is not removed in GetLine since
	// a line ending with \; should not be continued
	if (m_strTemp.GetLength() && m_strTemp[m_strTemp.GetLength() - 1] == _T(';'))
	{
		rStr += m_strTemp.Mid(0, m_strTemp.GetLength() - 1);
	}
	else
	{
		rStr += m_strTemp;
	}

#ifdef _DEBUG
	/// TRACE("%d : %s\n", GetLineNumber(), (LPCTSTR)rStr);
#endif
	return true;
}

//
// returns
//		false
//			if EOF
//		true
//			otherwise
//
// throws
//		CMemoryException
//
// Notes
//
bool CLogicalLineParser::GetLine(CString &rStr)
{
	if (m_bEof)
	{
		ASSERT( m_strLine.IsEmpty() );
		rStr.Empty();
		return false;
	}

	// Note: m_strLine is filled by ctor for first call
	int nLineTerminator = m_strLine.Find(_T(';'));

	if (nLineTerminator == -1)
	{
		rStr = m_strLine;
		m_bEof = !m_rLineParser.GetNextLine(m_strLine);
		return true;
	}
	else
	{
		// Note: Comment overrides logical line end
		int nComment = m_strLine.Find(_T('#'));
		if (nComment == -1 || nLineTerminator < nComment)
		{
			// keep semicolon
			rStr = m_strLine.Left(nLineTerminator + 1);
			m_strLine = m_strLine.Mid(nLineTerminator + 1);
			return true;
		}
		else
		{
			rStr = m_strLine;
			m_bEof = !m_rLineParser.GetNextLine(m_strLine);
			return true;
		}
	}
}

//
// returns the zero based line number last returned by GetNextLine
//
int CLogicalLineParser::GetLineNumber()
{
	// logical line parser is always one ahead of line parser
	return (max(0, m_rLineParser.GetLineNumber() - 1));
}
