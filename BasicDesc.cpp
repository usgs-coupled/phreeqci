// BasicDesc.cpp: implementation of the CBasicDesc class.
//
// $Date: 2001/09/14 20:11:58 $
// $Revision: 1.1.1.26 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "BasicDesc.h"
#include "Util.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBasicDesc::CBasicDesc(const CDatabase& rDatabase, int nIDFuncs, int nIDExplan, int nIDArgs)
: m_rDatabase(rDatabase), m_nIDFuncs(nIDFuncs), m_nIDExplan(nIDExplan), m_nIDArgs(nIDArgs)
{
	ASSERT(nIDFuncs != 0);
	ASSERT(nIDExplan != 0);
	ASSERT(nIDArgs != 0);
	m_bMapLoaded = false;
}

CBasicDesc::~CBasicDesc()
{

}

void CBasicDesc::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, m_nIDFuncs, m_lbFuncs);
	DDX_Control(pDX, m_nIDArgs, m_lbArgs);
	DDX_Control(pDX, m_nIDExplan, m_eExplan);
	if (!m_bMapLoaded)
	{
		LoadMap();
		m_bMapLoaded = true;
		FillFuncs();
	}
}

void CBasicDesc::LoadMap()
{
	ASSERT(m_bMapLoaded == false);
	ASSERT(m_mapFuncs.size() == 0);

	m_mapFuncs[_T("ACT(\"\")")]                  = _T("Activity of an aqueous, exchange, or surface species.");
	m_mapFuncs[_T("ALK")]                        = _T("Alkalinity of solution.");
	m_mapFuncs[_T("CELL_NO")]                    = _T("Cell number in TRANSPORT or ADVECTION calculations");
	m_mapFuncs[_T("CHARGE_BALANCE")]             = _T("Aqueous charge balance in equivalents.");
	m_mapFuncs[_T("DIST")]                       = _T("Distance to midpoint of cell in TRANSPORT calculations, cell number in ADVECTION calculations, \"-99\" in all other calculations.");
	m_mapFuncs[_T("EQUI(\"\")")]                 = _T("Moles of a phase in the pure-phase (equilibrium-phase) assemblage.");
	m_mapFuncs[_T("EXISTS( i1 [ , i2, ... ])")]  = _T("Determines if a value has been stored with a PUT statement for the list of one or more subscripts.The function equals 1 if a value has been stored and 0 if no value has been stored. Values are stored in global storage with PUT and are accessible by any Basic program. See description of PUT for more details.");
	m_mapFuncs[_T("GAS(\"\")")]                  = _T("Moles of a gas component in the gas phase.");
	m_mapFuncs[_T("GET( i1 [ , i2, ... ])")]     = _T("Retrieves the value that is identified by the list of one or more subscripts.Value is zero if PUT has not been used to store a value for the set of subscripts. Values stored in global storage with PUT are accessible by any Basic program. See description of PUT for more details.");
	m_mapFuncs[_T("KIN(\"\")")]                  = _T("Moles of a kinetic reactant.");
	m_mapFuncs[_T("LA(\"\")")]                   = _T("Log10 of activity of an aqueous, exchange, or surface species.");
	m_mapFuncs[_T("LM(\"\")")]                   = _T("Log10 of molality of an aqueous, exchange, or surface species.");
	m_mapFuncs[_T("M")]                          = _T("Current moles of reactant for which the rate is being calculated (see KINETICS).");
	m_mapFuncs[_T("M0")]                         = _T("Initial moles of reactant for which the rate is being calculated (see KINETICS).");
	m_mapFuncs[_T("MISC1(\"\")")]                = _T("Mole fraction of component 2 at the beginning of the miscibility gap, returns 1.0 if there is no miscibility gap (see SOLID_SOLUTIONS).");
	m_mapFuncs[_T("MISC2(\"\")")]                = _T("Mole fraction of component 2 at the end of the miscibility gap, returns 1.0 if there is no miscibility gap (see SOLID_SOLUTIONS).");
	m_mapFuncs[_T("MOL(\"\")")]                  = _T("Molality of an aqueous, exchange, or surface species.");
	m_mapFuncs[_T("MU")]                         = _T("Ionic strength of the solution.");
	m_mapFuncs[_T("PARM( i )")]                  = _T("Parameter array defined in KINETICS data block.");
	m_mapFuncs[_T("PERCENT_ERROR")]              = _T("Percent charge-balance error [100(cations-|anions|)/(cations + |anions|)].");
	m_mapFuncs[_T("PRINT")]                      = _T("Write to output file.");
	m_mapFuncs[_T("PUNCH")]                      = _T("Write to selected-output file.");
	m_mapFuncs[_T("PUT( x , i1 [ , i2, ... ])")] = _T("Saves value of x in global storage that is identified by a sequence of one or more subscripts. Value of x can be retrieved with GET( i1, [ , i2, ... ]) and a set of subscripts can be tested to determine if a value has been stored with EXISTS( i1 [ , i2, ... ]). PUT may be used in RATES, USER_PRINT, or USER_PUNCH Basic programs to store a value. The value may be retrieved by any of these Basic programs. The value persists until overwritten using a PUT statement with the same set of subscripts, or until the end of the run. For a KINETICS data block, the Basic programs for the rate expressions are evaluated in the order in which they are defined in the input file.");
	m_mapFuncs[_T("RXN")]                        = _T("Amount of reaction (moles) as defined in -steps in REACTION data block for a batch-reaction calculation, otherwise zero.");
	m_mapFuncs[_T("SAVE")]                       = _T("Last statement of Basic program that returns the moles of kinetic reactant, counted positive when the solution concentration of the reactant increases.");
	m_mapFuncs[_T("SI(\"\")")]                   = _T("Saturation index of a phase");
	m_mapFuncs[_T("SIM_NO")]                     = _T("Simulation number, equals one more than the number of END statements before current simulation.");
	m_mapFuncs[_T("SIM_TIME")]                   = _T("Time (s) from the beginning of a kinetic batch-reaction or transport calculation.");
	m_mapFuncs[_T("SR(\"\")")]                   = _T("Saturation ratio of a phase.");
	m_mapFuncs[_T("STEP_NO")]                    = _T("Step number in batch-reaction calculations, or shift number in ADVECTION and TRANSPORT calculations.");
	m_mapFuncs[_T("S_S(\"\")")]                  = _T("Current moles of a solid-solution component.");
	m_mapFuncs[_T("TC")]                         = _T("Temperature in Celsius.");
	m_mapFuncs[_T("TK")]                         = _T("Temperature in Kelvin.");
	m_mapFuncs[_T("TIME")]                       = _T("Time interval for which moles of reaction are calculated in rate programs, automatically set in the time-step algorithm of the numerical integration method.");
	m_mapFuncs[_T("TOT(\"\")")]                  = _T("Total molality of element or element redox state. TOT(\"water\") is total mass of water (kg).");
	m_mapFuncs[_T("TOTAL_TIME")]                 = _T("Cumulative time (s) including all advective (for which -time_step is defined) and advective-dispersive transport simulations from the beginning of the run or from last -initial_time identifier.");

	m_mapFuncs[_T("EDL(\"element\", \"surface\")")]   = _T("Gives the amount of \"element\" in the diffuse layer for \"surface.\"  \"surface\" should be the surface name, not the surface-site name (that is, no underscore).");
	m_mapFuncs[_T("SURF(\"element\", \"surface\")")]  = _T("Gives the amount of \"element\" sorbed to \"surface.\"  \"surface\" should be the surface name, not the surface-site name (that is, no underscore).");
}

