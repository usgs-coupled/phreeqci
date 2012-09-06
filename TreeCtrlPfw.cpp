// TreeCtrlPfw.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "TreeCtrlPfw.h"

#include "RichViewIn.h"

// Start
std::pair<CString, CString> CTreeCtrlPfw::COPY(
	"COPY solution 5 1-10 # copies solution 5 to solution 1 to 10"
	,
	"Copies a data entity, at the end of the simulation, to a new index number or a range of index numbers. Data entities can be: SOLUTION, EQUILIBRIUM_PHASES, EXCHANGE, GAS_PHASE, KINETICS, MIX, REACTION, REACTION_TEMPERATURE, SOLID_SOLUTION, SURFACE\n"
	"\n"
	"Example:\n"
	"COPY solution 5 1-10\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::DATABASE(
	"DATABASE wateq4f.dat"
	,
	"Specifies the database to be used. It must be the first keyword in the input file.\n"
	"\n"
	"Example:\n"
	"DATABASE wateq4f.dat\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::END(
	"END"
	,
	"Ends the data input for a simulation and signals PHREEQC to start calculating.\n"
	"\n"
	"Example:\n"
	"END\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SAVE(
	"SAVE solution 1-10"
	,
	"Saves, at the end of the simulation, the solution, exchange, gas phase, pure-phase, solid-solution, or surface to a new index number or a range of cells.\n"
	"\n"
	"Example:\n"
	"SAVE solution 1-10\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION(
	"SOLUTION 1-10 # a number or a range of numbers. Default: 1.\n"
	"pH 6.05 # Default: 7.\n"
	"pe 14.8 # Default: 4.\n"
	"-units mg/L # Default: mmol/kgw\n"
	"Na 1 mmol/L # Chemical symbol from the 1st column in SOLUTION_MASTER_SPECIES, concentration, concentration is adapted to charge balance\n"
	"Cl 37 charge # concentration is adapted to charge balance\n"
	"C(4) 0.6 as HCO3 # Concentration is in mg of HCO3 = 0.6 / 0.61 = 0.98 mmol/L"
	,
	"Defines initial solutions.\n"
	"\n"
	"Example:\n"
	"SOLUTION 1-10\n"
	"pH 6.05\n"
	"pe 14.8\n"
	"-units mg/L\n"
	"Na 1 mmol/L\n"
	"Cl 37 charge\n"
	"C(4) 0.6 as HCO3\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_temp(
	"temp 10"
	,
	"temperature in Celsius. Default: 25\n"
	"\n"
	"Example:\n"
	"temp 10\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_pH(
	"pH 6.05 Calcite 0.2 # pH is adapted to attain Calcite equilibrium, SI = 0.2"
	,
	"Default: pH 7.\n"
	"\n"
	"Example:\n"
	"pH 6.05 Calcite 0.2\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_pe(
	"pe 12 O2(g) -0.68 # pe is adapted to equilibrium with oxygen gas, partial pressure = 10^-0.68 atm"
	,
	"Default: pe 4.\n"
	"\n"
	"Example:\n"
	"pe 12 O2(g) -0.68\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_redox(
	"redox O(-2)/O(0) # uses activities of H2O and O2 for pe"
	,
	"A redox couple is used to calculate pe.\n"
	"\n"
	"Example:\n"
	"redox O(-2)/O(0)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_units(
	"units mmol/L"
	,
	"Define units in (mol or g) / (L or kgw or kgs). Prefix m for milli or u for micro. ppm = mg/kgs, ppt = ug/kgs. Default: mmol/kgw.\n"
	"\n"
	"Example:\n"
	"units mmol/L\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_density(
	"density 1.03"
	,
	"Default: 1 kg/L.\n"
	"\n"
	"Example:\n"
	"density 1.03\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_isotope(
	"-isotope 13C -13 1.5 # name, value in permil, uncertainty in permil"
	,
	"Defines isotopic composition for an element.\n"
	"\n"
	"Example:\n"
	"-isotope 13C -13 1.5\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_water(
	"-water 0.1"
	,
	"Defines mass of water (kg). Default: 1 kg.\n"
	"\n"
	"Example:\n"
	"-water 0.1\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD(
	"SOLUTION_SPREAD\n"
	"units mg/l\n"
	"temp 11\n"
	"Alkalinity    Ca    Cl    S(6)    Temp    pH    Li\n"
	"mg/l as HCO3                        ug/l\n"
	"233.4    87.7    22.7    94.4    13    7.3\n"
	"221.7    78.6    35.3    87.4    13.1    7.1    4\n"
	"221.3    75.0    24.9    83.2        6.9\n"
	"# The default temperature of 11oC will be given to the third sample.\n"
	"# Li is in microgram/L"
	,
	"SOLUTION_SPREAD offers a matrix-type input format for SOLUTION.\n"
	"\n"
	"Example:\n"
	"SOLUTION_SPREAD\n"
	"units mg/l\n"
	"temp 11\n"
	"Alkalinity	Ca	Cl	S(6)	Temp	pH	Li\n"
	"mg/l as HCO3						ug/l\n"
	"233.4	87.7	22.7	94.4	13	7.3\n"
	"221.7	78.6	35.3	87.4	13.1	7.1	4\n"
	"221.3	75.0	24.9	83.2		6.9\n"
	"# The default temperature of 11oC will be given to the third sample.\n"
	"# Li is in microgram/L\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_temp(
	"temp 11"
	,
	"The temperature (Celsius) that is given to samples with temperature not defined. Default: 25 oC.\n"
	"\n"
	"Example:\n"
	"temp 11\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_pH(
	"pH 6"
	,
	"The pH that is given to samples with pH not defined. Default: 7.\n"
	"\n"
	"Example:\n"
	"pH 6\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_pe(
	"pe 10"
	,
	"The pe that is given to samples with pe not defined. Default: 4.\n"
	"\n"
	"Example:\n"
	"pe 10\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_redox(
	"redox O(-2)/O(0) # uses activities of H2O and O2 for pe"
	,
	"A redox couple is used to calculate pe.\n"
	"\n"
	"Example:\n"
	"redox O(-2)/O(0)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_units(
	"units mmol/L"
	,
	"Define units in (mol or g) / (L or kgw or kgs). Prefix m for milli, or u for micro. ppm = mg/kgs, ppt = ug/kgs. Default: mmol/kgw.\n"
	"\n"
	"Example:\n"
	"units mmol/L\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_density(
	"density 1.03"
	,
	"The density that is given to samples with density undefined. Default: 1 kg/L.\n"
	"\n"
	"Example:\n"
	"density 1.03\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_isotope(
	"-isotope 13C -13 # name, value in permil"
	,
	"Sets isotopic composition for an element if undefined.\n"
	"\n"
	"Example:\n"
	"-isotope 13C -13\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_isotope_uncertainty(
	"-isotope_uncertainty 13C 1.5"
	,
	"Sets uncertainty (permil) for an isotope in samples if undefined.\n"
	"\n"
	"Example:\n"
	"-isotope_uncertainty 13C 1.5\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_water(
	"-water 0.1"
	,
	"Sets mass of water in samples if undefined. Default: 1 kg.\n"
	"\n"
	"Example:\n"
	"-water 0.1\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TITLE(
	"TITLE"
	,
	"Adds a title to a simulation.\n"
	"\n"
	"Example:\n"
	"TITLE\n"
);
std::pair<CString, CString> CTreeCtrlPfw::USE(
	"USE solution 2\n"
	"#\n"
	"USE solution none # the reaction step is omitted"
	,
	"Specifies a solution, exchange, equilibrium_phases, solid_solutions, and surface to be used in the batch-reaction calculation.\n"
	"\n"
	"Example:\n"
	"USE solution 2\n"
	"#\n"
	"USE solution none\n"
);

