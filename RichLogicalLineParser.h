// RichLogicalLineParser.h: interface for the CRichLogicalLineParser class.
//
// $Date: 2000/10/04 20:17:54 $
// $Revision: 1.1.1.25 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RICHLOGICALLINEPARSER_H__820CEE0D_6B85_48E9_B01F_B66E678E78F0__INCLUDED_)
#define AFX_RICHLOGICALLINEPARSER_H__820CEE0D_6B85_48E9_B01F_B66E678E78F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LogicalLineParser.h"

// forward declarations
class CRichEditLineParser;

class CRichLogicalLineParser : public CLogicalLineParser  
{
public:
	CRichLogicalLineParser(CRichEditLineParser& rRichEditLineParser);
	virtual ~CRichLogicalLineParser();

	DECLARE_DYNAMIC(CRichLogicalLineParser)

	virtual int GetLineNumber();
	virtual bool GetNextLine(CString& rStr);

protected:
	friend class CRichKeywordParser;
	CRichEditLineParser& m_rRichEditLineParser;
	// bool m_bEof;
	CHARRANGE m_crLine;
	// CString m_strLine;
	// CString m_strTemp;

	bool GetLine(CString& rStr);
};

#endif // !defined(AFX_RICHLOGICALLINEPARSER_H__820CEE0D_6B85_48E9_B01F_B66E678E78F0__INCLUDED_)
