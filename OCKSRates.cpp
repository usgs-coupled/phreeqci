// OCKSRates.cpp : implementation file
//
// $Date: 2004/10/23 01:35:36 $
// $Revision: 1.2 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "OCKSRates.h"

#include "KeywordLoader2.h"

// COMMENT: {10/20/2004 8:31:44 PM}#include "KeywordLoader.h"
#include "CStringLineParser.h"
#include "Util.h"
#include "KeywordPageListItems.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COCKSRates

IMPLEMENT_DYNAMIC(COCKSRates, baseOCKSRates)

COCKSRates::COCKSRates(CWnd* pWndParent)
	 : baseOCKSRates(IDS_PROPSHT_CAPTION12, pWndParent)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
}

COCKSRates::~COCKSRates()
{
}


BEGIN_MESSAGE_MAP(COCKSRates, baseOCKSRates)
	//{{AFX_MSG_MAP(COCKSRates)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COCKSRates message handlers

CString COCKSRates::GetString()
{
	/*
	Line 0: RATES 
	Line 1a:     Calcite
	Line 2a:     -start
	Basic: 1   rem M = current number of moles of calcite
	Basic: 2   rem M0 = number of moles of calcite initially present
	Basic: 3   rem PARM(1) = A/V, cm^2/L 
	Basic: 4   rem PARM(2) = exponent for M/M0
	Basic: 10  si_cc = SI("Calcite")
	Basic: 20  if (M <= 0 and si_cc < 0) then goto 200
	Basic: 30    k1 = 10^(0.198 - 444.0 / TK )
	Basic: 40    k2 = 10^(2.84 - 2177.0 / TK)
	Basic: 50    if TC <= 25 then k3 = 10^(-5.86 - 317.0 / TK )
	Basic: 60    if TC > 25 then k3  = 10^(-1.1 - 1737.0 / TK )
	Basic: 70    t = 1
	Basic: 80    if M0 > 0 then t = M/M0
	Basic: 90    if t = 0 then t = 1
	Basic: 100   area = PARM(1) * (t)^PARM(2)
	Basic: 110   rf = k1*ACT("H+")+k2*ACT("CO2")+k3*ACT("H2O")
	Basic: 120   rem 1e-3 converts mmol to mol
	Basic: 130   rate = area * 1e-3 * rf * (1 - 10^(2/3*si_cc))
	Basic: 140   moles = rate * TIME
	Basic: 200 SAVE moles
	Line 3a:     -end
	Line 1b:     Pyrite
	Line 2b:     -start
	Basic: 1   rem  PARM(1) = log10(A/V, 1/dm)
	Basic: 2   rem  PARM(2) = exp for (M/M0)
	Basic: 3   rem  PARM(3) = exp for O2
	Basic: 4   rem  PARM(4) = exp for H+
	Basic: 10  if (M <= 0) then goto 200
	Basic: 20  if (SI("Pyrite") >= 0) then goto 200
	Basic: 30    lograte = -10.19 + PARM(1) + PARM(2)*LOG10(M/M0)
	Basic: 40    lograte = lograte + PARM(3)*LM("O2") + PARM(4)*LM("H+")
	Basic: 50    moles = (10^lograte) * TIME
	Basic: 60    if (moles > M) then moles = M
	Basic: 200 SAVE moles
	Line 3b:     -end
	*/

	CString strLines;
	CString strFormat;

	// Line 0
	strLines = _T("RATES");

	// Line 1
	std::list<CRate>::const_iterator rate_iter = m_Page1.m_listRates.begin();
	for ( ; rate_iter != m_Page1.m_listRates.end(); ++rate_iter)
	{
		// Line 1 & 2
		strFormat.Format(_T("%s%4c%s%s-start"), 
			(LPCTSTR)s_strNewLine,
			_T(' '),
			(LPCTSTR)(*rate_iter).m_strName,
			(LPCTSTR)s_strNewLine
			);
		strLines += strFormat;

		// Basic
		std::list<basic_command>::const_iterator command_iter = (*rate_iter).m_listCommands.begin();
		for ( ; command_iter != (*rate_iter).m_listCommands.end(); ++command_iter)
		{
			strFormat.Format(_T("%s%d %s"), 
				(LPCTSTR)s_strNewLine,
				(*command_iter).nLine,
				(LPCTSTR)(*command_iter).strCommand
				);
			strLines += strFormat;
		}

		// Line 3
		strFormat.Format(_T("%s-end"), 
			(LPCTSTR)s_strNewLine
			);
		strLines += strFormat;
	}

	return strLines + s_strNewLine;
}

void COCKSRates::Edit(CString& rStr)
{
	CKeywordLoader2 keywordLoader2(rStr);

	// empty keyword will assert
	// ASSERT(count_rates >= 1);

	for (int i = 0; i < count_rates; ++i)
	{
		CRate rate(&rates[i]);
		m_Page1.m_listRates.push_back(rate);
	}
}

