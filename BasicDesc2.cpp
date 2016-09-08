// BasicDesc2.cpp: implementation of the CBasicDesc2 class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "BasicDesc2.h"
#include "Util.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const TCHAR ACT[]         = _T("ACT(\"species\")");
const TCHAR LA[]          = _T("LA(\"species\")");
const TCHAR LM[]          = _T("LM(\"species\")");
const TCHAR MOL[]         = _T("MOL(\"species\")");
const TCHAR LK_SPECIES[]  = _T("LK_SPECIES(\"species\")");

const TCHAR EQUI[]        = _T("EQUI(\"phase\")");
const TCHAR SI[]          = _T("SI(\"phase\")");
const TCHAR SR[]          = _T("SR(\"phase\")");
const TCHAR LK_PHASE[]    = _T("LK_PHASE(\"phase\")");

const TCHAR GAS[]         = _T("GAS(\"gas\")");

const TCHAR TOT[]         = _T("TOT(\"element\")");
const TCHAR SUM_SPECIES[] = _T("SUM_SPECIES(\"template\", \"element\")");
const TCHAR SUM_GAS[]     = _T("SUM_GAS(\"template\", \"element\")");
const TCHAR SYS_ELEMENT[] = _T("SYS(\"element\" [ , count_species, names$, types$, moles])");

const TCHAR KIN[]         = _T("KIN(\"reactant\")");
const TCHAR MISC1[]       = _T("MISC1(\"component\")");
const TCHAR MISC2[]       = _T("MISC2(\"component\")");

const TCHAR EDL[]         = _T("EDL(\"element\", \"surface\")");
const TCHAR C_SURF[]      = _T("SURF(\"element\", \"surface\")");
const TCHAR LK_NAMED[]    = _T("LK_NAMED(\"name\")");
const TCHAR S_S[]         = _T("S_S(\"component\")");
const TCHAR CALC_VALUE[]  = _T("CALC_VALUE(\"calc_value_name\")");
const TCHAR SUM_s_s[]     = _T("SUM_s_s(\"s_s_name\", \"element\")");

const TCHAR RHO[]         = _T("RHO");
const TCHAR RHO_0[]       = _T("RHO_0");

const TCHAR GAMMA[]       = _T("GAMMA(\"species\")");
const TCHAR LG[]          = _T("LG(\"species\")");
const TCHAR GET_POR[]     = _T("GET_POR(cell_no)");
const TCHAR CHANGE_SURF[] = _T("CHANGE_SURF(\"surface\", fraction, \"new_surface_name\", diffusion_coef, cell_no)");
const TCHAR CHANGE_POR[]  = _T("CHANGE_POR(new_porosity, cell_no)");

const TCHAR TOTMOLE[]     = _T("TOTMOLE(\"element\")");
const TCHAR ISO[]         = _T("ISO(a$)");
const TCHAR ISO_UNITS[]   = _T("ISO_UNITS(a$)");

const TCHAR EOL[]         = _T("EOL$");
const TCHAR CEIL[]        = _T("CEIL(x)");
const TCHAR FLOOR[]       = _T("FLOOR(x)");

const TCHAR GRAPH_X[]     = _T("GRAPH_X tot(\"Ca\") * 40.08e3");
const TCHAR GRAPH_Y[]     = _T("GRAPH_Y tot(\"F\") * 19e3");
const TCHAR GRAPH_SY[]    = _T("GRAPH_SY -la(\"H+\")");
const TCHAR PLOT_XY[]     = _T("PLOT_XY tot(\"Ca\") * 40.08e3, tot(\"F\") * 19e3, color = Blue, symbol = Circle, symbol_size = 6, y-axis = 1, line_width = 0");

const TCHAR PHASE_FORM[]  = _T("PHASE_FORMULA(\"phase\")");
const TCHAR LIST_S_S[]    = _T("LIST_S_S(\"s_s_name\", count, comp$, moles)");
const TCHAR PR_P[]        = _T("PR_P(\"gas\")");
const TCHAR PR_PHI[]      = _T("PR_PHI(\"gas\")");
const TCHAR GAS_P[]       = _T("GAS_P");
const TCHAR GAS_VM[]      = _T("GAS_VM");
const TCHAR PRESS[]       = _T("PRESSURE");
const TCHAR ERASE[]       = _T("ERASE");
const TCHAR EPS_R[]       = _T("EPS_R");
const TCHAR VM[]          = _T("VM(\"species\")");
const TCHAR DH_A[]        = _T("DH_A");
const TCHAR DH_B[]        = _T("DH_B");
const TCHAR DH_AV[]       = _T("DH_Av");
const TCHAR QBRN[]        = _T("QBRN");

const TCHAR KAPPA[]       = _T("KAPPA");
const TCHAR GFW[]         = _T("GFW(\"formula\")");
const TCHAR SOLN_VOL[]    = _T("SOLN_VOL");
const TCHAR EQUI_DELTA[]  = _T("EQUI_DELTA(\"phase\")");
const TCHAR KIN_DELTA[]   = _T("KIN_DELTA(\"reactant\")");

const TCHAR KIN_TIME[]    = _T("KIN_TIME");
const TCHAR STR_F[]       = _T("STR_F$(x, w, d)");
const TCHAR STR_E[]       = _T("STR_E$(x, w, d)");
const TCHAR SPECIES_FORMULA[] = _T("SPECIES_FORMULA$(species$, count_s, elt$, coef)");
const TCHAR EQ_FRAC[]     = _T("EQ_FRAC$(species$, eq, x$)");

