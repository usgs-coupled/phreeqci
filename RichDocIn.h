// RichDocIn.h : interface of the CRichDocIn class
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RICHDOCIN_H__7BC19A1F_C662_44EE_87F7_B0EF0857B1B3__INCLUDED_)
#define AFX_RICHDOCIN_H__7BC19A1F_C662_44EE_87F7_B0EF0857B1B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// forward declarations
// class CWorkspaceBar;	
class CTreeCtrlNode;

#include "TreeCtrlIn.h"
#include "Database.h"	// Added by ClassView

class CRichDocIn : public CRichEditDoc
{
// inner class
public:
	class XProperties
	{
	public:
		void SetDefaults(LPCTSTR lpszPathName);
		CString m_strOutPathName;
		CString m_strDBPathName;
		CDocument* m_pDoc;
		XProperties(){m_pDoc = NULL;}
		virtual ~XProperties(){}
	};

protected: // create from serialization only
	CRichDocIn();
	DECLARE_DYNCREATE(CRichDocIn)

// Attributes
public:
	XProperties m_props;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichDocIn)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
	virtual void DeleteContents();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnChangedViewList();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL
	virtual CRichEditCntrItem* CreateClientItem(REOBJECT* preo) const;
	virtual void UpdateFrameCounts();

// Implementation
public:
	const CDatabase& database;
	void UpdateDatabase(int nSimulation);
	virtual ~CRichDocIn();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	friend class CTreeCtrlIn;
	friend class CRunDlg;

// Generated message map functions
protected:

	CDatabase m_database;
	//{{AFX_MSG(CRichDocIn)
	afx_msg void OnRunInput();
	afx_msg void OnUpdateWindowNew(CCmdUI* pCmdUI);
	afx_msg void OnViewProperties();
	afx_msg void OnFileClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RICHDOCIN_H__7BC19A1F_C662_44EE_87F7_B0EF0857B1B3__INCLUDED_)
