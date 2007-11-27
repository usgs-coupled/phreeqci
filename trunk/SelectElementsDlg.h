#if !defined(AFX_SELECTELEMENTSDLG_H__CA79FFD2_1BA2_4E4A_A171_43E06CC2B7FB__INCLUDED_)
#define AFX_SELECTELEMENTSDLG_H__CA79FFD2_1BA2_4E4A_A171_43E06CC2B7FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectElementsDlg.h : header file
//
#include "CheckListCtrl.h"
#include "Database.h"

/////////////////////////////////////////////////////////////////////////////
// CSelectElementsDlg dialog

class CSelectElementsDlg : public CDialog
{
// Construction
public:
	CSelectElementsDlg(const CDatabase& database, CWnd* pParent = NULL);   // standard constructor
	std::list<CString> m_listElements;

// Dialog Data
	//{{AFX_DATA(CSelectElementsDlg)
	enum { IDD = IDD_SELECT_ELEMENTS };
	CListBox	m_ctrlListBox;
	CCheckListCtrl	m_ctrlElements;
	CString	m_strDatabase;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectElementsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	const CDatabase& m_database;
	void UpdateElementList(int nItem);

	// Generated message map functions
	//{{AFX_MSG(CSelectElementsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedElementList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSetfocusListEntries();
	//}}AFX_MSG

	// context menu check list commands
	afx_msg void OnViewChange(UINT uID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTELEMENTSDLG_H__CA79FFD2_1BA2_4E4A_A171_43E06CC2B7FB__INCLUDED_)
