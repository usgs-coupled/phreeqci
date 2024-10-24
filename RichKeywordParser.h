// RichKeywordParser.h: interface for the CRichKeywordParser class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RICHKEYWORDPARSER_H__FA34DCC6_D73A_4E94_A5D3_591E15B3BD5B__INCLUDED_)
#define AFX_RICHKEYWORDPARSER_H__FA34DCC6_D73A_4E94_A5D3_591E15B3BD5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KeywordParser.h"

// forward declarations
class CRichLogicalLineParser;

class CRichKeywordParser : public CKeywordParser  
{
public:
	// read-only
	const CHARRANGE& cr;

public:
	CRichKeywordParser(CRichLogicalLineParser& rRichLogicalLineParser);
	virtual ~CRichKeywordParser();

	DECLARE_DYNAMIC(CRichKeywordParser)
};

#endif // !defined(AFX_RICHKEYWORDPARSER_H__FA34DCC6_D73A_4E94_A5D3_591E15B3BD5B__INCLUDED_)
