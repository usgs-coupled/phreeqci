// OCKSRates.h : header file
//
// This class defines custom modal property sheet
// CCalculateValues.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#ifndef __CALCULATEVALUES_H__
#define __CALCULATEVALUES_H__

#include "CalculateValuesPg1.h"

/////////////////////////////////////////////////////////////////////////////
// CCalculateValues

#include "OleCtlKeywordSheet.h"

#ifndef baseCalculateValues
#define baseCalculateValues COleCtlKeywordSheet
#endif

class CCalculateValues : public baseCalculateValues
{
	DECLARE_DYNAMIC(CCalculateValues)

// Construction
public:
	CCalculateValues(CWnd* pWndParent = NULL);

// Attributes
public:
	CCalculateValuesPg1 m_Page1;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalculateValues)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual CString GetString();
	virtual void Edit(CString& rStr);
	virtual ~CCalculateValues();

// Generated message map functions
protected:
	//{{AFX_MSG(CCalculateValues)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __CALCULATEVALUES_H__
