// CKSExchange.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CKSExchange.h"

#include "KeywordLoader2.h"
#include "Util.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCKSExchange

IMPLEMENT_DYNAMIC(CCKSExchange, baseCKSExchange)

CCKSExchange::CCKSExchange(CWnd* pWndParent, CTreeCtrlNode simNode)
	 : baseCKSExchange(IDS_PROPSHT_CAPTION6, pWndParent)
	 , m_ranges(simNode, true)
	 , m_strNumFormat(_T("%d"))
{
	m_bSolution_equilibria = false;
	m_nEquilSolutionNum = N_NONE;
	std::set<CDBRange> setSolutions;
	m_strNumFormat = CUtil::CreateRange(m_setSolutions, m_ranges[CKeyword::K_SOLUTION]);


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

CCKSExchange::~CCKSExchange()
{
}


BEGIN_MESSAGE_MAP(CCKSExchange, baseCKSExchange)
	//{{AFX_MSG_MAP(CCKSExchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCKSExchange message handlers
CString CCKSExchange::GetString()
{
	/*
	Line 0: EXCHANGE 10 Measured exchange composition
	Line 1a: CaX2 0.3
	Line 1b: MgX2 0.2
	Line 1c: NaX 0.5
	Line 2a: CaY2 Ca-Montmorillonite equilibrium_phase 0.165
	Line 2b: NaZ Kinetic_clay kinetic_reactant 0.1
	Line 3: -equilibrate with solution 1
	*/
	CString strLines;
	CString strFormat;

	// Line 0
	strLines = GetLineZero(CKeyword::K_EXCHANGE);

	// Line 1
	std::list<CExchComp>::const_iterator const_iter = m_Page1.m_listExchComp.begin();
	for ( ; const_iter != m_Page1.m_listExchComp.end(); ++const_iter)
	{
		strFormat.Format(_T("%s%4c%-7s %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			(LPCTSTR)(*const_iter).m_strFormula,
			LDBL_DIG,
			(*const_iter).m_ldMoles
			);
		strFormat.TrimRight();
		strLines += strFormat;
	}

	// Line 2a
	const_iter = m_Page2.m_listExchComp.begin();
	for ( ; const_iter != m_Page2.m_listExchComp.end(); ++const_iter)
	{
		strFormat.Format(_T("%s%4c%-7s %-20s equilibrium_phase   %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			(LPCTSTR)(*const_iter).m_strFormula,
			(LPCTSTR)(*const_iter).m_strPhase_name,
			DBL_DIG,
			(*const_iter).m_dPhase_proportion
			);
		strFormat.TrimRight();
		strLines += strFormat;
	}
	// Line 2b
	const_iter = m_Page3.m_listExchComp.begin();
	for ( ; const_iter != m_Page3.m_listExchComp.end(); ++const_iter)
	{
		strFormat.Format(_T("%s%4c%-7s %-20s kinetic_reactant    %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			(LPCTSTR)(*const_iter).m_strFormula,
			(LPCTSTR)(*const_iter).m_strRate_name,
			DBL_DIG,
			(*const_iter).m_dPhase_proportion
			);
		strFormat.TrimRight();
		strLines += strFormat;
	}
	// Line 3
	if (m_bSolution_equilibria)
	{
		ASSERT(m_nEquilSolutionNum != N_NONE);
		strFormat.Format(_T("%s%4c-equilibrate with solution %d"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			m_nEquilSolutionNum
			);
		strLines += strFormat;
	}
	return strLines + s_strNewLine;
}

void CCKSExchange::Edit(CString& rStr)
{
	ASSERT(std::numeric_limits<double>::has_signaling_NaN == true);

	CKeywordLoader2 keywordLoader2(rStr);

	struct exchange* exchange_ptr;

	exchange_ptr = &exchange[0];
	m_n_user     = exchange_ptr->n_user;
	m_n_user_end = exchange_ptr->n_user_end;
	m_strDesc    = exchange_ptr->description;

	if (exchange_ptr->solution_equilibria) 
	{
		m_bSolution_equilibria = true;
		m_nEquilSolutionNum = exchange_ptr->n_solution;
	}
	else
	{
		m_bSolution_equilibria = false;
		m_nEquilSolutionNum = N_NONE;
	}

	ASSERT(m_Page1.m_listExchComp.empty());
	ASSERT(m_Page2.m_listExchComp.empty());
	ASSERT(m_Page3.m_listExchComp.empty());

	for (int i = 0; i < exchange_ptr->count_comps; ++i)
	{
		CExchComp exchComp(&(exchange_ptr->comps[i]));

		if (!exchComp.m_strRate_name.IsEmpty())
		{
			ASSERT(exchComp.m_strPhase_name.IsEmpty());
			m_Page3.m_listExchComp.push_back(exchComp);
		}
		else if (!exchComp.m_strPhase_name.IsEmpty())
		{
			m_Page2.m_listExchComp.push_back(exchComp);
		}
		else
		{
			m_Page1.m_listExchComp.push_back(exchComp);
		}
	}
}

