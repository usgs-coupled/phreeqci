// KSPhases.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KSPhases.h"

#include "KeywordLoader2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKSPhases

IMPLEMENT_DYNAMIC(CKSPhases, baseCKSPhases)

CKSPhases::CKSPhases(CWnd* pWndParent)
	 : baseCKSPhases(IDS_PROPSHT_CAPTION29, pWndParent)
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

CKSPhases::~CKSPhases()
{
}


BEGIN_MESSAGE_MAP(CKSPhases, baseCKSPhases)
	//{{AFX_MSG_MAP(CKSPhases)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CKSPhases message handlers


CString CKSPhases::GetString()
{
	/*
	Line 0:  PHASES
	Line 1a: Gypsum
	Line 2a:      CaSO4:2H2O = Ca+2 + SO4-2 + 2H2O
	Line 3a:      log_k     -4.58
	Line 4a:      delta_h   -0.109
	Line 5:       -analytical_expression 68.2401 0.0 -3221.51  -25.0627  0.0
	Line 1b: O2(g)
	Line 2b:      O2 = O2
	Line 3b:      log_k     -2.96
	Line 4b:      delta_h   1.844
	*/

	// Line 0
	CString strLines = _T("PHASES");
	CString strFormat;

	std::list<CPhase>::const_iterator cIter = m_Page1.m_listPhase.begin();
	for (; cIter != m_Page1.m_listPhase.end(); ++cIter)
	{
		// Line 1 phase name
		strFormat.Format(_T("%s%s"),
			(LPCTSTR)s_strNewLine,
			(LPCTSTR)cIter->m_strName
			);
		strLines += strFormat;

		// Line 2 rxn
		strFormat.Format(_T("%s%4c%s"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			(LPCTSTR)cIter->m_strEqn
			);
		strLines += strFormat;

		// Line 3 log_k
		if (cIter->m_dLogK == cIter->m_dLogK)
		{
			strFormat.Format(_T("%s%4c%-9s %.*g"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				_T("log_k"),
				DBL_DIG,
				cIter->m_dLogK
				);
			strLines += strFormat;
		}

		// Line 4 delta_h
		if (cIter->m_dDeltaH == cIter->m_dDeltaH)
		{
			CString strUnits;
			switch (cIter->m_nDeltaHUnits)
			{
			case kcal:
				strUnits = _T("kcal");
				break;
			case cal:
				strUnits = _T("cal");
				break;
			case kjoules:
				strUnits = _T("kJ");
				break;
			case joules:
				strUnits = _T("J");
				break;
			}
			strFormat.Format(_T("%s%4c%-9s %.*g %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				_T("delta_h"),
				DBL_DIG,
				cIter->m_dDeltaH,
				(LPCTSTR)strUnits
				);
			strLines += strFormat;
		}

		// Line 5 analytical_expression
		if (cIter->m_bHasAnalExp)
		{
			strFormat.Format(_T("%s%4c%-9s %.*g %.*g %.*g %.*g %.*g %.*g"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				_T("-analytical_expression"),
				DBL_DIG,
				cIter->m_dA1,
				DBL_DIG,
				cIter->m_dA2,
				DBL_DIG,
				cIter->m_dA3,
				DBL_DIG,
				cIter->m_dA4,
				DBL_DIG,
				cIter->m_dA5,
				DBL_DIG,
				cIter->m_dA6
				);
			strLines += strFormat;
		}

		// Line 6 -no_check
		if (!cIter->m_bCheckEqn)
		{
			strFormat.Format(_T("%s%4c%-9s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				_T("-no_check")
				);
			strLines += strFormat;
		}
	}

	return strLines + s_strNewLine;
}

void CKSPhases::Edit(CString& rStr)
{
	PhreeqcI p(rStr);
	p.GetData(this);
}
