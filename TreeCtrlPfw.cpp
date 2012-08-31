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
	"\n"
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
	"\n"
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
	"\n"
	);

// CTreeCtrlPfw

IMPLEMENT_DYNAMIC(CTreeCtrlPfw, baseCTreeCtrlPfw)

CTreeCtrlPfw::CTreeCtrlPfw()
: m_pWndDesc(0)
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

	/***
	// Add your specialized creation code here
	// Create the Image List
	if (!m_ctrlImage.Create(IDB_INPUT_ITEMS, 16, 0, RGB(255,0,255)))
	{
		TRACE0("Failed to create image list\n");
		return -1;
	}

	// Attach image list to Tree
	SetImageList(&m_ctrlImage, TVSIL_NORMAL);
	***/

	CTreeCtrlNode keys = this->InsertItem(_T("PHREEQC Keywords"));

	CTreeCtrlNode start = keys.AddTail(_T("Start"));
	start.AddTail(_T("COPY")).SetData((DWORD)&CTreeCtrlPfw::COPY);
	//start.AddTail(_T("DATABASE")).SetData((DWORD)&CTreeCtrlPfw::DATABASE);
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
	start.AddTail(_T("TITLE")).SetData((DWORD)&CTreeCtrlPfw::TITLE);
	start.AddTail(_T("USE")).SetData((DWORD)&CTreeCtrlPfw::USE);
	start.GetLastChild();
	start.GetChildAt(0).EnsureVisible();

	CTreeCtrlNode chem = keys.AddTail(_T("Chemical reaction"));
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

	CTreeCtrlNode phys = keys.AddTail(_T("Physical action"));
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
	phys.AddTail(_T("MIX")).SetData((DWORD)&CTreeCtrlPfw::MIX);
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

	CTreeCtrlNode output = keys.AddTail(_T("Output"));
	output.AddTail(_T("PRINT")).SetData((DWORD)&CTreeCtrlPfw::PRINT);
	output.AddTail(_T("SELECTED_OUTPUT")).SetData((DWORD)&CTreeCtrlPfw::SELECTED_OUTPUT);
	output.AddTail(_T("USER_GRAPH")).SetData((DWORD)&CTreeCtrlPfw::USER_GRAPH);
	output.AddTail(_T("USER_PRINT")).SetData((DWORD)&CTreeCtrlPfw::USER_PRINT);
	output.AddTail(_T("USER_PUNCH")).SetData((DWORD)&CTreeCtrlPfw::USER_PUNCH);

	CTreeCtrlNode pbasic = this->InsertItem(_T("PHREEQC BASIC statements"));
	pbasic.AddTail(_T("ACT(\"..\")"));

	CTreeCtrlNode basic = this->InsertItem(_T("GENERAL BASIC statements"));
	basic.AddTail(_T("AND"));

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
	if ((item.GetLevel() == 2 || item.GetLevel() == 3) && item.GetData())
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
		if ((item.GetLevel() == 2 || item.GetLevel() == 3) && item.GetData())
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
