// KSSurfaceMasterSpecies.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "KSSurfaceMasterSpecies.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKSSurfaceMasterSpecies

IMPLEMENT_DYNAMIC(CKSSurfaceMasterSpecies, baseCKSSurfaceMasterSpecies)

CKSSurfaceMasterSpecies::CKSSurfaceMasterSpecies(CWnd* pWndParent)
	 : baseCKSSurfaceMasterSpecies(IDS_PROPSHT_CAPTION32, pWndParent)
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

CKSSurfaceMasterSpecies::~CKSSurfaceMasterSpecies()
{
}


BEGIN_MESSAGE_MAP(CKSSurfaceMasterSpecies, baseCKSSurfaceMasterSpecies)
	//{{AFX_MSG_MAP(CKSSurfaceMasterSpecies)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CKSSurfaceMasterSpecies message handlers


CString CKSSurfaceMasterSpecies::GetString()
{
	/*
	Line 0:  SURFACE_MASTER_SPECIES 
	Line 1a:      Surf_s      Surf_sOH
	Line 1b:      Surf_w      Surf_wOH
	*/

	// Line 0
	CString strLines = _T("SURFACE_MASTER_SPECIES");
	CString strFormat;

	std::list<CMaster>::const_iterator cIter = m_Page1.m_listMaster.begin();
	for (; cIter != m_Page1.m_listMaster.end(); ++cIter)
	{
		// Line 1: surface binding-site name, surface master species 
		strFormat.Format(_T("%s%4c%-13s %-13s"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			(LPCTSTR)cIter->m_strName,
			(LPCTSTR)cIter->m_strMasterSpecies
			);
		strLines += strFormat;
	}

	return strLines + s_strNewLine;
}

void CKSSurfaceMasterSpecies::Edit(CString& rStr)
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

