// KeywordPageListItems.cpp: implementation of the CPurePhase class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phrqtype.h"          // LDBLE
#include "phreeqci2.h"
#include "KeywordPageListItems.h"

#include "phreeqc3/src/ExchComp.h"
#include "phreeqc3/src/PPassemblageComp.h"
#include "phreeqc3/src/GasComp.h"

#include "KPTransportPg1.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CPurePhase Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPurePhase::CPurePhase()
: m_strName(_T(""))
, m_dSI(0.0)
, m_dAmount(0.0)
, m_strAlt(_T(""))
, m_bDissolveOnly(false)
, m_bPrecipOnly(false)
, m_bForceEquality(false)
{
}

#if 0
CPurePhase::CPurePhase(const struct pure_phase *pure_phase_ptr)
: m_strName(pure_phase_ptr->name)
, m_dSI(pure_phase_ptr->si)
, m_dAmount(pure_phase_ptr->moles)
, m_strAlt(pure_phase_ptr->add_formula)
, m_bDissolveOnly(pure_phase_ptr->dissolve_only != 0)
, m_bPrecipOnly(pure_phase_ptr->precipitate_only != 0)
, m_bForceEquality(pure_phase_ptr->force_equality != 0)
{
}

#else

CPurePhase::CPurePhase(const cxxPPassemblageComp *ppComp)
: m_strName(ppComp->Get_name().c_str())
, m_dSI(ppComp->Get_si())
, m_dAmount(ppComp->Get_moles())
, m_strAlt(ppComp->Get_add_formula().c_str())
, m_bDissolveOnly(ppComp->Get_dissolve_only())
, m_bPrecipOnly(ppComp->Get_precipitate_only())
, m_bForceEquality(ppComp->Get_force_equality())
{
}
#endif

CPurePhase::~CPurePhase()
{
}

//////////////////////////////////////////////////////////////////////
// CGasComp Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGasComp::CGasComp()
: m_strName(_T(""))
, m_dP_Read(std::numeric_limits<double>::signaling_NaN())
{
	ASSERT(std::numeric_limits<double>::has_signaling_NaN == true);
}

#if 0
CGasComp::CGasComp(const struct gas_comp* gas_comp_ptr)
: m_strName(gas_comp_ptr->name)
, m_dP_Read(gas_comp_ptr->p_read == NAN ? std::numeric_limits<double>::signaling_NaN() : gas_comp_ptr->p_read)
{
	ASSERT(std::numeric_limits<double>::has_signaling_NaN == true);
}
#else
CGasComp::CGasComp(const cxxGasComp* gasComp)
: m_strName(gasComp->Get_phase_name().c_str())
, m_dP_Read(gasComp->Get_p_read() == NAN ? std::numeric_limits<double>::signaling_NaN() : gasComp->Get_p_read())
{
	ASSERT(std::numeric_limits<double>::has_signaling_NaN == true);
}
#endif

CGasComp::~CGasComp()
{
}

//////////////////////////////////////////////////////////////////////
// CExchComp Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExchComp::CExchComp()
: m_dPhase_proportion(0.0)
, m_ldMoles(0.0)
, m_strFormula(_T(""))
, m_strPhase_name(_T(""))
, m_strRate_name(_T(""))
{
}

#if 0
CExchComp::CExchComp(const struct exch_comp* exch_comp_ptr)
: m_dPhase_proportion(exch_comp_ptr->phase_proportion)
, m_ldMoles(exch_comp_ptr->moles)
{
	m_strFormula        = (exch_comp_ptr->formula    == NULL) ? _T("") : exch_comp_ptr->formula;
	m_strPhase_name     = (exch_comp_ptr->phase_name == NULL) ? _T("") : exch_comp_ptr->phase_name;
	m_strRate_name      = (exch_comp_ptr->rate_name  == NULL) ? _T("") : exch_comp_ptr->rate_name;
}

#else

CExchComp::CExchComp(const cxxExchComp* exchComp)
: m_dPhase_proportion(exchComp->Get_phase_proportion())
{
	m_strFormula    = exchComp->Get_formula().c_str();
	m_strPhase_name = exchComp->Get_phase_name().c_str();
	m_strRate_name  = exchComp->Get_rate_name().c_str();

	if (!m_strRate_name.IsEmpty())
	{
		ASSERT(m_strPhase_name.IsEmpty());
	}
	else if (!m_strPhase_name.IsEmpty())
	{
		ASSERT(m_strFormula.IsEmpty());
	}
	else
	{
		ASSERT(exchComp->Get_totals().size() == 1);
		cxxNameDouble::const_iterator it = exchComp->Get_totals().begin();
		if (it != exchComp->Get_totals().end())
		{
			m_ldMoles = (*it).second;
		}
	}
}
#endif

CExchComp::~CExchComp()
{
}

//////////////////////////////////////////////////////////////////////
// CNameCoef Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// COMMENT: {2/16/2012 5:45:07 PM}CNameCoef::CNameCoef()
// COMMENT: {2/16/2012 5:45:07 PM}: m_strName(_T(""))
// COMMENT: {2/16/2012 5:45:07 PM}, m_dCoef(std::numeric_limits<double>::signaling_NaN())
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	_ASSERTE( std::numeric_limits<double>::has_signaling_NaN );
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CNameCoef::CNameCoef(const struct name_coef* name_coef_ptr)
// COMMENT: {2/16/2012 5:45:07 PM}: m_strName(name_coef_ptr->name)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	_ASSERTE( std::numeric_limits<double>::has_signaling_NaN );
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	m_dCoef = (name_coef_ptr->coef == NAN) ?
// COMMENT: {2/16/2012 5:45:07 PM}		std::numeric_limits<double>::signaling_NaN() : name_coef_ptr->coef;
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}// use implicit copy ctor
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CNameCoef::~CNameCoef()
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}}

//////////////////////////////////////////////////////////////////////
// CConc Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// COMMENT: {2/16/2012 5:45:07 PM}CConc::CConc()
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	m_strDesc    = _T("");		
// COMMENT: {2/16/2012 5:45:07 PM}	m_dInputConc = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	m_strUnits   = _T("(Default)");
// COMMENT: {2/16/2012 5:45:07 PM}	m_strAs      = _T("");		
// COMMENT: {2/16/2012 5:45:07 PM}	m_dGFW       = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	m_strRedox   = _T("");
// COMMENT: {2/16/2012 5:45:07 PM}	m_dPhaseSI   = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	m_strPhase   = _T("");
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CConc::CConc(const struct solution* solution_ptr, const struct conc* conc_ptr)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	// element list
// COMMENT: {2/16/2012 5:45:07 PM}	ASSERT(conc_ptr->description != NULL);
// COMMENT: {2/16/2012 5:45:07 PM}	m_strDesc = conc_ptr->description;
// COMMENT: {2/16/2012 5:45:07 PM}		
// COMMENT: {2/16/2012 5:45:07 PM}	// concentration
// COMMENT: {2/16/2012 5:45:07 PM}	m_dInputConc = conc_ptr->input_conc;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// [units]
// COMMENT: {2/16/2012 5:45:07 PM}	if (conc_ptr->units != NULL && conc_ptr->units != solution_ptr->units)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		m_strUnits = conc_ptr->units;		
// COMMENT: {2/16/2012 5:45:07 PM}		m_strUnits.Replace(_T("mg/kgs"), _T("ppm"));
// COMMENT: {2/16/2012 5:45:07 PM}		m_strUnits.Replace(_T("ug/kgs"), _T("ppb"));
// COMMENT: {2/16/2012 5:45:07 PM}		m_strUnits.Replace(_T("g/kgs"),  _T("ppt"));
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	else
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		m_strUnits = _T("(Default)");
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// ([as formula] or [gfw gfw])
// COMMENT: {2/16/2012 5:45:07 PM}	if (conc_ptr->as != NULL)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		m_strAs = conc_ptr->as;
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	if (conc_ptr->gfw != 0.0)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		m_dGFW = conc_ptr->gfw;
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	else
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		m_dGFW = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// [redox couple]
// COMMENT: {2/16/2012 5:45:07 PM}	if (conc_ptr->n_pe != solution_ptr->default_pe)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		ASSERT(solution_ptr->pe[conc_ptr->n_pe].name != NULL);
// COMMENT: {2/16/2012 5:45:07 PM}		m_strRedox = solution_ptr->pe[conc_ptr->n_pe].name;
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// [(charge or phase name [saturation index])]
// COMMENT: {2/16/2012 5:45:07 PM}	m_dPhaseSI = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	if (conc_ptr->equation_name != NULL)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		m_strPhase = conc_ptr->equation_name;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}		if (m_strPhase.CompareNoCase(_T("charge")) != 0)
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			m_dPhaseSI = conc_ptr->phase_si;
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CConc::~CConc()
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CConc CConc::Create(LPCTSTR psz)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	/*
// COMMENT: {2/16/2012 5:45:07 PM}	Test cases
// COMMENT: {2/16/2012 5:45:07 PM}	mg/kgw as HCO3
// COMMENT: {2/16/2012 5:45:07 PM}	ppm
// COMMENT: {2/16/2012 5:45:07 PM}	mg/liter
// COMMENT: {2/16/2012 5:45:07 PM}	mg/L
// COMMENT: {2/16/2012 5:45:07 PM}	as HCO3
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	"     as SO4"
// COMMENT: {2/16/2012 5:45:07 PM}	"      as S  "
// COMMENT: {2/16/2012 5:45:07 PM}	"    as HCO3      CO2(g)     -3.5"
// COMMENT: {2/16/2012 5:45:07 PM}	"     ug/kgs as Fe S(6)/S(-2) Pyrite"
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	*/
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	CConc aConc;
// COMMENT: {2/16/2012 5:45:07 PM}	CString string(psz);
// COMMENT: {2/16/2012 5:45:07 PM}	string.TrimLeft();
// COMMENT: {2/16/2012 5:45:07 PM}	string.TrimRight();
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	aConc.m_strUnits = GetUnits(string);
// COMMENT: {2/16/2012 5:45:07 PM}	aConc.m_strAs    = GetAs(string);
// COMMENT: {2/16/2012 5:45:07 PM}	aConc.m_strGFW = GetGFW(string);
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	if (!string.IsEmpty())
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		aConc.m_strRedox = GetRedox(string);
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	if (!string.IsEmpty())
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		GetPhase(string, aConc);
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	ASSERT(string.IsEmpty());
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	return aConc;
// COMMENT: {2/16/2012 5:45:07 PM}}

