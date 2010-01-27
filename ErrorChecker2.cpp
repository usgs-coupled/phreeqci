// ErrorChecker2.cpp: implementation of the CErrorChecker2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "ErrorChecker2.h"

extern "C" {
#include "phreeqc/src/input.h"
#include "phreeqc/src/output.h"
#include "phreeqc/src/phrqproto.h"
DWORD check_errors_gui2(void *cookie, PFN_READ_CALLBACK pfnRead, PFN_OUTPUT_CALLBACK pfnWrite);
}

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CErrorChecker2::CErrorChecker2(HWND hWndRichEdit, CTreeCtrlNode node)
: m_hWndRichEdit(hWndRichEdit)
, m_node(node)
, m_nErrors(0)
, m_nWarnings(0)
, m_nLineNumber(0)
{
	m_tr.chrg.cpMin     = 0;
	m_nWindowTextLength = ::GetWindowTextLength(m_hWndRichEdit);

	::check_errors_gui2(this, ReadCallBack, WriteCallBack);

	CString strLine;
	strLine.Format("%d error(s), %d warning(s)", m_nErrors, m_nWarnings);
	m_node.AddTail(strLine, 3);
}

CErrorChecker2::~CErrorChecker2()
{
	::clean_up();
}

int CErrorChecker2::GetErrorCount()const
{
	return m_nErrors;
}

int CErrorChecker2::GetWarningCount()const
{
	return m_nWarnings;
}

int CErrorChecker2::GetLineNumber(void)const
{
	return this->m_nLineNumber;
}

int CErrorChecker2::ReadCallBack(void *cookie)
{
	ASSERT(cookie);
	CErrorChecker2* pThis = (CErrorChecker2*)cookie;
	if (pThis->m_hWndRichEdit)
	{
		static char buffer[8];
		static long nChars;
		pThis->m_tr.chrg.cpMax = min(pThis->m_tr.chrg.cpMin + 2, pThis->m_nWindowTextLength);
		if (pThis->m_tr.chrg.cpMin < pThis->m_tr.chrg.cpMax)
		{
			pThis->m_tr.lpstrText = buffer;
			nChars = ::SendMessage(pThis->m_hWndRichEdit, EM_GETTEXTRANGE, 0, (LPARAM)&pThis->m_tr);
#if _MSC_VER < 1400
			ASSERT(nChars == 2 || nChars == 1);
			if (nChars == 2 || nChars == 1) 
			{
				++pThis->m_tr.chrg.cpMin;
				if (buffer[0] == '\r' && buffer[1] == '\n') {
					++pThis->m_tr.chrg.cpMin;
					buffer[0] = '\n';
				}
				if (buffer[0] == '\n') ++pThis->m_nLineNumber;
				return buffer[0];
			}
#else
			ASSERT(nChars == 2 || nChars == 1 || nChars == 0);
			if (nChars == 2 || nChars == 1) 
			{
				++pThis->m_tr.chrg.cpMin;
				if (buffer[0] == '\r')
				{
					if (buffer[1] == '\n')
					{
						++pThis->m_tr.chrg.cpMin;
					}
					buffer[0] = '\n';
				}
				if (buffer[0] == '\n') ++pThis->m_nLineNumber;
				return buffer[0];
			}
#endif
		}
	}
	return EOF;
}

int CErrorChecker2::WriteCallBack(const int action, const int type, const char *err_str, const int stop, void *cookie, const char *format, va_list args)
{
	UNREFERENCED_PARAMETER(cookie);
	UNREFERENCED_PARAMETER(format);
	UNREFERENCED_PARAMETER(args);

	switch (action)
	{
	case ACTION_OUTPUT:
		{
			CTreeCtrlNode node;
			static CString str;

			ASSERT(cookie);
			CErrorChecker2 *pThis = (CErrorChecker2*)cookie;

			switch (type)
			{
			case OUTPUT_ERROR:
				str.Format("ERROR: %s", err_str);		
				node = pThis->m_node.AddTail(str, 3);
				node.SetData(pThis->GetLineNumber());
				++pThis->m_nErrors;
				break;

			case OUTPUT_WARNING:
				str.Format("WARNING: simulation %d: %s", ::simulation, err_str);		
				pThis->m_node.AddTail(str, 3);
				++pThis->m_nWarnings;
				break;
			}

			if (stop == STOP)
			{
				::RaiseException(INPUT_CONTAINS_ERRORS, 0, 0, NULL);
			}
		}
		break;

	case ACTION_OPEN:
		{
			TRACE("CErrorChecker2::WriteCallBack ACTION_OPEN\n");
			return OK;
		}
		break;

	default:
		break;
	}
	return 0;
}
