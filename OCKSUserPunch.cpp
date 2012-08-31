// OCKSUserPunch.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "phrqtype.h"          // LDBLE
#include "phreeqc.h"           // user_punch user_punch_count_headings user_punch_headings
#include "OCKSUserPunch.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COCKSUserPunch

IMPLEMENT_DYNAMIC(COCKSUserPunch, baseOCKSUserPunch)

COCKSUserPunch::COCKSUserPunch(CWnd* pWndParent)
	 : baseOCKSUserPunch(IDS_PROPSHT_CAPTION17, pWndParent)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
}

COCKSUserPunch::~COCKSUserPunch()
{
}


BEGIN_MESSAGE_MAP(COCKSUserPunch, baseOCKSUserPunch)
	//{{AFX_MSG_MAP(COCKSUserPunch)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COCKSUserPunch message handlers
/////////////////////////////////////////////////////////////////////////////
// COCKSRates message handlers

CString COCKSUserPunch::GetString()
{
	/*
	Line 0: USER_PUNCH
	Line 1:      -headings Na+ Mg+2 Pairs Rxn_increment
	Line 2:      -start
	Basic:  10 REM convert to ppm
	Basic:  20 PUNCH MOL("Na+")* 22.99 * 1000
	Basic:  30 PUNCH MOL("Mg+2")* 24.3 * 1000
	Basic:  40 pairs = MOL("NaCO3-") + MOL("MgCO3")
	Basic:  50 PUNCH pairs
	Basic:  60 REM punch reaction increment
	Basic:  70 PUNCH RXN
	Line 3:      -end
	*/

	CString strLines;
	CString strFormat;

	// Line 0
	strLines = _T("USER_PUNCH");

	// Line 1
	strFormat.Format(_T("%s-headings %s"),
		(LPCTSTR)s_strNewLine,
		(LPCTSTR)m_Page1.m_strHead
		);
	strLines += strFormat;

	// Line 2
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

void COCKSUserPunch::Edit(CString& rStr)
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