// Chemical reaction
std::pair<CString, CString> CTreeCtrlPfw::EQUILIBRIUM_PHASES(
	"EQUILIBRIUM_PHASES 1-10 # cell number or a range of cells. Default: 1.\n"
	"Calcite 0.2 1e-4 # name (must be defined in PHASES), Saturation Index (Default: 0), initial amount (moles, default: 10 moles)\n"
	"Dolomite 0 1e-3 diss # Dolomite can dissolve only\n"
	"Quartz 0 0 precipitate # Quartz can precipitate only"
	,
	"Defines pure phases that can react reversibly with the solution, the saturation index and amount.\n"
	"\n"
	"Example:\n"
	"EQUILIBRIUM_PHASES 1-10\n"
	"Calcite 0.2 1e-4\n"
	"Dolomite 0 1e-3 diss\n"
	"Quartz 0 0 precipitate\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::EQUILIBRIUM_PHASES_force_equality(
	"-force_equality"
	,
	"Forces target SI's to be reached, or else the calculation fails with an error message. See release.txt for an example.\n"
	"\n"
	"Example:\n"
	"-force_equality\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE(
	"# Option 1: Exchangers X and Y in equilibrium with solution 5\n"
	"EXCHANGE 1-10 # cell number or a range of cells. Default: 1.\n"
	"X   1.1 # name of EXCHANGE_MASTER_SPECIES, moles.\n"
	"Y   0.1\n"
	"-equilibrate 5 # initial composition by equilibrium with solution number 5.\n"
	"# Option 2: Exchanger composition is explicitly entered\n"
	"EXCHANGE 11 # measured exchange composition\n"
	"CaX2 0.3 # name of EXCHANGE_SPECIES, moles.\n"
	"NaX 0.5 # Example has 0.3 moles Ca, 0.5 moles Na, 2 * 0.3 + 0.5 = 1.1 moles X.\n"
	"# Option 3: Exchanger Mm is related to equilibrium_phase\n"
	"EXCHANGE 12	# Exchanger Mm is related to Montmorillonite\n"
	"Mm Montmorillonite equilibrium_phase 0.165 # name of EXCHANGE_SPECIES, name from PHASES, equi, exchange stoichiometry (mol Mm / mol montmorillonite).\n"
	"# Option 4: Exchanger Mm is related to kinetic reactant\n"
	"EXCHANGE 13 # Exchanger Mm is related to kinetic reactant Montmorillonite\n"
	"Mm Montmorillonite kinetic_reactant 0.165 # name of EXCHANGE_SPECIES, name from RATES, kin, exchange stoichiometry (mol Mm / mol montmorillonite)."
	,
	"Defines the amount and composition of exchangers.\n"
	"\n"
	"Example:\n"
	"# Option 1: Exchangers X and Y in equilibrium with solution 5\n"
	"EXCHANGE 1-10\n"
	"X   1.1\n"
	"Y   0.1\n"
	"-equilibrate 5\n"
	"# Option 2: Exchanger composition is explicitly entered\n"
	"EXCHANGE 11\n"
	"CaX2 0.3\n"
	"NaX 0.5\n"
	"# Option 3: Exchanger Mm is related to equilibrium_phase\n"
	"EXCHANGE 12\n"
	"Mm Montmorillonite equilibrium_phase 0.165\n"
	"# Option 4: Exchanger Mm is related to kinetic reactant\n"
	"EXCHANGE 13\n"
	"Mm Montmorillonite kinetic_reactant 0.165\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_equilibrate(
	"-equilibrate 5 # equilibrate with solution number 5."
	,
	"The initial exchange assemblage is defined by equilibrium with a solution.\n"
	"\n"
	"Example:\n"
	"-equilibrate 5\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_pitzer_exchange_gammas(
	"-pitzer_exchange_gammas false"
	,
	"In case activities are calculated with Pitzer: the activity coefficients for exchange species are equal to the aqueous activity coefficients if true, or to 1.0 if false. Default: true.\n"
	"\n"
	"Example:\n"
	"-pitzer_exchange_gammas false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::GAS_PHASE(
	"# Option 1: Fixed pressure gas phase\n"
	"GAS_PHASE 1-10 # cell number or a range of cells. Default: 1.\n"
	"-fixed_pressure # the gas_phase is a bubble with a maximal pressure and a variable volume (Default option).\n"
	"-pressure 1.0 # the gas bubble forms if the pressure is 1.0 atm or higher.\n"
	"H2O(g) 0.0 # name (must be defined in PHASES), initial partial pressure (atm).\n"
	"CO2(g) 1.1\n"
	"# Option 2: Fixed volume gas phase\n"
	"GAS_PHASE 2\n"
	"-fixed_volume # the gas_phase has a fixed volume with variable pressure.\n"
	"-volume 0.5	# the gas volume is 0.5 L.\n"
	"H2O(g) 0.0\n"
	"CO2(g) 0.0\n"
	"-equilibrate 2 # initial composition by equilibrium with solution 2."
	,
	"Defines the initial composition of a fixed-total-pressure or a fixed-volume multicomponent gas phase for cells.\n"
	"\n"
	"Example:\n"
	"# Option 1: Fixed pressure gas phase\n"
	"GAS_PHASE 1-10\n"
	"-fixed_pressure\n"
	"-pressure 1.0\n"
	"H2O(g) 0.0\n"
	"CO2(g) 1.1\n"
	"# Option 2: Fixed volume gas phase\n"
	"GAS_PHASE 2\n"
	"-fixed_volume\n"
	"-volume 0.5\n"
	"H2O(g) 0.0\n"
	"CO2(g) 0.0\n"
	"-equilibrate 2\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::GAS_PHASE_fixed_pressure(
	"-fixed_pressure"
	,
	"Defines the gas phase to have a fixed total pressure (a gas bubble), volume varies.\n"
	"\n"
	"Example:\n"
	"-fixed_pressure\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::GAS_PHASE_pressure(
	"-pressure 1.5 # The pressure is 1.5 atm at most. If the sum of the pressures of the component gases is less than 1.5 atm, the gas disappears."
	,
	"Sets the pressure of the gas phase to a fixed value (atm) that applies during all batch-reaction and transport calculations. Default: 1.0 atm\n"
	"\n"
	"Example:\n"
	"-pressure 1.5\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::GAS_PHASE_volume(
	"-volume 0.6 # volume is 0.6 L."
	,
	"Defines the volume (L) of the gas phase. Default: 1.0 L. Volume is the initial volume for a fixed_pressure gas. Volume is the fixed volume for a fixed_volume gas.\n"
	"\n"
	"Example:\n"
	"-volume 0.6\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::GAS_PHASE_temperature(
	"-temperature 20"
	,
	"Sets the initial temperature of the gas phase (degrees C). Default: 25 oC.\n"
	"\n"
	"Example:\n"
	"-temperature 20\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::GAS_PHASE_fixed_volume(
	"-fixed_volume"
	,
	"Defines the gas phase to have a fixed volume.\n"
	"\n"
	"Example:\n"
	"-fixed_volume\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::GAS_PHASE_equilibrate(
	"-equilibrate 5 # equilibrate with solution number 5."
	,
	"The initial gas phase is defined by equilibrium with a solution. Only for fixed_volume gas_phase.\n"
	"\n"
	"Example:\n"
	"-equilibrate 5\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::INCREMENTAL_REACTIONS(
	"INCREMENTAL_REACTIONS true"
	,
	"If FALSE, reactions are added stepwise to the composition of step 0 (Default). If TRUE, reactions are added stepwise to the composition of the previous step.\n"
	"\n"
	"Example:\n"
	"INCREMENTAL_REACTIONS true\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING(
	"INVERSE_MODELING\n"
	"-solutions 1 3 2 # Balance solution 2 from 1 and 3\n"
	"-phases\n"
	"Gypsum # Using mineral Gypsum (must be defined in PHASES)\n"
	"-balances # Any element not in phases\n"
	"Cl\n"
	"Na\n"
	"Alkalinity 1 # Permitted uncertainty for Alkalinity is 100%"
	,
	"Provides information for an inverse modeling calculation.\n"
	"\n"
	"Example:\n"
	"INVERSE_MODELING\n"
	"-solutions 1 3 2\n"
	"-phases\n"
	"Gypsum\n"
	"-balances\n"
	"Cl\n"
	"Na\n"
	"Alkalinity 1\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_solutions(
	"-solutions 1 3 2 # Balance solution 2 from 1 and 3"
	,
	"A list of solution numbers follows on the same line. Last solution is balanced by mixing previous ones.\n"
	"\n"
	"Example:\n"
	"-solutions 1 3 2\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_uncertainty(
	"-solutions 1 3 2\n"
	"-uncertainty 0.05 0.1 0.04 # 5% uncertainty for solution 1, 10% for 3, 4% for 2."
	,
	"Sets the uncertainty limits for each solution.\n"
	"\n"
	"Example:\n"
	"-solutions 1 3 2\n"
	"-uncertainty 0.05 0.1 0.04\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_phases(
	"-phases\n"
	"Gypsum\n"
	"Calcite # Using minerals Gypsum and Calcite (must be defined in PHASES)"
	,
	"Indicates the phases to be used in inverse modeling.\n"
	"\n"
	"Example:\n"
	"-phases\n"
	"Gypsum\n"
	"Calcite\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_balances(
	"-balances\n"
	"Cl\n"
	"Na\n"
	"Alkalinity 1 # Permitted uncertainty for Alkalinity is 100%"
	,
	"Any element not present in the phases to be used.\n"
	"\n"
	"Example:\n"
	"-balances\n"
	"Cl\n"
	"Na\n"
	"Alkalinity 1\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_isotopes(
	"-isotope\n"
	"13C"
	,
	"Specifies the isotopes to be used in the inverse model.\n"
	"\n"
	"Example:\n"
	"-isotope\n"
	"13C\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_range(
	"-range 1000 # consult the PHREEQC manual, p. 101"
	,
	"Specifies whether ranges in mole transfer for each phase should be calculated. Default: not done.\n"
	"\n"
	"Example:\n"
	"-range 1000\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_minimal(
	"-minimal"
	,
	"Specifies that models should be reduced to only the minimum number of phases. Default: not set.\n"
	"\n"
	"Example:\n"
	"-minimal\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_tolerance(
	"-tolerance 1e-10 # numbers smaller than 1e-10 are taken as 0."
	,
	"Sets the smallest non-zero number for the optimizing solver.\n"
	"\n"
	"Example:\n"
	"-tolerance 1e-10\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_force_solutions(
	"-solutions 1 3 2\n"
	"-force_solutions true false # solution 1 is always included."
	,
	"One or more solutions can be forced to be included in all range calculations.\n"
	"\n"
	"Example:\n"
	"-solutions 1 3 2\n"
	"-force_solutions true false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_uncertainty_water(
	"-uncertainty_water 2.5 # uncertainty is 2.5 moles"
	,
	"Sets the uncertainty (moles) for the water-balance equation. Default: 0.0 moles\n"
	"\n"
	"Example:\n"
	"-uncertainty_water 2.5\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_mineral_water(
	"-mineral_water false"
	,
	"If TRUE, water from minerals is included in the water-balance equation (default). If FALSE, water from minerals is excluded.\n"
	"\n"
	"Example:\n"
	"-mineral_water false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_multiple_precision(
	"-multiple_precision false # true possible in unix only"
	,
	"Signals use of the multiple precision version of routine cl1. Default: false.\n"
	"\n"
	"Example:\n"
	"-multiple_precision false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_mp_tolerance(
	"# -mp_tolerance 1e-14 # possible in unix only"
	,
	"Sets the smallest non-zero number for the mp solver. Default: 1e-12.\n"
	"\n"
	"Example:\n"
	"# -mp_tolerance 1e-14 # possible in unix only\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_censor_mp(
	"# -censor_mp 1e-20 # possible in unix only"
	,
	"Sets smallest non-zero number in the the linear equation array. Default: 1e-20\n"
	"\n"
	"Example:\n"
	"# -censor_mp 1e-20 # possible in unix only\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_lon_netpath(
	"-lon_netpath filename # Writes solution definitions to file called filename.lon"
	,
	"Writes current solution definitions in PHREEQC to a Netpath .lon file in a format readable by DBXL (NetpathXL distribution).\n"
	"\n"
	"Example:\n"
	"-lon_netpath filename\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_pat_netpath(
	"-pat_netpath filename # Writes inverse models to a sequence of files called filename-n.mod, where n is the model number. Also creates filename.pat."
	,
	"Creates (i) a NETPATH model file for each inverse model that is found and (ii) a file that contains the solutions associated with each model.\n"
	"\n"
	"Example:\n"
	"-pat_netpath filename\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::REACTION(
	"REACTION 1-10 # cell number or a range of cells. Default: 1.\n"
	"CaCO3 1.2 # Reactant, followed by  stoichiometric coefficient\n"
	"1e-3 in 2 steps # Adds 1.2e-3 mol Ca and C, 3.6e-3 mol O in 2 equal steps"
	,
	"Defines irreversible reactions that transfer elements to or from the aqueous solution.\n"
	"\n"
	"Example:\n"
	"REACTION 1-10\n"
	"CaCO3 1.2\n"
	"1e-3 in 2 steps\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::REACTION_TEMPERATURE(
	"REACTION_TEMPERATURE 1-10 # cell number or a range of cells. Default: 1.\n"
	"10 50 90 # solution composition is calculated for 3 listed temperatures (C)\n"
	"10 90 in 2 steps # alternative for writing the steps"
	,
	"Defines temperatures during batch-reaction steps.\n"
	"\n"
	"Example:\n"
	"REACTION_TEMPERATURE 1-10\n"
	"10 50 90\n"
	"10 90 in 2 steps\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SURFACE(
	"# Option 1: Initial surface composition in equilibrium with solution 1\n"
	"SURFACE 1-10 # a number or a range of numbers. Default: 1\n"
	"Hfo_w  2.4e-3  600  1.06 Dw 1e-13\n"
	"# 2.4e-3 mol sites, sp. surface = 600 m2/g, gram ferrihydrite = 1.06, diffusion coefficient 1e-13 m^2/s is used with option -multi_d true in TRANSPORT: Hfo (both sites Hfo_w and Hfo_s) is a surface that is transported with advection and dispersion (Default Dw = 0 m2/s, surface is not transported).\n"
	"Hfo_s  6e-5 # 6e-5 mol sites\n"
	"-equilibrate  1 # equilibrate with solution 1\n"
	"-donnan 1e-012\n"
	"# Option 2: Amount of surface site is coupled to equilibrium phase\n"
	"SURFACE 2\n"
	"Hfo_w  Fe(OH)3(a) eq 0.2 5.34e4 # weak sites, coupled to Fe(OH)3(a) (ferrihydrite), proportionality constant = 0.2, sp. surface = 5.34e4 m2/mol"
	,
	"Defines the amount and composition of surfaces.\n"
	"\n"
	"Example:\n"
	"# Option 1: Initial surface composition in equilibrium with solution 1\n"
	"SURFACE 1-10\n"
	"Hfo_w  2.4e-3  600  1.06 Dw 1e-13\n"
	"# 2.4e-3 mol sites, sp. surface = 600 m2/g, gram ferrihydrite = 1.06, diffusion coefficient 1e-13 m^2/s is used with option -multi_d true in TRANSPORT: Hfo (both sites Hfo_w and Hfo_s) is a surface that is transported with advection and dispersion (Default Dw = 0 m2/s, surface is not transported).\n"
	"Hfo_s  6e-5\n"
	"-equilibrate  1\n"
	"# Option 2: Amount of surface site is coupled to equilibrium phase\n"
	"SURFACE 2\n"
	"Hfo_w  Fe(OH)3(a) eq 0.2 5.34e4\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SURFACE_equilibrate(
	"-equilibrate 5 # equilibrate with solution number 5"
	,
	"The initial surface assemblage is defined by equilibrium with a solution.\n"
	"\n"
	"Example:\n"
	"-equilibrate 5\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SURFACE_capacitance(
	"-capacitance 1.1 7 # 1.1 F/m2 for the 0-1 plane (Default: 1 F/m2), 7 F/m2 for the 1-2 plane (Default: 5 F/m2)"
	,
	"Defines the capacitances (F/m2) for the 0-1 and the 1-2 planes in the triple layer and cd_music models.\n"
	"\n"
	"Example:\n"
	"-capacitance 1.1 7\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SURFACE_cd_music(
	"-cd_music # requires cd_music parm's in SURFACE_SPECIES"
	,
	"The CD_MUSIC surface complexation model will be used. Default: not used\n"
	"\n"
	"Example:\n"
	"-cd_music\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SURFACE_diffuse_layer(
	"-diffuse_layer 1e-9 # the thickness of the DDL is 1e-9 m. Default: 1e-8 m"
	,
	"-diffuse_layer 1e-9 # the thickness of the DDL is 1e-9 m. Default: 1e-8 m\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SURFACE_donnan(
	"-donnan 1e-7 viscosity 0.5 	# the thickness of the DDL is 1e-7 m (Default: 1e-8 m), relative viscosity DDL / H2O is 0.5 (Default: 1)\n"
	"-donnan debye_lengths 1.5 limit_ddl 0.9 viscosity 1 	# the thickness of the DDL is 1.5 Debye-lengths (Default: 1), maximum fraction of water that can be in the diffuse layer is 0.9 (Default: 0.8), relative viscosity DDL / H2O is 1 (Default: 1)"
	,
	"Sets the thickness (m or Debye-lengths) of, the maximum fraction of water in, and the relative viscosity of the diffuse layer, and calculates its composition with the Donnan assumption. Default: calculation is not done.\n"
	"\n"
	"Example:\n"
	"-donnan 1e-7 viscosity 0.5 \n"
	"-donnan debye_lengths 1.5 limit_ddl 0.9 viscosity 1 \n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SURFACE_only_counter_ions(
	"-only_counter_ions"
	,
	"In combination with (1) -donnan: the co-ions (with the same sign of charge as the surface) are excluded from the DDL, or (2) with -diffuse_layer: the concentration of co-ions in the DDL and the free solution are the same.\n"
	"\n"
	"Example:\n"
	"-only_counter_ions\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SURFACE_no_edl(
	"-no_edl"
	,
	"Allows to omit the electrostatic term from the calculation. Default: electrostatics is done\n"
	"\n"
	"Example:\n"
	"-no_edl\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SURFACE_sites_units(
	"-sites_units density\n"
	"SurfOH  2.6    600.    1.0 # SurfOH has site density 2.6/nm2, specific area 600 m2/g, mass 1 gram. Or: 2.6 * 600 * 1 * 1e18 / 6.022e23 = 2.59 mmol sites"
	,
	"Allows the units (sites/nm^2) for defining the number of surface sites instead of moles.\n"
	"\n"
	"Example:\n"
	"-sites_units density\n"
	"SurfOH  2.6    600.    1.0\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ADVECTION(
	"ADVECTION\n"
	"-cells 10\n"
	"-shifts 5"
	,
	"ADVECTION\n"
	"-cells 10\n"
	"-shifts 5\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::ADVECTION_cells(
	"-cells 10"
	,
	"Sets number of cells in the column.\n"
	"\n"
	"Example:\n"
	"-cells 10\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::ADVECTION_shifts(
	"-shifts 5"
	,
	"Sets number of shifts or time steps in the advection simulation (shifts * time_step = total simulation time).\n"
	"\n"
	"Example:\n"
	"-shifts 5\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::ADVECTION_time_step(
	"-time_step 1.0e6"
	,
	"Sets length of time (s) of each advective shift. Default: 0 s.\n"
	"\n"
	"Example:\n"
	"-time_step 1.0e6\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::ADVECTION_initial_time(
	"-initial_time 1.0e5"
	,
	"Sets the starting time (s) of an advection simulation. Default: 0 s.\n"
	"\n"
	"Example:\n"
	"-initial_time 1.0e5\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::ADVECTION_print_cells(
	"-print_cells 3-5 7 # Results for cells 3, 4, 5, and 7 are printed."
	,
	"Selects the cells for which results will be written to the output file. Default: all cells.\n"
	"\n"
	"Example:\n"
	"-print_cells 3-5 7\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::ADVECTION_print_frequency(
	"-print_frequency 3 # Printing is done after each 3rd shift."
	,
	"Selects shifts for which results will be written to the output file. Default: all shifts.\n"
	"\n"
	"Example:\n"
	"-print_frequency 3\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::ADVECTION_punch_cells(
	"-punch_cells 3-5 7 # Results for cells 3, 4, 5, and 7 are punched/graphed."
	,
	"Selects cells for which results will be written to the selected-output file and graphed. Default: all cells.\n"
	"\n"
	"Example:\n"
	"-punch_cells 3-5 7\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::ADVECTION_punch_frequency(
	"-punch_frequency 3 # Punching/graphing is done after each 3rd shift."
	,
	"Selects shifts for which results will be written to the selected-output file and  graphed. Default: all shifts.\n"
	"\n"
	"Example:\n"
	"-punch_frequency 3\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::ADVECTION_warnings(
	"-warnings false # Warning messages are not printed."
	,
	"Enables/disables printing of warning messages during advection calculations. Default: true.\n"
	"\n"
	"Example:\n"
	"-warnings false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::MIX(
	"MIX 1 # cell number or a range of cells. Default: 1.\n"
	"1 0.6 # takes fraction 0.6 of solution 1\n"
	"2 0.4 # adds fraction 0.4 of solution 2"
	,
	"Used for mixing two or more aqueous solutions, or for taking a fraction of a solution.\n"
	"\n"
	"Example:\n"
	"MIX 1\n"
	"1 0.6\n"
	"2 0.4\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT(
	"TRANSPORT\n"
	"-cells  16\n"
	"-length 0.005 # cell length 0.005 m, column length 16*0.005 = 0.08 m\n"
	"-dispersivity   0.002 # m\n"
	"-shifts 40 # 40/16 = 2.5 pore volumes\n"
	"-flow_direction forward\n"
	"-time_step 720 # 1 shift = 720 s. total time = 40*720 s\n"
	"-boundary_conditions flux flux # at column ends\n"
	"-diffusion_coefficient 0.3e-9 # m2/s\n"
	"-punch 16 # only punch/graph cell 16"
	,
	"Defines 1D transport processes: advection and dispersion, diffusion, and diffusion into stagnant zones.\n"
	"\n"
	"Example:\n"
	"TRANSPORT\n"
	"-cells  16\n"
	"-length 0.005\n"
	"-dispersivity   0.002\n"
	"-shifts 40\n"
	"-flow_direction forward\n"
	"-time_step 720\n"
	"-boundary_conditions flux flux\n"
	"-diffusion_coefficient 0.3e-9\n"
	"-punch 16\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_cells(
	"-cells 10"
	,
	"Defines the number of cells. Default: 1\n"
	"\n"
	"Example:\n"
	"-cells 10\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_shifts(
	"-shifts 2"
	,
	"Defines the number of advective timesteps or diffusion periods (shifts * time_step = total simulation time). Printing/punching/graphing follows each shift, unless suspended by print/punch_frequency. Default: 1.\n"
	"\n"
	"Example:\n"
	"-shifts 2\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_time_step(
	"-time_step 3600 1.0 # 3600 s, 1 time substep if -multi_D true"
	,
	"Defines time step (s) of each advective shift or diffusion period (default: 0 s) and time substeps in multicomponent diffusion (default: 1.0).\n"
	"\n"
	"Example:\n"
	"-time_step 3600 1.0\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_flow_direction(
	"-flow_direction diffusion"
	,
	"Defines direction of flow: forward, backward or diffusion. Default: forward\n"
	"\n"
	"Example:\n"
	"-flow_direction diffusion\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_boundary_conditions(
	"-boundary_conditions constant closed # constant at cell 1, closed at last cell"
	,
	"Defines boundary conditions for the first and last cell: constant, closed or flux. Default: flux flux\n"
	"\n"
	"Example:\n"
	"-boundary_conditions constant closed\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_lengths(
	"-lengths 0.5 # all cells are 0.5 m long"
	,
	"Defines length (m) of each cell. Default: 1 m\n"
	"\n"
	"Example:\n"
	"-lengths 0.5\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_dispersivities(
	"-dispersivities 0.05 # all cells have dispersivity 0.05 m"
	,
	"Defines dispersivity (m) of each cell. Default: 0 m.\n"
	"\n"
	"Example:\n"
	"-dispersivities 0.05\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_correct_disp(
	"-correct_disp"
	,
	"If true, dispersivity is multiplied with (1 + 1/cells) for column ends with flux boundary conditions. Default: false\n"
	"\n"
	"Example:\n"
	"-correct_disp\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_diffusion_coefficient(
	"-diffusion_coefficient 1e-10"
	,
	"Defines diffusion coefficient (m2/s) for all aqueous species. Default: 0.3e-9 m2/s\n"
	"\n"
	"Example:\n"
	"-diffusion_coefficient 1e-10\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_stagnant(
	"# Option 1:\n"
	"-stagnant 1 6.8e-4 0.2 0.1\n"
	"# PHREEQC calculates the mixing factors among mobile and immobile cells\n"
	"# 1 stagnant layer, exchange factor alpha = 6.8e-4/s, mobile por = 0.2, immobile por = 0.1\n"
	"# Option 2:\n"
	"-stagnant 10\n"
	"# 10 stagnant layers, mixing factors must be defined with MIX"
	,
	"Defines the maximum number of stagnant (immobile) cells associated with each cell in which advection occurs (mobile cell). Default: 0\n"
	"\n"
	"Example:\n"
	"# Option 1:\n"
	"-stagnant 1 6.8e-4 0.2 0.1\n"
	"# PHREEQC calculates the mixing factors among mobile and immobile cells\n"
	"# 1 stagnant layer, exchange factor alpha = 6.8e-4/s, mobile por = 0.2, immobile por = 0.1\n"
	"# Option 2:\n"
	"-stagnant 10\n"
	"# 10 stagnant layers, mixing factors must be defined with MIX\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_thermal_diffusion(
	"-thermal_diffusion 3 1e-6 # temperature is retarded by 3, diffusion coefficient is 1e-6 m2/s"
	,
	"Defines temperature retardation factor (-) and diffusion coefficient (m2/s).\n"
	"\n"
	"Example:\n"
	"-thermal_diffusion 3 1e-6\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_initial_time(
	"-initial_time 3600 # time starts at 1 hour"
	,
	"Sets the starting time (s) of a transport simulation.\n"
	"\n"
	"Example:\n"
	"-initial_time 3600\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_multi_d(
	"-multi_d true 1e-9 0.3 0.05 1.0\n"
	"# true/false, tracer diffusion coefficient (Dw, m2/s) in water at 25 oC, porosity (por, -), limiting porosity (-) below which diffusion stops, exponent n used in calculating the porewater diffusion coefficient Dp = Dw * por^n"
	,
	"Defines multicomponent diffusion parameters.\n"
	"\n"
	"Example:\n"
	"-multi_d true 1e-9 0.3 0.05 1.0\n"
	"# true/false, tracer diffusion coefficient (Dw, m2/s) in water at 25 oC, porosity (por, -), limiting porosity (-) below which diffusion stops, exponent n used in calculating the porewater diffusion coefficient Dp = Dw * por^n\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_interlayer_d(
	"-interlayer_d true 0.09 0.01 250\n"
	"# true/false, interlayer porosity (-), limiting interlayer porosity (-) below which diffusion stops, tortuosity factor for calculating the interlayer diffusion coefficient (D_il = Dw / (tortuosity factor).\n"
	"# See release.txt, version 2.16. NOTE: -multi_d must be set true."
	,
	"Enables the calculation of surface/interlayer diffusion (only for exchangeable cations on X-).\n"
	"\n"
	"Example:\n"
	"-interlayer_d true 0.09 0.01 250\n"
	"# true/false, interlayer porosity (-), limiting interlayer porosity (-) below which diffusion stops, tortuosity factor for calculating the interlayer diffusion coefficient (D_il = Dw / (tortuosity factor).\n"
	"# See release.txt, version 2.16. NOTE: -multi_d must be set true.\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_print_cells(
	"-print_cells 3-5 7 # Results for cells 3, 4, 5, and 7 are printed."
	,
	"Selects the cells for which results will be written to the output file. Default: all cells.\n"
	"\n"
	"Example:\n"
	"-print_cells 3-5 7\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_print_frequency(
	"-print_frequency 3 # Printing is done after each 3rd shift."
	,
	"-print_frequency 3 # Printing is done after each 3rd shift.\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_punch_cells(
	"-punch_cells 3-5 7 # Results for cells 3, 4, 5, and 7 are punched/graphed."
	,
	"-punch_cells 3-5 7 # Results for cells 3, 4, 5, and 7 are punched/graphed.\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_punch_frequency(
	"-punch_frequency 3 # Punching/graphing is done after each 3rd shift."
	,
	"Selects shifts for which results will be written to the selected-output file and  graphed. Default: all shifts.\n"
	"\n"
	"Example:\n"
	"-punch_frequency 3\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_warnings(
	"-warnings false # Warning messages are not printed."
	,
	"Enables/disables printing of warning messages. Default: true.\n"
	"\n"
	"Example:\n"
	"-warnings false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_dump(
	"-dump c:\\dump_it.phrq # filename"
	,
	"Writes a formatted PHREEQC input file of the complete calculation state each -dump_frequency.\n"
	"\n"
	"Example:\n"
	"-dump c:\\dump_it.phrq\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_dump_frequency(
	"-dump_frequency 3 # Dump-file writing is done after each 3rd shift."
	,
	"Selects shifts for which dump file is written.\n"
	"\n"
	"Example:\n"
	"-dump_frequency 3\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_dump_restart(
	"-dump_restart 123"
	,
	"If a transport simulation is restarted from a dump file, the starting shift number is given on this line.\n"
	"\n"
	"Example:\n"
	"-dump_restart 123\n"
	);

