// LineParser.h: interface for the CLineParser class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINEPARSER_H__FE0D8581_659A_45D2_876D_BA07D7C098F1__INCLUDED_)
#define AFX_LINEPARSER_H__FE0D8581_659A_45D2_876D_BA07D7C098F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// abstract base class
class CLineParser : public CObject  
{
public:
	virtual ~CLineParser() = 0;

	DECLARE_DYNAMIC(CLineParser)

	virtual bool GetNextLine(CString& rStr) = 0;
	virtual int GetLineNumber() = 0;

};

#endif // !defined(AFX_LINEPARSER_H__FE0D8581_659A_45D2_876D_BA07D7C098F1__INCLUDED_)
