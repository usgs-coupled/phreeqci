// CKSSolution.cpp : implementation file
//
// $Date: 2004/10/23 01:35:35 $
// $Revision: 1.2 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CKSSolution.h"
#include "KeywordLoader2.h"

// phreeqc routines
extern "C"
{
	int strcmp_nocase_arg1(const char *str1, const char *str2);
}
#include "KeywordPageListItems.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCKSSolution

IMPLEMENT_DYNAMIC(CCKSSolution, baseCKSSolution)

CCKSSolution::CCKSSolution(CWnd* pWndParent)
	 : baseCKSSolution(IDS_PROPSHT_CAPTION10, pWndParent)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
	AddPage(&m_Page2);
	AddPage(&m_Page3);
}

CCKSSolution::~CCKSSolution()
{
}


BEGIN_MESSAGE_MAP(CCKSSolution, baseCKSSolution)
	//{{AFX_MSG_MAP(CCKSSolution)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCKSSolution message handlers

CString CCKSSolution::GetString()
{
	/*
	Line 0:  SOLUTION 25 Test solution number 25
	Line 1:       temp      25.0
	Line 2:       pH        7.0     charge 
	Line 3:       pe        4.5
	Line 4:       redox     O(-2)/O(0)
	Line 5:       units     ppm
	Line 6:       density   1.02
	Line 7a:      Ca        80.
	Line 7b:      S(6)      96.     as SO4
	Line 7c:      S(-2)     1.      as S
	Line 7d:      N(5) N(3) 14.     as N
	Line 7e:      O(0)      8.0 
	Line 7f:      C         61.0    as HCO3      CO2(g)     -3.5
	Line 7g:      Fe        55.     ug/kgs as Fe S(6)/S(-2) Pyrite
	Line 8a:      -isotope  13C     -12.   1.  # permil PDB
	Line 8b:      -isotope  34S     15.    1.5 # permil CDT
	Line 9:       -water    0.5     # kg
	*/

	CString strLines;
	CString strFormat;
	
	// Line 0
	strLines = GetLineZero(CKeyword::K_SOLUTION);

	// Line 1 temp
	strFormat.Format(_T("%s%4c%-9s %.*g"),
		(LPCTSTR)s_strNewLine,
		_T(' '),
		_T("temp"),
		DBL_DIG,
		m_Page1.m_dTemp
		);
	strLines += strFormat;

	// Line 2 pH
	if (m_Page2.m_strChargeBalance.Compare(_T("(pH)")) == 0)
	{
		strFormat.Format(_T("%s%4c%-9s %.*g %s"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			_T("pH"),
			DBL_DIG,
			m_Page1.m_dPH,
			_T("charge")
			);
	}
	else if (!m_Page1.m_strPH_Equil.IsEmpty())
	{
		strFormat.Format(_T("%s%4c%-9s %.*g %s %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			_T("pH"),
			DBL_DIG,
			m_Page1.m_dPH,
			(LPCTSTR)m_Page1.m_strPH_Equil,
			DBL_DIG,
			m_Page1.m_dPH_SI
			);
	}
	else
	{
		strFormat.Format(_T("%s%4c%-9s %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			_T("pH"),
			DBL_DIG,
			m_Page1.m_dPH
			);
	}
	strLines += strFormat;

	// Line 3 pe
	if (m_Page2.m_strChargeBalance.Compare(_T("(pe)")) == 0)
	{
		strFormat.Format(_T("%s%4c%-9s %.*g %s"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			_T("pe"),
			DBL_DIG,
			m_Page1.m_dPE,
			_T("charge")
			);
	}
	else if (!m_Page1.m_strPE_Equil.IsEmpty())
	{
		strFormat.Format(_T("%s%4c%-9s %.*g %s %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			_T("pe"),
			DBL_DIG,
			m_Page1.m_dPE,
			(LPCTSTR)m_Page1.m_strPE_Equil,
			DBL_DIG,
			m_Page1.m_dPE_SI
			);
	}
	else
	{
		strFormat.Format(_T("%s%4c%-9s %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			_T("pe"),
			DBL_DIG,
			m_Page1.m_dPE
			);
	}
	strLines += strFormat;

	// Line 4 redox
	if (!m_Page1.m_strRedox.IsEmpty())
	{
		strFormat.Format(_T("%s%4c%-9s %s"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			_T("redox"),
			(LPCTSTR)m_Page1.m_strRedox
			);
		strLines += strFormat;
	}

	// Line 5 units
	m_Page2.m_strDefaultUnits.MakeLower();
	strFormat.Format(_T("%s%4c%-9s %s"),
		(LPCTSTR)s_strNewLine,
		_T(' '),
		_T("units"), 
		(LPCTSTR)m_Page2.m_strDefaultUnits
		);
	strLines += strFormat;	

	// Line 6 density
	strFormat.Format(_T("%s%4c%-9s %.*g"),
		(LPCTSTR)s_strNewLine,
		_T(' '),
		_T("density"),
		DBL_DIG,
		m_Page1.m_dDensity
		);
	strLines += strFormat;

	// Line 7
	std::list<CConc>::const_iterator iter = m_Page2.m_listConc.begin();
	CString strLine;
	for (; iter != m_Page2.m_listConc.end(); ++iter)
	{
		//  element list, concentration, [units],
		//  ([as formula] or [gfw gfw]), [redox couple],
		//  [(charge or phase name [saturation index])]	

		CConc conc(*iter);
	
		// element list, concentration
		ASSERT(!conc.m_strDesc.IsEmpty());
		ASSERT(conc.m_dInputConc != std::numeric_limits<double>::signaling_NaN());
		strFormat.Format(_T("%s%4c%-9s %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			(LPCTSTR)conc.m_strDesc,
			DBL_DIG,
			conc.m_dInputConc
			);
		strLine = strFormat;

		// [units]
		if (!conc.m_strUnits.IsEmpty() && conc.m_strUnits.Compare("(Default)") != 0)
		{
			strFormat.Format(_T(" %-10s"),
				(LPCTSTR)conc.m_strUnits
				);
			strLine += strFormat;
		}
		
		// [as formula]
		if (!conc.m_strAs.IsEmpty())
		{
			ASSERT(conc.m_dGFW == std::numeric_limits<double>::signaling_NaN());
			strFormat.Format(_T(" as %-10s"),
				(LPCTSTR)conc.m_strAs
				);
			strLine += strFormat;
		}
		
		// [gfw gfw]
		if (conc.m_dGFW != std::numeric_limits<double>::signaling_NaN())
		{
			ASSERT(conc.m_strAs.IsEmpty());
			strFormat.Format(_T(" gfw %.*g"),
				DBL_DIG,
				conc.m_dGFW
				);
			strLine += strFormat;
		}
		
		// [redox couple]
		if (!conc.m_strRedox.IsEmpty())
		{
			strFormat.Format(_T(" %-10s"),
				(LPCTSTR)conc.m_strRedox
				);
			strLine += strFormat;
		}
		
		// [(charge or phase name [saturation index])]
		if (!conc.m_strPhase.IsEmpty())
		{
			strFormat.Format(_T(" %-10s"),
				(LPCTSTR)conc.m_strPhase
				);
			strLine += strFormat;

			if (conc.m_strPhase.CompareNoCase(_T("charge")) == 0)
			{
				ASSERT(conc.m_dPhaseSI == std::numeric_limits<double>::signaling_NaN());
			}
			else
			{
				ASSERT(conc.m_dPhaseSI != std::numeric_limits<double>::signaling_NaN());
				strFormat.Format(_T(" %.*g"),
					DBL_DIG,
					conc.m_dPhaseSI
					);
				strLine += strFormat;
			}
		}
		
		strLine.TrimRight();
		strLines += strLine;
	}

	// Line 8 isotope
	std::list<CIsotope>::const_iterator isoIter = m_Page3.m_listIsotopes.begin();
	for (; isoIter != m_Page3.m_listIsotopes.end(); ++isoIter)
	{
		CIsotope isotope(*isoIter);
		if (isotope.m_dRatioUncertainty != std::numeric_limits<double>::signaling_NaN())
		{
			strFormat.Format(_T("%s%4c%-9s %-7s %.*g %.*g"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				_T("-isotope"),
				(LPCTSTR)isotope.m_strName,
				DBL_DIG,
				isotope.m_dRatio,
				DBL_DIG,
				isotope.m_dRatioUncertainty
				);
		}
		else
		{
			strFormat.Format(_T("%s%4c%-9s %-7s %.*g"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				_T("-isotope"),
				(LPCTSTR)isotope.m_strName,
				DBL_DIG,
				isotope.m_dRatio
				);
		}
		strLines += strFormat;
	}
	
	// Line 9 water
	strFormat.Format(_T("%s%4c%-9s %.*g # kg"),
		(LPCTSTR)s_strNewLine,
		_T(' '),
		_T("-water"),
		DBL_DIG,
		m_Page1.m_dWaterMass
		);
	strLines += strFormat;

	return strLines + s_strNewLine;
}

void CCKSSolution::Edit(CString& rStr)
{
	ASSERT(std::numeric_limits<double>::has_signaling_NaN == true);

	CKeywordLoader2 keywordLoader2(rStr);
	
	ASSERT( solution[0] != NULL );
	struct solution * solution_ptr = solution[0];
	
	m_n_user             = solution_ptr->n_user;
	m_n_user_end         = solution_ptr->n_user_end;
	m_strDesc            = solution_ptr->description;

	m_Page1.m_dPH        = solution_ptr->ph;
	m_Page1.m_dPE        = solution_ptr->solution_pe;
	m_Page1.m_dTemp      = solution_ptr->tc;
	m_Page1.m_strRedox   = solution_ptr->pe[solution_ptr->default_pe].name;
	m_Page1.m_dDensity   = solution_ptr->density;
	m_Page1.m_dWaterMass = solution_ptr->mass_water;
	
	m_Page2.m_strDefaultUnits = solution_ptr->units;
	m_Page2.m_strDefaultUnits.Replace(_T("mg/kgs"), _T("ppm"));
	m_Page2.m_strDefaultUnits.Replace(_T("ug/kgs"), _T("ppb"));
	m_Page2.m_strDefaultUnits.Replace(_T("g/kgs"), _T("ppt"));	

	// concs
	for (int i = 0; solution_ptr->totals[i].description != NULL; ++i)
	{
		if (strcmp(solution_ptr->totals[i].description, "H(1)") == 0)
		{
			if (solution_ptr->totals[i].equation_name != NULL)
			{
				/* check for charge */
				if (strcmp_nocase_arg1(solution_ptr->totals[i].equation_name,"charge") == 0)
				{
					m_Page2.m_strChargeBalance = _T("(pH)");
				}
				/* phase */
				else
				{
					m_Page1.m_strPH_Equil = solution_ptr->totals[i].equation_name;
					m_Page1.m_dPH_SI      = solution_ptr->totals[i].phase_si;
				}
			}
			continue;
		}

		if (strcmp(solution_ptr->totals[i].description, "E") == 0)
		{
			if (solution_ptr->totals[i].equation_name != NULL)
			{
				/* check for charge */
				if (strcmp_nocase_arg1(solution_ptr->totals[i].equation_name,"charge") == 0)
				{
					m_Page2.m_strChargeBalance = _T("(pe)");
				}
				/* phase */
				else
				{
					m_Page1.m_strPE_Equil = solution_ptr->totals[i].equation_name;
					m_Page1.m_dPE_SI      = solution_ptr->totals[i].phase_si;
				}
			}
			continue;
		}

		CConc conc(solution_ptr, &solution_ptr->totals[i]);
		if (conc.m_strPhase.CompareNoCase(_T("charge")) == 0)
		{
			m_Page2.m_strChargeBalance = conc.m_strDesc;
		}		
		m_Page2.m_listConc.push_back(conc);
	}

	// isotopes
	for (i = 0; i < solution_ptr->count_isotopes; ++i)
	{
		CIsotope isotope(&solution_ptr->isotopes[i]);
		m_Page3.m_listIsotopes.push_back(isotope);
	}
}
