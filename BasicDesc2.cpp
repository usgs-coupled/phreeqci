// BasicDesc2.cpp: implementation of the CBasicDesc2 class.
//
// $Date: 2004/09/20 20:37:37 $
// $Revision: 1.3 $
// $Name: R_2_10 $
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

CBasicDesc2::CBasicDesc2(const CDatabase& rDatabase, int nIDFuncs, int nIDExplan, int nIDArgs)
: m_rDatabase(rDatabase), m_nIDFuncs(nIDFuncs), m_nIDExplan(nIDExplan), m_nIDArgs(nIDArgs)
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

	m_mapFuncs[_T("ACT(\"species\")")]               = _T("Activity of an aqueous, exchange, or surface species.");
	m_mapFuncs[_T("ALK")]                            = _T("Alkalinity of solution.");
	m_mapFuncs[_T("CELL_NO")]                        = _T("Cell number in TRANSPORT or ADVECTION calculations");
	m_mapFuncs[_T("CHARGE_BALANCE")]                 = _T("Aqueous charge balance in equivalents.");
	m_mapFuncs[_T("DIST")]                           = _T("Distance to midpoint of cell in TRANSPORT calculations, cell number in ADVECTION calculations, \"-99\" in all other calculations.");
	m_mapFuncs[_T("EQUI(\"phase\")")]                = _T("Moles of a phase in the pure-phase (equilibrium-phase) assemblage.");
	m_mapFuncs[_T("EXISTS( i1 [ , i2, ... ])")]      = _T("Determines if a value has been stored with a PUT statement for the list of one or more subscripts.The function equals 1 if a value has been stored and 0 if no value has been stored. Values are stored in global storage with PUT and are accessible by any Basic program. See description of PUT for more details.");
	m_mapFuncs[_T("GAS(\"gas\")")]                   = _T("Moles of a gas component in the gas phase.");
	m_mapFuncs[_T("GET( i1 [ , i2, ... ])")]         = _T("Retrieves the value that is identified by the list of one or more subscripts.Value is zero if PUT has not been used to store a value for the set of subscripts. Values stored in global storage with PUT are accessible by any Basic program. See description of PUT for more details.");
	m_mapFuncs[_T("KIN(\"reactant\")")]              = _T("Moles of a kinetic reactant.");
	m_mapFuncs[_T("LA(\"species\")")]                = _T("Log10 of activity of an aqueous, exchange, or surface species.");
	m_mapFuncs[_T("LM(\"species\")")]                = _T("Log10 of molality of an aqueous, exchange, or surface species.");
	m_mapFuncs[_T("M")]                              = _T("Current moles of reactant for which the rate is being calculated (see KINETICS).");
	m_mapFuncs[_T("M0")]                             = _T("Initial moles of reactant for which the rate is being calculated (see KINETICS).");
	m_mapFuncs[_T("MISC1(\"component\")")]           = _T("Mole fraction of component 2 at the beginning of the miscibility gap, returns 1.0 if there is no miscibility gap (see SOLID_SOLUTIONS).");
	m_mapFuncs[_T("MISC2(\"component\")")]           = _T("Mole fraction of component 2 at the end of the miscibility gap, returns 1.0 if there is no miscibility gap (see SOLID_SOLUTIONS).");
	m_mapFuncs[_T("MOL(\"species\")")]               = _T("Molality of an aqueous, exchange, or surface species.");
	m_mapFuncs[_T("MU")]                             = _T("Ionic strength of the solution.");
	m_mapFuncs[_T("PARM( i )")]                      = _T("Parameter array defined in KINETICS data block.");
	m_mapFuncs[_T("PERCENT_ERROR")]                  = _T("Percent charge-balance error [100(cations-|anions|)/(cations + |anions|)].");
	m_mapFuncs[_T("PRINT")]                          = _T("Write to output file.");
	m_mapFuncs[_T("PUNCH")]                          = _T("Write to selected-output file.");
	m_mapFuncs[_T("PUT( x , i1 [ , i2, ... ])")]     = _T("Saves value of x in global storage that is identified by a sequence of one or more subscripts. Value of x can be retrieved with GET( i1, [ , i2, ... ]) and a set of subscripts can be tested to determine if a value has been stored with EXISTS( i1 [ , i2, ... ]). PUT may be used in RATES, USER_PRINT, or USER_PUNCH Basic programs to store a value. The value may be retrieved by any of these Basic programs. The value persists until overwritten using a PUT statement with the same set of subscripts, or until the end of the run. For a KINETICS data block, the Basic programs for the rate expressions are evaluated in the order in which they are defined in the input file.");
	m_mapFuncs[_T("RXN")]                            = _T("Amount of reaction (moles) as defined in -steps in REACTION data block for a batch-reaction calculation, otherwise zero.");
	m_mapFuncs[_T("SAVE")]                           = _T("Last statement of Basic program that returns the moles of kinetic reactant, counted positive when the solution concentration of the reactant increases.");
	m_mapFuncs[_T("SI(\"phase\")")]                  = _T("Saturation index of a phase");
	m_mapFuncs[_T("SIM_NO")]                         = _T("Simulation number, equals one more than the number of END statements before current simulation.");
	m_mapFuncs[_T("SIM_TIME")]                       = _T("Time (s) from the beginning of a kinetic batch-reaction or transport calculation.");
	m_mapFuncs[_T("SR(\"phase\")")]                  = _T("Saturation ratio of a phase.");
	m_mapFuncs[_T("STEP_NO")]                        = _T("Step number in batch-reaction calculations, or shift number in ADVECTION and TRANSPORT calculations.");
	m_mapFuncs[_T("S_S(\"component\")")]             = _T("Current moles of a solid-solution component.");
	m_mapFuncs[_T("TC")]                             = _T("Temperature in Celsius.");
	m_mapFuncs[_T("TK")]                             = _T("Temperature in Kelvin.");
	m_mapFuncs[_T("TIME")]                           = _T("Time interval for which moles of reaction are calculated in rate programs, automatically set in the time-step algorithm of the numerical integration method.");
	m_mapFuncs[_T("TOT(\"element\")")]               = _T("Total molality of element or element redox state. TOT(\"water\") is total mass of water (kg).");
	m_mapFuncs[_T("TOTAL_TIME")]                     = _T("Cumulative time (s) including all advective (for which -time_step is defined) and advective-dispersive transport simulations from the beginning of the run or from last -initial_time identifier.");
	m_mapFuncs[_T("EDL(\"element\", \"surface\")")]  = _T("Gives the amount of \"element\" in the diffuse layer for \"surface.\"  \"surface\" should be the surface name, not the surface-site name (that is, no underscore).");
	m_mapFuncs[_T("SURF(\"element\", \"surface\")")] = _T("Gives the amount of \"element\" sorbed to \"surface.\"  \"surface\" should be the surface name, not the surface-site name (that is, no underscore).");

	//{{
	m_mapFuncs[_T("LK_SPECIES(\"species\")")]                = _T("log10(K) of definition in (SOLUTION, EXCHANGE, SURFACE)_SPECIES");
	m_mapFuncs[_T("LK_NAMED(\"name\")")]                     = _T("log10(K) of definition in NAMED_EXPRESSIONS");
	m_mapFuncs[_T("LK_PHASE(\"phase\")")]                    = _T("log10(K) of definition in PHASES");
	m_mapFuncs[_T("SUM_SPECIES(\"template\", \"element\")")] = _T("Sum of element in aqueous, exchange, and surface species with specified template");
	m_mapFuncs[_T("SUM_GAS(\"template\", \"element\")")]     = _T("Sum of element in gases with specified template (for example template=\"{C,[13C],[14C]}{O,[18O]}2\" includes all CO2 gases)");
	m_mapFuncs[_T("SUM_s_s(\"s_s_name\", \"element\")")]     = _T("Sum of element in a specified solid solution");
	m_mapFuncs[_T("CALC_VALUE(\"calc_value_name\")")]        = _T("Evaluates a definition of CALCULATE_VALUES.");
	//}}

	//{{ 2.9 added functions
	m_mapFuncs[_T("PAD(a$, n)")]                     = _T("Pads a$ to a total of n characters with spaces. PAD returns a copy of a$ if a$ is more than 20 characters.");
	m_mapFuncs[_T("INSTR(a$, b$)")]                  = _T("Returns the character position of string b$ in a$, 0 if not found.");
	m_mapFuncs[_T("LTRIM(a$)")]                      = _T("Trims white space from beginning of string a$ and returns the result.");
	m_mapFuncs[_T("RTRIM(a$)")]                      = _T("Trims white space from end of string a$ and returns the result.");
	m_mapFuncs[_T("TRIM(a$)")]                       = _T("Trims white space from beginning and end of string a$ and returns the result.");
	m_mapFuncs[_T("DESCRIPTION")]                    = _T("The value defined for the description field of the SOLUTION keyword line.");

	// sys functions
	m_mapFuncs[_T("SYS(\"element\" [ , count_species, names$, types$, moles])")] =
		_T("Returns the total amount of element in the system;")
		_T(" count_species -- the number of species that contain element, including solution, equilibrium_phases, surfaces, exchangers, solid solutions, and gas phase species;")
		_T(" names$ -- a character array that has the name of each species;")
		_T(" type$ -- a character array that specifies the type of phase for the species, aq, equi, surf, ex, s_s, gas, diff.  Diff refers to the amount of the element in the diffuse layer of a surface when the explicit diffuse layer calculation is used;")
		_T(" moles -- an array containing the number of moles of the element in the species.");


	m_mapFuncs[_T("SYS(\"elements\" [ , count, names$, types$, values])")] = 
		_T("Returns total number of moles of dissolved elements other than H and O.")
		_T(" count is number of elements, valence states, exchangers, and surfaces.")
		_T(" Arrays are filled for each element and valence state, type is \"dis\"; exchanger, type is \"ex\", and surface, type is \"surf\". Values are moles.");

	m_mapFuncs[_T("SYS(\"phases\" [ , count, names$, types$, values])")] = 
		_T("Returns maximum saturation index of all phases.")
		_T(" count is number of phases in system.")
		_T(" Arrays are filled for each phase; values are saturation indexes.");

	m_mapFuncs[_T("SYS(\"aq\" [ , count, names$, types$, values])")] = 
		_T("Returns sum of moles of all aqueous species.")
		_T(" count is number of aqueous species in system.")
		_T(" Arrays are filled with each aqueous species; values are moles.");
	

	m_mapFuncs[_T("SYS(\"ex\" [ , count, names$, types$, values])")] = 
		_T("Returns sum of moles of all exchange species.")
		_T(" count is number of exchange species in system.")
		_T(" Arrays are filled with each exchange species; values are moles.");

	m_mapFuncs[_T("SYS(\"surf\" [ , count, names$, types$, values])")] = 
		_T("Returns sum of moles of all surface species.")
		_T(" count is number of surface species in system.")
		_T(" Arrays are filled with each surface species; values are moles.");


	m_mapFuncs[_T("SYS(\"s_s\" [ , count, names$, types$, values])")] = 
		_T("Returns sum of moles of solid solution components.")
		_T(" count is number of solid solution components in system.")
		_T(" Arrays are filled with each solid solution component; values are moles.");

	m_mapFuncs[_T("SYS(\"gas\" [ , count, names$, types$, values])")] = 
		_T("Returns sum of moles of all gas components.")
		_T(" count is number of gas components in system.")
		_T(" Arrays are filled with each gas component; values are moles.");
	//}} 2.9 added functions
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
			if (str == _T("ACT(\"species\")") || str == _T("LA(\"species\")") || str == _T("LM(\"species\")") || str == _T("MOL(\"species\")") || str == _T("LK_SPECIES(\"species\")"))
			{
				if (!(m_strPrev == _T("ACT(\"species\")") || m_strPrev == _T("LA(\"species\")") || m_strPrev == _T("LM(\"species\")") || m_strPrev == _T("MOL(\"species\")")))
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
			else if (str == _T("EQUI(\"phase\")") || str == _T("SI(\"phase\")") || str == _T("SR(\"phase\")") || str == _T("LK_PHASE(\"phase\")"))
			{
				if (!(m_strPrev == _T("EQUI(\"phase\")") || m_strPrev == _T("SI(\"phase\")") || m_strPrev == _T("SR(\"phase\")")))
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
			else if (str == _T("GAS(\"gas\")"))
			{
				if (!(m_strPrev == _T("GAS(\"gas\")")))
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
			else if (str == _T("TOT(\"element\")") || str == _T("SUM_SPECIES(\"template\", \"element\")") || str == _T("SUM_GAS(\"template\", \"element\")")
				|| str == _T("SYS(\"element\" [ , count_species, names$, types$, moles])")
				)
			{
				if (!(m_strPrev == _T("TOT(\"element\")") || m_strPrev == _T("SUM_SPECIES(\"template\", \"element\")") || m_strPrev == _T("SUM_GAS(\"template\", \"element\")"))
					|| m_strPrev == _T("SYS(\"element\" [ , count_species, names$, types$, moles])")
					)
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
			else if (str == _T("KIN(\"reactant\")"))
			{
				if (!(m_strPrev == _T("KIN(\"reactant\")")))
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
			else if (str == _T("MISC1(\"component\")") || str == _T("MISC2(\"component\")"))
			{
				if (!(m_strPrev == _T("MISC1(\"component\")") || m_strPrev == _T("MISC2(\"component\")")))
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
			else if (str == _T("S_S(\"component\")"))
			{
				if (!(m_strPrev == _T("S_S(\"component\")")))
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
			else if (str == _T("EDL(\"element\", \"surface\")") || str == _T("SURF(\"element\", \"surface\")"))
			{
				if (!(m_strPrev == _T("EDL(\"element\", \"surface\")") || m_strPrev == _T("SURF(\"element\", \"surface\")")))
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
			else if (str == _T("LK_NAMED(\"name\")"))
			{
				if (!(m_strPrev == _T("LK_NAMED(\"name\")")))
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
			else if (str == _T("CALC_VALUE(\"calc_value_name\")"))
			{
				if (!(m_strPrev == _T("CALC_VALUE(\"calc_value_name\")")))
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
			else if (str == _T("SUM_s_s(\"s_s_name\", \"element\")"))
			{
				if (!(m_strPrev == _T("SUM_s_s(\"s_s_name\", \"element\")")))
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
