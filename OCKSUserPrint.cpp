// OCKSUserPrint.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "phrqtype.h"          // LDBLE
#include "OCKSUserPrint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COCKSUserPrint

IMPLEMENT_DYNAMIC(COCKSUserPrint, baseOCKSUserPrint)

COCKSUserPrint::COCKSUserPrint(CWnd* pWndParent)
	 : baseOCKSUserPrint(IDS_PROPSHT_CAPTION16, pWndParent)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
}

COCKSUserPrint::~COCKSUserPrint()
{
}


BEGIN_MESSAGE_MAP(COCKSUserPrint, baseOCKSUserPrint)
	//{{AFX_MSG_MAP(COCKSUserPrint)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COCKSUserPrint message handlers
/////////////////////////////////////////////////////////////////////////////
// COCKSRates message handlers

CString COCKSUserPrint::GetString()
{
	/*
	Line 0: USER_PRINT
	Line 1:      -start
	Basic:  10 REM convert to ppm
	Basic:  20 PRINT "Sodium:    ", MOL("Na+")* 22.99 * 1000
	Basic:  30 PRINT "Magnesium: ", MOL("Mg+2")* 24.3 * 1000
	Basic:  40 pairs = MOL("NaCO3-") + MOL("MgCO3")
	Basic:  50 PRINT "Pairs (mol/kgw): ", pairs
	Basic:  60 REM print reaction increment
	Basic:  70 PRINT "Rxn incr : ", RXN
	Line 2:      -end
	*/

	CString strLines;
	CString strFormat;

	// Line 0
	strLines = _T("USER_PRINT");

	// Line 1
	strFormat.Format(_T("%s-start"),
		(LPCTSTR)s_strNewLine
		);
	strLines += strFormat;

	// Basic
	std::list<basic_command>::const_iterator command_iter = m_Page1.m_listCommands.begin();
	for ( ; command_iter != m_Page1.m_listCommands.end(); ++command_iter)
	{
		strFormat.Format(_T("%s%d %s"),
			(LPCTSTR)s_strNewLine,
			(*command_iter).nLine,
			(LPCTSTR)(*command_iter).strCommand
			);
		strLines += strFormat;
	}

	// Line 2
	strFormat.Format(_T("%s-end"),
		(LPCTSTR)s_strNewLine
		);
	strLines += strFormat;

	return strLines + s_strNewLine;
}

void COCKSUserPrint::Edit(CString& rStr)
{
	PhreeqcI p(rStr);
	p.GetData(this);
}
