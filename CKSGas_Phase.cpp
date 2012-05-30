// CKSGas_Phase.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CKSGas_Phase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCKSGas_Phase

IMPLEMENT_DYNAMIC(CCKSGas_Phase, baseCKSGas_Phase)

CCKSGas_Phase::CCKSGas_Phase(CWnd* pWndParent, CTreeCtrlNode simNode)
	 : baseCKSGas_Phase(IDS_PROPSHT_CAPTION4, pWndParent)
	 , m_Page1(simNode)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
}

CCKSGas_Phase::~CCKSGas_Phase()
{
}


BEGIN_MESSAGE_MAP(CCKSGas_Phase, baseCKSGas_Phase)
	//{{AFX_MSG_MAP(CCKSGas_Phase)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCKSGas_Phase message handlers

CString CCKSGas_Phase::GetString()
{
	CString strLines;
	CString strFormat;

	/*
	Line 0: GAS_PHASE 1-5 Air
	Line 1: -fixed_pressure
	Line 2: -pressure 1.0
	Line 3: -volume 1.0
	Line 4: -temperature 25.0
	Line 5a: CH4(g) 0.0
	Line 5b: CO2(g) 0.000316
	Line 5c: O2(g) 0.2
	Line 5d: N2(g) 0.78
	*/

	// Line 0
	strLines = GetLineZero(CKeyword::K_GAS_PHASE);

	// Line 1
	if (m_Page1.m_nType == cxxGasPhase::GP_PRESSURE)
	{
		strFormat.Format(_T("%s%4c-fixed_pressure"),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
	}
	else
	{
		ASSERT(m_Page1.m_nType == cxxGasPhase::GP_VOLUME);
		strFormat.Format(_T("%s%4c-fixed_volume"),
			(LPCTSTR)s_strNewLine,
			' '
			);
		if (m_Page1.m_nSolution !=  CCKPGas_PhasePg1::NONE)
		{
			strFormat.TrimRight();
			strLines += strFormat;
			// : -equilibrium with solution 10
			strFormat.Format(_T("%s%4c-equilibrium with solution %d"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				m_Page1.m_nSolution
				);
		}
	}	
	strFormat.TrimRight();
	strLines += strFormat;

	// Line 2
	strFormat.Format(_T("%s%4c-pressure %.*g"),
		(LPCTSTR)s_strNewLine,
		_T(' '),
		DBL_DIG,
		m_Page1.m_dPressureAtm
		);
	strFormat.TrimRight();
	strLines += strFormat;

	// Line 3
	strFormat.Format(_T("%s%4c-volume %.*g"),
		(LPCTSTR)s_strNewLine,
		_T(' '),
		DBL_DIG,
		m_Page1.m_dVolumeL
		);
	strFormat.TrimRight();
	strLines += strFormat;
	
	// Line 4
	strFormat.Format(_T("%s%4c-temperature %.*g"),
		(LPCTSTR)s_strNewLine,
		_T(' '),
		DBL_DIG,
		m_Page1.m_dTempC
		);
	strFormat.TrimRight();
	strLines += strFormat;

	// Line 5
	std::list<CGasComp>::const_iterator iterGasComp = m_Page1.m_listGasComp.begin();
	for (; iterGasComp != m_Page1.m_listGasComp.end(); ++iterGasComp)
	{
		if ((*iterGasComp).m_dP_Read != (*iterGasComp).m_dP_Read)
		{
			strFormat.Format(_T("%s%4c%-9s"),
				(LPCTSTR)s_strNewLine,
				_T(' '), 
				(LPCTSTR)(*iterGasComp).m_strName
				);
		}
		else
		{
			strFormat.Format(_T("%s%4c%-9s %.*g"),
				(LPCTSTR)s_strNewLine,
				_T(' '), 
				(LPCTSTR)(*iterGasComp).m_strName,
				DBL_DIG,
				(*iterGasComp).m_dP_Read
				);
		}
		strFormat.TrimRight();
		strLines += strFormat;
	}

	return strLines + s_strNewLine;
}

void CCKSGas_Phase::Edit(CString& rStr)
{
	PhreeqcI p(rStr);
	p.GetData(this);
}

