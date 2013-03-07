#pragma once


// CRunCellsPg1 dialog

#include "KeywordPage.h"
#include "runner.h"

#ifndef baseRunCellsPg1
#define baseRunCellsPg1 CKeywordPage
#endif

class CRunCellsPg1 : public baseRunCellsPg1
{
	DECLARE_DYNAMIC(CRunCellsPg1)

public:
	CRunCellsPg1();
	virtual ~CRunCellsPg1();

// Dialog Data
	enum { IDD = IDD_KEY_RUN_CELLS_PG1 };

	CComboBox	m_cboInitTimeUnits;
	CComboBox	m_cboTimeStepUnits;
	CEdit	    m_eInputDesc;

	enum TimeUnit 
	{
		TU_SECONDS,
		TU_MINUTES,
		TU_HOURS,
		TU_DAYS,
		TU_YEARS
	} m_tuTimeStep, m_tuInitTime;

	runner Runner;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnEnSetfocusEditCells();
	afx_msg void OnEnSetfocusEditStime();
	afx_msg void OnEnSetfocusEditTimest();
	afx_msg void OnCbnSetfocusComboStime();
	afx_msg void OnCbnSetfocusComboTimest();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
