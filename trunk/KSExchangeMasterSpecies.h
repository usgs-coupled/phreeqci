// KSExchangeMasterSpecies.h : header file
//
// This class defines custom modal property sheet 
// CKSExchangeMasterSpecies.
 
#ifndef __KSEXCHANGEMASTERSPECIES_H__
#define __KSEXCHANGEMASTERSPECIES_H__

#include "KPExchangeMasterSpeciesPg1.h"

/////////////////////////////////////////////////////////////////////////////
// CKSExchangeMasterSpecies

#include "OleCtlKeywordSheet.h"

#ifndef baseCKSExchangeMasterSpecies
#define baseCKSExchangeMasterSpecies COleCtlKeywordSheet
#endif

class CKSExchangeMasterSpecies : public baseCKSExchangeMasterSpecies
{
	DECLARE_DYNAMIC(CKSExchangeMasterSpecies)

// Construction
public:
	CKSExchangeMasterSpecies(CWnd* pWndParent = NULL);

// Attributes
public:
	CKPExchangeMasterSpeciesPg1 m_Page1;

// Operations
public:
	virtual CString GetString();
	virtual void Edit(CString& rStr);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKSExchangeMasterSpecies)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKSExchangeMasterSpecies();

// Generated message map functions
protected:
	//{{AFX_MSG(CKSExchangeMasterSpecies)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __KSEXCHANGEMASTERSPECIES_H__