// Output
std::pair<CString, CString> CTreeCtrlPfw::PRINT(
	"PRINT\n"
	"-reset false #suspends all printout"
	,
	"Selects which results are written to the output file.\n"
	"\n"
	"Example:\n"
	"PRINT\n"
	"-reset false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PRINT_alkalinity(
	"-alkalinity true"
	,
	"Prints the species that contribute to alkalinity. Default: false\n"
	"\n"
	"Example:\n"
	"-alkalinity true\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PRINT_echo_input(
	"-echo_input false"
	,
	"Echos input to output file if true. Default: true.\n"
	"\n"
	"Example:\n"
	"-echo_input false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PRINT_Eh(
	"-Eh false"
	,
	"Prints pe and Eh values derived from redox couples in initial solution if true. Default: true\n"
	"\n"
	"Example:\n"
	"-Eh false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PRINT_equilibrium_phases(
	"-equilibrium_phases false"
	,
	"Prints composition of EQUILIBRIUM_PHASES if true. Default: true\n"
	"\n"
	"Example:\n"
	"-equilibrium_phases false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PRINT_exchange(
	"-exchange false"
	,
	"Prints composition of EXCHANGE if true. Default: true\n"
	"\n"
	"Example:\n"
	"-exchange false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PRINT_gas_phase(
	"-gas_phase false"
	,
	"Prints composition of GAS_PHASE if true. Default: true.\n"
	"\n"
	"Example:\n"
	"-gas_phase false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PRINT_headings(
	"-headings false"
	,
	"Prints title and headings of each type of calculation if true. Default: true.\n"
	"\n"
	"Example:\n"
	"-headings false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PRINT_inverse_modeling(
	"-inverse_modeling false"
	,
	"Prints results of inverse modeling if true. Default: true.\n"
	"\n"
	"Example:\n"
	"-inverse_modeling false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PRINT_kinetics(
	"-kinetics false"
	,
	"Prints information about kinetic reactants if true. Default: true.\n"
	"\n"
	"Example:\n"
	"-kinetics false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PRINT_other(
	"-other false"
	,
	"Controls all printing to the output file not set by any of the other identifiers. Default: true.\n"
	"\n"
	"Example:\n"
	"-other false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PRINT_reset(
	"-reset false"
	,
	"Changes all print options to true or false. Default: true\n"
	"\n"
	"Example:\n"
	"-reset false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PRINT_saturation_indices(
	"-saturation_indices false"
	,
	"Prints saturation indices. Default: true.\n"
	"\n"
	"Example:\n"
	"-saturation_indices false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PRINT_solid_solutions(
	"-solid_solutions false"
	,
	"Prints compositions of SOLID_SOLUTIONS if true. Default: true.\n"
	"\n"
	"Example:\n"
	"-solid_solutions false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PRINT_species(
	"-species false"
	,
	"Prints the block headed by 'Distribution of species' if true. Default: true.\n"
	"\n"
	"Example:\n"
	"-species false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PRINT_surface(
	"-surface false"
	,
	"Prints composition of SURFACE if true. Default: true\n"
	"\n"
	"Example:\n"
	"-surface false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PRINT_totals(
	"-totals false"
	,
	"Prints the blocks headed by 'Solution composition' and 'Description of solution' if true. Default: true.\n"
	"\n"
	"Example:\n"
	"-totals false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PRINT_user_print(
	"-user_print false"
	,
	"Controls printout defined in USER_PRINT. Default: true\n"
	"\n"
	"Example:\n"
	"-user_print false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PRINT_selected_output(
	"-selected_output false"
	,
	"Controls punchout to the selected-output file. Default: true\n"
	"\n"
	"Example:\n"
	"-selected_output false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PRINT_status(
	"-status false"
	,
	"Controls printout of calculation progress on the screen. Default: true. -status n prints the status line every n milliseconds.\n"
	"\n"
	"Example:\n"
	"-status false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PRINT_warnings(
	"-warnings 10"
	,
	"Sets the maximum number of warnings to be printed. Default: 100. A negative number prints all warnings\n"
	"\n"
	"Example:\n"
	"-warnings 10\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PRINT_initial_isotopes(
	"-initial_isotopes false"
	,
	"Prints initial isotopes if true. Default: true,\n"
	"\n"
	"Example:\n"
	"-initial_isotopes false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PRINT_isotope_ratios(
	"-isotope_ratios false"
	,
	"Prints isotope ratios if true. Default: true.\n"
	"\n"
	"Example:\n"
	"-isotope_ratios false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PRINT_isotope_alphas(
	"-isotope_alphas false"
	,
	"Prints isotope alphas if true. Default: true.\n"
	"\n"
	"Example:\n"
	"-isotope_alphas false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PRINT_censor_species(
	"-censor_species 1e-8"
	,
	"Skips species with concentrations smaller than number. Default 0.\n"
	"\n"
	"Example:\n"
	"-censor_species 1e-8\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT(
	"SELECTED_OUTPUT\n"
	"-file sel1.csv # file name\n"
	"-reset false"
	,
	"Writes a file for processing by spreadsheets.\n"
	"\n"
	"Example:\n"
	"SELECTED_OUTPUT\n"
	"-file sel1.csv\n"
	"-reset false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_file(
	"-file sel1.csv"
	,
	"Defines the file name.\n"
	"\n"
	"Example:\n"
	"-file sel1.csv\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_selected_out(
	"-selected_out false"
	,
	"Controls printing to the selected-output file. Default: true\n"
	"\n"
	"Example:\n"
	"-selected_out false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_user_punch(
	"-user_punch false"
	,
	"Controls punch of USER_PUNCH statements. Default: true\n"
	"\n"
	"Example:\n"
	"-user_punch false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_high_precision(
	"-high_precision true"
	,
	"Prints results with 12 decimal places if true. Default: false.\n"
	"\n"
	"Example:\n"
	"-high_precision true\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_reset(
	"-reset false"
	,
	"Sets output for identifiers in this block to true or false.\n"
	"\n"
	"Example:\n"
	"-reset false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_simulation(
	"-simulation false"
	,
	"Prints simulation number if true. Default: true.\n"
	"\n"
	"Example:\n"
	"-simulation false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_state(
	"-state false"
	,
	"Prints the type of calculation performed if true. Default: true.\n"
	"\n"
	"Example:\n"
	"-state false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_solution(
	"-solution false"
	,
	"Prints solution number if true. Default: true.\n"
	"\n"
	"Example:\n"
	"-solution false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_distance(
	"-distance false"
	,
	"Prints (1) the X-coordinate of the cell in TRANSPORT, or (2) the cell number in ADVECTION, or (3) -99 for other calculations if true. Default: true.\n"
	"\n"
	"Example:\n"
	"-distance false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_time(
	"-time false"
	,
	"Prints (1) the cumulative model time for batch-reaction calculations with kinetics, or (2) the cumulative transport time for transport calculations, or (3) the advection shift number for advective transport calculations, or (4) -99 for other calculations if true. Default: true.\n"
	"\n"
	"Example:\n"
	"-time false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_step(
	"-step false"
	,
	"Prints (1) shift for transport calculations, or (2) reaction step for batch-reaction calculations, or (3) -99 for other calculations if true. Default: true.\n"
	"\n"
	"Example:\n"
	"-step false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_pH(
	"-pH false"
	,
	"Prints pH if true. Default: true.\n"
	"\n"
	"Example:\n"
	"-pH false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_pe(
	"-pe false"
	,
	"Prints pe if true. Default: true.\n"
	"\n"
	"Example:\n"
	"-pe false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_reaction(
	"-reaction true"
	,
	"Prints (1) reaction increment in a REACTION or (2) -99 for other calculations if true. Default: false.\n"
	"\n"
	"Example:\n"
	"-reaction true\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_temperature(
	"-temperature false"
	,
	"Prints temperature (Celsius) if true. Default: false.\n"
	"\n"
	"Example:\n"
	"-temperature false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_alkalinity(
	"-alkalinity true"
	,
	"Prints alkalinity (eq/kgw) if true. Default: false.\n"
	"\n"
	"Example:\n"
	"-alkalinity true\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_ionic_strength(
	"-ionic_strength true"
	,
	"Prints ionic strength (mu) if true. Default: false.\n"
	"\n"
	"Example:\n"
	"-ionic_strength true\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_water(
	"-water true"
	,
	"Writes mass of water if true. Default: false.\n"
	"\n"
	"Example:\n"
	"-water true\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_charge_balance(
	"-charge_balance true"
	,
	"Writes charge balance (eq) if true. Default: false.\n"
	"\n"
	"Example:\n"
	"-charge_balance true\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_percent_error(
	"-percent_error true"
	,
	"Writes percent error in charge balance if true. Default: false.\n"
	"\n"
	"Example:\n"
	"-percent_error true\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_totals(
	"-totals Ca C C(4) X Hfo_w # name from the 1st column of ..._MASTER_SPECIES"
	,
	"Defines elements for which the total concentration (mol/kgw) is printed.\n"
	"\n"
	"Example:\n"
	"-totals Ca C C(4) X Hfo_w\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_molalities(
	"-molalities Ca+2 CaX2 Hfo_wOCa+ # name defined in ..._SPECIES"
	,
	"Defines species for which the concentration (mol/kgw) is printed.\n"
	"\n"
	"Example:\n"
	"-molalities Ca+2 CaX2 Hfo_wOCa+\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_activities(
	"-activities Ca+2"
	,
	"Defines species for which log10 of activity is printed.\n"
	"\n"
	"Example:\n"
	"-activities Ca+2\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_equilibrium_phases(
	"-equilibrium_phases Calcite Gypsum"
	,
	"Defines equilibrium phases for which total amounts (...) and moles transferred (d_...) are printed.\n"
	"\n"
	"Example:\n"
	"-equilibrium_phases Calcite Gypsum\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_saturation_indices(
	"-saturation_indices Calcite CO2(g)"
	,
	"Defines equilibrium phases for which saturation indices [= log10 of partial pressure for gases] are written.\n"
	"\n"
	"Example:\n"
	"-saturation_indices Calcite CO2(g)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_gases(
	"-gases H2O(g) CO2(g)"
	,
	"Defines gas phases for which total moles are printed. Also writes the pressure (atm), the total moles and the volume (L) of the gas phase.\n"
	"\n"
	"Example:\n"
	"-gases H2O(g) CO2(g)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_kinetic_reactants(
	"-kinetic_reactants Quartz"
	,
	"Defines the kinetic reactants for which the current moles (k_...) and the moles transferred (dk_...) are printed.\n"
	"\n"
	"Example:\n"
	"-kinetic_reactants Quartz\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_solid_solutions(
	"-solid_solutions Gypsum"
	,
	"Defines the solid-solution components (s_...) for which the current moles are printed.\n"
	"\n"
	"Example:\n"
	"-solid_solutions Gypsum\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_inverse_modeling(
	"-inverse_modeling false"
	,
	"Prints results of inverse modeling if true. Default: true.\n"
	"\n"
	"Example:\n"
	"-inverse_modeling false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_calculate_values(
	"-calculate_values name1 name2"
	,
	"Prints results of calculated values (defined with CALCULATE_VALUES).\n"
	"\n"
	"Example:\n"
	"-calculate_values name1 name2\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_isotopes(
	"-isotopes"
	,
	"Defines the isotopes to be printed. See release.txt (Version 2.7) for explanation.\n"
	"\n"
	"Example:\n"
	"-isotopes\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::USER_GRAPH(
	"USER_GRAPH\n"
	"-headings Ca  F  pH\n"
	"-chart_title \"Fluorite Equilibrium\"\n"
	"-axis_scale x_axis 0 350 50\n"
	"-axis_scale y_axis 1 5 1\n"
	"-axis_scale sy_axis 5 7 0.5\n"
	"-axis_titles \"Ca (mg/L)\"  \"F (mg/L)\" \"pH\"\n"
	"-initial_solutions false\n"
	"-start\n"
	"10 graph_x tot(\"Ca\")*40.08e3\n"
	"20 graph_y tot(\"F\")*19e3\n"
	"30 graph_sy -la(\"H+\")\n"
	"-end"
	,
	"Defines the parameters to be charted and the appearance of the chart.\n"
	"\n"
	"Example:\n"
	"USER_GRAPH\n"
	"-headings Ca  F  pH\n"
	"-chart_title \"Fluorite Equilibrium\"\n"
	"-axis_scale x_axis 0 350 50\n"
	"-axis_scale y_axis 1 5 1\n"
	"-axis_scale sy_axis 5 7 0.5\n"
	"-axis_titles \"Ca (mg/L)\"  \"F (mg/L)\" \"pH\"\n"
	"-initial_solutions false\n"
	"-start\n"
	"10 graph_x tot(\"Ca\")*40.08e3\n"
	"20 graph_y tot(\"F\")*19e3\n"
	"30 graph_sy -la(\"H+\")\n"
	"-end\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::USER_GRAPH_headings(
	"-headings Ca  F  pH"
	,
	"Headings that appear as labels in the chart and on the first line of the Grid tab.\n"
	"\n"
	"Example:\n"
	"-headings Ca  F  pH\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::USER_GRAPH_axis_scale(
	"-axis_scale x_axis 0 350 50 25 # minimum 0, maximum 350, major tics at 50, minor tics at 25\n"
	"-axis_scale y_axis 1 5 1\n"
	"-axis_scale sy_axis 5 7 0.5 # the secondary y-axis"
	,
	"Indicates the range and intervals for the chart axes.\n"
	"\n"
	"Example:\n"
	"-axis_scale x_axis 0 350 50 25\n"
	"-axis_scale y_axis 1 5 1\n"
	"-axis_scale sy_axis 5 7 0.5\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::USER_GRAPH_axis_titles(
	"-axis_titles \"Ca (mg/L)\"  \"F (mg/L)\" \"pH\" # x-axis, y-axis, sy-axis"
	,
	"Specifies the axis titles.\n"
	"\n"
	"Example:\n"
	"-axis_titles \"Ca (mg/L)\"  \"F (mg/L)\" \"pH\"\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::USER_GRAPH_chart_title(
	"-chart_title \"Fluorite Equilibrium\""
	,
	"Provides the chart title.\n"
	"\n"
	"Example:\n"
	"-chart_title \"Fluorite Equilibrium\"\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::USER_GRAPH_connect_simulations(
	"-connect_simulations true # true/false"
	,
	"Connects data from subsequent simulations as a single line if true. If false, each simulation is plotted as a separate point. Default: false\n"
	"\n"
	"Example:\n"
	"-connect_simulations true\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::USER_GRAPH_grid_offset(
	"-grid_offset 3 2 # printing starts in B3"
	,
	"Specifies offsets where printing starts in the Grid tab.\n"
	"\n"
	"Example:\n"
	"-grid_offset 3 2\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::USER_GRAPH_initial_solutions(
	"-initial_solutions false"
	,
	"Plots the results of the initial solution calculations. Default: true.\n"
	"\n"
	"Example:\n"
	"-initial_solutions false\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::USER_GRAPH_plot_concentration_vs(
	"-plot_concentration_vs time # time plot\n"
	"-plot_concentration_vs x # distance plot"
	,
	"Determines, in transport calculations, whether distance or time is plotted on the x-axis.\n"
	"\n"
	"Example:\n"
	"-plot_concentration_vs time\n"
	"-plot_concentration_vs x\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::USER_GRAPH_plot_csv_file(
	"-plot_csv_file filename.csv # data start in A1, the file should be tab-delimited\n"
	"-grid_offset 1 4 # shifts PHREEQC output beyond the file-data\n"
	"# Example of a csv file: plots the first 3 F concentrations versus Ca of example file U_Gex1.phrq\n"
	"# Ca	F\n"
	"#  3.2709e+02	  1.7835e+00\n"
	"#  3.1104e+02	  1.8260e+00\n"
	"#  2.9527e+02	  1.8716e+00"
	,
	"Opens the specified file in the grid and plots the data in the chart (to be used with -grid_offset).\n"
	"\n"
	"Example:\n"
	"-plot_csv_file filename.csv\n"
	"-grid_offset 1 4\n"
	"# Example of a csv file: plots the first 3 F concentrations versus Ca of example file U_Gex1.phrq\n"
	"# Ca	F\n"
	"#  3.2709e+02	  1.7835e+00\n"
	"#  3.1104e+02	  1.8260e+00\n"
	"#  2.9527e+02	  1.8716e+00\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::USER_GRAPH_start(
	"-start"
	,
	"Marks the beginning of the BASIC program.\n"
	"\n"
	"Example:\n"
	"-start\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::USER_GRAPH_end(
	"-end"
	,
	"Marks the end of the BASIC program.\n"
	"\n"
	"Example:\n"
	"-end\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::USER_PRINT(
	"USER_PRINT\n"
	"-start # start of BASIC lines\n"
	"10 print 'The solution contains ', TOT(\"Na\") * 23e3, ' mg Na/L.'\n"
	"-end # end of BASIC lines"
	,
	"Prints user-defined information to the output file.\n"
	"\n"
	"Example:\n"
	"USER_PRINT\n"
	"-start\n"
	"10 print 'The solution contains ', TOT(\"Na\") * 23e3, ' mg Na/L.'\n"
	"-end\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::USER_PRINT_start(
	"-start"
	,
	"Marks the beginning of the BASIC program.\n"
	"\n"
	"Example:\n"
	"-start\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::USER_PRINT_end(
	"-end"
	,
	"Marks the end of the BASIC program.\n"
	"\n"
	"Example:\n"
	"-end\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::USER_PUNCH(
	"USER_PUNCH\n"
	"-headings mg_Na/L # column headings in the file\n"
	"-start # start of BASIC lines\n"
	"10 punch TOT(\"Na\") * 22.999e3\n"
	"-end # end of BASIC lines"
	,
	"Prints user-defined quantities to the selected_output file.\n"
	"\n"
	"Example:\n"
	"USER_PUNCH\n"
	"-headings mg_Na/L\n"
	"-start\n"
	"10 punch TOT(\"Na\") * 22.999e3\n"
	"-end\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::USER_PUNCH_headings(
	"-headings mg_Na/L P_CO2/atm"
	,
	"Defines the column headings.\n"
	"\n"
	"Example:\n"
	"-headings mg_Na/L P_CO2/atm\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::USER_PUNCH_start(
	"-start"
	,
	"Marks the beginning of the BASIC program.\n"
	"\n"
	"Example:\n"
	"-start\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::USER_PUNCH_end(
	"-end"
	,
	"Marks the end of the BASIC program.\n"
	"\n"
	"Example:\n"
	"-end\n"
	);


