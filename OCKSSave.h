// OCKSSave.h : header file
//
// This class defines custom modal property sheet 
// COCKSSave.
//
// $Date: 2000/10/04 20:17:50 $
// $Revision: 1.1.1.25 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////
 
#ifndef __OCKSSAVE_H__
#define __OCKSSAVE_H__

#include "KPSavePg1.h"

/////////////////////////////////////////////////////////////////////////////
// COCKSSave

#include "OleCtlKeywordSheet.h"
#include "TreeCtrlIn.h"

#ifndef baseOCKSSave
#define baseOCKSSave COleCtlKeywordSheet
#endif

class COCKSSave : public baseOCKSSave
{
	DECLARE_DYNAMIC(COCKSSave)

// Construction
public:
	COCKSSave(CWnd* pWndParent = NULL, CTreeCtrlNode simNode = CTreeCtrlNode());

// Attributes
public:
	CKPSavePg1 m_Page1;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COCKSSave)
	public:
	virtual int DoModal();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual CString GetString();
	virtual void Edit(CString& rStr);
	virtual void Edit(const CTreeCtrlNode& rNode);

	const std::list<CTreeCtrlNode>& GetDeleteNodes()const
		{ return m_listSaveNodes; }

	virtual ~COCKSSave();

// Generated message map functions
protected:
	std::list<CTreeCtrlNode> m_listSaveNodes;
	CTreeCtrlNode m_editNode;

	void ParseString(CString strKey);

	//{{AFX_MSG(COCKSSave)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __OCKSSAVE_H__
