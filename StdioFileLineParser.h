// StdioFileLineParser.h: interface for the CStdioFileLineParser class.
//
// $Date: 2000/10/04 20:17:56 $
// $Revision: 1.1.1.25 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STDIOFILELINEPARSER_H__02C78CCF_BA61_4A33_86A2_245584E2C3D9__INCLUDED_)
#define AFX_STDIOFILELINEPARSER_H__02C78CCF_BA61_4A33_86A2_245584E2C3D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineParser.h"

class CStdioFileLineParser : public CLineParser
{
public:
	virtual bool GetNextLine(CString& rStr);
	virtual int GetLineNumber();
	CStdioFileLineParser(LPCTSTR lpszFileName);
	virtual ~CStdioFileLineParser();

	DECLARE_DYNAMIC(CStdioFileLineParser)

protected:
	int m_nLineNumber;
	CStdioFile m_stdioFile;
};

#endif // !defined(AFX_STDIOFILELINEPARSER_H__02C78CCF_BA61_4A33_86A2_245584E2C3D9__INCLUDED_)
