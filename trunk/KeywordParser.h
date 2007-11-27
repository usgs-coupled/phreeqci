// KeywordParser.h: interface for the CKeywordParser class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYWORDPARSER_H__FA34DCC6_D73A_4E94_A5D3_591E15B3BD5B__INCLUDED_)
#define AFX_KEYWORDPARSER_H__FA34DCC6_D73A_4E94_A5D3_591E15B3BD5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Keyword.h"

// forward declarations
class CLogicalLineParser;

class CKeywordParser : public CObject  
{
public:
	enum type
	{
		typeNone    = -2,
		typeEof     = -1,
		typeError   =  0,
		typeOk      =  1,
		typeEmpty   =  2,
		typeKeyword =  3,
		typeOption  =  8,
	};

	// read-only
	const CString& line;
	const CString& line_save;
	const CString& tree_line;
	const enum type& type;
	const enum CKeyword::type& last_keyword;

public:
	static bool IsSolutionSpeciesOption(const CString& strLine);
	static bool IsOption(const std::set<CString>& setOptions, const CString& strLine);
	CKeywordParser(CLogicalLineParser& rLogicalLineParser);
	virtual ~CKeywordParser();

	DECLARE_DYNAMIC(CKeywordParser)

	enum type GetLine(CString& rStr);
	enum type GetLine();
	void PushBack();
	static CString GetTreeString(const CString& rStr);
	static CString GetInverseTreeString(const CString& rStr);
private:
	bool m_bPushBack;
	enum type m_type;
	CString m_strLine;
	CString m_strLineSave;
	CString m_strTreeLine;
	enum CKeyword::type m_LastKeyword;
	CLogicalLineParser& m_rLogicalLineParser;
};

#endif // !defined(AFX_KEYWORDPARSER_H__FA34DCC6_D73A_4E94_A5D3_591E15B3BD5B__INCLUDED_)
