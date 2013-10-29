// KPUserPunchPg1.h : header file
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

#ifndef __KPUSERPUNCHPG1_H__
#define __KPUSERPUNCHPG1_H__

/////////////////////////////////////////////////////////////////////////////
// CKPUserPunchPg1 dialog

#include "CommonKeywordPage.h"

#ifndef baseKPUserPunchPg1
#define baseKPUserPunchPg1 CCommonKeywordPage
#endif

class CKPUserPunchPg1 : public baseKPUserPunchPg1
{
	DECLARE_DYNCREATE(CKPUserPunchPg1)

// Construction
public:
	CKPUserPunchPg1();
	~CKPUserPunchPg1();

// Dialog Data
	//{{AFX_DATA(CKPUserPunchPg1)
	enum { IDD = IDD_KEY_USER_PUNCH_PG1 };
	CEditGrid	m_egBasic;
	CString	m_strHead;
	//}}AFX_DATA

	CBasicDesc2 m_basicDesc;

	std::list<basic_command> m_listCommands;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPUserPunchPg1)
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
	//{{AFX_MSG(CKPUserPunchPg1)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLbFuncs();
	afx_msg void OnSetfocusLbFuncs();
	afx_msg void OnSetfocusLbArgs();
	afx_msg void OnSetfocusEExplan();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBRenumber();
	afx_msg void OnEnterCellMshfgBasic();
	afx_msg void OnEnterCellMshfgNumDesc();
	afx_msg void OnKeyDownMshfgBasic(short FAR* KeyCode, short Shift);
	afx_msg void OnSetfocusEHead();
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



#endif // __KPUSERPUNCHPG1_H__
