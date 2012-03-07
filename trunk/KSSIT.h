// KSKSPitzer.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////
// This class defines custom modal property sheet
// CKSSIT.

#ifndef __KSSIT_H__
#define __KSSIT_H__

#include "KPSITEpsilon.h"

/////////////////////////////////////////////////////////////////////////////
// CKSSolutionSpecies
#include "OleCtlKeywordSheet.h"

#ifndef baseCKSSIT
#define baseCKSSIT COleCtlKeywordSheet
#endif

class CKSSIT : public baseCKSSIT
{
	DECLARE_DYNAMIC(CKSSIT)

// Construction
public:
	CKSSIT(CWnd* pWndParent = NULL);

// Attributes
public:
	CKPSITEpsilon m_PageEpsilon;

// Operations
public:
	virtual CString GetString();
	virtual void Edit(CString& rStr);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKSSolutionSpecies)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKSSIT();

// Generated message map functions
protected:
	//{{AFX_MSG(CKSSIT)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __KSSIT_H__