// Database
std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_MASTER_SPECIES(
	"EXCHANGE_MASTER_SPECIES\n"
	"X X- # exchange_master_species X (without charge), exchange_species X- (with charge).\n"
	"Y Y-2"
	,
	"Defines exchange_master_species and corresponding exchange_species.\n"
	"\n"
	"Example:\n"
	"EXCHANGE_MASTER_SPECIES\n"
	"X X-\n"
	"Y Y-2\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_SPECIES(
	"EXCHANGE_SPECIES\n"
	"# identity reaction for species in 2nd column of EXCHANGE_MASTER_SPECIES:\n"
	"X- = X-\n"
	"log_k 0\n"
	"# new exchange_species follows = sign:\n"
	"Na+ + X- = NaX\n"
	"log_k 0.0\n"
	"# define activity coefficient and active fraction coefficient:\n"
	"-gamma	4.0	0.075 0.0\n"
	"Ca+2 + 2X- = CaX2\n"
	"log_k 0.8 # log_k relative to NaX.\n"
	"-gamma	5.0	0.165 0.9"
	,
	"Defines the half-reaction and relative log K for each exchange species.\n"
	"\n"
	"Example:\n"
	"EXCHANGE_SPECIES\n"
	"# identity reaction for species in 2nd column of EXCHANGE_MASTER_SPECIES:\n"
	"X- = X-\n"
	"log_k 0\n"
	"# new exchange_species follows = sign:\n"
	"Na+ + X- = NaX\n"
	"log_k 0.0\n"
	"# define activity coefficient and active fraction coefficient:\n"
	"-gamma	4.0	0.075 0.0\n"
	"Ca+2 + 2X- = CaX2\n"
	"log_k 0.8\n"
	"-gamma	5.0	0.165 0.9\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_SPECIES_log_k(
	"log_k -6.25"
	,
	"Defines log K at 25 degrees C.\n"
	"\n"
	"Example:\n"
	"log_k -6.25\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_SPECIES_delta_h(
	"delta_h 51.3 kcal"
	,
	"Sets enthalpy of reaction at 25 degrees C (kJ/mol by default, or kcal/mol when indicated). Default: 0 kJ/mol.\n"
	"\n"
	"Example:\n"
	"delta_h 51.3 kcal\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_SPECIES_analytical_expression(
	"0.5 1.01 0.0 -1.1 0.0 0.0# log_k = 0.5 + (1.01 * T) + (0.0 / T) + (-1.1 * log10(T)) + (0.0 / T^2) + (0.0 * T^2), where T is temperature in Kelvin."
	,
	"Provides coefficients for an analytical expression of the temperature dependent log K.\n"
	"\n"
	"Example:\n"
	"0.5 1.01 0.0 -1.1 0.0 0.\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_SPECIES_gamma(
	"-gamma	5.0	0.165 0.50 # Debye-Hueckel a, b; a_f = 0.50"
	,
	"The WATEQ Debye-Hückel equation is used to calculate the activity coefficient for the exchange species; active fraction coefficient. Default: activity coefficient = 1, active fraction coefficient = 0.\n"
	"\n"
	"Example:\n"
	"-gamma	5.0	0.165 0.50\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_SPECIES_davies(
	"-davies"
	,
	"The Davies equation is used to calculate the activity coefficient for the exchange species. Default: activity coefficient = 1.\n"
	"\n"
	"Example:\n"
	"-davies\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_SPECIES_no_check(
	"-no_check"
	,
	"The reaction equation is not checked for charge and elemental balance. Default: check is true\n"
	"\n"
	"Example:\n"
	"-no_check\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_SPECIES_mole_balance(
	"EXCHANGE_SPECIES\n"
	"Ca0.5G + Mg+2 = Mg0.5G + Ca+2 # for a linear Ca/Mg exchange isotherm in the Gapon formulas\n"
	"log_k 0\n"
	"-no_check\n"
	"-mole_balance Mg0.5G"
	,
	"Specifies the stoichiometry of the species explicitly (to be used with '-no_check').\n"
	"\n"
	"Example:\n"
	"EXCHANGE_SPECIES\n"
	"Ca0.5G + Mg+2 = Mg0.5G + Ca+2\n"
	"log_k 0\n"
	"-no_check\n"
	"-mole_balance Mg0.5G\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_SPECIES_add_constant(
	"-add_constant"
	,
	"Adds a specified constant to log_k, used for isotope fractionation. See release.txt, versions 2.13 and 2.7.\n"
	"\n"
	"Example:\n"
	"-add_constant\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_SPECIES_add_logk(
	"-add_logk"
	,
	"Adds a constant to a log K, used for isotope fractionation. See release.txt, versions 2.13 and 2.7\n"
	"\n"
	"Example:\n"
	"-add_logk\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PHASES(
	"PHASES\n"
	"Calcite\n"
	"CaCO3 = Ca+2 + CO3-2\n"
	"log_k -8.48\n"
	"delta_h	-2.297 kcal\n"
	"-analytical	-171.9065	-0.077993	2839.319	71.595	0.0 0.0"
	,
	"Defines the name, chemical (dissociation) reaction, log K, and temperature dependence of log K for minerals and gases.\n"
	"\n"
	"Example:\n"
	"PHASES\n"
	"Calcite\n"
	"CaCO3 = Ca+2 + CO3-2\n"
	"log_k -8.48\n"
	"delta_h	-2.297 kcal\n"
	"-analytical	-171.9065	-0.077993	2839.319	71.595	0.0 0.0\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PHASES_log_k(
	"log_k -8.3"
	,
	"Sets log K at 25 oC. Default: 0.\n"
	"\n"
	"Example:\n"
	"log_k -8.3\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PHASES_delta_h(
	"delta_h 0.45 kcal"
	,
	"Sets reaction enthalpy at 25 oC (kJ/mol by default, or kcal/mol when indicated). Default: 0 kJ/mol.\n"
	"\n"
	"Example:\n"
	"delta_h 0.45 kcal\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PHASES_analytical_expression(
	"0.5 1.01 0.0 -1.1 0.0 0.0# log_k = 0.5 + (1.01 * T) + (0.0 / T) + (-1.1 * log10(T)) + (0.0 / T^2) + (0.0 * T^2), where T is temperature in Kelvin."
	,
	"Provides coefficients for an analytical expression of the temperature dependent log K.\n"
	"\n"
	"Example:\n"
	"0.5 1.01 0.0 -1.1 0.0 0.\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PHASES_no_check(
	"-no_check"
	,
	"The reaction equation is not checked for charge and elemental balance. Default: no_check is false\n"
	"\n"
	"Example:\n"
	"-no_check\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PHASES_add_constant(
	"-add_constant"
	,
	"Adds a specified constant to log_k.\n"
	"\n"
	"Example:\n"
	"-add_constant\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PHASES_add_logk(
	"-add_logk"
	,
	"Adds a constant to a log K, used for isotope fractionation. See release.txt, versions 2.13 and 2.7\n"
	"\n"
	"Example:\n"
	"-add_logk\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PITZER(
	"PITZER"
	,
	"Specifies parameters for the Pitzer ion-association model, see pitzer.dat and release.txt versions 2.13 and 2.12.\n"
	"\n"
	"Example:\n"
	"PITZER\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PITZER_ALPHAS(
	"-ALPHAS\n"
	"#Ion1     Ion2      Alpha1   Alpha2\n"
	"Fe+2      Cl-       2        1\n"
	"Fe+2      SO4-2     1.559    5.268"
	,
	"Identifier for Pitzer parameters Alpha1 and Alpha2.\n"
	"\n"
	"Example:\n"
	"-ALPHAS\n"
	"#Ion1     Ion2      Alpha1   Alpha2\n"
	"Fe+2      Cl-       2        1\n"
	"Fe+2      SO4-2     1.559    5.268\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PITZER_B0(
	"-B0\n"
	"Na+       Cl-       0.0765     -777.03     -4.4706        0.008946   -3.3158E-6"
	,
	"Identifier for Pitzer parameter B0.\n"
	"\n"
	"Example:\n"
	"-B0\n"
	"Na+       Cl-       0.0765     -777.03     -4.4706        0.008946   -3.3158E-6\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PITZER_B1(
	"-B1\n"
	"Na+       Cl-       0.2664        0           0           6.1608E-5   1.0715E-6"
	,
	"Identifier for Pitzer parameter B1.\n"
	"\n"
	"Example:\n"
	"-B1\n"
	"Na+       Cl-       0.2664        0           0           6.1608E-5   1.0715E-6\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PITZER_B2(
	"-B2\n"
	"Mg+2      SO4-2    -37.23         0           0          -0.253"
	,
	"Identifier for Pitzer parameter B2.\n"
	"\n"
	"Example:\n"
	"-B2\n"
	"Mg+2      SO4-2    -37.23         0           0          -0.253\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PITZER_C0(
	"-C0\n"
	"Na+       Cl-       0.00127     33.317      0.09421    -4.655E-5"
	,
	"Identifier for Pitzer parameter C0.\n"
	"\n"
	"Example:\n"
	"-C0\n"
	"Na+       Cl-       0.00127     33.317      0.09421    -4.655E-5\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PITZER_LAMDA(
	"-LAMDA\n"
	"Na+       CO2       0.1"
	,
	"Identifier for Pitzer parameter Lamda.\n"
	"\n"
	"Example:\n"
	"-LAMDA\n"
	"Na+       CO2       0.1\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PITZER_MacInnes(
	"-MacInnes true"
	,
	"Specifies to use the MacInnes assumption (gamma_K = gamma_Cl) for individual activities and activity coefficients. Default: -MacInnes false\n"
	"\n"
	"Example:\n"
	"-MacInnes true\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PITZER_PSI(
	"-PSI\n"
	"Na+       K+        Cl-       -0.0018"
	,
	"Identifier for Pitzer parameter Psi.\n"
	"\n"
	"Example:\n"
	"-PSI\n"
	"Na+       K+        Cl-       -0.0018\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PITZER_redox(
	"-redox"
	,
	"Enables redox calculations in Pitzer model.\n"
	"\n"
	"Example:\n"
	"-redox\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PITZER_THETA(
	"-THETA\n"
	"K+        Na+      -0.012"
	,
	"Identifier for Pitzer parameter Theta.\n"
	"\n"
	"Example:\n"
	"-THETA\n"
	"K+        Na+      -0.012\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PITZER_use_etheta(
	"-use_etheta true"
	,
	"Identifier to include or exclude nonsymmetric mixing terms. Default: -use_etheta true\n"
	"\n"
	"Example:\n"
	"-use_etheta true\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PITZER_ZETA(
	"-ZETA\n"
	"H+        Cl-       B(OH)3    -0.0102"
	,
	"Identifier for Pitzer parameter Zeta.\n"
	"\n"
	"Example:\n"
	"-ZETA\n"
	"H+        Cl-       B(OH)3    -0.0102\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SIT(
	"SIT"
	,
	"Specifies parameters for the SITP ion-association model, see release.txt version 2.16.\n"
	"\n"
	"Example:\n"
	"SIT\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::RATES(
	"RATES\n"
	"Quartz  # rate name\n"
	"-start\n"
	"10 A0 = parm(1) # parm(1) is defined in the KINETICS block\n"
	"20 V = parm(2)\n"
	"30 rate = 10^-13.7 * (1 - SR(\"Quartz\") * A0 / V * (m / m0)^0.67\n"
	"40 save rate * time # save and time must be in the rate statements\n"
	"-end"
	,
	"Defines the rate expression for kinetic reactions in BASIC statements.\n"
	"\n"
	"Example:\n"
	"RATES\n"
	"Quartz \n"
	"-start\n"
	"10 A0 = parm(1)\n"
	"20 V = parm(2)\n"
	"30 rate = 10^-13.7 * (1 - SR(\"Quartz\") * A0 / V * (m / m0)^0.67\n"
	"40 save rate * time\n"
	"-end\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::RATES_start(
	"-start"
	,
	"Marks the beginning of the BASIC program.\n"
	"\n"
	"Example:\n"
	"-start\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::RATES_end(
	"-end"
	,
	"Marks the end of the BASIC program.\n"
	"\n"
	"Example:\n"
	"-end\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_MASTER_SPECIES(
	"SOLUTION_MASTER_SPECIES\n"
	"#element	species	alk	gfw_formula	element_gfw\n"
	"H	H+	-1.0	H		1.008\n"
	"H(0)	H2	0.0	H\n"
	"H(1)	H+	-1.0	0.0\n"
	"Ca	Ca+2	0.0	Ca		40.08"
	,
	"Defines the element names and aqueous primary and secondary master species.\n"
	"\n"
	"Example:\n"
	"SOLUTION_MASTER_SPECIES\n"
	"#element	species	alk	gfw_formula	element_gfw\n"
	"H	H+	-1.0	H		1.008\n"
	"H(0)	H2	0.0	H\n"
	"H(1)	H+	-1.0	0.0\n"
	"Ca	Ca+2	0.0	Ca		40.08\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES(
	"SOLUTION_SPECIES\n"
	"H+ = H+ # an identity reaction for the master species\n"
	"log_k	0.0\n"
	"-gamma	9.0	0.0 # extended Debye-Hueckel a0 and b\n"
	"-dw 	9.31e-9 # tracer diffusion coefficient (m2/s)\n"
	"#\n"
	"2 NO3- + 12 H+ + 10 e- = N2 + 6 H2O # N2 is the newly defined species\n"
	"log_k	207.08\n"
	"delta_h -312.130 kcal\n"
	"-dw 	1.96e-9 # if gamma is not defined, the Davies equation is used"
	,
	"Defines the chemical (association) reaction, log K and reaction enthalpy (or analytical expression for log K versus T), activity-coefficient parameters (-gamma), tracer diffusion coefficient (-dw) for aqueous species, parameters for calculating the density of the solution (-Millero), and enrichment in the diffuse double layer (-erm_ddl).\n"
	"\n"
	"Example:\n"
	"SOLUTION_SPECIES\n"
	"H+ = H+\n"
	"log_k	0.0\n"
	"-gamma	9.0	0.0\n"
	"-dw 	9.31e-9\n"
	"#\n"
	"2 NO3- + 12 H+ + 10 e- = N2 + 6 H2O\n"
	"log_k	207.08\n"
	"delta_h -312.130 kcal\n"
	"-dw 	1.96e-9\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_log_k(
	"log_k -9.5"
	,
	"log K at 25 oC. Default: 0\n"
	"\n"
	"Example:\n"
	"log_k -9.5\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_delta_h(
	"delta_h -312.130 kcal"
	,
	"Sets reaction enthalpy at 25 oC (kJ/mol by default, or kcal/mol when indicated). Default: 0 kJ/mol.\n"
	"\n"
	"Example:\n"
	"delta_h -312.130 kcal\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_analytical_expression(
	"0.5 1.01 0.0 -1.1 0.0 0.0# log_k = 0.5 + (1.01 * T) + (0.0 / T) + (-1.1 * log10(T)) + (0.0 / T^2) + (0.0 * T^2), where T is temperature in Kelvin."
	,
	"Provides coefficients for an analytical expression of the temperature dependent log K.\n"
	"\n"
	"Example:\n"
	"0.5 1.01 0.0 -1.1 0.0 0.\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_gamma(
	"-gamma	9.0	0.1 # extended Debye-Hueckel a0 and b"
	,
	"Defines activity-coefficient parameters a0 and b.\n"
	"\n"
	"Example:\n"
	"-gamma	9.0	0.1\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_no_check(
	"-no_check"
	,
	"The reaction equation is not checked for charge and elemental balance. Default: check is true\n"
	"\n"
	"Example:\n"
	"-no_check\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_mole_balance(
	"HS- = S2-2 + H+\n"
	"-no_check\n"
	"-mole_balance S(-2)2 # S2-2 counts as S(-2)2 in the mole balance of S"
	,
	"Specifies the stoichiometry of the species explicitly (to be used with '-no_check').\n"
	"\n"
	"Example:\n"
	"HS- = S2-2 + H+\n"
	"-no_check\n"
	"-mole_balance S(-2)2\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_llnl_gamma(
	"-llnl_gamma 3.5"
	,
	"Defines the ion-size parameter in the LLNL aqueous model.\n"
	"\n"
	"Example:\n"
	"-llnl_gamma 3.5\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_co2_llnl_gamma(
	"-co2_llnl_gamma"
	,
	" For neutral species: the temperature dependent function for bdot given by -co2_coefs in llnl.dat must be used.\n"
	"\n"
	"Example:\n"
	"-co2_llnl_gamma\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_dw(
	"-dw 	1.88e-9 # m2/s"
	,
	"Defines tracer diffusion coefficient (m2/s) at 25oC.\n"
	"\n"
	"Example:\n"
	"-dw 	1.88e-9\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_erm_ddl(
	"-erm_ddl 1.1 # c_DDL = c_free * Boltzmann factor * 1.1. (Default = 1)."
	,
	"Enrichment factor in the Donnan approximation of the diffuse double layer.\n"
	"\n"
	"Example:\n"
	"-erm_ddl 1.1\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_Millero(
	"-Millero -19.69 0.1058 -0.001256 1.617 -0.075 0.0008262 # a b c d e f in the Millero equations, here for Ca+2. See release.txt 2.6"
	,
	"Provides coefficients for calculating the solution density (kg/L).\n"
	"\n"
	"Example:\n"
	"-Millero -19.69 0.1058 -0.001256 1.617 -0.075 0.0008262\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_activity_water(
	"-activity_water"
	,
	"Forces the activity coefficient for isotopic species of H2O to be activity(water)/55.5.\n"
	"\n"
	"Example:\n"
	"-activity_water\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_add_constant(
	"-add_constant"
	,
	"Adds a specified constant to log_k.\n"
	"\n"
	"Example:\n"
	"-add_constant\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_add_logk(
	"-add_logk"
	,
	"Adds a constant to a log K, used for isotope fractionation. See release.txt, versions 2.13 and 2.7\n"
	"\n"
	"Example:\n"
	"-add_logk\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SURFACE_MASTER_SPECIES(
	"SURFACE_MASTER_SPECIES\n"
	"Hfo_s	Hfo_sOH\n"
	"Hfo_w	Hfo_wOH # master_species, surface_species\n"
	"# The same electrostatic term is used for all Hfo_... species"
	,
	"Defines surface master species.\n"
	"\n"
	"Example:\n"
	"SURFACE_MASTER_SPECIES\n"
	"Hfo_s	Hfo_sOH\n"
	"Hfo_w	Hfo_wOH\n"
	"# The same electrostatic term is used for all Hfo_... species\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SURFACE_SPECIES(
	"SURFACE_SPECIES\n"
	"Hfo_sOH = Hfo_sOH\n"
	"log_k	0.0 # an identity reaction has log K = 0\n"
	"Hfo_sOH + H+ = Hfo_sOH2+\n"
	"log_k	7.29	# = pKa1,int\n"
	"# new species follows the = sign"
	,
	"Defines the association reaction and log K for surface species.\n"
	"\n"
	"Example:\n"
	"SURFACE_SPECIES\n"
	"Hfo_sOH = Hfo_sOH\n"
	"log_k	0.0\n"
	"Hfo_sOH + H+ = Hfo_sOH2+\n"
	"log_k	7.29\n"
	"# new species follows the = sign\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SURFACE_SPECIES_log_k(
	"log_k 7.29"
	,
	"Defines log K at 25 oC.\n"
	"\n"
	"Example:\n"
	"log_k 7.29\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SURFACE_SPECIES_delta_h(
	"delta_h 0.1"
	,
	"Sets reaction enthalpy at 25 oC (kJ/mol by default, or kcal/mol when indicated). Default: 0 kJ/mol.\n"
	"\n"
	"Example:\n"
	"delta_h 0.1\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SURFACE_SPECIES_analytical_expression(
	"0.5 1.01 0.0 -1.1 0.0 0.0# log_k = 0.5 + (1.01 * T) + (0.0 / T) + (-1.1 * log10(T)) + (0.0 / T^2) + (0.0 * T^2), where T is temperature in Kelvin."
	,
	"Provides coefficients for an analytical expression of the temperature dependent log K.\n"
	"\n"
	"Example:\n"
	"0.5 1.01 0.0 -1.1 0.0 0.\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SURFACE_SPECIES_cd_music(
	"-cd_music  0.25 -2.25 0 # (delta z0), (delta z1), (delta z2), the changes in charge at plane 0, 1 and 2. Default: 0 for all values"
	,
	"Defines the change of charge for 3 planes at the surface. Default: not used\n"
	"\n"
	"Example:\n"
	"-cd_music  0.25 -2.25 0\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SURFACE_SPECIES_no_check(
	"-no_check"
	,
	"The reaction equation is not checked for charge and elemental balance. Default: check is true\n"
	"\n"
	"Example:\n"
	"-no_check\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SURFACE_SPECIES_mole_balance(
	"SURFACE_MASTER_SPECIES\n"
	"Sor Sor\n"
	"#\n"
	"SURFACE_SPECIES\n"
	"Sor = Sor;    log_k 0.0\n"
	"Sor + 0.5Cd+2 = SorCd # Freundlich equation: SorCd = Kf * [Cd+2]^0.5\n"
	"-no_check\n"
	"-mole_balance SorCd\n"
	"log_k -100.0 # log ((Kf = 1) / m_Sor), multiply concentration of Sor by 1e100\n"
	"#\n"
	"SURFACE 1\n"
	"Sor 1e100 1.0 1e100\n"
	"-equil 1\n"
	"-no_edl true"
	,
	"Specifies the stoichiometry of the species explicitly (to be used with '-no_check').\n"
	"\n"
	"Example:\n"
	"SURFACE_MASTER_SPECIES\n"
	"Sor Sor\n"
	"#\n"
	"SURFACE_SPECIES\n"
	"Sor = Sor;    log_k 0.0\n"
	"Sor + 0.5Cd+2 = SorCd\n"
	"-no_check\n"
	"-mole_balance SorCd\n"
	"log_k -100.0\n"
	"#\n"
	"SURFACE 1\n"
	"Sor 1e100 1.0 1e100\n"
	"-equil 1\n"
	"-no_edl true\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SURFACE_SPECIES_add_constant(
	"-add_constant"
	,
	"Adds a specified constant to log_k.\n"
	"\n"
	"Example:\n"
	"-add_constant\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SURFACE_SPECIES_add_logk(
	"-add_logk"
	,
	"Adds a constant to a log K, used for isotope fractionation. See release.txt, versions 2.13 and 2.7\n"
	"\n"
	"Example:\n"
	"-add_logk\n"
	);

