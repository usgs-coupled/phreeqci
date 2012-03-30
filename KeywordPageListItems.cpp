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
#include "phreeqc3/src/Solution.h"
#include "phreeqc3/src/ISolutionComp.h"
#include "phreeqc3/src/SS.h"
#include "phreeqc3/src/SScomp.h"

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

CNameCoef::CNameCoef()
: m_strName(_T(""))
, m_dCoef(std::numeric_limits<double>::signaling_NaN())
{
	_ASSERTE( std::numeric_limits<double>::has_signaling_NaN );
}

CNameCoef::CNameCoef(const struct name_coef* name_coef_ptr)
: m_strName(name_coef_ptr->name)
{
	ASSERT( std::numeric_limits<double>::has_signaling_NaN );

	m_dCoef = (name_coef_ptr->coef == NAN) ?
		std::numeric_limits<double>::signaling_NaN() : name_coef_ptr->coef;
}
CNameCoef::CNameCoef(cxxNameDouble::const_iterator ci)
: m_strName(ci->first.c_str())
, m_dCoef((ci->second == NAN) ? std::numeric_limits<double>::signaling_NaN() : ci->second)
{
	ASSERT( std::numeric_limits<double>::has_signaling_NaN );
}

// use implicit copy ctor

CNameCoef::~CNameCoef()
{
}

//////////////////////////////////////////////////////////////////////
// CConc Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CConc::CConc()
{
	m_strDesc    = _T("");		
	m_dInputConc = std::numeric_limits<double>::signaling_NaN();
	m_strUnits   = _T("(Default)");
	m_strAs      = _T("");		
	m_dGFW       = std::numeric_limits<double>::signaling_NaN();
	m_strRedox   = _T("");
	m_dPhaseSI   = std::numeric_limits<double>::signaling_NaN();
	m_strPhase   = _T("");
}

#if 0
CConc::CConc(const struct solution* solution_ptr, const struct conc* conc_ptr)
{
	// element list
	ASSERT(conc_ptr->description != NULL);
	m_strDesc = conc_ptr->description;
		
	// concentration
	m_dInputConc = conc_ptr->input_conc;

	// [units]
	if (conc_ptr->units != NULL && conc_ptr->units != solution_ptr->units)
	{
		m_strUnits = conc_ptr->units;		
		m_strUnits.Replace(_T("mg/kgs"), _T("ppm"));
		m_strUnits.Replace(_T("ug/kgs"), _T("ppb"));
		m_strUnits.Replace(_T("g/kgs"),  _T("ppt"));
	}
	else
	{
		m_strUnits = _T("(Default)");
	}

	// ([as formula] or [gfw gfw])
	if (conc_ptr->as != NULL)
	{
		m_strAs = conc_ptr->as;
	}
	if (conc_ptr->gfw != 0.0)
	{
		m_dGFW = conc_ptr->gfw;
	}
	else
	{
		m_dGFW = std::numeric_limits<double>::signaling_NaN();
	}

	// [redox couple]
	if (conc_ptr->n_pe != solution_ptr->default_pe)
	{
		ASSERT(solution_ptr->pe[conc_ptr->n_pe].name != NULL);
		m_strRedox = solution_ptr->pe[conc_ptr->n_pe].name;
	}

	// [(charge or phase name [saturation index])]
	m_dPhaseSI = std::numeric_limits<double>::signaling_NaN();
	if (conc_ptr->equation_name != NULL)
	{
		m_strPhase = conc_ptr->equation_name;

		if (m_strPhase.CompareNoCase(_T("charge")) != 0)
		{
			m_dPhaseSI = conc_ptr->phase_si;
		}
	}
}
#else
CConc::CConc(const cxxSolution* soln, const cxxISolutionComp* comp)
{
	// element list
	ASSERT(!comp->Get_description().empty());
	this->m_strDesc = comp->Get_description().c_str();

	// concentration
	this->m_dInputConc = comp->Get_input_conc();

	// [units]
	if (!comp->Get_units().empty() && (comp->Get_units() != soln->Get_initial_data()->Get_units()))
	{
		this->m_strUnits = comp->Get_units().c_str();
		this->m_strUnits.Replace(_T("mg/kgs"), _T("ppm"));
		this->m_strUnits.Replace(_T("ug/kgs"), _T("ppb"));
		this->m_strUnits.Replace(_T("g/kgs"),  _T("ppt"));
	}
	else
	{
		this->m_strUnits = _T("(Default)");
	}

	// ([as formula] or [gfw gfw])
	if (!comp->Get_as().empty())
	{
		this->m_strAs = comp->Get_as().c_str();
	}
	if (comp->Get_gfw() != 0.0)
	{
		m_dGFW = comp->Get_gfw();
	}
	else
	{
		m_dGFW = std::numeric_limits<double>::signaling_NaN();
	}

	// [redox couple]
	if (comp->Get_pe_reaction().compare(soln->Get_initial_data()->Get_default_pe()) != 0)
	{
		this->m_strRedox = comp->Get_pe_reaction().c_str();
	}

	// [(charge or phase name [saturation index])]
	this->m_dPhaseSI = std::numeric_limits<double>::signaling_NaN();
	if (!comp->Get_equation_name().empty())
	{
		m_strPhase = comp->Get_equation_name().c_str();

		if (m_strPhase.CompareNoCase(_T("charge")) != 0)
		{
			this->m_dPhaseSI = comp->Get_phase_si();
		}
	}
}
#endif

