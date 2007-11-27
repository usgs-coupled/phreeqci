// KPUserPrintPg1.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "EditGrid.h"
#include "ClipListBox.h"
#include "KeywordPageListItems.h"
#include "BasicDesc2.h"
//{{AFX_INCLUDES()
#include "mshflexgrid.h"
//}}AFX_INCLUDES

#ifndef __KPUSERPRINTPG1_H__
#define __KPUSERPRINTPG1_H__

/////////////////////////////////////////////////////////////////////////////
// CKPUserPrintPg1 dialog

#include "KeywordPage.h"

#ifndef baseKPUserPrintPg1
#define baseKPUserPrintPg1 CKeywordPage
#endif

class CKPUserPrintPg1 : public baseKPUserPrintPg1
{
	DECLARE_DYNCREATE(CKPUserPrintPg1)

// Construction
public:
	CKPUserPrintPg1();
	~CKPUserPrintPg1();

// Dialog Data
	//{{AFX_DATA(CKPUserPrintPg1)
	enum { IDD = IDD_KEY_USER_PRINT_PG1 };
	CEdit	m_eInputDesc;
	CEditGrid	m_egBasic;
	//}}AFX_DATA

	CBasicDesc2 m_basicDesc;

	std::list<basic_command> m_listCommands;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPUserPrintPg1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	void DDX_ListCommands(CDataExchange *pDX);
	void DisplayCommands();

// Implementation
protected:
	std::map<CString, CString> m_mapFuncs;
	bool m_bRenumbering;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	// Generated message map functions
	//{{AFX_MSG(CKPUserPrintPg1)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLbFuncs();
	afx_msg void OnSetfocusLbFuncs();
	afx_msg void OnSetfocusLbArgs();
	afx_msg void OnSetfocusEExplan();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBRenumber();
	afx_msg void OnEnterCellMshfgBasic();
	afx_msg void OnKeyDownMshfgBasic(short FAR* KeyCode, short Shift);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnSetfocusTreeArgs(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	// custom BN_SETFOCUS notifications
	afx_msg void OnSetfocusBRenumber();

	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

};



#endif // __KPUSERPRINTPG1_H__
