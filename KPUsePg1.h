// KPUsePg1.h : header file
//
// $Date: 2001/05/12 02:25:26 $
// $Revision: 1.1.1.25 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "mshflexgrid.h"
//}}AFX_INCLUDES

#include "EditGrid.h"
#include "Tree.h"
#include "DefinedRanges.h"
#include "Keyword.h"

#ifndef __KPUSEPG1_H__
#define __KPUSEPG1_H__

/////////////////////////////////////////////////////////////////////////////
// CKPUsePg1 dialog

#include "KeywordPage.h"

#ifndef baseKPUsePg1
#define baseKPUsePg1 CKeywordPage
#endif

class CKPUsePg1 : public baseKPUsePg1
{
	DECLARE_DYNCREATE(CKPUsePg1)
	DECLARE_LAYOUT();

// Construction
public:
	enum RowIndex
	{
		FIRST_ROW                =  0,
		EQUILIBRIUM_PHASES_ROW   =  0,
		EXCHANGE_ROW             =  1,
		GAS_PHASE_ROW            =  2,
		KINETICS_ROW             =  3,
		MIX_ROW                  =  4,
		REACTION_ROW             =  5,
		REACTION_TEMPERATURE_ROW =  6,
		SOLID_SOLUTIONS_ROW      =  7,
		SOLUTION_ROW             =  8,
		SURFACE_ROW              =  9,
		LAST_ROW                 = 10,
	};

	enum Values
	{
		N_EMPTY = -1,
		N_NONE  = -2,
	};

	int m_arrNums[LAST_ROW];

	static const CString s_arrStrKeys[];

	static const CKeyword::type s_arrKeyTypes[LAST_ROW];

	CKPUsePg1(CTreeCtrlNode simNode = CTreeCtrlNode());
	~CKPUsePg1();

// Dialog Data
	//{{AFX_DATA(CKPUsePg1)
	enum { IDD = IDD_KEY_USE_PG1 };
	CEdit	m_eDesc;
	CEditGrid	m_egUse;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPUsePg1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitGrid();
	void DDX_Grid(CDataExchange* pDX);
	void ValidateGrid(CDataExchange* pDX);
	void ExchangeGrid(CDataExchange* pDX);
	void SetRowValue(enum RowIndex ri, int& value);

	CDefinedRanges m_ranges;

	std::set<CDBRange, std::less<CDBRange> > m_arrRangeSet[LAST_ROW];

	// Generated message map functions
	//{{AFX_MSG(CKPUsePg1)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEnterCellMshfgUse();
	afx_msg void OnKeyDownMshfgUse(short FAR* KeyCode, short Shift);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG

	// specialized grid notifications
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	// afx_msg LRESULT OnSetfocusEG(WPARAM wParam, LPARAM lParam);
	// afx_msg LRESULT OnKillfocusEG(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

#endif // __KPUSEPG1_H__
