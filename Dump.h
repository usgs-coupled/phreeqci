// Dump.h : header file
//
// This class defines custom modal property sheet
// CDump.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(__DUMP_H__)
#define __DUMP_H__

#include "DumpPg1.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Delete.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDump

#include "OleCtlKeywordSheet.h"
#include "TreeCtrlIn.h"

#ifndef baseDump
#define baseDump COleCtlKeywordSheet
#endif


class CDump : public baseDump
{
	DECLARE_DYNAMIC(CDump)

// Construction
public:
	CDump(CWnd* pWndParent = NULL);

// Attributes
public:
	CDumpPg1 m_Page1;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDump)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual CString GetString();
	virtual CString GetLine(CKeyword::type kt, StorageBinListItem& item);

	virtual void Edit(CString& rStr);
	virtual void Edit(const CTreeCtrlNode& rKeyNode);

	const std::list<CTreeCtrlNode>& GetDeleteNodes()const
		{ return m_listCopyNodes; }

	virtual ~CDump();

	// Generated message map functions
protected:
	std::list<CTreeCtrlNode> m_listCopyNodes;
	CTreeCtrlNode m_editNode;

	//{{AFX_MSG(CDump)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DUMP_H__)
