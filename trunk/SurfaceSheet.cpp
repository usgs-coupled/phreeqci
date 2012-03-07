// SurfaceSheet.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "SurfaceSheet.h"

#include "KeywordLoader2.h"

// CSurfaceSheet

IMPLEMENT_DYNAMIC(CSurfaceSheet, baseCSurfaceSheet)

// COMMENT: {12/22/2009 6:18:52 PM}CSurfaceSheet::CSurfaceSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
// COMMENT: {12/22/2009 6:18:52 PM}	:baseCSurfaceSheet(nIDCaption, pParentWnd, iSelectPage)
// COMMENT: {12/22/2009 6:18:52 PM}{
// COMMENT: {12/22/2009 6:18:52 PM}	AddPage(&this->Page);
// COMMENT: {12/22/2009 6:18:52 PM}}
// COMMENT: {12/22/2009 6:18:52 PM}
// COMMENT: {12/22/2009 6:18:52 PM}CSurfaceSheet::CSurfaceSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
// COMMENT: {12/22/2009 6:18:52 PM}	:baseCSurfaceSheet(pszCaption, pParentWnd, iSelectPage)
// COMMENT: {12/22/2009 6:18:52 PM}{
// COMMENT: {12/22/2009 6:18:52 PM}	AddPage(&this->Page);
// COMMENT: {12/22/2009 6:18:52 PM}}

CSurfaceSheet::CSurfaceSheet(CWnd* pWndParent, CTreeCtrlNode simNode)
: baseCSurfaceSheet(IDS_PROPSHT_CAPTION18, pWndParent)
, DefinedRanges(simNode, true)
// COMMENT: {12/28/2009 9:23:01 PM}, EquilSolutionNum(CSurfaceSheet::N_NONE)
{
	this->SolutionNumberFormat = CUtil::CreateRange(this->SetSolutions, this->DefinedRanges[CKeyword::K_SOLUTION]);
	AddPage(&this->Page);
}

CSurfaceSheet::~CSurfaceSheet()
{
}


BEGIN_MESSAGE_MAP(CSurfaceSheet, baseCSurfaceSheet)
END_MESSAGE_MAP()


