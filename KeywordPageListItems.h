// KeywordPageListItems.h: interface for the CPurePhase class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYWORDPAGELISTITEMS_H__13CB5247_EF41_48EA_86AA_A788A4219F30__INCLUDED_)
#define AFX_KEYWORDPAGELISTITEMS_H__13CB5247_EF41_48EA_86AA_A788A4219F30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DefinedRanges.h"

// COMMENT: {2/14/2012 6:45:13 PM}extern "C"
// COMMENT: {2/14/2012 6:45:13 PM}{
// COMMENT: {2/14/2012 6:45:13 PM}#define EXTERNAL extern
// COMMENT: {2/14/2012 6:45:13 PM}#include "phreeqc/src/global.h"
// COMMENT: {2/14/2012 6:45:13 PM}}

#include "global_structures.h" // DELTA_H_UNIT

class cxxExchComp;
class cxxPPassemblageComp;
class cxxGasComp;
class cxxSolution;
class cxxSolutionIsotope;
class cxxSS;
class cxxSScomp;
class cxxKineticsComp;

class CPurePhase  
{
public:
	CPurePhase();
#if 0
	CPurePhase(const struct pure_phase *pure_phase_ptr);
#else
	CPurePhase(const cxxPPassemblageComp *ppComp);
#endif
	virtual ~CPurePhase();
public:
	CString m_strName;
	double m_dSI;
	double m_dAmount;
	CString m_strAlt;
	bool m_bDissolveOnly;
	bool m_bPrecipOnly;
	bool m_bForceEquality;
};


class CGasComp  
{
public:
	CGasComp();
#if 0
	CGasComp(const struct gas_comp* gas_comp_ptr);
#else
	CGasComp(const cxxGasComp* gasComp);
#endif
	virtual ~CGasComp();
public:
	double m_dP_Read;
	CString m_strName;
};

class CExchComp
{
public:
	CExchComp();
#if 0
	CExchComp(const struct exch_comp* exch_comp_ptr);
#else
	CExchComp(const cxxExchComp* exchComp);
#endif
	virtual ~CExchComp();
public:
	double m_dPhase_proportion;
	long double m_ldMoles;
	CString m_strFormula;
	CString m_strPhase_name;
	CString m_strRate_name;
};

class CNameCoef
{
public:
	CNameCoef();
	CNameCoef(const struct name_coef* name_coef_ptr);
	CNameCoef(cxxNameDouble::const_iterator ci);
	// use implicit copy ctor
	~CNameCoef();
public:
	CString m_strName;
	double m_dCoef;
};

class CConc
{
public:
	CString m_strDesc;
	double m_dInputConc;
	CString m_strUnits;
	CString m_strAs;
	double m_dGFW;
	CString m_strGFW;
	CString m_strRedox;
	CString m_strPhase;
	double m_dPhaseSI;
	CString m_strPhaseSI;
public:
	static CString GetUnits(CString& rStr);
	static CString GetAs(CString& rStr);
	static CString GetGFW(CString& rStr);
	static CString GetRedox(CString& rStr);
	static void GetPhase(CString &rStr, CConc& rConc);

	static CConc Create(LPCTSTR psz);

	CConc();
#if 0
	CConc(const struct solution* solution_ptr, const struct conc* conc_ptr);
#else
	CConc(const cxxSolution* soln, const cxxISolutionComp* comp);
#endif
	virtual ~CConc();
	CString GetSubHeading()const;
};

class CIsotope
{
public:
	double m_dIsotopeNumber;
	CString m_strEltName;
	CString m_strName;
	double m_dRatio;
	double m_dRatioUncertainty;
public:
	CIsotope();
	CIsotope(const struct isotope* isotope_ptr);
	CIsotope(const struct iso* iso_ptr);
	CIsotope(const cxxSolutionIsotope* iso);
};

struct InvSol
{
	int nSol;
	double dUncertainty;
	bool bForce;
};

class CInvIsotope : public CIsotope
{
public:
	std::map<int, double> m_mapSol2Unc;
public:
	CInvIsotope();
	CString GetString(std::list<InvSol>& rlistInvSol, InvSol& rFinalInvSol, std::map<CString, double>& rDefaults);
	CInvIsotope(const struct isotope* isotope_ptr);
	CInvIsotope(const struct inv_isotope* inv_isotope_ptr);
};

class CS_S_Comp
{
public:
	long double m_ldMoles;
	CString m_strName;
public:	
	CS_S_Comp();
#if 0
	CS_S_Comp(const struct s_s_comp* s_s_comp_ptr);
#else
	CS_S_Comp(const cxxSScomp* comp);
#endif
	virtual ~CS_S_Comp();
};