// Miscellaneous
std::pair<CString, CString> CTreeCtrlPfw::CALCULATE_VALUES(
	"CALCULATE_VALUES\n"
	"pCa # pCa is name, can be called elsewhere in BASIC statements\n"
	"-start\n"
	"10 save -la(\"Ca+2\") # save must be present in the BASIC definition.\n"
	"-end\n"
	"# example of a call:\n"
	"USER_PRINT\n"
	"-start\n"
	"10 print calc_value(\"pCa\")\n"
	"-end"
	,
	"Define your own BASIC statements.\n"
	"\n"
	"Example:\n"
	"CALCULATE_VALUES\n"
	"pCa\n"
	"-start\n"
	"10 save -la(\"Ca+2\")\n"
	"-end\n"
	"# example of a call:\n"
	"USER_PRINT\n"
	"-start\n"
	"10 print calc_value(\"pCa\")\n"
	"-end\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::CALCULATE_VALUES_start(
	"-start"
	,
	"Indicates the start of the BASIC program.\n"
	"\n"
	"Example:\n"
	"-start\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::CALCULATE_VALUES_end(
	"-end"
	,
	"Indicates the end of the BASIC program.\n"
	"\n"
	"Example:\n"
	"-end\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::ISOTOPES(
	"ISOTOPES"
	,
	"Defines the names, units, and absolute ratio in the standard for isotopes. See release.txt (Version 2.7) for explanation.\n"
	"\n"
	"Example:\n"
	"ISOTOPES\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::ISOTOPES_isotope(
	"-isotope"
	,
	"Defines an isotope.	See release.txt (Version 2.7) for explanation.\n"
	"\n"
	"Example:\n"
	"-isotope\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::ISOTOPE_ALPHAS(
	"ISOTOPE_ALPHAS"
	,
	"Prints calculated isotopic ratios defined with CALCULATE_VALUES. See release.txt (Version 2.7) for explanation.\n"
	"\n"
	"Example:\n"
	"ISOTOPE_ALPHAS\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::ISOTOPE_RATIOS(
	"ISOTOPE_RATIOS"
	,
	"Prints (1) the absolute ratio of minor isotope to major isotope for each minor isotope and (2) the ratio converted to standard measurement units. See release.txt (Version 2.7) for explanation.\n"
	"\n"
	"Example:\n"
	"ISOTOPE_RATIOS\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KNOBS(
	"KNOBS\n"
	"-iterations 500 # allowed iteration number is 500\n"
	"-debug_model # print details of calculations in model.c"
	,
	"Allows adaption of convergence parameters and printout of debugging information.\n"
	"\n"
	"Example:\n"
	"KNOBS\n"
	"-iterations 500\n"
	"-debug_model\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KNOBS_iterations(
	"-iterations 500 # allowed iteration number is 500"
	,
	"Allows changing the maximum number of iterations. Default: -iterations 200\n"
	"\n"
	"Example:\n"
	"-iterations 500\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KNOBS_convergence_tolerance(
	"-convergence_tolerance 1e-12"
	,
	"Changes the convergence criterion for the solver. Default: -convergence_tolerance 1e-8, the mole balance error is to be smaller than 1e-8 * M_i, where M_i is the total concentration of element i.\n"
	"\n"
	"Example:\n"
	"-convergence_tolerance 1e-12\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KNOBS_tolerance(
	"-tolerance 1e-12"
	,
	"Sets the minimum non-zero number for the optimization solver. Default: -tolerance 1e-15\n"
	"\n"
	"Example:\n"
	"-tolerance 1e-12\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KNOBS_step_size(
	"-step_size 10"
	,
	"Sets the maximum step size change in activity during iterations. Default: -step_size 100, the activity may change by 2 orders of magnitude.\n"
	"\n"
	"Example:\n"
	"-step_size 10\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KNOBS_pe_step_size(
	"-pe_step_size 2.5"
	,
	"Sets the maximum pe step size change during iterations. Default: -pe_step_size 10, the pe (= -log(a_e)) may change by +- 1.\n"
	"\n"
	"Example:\n"
	"-pe_step_size 2.5\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KNOBS_diagonal_scale(
	"-diagonal_scale true"
	,
	"Allows scaling of mole balance equations with totals less than 1e-11 moles. Default: false.\n"
	"\n"
	"Example:\n"
	"-diagonal_scale true\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KNOBS_debug_diffuse_layer(
	"-debug_diffuse_layer true"
	,
	"Prints details of diffuse layer calculations. Default: -debug_diffuse_layer false\n"
	"\n"
	"Example:\n"
	"-debug_diffuse_layer true\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KNOBS_debug_inverse(
	"-debug_inverse true"
	,
	"Prints details of inverse model calculations. Default: -debug_inverse false.\n"
	"\n"
	"Example:\n"
	"-debug_inverse true\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KNOBS_debug_model(
	"-debug_model true"
	,
	"Prints details of calculations by subroutine model. Default: -debug_model false.\n"
	"\n"
	"Example:\n"
	"-debug_model true\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KNOBS_debug_prep(
	"-debug_prep true"
	,
	"Prints details of calculations by subroutine prep. Default: -debug_prep false.\n"
	"\n"
	"Example:\n"
	"-debug_prep true\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KNOBS_debug_set(
	"-debug_set true"
	,
	"Prints details of calculations by subroutine set. Default: -debug_set false.\n"
	"\n"
	"Example:\n"
	"-debug_set true\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KNOBS_logfile(
	"-logfile true"
	,
	"Prints various information to the file phreeqc.log, cf. Manual. Default: -logfile false\n"
	"\n"
	"Example:\n"
	"-logfile true\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::LLNL_AQUEOUS_MODEL_PARAMETERS(
	"LLNL_AQUEOUS_MODEL_PARAMETERS"
	,
	"Defines parameters for the LLNL aqueous model used in EQ3/6 and Geochemists Workbench, assembled in llnl.dat.	Values of Debye-Hueckel a and B, and bdot (ionic strength coefficient) are read at fixed temperatures and linearly interpolated between temperatures.\n"
	"\n"
	"Example:\n"
	"LLNL_AQUEOUS_MODEL_PARAMETERS\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::LLNL_AQUEOUS_MODEL_PARAMETERS_temperatures(
	"-temperatures"
	,
	"Specifies the temperatures for which Debye-Hückel a and B, and bdot are listed. See llnl.dat.\n"
	"\n"
	"Example:\n"
	"-temperatures\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::LLNL_AQUEOUS_MODEL_PARAMETERS_dh_a(
	"-dh_a"
	,
	"Specifies a set of values for the Debye-Hückel a parameter at fixed temperatures. See llnl.dat.\n"
	"\n"
	"Example:\n"
	"-dh_a\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::LLNL_AQUEOUS_MODEL_PARAMETERS_dh_b(
	"-dh_b"
	,
	"Specifies a set of values for the Debye-Hückel B parameter at fixed temperatures. See llnl.dat.\n"
	"\n"
	"Example:\n"
	"-dh_b\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::LLNL_AQUEOUS_MODEL_PARAMETERS_bdot(
	"-bdot"
	,
	"Specifies a set of values for the bdot parameter at fixed temperatures. See llnl.dat.\n"
	"\n"
	"Example:\n"
	"-bdot\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::LLNL_AQUEOUS_MODEL_PARAMETERS_co2_coefs(
	"-co2_coefs"
	,
	"Specifies the coefficients for the Drummond (1981) polynomial, used for calculating activity coefficients of neutral species. See llnl.dat.\n"
	"\n"
	"Example:\n"
	"-co2_coefs\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::NAMED_EXPRESSIONS(
	"NAMED_EXPRESSIONS"
	,
	"Defines analytical expressions that are functions of temperature for isotopes. See release.txt (Version 2.7) for explanation.\n"
	"\n"
	"Example:\n"
	"NAMED_EXPRESSIONS\n"
	);

// PHREEQC Basic Functions
std::pair<CString, CString> CTreeCtrlPfw::ACT(
	"ACT(\"Na+\")"
	,
	"Activity of an AQUEOUS, EXCHANGE, or SURFACE species.\n"
	"\n"
	"Example:\n"
	"ACT(\"Na+\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::ALK(
	"ALK"
	,
	"Alkalinity (eq/kgw) of the solution.\n"
	"\n"
	"Example:\n"
	"ALK\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::CALC_VALUE(
	"CALC_VALUE(\"pCa\")"
	,
	"Evaluates a definition of CALCULATE_VALUES.\n"
	"\n"
	"Example:\n"
	"CALC_VALUE(\"pCa\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::CELL_NO(
	"CELL_NO"
	,
	"Cell number or solution number.\n"
	"\n"
	"Example:\n"
	"CELL_NO\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::CHANGE_POR(
	"CHANGE_POR(0.21, cell_no) # porosity of cell 'cell_no' becomes 0.21"
	,
	"Modifies the porosity in a cell.\n"
	"\n"
	"Example:\n"
	"CHANGE_POR(0.21, cell_no)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::CHANGE_SURF(
	"change_surf(\"Hfo\", 0.2, \"Sorbedhfo\", 0, cell_no) 	# take a fraction 0.2 of 'Hfo', rename it to 'Sorbedhfo', with a diffusion coefficient of 0, in cell 'cell_no'\n"
	"#\n"
	"change_surf(\"Hfo\", 1, \"Hfo\", 1e-12, cell_no) # change the diffusion coefficient of 'Hfo' to  1e-12 m2/s in cell 'cell_no'"
	,
	"Changes the diffusion coefficient of (part of) a SURFACE, renames the SURFACE.\n"
	"\n"
	"Example:\n"
	"change_surf(\"Hfo\", 0.2, \"Sorbedhfo\", 0, cell_no) \n"
	"#\n"
	"change_surf(\"Hfo\", 1, \"Hfo\", 1e-12, cell_no)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::CHARGE_BALANCE(
	"CHARGE_BALANCE"
	,
	"Aqueous charge balance in equivalents.\n"
	"\n"
	"Example:\n"
	"CHARGE_BALANCE\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::DESCRIPTION(
	"DESCRIPTION"
	,
	"The description of the SOLUTION.\n"
	"\n"
	"Example:\n"
	"DESCRIPTION\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::DIST(
	"DIST"
	,
	"Returns (1) distance to cell-midpoint in TRANSPORT calculations, or (2) cell number in ADVECTION calculations, or (3) -99 in all other calculations.\n"
	"\n"
	"Example:\n"
	"DIST\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::EDL(
	"EDL(\"Na\", \"Hfo\") # mol Na in DDL of Hfo\n"
	"EDL(\"water\", \"Hfo\") # kg water in DDL of Hfo\n"
	"EDL(\"Charge\", \"Hfo\") # Charge (eq) of Hfo\n"
	"EDL(\"Sigma\", \"Hfo\") # Charge (C/m2) of Hfo\n"
	"EDL(\"Psi\", \"Hfo\") # Potential (V) of Hfo surface"
	,
	"The moles of an element, the kg of water in the diffuse layer, the charge (eq or C/m2) or the potential (V) of a surface. For CD-MUSIC surfaces, charge, sigma and psi can be requested for the 0, 1 and 2 planes, see release.txt, version 2.16 \n"
	"\n"
	"Example:\n"
	"EDL(\"Na\", \"Hfo\")\n"
	"EDL(\"water\", \"Hfo\")\n"
	"EDL(\"Charge\", \"Hfo\")\n"
	"EDL(\"Sigma\", \"Hfo\")\n"
	"EDL(\"Psi\", \"Hfo\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::EQUI(
	"EQUI(\"Calcite\")"
	,
	"Moles of a phase in EQUILIBRIUM_PHASES.\n"
	"\n"
	"Example:\n"
	"EQUI(\"Calcite\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::EXISTS(
	"EXISTS(cell_no, 2)"
	,
	"Checks if a memory location (defined by 1 or more subscripts) is filled.\n"
	"\n"
	"Example:\n"
	"EXISTS(cell_no, 2)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::GAS(
	"GAS(\"O2(g)\")"
	,
	"Moles of a gas component in GAS_PHASE.\n"
	"\n"
	"Example:\n"
	"GAS(\"O2(g)\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::GAMMA(
	"GAMMA(\"H+\")"
	,
	"Activity coefficient of a SOLUTION_SPECIES. LG(\"..\") gives the log10 of the activity coefficient.\n"
	"\n"
	"Example:\n"
	"GAMMA(\"H+\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::GET(
	"GET(cell_no, 2)"
	,
	"Retrieves the value in a memory location defined by 1 or more subscripts.\n"
	"\n"
	"Example:\n"
	"GET(cell_no, 2)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::GET_POR(
	"GET_POR(cell_no)"
	,
	"Porosity in a cell.\n"
	"\n"
	"Example:\n"
	"GET_POR(cell_no)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::GRAPH_X(
	"GRAPH_X tot(\"Na\")"
	,
	"Defines the x-axis of the graph (1 variable).\n"
	"\n"
	"Example:\n"
	"GRAPH_X tot(\"Na\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::GRAPH_Y(
	"GRAPH_Y tot(\"Cl\"), tot(\"K\")"
	,
	"Plots variables on the y-axis of the graph.\n"
	"\n"
	"Example:\n"
	"GRAPH_Y tot(\"Cl\"), tot(\"K\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::GRAPH_SY(
	"GRAPH_SY -la(\"H+\"), -la(\"e-\")"
	,
	"Plots variables on the secondary y-axis of the graph.\n"
	"\n"
	"Example:\n"
	"GRAPH_SY -la(\"H+\"), -la(\"e-\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::ISO(
	"10 O18_permil = ISO(\"[18O]\")"
	,
	"Gives an isotopic composition in the input units for an isotope.\n"
	"\n"
	"Example:\n"
	"10 O18_permil = ISO(\"[18O]\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::ISO_UNITS(
	"10 D_units$ = ISO_UNITS(\"D\")"
	,
	"Gives the input units for the isotope.\n"
	"\n"
	"Example:\n"
	"10 D_units$ = ISO_UNITS(\"D\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KIN(
	"KIN(\"Calcite\")"
	,
	"Actual moles of a kinetic reactant (m).\n"
	"\n"
	"Example:\n"
	"KIN(\"Calcite\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::LA(
	"-LA(\"H+\") # pH"
	,
	"Log10 of activity of a SOLUTION, EXCHANGE, or SURFACE_SPECIES.\n"
	"\n"
	"Example:\n"
	"-LA(\"H+\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::LG(
	"LG(\"H+\")"
	,
	"Log10 of the activity coefficient of a SOLUTION_SPECIES. GAMMA(\"..\") gives the activity coefficient.\n"
	"\n"
	"Example:\n"
	"LG(\"H+\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::LK_NAMED(
	"LK_NAMED(\"aa_13C\")"
	,
	"Log10 of a K value in NAMED_EXPRESSIONS at the current temperature.\n"
	"\n"
	"Example:\n"
	"LK_NAMED(\"aa_13C\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::LK_PHASE(
	"LK_PHASE(\"O2(g)\")"
	,
	"Log10 of a K value in PHASES at the current temperature.\n"
	"\n"
	"Example:\n"
	"LK_PHASE(\"O2(g)\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::LK_SPECIES(
	"LK_SPECIES(\"OH-\")"
	,
	"Log10 of a K value of a SOLUTION, EXCHANGE, or SURFACE_SPECIES at the current temperature.\n"
	"\n"
	"Example:\n"
	"LK_SPECIES(\"OH-\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::LM(
	"LM(\"Na+\")"
	,
	"Log10 of molality of a SOLUTION, EXCHANGE, or SURFACE_SPECIES.\n"
	"\n"
	"Example:\n"
	"LM(\"Na+\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::M(
	"M # to be used in RATES"
	,
	"Current moles of reactant for which the rate is being calculated.\n"
	"\n"
	"Example:\n"
	"M\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::M0(
	"M0 # to be used in RATES"
	,
	"Initial moles of reactant for which the rate is being calculated.\n"
	"\n"
	"Example:\n"
	"M0\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::MISC1(
	"MISC1(\"CaSrCO3\")"
	,
	"Returns (1) mole fraction of component 2 at the beginning of the miscibility gap, or (2) 1.0 if miscibility gap is absent.\n"
	"\n"
	"Example:\n"
	"MISC1(\"CaSrCO3\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::MISC2(
	"MISC2(\"CaSrCO3\")"
	,
	"Returns (1) mole fraction of component 2 at the end of the miscibility gap, or (2) 1.0 if miscibility gap is absent.\n"
	"\n"
	"Example:\n"
	"MISC2(\"CaSrCO3\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::MOL(
	"MOL(\"Na+\")"
	,
	"Molality (mol/kgw) of a SOLUTION, EXCHANGE, or SURFACE_SPECIES.\n"
	"\n"
	"Example:\n"
	"MOL(\"Na+\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::MU(
	"MU"
	,
	"Ionic strength of the solution.\n"
	"\n"
	"Example:\n"
	"MU\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::OSMOTIC(
	"OSMOTIC"
	,
	"Returns (1) the osmotic coefficient if the Pitzer model is used, or (2) 0.0 if the ion-association model is used.\n"
	"\n"
	"Example:\n"
	"OSMOTIC\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PARM(
	"10 A0 = PARM(1) # to be used in RATES"
	,
	"Parameter defined in KINETICS data block.\n"
	"\n"
	"Example:\n"
	"10 A0 = PARM(1)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PERCENT_ERROR(
	"PERCENT_ERROR"
	,
	"Percent charge-balance error [100(cations-|anions|)/(cations + |anions|)].\n"
	"\n"
	"Example:\n"
	"PERCENT_ERROR\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::BASIC_PRINT(
	"PRINT 'pH =', -la(\"H+\")"
	,
	"Write to output file.\n"
	"\n"
	"Example:\n"
	"PRINT 'pH =', -la(\"H+\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PUNCH(
	"PUNCH tot(\"Na\") * 23e3"
	,
	"Write to selected-output file.\n"
	"\n"
	"Example:\n"
	"PUNCH tot(\"Na\") * 23e3\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PUT(
	"PUT(tot(\"Na\"), 999)"
	,
	"Stores value of x in a memory location defined by 1 or more subscripts.\n"
	"\n"
	"Example:\n"
	"PUT(tot(\"Na\"), 999)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::RHO_m(
	"RHO_m"
	,
	"Density of the solution (kg/L) based on Millero (2001) algorithm. Needs Millero parameters in database.\n"
	"\n"
	"Example:\n"
	"RHO_m\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::RHO_v(
	"RHO_v"
	,
	"Density of the solution (kg/L) based on VOPO algorithm (Monnin, 1989). Needs VOPO parameters (pitzer.dat).\n"
	"\n"
	"Example:\n"
	"RHO_v\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::RXN(
	"RXN"
	,
	"Moles of reaction defined in -steps in REACTION data block for a batch-reaction calculation, otherwise zero.\n"
	"\n"
	"Example:\n"
	"RXN\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::BASIC_SAVE(
	"SAVE rate * time"
	,
	"Defines the amount for the integration step of a rate. Amount counts positive when the solution concentration increases (the kinetic reactant (M) decreases in that case).\n"
	"\n"
	"Example:\n"
	"SAVE rate * time\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SC(
	"SC"
	,
	"specific conductance (uS/cm). Needs diffusion coefficients (phreeqd.dat).\n"
	"\n"
	"Example:\n"
	"SC\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SI(
	"SI(\"Calcite\")"
	,
	"Saturation index of a phase.\n"
	"\n"
	"Example:\n"
	"SI(\"Calcite\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SIM_NO(
	"SIM_NO"
	,
	"Simulation number, equals 1 + (the number of END statements before the current simulation).\n"
	"\n"
	"Example:\n"
	"SIM_NO\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SIM_TIME(
	"SIM_TIME"
	,
	"Time from the start of a kinetic batch-reaction or transport calculation.\n"
	"\n"
	"Example:\n"
	"SIM_TIME\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SR(
	"SR(\"Calcite\")"
	,
	"Saturation ratio of a phase.\n"
	"\n"
	"Example:\n"
	"SR(\"Calcite\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::STEP_NO(
	"STEP_NO"
	,
	"Returns (1) step number in batch-reaction calculations, or (2) shift number in ADVECTION and TRANSPORT calculations.\n"
	"\n"
	"Example:\n"
	"STEP_NO\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SUM_GAS(
	"SUM_GAS(\"{CO2, O2}\",\"O\") # returns moles of O\n"
	"# write the chemical formula in {..}, NOT the name of the gas in PHASES"
	,
	"Summed moles of an element in gas-formulas listed in {.., ..}.\n"
	"\n"
	"Example:\n"
	"SUM_GAS(\"{CO2, O2}\",\"O\")\n"
	"# write the chemical formula in {..}, NOT the name of the gas in PHASES\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SUM_SPECIES(
	"SUM_SPECIES(\"{OH-, NaOH}\",\"H\") # returns moles of H"
	,
	"Summed moles of an element in SOLUTION, EXCHANGE, and SURFACE_SPECIES listed in {.., .., ..}.\n"
	"\n"
	"Example:\n"
	"SUM_SPECIES(\"{OH-, NaOH}\",\"H\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SUM_S_S(
	"SUM_S_S(\"CaCdCO3\",\"O\")"
	,
	"Summed moles of an element in SOLID_SOLUTIONS.\n"
	"\n"
	"Example:\n"
	"SUM_S_S(\"CaCdCO3\",\"O\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SURF(
	"SURF(\"As\", \"Hfo\")"
	,
	"Moles of an element sorbed on a SURFACE.\n"
	"\n"
	"Example:\n"
	"SURF(\"As\", \"Hfo\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SYS(
	"SYS(\"Na\")"
	,
	"Total moles of an element in all phases (SOLUTION, EQUILIBRIUM_PHASES, SURFACE, EXCHANGE, SOLID_SOLUTIONS, and GAS_PHASE).\n"
	"\n"
	"Example:\n"
	"SYS(\"Na\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::S_S(
	"S_S(\"Otavite\")"
	,
	"Current moles of a SOLID_SOLUTIONS component.\n"
	"\n"
	"Example:\n"
	"S_S(\"Otavite\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TC(
	"TC"
	,
	"Temperature in Celsius.\n"
	"\n"
	"Example:\n"
	"TC\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TK(
	"TK"
	,
	"Temperature in Kelvin.\n"
	"\n"
	"Example:\n"
	"TK\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TIME(
	"TIME"
	,
	"Time interval for kinetic integration of rates, automatically set and adjusted by the numerical integration method.\n"
	"\n"
	"Example:\n"
	"TIME\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TOT(
	"TOT(\"C(4)\")"
	,
	"Total molality (mol/kgw) of element or element redox state defined as SOLUTION, EXCHANGE, or SURFACE_MASTER_SPECIES. TOT(\"water\") gives kg water.\n"
	"\n"
	"Example:\n"
	"TOT(\"C(4)\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TOTMOLE(
	"10 total_Ca_in_moles = TOTMOLE(\"Ca\")"
	,
	"Provides the total number of moles of an element or element redox state in solution. TOTMOLE(\"ater\") gives number of moles of water. TOTMOL or TOTMOLES are also accepted.\n"
	"\n"
	"Example:\n"
	"10 total_Ca_in_moles = TOTMOLE(\"Ca\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TOTAL_TIME(
	"TOTAL_TIME"
	,
	"Cumulative time (s) since the start of the run or since -initial_time was set.\n"
	"\n"
	"Example:\n"
	"TOTAL_TIME\n"
	);

