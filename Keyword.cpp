// Keyword.cpp: implementation of the CKeyword class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "Keyword.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKeyword::CKeyword()
{
}

CKeyword::~CKeyword()
{

}

enum CKeyword::type CKeyword::GetKeywordType(LPCTSTR lpsz)
{
	CString strLower(lpsz);
	strLower.MakeLower();

	std::map<CString, CKeyword::type>::const_iterator item = GetKeywordMap().find(strLower);

	if (item != GetKeywordMap().end())
	{
		switch ((*item).second)
		{
		case CKeyword::K_CELL:
			break;
		default:
			return (*item).second;
		}
	}

	return CKeyword::K_NOT_KEYWORD;
}

enum CKeyword::type CKeyword::GetKeywordType2(LPCTSTR lpsz)
{
	CString strLower(lpsz);
	strLower.MakeLower();

	std::map<CString, CKeyword::type>::const_iterator item = GetKeywordMap().find(strLower);

	if (item != GetKeywordMap().end())
	{
		return (*item).second;
	}

	return CKeyword::K_NOT_KEYWORD;
}

bool CKeyword::IsKeyword(LPCTSTR lpsz)
{
	return (GetKeywordType(lpsz) != K_NOT_KEYWORD);
}

bool CKeyword::IsKeyword2(LPCTSTR lpsz)
{
	return (GetKeywordType2(lpsz) != K_NOT_KEYWORD);
}

CString CKeyword::GetString(enum CKeyword::type nType)
{
	// excludes "cell"
	std::map<CKeyword::type, LPCTSTR>::const_iterator item = GetInverseKeywordMap().find(nType);

	if (item != GetInverseKeywordMap().end())
	{
		switch ((*item).first)
		{
		case CKeyword::K_CELL:
			break;
		default:
			return (*item).second;
		}
	}

	return CString("NOT_KEYWORD");
}

CString CKeyword::GetString2(enum CKeyword::type nType)
{
	// includes "cell"
	std::map<CKeyword::type, LPCTSTR>::const_iterator item = GetInverseKeywordMap().find(nType);

	if (item != GetInverseKeywordMap().end())
	{
		return (*item).second;
	}

	return CString("NOT_KEYWORD");
}

CString CKeyword::GetHelpIndex(enum CKeyword::type nType)
{
	std::map<CKeyword::type, LPCTSTR>::const_iterator item = GetKey2HelpMap().find(nType);

	if (item != GetKey2HelpMap().end())
	{
		return (*item).second;
	}

	return CString("");
}

const CKeyword::CKeywordMap& CKeyword::GetKeywordMap()
{
	static CKeyword::CKeywordMap s_map;
	return s_map;
}

const CKeyword::XInverseKeywordMap& CKeyword::GetInverseKeywordMap()
{
	static CKeyword::XInverseKeywordMap s_map;
	return s_map;
}

const CKeyword::CKey2HelpMap& CKeyword::GetKey2HelpMap()
{
	static CKeyword::CKey2HelpMap s_map;
	return s_map;
}