CConc::~CConc()
{
}

CConc CConc::Create(LPCTSTR psz)
{
	/*
	Test cases
	mg/kgw as HCO3
	ppm
	mg/liter
	mg/L
	as HCO3

	"     as SO4"
	"      as S  "
	"    as HCO3      CO2(g)     -3.5"
	"     ug/kgs as Fe S(6)/S(-2) Pyrite"


	*/

	CConc aConc;
	CString string(psz);
	string.TrimLeft();
	string.TrimRight();

	aConc.m_strUnits = GetUnits(string);
	aConc.m_strAs    = GetAs(string);
	aConc.m_strGFW = GetGFW(string);

	if (!string.IsEmpty())
	{
		aConc.m_strRedox = GetRedox(string);
	}
	if (!string.IsEmpty())
	{
		GetPhase(string, aConc);
	}

	ASSERT(string.IsEmpty());

	return aConc;
}

//////////////////////////////////////////////////////////////////////
// CIsotope Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIsotope::CIsotope()
{
	m_dIsotopeNumber    = std::numeric_limits<double>::signaling_NaN();
	m_strEltName        = _T("");
	m_strName           = _T("");
	m_dRatio            = std::numeric_limits<double>::signaling_NaN();
	m_dRatioUncertainty = std::numeric_limits<double>::signaling_NaN();
}
CIsotope::CIsotope(const struct isotope* isotope_ptr)
{
	m_dIsotopeNumber    = isotope_ptr->isotope_number;
	m_strEltName        = isotope_ptr->elt_name;
	m_strName.Format(_T("%d%s"), (int)m_dIsotopeNumber, m_strEltName);
	m_dRatio            = isotope_ptr->ratio;
	m_dRatioUncertainty = (isotope_ptr->ratio_uncertainty != NAN) ?
		isotope_ptr->ratio_uncertainty : std::numeric_limits<double>::signaling_NaN();

}
CIsotope::CIsotope(const cxxSolutionIsotope* iso)
{
	this->m_dIsotopeNumber    = iso->Get_isotope_number();
	this->m_strEltName        = iso->Get_elt_name().c_str();
	this->m_strName.Format(_T("%d%s"), (int)iso->Get_isotope_number(), iso->Get_elt_name().c_str());
	this->m_dRatio            = iso->Get_ratio();;
	this->m_dRatioUncertainty = (iso->Get_ratio_uncertainty_defined() ? iso->Get_ratio_uncertainty() : std::numeric_limits<double>::signaling_NaN());
}
CIsotope::CIsotope(const struct iso* iso_ptr)
{
	m_strEltName        = _T("");
	m_strName           = iso_ptr->name;
	m_dIsotopeNumber    = std::numeric_limits<double>::signaling_NaN();
	m_dRatio            = (iso_ptr->value != NAN) ? iso_ptr->value : std::numeric_limits<double>::signaling_NaN();
	m_dRatioUncertainty = (iso_ptr->uncertainty != NAN) ? iso_ptr->uncertainty : std::numeric_limits<double>::signaling_NaN();
}
//////////////////////////////////////////////////////////////////////
// CInvIsotope Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInvIsotope::CInvIsotope()
: CIsotope()
{
}
CInvIsotope::CInvIsotope(const struct isotope* isotope_ptr)
: CIsotope(isotope_ptr)
{
}

