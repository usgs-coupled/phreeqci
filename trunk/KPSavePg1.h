// KPSavePg1.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "mshflexgrid.h"
//}}AFX_INCLUDES

#include "EditGrid.h"
#include "Tree.h"
#include "Keyword.h"
#include "DefinedRanges.h"

#ifndef __KPSAVEPG1_H__
#define __KPSAVEPG1_H__

/////////////////////////////////////////////////////////////////////////////
// CKPSavePg1 dialog

#include "KeywordPage.h"

#ifndef baseKPSavePg1
#define baseKPSavePg1 CKeywordPage
#endif

class CKPSavePg1 : public baseKPSavePg1
{
	DECLARE_DYNCREATE(CKPSavePg1)
	DECLARE_LAYOUT();

// Construction
public:
	CKPSavePg1();
	~CKPSavePg1();

public:

	enum RowIndex
	{
		FIRST_ROW                = 0,
		EQUILIBRIUM_PHASES_ROW   = 0,
		EXCHANGE_ROW             = 1,
		GAS_PHASE_ROW            = 2,
		SOLID_SOLUTIONS_ROW      = 3,
		SOLUTION_ROW             = 4,
		SURFACE_ROW              = 5,
		LAST_ROW                 = 6,
	};

	enum Values
	{
		N_EMPTY = -1,
	};


	static const CKeyword::type s_arrKeyTypes[LAST_ROW];
	CRange m_arrRange[LAST_ROW];

// Dialog Data
	//{{AFX_DATA(CKPSavePg1)
	enum { IDD = IDD_KEY_SAVE_PG1 };
	CButton	m_btnInputDesc;
	CEdit	m_eInputDesc;
	CEditGrid	m_egSave;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPSavePg1)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void DDX_Save(CDataExchange *pDX, int nIDC);
	void InitSave(CDataExchange *pDX, int nIDC);

	// Generated message map functions
	//{{AFX_MSG(CKPSavePg1)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnEnterCellMshfgSave();
	afx_msg void OnKeyDownMshfgSave(short FAR* KeyCode, short Shift);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG

	// specialized grid notifications
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

};



#endif // __KPSAVEPG1_H__