class CS_S
{
public:
	/***
		SS_PARM_NONE = -1,
		SS_PARM_A0_A1 = 0,
		SS_PARM_GAMMAS = 1,
		SS_PARM_DIST_COEF = 2,
		SS_PARM_MISCIBILITY = 3,
		SS_PARM_SPINODAL = 4,
		SS_PARM_CRITICAL = 5,
		SS_PARM_ALYOTROPIC = 6,
		SS_PARM_DIM_GUGG = 7,
		SS_PARM_WALDBAUM = 8,
		SS_PARM_MARGULES = 9
	***/
	enum InputCase
	{
		IC_GUGG_NONDIMENSIONAL       = 0,  // cxxSS::SS_PARM_A0_A1
		IC_ACTIVITY_COEFFICIENTS     = 1,  // cxxSS::SS_PARM_GAMMAS
		IC_DISTRIBUTION_COEFFICIENTS = 2,  // cxxSS::SS_PARM_DIST_COEF
		IC_MISCIBILITY_GAP           = 3,  // cxxSS::SS_PARM_MISCIBILITY
		IC_SPINODAL_GAP              = 4,  // cxxSS::SS_PARM_SPINODAL
		IC_CRITICAL_POINT            = 5,  // cxxSS::SS_PARM_CRITICAL
		IC_ALYOTROPIC_POINT          = 6,  // cxxSS::SS_PARM_ALYOTROPIC
		IC_GUGG_KJ                   = 7,  // cxxSS::SS_PARM_DIM_GUGG
		IC_THOMPSON                  = 8,  // cxxSS::SS_PARM_WALDBAUM
		IC_MARGULES                  = 9,  // cxxSS::SS_PARM_MARGULES
	} m_nInputCase;

	std::list<CS_S_Comp> m_listComp;
	CString m_strName;
	long double m_arrldP[4];
	double m_dTk;

public:	
	CS_S();
#if 0
	CS_S(const struct s_s* s_s_ptr);
#else
	CS_S(const cxxSS* ss);
#endif
	virtual ~CS_S();
};

class basic_command
{
public:
	long nLine;
	CString strCommand;
};

class CRate  
{
public:
	CRate();
	CRate(const struct rate *rate_ptr);
	CString GetString();
	virtual ~CRate();
public:
	CString m_strName;
	std::list<basic_command> m_listCommands;
};


class CInvPhase
{
public:
	CInvPhase();
	CInvPhase(const struct inv_phases *inv_phases_ptr);
	CString GetString();
	CString GetString(int nWidth);
	virtual ~CInvPhase();
public:
	CString m_strName;
	enum Constraint
	{
		IPC_EITHER,
		IPC_PRECIPITATE,
		IPC_DISSOLVE,
	} m_nConstraint;
	bool m_bForce;
	std::map<CString, CIsotope> m_mapIsotopes;
};

class CInvElem
{
public:
	CInvElem();
	CString GetString(std::list<InvSol>& rlistInvSol, InvSol& rFinalInvSol);
	virtual ~CInvElem();
public:
	CString m_strName;
	std::map<int, double> m_mapSol2Unc;
};

class CSurfComp
{
public:
	CSurfComp();
#if 0
	CSurfComp(const struct surface* surface_ptr, const struct surface_comp* surface_comp_ptr);
#else
	CSurfComp(const cxxSurface* surface_ptr, const cxxSurfaceComp* surface_comp_ptr);
#endif
	virtual ~CSurfComp();

	// use implicit copy ctor

public:
	double m_dGrams;
	double m_dSpecific_area;
	double m_dPhase_proportion;
	double m_dMoles;
	double m_dCapacitance0;
	double m_dCapacitance1;
	double m_dDw;
	CString m_strFormula;
	CString m_strPhase_name;
	CString m_strRate_name;
};

class CKineticComp  
{
public:
	CKineticComp();
#if 0
	CKineticComp(const struct kinetics_comp *kinetics_comp_ptr);
#else
	CKineticComp(const cxxKineticsComp *comp);
#endif
	virtual ~CKineticComp();

public:
	CString m_strRateName;
	std::list<CNameCoef> m_listNameCoef;
	double m_dTol;
	double m_dM;
	double m_dM0;
	std::list<double> m_listDParams;
private:
	std::list<CString> m_listCParams;
	double m_dInitialMoles;
	double m_dMoles;
};

