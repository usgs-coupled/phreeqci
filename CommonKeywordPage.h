// CommonKeywordPage.h : header file
//
// $Date: 2000/10/04 20:17:38 $
// $Revision: 1.1.1.27 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "mshflexgrid.h"
//}}AFX_INCLUDES
#include "EditGrid.h"
#if !defined(AFX_COMMONKEYWORDPAGE_H__7EC8913D_1FF0_4C6A_BD41_BD266D0A30C3__INCLUDED_)
#define AFX_COMMONKEYWORDPAGE_H__7EC8913D_1FF0_4C6A_BD41_BD266D0A30C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCommonKeywordPage dialog

#include "CommonKeywordSheet.h"

#include "KeywordPage.h"

#ifndef baseCommonKeywordPage
#define baseCommonKeywordPage CKeywordPage
#endif

class CCommonKeywordPage : public baseCommonKeywordPage
{
	DECLARE_DYNCREATE(CCommonKeywordPage)

// Construction
public:
	CCommonKeywordPage();
	CCommonKeywordPage(UINT nIDTemplate, UINT nIDCaption = 0);
	CCommonKeywordPage(LPCTSTR lpszTemplateName, UINT nIDCaption = 0);

	~CCommonKeywordPage();

public:
	CCommonKeywordSheet* GetSheet();

// Dialog Data
	//{{AFX_DATA(CCommonKeywordPage)
	enum { IDD = IDD_COMMON_KEYWORD_PAGE };
	CButton	m_btnInputDesc;
	CEdit	m_eInputDesc;
	CEditGrid	m_egNumDesc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCommonKeywordPage)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitNumDesc();
	void DDX_NumDesc(CDataExchange* pDX, int nIDC);
	// Generated message map functions
	//{{AFX_MSG(CCommonKeywordPage)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDownMshfgNumDesc(short FAR* KeyCode, short Shift);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#ifndef _DEBUG  // debug version in CommonKeywordPage.cpp
inline CCommonKeywordSheet* CCommonKeywordPage::GetSheet()
   { return (CCommonKeywordSheet*)GetParent(); }
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMONKEYWORDPAGE_H__7EC8913D_1FF0_4C6A_BD41_BD266D0A30C3__INCLUDED_)
