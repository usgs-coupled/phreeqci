// CommonKeywordSheet.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMONKEYWORDSHEET_H__3C923E34_73BC_408B_B0EE_15B227209FD5__INCLUDED_)
#define AFX_COMMONKEYWORDSHEET_H__3C923E34_73BC_408B_B0EE_15B227209FD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCommonKeywordSheet

#include "Keyword.h"

#include "OleCtlKeywordSheet.h"

#ifndef baseCommonKeywordSheet
#define baseCommonKeywordSheet COleCtlKeywordSheet
#endif

class CCommonKeywordSheet : public baseCommonKeywordSheet
{
	DECLARE_DYNAMIC(CCommonKeywordSheet)

// Construction
public:
	CCommonKeywordSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CCommonKeywordSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	enum n_userValues
	{
		N_EMPTY = -1,
	};


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommonKeywordSheet)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	CString GetLineZero(enum CKeyword::type nType);
	CString m_strDesc;
	int m_n_user_end;
	int m_n_user;
	virtual ~CCommonKeywordSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCommonKeywordSheet)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMONKEYWORDSHEET_H__3C923E34_73BC_408B_B0EE_15B227209FD5__INCLUDED_)