const TCHAR DIFF_C[]           = _T("DIFF_C(species$)");
const TCHAR SYS_EQUI[]         = _T("SYS(\"equi\" [ , count, names$, types$, values])");
const TCHAR EDL_SPECIES[]      = _T("EDL_SPECIES(\"surface\", count, name$, moles, area, thickness)");
const TCHAR KINETICS_FORMULA[] = _T("KINETICS_FORMULA(\"reactant\", count, elt$, coef)");
const TCHAR SYS_KIN[]          = _T("SYS(\"kin\" [ , count, names$, types$, values])");

//{{NEW BASIC HERE}}

CBasicDesc2::CBasicDesc2(const CDatabase& rDatabase, int nIDFuncs, int nIDExplan, int nIDArgs, bool bUserGraph)
: m_rDatabase(rDatabase), m_nIDFuncs(nIDFuncs), m_nIDExplan(nIDExplan), m_nIDArgs(nIDArgs), m_bUserGraph(bUserGraph)
{
	ASSERT(nIDFuncs != 0);
	ASSERT(nIDExplan != 0);
	ASSERT(nIDArgs != 0);
	m_bMapLoaded = false;
}

CBasicDesc2::~CBasicDesc2()
{

}

void CBasicDesc2::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, m_nIDFuncs, m_lbFuncs);
	DDX_Control(pDX, m_nIDArgs, m_treeArgs);
	DDX_Control(pDX, m_nIDExplan, m_eExplan);
	if (!m_bMapLoaded)
	{
		LoadMap();
		m_bMapLoaded = true;
		FillFuncs();
	}
}

