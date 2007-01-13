// CKSReaction.h : header file
//
// This class defines custom modal property sheet 
// CCKSReaction.
//
// $Id$
//////////////////////////////////////////////////////////////////////
 
#ifndef __CKSREACTION_H__
#define __CKSREACTION_H__

#include "CKPReactionPg1.h"

/////////////////////////////////////////////////////////////////////////////
// CCKSReaction

#include "CommonKeywordSheet.h"

#ifndef baseCKSReaction
#define baseCKSReaction CCommonKeywordSheet
#endif

class CCKSReaction : public baseCKSReaction
{
	DECLARE_DYNAMIC(CCKSReaction)

// Construction
public:
	CCKSReaction(CWnd* pWndParent = NULL);

// Attributes
public:
	CCKPReactionPg1 m_Page1;
	CCKPReactionPg2 m_Page2;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCKSReaction)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCKSReaction();

	virtual CString GetString();
	virtual void Edit(CString& rStr);

// Generated message map functions
protected:
	//{{AFX_MSG(CCKSReaction)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __CKSREACTION_H__
