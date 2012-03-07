#pragma once

#include <afxctl.h> // for class CPictureHolder
#include "KeywordPage.h"
#include "EditGrid.h"
#include "afxwin.h"
// COMMENT: {11/9/2009 4:28:37 PM}#define PITZER_EXTERNAL extern "C"
// COMMENT: {11/9/2009 4:28:37 PM}#include "phreeqc/src/pitzer.h"
#include "PitzParam.h"
#include "Database.h"

// CKPPitzerAlphas dialog

#ifndef baseCKPPitzerAlphas
#define baseCKPPitzerAlphas CKeywordPage
#endif

class CKPPitzerAlphas : public baseCKPPitzerAlphas
{
	DECLARE_DYNCREATE(CKPPitzerAlphas)

public:
	CKPPitzerAlphas(UINT nIDTemplate = CKPPitzerAlphas::IDD, UINT nIDCaption = 0);
	virtual ~CKPPitzerAlphas();

// Dialog Data
	enum { IDD = IDD_KEY_PITZER_ALPHAS };

protected:
	bool m_bIgnoreChanges;
	HWND m_hWndLastControl;    // last control used
	BOOL m_bEditLastControl;   // last control was an edit item
	CPictureHolder m_pictHolderRightArrow;
	CPictureHolder m_pictHolderStar;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void InitGrid(CDataExchange* pDX, int nIDC);
	void ResetFocus();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditAlpha1();
	afx_msg void OnEnChangeEditAlpha2();

	afx_msg void OnEnKillfocusEditAlpha1();
	afx_msg void OnEnKillfocusEditAlpha2();

	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChange(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnAllowInsertRow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInsertRow(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnAllowDeleteRow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteRow(WPARAM wParam, LPARAM lParam);

	CEdit       m_ctrlAlpha1;
	CEdit       m_ctrlAlpha2;
	CSRCDBPG	m_pager;
	CEditGrid	m_ctrlGrid;
	CEdit	    m_eInputDesc;

	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	void OnRowColChangeGrid();
	void OnEnterCellGrid();
	void OnLeaveCellGrid();
	void OnGotoFirstClicked();
	void OnGotoLastClicked();
	void OnGotoNewClicked();
	void OnGotoNextClicked();
	void OnGotoPrevClicked();
	void OnDblClickGrid();
	void OnKeyDownGrid(short* KeyCode, short Shift);
	CComboBox m_ctrlCation;
	CComboBox m_ctrlAnion;

public:
	std::list<CPitzParam> m_listParams;

	afx_msg void OnCbnEditchangeCbCation();
	afx_msg void OnCbnSelchangeCbCation();
	afx_msg void OnCbnEditchangeCbAnion();
	afx_msg void OnCbnSelchangeCbAnion();
	afx_msg void OnCbnSetfocusCbCation();
	afx_msg void OnCbnSetfocusCbAnion();
	afx_msg void OnEnSetfocusEditAlpha1();
	afx_msg void OnEnSetfocusEditAlpha2();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
