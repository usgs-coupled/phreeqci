// KeywordSheet.h : header file
//
// $Date: 2004/10/23 01:35:36 $
// $Revision: 1.2 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYWORDSHEET_H__7476D9BF_8EA8_48C1_B3DA_BA3F8AAD5686__INCLUDED_)
#define AFX_KEYWORDSHEET_H__7476D9BF_8EA8_48C1_B3DA_BA3F8AAD5686__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CKeywordSheet

#include "AccelPropertySheet.h"
#include "Database.h"
#include "Util.h"
#include <Htmlhelp.h>

// forward declaration
class CRichDocIn;

#ifndef baseCKeywordSheet
#define baseCKeywordSheet CAccelPropertySheet
#endif

class CKeywordSheet : public baseCKeywordSheet
{
	DECLARE_DYNAMIC(CKeywordSheet)

// Construction
public:
	CKeywordSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CKeywordSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	const bool& bNeedDatabase;
	const bool& bContextHelp;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeywordSheet)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	static CString s_strNewLine;
	virtual CString GetString();
	virtual void Edit(CString& rStr);
	virtual ~CKeywordSheet();

	// Generated message map functions
protected:
	bool m_bNeedDatabase;
	bool m_bContextHelp;
	CRichDocIn* GetActiveDocument()const;
	const CDatabase& GetDatabase()const;
	//{{AFX_MSG(CKeywordSheet)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYWORDSHEET_H__7476D9BF_8EA8_48C1_B3DA_BA3F8AAD5686__INCLUDED_)
