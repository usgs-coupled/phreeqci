// KSSolutionMasterSpecies.h : header file
//
// This class defines custom modal property sheet 
// CKSSolutionMasterSpecies.
 
#ifndef __KSSOLUTIONMASTERSPECIES_H__
#define __KSSOLUTIONMASTERSPECIES_H__

#include "KPSolutionMasterSpeciesPg1.h"

/////////////////////////////////////////////////////////////////////////////
// CKSSolutionMasterSpecies

#include "OleCtlKeywordSheet.h"

#ifndef baseCKSSolutionMasterSpecies
#define baseCKSSolutionMasterSpecies COleCtlKeywordSheet
#endif

class CKSSolutionMasterSpecies : public baseCKSSolutionMasterSpecies
{
	DECLARE_DYNAMIC(CKSSolutionMasterSpecies)

// Construction
public:
	CKSSolutionMasterSpecies(CWnd* pWndParent = NULL);

// Attributes
public:
	CKPSolutionMasterSpeciesPg1 m_Page1;

// Operations
public:
	virtual CString GetString();
	virtual void Edit(CString& rStr);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKSSolutionMasterSpecies)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKSSolutionMasterSpecies();

// Generated message map functions
protected:
	//{{AFX_MSG(CKSSolutionMasterSpecies)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __KSSOLUTIONMASTERSPECIES_H__
