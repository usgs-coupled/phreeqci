// CKSSurface.h : header file
//
// This class defines custom modal property sheet 
// CCKSSurface.
//
// $Id$
//////////////////////////////////////////////////////////////////////
 
#ifndef __CKSSURFACE_H__
#define __CKSSURFACE_H__

#include "CSPSurfacePg1.h"

/////////////////////////////////////////////////////////////////////////////
// CCKSSurface

#include "CommonKeywordSheet.h"

#ifndef baseCKSSurface
#define baseCKSSurface CCommonKeywordSheet
#endif

class CCKSSurface : public baseCKSSurface
{
	DECLARE_DYNAMIC(CCKSSurface)

// Construction
public:
	CCKSSurface(CWnd* pWndParent = NULL, CTreeCtrlNode simNode = CTreeCtrlNode());

// Attributes
public:
	enum EquilSolutionNum
	{
		N_NONE = -1,
	};
	CCSPSurfacePg1 m_Page1;
	CCSPSurfacePg2 m_Page2;
	CCSPSurfacePg3 m_Page3;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCKSSurface)
	//}}AFX_VIRTUAL

// Implementation
public:
	bool m_bOnlyCounterIons;
	CString m_strNumFormat;
	double m_dThickness;
	std::set<CDBRange> m_setSolutions;
	bool m_bSolution_equilibria;
// COMMENT: {10/11/2006 8:55:29 PM}	bool m_bDiffuseLayer;
// COMMENT: {10/5/2006 9:27:25 PM}	bool m_bNoEDL;
// COMMENT: {10/11/2006 8:55:44 PM}	bool m_bDonnan;
	enum SITES_UNITS m_siteUnits;
	int m_nEquilSolutionNum;

	enum SURFACE_TYPE m_surfaceType;
	enum DIFFUSE_LAYER_TYPE m_dlType;

	virtual ~CCKSSurface();

	virtual CString GetString();
	virtual void Edit(CString& rStr);

// Generated message map functions
protected:
	CDefinedRanges m_ranges;
	void Fill();

	//{{AFX_MSG(CCKSSurface)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __CKSSURFACE_H__