CInvIsotope::CInvIsotope(const struct inv_isotope* inv_isotope_ptr)
{

	m_dIsotopeNumber    = inv_isotope_ptr->isotope_number;
	m_strEltName        = inv_isotope_ptr->elt_name;
	m_dRatio            = std::numeric_limits<double>::signaling_NaN();
	m_dRatioUncertainty = std::numeric_limits<double>::signaling_NaN();

	// m_strName           = inv_isotope_ptr->isotope_name;
	m_strName.Format(_T("%d%s"), (int)m_dIsotopeNumber, m_strEltName);
}

CString CInvIsotope::GetString(std::list<InvSol>& rlistInvSol, InvSol& rFinalInvSol, std::map<CString, double>& rDefaults)
{
	CString str;
	CString strNums;

	str.Format(_T("%4c%-11s"),
		_T(' '),
		m_strName
		);

	// foreach init soln
	std::list<InvSol>::iterator iterSol = rlistInvSol.begin();
	for (; iterSol != rlistInvSol.end(); ++iterSol)
	{
		std::map<int, double>::iterator iterUnc = m_mapSol2Unc.find(iterSol->nSol);
		if (iterUnc != m_mapSol2Unc.end())
		{
			strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, iterUnc->second);		
		}
		else
		{
			std::map<CString, double>::iterator iterFind = rDefaults.find(m_strName);
			if (iterFind != rDefaults.end())
			{
				strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, iterFind->second);		
			}
			else
			{
				strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, 0.0);		
			}
		}
		str += strNums;
	}

	// final soln
	std::map<int, double>::iterator iterUnc = m_mapSol2Unc.find(rFinalInvSol.nSol);
	if (iterUnc != m_mapSol2Unc.end())
	{
		strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, iterUnc->second);
	}
	else
	{
		std::map<CString, double>::iterator iterFind = rDefaults.find(m_strName);
		if (iterFind != rDefaults.end())
		{
			strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, iterFind->second);		
		}
		else
		{
			strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, 0.0);		
		}
	}
	str += strNums;
	str.TrimRight();
	return str;
}

//////////////////////////////////////////////////////////////////////
// CS_S_Comp Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CS_S_Comp::CS_S_Comp()
{
	_ASSERTE( std::numeric_limits<long double>::has_signaling_NaN );
	m_ldMoles = std::numeric_limits<long double>::signaling_NaN();
	m_strName = _T("");
}

#if 0
CS_S_Comp::CS_S_Comp(const struct s_s_comp* s_s_comp_ptr)
{
	m_strName = s_s_comp_ptr->name;
	m_ldMoles = s_s_comp_ptr->moles;
}
#else
CS_S_Comp::CS_S_Comp(const cxxSScomp* comp)
{
	this->m_strName = comp->Get_name().c_str();
	this->m_ldMoles = comp->Get_moles();
}
#endif

CS_S_Comp::~CS_S_Comp()
{
}

//////////////////////////////////////////////////////////////////////
// CS_S Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CS_S::CS_S()
{
	_ASSERTE(std::numeric_limits<long double>::has_signaling_NaN);
	_ASSERTE(std::numeric_limits<double>::has_signaling_NaN);

	m_strName = _T("");
	m_arrldP[0] = m_arrldP[1] = m_arrldP[2] = m_arrldP[3] = std::numeric_limits<long double>::signaling_NaN();
	m_dTk = std::numeric_limits<double>::signaling_NaN();
}