// GENERAL BASIC statements
std::pair<CString, CString> CTreeCtrlPfw::AND(
	"IF (1 = 1) AND (2 = 2) THEN print 'both TRUE!'"
	,
	"Relational operator.\n"
	"\n"
	"Example:\n"
	"IF (1 = 1) AND (2 = 2) THEN print 'both TRUE!'\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::OR(
	"IF (1 = 1) OR (2 = -2) THEN print 'one is TRUE!'"
	,
	"Relational operator.\n"
	"\n"
	"Example:\n"
	"IF (1 = 1) OR (2 = -2) THEN print 'one is TRUE!'\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::NOT(
	"IF NOT (1 = -1) THEN print 'TRUE!'"
	,
	"Boolean operator.\n"
	"\n"
	"Example:\n"
	"IF NOT (1 = -1) THEN print 'TRUE!'\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::ABS(
	"ABS(-5) # = 5"
	,
	"Absolute value.\n"
	"\n"
	"Example:\n"
	"ABS(-5)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::ARCTAN(
	"ARCTAN(2) # = 1.1071e+00"
	,
	"Arctangent function.\n"
	"\n"
	"Example:\n"
	"ARCTAN(2)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::ASC(
	"ASC(\"A\") # = 65"
	,
	"ASCII value for character.\n"
	"\n"
	"Example:\n"
	"ASC(\"A\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::CEIL(
	"CEIL(2.8)"
	,
	"Returns the smallest integer greater than or equal to x.\n"
	"\n"
	"Example:\n"
	"CEIL(2.8)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::CHR(
	"CHR$(13) # = carriage return"
	,
	"Convert ASCII value to character.\n"
	"\n"
	"Example:\n"
	"CHR$(13)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::COS(
	"COS(3.1415) # = -1.0"
	,
	"Cosine function (use radians).\n"
	"\n"
	"Example:\n"
	"COS(3.1415)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::DIM(
	"DIM ar(2)"
	,
	"Dimension an array.\n"
	"\n"
	"Example:\n"
	"DIM ar(2)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::DATA(
	"DATA 1, 5, 7"
	,
	"List of data, can be read sequentially using READ.\n"
	"\n"
	"Example:\n"
	"DATA 1, 5, 7\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::ELSE(
	"IF (1 = 1) THEN print 'TRUE!' ELSE print 'FALSE!'"
	,
	"Boolean operator.\n"
	"\n"
	"Example:\n"
	"IF (1 = 1) THEN print 'TRUE!' ELSE print 'FALSE!'\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::EOL(
	"10 PRINT \"line 1\"+EOL$+\"line 2\"+EOL$"
	,
	"Returns the end of line character for whatever operating system you are running.\n"
	"\n"
	"Example:\n"
	"10 PRINT \"line 1\"+EOL$+\"line 2\"+EOL$\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::EXP(
	"exp(1) # = 2.718"
	,
	"Exponent, e^a.\n"
	"\n"
	"Example:\n"
	"exp(1)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::FOR(
	"100 FOR i = 1 TO 22 STEP 2\n"
	"110 print i\n"
	"120 NEXT i"
	,
	"For loop.\n"
	"\n"
	"Example:\n"
	"100 FOR i = 1 TO 22 STEP 2\n"
	"110 print i\n"
	"120 NEXT i\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::NEXT(
	"NEXT i"
	,
	"Ends a nest of instructions and advances FOR loop.\n"
	"\n"
	"Example:\n"
	"NEXT i\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::FLOOR(
	"FLOOR(2.8)"
	,
	"Returns the largest integer less than or equal to x.\n"
	"\n"
	"Example:\n"
	"FLOOR(2.8)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::GOTO(
	"GOTO 100"
	,
	"Go to line number.\n"
	"\n"
	"Example:\n"
	"GOTO 100\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::GOSUB(
	"GOSUB 100"
	,
	"Go to subroutine.\n"
	"\n"
	"Example:\n"
	"GOSUB 100\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::RETURN(
	"RETURN"
	,
	"Return from subroutine (GOSUB).\n"
	"\n"
	"Example:\n"
	"RETURN\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::IF(
	"IF i >= 2 THEN i = 2 ELSE i = 1"
	,
	"If conditional statement.\n"
	"\n"
	"Example:\n"
	"IF i >= 2 THEN i = 2 ELSE i = 1\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::INSTR(
	"INSTR(\"aab\", \"b\") # = 3"
	,
	"Returns the character position of substring within a string, 0 iF not found.\n"
	"\n"
	"Example:\n"
	"INSTR(\"aab\", \"b\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::LEN(
	"LEN(\"ab\") # = 2"
	,
	"Number of characters in string.\n"
	"\n"
	"Example:\n"
	"LEN(\"ab\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::LOG(
	"LOG(10) # = 2.303"
	,
	"Natural logarithm.\n"
	"\n"
	"Example:\n"
	"LOG(10)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::LOG10(
	"LOG10(10) # = 1"
	,
	"Base 10 logarithm.\n"
	"\n"
	"Example:\n"
	"LOG10(10)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::LTRIM(
	"LTRIM(\" ab\") # =ab"
	,
	"Trims white space from beginning of a string.\n"
	"\n"
	"Example:\n"
	"LTRIM(\" ab\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::MID(
	"MID$(\"aab\", 3, 1) # =b"
	,
	"Extracts m characters from position n of a string.\n"
	"\n"
	"Example:\n"
	"MID$(\"aab\", 3, 1)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::MOD(
	"3.2 MOD 2 # = 1.2"
	,
	"Returns remainder of a division.\n"
	"\n"
	"Example:\n"
	"3.2 MOD 2\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::ON_GOTO(
	"ON 2 GOTO 200, 120, 150 # program goes to line 120"
	,
	"If the expression's value, rounded to an integer, is N, go to the Nth line number in the list.\n"
	"\n"
	"Example:\n"
	"ON 2 GOTO 200, 120, 150\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::ON_GOSUB(
	"ON 2 GOSUB 200, 120, 150"
	,
	"If the expression's value, rounded to an integer, is N, go to the subroutine that starts in the Nth line number in the list.\n"
	"\n"
	"Example:\n"
	"ON 2 GOSUB 200, 120, 150\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PAD(
	"PAD(\"a\", 2) + \"b\" # =a b"
	,
	"Extends a string with spaces if n is greater than the length of the string.\n"
	"\n"
	"Example:\n"
	"PAD(\"a\", 2) + \"b\"\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::READ(
	"READ a"
	,
	"Read sequentially from DATA statement.\n"
	"\n"
	"Example:\n"
	"READ a\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::REM(
	"20 REM a = 10^1.2"
	,
	"Indicates that a line is a remark with no effect on the calculations.\n"
	"\n"
	"Example:\n"
	"20 REM a = 10^1.2\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::RESTORE(
	"RESTORE 130"
	,
	"Set pointer to DATA statement in a line for subsequent READ.\n"
	"\n"
	"Example:\n"
	"RESTORE 130\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::RTRIM(
	"RTRIM(\"a \") + \"b\" # =ab"
	,
	"Trims white space from the end of a string.\n"
	"\n"
	"Example:\n"
	"RTRIM(\"a \") + \"b\"\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SGN(
	"SGN(-10) # = -1"
	,
	"Sign of a number.\n"
	"\n"
	"Example:\n"
	"SGN(-10)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SIN(
	"SIN(3.1416 / 2) # = 1.0"
	,
	"Sine function (uses radians).\n"
	"\n"
	"Example:\n"
	"SIN(3.1416 / 2)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SQR(
	"SQR(2) # = 4"
	,
	"Squares a number.\n"
	"\n"
	"Example:\n"
	"SQR(2)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SQRT(
	"SQRT(4) # = 2"
	,
	"Square root of a number.\n"
	"\n"
	"Example:\n"
	"SQRT(4)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::STR(
	"STR$(3.14) # = 3.1400e+00"
	,
	"Convert number to a string (the number is first converted to a real or an integer).\n"
	"\n"
	"Example:\n"
	"STR$(3.14)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TAN(
	"TAN(3.141592654) # = 4.1021e-10"
	,
	"Tangent function (uses radians).\n"
	"\n"
	"Example:\n"
	"TAN(3.141592654)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TRIM(
	"TRIM(\" a \") + \"b\" # =ab"
	,
	"Trims white space from beginning and end of a string.\n"
	"\n"
	"Example:\n"
	"TRIM(\" a \") + \"b\"\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::VAL(
	"VAL(\"1.1\") # = 1.1000e+00"
	,
	"Convert string to number.\n"
	"\n"
	"Example:\n"
	"VAL(\"1.1\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::WHILE(
	"100 WHILE i > 3\n"
	"110 print i\n"
	"120 i = i - 1\n"
	"130 WEND"
	,
	"While loop.\n"
	"\n"
	"Example:\n"
	"100 WHILE i > 3\n"
	"110 print i\n"
	"120 i = i - 1\n"
	"130 WEND\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::WEND(
	"WEND"
	,
	"Indicates the end of a while loop.\n"
	"\n"
	"Example:\n"
	"WEND\n"
	);



// CTreeCtrlPfw

IMPLEMENT_DYNAMIC(CTreeCtrlPfw, baseCTreeCtrlPfw)

CTreeCtrlPfw::CTreeCtrlPfw()
: m_pWndDesc(0)
, KeyDetail(KD_INTERMEDIATE)
{
}

CTreeCtrlPfw::~CTreeCtrlPfw()
{
}


BEGIN_MESSAGE_MAP(CTreeCtrlPfw, CTreeCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CTreeCtrlPfw::OnNMDblclk)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CTreeCtrlPfw::OnTvnSelchanged)
END_MESSAGE_MAP()



// CTreeCtrlPfw message handlers



int CTreeCtrlPfw::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (baseCTreeCtrlPfw::OnCreate(lpCreateStruct) == -1)
		return -1;

	this->FillTree(this->KeyDetail);

	return 0;
}

void CTreeCtrlPfw::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	UNUSED_ALWAYS(pNMHDR);
	// Add your control notification handler code here

// COMMENT: {8/23/2012 11:14:22 PM}	UINT uFlags;
// COMMENT: {8/23/2012 11:14:22 PM}	CTreeCtrlNode node = HitTest(point, &uFlags);
// COMMENT: {8/23/2012 11:14:22 PM}	if ((node != 0) && (TVHT_ONITEM & uFlags))

	CTreeCtrlNode item = this->GetSelectedItem();
	if (/**(item.GetLevel() == 2 || item.GetLevel() == 3) && **/ item.GetData())
	{
		if (CWnd* pWnd = ::AfxGetMainWnd())
		{
			if (CMDIFrameWnd *frm = dynamic_cast<CMDIFrameWnd*>(pWnd))
			{
				if (CMDIChildWnd *child = frm->MDIGetActive())
				{
					if (CRichViewIn *in = dynamic_cast<CRichViewIn*>(child->GetActiveView()))
					{
						CString strText((LPCTSTR)((std::pair<CString, CString>*)item.GetData())->first);
						strText += _T('\n');
						in->GetRichEditCtrl().ReplaceSel(strText, TRUE);
					}
					else
					{
						// sound warning message
						::MessageBeep(MB_ICONEXCLAMATION);
					}
					*pResult = TRUE;
					return;
				}
			}
		}
	}
	*pResult = 0;
}

void CTreeCtrlPfw::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	//CTreeCtrlNode item = this->GetSelectedItem();
	if (this->m_pWndDesc)
	{
		CTreeCtrlNode item(pNMTreeView->itemNew.hItem, this);
		if (/**(item.GetLevel() == 2 || item.GetLevel() == 3) && **/ item.GetData())
		{
			CString strText((LPCTSTR)((std::pair<CString, CString>*)item.GetData())->second);
			strText.Replace(_T("\n"), _T("\r\n"));
			this->m_pWndDesc->SetWindowTextA(strText);
		}
		else
		{
			this->m_pWndDesc->SetWindowTextA(_T(""));
		}
	}
	*pResult = 0;
}

void CTreeCtrlPfw::SetKeywordDetail(KeywordDetail kd)
{
	ASSERT((KD_NOVICE <= this->KeyDetail) && (this->KeyDetail <= KD_PHREAK));
	ASSERT((KD_NOVICE <= kd) && (kd <= KD_PHREAK));
	if ((KD_NOVICE <= kd) && (kd <= KD_PHREAK))
	{
		if (kd != this->KeyDetail)
		{
			this->DeleteAllItems();
			this->FillTree(kd);
			this->KeyDetail = kd;
		}
	}
}

CTreeCtrlPfw::KeywordDetail CTreeCtrlPfw::GetKeywordDetail(void)const
{
	ASSERT((KD_NOVICE <= this->KeyDetail) && (this->KeyDetail <= KD_PHREAK));
	return this->KeyDetail;
}

