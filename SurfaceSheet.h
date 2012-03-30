#pragma once

#include "SurfacePage.h"

// CSurfaceSheet

#include "CommonKeywordSheet.h"
#include "DefinedRanges.h"

#ifndef baseCSurfaceSheet
#define baseCSurfaceSheet CCommonKeywordSheet
#endif

class CSurfaceSheet : public baseCSurfaceSheet
{
	DECLARE_DYNAMIC(CSurfaceSheet)

public:
// COMMENT: {12/22/2009 6:18:06 PM}	CSurfaceSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
// COMMENT: {12/22/2009 6:18:06 PM}	CSurfaceSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSurfaceSheet(CWnd* pWndParent = NULL, CTreeCtrlNode simNode = CTreeCtrlNode());
	virtual ~CSurfaceSheet();

	virtual CString GetString();
	virtual void Edit(CString& rStr);

// COMMENT: {12/28/2009 9:22:47 PM}	enum EquilSolutionNum
// COMMENT: {12/28/2009 9:22:47 PM}	{
// COMMENT: {12/28/2009 9:22:47 PM}		N_NONE = -1
// COMMENT: {12/28/2009 9:22:47 PM}	};
	std::set<CDBRange>  SetSolutions;
	CString             SolutionNumberFormat;
// COMMENT: {12/28/2009 9:22:50 PM}	int                 EquilSolutionNum;

	CDefinedRanges  DefinedRanges;

protected:
	DECLARE_MESSAGE_MAP()

public:
	CSurfacePage    Page;

};


