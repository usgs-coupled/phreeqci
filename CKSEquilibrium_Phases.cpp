// CKSEquilibrium_Phases.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CKSEquilibrium_Phases.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCKSEquilibrium_Phases

IMPLEMENT_DYNAMIC(CCKSEquilibrium_Phases, baseCKSEquilibrium_Phases)

CCKSEquilibrium_Phases::CCKSEquilibrium_Phases(CWnd* pWndParent)
	 : baseCKSEquilibrium_Phases(IDS_PROPSHT_CAPTION3, pWndParent)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
}

CCKSEquilibrium_Phases::~CCKSEquilibrium_Phases()
{
}


BEGIN_MESSAGE_MAP(CCKSEquilibrium_Phases, baseCKSEquilibrium_Phases)
	//{{AFX_MSG_MAP(CCKSEquilibrium_Phases)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCKSEquilibrium_Phases message handlers

CString CCKSEquilibrium_Phases::GetString()
{
	CString strLines = GetLineZero(CKeyword::K_EQUILIBRIUM_PHASES);
	CString strFormat;

	std::list<CPurePhase>::const_iterator phaseIter = m_Page1.m_listPurePhase.begin();
	for (; phaseIter != m_Page1.m_listPurePhase.end(); ++phaseIter)
	{
		strFormat.Format(_T("%s%4c%-9s %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			(LPCTSTR)(*phaseIter).m_strName,
			DBL_DIG,
			(*phaseIter).m_dSI
			);
		strLines += strFormat;

		if ((*phaseIter).m_strAlt.IsEmpty())
		{
			strFormat.Format(_T(" %.*g"),
				DBL_DIG,
				(*phaseIter).m_dAmount
				);
			strFormat.TrimRight();
			strLines += strFormat;
			if ((*phaseIter).m_bDissolveOnly)
			{
				strLines += _T(" dissolve_only");
			}
			else if ((*phaseIter).m_bPrecipOnly)
			{
				strLines += _T(" precipitate_only");
			}

		}
		else
		{
			strFormat.Format(_T(" %-9s %.*g"),
				(LPCTSTR)(*phaseIter).m_strAlt,
				DBL_DIG,
				(*phaseIter).m_dAmount
				);
			strFormat.TrimRight();
			strLines += strFormat;
		}

		// -force_equality
		if ((*phaseIter).m_bForceEquality)
		{
			strFormat.Format(_T("%s%8c-force_equality"),
				(LPCTSTR)s_strNewLine,
				_T(' ')
				);
			strLines += strFormat;
		}
	}
	return strLines + s_strNewLine;
}

void CCKSEquilibrium_Phases::Edit(CString& rStr)
{
	PhreeqcI p(rStr);
	p.GetData(this);
}
