// Delete.h : header file
//
// This class defines custom modal property sheet
// CDelete.
//
// $Id: Delete.h 138 2005-02-10 02:58:28Z charlton $
//////////////////////////////////////////////////////////////////////

#if !defined(__DELETE_H__)
#define __DELETE_H__

#include "DeletePg1.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Delete.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDelete

#include "OleCtlKeywordSheet.h"
#include "TreeCtrlIn.h"

#ifndef baseDelete
#define baseDelete COleCtlKeywordSheet
#endif


class CDelete : public baseDelete
{
	DECLARE_DYNAMIC(CDelete)

// Construction
public:
	CDelete(CWnd* pWndParent, CTreeCtrlNode simNode);
	///CDelete(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	///CDelete(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	CDeletePg1 m_Page1;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDelete)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual CString GetString();
	virtual CString GetLine(CKeyword::type kt, StorageBinListItem& item);

	virtual void Edit(CString& rStr);
	virtual void Edit(const CTreeCtrlNode& rKeyNode);

	const std::list<CTreeCtrlNode>& GetDeleteNodes()const
		{ return m_listCopyNodes; }

	virtual ~CDelete();

	// Generated message map functions
protected:
	std::list<CTreeCtrlNode> m_listCopyNodes;
	CTreeCtrlNode m_editNode;

	CDelete(); // NOT IMPLEMENTED

	//{{AFX_MSG(CDelete)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DELETE_H__)
