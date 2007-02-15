// KeywordParser.cpp: implementation of the CKeywordParser class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "KeywordParser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "LogicalLineParser.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKeywordParser::CKeywordParser(CLogicalLineParser& rLogicalLineParser)
: m_rLogicalLineParser(rLogicalLineParser),
  line(m_strLine), line_save(m_strLineSave), tree_line(m_strTreeLine),
  type(m_type), last_keyword(m_LastKeyword)
{
	m_bPushBack = false;
	m_type = typeNone;
}

CKeywordParser::~CKeywordParser()
{

}

IMPLEMENT_DYNAMIC(CKeywordParser, CObject)

void CKeywordParser::PushBack()
{
	if (m_type != typeNone)
	{
		m_bPushBack = true;
	}
}

enum CKeywordParser::type CKeywordParser::GetLine(CString& rStr)
{
	enum type typeReturn = CKeywordParser::GetLine();
	rStr = line_save;
	return typeReturn;
}

enum CKeywordParser::type CKeywordParser::GetLine()
{
	if (m_bPushBack || m_type == typeEof)
	{
		m_bPushBack = false;
		return m_type;
	}

	if (m_rLogicalLineParser.GetNextLine(m_strLineSave))
	{
		// Note: m_strLine(line) contains comments if any
		// unlike phreeqc's line only white space is removed
		m_strLine = m_strLineSave;
		m_strLine.TrimLeft();
		m_strLine.TrimRight();

		m_strTreeLine = GetTreeString(m_strLine);

		// Determine type
		m_type = typeOk;

		CString strTmp = m_strLine.SpanExcluding(_T(" \t#"));

		enum CKeyword::type keyType;

		if (m_strLine.IsEmpty())
		{
			m_type = typeEmpty;
		}
		else if ((keyType = CKeyword::GetKeywordType(strTmp)) != CKeyword::K_NOT_KEYWORD)
		{
			m_LastKeyword = keyType;
			m_type = typeKeyword;
		}
		else if (m_strLine[0] == _T('-') && m_strLine.GetLength() > 1 && ::_istalpha(m_strLine[1]))
		{
			m_type = typeOption;
		}
	}
	else
	{
		// no more lines
		ASSERT( m_strLineSave.IsEmpty() );
		m_strLine = m_strTreeLine = m_strLineSave;
		m_type = typeEof;
	}

	return m_type;
}

CString CKeywordParser::GetTreeString(const CString& rStr)
{
	CString strTree(rStr);
	strTree.Replace(_T('\t'), _T('»'));
	return strTree;
}

CString CKeywordParser::GetInverseTreeString(const CString& rStr)
{
	CString strInvTree(rStr);
	strInvTree.Replace(_T('»'), _T('\t'));
	return strInvTree;
}

bool CKeywordParser::IsOption(const std::set<CString>& setOptions, const CString& strLine)
{
	std::set<CString>::const_iterator strIter = setOptions.find(strLine);

	return (strIter != setOptions.end());
}

bool CKeywordParser::IsSolutionSpeciesOption(const CString &strLine)
{
	CString strToken = strLine.SpanExcluding(_T(" \t#"));
	strToken.MakeLower();
	strToken.Remove(_T('-'));

	static const char *opt_list[] = {
		"no_check",           /* 0 */
		"check",              /* 1 */
		"gamma",              /* 2 */
		"mb",                 /* 3 */
		"mass_balance",       /* 4 */
		"log_k",              /* 5 */
		"logk",               /* 6 */
		"delta_h",            /* 7 */
		"deltah",             /* 8 */
		"analytical_expression", /* 9 */
		"a_e",                /* 10 */
		"ae",                 /* 11 */
		"mole_balance"        /* 12 */
		};

	static std::set<CString> optionSet;

	if (optionSet.empty())
	{
		int size = sizeof(opt_list)/sizeof(const char *);
		for (int i = 0; i < size; ++i)
		{
			optionSet.insert(opt_list[i]);
		}
	}

	if (IsOption(optionSet, strLine))
	{
		return true;
	}
	else
	{
		int size = sizeof(opt_list)/sizeof(const char *);
		for (int i = 0; i < size; ++i)
		{
			if (strstr(opt_list[i], strToken) == opt_list[i])
			{
				return true;
			}
		}
	}
	return false;
}
