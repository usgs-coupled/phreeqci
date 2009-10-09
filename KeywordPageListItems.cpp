// KeywordPageListItems.cpp: implementation of the CPurePhase class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "KeywordPageListItems.h"

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
{
}

CPurePhase::CPurePhase(const struct pure_phase *pure_phase_ptr)
: m_strName(pure_phase_ptr->name)
, m_dSI(pure_phase_ptr->si)
, m_dAmount(pure_phase_ptr->moles)
, m_strAlt(pure_phase_ptr->add_formula)
, m_bDissolveOnly(pure_phase_ptr->dissolve_only != 0)
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

CGasComp::CGasComp(const struct gas_comp* gas_comp_ptr)
: m_strName(gas_comp_ptr->name)
, m_dP_Read(gas_comp_ptr->p_read == NAN ? std::numeric_limits<double>::signaling_NaN() : gas_comp_ptr->p_read)
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

CExchComp::CExchComp(const struct exch_comp* exch_comp_ptr)
: m_dPhase_proportion(exch_comp_ptr->phase_proportion)
, m_ldMoles(exch_comp_ptr->moles)
{
	m_strFormula        = (exch_comp_ptr->formula    == NULL) ? _T("") : exch_comp_ptr->formula;
	m_strPhase_name     = (exch_comp_ptr->phase_name == NULL) ? _T("") : exch_comp_ptr->phase_name;
	m_strRate_name      = (exch_comp_ptr->rate_name  == NULL) ? _T("") : exch_comp_ptr->rate_name;
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
	_ASSERTE( std::numeric_limits<double>::has_signaling_NaN );

	m_dCoef = (name_coef_ptr->coef == NAN) ?
		std::numeric_limits<double>::signaling_NaN() : name_coef_ptr->coef;
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

CS_S_Comp::CS_S_Comp(const struct s_s_comp* s_s_comp_ptr)
{
	m_strName = s_s_comp_ptr->name;
	m_ldMoles = s_s_comp_ptr->moles;
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
}

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

CKineticComp::CKineticComp(const struct kinetics_comp *kinetics_comp_ptr)
{
	m_strRateName = kinetics_comp_ptr->rate_name == NULL ? _T("") : kinetics_comp_ptr->rate_name;
	if (m_strRateName.IsEmpty())
	{
		m_strRateName = _T("NoName");
	}
	
	for (int i = 0; i < kinetics_comp_ptr->count_list; ++i)
	{
		CNameCoef nameCoef(&kinetics_comp_ptr->list[i]);
		m_listNameCoef.push_back(nameCoef);
	}

	m_dTol = kinetics_comp_ptr->tol;
	m_dM   = kinetics_comp_ptr->m;
	m_dM0  = kinetics_comp_ptr->m0;

	for (int j = 0; j < kinetics_comp_ptr->count_d_params; ++j)
	{
		double d(kinetics_comp_ptr->d_params[j]);
		m_listDParams.push_back(d);
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
	m_bCheckEqn    = true;
	m_nDeltaHUnits = kjoules;
	m_nActType     = AT_DAVIES;

	m_dLogK        = std::numeric_limits<double>::signaling_NaN();
	m_dDeltaH      = std::numeric_limits<double>::signaling_NaN();

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
	m_nActType = AT_UNKNOWN;
	m_dDHa = species_ptr->dha;
	m_dDHb = species_ptr->dhb;
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

	case 4: // exchange; fall through
	case 6: // surface; fall through
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
	ASSERT(m_nActType != AT_UNKNOWN);
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

	//{{
	if (m_strName != _T("E"))
	{
		if (m_bPrimary)
		{
			m_dGFW = master_ptr->elt->gfw;
		}
		else
		{
			m_dGFW = master_ptr->gfw;
		}

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
		m_dGFW = 0.0;
		m_strFormula = _T("");
	}
}

CMaster::~CMaster()
{
}

