// KSSolutionMasterSpecies.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "KSSolutionMasterSpecies.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKSSolutionMasterSpecies

IMPLEMENT_DYNAMIC(CKSSolutionMasterSpecies, baseCKSSolutionMasterSpecies)

CKSSolutionMasterSpecies::CKSSolutionMasterSpecies(CWnd* pWndParent)
	 : baseCKSSolutionMasterSpecies(IDS_PROPSHT_CAPTION30, pWndParent)
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

CKSSolutionMasterSpecies::~CKSSolutionMasterSpecies()
{
}


BEGIN_MESSAGE_MAP(CKSSolutionMasterSpecies, baseCKSSolutionMasterSpecies)
	//{{AFX_MSG_MAP(CKSSolutionMasterSpecies)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CKSSolutionMasterSpecies message handlers

CString CKSSolutionMasterSpecies::GetString()
{
	/*
	Line 0:  SOLUTION_MASTER_SPECIES 
	Line 1a:     H            H+      -1.0   1.008          1.008
	Line 1b:     H(0)         H2      0.0    1.008
	Line 1c:     S            SO4-2   0.0    SO4            32.06
	Line 1d:     S(6)         SO4-2   0.0    SO4
	Line 1e:     S(-2)        HS-     1.0    S
	Line 1f:     Alkalinity   CO3-2   1.0    Ca0.5(CO3)0.5  50.04
	*/

	// Line 0
	CString strLines = _T("SOLUTION_MASTER_SPECIES");
	CString strFormat;

	std::list<CMaster>::const_iterator cIter = m_Page1.m_listMaster.begin();
	for (; cIter != m_Page1.m_listMaster.end(); ++cIter)
	{
		// element name, master species, alkalinity
		strFormat.Format(_T("%s%4c%-13s %-13s %4.*g    "),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			(LPCTSTR)cIter->m_strName,
			(LPCTSTR)cIter->m_strMasterSpecies,
			DBL_DIG,
			cIter->m_dAlk
			);
		strLines += strFormat;

		// ( gram formula weight or formula )
		if (cIter->m_strFormula.IsEmpty())
		{
			strFormat.Format(_T(" %-*.*g"),
				DBL_DIG,
				DBL_DIG,
				cIter->m_dGFW
				);
		}
		else
		{
			strFormat.Format(_T(" %-*s"),
				DBL_DIG,
				cIter->m_strFormula
				);
		}
		strLines += strFormat;

		// gram formula weight of element
		if (cIter->m_bPrimary)
		{
			strFormat.Format(_T(" %.*g"),
				DBL_DIG,
				cIter->m_dGFWElement
				);
			strLines += strFormat;
		}
	}

	return strLines + s_strNewLine;
}

void CKSSolutionMasterSpecies::Edit(CString& rStr)
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