//////////////////////////////////////////////////////////////////////
// CIsotope Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// COMMENT: {2/16/2012 5:45:07 PM}CIsotope::CIsotope()
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	m_dIsotopeNumber    = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	m_strEltName        = _T("");
// COMMENT: {2/16/2012 5:45:07 PM}	m_strName           = _T("");
// COMMENT: {2/16/2012 5:45:07 PM}	m_dRatio            = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	m_dRatioUncertainty = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}CIsotope::CIsotope(const struct isotope* isotope_ptr)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	m_dIsotopeNumber    = isotope_ptr->isotope_number;
// COMMENT: {2/16/2012 5:45:07 PM}	m_strEltName        = isotope_ptr->elt_name;
// COMMENT: {2/16/2012 5:45:07 PM}	m_strName.Format(_T("%d%s"), (int)m_dIsotopeNumber, m_strEltName);
// COMMENT: {2/16/2012 5:45:07 PM}	m_dRatio            = isotope_ptr->ratio;
// COMMENT: {2/16/2012 5:45:07 PM}	m_dRatioUncertainty = (isotope_ptr->ratio_uncertainty != NAN) ?
// COMMENT: {2/16/2012 5:45:07 PM}		isotope_ptr->ratio_uncertainty : std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}CIsotope::CIsotope(const struct iso* iso_ptr)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	m_strEltName        = _T("");
// COMMENT: {2/16/2012 5:45:07 PM}	m_strName           = iso_ptr->name;
// COMMENT: {2/16/2012 5:45:07 PM}	m_dIsotopeNumber    = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	m_dRatio            = (iso_ptr->value != NAN) ? iso_ptr->value : std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	m_dRatioUncertainty = (iso_ptr->uncertainty != NAN) ? iso_ptr->uncertainty : std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}}
//////////////////////////////////////////////////////////////////////
// CInvIsotope Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// COMMENT: {2/16/2012 5:45:07 PM}CInvIsotope::CInvIsotope()
// COMMENT: {2/16/2012 5:45:07 PM}: CIsotope()
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}CInvIsotope::CInvIsotope(const struct isotope* isotope_ptr)
// COMMENT: {2/16/2012 5:45:07 PM}: CIsotope(isotope_ptr)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CInvIsotope::CInvIsotope(const struct inv_isotope* inv_isotope_ptr)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	m_dIsotopeNumber    = inv_isotope_ptr->isotope_number;
// COMMENT: {2/16/2012 5:45:07 PM}	m_strEltName        = inv_isotope_ptr->elt_name;
// COMMENT: {2/16/2012 5:45:07 PM}	m_dRatio            = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	m_dRatioUncertainty = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// m_strName           = inv_isotope_ptr->isotope_name;
// COMMENT: {2/16/2012 5:45:07 PM}	m_strName.Format(_T("%d%s"), (int)m_dIsotopeNumber, m_strEltName);
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CString CInvIsotope::GetString(std::list<InvSol>& rlistInvSol, InvSol& rFinalInvSol, std::map<CString, double>& rDefaults)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	CString str;
// COMMENT: {2/16/2012 5:45:07 PM}	CString strNums;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	str.Format(_T("%4c%-11s"),
// COMMENT: {2/16/2012 5:45:07 PM}		_T(' '),
// COMMENT: {2/16/2012 5:45:07 PM}		m_strName
// COMMENT: {2/16/2012 5:45:07 PM}		);
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// foreach init soln
// COMMENT: {2/16/2012 5:45:07 PM}	std::list<InvSol>::iterator iterSol = rlistInvSol.begin();
// COMMENT: {2/16/2012 5:45:07 PM}	for (; iterSol != rlistInvSol.end(); ++iterSol)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		std::map<int, double>::iterator iterUnc = m_mapSol2Unc.find(iterSol->nSol);
// COMMENT: {2/16/2012 5:45:07 PM}		if (iterUnc != m_mapSol2Unc.end())
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, iterUnc->second);		
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}		else
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			std::map<CString, double>::iterator iterFind = rDefaults.find(m_strName);
// COMMENT: {2/16/2012 5:45:07 PM}			if (iterFind != rDefaults.end())
// COMMENT: {2/16/2012 5:45:07 PM}			{
// COMMENT: {2/16/2012 5:45:07 PM}				strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, iterFind->second);		
// COMMENT: {2/16/2012 5:45:07 PM}			}
// COMMENT: {2/16/2012 5:45:07 PM}			else
// COMMENT: {2/16/2012 5:45:07 PM}			{
// COMMENT: {2/16/2012 5:45:07 PM}				strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, 0.0);		
// COMMENT: {2/16/2012 5:45:07 PM}			}
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}		str += strNums;
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// final soln
// COMMENT: {2/16/2012 5:45:07 PM}	std::map<int, double>::iterator iterUnc = m_mapSol2Unc.find(rFinalInvSol.nSol);
// COMMENT: {2/16/2012 5:45:07 PM}	if (iterUnc != m_mapSol2Unc.end())
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, iterUnc->second);
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	else
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		std::map<CString, double>::iterator iterFind = rDefaults.find(m_strName);
// COMMENT: {2/16/2012 5:45:07 PM}		if (iterFind != rDefaults.end())
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, iterFind->second);		
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}		else
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, 0.0);		
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	str += strNums;
// COMMENT: {2/16/2012 5:45:07 PM}	str.TrimRight();
// COMMENT: {2/16/2012 5:45:07 PM}	return str;
// COMMENT: {2/16/2012 5:45:07 PM}}

//////////////////////////////////////////////////////////////////////
// CS_S_Comp Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// COMMENT: {2/16/2012 5:45:07 PM}CS_S_Comp::CS_S_Comp()
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	_ASSERTE( std::numeric_limits<long double>::has_signaling_NaN );
// COMMENT: {2/16/2012 5:45:07 PM}	m_ldMoles = std::numeric_limits<long double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	m_strName = _T("");
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CS_S_Comp::CS_S_Comp(const struct s_s_comp* s_s_comp_ptr)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	m_strName = s_s_comp_ptr->name;
// COMMENT: {2/16/2012 5:45:07 PM}	m_ldMoles = s_s_comp_ptr->moles;
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CS_S_Comp::~CS_S_Comp()
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}}

//////////////////////////////////////////////////////////////////////
// CS_S Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// COMMENT: {2/16/2012 5:45:07 PM}CS_S::CS_S()
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	_ASSERTE(std::numeric_limits<long double>::has_signaling_NaN);
// COMMENT: {2/16/2012 5:45:07 PM}	_ASSERTE(std::numeric_limits<double>::has_signaling_NaN);
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	m_strName = _T("");
// COMMENT: {2/16/2012 5:45:07 PM}	m_arrldP[0] = m_arrldP[1] = m_arrldP[2] = m_arrldP[3] = std::numeric_limits<long double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	m_dTk = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CS_S::CS_S(const struct s_s* s_s_ptr)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	m_strName = s_s_ptr->name;
// COMMENT: {2/16/2012 5:45:07 PM}	m_dTk     = s_s_ptr->tk;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	m_nInputCase = static_cast<enum InputCase>(s_s_ptr->input_case);
// COMMENT: {2/16/2012 5:45:07 PM}	int nParams = 0;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	switch (s_s_ptr->input_case)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}	case IC_GUGG_NONDIMENSIONAL :
// COMMENT: {2/16/2012 5:45:07 PM}	case IC_MISCIBILITY_GAP :
// COMMENT: {2/16/2012 5:45:07 PM}	case IC_SPINODAL_GAP :
// COMMENT: {2/16/2012 5:45:07 PM}	case IC_CRITICAL_POINT :
// COMMENT: {2/16/2012 5:45:07 PM}	case IC_ALYOTROPIC_POINT :
// COMMENT: {2/16/2012 5:45:07 PM}	case IC_GUGG_KJ :
// COMMENT: {2/16/2012 5:45:07 PM}	case IC_THOMPSON :
// COMMENT: {2/16/2012 5:45:07 PM}	case IC_MARGULES :
// COMMENT: {2/16/2012 5:45:07 PM}		nParams = 2;
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}	case IC_ACTIVITY_COEFFICIENTS:
// COMMENT: {2/16/2012 5:45:07 PM}	case IC_DISTRIBUTION_COEFFICIENTS :
// COMMENT: {2/16/2012 5:45:07 PM}		nParams = 4;
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}	default :
// COMMENT: {2/16/2012 5:45:07 PM}		ASSERT(FALSE);
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	int i = 0;
// COMMENT: {2/16/2012 5:45:07 PM}	for (; i < nParams; ++i)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		m_arrldP[i] = s_s_ptr->p[i];
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	// fill remaining
// COMMENT: {2/16/2012 5:45:07 PM}	for (int j = i; j < 4; ++j)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		m_arrldP[j] = std::numeric_limits<long double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	for (i = 0; i < s_s_ptr->count_comps; ++i)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		CS_S_Comp comp(&s_s_ptr->comps[i]);
// COMMENT: {2/16/2012 5:45:07 PM}		m_listComp.push_back(comp);
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}// use implicit copy ctor
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CS_S::~CS_S()
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}}

//////////////////////////////////////////////////////////////////////
// CRate Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// COMMENT: {2/16/2012 5:45:07 PM}CRate::CRate()
// COMMENT: {2/16/2012 5:45:07 PM}: m_strName(_T(""))
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CRate::CRate(const struct rate *rate_ptr)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	m_strName = rate_ptr->name;
// COMMENT: {2/16/2012 5:45:07 PM}	// parse commands
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	CString str = rate_ptr->commands;
// COMMENT: {2/16/2012 5:45:07 PM}	LPTSTR lpsz = str.GetBuffer(str.GetLength() + 4);
// COMMENT: {2/16/2012 5:45:07 PM}	if (lpsz == NULL)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		ASSERT(FALSE);
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	else
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		// get first line
// COMMENT: {2/16/2012 5:45:07 PM}		LPTSTR lpszLine = _tcstok(lpsz, _T(";"));
// COMMENT: {2/16/2012 5:45:07 PM}		while (lpszLine)
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			basic_command command;
// COMMENT: {2/16/2012 5:45:07 PM}			// get line number
// COMMENT: {2/16/2012 5:45:07 PM}			LPTSTR lpszCommand;
// COMMENT: {2/16/2012 5:45:07 PM}			command.nLine = strtol(lpszLine, &lpszCommand, 10);
// COMMENT: {2/16/2012 5:45:07 PM}			// eat single space
// COMMENT: {2/16/2012 5:45:07 PM}			if (lpszCommand && lpszCommand[0] == _T(' '))
// COMMENT: {2/16/2012 5:45:07 PM}				lpszCommand++;
// COMMENT: {2/16/2012 5:45:07 PM}			command.strCommand = lpszCommand;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}			// test for tabs
// COMMENT: {2/16/2012 5:45:07 PM}			command.strCommand.Replace(_T('\t'), _T(' '));
// COMMENT: {2/16/2012 5:45:07 PM}			ASSERT(command.strCommand.Find(_T("\t"), 0) == -1);
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}			// get next line
// COMMENT: {2/16/2012 5:45:07 PM}			lpszLine = _tcstok(NULL, _T(";"));
// COMMENT: {2/16/2012 5:45:07 PM}			m_listCommands.push_back(command);
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}CString CRate::GetString()
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	CString str;
// COMMENT: {2/16/2012 5:45:07 PM}	str.Format("%4c%s%s%4c-start%s",
// COMMENT: {2/16/2012 5:45:07 PM}		' ',
// COMMENT: {2/16/2012 5:45:07 PM}		m_strName,
// COMMENT: {2/16/2012 5:45:07 PM}		"\r\n",
// COMMENT: {2/16/2012 5:45:07 PM}		' ',
// COMMENT: {2/16/2012 5:45:07 PM}		"\r\n"
// COMMENT: {2/16/2012 5:45:07 PM}		);
// COMMENT: {2/16/2012 5:45:07 PM}	std::list<basic_command>::iterator commandIter = m_listCommands.begin();
// COMMENT: {2/16/2012 5:45:07 PM}	CString strLine;
// COMMENT: {2/16/2012 5:45:07 PM}	for (; commandIter != m_listCommands.end(); ++commandIter)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		strLine.Format("%d %s%s",
// COMMENT: {2/16/2012 5:45:07 PM}			(*commandIter).nLine,
// COMMENT: {2/16/2012 5:45:07 PM}			(*commandIter).strCommand,
// COMMENT: {2/16/2012 5:45:07 PM}			"\r\n"
// COMMENT: {2/16/2012 5:45:07 PM}			);
// COMMENT: {2/16/2012 5:45:07 PM}		str += strLine;
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	return str;
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CRate::~CRate()
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}}

