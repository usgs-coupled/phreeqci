// KSExchangeSpecies.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

// This class defines custom modal property sheet 
// CKSExchangeSpecies.
 
#ifndef __KSEXCHANGESPECIES_H__
#define __KSEXCHANGESPECIES_H__

#include "KPExchangeSpeciesPg1.h"

/////////////////////////////////////////////////////////////////////////////
// CKSExchangeSpecies
#include "OleCtlKeywordSheet.h"

#ifndef baseCKSExchangeSpecies
#define baseCKSExchangeSpecies COleCtlKeywordSheet
#endif

class CKSExchangeSpecies : public baseCKSExchangeSpecies
{
	DECLARE_DYNAMIC(CKSExchangeSpecies)

// Construction
public:
	CKSExchangeSpecies(CWnd* pWndParent = NULL);

// Attributes
public:
	CKPExchangeSpeciesPg1 m_Page1;

// Operations
public:
	virtual CString GetString();
	virtual void Edit(CString& rStr);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKSExchangeSpecies)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKSExchangeSpecies();

// Generated message map functions
protected:
	//{{AFX_MSG(CKSExchangeSpecies)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __KSEXCHANGESPECIES_H__
