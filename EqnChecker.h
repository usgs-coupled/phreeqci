// EqnChecker.h: interface for the CEqnChecker class.
//
// $Date: 2004/10/23 01:35:35 $
// $Revision: 1.2 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EQNCHECKER_H__6163F014_FC22_4235_97B4_DCDECE9C0B85__INCLUDED_)
#define AFX_EQNCHECKER_H__6163F014_FC22_4235_97B4_DCDECE9C0B85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// COMMENT: {10/20/2004 8:07:54 PM}#include "PhreeqcCallback.h"

class CEqnChecker  
{
public:
	CEqnChecker();
	virtual ~CEqnChecker();
public:
	CString GetLastError();
	bool CheckAssociationRxn(LPCTSTR pstr, bool bCheck);
	bool CheckDissociationRxn(LPCTSTR pstr, bool bCheck);

public:
// COMMENT: {10/20/2004 8:08:06 PM}	static void CALLBACK ErrorCallBack(DWORD dwCookie, LPCSTR lpcstr);
	static int WriteCallBack(const int type, const char *err_str, const int stop, void *cookie, const char *format, va_list args);

protected:
// COMMENT: {10/20/2004 8:08:00 PM}	CPhreeqcCallback* m_pPhreeqcCallback;
	CString m_strErrors;
	unsigned int m_nErrors;


	bool CheckRxn(LPCTSTR pstr, bool bAssociation, bool bCheck);

};

#endif // !defined(AFX_EQNCHECKER_H__6163F014_FC22_4235_97B4_DCDECE9C0B85__INCLUDED_)
