// Keyword.h: interface for the CKeyword class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYWORD_H__2268A8B0_82CF_4948_8E8F_F9663DE17E2D__INCLUDED_)
#define AFX_KEYWORD_H__2268A8B0_82CF_4948_8E8F_F9663DE17E2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKeyword  
{
public:

	enum type {
		K_NOT_KEYWORD                   = -1,
		//      taken from global.h
		K_EOF                           =  0,
		K_END                           =  1,
		K_SOLUTION_SPECIES              =  2,
		K_SOLUTION_MASTER_SPECIES       =  3,
		K_SOLUTION                      =  4,
		K_PHASES                        =  5,
		K_REACTION                      =  7,
		K_MIX                           =  8,
		K_USE                           =  9,
		K_SAVE                          = 10,
		K_EXCHANGE_SPECIES              = 11,
		K_EXCHANGE_MASTER_SPECIES       = 12,
		K_EXCHANGE                      = 13,
		K_SURFACE_SPECIES               = 14,
		K_SURFACE_MASTER_SPECIES        = 15,
		K_SURFACE                       = 16,
		K_REACTION_TEMPERATURE          = 17,
		K_INVERSE_MODELING              = 18,
		K_GAS_PHASE                     = 19,
		K_TRANSPORT                     = 20,
		K_SELECTED_OUTPUT               = 22,
		K_KNOBS                         = 24,
		K_PRINT                         = 25,
		K_EQUILIBRIUM_PHASES            = 26,
		K_TITLE                         = 30,
		//      New keywords added by version 2
		K_ADVECTION                     = 32,
		K_KINETICS                      = 33,
		K_INCREMENTAL_REACTIONS         = 34,
		K_RATES                         = 36,
		K_USER_PRINT                    = 38,
		K_USER_PUNCH                    = 39,
		K_SOLID_SOLUTIONS               = 40,
		K_SOLUTION_SPREAD               = 42,
		K_USER_GRAPH                    = 46,
		K_LLNL_AQUEOUS_MODEL_PARAMETERS = 47,
		K_DATABASE                      = 49,
		//      isotope keywords
		K_NAMED_ANALYTICAL_EXPRESSION   = 50,
		K_ISOTOPES                      = 54,
		K_CALCULATE_VALUES              = 55,
		K_ISOTOPE_RATIOS                = 56,
		K_ISOTOPE_ALPHAS                = 57,
		// new to 2.9
		K_COPY                          = 58,
	};
	// synonyms
	// K_PURE_PHASES             =  6, K_EQUILIBRIUM_PHASES
	// K_DEBUG                   = 21, K_KNOBS
	// K_SELECT_OUTPUT           = 23, K_SELECTED_OUTPUT
	// K_EQUILIBRIA              = 27, K_EQUILIBRIUM_PHASES
	// K_EQUILIBRIUM             = 28, K_EQUILIBRIUM_PHASES
	// K_PURE                    = 29, K_EQUILIBRIUM_PHASES
	// K_COMMENT                 = 31, K_TITLE
	//      New keywords added by version 2
	// K_INCREMENTAL             = 35, K_INCREMENTAL_REACTIONS
	// K_SOLUTION_S              = 37, K_SOLUTION_SPREAD
	// K_SOLID_SOLUTION          = 41, K_SOLID_SOLUTIONS
	// K_SPREAD_SOLUTION         = 43, K_SOLUTION_SPREAD
	// K_SELECTED_OUT            = 44, K_SELECTED_OUTPUT
	// K_SELECT_OUT              = 45, K_SELECTED_OUTPUT

// inner class
private:
	// Note the key(the first member) should have operator< defined.  Since LPCTSTR is
	// not a class type LPCTSTR cannot be used since it is not a class and therefore
	// cannot define an operator<.
	class CKeywordMap : public std::map<CString, type>
	{
	public:
		CKeywordMap();
		virtual ~CKeywordMap();

	};

	class XInverseKeywordMap : public std::map<type, LPCTSTR>
	{
	public:
		XInverseKeywordMap();
		virtual ~XInverseKeywordMap();

	};

	class CKey2HelpMap : public std::map<type, LPCTSTR>
	{
	public:
		CKey2HelpMap();
		virtual ~CKey2HelpMap();
	};


	static const CKeyword::CKeywordMap&        GetKeywordMap();
	static const CKeyword::XInverseKeywordMap& GetInverseKeywordMap();
	static const CKeyword::CKey2HelpMap&       GetKey2HelpMap();
public:
	static bool    IsKeyword(LPCTSTR lpsz);
	static type    GetKeywordType(LPCTSTR lpsz);
	static CString GetString(CKeyword::type nType);
	static CString GetHelpIndex(CKeyword::type nType);
	CKeyword();
	virtual ~CKeyword();
};



#endif // !defined(AFX_KEYWORD_H__2268A8B0_82CF_4948_8E8F_F9663DE17E2D__INCLUDED_)