void CBasicDesc::FillFuncs()
{
	// fill list box
	std::map<CString, CString>::iterator funcs = m_mapFuncs.begin();
	for (; funcs != m_mapFuncs.end(); ++funcs)
	{
		m_lbFuncs.AddString(funcs->first);
	}
	//m_lbFuncs.SetSel(0);

	// select first func if exiss
	funcs = m_mapFuncs.begin();
	if (funcs != m_mapFuncs.end())
	{
		m_lbFuncs.SelectString(-1, funcs->first);
		// m_eExplan.SetWindowText(funcs->second);
	}
	OnSelchangeLbFuncs();
}

void CBasicDesc::OnSelchangeLbFuncs() 
{
	int nIndex = m_lbFuncs.GetCurSel();

	if (nIndex != LB_ERR)
	{
		CString str;
		m_lbFuncs.GetText(nIndex, str);

		std::map<CString, CString>::iterator find = m_mapFuncs.find(str);

		if (find != m_mapFuncs.end())
		{
			m_eExplan.SetWindowText(find->second);
			if (str == _T("ACT(\"\")") || str == _T("LA(\"\")") || str == _T("LM(\"\")") || str == _T("MOL(\"\")"))
			{
				CUtil::InsertAqSpecies(&m_lbArgs, m_rDatabase);
			}
			else if (str == _T("EQUI(\"\")") || str == _T("SI(\"\")") || str == _T("SR(\"\")"))
			{
				CUtil::InsertPhases(&m_lbArgs, m_rDatabase);
			}
			else if (str == _T("GAS(\"\")"))
			{
				CUtil::InsertGases(&m_lbArgs, m_rDatabase);
			}
			else if (str == _T("TOT(\"\")"))
			{
				CUtil::InsertAqElements(&m_lbArgs, m_rDatabase);
				m_lbArgs.AddString(_T("water"));
			}
			else if (str == _T("KIN(\"\")"))
			{
				CUtil::InsertRates(&m_lbArgs, m_rDatabase);
			}
			else if (str == _T("MISC1(\"\")") || str == _T("MISC2(\"\")"))
			{
				CUtil::InsertSolidSolutions(&m_lbArgs, m_rDatabase);
			}
			else if (str == _T("S_S(\"\")"))
			{
				CUtil::InsertSolidSolutionComps(&m_lbArgs, m_rDatabase);
			}
			else
			{
				m_lbArgs.ResetContent();
			}
		}
		else
		{
			m_eExplan.SetWindowText(_T(""));
			m_lbArgs.ResetContent();
		}
	}

}
