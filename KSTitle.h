// KSTitle.h : header file
//
// This class defines custom modal property sheet 
// CKSTitle.
//
// $Date: 2001/09/14 20:12:03 $
// $Revision: 1.1.1.26 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////
 
#ifndef __KSTITLE_H__
#define __KSTITLE_H__

#include "KPTitlePg1.h"

/////////////////////////////////////////////////////////////////////////////
// CKSTitle

#include "KeywordSheet.h"

#ifndef baseKSTitle
#define baseKSTitle CKeywordSheet
#endif

class CKSTitle : public baseKSTitle
{
	DECLARE_DYNAMIC(CKSTitle)

// Construction
public:
	CKSTitle(CWnd* pWndParent = NULL);

// Attributes
public:
	CKPTitlePg1 m_Page1;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKSTitle)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void Edit(CString& rStr);
	virtual CString GetString();
	virtual ~CKSTitle();

// Generated message map functions
protected:
	//{{AFX_MSG(CKSTitle)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __KSTITLE_H__
