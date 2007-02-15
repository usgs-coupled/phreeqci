// CKSInverse.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CKSInverse.h"

#include "KeywordLoader2.h"
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

	return strLines + s_strNewLine;
}

void CCKSInverse::Edit(CString& rStr)
{
	ASSERT(std::numeric_limits<double>::has_signaling_NaN == true);

	CKeywordLoader2 keywordLoader2(rStr);

	ASSERT(count_inverse == 1);
	ASSERT(inverse != NULL);
	
	struct inverse* inverse_ptr = &inverse[0];

	m_n_user  = inverse_ptr->n_user;
	m_strDesc = inverse_ptr->description;


	ASSERT(inverse_ptr->count_solns > 0);

	// initial solns
	double dLastUncertainty = 0.05;
	bool bLastForce = false;
	for (int i = 0; i < inverse_ptr->count_solns - 1; ++i)
	{
		InvSol invSol;

		invSol.nSol = inverse_ptr->solns[i];
	
		if (i < inverse_ptr->count_uncertainties)
		{
			dLastUncertainty = inverse_ptr->uncertainties[i];
		}
		invSol.dUncertainty = dLastUncertainty;

		if (i < inverse_ptr->count_force_solns)
		{
			bLastForce = (inverse_ptr->force_solns[i] == TRUE);
		}
		invSol.bForce = bLastForce;

		m_listInvSol.push_back(invSol);
	}

	// final soln
	if (inverse_ptr->count_solns > 0)
	{
		int j = inverse_ptr->count_solns - 1;
		m_invSolFinal.nSol = inverse_ptr->solns[j];
		if (j < inverse_ptr->count_uncertainties)
		{
			dLastUncertainty = inverse_ptr->uncertainties[j];
		}
		m_invSolFinal.dUncertainty = dLastUncertainty;

		if (j < inverse_ptr->count_force_solns)
		{
			bLastForce = (inverse_ptr->force_solns[j] == TRUE);
		}
		m_invSolFinal.bForce = bLastForce;
	}

	// -phases
	for (int nPhase = 0; nPhase < inverse_ptr->count_phases; ++nPhase)
	{
		CInvPhase invPhase(&inverse_ptr->phases[nPhase]);
		
		for (int nIsotope = 0; nIsotope < inverse_ptr->phases[nPhase].count_isotopes; ++nIsotope)
		{
			CIsotope isotope(&inverse_ptr->phases[nPhase].isotopes[nIsotope]);
			invPhase.m_mapIsotopes[isotope.m_strName] = isotope;
		}
		m_Page1B.m_listInvPhase.push_back(invPhase);
	}

	// water_uncertainty

	// -balances
	// pH
	CInvElem elem;
	elem.m_strName = _T("pH");
	bool bAddpH = false;

	// foreach soln
	dLastUncertainty = 0.05;
	for (i = 0; i < inverse_ptr->count_solns; ++i)
	{
		if (i < inverse_ptr->count_ph_uncertainties)
		{
			dLastUncertainty = inverse_ptr->ph_uncertainties[i];
			if (dLastUncertainty != 0.05)
			{
				bAddpH = true;
			}
		}
		elem.m_mapSol2Unc[inverse_ptr->solns[i]] = dLastUncertainty;
	}
	
	// final solution
	if (i < inverse_ptr->count_ph_uncertainties)
	{
		dLastUncertainty = inverse_ptr->ph_uncertainties[i];
		if (dLastUncertainty != 0.05)
		{
			bAddpH = true;
		}
	}
	elem.m_mapSol2Unc[m_invSolFinal.nSol] = dLastUncertainty;
	if (bAddpH)
	{
		m_listInvElem.push_back(elem);
	}

	// now foreach elem
	for (int nElt = 0; nElt < inverse_ptr->count_elts; ++nElt)
	{
		// get element name
		CInvElem elem;
		elem.m_strName = inverse_ptr->elts[nElt].name;

		if (inverse_ptr->elts[nElt].count_uncertainties == 0)
		{
			// foreach soln
			std::list<InvSol>::iterator iterInvSol = m_listInvSol.begin();
			for (; iterInvSol != m_listInvSol.end(); ++iterInvSol)
			{
				elem.m_mapSol2Unc[iterInvSol->nSol] = iterInvSol->dUncertainty;
			}
			// final solution
			elem.m_mapSol2Unc[m_invSolFinal.nSol] = m_invSolFinal.dUncertainty;
		}
		else
		{
			ASSERT(inverse_ptr->elts[nElt].count_uncertainties > 0);

			// foreach soln
			std::list<InvSol>::iterator iterInvSol = m_listInvSol.begin();
			for (int nUnc = 0; iterInvSol != m_listInvSol.end(); ++iterInvSol, ++nUnc)
			{
				if (nUnc < inverse_ptr->elts[nElt].count_uncertainties)
				{
					dLastUncertainty = inverse_ptr->elts[nElt].uncertainties[nUnc];
				}
				elem.m_mapSol2Unc[iterInvSol->nSol] = dLastUncertainty;
			}

			// final solution
			if (nUnc < inverse_ptr->elts[nElt].count_uncertainties)
			{
				dLastUncertainty = inverse_ptr->elts[nElt].uncertainties[nUnc];
			}
			elem.m_mapSol2Unc[m_invSolFinal.nSol] = dLastUncertainty;
		}

		// add to list
		m_listInvElem.push_back(elem);
	}

	// -isotopes
	// foreach isotope
	// for (int nIso = 0; nIso < inverse_ptr->count_isotopes; ++nIso)
	for (int nIso = 0; nIso < inverse_ptr->count_i_u; ++nIso)
	{
		// CInvIsotope invIsotope(&inverse_ptr->isotopes[nIso]);
		CInvIsotope invIsotope(&inverse_ptr->i_u[nIso]);

		if (inverse_ptr->i_u[nIso].count_uncertainties != 0)
		{
			// foreach initial soln
			std::list<InvSol>::iterator iterInvSol = m_listInvSol.begin();
			for (int nIsoUnc = 0; iterInvSol != m_listInvSol.end(); ++iterInvSol, ++nIsoUnc)
			{
				if (nIsoUnc < inverse_ptr->i_u[nIso].count_uncertainties)
				{
					dLastUncertainty = inverse_ptr->i_u[nIso].uncertainties[nIsoUnc];
				}
				invIsotope.m_mapSol2Unc[iterInvSol->nSol] = dLastUncertainty;
			}
			// final solution
			if (nIsoUnc < inverse_ptr->i_u[nIso].count_uncertainties)
			{
				dLastUncertainty = inverse_ptr->i_u[nIso].uncertainties[nIsoUnc];
			}
			invIsotope.m_mapSol2Unc[m_invSolFinal.nSol] = dLastUncertainty;
		}		
		m_Page3.m_listInvIsotopes.push_back(invIsotope);
	}


	// -range
	m_Page1A.m_bRange = inverse_ptr->range;
	m_Page1A.m_dRange = inverse_ptr->range_max;

	// minimal
	m_Page1A.m_bMinimal = inverse_ptr->minimal;

	// -tolerance
	m_Page1A.m_dTol = inverse_ptr->tolerance;

	// -uncertainty_water
	m_Page2.m_bWaterUncert = (inverse_ptr->water_uncertainty != 0.0);
	m_Page2.m_dWaterUncert = inverse_ptr->water_uncertainty;

	// -mineral_water
	m_Page1A.m_bMineralWater = inverse_ptr->mineral_water;

	// -multiple_precision
	m_Page1A.m_bMPSolve = inverse_ptr->mp;

	// -mp_tolerance
	m_Page1A.m_dMPTol = inverse_ptr->mp_tolerance;

	// -censor_mp
	m_Page1A.m_dMPCensor = inverse_ptr->mp_censor;
}
