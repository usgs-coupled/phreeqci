#pragma once

#include "PhreeqcI.h"

class CErrorChecker3 : public PhreeqcI
{
public:
	CErrorChecker3(HWND hWndRichEdit, CTreeCtrlNode node);
	/*virtual*/ ~CErrorChecker3(void);

	int GetWarningCount(void)const;
	int GetErrorCount(void)const;

	int GetLineNumber(void)const;

	virtual int getc(void);
	virtual void warning_msg(const char *err_str);
	virtual void error_msg(const char * str, bool stop=false);

	// opening files
	virtual bool output_open(const char *file_name, std::ios_base::openmode mode = std::ios_base::out);
	virtual bool log_open(const char *file_name, std::ios_base::openmode mode = std::ios_base::out);
	virtual bool punch_open(const char *file_name, std::ios_base::openmode mode = std::ios_base::out);
	virtual bool error_open(const char *file_name, const char * mode = "w");
	virtual bool dump_open(const char *file_name, std::ios_base::openmode mode = std::ios_base::out);

protected:
	const HWND     m_hWndRichEdit;
	CTreeCtrlNode  m_node;
	int            m_nErrors;
	int            m_nWarnings;
	int            m_nLineNumber;
	TEXTRANGE      m_tr;
	int            m_nWindowTextLength;
};
