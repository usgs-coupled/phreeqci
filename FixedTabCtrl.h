// FixedTabCtrl.h : header file
//
// $Date: 2000/10/04 20:17:41 $
// $Revision: 1.1.1.25 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIXEDTABCTRL_H__88DAD8C6_F131_4C30_9E80_47907BE3C21B__INCLUDED_)
#define AFX_FIXEDTABCTRL_H__88DAD8C6_F131_4C30_9E80_47907BE3C21B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFixedTabCtrl window

class CFixedTabCtrl : public CTabCtrl
{
// Construction
public:
	CFixedTabCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFixedTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFixedTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFixedTabCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg LRESULT OnNotifyFormat(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIXEDTABCTRL_H__88DAD8C6_F131_4C30_9E80_47907BE3C21B__INCLUDED_)
