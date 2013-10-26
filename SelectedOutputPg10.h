#pragma once

#include "CommonKeywordPage.h"
//#include "KeywordPage.h"
#include "EditGrid.h"
#include "CheckListCtrl.h"
#include "GridListDoc.h"


#ifndef baseCSelectedOutputPg10
#define baseCSelectedOutputPg10 CCommonKeywordPage
#endif

// calculate values
class CSelectedOutputPg10 : public baseCSelectedOutputPg10
{
	DECLARE_DYNAMIC(CSelectedOutputPg10)

public:
	CSelectedOutputPg10();
	virtual ~CSelectedOutputPg10();

// Dialog Data
#define CMSHFlexGrid CEditGrid
	enum { IDD = IDD_KEY_SELECTED_OUTPUT_PG10 };
	CCheckListCtrl	   clcCalcValues;
	CMSHFlexGrid	   egCalcValues;
	std::list<CString> listCalcValues;
#undef CMSHFlexGrid

public:
	virtual BOOL OnKillActive();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	afx_msg void OnItemchangedClCalcValues(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusClCalcValues(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDownMshfgCalcValues(short FAR* KeyCode, short Shift);

	virtual BOOL OnInitDialog();
	afx_msg void OnEnterCellMshfgCalcValues();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()

protected:
	CGridListDoc glDoc;

	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);	
	afx_msg void OnEnterCellMshfgNumDesc();
	DECLARE_MESSAGE_MAP()
};
