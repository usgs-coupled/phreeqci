// LogicalLineParser.h: interface for the CLogicalLineParser class.
//
// $Date: 2000/10/04 20:17:47 $
// $Revision: 1.1.1.27 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGICALLINEPARSER_H__9D1F7CDB_74E9_48F7_8D82_79E63261C401__INCLUDED_)
#define AFX_LOGICALLINEPARSER_H__9D1F7CDB_74E9_48F7_8D82_79E63261C401__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineParser.h"

class CLogicalLineParser : public CLineParser
{
public:
	DECLARE_DYNAMIC(CLogicalLineParser)

	CLogicalLineParser(CLineParser& rLineParser);
	virtual ~CLogicalLineParser();

	virtual int GetLineNumber();
	virtual bool GetNextLine(CString& rStr);

protected:
	bool m_bEof;
	CLineParser& m_rLineParser;
	CString m_strLine;
	CString m_strTemp;

	bool GetLine(CString& rStr);
};
#endif // !defined(AFX_LOGICALLINEPARSER_H__9D1F7CDB_74E9_48F7_8D82_79E63261C401__INCLUDED_)
