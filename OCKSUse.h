// OCKSUse.h : header file
//
// This class defines custom modal property sheet 
// COCKSUse.
//
// $Date: 2000/10/04 20:17:50 $
// $Revision: 1.1.1.26 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////
 
#ifndef __OCKSUSE_H__
#define __OCKSUSE_H__

#include "KPUsePg1.h"

/////////////////////////////////////////////////////////////////////////////
// COCKSUse

#include "OleCtlKeywordSheet.h"
#include "TreeCtrlIn.h"

#ifndef baseOCKSUse
#define baseOCKSUse COleCtlKeywordSheet
#endif

class COCKSUse : public baseOCKSUse
{
	DECLARE_DYNAMIC(COCKSUse)

// Construction
public:
	COCKSUse(CWnd* pWndParent = NULL, CTreeCtrlNode simNode = CTreeCtrlNode());

// Attributes
public:
	CKPUsePg1 m_Page1;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COCKSUse)
	public:
	virtual int DoModal();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual CString GetString();
	virtual void Edit(CString& rStr);
	virtual void Edit(const CTreeCtrlNode& rKeyNode);

	const std::list<CTreeCtrlNode>& GetDeleteNodes()const
		{ return m_listUseNodes; }

	virtual ~COCKSUse();


// Generated message map functions
protected:
	std::list<CTreeCtrlNode> m_listUseNodes;
	CTreeCtrlNode m_editNode;

	void ParseString(const CString& str);
	//{{AFX_MSG(COCKSUse)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __OCKSUSE_H__
