// OCKSRates.h : header file
//
// This class defines custom modal property sheet 
// COCKSRates.
//
// $Id$
//////////////////////////////////////////////////////////////////////
 
#ifndef __OCKSRATES_H__
#define __OCKSRATES_H__

#include "KPRatesPg1.h"

/////////////////////////////////////////////////////////////////////////////
// COCKSRates

#include "OleCtlKeywordSheet.h"

#ifndef baseOCKSRates
#define baseOCKSRates COleCtlKeywordSheet
#endif

class COCKSRates : public baseOCKSRates
{
	DECLARE_DYNAMIC(COCKSRates)

// Construction
public:
	COCKSRates(CWnd* pWndParent = NULL);

// Attributes
public:
	CKPRatesPg1 m_Page1;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COCKSRates)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual CString GetString();
	virtual void Edit(CString& rStr);
	virtual ~COCKSRates();

// Generated message map functions
protected:
	//{{AFX_MSG(COCKSRates)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __OCKSRATES_H__
