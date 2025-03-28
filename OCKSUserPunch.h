// OCKSUserPunch.h : header file
//
// This class defines custom modal property sheet
// COCKSUserPunch.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#ifndef __OCKSUSERPUNCH_H__
#define __OCKSUSERPUNCH_H__

#include "KPUserPunchPg1.h"

/////////////////////////////////////////////////////////////////////////////
// COCKSUserPunch

#include "OleCtlKeywordSheet.h"

#ifndef baseOCKSUserPunch
#define baseOCKSUserPunch CCommonKeywordSheet
#endif

class COCKSUserPunch : public baseOCKSUserPunch
{
	DECLARE_DYNAMIC(COCKSUserPunch)

// Construction
public:
	COCKSUserPunch(CWnd* pWndParent = NULL);

// Attributes
public:
	CKPUserPunchPg1 m_Page1;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COCKSUserPunch)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual CString GetString();
	virtual void Edit(CString& rStr);
	virtual ~COCKSUserPunch();

// Generated message map functions
protected:
	//{{AFX_MSG(COCKSUserPunch)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __OCKSUSERPUNCH_H__
