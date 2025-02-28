// OCKSAdvection.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "OCKSAdvection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define DAYS_PER_YEAR 365
#define HOURS_PER_DAY 24
#define MINS_PER_HOUR 60
#define SECS_PER_MIN 60


/////////////////////////////////////////////////////////////////////////////
// COCKSAdvection

IMPLEMENT_DYNAMIC(COCKSAdvection, baseCOCKSAdvection)

COCKSAdvection::COCKSAdvection(CWnd* pWndParent)
	 : baseCOCKSAdvection(IDS_PROPSHT_CAPTION23, pWndParent)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
	AddPage(&m_Page2);
}

COCKSAdvection::~COCKSAdvection()
{
}


BEGIN_MESSAGE_MAP(COCKSAdvection, baseCOCKSAdvection)
	//{{AFX_MSG_MAP(COCKSAdvection)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COCKSAdvection message handlers

CString COCKSAdvection::GetString()
{
	/*
	Line 0: ADVECTION
	Line 1:      -cells 5
	Line 2:      -shifts 25
	Line 3:      -time_step 3.15e7 # seconds = 1 yr.
	Line 4:      -initial_time 1000
	Line 5:      -print_cells 1-3 5
	Line 6:      -print_frequency 5
	Line 7:      -punch_cells 2-5
	Line 8:      -punch_frequency 5
	Line 9:      -warnings false
	*/

	// Line 0
	CString strLines = _T("ADVECTION");
	CString strFormat;

	// Line 1
	if (m_Page1.m_nCells > 0)
	{
		strFormat.Format(_T("%s%4c-cells %d"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			m_Page1.m_nCells
			);
		strLines += strFormat;
	}

	// Line 2
	if (m_Page1.m_nShifts > 0)
	{
		strFormat.Format(_T("%s%4c-shifts %d"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			m_Page1.m_nShifts
			);
		strLines += strFormat;
	}

	// Line 3:      -time_step 3.15e7 # seconds = 1 yr.
	if (m_Page1.m_dTimeStep > 0)
	{
		double dTime = 0;
		char* pszUnits = 0;

		switch (m_Page1.m_tuTimeStep)
		{
		case CKPAdvectionPg1::TU_YEARS :
			dTime = m_Page1.m_dTimeStep * DAYS_PER_YEAR * HOURS_PER_DAY * MINS_PER_HOUR * SECS_PER_MIN;
			if (m_Page1.m_dTimeStep > 1.0)
			{
				pszUnits = _T("years");
			}
			else
			{
				pszUnits = _T("year");
			}
			break;
		case CKPAdvectionPg1::TU_DAYS :
			dTime = m_Page1.m_dTimeStep * HOURS_PER_DAY * MINS_PER_HOUR * SECS_PER_MIN;
			if (m_Page1.m_dTimeStep > 1.0)
			{
				pszUnits = _T("days");
			}
			else
			{
				pszUnits = _T("day");
			}
			break;
		case CKPAdvectionPg1::TU_HOURS :
			dTime = m_Page1.m_dTimeStep * MINS_PER_HOUR * SECS_PER_MIN;
			if (m_Page1.m_dTimeStep > 1.0)
			{
				pszUnits = _T("hours");
			}
			else
			{
				pszUnits = _T("hour");
			}
			break;
		case CKPAdvectionPg1::TU_MINUTES :
			dTime = m_Page1.m_dTimeStep * SECS_PER_MIN;
			if (m_Page1.m_dTimeStep > 1.0)
			{
				pszUnits = _T("minutes");
			}
			else
			{
				pszUnits = _T("minute");
			}
			break;
		}
		if (pszUnits)
		{
			strFormat.Format(_T("%s%4c-time_step %.*g # seconds = %.*g %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				DBL_DIG,
				dTime,
				DBL_DIG,
				m_Page1.m_dTimeStep,
				pszUnits
				);
		}
		else
		{
			strFormat.Format(_T("%s%4c-time_step %.*g # seconds"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				DBL_DIG,
				m_Page1.m_dTimeStep
				);
		}
		strLines += strFormat;
	}

	// Line 4
	if (m_Page1.m_dInitTime > 0)
	{
		double dTime = 0;
		char* pszUnits = 0;

		switch (m_Page1.m_tuInitTime)
		{
		case CKPAdvectionPg1::TU_YEARS :
			dTime = m_Page1.m_dInitTime * DAYS_PER_YEAR * HOURS_PER_DAY * MINS_PER_HOUR * SECS_PER_MIN;
			if (m_Page1.m_dInitTime > 1.0)
			{
				pszUnits = _T("years");
			}
			else
			{
				pszUnits = _T("year");
			}
			break;
		case CKPAdvectionPg1::TU_DAYS :
			dTime = m_Page1.m_dInitTime * HOURS_PER_DAY * MINS_PER_HOUR * SECS_PER_MIN;
			if (m_Page1.m_dInitTime > 1.0)
			{
				pszUnits = _T("days");
			}
			else
			{
				pszUnits = _T("day");
			}
			break;
		case CKPAdvectionPg1::TU_HOURS :
			dTime = m_Page1.m_dInitTime * MINS_PER_HOUR * SECS_PER_MIN;
			if (m_Page1.m_dInitTime > 1.0)
			{
				pszUnits = _T("hours");
			}
			else
			{
				pszUnits = _T("hour");
			}
			break;
		case CKPAdvectionPg1::TU_MINUTES :
			dTime = m_Page1.m_dInitTime * SECS_PER_MIN;
			if (m_Page1.m_dInitTime > 1.0)
			{
				pszUnits = _T("minutes");
			}
			else
			{
				pszUnits = _T("minute");
			}
			break;
		}
		if (pszUnits)
		{
			strFormat.Format(_T("%s%4c-initial_time %.*g # seconds = %.*g %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				DBL_DIG,
				dTime,
				DBL_DIG,
				m_Page1.m_dInitTime,
				pszUnits
				);
		}
		else
		{
			strFormat.Format(_T("%s%4c-initial_time %.*g # seconds"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				DBL_DIG,
				m_Page1.m_dInitTime
				);
		}
		strLines += strFormat;
	}

	// Line 5:      -print_cells 1-3 5
	std::list<CRange>::const_iterator cIter = m_Page2.m_listPrintRange.begin();
	if (cIter != m_Page2.m_listPrintRange.end())
	{
		strFormat.Format(_T("%s%4c-print_cells"),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
	}
	for (int i = 0; cIter != m_Page2.m_listPrintRange.end(); ++cIter, ++i)
	{
		if ((i % 7) || i == 0)
		{
			strFormat.Format(_T(" %s"), (LPCTSTR)cIter->toCString());
		}
		else
		{
			strFormat.Format(_T("%s%4c             %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(LPCTSTR)(LPCTSTR)cIter->toCString()
				);
		}
		strLines += strFormat;
	}

	// Line 6:      -print_frequency 5
	if (m_Page2.m_nPrintModulus > 1)
	{
		strFormat.Format(_T("%s%4c-print_frequency %d"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			m_Page2.m_nPrintModulus
			);
		strLines += strFormat;
	}

	// Line 7:      -punch_cells 2-5
	cIter = m_Page2.m_listPunchRange.begin();
	if (cIter != m_Page2.m_listPunchRange.end())
	{
		strFormat.Format(_T("%s%4c-punch_cells"),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
	}
	for (int i = 0; cIter != m_Page2.m_listPunchRange.end(); ++cIter, ++i)
	{
		if ((i % 7) || i == 0)
		{
			strFormat.Format(_T(" %s"), (LPCTSTR)cIter->toCString());
		}
		else
		{
			strFormat.Format(_T("%s%4c             %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(LPCTSTR)(LPCTSTR)cIter->toCString()
				);
		}
		strLines += strFormat;
	}

	// Line 8:      -punch_frequency 5
	if (m_Page2.m_nPunchModulus > 1)
	{
		strFormat.Format(_T("%s%4c-punch_frequency %d"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			m_Page2.m_nPunchModulus
			);
		strLines += strFormat;
	}

	// Line 9
	if (m_Page1.m_bPrintWarnings)
	{
		strFormat.Format(_T("%s%4c-warnings true"),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
	}
	else
	{
		strFormat.Format(_T("%s%4c-warnings false"),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
	}

	return strLines + s_strNewLine;
}

void COCKSAdvection::Edit(CString& rStr)
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
