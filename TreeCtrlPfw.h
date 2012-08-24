#pragma once


// CTreeCtrlPfw
#include "Tree.h"
#include "TreeCtrlKeyword.h"


#ifndef baseCTreeCtrlPfw
#define baseCTreeCtrlPfw CTreeCtrlEx
#endif

class CTreeCtrlPfw : public baseCTreeCtrlPfw
{
	DECLARE_DYNAMIC(CTreeCtrlPfw)

public:
	CTreeCtrlPfw();
	virtual ~CTreeCtrlPfw();

	//static CString CTreeCtrlPfw::COPY;
	static std::pair<CString, CString> CTreeCtrlPfw::COPY;
	static std::pair<CString, CString> CTreeCtrlPfw::DATABASE;
	static std::pair<CString, CString> CTreeCtrlPfw::END;
	static std::pair<CString, CString> CTreeCtrlPfw::SAVE;
	static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_temp;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_pH;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_pe;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_redox;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_units;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_density;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_isotope;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_water;
	static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_temp;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_pH;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_pe;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_redox;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_units;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_density;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_isotope;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_isotope_uncertainty;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_water;
 
	static std::pair<CString, CString> CTreeCtrlPfw::TITLE;
	static std::pair<CString, CString> CTreeCtrlPfw::USE;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	void SetDescWindow(CWnd *pWnd) { m_pWndDesc = pWnd; };

protected:
	CWnd *m_pWndDesc;
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
};


