// RichEditLineParser.h: interface for the CRichEditLineParser class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RICHEDITLINEPARSER_H__FDC002E8_0836_11D4_9AE4_00E029494D43__INCLUDED_)
#define AFX_RICHEDITLINEPARSER_H__FDC002E8_0836_11D4_9AE4_00E029494D43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tree.h"

const size_t BUFFER_SIZE = 32000;

#include "LocalException.h"
#include "LineParser.h"

class CRichEditLineParser : public CLineParser  
{
	// inner class
public:
	class CNotWindowException : public CLocalException  
	{
	public:
		CNotWindowException();
		virtual ~CNotWindowException();
	};

public:
	virtual int GetLineNumber();
	void PushBack();
	virtual bool GetNextLine(CString& rStr); // , bool bIncludeCRLF = false);
	CRichEditLineParser(CRichEditCtrl* pCtrl);
	CRichEditLineParser(HWND hwndRichEdit);
	virtual ~CRichEditLineParser();

	DECLARE_DYNAMIC(CRichEditLineParser)

protected:
	friend class CRichLogicalLineParser;
	void Init();
	void FillBuffer();

#ifdef _DEBUG
	bool VerifyRange(const CString& rStr);
#endif

	bool m_bPushBack;
	int m_nLineNumber;
	int m_nNewLine;
	int m_nextChar;
	int m_nWindowTextLength;
	int m_lastChar;
	CString m_strLastLine;
	TCHAR m_buffer[BUFFER_SIZE];
	CHARRANGE m_crBuffer;		// holds the CHARRANGE of the current buffer
	CHARRANGE m_crLine;			// holds the CHARRANGE of last line including CRLF
	const HWND m_hwndRichEdit;
};

#endif // !defined(AFX_RICHEDITLINEPARSER_H__FDC002E8_0836_11D4_9AE4_00E029494D43__INCLUDED_)
