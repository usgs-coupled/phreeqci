// CKSSurface.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CKSSurface.h"

#include "KeywordLoader2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCKSSurface

IMPLEMENT_DYNAMIC(CCKSSurface, baseCKSSurface)

CCKSSurface::CCKSSurface(CWnd* pWndParent, CTreeCtrlNode simNode)
	 : baseCKSSurface(IDS_PROPSHT_CAPTION18, pWndParent)
	 , m_ranges(simNode, true)
	 , m_strNumFormat(_T("%d"))
{
	m_bSolution_equilibria = false;
	m_nEquilSolutionNum = N_NONE;
	m_strNumFormat = CUtil::CreateRange(m_setSolutions, m_ranges[CKeyword::K_SOLUTION]);

// COMMENT: {10/11/2006 8:56:07 PM}	m_bDiffuseLayer = false;
// COMMENT: {10/11/2006 8:56:07 PM}	m_bDonnan = false;
	m_dlType = NO_DL;


	m_dThickness = 1e-8;
// COMMENT: {10/5/2006 9:28:40 PM}	m_bNoEDL = false;
	m_surfaceType = DDL;
	m_bOnlyCounterIons = false;

	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
	AddPage(&m_Page2);
	AddPage(&m_Page3);
}

CCKSSurface::~CCKSSurface()
{
}


BEGIN_MESSAGE_MAP(CCKSSurface, baseCKSSurface)
	//{{AFX_MSG_MAP(CCKSSurface)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCKSSurface message handlers

/////////////////////////////////////////////////////////////////////////////
// CSurfaceSheet message handlers

