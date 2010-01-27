#pragma once

#include <afxctl.h> // for class CPictureHolder
#include "KeywordPage.h"
#include "EditGrid.h"
#include "afxwin.h"
#include "PitzParam.h"
#include "Database.h"

// CKPPitzerNNV dialog

#ifndef baseCKPPitzerNNV
#define baseCKPPitzerNNV CKeywordPage
#endif

class CKPPitzerNNV : public baseCKPPitzerNNV
{
	DECLARE_DYNCREATE(CKPPitzerNNV)

public:
	CKPPitzerNNV(UINT nIDTemplate = CKPPitzerNNV::IDD, UINT nIDCaption = 0);
	virtual ~CKPPitzerNNV();

// Dialog Data
	enum { IDD = IDD_KEY_PITZER_NNV };

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
	afx_msg void OnEnChangeEditA0();
	afx_msg void OnEnChangeEditA1();
	afx_msg void OnEnChangeEditA2();
	afx_msg void OnEnChangeEditA3();
	afx_msg void OnEnChangeEditA4();
	afx_msg void OnEnChangeEditA5();

	afx_msg void OnEnSetfocusEditA0();
	afx_msg void OnEnSetfocusEditA1();
	afx_msg void OnEnSetfocusEditA2();
	afx_msg void OnEnSetfocusEditA3();
	afx_msg void OnEnSetfocusEditA4();
	afx_msg void OnEnSetfocusEditA5();

	afx_msg void OnEnKillfocusEditA0();
	afx_msg void OnEnKillfocusEditA1();
	afx_msg void OnEnKillfocusEditA2();
	afx_msg void OnEnKillfocusEditA3();
	afx_msg void OnEnKillfocusEditA4();
	afx_msg void OnEnKillfocusEditA5();

	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChange(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnAllowInsertRow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInsertRow(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnAllowDeleteRow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteRow(WPARAM wParam, LPARAM lParam);

	CEdit       m_ctrlA0;
	CEdit       m_ctrlA1;
	CEdit       m_ctrlA2;
	CEdit       m_ctrlA3;
	CEdit       m_ctrlA4;
	CEdit       m_ctrlA5;
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
	CComboBox m_ctrlN1; // IDC_CB_N1 n
	CComboBox m_ctrlN2; // IDC_CB_N2 n
	CComboBox m_ctrlV;  // IDC_CB_V nca

public:
	std::list<CPitzParam> m_listParams;

	afx_msg void OnCbnEditchangeCbN1();
	afx_msg void OnCbnSelchangeCbN1();
	afx_msg void OnCbnEditchangeCbN2();
	afx_msg void OnCbnSelchangeCbN2();
	afx_msg void OnCbnEditchangeCbV();
	afx_msg void OnCbnSelchangeCbV();
	afx_msg void OnCbnKillfocusCbN1();
	afx_msg void OnCbnSetfocusCbN2();
	afx_msg void OnCbnSetfocusCbV();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
