// KeywordLoader2.h: interface for the CKeywordLoader2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYWORDLOADER2_H__D3C59018_75BE_40AE_BBBE_1ACD85307155__INCLUDED_)
#define AFX_KEYWORDLOADER2_H__D3C59018_75BE_40AE_BBBE_1ACD85307155__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <sstream>

class CKeywordLoader2  
{
public:
	CKeywordLoader2(CString& rString, bool bMoreThanOneKeyword=false);
	virtual ~CKeywordLoader2();

	static int ReadCallBack(void *cookie);
	static int WriteCallBack(const int action, const int type, const char *err_str, const int stop, void *cookie, const char *format, va_list args);

protected:
	std::string        m_s;
	std::istringstream m_iss;
};

#endif // !defined(AFX_KEYWORDLOADER2_H__D3C59018_75BE_40AE_BBBE_1ACD85307155__INCLUDED_)