#if 0
CS_S::CS_S(const struct s_s* s_s_ptr)
{
	m_strName = s_s_ptr->name;
	m_dTk     = s_s_ptr->tk;

	m_nInputCase = static_cast<enum InputCase>(s_s_ptr->input_case);
	int nParams = 0;

	switch (s_s_ptr->input_case)
	{
	case IC_GUGG_NONDIMENSIONAL :
	case IC_MISCIBILITY_GAP :
	case IC_SPINODAL_GAP :
	case IC_CRITICAL_POINT :
	case IC_ALYOTROPIC_POINT :
	case IC_GUGG_KJ :
	case IC_THOMPSON :
	case IC_MARGULES :
		nParams = 2;
		break;
	case IC_ACTIVITY_COEFFICIENTS:
	case IC_DISTRIBUTION_COEFFICIENTS :
		nParams = 4;
		break;
	default :
		ASSERT(FALSE);
	}

	int i = 0;
	for (; i < nParams; ++i)
	{
		m_arrldP[i] = s_s_ptr->p[i];
	}
	// fill remaining
	for (int j = i; j < 4; ++j)
	{
		m_arrldP[j] = std::numeric_limits<long double>::signaling_NaN();
	}

	for (i = 0; i < s_s_ptr->count_comps; ++i)
	{
		CS_S_Comp comp(&s_s_ptr->comps[i]);
		m_listComp.push_back(comp);
	}
}
# else
CS_S::CS_S(const cxxSS* ss)
{
	this->m_strName = ss->Get_name().c_str();
	this->m_dTk     = ss->Get_tk();

	this->m_nInputCase = static_cast<enum InputCase>(ss->Get_input_case());
	if (this->m_nInputCase == cxxSS::SS_PARM_NONE)
	{
		// hack for new ErrorHandling version
		this->m_nInputCase = CS_S::IC_GUGG_NONDIMENSIONAL;
	}

	size_t nParams = 0;
	switch (this->m_nInputCase)
	{
	case IC_GUGG_NONDIMENSIONAL :
	case IC_MISCIBILITY_GAP :
	case IC_SPINODAL_GAP :
	case IC_CRITICAL_POINT :
	case IC_ALYOTROPIC_POINT :
	case IC_GUGG_KJ :
	case IC_THOMPSON :
	case IC_MARGULES :
		nParams = 2;
		break;
	case IC_ACTIVITY_COEFFICIENTS:
	case IC_DISTRIBUTION_COEFFICIENTS :
		nParams = 4;
		break;
	default :
		ASSERT(FALSE);
	}

	size_t i = 0;
	for (; i < nParams; ++i)
	{
		this->m_arrldP[i] = ss->Get_p()[i];
	}

	// fill remaining
	for (size_t j = i; j < 4; ++j)
	{
		this->m_arrldP[j] = std::numeric_limits<long double>::signaling_NaN();
	}

	for (i = 0; i < ss->Get_ss_comps().size(); ++i)
	{
		CS_S_Comp comp(&ss->Get_ss_comps()[i]);
		this->m_listComp.push_back(comp);
	}
}
# endif

// use implicit copy ctor

CS_S::~CS_S()
{
}

//////////////////////////////////////////////////////////////////////
// CRate Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRate::CRate()
: m_strName(_T(""))
{
}

CRate::CRate(const struct rate *rate_ptr)
{
	m_strName = rate_ptr->name;
	// parse commands

	CString str = rate_ptr->commands;
	LPTSTR lpsz = str.GetBuffer(str.GetLength() + 4);
	if (lpsz == NULL)
	{
		ASSERT(FALSE);
	}
	else
	{
		// get first line
		LPTSTR lpszLine = _tcstok(lpsz, _T(";"));
		while (lpszLine)
		{
			basic_command command;
			// get line number
			LPTSTR lpszCommand;
			command.nLine = strtol(lpszLine, &lpszCommand, 10);
			// eat single space
			if (lpszCommand && lpszCommand[0] == _T(' '))
				lpszCommand++;
			command.strCommand = lpszCommand;

			// test for tabs
			command.strCommand.Replace(_T('\t'), _T(' '));
			ASSERT(command.strCommand.Find(_T("\t"), 0) == -1);

			// get next line
			lpszLine = _tcstok(NULL, _T(";"));
			m_listCommands.push_back(command);
		}
	}
}
CString CRate::GetString()
{
	CString str;
	str.Format("%4c%s%s%4c-start%s",
		' ',
		m_strName,
		"\r\n",
		' ',
		"\r\n"
		);
	std::list<basic_command>::iterator commandIter = m_listCommands.begin();
	CString strLine;
	for (; commandIter != m_listCommands.end(); ++commandIter)
	{
		strLine.Format("%d %s%s",
			(*commandIter).nLine,
			(*commandIter).strCommand,
			"\r\n"
			);
		str += strLine;
	}
	return str;
}

CRate::~CRate()
{
}

//////////////////////////////////////////////////////////////////////
// CInvPhase Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInvPhase::CInvPhase()
: m_strName(_T("")), m_bForce(false), m_nConstraint(IPC_EITHER)
{
}

