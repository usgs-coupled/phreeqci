// CKSReaction_Temperature.cpp : implementation file
//
// $Date: 2004/10/23 01:35:35 $
// $Revision: 1.2 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CKSReaction_Temperature.h"

#include "KeywordLoader2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCKSReaction_Temperature

IMPLEMENT_DYNAMIC(CCKSReaction_Temperature, baseCKSReaction_Temperature)

CCKSReaction_Temperature::CCKSReaction_Temperature(CWnd* pWndParent)
	 : baseCKSReaction_Temperature(IDS_PROPSHT_CAPTION9, pWndParent)
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

CCKSReaction_Temperature::~CCKSReaction_Temperature()
{
}


BEGIN_MESSAGE_MAP(CCKSReaction_Temperature, baseCKSReaction_Temperature)
	//{{AFX_MSG_MAP(CCKSReaction_Temperature)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCKSReaction_Temperature message handlers

CString CCKSReaction_Temperature::GetString()
{
	/*
	Line 0: REACTION_TEMPERATURE 1 Three explicit reaction temperatures.
	Line 1:     15.0     25.0     35.0

	or

	Line 0: REACTION_TEMPERATURE 1 Three implicit reaction temperatures.
	Line 1:      15.0     35.0 in 3 steps	
	*/

	CString strLines;
	
	// Line 0
	strLines = GetLineZero(CKeyword::K_REACTION_TEMPERATURE);

	CString strFormat;

	// Line 1
	if (m_Page1.m_nType == CCKPReaction_TemperaturePg1::TYPE_LINEAR)
	{
		// implicit
		strFormat.Format(_T("%s%4c%.*g %.*g in %d steps"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			DBL_DIG,
			m_Page1.m_dTemp1,
			DBL_DIG,
			m_Page1.m_dTemp2,
			m_Page1.m_nLinearSteps
			);
		strFormat.TrimRight();
		strLines += strFormat;
	}
	else
	{
		// explicit
		CString strNum;
		std::list<double>::const_iterator iter = m_Page1.m_listSteps.begin();
		strFormat.Format(_T("\n%4c"),
			_T(' ')
			);
		for (int i = 1; iter != m_Page1.m_listSteps.end(); ++iter, ++i)
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
		strLines += strFormat;
	}

	return strLines + s_strNewLine;
}

void CCKSReaction_Temperature::Edit(CString& rStr)
{
	ASSERT(std::numeric_limits<double>::has_signaling_NaN == true);

	CKeywordLoader2 keywordLoader2(rStr);

	ASSERT( count_temperature == 1);

	struct temperature* temp_ptr = &temperature[0];

	// reaction number
	m_n_user     = temp_ptr->n_user;
	m_n_user_end = temp_ptr->n_user_end;
	m_strDesc    = temp_ptr->description;

	// fill page 1
	if (temp_ptr->count_t < 0)
	{
		// implicit
		m_Page1.m_nType = CCKPReaction_TemperaturePg1::TYPE_LINEAR;
		m_Page1.m_nLinearSteps = -temp_ptr->count_t;
		m_Page1.m_dTemp1 = temp_ptr->t[0];
		m_Page1.m_dTemp2 = temp_ptr->t[1];
	}
	else
	{
		// explicit
		m_Page1.m_nType = CCKPReaction_TemperaturePg1::TYPE_LIST;

		for (int i = 0; i < temp_ptr->count_t; ++i)
		{
			m_Page1.m_listSteps.push_back(temp_ptr->t[i]);
		}
	}
}