class CSpecies
{
public:
	CSpecies(const struct species *species_ptr);
	CSpecies();
	virtual ~CSpecies();
public:
	CString m_strEqn;
	CString m_strMoleBalance;
	double m_dLogK;
	double m_dDeltaH;
	bool m_bHasAnalExp;
	bool m_bHasCDMusic;
	bool m_bHasMillero;
	bool m_bCheckEqn;
	double m_dA1;
	double m_dA2;
	double m_dA3;
	double m_dA4;
	double m_dA5;
	double m_dA6;
	enum ActType
	{
		AT_NONE         = 0,
		AT_DAVIES       = 1,
		AT_DEBYE_HUCKEL = 2,
		AT_LLNL_DH      = 3,
		AT_LLNL_DH_CO2  = 4,
	} m_nActType;
	double m_dDHa;
	double m_dDHb;
	double m_dA_F;
	DELTA_H_UNIT m_nDeltaHUnits;
	double m_dCDMusic[5];
	double m_dw;         // tracer diffusion coefficient in water at 25oC, m2/s */
	double m_erm_ddl;    // enrichment factor in DDL
	double m_millero[6]; // regression coefficients to calculate temperature dependent phi_0 and b_v of Millero density model
protected:
	static CString WriteEqn(const struct species *species_ptr);
	int gflag;					/* flag for preferred activity coef eqn */ /* 0-9 */
	int exch_gflag;				/* flag for preferred activity coef eqn */ /* 1;3;7 */
};

class CPhase
{
public:
	CPhase(const struct phase *phase_ptr);
	CPhase();
	virtual ~CPhase();
public:
	CString m_strName;
	CString m_strEqn;
	CString m_strMoleBalance;
	double m_dLogK;
	double m_dDeltaH;
	bool m_bHasAnalExp;
	bool m_bCheckEqn;
	double m_dA1;
	double m_dA2;
	double m_dA3;
	double m_dA4;
	double m_dA5;
	double m_dA6;
	DELTA_H_UNIT m_nDeltaHUnits;
protected:
	static CString WriteEqn(const struct phase *phase_ptr);
};

class CMaster
{
public:
	CMaster(const struct master *master_ptr);
	CMaster();
	virtual ~CMaster();
public:
	CString m_strName;               // element name
	CString m_strMasterSpecies;      // master species
	double m_dAlk;                   // alkalinity
	double m_dGFW;                   // gram formula weight
	CString m_strFormula;            // formula
	double m_dGFWElement;            // gram formula weight of element
	bool m_bPrimary;
};

class CTransport
{
public:
	CTransport();
	virtual ~CTransport();
	void Update();

	void UpdatePrintRange(std::list<CRange> &list);
	void UpdatePunchRange(std::list<CRange> &list);

public:

	// Line 1:     -cells                 5
	int count_cells;

	// Line 2:     -shifts                25
	int count_shifts;

	// Line 3:      -time_step 3.15e7 # seconds = 1 yr.
	double timest;
	double mcd_substeps;

	// Line 4:     -flow_direction        forward
	// this is indexed from an enum (different from ::ishift)
	int shift;

	// Line 5:     -boundary_conditions   flux constant
	// these are indexed from an enum (different from ::bcon_first and ::bcon_last)
	int bc_first;
	int bc_last;

	// Line 6:     -lengths               4*1.0 2.0
	std::list<CRepeat> lengths_list;

	// Line 7:     -dispersivities        4*0.1 0.2
	std::list<CRepeat> disp_list;

	// Line 8:     -correct_disp          true
	int correct_disp;

	// Line 9:     -diffusion_coefficient 1.0e-9
	double diffc;

	// Line 10:    -stagnant              1  6.8e-6   0.3   0.1
	// (stag_data)
	int    count_stag;
	double exch_f;
	double th_m;
	double th_im;

	// Line 11:    -thermal_diffusion     3.0   0.5e-6
	double tempr;
	double heat_diffc;

	// Line 12:    -initial_time          1000
	double initial_total_time;

	// Line 13:    -print_cells           1-3 5
	std::list<CRange> print_range_list;

	// Line 14:      -print_frequency 5
	int print_modulus;

	// Line 15:      -punch_cells 2-5
	std::list<CRange> punch_range_list;

	// Line 16:      -punch_frequency 5
	int punch_modulus;

	// Line 17:    -dump                  dump.file
	int dump_in;
	CString dump_file_name;

	// Line 18:    -dump_frequency        10
	int dump_modulus;

	// Line 19:    -dump_restart          20
	int transport_start;

	// Line 20:    -warnings              false
	int transport_warnings;

	// Multicomponent diffusion
	// Line 21: -multi_d true 1e-9 0.3 0.05 1.0
	int    multi_Dflag;
	double default_Dw;
	double multi_Dpor;
	double multi_Dpor_lim;
	double multi_Dn;

	// Interlayer diffusion
	// Line 22: -interlayer_D true 0.09 0.01 150
	int    interlayer_Dflag;
	double interlayer_Dpor;
	double interlayer_Dpor_lim;
	double interlayer_tortf;

	// count of TRANSPORT keywords
	int simul_tr;
};


#endif // !defined(AFX_KEYWORDPAGELISTITEMS_H__13CB5247_EF41_48EA_86AA_A788A4219F30__INCLUDED_)
