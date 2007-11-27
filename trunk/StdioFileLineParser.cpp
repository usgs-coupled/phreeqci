// StdioFileLineParser.cpp: implementation of the CStdioFileLineParser class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "StdioFileLineParser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//
//
// throw ( CFileException )
//
CStdioFileLineParser::CStdioFileLineParser(LPCTSTR lpszFileName)
: m_stdioFile(lpszFileName, CFile::modeRead|CFile::shareDenyWrite)
{
	// Open file readonly and let other processes read but not
	// write to the file while it is being processed
	m_nLineNumber = 0;
}

CStdioFileLineParser::~CStdioFileLineParser()
{

}

IMPLEMENT_DYNAMIC(CStdioFileLineParser, CLineParser)

//
// returns the zero based line number last returned by GetNextLine
//
int CStdioFileLineParser::GetLineNumber()
{
	// logical line parser is always one ahead of line parser
	return (max(0, m_nLineNumber - 1));
}

//
//
// throw ( CFileException )
//
bool CStdioFileLineParser::GetNextLine(CString& rStr)
{
	BOOL bVal = m_stdioFile.ReadString(rStr); // throw( CFileException )
	if (bVal)
	{
		m_nLineNumber++;
	}
#ifdef _DEBUG
	else
	{
		ASSERT(rStr.IsEmpty());
	}
#endif
#ifdef _DEBUG
	/// TRACE("%d : %s\n", GetLineNumber(), (LPCTSTR)rStr);
#endif
	return (bVal != 0);
}


