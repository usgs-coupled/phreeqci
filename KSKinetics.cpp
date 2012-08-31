// KSKinetics.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KSKinetics.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCKSKinetics

IMPLEMENT_DYNAMIC(CCKSKinetics, baseCKSKinetics)

CCKSKinetics::CCKSKinetics(CWnd* pWndParent)
	 : baseCKSKinetics(IDS_PROPSHT_CAPTION13, pWndParent)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
	AddPage(&m_Page1A);
	AddPage(&m_Page2);
}

CCKSKinetics::~CCKSKinetics()
{
}


BEGIN_MESSAGE_MAP(CCKSKinetics, baseCKSKinetics)
	//{{AFX_MSG_MAP(CCKSKinetics)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCKSKinetics message handlers


/***
struct kinetics {
	int n_user;
	int n_user_end;
	char *description;
	int count_comps;
	struct kinetics_comp *comps;
	int count_steps;
	double *steps;
	double step_divide;
	char *units;
	struct elt_list *totals;
 	int rk;
};

struct kinetics_comp {
	char *rate_name;
	struct name_coef *list;
	int count_list;
	double tol;
	LDBLE m;
	LDBLE initial_moles;
	double m0;
	LDBLE moles;
	int count_c_params;
	char **c_params;
	int count_d_params;
 	double *d_params;
};

EXTERNAL struct kinetics *kinetics;
EXTERNAL int count_kinetics;

class CKineticComp
{
	CKineticComp(const struct* kinetics_comp_ptr);

CString m_strRateName;
std::list<CNameCoef> m_listNameCoef;
double m_dTol;
double m_dM;
double m_dInitialMoles;
double m_dM0;
double m_dMoles;
std::list<CString> m_listCParams;
std::list<double> m_listDParams;
}

***/

void CCKSKinetics::Edit(CString& rStr)
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

