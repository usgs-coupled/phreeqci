// RichLogicalLineParser.cpp: implementation of the CRichLogicalLineParser class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "RichLogicalLineParser.h"

#include "RichEditLineParser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CRichLogicalLineParser, CLogicalLineParser)

CRichLogicalLineParser::CRichLogicalLineParser(CRichEditLineParser& rRichEditLineParser)
: CLogicalLineParser(rRichEditLineParser), m_rRichEditLineParser(rRichEditLineParser)
{
	// Note:
	// This is dependant upon the base ctor calling
	// m_bEof = !m_rLineParser.GetNextLine(m_strLine);
	m_crLine.cpMax = m_rRichEditLineParser.m_crLine.cpMin;
}

CRichLogicalLineParser::~CRichLogicalLineParser()
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
bool CRichLogicalLineParser::GetNextLine(CString& rStr)
{
	rStr.Empty();

	if (!GetLine(m_strTemp))
	{
		m_crLine.cpMin = m_crLine.cpMax;
		m_crLine.cpMax = -1;
#ifdef _DEBUG
		/// TRACE("%d : %s\n", GetLineNumber(), (LPCTSTR)rStr);
#endif
		return false;
	}

	// Note: Comment overrides line continuation
	int nComment = m_strTemp.Find(_T('#'));

	// save start of range
	long cpMinSave = m_crLine.cpMin;

	while (nComment == -1 && m_strTemp.GetLength() && m_strTemp[m_strTemp.GetLength() - 1] == _T('\\'))
	{
		// extract line w/o line continuation
		rStr += m_strTemp.Mid(0, m_strTemp.GetLength() - 1);
		GetLine(m_strTemp);
		nComment = m_strTemp.Find(_T('#'));
		// reset start of range
		m_crLine.cpMin = cpMinSave;
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
bool CRichLogicalLineParser::GetLine(CString &rStr)
{
	if (m_bEof)
	{
		ASSERT( m_strLine.IsEmpty() );
		rStr.Empty();
		return false;
	}

	// Note: m_strLine is filled by ctor for first call
	int nLineTerminator = m_strLine.Find(_T(';'));

	m_crLine.cpMin = m_crLine.cpMax;

	if (nLineTerminator == -1)
	{
		rStr = m_strLine;
		m_bEof = !m_rRichEditLineParser.GetNextLine(m_strLine);
		m_crLine.cpMax = m_rRichEditLineParser.m_crLine.cpMin;
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
			m_crLine.cpMax = m_crLine.cpMin + nLineTerminator + 1;
			return true;
		}
		else
		{
			rStr = m_strLine;
			m_crLine.cpMax = m_rRichEditLineParser.m_crLine.cpMax;
			m_bEof = !m_rRichEditLineParser.GetNextLine(m_strLine);
			m_crLine.cpMax = m_rRichEditLineParser.m_crLine.cpMin;
			return true;
		}
	}
}

//
// returns the zero based line number last returned by GetNextLine
//
int CRichLogicalLineParser::GetLineNumber()
{
	// logical line parser is always one ahead of line parser
	return (max(0, m_rRichEditLineParser.GetLineNumber() - 1));
}
