// KSPitzer.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KSSIT.h"

#include "KeywordLoader2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKSSIT

IMPLEMENT_DYNAMIC(CKSSIT, baseCKSSIT)

CKSSIT::CKSSIT(CWnd* pWndParent)
	 : baseCKSSIT(IDS_STRING662, pWndParent)
	 , m_PageEpsilon(CKPSITEpsilon::IDD, IDS_STRING663)
{
	m_bNeedDatabase = true;
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the
	// active one is to call SetActivePage().

	AddPage(&m_PageEpsilon);
}

CKSSIT::~CKSSIT()
{
}


BEGIN_MESSAGE_MAP(CKSSIT, baseCKSSIT)
	//{{AFX_MSG_MAP(CKSSIT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CKSSIT message handlers

CString CKSSIT::GetString()
{
	// Line 0
	CString strLines = _T("SIT");

	CString strFormat;
	CString strAppend;

	// -epsilon
	strFormat.Format(_T("%s%s"),
		(LPCTSTR)s_strNewLine,
		_T("-epsilon")
		);
	strLines += strFormat;

	std::list<CPitzParam>::const_iterator cIter = m_PageEpsilon.m_listParams.begin();
	for (; cIter != m_PageEpsilon.m_listParams.end(); ++cIter)
	{
		// species
		strFormat.Format(_T("%s  %-11s  %-11s"),
			(LPCTSTR)s_strNewLine,
			cIter->species[0],
			cIter->species[1]
		);
		strLines += strFormat;

		// analytical_expression
		int nnums = 6;
		for (; nnums; --nnums)
		{
			if (cIter->a[nnums - 1] != 0 && (cIter->a[nnums - 1] == cIter->a[nnums - 1]))
			{
				break;
			}
		}
		// need at least one
		if (nnums == 0) nnums = 1;
		for (int i = 0; i < nnums; ++i)
		{
			if (cIter->a[i] != cIter->a[i])
			{
				strFormat.Format(_T("   %.*g"),
					DBL_DIG,
					0.0
				);
			}
			else
			{
				strFormat.Format(_T("   %.*g"),
					DBL_DIG,
					cIter->a[i]
				);
			}
			strAppend.Format(_T("%-10s"), strFormat);
			strLines += strAppend;
		}
	}

	return strLines + s_strNewLine;
}

void CKSSIT::Edit(CString& rStr)
{
	CKeywordLoader2 keywordLoader2(rStr);

	for (int i = 0; i < ::count_sit_param; ++i)
	{
		CPitzParam p(::sit_params[i]);
		if (p.type == TYPE_SIT_EPSILON)
		{
			this->m_PageEpsilon.m_listParams.push_back(p);
		}
	}
}
