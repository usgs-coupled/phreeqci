// KSSurfaceMasterSpecies.h : header file
//
// This class defines custom modal property sheet 
// CKSSurfaceMasterSpecies.
 
#ifndef __KSSURFACEMASTERSPECIES_H__
#define __KSSURFACEMASTERSPECIES_H__

#include "KPSurfaceMasterSpeciesPg1.h"

/////////////////////////////////////////////////////////////////////////////
// CKSSurfaceMasterSpecies

#include "OleCtlKeywordSheet.h"

#ifndef baseCKSSurfaceMasterSpecies
#define baseCKSSurfaceMasterSpecies COleCtlKeywordSheet
#endif

class CKSSurfaceMasterSpecies : public baseCKSSurfaceMasterSpecies
{
	DECLARE_DYNAMIC(CKSSurfaceMasterSpecies)

// Construction
public:
	CKSSurfaceMasterSpecies(CWnd* pWndParent = NULL);

// Attributes
public:
	CKPSurfaceMasterSpeciesPg1 m_Page1;

// Operations
public:
	virtual CString GetString();
	virtual void Edit(CString& rStr);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKSSurfaceMasterSpecies)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKSSurfaceMasterSpecies();

// Generated message map functions
protected:
	//{{AFX_MSG(CKSSurfaceMasterSpecies)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __KSSURFACEMASTERSPECIES_H__
