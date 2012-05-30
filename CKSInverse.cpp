// CKSInverse.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CKSInverse.h"

#include "Util.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCKSInverse

IMPLEMENT_DYNAMIC(CCKSInverse, baseCKSInverse)

CCKSInverse::CCKSInverse(CWnd* pWndParent, CTreeCtrlNode simNode)
	 : baseCKSInverse(IDS_PROPSHT_CAPTION14, pWndParent)
	 , m_ranges(simNode)
{
	// default to no final soln
	m_invSolFinal.nSol = -1;

	// create soln range
	m_strNumFormat = CUtil::CreateRange(m_setSolutions, m_ranges[CKeyword::K_SOLUTION]);

	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	AddPage(&m_Page1A);
	AddPage(&m_Page1B);
	AddPage(&m_Page2);
	AddPage(&m_Page3);
}

CCKSInverse::~CCKSInverse()
{
}


BEGIN_MESSAGE_MAP(CCKSInverse, baseCKSInverse)
	//{{AFX_MSG_MAP(CCKSInverse)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCKSInverse message handlers

CString CCKSInverse::GetString()
{
	/*
	Line 0:  INVERSE_MODELING 1
	Line 1:       -solutions       10 3 5
	Line 2:       -uncertainty     0.02   0.04
	Line 3:       -phases 
	Line 4a:           Calcite     force  pre   13C   -1.0   1
	Line 4b:           Anhydrite   force  dis   34S   13.5   2
	Line 4c:           CaX2
	Line 4d:           NaX
	Line 5:       -balances 
	Line 6a            pH          0.1
	Line 6b:           Ca          0.01     -0.005
	Line 6c:           Alkalinity  -1.0e-6
	Line 6d:           Fe          0.05     0.1      0.2
	Line 7:       -isotopes
	Line 8a:            13C        0.05     0.1      0.05 
	Line 8b:            34S        1.0
	Line 9:       -range           10000
	Line 10:      -minimal
	Line 11:      -tolerance       1e-10
	Line 12:      -force_solutions true     false
	Line 13:      -uncertainty_water  0.55  # moles (~1%) 
	Line 14:      -mineral_water   false
	New:          -lon_netpath  prefix
	New:          -pat_netpath  prefix
	*/

	CString strLines;
	CString strFormat;
	
	// Line 0
	strLines = GetLineZero(CKeyword::K_INVERSE_MODELING);

	// Line 1
	std::list<InvSol>::iterator iterInvSol;
	iterInvSol = m_listInvSol.begin();
	if (iterInvSol != m_listInvSol.end())
	{
		strFormat.Format(_T("%s%4c-solutions     "),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		CString strNums;
		for (; iterInvSol != m_listInvSol.end(); ++iterInvSol)
		{
			strNums.Format(_T(" %-*ld"), 8, iterInvSol->nSol);
			strFormat += strNums;
		}
		// final soln
		strNums.Format(_T(" %-*ld"), 8, m_invSolFinal.nSol);
		strNums.TrimRight();

		strFormat += strNums;

		strLines += strFormat;
	}


	// Line 2
	iterInvSol = m_listInvSol.begin();
	if (iterInvSol != m_listInvSol.end())
	{
		strFormat.Format(_T("%s%4c-uncertainty   "),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		CString strNums;
		for (; iterInvSol != m_listInvSol.end(); ++iterInvSol)
		{
			strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, iterInvSol->dUncertainty);
			strFormat += strNums;
		}
		// final soln
		strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, m_invSolFinal.dUncertainty);
		strNums.TrimRight();
		strFormat += strNums;

		strLines += strFormat;
	}

	// Line 3
	if (m_Page1B.m_listInvPhase.size() != 0)
	{
		strFormat.Format(_T("%s%4c-phases"),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;

		// find longest phase name
		int c = 11;
		std::list<CInvPhase>::iterator iterInvPhase = m_Page1B.m_listInvPhase.begin();
		for (; iterInvPhase !=  m_Page1B.m_listInvPhase.end(); ++iterInvPhase)
		{
			c = max(c, iterInvPhase->m_strName.GetLength());
		}

		// Line 4
		iterInvPhase = m_Page1B.m_listInvPhase.begin();
		for (; iterInvPhase !=  m_Page1B.m_listInvPhase.end(); ++iterInvPhase)
		{
			strFormat.Format(_T("%s%4c%s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				iterInvPhase->GetString(c)
				);
			strFormat.TrimRight();
			strLines += strFormat;
		}
	}

	// Line 5
	if (m_listInvElem.size() != 0)
	{
		strFormat.Format(_T("%s%4c-balances"),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;

		// Line 6
		// MAYBE: check if pH needs to be output
		std::list<CInvElem>::iterator iterInvElem = m_listInvElem.begin();
		for (; iterInvElem !=  m_listInvElem.end(); ++iterInvElem)
		{
			strFormat.Format(_T("%s%4c%s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				iterInvElem->GetString(m_listInvSol, m_invSolFinal)
				);
			strLines += strFormat;
		}
	}

	// Line 7
	if (m_Page3.m_listInvIsotopes.size() != 0)
	{
		strFormat.Format(_T("%s%4c-isotopes"),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;

		// Line 8
		std::list<CInvIsotope>::iterator iterInvIsotope = m_Page3.m_listInvIsotopes.begin();
		for (; iterInvIsotope !=  m_Page3.m_listInvIsotopes.end(); ++iterInvIsotope)
		{
			strFormat.Format(_T("%s%4c%s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				iterInvIsotope->GetString(m_listInvSol, m_invSolFinal, m_Page3.m_mapIso2DefaultUnc)
				);
			strLines += strFormat;
		}
	}

	// Line 9
	if (m_Page1A.m_bRange)
	{
		strFormat.Format(_T("%s%4c-range             %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			DBL_DIG,
			m_Page1A.m_dRange
			);
		strFormat.TrimRight();
		strLines += strFormat;
	}

	// Line 10
	if (m_Page1A.m_bMinimal)
	{
		strFormat.Format(_T("%s%4c-minimal"),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strFormat.TrimRight();
		strLines += strFormat;
	}

	// Line 11
	strFormat.Format(_T("%s%4c-tolerance         %.*g"),
		(LPCTSTR)s_strNewLine,
		_T(' '),
		DBL_DIG,
		m_Page1A.m_dTol
		);
	strFormat.TrimRight();
	strLines += strFormat;

	// Line 12
	bool bAtLeastOneTrue = false;
	iterInvSol = m_listInvSol.begin();
	if (iterInvSol != m_listInvSol.end())
	{
		strFormat.Format(_T("%s%4c-force_solutions  "),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		CString strTorF;
		for (; iterInvSol != m_listInvSol.end(); ++iterInvSol)
		{
			if (iterInvSol->bForce)
				bAtLeastOneTrue = true;
			strFormat += (iterInvSol->bForce) ? _T(" true") : _T(" false");
		}
		// final soln
		if (m_invSolFinal.bForce)
			bAtLeastOneTrue = true;
		strFormat += (m_invSolFinal.bForce) ? _T(" true") : _T(" false");

		if (bAtLeastOneTrue)
			strLines += strFormat;
	}

	// Line 13
	if (m_Page2.m_bWaterUncert)
	{
		strFormat.Format(_T("%s%4c-uncertainty_water %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			DBL_DIG,
			m_Page2.m_dWaterUncert
			);
		strFormat.TrimRight();
		strLines += strFormat;
	}

	// Line 14
	if (m_Page1A.m_bMineralWater)
	{
		strFormat.Format(_T("%s%4c-mineral_water     true"),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
	}
	else
	{
		strFormat.Format(_T("%s%4c-mineral_water     false"),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
	}

	if (m_Page1A.m_bMPSolve)
	{
		// -multiple_precision
		strFormat.Format(_T("%s%4c-multiple_precision     true"),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;

		// -mp_tolerance
		strFormat.Format(_T("%s%4c-mp_tolerance %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			DBL_DIG,
			m_Page1A.m_dMPTol
			);
		strFormat.TrimRight();
		strLines += strFormat;

		// -censor_mp
		strFormat.Format(_T("%s%4c-censor_mp %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			DBL_DIG,
			m_Page1A.m_dMPCensor
			);
		strFormat.TrimRight();
		strLines += strFormat;
	}

	if (m_Page1A.m_bLonNetpath)
	{
		// -lon_netpath
		strFormat.Format(_T("%s%4c-lon_netpath %s"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			(LPCTSTR)m_Page1A.m_sLonPrefix
			);
		strLines += strFormat;
	}
	if (m_Page1A.m_bPatNetpath)
	{
		// -pat_netpath
		strFormat.Format(_T("%s%4c-pat_netpath %s"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			(LPCTSTR)m_Page1A.m_sPatPrefix
			);
		strLines += strFormat;
	}

	return strLines + s_strNewLine;
}

void CCKSInverse::Edit(CString& rStr)
{
	PhreeqcI p(rStr);
	p.GetData(this);
}