void CBasicDesc2::LoadMap()
{
	ASSERT(m_bMapLoaded == false);
	ASSERT(m_mapFuncs.size() == 0);

	m_mapFuncs[ACT]                                  = _T("Activity of an aqueous, exchange, or surface species.");
	m_mapFuncs[_T("ALK")]                            = _T("Alkalinity of solution.");
	m_mapFuncs[_T("CELL_NO")]                        = _T("Cell number in TRANSPORT or ADVECTION calculations");
	m_mapFuncs[_T("CHARGE_BALANCE")]                 = _T("Aqueous charge balance in equivalents.");
	m_mapFuncs[_T("DIST")]                           = _T("Distance to midpoint of cell in TRANSPORT calculations, cell number in ADVECTION calculations, \"-99\" in all other calculations.");
	m_mapFuncs[EQUI]                                 = _T("Moles of a phase in the pure-phase (equilibrium-phase) assemblage.");
	m_mapFuncs[_T("EXISTS( i1 [ , i2, ... ])")]      = _T("Determines if a value has been stored with a PUT statement for the list of one or more subscripts.The function equals 1 if a value has been stored and 0 if no value has been stored. Values are stored in global storage with PUT and are accessible by any Basic program. See description of PUT for more details.");
	m_mapFuncs[GAS]                                  = _T("Moles of a gas component in the gas phase.");
	m_mapFuncs[_T("GET( i1 [ , i2, ... ])")]         = _T("Retrieves the value that is identified by the list of one or more subscripts.Value is zero if PUT has not been used to store a value for the set of subscripts. Values stored in global storage with PUT are accessible by any Basic program. See description of PUT for more details.");
	m_mapFuncs[KIN]                                  = _T("Moles of a kinetic reactant.");
	m_mapFuncs[LA]                                   = _T("Log10 of activity of an aqueous, exchange, or surface species.");
	m_mapFuncs[LM]                                   = _T("Log10 of molality of an aqueous, exchange, or surface species.");
	m_mapFuncs[_T("M")]                              = _T("Current moles of reactant for which the rate is being calculated (see KINETICS).");
	m_mapFuncs[_T("M0")]                             = _T("Initial moles of reactant for which the rate is being calculated (see KINETICS).");
	m_mapFuncs[MISC1]                                = _T("Mole fraction of component 2 at the beginning of the miscibility gap, returns 1.0 if there is no miscibility gap (see SOLID_SOLUTIONS).");
	m_mapFuncs[MISC2]                                = _T("Mole fraction of component 2 at the end of the miscibility gap, returns 1.0 if there is no miscibility gap (see SOLID_SOLUTIONS).");
	m_mapFuncs[MOL]                                  = _T("Molality of an aqueous, exchange, or surface species.");
	m_mapFuncs[_T("MU")]                             = _T("Ionic strength of the solution.");
	m_mapFuncs[_T("PARM( i )")]                      = _T("Parameter array defined in KINETICS data block.");
	m_mapFuncs[_T("PERCENT_ERROR")]                  = _T("Percent charge-balance error [100(cations-|anions|)/(cations + |anions|)].");
	m_mapFuncs[_T("PRINT")]                          = _T("Write to output file.");
	m_mapFuncs[_T("PUNCH")]                          = _T("Write to selected-output file.");
	m_mapFuncs[_T("PUT( x , i1 [ , i2, ... ])")]     = _T("Saves value of x in global storage that is identified by a sequence of one or more subscripts. Value of x can be retrieved with GET( i1, [ , i2, ... ]) and a set of subscripts can be tested to determine if a value has been stored with EXISTS( i1 [ , i2, ... ]). PUT may be used in RATES, USER_PRINT, or USER_PUNCH Basic programs to store a value. The value may be retrieved by any of these Basic programs. The value persists until overwritten using a PUT statement with the same set of subscripts, or until the end of the run. For a KINETICS data block, the Basic programs for the rate expressions are evaluated in the order in which they are defined in the input file.");
	m_mapFuncs[_T("RXN")]                            = _T("Amount of reaction (moles) as defined in -steps in REACTION data block for a batch-reaction calculation, otherwise zero.");
	m_mapFuncs[_T("SAVE")]                           = _T("Last statement of Basic program that returns the moles of kinetic reactant, counted positive when the solution concentration of the reactant increases.");
	m_mapFuncs[SI]                                   = _T("Saturation index of a phase. For gases, this value is equal to log10(fugacity). For ideal gases, fugacity equals partial pressure. For Peng-Robinson gases, the Basic functions PR_P and PR_PHI can be used to obtain the gas partial pressure and the fugacity coefficient.");
	m_mapFuncs[_T("SIM_NO")]                         = _T("Simulation number, equals one more than the number of END statements before current simulation.");
	m_mapFuncs[_T("SIM_TIME")]                       = _T("Time (s) from the beginning of a kinetic batch-reaction or transport calculation.");
	m_mapFuncs[SR]                                   = _T("Saturation ratio of a phase. For gases, SR returns the fugacity of the gas (P*phi/1 atm).");
	m_mapFuncs[_T("STEP_NO")]                        = _T("Step number in batch-reaction calculations, or shift number in ADVECTION and TRANSPORT calculations.");
	m_mapFuncs[S_S]                                  = _T("Current moles of a solid-solution component.");
	m_mapFuncs[_T("TC")]                             = _T("Temperature in Celsius.");
	m_mapFuncs[_T("TK")]                             = _T("Temperature in Kelvin.");
	m_mapFuncs[_T("TIME")]                           = _T("Time interval for which moles of reaction are calculated in rate programs, automatically set in the time-step algorithm of the numerical integration method.");
	m_mapFuncs[TOT]                                  = _T("Total molality of element or element redox state. TOT(\"water\") is total mass of water (kg).");
	m_mapFuncs[_T("TOTAL_TIME")]                     = _T("Cumulative time (s) including all advective (for which -time_step is defined) and advective-dispersive transport simulations from the beginning of the run or from last -initial_time identifier.");
	m_mapFuncs[EDL]                                  = _T("Gives the amount of \"element\" in the diffuse layer for \"surface.\"  \"surface\" should be the surface name, not the surface-site name (that is, no underscore).");
	m_mapFuncs[C_SURF]                               = _T("Gives the amount of \"element\" sorbed to \"surface.\"  \"surface\" should be the surface name, not the surface-site name (that is, no underscore).");

	//{{
	m_mapFuncs[LK_SPECIES]                           = _T("log10(K) of definition in (SOLUTION, EXCHANGE, SURFACE)_SPECIES");
	m_mapFuncs[LK_NAMED]                             = _T("log10(K) of definition in NAMED_EXPRESSIONS");
	m_mapFuncs[LK_PHASE]                             = _T("log10(K) of definition in PHASES");
	m_mapFuncs[SUM_SPECIES]                          = _T("Sum of element in aqueous, exchange, and surface species with specified template");
	m_mapFuncs[SUM_GAS]                              = _T("Sum of element in gases with specified template (for example template=\"{C,[13C],[14C]}{O,[18O]}2\" includes all CO2 gases)");
	m_mapFuncs[SUM_s_s]                              = _T("Sum of element in a specified solid solution");
	m_mapFuncs[CALC_VALUE]                           = _T("Evaluates a definition of CALCULATE_VALUES.");
	//}}

	//{{ 2.9 added functions
	m_mapFuncs[_T("PAD(a$, n)")]                     = _T("Pads a$ to a total of n characters with spaces. PAD returns a copy of a$ if a$ is more than 20 characters.");
	m_mapFuncs[_T("INSTR(a$, b$)")]                  = _T("Returns the character position of string b$ in a$, 0 if not found.");
	m_mapFuncs[_T("LTRIM(a$)")]                      = _T("Trims white space from beginning of string a$ and returns the result.");
	m_mapFuncs[_T("RTRIM(a$)")]                      = _T("Trims white space from end of string a$ and returns the result.");
	m_mapFuncs[_T("TRIM(a$)")]                       = _T("Trims white space from beginning and end of string a$ and returns the result.");
	m_mapFuncs[_T("DESCRIPTION")]                    = _T("The value defined for the description field of the SOLUTION keyword line.");

	// sys functions
	m_mapFuncs[_T("SYS(\"aq\" [ , count, names$, types$, values])")] = 
		_T("Returns sum of moles of all aqueous species.")
		_T(" count is number of aqueous species in system.")
		_T(" Arrays are filled with each aqueous species; values are moles.");

	m_mapFuncs[SYS_ELEMENT] =
		_T("Returns the total amount of element in the system;")
		_T(" count_species -- the number of species that contain element, including solution, equilibrium_phases, surfaces, exchangers, solid solutions, and gas phase species;")
		_T(" names$ -- a character array that has the name of each species;")
		_T(" type$ -- a character array that specifies the type of phase for the species, aq, equi, surf, ex, s_s, gas, diff.  Diff refers to the amount of the element in the diffuse layer of a surface when the explicit diffuse layer calculation is used;")
		_T(" moles -- an array containing the number of moles of the element in the species.");

	m_mapFuncs[_T("SYS(\"elements\" [ , count, names$, types$, values])")] = 
		_T("Returns total number of moles of dissolved elements other than H and O.")
		_T(" count is number of elements, valence states, exchangers, and surfaces.")
		_T(" Arrays are filled for each element and valence state, type is \"dis\"; exchanger, type is \"ex\", and surface, type is \"surf\". Values are moles.");

	m_mapFuncs[SYS_EQUI] = 
		_T("Returns the sum of moles of all equilibrium phases in the calculation.")
		_T(" count is number of equilibrium phases.")
		_T(" Arrays are filled with each equilibrium phase; values are moles of each equilibrium phase.");

	m_mapFuncs[_T("SYS(\"ex\" [ , count, names$, types$, values])")] = 
		_T("Returns sum of moles of all exchange species.")
		_T(" count is number of exchange species in system.")
		_T(" Arrays are filled with each exchange species; values are moles.");

	m_mapFuncs[_T("SYS(\"gas\" [ , count, names$, types$, values])")] = 
		_T("Returns sum of moles of all gas components.")
		_T(" count is number of gas components in system.")
		_T(" Arrays are filled with each gas component; values are moles.");

	m_mapFuncs[SYS_KIN] = 
		_T("Returns the sum of moles of all kinetic reactants.")
		_T(" count is number of kinetic reactants.")
		_T(" Arrays are filled with each kinetic reactant; values are moles of each kinetic reactant.");

	m_mapFuncs[_T("SYS(\"phases\" [ , count, names$, types$, values])")] = 
		_T("Returns maximum saturation index of all phases.")
		_T(" count is number of phases in system.")
		_T(" Arrays are filled for each phase; values are saturation indexes.");

	m_mapFuncs[_T("SYS(\"surf\" [ , count, names$, types$, values])")] = 
		_T("Returns sum of moles of all surface species.")
		_T(" count is number of surface species in system.")
		_T(" Arrays are filled with each surface species; values are moles.");

	m_mapFuncs[_T("SYS(\"s_s\" [ , count, names$, types$, values])")] = 
		_T("Returns sum of moles of solid solution components.")
		_T(" count is number of solid solution components in system.")
		_T(" Arrays are filled with each solid solution component; values are moles.");

	m_mapFuncs[GET_POR]       = _T("Returns the porosity in cell 'cell_no'.");
	m_mapFuncs[CHANGE_POR]    = _T("Sets the porosity in cell 'cell_no' to 'new_prosity'.");
	m_mapFuncs[CHANGE_SURF]   = _T("Changes the diffusion coefficient of (part of) a SURFACE, and hence to change the status from mobile to immobile or immobile to mobile.");
	m_mapFuncs[_T("OSMOTIC")] = _T("Returns the osmotic coefficient if the Pitzer model (PITZER keyword data block) is used or 0.0 if the ion-association model is used.");

	//}} 2.9 added functions

	// added for version 2.15.0
	m_mapFuncs[_T("SC")] = _T("Returns the specific conductance(uS/cm) for the solution at 25 C. Requires the use of the phreeqd.dat database.");

	m_mapFuncs[RHO]         = _T("Returns the density of the solution.");
	m_mapFuncs[RHO_0]       = _T("Returns the density of pure water at the current temperature.");
	m_mapFuncs[GAMMA]       = _T("Returns the activity coefficient of the given aqueous species.");
	m_mapFuncs[LG]          = _T("Returns the log base 10 of the activity coefficient of the given aqueous species.");

	//{{ added 4189
	m_mapFuncs[TOTMOLE] = 
		_T("Returns the total number of moles of an element")
		_T(" or element valence state in solution. Special values are")
		_T(" \"water\", which gives number of moles of water, and")
		_T(" \"charge\", which gives total equivalents of charge")
		_T(" imbalance in solution (same as Basic function")
		_T(" CHARGE_BALANCE). In contrast, the Basic function TOT")
		_T(" returns moles per kilogram of water, or equivalents per")
		_T(" kilogram of water for TOT(\"charge\").");
	//}} added 4189

	//{{ added 4191
	m_mapFuncs[ISO] = 
		_T("Returns the isotopic composition in the input units for")
		_T(" an isotope--permil, pmc, or TU in current version of")
		_T(" iso.dat. The string argument can be an isotope name, or")
		_T(" any item defined in the ISOTOPE_RATIOS data block, For")
		_T(" example, ISO(\"R(13C)_Calcite\") will return the carbon-13")
		_T(" composition of the calcite solid solution in permil")
		_T(" because of the definitions in iso.dat.");

	m_mapFuncs[ISO_UNITS] = 
		_T("Returns the input units for the given isotope.")
		_T(" The string argument can be an")
		_T(" isotope name or an item defined in the ISOTOPE_RATIOS data")
		_T(" block as explained for the Basic function ISO.");
	//}} added 4191

	//{{ added 5189
	m_mapFuncs[EOL] =
		_T("The end of line character for the operating system you are running.");
	m_mapFuncs[CEIL] = 
		_T("Returns the smallest integer greater than or equal to x.");
	m_mapFuncs[FLOOR] = 
		_T("Returns the largest integer less than or equal to x.");
	//}} added 5189

	m_mapFuncs[PHASE_FORM] = 
		_T("With four arguments, PHASE_FORMULA returns a string that contains the chemical formula for")
		_T(" the phase, and, in addition, returns values for count, elt$, coef. Count is the dimension of the elt$")
		_T(" and coef arrays. Elt$ is a character array with the name of each element in the chemical formula for")
		_T(" the phase. Coef is a numeric array containing the number of atoms of each element in the phase")
		_T(" formula, in the order defined by Elt$, which is alphabetical by element.");

	m_mapFuncs[LIST_S_S] = 
		_T("Returns the sum of the moles of components in a solid solution and the composition of the solid")
		_T(" solution. The first argument is an input value specifying the name of the solid solution. Count is an")
		_T(" output variable containing the number of components in the solid solution. Comp$ is an output")
		_T(" character array containing the names of each component in the solid solution. Moles is an output")
		_T(" numeric array containing the number of moles of each component, in the order defined by Comp$.")
		_T(" Arrays are in sort order by number of moles.");

	m_mapFuncs[PR_P] = 
		_T("Pressure (atm) of a gas component in a Peng-Robinson GAS_PHASE.");

	m_mapFuncs[PR_PHI] = 
		_T("Fugacity coefficient of a gas component in a Peng-Robinson GAS_PHASE.");

	m_mapFuncs[GAS_P] = 
		_T("Pressure of the GAS_PHASE (atm), either specified for a fixed-pressure gas phase, or calculated")
		_T(" for a fixed-volume gas phase..Related functions are PR_P and PRESSURE.");

	m_mapFuncs[GAS_VM] = 
		_T("Molar volume (L/mol) of the GAS_PHASE (calculated with Peng-Robinson).");

	m_mapFuncs[PRESS] = 
		_T("Current pressure applied to the solution (atm). PRESSURE is a specified value except for")
		_T(" fixed-volume GAS_PHASE calculations.");

	m_mapFuncs[ERASE] = 
		_T("Revert the Basic variable to an undimensioned state so that it can be used as a scalar or")
		_T(" dimensioned with another DIM statement. Applies only to variables that have been")
		_T(" dimensioned with a DIM statement.");

	m_mapFuncs[EPS_R] = 
		_T("Relative dielectric constant.");

	m_mapFuncs[VM] = 
		_T("Returns the specific volume (cm3/mol) of a SOLUTION_SPECIES, relative to VM(\"H+\") = 0, a")
		_T(" function of temperature, pressure and ionic strength.");

	m_mapFuncs[DH_A] = 
		_T("Debye-Hückel A parameter in the activity coefficient equation, (mol/kg)^-0.5.");

	m_mapFuncs[DH_B] = 
		_T("Debye-Hückel B parameter in the activity coefficient equation, angstrom^-1(mol/kg)^-0.5.");

	m_mapFuncs[DH_AV] = 
		_T("Debye-Hückel limiting slope of specific volume vs. ionic strength, (cm3/mol)(mol/kg)^-0.5.");

	m_mapFuncs[QBRN] = 
		_T("The Born parameter for calculating the temperature dependence of the specific volume of an")
		_T(" aqueous species at infinite dilution. This is the pressure derivative of the relative dielectric constant")
		_T(" of water multiplied by 41.84 bar cm^3/cal.");

	m_mapFuncs[KAPPA] = 
		_T("Compressibility of pure water at current pressure and temperature.");

	m_mapFuncs[GFW] = 
		_T("Returns the gram formula weight of the specified formula.");

	m_mapFuncs[SOLN_VOL] = 
		_T("Volume of the solution, in liters.");

	m_mapFuncs[EQUI_DELTA] = 
		_T("Moles of a phase in the equilibrium-phase assemblage that reacted during the current calculation.");

	m_mapFuncs[KIN_DELTA] = 
		_T("Moles of a kinetic reactant that reacted during the current calculation.");

	m_mapFuncs[KIN_TIME] = 
		_T("Returns the time interval in seconds of the last kinetic integration. KIN_DELTA(\"CH2O\")/KIN_TIME will give the average rate over the time interval for reaction CH2O.");

	m_mapFuncs[STR_F] = 
		_T("Returns a string from a number with a given width (w) and number of decimal places (d). w is the minimum width of ")
		_T("the string. The string is padded with spaces to the left to produce a string of the specified width (w)");

	m_mapFuncs[STR_E] = 
		_T("Returns a string with exponential format from a number with a given width (w) and number of decimal places (d). w is \n")
		_T("the minimum width of the string. The string is padded with spaces to the left to produce a string of the specified \n")
		_T("width (w)\n");

	m_mapFuncs[SPECIES_FORMULA] = 
		_T("Returns the stoichiometry of an aqueous, exchange, or surface \n")
		_T("species. The function returns a string: \"aq\" for \n")
		_T("aqueous, \"ex\" for exchange, \"surf\" for surface, \n")
		_T("and \"none\" if there is no species of that name. \n")
		_T("The four arguments are \n")
		_T("(1) the name of the species (input), \n")
		_T("(2) the number of elements, including charge (output), \n")
		_T("(3) a string array of element names (output), \n")
		_T("(4) a number array of coefficients corresponding to the elements (output). \n");

	m_mapFuncs[EQ_FRAC] = 
		_T("Returns the equivalent fraction of a surface \n")
		_T("or exchange species. The three arguments are \n")
		_T("(1) Species name (input), \n")
		_T("(2) Equivalents of exchange or surface sites \n")
		_T("    per mole of the species (output), \n")
		_T("(3) The name of the surface or exchange site \n")
		_T("    (output). \n");

	m_mapFuncs[DIFF_C] = 
		_T("Diffusion coefficient at 25 C for the specified aqueous species. \n");

	m_mapFuncs[EDL_SPECIES] = 
		_T("The total number of moles of species in the diffuse ")
		_T("layer. The arguments to the function are as follows: ")
		_T("\r\n")
		_T("surface$ is the name of a surface, such as \"Hfo\", excluding ")
		_T("the site type (such as \"_s\").\r\n")
		_T("count is the number of species in the diffuse layer.\r\n")
		_T("name$ is an array of size count that contains the ")
		_T("names of aqueous species in the diffuse layer ")
		_T("of surface surf$.\r\n")
		_T("moles is an array of size count that contains the number ")
		_T("of moles of each aqueous species in the diffuse layer ")
		_T("of surface surf$.\r\n")
		_T("area is the area of the surface in m^2.\r\n")
		_T("thickness is the thickness of the diffuse layer in m.\r\n")
		_T("\r\n")
		_T("The volume of the diffuse layer is area * thickness, and ")
		_T("the concentrations of the species in the diffuse layer are ")
		_T("the number of moles divided by the volume.");

	m_mapFuncs[KINETICS_FORMULA] = 
		_T("Searches for a kinitic reaction definition. If found, returns the first argument otherwise an empty string is returned. \r\n")
		_T("The arguments to the function are as follows: \r\n")
		_T("reactant is name of the kinetic reaction definition. \r\n")
		_T("count is the number of items in the arrays elt$ and coef; \r\n")
		_T("elt$ is a list of element names in the formula for the kinetic reaction; \r\n")
		_T("and coef is a numeric array containing the number of atoms of each element in ")
		_T("the kinetic-reaction formula, in the order defined by elt$, which is alphabetical by element.");

	//{{NEW BASIC HERE}}

	if (this->m_bUserGraph)
	{
		// currently only used by UserGraphPg2.cpp
		//
		m_mapFuncs[GRAPH_X] = 
			_T("Used in USER_GRAPH data block to define the X values for points.  Here, Ca in mg/L is the X ")
			_T("value for points of the chart.  See the description of the USER_GRAPH keyword for more details.");

		m_mapFuncs[GRAPH_Y] = 
			_T("Used in USER_GRAPH data block to define the Y values for points plotted on the primary Y axis.  ")
			_T("Here, F in mg/L is the Y value for points. See the description of the USER_GRAPH keyword for ")
			_T("more details.");

		m_mapFuncs[GRAPH_SY] = 
			_T("Used in USER_GRAPH data block to define the Y values for points plotted on the secondary Y ")
			_T("axis. Here, pH is the Y value for points plotted on the secondary Y axis. See the description of the ")
			_T("USER_GRAPH keyword for more details.");

		m_mapFuncs[PLOT_XY] = 
			_T("Used in USER_GRAPH data block to define the points to chart; here, Ca in mg/L is the X value ")
			_T("for points, F in mg/L is the Y value for points, the symbols are blue circles, the points are plotted ")
			_T("relative to the Y axis, and no line connects the points. See the description of the USER_GRAPH ")
			_T("keyword for more details.");
	}
}

