// CalculateValues.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CalculateValues.h"

#include "CStringLineParser.h"
#include "Util.h"
#include "KeywordPageListItems.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalculateValues

IMPLEMENT_DYNAMIC(CCalculateValues, baseCalculateValues)

CCalculateValues::CCalculateValues(CWnd* pWndParent)
	 : baseCalculateValues(IDS_PROPSHT_CAPTION12, pWndParent)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
}

CCalculateValues::~CCalculateValues()
{
}


BEGIN_MESSAGE_MAP(CCalculateValues, baseCalculateValues)
	//{{AFX_MSG_MAP(CCalculateValues)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCalculateValues message handlers

CString CCalculateValues::GetString()
{
	/*
	Line 0: CALCULATE_VALUES
	Line 1: R(D)
	Line 2: -start
	Line 3: 10 ratio = -9999.999
	Line 3a: 20 if (TOT("D") <= 0) THEN GOTO 100
	Line 3b: 30 total_D = TOT("D")
	Line 3c: 40 total_H = TOT("H")
	Line 3d: 50 ratio = total_D/total_H
	Line 3e: 100 SAVE ratio
	Line 4: -end
	*/
	CString strLines;
	CString strFormat;

	// Line 0
	strLines = _T("CALCULATE_VALUES");

	// Line 1
	std::list<CCalcValue>::const_iterator rate_iter = m_Page1.m_listCalcValues.begin();
	for ( ; rate_iter != m_Page1.m_listCalcValues.end(); ++rate_iter)
	{
		// Line 1 & 2
		strFormat.Format(_T("%s%4c%s%s-start"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			(LPCTSTR)(*rate_iter).m_strName,
			(LPCTSTR)s_strNewLine
			);
		strLines += strFormat;

		// Basic
		std::list<basic_command>::const_iterator command_iter = (*rate_iter).m_listCommands.begin();
		for ( ; command_iter != (*rate_iter).m_listCommands.end(); ++command_iter)
		{
			strFormat.Format(_T("%s%d %s"),
				(LPCTSTR)s_strNewLine,
				(*command_iter).nLine,
				(LPCTSTR)(*command_iter).strCommand
				);
			strLines += strFormat;
		}

		// Line 4
		strFormat.Format(_T("%s-end"),
			(LPCTSTR)s_strNewLine
			);
		strLines += strFormat;
	}

	return strLines + s_strNewLine;
}

void CCalculateValues::Edit(CString& rStr)
{
	try
	{
		PhreeqcI p(rStr);
		p.GetData(this);
	}
	catch (...)
	{
		CString strResource;
		strResource.LoadString(IDS_EXCEPTION_ACCESS_VIOLATION);
		::MessageBox(NULL, strResource, _T("Unhandled Exception"), MB_OK|MB_ICONERROR);
	}
}