//////////////////////////////////////////////////////////////////////
// CInvPhase Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// COMMENT: {2/16/2012 5:45:07 PM}CInvPhase::CInvPhase()
// COMMENT: {2/16/2012 5:45:07 PM}: m_strName(_T("")), m_bForce(false), m_nConstraint(IPC_EITHER)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CInvPhase::CInvPhase(const struct inv_phases *inv_phases_ptr)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	m_strName = inv_phases_ptr->name;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	m_bForce = (inv_phases_ptr->force == TRUE);
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	switch (inv_phases_ptr->constraint)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}	case EITHER :
// COMMENT: {2/16/2012 5:45:07 PM}		m_nConstraint = IPC_EITHER;
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	case PRECIPITATE :
// COMMENT: {2/16/2012 5:45:07 PM}		m_nConstraint = IPC_PRECIPITATE;
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	case DISSOLVE :
// COMMENT: {2/16/2012 5:45:07 PM}		m_nConstraint = IPC_DISSOLVE;
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	default:
// COMMENT: {2/16/2012 5:45:07 PM}		m_nConstraint = IPC_EITHER;
// COMMENT: {2/16/2012 5:45:07 PM}		ASSERT(FALSE);
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CString CInvPhase::GetString()
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	return GetString(11);
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CString CInvPhase::GetString(int nWidth)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	CString str;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	str.Format(_T("%4c%-*s %s"),
// COMMENT: {2/16/2012 5:45:07 PM}		_T(' '),
// COMMENT: {2/16/2012 5:45:07 PM}		nWidth,
// COMMENT: {2/16/2012 5:45:07 PM}		m_strName,
// COMMENT: {2/16/2012 5:45:07 PM}		m_bForce ? _T("force") : _T("     ")
// COMMENT: {2/16/2012 5:45:07 PM}		);
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	switch (m_nConstraint)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}	case IPC_PRECIPITATE :
// COMMENT: {2/16/2012 5:45:07 PM}		str += _T(" pre");
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}	case IPC_DISSOLVE :
// COMMENT: {2/16/2012 5:45:07 PM}		str += _T(" dis");
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// foreach isotope
// COMMENT: {2/16/2012 5:45:07 PM}	CString strIso;
// COMMENT: {2/16/2012 5:45:07 PM}	std::map<CString, CIsotope>::iterator iterPair = m_mapIsotopes.begin();
// COMMENT: {2/16/2012 5:45:07 PM}	for (; iterPair != m_mapIsotopes.end(); ++iterPair)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		strIso.Format("   %s   %-*.*g %-*.*g",
// COMMENT: {2/16/2012 5:45:07 PM}			(LPCTSTR)iterPair->second.m_strName,
// COMMENT: {2/16/2012 5:45:07 PM}			6,
// COMMENT: {2/16/2012 5:45:07 PM}			DBL_DIG,
// COMMENT: {2/16/2012 5:45:07 PM}			iterPair->second.m_dRatio,
// COMMENT: {2/16/2012 5:45:07 PM}			6,
// COMMENT: {2/16/2012 5:45:07 PM}			DBL_DIG,
// COMMENT: {2/16/2012 5:45:07 PM}			iterPair->second.m_dRatioUncertainty
// COMMENT: {2/16/2012 5:45:07 PM}			);
// COMMENT: {2/16/2012 5:45:07 PM}		str += strIso;
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	str.TrimRight();
// COMMENT: {2/16/2012 5:45:07 PM}	return str;
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CInvPhase::~CInvPhase()
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}}

//////////////////////////////////////////////////////////////////////
// CInvElem Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// COMMENT: {2/16/2012 5:45:07 PM}CInvElem::CInvElem()
// COMMENT: {2/16/2012 5:45:07 PM}: m_strName(_T(""))
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CString CInvElem::GetString(std::list<InvSol>& rlistInvSol, InvSol& rFinalInvSol)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	CString str;
// COMMENT: {2/16/2012 5:45:07 PM}	CString strNums;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	str.Format(_T("%4c%-11s"),
// COMMENT: {2/16/2012 5:45:07 PM}		_T(' '),
// COMMENT: {2/16/2012 5:45:07 PM}		m_strName
// COMMENT: {2/16/2012 5:45:07 PM}		);
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// foreach init soln
// COMMENT: {2/16/2012 5:45:07 PM}	std::list<InvSol>::iterator iterSol = rlistInvSol.begin();
// COMMENT: {2/16/2012 5:45:07 PM}	for (; iterSol != rlistInvSol.end(); ++iterSol)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		std::map<int, double>::iterator iterUnc = m_mapSol2Unc.find(iterSol->nSol);
// COMMENT: {2/16/2012 5:45:07 PM}		if (iterUnc != m_mapSol2Unc.end())
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, iterUnc->second);		
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}		else
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			if (m_strName.CompareNoCase(_T("pH")) == 0)
// COMMENT: {2/16/2012 5:45:07 PM}			{
// COMMENT: {2/16/2012 5:45:07 PM}				strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, 0.05);		
// COMMENT: {2/16/2012 5:45:07 PM}			}
// COMMENT: {2/16/2012 5:45:07 PM}			else
// COMMENT: {2/16/2012 5:45:07 PM}			{
// COMMENT: {2/16/2012 5:45:07 PM}				strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, iterSol->dUncertainty);		
// COMMENT: {2/16/2012 5:45:07 PM}			}
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}		str += strNums;
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// final soln
// COMMENT: {2/16/2012 5:45:07 PM}	std::map<int, double>::iterator iterUnc = m_mapSol2Unc.find(rFinalInvSol.nSol);
// COMMENT: {2/16/2012 5:45:07 PM}	if (iterUnc != m_mapSol2Unc.end())
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, iterUnc->second);
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	else
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		if (m_strName.CompareNoCase(_T("pH")) == 0)
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, 0.05);		
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}		else
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, rFinalInvSol.dUncertainty);
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	str += strNums;
// COMMENT: {2/16/2012 5:45:07 PM}	str.TrimRight();
// COMMENT: {2/16/2012 5:45:07 PM}	return str;
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CInvElem::~CInvElem()
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}}

//////////////////////////////////////////////////////////////////////
// CSurfComp Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// COMMENT: {2/16/2012 5:45:07 PM}CSurfComp::CSurfComp()
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	m_strFormula        = _T("");
// COMMENT: {2/16/2012 5:45:07 PM}	m_dSpecific_area    = -99.9;
// COMMENT: {2/16/2012 5:45:07 PM}	m_dPhase_proportion = -99.9;
// COMMENT: {2/16/2012 5:45:07 PM}	m_dGrams            = -99.9;
// COMMENT: {2/16/2012 5:45:07 PM}	m_dMoles            = -99.9;
// COMMENT: {2/16/2012 5:45:07 PM}	m_dCapacitance0     = -99.9;
// COMMENT: {2/16/2012 5:45:07 PM}	m_dCapacitance1     = -99.9;
// COMMENT: {2/16/2012 5:45:07 PM}	m_strPhase_name     = _T("");
// COMMENT: {2/16/2012 5:45:07 PM}	m_strRate_name      = _T("");
// COMMENT: {2/16/2012 5:45:07 PM}	m_dDw               = 0.;
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CSurfComp::CSurfComp(const struct surface* surface_ptr, const struct surface_comp* surface_comp_ptr)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	ASSERT(surface_comp_ptr->formula != NULL);
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	m_dSpecific_area    = surface_ptr->charge[surface_comp_ptr->charge].specific_area;
// COMMENT: {2/16/2012 5:45:07 PM}	m_dGrams            = surface_ptr->charge[surface_comp_ptr->charge].grams;
// COMMENT: {2/16/2012 5:45:07 PM}	m_dCapacitance0     = surface_ptr->charge[surface_comp_ptr->charge].capacitance[0];
// COMMENT: {2/16/2012 5:45:07 PM}	m_dCapacitance1     = surface_ptr->charge[surface_comp_ptr->charge].capacitance[1];
// COMMENT: {2/16/2012 5:45:07 PM}	m_strFormula        = surface_comp_ptr->formula;
// COMMENT: {2/16/2012 5:45:07 PM}	m_dMoles            = surface_comp_ptr->moles;
// COMMENT: {2/16/2012 5:45:07 PM}	m_strPhase_name     = surface_comp_ptr->phase_name == NULL ? _T("") : surface_comp_ptr->phase_name;
// COMMENT: {2/16/2012 5:45:07 PM}	m_strRate_name      = surface_comp_ptr->rate_name  == NULL ? _T("") : surface_comp_ptr->rate_name;
// COMMENT: {2/16/2012 5:45:07 PM}	m_dPhase_proportion = surface_comp_ptr->phase_proportion;
// COMMENT: {2/16/2012 5:45:07 PM}	m_dDw               = surface_comp_ptr->Dw;
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}// use implicit copy ctor
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CSurfComp::~CSurfComp()
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}

