// KSSolutionSpecies.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////
// This class defines custom modal property sheet 
// CKSSolutionSpecies.
 
#ifndef __KSSOLUTIONSPECIES_H__
#define __KSSOLUTIONSPECIES_H__

#include "KPSolutionSpeciesPg1.h"

/////////////////////////////////////////////////////////////////////////////
// CKSSolutionSpecies
#include "OleCtlKeywordSheet.h"

#ifndef baseCKSSolutionSpecies
#define baseCKSSolutionSpecies COleCtlKeywordSheet
#endif

class CKSSolutionSpecies : public baseCKSSolutionSpecies
{
	DECLARE_DYNAMIC(CKSSolutionSpecies)

// Construction
public:
	CKSSolutionSpecies(CWnd* pWndParent = NULL);

// Attributes
public:
	CKPSolutionSpeciesPg1 m_Page1;

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
	virtual ~CKSSolutionSpecies();

// Generated message map functions
protected:
	//{{AFX_MSG(CKSSolutionSpecies)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __KSSOLUTIONSPECIES_H__
