// OCKSSolution_Spread.h : header file
//
// This class defines custom modal property sheet 
// COCKSSolution_Spread.
//
// $Date: 2002/11/13 21:39:18 $
// $Revision: 1.2 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////
 
#ifndef __OCKSSOLUTION5F_SPREAD_H__
#define __OCKSSOLUTION5F_SPREAD_H__

#include "KPSolution_SpreadPg1.h"

/////////////////////////////////////////////////////////////////////////////
// COCKSSolution_Spread
#include "CommonKeywordSheet.h"

#ifndef baseOCKSSolution_Spread
#define baseOCKSSolution_Spread CCommonKeywordSheet
#endif

class COCKSSolution_Spread : public baseOCKSSolution_Spread
{
	DECLARE_DYNAMIC(COCKSSolution_Spread)

// Construction
public:
	COCKSSolution_Spread(CWnd* pWndParent = NULL);

// Attributes
public:
	CKPSolution_SpreadPg1 m_Page1;
	CKPSolution_SpreadPg2 m_Page2;
	CKPSolution_SpreadPg3 m_Page3;

	CKPSolution_SpreadPg4* m_pPage4;
	std::list<CIsotope> m_listIsotopes;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COCKSSolution_Spread)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COCKSSolution_Spread();

	virtual CString GetString();
	virtual void Edit(CString& rStr);

// Generated message map functions
protected:
	//{{AFX_MSG(COCKSSolution_Spread)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __OCKSSOLUTION5F_SPREAD_H__
