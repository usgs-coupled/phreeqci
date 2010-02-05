// KeywordLoader2.cpp: implementation of the CKeywordLoader2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "KeywordLoader2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern "C" {
#include "phreeqc/src/input.h"
#include "phreeqc/src/output.h"
#include "phreeqc/src/phrqproto.h"
DWORD load_keyword2(void *cookie, PFN_READ_CALLBACK pfnRead, PFN_OUTPUT_CALLBACK pfnWrite);
DWORD load_keywords(void *cookie, PFN_READ_CALLBACK pfnRead, PFN_OUTPUT_CALLBACK pfnWrite);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKeywordLoader2::CKeywordLoader2(CString& rString, bool bMoreThanOneKeyword)
: m_s(rString)
, m_iss(m_s)
{
	if (bMoreThanOneKeyword)
	{
		::load_keywords(this, ReadCallBack, WriteCallBack);
	}
	else
	{
		::load_keyword2(this, ReadCallBack, WriteCallBack);
	}
}

CKeywordLoader2::~CKeywordLoader2()
{
	::clean_up();
}

int CKeywordLoader2::ReadCallBack(void *cookie)
{
	ASSERT(cookie);
	CKeywordLoader2 *pThis = (CKeywordLoader2*)cookie;

	static int i;
	i = pThis->m_iss.get();
	if (i == '\r') {
		i = pThis->m_iss.get();
#if _MSC_VER < 1400
		ASSERT(i == '\n');
#endif
		if (i != '\n') {
			pThis->m_iss.unget();
			i = '\n';
		}
	}
	return i;
}

int CKeywordLoader2::WriteCallBack(const int action, const int type, const char *err_str, const int stop, void *cookie, const char *format, va_list args)
{
	UNREFERENCED_PARAMETER(cookie);
	UNREFERENCED_PARAMETER(format);
	UNREFERENCED_PARAMETER(args);

	switch (action)
	{
	case ACTION_OUTPUT:
		{
			if (stop == STOP) {
				::RaiseException(INPUT_CONTAINS_ERRORS, 0, 0, NULL);
			}
		}
		break;
	case ACTION_OPEN:
		{
			switch (type)
			{
			case OUTPUT_PUNCH:
				::free_check_null(::selected_output_file_name);
				::selected_output_file_name = ::string_duplicate(err_str);
				return OK;
				break;

			case OUTPUT_DUMP:
				::free_check_null(::dump_file_name);
				::dump_file_name = ::string_duplicate(err_str);
				return OK;
				break;
			}
		}
	}

	return 0;
}
