// EqnChecker2.h: interface for the CEqnChecker2 class.
//
// $Date: 2004/11/03 06:56:11 $
// $Revision: 1.2 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EQNCHECKER2_H__6163F014_FC22_4235_97B4_DCDECE9C0B85__INCLUDED_)
#define AFX_EQNCHECKER2_H__6163F014_FC22_4235_97B4_DCDECE9C0B85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// COMMENT: {10/20/2004 8:07:54 PM}#include "PhreeqcCallback.h"

class CEqnChecker2  
{
public:
	CEqnChecker2();
	virtual ~CEqnChecker2();
public:
	CString GetLastError();
	bool CheckAssociationRxn(LPCTSTR pstr, bool bCheck);
	bool CheckDissociationRxn(LPCTSTR pstr, bool bCheck);

public:
	static int WriteCallBack(const int action, const int type, const char *err_str, const int stop, void *cookie, const char *format, va_list args);

protected:
	CString m_strErrors;
	unsigned int m_nErrors;

	bool CheckRxn(LPCTSTR pstr, bool bAssociation, bool bCheck);

};

#endif // !defined(AFX_EQNCHECKER2_H__6163F014_FC22_4235_97B4_DCDECE9C0B85__INCLUDED_)