//////////////////////////////////////////////////////////////////////
// CKineticComp Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// COMMENT: {2/16/2012 5:45:07 PM}CKineticComp::CKineticComp()
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	m_dTol          = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	m_dM            = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	m_dInitialMoles = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	m_dM0           = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	m_dMoles        = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	m_dM            = 1.0;
// COMMENT: {2/16/2012 5:45:07 PM}	m_dM0           = 1.0;
// COMMENT: {2/16/2012 5:45:07 PM}	m_dTol          = 1e-8;
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CKineticComp::CKineticComp(const struct kinetics_comp *kinetics_comp_ptr)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	m_strRateName = kinetics_comp_ptr->rate_name == NULL ? _T("") : kinetics_comp_ptr->rate_name;
// COMMENT: {2/16/2012 5:45:07 PM}	if (m_strRateName.IsEmpty())
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		m_strRateName = _T("NoName");
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	
// COMMENT: {2/16/2012 5:45:07 PM}	for (int i = 0; i < kinetics_comp_ptr->count_list; ++i)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		CNameCoef nameCoef(&kinetics_comp_ptr->list[i]);
// COMMENT: {2/16/2012 5:45:07 PM}		m_listNameCoef.push_back(nameCoef);
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	m_dTol = kinetics_comp_ptr->tol;
// COMMENT: {2/16/2012 5:45:07 PM}	m_dM   = kinetics_comp_ptr->m;
// COMMENT: {2/16/2012 5:45:07 PM}	m_dM0  = kinetics_comp_ptr->m0;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	for (int j = 0; j < kinetics_comp_ptr->count_d_params; ++j)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		double d(kinetics_comp_ptr->d_params[j]);
// COMMENT: {2/16/2012 5:45:07 PM}		m_listDParams.push_back(d);
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CKineticComp::~CKineticComp()
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CString CConc::GetUnits(CString &rStr)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	// fill set with available units
// COMMENT: {2/16/2012 5:45:07 PM}	std::set<CString> setUnits;
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("Mol/l"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("mMol/l"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("uMol/l"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("g/l"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("mg/l"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("ug/l"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("Mol/kgs"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("mMol/kgs"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("uMol/kgs"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("g/kgs"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("mg/kgs"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("ug/kgs"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("Mol/kgw"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("mMol/kgw"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("uMol/kgw"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("g/kgw"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("mg/kgw"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("ug/kgw"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("eq/l"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("meq/l"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("ueq/l"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("eq/kgs"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("meq/kgs"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("ueq/kgs"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("eq/kgw"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("meq/kgw"));
// COMMENT: {2/16/2012 5:45:07 PM}	setUnits.insert(_T("ueq/kgw"));
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// Remove space between "kg" and "solution" or "water" in units
// COMMENT: {2/16/2012 5:45:07 PM}	rStr.Replace(_T("Kg"), _T("kg"));
// COMMENT: {2/16/2012 5:45:07 PM}	rStr.Replace(_T("KG"), _T("kg"));
// COMMENT: {2/16/2012 5:45:07 PM}	while (rStr.Replace(_T("kg "), _T("kg")));
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// get first token
// COMMENT: {2/16/2012 5:45:07 PM}	CString strToken = rStr.SpanExcluding(_T(" "));
// COMMENT: {2/16/2012 5:45:07 PM}	strToken.MakeLower();
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// check for parts per units
// COMMENT: {2/16/2012 5:45:07 PM}	if (strToken.Compare("ppt") == 0)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		rStr = rStr.Mid(strToken.GetLength());
// COMMENT: {2/16/2012 5:45:07 PM}		rStr.TrimLeft();
// COMMENT: {2/16/2012 5:45:07 PM}		return strToken;
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	if (strToken.Compare("ppm") == 0)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		rStr = rStr.Mid(strToken.GetLength());
// COMMENT: {2/16/2012 5:45:07 PM}		rStr.TrimLeft();
// COMMENT: {2/16/2012 5:45:07 PM}		return strToken;
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	if (strToken.Compare("ppb") == 0)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		rStr = rStr.Mid(strToken.GetLength());
// COMMENT: {2/16/2012 5:45:07 PM}		rStr.TrimLeft();
// COMMENT: {2/16/2012 5:45:07 PM}		return strToken;
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// save orignal token
// COMMENT: {2/16/2012 5:45:07 PM}	CString strTokenOrig(strToken);
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	strToken.Replace("milli",      "m");
// COMMENT: {2/16/2012 5:45:07 PM}	strToken.Replace("micro",      "u");
// COMMENT: {2/16/2012 5:45:07 PM}	strToken.Replace("grams",      "g");
// COMMENT: {2/16/2012 5:45:07 PM}	strToken.Replace("gram",       "g");
// COMMENT: {2/16/2012 5:45:07 PM}	strToken.Replace("moles",      "Mol");
// COMMENT: {2/16/2012 5:45:07 PM}	strToken.Replace("mole",       "Mol");
// COMMENT: {2/16/2012 5:45:07 PM}	strToken.Replace("mol",        "Mol");
// COMMENT: {2/16/2012 5:45:07 PM}	strToken.Replace("liter",      "l");
// COMMENT: {2/16/2012 5:45:07 PM}	strToken.Replace("kgh",        "kgw");
// COMMENT: {2/16/2012 5:45:07 PM}	strToken.Replace("equivalents","eq");
// COMMENT: {2/16/2012 5:45:07 PM}	strToken.Replace("equivalent", "eq");
// COMMENT: {2/16/2012 5:45:07 PM}	strToken.Replace("equiv",      "eq");
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// trim extraneous endings (ie kgsolution => kgs)
// COMMENT: {2/16/2012 5:45:07 PM}	int nFind;
// COMMENT: {2/16/2012 5:45:07 PM}	if ((nFind = strToken.Find("/l")) != -1)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		strToken = strToken.Left(nFind + 2);
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	if ((nFind = strToken.Find("/kgs")) != -1)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		strToken = strToken.Left(nFind + 4);
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	if ((nFind = strToken.Find("/kgw")) != -1)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		strToken = strToken.Left(nFind + 4);
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// check if unit is in list
// COMMENT: {2/16/2012 5:45:07 PM}	CString strUnits;
// COMMENT: {2/16/2012 5:45:07 PM}	std::set<CString>::const_iterator cIter;
// COMMENT: {2/16/2012 5:45:07 PM}	cIter = setUnits.find(strToken);
// COMMENT: {2/16/2012 5:45:07 PM}	if (cIter != setUnits.end())
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		strUnits = (*cIter);
// COMMENT: {2/16/2012 5:45:07 PM}		rStr = rStr.Mid(strTokenOrig.GetLength());
// COMMENT: {2/16/2012 5:45:07 PM}		rStr.TrimLeft();
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	return strUnits;
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CString CConc::GetAs(CString &rStr)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	CString strAs;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// get first token
// COMMENT: {2/16/2012 5:45:07 PM}	CString strToken = rStr.SpanExcluding(_T(" "));
// COMMENT: {2/16/2012 5:45:07 PM}	if (strToken.Compare("as") == 0)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		rStr = rStr.Mid(strToken.GetLength());
// COMMENT: {2/16/2012 5:45:07 PM}		rStr.TrimLeft();
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}		strAs = rStr.SpanExcluding(" ");
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}		rStr = rStr.Mid(strAs.GetLength());
// COMMENT: {2/16/2012 5:45:07 PM}		rStr.TrimLeft();
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	return strAs;
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CString CConc::GetGFW(CString &rStr)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	CString strGFW;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// get first token
// COMMENT: {2/16/2012 5:45:07 PM}	CString strToken = rStr.SpanExcluding(_T(" "));
// COMMENT: {2/16/2012 5:45:07 PM}	if (strToken.Compare("gfw") == 0)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		rStr = rStr.Mid(strToken.GetLength());
// COMMENT: {2/16/2012 5:45:07 PM}		rStr.TrimLeft();
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}		strGFW = rStr.SpanExcluding(" ");
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}		rStr = rStr.Mid(strGFW.GetLength());
// COMMENT: {2/16/2012 5:45:07 PM}		rStr.TrimLeft();
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	return strGFW;
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CString CConc::GetRedox(CString &rStr)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	CString strRedox;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// get first token
// COMMENT: {2/16/2012 5:45:07 PM}	CString strToken = rStr.SpanExcluding(_T(" "));
// COMMENT: {2/16/2012 5:45:07 PM}	if (strToken.Find("/") != -1)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		strRedox = strToken;
// COMMENT: {2/16/2012 5:45:07 PM}		rStr = rStr.Mid(strToken.GetLength());
// COMMENT: {2/16/2012 5:45:07 PM}		rStr.TrimLeft();
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	return strRedox;
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}void CConc::GetPhase(CString &rStr, CConc& rConc)
// COMMENT: {2/16/2012 5:45:07 PM}{	
// COMMENT: {2/16/2012 5:45:07 PM}	CString strToken = rStr.SpanExcluding(" ");
// COMMENT: {2/16/2012 5:45:07 PM}	if (strToken.Compare("charge") != 0)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		rConc.m_strPhase = strToken;
// COMMENT: {2/16/2012 5:45:07 PM}		rStr = rStr.Mid(strToken.GetLength());
// COMMENT: {2/16/2012 5:45:07 PM}		rStr.TrimLeft();
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}		rConc.m_strPhaseSI = rStr.SpanExcluding(" ");
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}		rStr = rStr.Mid(rConc.m_strPhaseSI.GetLength());
// COMMENT: {2/16/2012 5:45:07 PM}		rStr.TrimLeft();
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	else
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		rConc.m_strPhase = strToken;
// COMMENT: {2/16/2012 5:45:07 PM}		rStr = rStr.Mid(strToken.GetLength());
// COMMENT: {2/16/2012 5:45:07 PM}		rStr.TrimLeft();
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CString CConc::GetSubHeading()const
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	// [units], (as formula] or [gfw gfw]) [redox_couple] [charge or phase_name [saturation index])]
// COMMENT: {2/16/2012 5:45:07 PM}	CString strSubHeading;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	if (!m_strUnits.IsEmpty())
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		strSubHeading += m_strUnits + " ";
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	if (!m_strAs.IsEmpty())
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		strSubHeading += CString("as ") + m_strAs + " ";
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	else if(!m_strGFW.IsEmpty())
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		strSubHeading += CString("gfw ") + m_strGFW + " ";
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	if (!m_strRedox.IsEmpty())
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		strSubHeading += m_strRedox + " ";
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	if (!m_strPhase.IsEmpty())
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		strSubHeading += m_strPhase + " ";
// COMMENT: {2/16/2012 5:45:07 PM}		if (!m_strPhaseSI.IsEmpty())
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			strSubHeading += m_strPhaseSI;
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	strSubHeading.TrimRight();
// COMMENT: {2/16/2012 5:45:07 PM}	return strSubHeading;
// COMMENT: {2/16/2012 5:45:07 PM}}

//////////////////////////////////////////////////////////////////////
// CSpecies Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// COMMENT: {2/16/2012 5:45:07 PM}CSpecies::CSpecies()
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	m_bHasAnalExp  = false;
// COMMENT: {2/16/2012 5:45:07 PM}	m_bHasCDMusic  = false;
// COMMENT: {2/16/2012 5:45:07 PM}	m_bHasMillero  = false;
// COMMENT: {2/16/2012 5:45:07 PM}	m_bCheckEqn    = true;
// COMMENT: {2/16/2012 5:45:07 PM}	m_nDeltaHUnits = kjoules;
// COMMENT: {2/16/2012 5:45:07 PM}	m_nActType     = AT_DAVIES;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	m_dLogK        = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	m_dDeltaH      = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	m_dA_F         = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	for (int i = 0; i < 6; ++i)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		this->m_millero[i] = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// tracer diffusion coefficient in water at 25oC, m2/s
// COMMENT: {2/16/2012 5:45:07 PM}	this->m_dw = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// enrichment factor in DDL
// COMMENT: {2/16/2012 5:45:07 PM}	this->m_erm_ddl = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CSpecies::CSpecies(const struct species *species_ptr)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	int i;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// assoc rxn
// COMMENT: {2/16/2012 5:45:07 PM}	m_strEqn = WriteEqn(species_ptr);
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// check eqn
// COMMENT: {2/16/2012 5:45:07 PM}	m_bCheckEqn = (species_ptr->check_equation == TRUE);
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// mole balance
// COMMENT: {2/16/2012 5:45:07 PM}	m_strMoleBalance = (species_ptr->mole_balance == NULL) ? _T("") : species_ptr->mole_balance;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// Check for analytical expression
// COMMENT: {2/16/2012 5:45:07 PM}	m_bHasAnalExp = false;
// COMMENT: {2/16/2012 5:45:07 PM}	for (i = 2; i < 8; ++i)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		if (species_ptr->logk[i] != 0.0)
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			m_bHasAnalExp = true;
// COMMENT: {2/16/2012 5:45:07 PM}			break;
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// Check for CD_MUSIC
// COMMENT: {2/16/2012 5:45:07 PM}	m_bHasCDMusic = false;
// COMMENT: {2/16/2012 5:45:07 PM}	for (i = 0; i < 5; ++i)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		if (species_ptr->cd_music[i] != 0.0)
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			m_bHasCDMusic = true;
// COMMENT: {2/16/2012 5:45:07 PM}			break;
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	if (m_bHasAnalExp)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		m_dA1 = species_ptr->logk[2];
// COMMENT: {2/16/2012 5:45:07 PM}		m_dA2 = species_ptr->logk[3];
// COMMENT: {2/16/2012 5:45:07 PM}		m_dA3 = species_ptr->logk[4];
// COMMENT: {2/16/2012 5:45:07 PM}		m_dA4 = species_ptr->logk[5];
// COMMENT: {2/16/2012 5:45:07 PM}		m_dA5 = species_ptr->logk[6];
// COMMENT: {2/16/2012 5:45:07 PM}		m_dA6 = species_ptr->logk[7];
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	if (m_bHasCDMusic)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		for (i = 0; i < 5; ++i)
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			this->m_dCDMusic[i] = species_ptr->cd_music[i];
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// Log K
// COMMENT: {2/16/2012 5:45:07 PM}	if (!m_bHasAnalExp || species_ptr->logk[0] != 0.0)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		m_dLogK = species_ptr->logk[0];
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	else
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		m_dLogK = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// Delta H
// COMMENT: {2/16/2012 5:45:07 PM}	if (species_ptr->logk[1] != 0.0)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		m_dDeltaH = species_ptr->logk[1];
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	else
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		m_dDeltaH = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	m_nDeltaHUnits = species_ptr->original_units;
// COMMENT: {2/16/2012 5:45:07 PM}	switch (m_nDeltaHUnits)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}	case joules:
// COMMENT: {2/16/2012 5:45:07 PM}		m_dDeltaH *= 1000;
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}	case cal:
// COMMENT: {2/16/2012 5:45:07 PM}		m_dDeltaH *= 1000/JOULES_PER_CALORIE;
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}	case kcal:
// COMMENT: {2/16/2012 5:45:07 PM}		m_dDeltaH /= JOULES_PER_CALORIE;
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}	case kjoules:
// COMMENT: {2/16/2012 5:45:07 PM}		// do nothing
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}	default:
// COMMENT: {2/16/2012 5:45:07 PM}		ASSERT(FALSE);
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// determine act coef type
// COMMENT: {2/16/2012 5:45:07 PM}	m_nActType = AT_NONE;
// COMMENT: {2/16/2012 5:45:07 PM}	m_dDHa = species_ptr->dha;
// COMMENT: {2/16/2012 5:45:07 PM}	m_dDHb = species_ptr->dhb;
// COMMENT: {2/16/2012 5:45:07 PM}	m_dA_F = (species_ptr->a_f == 0) ? std::numeric_limits<double>::signaling_NaN() : species_ptr->a_f;
// COMMENT: {2/16/2012 5:45:07 PM}	switch (species_ptr->gflag)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}	case 0:
// COMMENT: {2/16/2012 5:45:07 PM}		ASSERT(species_ptr->z == 0.0);
// COMMENT: {2/16/2012 5:45:07 PM}		ASSERT(m_dDHa == 0.0 && m_dDHb == 0.1);
// COMMENT: {2/16/2012 5:45:07 PM}		m_nActType = AT_DAVIES;
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}	case 1:
// COMMENT: {2/16/2012 5:45:07 PM}		m_nActType = AT_DAVIES;
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}	case 2:
// COMMENT: {2/16/2012 5:45:07 PM}		m_nActType = AT_DEBYE_HUCKEL;
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}	case 3:
// COMMENT: {2/16/2012 5:45:07 PM}		if (species_ptr->z == 0.0)
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			if (m_dDHa == 0.0 && m_dDHb == 0.1)
// COMMENT: {2/16/2012 5:45:07 PM}			{
// COMMENT: {2/16/2012 5:45:07 PM}				m_nActType = AT_DAVIES;
// COMMENT: {2/16/2012 5:45:07 PM}			}
// COMMENT: {2/16/2012 5:45:07 PM}			else
// COMMENT: {2/16/2012 5:45:07 PM}			{
// COMMENT: {2/16/2012 5:45:07 PM}				m_nActType = AT_DEBYE_HUCKEL;
// COMMENT: {2/16/2012 5:45:07 PM}			}
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}		else
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			if (m_dDHa == 0.0 && m_dDHb == 0.0)
// COMMENT: {2/16/2012 5:45:07 PM}			{
// COMMENT: {2/16/2012 5:45:07 PM}				m_nActType = AT_DAVIES;
// COMMENT: {2/16/2012 5:45:07 PM}			}
// COMMENT: {2/16/2012 5:45:07 PM}			else
// COMMENT: {2/16/2012 5:45:07 PM}			{
// COMMENT: {2/16/2012 5:45:07 PM}				m_nActType = AT_DEBYE_HUCKEL;
// COMMENT: {2/16/2012 5:45:07 PM}			}
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}	case 5:
// COMMENT: {2/16/2012 5:45:07 PM}		ASSERT(m_dDHa == 0.0 && m_dDHb == 0.0);
// COMMENT: {2/16/2012 5:45:07 PM}		m_nActType = AT_DAVIES;
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}	case 7:
// COMMENT: {2/16/2012 5:45:07 PM}		m_nActType = AT_LLNL_DH;
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}	case 8:
// COMMENT: {2/16/2012 5:45:07 PM}		ASSERT(m_dDHa == 0.0);
// COMMENT: {2/16/2012 5:45:07 PM}		m_nActType = AT_LLNL_DH_CO2;
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	case 4: // exchange
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 7:21:59 PM}		if (m_dDHa == 0.0 && m_dDHb == 0.0)
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 7:21:59 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 7:21:59 PM}			ASSERT(species_ptr->exch_gflag == 1);
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 7:21:59 PM}			// m_nActType = AT_DAVIES;
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 7:21:59 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 7:21:59 PM}		else
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 7:21:59 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 7:21:59 PM}			ASSERT(species_ptr->exch_gflag == 2);
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 7:21:59 PM}			// m_nActType = AT_DEBYE_HUCKEL;
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 7:21:59 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}		switch (species_ptr->exch_gflag)
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}		case 1:
// COMMENT: {2/16/2012 5:45:07 PM}			this->m_nActType = AT_DAVIES;
// COMMENT: {2/16/2012 5:45:07 PM}			break;
// COMMENT: {2/16/2012 5:45:07 PM}		case 2:
// COMMENT: {2/16/2012 5:45:07 PM}			this->m_nActType = AT_DEBYE_HUCKEL;
// COMMENT: {2/16/2012 5:45:07 PM}			break;
// COMMENT: {2/16/2012 5:45:07 PM}		case 3:
// COMMENT: {2/16/2012 5:45:07 PM}			this->m_nActType = AT_NONE;
// COMMENT: {2/16/2012 5:45:07 PM}			break;
// COMMENT: {2/16/2012 5:45:07 PM}		case 7:
// COMMENT: {2/16/2012 5:45:07 PM}			this->m_nActType = AT_LLNL_DH;
// COMMENT: {2/16/2012 5:45:07 PM}			break;
// COMMENT: {2/16/2012 5:45:07 PM}		default:
// COMMENT: {2/16/2012 5:45:07 PM}			ASSERT(FALSE);
// COMMENT: {2/16/2012 5:45:07 PM}			break;
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	case 6: // surface
// COMMENT: {2/16/2012 5:45:07 PM}		if (m_dDHa == 0.0 && m_dDHb == 0.0)
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			m_nActType = AT_DAVIES;
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}		else
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			m_nActType = AT_DEBYE_HUCKEL;
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}	default:
// COMMENT: {2/16/2012 5:45:07 PM}		ASSERT(FALSE);
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 4:47:25 PM}	if (species_ptr->type == EX)
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 4:47:25 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 4:47:25 PM}		ASSERT(species_ptr->gflag == 4);
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 4:47:25 PM}		switch (species_ptr->exch_gflag)
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 4:47:25 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 4:47:25 PM}		case 1: // davies
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 4:47:25 PM}			this->m_nActType = AT_DAVIES;
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 4:47:25 PM}			break;
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 4:47:25 PM}		case 2: // debye-huckle
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 4:47:25 PM}			this->m_nActType = AT_DEBYE_HUCKEL;
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 4:47:25 PM}			break;
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 4:47:25 PM}		case 3:
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 4:47:25 PM}			break;
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 4:47:25 PM}		case 7: // llnl_gamma
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 4:47:25 PM}			this->m_nActType = AT_LLNL_DH;
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 4:47:25 PM}			break;
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 4:47:25 PM}		default:
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 4:47:25 PM}			ASSERT(FALSE);
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 4:47:25 PM}			break;
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 4:47:25 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 4:47:25 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// tracer diffusion coefficient in water at 25oC, m2/s
// COMMENT: {2/16/2012 5:45:07 PM}	this->m_dw = (species_ptr->dw == 0) ? std::numeric_limits<double>::signaling_NaN() : species_ptr->dw;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// enrichment factor in DDL
// COMMENT: {2/16/2012 5:45:07 PM}	this->m_erm_ddl = (species_ptr->erm_ddl == 1) ? std::numeric_limits<double>::signaling_NaN() : species_ptr->erm_ddl;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// regression coefficients to calculate temperature dependent phi_0 and b_v of Millero density model
// COMMENT: {2/16/2012 5:45:07 PM}	this->m_bHasMillero = false;
// COMMENT: {2/16/2012 5:45:07 PM}	for (int i = 0; i < 6; ++i)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		if (species_ptr->millero[i] != 0)
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			this->m_bHasMillero = true;
// COMMENT: {2/16/2012 5:45:07 PM}			break;
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	for (int i = 0; i < 6; ++i)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		if (this->m_bHasMillero)
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			this->m_millero[i] = species_ptr->millero[i];
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}		else
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			this->m_millero[i] = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}// COMMENT: {12/10/2009 6:39:04 PM}	ASSERT(m_nActType != AT_UNKNOWN);
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CSpecies::~CSpecies()
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CString CSpecies::WriteEqn(const species *species_ptr)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	CString strEqn;
// COMMENT: {2/16/2012 5:45:07 PM}	CString strFormat;
// COMMENT: {2/16/2012 5:45:07 PM}	int i;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// left hand side
// COMMENT: {2/16/2012 5:45:07 PM}	bool bFirst = true;
// COMMENT: {2/16/2012 5:45:07 PM}	for (i = 0; species_ptr->rxn->token[i].s != NULL; ++i)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		if (species_ptr->rxn->token[i].coef < 0) continue;
// COMMENT: {2/16/2012 5:45:07 PM}		if(!bFirst)
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			strEqn += _T(" + ");
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}		bFirst = false;
// COMMENT: {2/16/2012 5:45:07 PM}		if (species_ptr->rxn->token[i].coef != 1)
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			strFormat.Format("%g%s", species_ptr->rxn->token[i].coef, species_ptr->rxn->token[i].s->name);
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}		else
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			strFormat.Format("%s", species_ptr->rxn->token[i].s->name);
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}		strEqn += strFormat;
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// right hand side
// COMMENT: {2/16/2012 5:45:07 PM}	strEqn += _T(" = ");
// COMMENT: {2/16/2012 5:45:07 PM}	bFirst = true;
// COMMENT: {2/16/2012 5:45:07 PM}	for (i = 0; species_ptr->rxn->token[i].s != NULL; ++i)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		if (species_ptr->rxn->token[i].coef > 0) continue;
// COMMENT: {2/16/2012 5:45:07 PM}		if(!bFirst)
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			strEqn += _T(" + ");
// COMMENT: {2/16/2012 5:45:07 PM}		} 
// COMMENT: {2/16/2012 5:45:07 PM}		bFirst = false;
// COMMENT: {2/16/2012 5:45:07 PM}		if (-species_ptr->rxn->token[i].coef != 1)
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			strFormat.Format("%g%s", -species_ptr->rxn->token[i].coef, species_ptr->rxn->token[i].s->name);
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}		else
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			strFormat.Format("%s", species_ptr->rxn->token[i].s->name);
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}		strEqn += strFormat;
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	return strEqn;
// COMMENT: {2/16/2012 5:45:07 PM}}

