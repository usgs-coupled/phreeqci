// CKSMix.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CKSMix.h"

#include "KeywordLoader2.h"
#include "Util.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCKSMix

IMPLEMENT_DYNAMIC(CCKSMix, baseCKSMix)

CCKSMix::CCKSMix(CWnd* pWndParent, CTreeCtrlNode simNode)
	 : baseCKSMix(IDS_PROPSHT_CAPTION7, pWndParent)
	 , m_ranges(simNode, true)
	 , m_strNumFormat(_T("%d"))
{
	std::set<CDBRange> setSolutions;
	m_strNumFormat = CUtil::CreateRange(m_setSolutions, m_ranges[CKeyword::K_SOLUTION]);

	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
}

CCKSMix::~CCKSMix()
{
}


BEGIN_MESSAGE_MAP(CCKSMix, baseCKSMix)
	//{{AFX_MSG_MAP(CCKSMix)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCKSMix message handlers


CString CCKSMix::GetString()
{
	/*
	Line 0:  MIX 2 Mixing solutions 5, 6, and 7.
	Line 1a:      5     1.1
	Line 1b:      6     0.5
	Line 1c:      7     0.3
	*/

	CString strLines;
	CString strFormat;
	
	// Line 0
	strLines = GetLineZero(CKeyword::K_MIX);

	// Line 1
	std::list<mix_comp>::const_iterator iter = m_Page1.m_listMixComp.begin();
	for (; iter !=m_Page1.m_listMixComp.end(); ++iter)
	{
		strFormat.Format(_T("%s%4c%d%4c%.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			(*iter).n_solution,
			_T(' '),
			DBL_DIG,
			(*iter).fraction
			);
		strFormat.TrimRight();
		strLines += strFormat;
	}

	return strLines + s_strNewLine;
}

void CCKSMix::Edit(CString& rStr)
{
	ASSERT(std::numeric_limits<double>::has_signaling_NaN == true);

	CKeywordLoader2 keywordLoader2(rStr);

	ASSERT( count_mix == 1 );
	ASSERT( mix != NULL );

	struct mix* mix_ptr = &mix[0];

	// mix number
	m_n_user  = mix_ptr->n_user;
	m_strDesc = mix_ptr->description;

	// fill mix_comp list
	for (int i = 0; i < mix_ptr->count_comps; ++i)
	{
		m_Page1.m_listMixComp.push_back(mix_ptr->comps[i]);
	}
}