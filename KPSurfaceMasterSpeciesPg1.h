// KPSurfaceMasterSpeciesPg1.h : header file
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

#ifndef __KPSURFACEMASTERSPECIESPG1_H__
#define __KPSURFACEMASTERSPECIESPG1_H__

/////////////////////////////////////////////////////////////////////////////
// CKPSurfaceMasterSpeciesPg1 dialog

#ifndef baseCKPSurfaceMasterSpeciesPg1
#define baseCKPSurfaceMasterSpeciesPg1 CKeywordPage
#endif

class CKPSurfaceMasterSpeciesPg1 : public baseCKPSurfaceMasterSpeciesPg1
{
	DECLARE_DYNCREATE(CKPSurfaceMasterSpeciesPg1)

public:
	std::list<CMaster> m_listMaster;

// Construction
public:
	CKPSurfaceMasterSpeciesPg1();
	~CKPSurfaceMasterSpeciesPg1();

// Dialog Data
#define CMSHFlexGrid CEditGrid
#define CSRCDBPG CSRCDBPGCH
	//{{AFX_DATA(CKPSurfaceMasterSpeciesPg1)
	enum { IDD = IDD_KEY_SURFACE_MASTER_SPECIES };
	CEdit	m_ctrlSMasterSpecies;
	CEdit	m_ctrlSurfaceName;
	CEdit	m_eInputDesc;
	CMSHFlexGrid	m_ctrlGrid;
	CSRCDBPG	m_pager;
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
	//{{AFX_VIRTUAL(CKPSurfaceMasterSpeciesPg1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitGrid(CDataExchange* pDX, int nIDC);
	void ResetFocus();

	// Generated message map functions
	//{{AFX_MSG(CKPSurfaceMasterSpeciesPg1)
	afx_msg void OnGotoFirstClicked();
	afx_msg void OnGotoLastClicked();
	afx_msg void OnGotoNextClicked();
	afx_msg void OnGotoNewClicked();
	afx_msg void OnGotoPrevClicked();
	afx_msg void OnRowColChangeGrid();
	afx_msg void OnLeaveCellGrid();
	afx_msg void OnDblClickGrid();
	afx_msg void OnEnterCellGrid();
	afx_msg void OnKeyDownGrid(short FAR* KeyCode, short Shift);
	afx_msg void OnChangeEditSurfaceName();
	afx_msg void OnKillfocusEditExchangeName();
	afx_msg void OnSetfocusEditExchangeName();
	afx_msg void OnChangeEditSurfaceMasterSpecies();
	afx_msg void OnKillfocusEditExchangeMasterSpecies();
	afx_msg void OnSetfocusEditExchangeMasterSpecies();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChange(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

};



#endif // __KPSURFACEMASTERSPECIESPG1_H__
