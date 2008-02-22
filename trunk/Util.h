// Util.h: interface for the CUtil class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UTIL_H__8FE16B28_5C7A_4FAC_B1BE_43C7C66B4527__INCLUDED_)
#define AFX_UTIL_H__8FE16B28_5C7A_4FAC_B1BE_43C7C66B4527__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Keyword.h"

// forward declarations
class CMainFrame;
class CStatusBar;
class CRange;
class CDatabase;
class CCheckListCtrl;
class CTreeCtrlNode;

class CUtil  
{
public:
	static bool FileExists(LPCTSTR lpszPathName);
	static CMainFrame& GetMainFrame();
	static CStatusBar& GetStatusBar();
	static CString SetFileExtension(LPCTSTR lpszPathName, LPCTSTR lpszExt);
	static bool IsFileWriteable(LPCTSTR szPath);
	static CString GetTitleWithExt(LPCTSTR lpszPathName);
	static bool IsFileOutput(LPCTSTR lpszPathName);
	static bool IsFileBinary(LPCTSTR lpszPathName);
	static CString CreateRange(std::set<CDBRange>& rDBObjectSet, const std::set<CRange>& rRangeSet);

	// IsValid Routines
private:
	static bool IsValidName(const CString& strFormula);
	static CString s_strLastError;
public:
	static bool IsValidExchangerName(const CString& strFormula);
	static bool IsValidSurfaceName(const CString& strFormula);
	static bool IsValidElementName(const CString& strFormula);
	static bool IsValidMasterSpeciesName(const CString& strFormula);

	static bool IsValidRedoxCouple(const CString& strCouple);
	static CString GetLastErrorString();

	static CDC* PrepareDC(CWnd* pWnd);
	static CComboBox* PrepareCombo(HWND hWndCombo);

	// combo routines
	static int InsertAqElements(HWND hWndCombo, const CDatabase& rDatabase);
	static int InsertAqSpecies(HWND hWndCombo, const CDatabase& rDatabase);
	static int InsertConcUnits(HWND hWndCombo, bool bAlk = false);
	static int InsertExchangeSites(HWND hWndCombo, const CDatabase& rDatabase);
	static int InsertExchangeSpecies(HWND hWndCombo, const CDatabase& rDatabase);
	static int InsertGases(HWND hWndCombo, const CDatabase& rDatabase);
	static int InsertIsotopes(HWND hWndCombo);
	static int InsertPhases(HWND hWndCombo, const CDatabase& rDatabase);
	static int InsertPhasesExchangeSpecies(HWND hWndCombo, const CDatabase& rDatabase);
	static int InsertRange(HWND hWndCombo, std::set<CDBRange>& rNumSet);
	static int InsertRates(HWND hWndCombo, const CDatabase& rDatabase);
	static int InsertRedox(HWND hWndCombo, const CDatabase& rDatabase);
	static int InsertSolidSolutions(HWND hWndCombo, const CDatabase& rDatabase);
	static int InsertTotals(HWND hWndCombo, const CDatabase& rDatabase);


	// check list control routines
	static void InsertAqElements(CCheckListCtrl* pCLC, const CDatabase& rDatabase);
	static void InsertAqSpecies(CCheckListCtrl* pCLC, const CDatabase& rDatabase);
	static void InsertGases(CCheckListCtrl* pCLC, const CDatabase& rDatabase);
	static void InsertIsotopes(CCheckListCtrl* pCLC);
	static void InsertPhases(CCheckListCtrl* pCLC, const CDatabase& rDatabase);
	static void InsertPhasesExchangeSpecies(CCheckListCtrl* pCLC, const CDatabase& rDatabase);
	static void InsertRange(CCheckListCtrl* pCLC, std::set<CDBRange>& rNumSet);
	static void InsertRates(CCheckListCtrl* pCLC, const CDatabase& rDatabase);
	static void InsertSolidSolutions(CCheckListCtrl* pCLC, const CDatabase& rDatabase);
	static void InsertTotals(CCheckListCtrl* pCLC, const CDatabase& rDatabase);

	// list box routines
	static void InsertAqElements(CListBox* pLB, const CDatabase& rDatabase);
	static void InsertAqSpecies(CListBox* pLB, const CDatabase& rDatabase);
	static void InsertGases(CListBox* pLB, const CDatabase& rDatabase);
	static void InsertPhases(CListBox* pLB, const CDatabase& rDatabase);
	static void InsertRates(CListBox* pLB, const CDatabase& rDatabase);
	static void InsertSolidSolutionComps(CListBox* pLB, const CDatabase& rDatabase);
	static void InsertSolidSolutions(CListBox* pLB, const CDatabase& rDatabase);

	// tree view routines
	static void InsertAqSpecies(CTreeCtrl* pTree, const CDatabase& rDatabase, HTREEITEM hParent = TVI_ROOT);
	static void InsertPhases(CTreeCtrl* pTree, const CDatabase& rDatabase, HTREEITEM hParent = TVI_ROOT);
	static void InsertGases(CTreeCtrl* pTree, const CDatabase& rDatabase, HTREEITEM hParent = TVI_ROOT);
	static void InsertAqElements(CTreeCtrl* pTree, const CDatabase& rDatabase, HTREEITEM hParent = TVI_ROOT);
	static void InsertRates(CTreeCtrl* pTree, const CDatabase& rDatabase, HTREEITEM hParent = TVI_ROOT);
	static void InsertSolidSolutions(CTreeCtrl* pTree, const CDatabase& rDatabase, HTREEITEM hParent = TVI_ROOT);
	static void InsertSolidSolutionComps(CTreeCtrl* pTree, const CDatabase& rDatabase, HTREEITEM hParent = TVI_ROOT);
	static void InsertNamedExp(CTreeCtrl* pTree, const CDatabase& rDatabase, HTREEITEM hParent = TVI_ROOT);
	static void InsertCalcVal(CTreeCtrl* pTree, const CDatabase& rDatabase, HTREEITEM hParent = TVI_ROOT);	

};

#endif // !defined(AFX_UTIL_H__8FE16B28_5C7A_4FAC_B1BE_43C7C66B4527__INCLUDED_)
