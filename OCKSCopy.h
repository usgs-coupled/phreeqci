// OCKSCopy.h : header file
//
// This class defines custom modal property sheet 
// COCKSCopy.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(__OCKSCOPY_H__)
#define __OCKSCOPY_H__

#include "KPCopyPg1.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OCKSCopy.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COCKSCopy

#include "OleCtlKeywordSheet.h"
#include "TreeCtrlIn.h"

#ifndef baseOCKSCopy
#define baseOCKSCopy COleCtlKeywordSheet
#endif


class COCKSCopy : public baseOCKSCopy
{
	DECLARE_DYNAMIC(COCKSCopy)

// Construction
public:
	COCKSCopy(CWnd* pWndParent, CTreeCtrlNode simNode);
	///COCKSCopy(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	///COCKSCopy(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	CKPCopyPg1 m_Page1;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COCKSCopy)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual CString GetString();
	virtual void Edit(CString& rStr);
	virtual void Edit(const CTreeCtrlNode& rKeyNode);

	const std::list<CTreeCtrlNode>& GetDeleteNodes()const
		{ return m_listCopyNodes; }

	virtual ~COCKSCopy();

	// Generated message map functions
protected:
	std::list<CTreeCtrlNode> m_listCopyNodes;
	CTreeCtrlNode m_editNode;

	void ParseString(const CString& str);

	COCKSCopy(); // NOT IMPLEMENTED

	//{{AFX_MSG(COCKSCopy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__OCKSCOPY_H__)
