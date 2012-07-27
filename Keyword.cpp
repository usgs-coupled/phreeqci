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
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_ADVECTION,               _T("/phreeqc_html/final-36.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_END,                     _T("/phreeqc_html/final-37.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_EQUILIBRIUM_PHASES,      _T("/phreeqc_html/final-38.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_EXCHANGE,                _T("/phreeqc_html/final-39.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_EXCHANGE_MASTER_SPECIES, _T("/phreeqc_html/final-40.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_EXCHANGE_SPECIES,        _T("/phreeqc_html/final-41.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_GAS_PHASE,               _T("/phreeqc_html/final-42.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_INCREMENTAL_REACTIONS,   _T("/phreeqc_html/final-43.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_INVERSE_MODELING,        _T("/phreeqc_html/final-44.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_KINETICS,                _T("/phreeqc_html/final-45.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_KNOBS,                   _T("/phreeqc_html/final-46.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_MIX,                     _T("/phreeqc_html/final-47.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_PHASES,                  _T("/phreeqc_html/final-48.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_PRINT,                   _T("/phreeqc_html/final-49.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_RATES,                   _T("/phreeqc_html/final-50.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_REACTION,                _T("/phreeqc_html/final-51.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_REACTION_TEMPERATURE,    _T("/phreeqc_html/final-52.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SAVE,                    _T("/phreeqc_html/final-53.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SELECTED_OUTPUT,         _T("/phreeqc_html/final-54.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SOLID_SOLUTIONS,         _T("/phreeqc_html/final-55.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SOLUTION,                _T("/phreeqc_html/final-56.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SOLUTION_MASTER_SPECIES, _T("/phreeqc_html/final-57.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SOLUTION_SPECIES,        _T("/phreeqc_html/final-58.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SOLUTION_SPREAD,         _T("/phreeqc_html/final-59.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SURFACE,                 _T("/phreeqc_html/final-60.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SURFACE_MASTER_SPECIES,  _T("/phreeqc_html/final-61.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_SURFACE_SPECIES,         _T("/phreeqc_html/final-62.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_TITLE,                   _T("/phreeqc_html/final-63.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_TRANSPORT,               _T("/phreeqc_html/final-64.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_USE,                     _T("/phreeqc_html/final-65.html")));
	// (*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_USER_GRAPH,              _T("/phreeqc_html/final-XX.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_USER_PRINT,              _T("/phreeqc_html/final-66.html")));
	(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_USER_PUNCH,              _T("/phreeqc_html/final-67.html")));

	// TODO phreeqc3
	//(*this).insert(std::map<CKeyword::type, LPCTSTR>::value_type(CKeyword::K_REACTION_PRESSURE,       _T("TODO")));

}

CKeyword::CKey2HelpMap::~CKey2HelpMap()
{
}