CString CCKSSurface::GetString()
{
	/*
	Line 0a: SURFACE 1 Surface in equilibrium with solution 10
	Line 1a: -equilibrate with solution 10
	Line 2a: Surfa_w 1.0 1000. 0.33
	Line 2b: Surfa_s 0.01
	Line 2c: Surfb 0.5 1000. 0.33
	Line 0b: SURFACE 3 Sites related to pure phase and kinetic reactant
	Line 1b: -equilibrate with solution 10
	Line 3a: Surfc_wOH Fe(OH)3(a) equilibrium_phase 0.1 1e5
	Line 3b: Surfc_sOH Fe(OH)3(a) equilibrium_phase 0.001
	Line 3b: Surfd_sOH Al(OH)3(a) kinetic 0.001 2e4
	Line 4: -no_edl
	Line 0c: SURFACE 5 Explicit calculation of diffuse layer composition
	Line 1c: -equilibrate with solution 10
	Line 2d: Surfe_w 0.5 1000. 0.33
	Line 5: -diffuse_layer 2e-8
	Line 6: -only_counter_ions

	New:
	-donnan
	-cd_music
	*/

	CString strLines;
	
	// Line 0
	strLines = GetLineZero(CKeyword::K_SURFACE);

	CString strFormat;

	// Line 1
	if (m_nEquilSolutionNum != -1)
	{
		strFormat.Format(_T("%s%4c-equilibrate with solution %d"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			m_nEquilSolutionNum
			);
		strLines += strFormat;
	}

	// Line 2
	std::list<CSurfComp>::const_iterator const_iter = m_Page1.m_listSurfComp.begin();
	std::set<CString> setSurfaces;
	for ( ; const_iter != m_Page1.m_listSurfComp.end(); ++const_iter)
	{
		CString strSurface = (*const_iter).m_strFormula.SpanExcluding(_T("_"));
		std::pair<std::set<CString>::iterator, bool> ans = setSurfaces.insert(strSurface);
		if (ans.second)
		{
			strFormat.Format(_T("%s%4c%-7s %-6lg %-6lg %-6lg"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(*const_iter).m_strFormula,
				(*const_iter).m_dMoles,
				(*const_iter).m_dSpecific_area,
				(*const_iter).m_dGrams
				);
		}
		else
		{
			strFormat.Format(_T("%s%4c%-7s %-6lg"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(*const_iter).m_strFormula,
				(*const_iter).m_dMoles
				);
		}
		strFormat.TrimRight();
		strLines += strFormat;
	}

	// Line 3
	const_iter = m_Page2.m_listSurfComp.begin();
	setSurfaces.clear();
	for ( ; const_iter != m_Page2.m_listSurfComp.end(); ++const_iter)
	{
		CString strSurface = (*const_iter).m_strFormula.SpanExcluding(_T("_"));
		std::pair<std::set<CString>::iterator, bool> ans = setSurfaces.insert(strSurface);
		if (ans.second)
		{
			strFormat.Format(_T("%s%4c%-7s %-15s equilibrium_phase %-6lg %-6lg"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(*const_iter).m_strFormula,
				(*const_iter).m_strPhase_name,
				(*const_iter).m_dPhase_proportion,
				(*const_iter).m_dSpecific_area
				);
		}
		else
		{
			strFormat.Format(_T("%s%4c%-7s %-15s equilibrium_phase %-6lg"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(*const_iter).m_strFormula,
				(*const_iter).m_strPhase_name,
				(*const_iter).m_dPhase_proportion
				);
		}
		strFormat.TrimRight();
		strLines += strFormat;
	}

	// Line 3
	const_iter = m_Page3.m_listSurfComp.begin();
	setSurfaces.clear();
	for ( ; const_iter != m_Page3.m_listSurfComp.end(); ++const_iter)
	{
		CString strSurface = (*const_iter).m_strFormula.SpanExcluding(_T("_"));
		std::pair<std::set<CString>::iterator, bool> ans = setSurfaces.insert(strSurface);
		if (ans.second)
		{
			strFormat.Format(_T("%s%4c%-7s %-15s kinetic           %-6lg %-6lg"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(*const_iter).m_strFormula,
				(*const_iter).m_strRate_name,
				(*const_iter).m_dPhase_proportion,
				(*const_iter).m_dSpecific_area
				);
		}
		else
		{
			strFormat.Format(_T("%s%4c%-7s %-15s kinetic           %-6lg"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(*const_iter).m_strFormula,
				(*const_iter).m_strRate_name,
				(*const_iter).m_dPhase_proportion
				);
		}
		strFormat.TrimRight();
		strLines += strFormat;
	}

	switch (this->m_surfaceType)
	{
	case NO_EDL:
		strFormat.Format(_T("%s%4c-no_edl"),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
		break;

	case DDL:
		// do nothing
		break;

	case CD_MUSIC:
		strFormat.Format(_T("%s%4c-cd_music"),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
		break;
	}

	// Lines 4, 5, and 6
	switch (this->m_dlType)
	{
	case NO_DL:
		break;

	case BORKOVEK_DL:
		strFormat.Format(_T("%s%4c-diffuse_layer %g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			m_dThickness
			);
		strLines += strFormat;
		if (m_bOnlyCounterIons)
		{
			strFormat.Format(_T("%s%4c-only_counter_ions"),
				(LPCTSTR)s_strNewLine,
				_T(' ')
				);
			strLines += strFormat;
		}
		break;

	case DONNAN_DL:
		strFormat.Format(_T("%s%4c-donnan %g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			m_dThickness
			);
		strLines += strFormat;
		break;
	}

// COMMENT: {10/11/2006 8:57:31 PM}	if (m_bDiffuseLayer)
// COMMENT: {10/11/2006 8:57:31 PM}	{
// COMMENT: {10/11/2006 8:57:31 PM}		strFormat.Format(_T("%s%4c-diffuse_layer %g"),
// COMMENT: {10/11/2006 8:57:31 PM}			(LPCTSTR)s_strNewLine,
// COMMENT: {10/11/2006 8:57:31 PM}			_T(' '),
// COMMENT: {10/11/2006 8:57:31 PM}			m_dThickness
// COMMENT: {10/11/2006 8:57:31 PM}			);
// COMMENT: {10/11/2006 8:57:31 PM}		strLines += strFormat;
// COMMENT: {10/11/2006 8:57:31 PM}		if (m_bOnlyCounterIons)
// COMMENT: {10/11/2006 8:57:31 PM}		{
// COMMENT: {10/11/2006 8:57:31 PM}			strFormat.Format(_T("%s%4c-only_counter_ions"),
// COMMENT: {10/11/2006 8:57:31 PM}				(LPCTSTR)s_strNewLine,
// COMMENT: {10/11/2006 8:57:31 PM}				_T(' ')
// COMMENT: {10/11/2006 8:57:31 PM}				);
// COMMENT: {10/11/2006 8:57:31 PM}			strLines += strFormat;
// COMMENT: {10/11/2006 8:57:31 PM}		}
// COMMENT: {10/11/2006 8:57:31 PM}	}
// COMMENT: {10/5/2006 9:29:34 PM}	else if (m_bNoEDL)
// COMMENT: {10/5/2006 9:29:34 PM}	{
// COMMENT: {10/5/2006 9:29:34 PM}		strFormat.Format(_T("%s%4c-no_edl"),
// COMMENT: {10/5/2006 9:29:34 PM}			(LPCTSTR)s_strNewLine,
// COMMENT: {10/5/2006 9:29:34 PM}			_T(' ')
// COMMENT: {10/5/2006 9:29:34 PM}			);
// COMMENT: {10/5/2006 9:29:34 PM}		strLines += strFormat;
// COMMENT: {10/5/2006 9:29:34 PM}	}
	
	return strLines + s_strNewLine;
}

void CCKSSurface::Edit(CString& rStr)
{
	CKeywordLoader2 keywordLoader2(rStr);

	Fill();
}

void CCKSSurface::Fill()
{
	struct surface* surface_ptr = &surface[0];

	// Surface number
	m_n_user     = surface_ptr->n_user;
	m_n_user_end = surface_ptr->n_user_end;
	m_strDesc    = surface_ptr->description;
	
	// solution equilibration
	if (surface_ptr->solution_equilibria) 
	{
		m_nEquilSolutionNum = surface_ptr->n_solution;
	}
	
	// diffuse_layer equilibria
	m_dlType = surface_ptr->dl_type;

	m_bOnlyCounterIons = false;
	if (m_dlType == BORKOVEK_DL || m_dlType == DONNAN_DL)
	{
		m_dThickness       = surface_ptr->thickness;
		m_bOnlyCounterIons = surface_ptr->only_counter_ions != 0;
	}

// COMMENT: {10/11/2006 8:54:28 PM}	if (surface_ptr->diffuse_layer == TRUE)
// COMMENT: {10/11/2006 8:54:28 PM}	{
// COMMENT: {10/11/2006 8:54:28 PM}		m_bDiffuseLayer    = true;
// COMMENT: {10/11/2006 8:54:28 PM}		m_dThickness       = surface_ptr->thickness;
// COMMENT: {10/11/2006 8:54:28 PM}		m_bOnlyCounterIons = surface_ptr->only_counter_ions != 0;
// COMMENT: {10/11/2006 8:54:28 PM}	}
// COMMENT: {10/11/2006 8:54:28 PM}	else
// COMMENT: {10/11/2006 8:54:28 PM}	{
// COMMENT: {10/11/2006 8:54:28 PM}		m_bDiffuseLayer = false;
// COMMENT: {10/11/2006 8:54:28 PM}	}

	m_surfaceType = surface_ptr->type;
// COMMENT: {10/11/2006 8:57:43 PM}	m_bDonnan = (surface_ptr->donnan != 0);


// COMMENT: {10/4/2006 10:52:25 PM}	// no_edl
// COMMENT: {10/4/2006 10:52:25 PM}	if (surface_ptr->edl == FALSE && surface_ptr->diffuse_layer == FALSE)
// COMMENT: {10/4/2006 10:52:25 PM}	{
// COMMENT: {10/4/2006 10:52:25 PM}		m_bNoEDL = true;
// COMMENT: {10/4/2006 10:52:25 PM}	}
// COMMENT: {10/4/2006 10:52:25 PM}	else
// COMMENT: {10/4/2006 10:52:25 PM}	{
// COMMENT: {10/4/2006 10:52:25 PM}		m_bNoEDL = false;
// COMMENT: {10/4/2006 10:52:25 PM}	}
// COMMENT: {10/4/2006 10:52:25 PM}
	if (surface_ptr->count_charge > 0)
	{
		for (int i = 0; i < surface_ptr->count_comps; ++i)
		{
			const struct surface_comp* surface_comp_ptr = &(surface_ptr->comps[i]);
			CSurfComp surfComp(surface_ptr, surface_comp_ptr);
			if (!surfComp.m_strRate_name.IsEmpty())
			{
				ASSERT( surfComp.m_strPhase_name.IsEmpty() == TRUE );
				m_Page3.m_listSurfComp.push_back(surfComp);
			}
			else if (!surfComp.m_strPhase_name.IsEmpty())
			{
				m_Page2.m_listSurfComp.push_back(surfComp);
			}
			else
			{
				m_Page1.m_listSurfComp.push_back(surfComp);
			}
		}
	}
}

BOOL CCKSSurface::OnInitDialog() 
{
	BOOL bResult = baseCKSSurface::OnInitDialog();
	
	// Add your specialized code here
	CTabCtrl* pTabCtrl =  GetTabControl();
	ASSERT( pTabCtrl != NULL );

	TCITEM item;
	item.mask = TCIF_TEXT;

	item.pszText = "&General";
	pTabCtrl->SetItem(0, &item);
	item.pszText = "&Equilibrium phases (Advanced)";
	pTabCtrl->SetItem(1, &item);
	item.pszText = "&Kinetic reactants (Advanced)";
	pTabCtrl->SetItem(2, &item);
	
	return bResult;
}