void CBasicDesc2::FillFuncs()
{
	// fill list box
	std::map<CString, CString>::iterator funcs = m_mapFuncs.begin();
	for (; funcs != m_mapFuncs.end(); ++funcs)
	{
		m_lbFuncs.AddString(funcs->first);
	}

	// select first func if exists
	funcs = m_mapFuncs.begin();
	if (funcs != m_mapFuncs.end())
	{
		m_lbFuncs.SelectString(-1, funcs->first);
	}
	OnSelchangeLbFuncs();
}

void CBasicDesc2::OnSelchangeLbFuncs() 
{
	int nIndex = m_lbFuncs.GetCurSel();
	CWaitCursor wait;

	if (nIndex != LB_ERR)
	{
		CString str;
		m_lbFuncs.GetText(nIndex, str);

		std::map<CString, CString>::iterator find = m_mapFuncs.find(str);

		if (find != m_mapFuncs.end())
		{
			m_eExplan.SetWindowText(find->second);
			m_eExplan.RedrawWindow();
			if (str == ACT || str == LA || str == LM || str == MOL || str == LK_SPECIES || str == GAMMA || str == LG || str == VM || str == SPECIES_FORMULA || str == DIFF_C)
			{
				if ( !(m_strPrev == ACT || m_strPrev == LA || m_strPrev == LM || m_strPrev == MOL || m_strPrev == LK_SPECIES || m_strPrev == GAMMA || m_strPrev == LG || m_strPrev == VM || m_strPrev == SPECIES_FORMULA || m_strPrev == DIFF_C) )
				{
					m_treeArgs.DeleteAllItems();

					HTREEITEM hArg1 = m_treeArgs.InsertItem(_T("species"));
					CUtil::InsertAqSpecies(&m_treeArgs, m_rDatabase, hArg1);
					if (m_treeArgs.ItemHasChildren(hArg1))
					{
						m_treeArgs.Expand(hArg1, TVE_EXPAND);
					}
					else
					{
						m_treeArgs.DeleteAllItems();
					}
				}
			}
			else if (str == EQUI || str == EQUI_DELTA || str == SI || str == SR || str == LK_PHASE || str == PHASE_FORM)
			{
				if ( !(m_strPrev == EQUI || m_strPrev == EQUI_DELTA || m_strPrev == SI || m_strPrev == SR || m_strPrev == LK_PHASE || m_strPrev == PHASE_FORM) )
				{
					m_treeArgs.DeleteAllItems();

					HTREEITEM hArg1 = m_treeArgs.InsertItem(_T("phase"));
					CUtil::InsertPhases(&m_treeArgs, m_rDatabase, hArg1);
					if (m_treeArgs.ItemHasChildren(hArg1))
					{
						m_treeArgs.Expand(hArg1, TVE_EXPAND);
					}
					else
					{
						m_treeArgs.DeleteAllItems();
					}
				}
			}
			else if (str == GAS || str == PR_P || str == PR_PHI)
			{
				if (!(m_strPrev == GAS || m_strPrev == PR_P || m_strPrev == PR_PHI))
				{
					m_treeArgs.DeleteAllItems();

					HTREEITEM hArg1 = m_treeArgs.InsertItem(_T("gas"));
					CUtil::InsertGases(&m_treeArgs, m_rDatabase, hArg1);
					if (m_treeArgs.ItemHasChildren(hArg1))
					{
						m_treeArgs.Expand(hArg1, TVE_EXPAND);
					}
					else
					{
						m_treeArgs.DeleteAllItems();
					}
				}
			}
			else if (str == TOT || str == SUM_SPECIES || str == SUM_GAS	|| str == SYS_ELEMENT || str == TOTMOLE)
			{
				if ( !(m_strPrev == TOT || m_strPrev == SUM_SPECIES || m_strPrev == SUM_GAS	|| m_strPrev == SYS_ELEMENT  || m_strPrev == TOTMOLE) )
				{
					m_treeArgs.DeleteAllItems();

					HTREEITEM hArg1 = m_treeArgs.InsertItem(_T("element"));
					CUtil::InsertAqElements(&m_treeArgs, m_rDatabase, hArg1);
					m_treeArgs.InsertItem(_T("water"), hArg1);
					if (m_treeArgs.ItemHasChildren(hArg1))
					{
						m_treeArgs.Expand(hArg1, TVE_EXPAND);
					}
					else
					{
						m_treeArgs.DeleteAllItems();
					}
				}
			}
			else if (str == KIN || str == KIN_DELTA || str == KINETICS_FORMULA)
			{
				if ( !(m_strPrev == KIN || m_strPrev == KIN_DELTA || m_strPrev == KINETICS_FORMULA) )
				{
					m_treeArgs.DeleteAllItems();

					HTREEITEM hArg1 = m_treeArgs.InsertItem(_T("reactant"));
					CUtil::InsertRates(&m_treeArgs, m_rDatabase, hArg1);
					if (m_treeArgs.ItemHasChildren(hArg1))
					{
						m_treeArgs.Expand(hArg1, TVE_EXPAND);
					}
					else
					{
						m_treeArgs.DeleteAllItems();
					}
				}
			}
			else if (str == MISC1 || str == MISC2)
			{
				if ( !(m_strPrev == MISC1 || m_strPrev == MISC2) )
				{
					m_treeArgs.DeleteAllItems();

					HTREEITEM hArg1 = m_treeArgs.InsertItem(_T("component"));
					CUtil::InsertSolidSolutions(&m_treeArgs, m_rDatabase, hArg1);
					if (m_treeArgs.ItemHasChildren(hArg1))
					{
						m_treeArgs.Expand(hArg1, TVE_EXPAND);
					}
					else
					{
						m_treeArgs.DeleteAllItems();
					}
				}
			}
			else if (str == S_S)
			{
				if ( !(m_strPrev == S_S) )
				{
					m_treeArgs.DeleteAllItems();

					HTREEITEM hArg1 = m_treeArgs.InsertItem(_T("component"));
					CUtil::InsertSolidSolutionComps(&m_treeArgs, m_rDatabase, hArg1);
					if (m_treeArgs.ItemHasChildren(hArg1))
					{
						m_treeArgs.Expand(hArg1, TVE_EXPAND);
					}
					else
					{
						m_treeArgs.DeleteAllItems();
					}
				}
			}
			else if (str == EDL)
			{
				if ( !(m_strPrev == EDL) )
				{
					m_treeArgs.DeleteAllItems();

					HTREEITEM hArg1 = m_treeArgs.InsertItem(_T("element"));
					CUtil::InsertAqElements(&m_treeArgs, m_rDatabase, hArg1);

					// add special options for "element"
					m_treeArgs.InsertItem("charge", hArg1);
					m_treeArgs.InsertItem("charge1", hArg1);
					m_treeArgs.InsertItem("charge2", hArg1);
					m_treeArgs.InsertItem("sigma", hArg1);
					m_treeArgs.InsertItem("sigma1", hArg1);
					m_treeArgs.InsertItem("sigma2", hArg1);
					m_treeArgs.InsertItem("psi", hArg1);
					m_treeArgs.InsertItem("psi1", hArg1);
					m_treeArgs.InsertItem("psi2", hArg1);
					m_treeArgs.InsertItem("water", hArg1);

					HTREEITEM hArg2 = m_treeArgs.InsertItem(_T("surface"));
					std::set<CDBElement>::const_iterator elemIter = m_rDatabase.m_elementSet.begin();
					std::set<CString> setSurfaces;
					for ( ; elemIter != m_rDatabase.m_elementSet.end(); ++elemIter)
					{
						if ( (*elemIter).m_type == CDBElement::typeSURF )
						{
							CString strSurface = (*elemIter).m_strName.SpanExcluding(_T("_"));
							setSurfaces.insert(strSurface);
						}
					}
					std::set<CString>::const_iterator surfIter = setSurfaces.begin();
					for ( ; surfIter != setSurfaces.end(); ++surfIter)
					{
						m_treeArgs.InsertItem(*surfIter, hArg2);
					}
				}
			}
			else if (str == C_SURF)
			{
				if ( !(m_strPrev == C_SURF) )
				{
					m_treeArgs.DeleteAllItems();

					HTREEITEM hArg1 = m_treeArgs.InsertItem(_T("element"));
					CUtil::InsertAqElements(&m_treeArgs, m_rDatabase, hArg1);

					HTREEITEM hArg2 = m_treeArgs.InsertItem(_T("surface"));
					std::set<CDBElement>::const_iterator elemIter = m_rDatabase.m_elementSet.begin();
					std::set<CString> setSurfaces;
					for ( ; elemIter != m_rDatabase.m_elementSet.end(); ++elemIter)
					{
						if ( (*elemIter).m_type == CDBElement::typeSURF )
						{
							CString strSurface = (*elemIter).m_strName.SpanExcluding(_T("_"));
							setSurfaces.insert(strSurface);
						}
					}
					std::set<CString>::const_iterator surfIter = setSurfaces.begin();
					for ( ; surfIter != setSurfaces.end(); ++surfIter)
					{
						m_treeArgs.InsertItem(*surfIter, hArg2);
					}
				}
			}
			else if (str == CHANGE_SURF || str == EDL_SPECIES)
			{
				if ( !(m_strPrev == CHANGE_SURF || m_strPrev == EDL_SPECIES))
				{
					m_treeArgs.DeleteAllItems();

					HTREEITEM hArg = m_treeArgs.InsertItem(_T("surface"));
					std::set<CDBElement>::const_iterator elemIter = m_rDatabase.m_elementSet.begin();
					std::set<CString> setSurfaces;
					for ( ; elemIter != m_rDatabase.m_elementSet.end(); ++elemIter)
					{
						if ( (*elemIter).m_type == CDBElement::typeSURF )
						{
							CString strSurface = (*elemIter).m_strName.SpanExcluding(_T("_"));
							setSurfaces.insert(strSurface);
						}
					}
					std::set<CString>::const_iterator surfIter = setSurfaces.begin();
					for ( ; surfIter != setSurfaces.end(); ++surfIter)
					{
						m_treeArgs.InsertItem(*surfIter, hArg);
					}
				}
			}
			else if (str == LK_NAMED)
			{
				if ( !(m_strPrev == LK_NAMED) )
				{
					m_treeArgs.DeleteAllItems();

					HTREEITEM hArg1 = m_treeArgs.InsertItem(_T("named"));
					CUtil::InsertNamedExp(&m_treeArgs, m_rDatabase, hArg1);
					if (m_treeArgs.ItemHasChildren(hArg1))
					{
						m_treeArgs.Expand(hArg1, TVE_EXPAND);
					}
					else
					{
						m_treeArgs.DeleteAllItems();
					}
				}
			}
			else if (str == CALC_VALUE)
			{
				if ( !(m_strPrev == CALC_VALUE) )
				{
					m_treeArgs.DeleteAllItems();

					HTREEITEM hArg1 = m_treeArgs.InsertItem(_T("calc_value_name"));
					CUtil::InsertCalcVal(&m_treeArgs, m_rDatabase, hArg1);
					if (m_treeArgs.ItemHasChildren(hArg1))
					{
						m_treeArgs.Expand(hArg1, TVE_EXPAND);
					}
					else
					{
						m_treeArgs.DeleteAllItems();
					}
				}
			}
			else if (str == SUM_s_s || str == LIST_S_S)
			{
				if ( !(m_strPrev == SUM_s_s || m_strPrev == LIST_S_S) )
				{
					m_treeArgs.DeleteAllItems();

					HTREEITEM hArg1 = m_treeArgs.InsertItem(_T("s_s_name"));					
					CUtil::InsertSolidSolutions(&m_treeArgs, m_rDatabase, hArg1);


					HTREEITEM hArg2 = m_treeArgs.InsertItem(_T("element"));
					CUtil::InsertAqElements(&m_treeArgs, m_rDatabase, hArg2);

					if (m_treeArgs.ItemHasChildren(hArg1))
					{
						if (m_treeArgs.ItemHasChildren(hArg2))
						{
							// do nothing
						}
						else
						{
							m_treeArgs.Expand(hArg1, TVE_EXPAND);
							m_treeArgs.DeleteItem(hArg2);
							m_treeArgs.EnsureVisible(hArg1);
						}
					}
					else
					{
						if (m_treeArgs.ItemHasChildren(hArg2))
						{
							m_treeArgs.Expand(hArg2, TVE_EXPAND);
							m_treeArgs.DeleteItem(hArg1);
							m_treeArgs.EnsureVisible(hArg2);
						}
					}
				}
			}
			else if (str == EQ_FRAC)
			{
				if ( !(m_strPrev == EQ_FRAC) )
				{
					m_treeArgs.DeleteAllItems();

					HTREEITEM hArg1 = m_treeArgs.InsertItem(_T("species"));
					CUtil::InsertAqSpeciesSurfEx(&m_treeArgs, m_rDatabase, hArg1);
					if (m_treeArgs.ItemHasChildren(hArg1))
					{
						m_treeArgs.Expand(hArg1, TVE_EXPAND);
					}
					else
					{
						m_treeArgs.DeleteAllItems();
					}
				}
			}
			//{{NEW BASIC HERE}}
			else
			{
				m_treeArgs.DeleteAllItems();
			}
		}
		else
		{
			m_eExplan.SetWindowText(_T(""));
			m_treeArgs.DeleteAllItems();

		}
		m_strPrev = str;
	}

}