CInvPhase::CInvPhase(const struct inv_phases *inv_phases_ptr)
{
	m_strName = inv_phases_ptr->name;

	m_bForce = (inv_phases_ptr->force == TRUE);

	switch (inv_phases_ptr->constraint)
	{
	case EITHER :
		m_nConstraint = IPC_EITHER;
		break;

	case PRECIPITATE :
		m_nConstraint = IPC_PRECIPITATE;
		break;

	case DISSOLVE :
		m_nConstraint = IPC_DISSOLVE;
		break;

	default:
		m_nConstraint = IPC_EITHER;
		ASSERT(FALSE);
		break;
	}
}

CString CInvPhase::GetString()
{
	return GetString(11);
}

CString CInvPhase::GetString(int nWidth)
{
	CString str;

	str.Format(_T("%4c%-*s %s"),
		_T(' '),
		nWidth,
		m_strName,
		m_bForce ? _T("force") : _T("     ")
		);

	switch (m_nConstraint)
	{
	case IPC_PRECIPITATE :
		str += _T(" pre");
		break;
	case IPC_DISSOLVE :
		str += _T(" dis");
		break;
	}

	// foreach isotope
	CString strIso;
	std::map<CString, CIsotope>::iterator iterPair = m_mapIsotopes.begin();
	for (; iterPair != m_mapIsotopes.end(); ++iterPair)
	{
		strIso.Format("   %s   %-*.*g %-*.*g",
			(LPCTSTR)iterPair->second.m_strName,
			6,
			DBL_DIG,
			iterPair->second.m_dRatio,
			6,
			DBL_DIG,
			iterPair->second.m_dRatioUncertainty
			);
		str += strIso;
	}

	str.TrimRight();
	return str;
}


CInvPhase::~CInvPhase()
{
}

//////////////////////////////////////////////////////////////////////
// CInvElem Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInvElem::CInvElem()
: m_strName(_T(""))
{
}

CString CInvElem::GetString(std::list<InvSol>& rlistInvSol, InvSol& rFinalInvSol)
{
	CString str;
	CString strNums;

	str.Format(_T("%4c%-11s"),
		_T(' '),
		m_strName
		);

	// foreach init soln
	std::list<InvSol>::iterator iterSol = rlistInvSol.begin();
	for (; iterSol != rlistInvSol.end(); ++iterSol)
	{
		std::map<int, double>::iterator iterUnc = m_mapSol2Unc.find(iterSol->nSol);
		if (iterUnc != m_mapSol2Unc.end())
		{
			strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, iterUnc->second);		
		}
		else
		{
			if (m_strName.CompareNoCase(_T("pH")) == 0)
			{
				strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, 0.05);		
			}
			else
			{
				strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, iterSol->dUncertainty);		
			}
		}
		str += strNums;
	}

	// final soln
	std::map<int, double>::iterator iterUnc = m_mapSol2Unc.find(rFinalInvSol.nSol);
	if (iterUnc != m_mapSol2Unc.end())
	{
		strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, iterUnc->second);
	}
	else
	{
		if (m_strName.CompareNoCase(_T("pH")) == 0)
		{
			strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, 0.05);		
		}
		else
		{
			strNums.Format(_T(" %-*.*g"), 8, DBL_DIG, rFinalInvSol.dUncertainty);
		}
	}

	str += strNums;
	str.TrimRight();
	return str;
}

CInvElem::~CInvElem()
{
}

//////////////////////////////////////////////////////////////////////
// CSurfComp Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSurfComp::CSurfComp()
{
	m_strFormula        = _T("");
	m_dSpecific_area    = -99.9;
	m_dPhase_proportion = -99.9;
	m_dGrams            = -99.9;
	m_dMoles            = -99.9;
	m_dCapacitance0     = -99.9;
	m_dCapacitance1     = -99.9;
	m_strPhase_name     = _T("");
	m_strRate_name      = _T("");
	m_dDw               = 0.;
}

#if 0