//////////////////////////////////////////////////////////////////////
// CKeywordMap Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKeyword::CKeywordMap::CKeywordMap()
{
	// taken from global.h
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("eof"),                           CKeyword::K_EOF));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("end"),                           CKeyword::K_END));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("solution_species"),              CKeyword::K_SOLUTION_SPECIES));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("solution_master_species"),       CKeyword::K_SOLUTION_MASTER_SPECIES));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("solution"),                      CKeyword::K_SOLUTION));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("phases"),                        CKeyword::K_PHASES));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("pure_phases"),                   CKeyword::K_EQUILIBRIUM_PHASES));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("reaction"),                      CKeyword::K_REACTION));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("mix"),                           CKeyword::K_MIX));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("use"),                           CKeyword::K_USE));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("save"),                          CKeyword::K_SAVE));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("exchange_species"),              CKeyword::K_EXCHANGE_SPECIES));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("exchange_master_species"),       CKeyword::K_EXCHANGE_MASTER_SPECIES));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("exchange"),                      CKeyword::K_EXCHANGE));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("surface_species"),               CKeyword::K_SURFACE_SPECIES));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("surface_master_species"),        CKeyword::K_SURFACE_MASTER_SPECIES));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("surface"),                       CKeyword::K_SURFACE));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("reaction_temperature"),          CKeyword::K_REACTION_TEMPERATURE));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("inverse_modeling"),              CKeyword::K_INVERSE_MODELING));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("gas_phase"),                     CKeyword::K_GAS_PHASE));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("transport"),                     CKeyword::K_TRANSPORT));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("debug"),                         CKeyword::K_KNOBS));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("selected_output"),               CKeyword::K_SELECTED_OUTPUT));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("select_output"),                 CKeyword::K_SELECTED_OUTPUT));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("knobs"),                         CKeyword::K_KNOBS));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("print"),                         CKeyword::K_PRINT));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("equilibrium_phases"),            CKeyword::K_EQUILIBRIUM_PHASES));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("equilibria"),                    CKeyword::K_EQUILIBRIUM_PHASES));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("equilibrium"),                   CKeyword::K_EQUILIBRIUM_PHASES));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("pure"),                          CKeyword::K_EQUILIBRIUM_PHASES));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("equilibrium_phase"),             CKeyword::K_EQUILIBRIUM_PHASES));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("title"),                         CKeyword::K_TITLE));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("comment"),                       CKeyword::K_TITLE));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("advection"),                     CKeyword::K_ADVECTION));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("kinetics"),                      CKeyword::K_KINETICS));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("incremental_reactions"),         CKeyword::K_INCREMENTAL_REACTIONS));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("incremental"),                   CKeyword::K_INCREMENTAL_REACTIONS));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("rates"),                         CKeyword::K_RATES));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("solution_s"),                    CKeyword::K_SOLUTION_SPREAD));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("user_print"),                    CKeyword::K_USER_PRINT));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("user_punch"),                    CKeyword::K_USER_PUNCH));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("solid_solutions"),               CKeyword::K_SOLID_SOLUTIONS));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("solid_solution"),                CKeyword::K_SOLID_SOLUTIONS));	
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("solution_spread"),               CKeyword::K_SOLUTION_SPREAD));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("spread_solution"),               CKeyword::K_SOLUTION_SPREAD));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("selected_out"),                  CKeyword::K_SELECTED_OUTPUT));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("select_out"),                    CKeyword::K_SELECTED_OUTPUT));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("user_graph"),                    CKeyword::K_USER_GRAPH));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("llnl_aqueous_model_parameters"), CKeyword::K_LLNL_AQUEOUS_MODEL_PARAMETERS));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("llnl_aqueous_model"),            CKeyword::K_LLNL_AQUEOUS_MODEL_PARAMETERS));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("database"),                      CKeyword::K_DATABASE));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("named_analytical_expression"),   CKeyword::K_NAMED_EXPRESSIONS));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("named_analytical_expressions"),  CKeyword::K_NAMED_EXPRESSIONS));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("named_expressions"),             CKeyword::K_NAMED_EXPRESSIONS));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("named_log_k"),                   CKeyword::K_NAMED_EXPRESSIONS));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("isotopes"),                      CKeyword::K_ISOTOPES));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("calculate_values"),              CKeyword::K_CALCULATE_VALUES));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("isotope_ratios"),                CKeyword::K_ISOTOPE_RATIOS));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("isotope_alphas"),                CKeyword::K_ISOTOPE_ALPHAS));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("copy"),                          CKeyword::K_COPY));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("pitzer"),                        CKeyword::K_PITZER));
	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("sit"),                           CKeyword::K_SIT));

	//{{NEW 3.0
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("solution_raw"),                  CKeyword::K_SOLUTION_RAW));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("exchange_raw"),                  CKeyword::K_EXCHANGE_RAW));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("surface_raw"),                   CKeyword::K_SURFACE_RAW));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("equilibrium_phases_raw"),        CKeyword::K_EQUILIBRIUM_PHASES_RAW));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("kinetics_raw"),                  CKeyword::K_KINETICS_RAW));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("solid_solutions_raw"),           CKeyword::K_SOLID_SOLUTIONS_RAW));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("gas_phase_raw"),                 CKeyword::K_GAS_PHASE_RAW));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("reaction_raw"),                  CKeyword::K_REACTION_RAW));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("mix_raw"),                       CKeyword::K_MIX_RAW));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("reaction_temperature_raw"),      CKeyword::K_REACTION_TEMPERATURE_RAW));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("dump"),                          CKeyword::K_DUMP));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("solution_modify"),               CKeyword::K_SOLUTION_MODIFY));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("equilibrium_phases_modify"),     CKeyword::K_EQUILIBRIUM_PHASES_MODIFY));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("exchange_modify"),               CKeyword::K_EXCHANGE_MODIFY));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("surface_modify"),                CKeyword::K_SURFACE_MODIFY));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("solid_solutions_modify"),        CKeyword::K_SOLID_SOLUTIONS_MODIFY));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("gas_phase_modify"),              CKeyword::K_GAS_PHASE_MODIFY));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("kinetics_modify"),               CKeyword::K_KINETICS_MODIFY));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("delete"),                        CKeyword::K_DELETE));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("run_cells"),                     CKeyword::K_RUN_CELLS));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("reaction_modify"),               CKeyword::K_REACTION_MODIFY));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("reaction_temperature_modify"),   CKeyword::K_REACTION_TEMPERATURE_MODIFY));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("solid_solution_modify"),         CKeyword::K_SOLID_SOLUTIONS_MODIFY));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("reaction_pressure"),             CKeyword::K_REACTION_PRESSURE));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("reaction_pressures"),            CKeyword::K_REACTION_PRESSURE));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("reaction_pressure_raw"),         CKeyword::K_REACTION_PRESSURE_RAW));
    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("reaction_pressure_modify"),      CKeyword::K_REACTION_PRESSURE_MODIFY));
	//}}NEW 3.0

	(*this).insert(std::map<CString, CKeyword::type>::value_type(_T("cell"),                          CKeyword::K_CELL));

    (*this).insert(std::map<CString, CKeyword::type>::value_type(_T("include$"),                      CKeyword::K_INCLUDE));
}

