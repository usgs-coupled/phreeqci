// KSSurfaceSpecies.h : header file
//
// $Date: 2001/08/20 20:36:16 $
// $Revision: 1.1.1.8 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////
// This class defines custom modal property sheet 
// CKSSurfaceSpecies.
 
#ifndef __KSSURFACESPECIES_H__
#define __KSSURFACESPECIES_H__

#include "KPSurfaceSpeciesPg1.h"

/////////////////////////////////////////////////////////////////////////////
// CKSSurfaceSpecies
#include "OleCtlKeywordSheet.h"

#ifndef baseCKSSurfaceSpecies
#define baseCKSSurfaceSpecies COleCtlKeywordSheet
#endif

class CKSSurfaceSpecies : public baseCKSSurfaceSpecies
{
	DECLARE_DYNAMIC(CKSSurfaceSpecies)

// Construction
public:
	CKSSurfaceSpecies(CWnd* pWndParent = NULL);

// Attributes
public:
	CKPSurfaceSpeciesPg1 m_Page1;

// Operations
public:
	virtual CString GetString();
	virtual void Edit(CString& rStr);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKSSurfaceSpecies)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKSSurfaceSpecies();

// Generated message map functions
protected:
	//{{AFX_MSG(CKSSurfaceSpecies)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __KSSURFACESPECIES_H__