CSurfComp::CSurfComp(const struct surface* surface_ptr, const struct surface_comp* surface_comp_ptr)
{
	ASSERT(surface_comp_ptr->formula != NULL);

	m_dSpecific_area    = surface_ptr->charge[surface_comp_ptr->charge].specific_area;
	m_dGrams            = surface_ptr->charge[surface_comp_ptr->charge].grams;
	m_dCapacitance0     = surface_ptr->charge[surface_comp_ptr->charge].capacitance[0];
	m_dCapacitance1     = surface_ptr->charge[surface_comp_ptr->charge].capacitance[1];
	m_strFormula        = surface_comp_ptr->formula;
	m_dMoles            = surface_comp_ptr->moles;
	m_strPhase_name     = surface_comp_ptr->phase_name == NULL ? _T("") : surface_comp_ptr->phase_name;
	m_strRate_name      = surface_comp_ptr->rate_name  == NULL ? _T("") : surface_comp_ptr->rate_name;
	m_dPhase_proportion = surface_comp_ptr->phase_proportion;
	m_dDw               = surface_comp_ptr->Dw;
}

#else

CSurfComp::CSurfComp(const cxxSurface* surface_ptr, const cxxSurfaceComp* surface_comp_ptr)
{
	ASSERT(!surface_comp_ptr->Get_formula().empty());

	if (const cxxSurfaceCharge* c = surface_ptr->Find_charge(surface_comp_ptr->Get_charge_name()))
	{
		this->m_dSpecific_area = c->Get_specific_area();
		this->m_dGrams         = c->Get_grams();
		this->m_dCapacitance0  = c->Get_capacitance0();
		this->m_dCapacitance1  = c->Get_capacitance1();

	}
	else
	{
		ASSERT(FALSE);
	}

	this->m_strFormula        = surface_comp_ptr->Get_formula().c_str();
	this->m_dMoles            = surface_comp_ptr->Get_moles();
	this->m_strPhase_name     = surface_comp_ptr->Get_phase_name().c_str();
	this->m_strRate_name      = surface_comp_ptr->Get_rate_name().c_str();
	this->m_dPhase_proportion = surface_comp_ptr->Get_phase_proportion();
	this->m_dDw               = surface_comp_ptr->Get_Dw();
}

#endif

// use implicit copy ctor

CSurfComp::~CSurfComp()
{
}


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
CString CConc::GetUnits(CString &rStr)
{
	// fill set with available units
	std::set<CString> setUnits;
	setUnits.insert(_T("Mol/l"));
	setUnits.insert(_T("mMol/l"));
	setUnits.insert(_T("uMol/l"));
	setUnits.insert(_T("g/l"));
	setUnits.insert(_T("mg/l"));
	setUnits.insert(_T("ug/l"));
	setUnits.insert(_T("Mol/kgs"));
	setUnits.insert(_T("mMol/kgs"));
	setUnits.insert(_T("uMol/kgs"));
	setUnits.insert(_T("g/kgs"));
	setUnits.insert(_T("mg/kgs"));
	setUnits.insert(_T("ug/kgs"));
	setUnits.insert(_T("Mol/kgw"));
	setUnits.insert(_T("mMol/kgw"));
	setUnits.insert(_T("uMol/kgw"));
	setUnits.insert(_T("g/kgw"));
	setUnits.insert(_T("mg/kgw"));
	setUnits.insert(_T("ug/kgw"));
	setUnits.insert(_T("eq/l"));
	setUnits.insert(_T("meq/l"));
	setUnits.insert(_T("ueq/l"));
	setUnits.insert(_T("eq/kgs"));
	setUnits.insert(_T("meq/kgs"));
	setUnits.insert(_T("ueq/kgs"));
	setUnits.insert(_T("eq/kgw"));
	setUnits.insert(_T("meq/kgw"));
	setUnits.insert(_T("ueq/kgw"));

	// Remove space between "kg" and "solution" or "water" in units
	rStr.Replace(_T("Kg"), _T("kg"));
	rStr.Replace(_T("KG"), _T("kg"));
	while (rStr.Replace(_T("kg "), _T("kg")));

	// get first token
	CString strToken = rStr.SpanExcluding(_T(" "));
	strToken.MakeLower();

	// check for parts per units
	if (strToken.Compare("ppt") == 0)
	{
		rStr = rStr.Mid(strToken.GetLength());
		rStr.TrimLeft();
		return strToken;
	}
	if (strToken.Compare("ppm") == 0)
	{
		rStr = rStr.Mid(strToken.GetLength());
		rStr.TrimLeft();
		return strToken;
	}
	if (strToken.Compare("ppb") == 0)
	{
		rStr = rStr.Mid(strToken.GetLength());
		rStr.TrimLeft();
		return strToken;
	}

	// save orignal token
	CString strTokenOrig(strToken);

	strToken.Replace("milli",      "m");
	strToken.Replace("micro",      "u");
	strToken.Replace("grams",      "g");
	strToken.Replace("gram",       "g");
	strToken.Replace("moles",      "Mol");
	strToken.Replace("mole",       "Mol");
	strToken.Replace("mol",        "Mol");
	strToken.Replace("liter",      "l");
	strToken.Replace("kgh",        "kgw");
	strToken.Replace("equivalents","eq");
	strToken.Replace("equivalent", "eq");
	strToken.Replace("equiv",      "eq");

	// trim extraneous endings (ie kgsolution => kgs)
	int nFind;
	if ((nFind = strToken.Find("/l")) != -1)
	{
		strToken = strToken.Left(nFind + 2);
	}
	if ((nFind = strToken.Find("/kgs")) != -1)
	{
		strToken = strToken.Left(nFind + 4);
	}
	if ((nFind = strToken.Find("/kgw")) != -1)
	{
		strToken = strToken.Left(nFind + 4);
	}

	// check if unit is in list
	CString strUnits;
	std::set<CString>::const_iterator cIter;
	cIter = setUnits.find(strToken);
	if (cIter != setUnits.end())
	{
		strUnits = (*cIter);
		rStr = rStr.Mid(strTokenOrig.GetLength());
		rStr.TrimLeft();
	}
	return strUnits;
}

