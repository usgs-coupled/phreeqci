// KSExchangeSpecies.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KSExchangeSpecies.h"

#include "KeywordLoader2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKSExchangeSpecies

IMPLEMENT_DYNAMIC(CKSExchangeSpecies, baseCKSExchangeSpecies)

CKSExchangeSpecies::CKSExchangeSpecies(CWnd* pWndParent)
	 : baseCKSExchangeSpecies(IDS_PROPSHT_CAPTION28, pWndParent)
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

CKSExchangeSpecies::~CKSExchangeSpecies()
{
}


BEGIN_MESSAGE_MAP(CKSExchangeSpecies, baseCKSExchangeSpecies)
	//{{AFX_MSG_MAP(CKSExchangeSpecies)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CKSExchangeSpecies message handlers

CString CKSExchangeSpecies::GetString()
{
	/*
	Line 0:  EXCHANGE_SPECIES
	Line 1a: SO4-2 = SO4-2
	Line 2a:      log_k     0.0
	Line 5a:      -gamma    5.0     -0.04
	Line 1b: SO4-2 + 9H+ + 8e- = HS- + 4H2O
	Line 2b:      log_k     33.652
	Line 3b:      delta_h   -40.14
	Line 5b:      -gamma    3.5     0.0
	Line 1c: H2O = OH- + H+
	Line 2c:      log_k     -14.000
	Line 3c:      delta_h   13.362  kcal 
	Line 4c:      -a_e -283.971 -0.05069842 13323.0  102.24447 -1119669.0
	Line 5c:      -gamma    3.5     0.0
	Line 1d: HS-  = S2-2 + H+
	Line 2d:      log_k     -14.528
	Line 3d:      delta_h   11.4
	Line 6:       -no_check 
	Line 7d:      -mole_balance    S(-2)2
	*/

	// Line 0
	CString strLines = _T("EXCHANGE_SPECIES");
	CString strFormat;

	std::list<CSpecies>::const_iterator cIter = m_Page1.m_listSpecies.begin();
	for (; cIter != m_Page1.m_listSpecies.end(); ++cIter)
	{
		// Line 1 rxn
		strFormat.Format(_T("%s%s"),
			(LPCTSTR)s_strNewLine,
			(LPCTSTR)cIter->m_strEqn
			);
		strLines += strFormat;

		// Line 2 log_k
		if (cIter->m_dLogK != std::numeric_limits<double>::signaling_NaN())
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

		// Line 3 delta_h
		if (cIter->m_dDeltaH != std::numeric_limits<double>::signaling_NaN())
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

		// Line 4 analytical_expression
		if (cIter->m_bHasAnalExp)
		{
			strFormat.Format(_T("%s%4c%-9s %.*g %.*g %.*g %.*g %.*g"),
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
				cIter->m_dA5
				);
			strLines += strFormat;
		}

		// Line 5 -gamma
		if (cIter->m_nActType == CSpecies::AT_DEBYE_HUCKEL)
		{
			strFormat.Format(_T("%s%4c%-9s %.*g %.*g"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				_T("-gamma"),
				DBL_DIG,
				cIter->m_dDHa,
				DBL_DIG,
				cIter->m_dDHb
				);
			strLines += strFormat;
		}

		// llnl_gamma
		if (cIter->m_nActType == CSpecies::AT_LLNL_DH)
		{
			strFormat.Format(_T("%s%4c%-9s %.*g"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				_T("-llnl_gamma"),
				DBL_DIG,
				cIter->m_dDHa
				);
			strLines += strFormat;
		}

		// co2_llnl_gamma
		if (cIter->m_nActType == CSpecies::AT_LLNL_DH_CO2)
		{
			strFormat.Format(_T("%s%4c%-9s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				_T("-co2_llnl_gamma"),
				DBL_DIG,
				cIter->m_dDHa
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

			// Line 7 -mole_balance
// COMMENT: {8/9/2001 2:20:54 PM}			ASSERT(!cIter->m_strMoleBalance.IsEmpty());
			if (!cIter->m_strMoleBalance.IsEmpty())
			{
				strFormat.Format(_T("%s%4c%-9s"),
					(LPCTSTR)s_strNewLine,
					_T(' '),
					_T("-mole_balance"),
					(LPCTSTR)cIter->m_strMoleBalance
					);
				strLines += strFormat;
			}
		}
	}

	return strLines + s_strNewLine;
}


void CKSExchangeSpecies::Edit(CString& rStr)
{
	CKeywordLoader2 keywordLoader2(rStr);

	bool bEnableLLNL = false;

	for (int i = 0; i < count_s; ++i)
	{
		if(s[i]->rxn == NULL) continue;
		CSpecies species(s[i]);
		m_Page1.m_listSpecies.push_back(species);
		if (species.m_nActType == CSpecies::AT_LLNL_DH || species.m_nActType == CSpecies::AT_LLNL_DH_CO2)
		{
			bEnableLLNL = true;
		}
	}
	m_Page1.EnableLLNL(bEnableLLNL);
}