//////////////////////////////////////////////////////////////////////
// CPhase Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// COMMENT: {2/16/2012 5:45:07 PM}CPhase::CPhase()
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	m_bHasAnalExp  = false;
// COMMENT: {2/16/2012 5:45:07 PM}	m_bCheckEqn    = true;
// COMMENT: {2/16/2012 5:45:07 PM}	m_nDeltaHUnits = kjoules;
// COMMENT: {2/16/2012 5:45:07 PM}	m_dLogK        = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	m_dDeltaH      = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CPhase::CPhase(const struct phase *phase_ptr)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	// phase name
// COMMENT: {2/16/2012 5:45:07 PM}	m_strName = (phase_ptr->name) ? phase_ptr->name : _T("");
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// diss. rxn
// COMMENT: {2/16/2012 5:45:07 PM}	m_strEqn = WriteEqn(phase_ptr);
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// check eqn
// COMMENT: {2/16/2012 5:45:07 PM}	m_bCheckEqn = (phase_ptr->check_equation == TRUE);
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// Check for analytical expression
// COMMENT: {2/16/2012 5:45:07 PM}	m_bHasAnalExp = false;
// COMMENT: {2/16/2012 5:45:07 PM}	for (int i = 2; i < 8; ++i)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		if (phase_ptr->logk[i] != 0.0)
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			m_bHasAnalExp = true;
// COMMENT: {2/16/2012 5:45:07 PM}			break;
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	if (m_bHasAnalExp)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		m_dA1 = phase_ptr->logk[2];
// COMMENT: {2/16/2012 5:45:07 PM}		m_dA2 = phase_ptr->logk[3];
// COMMENT: {2/16/2012 5:45:07 PM}		m_dA3 = phase_ptr->logk[4];
// COMMENT: {2/16/2012 5:45:07 PM}		m_dA4 = phase_ptr->logk[5];
// COMMENT: {2/16/2012 5:45:07 PM}		m_dA5 = phase_ptr->logk[6];
// COMMENT: {2/16/2012 5:45:07 PM}		m_dA6 = phase_ptr->logk[7];
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// Log K
// COMMENT: {2/16/2012 5:45:07 PM}	if (!m_bHasAnalExp || phase_ptr->logk[0] != 0.0)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		m_dLogK = phase_ptr->logk[0];
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	else
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		m_dLogK = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// Delta H
// COMMENT: {2/16/2012 5:45:07 PM}	if (phase_ptr->logk[1] != 0.0)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		m_dDeltaH = phase_ptr->logk[1];
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	else
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		m_dDeltaH = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	m_nDeltaHUnits = phase_ptr->original_units;
// COMMENT: {2/16/2012 5:45:07 PM}	switch (m_nDeltaHUnits)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}	case joules:
// COMMENT: {2/16/2012 5:45:07 PM}		m_dDeltaH *= 1000;
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}	case cal:
// COMMENT: {2/16/2012 5:45:07 PM}		m_dDeltaH *= 1000/JOULES_PER_CALORIE;
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}	case kcal:
// COMMENT: {2/16/2012 5:45:07 PM}		m_dDeltaH /= JOULES_PER_CALORIE;
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}	case kjoules:
// COMMENT: {2/16/2012 5:45:07 PM}		// do nothing
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}	default:
// COMMENT: {2/16/2012 5:45:07 PM}		ASSERT(FALSE);
// COMMENT: {2/16/2012 5:45:07 PM}		break;
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CPhase::~CPhase()
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CString CPhase::WriteEqn(const struct phase *phase_ptr)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	CString strEqn;
// COMMENT: {2/16/2012 5:45:07 PM}	CString strFormat;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// formula
// COMMENT: {2/16/2012 5:45:07 PM}	strEqn = (phase_ptr->formula) ? phase_ptr->formula : _T("");
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// left hand side
// COMMENT: {2/16/2012 5:45:07 PM}	int j;
// COMMENT: {2/16/2012 5:45:07 PM}	for (j = 1; phase_ptr->rxn->token[j].s != NULL; ++j)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		if (phase_ptr->rxn->token[j].coef > 0) continue;
// COMMENT: {2/16/2012 5:45:07 PM}		strEqn += _T(" + ");
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}		if (-phase_ptr->rxn->token[j].coef != 1)
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			strFormat.Format("%g%s", -phase_ptr->rxn->token[j].coef, phase_ptr->rxn->token[j].s->name);
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}		else
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			strFormat.Format("%s", phase_ptr->rxn->token[j].s->name);
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}		strEqn += strFormat;
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	// right hand side
// COMMENT: {2/16/2012 5:45:07 PM}	strEqn += _T(" = ");
// COMMENT: {2/16/2012 5:45:07 PM}	bool bFirst = true;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	for (j = 1; phase_ptr->rxn->token[j].s != NULL; ++j)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		if (phase_ptr->rxn->token[j].coef < 0) continue;
// COMMENT: {2/16/2012 5:45:07 PM}		if (!bFirst)
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			strEqn += _T(" + ");
// COMMENT: {2/16/2012 5:45:07 PM}		} 
// COMMENT: {2/16/2012 5:45:07 PM}		bFirst = false;
// COMMENT: {2/16/2012 5:45:07 PM}		if (phase_ptr->rxn->token[j].coef != 1)
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			strFormat.Format(_T("%g%s"), phase_ptr->rxn->token[j].coef, phase_ptr->rxn->token[j].s->name);
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}		else
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			strFormat.Format(_T("%s"), phase_ptr->rxn->token[j].s->name);
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}		strEqn += strFormat;
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	return strEqn;
// COMMENT: {2/16/2012 5:45:07 PM}}