CString CConc::GetAs(CString &rStr)
{
	CString strAs;

	// get first token
	CString strToken = rStr.SpanExcluding(_T(" "));
	if (strToken.Compare("as") == 0)
	{
		rStr = rStr.Mid(strToken.GetLength());
		rStr.TrimLeft();

		strAs = rStr.SpanExcluding(" ");

		rStr = rStr.Mid(strAs.GetLength());
		rStr.TrimLeft();
	}
	return strAs;
}

CString CConc::GetGFW(CString &rStr)
{
	CString strGFW;

	// get first token
	CString strToken = rStr.SpanExcluding(_T(" "));
	if (strToken.Compare("gfw") == 0)
	{
		rStr = rStr.Mid(strToken.GetLength());
		rStr.TrimLeft();

		strGFW = rStr.SpanExcluding(" ");

		rStr = rStr.Mid(strGFW.GetLength());
		rStr.TrimLeft();
	}
	return strGFW;
}

CString CConc::GetRedox(CString &rStr)
{
	CString strRedox;

	// get first token
	CString strToken = rStr.SpanExcluding(_T(" "));
	if (strToken.Find("/") != -1)
	{
		strRedox = strToken;
		rStr = rStr.Mid(strToken.GetLength());
		rStr.TrimLeft();
	}
	return strRedox;
}

void CConc::GetPhase(CString &rStr, CConc& rConc)
{	
	CString strToken = rStr.SpanExcluding(" ");
	if (strToken.Compare("charge") != 0)
	{
		rConc.m_strPhase = strToken;
		rStr = rStr.Mid(strToken.GetLength());
		rStr.TrimLeft();

		rConc.m_strPhaseSI = rStr.SpanExcluding(" ");

		rStr = rStr.Mid(rConc.m_strPhaseSI.GetLength());
		rStr.TrimLeft();
	}
	else
	{
		rConc.m_strPhase = strToken;
		rStr = rStr.Mid(strToken.GetLength());
		rStr.TrimLeft();
	}
}

CString CConc::GetSubHeading()const
{
	// [units], (as formula] or [gfw gfw]) [redox_couple] [charge or phase_name [saturation index])]
	CString strSubHeading;

	if (!m_strUnits.IsEmpty())
	{
		strSubHeading += m_strUnits + " ";
	}
	if (!m_strAs.IsEmpty())
	{
		strSubHeading += CString("as ") + m_strAs + " ";
	}
	else if(!m_strGFW.IsEmpty())
	{
		strSubHeading += CString("gfw ") + m_strGFW + " ";
	}
	if (!m_strRedox.IsEmpty())
	{
		strSubHeading += m_strRedox + " ";
	}
	if (!m_strPhase.IsEmpty())
	{
		strSubHeading += m_strPhase + " ";
		if (!m_strPhaseSI.IsEmpty())
		{
			strSubHeading += m_strPhaseSI;
		}
	}

	strSubHeading.TrimRight();
	return strSubHeading;
}

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
