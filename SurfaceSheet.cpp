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
	case cxxSurface::SITES_DENSITY:
		strFormat.Format(_T("%s%4c-sites DENSITY"),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
		break;

	case cxxSurface::SITES_ABSOLUTE:
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
			if (this->Page.type != cxxSurface::NO_EDL)
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

			if (this->Page.type == cxxSurface::CD_MUSIC)
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
	case cxxSurface::NO_EDL:
		strFormat.Format(_T("%s%4c-no_edl"),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
		break;

	case cxxSurface::DDL:
		// do nothing
		break;

	case cxxSurface::CD_MUSIC:
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
	case cxxSurface::NO_DL:
		break;

	case cxxSurface::DONNAN_DL:
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

	case cxxSurface::BORKOVEK_DL:
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
	PhreeqcI p(rStr);
	p.GetData(this);
}