CString CCKSKinetics::GetString()
{
	/*
	Line 0:  KINETICS 1 Define 3 explicit time steps
	Line 1a: Pyrite
	Line 2a:     -formula FeS2 1.0 FeAs2 0.001
	Line 3a:     -m       1e-3
	Line 4a:     -m0      1e-3
	Line 5a:     -parms   3.0   0.67   .5   -0.11 
	Line 6a:     -tol     1e-9
	Line 1b: Calcite 
	Line 3b:     -m       7.e-4
	Line 4b:     -m0      7.e-4
	Line 5b:     -parms   5.0      0.3
	Line 6b:     -tol     1.e-8
	Line 1c: Organic_C 
	Line 2c:     -formula CH2O(NH3)0.1 0.5 
	Line 3c:     -m       5.e-3
	Line 4c:     -m0      5.e-3
	Line 6c:     -tol     1.e-8
	Line 7:  -steps       100 200 300  # seconds
	Line 8:  -step_divide 100
	Line 9:  -runge_kutta 6
	## Version 2.9
	Line 10: -cvode true
	##
	-cvode_steps    steps
	-cvode_order    order
	-bad_step_max   bad_steps
	*/

	CString strLines;
	CString strFormat;

	// Line 0
	strLines = GetLineZero(CKeyword::K_KINETICS);

	// Line 1
	std::list<CKineticComp>::const_iterator kinetic_iter = m_Page1.m_listComps.begin();
	for (; kinetic_iter != m_Page1.m_listComps.end(); ++kinetic_iter)
	{
		// Line 1
		strFormat.Format(_T("%s%s"), 
			(LPCTSTR)s_strNewLine,
			(LPCTSTR)(*kinetic_iter).m_strRateName
			);
		strLines += strFormat;

		// Line 2
		if (kinetic_iter->m_listNameCoef.begin() != kinetic_iter->m_listNameCoef.end())
		{
			strFormat.Format(_T("%s%4c%-9s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				_T("-formula")
				);
			CString strSubFormat;
			std::list<CNameCoef>::const_iterator nameCoefIter = kinetic_iter->m_listNameCoef.begin();
			for (; nameCoefIter != kinetic_iter->m_listNameCoef.end(); ++nameCoefIter)
			{
				strSubFormat.Format(_T(" %s  %.*g"),
					(LPCTSTR)nameCoefIter->m_strName,
					DBL_DIG, 
					nameCoefIter->m_dCoef
					);
				strFormat += strSubFormat;
			}
			strLines += strFormat;
		}

		// Line 3
		strFormat.Format(_T("%s%4c%-9s %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			_T("-m"),
			DBL_DIG, 
			kinetic_iter->m_dM
			);
		strLines += strFormat;

		// Line 4
		strFormat.Format(_T("%s%4c%-9s %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			_T("-m0"),
			DBL_DIG, 
			kinetic_iter->m_dM0
			);
		strLines += strFormat;

		// Line 5
		if (kinetic_iter->m_listDParams.begin() != kinetic_iter->m_listDParams.end())
		{
			strFormat.Format(_T("%s%4c%-9s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				_T("-parms")
				);
			CString strSubFormat;
			std::list<double>::const_iterator dParamsIter = kinetic_iter->m_listDParams.begin();
			for (; dParamsIter != kinetic_iter->m_listDParams.end(); ++dParamsIter)
			{
				strSubFormat.Format(_T(" %.*g"),
					DBL_DIG, 
					*dParamsIter
					);
				strFormat += strSubFormat;
			}
			strLines += strFormat;
		}

		// Line 6
		strFormat.Format(_T("%s%4c%-9s %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			_T("-tol"),
			DBL_DIG, 
			kinetic_iter->m_dTol
			);
		strLines += strFormat;
	}

	// Line 7
	if (m_Page2.m_nAmountType == CCKPKineticsPg2::TYPE_LINEAR)
	{
		strFormat.Format(_T("%s%-12s %.*g in %d steps # seconds"),
			(LPCTSTR)s_strNewLine,
			_T("-steps"),
			DBL_DIG, 
			m_Page2.m_dLinearAmt,
			m_Page2.m_nLinearSteps
			);
		strLines += strFormat;
	}
	else
	{
		ASSERT(m_Page2.m_nAmountType == CCKPKineticsPg2::TYPE_LIST);
		strFormat.Format(_T("%s%-12s"),
			(LPCTSTR)s_strNewLine,
			_T("-steps")
			);

		CString strSubFormat;
		std::list<double>::const_iterator doubleIter = m_Page2.m_listSteps.begin();
		for (; doubleIter != m_Page2.m_listSteps.end(); ++doubleIter)
		{
			strSubFormat.Format(_T(" %.*g"),
				DBL_DIG, 
				*doubleIter
				);
			strFormat += strSubFormat;
		}
		strLines += strFormat;
	}

	// Line 8
	strFormat.Format(_T("%s%-12s %.*g"),
		(LPCTSTR)s_strNewLine,
		_T("-step_divide"),
		DBL_DIG, 
		m_Page1A.m_dStepDivide
		);
	strLines += strFormat;

	// Line 9
	int nRK = 3;
	switch (m_Page1A.m_nRKType)
	{
	case CCKPKineticsPg1A::RK_1 :
		nRK = 1;
		break;
	case CCKPKineticsPg1A::RK_2 :
		nRK = 2;
		break;
	case CCKPKineticsPg1A::RK_3 :
		nRK = 3;
		break;
	case CCKPKineticsPg1A::RK_6 :
		nRK = 6;
		break;
	default :
		ASSERT(FALSE);
		break;
	}
	strFormat.Format(_T("%s%-12s %d"),
		(LPCTSTR)s_strNewLine,
		_T("-runge_kutta"),
		nRK
		);
	strLines += strFormat;

	strFormat.Format(_T("%s%-12s %d"),
		(LPCTSTR)s_strNewLine,
		_T("-bad_step_max"),
		m_Page1A.m_nODEMethodType == CCKPKineticsPg1A::ODE_CVODE ? m_Page1A.m_nCVMaxBadSteps : m_Page1A.m_nRKMaxBadSteps
		);
	strLines += strFormat;

	// Line 10: -cvode true
	if (m_Page1A.m_nODEMethodType == CCKPKineticsPg1A::ODE_CVODE)
	{
		strFormat.Format(_T("%s%-12s"),
			(LPCTSTR)s_strNewLine,
			_T("-cvode true")
			);
		strLines += strFormat;

		strFormat.Format(_T("%s%-12s %d"),
			(LPCTSTR)s_strNewLine,
			_T("-cvode_steps"),
			m_Page1A.m_nCVSteps
			);
		strLines += strFormat;

		strFormat.Format(_T("%s%-12s %d"),
			(LPCTSTR)s_strNewLine,
			_T("-cvode_order"),
			m_Page1A.m_nCVOrder
			);
		strLines += strFormat;
	}

	return strLines + s_strNewLine;
}