void CTreeCtrlPfw::FillTree(KeywordDetail kd)
{
	CTreeCtrlNode keys = this->InsertItem(_T("PHREEQC Keywords"));

	if (CTreeCtrlNode start = keys.AddTail(_T("Start")))
	{
		if (kd >= KD_INTERMEDIATE)
		{
			start.AddTail(_T("COPY")).SetData((DWORD)&CTreeCtrlPfw::COPY);
		}

#if 0
		start.AddTail(_T("DATABASE")).SetData((DWORD)&CTreeCtrlPfw::DATABASE);
#endif
		start.AddTail(_T("END")).SetData((DWORD)&CTreeCtrlPfw::END);
		start.AddTail(_T("SAVE")).SetData((DWORD)&CTreeCtrlPfw::SAVE);
		start.AddTail(_T("SOLUTION")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION);
			start.GetLastChild().AddTail(_T("temp")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_temp);
			start.GetLastChild().AddTail(_T("pH")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_pH);
			start.GetLastChild().AddTail(_T("pe")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_pe);
			start.GetLastChild().AddTail(_T("redox")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_redox);
			start.GetLastChild().AddTail(_T("units")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_units);
			start.GetLastChild().AddTail(_T("density")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_density);
			start.GetLastChild().AddTail(_T("-isotope")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_isotope);
			start.GetLastChild().AddTail(_T("-water")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_water);
		if (kd >= KD_INTERMEDIATE)
		{
			start.AddTail(_T("SOLUTION_SPREAD")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPREAD);
				start.GetLastChild().AddTail(_T("temp")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPREAD_temp);
				start.GetLastChild().AddTail(_T("pH")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPREAD_pH);
				start.GetLastChild().AddTail(_T("pe")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPREAD_pe);
				start.GetLastChild().AddTail(_T("redox")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPREAD_redox);
				start.GetLastChild().AddTail(_T("units")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPREAD_units);
				start.GetLastChild().AddTail(_T("density")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPREAD_density);
				start.GetLastChild().AddTail(_T("-isotope")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPREAD_isotope);
				start.GetLastChild().AddTail(_T("-isotope_uncertainty")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPREAD_isotope_uncertainty);
				start.GetLastChild().AddTail(_T("-water")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPREAD_water);
		}
		start.AddTail(_T("TITLE")).SetData((DWORD)&CTreeCtrlPfw::TITLE);
		start.AddTail(_T("USE")).SetData((DWORD)&CTreeCtrlPfw::USE);
		start.GetLastChild();
		start.GetChildAt(0).EnsureVisible();
	}

	if (CTreeCtrlNode chem = keys.AddTail(_T("Chemical reaction")))
	{
		chem.AddTail(_T("EQUILIBRIUM_PHASES")).SetData((DWORD)&CTreeCtrlPfw::EQUILIBRIUM_PHASES);
			chem.GetLastChild().AddTail(_T("-force_equality")).SetData((DWORD)&CTreeCtrlPfw::EQUILIBRIUM_PHASES_force_equality);
		chem.AddTail(_T("EXCHANGE")).SetData((DWORD)&CTreeCtrlPfw::EXCHANGE);
			chem.GetLastChild().AddTail(_T("-equilibrate")).SetData((DWORD)&CTreeCtrlPfw::EXCHANGE_equilibrate);
			chem.GetLastChild().AddTail(_T("-pitzer_exchange_gammas")).SetData((DWORD)&CTreeCtrlPfw::EXCHANGE_pitzer_exchange_gammas);
		chem.AddTail(_T("GAS_PHASE")).SetData((DWORD)&CTreeCtrlPfw::GAS_PHASE);
			chem.GetLastChild().AddTail(_T("-fixed_pressure")).SetData((DWORD)&CTreeCtrlPfw::GAS_PHASE_fixed_pressure);
			chem.GetLastChild().AddTail(_T("-pressure")).SetData((DWORD)&CTreeCtrlPfw::GAS_PHASE_pressure);
			chem.GetLastChild().AddTail(_T("-volume")).SetData((DWORD)&CTreeCtrlPfw::GAS_PHASE_volume);
			chem.GetLastChild().AddTail(_T("-temperature")).SetData((DWORD)&CTreeCtrlPfw::GAS_PHASE_temperature);
			chem.GetLastChild().AddTail(_T("-fixed_volume")).SetData((DWORD)&CTreeCtrlPfw::GAS_PHASE_fixed_volume);
			chem.GetLastChild().AddTail(_T("-equilibrate")).SetData((DWORD)&CTreeCtrlPfw::GAS_PHASE_equilibrate);
		chem.AddTail(_T("INCREMENTAL_REACTIONS")).SetData((DWORD)&CTreeCtrlPfw::INCREMENTAL_REACTIONS);
		if (kd >= KD_INTERMEDIATE)
		{
			chem.AddTail(_T("INVERSE_MODELING")).SetData((DWORD)&CTreeCtrlPfw::INVERSE_MODELING);
				chem.GetLastChild().AddTail(_T("-solutions")).SetData((DWORD)&CTreeCtrlPfw::INVERSE_MODELING_solutions);
				chem.GetLastChild().AddTail(_T("-uncertainty")).SetData((DWORD)&CTreeCtrlPfw::INVERSE_MODELING_uncertainty);
				chem.GetLastChild().AddTail(_T("-phases")).SetData((DWORD)&CTreeCtrlPfw::INVERSE_MODELING_phases);
				chem.GetLastChild().AddTail(_T("-balances")).SetData((DWORD)&CTreeCtrlPfw::INVERSE_MODELING_balances);
				chem.GetLastChild().AddTail(_T("-isotopes")).SetData((DWORD)&CTreeCtrlPfw::INVERSE_MODELING_isotopes);
				chem.GetLastChild().AddTail(_T("-range")).SetData((DWORD)&CTreeCtrlPfw::INVERSE_MODELING_range);
				chem.GetLastChild().AddTail(_T("-minimal")).SetData((DWORD)&CTreeCtrlPfw::INVERSE_MODELING_minimal);
				chem.GetLastChild().AddTail(_T("-tolerance")).SetData((DWORD)&CTreeCtrlPfw::INVERSE_MODELING_tolerance);
				chem.GetLastChild().AddTail(_T("-force_solutions")).SetData((DWORD)&CTreeCtrlPfw::INVERSE_MODELING_force_solutions);
				chem.GetLastChild().AddTail(_T("-uncertainty_water")).SetData((DWORD)&CTreeCtrlPfw::INVERSE_MODELING_uncertainty_water);
				chem.GetLastChild().AddTail(_T("-mineral_water")).SetData((DWORD)&CTreeCtrlPfw::INVERSE_MODELING_mineral_water);
				chem.GetLastChild().AddTail(_T("-multiple_precision")).SetData((DWORD)&CTreeCtrlPfw::INVERSE_MODELING_multiple_precision);
				chem.GetLastChild().AddTail(_T("-mp_tolerance")).SetData((DWORD)&CTreeCtrlPfw::INVERSE_MODELING_mp_tolerance);
				chem.GetLastChild().AddTail(_T("-censor_mp")).SetData((DWORD)&CTreeCtrlPfw::INVERSE_MODELING_censor_mp);
				chem.GetLastChild().AddTail(_T("-lon_netpath")).SetData((DWORD)&CTreeCtrlPfw::INVERSE_MODELING_lon_netpath);
				chem.GetLastChild().AddTail(_T("-pat_netpath")).SetData((DWORD)&CTreeCtrlPfw::INVERSE_MODELING_pat_netpath);
		}
		chem.AddTail(_T("REACTION")).SetData((DWORD)&CTreeCtrlPfw::REACTION);
		chem.AddTail(_T("REACTION_TEMPERATURE")).SetData((DWORD)&CTreeCtrlPfw::REACTION_TEMPERATURE);
		chem.AddTail(_T("SURFACE")).SetData((DWORD)&CTreeCtrlPfw::SURFACE);
			chem.GetLastChild().AddTail(_T("-equilibrate")).SetData((DWORD)&CTreeCtrlPfw::SURFACE_equilibrate);
			chem.GetLastChild().AddTail(_T("-capacitance")).SetData((DWORD)&CTreeCtrlPfw::SURFACE_capacitance);
			chem.GetLastChild().AddTail(_T("-cd_music")).SetData((DWORD)&CTreeCtrlPfw::SURFACE_cd_music);
			chem.GetLastChild().AddTail(_T("-diffuse_layer")).SetData((DWORD)&CTreeCtrlPfw::SURFACE_diffuse_layer);
			chem.GetLastChild().AddTail(_T("-donnan")).SetData((DWORD)&CTreeCtrlPfw::SURFACE_donnan);
			chem.GetLastChild().AddTail(_T("-only_counter_ions")).SetData((DWORD)&CTreeCtrlPfw::SURFACE_only_counter_ions);
			chem.GetLastChild().AddTail(_T("-no_edl")).SetData((DWORD)&CTreeCtrlPfw::SURFACE_no_edl);
			chem.GetLastChild().AddTail(_T("-sites_units")).SetData((DWORD)&CTreeCtrlPfw::SURFACE_sites_units);
	}

	if (CTreeCtrlNode phys = keys.AddTail(_T("Physical action")))
	{
		if (kd >= KD_INTERMEDIATE)
		{
			phys.AddTail(_T("ADVECTION")).SetData((DWORD)&CTreeCtrlPfw::ADVECTION);
				phys.GetLastChild().AddTail(_T("-cells")).SetData((DWORD)&CTreeCtrlPfw::ADVECTION_cells);
				phys.GetLastChild().AddTail(_T("-shifts")).SetData((DWORD)&CTreeCtrlPfw::ADVECTION_shifts);
				phys.GetLastChild().AddTail(_T("-time_step")).SetData((DWORD)&CTreeCtrlPfw::ADVECTION_time_step);
				phys.GetLastChild().AddTail(_T("-initial_time")).SetData((DWORD)&CTreeCtrlPfw::ADVECTION_initial_time);
				phys.GetLastChild().AddTail(_T("-print_cells")).SetData((DWORD)&CTreeCtrlPfw::ADVECTION_print_cells);
				phys.GetLastChild().AddTail(_T("-print_frequency")).SetData((DWORD)&CTreeCtrlPfw::ADVECTION_print_frequency);
				phys.GetLastChild().AddTail(_T("-punch_cells")).SetData((DWORD)&CTreeCtrlPfw::ADVECTION_punch_cells);
				phys.GetLastChild().AddTail(_T("-punch_frequency")).SetData((DWORD)&CTreeCtrlPfw::ADVECTION_punch_frequency);
				phys.GetLastChild().AddTail(_T("-warnings")).SetData((DWORD)&CTreeCtrlPfw::ADVECTION_warnings);
		}
		phys.AddTail(_T("MIX")).SetData((DWORD)&CTreeCtrlPfw::MIX);
		if (kd >= KD_INTERMEDIATE)
		{
			phys.AddTail(_T("TRANSPORT")).SetData((DWORD)&CTreeCtrlPfw::TRANSPORT);
				phys.GetLastChild().AddTail(_T("-cells")).SetData((DWORD)&CTreeCtrlPfw::TRANSPORT_cells);
				phys.GetLastChild().AddTail(_T("-shifts")).SetData((DWORD)&CTreeCtrlPfw::TRANSPORT_shifts);
				phys.GetLastChild().AddTail(_T("-time_step")).SetData((DWORD)&CTreeCtrlPfw::TRANSPORT_time_step);
				phys.GetLastChild().AddTail(_T("-flow_direction")).SetData((DWORD)&CTreeCtrlPfw::TRANSPORT_flow_direction);
				phys.GetLastChild().AddTail(_T("-boundary_conditions")).SetData((DWORD)&CTreeCtrlPfw::TRANSPORT_boundary_conditions);
				phys.GetLastChild().AddTail(_T("-lengths")).SetData((DWORD)&CTreeCtrlPfw::TRANSPORT_lengths);
				phys.GetLastChild().AddTail(_T("-dispersivities")).SetData((DWORD)&CTreeCtrlPfw::TRANSPORT_dispersivities);
				phys.GetLastChild().AddTail(_T("-correct_disp")).SetData((DWORD)&CTreeCtrlPfw::TRANSPORT_correct_disp);
				phys.GetLastChild().AddTail(_T("-diffusion_coefficient")).SetData((DWORD)&CTreeCtrlPfw::TRANSPORT_diffusion_coefficient);
				phys.GetLastChild().AddTail(_T("-stagnant")).SetData((DWORD)&CTreeCtrlPfw::TRANSPORT_stagnant);
				phys.GetLastChild().AddTail(_T("-thermal_diffusion")).SetData((DWORD)&CTreeCtrlPfw::TRANSPORT_thermal_diffusion);
				phys.GetLastChild().AddTail(_T("-initial_time")).SetData((DWORD)&CTreeCtrlPfw::TRANSPORT_initial_time);
				phys.GetLastChild().AddTail(_T("-multi_d")).SetData((DWORD)&CTreeCtrlPfw::TRANSPORT_multi_d);
				phys.GetLastChild().AddTail(_T("-interlayer_d")).SetData((DWORD)&CTreeCtrlPfw::TRANSPORT_interlayer_d);
				phys.GetLastChild().AddTail(_T("-print_cells")).SetData((DWORD)&CTreeCtrlPfw::TRANSPORT_print_cells);
				phys.GetLastChild().AddTail(_T("-print_frequency")).SetData((DWORD)&CTreeCtrlPfw::TRANSPORT_print_frequency);
				phys.GetLastChild().AddTail(_T("-punch_cells")).SetData((DWORD)&CTreeCtrlPfw::TRANSPORT_punch_cells);
				phys.GetLastChild().AddTail(_T("-punch_frequency")).SetData((DWORD)&CTreeCtrlPfw::TRANSPORT_punch_frequency);
				phys.GetLastChild().AddTail(_T("-warnings")).SetData((DWORD)&CTreeCtrlPfw::TRANSPORT_warnings);
				phys.GetLastChild().AddTail(_T("-dump")).SetData((DWORD)&CTreeCtrlPfw::TRANSPORT_dump);
				phys.GetLastChild().AddTail(_T("-dump_frequency")).SetData((DWORD)&CTreeCtrlPfw::TRANSPORT_dump_frequency);
				phys.GetLastChild().AddTail(_T("-dump_restart")).SetData((DWORD)&CTreeCtrlPfw::TRANSPORT_dump_restart);
		}
	}

	if (kd >= KD_INTERMEDIATE)
	{
		CTreeCtrlNode output = keys.AddTail(_T("Output"));
		output.AddTail(_T("PRINT")).SetData((DWORD)&CTreeCtrlPfw::PRINT);
			output.GetLastChild().AddTail(_T("-alkalinity")).SetData((DWORD)&CTreeCtrlPfw::PRINT_alkalinity);
			output.GetLastChild().AddTail(_T("-echo_input")).SetData((DWORD)&CTreeCtrlPfw::PRINT_echo_input);
			output.GetLastChild().AddTail(_T("-Eh")).SetData((DWORD)&CTreeCtrlPfw::PRINT_Eh);
			output.GetLastChild().AddTail(_T("-equilibrium_phases")).SetData((DWORD)&CTreeCtrlPfw::PRINT_equilibrium_phases);
			output.GetLastChild().AddTail(_T("-exchange")).SetData((DWORD)&CTreeCtrlPfw::PRINT_exchange);
			output.GetLastChild().AddTail(_T("-gas_phase")).SetData((DWORD)&CTreeCtrlPfw::PRINT_gas_phase);
			output.GetLastChild().AddTail(_T("-headings")).SetData((DWORD)&CTreeCtrlPfw::PRINT_headings);
			output.GetLastChild().AddTail(_T("-inverse_modeling")).SetData((DWORD)&CTreeCtrlPfw::PRINT_inverse_modeling);
			output.GetLastChild().AddTail(_T("-kinetics")).SetData((DWORD)&CTreeCtrlPfw::PRINT_kinetics);
			output.GetLastChild().AddTail(_T("-other")).SetData((DWORD)&CTreeCtrlPfw::PRINT_other);
			output.GetLastChild().AddTail(_T("-reset")).SetData((DWORD)&CTreeCtrlPfw::PRINT_reset);
			output.GetLastChild().AddTail(_T("-saturation_indices")).SetData((DWORD)&CTreeCtrlPfw::PRINT_saturation_indices);
			output.GetLastChild().AddTail(_T("-solid_solutions")).SetData((DWORD)&CTreeCtrlPfw::PRINT_solid_solutions);
			output.GetLastChild().AddTail(_T("-species")).SetData((DWORD)&CTreeCtrlPfw::PRINT_species);
			output.GetLastChild().AddTail(_T("-surface")).SetData((DWORD)&CTreeCtrlPfw::PRINT_surface);
			output.GetLastChild().AddTail(_T("-totals")).SetData((DWORD)&CTreeCtrlPfw::PRINT_totals);
			output.GetLastChild().AddTail(_T("-user_print")).SetData((DWORD)&CTreeCtrlPfw::PRINT_user_print);
			output.GetLastChild().AddTail(_T("-selected_output")).SetData((DWORD)&CTreeCtrlPfw::PRINT_selected_output);
			output.GetLastChild().AddTail(_T("-status")).SetData((DWORD)&CTreeCtrlPfw::PRINT_status);
			output.GetLastChild().AddTail(_T("-warnings")).SetData((DWORD)&CTreeCtrlPfw::PRINT_warnings);
			output.GetLastChild().AddTail(_T("-initial_isotopes")).SetData((DWORD)&CTreeCtrlPfw::PRINT_initial_isotopes);
			output.GetLastChild().AddTail(_T("-isotope_ratios")).SetData((DWORD)&CTreeCtrlPfw::PRINT_isotope_ratios);
			output.GetLastChild().AddTail(_T("-isotope_alphas")).SetData((DWORD)&CTreeCtrlPfw::PRINT_isotope_alphas);
			output.GetLastChild().AddTail(_T("-censor_species")).SetData((DWORD)&CTreeCtrlPfw::PRINT_censor_species);
		output.AddTail(_T("SELECTED_OUTPUT")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT);
			output.GetLastChild().AddTail(_T("-file")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_file);
			output.GetLastChild().AddTail(_T("-selected_out")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_selected_out);
			output.GetLastChild().AddTail(_T("-user_punch")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_user_punch);
			output.GetLastChild().AddTail(_T("-high_precision")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_high_precision);
			output.GetLastChild().AddTail(_T("-reset")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_reset);
			output.GetLastChild().AddTail(_T("-simulation")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_simulation);
			output.GetLastChild().AddTail(_T("-state")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_state);
			output.GetLastChild().AddTail(_T("-solution")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_solution);
			output.GetLastChild().AddTail(_T("-distance")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_distance);
			output.GetLastChild().AddTail(_T("-time")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_time);
			output.GetLastChild().AddTail(_T("-step")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_step);
			output.GetLastChild().AddTail(_T("-pH")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_pH);
			output.GetLastChild().AddTail(_T("-pe")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_pe);
			output.GetLastChild().AddTail(_T("-reaction")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_reaction);
			output.GetLastChild().AddTail(_T("-temperature")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_temperature);
			output.GetLastChild().AddTail(_T("-alkalinity")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_alkalinity);
			output.GetLastChild().AddTail(_T("-ionic_strength")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_ionic_strength);
			output.GetLastChild().AddTail(_T("-water")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_water);
			output.GetLastChild().AddTail(_T("-charge_balance")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_charge_balance);
			output.GetLastChild().AddTail(_T("-percent_error")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_percent_error);
			output.GetLastChild().AddTail(_T("-totals")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_totals);
			output.GetLastChild().AddTail(_T("-molalities")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_molalities);
			output.GetLastChild().AddTail(_T("-activities")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_activities);
			output.GetLastChild().AddTail(_T("-equilibrium_phases")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_equilibrium_phases);
			output.GetLastChild().AddTail(_T("-saturation_indices")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_saturation_indices);
			output.GetLastChild().AddTail(_T("-gases")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_gases);
			output.GetLastChild().AddTail(_T("-kinetic_reactants")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_kinetic_reactants);
			output.GetLastChild().AddTail(_T("-solid_solutions")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_solid_solutions);
			output.GetLastChild().AddTail(_T("-inverse_modeling")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_inverse_modeling);
			output.GetLastChild().AddTail(_T("-calculate_values")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_calculate_values);
			output.GetLastChild().AddTail(_T("-isotopes")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT_isotopes);
		output.AddTail(_T("USER_GRAPH")).SetData((DWORD)&CTreeCtrlPfw::USER_GRAPH);
			output.GetLastChild().AddTail(_T("-headings")).SetData((DWORD)&CTreeCtrlPfw::USER_GRAPH_headings);
			output.GetLastChild().AddTail(_T("-axis_scale")).SetData((DWORD)&CTreeCtrlPfw::USER_GRAPH_axis_scale);
			output.GetLastChild().AddTail(_T("-axis_titles")).SetData((DWORD)&CTreeCtrlPfw::USER_GRAPH_axis_titles);
			output.GetLastChild().AddTail(_T("-chart_title")).SetData((DWORD)&CTreeCtrlPfw::USER_GRAPH_chart_title);
			output.GetLastChild().AddTail(_T("-connect_simulations")).SetData((DWORD)&CTreeCtrlPfw::USER_GRAPH_connect_simulations);
			output.GetLastChild().AddTail(_T("-grid_offset")).SetData((DWORD)&CTreeCtrlPfw::USER_GRAPH_grid_offset);
			output.GetLastChild().AddTail(_T("-initial_solutions")).SetData((DWORD)&CTreeCtrlPfw::USER_GRAPH_initial_solutions);
			output.GetLastChild().AddTail(_T("-plot_concentration_vs")).SetData((DWORD)&CTreeCtrlPfw::USER_GRAPH_plot_concentration_vs);
			output.GetLastChild().AddTail(_T("-plot_csv_file")).SetData((DWORD)&CTreeCtrlPfw::USER_GRAPH_plot_csv_file);
			output.GetLastChild().AddTail(_T("-start")).SetData((DWORD)&CTreeCtrlPfw::USER_GRAPH_start);
			output.GetLastChild().AddTail(_T("-end")).SetData((DWORD)&CTreeCtrlPfw::USER_GRAPH_end);
		output.AddTail(_T("USER_PRINT")).SetData((DWORD)&CTreeCtrlPfw::USER_PRINT);
			output.GetLastChild().AddTail(_T("-start")).SetData((DWORD)&CTreeCtrlPfw::USER_PRINT_start);
			output.GetLastChild().AddTail(_T("-end")).SetData((DWORD)&CTreeCtrlPfw::USER_PRINT_end);
		output.AddTail(_T("USER_PUNCH")).SetData((DWORD)&CTreeCtrlPfw::USER_PUNCH);
			output.GetLastChild().AddTail(_T("-headings")).SetData((DWORD)&CTreeCtrlPfw::USER_PUNCH_headings);
			output.GetLastChild().AddTail(_T("-start")).SetData((DWORD)&CTreeCtrlPfw::USER_PUNCH_start);
			output.GetLastChild().AddTail(_T("-end")).SetData((DWORD)&CTreeCtrlPfw::USER_PUNCH_end);
	}

	// Database
	if (kd >= KD_PHREAK)
	{
		CTreeCtrlNode db = keys.AddTail(_T("Database"));
		db.AddTail(_T("EXCHANGE_MASTER_SPECIES")).SetData((DWORD)&CTreeCtrlPfw::EXCHANGE_MASTER_SPECIES);
		db.AddTail(_T("EXCHANGE_SPECIES")).SetData((DWORD)&CTreeCtrlPfw::EXCHANGE_SPECIES);
			db.GetLastChild().AddTail(_T("-log_k")).SetData((DWORD)&CTreeCtrlPfw::EXCHANGE_SPECIES_log_k);
			db.GetLastChild().AddTail(_T("-delta_h")).SetData((DWORD)&CTreeCtrlPfw::EXCHANGE_SPECIES_delta_h);
			db.GetLastChild().AddTail(_T("-analytical_expression")).SetData((DWORD)&CTreeCtrlPfw::EXCHANGE_SPECIES_analytical_expression);
			db.GetLastChild().AddTail(_T("-gamma")).SetData((DWORD)&CTreeCtrlPfw::EXCHANGE_SPECIES_gamma);
			db.GetLastChild().AddTail(_T("-davies")).SetData((DWORD)&CTreeCtrlPfw::EXCHANGE_SPECIES_davies);
			db.GetLastChild().AddTail(_T("-no_check")).SetData((DWORD)&CTreeCtrlPfw::EXCHANGE_SPECIES_no_check);
			db.GetLastChild().AddTail(_T("-mole_balance")).SetData((DWORD)&CTreeCtrlPfw::EXCHANGE_SPECIES_mole_balance);
			db.GetLastChild().AddTail(_T("-add_constant")).SetData((DWORD)&CTreeCtrlPfw::EXCHANGE_SPECIES_add_constant);
			db.GetLastChild().AddTail(_T("-add_logk")).SetData((DWORD)&CTreeCtrlPfw::EXCHANGE_SPECIES_add_logk);
		db.AddTail(_T("PHASES")).SetData((DWORD)&CTreeCtrlPfw::PHASES);
			db.GetLastChild().AddTail(_T("-log_k")).SetData((DWORD)&CTreeCtrlPfw::PHASES_log_k);
			db.GetLastChild().AddTail(_T("-delta_h")).SetData((DWORD)&CTreeCtrlPfw::PHASES_delta_h);
			db.GetLastChild().AddTail(_T("-analytical_expression")).SetData((DWORD)&CTreeCtrlPfw::PHASES_analytical_expression);
			db.GetLastChild().AddTail(_T("-no_check")).SetData((DWORD)&CTreeCtrlPfw::PHASES_no_check);
			db.GetLastChild().AddTail(_T("-add_constant")).SetData((DWORD)&CTreeCtrlPfw::PHASES_add_constant);
			db.GetLastChild().AddTail(_T("-add_logk")).SetData((DWORD)&CTreeCtrlPfw::PHASES_add_logk);
		db.AddTail(_T("PITZER")).SetData((DWORD)&CTreeCtrlPfw::PITZER);
			db.GetLastChild().AddTail(_T("-ALPHAS")).SetData((DWORD)&CTreeCtrlPfw::PITZER_ALPHAS);
			db.GetLastChild().AddTail(_T("-B0")).SetData((DWORD)&CTreeCtrlPfw::PITZER_B0);
			db.GetLastChild().AddTail(_T("-B1")).SetData((DWORD)&CTreeCtrlPfw::PITZER_B1);
			db.GetLastChild().AddTail(_T("-B2")).SetData((DWORD)&CTreeCtrlPfw::PITZER_B2);
			db.GetLastChild().AddTail(_T("-C0")).SetData((DWORD)&CTreeCtrlPfw::PITZER_C0);
			db.GetLastChild().AddTail(_T("-LAMDA")).SetData((DWORD)&CTreeCtrlPfw::PITZER_LAMDA);
			db.GetLastChild().AddTail(_T("-MacInnes")).SetData((DWORD)&CTreeCtrlPfw::PITZER_MacInnes);
			db.GetLastChild().AddTail(_T("-PSI")).SetData((DWORD)&CTreeCtrlPfw::PITZER_PSI);
			db.GetLastChild().AddTail(_T("-redox")).SetData((DWORD)&CTreeCtrlPfw::PITZER_redox);
			db.GetLastChild().AddTail(_T("-THETA")).SetData((DWORD)&CTreeCtrlPfw::PITZER_THETA);
			db.GetLastChild().AddTail(_T("-use_etheta")).SetData((DWORD)&CTreeCtrlPfw::PITZER_use_etheta);
			db.GetLastChild().AddTail(_T("-ZETA")).SetData((DWORD)&CTreeCtrlPfw::PITZER_ZETA);
		db.AddTail(_T("SIT")).SetData((DWORD)&CTreeCtrlPfw::SIT);
		db.AddTail(_T("RATES")).SetData((DWORD)&CTreeCtrlPfw::RATES);
			db.GetLastChild().AddTail(_T("-start")).SetData((DWORD)&CTreeCtrlPfw::RATES_start);
			db.GetLastChild().AddTail(_T("-end")).SetData((DWORD)&CTreeCtrlPfw::RATES_end);
		db.AddTail(_T("SOLUTION_MASTER_SPECIES")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_MASTER_SPECIES);
		db.AddTail(_T("SOLUTION_SPECIES")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPECIES);
			db.GetLastChild().AddTail(_T("-log_k")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPECIES_log_k);
			db.GetLastChild().AddTail(_T("-delta_h")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPECIES_delta_h);
			db.GetLastChild().AddTail(_T("-analytical_expression")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPECIES_analytical_expression);
			db.GetLastChild().AddTail(_T("-gamma")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPECIES_gamma);
			db.GetLastChild().AddTail(_T("-no_check")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPECIES_no_check);
			db.GetLastChild().AddTail(_T("-mole_balance")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPECIES_mole_balance);
			db.GetLastChild().AddTail(_T("-llnl_gamma")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPECIES_llnl_gamma);
			db.GetLastChild().AddTail(_T("-co2_llnl_gamma")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPECIES_co2_llnl_gamma);
			db.GetLastChild().AddTail(_T("-dw")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPECIES_dw);
			db.GetLastChild().AddTail(_T("-erm_ddl")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPECIES_erm_ddl);
			db.GetLastChild().AddTail(_T("-Millero")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPECIES_Millero);
			db.GetLastChild().AddTail(_T("-activity_water")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPECIES_activity_water);
			db.GetLastChild().AddTail(_T("-add_constant")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPECIES_add_constant);
			db.GetLastChild().AddTail(_T("-add_logk")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPECIES_add_logk);
		db.AddTail(_T("SURFACE_MASTER_SPECIES")).SetData((DWORD)&CTreeCtrlPfw::SURFACE_MASTER_SPECIES);
		db.AddTail(_T("SURFACE_SPECIES")).SetData((DWORD)&CTreeCtrlPfw::SURFACE_SPECIES);
			db.GetLastChild().AddTail(_T("-log_k")).SetData((DWORD)&CTreeCtrlPfw::SURFACE_SPECIES_log_k);
			db.GetLastChild().AddTail(_T("-delta_h")).SetData((DWORD)&CTreeCtrlPfw::SURFACE_SPECIES_delta_h);
			db.GetLastChild().AddTail(_T("-analytical_expression")).SetData((DWORD)&CTreeCtrlPfw::SURFACE_SPECIES_analytical_expression);
			db.GetLastChild().AddTail(_T("-cd_music")).SetData((DWORD)&CTreeCtrlPfw::SURFACE_SPECIES_cd_music);
			db.GetLastChild().AddTail(_T("-no_check")).SetData((DWORD)&CTreeCtrlPfw::SURFACE_SPECIES_no_check);
			db.GetLastChild().AddTail(_T("-mole_balance")).SetData((DWORD)&CTreeCtrlPfw::SURFACE_SPECIES_mole_balance);
			db.GetLastChild().AddTail(_T("-add_constant")).SetData((DWORD)&CTreeCtrlPfw::SURFACE_SPECIES_add_constant);
			db.GetLastChild().AddTail(_T("-add_logk")).SetData((DWORD)&CTreeCtrlPfw::SURFACE_SPECIES_add_logk);
	}

	// Miscellaneous
	if (kd >= KD_PHREAK)
	{
		CTreeCtrlNode misc = keys.AddTail(_T("Miscellaneous"));	
		misc.AddTail(_T("CALCULATE_VALUES")).SetData((DWORD)&CTreeCtrlPfw::CALCULATE_VALUES);
			misc.GetLastChild().AddTail(_T("-start")).SetData((DWORD)&CTreeCtrlPfw::CALCULATE_VALUES_start);
			misc.GetLastChild().AddTail(_T("-end")).SetData((DWORD)&CTreeCtrlPfw::CALCULATE_VALUES_end);
		misc.AddTail(_T("ISOTOPES")).SetData((DWORD)&CTreeCtrlPfw::ISOTOPES);
			misc.GetLastChild().AddTail(_T("-isotope")).SetData((DWORD)&CTreeCtrlPfw::ISOTOPES_isotope);
		misc.AddTail(_T("ISOTOPE_ALPHAS")).SetData((DWORD)&CTreeCtrlPfw::ISOTOPE_ALPHAS);
		misc.AddTail(_T("ISOTOPE_RATIOS")).SetData((DWORD)&CTreeCtrlPfw::ISOTOPE_RATIOS);
		misc.AddTail(_T("KNOBS")).SetData((DWORD)&CTreeCtrlPfw::KNOBS);
			misc.GetLastChild().AddTail(_T("-iterations")).SetData((DWORD)&CTreeCtrlPfw::KNOBS_iterations);
			misc.GetLastChild().AddTail(_T("-convergence_tolerance")).SetData((DWORD)&CTreeCtrlPfw::KNOBS_convergence_tolerance);
			misc.GetLastChild().AddTail(_T("-tolerance")).SetData((DWORD)&CTreeCtrlPfw::KNOBS_tolerance);
			misc.GetLastChild().AddTail(_T("-step_size")).SetData((DWORD)&CTreeCtrlPfw::KNOBS_step_size);
			misc.GetLastChild().AddTail(_T("-pe_step_size")).SetData((DWORD)&CTreeCtrlPfw::KNOBS_pe_step_size);
			misc.GetLastChild().AddTail(_T("-diagonal_scale")).SetData((DWORD)&CTreeCtrlPfw::KNOBS_diagonal_scale);
			misc.GetLastChild().AddTail(_T("-debug_diffuse_layer")).SetData((DWORD)&CTreeCtrlPfw::KNOBS_debug_diffuse_layer);
			misc.GetLastChild().AddTail(_T("-debug_inverse")).SetData((DWORD)&CTreeCtrlPfw::KNOBS_debug_inverse);
			misc.GetLastChild().AddTail(_T("-debug_model")).SetData((DWORD)&CTreeCtrlPfw::KNOBS_debug_model);
			misc.GetLastChild().AddTail(_T("-debug_prep")).SetData((DWORD)&CTreeCtrlPfw::KNOBS_debug_prep);
			misc.GetLastChild().AddTail(_T("-debug_set")).SetData((DWORD)&CTreeCtrlPfw::KNOBS_debug_set);
			misc.GetLastChild().AddTail(_T("-logfile")).SetData((DWORD)&CTreeCtrlPfw::KNOBS_logfile);
		misc.AddTail(_T("LLNL_AQUEOUS_MODEL_PARAMETERS")).SetData((DWORD)&CTreeCtrlPfw::LLNL_AQUEOUS_MODEL_PARAMETERS);
			misc.GetLastChild().AddTail(_T("-temperatures")).SetData((DWORD)&CTreeCtrlPfw::LLNL_AQUEOUS_MODEL_PARAMETERS_temperatures);
			misc.GetLastChild().AddTail(_T("-dh_a")).SetData((DWORD)&CTreeCtrlPfw::LLNL_AQUEOUS_MODEL_PARAMETERS_dh_a);
			misc.GetLastChild().AddTail(_T("-dh_b")).SetData((DWORD)&CTreeCtrlPfw::LLNL_AQUEOUS_MODEL_PARAMETERS_dh_b);
			misc.GetLastChild().AddTail(_T("-bdot")).SetData((DWORD)&CTreeCtrlPfw::LLNL_AQUEOUS_MODEL_PARAMETERS_bdot);
			misc.GetLastChild().AddTail(_T("-co2_coefs")).SetData((DWORD)&CTreeCtrlPfw::LLNL_AQUEOUS_MODEL_PARAMETERS_co2_coefs);
		misc.AddTail(_T("NAMED_EXPRESSIONS")).SetData((DWORD)&CTreeCtrlPfw::NAMED_EXPRESSIONS);
	}

	if (kd >= KD_INTERMEDIATE)
	{
		CTreeCtrlNode pbasic = this->InsertItem(_T("PHREEQC BASIC statements"));
		pbasic.AddTail(_T("ACT(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::ACT);
		pbasic.AddTail(_T("ALK")).SetData((DWORD)&CTreeCtrlPfw::ALK);
		pbasic.AddTail(_T("CALC_VALUE(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::CALC_VALUE);
		pbasic.AddTail(_T("CELL_NO")).SetData((DWORD)&CTreeCtrlPfw::CELL_NO);
		pbasic.AddTail(_T("CHANGE_POR(.., ..)")).SetData((DWORD)&CTreeCtrlPfw::CHANGE_POR);
		pbasic.AddTail(_T("CHANGE_SURF(\"..\", .., \"..\", .., ..)")).SetData((DWORD)&CTreeCtrlPfw::CHANGE_SURF);
		pbasic.AddTail(_T("CHARGE_BALANCE")).SetData((DWORD)&CTreeCtrlPfw::CHARGE_BALANCE);
		pbasic.AddTail(_T("DESCRIPTION")).SetData((DWORD)&CTreeCtrlPfw::DESCRIPTION);
		pbasic.AddTail(_T("DIST")).SetData((DWORD)&CTreeCtrlPfw::DIST);
		pbasic.AddTail(_T("EDL(\"..\", \"..\")")).SetData((DWORD)&CTreeCtrlPfw::EDL);
		pbasic.AddTail(_T("EQUI(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::EQUI);
		pbasic.AddTail(_T("EXISTS(..[, .., ..])")).SetData((DWORD)&CTreeCtrlPfw::EXISTS);
		pbasic.AddTail(_T("GAS(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::GAS);
		pbasic.AddTail(_T("GAMMA(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::GAMMA);
		pbasic.AddTail(_T("GET(..[, .., ..])")).SetData((DWORD)&CTreeCtrlPfw::GET);
		pbasic.AddTail(_T("GET_POR(..)")).SetData((DWORD)&CTreeCtrlPfw::GET_POR);
		pbasic.AddTail(_T("GRAPH_X")).SetData((DWORD)&CTreeCtrlPfw::GRAPH_X);
		pbasic.AddTail(_T("GRAPH_Y")).SetData((DWORD)&CTreeCtrlPfw::GRAPH_Y);
		pbasic.AddTail(_T("GRAPH_SY")).SetData((DWORD)&CTreeCtrlPfw::GRAPH_SY);
		pbasic.AddTail(_T("ISO(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::ISO);
		pbasic.AddTail(_T("ISO_UNITS(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::ISO_UNITS);
		pbasic.AddTail(_T("KIN(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::KIN);
		pbasic.AddTail(_T("LA(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::LA);
		pbasic.AddTail(_T("LG(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::LG);
		pbasic.AddTail(_T("LK_NAMED(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::LK_NAMED);
		pbasic.AddTail(_T("LK_PHASE(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::LK_PHASE);
		pbasic.AddTail(_T("LK_SPECIES(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::LK_SPECIES);
		pbasic.AddTail(_T("LM(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::LM);
		pbasic.AddTail(_T("M")).SetData((DWORD)&CTreeCtrlPfw::M);
		pbasic.AddTail(_T("M0")).SetData((DWORD)&CTreeCtrlPfw::M0);
		pbasic.AddTail(_T("MISC1(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::MISC1);
		pbasic.AddTail(_T("MISC2(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::MISC2);
		pbasic.AddTail(_T("MOL(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::MOL);
		pbasic.AddTail(_T("MU")).SetData((DWORD)&CTreeCtrlPfw::MU);
		pbasic.AddTail(_T("OSMOTIC")).SetData((DWORD)&CTreeCtrlPfw::OSMOTIC);
		pbasic.AddTail(_T("PARM(..)")).SetData((DWORD)&CTreeCtrlPfw::PARM);
		pbasic.AddTail(_T("PERCENT_ERROR")).SetData((DWORD)&CTreeCtrlPfw::PERCENT_ERROR);
		pbasic.AddTail(_T("PRINT")).SetData((DWORD)&CTreeCtrlPfw::BASIC_PRINT);
		pbasic.AddTail(_T("PUNCH")).SetData((DWORD)&CTreeCtrlPfw::PUNCH);
		pbasic.AddTail(_T("PUT(x, ..[, .., ..])")).SetData((DWORD)&CTreeCtrlPfw::PUT);
		pbasic.AddTail(_T("RHO_m")).SetData((DWORD)&CTreeCtrlPfw::RHO_m);
		pbasic.AddTail(_T("RHO_v")).SetData((DWORD)&CTreeCtrlPfw::RHO_v);
		pbasic.AddTail(_T("RXN")).SetData((DWORD)&CTreeCtrlPfw::RXN);
		pbasic.AddTail(_T("SAVE ..")).SetData((DWORD)&CTreeCtrlPfw::BASIC_SAVE);
		pbasic.AddTail(_T("SC")).SetData((DWORD)&CTreeCtrlPfw::SC);
		pbasic.AddTail(_T("SI(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::SI);
		pbasic.AddTail(_T("SIM_NO")).SetData((DWORD)&CTreeCtrlPfw::SIM_NO);
		pbasic.AddTail(_T("SIM_TIME")).SetData((DWORD)&CTreeCtrlPfw::SIM_TIME);
		pbasic.AddTail(_T("SR(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::SR);
		pbasic.AddTail(_T("STEP_NO")).SetData((DWORD)&CTreeCtrlPfw::STEP_NO);
		pbasic.AddTail(_T("SUM_GAS(\"{..}\",\"..\")")).SetData((DWORD)&CTreeCtrlPfw::SUM_GAS);
		pbasic.AddTail(_T("SUM_SPECIES(\"{..}\",\"..\")")).SetData((DWORD)&CTreeCtrlPfw::SUM_SPECIES);
		pbasic.AddTail(_T("SUM_S_S(\"..\",\"..\")")).SetData((DWORD)&CTreeCtrlPfw::SUM_S_S);
		pbasic.AddTail(_T("SURF(\"..\", \"..\") ")).SetData((DWORD)&CTreeCtrlPfw::SURF);
		pbasic.AddTail(_T("SYS(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::SYS);
		pbasic.AddTail(_T("S_S(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::S_S);
		pbasic.AddTail(_T("TC")).SetData((DWORD)&CTreeCtrlPfw::TC);
		pbasic.AddTail(_T("TK")).SetData((DWORD)&CTreeCtrlPfw::TK);
		pbasic.AddTail(_T("TIME")).SetData((DWORD)&CTreeCtrlPfw::TIME);
		pbasic.AddTail(_T("TOT(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::TOT);
		pbasic.AddTail(_T("TOTMOLE(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::TOTMOLE);
		pbasic.AddTail(_T("TOTAL_TIME")).SetData((DWORD)&CTreeCtrlPfw::TOTAL_TIME);

		CTreeCtrlNode basic = this->InsertItem(_T("GENERAL BASIC statements"));
		basic.AddTail(_T("AND")).SetData((DWORD)&CTreeCtrlPfw::AND);
		basic.AddTail(_T("OR")).SetData((DWORD)&CTreeCtrlPfw::OR);
		basic.AddTail(_T("NOT")).SetData((DWORD)&CTreeCtrlPfw::NOT);
		basic.AddTail(_T("ABS(..)")).SetData((DWORD)&CTreeCtrlPfw::ABS);
		basic.AddTail(_T("ARCTAN(..)")).SetData((DWORD)&CTreeCtrlPfw::ARCTAN);
		basic.AddTail(_T("ASC(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::ASC);
		basic.AddTail(_T("CEIL(..)")).SetData((DWORD)&CTreeCtrlPfw::CEIL);
		basic.AddTail(_T("CHR$(..)")).SetData((DWORD)&CTreeCtrlPfw::CHR);
		basic.AddTail(_T("COS(..)")).SetData((DWORD)&CTreeCtrlPfw::COS);
		basic.AddTail(_T("DIM ..(..)")).SetData((DWORD)&CTreeCtrlPfw::DIM);
		basic.AddTail(_T("DATA .., ..")).SetData((DWORD)&CTreeCtrlPfw::DATA);
		basic.AddTail(_T("ELSE")).SetData((DWORD)&CTreeCtrlPfw::ELSE);
		basic.AddTail(_T("EOL$")).SetData((DWORD)&CTreeCtrlPfw::EOL);
		basic.AddTail(_T("EXP(..)")).SetData((DWORD)&CTreeCtrlPfw::EXP);
		basic.AddTail(_T("FOR .. TO .. STEP ..")).SetData((DWORD)&CTreeCtrlPfw::FOR);
		basic.AddTail(_T("NEXT ..")).SetData((DWORD)&CTreeCtrlPfw::NEXT);
		basic.AddTail(_T("FLOOR(..)")).SetData((DWORD)&CTreeCtrlPfw::FLOOR);
		basic.AddTail(_T("GOTO ..")).SetData((DWORD)&CTreeCtrlPfw::GOTO);
		basic.AddTail(_T("GOSUB ..")).SetData((DWORD)&CTreeCtrlPfw::GOSUB);
		basic.AddTail(_T("RETURN")).SetData((DWORD)&CTreeCtrlPfw::RETURN);
		basic.AddTail(_T("IF .. THEN .. ELSE ..")).SetData((DWORD)&CTreeCtrlPfw::IF);
		basic.AddTail(_T("INSTR(\"..\", \"..\")")).SetData((DWORD)&CTreeCtrlPfw::INSTR);
		basic.AddTail(_T("LEN(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::LEN);
		basic.AddTail(_T("LOG(..)")).SetData((DWORD)&CTreeCtrlPfw::LOG);
		basic.AddTail(_T("LOG10(..)")).SetData((DWORD)&CTreeCtrlPfw::LOG10);
		basic.AddTail(_T("LTRIM(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::LTRIM);
		basic.AddTail(_T("MID$(\"..\", n, m)")).SetData((DWORD)&CTreeCtrlPfw::MID);
		basic.AddTail(_T(".. MOD ..")).SetData((DWORD)&CTreeCtrlPfw::MOD);
		basic.AddTail(_T("ON .. GOTO .., ..")).SetData((DWORD)&CTreeCtrlPfw::ON_GOTO);
		basic.AddTail(_T("ON .. GOSUB ..")).SetData((DWORD)&CTreeCtrlPfw::ON_GOSUB);
		basic.AddTail(_T("PAD(\"..\", n)")).SetData((DWORD)&CTreeCtrlPfw::PAD);
		basic.AddTail(_T("READ")).SetData((DWORD)&CTreeCtrlPfw::READ);
		basic.AddTail(_T("REM")).SetData((DWORD)&CTreeCtrlPfw::REM);
		basic.AddTail(_T("RESTORE ..")).SetData((DWORD)&CTreeCtrlPfw::RESTORE);
		basic.AddTail(_T("RTRIM(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::RTRIM);
		basic.AddTail(_T("SGN(..)")).SetData((DWORD)&CTreeCtrlPfw::SGN);
		basic.AddTail(_T("SIN(..)")).SetData((DWORD)&CTreeCtrlPfw::SIN);
		basic.AddTail(_T("SQR(..)")).SetData((DWORD)&CTreeCtrlPfw::SQR);
		basic.AddTail(_T("SQRT(..)")).SetData((DWORD)&CTreeCtrlPfw::SQRT);
		basic.AddTail(_T("STR$(..)")).SetData((DWORD)&CTreeCtrlPfw::STR);
		basic.AddTail(_T("TAN(..)")).SetData((DWORD)&CTreeCtrlPfw::TAN);
		basic.AddTail(_T("TRIM(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::TRIM);
		basic.AddTail(_T("VAL(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::VAL);
		basic.AddTail(_T("WHILE ..")).SetData((DWORD)&CTreeCtrlPfw::WHILE);
		basic.AddTail(_T("WEND")).SetData((DWORD)&CTreeCtrlPfw::WEND);
	}
}