CKeyword::CKeywordMap::~CKeywordMap()
{

}

//////////////////////////////////////////////////////////////////////
// CInverseKeywordMap Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKeyword::XInverseKeywordMap::XInverseKeywordMap()
{
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_ADVECTION,                     _T("ADVECTION")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_END,                           _T("END")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_EQUILIBRIUM_PHASES,            _T("EQUILIBRIUM_PHASES")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_EXCHANGE,                      _T("EXCHANGE")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_EXCHANGE_MASTER_SPECIES,       _T("EXCHANGE_MASTER_SPECIES")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_EXCHANGE_SPECIES,              _T("EXCHANGE_SPECIES")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_GAS_PHASE,                     _T("GAS_PHASE")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_INCREMENTAL_REACTIONS,         _T("INCREMENTAL_REACTIONS")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_INVERSE_MODELING,              _T("INVERSE_MODELING")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_KINETICS,                      _T("KINETICS")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_KNOBS,                         _T("KNOBS")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_MIX,                           _T("MIX")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_PHASES,                        _T("PHASES")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_PRINT,                         _T("PRINT")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_RATES,                         _T("RATES")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_REACTION,                      _T("REACTION")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_REACTION_TEMPERATURE,          _T("REACTION_TEMPERATURE")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SAVE,                          _T("SAVE")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SELECTED_OUTPUT,               _T("SELECTED_OUTPUT")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SOLID_SOLUTIONS,               _T("SOLID_SOLUTIONS")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SOLUTION,                      _T("SOLUTION")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SOLUTION_MASTER_SPECIES,       _T("SOLUTION_MASTER_SPECIES")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SOLUTION_SPECIES,              _T("SOLUTION_SPECIES")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SOLUTION_SPREAD,               _T("SOLUTION_SPREAD")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SURFACE,                       _T("SURFACE")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SURFACE_MASTER_SPECIES,        _T("SURFACE_MASTER_SPECIES")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SURFACE_SPECIES,               _T("SURFACE_SPECIES")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_TITLE,                         _T("TITLE")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_TRANSPORT,                     _T("TRANSPORT")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_USE,                           _T("USE")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_USER_GRAPH,                    _T("USER_GRAPH")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_USER_PRINT,                    _T("USER_PRINT")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_USER_PUNCH,                    _T("USER_PUNCH")));

	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_LLNL_AQUEOUS_MODEL_PARAMETERS, _T("LLNL_AQUEOUS_MODEL_PARAMETERS")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_NAMED_EXPRESSIONS,             _T("NAMED_EXPRESSIONS")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_ISOTOPES,                      _T("ISOTOPES")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_CALCULATE_VALUES,              _T("CALCULATE_VALUES")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_ISOTOPE_RATIOS,                _T("ISOTOPE_RATIOS")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_ISOTOPE_ALPHAS,                _T("ISOTOPE_ALPHAS")));

	//{{NEW 3.0
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_COPY,                          _T("COPY")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_PITZER,                        _T("PITZER")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SIT,                           _T("SIT")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SOLUTION_RAW,                  _T("SOLUTION_RAW")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_EXCHANGE_RAW,                  _T("EXCHANGE_RAW")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SURFACE_RAW,                   _T("SURFACE_RAW")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_EQUILIBRIUM_PHASES_RAW,        _T("EQUILIBRIUM_PHASES_RAW")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_KINETICS_RAW,                  _T("KINETICS_RAW")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SOLID_SOLUTIONS_RAW,           _T("SOLID_SOLUTIONS_RAW")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_GAS_PHASE_RAW,                 _T("GAS_PHASE_RAW")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_REACTION_RAW,                  _T("REACTION_RAW")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_MIX_RAW,                       _T("MIX_RAW")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_REACTION_TEMPERATURE_RAW,      _T("REACTION_TEMPERATURE_RAW")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_DUMP,                          _T("DUMP")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SOLUTION_MODIFY,               _T("SOLUTION_MODIFY")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_EQUILIBRIUM_PHASES_MODIFY,     _T("EQUILIBRIUM_PHASES_MODIFY")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_EXCHANGE_MODIFY,               _T("EXCHANGE_MODIFY")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SURFACE_MODIFY,                _T("SURFACE_MODIFY")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SOLID_SOLUTIONS_MODIFY,        _T("SOLID_SOLUTIONS_MODIFY")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_GAS_PHASE_MODIFY,              _T("GAS_PHASE_MODIFY")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_KINETICS_MODIFY,               _T("KINETICS_MODIFY")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_DELETE,                        _T("DELETE")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_RUN_CELLS,                     _T("RUN_CELLS")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_REACTION_MODIFY,               _T("REACTION_MODIFY")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_REACTION_TEMPERATURE_MODIFY,   _T("REACTION_TEMPERATURE_MODIFY")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_REACTION_PRESSURE,             _T("REACTION_PRESSURE")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_REACTION_PRESSURE_RAW,         _T("REACTION_PRESSURE_RAW")));
    (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_REACTION_PRESSURE_MODIFY,      _T("REACTION_PRESSURE_MODIFY")));   
	//}}NEW 3.0

	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_CELL,                          _T("CELL")));

	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_INCLUDE,                       _T("INCLUDE$")));
}

