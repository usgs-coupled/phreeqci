// RichEditLineParserSafe.h: interface for the CRichEditLineParserSafe class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RICHEDITLINEPARSERSAFE_H__4313CBBE_F84D_4A99_8C93_276C0F41A0AD__INCLUDED_)
#define AFX_RICHEDITLINEPARSERSAFE_H__4313CBBE_F84D_4A99_8C93_276C0F41A0AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RichEditLineParser.h"
#include "RichDocOut.h"
#include "RichViewOut.h"
#include "LocalException.h"

class CRichEditLineParserSafe : public CRichEditLineParser  
{
// inner class
public:
	class CSignaledException : public CLocalException  
	{
	public:
		CSignaledException();
		virtual ~CSignaledException();

	};
public:
	bool GetNextLine(CString& rStr);
	CRichEditLineParserSafe(CRichViewOut::CThreadParam* p);
	virtual ~CRichEditLineParserSafe();

protected:
	HANDLE m_AbortEvent;
	void FillBuffer();
};



#endif // !defined(AFX_RICHEDITLINEPARSERSAFE_H__4313CBBE_F84D_4A99_8C93_276C0F41A0AD__INCLUDED_)
