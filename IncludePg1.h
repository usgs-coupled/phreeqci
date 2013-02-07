#pragma once

#include "KeywordPage.h"

// CIncludePg1 dialog

#ifndef baseCIncludePg1
#define baseCIncludePg1 CKeywordPage
#endif

class CIncludePg1 : public baseCIncludePg1
{
	DECLARE_DYNAMIC(CIncludePg1)

public:
	CIncludePg1();
	virtual ~CIncludePg1();

// Dialog Data
	enum { IDD = IDD_KEY_INCLUDE_PG1 };
	CEdit	m_eInputDesc;
	CString	m_strFileName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBrowse();
	afx_msg void OnSetfocusFileName();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnSetfocusBrowse();
	DECLARE_MESSAGE_MAP()
};
