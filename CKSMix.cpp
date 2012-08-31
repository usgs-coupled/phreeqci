// CKSMix.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CKSMix.h"

#include "Util.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCKSMix

IMPLEMENT_DYNAMIC(CCKSMix, baseCKSMix)

CCKSMix::CCKSMix(CWnd* pWndParent, CTreeCtrlNode simNode)
	 : baseCKSMix(IDS_PROPSHT_CAPTION7, pWndParent)
	 , m_ranges(simNode, true)
	 , m_strNumFormat(_T("%d"))
{
	std::set<CDBRange> setSolutions;
	m_strNumFormat = CUtil::CreateRange(m_setSolutions, m_ranges[CKeyword::K_SOLUTION]);

	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
}

CCKSMix::~CCKSMix()
{
}


BEGIN_MESSAGE_MAP(CCKSMix, baseCKSMix)
	//{{AFX_MSG_MAP(CCKSMix)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCKSMix message handlers


CString CCKSMix::GetString()
{
	/*
	Line 0:  MIX 2 Mixing solutions 5, 6, and 7.
	Line 1a:      5     1.1
	Line 1b:      6     0.5
	Line 1c:      7     0.3
	*/

	CString strLines;
	CString strFormat;
	
	// Line 0
	strLines = GetLineZero(CKeyword::K_MIX);

	// Line 1
	std::map<int, LDBLE>::const_iterator iter = m_Page1.mixComps.begin();
	for (; iter != m_Page1.mixComps.end(); ++iter)
	{
		strFormat.Format(_T("%s%4c%d%4c%.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			(*iter).first,
			_T(' '),
			DBL_DIG,
			(*iter).second
			);
		strFormat.TrimRight();
		strLines += strFormat;
	}

	return strLines + s_strNewLine;
}

void CCKSMix::Edit(CString& rStr)
{
	ASSERT(std::numeric_limits<double>::has_signaling_NaN == true);

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