// KSKnobs.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "KSKnobs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKSKnobs

IMPLEMENT_DYNAMIC(CKSKnobs, baseCKSKnobs)

CKSKnobs::CKSKnobs(CWnd* pWndParent)
	 : baseCKSKnobs(IDS_PROPSHT_CAPTION33, pWndParent)
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

CKSKnobs::~CKSKnobs()
{
}


BEGIN_MESSAGE_MAP(CKSKnobs, baseCKSKnobs)
	//{{AFX_MSG_MAP(CKSKnobs)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CKSKnobs message handlers

CString CKSKnobs::GetString()
{
	/*
	Line 0:  KNOBS 
	Line 1:       -iterations             150
	Line 2:       -convergence_tolerance  1e-8
	Line 3:       -tolerance              1e-14
	Line 4:       -step_size              10.
	Line 5:       -pe_step_size           5.
	Line 6:       -diagonal_scale         TRUE
	Line 7:       -debug_diffuse_layer    TRUE
	Line 8:       -debug_inverse          TRUE
	Line 9:       -debug_model            TRUE
	Line 10:      -debug_prep             TRUE
	Line 11:      -debug_set              TRUE
	Line 12:      -logfile                TRUE
	*/

	// Line 0
	CString strLines = _T("KNOBS");
	CString strFormat;

	// Line 1:       -iterations             150
	strFormat.Format(_T("%s%4c-%-21s %d"),
		(LPCTSTR)s_strNewLine,
		_T(' '),
		_T("iterations"),
		m_Page1.m_nItmax
		);
	strLines += strFormat;

	// Line 2:       -convergence_tolerance  1e-8
	strFormat.Format(_T("%s%4c-%-21s %.*g"),
		(LPCTSTR)s_strNewLine,
		_T(' '),
		_T("convergence_tolerance"),
		DBL_DIG,
		m_Page1.m_dConvergenceTolerance
		);
	strLines += strFormat;

	// Line 3:       -tolerance              1e-14
	strFormat.Format(_T("%s%4c-%-21s %.*g"),
		(LPCTSTR)s_strNewLine,
		_T(' '),
		_T("tolerance"),
		DBL_DIG,
		m_Page1.m_dIneqTol
		);
	strLines += strFormat;

	// Line 4:       -step_size              10.
	strFormat.Format(_T("%s%4c-%-21s %.*g"),
		(LPCTSTR)s_strNewLine,
		_T(' '),
		_T("step_size"),
		DBL_DIG,
		m_Page1.m_dStepSize
		);
	strLines += strFormat;

	// Line 5:       -pe_step_size           5.
	strFormat.Format(_T("%s%4c-%-21s %.*g"),
		(LPCTSTR)s_strNewLine,
		_T(' '),
		_T("pe_step_size"),
		DBL_DIG,
		m_Page1.m_dPeStepSize
		);
	strLines += strFormat;

#ifdef ENABLE_SCALE_PURE_PHASES
	// Line X:       -scale_pure_phases
	strFormat.Format(_T("%s%4c-%-21s %.*g"),
		(LPCTSTR)s_strNewLine,
		_T(' '),
		_T("scale_pure_phases"),
		DBL_DIG,
		m_Page1.m_dPPScale
		);
	strLines += strFormat;
#endif

	const CString arrStr[] =
	{
		_T("diagonal_scale"),         // 5        T/F  diagonal_scale       0
		_T("debug_model"),            // 6        T/F  debug_model          1
		_T("debug_prep"),             // 7        T/F  debug_prep           2
		_T("debug_set"),              // 8        T/F  debug_set            3
		_T("debug_inverse"),          // 9        T/F  debug_inverse        4
		_T("logfile"),                // 10       T/F  pr.logfile           5
		_T("debug_diffuse_layer"),	  // 12       T/F  debug_diffuse_layer  6
		_T("delay_mass_water"), 	  // 13       T/F  delay_mass_water     7
	};

	ASSERT(sizeof(m_Page1.m_arrValue) / sizeof(m_Page1.m_arrValue[0]) == sizeof(arrStr) / sizeof(arrStr[0]));

	int nCount = sizeof(arrStr) / sizeof(arrStr[0]);

	for (int i = 0; i < nCount; ++i)
	{
		switch (m_Page1.m_arrValue[i])
		{
		case CKPKnobsPg1::AS_IS :
			break;

		case CKPKnobsPg1::AS_TRUE :
			strFormat.Format(_T("%s%4c-%-21s %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				arrStr[i],
				_T("true")
				);
			strLines += strFormat;
			break;

		case CKPKnobsPg1::AS_FALSE :
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

	return strLines + s_strNewLine;
}

void CKSKnobs::Edit(CString& rStr)
{
	try
	{
		PhreeqcI p(rStr);
		p.GetData(this);
	}
	catch (...)
	{
		CString strResource;
		strResource.LoadString(IDS_EXCEPTION_ACCESS_VIOLATION);
		::MessageBox(NULL, strResource, _T("Unhandled Exception"), MB_OK|MB_ICONERROR);
	}
}
