// KSPrint.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KSPrint.h"

#include "KeywordLoader2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKSPrint

IMPLEMENT_DYNAMIC(CKSPrint, baseCKSPrint)

CKSPrint::CKSPrint(CWnd* pWndParent)
	 : baseCKSPrint(IDS_PROPSHT_CAPTION19, pWndParent)
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

CKSPrint::~CKSPrint()
{
}


BEGIN_MESSAGE_MAP(CKSPrint, baseCKSPrint)
	//{{AFX_MSG_MAP(CKSPrint)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CKSPrint message handlers


CString CKSPrint::GetString()
{
	/*
	Line 0:  PRINT 
	Line 1:       -reset                false
	Line 2:       -eh                   true
	Line 3:       -equilibrium_phases   true
	Line 4:       -exchange             true
	Line 5:       -gas_phase            true
	Line 6:       -headings             true
	Line 7:       -inverse              true
	Line 8:       -kinetics             true
	Line 9:       -other                true
	Line 10:      -saturation_indices   true
	Line 11:      -solid_solutions      true
	Line 12:      -species              true
	Line 13:      -surface              true
	Line 14:      -totals               true
	Line 15:      -user_print           true
	Line 16:      -selected_output      false
	Line 17:      -status               false
	*/

	// Line 0
	CString strLines = _T("PRINT");
	CString strFormat;

	const CString arrStr[] =
	{
		_T("alkalinity"),              // 0
		_T("echo_input"),              // 1
		_T("eh"),                      // 2
		_T("equilibrium_phases"),      // 3
		_T("exchange"),                // 4
		_T("gas_phase"),               // 5
		_T("headings"),                // 6 
		_T("inverse"),                 // 7
		_T("kinetics"),                // 8
		_T("other"),                   // 9
		_T("saturation_indices"),      // 10
		_T("solid_solutions"),         // 11
		_T("species"),                 // 12
		_T("surface"),                 // 13
		_T("totals"),                  // 14
		_T("user_print"),              // 15
		_T("selected_output"),         // 16
		_T("status"),                  // 17
	};

	ASSERT(sizeof(m_Page1.m_arrValue) / sizeof(m_Page1.m_arrValue[0]) == sizeof(arrStr) / sizeof(arrStr[0]));

	int nCount = sizeof(arrStr) / sizeof(arrStr[0]);

	for (int i = 0; i < nCount; ++i)
	{
		switch (m_Page1.m_arrValue[i])
		{
		case CKPPrintPg1::AS_IS :
			break;

		case CKPPrintPg1::AS_TRUE :
			strFormat.Format(_T("%s%4c-%-21s %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				arrStr[i],
				_T("true")
				);
			strLines += strFormat;
			break;

		case CKPPrintPg1::AS_FALSE :
			strFormat.Format(_T("%s%4c-%-21s %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				arrStr[i],
				_T("false")
				);
			strLines += strFormat;
			break;
		}
	}

	// warnings
	strFormat.Format(_T("%s%4c-%-21s %d"),
		(LPCTSTR)s_strNewLine,
		_T(' '),
		_T("warnings"),
		m_Page1.m_nLimit
		);
	strLines += strFormat;

	// censor
	if (m_Page1.m_censor != 0.0)
	{
		strFormat.Format(_T("%s%4c-%-21s %g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			_T("censor_species"),
			m_Page1.m_censor
			);
		strLines += strFormat;
	}

	return strLines + s_strNewLine;
}

void CKSPrint::Edit(CString& rStr)
{
	CKeywordLoader2 keywordLoader2(rStr);

	int val;

	val = pr.alkalinity;
	m_Page1.m_arrValue[0] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.echo_input;
	m_Page1.m_arrValue[1] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.eh;
	m_Page1.m_arrValue[2] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.pp_assemblage;
	m_Page1.m_arrValue[3] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.exchange;
	m_Page1.m_arrValue[4] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.gas_phase;
	m_Page1.m_arrValue[5] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.headings;
	m_Page1.m_arrValue[6] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.inverse;
	m_Page1.m_arrValue[7] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.kinetics;
	m_Page1.m_arrValue[8] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.use;
	m_Page1.m_arrValue[9] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.saturation_indices;
	m_Page1.m_arrValue[10] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.s_s_assemblage;
	m_Page1.m_arrValue[11] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.species;
	m_Page1.m_arrValue[12] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.surface;
	m_Page1.m_arrValue[13] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.totals;
	m_Page1.m_arrValue[14] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.user_print;
	m_Page1.m_arrValue[15] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;	
	
	val = pr.punch;
	m_Page1.m_arrValue[16] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.status;
	m_Page1.m_arrValue[17] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	m_Page1.m_nLimit = pr.warnings;

	m_Page1.m_censor = ::censor;
}

