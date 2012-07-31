#pragma once

#include "EditGrid.h"
#include "ClipListBox.h"
#include "KeywordPageListItems.h"
#include "BasicDesc2.h"
#include "mshflexgrid.h"

// CUserGraphPg1 dialog

#include "CommonKeywordPage.h"

#ifndef baseUserGraphPg1
#define baseUserGraphPg1 CCommonKeywordPage
#endif

class CUserGraphPg1 : public baseUserGraphPg1
{
	DECLARE_DYNCREATE(CUserGraphPg1)

public:
	CUserGraphPg1();
	virtual ~CUserGraphPg1();

// Dialog Data
	enum { IDD = IDD_KEY_USER_GRAPH_PG1 };
	CEditGrid	m_egBasic;
	CString	    m_strHeadings;

	CString	    axis_title_x;
	CString	    axis_title_y;
	CString	    axis_title_y2;

	CString	    chart_title;

	// x-axis
	bool        auto_min_x;
	double      min_x;

	bool        auto_max_x;
	double      max_x;

	bool        auto_maj_x;
	double      maj_x;

	bool        auto_minor_x;
	double      minor_x;

	bool        log_x;

	// y-axis
	bool        auto_min_y;
	double      min_y;

	bool        auto_max_y;
	double      max_y;

	bool        auto_maj_y;
	double      maj_y;

	bool        auto_minor_y;
	double      minor_y;

	bool        log_y;

	// y2-axis
	bool        auto_min_y2;
	double      min_y2;

	bool        auto_max_y2;
	double      max_y2;

	bool        auto_maj_y2;
	double      maj_y2;

	bool        auto_minor_y2;
	double      minor_y2;

	bool        log_y2;

	bool        m_initial_solutions;
	bool        m_connect_simulations;
	bool        m_detach;
	bool        m_active;

	enum ChartType
	{
		CT_CONC_VS_X,
		CT_CONC_VS_T
	} m_chart_type;

	std::list<basic_command> m_listCommands;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:

	virtual BOOL OnInitDialog();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEnterCellMshfgNumDesc();
	afx_msg void OnSetfocusEHead();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()

	// custom BN_SETFOCUS notifications
	afx_msg void OnSetfocusBRenumber();

	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckDetach();

	afx_msg void OnBnClickedChkAutoMinX();
	afx_msg void OnBnClickedChkAutoMaxX();
	afx_msg void OnBnClickedChkAutoMajX();
	afx_msg void OnBnClickedChkAutoMinorX();

	afx_msg void OnBnClickedChkAutoMinY();
	afx_msg void OnBnClickedChkAutoMaxY();
	afx_msg void OnBnClickedChkAutoMajY();
	afx_msg void OnBnClickedChkAutoMinorY();

	afx_msg void OnBnClickedChkAutoMinY2();
	afx_msg void OnBnClickedChkAutoMaxY2();
	afx_msg void OnBnClickedChkAutoMajY2();
	afx_msg void OnBnClickedChkAutoMinorY2();

	afx_msg void OnBnSetfocusCheckDetach();
	afx_msg void OnEnSetfocusEditCt();
	afx_msg void OnBnSetfocusRadioConc();

	afx_msg void OnBnSetfocusChkAutoMinX();
	afx_msg void OnBnSetfocusChkAutoMinY();
	afx_msg void OnBnSetfocusChkAutoMinY2();

	afx_msg void OnBnSetfocusChkAutoMaxX();
	afx_msg void OnBnSetfocusChkAutoMaxY();
	afx_msg void OnBnSetfocusChkAutoMaxY2();

	afx_msg void OnBnSetfocusChkAutoMajX();
	afx_msg void OnBnSetfocusChkAutoMajY();
	afx_msg void OnBnSetfocusChkAutoMajY2();

	afx_msg void OnBnSetfocusChkAutoMinorX();
	afx_msg void OnBnSetfocusChkAutoMinorY();
	afx_msg void OnBnSetfocusChkAutoMinorY2();

	afx_msg void OnEnSetfocusEditMinX();
	afx_msg void OnEnSetfocusEditMinY();
	afx_msg void OnEnSetfocusEditMinY2();

	afx_msg void OnEnSetfocusEditMaxX();
	afx_msg void OnEnSetfocusEditMaxY();
	afx_msg void OnEnSetfocusEditMaxY2();

	afx_msg void OnEnSetfocusEditMajX();
	afx_msg void OnEnSetfocusEditMajY();
	afx_msg void OnEnSetfocusEditMajY2();
	
	afx_msg void OnEnSetfocusEditMinorX();
	afx_msg void OnEnSetfocusEditMinorY();
	afx_msg void OnEnSetfocusEditMinorY2();

	afx_msg void OnBnSetfocusChkLogX();
	afx_msg void OnBnSetfocusChkLogY();
	afx_msg void OnBnSetfocusChkLogY2();
	afx_msg void OnBnSetfocusCheckInitSolns();
	afx_msg void OnBnSetfocusCheckConnectSims();
	afx_msg void OnBnSetfocusCheckActive();

	afx_msg void OnEnSetfocusEditXat();
	afx_msg void OnEnSetfocusEditYat();
	afx_msg void OnEnSetfocusEditY2at();
};
