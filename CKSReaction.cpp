// CKSReaction.cpp : implementation file
//
// $Date: 2004/10/23 01:35:35 $
// $Revision: 1.2 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CKSReaction.h"

#include "KeywordLoader2.h"
#include "Util.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCKSReaction

IMPLEMENT_DYNAMIC(CCKSReaction, baseCKSReaction)

CCKSReaction::CCKSReaction(CWnd* pWndParent)
	 : baseCKSReaction(IDS_PROPSHT_CAPTION8, pWndParent)
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

CCKSReaction::~CCKSReaction()
{
}


BEGIN_MESSAGE_MAP(CCKSReaction, baseCKSReaction)
	//{{AFX_MSG_MAP(CCKSReaction)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCKSReaction message handlers

CString CCKSReaction::GetString()
{
	/*
	Line 0:  REACTION 5 Add sodium chloride and calcite to solution.
	Line 1a:      NaCl     2.0
	Line 1b:      Calcite  0.001
	Line 2:       0.25     0.5     0.75     1.0  moles
	*/

	CString strLines;
	CString strFormat;

	// Line 0
	strLines = GetLineZero(CKeyword::K_REACTION);

	// Line 1
	std::list<CNameCoef>::const_iterator const_iter = m_Page1.m_listNameCoef.begin();
	for ( ; const_iter != m_Page1.m_listNameCoef.end(); ++const_iter)
	{
		strFormat.Format(_T("%s%4c%-10s %g"), 
			(LPCTSTR)s_strNewLine,
			_T(' '),
			(LPCTSTR)(*const_iter).m_strName,
			(*const_iter).m_dCoef
			);
		strFormat.TrimRight();
		strLines += strFormat;
	}

	// Line 2
	CString strUnits;
	switch (m_Page2.m_nUnits)
	{
	case CCKPReactionPg2::UNITS_MICROMOLES :
		strUnits = _T("micromoles");
		break;
	case CCKPReactionPg2::UNITS_MILLIMOLES :
		strUnits = _T("millimoles");
		break;
	case CCKPReactionPg2::UNITS_MOLES :
		strUnits = _T("moles");
		break;
	}

	if (m_Page2.m_nType == CCKPReactionPg2::TYPE_LINEAR)
	{
		strFormat.Format(_T("%s%4c%.*g %s in %d steps"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			DBL_DIG,
			m_Page2.m_dLinearAmt,
			(LPCTSTR)strUnits,
			m_Page2.m_nLinearSteps
			);
		strFormat.TrimRight();
		strLines += strFormat;
	}
	else
	{
		ASSERT(m_Page2.m_nType == CCKPReactionPg2::TYPE_LIST);

		CString strNum;
		std::list<double>::const_iterator iter = m_Page2.m_listSteps.begin();
		strFormat.Format(_T("%s%4c"),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		for (int i = 1; iter != m_Page2.m_listSteps.end(); ++iter, ++i)
		{
			if (i % 8 == 0)
			{
				strLines += strFormat;
				strFormat.Format(_T("%s%4c"),
					(LPCTSTR)s_strNewLine,
					_T(' ')
					);
			}
			strNum.Format(_T("%.*g  "),
				DBL_DIG,
				(*iter)
				);
			strFormat += strNum;
		}
		strFormat.TrimRight();
		strFormat += CString(_T(" ")) + strUnits;
		strLines += strFormat;
	}

	return strLines + s_strNewLine;

}

void CCKSReaction::Edit(CString& rStr)
{
	ASSERT(std::numeric_limits<double>::has_signaling_NaN == true);

	CKeywordLoader2 keywordLoader2(rStr);

	ASSERT( count_irrev == 1);

	struct irrev* irrev_ptr = &irrev[0];

	// reaction number
	m_n_user     = irrev_ptr->n_user;
	m_n_user_end = irrev_ptr->n_user_end;
	m_strDesc    = irrev_ptr->description;

	// fill page 1
	for (int i = 0; i < irrev_ptr->count_list; ++i)
	{
		m_Page1.m_listNameCoef.push_back(&irrev_ptr->list[i]);
	}

	// fill page 2
	if(irrev_ptr->count_steps < 0)
	{
		// equal increments
		m_Page2.m_nType = CCKPReactionPg2::TYPE_LINEAR;

		m_Page2.m_nLinearSteps = -irrev_ptr->count_steps;
		m_Page2.m_dLinearAmt = irrev_ptr->steps[0];
	}
	else
	{
		// list of increments
		m_Page2.m_nType = CCKPReactionPg2::TYPE_LIST;

		for (int i = 0; i < irrev_ptr->count_steps; ++i)
		{
			m_Page2.m_listSteps.push_back(irrev_ptr->steps[i]);
		}
	}

	if (CString(_T("Mol")).CompareNoCase(irrev->units) == 0)
	{
		m_Page2.m_nUnits = CCKPReactionPg2::UNITS_MOLES;
	}
	else if (CString(_T("mMol")).CompareNoCase(irrev->units) == 0)
	{
		m_Page2.m_nUnits = CCKPReactionPg2::UNITS_MILLIMOLES;
	}
	else if (CString(_T("uMol")).CompareNoCase(irrev->units) == 0)
	{
		m_Page2.m_nUnits = CCKPReactionPg2::UNITS_MICROMOLES;
	}
}
