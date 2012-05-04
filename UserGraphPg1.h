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

// COMMENT: {5/1/2012 8:06:29 PM}	CBasicDesc2 m_basicDesc;

	std::list<basic_command> m_listCommands;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
// COMMENT: {5/1/2012 8:16:00 PM}	void DDX_ListCommands(CDataExchange *pDX);
// COMMENT: {5/1/2012 8:16:24 PM}	void DisplayCommands();

// Implementation
protected:
// COMMENT: {5/1/2012 8:15:10 PM}	std::map<CString, CString> m_mapFuncs;
// COMMENT: {5/1/2012 8:15:10 PM}	bool m_bRenumbering;

	virtual BOOL OnInitDialog();

// COMMENT: {5/1/2012 8:11:39 PM}	afx_msg void OnSelchangeLbFuncs();
// COMMENT: {5/1/2012 8:11:39 PM}	afx_msg void OnSetfocusLbFuncs();
// COMMENT: {5/1/2012 8:11:39 PM}	afx_msg void OnSetfocusLbArgs();
// COMMENT: {5/1/2012 8:11:39 PM}	afx_msg void OnSetfocusEExplan();
	afx_msg void OnSize(UINT nType, int cx, int cy);
// COMMENT: {5/1/2012 8:15:35 PM}	afx_msg void OnBRenumber();
	afx_msg void OnEnterCellMshfgBasic();
	afx_msg void OnKeyDownMshfgBasic(short FAR* KeyCode, short Shift);
	afx_msg void OnSetfocusEHead();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
// COMMENT: {5/1/2012 8:19:34 PM}	afx_msg void OnSetfocusTreeArgs(NMHDR* pNMHDR, LRESULT* pResult);
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
	afx_msg void OnBnClickedChkAutoMinY();
	afx_msg void OnBnClickedChkAutoMaxY();
	afx_msg void OnBnClickedChkAutoMajY();
	afx_msg void OnBnClickedChkAutoMinY2();
	afx_msg void OnBnClickedChkAutoMaxY2();
	afx_msg void OnBnClickedChkAutoMajY2();
};