//////////////////////////////////////////////////////////////////////
// CMaster Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// COMMENT: {2/16/2012 5:45:07 PM}CMaster::CMaster()
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	m_bPrimary = true;
// COMMENT: {2/16/2012 5:45:07 PM}	m_dAlk = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}	m_dGFW = std::numeric_limits<double>::signaling_NaN();
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}/*
// COMMENT: {2/16/2012 5:45:07 PM}CString strName = master[i]->elt->name;
// COMMENT: {2/16/2012 5:45:07 PM}TRACE("%-13s", strName);
// COMMENT: {2/16/2012 5:45:07 PM}CString strMasterSpecies = master[i]->s->name;
// COMMENT: {2/16/2012 5:45:07 PM}TRACE(" %-13s", strMasterSpecies);
// COMMENT: {2/16/2012 5:45:07 PM}double dAlk = master[i]->alk;
// COMMENT: {2/16/2012 5:45:07 PM}TRACE(" %g", dAlk);
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}if (master[i]->gfw_formula)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	m_strFormula = master[i]->gfw_formula;
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}else
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	m
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}if (master[i]->gfw_formula)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	if (strName.Find(_T('(')) == -1)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		TRACE(" %-13s %g\n", master[i]->gfw_formula, master[i]->elt->gfw);
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	else
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		TRACE(" %-13s\n", master[i]->gfw_formula);
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}else
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	if (strName.Find(_T('(')) == -1)
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		TRACE(" %g %g\n", master[i]->gfw, master[i]->gfw);
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	else
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		TRACE(" %g\n", master[i]->gfw);
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}*/
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CMaster::CMaster(const struct master *master_ptr)
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}	m_strName          = (master_ptr->elt->name) ? master_ptr->elt->name : _T("");
// COMMENT: {2/16/2012 5:45:07 PM}	m_strMasterSpecies = (master_ptr->s->name) ? master_ptr->s->name : _T("");
// COMMENT: {2/16/2012 5:45:07 PM}	m_dAlk             = master_ptr->alk;
// COMMENT: {2/16/2012 5:45:07 PM}	m_bPrimary         = (master_ptr->primary == TRUE);
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}	if (m_strName != _T("E"))
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		m_dGFW        = master_ptr->gfw;
// COMMENT: {2/16/2012 5:45:07 PM}		m_dGFWElement = master_ptr->elt->gfw;
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}		// check for gfw formula
// COMMENT: {2/16/2012 5:45:07 PM}		if (master_ptr->gfw_formula)
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			m_strFormula = master_ptr->gfw_formula;
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}		else
// COMMENT: {2/16/2012 5:45:07 PM}		{
// COMMENT: {2/16/2012 5:45:07 PM}			m_strFormula = _T("");
// COMMENT: {2/16/2012 5:45:07 PM}		}
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}	else
// COMMENT: {2/16/2012 5:45:07 PM}	{
// COMMENT: {2/16/2012 5:45:07 PM}		ASSERT(master_ptr->gfw == 0.0);
// COMMENT: {2/16/2012 5:45:07 PM}		ASSERT(master_ptr->elt->gfw == 0.0);
// COMMENT: {2/16/2012 5:45:07 PM}		m_dGFW = 0.0;
// COMMENT: {2/16/2012 5:45:07 PM}		m_dGFWElement = 0.0;
// COMMENT: {2/16/2012 5:45:07 PM}		m_strFormula = _T("");
// COMMENT: {2/16/2012 5:45:07 PM}	}
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
// COMMENT: {2/16/2012 5:45:07 PM}CMaster::~CMaster()
// COMMENT: {2/16/2012 5:45:07 PM}{
// COMMENT: {2/16/2012 5:45:07 PM}}
// COMMENT: {2/16/2012 5:45:07 PM}
//////////////////////////////////////////////////////////////////////
// CTransport Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTransport::CTransport()
{
	this->Update();
}

CTransport::~CTransport()
{
}

