// ErrorChecker2.h: interface for the CErrorChecker2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ERRORCHECKER2_H__4BBD3A41_A2B0_402A_9C37_7C0E8308988B__INCLUDED_)
#define AFX_ERRORCHECKER2_H__4BBD3A41_A2B0_402A_9C37_7C0E8308988B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CErrorChecker2  
{
public:
	CErrorChecker2(HWND hWndRichEdit, CTreeCtrlNode node);
	virtual ~CErrorChecker2(void);

	int GetWarningCount(void)const;
	int GetErrorCount(void)const;

	int GetLineNumber(void)const;


	static int ReadCallBack(void *cookie);
	static int WriteCallBack(const int action, const int type, const char *err_str, const int stop, void *cookie, const char *format, va_list args);


protected:
	const HWND     m_hWndRichEdit;
	CTreeCtrlNode  m_node;
	int            m_nErrors;
	int            m_nWarnings;
	int            m_nLineNumber;
	TEXTRANGE      m_tr;
	int            m_nWindowTextLength;
};

#endif // !defined(AFX_ERRORCHECKER2_H__4BBD3A41_A2B0_402A_9C37_7C0E8308988B__INCLUDED_)