CKeyword::XInverseKeywordMap::~XInverseKeywordMap()
{
}

//////////////////////////////////////////////////////////////////////
// CKey2HelpMap Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKeyword::CKey2HelpMap::CKey2HelpMap()
{
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_ADVECTION,                     _T("/html/phreeqc3-6.htm#50651137_87438")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_CALCULATE_VALUES,              _T("/html/phreeqc3-7.htm#50651137_35035")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_COPY,                          _T("/html/phreeqc3-8.htm#50651137_76644")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_DATABASE,                      _T("/html/phreeqc3-9.htm#50651137_88316")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_DELETE,                        _T("/html/phreeqc3-10.htm#50651137_33574")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_DUMP,                          _T("/html/phreeqc3-11.htm#50651137_49635")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_END,                           _T("/html/phreeqc3-12.htm#50651137_63178")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_EQUILIBRIUM_PHASES,            _T("/html/phreeqc3-13.htm#50651137_61207")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_EXCHANGE,                      _T("/html/phreeqc3-14.htm#50651137_84573")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_EXCHANGE_MASTER_SPECIES,       _T("/html/phreeqc3-15.htm#50651137_33835")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_EXCHANGE_SPECIES,              _T("/html/phreeqc3-16.htm#50651137_65476")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_GAS_PHASE,                     _T("/html/phreeqc3-17.htm#50651137_83409")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_INCLUDE,                       _T("/html/phreeqc3-18.htm#50651137_74991")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_INCREMENTAL_REACTIONS,         _T("/html/phreeqc3-19.htm#50651137_79204")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_INVERSE_MODELING,              _T("/html/phreeqc3-20.htm#50651137_11066")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_ISOTOPES,                      _T("/html/phreeqc3-21.htm#50651137_32306")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_ISOTOPE_ALPHAS,                _T("/html/phreeqc3-22.htm#50651137_17508")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_ISOTOPE_RATIOS,                _T("/html/phreeqc3-23.htm#50651137_17097")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_KINETICS,                      _T("/html/phreeqc3-24.htm#50651137_55637")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_KNOBS,                         _T("/html/phreeqc3-25.htm#50651137_79000")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_LLNL_AQUEOUS_MODEL_PARAMETERS, _T("/html/phreeqc3-26.htm#50651137_10503")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_MIX,                           _T("/html/phreeqc3-27.htm#50651137_23725")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_NAMED_EXPRESSIONS,             _T("/html/phreeqc3-28.htm#50651137_79962")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_PHASES,                        _T("/html/phreeqc3-29.htm#50651137_84418")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_PITZER,                        _T("/html/phreeqc3-30.htm#50651137_13650")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_PRINT,                         _T("/html/phreeqc3-31.htm#50651137_92102")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_RATES,                         _T("/html/phreeqc3-32.htm#50651137_97907")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_REACTION,                      _T("/html/phreeqc3-33.htm#50651137_75635")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_REACTION_PRESSURE,             _T("/html/phreeqc3-34.htm#50651137_65966")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_REACTION_TEMPERATURE,          _T("/html/phreeqc3-35.htm#50651137_75016")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_RUN_CELLS,                     _T("/html/phreeqc3-36.htm#50651137_78104")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SAVE,                          _T("/html/phreeqc3-37.htm#50651137_81857")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SELECTED_OUTPUT,               _T("/html/phreeqc3-38.htm#50651137_20239")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SIT,                           _T("/html/phreeqc3-39.htm#50651137_88782")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SOLID_SOLUTIONS,               _T("/html/phreeqc3-40.htm#50651137_77444")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SOLUTION,                      _T("/html/phreeqc3-41.htm#50651137_30253")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SOLUTION_MASTER_SPECIES,       _T("/html/phreeqc3-42.htm#50651137_19910")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SOLUTION_SPECIES,              _T("/html/phreeqc3-43.htm#50651137_96148")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SOLUTION_SPREAD,               _T("/html/phreeqc3-44.htm#50651137_84297")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SURFACE,                       _T("/html/phreeqc3-45.htm#50651137_56392")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SURFACE_MASTER_SPECIES,        _T("/html/phreeqc3-46.htm#50651137_58106")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SURFACE_SPECIES,               _T("/html/phreeqc3-47.htm#50651137_92844")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_TITLE,                         _T("/html/phreeqc3-48.htm#50651137_48632")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_TRANSPORT,                     _T("/html/phreeqc3-49.htm#50651137_87317")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_USE,                           _T("/html/phreeqc3-50.htm#50651137_78143")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_USER_GRAPH,                    _T("/html/phreeqc3-51.htm#50651137_26121")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_USER_PRINT,                    _T("/html/phreeqc3-52.htm#50651137_55085")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_USER_PUNCH,                    _T("/html/phreeqc3-53.htm#50651137_56415")));
}

CKeyword::CKey2HelpMap::~CKey2HelpMap()
{
}