void CTransport::Update()
{
// COMMENT: {2/16/2012 5:46:56 PM}	// Line 1:     -cells                 5
// COMMENT: {2/16/2012 5:46:56 PM}	this->count_cells = ::count_cells;                  // m_Page1.m_nCells
// COMMENT: {2/16/2012 5:46:56 PM}
// COMMENT: {2/16/2012 5:46:56 PM}	// Line 2:     -shifts                25
// COMMENT: {2/16/2012 5:46:56 PM}	this->count_shifts = ::count_shifts;                // m_Page1.m_nShifts
// COMMENT: {2/16/2012 5:46:56 PM}
// COMMENT: {2/16/2012 5:46:56 PM}	// Line 3:      -time_step 3.15e7 # seconds = 1 yr.
// COMMENT: {2/16/2012 5:46:56 PM}	this->timest       = ::timest;                      // m_Page1.m_dTimeStep && m_Page1.m_tuTimeStep
// COMMENT: {2/16/2012 5:46:56 PM}	this->mcd_substeps = ::mcd_substeps;                // m_Page6.m_mcd_substeps
// COMMENT: {2/16/2012 5:46:56 PM}	
// COMMENT: {2/16/2012 5:46:56 PM}	// Line 4:     -flow_direction        forward
// COMMENT: {2/16/2012 5:46:56 PM}	switch (::ishift)                                   // m_Page3.m_fdFD
// COMMENT: {2/16/2012 5:46:56 PM}	{
// COMMENT: {2/16/2012 5:46:56 PM}	case -1:
// COMMENT: {2/16/2012 5:46:56 PM}		this->shift = CKPTransportPg3::FD_BACK;
// COMMENT: {2/16/2012 5:46:56 PM}		break;
// COMMENT: {2/16/2012 5:46:56 PM}	case 0:
// COMMENT: {2/16/2012 5:46:56 PM}		this->shift = CKPTransportPg3::FD_DIFF_ONLY;
// COMMENT: {2/16/2012 5:46:56 PM}		break;
// COMMENT: {2/16/2012 5:46:56 PM}	case 1:
// COMMENT: {2/16/2012 5:46:56 PM}		this->shift = CKPTransportPg3::FD_FORWARD;
// COMMENT: {2/16/2012 5:46:56 PM}		break;
// COMMENT: {2/16/2012 5:46:56 PM}	default:
// COMMENT: {2/16/2012 5:46:56 PM}		ASSERT(FALSE);
// COMMENT: {2/16/2012 5:46:56 PM}		break;
// COMMENT: {2/16/2012 5:46:56 PM}	}
// COMMENT: {2/16/2012 5:46:56 PM}
// COMMENT: {2/16/2012 5:46:56 PM}	// Line 5:     -boundary_conditions   flux constant
// COMMENT: {2/16/2012 5:46:56 PM}	switch (::bcon_first)                               // m_Page3.m_bcFirst
// COMMENT: {2/16/2012 5:46:56 PM}	{
// COMMENT: {2/16/2012 5:46:56 PM}	case 1:
// COMMENT: {2/16/2012 5:46:56 PM}		this->bc_first = CKPTransportPg3::BC_CONSTANT;
// COMMENT: {2/16/2012 5:46:56 PM}		break;
// COMMENT: {2/16/2012 5:46:56 PM}	case 2:
// COMMENT: {2/16/2012 5:46:56 PM}		this->bc_first = CKPTransportPg3::BC_CLOSED;
// COMMENT: {2/16/2012 5:46:56 PM}		break;
// COMMENT: {2/16/2012 5:46:56 PM}	case 3:
// COMMENT: {2/16/2012 5:46:56 PM}		this->bc_first = CKPTransportPg3::BC_FLUX;
// COMMENT: {2/16/2012 5:46:56 PM}		break;
// COMMENT: {2/16/2012 5:46:56 PM}	default:
// COMMENT: {2/16/2012 5:46:56 PM}		ASSERT(FALSE);
// COMMENT: {2/16/2012 5:46:56 PM}		this->bc_first = CKPTransportPg3::BC_FLUX;
// COMMENT: {2/16/2012 5:46:56 PM}		break;
// COMMENT: {2/16/2012 5:46:56 PM}	}
// COMMENT: {2/16/2012 5:46:56 PM}
// COMMENT: {2/16/2012 5:46:56 PM}	switch (::bcon_last)                                // m_Page3.m_bcLast
// COMMENT: {2/16/2012 5:46:56 PM}	{
// COMMENT: {2/16/2012 5:46:56 PM}	case 1:
// COMMENT: {2/16/2012 5:46:56 PM}		this->bc_last = CKPTransportPg3::BC_CONSTANT;
// COMMENT: {2/16/2012 5:46:56 PM}		break;
// COMMENT: {2/16/2012 5:46:56 PM}	case 2:
// COMMENT: {2/16/2012 5:46:56 PM}		this->bc_last = CKPTransportPg3::BC_CLOSED;
// COMMENT: {2/16/2012 5:46:56 PM}		break;
// COMMENT: {2/16/2012 5:46:56 PM}	case 3:
// COMMENT: {2/16/2012 5:46:56 PM}		this->bc_last = CKPTransportPg3::BC_FLUX;
// COMMENT: {2/16/2012 5:46:56 PM}		break;
// COMMENT: {2/16/2012 5:46:56 PM}	default:
// COMMENT: {2/16/2012 5:46:56 PM}		ASSERT(FALSE);
// COMMENT: {2/16/2012 5:46:56 PM}		this->bc_last = CKPTransportPg3::BC_FLUX;
// COMMENT: {2/16/2012 5:46:56 PM}		break;
// COMMENT: {2/16/2012 5:46:56 PM}	}
// COMMENT: {2/16/2012 5:46:56 PM}
// COMMENT: {2/16/2012 5:46:56 PM}
// COMMENT: {2/16/2012 5:46:56 PM}	// Line 6:     -lengths               4*1.0 2.0
// COMMENT: {2/16/2012 5:46:56 PM}	// Line 7:     -dispersivities        4*0.1 0.2
// COMMENT: {2/16/2012 5:46:56 PM}	this->lengths_list.clear();                         // m_Page4.m_lrLengths
// COMMENT: {2/16/2012 5:46:56 PM}	this->disp_list.clear();                            // m_Page4.m_lrDisps
// COMMENT: {2/16/2012 5:46:56 PM}	ASSERT(::cell_data);
// COMMENT: {2/16/2012 5:46:56 PM}	CRepeat rLength(::cell_data[0].length);
// COMMENT: {2/16/2012 5:46:56 PM}	CRepeat rDisp(::cell_data[0].disp);
// COMMENT: {2/16/2012 5:46:56 PM}	for (int i = 1; i < ::count_cells; ++i)
// COMMENT: {2/16/2012 5:46:56 PM}	{
// COMMENT: {2/16/2012 5:46:56 PM}		// lengths
// COMMENT: {2/16/2012 5:46:56 PM}		if (::cell_data[i].length == rLength.GetDValue())
// COMMENT: {2/16/2012 5:46:56 PM}		{
// COMMENT: {2/16/2012 5:46:56 PM}			rLength.Increment();
// COMMENT: {2/16/2012 5:46:56 PM}		}
// COMMENT: {2/16/2012 5:46:56 PM}		else
// COMMENT: {2/16/2012 5:46:56 PM}		{
// COMMENT: {2/16/2012 5:46:56 PM}			this->lengths_list.push_back(rLength);
// COMMENT: {2/16/2012 5:46:56 PM}			rLength = CRepeat(::cell_data[i].length);
// COMMENT: {2/16/2012 5:46:56 PM}		}
// COMMENT: {2/16/2012 5:46:56 PM}
// COMMENT: {2/16/2012 5:46:56 PM}		// disps
// COMMENT: {2/16/2012 5:46:56 PM}		if (::cell_data[i].disp == rDisp.GetDValue())
// COMMENT: {2/16/2012 5:46:56 PM}		{
// COMMENT: {2/16/2012 5:46:56 PM}			rDisp.Increment();
// COMMENT: {2/16/2012 5:46:56 PM}		}
// COMMENT: {2/16/2012 5:46:56 PM}		else
// COMMENT: {2/16/2012 5:46:56 PM}		{
// COMMENT: {2/16/2012 5:46:56 PM}			this->disp_list.push_back(rDisp);
// COMMENT: {2/16/2012 5:46:56 PM}			rDisp = CRepeat(::cell_data[i].disp);
// COMMENT: {2/16/2012 5:46:56 PM}		}
// COMMENT: {2/16/2012 5:46:56 PM}	}
// COMMENT: {2/16/2012 5:46:56 PM}	if (!(rLength.GetCount() == (size_t)::count_cells && rLength.GetDValue() == 1.0))
// COMMENT: {2/16/2012 5:46:56 PM}	{
// COMMENT: {2/16/2012 5:46:56 PM}		this->lengths_list.push_back(rLength);
// COMMENT: {2/16/2012 5:46:56 PM}	}
// COMMENT: {2/16/2012 5:46:56 PM}	if (!(rDisp.GetCount() == (size_t)::count_cells && rDisp.GetDValue() == 0.0))
// COMMENT: {2/16/2012 5:46:56 PM}	{
// COMMENT: {2/16/2012 5:46:56 PM}		this->disp_list.push_back(rDisp);
// COMMENT: {2/16/2012 5:46:56 PM}	}
// COMMENT: {2/16/2012 5:46:56 PM}
// COMMENT: {2/16/2012 5:46:56 PM}	// Line 8:     -correct_disp          true
// COMMENT: {2/16/2012 5:46:56 PM}	this->correct_disp       = ::correct_disp;          // m_Page3.m_bCorrectDisp
// COMMENT: {2/16/2012 5:46:56 PM}
// COMMENT: {2/16/2012 5:46:56 PM}	// Line 9:     -diffusion_coefficient 1.0e-9
// COMMENT: {2/16/2012 5:46:56 PM}	this->diffc              = ::diffc;                 // m_Page3.m_dDiffCoef
// COMMENT: {2/16/2012 5:46:56 PM}
// COMMENT: {2/16/2012 5:46:56 PM}	// Line 10:    -stagnant              1  6.8e-6   0.3   0.1
// COMMENT: {2/16/2012 5:46:56 PM}	this->count_stag = ::stag_data->count_stag;         // m_Page5.m_nStagCells
// COMMENT: {2/16/2012 5:46:56 PM}	this->exch_f     = ::stag_data->exch_f;             // m_Page5.m_dExchFactor
// COMMENT: {2/16/2012 5:46:56 PM}	this->th_m       = ::stag_data->th_m;               // m_Page5.m_dThetaM
// COMMENT: {2/16/2012 5:46:56 PM}	this->th_im      = ::stag_data->th_im;              // m_Page5.m_dThetaIM
// COMMENT: {2/16/2012 5:46:56 PM}
// COMMENT: {2/16/2012 5:46:56 PM}	// Line 11:    -thermal_diffusion     3.0   0.5e-6
// COMMENT: {2/16/2012 5:46:56 PM}	this->tempr              = ::tempr;                 // m_Page3.m_dTRF
// COMMENT: {2/16/2012 5:46:56 PM}	this->heat_diffc         = ::heat_diffc;            // m_Page3.m_dTDC
// COMMENT: {2/16/2012 5:46:56 PM}
// COMMENT: {2/16/2012 5:46:56 PM}	// Line 12:    -initial_time          1000
// COMMENT: {2/16/2012 5:46:56 PM}	this->initial_total_time = ::initial_total_time;    // m_Page1.m_dInitTime
// COMMENT: {2/16/2012 5:46:56 PM}
// COMMENT: {2/16/2012 5:46:56 PM}	// Line 13:    -print_cells           1-3 5
// COMMENT: {2/16/2012 5:46:56 PM}	this->UpdatePrintRange(this->print_range_list);     // m_Page2.m_listPrintRange
// COMMENT: {2/16/2012 5:46:56 PM}
// COMMENT: {2/16/2012 5:46:56 PM}	// Line 14:    -print_frequency       5
// COMMENT: {2/16/2012 5:46:56 PM}	this->print_modulus      = ::print_modulus;         // m_Page2.m_nPrintModulus
// COMMENT: {2/16/2012 5:46:56 PM}
// COMMENT: {2/16/2012 5:46:56 PM}	// Line 15:    -punch_cells           2-5
// COMMENT: {2/16/2012 5:46:56 PM}	this->UpdatePunchRange(this->punch_range_list);     // m_Page2.m_listPunchRange
// COMMENT: {2/16/2012 5:46:56 PM}
// COMMENT: {2/16/2012 5:46:56 PM}	// Line 16:    -punch_frequency       5
// COMMENT: {2/16/2012 5:46:56 PM}	this->punch_modulus      = ::punch_modulus;         // m_Page2.m_nPunchModulus
// COMMENT: {2/16/2012 5:46:56 PM}
// COMMENT: {2/16/2012 5:46:56 PM}	// Line 17:    -dump                  dump.file
// COMMENT: {2/16/2012 5:46:56 PM}	this->dump_in        = ::dump_in;                   // m_Page5.m_bDumpToFile
// COMMENT: {2/16/2012 5:46:56 PM}	this->dump_file_name = ::dump_file_name;            // m_Page5.m_strDumpFileName
// COMMENT: {2/16/2012 5:46:56 PM}
// COMMENT: {2/16/2012 5:46:56 PM}	// Line 18:    -dump_frequency        10
// COMMENT: {2/16/2012 5:46:56 PM}	this->dump_modulus       = ::dump_modulus;          // m_Page5.m_nDumpModulus
// COMMENT: {2/16/2012 5:46:56 PM}
// COMMENT: {2/16/2012 5:46:56 PM}	// Line 19:    -dump_restart          20
// COMMENT: {2/16/2012 5:46:56 PM}	this->transport_start    = ::transport_start;       // m_Page5.m_nDumpRestart
// COMMENT: {2/16/2012 5:46:56 PM}
// COMMENT: {2/16/2012 5:46:56 PM}	// Line 20:    -warnings              false
// COMMENT: {2/16/2012 5:46:56 PM}	this->transport_warnings = ::transport_warnings;    // m_Page1.m_bPrintWarnings
// COMMENT: {2/16/2012 5:46:56 PM}
// COMMENT: {2/16/2012 5:46:56 PM}	// Multicomponent diffusion
// COMMENT: {2/16/2012 5:46:56 PM}	// Line 21: -multi_d true 1e-9 0.3 0.05 1.0
// COMMENT: {2/16/2012 5:46:56 PM}	this->multi_Dflag        = ::multi_Dflag;           // m_Page6.m_bUseMCD
// COMMENT: {2/16/2012 5:46:56 PM}	this->default_Dw         = ::default_Dw;            // m_Page6.m_default_Dw
// COMMENT: {2/16/2012 5:46:56 PM}	this->multi_Dpor         = ::multi_Dpor;            // m_Page6.m_multi_Dpor
// COMMENT: {2/16/2012 5:46:56 PM}	this->multi_Dpor_lim     = ::multi_Dpor_lim;        // m_Page6.m_multi_Dpor_lim
// COMMENT: {2/16/2012 5:46:56 PM}	this->multi_Dn           = ::multi_Dn;              // m_Page6.m_multi_Dn
// COMMENT: {2/16/2012 5:46:56 PM}
// COMMENT: {2/16/2012 5:46:56 PM}	// Interlayer diffusion
// COMMENT: {2/16/2012 5:46:56 PM}	// Line 22: -interlayer_D true 0.09 0.01 150
// COMMENT: {2/16/2012 5:46:56 PM}	this->interlayer_Dflag    = ::interlayer_Dflag;     // m_Page6.m_bUseID
// COMMENT: {2/16/2012 5:46:56 PM}	this->interlayer_Dpor     = ::interlayer_Dpor;      // m_Page6.m_interlayer_Dpor
// COMMENT: {2/16/2012 5:46:56 PM}	this->interlayer_Dpor_lim = ::interlayer_Dpor_lim;  // m_Page6.m_interlayer_Dpor_lim
// COMMENT: {2/16/2012 5:46:56 PM}	this->interlayer_tortf    = ::interlayer_tortf;     // m_Page6.m_interlayer_tortf
// COMMENT: {2/16/2012 5:46:56 PM}
// COMMENT: {2/16/2012 5:46:56 PM}	// count of TRANSPORT keywords
// COMMENT: {2/16/2012 5:46:56 PM}	this->simul_tr = ::simul_tr;
}