// CSurfaceSheet message handlers
CString CSurfaceSheet::GetString()
{
	CString strLines;
	
	// Line 0
	strLines = GetLineZero(CKeyword::K_SURFACE);

	CString strFormat;

	// Line 1
	if (this->Page.n_solution != CSurfacePage::N_NONE)
	{
		strFormat.Format(_T("%s%4c-equilibrate with solution %d"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			this->Page.n_solution
			);
		strLines += strFormat;
	}

	// sites units
	switch (this->Page.sites_units)
	{
	case SITES_DENSITY:
		strFormat.Format(_T("%s%4c-sites DENSITY"),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
		break;

	case SITES_ABSOLUTE:
		break;

	default:
		ASSERT(FALSE);
		break;
	}

	// Line 2
	std::list<CSurfComp>::const_iterator const_iter = this->Page.ListSurfComp[CSurfacePage::TI_GENERAL].begin();
	std::set<CString> setSurfaces;
	for ( ; const_iter != this->Page.ListSurfComp[CSurfacePage::TI_GENERAL].end(); ++const_iter)
	{
		CString strSurface = (*const_iter).m_strFormula.SpanExcluding(_T("_"));
		std::pair<std::set<CString>::iterator, bool> ans = setSurfaces.insert(strSurface);
		if (ans.second)
		{
// COMMENT: {1/27/2010 10:30:11 PM}			strFormat.Format(_T("%s%4c%-7s    %-6lg    %-6lg    %-6lg"),
// COMMENT: {1/27/2010 10:30:11 PM}				(LPCTSTR)s_strNewLine,
// COMMENT: {1/27/2010 10:30:11 PM}				_T(' '),
// COMMENT: {1/27/2010 10:30:11 PM}				(*const_iter).m_strFormula,
// COMMENT: {1/27/2010 10:30:11 PM}				(*const_iter).m_dMoles,
// COMMENT: {1/27/2010 10:30:11 PM}				(*const_iter).m_dSpecific_area,
// COMMENT: {1/27/2010 10:30:11 PM}				(*const_iter).m_dGrams
// COMMENT: {1/27/2010 10:30:11 PM}				);
			strFormat.Format(_T("%s%4c%-7s    %-6lg"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(*const_iter).m_strFormula,
				(*const_iter).m_dMoles
				);
			if (this->Page.type != NO_EDL)
			{
				CString extra;
				extra.Format(_T("    %-6lg    %-6lg"),
					(*const_iter).m_dSpecific_area,
					(*const_iter).m_dGrams
					);
				strFormat += extra;
				if (this->Page.transport)
				{
					// this assumes that Dw is not used when NO_EDL is set
					CString extra;
					if ((*const_iter).m_dDw >= 0)
					{
						extra.Format(_T("%8cDw %-6lg"),
							_T(' '),
							(*const_iter).m_dDw
							);
					}
					strFormat += extra;
				}
			}
			strFormat.TrimRight();
			strLines += strFormat;

			if (this->Page.type == CD_MUSIC)
			{
				strFormat.Format(_T("%s%8c-capacitance %-6lg %-6lg"),
					(LPCTSTR)s_strNewLine,
					_T(' '),
					(*const_iter).m_dCapacitance0,
					(*const_iter).m_dCapacitance1
					);
				strFormat.TrimRight();
				strLines += strFormat;
			}
		}
		else
		{
			strFormat.Format(_T("%s%4c%-7s    %-6lg"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(*const_iter).m_strFormula,
				(*const_iter).m_dMoles
				);
			strFormat.TrimRight();
			strLines += strFormat;
		}
	}

	// Line 3
	const_iter = this->Page.ListSurfComp[CSurfacePage::TI_EQUILIBRIUM_PHASES].begin();
	setSurfaces.clear();
	for ( ; const_iter != this->Page.ListSurfComp[CSurfacePage::TI_EQUILIBRIUM_PHASES].end(); ++const_iter)
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
	const_iter = this->Page.ListSurfComp[CSurfacePage::TI_KINETIC_REACTANTS].begin();
	setSurfaces.clear();
	for ( ; const_iter != this->Page.ListSurfComp[CSurfacePage::TI_KINETIC_REACTANTS].end(); ++const_iter)
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

	
	switch (this->Page.type)
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
	switch (this->Page.dl_type)
	{
	case NO_DL:
		break;

	case DONNAN_DL:
		if (this->Page.DT == CSurfacePage::DT_DEBYE_LENGTHS)
		{
			if (this->Page.DDL_viscosity != 1.)
			{
				strFormat.Format(_T("%s%4c-donnan debye_lengths %.*g limit_ddl %.*g viscosity %.*g"),
					(LPCTSTR)s_strNewLine,
					_T(' '),
					DBL_DIG,
					this->Page.debye_lengths,
					DBL_DIG,
					this->Page.DDL_limit,
					DBL_DIG,
					this->Page.DDL_viscosity
					);
			}
			else
			{
				strFormat.Format(_T("%s%4c-donnan debye_lengths %.*g limit_ddl %.*g"),
					(LPCTSTR)s_strNewLine,
					_T(' '),
					DBL_DIG,
					this->Page.debye_lengths,
					DBL_DIG,
					this->Page.DDL_limit
					);
			}
		}
		else
		{
			ASSERT(this->Page.DT == CSurfacePage::DT_THICKNESS); // new type?
			if (this->Page.DDL_viscosity != 1.)
			{
				strFormat.Format(_T("%s%4c-donnan %.*g viscosity %.*g"),
					(LPCTSTR)s_strNewLine,
					_T(' '),
					DBL_DIG,
					this->Page.thickness,
					DBL_DIG,
					this->Page.DDL_viscosity
					);
			}
			else
			{
				strFormat.Format(_T("%s%4c-donnan %.*g"),
					(LPCTSTR)s_strNewLine,
					_T(' '),
					DBL_DIG,
					this->Page.thickness
					);
			}
		}
		strLines += strFormat;

		if (this->Page.only_counter_ions)
		{
			strFormat.Format(_T("%s%4c-only_counter_ions"),
				(LPCTSTR)s_strNewLine,
				_T(' ')
				);
			strLines += strFormat;
		}
		break;

	case BORKOVEK_DL:
		strFormat.Format(_T("%s%4c-diffuse_layer %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			DBL_DIG,
			this->Page.thickness
			);
		strLines += strFormat;
		if (this->Page.only_counter_ions)
		{
			strFormat.Format(_T("%s%4c-only_counter_ions"),
				(LPCTSTR)s_strNewLine,
				_T(' ')
				);
			strLines += strFormat;
		}
		break;
	}

	return strLines + s_strNewLine;
}

void CSurfaceSheet::Edit(CString& rStr)
{
	CKeywordLoader2 keywordLoader2(rStr);

	struct surface* surface_ptr = &surface[0];

	// Surface number
	this->m_n_user      = surface_ptr->n_user;
	this->m_n_user_end  = surface_ptr->n_user_end;
	this->m_strDesc     = surface_ptr->description;

	this->Page.dl_type           = surface_ptr->dl_type;
	this->Page.type              = surface_ptr->type;
	this->Page.thickness         = surface_ptr->thickness;
	this->Page.sites_units       = surface_ptr->sites_units;
	this->Page.transport         = FALSE;


	// solution equilibration
	if (surface_ptr->solution_equilibria)
	{
		this->Page.n_solution = surface_ptr->n_solution;
		ASSERT(this->Page.n_solution > 0);
	}
	else
	{
		this->Page.n_solution = CSurfacePage::N_NONE;
	}

	// counter ions only
	this->Page.only_counter_ions = surface_ptr->only_counter_ions;

	// viscosity
	this->Page.DDL_viscosity = surface_ptr->DDL_viscosity;

	// Donnan (thickness or debye)
	this->Page.DT = CSurfacePage::DT_THICKNESS;
	if (surface_ptr->debye_lengths != 0.0)
	{
		this->Page.DT            = CSurfacePage::DT_DEBYE_LENGTHS;
		this->Page.debye_lengths = surface_ptr->debye_lengths;
		this->Page.DDL_limit     = surface_ptr->DDL_limit;
	}



	if (surface_ptr->count_charge > 0)
	{
		for (int i = 0; i < surface_ptr->count_comps; ++i)
		{
			const struct surface_comp* surface_comp_ptr = &(surface_ptr->comps[i]);
			CSurfComp surfComp(surface_ptr, surface_comp_ptr);
			if (!surfComp.m_strRate_name.IsEmpty())
			{
				ASSERT( surfComp.m_strPhase_name.IsEmpty() == TRUE );
				this->Page.ListSurfComp[CSurfacePage::TI_KINETIC_REACTANTS].push_back(surfComp);
			}
			else if (!surfComp.m_strPhase_name.IsEmpty())
			{
				this->Page.ListSurfComp[CSurfacePage::TI_EQUILIBRIUM_PHASES].push_back(surfComp);
			}
			else
			{
				this->Page.ListSurfComp[CSurfacePage::TI_GENERAL].push_back(surfComp);
			}

			// check for transport
			if (surface_ptr->transport)
			{
				this->Page.transport = TRUE;
			}
		}
	}

}
