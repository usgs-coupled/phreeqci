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
#include "phreeqc3/src/KineticsComp.h"

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

CGasComp::CGasComp(const cxxGasComp* gasComp)
: m_strName(gasComp->Get_phase_name().c_str())
, m_dP_Read(gasComp->Get_p_read() == NAN ? std::numeric_limits<double>::signaling_NaN() : gasComp->Get_p_read())
{
	ASSERT(std::numeric_limits<double>::has_signaling_NaN == true);
}

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
CIsotope::CIsotope(const struct const_iso* iso_ptr)
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

CS_S_Comp::CS_S_Comp(const cxxSScomp* comp)
{
	this->m_strName = comp->Get_name().c_str();
	this->m_ldMoles = comp->Get_moles();
}

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

CRate::CRate(const std::list<std::string>& strs)
{
	std::list< std::string >::const_iterator it = strs.begin();
	for (; it != strs.end(); ++it)
	{
		//CString line((*it).c_str());
		//LPTSTR lpszLine = line.GetBuffer(line.GetLength() + 4);

		basic_command command;
		command.strCommand = (*it).c_str();
		LPTSTR lpszLine = command.strCommand.GetBuffer(command.strCommand.GetLength() + 4);

		// get line number
		LPTSTR lpszCommand;
		command.nLine = ::strtol(lpszLine, &lpszCommand, 10);

		// eat single space
		if (lpszCommand && lpszCommand[0] == _T(' '))
			lpszCommand++;
		command.strCommand = lpszCommand;

		// test for tabs
		command.strCommand.Replace(_T('\t'), _T(' '));
		ASSERT(command.strCommand.Find(_T("\t"), 0) == -1);

		m_listCommands.push_back(command);
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
// CCalcValue Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalcValue::CCalcValue()
: m_strName(_T(""))
{
}

CCalcValue::CCalcValue(const struct calculate_value *calculate_value_ptr)
{
	m_strName = calculate_value_ptr->name;
	// parse commands

	CString str = calculate_value_ptr->commands;
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

CCalcValue::CCalcValue(const std::list<std::string>& strs)
{
	std::list< std::string >::const_iterator it = strs.begin();
	for (; it != strs.end(); ++it)
	{
		//CString line((*it).c_str());
		//LPTSTR lpszLine = line.GetBuffer(line.GetLength() + 4);

		basic_command command;
		command.strCommand = (*it).c_str();
		LPTSTR lpszLine = command.strCommand.GetBuffer(command.strCommand.GetLength() + 4);

		// get line number
		LPTSTR lpszCommand;
		command.nLine = ::strtol(lpszLine, &lpszCommand, 10);

		// eat single space
		if (lpszCommand && lpszCommand[0] == _T(' '))
			lpszCommand++;
		command.strCommand = lpszCommand;

		// test for tabs
		command.strCommand.Replace(_T('\t'), _T(' '));
		ASSERT(command.strCommand.Find(_T("\t"), 0) == -1);

		m_listCommands.push_back(command);
	}
}

CString CCalcValue::GetString()
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

CCalcValue::~CCalcValue()
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

CKineticComp::CKineticComp()
{
	m_dTol          = std::numeric_limits<double>::signaling_NaN();
	m_dM            = std::numeric_limits<double>::signaling_NaN();
	m_dInitialMoles = std::numeric_limits<double>::signaling_NaN();
	m_dM0           = std::numeric_limits<double>::signaling_NaN();
	m_dMoles        = std::numeric_limits<double>::signaling_NaN();

	m_dM            = 1.0;
	m_dM0           = 1.0;
	m_dTol          = 1e-8;
}
CKineticComp::CKineticComp(const cxxKineticsComp *comp)
{
	this->m_strRateName =  comp->Get_rate_name().c_str();
	if (m_strRateName.IsEmpty())
	{
		this->m_strRateName = _T("NoName");
	}
	
	cxxNameDouble::const_iterator i = comp->Get_namecoef().begin();
	for (; i != comp->Get_namecoef().end(); ++i)
	{
		CNameCoef nameCoef(i);
		this->m_listNameCoef.push_back(nameCoef);
	}

	this->m_dTol = comp->Get_tol();
	this->m_dM   = comp->Get_m();
	this->m_dM0  = comp->Get_m0();

	for (size_t j = 0; j < comp->Get_d_params().size(); ++j)
	{
		double d(comp->Get_d_params()[j]);
		this->m_listDParams.push_back(d);
	}
}
CKineticComp::~CKineticComp()
{
}

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
CSpecies::CSpecies()
{
	m_bHasAnalExp  = false;
	m_bHasCDMusic  = false;
	m_bHasMillero  = false;
	m_bCheckEqn    = true;
	m_nDeltaHUnits = kjoules;
	m_nActType     = AT_DAVIES;

	m_dLogK        = std::numeric_limits<double>::signaling_NaN();
	m_dDeltaH      = std::numeric_limits<double>::signaling_NaN();
	m_dA_F         = std::numeric_limits<double>::signaling_NaN();
	for (int i = 0; i < 6; ++i)
	{
		this->m_millero[i] = std::numeric_limits<double>::signaling_NaN();
	}

	// tracer diffusion coefficient in water at 25oC, m2/s
	this->m_dw = std::numeric_limits<double>::signaling_NaN();

	// enrichment factor in DDL
	this->m_erm_ddl = std::numeric_limits<double>::signaling_NaN();
}

CSpecies::CSpecies(const struct species *species_ptr)
{
	int i;

	// assoc rxn
	m_strEqn = WriteEqn(species_ptr);

	// check eqn
	m_bCheckEqn = (species_ptr->check_equation == TRUE);

	// mole balance
	m_strMoleBalance = (species_ptr->mole_balance == NULL) ? _T("") : species_ptr->mole_balance;

	// Check for analytical expression
	m_bHasAnalExp = false;
	for (i = 2; i < 8; ++i)
	{
		if (species_ptr->logk[i] != 0.0)
		{
			m_bHasAnalExp = true;
			break;
		}
	}

	// Check for CD_MUSIC
	m_bHasCDMusic = false;
	for (i = 0; i < 5; ++i)
	{
		if (species_ptr->cd_music[i] != 0.0)
		{
			m_bHasCDMusic = true;
			break;
		}
	}

	if (m_bHasAnalExp)
	{
		m_dA1 = species_ptr->logk[2];
		m_dA2 = species_ptr->logk[3];
		m_dA3 = species_ptr->logk[4];
		m_dA4 = species_ptr->logk[5];
		m_dA5 = species_ptr->logk[6];
		m_dA6 = species_ptr->logk[7];
	}

	if (m_bHasCDMusic)
	{
		for (i = 0; i < 5; ++i)
		{
			this->m_dCDMusic[i] = species_ptr->cd_music[i];
		}
	}

	// Log K
	if (!m_bHasAnalExp || species_ptr->logk[0] != 0.0)
	{
		m_dLogK = species_ptr->logk[0];
	}
	else
	{
		m_dLogK = std::numeric_limits<double>::signaling_NaN();
	}

	// Delta H
	if (species_ptr->logk[1] != 0.0)
	{
		m_dDeltaH = species_ptr->logk[1];
	}
	else
	{
		m_dDeltaH = std::numeric_limits<double>::signaling_NaN();
	}

	m_nDeltaHUnits = species_ptr->original_units;
	switch (m_nDeltaHUnits)
	{
	case joules:
		m_dDeltaH *= 1000;
		break;
	case cal:
		m_dDeltaH *= 1000/JOULES_PER_CALORIE;
		break;
	case kcal:
		m_dDeltaH /= JOULES_PER_CALORIE;
		break;
	case kjoules:
		// do nothing
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	// determine act coef type
	m_nActType = AT_NONE;
	m_dDHa = species_ptr->dha;
	m_dDHb = species_ptr->dhb;
	m_dA_F = (species_ptr->a_f == 0) ? std::numeric_limits<double>::signaling_NaN() : species_ptr->a_f;
	switch (species_ptr->gflag)
	{
	case 0:
		ASSERT(species_ptr->z == 0.0);
		ASSERT(m_dDHa == 0.0 && m_dDHb == 0.1);
		m_nActType = AT_DAVIES;
		break;
	case 1:
		m_nActType = AT_DAVIES;
		break;
	case 2:
		m_nActType = AT_DEBYE_HUCKEL;
		break;
	case 3:
		if (species_ptr->z == 0.0)
		{
			if (m_dDHa == 0.0 && m_dDHb == 0.1)
			{
				m_nActType = AT_DAVIES;
			}
			else
			{
				m_nActType = AT_DEBYE_HUCKEL;
			}
		}
		else
		{
			if (m_dDHa == 0.0 && m_dDHb == 0.0)
			{
				m_nActType = AT_DAVIES;
			}
			else
			{
				m_nActType = AT_DEBYE_HUCKEL;
			}
		}
		break;
	case 5:
		ASSERT(m_dDHa == 0.0 && m_dDHb == 0.0);
		m_nActType = AT_DAVIES;
		break;
	case 7:
		m_nActType = AT_LLNL_DH;
		break;
	case 8:
		ASSERT(m_dDHa == 0.0);
		m_nActType = AT_LLNL_DH_CO2;
		break;

	case 4: // exchange
		switch (species_ptr->exch_gflag)
		{
		case 1:
			this->m_nActType = AT_DAVIES;
			break;
		case 2:
			this->m_nActType = AT_DEBYE_HUCKEL;
			break;
		case 3:
			this->m_nActType = AT_NONE;
			break;
		case 7:
			this->m_nActType = AT_LLNL_DH;
			break;
		default:
			ASSERT(FALSE);
			break;
		}
		break;

	case 6: // surface
		if (m_dDHa == 0.0 && m_dDHb == 0.0)
		{
			m_nActType = AT_DAVIES;
		}
		else
		{
			m_nActType = AT_DEBYE_HUCKEL;
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	// tracer diffusion coefficient in water at 25oC, m2/s
	this->m_dw = (species_ptr->dw == 0) ? std::numeric_limits<double>::signaling_NaN() : species_ptr->dw;

	// enrichment factor in DDL
	this->m_erm_ddl = (species_ptr->erm_ddl == 1) ? std::numeric_limits<double>::signaling_NaN() : species_ptr->erm_ddl;

	// regression coefficients to calculate temperature dependent phi_0 and b_v of Millero density model
	this->m_bHasMillero = false;
	for (int i = 0; i < 6; ++i)
	{
		if (species_ptr->millero[i] != 0)
		{
			this->m_bHasMillero = true;
			break;
		}
	}
	for (int i = 0; i < 6; ++i)
	{
		if (this->m_bHasMillero)
		{
			this->m_millero[i] = species_ptr->millero[i];
		}
		else
		{
			this->m_millero[i] = std::numeric_limits<double>::signaling_NaN();
		}
	}
}

CSpecies::~CSpecies()
{
}

CString CSpecies::WriteEqn(const species *species_ptr)
{
	CString strEqn;
	CString strFormat;
	int i;

	// left hand side
	bool bFirst = true;
	for (i = 0; species_ptr->rxn->token[i].s != NULL; ++i)
	{
		if (species_ptr->rxn->token[i].coef < 0) continue;
		if(!bFirst)
		{
			strEqn += _T(" + ");
		}
		bFirst = false;
		if (species_ptr->rxn->token[i].coef != 1)
		{
			strFormat.Format("%g%s", species_ptr->rxn->token[i].coef, species_ptr->rxn->token[i].s->name);
		}
		else
		{
			strFormat.Format("%s", species_ptr->rxn->token[i].s->name);
		}
		strEqn += strFormat;
	}

	// right hand side
	strEqn += _T(" = ");
	bFirst = true;
	for (i = 0; species_ptr->rxn->token[i].s != NULL; ++i)
	{
		if (species_ptr->rxn->token[i].coef > 0) continue;
		if(!bFirst)
		{
			strEqn += _T(" + ");
		} 
		bFirst = false;
		if (-species_ptr->rxn->token[i].coef != 1)
		{
			strFormat.Format("%g%s", -species_ptr->rxn->token[i].coef, species_ptr->rxn->token[i].s->name);
		}
		else
		{
			strFormat.Format("%s", species_ptr->rxn->token[i].s->name);
		}
		strEqn += strFormat;
	}
	return strEqn;
}

//////////////////////////////////////////////////////////////////////
// CPhase Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPhase::CPhase()
{
	m_bHasAnalExp  = false;
	m_bCheckEqn    = true;
	m_nDeltaHUnits = kjoules;
	m_dLogK        = std::numeric_limits<double>::signaling_NaN();
	m_dDeltaH      = std::numeric_limits<double>::signaling_NaN();
}

CPhase::CPhase(const struct phase *phase_ptr)
{
	// phase name
	m_strName = (phase_ptr->name) ? phase_ptr->name : _T("");

	// diss. rxn
	m_strEqn = WriteEqn(phase_ptr);

	// check eqn
	m_bCheckEqn = (phase_ptr->check_equation == TRUE);

	// Check for analytical expression
	m_bHasAnalExp = false;
	for (int i = 2; i < 8; ++i)
	{
		if (phase_ptr->logk[i] != 0.0)
		{
			m_bHasAnalExp = true;
			break;
		}
	}
	if (m_bHasAnalExp)
	{
		m_dA1 = phase_ptr->logk[2];
		m_dA2 = phase_ptr->logk[3];
		m_dA3 = phase_ptr->logk[4];
		m_dA4 = phase_ptr->logk[5];
		m_dA5 = phase_ptr->logk[6];
		m_dA6 = phase_ptr->logk[7];
	}

	// Log K
	if (!m_bHasAnalExp || phase_ptr->logk[0] != 0.0)
	{
		m_dLogK = phase_ptr->logk[0];
	}
	else
	{
		m_dLogK = std::numeric_limits<double>::signaling_NaN();
	}

	// Delta H
	if (phase_ptr->logk[1] != 0.0)
	{
		m_dDeltaH = phase_ptr->logk[1];
	}
	else
	{
		m_dDeltaH = std::numeric_limits<double>::signaling_NaN();
	}

	m_nDeltaHUnits = phase_ptr->original_units;
	switch (m_nDeltaHUnits)
	{
	case joules:
		m_dDeltaH *= 1000;
		break;
	case cal:
		m_dDeltaH *= 1000/JOULES_PER_CALORIE;
		break;
	case kcal:
		m_dDeltaH /= JOULES_PER_CALORIE;
		break;
	case kjoules:
		// do nothing
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	// molar volume (see read_phase_vm)
	m_Vm = std::numeric_limits<double>::signaling_NaN();
	m_nDeltaVUnits = cm3_per_mol;
	if (phase_ptr->delta_v[1] == phase_ptr->logk[vm0])
	{
		double factor = 1.0;
		if (dm3_per_mol == phase_ptr->original_deltav_units)
		{
			factor = 1e3;
		}
		else if (m3_per_mol == phase_ptr->original_deltav_units)
		{
			factor = 1e6;
		}
		m_Vm = phase_ptr->logk[vm0] / factor;
		m_nDeltaVUnits = phase_ptr->original_deltav_units;
	}

	// gas: critical TK, critical P(atm), Pitzer acentric coeff
	m_t_c   = phase_ptr->t_c;
	m_p_c   = phase_ptr->p_c;
	m_omega = phase_ptr->omega;
}

CPhase::~CPhase()
{
}

CString CPhase::WriteEqn(const struct phase *phase_ptr)
{
	CString strEqn;
	CString strFormat;

	// formula
	strEqn = (phase_ptr->formula) ? phase_ptr->formula : _T("");

	// left hand side
	int j;
	for (j = 1; phase_ptr->rxn->token[j].s != NULL; ++j)
	{
		if (phase_ptr->rxn->token[j].coef > 0) continue;
		strEqn += _T(" + ");

		if (-phase_ptr->rxn->token[j].coef != 1)
		{
			strFormat.Format("%g%s", -phase_ptr->rxn->token[j].coef, phase_ptr->rxn->token[j].s->name);
		}
		else
		{
			strFormat.Format("%s", phase_ptr->rxn->token[j].s->name);
		}
		strEqn += strFormat;
	}

	// right hand side
	strEqn += _T(" = ");
	bool bFirst = true;

	for (j = 1; phase_ptr->rxn->token[j].s != NULL; ++j)
	{
		if (phase_ptr->rxn->token[j].coef < 0) continue;
		if (!bFirst)
		{
			strEqn += _T(" + ");
		} 
		bFirst = false;
		if (phase_ptr->rxn->token[j].coef != 1)
		{
			strFormat.Format(_T("%g%s"), phase_ptr->rxn->token[j].coef, phase_ptr->rxn->token[j].s->name);
		}
		else
		{
			strFormat.Format(_T("%s"), phase_ptr->rxn->token[j].s->name);
		}
		strEqn += strFormat;
	}
	return strEqn;
}

//////////////////////////////////////////////////////////////////////
// CMaster Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMaster::CMaster()
{
	m_bPrimary = true;
	m_dAlk = std::numeric_limits<double>::signaling_NaN();
	m_dGFW = std::numeric_limits<double>::signaling_NaN();
}

/*
CString strName = master[i]->elt->name;
TRACE("%-13s", strName);
CString strMasterSpecies = master[i]->s->name;
TRACE(" %-13s", strMasterSpecies);
double dAlk = master[i]->alk;
TRACE(" %g", dAlk);

if (master[i]->gfw_formula)
{
	m_strFormula = master[i]->gfw_formula;
}
else
{
	m
}

if (master[i]->gfw_formula)
{
	if (strName.Find(_T('(')) == -1)
	{
		TRACE(" %-13s %g\n", master[i]->gfw_formula, master[i]->elt->gfw);
	}
	else
	{
		TRACE(" %-13s\n", master[i]->gfw_formula);
	}
}
else
{
	if (strName.Find(_T('(')) == -1)
	{
		TRACE(" %g %g\n", master[i]->gfw, master[i]->gfw);
	}
	else
	{
		TRACE(" %g\n", master[i]->gfw);
	}
}
*/

CMaster::CMaster(const struct master *master_ptr)
{
	m_strName          = (master_ptr->elt->name) ? master_ptr->elt->name : _T("");
	m_strMasterSpecies = (master_ptr->s->name) ? master_ptr->s->name : _T("");
	m_dAlk             = master_ptr->alk;
	m_bPrimary         = (master_ptr->primary == TRUE);

	if (m_strName != _T("E"))
	{
		m_dGFW        = master_ptr->gfw;
		m_dGFWElement = master_ptr->elt->gfw;

		// check for gfw formula
		if (master_ptr->gfw_formula)
		{
			m_strFormula = master_ptr->gfw_formula;
		}
		else
		{
			m_strFormula = _T("");
		}
	}
	else
	{
		ASSERT(master_ptr->gfw == 0.0);
		ASSERT(master_ptr->elt->gfw == 0.0);
		m_dGFW = 0.0;
		m_dGFWElement = 0.0;
		m_strFormula = _T("");
	}
}

CMaster::~CMaster()
{
}

//////////////////////////////////////////////////////////////////////
// CTransport Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTransport::CTransport(PhreeqcI* phreeqci)
{
	if (phreeqci)
	{
		phreeqci->Update(this);
	}
}

CTransport::~CTransport()
{
}
