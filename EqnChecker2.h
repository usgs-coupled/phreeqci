// EqnChecker2.h: interface for the CEqnChecker2 class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EQNCHECKER2_H__6163F014_FC22_4235_97B4_DCDECE9C0B85__INCLUDED_)
#define AFX_EQNCHECKER2_H__6163F014_FC22_4235_97B4_DCDECE9C0B85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PhreeqcI.h"

class CEqnChecker2 : public PhreeqcI 
{
public:
	CEqnChecker2();
	virtual ~CEqnChecker2();
public:
	CString GetLastError();
	bool CheckAssociationRxn(LPCTSTR pstr, bool bCheck);
	bool CheckDissociationRxn(LPCTSTR pstr, bool bCheck);

	virtual void error_msg(const char * str, bool stop=false);

protected:
	CString m_strErrors;
	unsigned int m_nErrors;

	bool CheckRxn(LPCTSTR pstr, bool bAssociation, bool bCheck);

};

#endif // !defined(AFX_EQNCHECKER2_H__6163F014_FC22_4235_97B4_DCDECE9C0B85__INCLUDED_)
