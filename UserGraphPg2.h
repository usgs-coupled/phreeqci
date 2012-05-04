#pragma once

#include "EditGrid.h"
#include "ClipListBox.h"
#include "KeywordPageListItems.h"
#include "BasicDesc2.h"
#include "mshflexgrid.h"

// CUserGraphPg2 dialog

#include "CommonKeywordPage.h"

#ifndef baseUserGraphPg2
#define baseUserGraphPg2 CCommonKeywordPage
#endif

class CUserGraphPg2 : public baseUserGraphPg2
{
	DECLARE_DYNCREATE(CUserGraphPg2)

public:
	CUserGraphPg2();
	virtual ~CUserGraphPg2();

// Dialog Data
	enum { IDD = IDD_KEY_USER_GRAPH_PG2 };
	CEditGrid	m_egBasic;
	CString	    m_strHead;
	bool        m_initial_solutions;
	bool        m_connect_simulations;
	bool        m_detach;
	bool        m_active;

	enum ChartType
	{
		CT_CONC_VS_X,
		CT_CONC_VS_T
	} m_chart_type;

	CBasicDesc2 m_basicDesc;

	std::list<basic_command> m_listCommands;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void DDX_ListCommands(CDataExchange *pDX);
	void DisplayCommands();

// Implementation
protected:
	std::map<CString, CString> m_mapFuncs;
	bool m_bRenumbering;

	virtual BOOL OnInitDialog();

	afx_msg void OnSelchangeLbFuncs();
	afx_msg void OnSetfocusLbFuncs();
	afx_msg void OnSetfocusLbArgs();
	afx_msg void OnSetfocusEExplan();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBRenumber();
	afx_msg void OnEnterCellMshfgBasic();
	afx_msg void OnKeyDownMshfgBasic(short FAR* KeyCode, short Shift);
	afx_msg void OnSetfocusEHead();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnSetfocusTreeArgs(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_EVENTSINK_MAP()

	// custom BN_SETFOCUS notifications
	afx_msg void OnSetfocusBRenumber();

	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};
