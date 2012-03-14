// CKPMixPg1.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "mshflexgrid.h"
//}}AFX_INCLUDES

#ifndef __CKPMIXPG1_H__
#define __CKPMIXPG1_H__

/////////////////////////////////////////////////////////////////////////////
// CCKPMixPg1 dialog

#include "CheckListCtrl.h"
#include "EditGrid.h"
#include "GridListDoc.h"
#include "KeywordPageListItems.h"
#include "DefinedRanges.h"

#include "CommonKeywordPage.h"

#ifndef baseCKPMixPg1
#define baseCKPMixPg1 CCommonKeywordPage
#endif

class CCKPMixPg1 : public baseCKPMixPg1
{
	DECLARE_DYNCREATE(CCKPMixPg1)
	DECLARE_LAYOUT();

// Construction
public:
	CCKPMixPg1();
	~CCKPMixPg1();

// Dialog Data
	//{{AFX_DATA(CCKPMixPg1)
	enum { IDD = IDD_KEY_MIX_PG1 };
	CCheckListCtrl	m_ctrlSolCheckList;
	CEditGrid	m_ctrlMixEditGrid;
	//}}AFX_DATA
public:
	std::map<int, LDBLE> mixComps;

protected:
	CGridListDoc m_glDoc;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCKPMixPg1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	void DDX_MixCompList(CDataExchange* pDX, int nIDC);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCKPMixPg1)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedClSols(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDownMshfgMix(short FAR* KeyCode, short Shift);
	afx_msg void OnSetfocusClSols(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnterCellMshfgNumDesc();
	afx_msg void OnEnterCellMshfgMix();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG

	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

};



#endif // __CKPMIXPG1_H__
