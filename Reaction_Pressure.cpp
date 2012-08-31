// Reaction_Temperature.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "Reaction_Pressure.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReaction_Pressure

IMPLEMENT_DYNAMIC(CReaction_Pressure, baseCReaction_Pressure)

CReaction_Pressure::CReaction_Pressure(CWnd* pWndParent)
	 : baseCReaction_Pressure(IDS_PROPSHT_CAPTION35, pWndParent)
{
	m_bNeedDatabase = false;
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
}

CReaction_Pressure::~CReaction_Pressure()
{
}


BEGIN_MESSAGE_MAP(CReaction_Pressure, baseCReaction_Pressure)
	//{{AFX_MSG_MAP(CReaction_Pressure)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CReaction_Pressure message handlers

CString CReaction_Pressure::GetString()
{
	/*
	Line 0: REACTION_PRESSURE 1 Three explicit reaction pressures.
	Line 1:     1.0     250.5     500.0

	or

	Line 0: REACTION_PRESSURE 1 Three implicit reaction pressures.
	Line 1:      1.0     500.0 in 3 steps
	*/

	CString strLines;
	
	// Line 0
	strLines = GetLineZero(CKeyword::K_REACTION_PRESSURE);

	CString strFormat;

	// Line 1
	if (m_Page1.m_nType == CReaction_PressurePg1::TYPE_LINEAR)
	{
		// implicit
		strFormat.Format(_T("%s%4c%.*g %.*g in %d steps"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			DBL_DIG,
			m_Page1.m_dPress1,
			DBL_DIG,
			m_Page1.m_dPress2,
			m_Page1.m_nLinearSteps
			);
		strFormat.TrimRight();
		strLines += strFormat;
	}
	else
	{
		// explicit
		CString strNum;
		std::list<double>::const_iterator iter = m_Page1.m_listSteps.begin();
		strFormat.Format(_T("\n%4c"),
			_T(' ')
			);
		for (int i = 1; iter != m_Page1.m_listSteps.end(); ++iter, ++i)
		{
			if (i % 8 == 0)
			{
				strLines += strFormat;
				strFormat.Format(_T("%s%4c"),
					(LPCTSTR)s_strNewLine,
					_T(' ')
					);
			}
			strNum.Format(_T("%.*g  "),
				DBL_DIG,
				(*iter)
				);
			strFormat += strNum;
		}
		strFormat.TrimRight();
		strLines += strFormat;
	}

	return strLines + s_strNewLine;
}

void CReaction_Pressure::Edit(CString& rStr)
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
