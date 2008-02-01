// CKPEquilibrium_PhasesPg1.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "mshflexgrid.h"
//}}AFX_INCLUDES

#ifndef __CKPEQUILIBRIUM5F_PHASESPG1_H__
#define __CKPEQUILIBRIUM5F_PHASESPG1_H__

/////////////////////////////////////////////////////////////////////////////
// CCKPEquilibrium_PhasesPg1 dialog

#include "CheckListCtrl.h"
#include "EditGrid.h"
#include "GridListDoc.h"
#include "KeywordPageListItems.h"

#include "CommonKeywordPage.h"

#ifndef baseCKPEquilibrium_PhasesPg1
#define baseCKPEquilibrium_PhasesPg1 CCommonKeywordPage
#endif

class CCKPEquilibrium_PhasesPg1 : public baseCKPEquilibrium_PhasesPg1
{
	DECLARE_DYNCREATE(CCKPEquilibrium_PhasesPg1)
	DECLARE_LAYOUT();

// Construction
public:
	std::list<CPurePhase> m_listPurePhase;
	CCKPEquilibrium_PhasesPg1();
	~CCKPEquilibrium_PhasesPg1();


// Dialog Data
	//{{AFX_DATA(CCKPEquilibrium_PhasesPg1)
	enum { IDD = IDD_KEY_EQUILIBRIUM_PHASES_PG1 };
	CCheckListCtrl	m_ctrlPhaseCheckList;
	CEditGrid	m_ctrlPhaseEditGrid;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCKPEquilibrium_PhasesPg1)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CGridListDoc m_glDoc;
// COMMENT: {8/28/2001 1:28:54 PM}	bool m_bComboFilledOnce;
	BOOL InitPhaseCheckList();
	BOOL InitPhaseEditGrid();
	void DDX_PurePhaseList(CDataExchange *pDX, int nIDC);

	// Generated message map functions
	//{{AFX_MSG(CCKPEquilibrium_PhasesPg1)
	virtual BOOL OnInitDialog();
	afx_msg void OnEnterCellMshfgNumDesc();
	afx_msg void OnEnterCellMshfgPhase();
	afx_msg void OnSetfocusClPhase(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnItemchangedClPhase(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDownMshfgPhase(short FAR* KeyCode, short Shift);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG

	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);

	afx_msg void OnViewChange(UINT uID);


	DECLARE_MESSAGE_MAP()

};



#endif // __CKPEQUILIBRIUM5F_PHASESPG1_H__
