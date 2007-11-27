// KPSolutionMasterSpeciesPg1.h : header file
//
//{{AFX_INCLUDES()
#include "srcdbpg.h"
#include "mshflexgrid.h"
//}}AFX_INCLUDES

#include "KeywordPage.h"
#include "EditGrid.h"
#include "KeywordPageListItems.h"
#include "SRCDBPGCH.h"
#include <afxctl.h> // for class CPictureHolder

#ifndef __KPSOLUTIONMASTERSPECIESPG1_H__
#define __KPSOLUTIONMASTERSPECIESPG1_H__

/////////////////////////////////////////////////////////////////////////////
// CKPSolutionMasterSpeciesPg1 dialog

#ifndef baseCKPSolutionMasterSpeciesPg1
#define baseCKPSolutionMasterSpeciesPg1 CKeywordPage
#endif

class CKPSolutionMasterSpeciesPg1 : public baseCKPSolutionMasterSpeciesPg1
{
	DECLARE_DYNCREATE(CKPSolutionMasterSpeciesPg1)

public:
	std::list<CMaster> m_listMaster;

// Construction
public:
	CKPSolutionMasterSpeciesPg1();
	~CKPSolutionMasterSpeciesPg1();

// Dialog Data
#define CMSHFlexGrid CEditGrid
#define CSRCDBPG CSRCDBPGCH
	//{{AFX_DATA(CKPSolutionMasterSpeciesPg1)
	enum { IDD = IDD_KEY_SOLUTION_MASTER_SPECIES };
	CEdit	m_eInputDesc;
	CEdit	m_ctrlElementName;
	CEdit	m_ctrlMasterSpecies;
	CEdit	m_ctrlAlkalinity;
	CEdit	m_ctrlFormulaOrGFW;
	CEdit	m_ctrlGFWOfElement;
	CSRCDBPG	m_pager;
	CMSHFlexGrid	m_ctrlGrid;
	//}}AFX_DATA
#undef CSRCDBPG
#undef CMSHFlexGrid

protected:
	bool m_bIgnoreChanges;
	CPictureHolder m_pictHolderRightArrow;
	CPictureHolder m_pictHolderStar;
	HWND m_hWndLastControl;    // last control used
	BOOL m_bEditLastControl;   // last control was an edit item


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPSolutionMasterSpeciesPg1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitGrid(CDataExchange* pDX, int nIDC);
	void ResetFocus();

	// Generated message map functions
	//{{AFX_MSG(CKPSolutionMasterSpeciesPg1)
	afx_msg void OnChangeEditElementName();
	afx_msg void OnKillfocusEditElementName();
	afx_msg void OnSetfocusEditElementName();
	afx_msg void OnChangeEditMasterSpecies();
	afx_msg void OnKillfocusEditMasterSpecies();
	afx_msg void OnSetfocusEditMasterSpecies();
	afx_msg void OnChangeEditAlkalinity();
	afx_msg void OnKillfocusEditAlkalinity();
	afx_msg void OnSetfocusEditAlkalinity();
	afx_msg void OnChangeEditFormulaOrGfw();
	afx_msg void OnKillfocusEditFormulaOrGfw();
	afx_msg void OnSetfocusEditFormulaOrGfw();
	afx_msg void OnChangeEditGfwElement();
	afx_msg void OnKillfocusEditGfwElement();
	afx_msg void OnSetfocusEditGfwElement();
	afx_msg void OnGotoFirstClicked();
	afx_msg void OnGotoLastClicked();
	afx_msg void OnGotoNewClicked();
	afx_msg void OnGotoNextClicked();
	afx_msg void OnGotoPrevClicked();
	afx_msg void OnRowColChangeGrid();
	afx_msg void OnLeaveCellGrid();
	afx_msg void OnDblClickGrid();
	afx_msg void OnEnterCellGrid();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnKeyDownGrid(short FAR* KeyCode, short Shift);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG

	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChange(WPARAM wParam, LPARAM lParam);

	// custom radio notifications
	afx_msg void OnClickedRadio(UINT nID);
	afx_msg void OnSetfocusRadio(UINT nID);
	DECLARE_MESSAGE_MAP()

};



#endif // __KPSOLUTIONMASTERSPECIESPG1_H__