void CTransport::UpdatePrintRange(std::list<CRange> &list)
{
	list.clear();

// COMMENT: {2/16/2012 5:47:15 PM}	CRange range;
// COMMENT: {2/16/2012 5:47:15 PM}	range.nMin = -1;
// COMMENT: {2/16/2012 5:47:15 PM}	int max_cell;
// COMMENT: {2/16/2012 5:47:15 PM}	if (::stag_data->count_stag)
// COMMENT: {2/16/2012 5:47:15 PM}	{
// COMMENT: {2/16/2012 5:47:15 PM}		max_cell = (::stag_data->count_stag  + 1) * ::count_cells + 1;
// COMMENT: {2/16/2012 5:47:15 PM}	}
// COMMENT: {2/16/2012 5:47:15 PM}	else
// COMMENT: {2/16/2012 5:47:15 PM}	{
// COMMENT: {2/16/2012 5:47:15 PM}		max_cell = ::count_cells;
// COMMENT: {2/16/2012 5:47:15 PM}	}
// COMMENT: {2/16/2012 5:47:15 PM}	for (int i = 0; i < max_cell; ++i)
// COMMENT: {2/16/2012 5:47:15 PM}	{
// COMMENT: {2/16/2012 5:47:15 PM}		if (::cell_data[i].print == TRUE)
// COMMENT: {2/16/2012 5:47:15 PM}		{
// COMMENT: {2/16/2012 5:47:15 PM}			if (range.nMin == -1)
// COMMENT: {2/16/2012 5:47:15 PM}			{
// COMMENT: {2/16/2012 5:47:15 PM}				range.nMin = i + 1;
// COMMENT: {2/16/2012 5:47:15 PM}			}
// COMMENT: {2/16/2012 5:47:15 PM}			range.nMax = i + 1;
// COMMENT: {2/16/2012 5:47:15 PM}		}
// COMMENT: {2/16/2012 5:47:15 PM}		else
// COMMENT: {2/16/2012 5:47:15 PM}		{
// COMMENT: {2/16/2012 5:47:15 PM}			if (range.nMin != -1)
// COMMENT: {2/16/2012 5:47:15 PM}			{
// COMMENT: {2/16/2012 5:47:15 PM}				list.push_back(range);
// COMMENT: {2/16/2012 5:47:15 PM}				range.nMin = -1;
// COMMENT: {2/16/2012 5:47:15 PM}			}
// COMMENT: {2/16/2012 5:47:15 PM}		}
// COMMENT: {2/16/2012 5:47:15 PM}	}
// COMMENT: {2/16/2012 5:47:15 PM}	if (range.nMin != -1)
// COMMENT: {2/16/2012 5:47:15 PM}	{
// COMMENT: {2/16/2012 5:47:15 PM}		// No ranges if all cells are selected
// COMMENT: {2/16/2012 5:47:15 PM}		if (range.nMin == 1 && range.nMax == ::count_cells)
// COMMENT: {2/16/2012 5:47:15 PM}		{
// COMMENT: {2/16/2012 5:47:15 PM}			ASSERT(list.empty());
// COMMENT: {2/16/2012 5:47:15 PM}		}
// COMMENT: {2/16/2012 5:47:15 PM}		else
// COMMENT: {2/16/2012 5:47:15 PM}		{
// COMMENT: {2/16/2012 5:47:15 PM}			list.push_back(range);
// COMMENT: {2/16/2012 5:47:15 PM}		}
// COMMENT: {2/16/2012 5:47:15 PM}
// COMMENT: {2/16/2012 5:47:15 PM}	}
}

void CTransport::UpdatePunchRange(std::list<CRange> &list)
{
	list.clear();

// COMMENT: {2/16/2012 5:47:32 PM}	CRange range;
// COMMENT: {2/16/2012 5:47:32 PM}	range.nMin = -1;
// COMMENT: {2/16/2012 5:47:32 PM}	int max_cell;
// COMMENT: {2/16/2012 5:47:32 PM}	if (::stag_data->count_stag)
// COMMENT: {2/16/2012 5:47:32 PM}	{
// COMMENT: {2/16/2012 5:47:32 PM}		max_cell = (::stag_data->count_stag  + 1) * ::count_cells + 1;
// COMMENT: {2/16/2012 5:47:32 PM}	}
// COMMENT: {2/16/2012 5:47:32 PM}	else
// COMMENT: {2/16/2012 5:47:32 PM}	{
// COMMENT: {2/16/2012 5:47:32 PM}		max_cell = ::count_cells;
// COMMENT: {2/16/2012 5:47:32 PM}	}
// COMMENT: {2/16/2012 5:47:32 PM}	for (int i = 0; i < max_cell; ++i)
// COMMENT: {2/16/2012 5:47:32 PM}	{
// COMMENT: {2/16/2012 5:47:32 PM}		if (::cell_data[i].punch == TRUE)
// COMMENT: {2/16/2012 5:47:32 PM}		{
// COMMENT: {2/16/2012 5:47:32 PM}			if (range.nMin == -1)
// COMMENT: {2/16/2012 5:47:32 PM}			{
// COMMENT: {2/16/2012 5:47:32 PM}				range.nMin = i + 1;
// COMMENT: {2/16/2012 5:47:32 PM}			}
// COMMENT: {2/16/2012 5:47:32 PM}			range.nMax = i + 1;
// COMMENT: {2/16/2012 5:47:32 PM}		}
// COMMENT: {2/16/2012 5:47:32 PM}		else
// COMMENT: {2/16/2012 5:47:32 PM}		{
// COMMENT: {2/16/2012 5:47:32 PM}			if (range.nMin != -1)
// COMMENT: {2/16/2012 5:47:32 PM}			{
// COMMENT: {2/16/2012 5:47:32 PM}				list.push_back(range);
// COMMENT: {2/16/2012 5:47:32 PM}				range.nMin = -1;
// COMMENT: {2/16/2012 5:47:32 PM}			}
// COMMENT: {2/16/2012 5:47:32 PM}		}
// COMMENT: {2/16/2012 5:47:32 PM}	}
// COMMENT: {2/16/2012 5:47:32 PM}	if (range.nMin != -1)
// COMMENT: {2/16/2012 5:47:32 PM}	{
// COMMENT: {2/16/2012 5:47:32 PM}		// No ranges if all cells are selected
// COMMENT: {2/16/2012 5:47:32 PM}		if (range.nMin == 1 && range.nMax == ::count_cells)
// COMMENT: {2/16/2012 5:47:32 PM}		{
// COMMENT: {2/16/2012 5:47:32 PM}			ASSERT(list.empty());
// COMMENT: {2/16/2012 5:47:32 PM}		}
// COMMENT: {2/16/2012 5:47:32 PM}		else
// COMMENT: {2/16/2012 5:47:32 PM}		{
// COMMENT: {2/16/2012 5:47:32 PM}			list.push_back(range);
// COMMENT: {2/16/2012 5:47:32 PM}		}
// COMMENT: {2/16/2012 5:47:32 PM}	}
}
