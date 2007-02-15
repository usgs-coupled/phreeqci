// CStringLineParser.h: interface for the CCStringLineParser class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSTRINGLINEPARSER_H__542AB846_A66A_44B2_A1A2_58B78F466A0C__INCLUDED_)
#define AFX_CSTRINGLINEPARSER_H__542AB846_A66A_44B2_A1A2_58B78F466A0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineParser.h"

class CCStringLineParser : public CLineParser  
{
public:
	CCStringLineParser(const CString& rString);
	virtual ~CCStringLineParser();

	DECLARE_DYNAMIC(CCStringLineParser)

	virtual bool GetNextLine(CString& rStr);
	virtual int GetLineNumber();
protected:
	int m_nLineNumber;
	int m_lastChar;
	int m_nextChar;
	const CString& m_rString;
};

#endif // !defined(AFX_CSTRINGLINEPARSER_H__542AB846_A66A_44B2_A1A2_58B78F466A0C__INCLUDED_)
