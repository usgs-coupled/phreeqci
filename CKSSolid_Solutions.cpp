// CKSSolid_Solutions.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CKSSolid_Solutions.h"

#include "KeywordLoader2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCKSSolid_Solutions

IMPLEMENT_DYNAMIC(CCKSSolid_Solutions, baseCKSSolid_Solutions)

CCKSSolid_Solutions::CCKSSolid_Solutions(CWnd* pWndParent)
	 : baseCKSSolid_Solutions(IDS_PROPSHT_CAPTION11, pWndParent)
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

CCKSSolid_Solutions::~CCKSSolid_Solutions()
{
}


BEGIN_MESSAGE_MAP(CCKSSolid_Solutions, baseCKSSolid_Solutions)
	//{{AFX_MSG_MAP(CCKSSolid_Solutions)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCKSSolid_Solutions message handlers

CString CCKSSolid_Solutions::GetString()
{
	/*
	Line 0: SOLID_SOLUTIONS 1 Two solid solutions
	Line 1a: CaSrBaSO4 # greater than 2 components, ideal
	Line 2a: -comp Anhydrite 1.500
	Line 2b: -comp Celestite 0.05
	Line 2c: -comp Barite 0.05
	Line 1b: Ca(x)Mg(1-x)CO3 # Binary, nonideal
	Line 3: -comp1 Calcite 0.097
	Line 4: -comp2 Ca.5Mg.5CO3 0.003
	Line 5: -temp 25.0
	Line 6: -tempk 298.15
	Line 7: -Gugg_nondim 5.08 1.90
	Optional definitions of excess free-energy parameters for nonideal solid solutions:
	Line 8: -Gugg_kj 12.593 4.70
	Line 9: -activity_coefficients 24.05 1075. 0.0001 0.9999
	Line 10: -distribution_coefficients .0483 1248. .0001 .9999
	Line 11: -miscibility_gap 0.0428 0.9991
	Line 12: -spinodal_gap 0.2746 0.9483
	Line 13: -critical_point 0.6761 925.51
	Line 14: -alyotropic_point 0.5768 -8.363
	Line 15: -Thompson 17.303 7.883
	Line 16: -Margules -0.62 7.6
	*/

	CString strLines;
	CString strFormat;

	// Line 0
	strLines = GetLineZero(CKeyword::K_SOLID_SOLUTIONS);

	std::list<CS_S>::const_iterator s_sIter = m_Page1.m_listS_S.begin();
	for ( ; s_sIter != m_Page1.m_listS_S.end(); ++s_sIter)
	{
		// Line 1a
		strFormat.Format(_T("%s%4c%s"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			(LPCTSTR)(*s_sIter).m_strName
			);

		strLines += strFormat;

		// Line 2
		std::list<CS_S_Comp>::const_iterator compIter = (*s_sIter).m_listComp.begin();
		for ( ; compIter != (*s_sIter).m_listComp.end(); ++compIter)
		{
			strFormat.Format(_T("%s%8c-comp %s %.*g"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(LPCTSTR)(*compIter).m_strName,
				LDBL_DIG,
				(*compIter).m_ldMoles
				);
			strLines += strFormat;
		}
	}

	s_sIter = m_Page2.m_listS_S.begin();
	for ( ; s_sIter != m_Page2.m_listS_S.end(); ++s_sIter)
	{
		// Line 1b
		strFormat.Format(_T("%s%4c%s"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			(LPCTSTR)(*s_sIter).m_strName
			);

		strLines += strFormat;

		// Line 3
		std::list<CS_S_Comp>::const_iterator compIter = (*s_sIter).m_listComp.begin();
		ASSERT( compIter != (*s_sIter).m_listComp.end() );
		strFormat.Format(_T("%s%8c-comp1 %s %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			(LPCTSTR)(*compIter).m_strName,
			LDBL_DIG,
			(*compIter).m_ldMoles
			);
		strLines += strFormat;

		// Line 4
		compIter++;
		ASSERT( compIter != (*s_sIter).m_listComp.end() );
		strFormat.Format(_T("%s%8c-comp2 %s %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			(LPCTSTR)(*compIter).m_strName,
			LDBL_DIG,
			(*compIter).m_ldMoles
			);
		strLines += strFormat;

		// Lines 5 and 6
		strFormat.Format(_T("%s%8c-tempk %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			DBL_DIG,
			(*s_sIter).m_dTk
			);
		strLines += strFormat;

		// Lines 7 - 16
		std::map<enum CS_S::InputCase, CString>::const_iterator item = m_Page2.m_mapIC2String.find((*s_sIter).m_nInputCase);
		ASSERT( item != m_Page2.m_mapIC2String.end() );

		if ((*s_sIter).m_nInputCase == CS_S::IC_ACTIVITY_COEFFICIENTS
			||
			(*s_sIter).m_nInputCase == CS_S::IC_DISTRIBUTION_COEFFICIENTS)
		{
			strFormat.Format(_T("%s%8c%s %.*g %.*g %.*g %.*g"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(LPCTSTR)(*item).second,
				LDBL_DIG,
				(*s_sIter).m_arrldP[0],
				LDBL_DIG,
				(*s_sIter).m_arrldP[1],
				LDBL_DIG,
				(*s_sIter).m_arrldP[2],
				LDBL_DIG,
				(*s_sIter).m_arrldP[3]
				);
			strLines += strFormat;
		}
		else
		{
			strFormat.Format(_T("%s%8c%s %.*g %.*g"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(LPCTSTR)(*item).second,
				LDBL_DIG,
				(*s_sIter).m_arrldP[0],
				LDBL_DIG,
				(*s_sIter).m_arrldP[1]
				);
			strLines += strFormat;
		}
	}

	return strLines + s_strNewLine;
}

void CCKSSolid_Solutions::Edit(CString &rStr)
{
	CKeywordLoader2 keywordLoader2(rStr);

	ASSERT(count_s_s_assemblage == 1);

	struct s_s_assemblage* s_s_assemblage_ptr = &s_s_assemblage[0];

	// Surface number
	m_n_user     = s_s_assemblage_ptr->n_user;
	m_n_user_end = s_s_assemblage_ptr->n_user_end;
	m_strDesc    = s_s_assemblage_ptr->description;

	for (int i = 0; i < s_s_assemblage_ptr->count_s_s; ++i)
	{
		const struct s_s* s_s_ptr = &(s_s_assemblage_ptr->s_s[i]);
		CS_S S_S(s_s_ptr);

		if (S_S.m_arrldP[0] == 0.0 && S_S.m_arrldP[1] == 0.0)
		{
			// ideal
			ASSERT( S_S.m_listComp.size() >= 2 );
			m_Page1.m_listS_S.push_back(S_S);
		}
		else
		{
			// non-ideal
			ASSERT( S_S.m_listComp.size() == 2 );
			m_Page2.m_listS_S.push_back(S_S);
		}
	}
}

