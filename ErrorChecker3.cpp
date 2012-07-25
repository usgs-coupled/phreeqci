#include "stdafx.h"
#include "ErrorChecker3.h"
#include "SeException.h"

CErrorChecker3::CErrorChecker3(HWND hWndRichEdit, CTreeCtrlNode node)
: m_hWndRichEdit(hWndRichEdit)
, m_node(node)
, m_nErrors(0)
, m_nWarnings(0)
, m_nLineNumber(0)
{
	this->m_tr.chrg.cpMin     = 0;
	this->m_nWindowTextLength = ::GetWindowTextLength(this->m_hWndRichEdit);

	try
	{
		this->local_init();
		this->push_istream(&this->m_iss, false);
		for (this->simulation = 1; true; ++this->simulation)
		{
			if (this->read_input() == EOF) break;
		}
		this->input_error = 0;
	}
	catch (CSeException *e)
	{
		TCHAR trcMsg[1024];
		e->GetErrorMessage(trcMsg, 1024);
		TRACE(trcMsg);
		TRACE(_T("\n"));
		e->ReportError(MB_OK | MB_ICONSTOP);
		e->Delete();
	}
	catch (DWORD dw)
	{
		switch (dw)
		{
		case EXCEPTION_ACCESS_VIOLATION:
			ASSERT(FALSE);
			// ignore for now
			break;
		case INPUT_CONTAINS_ERRORS:
			ASSERT(FALSE);
			// ignore for now
			break;
		default:
			ASSERT(FALSE);
			break;
		}
	}
	catch (...)
	{
		TRACE("Caught exception");
	}

	CString strLine;
	strLine.Format("%d error(s), %d warning(s)", this->m_nErrors, this->m_nWarnings);
	this->m_node.AddTail(strLine, 3);
}

CErrorChecker3::~CErrorChecker3(void)
{
}

int CErrorChecker3::GetErrorCount()const
{
	return m_nErrors;
}

int CErrorChecker3::GetWarningCount()const
{
	return m_nWarnings;
}

int CErrorChecker3::GetLineNumber(void)const
{
	return this->m_nLineNumber;
}

int CErrorChecker3::getc(void)
{
	if (this->m_hWndRichEdit)
	{
		static char buffer[8];
		static long nChars;
		this->m_tr.chrg.cpMax = min(this->m_tr.chrg.cpMin + 2, this->m_nWindowTextLength);
		if (this->m_tr.chrg.cpMin < this->m_tr.chrg.cpMax)
		{
			this->m_tr.lpstrText = buffer;
			nChars = ::SendMessage(this->m_hWndRichEdit, EM_GETTEXTRANGE, 0, (LPARAM)&this->m_tr);
#if _MSC_VER < 1400
			ASSERT(nChars == 2 || nChars == 1);
			if (nChars == 2 || nChars == 1) 
			{
				++this->m_tr.chrg.cpMin;
				if (buffer[0] == '\r' && buffer[1] == '\n') {
					++this->m_tr.chrg.cpMin;
					buffer[0] = '\n';
				}
				if (buffer[0] == '\n') ++this->m_nLineNumber;
				return buffer[0];
			}
#else
			ASSERT(nChars == 2 || nChars == 1 || nChars == 0);
			if (nChars == 2 || nChars == 1) 
			{
				++this->m_tr.chrg.cpMin;
				if (buffer[0] == '\r')
				{
					if (buffer[1] == '\n')
					{
						++this->m_tr.chrg.cpMin;
					}
					buffer[0] = '\n';
				}
				if (buffer[0] == '\n') ++this->m_nLineNumber;
				return buffer[0];
			}
#endif
		}
	}
	return EOF;
}

void CErrorChecker3::warning_msg(const char *err_str)
{
	CString str;
	str.Format("Simulation %d: %s", this->simulation, err_str);		
	CTreeCtrlNode node = this->m_node.AddTail(str, 3);
	node.SetData(this->GetLineNumber());
	++this->m_nWarnings;
}

void CErrorChecker3::error_msg(const char *err_str, bool stop)
{
	CString str;
	str.Format("%s", err_str);		
	CTreeCtrlNode node = this->m_node.AddTail(str, 3);
	node.SetData(this->GetLineNumber());
	++this->m_nErrors;
	if (stop)
	{
		throw INPUT_CONTAINS_ERRORS;
	}
}

bool CErrorChecker3::output_open(const char *file_name, std::ios_base::openmode mode)
{
	UNUSED_ALWAYS(file_name);
	UNUSED_ALWAYS(mode);
	TRACE("CErrorChecker3::output_open\n");
	return true;
}

bool CErrorChecker3::log_open(const char *file_name, std::ios_base::openmode mode)
{
	UNUSED_ALWAYS(file_name);
	UNUSED_ALWAYS(mode);
	TRACE("CErrorChecker3::log_open\n");
	return true;
}

bool CErrorChecker3::punch_open(const char *file_name, std::ios_base::openmode mode)
{
	UNUSED_ALWAYS(file_name);
	UNUSED_ALWAYS(mode);
	TRACE("CErrorChecker3::punch_open\n");
	return true;
}

bool CErrorChecker3::error_open(const char *file_name, const char *mode)
{
	UNUSED_ALWAYS(file_name);
	UNUSED_ALWAYS(mode);
	TRACE("CErrorChecker3::error_open\n");
	return true;
}

bool CErrorChecker3::dump_open(const char *file_name, std::ios_base::openmode mode)
{
	UNUSED_ALWAYS(file_name);
	UNUSED_ALWAYS(mode);
	TRACE("CErrorChecker3::dump_open\n");
	return true;
}
