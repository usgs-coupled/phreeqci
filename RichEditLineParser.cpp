// RichEditLineParser.cpp: implementation of the CRichEditLineParser class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "RichEditLineParser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//
// throws CRichEditLineParser::CNotWindowException
//
CRichEditLineParser::CRichEditLineParser(HWND hwndRichEdit)
: m_hwndRichEdit(hwndRichEdit)
{
	Init();
}

//
// throws CRichEditLineParser::CNotWindowException
//
CRichEditLineParser::CRichEditLineParser(CRichEditCtrl *pCtrl)
: m_hwndRichEdit(pCtrl->m_hWnd)
{
	Init();
}

//
// throws CRichEditLineParser::CNotWindowException
//
void CRichEditLineParser::Init()
{
	if ( !::IsWindow(m_hwndRichEdit) )
	{
		// Note: the dtor will not be called for this
		throw new CNotWindowException();
	}

	m_bPushBack = false;

	m_nextChar          = 0;
	m_lastChar          = 0;
	m_crBuffer.cpMin    = 0;
	m_nLineNumber       = 0;
	m_nNewLine          = 0;

	m_nWindowTextLength = ::GetWindowTextLength(m_hwndRichEdit);

	m_crBuffer.cpMin = 0;
	m_crBuffer.cpMax = -2;
}


CRichEditLineParser::~CRichEditLineParser()
{
}

IMPLEMENT_DYNAMIC(CRichEditLineParser, CLineParser)

//
// Fills buffer
//
void CRichEditLineParser::FillBuffer()
{
	if (m_crBuffer.cpMax == -2)
	{
		// this is the first call
		ASSERT( m_crBuffer.cpMin == 0 );
		m_crBuffer.cpMax = min(BUFFER_SIZE - 1, m_nWindowTextLength);
	}
	else
	{
		ASSERT( m_crBuffer.cpMax >= 0 );
		m_crBuffer.cpMin = m_crBuffer.cpMax;
		m_crBuffer.cpMax = min(m_crBuffer.cpMin + BUFFER_SIZE - 1, m_nWindowTextLength);
	}

	TEXTRANGE tr;
	tr.chrg = m_crBuffer;
	tr.lpstrText = m_buffer;
	int nChars;

	// window may have been destroyed
	if (::IsWindow(m_hwndRichEdit))
	{
		nChars = ::SendMessage(m_hwndRichEdit, EM_GETTEXTRANGE, 0, (LPARAM)&tr);
	}
	else
	{
		// EOF if window no longer exists
		m_lastChar = 0;
		return;
	}

	if (nChars > 0) // if (nChars == 0) EOF
	{
		m_nextChar = 0;
		m_lastChar = nChars;
	}
	else
	{
		// EOF
		m_lastChar = 0;
	}
}

//
// returns the zero based line number last returned by GetNextLine
//
int CRichEditLineParser::GetLineNumber()
{
	// ASSERT( (m_nLineNumber - 1) >= 0 );
	// return m_nLineNumber - 1;
	return (max(0, m_nNewLine - 1));
}

//
// returns:
//		false if end-of-file was reached without reading any data.
//		true  otherwise
//
bool CRichEditLineParser::GetNextLine(CString& rStr) // , bool bIncludeCRLF /* = false */)
{
	bool bIncludeCRLF = false;
	if (m_bPushBack)
	{
		// what happens if GetNextLine has not been called yet?
		rStr = m_strLastLine;
		m_bPushBack = false;
		ASSERT( VerifyRange(rStr) );
#ifdef _DEBUG
		/// TRACE("%d : %s\n", GetLineNumber(), (LPCTSTR)rStr);
#endif
		return (m_lastChar > 0);
	}

	rStr = &afxChNil;    // empty string without deallocating

	// save beginning of line
	m_crLine.cpMin = m_nextChar + m_crBuffer.cpMin;

	for (;;)
	{
		if (m_nextChar >= m_lastChar)
		{
		    FillBuffer();
		    // Note: m_nextChar is not modified on EOF
			if (m_nextChar >= m_lastChar)
			{
				ASSERT( m_lastChar == 0 );	// must be EOF
				m_strLastLine = rStr;
				// save ending of line
				m_crLine.cpMax = m_nextChar + m_crBuffer.cpMin;
				ASSERT(	VerifyRange(rStr) );
#ifdef _DEBUG
				/// TRACE("%d : %s\n", GetLineNumber(), (LPCTSTR)rStr);
#endif
				return !(rStr.IsEmpty());
			}
		}

		int i;
		bool bEol = false;
		TCHAR c = 0;

		for (i = m_nextChar; i < m_lastChar; ++i)
		{
			c = m_buffer[i];
			if ((c == '\n') || (c == '\r'))
			{
				m_nLineNumber++;
				m_nNewLine++;
				bEol = true;
				break;
			}
		}

		rStr += CString(&m_buffer[m_nextChar], i - m_nextChar);
		m_nextChar = i;

		if (bEol)
		{
			if (bIncludeCRLF)
			{
				rStr += m_buffer[m_nextChar];
			}

			// skip CR or LF
			m_nextChar++;
			if (c == '\r')
			{
				// this handles the special case when m_lastChar == '\r'
				if (m_nextChar >= m_lastChar)
				{
					FillBuffer();
				}
				if ((m_nextChar < m_lastChar) && (m_buffer[m_nextChar] == '\n'))
				{
					if (bIncludeCRLF)
					{
						rStr += m_buffer[m_nextChar];
					}
					// skip LF
					m_nextChar++;
				}
			}
			break;
		}
	}
	// save ending of line
	m_crLine.cpMax = m_nextChar + m_crBuffer.cpMin;

	ASSERT( VerifyRange(rStr) );

	m_strLastLine = rStr;
#ifdef _DEBUG
	/// TRACE("%d : %s\n", GetLineNumber(), (LPCTSTR)rStr);
#endif
	return true;
}

//
//
//
void CRichEditLineParser::PushBack()
{
	ASSERT( m_bPushBack == false );	// mulitiple PushBack not supported
	m_bPushBack = true;
}

#ifdef _DEBUG
bool CRichEditLineParser::VerifyRange(const CString &rStr)
{
	TCHAR* buffer = new TCHAR[m_crLine.cpMax - m_crLine.cpMin + 1];
	ASSERT( buffer );

	TEXTRANGE tr;
	tr.chrg = m_crLine;
	tr.lpstrText = buffer;

	int nChars = ::SendMessage(m_hwndRichEdit, EM_GETTEXTRANGE, 0, (LPARAM)&tr);
	UNUSED(nChars);

	if( _tcsncmp(rStr, buffer, rStr.GetLength()) == 0 )
	{
		delete [] buffer;
		return true;
	}
	else
	{
		delete [] buffer;
		return false;
	}
}
#endif

//////////////////////////////////////////////////////////////////////
// CNotWindowException Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//
//
//
CRichEditLineParser::CNotWindowException::CNotWindowException()
{

}

//
//
//
CRichEditLineParser::CNotWindowException::~CNotWindowException()
{

}


