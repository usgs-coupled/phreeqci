// KeywordLoader.h: interface for the CKeywordLoader class.
//
// $Date: 2004/10/23 01:35:36 $
// $Revision: 1.2 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYWORDLOADER_H__0E828070_DE68_47C6_9012_0AE72C236A5D__INCLUDED_)
#define AFX_KEYWORDLOADER_H__0E828070_DE68_47C6_9012_0AE72C236A5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LogicalLineParser.h"
#include "PhreeqcCallback.h"

class CKeywordLoader  
{
public:
	CKeywordLoader(CLogicalLineParser& rLogicalLineParser);
	CKeywordLoader(CString& rString);
	virtual ~CKeywordLoader();

public:
	static BOOL CALLBACK LineCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);

protected:
	CLogicalLineParser& m_rLogicalLineParser;

	CPhreeqcCallback* m_pPhreeqcCallback;

};

#endif // !defined(AFX_KEYWORDLOADER_H__0E828070_DE68_47C6_9012_0AE72C236A5D__INCLUDED_)
