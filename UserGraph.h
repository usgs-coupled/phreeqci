// UserGraph.h : header file
//
// This class defines custom modal property sheet
// CUserGraph.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "UserGraphPg1.h"
#include "UserGraphPg2.h"

/////////////////////////////////////////////////////////////////////////////
// CUserGraph

#include "CommonKeywordSheet.h"

#ifndef baseUserGraph
#define baseUserGraph CCommonKeywordSheet
#endif

class CUserGraph : public baseUserGraph
{
	DECLARE_DYNAMIC(CUserGraph)

// Construction
public:
	CUserGraph(CWnd* pWndParent = NULL);

// Attributes
public:
	CUserGraphPg1 m_Page1;
	//CUserGraphPg2 m_Page2;
	CUserGraphPg2 *m_pPage2;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserGraph)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual CString GetString();
	virtual void Edit(CString& rStr);
	virtual ~CUserGraph();

// Generated message map functions
protected:
	//{{AFX_MSG(CUserGraph)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
