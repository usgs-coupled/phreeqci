// CheckListCtrl.h : header file
//
// $Date: 2000/10/04 20:17:36 $
// $Revision: 1.1.1.25 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKLISTCTRL_H__E4A10ADE_42FB_45D9_B868_6613ACECE11B__INCLUDED_)
#define AFX_CHECKLISTCTRL_H__E4A10ADE_42FB_45D9_B868_6613ACECE11B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCheckListCtrl window

class CCheckListCtrl : public CListCtrl
{
// Construction
public:
	CCheckListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	int FindSubItem(int nSubItem, LPCTSTR psz)const;
	void SetEnabled(int nItem, BOOL bEnabled);
	int InsertItem(int nItem, LPCTSTR lpszItem);
	int GetCheckBox(int nItem)const;
	void SetCheckBox(int nItem, int nCheckState);

	virtual ~CCheckListCtrl();

	// Generated message map functions
protected:
	CImageList* m_pImageList;
	//{{AFX_MSG(CCheckListCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKLISTCTRL_H__E4A10ADE_42FB_45D9_B868_6613ACECE11B__INCLUDED_)
