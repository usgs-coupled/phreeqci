// KSSelectedOutput.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KSSelectedOutput.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKSSelectedOutput

IMPLEMENT_DYNAMIC(CKSSelectedOutput, baseCKSSelectedOutput)

CKSSelectedOutput::CKSSelectedOutput(CWnd* pWndParent)
	 : baseCKSSelectedOutput(IDS_PROPSHT_CAPTION21, pWndParent)
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
	AddPage(&m_Page4);
	AddPage(&m_Page5);
	AddPage(&m_Page6);
	AddPage(&m_Page7);
	AddPage(&m_Page8);
	AddPage(&m_Page9);
	AddPage(&m_Page10);
}

CKSSelectedOutput::~CKSSelectedOutput()
{
}


BEGIN_MESSAGE_MAP(CKSSelectedOutput, baseCKSSelectedOutput)
	//{{AFX_MSG_MAP(CKSSelectedOutput)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CKSSelectedOutput message handlers

void CKSSelectedOutput::Set_n_user(int user)
{
	this->m_n_user = user;

	std::ostringstream os;
	os << "selected_output_" << user << ".sel";
	this->m_Page1.m_strFileName = os.str().c_str();
}

CString CKSSelectedOutput::GetString()
{
	/*
	Line 0: SELECTED_OUTPUT 1
	Line 1:	-file			       selected.out
	Line 2:	-high_precision        false
	Line 3:	-reset                 true
	Line 4:	-simulation            true
	Line 5:	-state                 true
	Line 6:	-solution              true
	Line 7:	-distance              true
	Line 8:	-time                  true
	Line 9:	-step                  true
	Line 10: -pH                   true
	Line 11: -pe                   true
	Line 12: -reaction             false
	Line 13: -temperature          false
	Line 14: -alkalinity           false
	Line 15: -ionic_strength       false
	Line 16: -water			       false
	Line 17: -charge_balance       false
	Line 18: -percent_error        false
	Line 19: -totals               Hfo_s  C  C(4)  C(-4)  N  N(0)  
	Line 20:                       Fe  Fe(3)  Fe(2)  Ca  Mg  Na  Cl
	Line 21: -molalities           Fe+2  Hfo_sOZn+  ZnX2
	Line 22: -activities           H+  Ca+2  CO2  HCO3-  CO3-2
	Line 23: -equilibrium_phases   Calcite Dolomite  Sphalerite
	Line 24: -saturation_indices   CO2(g)  Siderite
	Line 25: -gases                CO2(g)  N2(g)     O2(g)
	Line 26: -kinetic_reactants    CH2O    Pyrite
	Line 27: -solid_solutions      CaSO4   SrSO4
	Line 28: -isotopes             R(D) R(D)_H3O+ R(D)_H2O(g)
	Line 29: -calculate_values     R(D) R(D)_H3O+ R(D)_H2O(g)
	Line 30: -inverse_modeling     true
	Line 31: -active               true
	Line 32: -user_punch           true
	*/

	// Line 0
	CString strLines = GetLineZero(CKeyword::K_SELECTED_OUTPUT);
	CString strFormat;

	// Line 1
	if (!m_Page1.m_strFileName.IsEmpty())
	{
		strFormat.Format(_T("%s%4c-file                 %s"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			m_Page1.m_strFileName
			);
		strLines += strFormat;
	}

	const CString arrStr[] =
	{
		_T("active"),               // 0 used to be selected_out (Line 31)

		_T("user_punch"),           // 1 (Line 32)
		_T("high_precision"),       // 2 (Line 2)

		// 3..17
		_T("simulation"),           // 3
		_T("state"),                // 4
		_T("solution"),             // 5
		_T("distance"),             // 6
		_T("time"),                 // 7
		_T("step"),                 // 8
		_T("pH"),                   // 9
		_T("pe"),                   // 10
		_T("reaction"),             // 11
		_T("temperature"),          // 12
		_T("alkalinity"),           // 13
		_T("ionic_strength"),       // 14
		_T("water"),                // 15
		_T("charge_balance"),       // 16
		_T("percent_error"),        // 17

		// totals
		// molalities
		// activities
		// equilibrium_phases
		// saturation_indices
		// gases
		// kinetic_reactants
		// solid_solutions

		//{{
		// isotopes
		// calculate_values
		//}}

		_T("inverse_modeling"),     // 18

		//{{
		// active
		// user_punch
		//}}
	};

	// Line 2:     -high_precision       false
	for (int i = 2; i <= 2; ++i)
	{
		switch (m_Page1.m_arrValue[i])
		{
		case CKPSelectedOutputPg1::AS_IS :
			break;

		case CKPSelectedOutputPg1::AS_TRUE :
			strFormat.Format(_T("%s%4c-%-20s %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				arrStr[i],
				_T("true")
				);
			strLines += strFormat;
			break;

		case CKPSelectedOutputPg1::AS_FALSE :
			strFormat.Format(_T("%s%4c-%-20s %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				arrStr[i],
				_T("false")
				);
			strLines += strFormat;
			break;
		}
	}

	int nTrue  = 0;
	int nFalse = 0;
	int nAsIs  = 0;
	for (int j = 3; j <= 17; ++j)
	{
		switch (m_Page1.m_arrValue[j])
		{
		case CKPSelectedOutputPg1::AS_IS :
			nAsIs++;
			break;
		case CKPSelectedOutputPg1::AS_TRUE :
			nTrue++;
			break;
		case CKPSelectedOutputPg1::AS_FALSE :
			nFalse++;
			break;
		}
	}

	// Line 3:	-reset			true
	enum CKPSelectedOutputPg1::ValueType vtReset = CKPSelectedOutputPg1::AS_IS;
	if (nTrue >= nFalse && nAsIs == 0)
	{
		strFormat.Format(_T("%s%4c-%-20s %s"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			_T("reset"),
			_T("true")
			);
		strLines += strFormat;
		vtReset = CKPSelectedOutputPg1::AS_TRUE;
	}
	else if (nTrue < nFalse && nAsIs == 0)
	{
		strFormat.Format(_T("%s%4c-%-20s %s"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			_T("reset"),
			_T("false")
			);
		strLines += strFormat;
		vtReset = CKPSelectedOutputPg1::AS_FALSE;
	}

	// Line 4:  -simulation		true
	// Line 5:  -state			true
	// Line 6:  -solution		true
	// Line 7:  -distance		true
	// Line 8:  -time 			true
	// Line 9:	 -step			true
	// Line 10: -pH			true
	// Line 11: -pe			true
	// Line 12: -reaction			false
	// Line 13: -temperature			false
	// Line 14: -alkalinity			false
	// Line 15: -ionic_strength			false
	// Line 16: -water			false
	// Line 17: -charge_balance			false
	// Line 18: -percent_error			false
	for (int i = 3; i <= 17; ++i)
	{
		switch (m_Page1.m_arrValue[i])
		{
		case CKPSelectedOutputPg1::AS_IS :
			break;

		case CKPSelectedOutputPg1::AS_TRUE :
			if (vtReset != CKPSelectedOutputPg1::AS_TRUE)
			{
				strFormat.Format(_T("%s%4c-%-20s %s"),
					(LPCTSTR)s_strNewLine,
					_T(' '),
					arrStr[i],
					_T("true")
					);
				strLines += strFormat;
			}
			break;

		case CKPSelectedOutputPg1::AS_FALSE :
			if (vtReset != CKPSelectedOutputPg1::AS_FALSE)
			{
				strFormat.Format(_T("%s%4c-%-20s %s"),
					(LPCTSTR)s_strNewLine,
					_T(' '),
					arrStr[i],
					_T("false")
					);
				strLines += strFormat;
			}
			break;
		}
	}

	// Line 19:	-totals			Hfo_s  C  C(4)  C(-4)  N  N(0)
	std::list<CString>::iterator iterStr = m_Page2.m_listTotals.begin();
	if (iterStr != m_Page2.m_listTotals.end())
	{
		strFormat.Format(_T("%s%4c-totals             "),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
	}
	for (int i = 0; iterStr != m_Page2.m_listTotals.end(); ++iterStr, ++i)
	{
		if (!(i % 7))
		{
			if (::_tcscmp(_T("Si"), *iterStr) == 0)
			{
				// Don't allow Si to be the first element on line
				// it'll be interpreted as saturation index
				--i;
			}
		}
		if ((i % 7) || (i == 0))
		{
			strFormat.Format(_T("  %s"), (LPCTSTR)(*iterStr));
		}
		else
		{
			strFormat.Format(_T("%s%4c                      %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(LPCTSTR)(*iterStr)
				);
		}
		strLines += strFormat;
	}

	// Line 21:	-molalities			Fe+2  Hfo_sOZn+  ZnX2
	iterStr = m_Page3.m_listMolalities.begin();
	if (iterStr != m_Page3.m_listMolalities.end())
	{
		strFormat.Format(_T("%s%4c-molalities         "),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
	}
	for (int i = 0; iterStr != m_Page3.m_listMolalities.end(); ++iterStr, ++i)
	{
		if ((i % 4) || i == 0)
		{
			strFormat.Format(_T("  %s"), (LPCTSTR)(*iterStr));
		}
		else
		{
			strFormat.Format(_T("%s%4c                      %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(LPCTSTR)(*iterStr)
				);
		}
		strLines += strFormat;
	}

	// Line 22:	-activities			H+  Ca+2  CO2  HCO3-  CO3-2
	iterStr = m_Page4.m_listActivities.begin();
	if (iterStr != m_Page4.m_listActivities.end())
	{
		strFormat.Format(_T("%s%4c-activities         "),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
	}
	for (int i = 0; iterStr != m_Page4.m_listActivities.end(); ++iterStr, ++i)
	{
		if ((i % 4) || i == 0)
		{
			strFormat.Format(_T("  %s"), (LPCTSTR)(*iterStr));
		}
		else
		{
			strFormat.Format(_T("%s%4c                      %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(LPCTSTR)(*iterStr)
				);
		}
		strLines += strFormat;
	}

	// Line 23:	-equilibrium_phases			Calcite Dolomite  Sphalerite
	iterStr = m_Page5.m_listPhases.begin();
	if (iterStr != m_Page5.m_listPhases.end())
	{
		strFormat.Format(_T("%s%4c-equilibrium_phases "),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
	}
	for (int i = 0; iterStr != m_Page5.m_listPhases.end(); ++iterStr, ++i)
	{
		if ((i % 4) || i == 0)
		{
			strFormat.Format(_T("  %s"), (LPCTSTR)(*iterStr));
		}
		else
		{
			strFormat.Format(_T("%s%4c                      %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(LPCTSTR)(*iterStr)
				);
		}
		strLines += strFormat;
	}

	// Line 24:	-saturation_indices			CO2(g)  Siderite
	iterStr = m_Page6.m_listPhases.begin();
	if (iterStr != m_Page6.m_listPhases.end())
	{
		strFormat.Format(_T("%s%4c-saturation_indices "),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
	}
	for (int i = 0; iterStr != m_Page6.m_listPhases.end(); ++iterStr, ++i)
	{
		if ((i % 4) || i == 0)
		{
			strFormat.Format(_T("  %s"), (LPCTSTR)(*iterStr));
		}
		else
		{
			strFormat.Format(_T("%s%4c                      %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(LPCTSTR)(*iterStr)
				);
		}
		strLines += strFormat;
	}

	// Line 25:	-gases			CO2(g)  N2(g)     O2(g)
	iterStr = m_Page7.m_listGases.begin();
	if (iterStr != m_Page7.m_listGases.end())
	{
		strFormat.Format(_T("%s%4c-gases              "),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
	}
	for (int i = 0; iterStr != m_Page7.m_listGases.end(); ++iterStr, ++i)
	{
		if ((i % 4) || i == 0)
		{
			strFormat.Format(_T("  %s"), (LPCTSTR)(*iterStr));
		}
		else
		{
			strFormat.Format(_T("%s%4c                      %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(LPCTSTR)(*iterStr)
				);
		}
		strLines += strFormat;
	}

	// Line 26:	-kinetic_reactants			CH2O    Pyrite
	iterStr = m_Page8.m_listKinetic.begin();
	if (iterStr != m_Page8.m_listKinetic.end())
	{
		strFormat.Format(_T("%s%4c-kinetic_reactants  "),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
	}
	for (int i = 0; iterStr != m_Page8.m_listKinetic.end(); ++iterStr, ++i)
	{
		if ((i % 4) || i == 0)
		{
			strFormat.Format(_T("  %s"), (LPCTSTR)(*iterStr));
		}
		else
		{
			strFormat.Format(_T("%s%4c                      %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(LPCTSTR)(*iterStr)
				);
		}
		strLines += strFormat;
	}

	// Line 27:	-solid_solutions			CaSO4   SrSO4
	iterStr = m_Page9.m_listSolid.begin();
	if (iterStr != m_Page9.m_listSolid.end())
	{
		strFormat.Format(_T("%s%4c-solid_solutions    "),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
	}
	for (int i = 0; iterStr != m_Page9.m_listSolid.end(); ++iterStr, ++i)
	{
		if ((i % 4) || i == 0)
		{
			strFormat.Format(_T("  %s"), (LPCTSTR)(*iterStr));
		}
		else
		{
			strFormat.Format(_T("%s%4c                      %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(LPCTSTR)(*iterStr)
				);
		}
		strLines += strFormat;
	}

	// Line 28:	-isotopes			R(D) R(D)_H3O+ R(D)_H2O(g)
	iterStr = this->listIsotopes.begin();
	if (iterStr != this->listIsotopes.end())
	{
		strFormat.Format(_T("%s%4c-isotopes           "),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
	}
	for (int i = 0; iterStr != this->listIsotopes.end(); ++iterStr, ++i)
	{
		if ((i % 4) || i == 0)
		{
			strFormat.Format(_T("  %s"), (LPCTSTR)(*iterStr));
		}
		else
		{
			strFormat.Format(_T("%s%4c                      %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(LPCTSTR)(*iterStr)
				);
		}
		strLines += strFormat;
	}

	// Line 29:	-calculate_values			R(D) R(D)_H3O+ R(D)_H2O(g)
	iterStr = this->m_Page10.listCalcValues.begin();
	if (iterStr != this->m_Page10.listCalcValues.end())
	{
		strFormat.Format(_T("%s%4c-calculate_values   "),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
	}
	for (int i = 0; iterStr != this->m_Page10.listCalcValues.end(); ++iterStr, ++i)
	{
		if ((i % 4) || i == 0)
		{
			strFormat.Format(_T("  %s"), (LPCTSTR)(*iterStr));
		}
		else
		{
			strFormat.Format(_T("%s%4c                      %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(LPCTSTR)(*iterStr)
				);
		}
		strLines += strFormat;
	}

	// Line 30:	-inverse_modeling			true
	switch (m_Page1.m_arrValue[18])
	{
	case CKPSelectedOutputPg1::AS_IS :
		break;

	case CKPSelectedOutputPg1::AS_TRUE :
		strFormat.Format(_T("%s%4c-%-20s %s"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			arrStr[18],
			_T("true")
			);
		strLines += strFormat;
		break;

	case CKPSelectedOutputPg1::AS_FALSE :
		strFormat.Format(_T("%s%4c-%-20s %s"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			arrStr[18],
			_T("false")
			);
		strLines += strFormat;
		break;
	}

	// Line 31:     -active           true
	for (int i = 1; i <= 1; ++i)
	{
		switch (m_Page1.m_arrValue[i])
		{
		case CKPSelectedOutputPg1::AS_IS :
			break;

		case CKPSelectedOutputPg1::AS_TRUE :
			strFormat.Format(_T("%s%4c-%-20s %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				arrStr[0],
				_T("true")
				);
			strLines += strFormat;
			break;

		case CKPSelectedOutputPg1::AS_FALSE :
			strFormat.Format(_T("%s%4c-%-20s %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				arrStr[0],
				_T("false")
				);
			strLines += strFormat;
			break;
		}
	}

	// Line 32:     -user_punch           true
	for (int i = 1; i <= 1; ++i)
	{
		switch (m_Page1.m_arrValue[i])
		{
		case CKPSelectedOutputPg1::AS_IS :
			break;

		case CKPSelectedOutputPg1::AS_TRUE :
			strFormat.Format(_T("%s%4c-%-20s %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				arrStr[i],
				_T("true")
				);
			strLines += strFormat;
			break;

		case CKPSelectedOutputPg1::AS_FALSE :
			strFormat.Format(_T("%s%4c-%-20s %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				arrStr[i],
				_T("false")
				);
			strLines += strFormat;
			break;
		}
	}

	return strLines + s_strNewLine;
}

void CKSSelectedOutput::Edit(CString& rStr)
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

