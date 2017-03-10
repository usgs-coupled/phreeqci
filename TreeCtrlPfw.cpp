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
	"Alkalinity	Ca	Cl	S(6)	Temp	pH	Li\n"
	"mg/l as HCO3						ug/l\n"
	"233.4	87.7	22.7	94.4	13	7.3\n"
	"221.7	78.6	35.3	87.4	13.1	7.1	4\n"
	"221.3	75.0	24.9	83.2		6.9\n"
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
std::pair<CString, CString> CTreeCtrlPfw::KINETICS(
	"KINETICS 1-10 # cell number or a range of cells. Default: 1.\n"
	"Calcite # name (must be defined in RATES)\n"
	"-formula CaCO3 1.0 # reactant's elements and stoichiometry.\n"
	"-m0 1.0 # initial reactant moles."
	,
	"Specifies kinetic reactions, chemical formulas of reactants and reaction parameters.\n"
	"\n"
	"Example:\n"
	"KINETICS 1-10\n"
	"Calcite\n"
	"-formula CaCO3 1.0\n"
	"-m0 1.0\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KINETICS_cvode(
	"-cvode true"
	,
	"Indicates to use the CVODE solver instead of the Runge Kutta method. CVODE is for stiff equations with widely varying and interacting kinetic rates.\n"
	"\n"
	"Example:\n"
	"-cvode true\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KINETICS_formula(
	"-formula CaCO3 1.2 # adds 1.2 moles Ca and C, and 3.6 moles O per mol kinetic reaction."
	,
	"Specifies the chemical formula of the kinetic reactant.\n"
	"\n"
	"Example:\n"
	"-formula CaCO3 1.2\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KINETICS_m(
	"-m 0.5 # current mass is 0.5 moles"
	,
	"Current moles of reactant.\n"
	"\n"
	"Example:\n"
	"-m 0.5\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KINETICS_m0(
	"-m0 1.0 # Initial mass is 1.0 moles"
	,
	"Initial moles of reactant. Default: 1.0 moles.\n"
	"\n"
	"Example:\n"
	"-m0 1.0\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KINETICS_parms(
	"-parms 0.5 3.2 # parm(1) = 0.5, parm(2) = 3.2"
	,
	"A list of numbers may be entered that can be used in the BASIC program of the rate as parm(1), parm(2), etc.\n"
	"\n"
	"Example:\n"
	"-parms 0.5 3.2\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KINETICS_tol(
	"-tol 1e-6 # tolerance is 1e-6 moles."
	,
	"Tolerance for integration procedure (moles). Default: 1e-8 moles\n"
	"\n"
	"Example:\n"
	"-tol 1e-6\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KINETICS_steps(
	"# Option 1\n"
	"-steps 300 in 3 steps # total time is 300 seconds, subdivided in 3 steps of 100 s with output-results.\n"
	"# Option 2\n"
	"-steps 0 50 150 100 # 4 steps with output-results, total time is 300 seconds with INCREMENTAL_REACTIONS true.\n"
	"# Option 3\n"
	"-steps 4*50 100 # 5 steps with output-results, total time is 300 seconds with INCREMENTAL_REACTIONS true."
	,
	"Time steps over which to integrate the rate expressions (seconds).\n"
	"\n"
	"Example:\n"
	"# Option 1\n"
	"-steps 300 in 3 steps\n"
	"# Option 2\n"
	"-steps 0 50 150 100\n"
	"# Option 3\n"
	"-steps 4*50 100\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KINETICS_step_divide(
	"-steps 300\n"
	"-step_divide 10 # integration takes intervals of 300 / 10 = 30 s. Output is written after 300 s.\n"
	"-step_divide 1e-6 # at most 1e-6 moles of reactant is added in any subinterval."
	,
	"If the following number is greater than 1, the time subintervals of the integration are constrained. If the following number is less than 1, the maximum moles of reaction in an integration subinterval are constrained. Default: -step_divide 1.0.\n"
	"\n"
	"Example:\n"
	"-steps 300\n"
	"-step_divide 10\n"
	"-step_divide 1e-6\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KINETICS_runge_kutta(
	"-runge_kutta 1 # order is reduced to 1 (but is increased automatically if needed by -tol).\n"
	"-runge_kutta 6 # order is always 5"
	,
	"Designates the order of the RK integration method. Default: -runge_kutta 3\n"
	"\n"
	"Example:\n"
	"-runge_kutta 1\n"
	"-runge_kutta 6\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KINETICS_bad_step_max(
	"-bad_step_max 2000 # if number must be increased > 500, try the stiff solver -cvode"
	,
	"Sets the maximum number of times a rate integration may fail before execution of the program is terminated. Default: -bad_step_max 500\n"
	"\n"
	"Example:\n"
	"-bad_step_max 2000\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KINETICS_cvode_steps(
	"-cvode_steps 100"
	,
	"Sets the maximum number of steps for CVODE. Default: -cvode_steps 100\n"
	"\n"
	"Example:\n"
	"-cvode_steps 100\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KINETICS_cvode_order(
	"-cvode_order 5"
	,
	"Sets the number of terms (1 - 5) in the BFD method. Default: -cvode_order 5\n"
	"\n"
	"Example:\n"
	"-cvode_order 5\n"
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
std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS(
	"SOLID_SOLUTIONS\n"
	"CaMgSrCO3 # name of solid solution\n"
	"-comp Calcite 1e-3 # name (defined in PHASES, or formula), initial amount (moles)\n"
	"-comp Magnesite 2e-5\n"
	"-comp SrCO3 2e-5 # ideal solid solutions may have more than 2 components\n"
	"CaSrCO3\n"
	"-comp1 Calcite 1e-3\n"
	"-comp2 Magnesite 2e-5 # non-ideal solid solutions have 2 components"
	,
	"Defines a solid-solution assemblage.\n"
	"\n"
	"Example:\n"
	"SOLID_SOLUTIONS\n"
	"CaMgSrCO3\n"
	"-comp Calcite 1e-3\n"
	"-comp Magnesite 2e-5\n"
	"-comp SrCO3 2e-5\n"
	"CaSrCO3\n"
	"-comp1 Calcite 1e-3\n"
	"-comp2 Magnesite 2e-5\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_comp(
	"-comp Calcite 1e-3"
	,
	"Defines a component of an ideal solid solution.\n"
	"\n"
	"Example:\n"
	"-comp Calcite 1e-3\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_comp1(
	"-comp1 Calcite 1e-3 # name (defined in PHASES, or formula), initial amount (moles)"
	,
	"Defines the first component of a nonideal, binary solid solution.\n"
	"\n"
	"Example:\n"
	"-comp1 Calcite 1e-3\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_comp2(
	"-comp2 Magnesite 2e-5 # name (defined in PHASES, or formula), initial amount (moles)"
	,
	"Defines the second component of a nonideal, binary solid solution.\n"
	"\n"
	"Example:\n"
	"-comp2 Magnesite 2e-5\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_temp(
	"-temp 25"
	,
	"Temperature (Celsius) at which excess free-energy parameters are defined.\n"
	"\n"
	"Example:\n"
	"-temp 25\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_tempk(
	"-tempk 298"
	,
	"Temperature (Kelvin) at which excess free-energy parameters are defined.\n"
	"\n"
	"Example:\n"
	"-tempk 298\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_Gugg_nondim(
	"-Gugg_nondim 1.3 2.5 # labda_1 = exp((x_2)^2 * (1.3 - 2.5 * (4 * x_1 - 1))); labda_2 = exp((x_1)^2 * (1.3 + 2.5 * (4 * x_2 - 1)))"
	,
	"Defines nondimensional Guggenheim parameters for calculating the activity coefficients (labda) in the binary solid solution.\n"
	"\n"
	"Example:\n"
	"-Gugg_nondim 1.3 2.5\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_Gugg_kJ(
	"-Gugg_kJ 12.6 4.7"
	,
	"Defines Guggenheim parameters (kJ/mol) of the excess free energy of the nonideal, binary solid solution.\n"
	"\n"
	"Example:\n"
	"-Gugg_kJ 12.6 4.7\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_activity_coefficients(
	"-activity_coefficients 24 99 1e-3 0.999 # labda_1, labda_1, x_2 for labda_1, x_2 for labda_1"
	,
	"Two activity coefficients (labda) are used to calculate Guggenheim parameters\n"
	"\n"
	"Example:\n"
	"-activity_coefficients 24 99 1e-3 0.999\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_distribution_coefficients(
	"-distribution_coefficients 1.1 223 0.9 0.083 # D1 = (x_1 / x_2) / (a_1 / a_2), D2, x_2 for D1, x_2 for D2"
	,
	"Two distribution coefficients (D) are used to calculate Guggenheim parameters.\n"
	"\n"
	"Example:\n"
	"-distribution_coefficients 1.1 223 0.9 0.083\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_miscibility_gap(
	"-miscibility_gap 0.3 0.55 # x_2, x_2 bordering the miscibility gap"
	,
	"The miscibility gap is used to calculate Guggenheim parameters.\n"
	"\n"
	"Example:\n"
	"-miscibility_gap 0.3 0.55\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_spinodal_gap(
	"-spinodal_gap 0.3 0.55 # x_2, x_2 bordering the spinodal gap"
	,
	"The spinodal gap is used to calculate Guggenheim parameters.\n"
	"\n"
	"Example:\n"
	"-spinodal_gap 0.3 0.55\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_critical_point(
	"-critical_point 0.67 325 # x_2, temperature (K) at the critical point"
	,
	"The critical point and temperature (Kelvin) are used to calculate Guggenheim parameters.\n"
	"\n"
	"Example:\n"
	"-critical_point 0.67 325\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_alyotropic_point(
	"-alyotropic_point 0.59 -9.4 # x_2, log10(TSP) at the alyotropic point"
	,
	"The alyotropic point and the total solubility product (TSP) at that point are used to calculate Guggenheim parameters.\n"
	"\n"
	"Example:\n"
	"-alyotropic_point 0.59 -9.4\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_Thompson(
	"-Thompson 17.3 7.9 # wg2, wg1"
	,
	"Thompson and Waldbaum parameters (wg2 and wg1) are used to calculate Guggenheim parameters.\n"
	"\n"
	"Example:\n"
	"-Thompson 17.3 7.9\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_Margules(
	"-Margules -0.6 7.6 # alpha2, alpha3"
	,
	"Margules parameters (alpha2 and alpha3) are used to calculate dimensional Guggenheim parameters.\n"
	"\n"
	"Example:\n"
	"-Margules -0.6 7.6\n"
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
//{{NEW BASIC HERE}}
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
std::pair<CString, CString> CTreeCtrlPfw::APHI(
	"APHI"
	,
	"The A(phi) parameter of the Pitzer formulation of aqueous thermodynamics at the current solution conditions.\n"
	"\n"
	"Example:\n"
	"APHI\n"
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
std::pair<CString, CString> CTreeCtrlPfw::DH_A(
	"DH_A"
	,
	"Debye-Hückel A parameter in the activity coefficient equation, (mol/kg)^(-0.5).\n"
	"\n"
	"Example:\n"
	"DH_A\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::DH_Av(
	"DH_Av"
	,
	"Debye-Hückel limiting slope of specific volume vs. ionic strength, (cm3/mol)(mol/kg)^(-0.5).\n"
	"\n"
	"Example:\n"
	"DH_Av\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::DH_B(
	"DH_B"
	,
	"Debye-Hückel B parameter in the activity coefficient equation, angstrom-1(mol/kg)^(-0.5).\n"
	"\n"
	"Example:\n"
	"DH_B\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::DIFF_C(
	"DIFF_C(\"CO3-2\")"
	,
	"Returns (1) distance to cell-midpoint in TRANSPORT calculations, or (2) cell number in ADVECTION calculations, or (3) -99 in all other calculations.\n"
	"\n"
	"Example:\n"
	"d = DIFF_C(\"CO3-2\")\n"
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

std::pair<CString, CString> CTreeCtrlPfw::EDL_SPECIES(
	"t = EDL_SPECIES(\"Hfo\", count, name$, moles, area, thickness)"
	,
	"The total number of moles of species in the diffuse\n"
	"layer. The arguments to the function are as follows:\n"
	"\n"
	"surf$ is the name of a surface, such as \"Hfo\", excluding\n"
	"	the site type (such as \"_s\").\n"
	"count is the number of species in the diffuse layer.\n"
	"name$ is an array of size count that contains the\n"
	"	names of aqueous species in the diffuse layer\n"
	"	of surface surf$.\n"
	"moles is an array of size count that contains the number\n"
	"	of moles of each aqueous species in the diffuse layer\n"
	"	of surface surf$.\n"
	"area  is the area of the surface in m^2.\n"
	"thickness is the thickness of the diffuse layer in m.\n"
	"\n"
	"The volume of the diffuse layer is area * thickness, and\n"
	"the concentrations of the species in the diffuse layer are\n"
	"the number of moles divided by the volume.\n"
	"\n"
	"Example:\n"
	"10 t = EDL_SPECIES(\"Hfo\", count, name$, moles, area, thickness)\n"
	"20 PRINT \"Surface: Hfo\"\n"
	"30 PRINT \"Area:       \", area\n"
	"40 PRINT \"Thickness:  \", thickness\n"
	"45 PRINT \"Volume:     \", area * thickness\n"
	"50 for i = 1 to count\n"
	"60   PRINT PAD(name$(i),20), moles(i)\n"
	"70 next i\n"
	"\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::EPS_R(
	"EPS_R"
	,
	"Relative dielectric constant.\n"
	"\n"
	"Example:\n"
	"EPS_R\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::EQ_FRAC(
	"EQ_FRAC(\"AlX3\", eq, x$)"
	,
	"Returns the equivalent fraction of a surface \n"
	"or exchange species. The three arguments are \n"
	"(1) Species name (input), \n"
	"(2) Equivalents of exchange or surface sites \n"
	"    per mole of the species (output), \n"
	"(3) The name of the surface or exchange site \n"
	"    (output). \n"
	"Example: \n"
	" \n"
	"10 f = EQ_FRAC(\"AlX3\", eq, x$) \n"
	" \n"
	"f = equivlalent fraction of AlX3 relative to  \n"
	"    total equivalents of X sites. \n"
	"eq = 3.0 \n"
	"x$ = \"X\" \n"
	" \n"
	"If the species name is not found to be a surface \n"
	"or exchange species, the return value is 0, \n"
	"the second argument is set to 0, and the third \n"
	"argument is set to an empty string. \n"
	);
std::pair<CString, CString> CTreeCtrlPfw::EQUI(
	"EQUI(\"Calcite\")"
	,
	"Moles of a phase in EQUILIBRIUM_PHASES.\n"
	"\n"
	"Example:\n"
	"EQUI(\"Calcite\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::EQUI_DELTA(
	"EQUI_DELTA(\"Calcite\")"
	,
	"Moles of a phase in the equilibrium-phase assemblage that reacted during the current calculation.\n"
	"\n"
	"Example:\n"
	"EQUI_DELTA(\"Calcite\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::EXISTS(
	"EXISTS(cell_no, 2)"
	,
	"Checks if a memory location (defined by 1 or more subscripts) is filled.\n"
	"\n"
	"Example:\n"
	"EXISTS(cell_no, 2)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::GAMMA(
	"GAMMA(\"H+\")"
	,
	"Activity coefficient of a SOLUTION_SPECIES. LG(\"..\") gives the log10 of the activity coefficient.\n"
	"\n"
	"Example:\n"
	"GAMMA(\"H+\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::GAS(
	"GAS(\"O2(g)\")"
	,
	"Moles of a gas component in GAS_PHASE.\n"
	"\n"
	"Example:\n"
	"GAS(\"O2(g)\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::GAS_P(
	"GAS_P"
	,
	"Pressure of the GAS_PHASE (atm), either specified for a fixed-pressure gas phase, or calculated for"
	"a fixed-volume gas phase. Related functions are PR_P and PRESSURE.\n"
	"\n"
	"Example:\n"
	"GAS_P\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::GAS_VM(
	"GAS_VM"
	,
	"Molar volume (L/mol, liter per mole) of the GAS_PHASE (calculated with Peng-Robinson).\n"
	"\n"
	"Example:\n"
	"GAS_VM\n"
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
std::pair<CString, CString> CTreeCtrlPfw::GFW(
	"GFW(\"CaCO3\")"
	,
	"Returns the gram formula weight of the specified formula.\n"
	"\n"
	"Example:\n"
	"GFW(\"CaCO3\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::GRAPH_SY(
	"GRAPH_SY -la(\"H+\"), -la(\"e-\")"
	,
	"Plots variables on the secondary y-axis of the graph.\n"
	"\n"
	"Example:\n"
	"GRAPH_SY -la(\"H+\"), -la(\"e-\")\n"
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
std::pair<CString, CString> CTreeCtrlPfw::KAPPA(
	"KAPPA"
	,
	"Compressibility of pure water at current pressure and temperature.\n"
	"\n"
	"Example:\n"
	"KAPPA\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KIN(
	"KIN(\"Calcite\")"
	,
	"Actual moles of a kinetic reactant (m).\n"
	"\n"
	"Example:\n"
	"KIN(\"Calcite\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KINETICS_FORMULA(
	"n$ = KINETICS_FORMULA$(\"Albite\", count, elt$, coef)"
	,
	"Kinetic reactant formula.\n"
	"\n"
	"Example:\n"
	"n$ = KINETICS_FORMULA$(\"Albite\", count, elt$, coef)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KIN_DELTA(
	"KIN_DELTA(\"Calcite\")"
	,
	"Moles of a kinetic reactant that reacted during the current calculation.\n"
	"\n"
	"Example:\n"
	"KIN_DELTA(\"CH2O\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::KIN_TIME(
	"KIN_TIME"
	,
	"Returns the time interval in seconds of the last kinetic integration.\n"
	"\n"
	"Example:\n"
	"KINETICS\n"
	"Calcite\n"
	"	-m 1\n"
	"-step  864 8640\n"
	"\n"
	"KIN_TIME will return 864 after the first\n"
	"step and 8640 after the second.\n"
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
std::pair<CString, CString> CTreeCtrlPfw::LIST_S_S(
	"LIST_S_S(\"Carbonate_s_s\", nc, comp$, mols)"
	,
	"Returns the sum of the moles of components in a solid solution and the composition of the solid"
	" solution. The first argument is an input value specifying the name of the solid solution. Count is an"
	" output variable containing the number of components in the solid solution. Comp$ is an output"
	" character array containing the names of each component in the solid solution. Moles is an output"
	" numeric array containing the number of moles of each component, in the order defined by Comp$."
	" Arrays are in sort order by number of moles.\n"
	"\n"
	"Example:\n"
	"LIST_S_S(\"Carbonate_s_s\", nc, comp$, mols)\n"
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
std::pair<CString, CString> CTreeCtrlPfw::PHASE_FORMULA(
	"PHASE_FORMULA(\"Dolomite\", count, elt$, coef)"
	,
	"With four arguments, PHASE_FORMULA returns a string that contains the chemical formula for the"
	" phase, and, in addition, returns values for count, elt$, coef. Count is the dimension of the elt$ and coef"
	" arrays. Elt$ is a character array with the name of each element in the chemical formula for the phase"
	" Coef is a numeric array containing the number of atoms of each element in the phase formula, in the"
	" order defined by elt$, which is alphabetical by element.\n"
	"\n"
	"Example:\n"
	"PHASE_FORMULA(\"Carbonate_s_s\", count, elt$, coef)\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PHASE_VM(
	"PHASE_VM"
	,
	"Returns the molar volume for a mineral, (cm^3/mol). The molar volume is defined for the mineral in PHASES with the -vm option.\n"
	"\n"
	"Example:\n"
	"PHASE_VM(\"Quartz\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PLOT_XY(
	"PLOT_XY tot(\"Ca\") * 40.08e3, tot(\"F\") * 19e3, color = Blue, symbol = Circle, symbol_size = 6, y-axis = 1, line_width = 0"
	,
	"Used in USER_GRAPH data block to define the points to chart; here, Ca in mg/L is the X value for"
	" points, F in mg/L is the Y value for points, the symbols are blue circles, the points are plotted relative"
	" to the Y axis, and no line connects the points. See the description of the USER_GRAPH keyword for"
	" more details.\n"
	"\n"
	"Example:\n"
	"PLOT_XY tot(\"Ca\") * 40.08e3, tot(\"F\") * 19e3, color = Blue, symbol = Circle, symbol_size = 6, y-axis = 1, line_width = 0\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::PR_P(
	"PR_P(\"CO2(g)\")"
	,
	"Pressure (atm) of a gas component in a Peng-Robinson GAS_PHASE.\n"
	"\n"
	"Example:\n"
	"PR_P(\"CO2(g)\")"
	);
std::pair<CString, CString> CTreeCtrlPfw::PR_PHI(
	"PR_PHI(\"CO2(g)\")"
	,
	"Fugacity coefficient of a gas component in a Peng-Robinson GAS_PHASE.\n"
	"\n"
	"Example:\n"
	"PR_PHI(\"CO2(g)\")"
	);
std::pair<CString, CString> CTreeCtrlPfw::PRESSURE_BASIC(
	"PRESSURE"
	,
	"Current pressure applied to the solution (atm). PRESSURE is a specified value except for"
	" fixed-volume GAS_PHASE calculations.\n"
	"\n"
	"Example:\n"
	"PRESSURE\n"
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
std::pair<CString, CString> CTreeCtrlPfw::QBRN(
	"QBRN"
	,
	"The Born parameter for calculating the temperature dependence of the specific volume of an aqueous"
	" species at infinite dilution. This is the pressure derivative of the relative dielectric constant of water"
	" multiplied by 41.84 bar cm^3/cal (bar cubic centimeter per calorie).\n"
	"\n"
	"Example:\n"
	"QBRN\n"
	);
#if defined PHREEQ98
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
#else
std::pair<CString, CString> CTreeCtrlPfw::RHO(
	"RHO"
	,
	"Density of the solution (kg/L).\n"
	"\n"
	"Example:\n"
	"RHO\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::RHO_0(
	"RHO_0"
	,
	"Density of pure water at the current temperature (kg/L).\n"
	"\n"
	"Example:\n"
	"RHO_0\n"
	);
#endif
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
std::pair<CString, CString> CTreeCtrlPfw::SOLN_VOL(
	"SOLN_VOL"
	,
	"Volume of the solution, in liters.\n"
	"\n"
	"Example:\n"
	"SOLN_VOL\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::SPECIES_FORMULA(
	"SPECIES_FORMULA$"
	,
	"Returns the stoichiometry of an aqueous, exchange, or surface \n"
	"species. The function returns a string: \"aq\" for \n"
	"aqueous, \"ex\" for exchange, \"surf\" for surface, \n"
	"and \"none\" if there is no species of that name. \n"
	"The four arguments are \n"
	"(1) the name of the species (input), \n"
	"(2) the number of elements, including charge (output), \n"
	"(3) an string array of element names (output), \n"
	"(4) a number array of coefficients corresponding to the elements (output). \n"
	"\n"
	"Example: \n"
	"10   name$ = \"AlX3\" \n"
	"20   ty$ = SPECIES_FORMULA(name$, count_s, elt$, coef) \n"
	"20   print pad(name$, 15), ty$ \n"
	"30   for j = 1 to count_s \n"
	"40     print pad(blank$, 5), pad(elt$(j),5), str_f$(coef(j), 5, 0) \n"
	"50   next j \n"
	"\n"	
	"	Produces the following output: \n"
	"\n"	
	"AlX3            ex \n"
	"      Al        1 \n"
	"      X         3 \n"
	"      charge     0 \n"
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
std::pair<CString, CString> CTreeCtrlPfw::STR_E(
	"STR_E$(x, w, d)"
	,
	"Returns a string with exponential format from a number with a given width (w) and number of decimal places (d). w is \n"
	"the minimum width of the string. The string is padded with spaces to the left to produce a string of the specified \n"
	"width (w)\n"
	"\n"
	"Example:\n"
	"If x = 123456.789, then STR_E$(x,15,5) produces the following\n"
	"on a Windows computer:\n"	
	"   1.23457e+005\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::STR_F(
	"STR_F$(x, w, d)"
	,
	"Returns a string from a number with a given width (w) and number of decimal places (d). w is the minimum width of \n"
	"the string. The string is padded with spaces to the left to produce a string of the specified width (w)\n"
	"\n"
	"Example:\n"
	"If x = 123456.789, then STR_F$(x,15,5) produces the following\n"
	"on a Windows computer:\n"	
	"   123456.78900\n"
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
std::pair<CString, CString> CTreeCtrlPfw::TOTAL_TIME(
	"TOTAL_TIME"
	,
	"Cumulative time (s) since the start of the run or since -initial_time was set.\n"
	"\n"
	"Example:\n"
	"TOTAL_TIME\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::TOTMOLE(
	"10 total_Ca_in_moles = TOTMOLE(\"Ca\")"
	,
	"Provides the total number of moles of an element or element redox state in solution. TOTMOLE(\"ater\") gives number of moles of water. TOTMOL or TOTMOLES are also accepted.\n"
	"\n"
	"Example:\n"
	"10 total_Ca_in_moles = TOTMOLE(\"Ca\")\n"
	);
std::pair<CString, CString> CTreeCtrlPfw::VM(
	"VM(\"Na+\")"
	,
	"Returns the specific volume (cm^3/mol) of a SOLUTION_SPECIES, relative to VM(\"H+\") = 0, a"
	"function of temperature, pressure, and ionic strength.\n"
	"\n"
	"Example:\n"
	"VM(\"Na+\")\n"
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

std::pair<CString, CString> CTreeCtrlPfw::ex1(
	"TITLE Example 1.--Add uranium and speciate seawater.\n"
	"SOLUTION 1  SEAWATER FROM NORDSTROM AND OTHERS (1979)\n"
	"        units   ppm\n"
	"        pH      8.22\n"
	"        pe      8.451\n"
	"        density 1.023\n"
	"        temp    25.0\n"
	"        redox   O(0)/O(-2)\n"
	"        Ca              412.3\n"
	"        Mg              1291.8\n"
	"        Na              10768.0\n"
	"        K               399.1\n"
	"        Fe              0.002\n"
	"        Mn              0.0002  pe\n"
	"        Si              4.28\n"
	"        Cl              19353.0\n"
	"        Alkalinity      141.682 as HCO3\n"
	"        S(6)            2712.0\n"
	"        N(5)            0.29    gfw   62.0\n"
	"        N(-3)           0.03    as    NH4\n"
	"        U               3.3     ppb   N(5)/N(-3)\n"
	"        O(0)            1.0     O2(g) -0.7\n"
	"SOLUTION_MASTER_SPECIES\n"
	"        U       U+4     0.0     238.0290     238.0290\n"
	"        U(4)    U+4     0.0     238.0290\n"
	"        U(5)    UO2+    0.0     238.0290\n"
	"        U(6)    UO2+2   0.0     238.0290\n"
	"SOLUTION_SPECIES\n"
	"        #primary master species for U\n"
	"        #is also secondary master species for U(4)\n"
	"        U+4 = U+4\n"
	"                log_k          0.0\n"
	"        U+4 + 4 H2O = U(OH)4 + 4 H+\n"
	"                log_k          -8.538\n"
	"                delta_h        24.760 kcal\n"
	"        U+4 + 5 H2O = U(OH)5- + 5 H+\n"
	"                log_k          -13.147\n"
	"                delta_h        27.580 kcal\n"
	"        #secondary master species for U(5)\n"
	"        U+4 + 2 H2O = UO2+ + 4 H+ + e-\n"
	"                log_k          -6.432\n"
	"                delta_h        31.130 kcal\n"
	"        #secondary master species for U(6)\n"
	"        U+4 + 2 H2O = UO2+2 + 4 H+ + 2 e-\n"
	"                log_k          -9.217\n"
	"                delta_h        34.430 kcal\n"
	"        UO2+2 + H2O = UO2OH+ + H+\n"
	"                log_k          -5.782\n"
	"                delta_h        11.015 kcal\n"
	"        2UO2+2 + 2H2O = (UO2)2(OH)2+2 + 2H+\n"
	"                log_k          -5.626\n"
	"                delta_h        -36.04 kcal\n"
	"        3UO2+2 + 5H2O = (UO2)3(OH)5+ + 5H+\n"
	"                log_k          -15.641\n"
	"                delta_h        -44.27 kcal\n"
	"        UO2+2 + CO3-2 = UO2CO3\n"
	"                log_k          10.064\n"
	"                delta_h        0.84 kcal\n"
	"        UO2+2 + 2CO3-2 = UO2(CO3)2-2\n"
	"                log_k          16.977\n"
	"                delta_h        3.48 kcal\n"
	"        UO2+2 + 3CO3-2 = UO2(CO3)3-4\n"
	"                log_k          21.397\n"
	"                delta_h        -8.78 kcal\n"
	"PHASES\n"
	"        Uraninite\n"
	"        UO2 + 4 H+ = U+4 + 2 H2O\n"
	"        log_k          -3.490\n"
	"        delta_h        -18.630 kcal\n"
	"END\n"
	,
	"Example 1—Speciation Calculation\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ex2(
	"TITLE Example 2.--Temperature dependence of solubility\n"
	"                  of gypsum and anhydrite\n"
	"SOLUTION 1 Pure water\n"
	"        pH      7.0\n"
	"        temp    25.0                \n"
	"EQUILIBRIUM_PHASES 1\n"
	"        Gypsum          0.0     1.0\n"
	"        Anhydrite       0.0     1.0\n"
	"REACTION_TEMPERATURE 1\n"
	"        25.0 75.0 in 51 steps\n"
	"SELECTED_OUTPUT\n"
	"        -file   ex2.sel\n"
	"        -temperature\n"
	"        -si     anhydrite  gypsum\n"
	"USER_GRAPH 1 Example 2\n"
	"        -headings Temperature Gypsum Anhydrite\n"
	"        -chart_title \"Gypsum-Anhydrite Stability\"\n"
	"        -axis_scale x_axis 25 75 5 0\n"
	"        -axis_scale y_axis auto 0.05 0.1\n"
	"        -axis_titles \"Temperature, in degrees celsius\" \"Saturation index\"\n"
	"        -initial_solutions false\n"
	"  -start\n"
	"  10 graph_x TC\n"
	"  20 graph_y SI(\"Gypsum\") SI(\"Anhydrite\")\n"
	"  -end\n"
	"END\n"
	,
	"Example 2—Equilibration with Pure Phases\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ex2b(
	"TITLE Calculate gypsum/anhydrite transitions, 30 - 170 oC, 1 - 1000 atm\n"
	"      Data in ex2b.tsv from Blount and Dickson, 1973, Am. Mineral. 58, 323, fig. 2.\n"
	"PRINT; -reset false\n"
	"SOLUTION 1\n"
	"EQUILIBRIUM_PHASES\n"
	"Gypsum\n"
	"REACTION_TEMPERATURE\n"
	" 30 90 in 10\n"
	"USER_GRAPH 1 Example 2B, (P, T)-dependent solubilities of Gypsum and Anhydrite\n"
	" -plot_tsv_file ex2b.tsv\n"
	" -axis_titles \"Temperature, in degrees celsius\" \"Solubility, in moles per \\\n"
	"kilogram water\"\n"
	" -axis_scale x_axis 30 170\n"
	" -axis_scale y_axis 1e-3 0.05 auto auto log\n"
	" 10 plot_xy tc, tot(\"Ca\"), color = Red, symbol = None\n"
	" -end\n"
	"END # 1st simulation\n"
	"\n"
	"USE solution 1 \n"
	"USE equilibrium_phases 1\n"
	"USE reaction_temperature 1\n"
	"REACTION_PRESSURE 2\n"
	" 493\n"
	"USER_GRAPH\n"
	" 10 plot_xy tc, tot(\"Ca\"), color = Red, symbol = None\n"
	"END\n"
	"\n"
	"USE solution 1 \n"
	"USE equilibrium_phases 1\n"
	"USE reaction_temperature 1\n"
	"REACTION_PRESSURE 3\n"
	" 987\n"
	"USER_GRAPH\n"
	" 20 plot_xy tc, tot(\"Ca\"), color = Red, symbol = None\n"
	"END # 2nd simulation\n"
	"\n"
	"USE solution 1 \n"
	"EQUILIBRIUM_PHASES 4\n"
	"Anhydrite\n"
	"REACTION_TEMPERATURE 4\n"
	" 50 170 in 10\n"
	"USER_GRAPH\n"
	" 10 plot_xy tc, tot(\"Ca\"), color = Green, symbol = None\n"
	"END\n"
	"\n"
	"USE solution 1 \n"
	"USE equilibrium_phases 4\n"
	"USE reaction_temperature 4\n"
	"USE reaction_pressure 2\n"
	"USER_GRAPH\n"
	" 10 plot_xy tc, tot(\"Ca\"), color = Green, symbol = None\n"
	" -end\n"
	"END\n"
	"\n"
	"USE solution 1 \n"
	"USE equilibrium_phases 4\n"
	"USE reaction_temperature 4\n"
	"USE reaction_pressure 3\n"
	"USER_GRAPH\n"
	" 10 plot_xy tc, tot(\"Ca\"), color = Green, symbol = None\n"
	" -end\n"
	"END\n"
	"\n"
	,
	"Example 2—Equilibration with Pure Phases\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ex3(
	"TITLE Example 3, part A.--Calcite equilibrium at log Pco2 = -2.0 and 25C.        \n"
	"SOLUTION 1  Pure water\n"
	"        pH      7.0\n"
	"        temp    25.0\n"
	"EQUILIBRIUM_PHASES\n"
	"        CO2(g)          -2.0    \n"
	"        Calcite         0.0\n"
	"SAVE solution 1\n"
	"END\n"
	"TITLE Example 3, part B.--Definition of seawater.\n"
	"SOLUTION 2  Seawater\n"
	"        units   ppm\n"
	"        pH      8.22\n"
	"        pe      8.451\n"
	"        density 1.023\n"
	"        temp    25.0\n"
	"        Ca              412.3\n"
	"        Mg              1291.8\n"
	"        Na              10768.0\n"
	"        K               399.1\n"
	"        Si              4.28\n"
	"        Cl              19353.0\n"
	"        Alkalinity      141.682 as HCO3\n"
	"        S(6)            2712.0\n"
	"END\n"
	"TITLE Example 3, part C.--Mix 70% groundwater, 30% seawater.\n"
	"MIX 1\n"
	"        1      0.7\n"
	"        2      0.3\n"
	"SAVE solution   3\n"
	"END\n"
	"TITLE Example 3, part D.--Equilibrate mixture with calcite and dolomite.\n"
	"EQUILIBRIUM_PHASES 1\n"
	"        Calcite         0.0\n"
	"        Dolomite        0.0\n"
	"USE solution 3\n"
	"END\n"
	"TITLE Example 3, part E.--Equilibrate mixture with calcite only.\n"
	"EQUILIBRIUM_PHASES 2\n"
	"        Calcite         0.0\n"
	"USE solution 3\n"
	"END\n"
	,
	"Example 3—Mixing\n"
// COMMENT: {9/6/2012 10:03:42 PM}	"TITLE Example 3\n"
// COMMENT: {9/6/2012 10:03:42 PM}	" A.--Calcite equilibrium at log Pco2 = -2.0 and 25C.\n"
// COMMENT: {9/6/2012 10:03:42 PM}	" B.--Definition of seawater.\n"
// COMMENT: {9/6/2012 10:03:42 PM}	" C.--Mix 70% groundwater, 30% seawater.\n"
// COMMENT: {9/6/2012 10:03:42 PM}	" D.--Equilibrate mixture with calcite and dolomite.\n"
// COMMENT: {9/6/2012 10:03:42 PM}	" E.--Equilibrate mixture with calcite only.\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ex4(
	"TITLE Example 4a.--Rainwater evaporation\n"
	"SOLUTION 1  Precipitation from Central Oklahoma\n"
	"        units           mg/L\n"
	"        pH              4.5   # estimated\n"
	"        temp            25.0\n"
	"        Ca              .384\n"
	"        Mg              .043\n"
	"        Na              .141\n"
	"        K               .036\n"
	"        Cl              .236\n"
	"        C(4)            .1      CO2(g)  -3.5\n"
	"        S(6)            1.3\n"
	"        N(-3)           .208\n"
	"        N(5)            .237\n"
	"REACTION 1\n"
	"        H2O     -1.0\n"
	"        52.73 moles\n"
	"SAVE solution 2\n"
	"END\n"
	"TITLE Example 4b.--Factor of 20 more solution\n"
	"MIX\n"
	"        2       20.\n"
	"SAVE solution 3\n"
	"END\n"
	,
	"Example 4—Evaporation and Homogeneous Redox Reactions\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ex5(
	"TITLE Example 5.--Add oxygen, equilibrate with pyrite, calcite, and goethite.\n"
	"SOLUTION 1  PURE WATER\n"
	"        pH      7.0\n"
	"        temp    25.0\n"
	"EQUILIBRIUM_PHASES 1\n"
	"        Pyrite          0.0\n"
	"        Goethite        0.0\n"
	"        Calcite         0.0\n"
	"        CO2(g)         -3.5\n"
	"        Gypsum          0.0     0.0\n"
	"REACTION 1\n"
	"        O2      1.0\n"
	"        NaCl    0.5\n"
	"        0.0     0.001   0.005   0.01   0.03   0.05\n"
	"SELECTED_OUTPUT\n"
	"        -file   ex5.sel\n"
	"        -total  Cl\n"
	"        -si     Gypsum\n"
	"        -equilibrium_phases  pyrite goethite calcite CO2(g) gypsum\n"
	"USER_GRAPH Example 5\n"
	"        -headings Pyrite Goethite Calcite CO2(g) Gypsum SI_Gypsum\n"
	"        -chart_title \"Pyrite Oxidation\"\n"
	"        -axis_titles \"O2 added, in millimoles\" \"Millimoles dissolved\" \\\n"
	"             \"Saturation index\"\n"
	"  10 x = RXN * 1e3\n"
	"  20 PLOT_XY x, 1e3 * (10 - EQUI(\"Pyrite\")), symbol = Plus\n"
	"  30 PLOT_XY x, 1e3 * (10 - EQUI(\"Goethite\")), symbol = Plus\n"
	"  40 PLOT_XY x, 1e3 * (10 - EQUI(\"Calcite\")), symbol = Plus\n"
	"  50 PLOT_XY x, 1e3 * (10 - EQUI(\"CO2(g)\")), symbol = Plus\n"
	"  60 PLOT_XY x, 1e3 * (-EQUI(\"Gypsum\")), symbol = Plus, color = Magenta\n"
	"  70 PLOT_XY x, SI(\"Gypsum\"), y-axis = 2, line_width = 2, symbol = Circle, \\\n"
	"  	symbol_size = 8, color = Magenta\n"
	"END\n"
	,
	"Example 5—Irreversible Reactions\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ex6(
	"TITLE Simulation 6A.--React to phase boundaries.\n"
	"SOLUTION 1  PURE WATER\n"
	"            pH    7.0 charge\n"
	"          temp   25.0\n"
	"PHASES\n"
	"        Gibbsite\n"
	"                Al(OH)3 + 3 H+ = Al+3 + 3 H2O\n"
	"                log_k          8.049\n"
	"                delta_h        -22.792 kcal\n"
	"        Kaolinite\n"
	"                Al2Si2O5(OH)4 + 6 H+ = H2O + 2 H4SiO4 + 2 Al+3\n"
	"                log_k          5.708\n"
	"                delta_h        -35.306 kcal\n"
	"        K-mica\n"
	"                KAl3Si3O10(OH)2 + 10 H+ = 3 Al+3 + 3 H4SiO4 + K+\n"
	"                log_k          12.970\n"
	"                delta_h        -59.377 kcal\n"
	"        K-feldspar\n"
	"                KAlSi3O8 + 4 H2O + 4 H+ = Al+3 + 3 H4SiO4 + K+\n"
	"                log_k          0.875\n"
	"                delta_h        -12.467 kcal\n"
	"SELECTED_OUTPUT\n"
	"        -file           ex6A-B.sel\n"
	"        -activities     K+ H+ H4SiO4\n"
	"        -si             Gibbsite Kaolinite K-mica K-feldspar\n"
	"        -equilibrium    Gibbsite Kaolinite K-mica K-feldspar\n"
	"END\n"
	"TITLE Simulation 6A1.--Find amount of K-feldspar dissolved to\n"
	"                    reach gibbsite saturation.\n"
	"USE solution 1\n"
	"EQUILIBRIUM_PHASES 1\n"
	"        Gibbsite        0.0     KAlSi3O8        10.0\n"
	"        Kaolinite       0.0     0.0\n"
	"        K-mica          0.0     0.0\n"
	"        K-feldspar      0.0     0.0\n"
	"USER_GRAPH 1 Simulation 6\n"
	"        -headings 6A--Intersections\n"
	"        -chart_title \"K-Feldspar Reaction Path\"\n"
	"        -axis_titles \"Log[H4SiO4]\" \"Log([K+] / [H+])\"\n"
	"        -axis_scale x_axis -8.0 0.0 1 1\n"
	"        -axis_scale y_axis -1.0 8.0 1 1\n"
	"  10 PLOT_XY LA(\"H4SiO4\"),(LA(\"K+\")-LA(\"H+\")), color = Red, line_w = 0, \\\n"
	"  	symbol = Circle, symbol_size = 10\n"
	"END\n"
	"TITLE Simulation 6A2.--Find amount of K-feldspar dissolved to\n"
	"                    reach kaolinite saturation.\n"
	"USE solution 1\n"
	"EQUILIBRIUM_PHASES 1\n"
	"        Gibbsite        0.0     0.0\n"
	"        Kaolinite       0.0     KAlSi3O8        10.0\n"
	"        K-mica          0.0     0.0\n"
	"        K-feldspar      0.0     0.0\n"
	"END\n"
	"TITLE Simulation 6A3.--Find amount of K-feldspar dissolved to\n"
	"                    reach K-mica saturation.\n"
	"USE solution 1\n"
	"EQUILIBRIUM_PHASES 1\n"
	"        Gibbsite        0.0     0.0\n"
	"        Kaolinite       0.0     0.0\n"
	"        K-mica          0.0     KAlSi3O8        10.0\n"
	"        K-feldspar      0.0     0.0\n"
	"END\n"
	"TITLE Simulation 6A4.--Find amount of K-feldspar dissolved to\n"
	"                    reach K-feldspar saturation.\n"
	"USE solution 1\n"
	"EQUILIBRIUM_PHASES 1\n"
	"        Gibbsite        0.0     0.0\n"
	"        Kaolinite       0.0     0.0\n"
	"        K-mica          0.0     0.0\n"
	"        K-feldspar      0.0     KAlSi3O8        10.0\n"
	"END\n"
	"TITLE Simulation 6A5.--Find point with kaolinite present,\n"
	"                    but no gibbsite.\n"
	"USE solution 1\n"
	"EQUILIBRIUM_PHASES 1\n"
	"        Gibbsite        0.0     KAlSi3O8        10.0\n"
	"        Kaolinite       0.0     1.0\n"
	"END\n"
	"TITLE Simulation 6A6.--Find point with K-mica present,\n"
	"                    but no kaolinite\n"
	"USE solution 1\n"
	"EQUILIBRIUM_PHASES 1\n"
	"        Kaolinite       0.0     KAlSi3O8        10.0\n"
	"        K-mica          0.0     1.0\n"
	"END\n"
	"TITLE Simulation 6B.--Path between phase boundaries.\n"
	"USE solution 1\n"
	"EQUILIBRIUM_PHASES 1\n"
	"        Kaolinite       0.0     0.0\n"
	"        Gibbsite        0.0     0.0\n"
	"        K-mica          0.0     0.0\n"
	"        K-feldspar      0.0     0.0\n"
	"REACTION 1\n"
	"        K-feldspar      1.0\n"
	"        0.04 0.08 0.16 0.32 0.64 1.0 2.0 4.0\n"
	"        8.0 16.0 32.0 64.0 100 200 umol\n"
	"USER_GRAPH\n"
	"        -headings 6B--Increments\n"
	"  10 PLOT_XY LA(\"H4SiO4\"),(LA(\"K+\")-LA(\"H+\")), color = Blue, line_w = 0, \\\n"
	"  		symbol = XCross, symbol_size = 7\n"
	"END\n"
	"TITLE Simulation 6C.--kinetic calculation\n"
	"SOLUTION 1\n"
	"        -units mol/kgw\n"
	"        Al         1.e-13\n"
	"        K          1.e-13\n"
	"        Si         3.e-13\n"
	"EQUILIBRIUM_PHASES 1\n"
	"        Gibbsite   0.0  0.0\n"
	"        Kaolinite  0.0  0.0\n"
	"        K-mica     0.0  0.0\n"
	"KINETICS 1\n"
	"K-feldspar\n"
	"# k0 * A/V = 1e-16 mol/cm2/s * (10% fsp, 0.1mm cubes) 136/cm = 136.e-13 mol/dm3/s\n"
	"        -parms 1.36e-11\n"
	"        -m0 2.16\n"
	"        -m  1.94\n"
	"        -step_divide 1e-6\n"
	"        -steps    1e2 1e3 1e4 1e5 1e6 1e7 1e8\n"
	"#        -steps    1e2 1e3 1e4 1e5 63240.0 64950.0 1347610.0 1010300.0 45242800.0\n"
	"INCREMENTAL_REACTIONS true\n"
	"RATES\n"
	"K-feldspar\n"
	"-start\n"
	"  10  REM store the initial amount of K-feldspar\n"
	"  20  IF EXISTS(1) = 0 THEN PUT(M, 1)\n"
	"  30  REM calculate moles of reaction\n"
	"  40  SR_kfld = SR(\"K-feldspar\")\n"
	"  50  moles = PARM(1) * (M/M0)^0.67 * (1 - SR_kfld) * TIME\n"
	"  60  REM The following is for printout of phase transitions\n"
	"  80  REM      Start Gibbsite\n"
	"  90  if ABS(SI(\"Gibbsite\")) > 1e-3 THEN GOTO 150\n"
	"  100   i = 2\n"
	"  110   GOSUB 1500\n"
	"  150 REM      Start Gibbsite -> Kaolinite\n"
	"  160 if ABS(SI(\"Kaolinite\")) > 1e-3 THEN GOTO 200\n"
	"  170   i = 3\n"
	"  180   GOSUB 1500\n"
	"  200 REM      End Gibbsite -> Kaolinite\n"
	"  210 if ABS(SI(\"Kaolinite\")) > 1e-3 OR EQUI(\"Gibbsite\") > 0 THEN GOTO 250\n"
	"  220   i = 4\n"
	"  230   GOSUB 1500\n"
	"  250 REM      Start Kaolinite -> K-mica\n"
	"  260 if ABS(SI(\"K-mica\")) > 1e-3 THEN GOTO 300\n"
	"  270   i = 5\n"
	"  280   GOSUB 1500\n"
	"  300 REM      End Kaolinite -> K-mica\n"
	"  310 if ABS(SI(\"K-mica\")) > 1e-3 OR EQUI(\"Kaolinite\") > 0 THEN GOTO 350\n"
	"  320   i = 6\n"
	"  330   GOSUB 1500\n"
	"  350 REM      Start K-mica -> K-feldspar\n"
	"  360 if ABS(SI(\"K-feldspar\")) > 1e-3 THEN GOTO 1000\n"
	"  370   i = 7\n"
	"  380   GOSUB 1500\n"
	"  1000 SAVE moles\n"
	"  1010 END\n"
	"  1500 REM subroutine to store data\n"
	"  1510 if GET(i) >= M THEN RETURN\n"
	"  1520     PUT(M, i)\n"
	"  1530     PUT(TOTAL_TIME, i, 1)\n"
	"  1540     PUT(LA(\"K+\")-LA(\"H+\"), i, 2)\n"
	"  1550     PUT(LA(\"H4SiO4\"), i, 3)\n"
	"  1560 RETURN\n"
	"-end\n"
	"USER_PRINT\n"
	"  10 DATA \"A: Gibbsite               \", \"B: Gibbsite  -> Kaolinite \", \\\n"
	"          \"C: Gibbsite  -> Kaolinite \", \"D: Kaolinite -> K-mica    \", \\\n"
	"          \"E: Kaolinite -> K-mica    \", \"F: K-mica    -> K-feldspar\"\n"
	"  20 PRINT \\\n"
	"  \"        Transition                 Time   K-feldspar      LA(K/H)   LA(H4SiO4)\"\n"
	"  30 PRINT \"                                            transfer\"\n"
	"  40 PRINT \"                                            (umoles)\"\n"
	"  50 FOR i = 2 TO 7\n"
	"  60   READ s$\n"
	"  70   IF EXISTS(i) THEN PRINT s$, GET(i,1), (GET(1) - GET(i))*1e6, GET(i,2), GET(i,3)\n"
	"  80 NEXT i\n"
	"SELECTED_OUTPUT\n"
	"        -file ex6C.sel\n"
	"        -reset false\n"
	"USER_PUNCH\n"
	"  -headings pH+log[K]  log[H4SiO4]\n"
	"  10 PUNCH LA(\"K+\")-LA(\"H+\") LA(\"H4SiO4\")\n"
	"USER_GRAPH\n"
	"  -headings 6C--Kinetics\n"
	"  10 PLOT_XY LA(\"H4SiO4\"),(LA(\"K+\")-LA(\"H+\")), color = Blue, line_w = 2, symbol = None\n"
	"END\n"
	"PRINT; -user_print false\n"
	"# --Plot the phase boundaries with USER_GRAPH..\n"
	"PHASES\n"
	" K_H; KH = K+ - H+; -no_check\n"
	"USER_GRAPH\n"
	"-initial_solutions true\n"
	" 10 PLOT_XY LA(\"H4SiO4\"), SI(\"K_H\"), color = Black, symbol = None \n"
	"SOLUTION 1\n"
	" pH 11; K 1 K_H 8; Al 1 Gibbsite; Si 1 K-mica\n"
	"SOLUTION 2\n"
	" pH 7; K 1 K-mica; Al 1 Gibbsite; Si 1 Kaolinite\n"
	"SOLUTION 3\n"
	" pH 7; K 1 K-mica; Al 1 K-feldspar; Si 1 Kaolinite\n"
	"SOLUTION 4\n"
	" pH 7; K 1 K_H -1; Al 1 Kaolinite; Si 1 K-feldspar\n"
	"END\n"
	"USER_GRAPH\n"
	" 10 PLOT_XY LA(\"H4SiO4\"), SI(\"K_H\"), color = Black, symbol = None \n"
	"SOLUTION 1\n"
	" pH 11; K 1 K_H 8; Al 1 K-feldspar; Si 1 K-mica\n"
	"SOLUTION 2\n"
	" pH 7; K 1 K-mica; Al 1 K-feldspar; Si 1 Kaolinite\n"
	"SOLUTION 3\n"
	" pH 7; K 1 K-mica; Al 1 Gibbsite; Si 1 Kaolinite\n"
	"SOLUTION 4\n"
	" pH 7; K 1 K_H -1; Al 1 Gibbsite; Si 1 Kaolinite\n"
	"END \n"
	,
	"Example 6—Reaction-Path Calculations\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ex7(
	"TITLE Example 7.--Organic decomposition with fixed-pressure and\n"
	"                  fixed-volume gas phases\n"
	"SOLUTION_MASTER_SPECIES\n"
	"N(-3)    NH4+           0.0     N\n"
	"SOLUTION_SPECIES\n"
	"NH4+ = NH3 + H+\n"
	"        log_k           -9.252\n"
	"        delta_h 12.48   kcal\n"
	"        -analytic    0.6322    -0.001225     -2835.76\n"
	"\n"
	"NO3- + 10 H+ + 8 e- = NH4+ + 3 H2O\n"
	"        log_k           119.077\n"
	"        delta_h -187.055        kcal\n"
	"        -gamma    2.5000    0.0000\n"
	"PHASES\n"
	"NH3(g)\n"
	"        NH3 = NH3\n"
	"        log_k           1.770\n"
	"        delta_h -8.170  kcal\n"
	"SOLUTION 1\n"
	"EQUILIBRIUM_PHASES 1\n"
	"        Calcite\n"
	"        CO2(g)  -1.5\n"
	"SAVE solution 1\n"
	"SELECTED_OUTPUT\n"
	"        -reset false\n"
	"        -file ex7.sel\n"
	"        -simulation     true\n"
	"        -state          true\n"
	"        -reaction       true\n"
	"        -si CO2(g) CH4(g) N2(g) NH3(g)\n"
	"        -gas CO2(g) CH4(g) N2(g) NH3(g)\n"
	"END\n"
	"#  Simulation 2: Decomposition of organic matter, CH2O(NH3).07,\n"
	"#  at fixed pressure of 1.1 atm\n"
	"USE solution 1\n"
	"GAS_PHASE 1 Fixed-pressure gas phase\n"
	"        -fixed_pressure\n"
	"        -pressure       1.1\n"
	"        CO2(g)          0.0\n"
	"        CH4(g)          0.0\n"
	"        N2(g)           0.0\n"
	"        H2O(g)          0.0\n"
	"REACTION 1\n"
	"        CH2O(NH3)0.07     1.0\n"
	"        1. 2. 3. 4. 8. 16. 32 64. 125. 250. 500. 1000. mmol\n"
	"USER_GRAPH 1 Example 7\n"
	"        -headings Fixed_Pressure: CH4 CO2 N2 H2O #Volume\n"
	"        -chart_title \"Gas Composition\"\n"
	"        -axis_titles \"Organic matter reacted, in millimoles\" \\\n"
	"        	     \"Log(Partial pressure, in atmospheres)\" \"Volume, in liters\"\n"
	"        -axis_scale x_axis 1 1e3 auto auto log\n"
	"        -axis_scale y_axis -5.0 1.0 1 1\n"
	"        -connect_simulations false\n"
	"  -start\n"
	"  10 IF GAS(\"CH4(g)\") < 1e-10 THEN GOTO 100\n"
	"  20 mM_OM = RXN * 1e3\n"
	"  30 PLOT_XY -10, -10, line_width = 0, symbol_size = 0\n"
	"  40 PLOT_XY mM_OM, SI(\"CH4(g)\"), color = Black, symbol = XCross\n"
	"  50 PLOT_XY mM_OM, SI(\"CO2(g)\"), color = Red, symbol = XCross\n"
	"  60 PLOT_XY mM_OM, SI(\"N2(g)\"), color = Teal, symbol = XCross\n"
	"  70 PLOT_XY mM_OM, SI(\"H2O(g)\"), color = Blue, symbol = XCross\n"
	"  100 REM end of program\n"
	"  -end\n"
	"USER_GRAPH 2 Example 7\n"
	"        -headings  Fixed_P:...Pressure Fixed_P:...Volume\n"
	"        -chart_title \\\n"
	"            \"Total Gas Pressure and Volume\"\n"
	"        -axis_titles \"Organic matter reacted, in millimoles\" \\\n"
	"                     \"Log(Pressure, in atmospheres)\" \"Volume, in liters\"\n"
	"        -axis_scale x_axis 1 1e3 auto auto log\n"
	"        -axis_scale y_axis -5.0 1.0 1 1\n"
	"        -axis_scale y2_axis 1e-3 1e5 auto auto log\n"
	"        -connect_simulations false\n"
	"  -start\n"
	"  10 IF GAS(\"CH4(g)\") < 1e-10 THEN GOTO 100\n"
	"  20 mM_OM = RXN * 1e3\n"
	"  30 moles = (GAS(\"CH4(g)\") + GAS(\"CO2(g)\") + GAS(\"N2(g)\") + GAS(\"H2O(g)\"))\n"
	"  40 vol = moles * 0.08207 * TK / 1.1\n"
	"  50 PLOT_XY mM_OM, LOG10(1.1), color = Magenta, symbol = XCross\n"
	"  60 PLOT_XY mM_OM, vol, color = Cyan, symbol = XCross, y_axis = 2\n"
	"  100 REM end of program\n"
	"  -end\n"
	"END\n"
	"#  Simulation 3: Decomposition of organic matter, CH2O(NH3).07,\n"
	"#  at fixed volume of 23.19 L\n"
	"USE solution 1\n"
	"USE reaction 1\n"
	"GAS_PHASE 1 Fixed volume gas phase\n"
	"        -fixed_volume\n"
	"        -volume         23.19\n"
	"        CO2(g)          0.0\n"
	"        CH4(g)          0.0\n"
	"        N2(g)           0.0\n"
	"        H2O(g)          0.0\n"
	"        -equilibrate 1\n"
	"USER_GRAPH 1\n"
	"        -headings Fixed_Volume: CH4 CO2 N2 H2O\n"
	"  -start\n"
	"  10 mM_OM = RXN * 1e3\n"
	"  20 PLOT_XY -10, -10, line_width = 0, symbol_size = 0\n"
	"  30 PLOT_XY mM_OM, SI(\"CH4(g)\"), color = Black, symbol = Circle\n"
	"  40 PLOT_XY mM_OM, SI(\"CO2(g)\"), color = Red, symbol = Circle\n"
	"  50 PLOT_XY mM_OM, SI(\"N2(g)\"), color = Teal, symbol = Circle\n"
	"  60 PLOT_XY mM_OM, SI(\"H2O(g)\"), color = Blue, symbol = Circle, symbol_size = 5\n"
	"  -end\n"
	"USER_GRAPH 2\n"
	"        -headings Fixed_V:...Pressure Fixed_V:...Volume\n"
	"  -start\n"
	"  10 mM_OM = RXN * 1e3\n"
	"  20 tot_p = SR(\"CH4(g)\") + SR(\"CO2(g)\") + SR(\"N2(g)\") + SR(\"H2O(g)\")\n"
	"  30 PLOT_XY mM_OM, LOG10(tot_p), color = Magenta, symbol = Circle\n"
	"  40 PLOT_XY mM_OM, 23.19, color = Cyan, line_width = 1 symbol = Circle, y_axis = 2\n"
	"  -end\n"
	"END\n"
	,
	"Example 7—Gas-Phase Calculations\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ex8(
	"TITLE Example 8.--Sorption of zinc on hydrous iron oxides.\n"
	"SURFACE_SPECIES\n"
	"     Hfo_sOH  + H+ = Hfo_sOH2+\n"
	"     log_k  7.18\n"
	"     Hfo_sOH = Hfo_sO- + H+\n"
	"     log_k  -8.82\n"
	"     Hfo_sOH + Zn+2 = Hfo_sOZn+ + H+\n"
	"     log_k  0.66\n"
	"     Hfo_wOH  + H+ = Hfo_wOH2+\n"
	"     log_k  7.18\n"
	"     Hfo_wOH = Hfo_wO- + H+\n"
	"     log_k  -8.82\n"
	"     Hfo_wOH + Zn+2 = Hfo_wOZn+ + H+\n"
	"     log_k  -2.32\n"
	"SURFACE 1\n"
	"     Hfo_sOH        5e-6    600.    0.09\n"
	"     Hfo_wOH        2e-4\n"
	"#     -donnan\n"
	"END\n"
	"SOLUTION 1\n"
	"     -units  mmol/kgw\n"
	"     pH      8.0\n"
	"     Zn      0.0001 \n"
	"     Na      100.    charge \n"
	"     N(5)    100.\n"
	"SELECTED_OUTPUT\n"
	"     -file Zn1e_7\n"
	"     -reset false\n"
	"USER_PUNCH\n"
	"  10 FOR i = 5.0 to 8 STEP 0.25\n"
	"  20 a$ = EOL$ + \"USE solution 1\" + CHR$(59) + \" USE surface 1\" + EOL$\n"
	"  30 a$ = a$ + \"EQUILIBRIUM_PHASES 1\" + EOL$\n"
	"  40 a$ = a$ + \"   Fix_H+ \" + STR$(-i) + \" NaOH 10.0\" + EOL$\n"
	"  50 a$ = a$ + \"END\" + EOL$\n"
	"  60 PUNCH a$\n"
	"  70 NEXT i\n"
	"END\n"
	"SOLUTION 2\n"
	"     -units  mmol/kgw\n"
	"     pH      8.0\n"
	"     Zn      0.1 \n"
	"     Na      100.    charge \n"
	"     N(5)    100.\n"
	"SELECTED_OUTPUT\n"
	"     -file Zn1e_4\n"
	"     -reset false\n"
	"USER_PUNCH\n"
	"  10 FOR i = 5 to 8 STEP 0.25\n"
	"  20 a$ = EOL$ + \"USE solution 2\" + CHR$(59) + \" USE surface 1\" + EOL$\n"
	"  30 a$ = a$ + \"EQUILIBRIUM_PHASES 1\" + EOL$\n"
	"  40 a$ = a$ + \"   Fix_H+ \" + STR$(-i) + \" NaOH 10.0\" + EOL$\n"
	"  50 a$ = a$ + \"END\" + EOL$\n"
	"  60 PUNCH a$\n"
	"  70 NEXT i\n"
	"END\n"
	"#\n"
	"# Model definitions\n"
	"#\n"
	"PHASES\n"
	"     Fix_H+\n"
	"     H+ = H+\n"
	"     log_k  0.0\n"
	"END\n"
	"#\n"
	"#   Zn = 1e-7\n"
	"SELECTED_OUTPUT\n"
	"     -file ex8.sel\n"
	"     -reset true\n"
	"     -molalities     Zn+2    Hfo_wOZn+      Hfo_sOZn+\n"
	"USER_PUNCH\n"
	" 10\n"
	"USER_GRAPH 1 Example 8\n"
	"     -headings pH Zn_solute Zn_weak_sites Zn_strong_sites Charge_balance\n"
	"     -chart_title \"Total Zn = 1e-7 molal\"\n"
	"     -axis_titles pH \"Moles per kilogram water\" \"Charge balance, in milliequivalents\"\n"
	"     -axis_scale x_axis 5.0 8.0 1 0.25\n"
	"     -axis_scale y_axis 1e-11 1e-6 1 1 log\n"
	"     -axis_scale sy_axis -0.15 0 0.03\n"
	"  -start\n"
	"  10 GRAPH_X -LA(\"H+\")\n"
	"  20 GRAPH_Y MOL(\"Zn+2\"), MOL(\"Hfo_wOZn+\"), MOL(\"Hfo_sOZn+\")\n"
	"  30 GRAPH_SY CHARGE_BALANCE * 1e3\n"
	"  -end\n"
	"INCLUDE$ Zn1e_7\n"
	"END\n"
	"USER_GRAPH 1\n"
	"     -detach\n"
	"END\n"
	"#\n"
	"#   Zn = 1e-4\n"
	"USER_GRAPH 2 Example 8\n"
	"     -chart_title \"Total Zn = 1e-4 molal\"\n"
	"     -headings pH Zn_solute Zn_weak_sites Zn_strong_sites Charge_balance\n"
	"     -axis_titles pH \"Moles per kilogram water\" \"Charge balance, in milliequivalents\"\n"
	"     -axis_scale x_axis 5.0 8.0 1 0.25\n"
	"     -axis_scale y_axis 1e-8 1e-3 1 1 log\n"
	"     -axis_scale sy_axis -0.15 0 0.03\n"
	"  -start\n"
	"  10 GRAPH_X -LA(\"H+\")\n"
	"  20 GRAPH_Y MOL(\"Zn+2\"), MOL(\"Hfo_wOZn+\"), MOL(\"Hfo_sOZn+\")\n"
	"  30 GRAPH_SY CHARGE_BALANCE * 1e3\n"
	"  -end        \n"
	"INCLUDE$ Zn1e_4\n"
	"END\n"	,
	"Example 8—Surface Complexation\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ex9(
	"TITLE Example 9.--Kinetically controlled oxidation of ferrous \n"
	"                  iron. Decoupled valence states of iron.\n"
	"SOLUTION_MASTER_SPECIES\n"
	"Fe_di              Fe_di+2    0.0     Fe_di              55.847\n"
	"Fe_tri             Fe_tri+3   0.0     Fe_tri             55.847\n"
	"SOLUTION_SPECIES\n"
	"Fe_di+2 = Fe_di+2\n"
	"        log_k   0.0\n"
	"Fe_tri+3 = Fe_tri+3\n"
	"        log_k   0.0\n"
	"#\n"
	"# Fe+2 species\n"
	"#\n"
	"Fe_di+2 + H2O = Fe_diOH+ + H+\n"
	"        log_k   -9.5\n"
	"        delta_h 13.20   kcal\n"
	"#\n"
	"#... and also other Fe+2 species\n"
	"#\n"
	"Fe_di+2 + Cl- = Fe_diCl+\n"
	"        log_k   0.14\n"
	"Fe_di+2 + CO3-2 = Fe_diCO3\n"
	"        log_k   4.38\n"
	"Fe_di+2 + HCO3- = Fe_diHCO3+\n"
	"        log_k   2.0\n"
	"Fe_di+2 + SO4-2 = Fe_diSO4\n"
	"        log_k   2.25\n"
	"        delta_h 3.230   kcal\n"
	"Fe_di+2 + HSO4- = Fe_diHSO4+\n"
	"        log_k   1.08\n"
	"Fe_di+2 + 2HS- = Fe_di(HS)2\n"
	"        log_k   8.95\n"
	"Fe_di+2 + 3HS- = Fe_di(HS)3-\n"
	"        log_k   10.987\n"
	"Fe_di+2 + HPO4-2 = Fe_diHPO4\n"
	"        log_k   3.6\n"
	"Fe_di+2 + H2PO4- = Fe_diH2PO4+\n"
	"        log_k   2.7\n"
	"Fe_di+2 + F- = Fe_diF+\n"
	"        log_k   1.0\n"
	"#\n"
	"# Fe+3 species\n"
	"#\n"
	"Fe_tri+3 + H2O = Fe_triOH+2 + H+\n"
	"        log_k   -2.19\n"
	"        delta_h 10.4    kcal\n"
	"#\n"
	"#... and also other Fe+3 species\n"
	"#\n"
	"Fe_tri+3 + 2 H2O = Fe_tri(OH)2+ + 2 H+\n"
	"        log_k   -5.67\n"
	"        delta_h 17.1    kcal\n"
	"Fe_tri+3 + 3 H2O = Fe_tri(OH)3 + 3 H+\n"
	"        log_k   -12.56\n"
	"        delta_h 24.8    kcal\n"
	"Fe_tri+3 + 4 H2O = Fe_tri(OH)4- + 4 H+\n"
	"        log_k   -21.6\n"
	"        delta_h 31.9    kcal\n"
	"2 Fe_tri+3 + 2 H2O = Fe_tri2(OH)2+4 + 2 H+\n"
	"        log_k   -2.95\n"
	"        delta_h 13.5    kcal\n"
	"3 Fe_tri+3 + 4 H2O = Fe_tri3(OH)4+5 + 4 H+\n"
	"        log_k   -6.3\n"
	"        delta_h 14.3    kcal\n"
	"Fe_tri+3 + Cl- = Fe_triCl+2\n"
	"        log_k   1.48\n"
	"        delta_h 5.6     kcal\n"
	"Fe_tri+3 + 2 Cl- = Fe_triCl2+\n"
	"        log_k   2.13\n"
	"Fe_tri+3 + 3 Cl- = Fe_triCl3\n"
	"        log_k   1.13\n"
	"Fe_tri+3 + SO4-2 = Fe_triSO4+\n"
	"        log_k   4.04\n"
	"        delta_h 3.91    kcal\n"
	"Fe_tri+3 + HSO4- = Fe_triHSO4+2\n"
	"        log_k   2.48\n"
	"Fe_tri+3 + 2 SO4-2 = Fe_tri(SO4)2-\n"
	"        log_k   5.38\n"
	"        delta_h 4.60    kcal\n"
	"Fe_tri+3 + HPO4-2 = Fe_triHPO4+\n"
	"        log_k   5.43\n"
	"        delta_h 5.76    kcal\n"
	"Fe_tri+3 + H2PO4- = Fe_triH2PO4+2\n"
	"        log_k   5.43\n"
	"Fe_tri+3 + F- = Fe_triF+2\n"
	"        log_k   6.2\n"
	"        delta_h 2.7     kcal\n"
	"Fe_tri+3 + 2 F- = Fe_triF2+\n"
	"        log_k   10.8\n"
	"        delta_h 4.8     kcal\n"
	"Fe_tri+3 + 3 F- = Fe_triF3\n"
	"        log_k   14.0\n"
	"        delta_h 5.4     kcal\n"
	"PHASES\n"
	"Goethite\n"
	"        Fe_triOOH + 3 H+ = Fe_tri+3 + 2 H2O\n"
	"        log_k   -1.0\n"
	"END\n"
	"SOLUTION 1\n"
	"        pH  7.0\n"
	"        pe 10.0  O2(g) -0.67\n"
	"        Fe_di  0.1\n"
	"        Na  10.\n"
	"        Cl  10.  charge\n"
	"EQUILIBRIUM_PHASES 1\n"
	"        O2(g)           -0.67\n"
	"RATES\n"
	"Fe_di_ox\n"
	"  -start\n"
	"  10  Fe_di = TOT(\"Fe_di\")\n"
	"  20  if (Fe_di <= 0) then goto 200\n"
	"  30  p_o2 = SR(\"O2(g)\")\n"
	"  40  moles = (2.91e-9 + 1.33e12 * (ACT(\"OH-\"))^2 * p_o2) * Fe_di * TIME\n"
	"  200 SAVE moles\n"
	"  -end\n"
	"KINETICS 1\n"
	"Fe_di_ox\n"
	"        -formula  Fe_di  -1.0  Fe_tri  1.0\n"
	"        -steps 100 400 3100 10800 21600 5.04e4 8.64e4 1.728e5 1.728e5 1.728e5 1.728e5 \n"
	"        -step_divide 1e-4\n"
	"INCREMENTAL_REACTIONS true\n"
	"SELECTED_OUTPUT\n"
	"        -file ex9.sel\n"
	"        -reset false\n"
	"USER_PUNCH\n"
	"        -headings Days  Fe(2)  Fe(3)  pH  si_goethite\n"
	"  10 PUNCH SIM_TIME / 3600 / 24, TOT(\"Fe_di\")*1e6, TOT(\"Fe_tri\")*1e6, -LA(\"H+\"), \\\n"
	"           SI(\"Goethite\")\n"
	"USER_GRAPH Example 9\n"
	"        -headings _time_ Fe(2) Fe(3) pH\n"
	"        -chart_title \"Oxidation of Ferrous Iron\"\n"
	"        -axis_titles \"Time, in days\" \"Micromole per kilogram water\" \"pH\"\n"
	"        -axis_scale secondary_y_axis 4.0 7.0 1.0 0.5\n"
	"  -start\n"
	"  10 GRAPH_X TOTAL_TIME / 3600 / 24 \n"
	"  20 GRAPH_Y TOT(\"Fe_tri\")*1e6, TOT(\"Fe_di\")*1e6 \n"
	"  30 GRAPH_SY -LA(\"H+\")\n"
	"  -end\n"
	"END\n"
	,
	"Example 9—Kinetic Oxidation of Dissolved Ferrous Iron with Oxygen\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ex10(
	"TITLE Example 10.--Solid solution of strontianite and aragonite.\n"
	"PHASES\n"
	"        Strontianite\n"
	"                SrCO3 = CO3-2 + Sr+2\n"
	"                log_k           -9.271\n"
	"        Aragonite\n"
	"                CaCO3 = CO3-2 + Ca+2\n"
	"                log_k           -8.336\n"
	"END\n"
	"SOLID_SOLUTIONS 1\n"
	"        Ca(x)Sr(1-x)CO3 \n"
	"                -comp1   Aragonite       0 \n"
	"                -comp2   Strontianite    0 \n"
	"                -Gugg_nondim   3.43    -1.82\n"
	"END\n"
	"SOLUTION 1\n"
	"        -units mmol/kgw\n"
	"        pH 5.93 charge\n"
	"        Ca      3.932\n"
	"        C       7.864\n"
	"EQUILIBRIUM_PHASES 1\n"
	"        CO2(g) -0.01265 10\n"
	"        Aragonite\n"
	"SAVE solution 1\n"
	"END\n"
	"#\n"
	"#  Total of 0.00001 to 0.005 moles of SrCO3 added\n"
	"#\n"
	"USE solution 1\n"
	"USE solid_solution 1\n"
	"REACTION 1\n"
	"        SrCO3   1.0\n"
	"        .005 in 500 steps \n"
	"PRINT\n"
	"        -reset false\n"
	"        -echo true\n"
	"        -user_print true\n"
	"USER_PRINT\n"
	"-start\n"
	"  10 sum = (S_S(\"Strontianite\") + S_S(\"Aragonite\"))\n"
	"  20 if sum = 0 THEN GOTO 110\n"
	"  30 xb = S_S(\"Strontianite\")/sum\n"
	"  40 xc = S_S(\"Aragonite\")/sum\n"
	"  50 PRINT \"Simulation number:    \", SIM_NO\n"
	"  60 PRINT \"Reaction step number: \", STEP_NO\n"
	"  70 PRINT \"SrCO3 added:          \", RXN\n"
	"  80 PRINT \"Log Sigma pi:         \", LOG10 (ACT(\"CO3-2\") * (ACT(\"Ca+2\") + ACT(\"Sr+2\")))\n"
	"  90 PRINT \"XAragonite:           \", xc\n"
	" 100 PRINT \"XStrontianite:        \", xb\n"
	" 110 PRINT \"XCa:                  \", TOT(\"Ca\")/(TOT(\"Ca\") + TOT(\"Sr\"))\n"
	" 120 PRINT \"XSr:                  \", TOT(\"Sr\")/(TOT(\"Ca\") + TOT(\"Sr\"))\n"
	" 130 PRINT \"Misc 1:               \", MISC1(\"Ca(x)Sr(1-x)CO3\")\n"
	" 140 PRINT \"Misc 2:               \", MISC2(\"Ca(x)Sr(1-x)CO3\")\n"
	"-end\n"
	"SELECTED_OUTPUT\n"
	"        -file ex10.sel\n"
	"        -reset false\n"
	"        -reaction true\n"
	"USER_PUNCH\n"
	"-head   lg_SigmaPi X_Arag X_Stront X_Ca_aq X_Sr_aq mol_Misc1 mol_Misc2 \\\n"
	"     mol_Arag mol_Stront\n"
	"-start\n"
	"  10 sum = (S_S(\"Strontianite\") + S_S(\"Aragonite\"))\n"
	"  20 if sum = 0 THEN GOTO 60\n"
	"  30 xb = S_S(\"Strontianite\")/(S_S(\"Strontianite\") + S_S(\"Aragonite\"))\n"
	"  40 xc = S_S(\"Aragonite\")/(S_S(\"Strontianite\") + S_S(\"Aragonite\"))\n"
	"  50 REM Sigma Pi\n"
	"  60 PUNCH LOG10(ACT(\"CO3-2\") * (ACT(\"Ca+2\") + ACT(\"Sr+2\")))\n"
	"  70 PUNCH xc                                 # Mole fraction aragonite\n"
	"  80 PUNCH xb                                 # Mole fraction strontianite\n"
	"  90 PUNCH TOT(\"Ca\")/(TOT(\"Ca\") + TOT(\"Sr\"))  # Mole aqueous calcium\n"
	"  100 PUNCH TOT(\"Sr\")/(TOT(\"Ca\") + TOT(\"Sr\")) # Mole aqueous strontium\n"
	"  110 x1 = MISC1(\"Ca(x)Sr(1-x)CO3\")\n"
	"  120 x2 = MISC2(\"Ca(x)Sr(1-x)CO3\")\n"
	"  130 if (xb < x1 OR xb > x2) THEN GOTO 250\n"
	"  140    nc = S_S(\"Aragonite\")\n"
	"  150    nb = S_S(\"Strontianite\")\n"
	"  160    mol2 = ((x1 - 1)/x1)*nb + nc\n"
	"  170    mol2 = mol2 / ( ((x1 -1)/x1)*x2 + (1 - x2))\n"
	"  180    mol1 = (nb - mol2*x2)/x1\n"
	"  190    REM                                 # Moles of misc. end members if in gap\n"
	"  200    PUNCH mol1\n"
	"  210    PUNCH mol2\n"
	"  220    GOTO 300\n"
	"  250    REM                                 # Moles of misc. end members if not in gap\n"
	"  260    PUNCH 1e-10\n"
	"  270    PUNCH 1e-10\n"
	"  300 PUNCH S_S(\"Aragonite\")                 # Moles aragonite\n"
	"  310 PUNCH S_S(\"Strontianite\")              # Moles Strontianite\n"
	"-end\n"
	"USER_GRAPH Example 10\n"
	"        -headings x_Aragonite  x_Srontianite\n"
	"        -chart_title \"Aragonite-Strontianite Solid Solution\"\n"
	"        -axis_titles \"Log(SrCO3 added, in moles)\" \"Log(Mole fraction of component)\"\n"
	"        -axis_scale x_axis -5 1 1 1\n"
	"        -axis_scale y_axis -5 0.1 1 1\n"
	"        -connect_simulations true\n"
	"        -start\n"
	"  10 sum = (S_S(\"Strontianite\") + S_S(\"Aragonite\"))\n"
	"  20 IF sum = 0 THEN GOTO 70\n"
	"  30 xb = S_S(\"Strontianite\")/ sum\n"
	"  40 xc = S_S(\"Aragonite\")/ sum\n"
	"  50 PLOT_XY LOG10(RXN), LOG10(xc), line_w = 2, symbol_size = 0\n"
	"  60 PLOT_XY LOG10(RXN), LOG10(xb), line_w = 2, symbol_size = 0\n"
	"  70 rem\n"
	"  -end\n"
	"END     \n"
	"#\n"
	"#  Total of 0.005 to 0.1 moles of SrCO3 added\n"
	"#\n"
	"USE solution 1\n"
	"USE solid_solution 1\n"
	"REACTION 1\n"
	"        SrCO3   1.0\n"
	"        .1 in 20 steps \n"
	"END     \n"
	"#\n"
	"#  Total of 0.1 to 10 moles of SrCO3 added\n"
	"#\n"
	"USE solution 1\n"
	"USE solid_solution 1\n"
	"REACTION 1\n"
	"        SrCO3   1.0\n"
	"        10.0 in 100 steps \n"
	"END     \n"
	,
	"Example 10—Aragonite-Strontianite Solid Solution\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ex11(
	"TITLE Example 11.--Transport and cation exchange.\n"
	"SOLUTION 0  CaCl2\n"
	"        units            mmol/kgw\n"
	"        temp             25.0\n"
	"        pH               7.0     charge\n"
	"        pe               12.5    O2(g)   -0.68\n"
	"        Ca               0.6\n"
	"        Cl               1.2\n"
	"SOLUTION 1-40  Initial solution for column\n"
	"        units            mmol/kgw\n"
	"        temp             25.0\n"
	"        pH               7.0     charge\n"
	"        pe               12.5    O2(g)   -0.68\n"
	"        Na               1.0\n"
	"        K                0.2\n"
	"        N(5)             1.2\n"
	"END\n"
	"EXCHANGE 1-40\n"
	"        -equilibrate 1\n"
	"        X                0.0011\n"
	"COPY cell 1 101\n"
	"END\n"
	"ADVECTION\n"
	"        -cells           40\n"
	"        -shifts          100\n"
	"        -punch_cells     40\n"
	"        -punch_frequency 1\n"
	"        -print_cells     40\n"
	"        -print_frequency 20\n"
	"PRINT; -reset false; -status false\n"
	"SELECTED_OUTPUT\n"
	"        -file            ex11adv.sel\n"
	"        -reset           false\n"
	"        -step\n"
	"        -totals          Na Cl K Ca\n"
	"USER_PUNCH\n"
	"  -heading  Pore_vol\n"
	"  10 PUNCH (STEP_NO + .5) / 40.\n"
	"USER_GRAPH 1 Example 11\n"
	"  -chart_title \"Using ADVECTION Data Block\"\n"
	"  -headings Cl Na K Ca\n"
	"  -axis_titles \"Pore volumes\" \"Millimoles per kilogram water\"\n"
	"  -axis_scale x_axis 0 2.5\n"
	"  -axis_scale y_axis 0 1.4\n"
	"  -plot_concentration_vs time\n"
	"  -start\n"
	"  10 x = (STEP_NO + 0.5) / cell_no\n"
	"  20 PLOT_XY x, TOT(\"Cl\")*1000, symbol = None\n"
	"  30 PLOT_XY x, TOT(\"Na\")*1000, symbol = None\n"
	"  40 PLOT_XY x, TOT(\"K\")*1000, symbol = None\n"
	"  50 PLOT_XY x, TOT(\"Ca\")*1000, symbol = None\n"
	"  60 PUT(1, 1)\n"
	"  -end\n"
	"COPY cell 101 1-40\n"
	"END\n"
	"USER_GRAPH 1\n"
	"        -detach\n"
	"END\n"
	"TRANSPORT\n"
	"        -cells           40\n"
	"        -lengths         0.002\n"
	"        -shifts          100\n"
	"        -time_step       720.0\n"
	"        -flow_direction  forward\n"
	"        -boundary_conditions   flux  flux\n"
	"        -diffusion_coefficient 0.0e-9\n"
	"        -dispersivities  0.002\n"
	"        -correct_disp    true\n"
	"        -punch_cells     40\n"
	"        -punch_frequency 1\n"
	"        -print_cells     40\n"
	"        -print_frequency 20\n"
	"SELECTED_OUTPUT\n"
	"        -file            ex11trn.sel\n"
	"        -reset           false\n"
	"        -step\n"
	"        -totals          Na Cl K Ca\n"
	"        -high_precision true\n"
	"USER_GRAPH 2 Example 11\n"
	"  -chart_title \"Using TRANSPORT Data Block\"\n"
	"  -headings Cl Na K Ca Cl_analytical\n"
	"  -axis_titles \"Pore volumes\" \"Millimoles per kilogram water\"\n"
	"  -axis_scale x_axis 0 2.5\n"
	"  -axis_scale y_axis 0 1.4\n"
	"#  -batch \\temp\\11.gif false # After saving, the chart on the monitor is closed.\n"
	"  -plot_concentration_vs time\n"
	"  10 x = (STEP_NO + 0.5) / cell_no\n"
	"  20 PLOT_XY x, TOT(\"Cl\")*1000, symbol = Plus, symbol_size = 2\n"
	"  30 PLOT_XY x, TOT(\"Na\")*1000, symbol = Plus, symbol_size = 2\n"
	"  40 PLOT_XY x, TOT(\"K\") *1000, symbol = Plus, symbol_size = 2\n"
	"  50 PLOT_XY x, TOT(\"Ca\")*1000, symbol = Plus, symbol_size = 2\n"
	"# calculate Cl_analytical...\n"
	"  60 DATA 0.254829592, -0.284496736, 1.421413741, -1.453152027, 1.061405429, 0.3275911\n"
	"  70 READ a1, a2, a3, a4, a5, a6\n"
	"  80 Peclet = 0.08 / 0.002\n"
	"  90  z = (1 - x) / SQRT(4 * x / Peclet)\n"
	"  100 PA = 0\n"
	"  110 GOSUB 2000 # calculate e_erfc = exp(PA) * erfc(z)\n"
	"  120 e_erfc1 = e_erfc\n"
	"  130 z = (1 + x) / SQRT(4 * x / Peclet)\n"
	"  140 PA = Peclet\n"
	"  150 GOSUB 2000 # calculate exp(PA) * erfc(z)\n"
	"  160 y = 0.6 * (e_erfc1 + e_erfc)\n"
	"  170 PLOT_XY x, y, line_width = 0, symbol = Circle, color = Red\n"
	"  180 d = (y - TOT(\"Cl\")*1000)^2\n"
	"  190 IF EXISTS(10) THEN PUT(d + GET(10), 10) ELSE PUT(d, 10)\n"
	"  200 IF STEP_NO = 2 * CELL_NO THEN print 'SSQD for Cl after 2 Pore Volumes: ', GET(10), '(mmol/L)^2'\n"
	"  210 END\n"
	"  2000 REM calculate e_erfc = exp(PA) * erfc(z)...\n"
	"  2010 sgz = SGN(z)\n"
	"  2020 z = ABS(z)\n"
	"  2050 b = 1 / (1 + a6 * z)\n"
	"  2060 e_erfc = b * (a1 + b * (a2 + b * (a3 + b * (a4 + b * a5)))) * EXP(PA - (z * z))\n"
	"  2070 IF sgz = -1 THEN e_erfc = 2 * EXP(PA) - e_erfc\n"
	"  2080 RETURN\n"
	"END\n"
	,
	"Example 11—Transport and Cation Exchange\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ex12(
	"TITLE Example 12.--Advective and diffusive transport of heat and solutes.\n"
	"      Two different boundary conditions at column ends.\n"
	"      After diffusion temperature should equal Na-conc in mmol/l.\n"
	"SOLUTION 0   24.0 mM KNO3\n"
	"    units mol/kgw\n"
	"    temp  0     # Incoming solution 0C\n"
	"    pH    7.0\n"
	"    pe   12.0   O2(g) -0.67\n"
	"    K    24.e-3\n"
	"    N(5) 24.e-3\n"
	"SOLUTION 1-60   0.001 mM KCl\n"
	"    units mol/kgw\n"
	"    temp 25    # Column is at 25C\n"
	"    pH   7.0\n"
	"    pe  12.0   O2(g) -0.67\n"
	"    K    1e-6\n"
	"    Cl   1e-6\n"
	"EXCHANGE_SPECIES\n"
	"    Na+ + X- = NaX\n"
	"    log_k       0.0\n"
	"    -gamma      4.0     0.075\n"
	"\n"
	"    H+ + X- = HX\n"
	"    log_k       -99.\n"
	"    -gamma      9.0     0.0\n"
	"\n"
	"    K+ + X- = KX\n"
	"    log_k       0.0\n"
	"    gamma       3.5     0.015\n"
	"EXCHANGE 1-60\n"
	"    KX    0.048\n"
	"PRINT\n"
	"   -reset   false\n"
	"   -selected_output false\n"
	"   -status false\n"
	"SELECTED_OUTPUT\n"
	"   -file    ex12.sel\n"
	"   -reset   false\n"
	"   -dist    true\n"
	"   -high_precision  true\n"
	"   -temp    true\n"
	"USER_PUNCH\n"
	"        -head Na_mmol K_mmol Cl_mmol\n"
	"  10 PUNCH TOT(\"Na\")*1000, TOT(\"K\")*1000, TOT(\"Cl\")*1000\n"
	"TRANSPORT           # Make column temperature 0C, displace Cl\n"
	"   -cells   60\n"
	"   -shifts  60\n"
	"   -flow_direction  forward\n"
	"   -boundary_conditions flux  flux\n"
	"   -lengths 0.333333\n"
	"   -dispersivities        0.0     # No dispersion\n"
	"   -diffusion_coefficient 0.0     # No diffusion\n"
	"   -thermal_diffusion     1.0     # No retardation for heat\n"
	"END\n"
	"SOLUTION 0   Fixed temp 24C, and NaCl conc (first type boundary cond) at inlet\n"
	"    units  mol/kgw\n"
	"    temp 24\n"
	"    pH  7.0\n"
	"    pe  12.0   O2(g) -0.67\n"
	"    Na  24.e-3\n"
	"    Cl  24.e-3\n"
	"SOLUTION 58-60  Same as soln 0 in cell 20 at closed column end (second type boundary cond)\n"
	"    units  mol/kgw\n"
	"    temp 24\n"
	"    pH  7.0\n"
	"    pe  12.0   O2(g) -0.67\n"
	"    Na  24.e-3\n"
	"    Cl  24.e-3\n"
	"EXCHANGE 58-60\n"
	"    NaX  0.048\n"
	"PRINT\n"
	"   -selected_output true\n"
	"TRANSPORT            # Diffuse 24C, NaCl solution from column end\n"
	"   -shifts 1\n"
	"   -flow_direction         diffusion\n"
	"   -boundary_conditions    constant  closed\n"
	"   -thermal_diffusion      3.0       # heat is retarded equal to Na\n"
	"   -diffusion_coefficient  0.3e-9    # m^2/s\n"
	"   -time_step              1.0e+10   # 317 years give 19 mixes\n"
	"USER_GRAPH 1 Example 12\n"
	"   -headings Na Cl Temp Analytical\n"
	"   -chart_title \"Diffusion of Solutes and Heat\"\n"
	"   -axis_titles \"Distance, in meters\" \"Millimoles per kilogram water\", \"Degrees celsius\"\n"
	"   -axis_scale x_axis 0 20\n"
	"   -axis_scale y_axis 0 25\n"
	"   -axis_scale sy_axis 0 25\n"
	"   -initial_solutions false\n"
	"   -plot_concentration_vs x\n"
	"   -start\n"
	"  10 x = DIST\n"
	"  20 PLOT_XY x, TOT(\"Na\")*1000, symbol = Plus\n"
	"  30 PLOT_XY x, TOT(\"Cl\")*1000, symbol = Plus\n"
	"  40 PLOT_XY x, TC, symbol = XCross, color = Magenta, symbol_size = 8, y-axis 2\n"
	"  50 if (x > 10 OR SIM_TIME <= 0) THEN END\n"
	"  60 DATA 0.254829592, -0.284496736, 1.421413741, -1.453152027, 1.061405429, 0.3275911\n"
	"  70 READ a1, a2, a3, a4, a5, a6\n"
	"# Calculate and plot Cl analytical...\n"
	"  80 z = x / (2 * SQRT(3e-10 * SIM_TIME / 1.0))\n"
	"  90 GOSUB 2000\n"
	"  100 PLOT_XY x, 24 * erfc, color = Blue, symbol = Circle, symbol_size = 10,\\\n"
	"                            line_width = 0\n"
	"# Calculate and plot 3 times retarded Na and temperature analytical...\n"
	"  110 z = z * SQRT(3.0)\n"
	"  120 GOSUB 2000\n"
	"  130 PLOT_XY x, 24 * erfc, color = Blue, symbol = Circle, symbol_size = 10,\\\n"
	"                            line_width = 0\n"
	"  140 END\n"
	"  2000 REM calculate erfc...\n"
	"  2050 b = 1 / (1 + a6 * z)\n"
	"  2060 erfc = b * (a1 + b * (a2 + b * (a3 + b * (a4 + b * a5)))) * EXP(-(z * z))\n"
	"  2080 RETURN\n"
	"   -end\n"
	"END\n"
	,
	"Example 12—Advective and Diffusive Flux of Heat and Solutes\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ex13a(
	"TITLE Example 13A.--1 mmol/L NaCl/NO3 enters column with stagnant zones.\n"
	"                    Implicit definition of first-order exchange model.\n"
	"SOLUTION 0    # 1 mmol/L NaCl\n"
	"   units   mmol/l\n"
	"   pH       7.0\n"
	"   pe      13.0    O2(g)   -0.7\n"
	"   Na       1.0    # Na has Retardation = 2\n"
	"   Cl       1.0    # Cl has Retardation = 1, stagnant exchange\n"
	"   N(5)     1.0    # NO3 is conservative\n"
	"#       charge imbalance is no problem ...\n"
	"END\n"
	"SOLUTION 1-41  # Column with KNO3\n"
	"   units   mmol/l\n"
	"   pH       7.0\n"
	"   pe      13.0   O2(g)    -0.7\n"
	"   K        1.0\n"
	"   N(5)     1.0\n"
	"EXCHANGE_SPECIES # For linear exchange, make KX exch. coeff. equal to NaX\n"
	"   K+ + X- = KX\n"
	"   log_k   0.0\n"
	"   -gamma  3.5     0.015\n"
	"EXCHANGE 1-41\n"
	"   -equil  1\n"
	"   X       1.e-3\n"
	"END\n"
	"PRINT\n"
	"   -reset false\n"
	"   -echo_input true\n"
	"   -status false\n"
	"TRANSPORT\n"
	"   -cells  20\n"
	"   -shifts 5\n"
	"   -flow_direction  forward\n"
	"   -time_step       3600\n"
	"   -boundary_conditions   flux  flux\n"
	"   -diffusion_coefficient 0.0\n"
	"   -lengths         0.1\n"
	"   -dispersivities  0.015\n"
	"   -stagnant   1  6.8e-6  0.3        0.1\n"
	"#   1 stagnant layer^, ^alpha, ^epsil(m), ^epsil(im)\n"
	"END\n"
	"SOLUTION 0  # Original solution with KNO3 reenters\n"
	"   units   mmol/l\n"
	"   pH       7.0\n"
	"   pe      13.0   O2(g)    -0.7\n"
	"   K        1.0\n"
	"   N(5)     1.0\n"
	"END\n"
	"SELECTED_OUTPUT\n"
	"   -file   ex13a.sel\n"
	"   -reset  false\n"
	"   -solution\n"
	"   -distance       true\n"
	"USER_PUNCH\n"
	"   -headings Cl_mmol Na_mmol\n"
	"  10 PUNCH TOT(\"Cl\")*1000, TOT(\"Na\")*1000\n"
	"TRANSPORT\n"
	"   -shifts 10\n"
	"   -punch_cells        1-20\n"
	"   -punch_frequency    10\n"
	"USER_GRAPH 1 Example 13A\n"
	"   -headings Distance Na Cl\n"
	"   -chart_title \"Dual Porosity, First-Order Exchange with Implicit Mixing Factors\"\n"
	"   -axis_titles \"Distance, in meters\" \"Millimoles per kilogram water\"\n"
	"   -axis_scale x_axis 0 2\n"
	"   -axis_scale y_axis 0 0.8\n"
	"   -plot_concentration_vs x\n"
	"   -start\n"
	"  10 GRAPH_X DIST\n"
	"  20 GRAPH_Y TOT(\"Na\")*1000 TOT(\"Cl\")*1000\n"
	"  -end\n"
	"END\n"
	,
	"Example 13—1D Transport in a Dual Porosity Column with Cation Exchange\n"
	"Stagnant zone with implicitly defined mixing factors\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ex13b(
	"TITLE Example 13B.--1 mmol/l NaCl/NO3 enters column with stagnant zones.\n"
	"                    Explicit definition of first-order exchange factors.\n"
	"SOLUTION 0    # 1 mmol/l NaCl\n"
	"   units   mmol/l\n"
	"   pH       7.0\n"
	"   pe      13.0    O2(g)   -0.7\n"
	"   Na       1.0    # Na has Retardation = 2\n"
	"   Cl       1.0    # Cl has Retardation = 1, stagnant exchange\n"
	"   N(5)     1.0    # NO3 is conservative\n"
	"#       charge imbalance is no problem ...\n"
	"END\n"
	"SOLUTION 1-41  # Column with KNO3\n"
	"   units   mmol/l\n"
	"   pH       7.0\n"
	"   pe      13.0   O2(g)    -0.7\n"
	"   K        1.0\n"
	"   N(5)     1.0\n"
	"EXCHANGE_SPECIES # For linear exchange, make KX exch. coeff. equal to NaX\n"
	"   K+ + X- = KX\n"
	"   log_k   0.0\n"
	"   -gamma  3.5     0.015\n"
	"EXCHANGE 1-41\n"
	"   -equil  1\n"
	"   X       1.e-3\n"
	"END\n"
	"PRINT\n"
	"        -reset false\n"
	"        -echo_input true\n"
	"        -status false\n"
	"MIX  1;  1 .93038;      22 .06962       ;MIX  2;         2 .93038;      23 .06962;\n"
	"MIX  3;  3 .93038;      24 .06962       ;MIX  4;         4 .93038;      25 .06962;\n"
	"MIX  5;  5 .93038;      26 .06962       ;MIX  6;         6 .93038;      27 .06962;\n"
	"MIX  7;  7 .93038;      28 .06962       ;MIX  8;         8 .93038;      29 .06962;\n"
	"MIX  9;  9 .93038;      30 .06962       ;MIX 10;        10 .93038;      31 .06962;\n"
	"MIX 11; 11 .93038;      32 .06962       ;MIX 12;        12 .93038;      33 .06962;\n"
	"MIX 13; 13 .93038;      34 .06962       ;MIX 14;        14 .93038;      35 .06962;\n"
	"MIX 15; 15 .93038;      36 .06962       ;MIX 16;        16 .93038;      37 .06962;\n"
	"MIX 17; 17 .93038;      38 .06962       ;MIX 18;        18 .93038;      39 .06962;\n"
	"MIX 19; 19 .93038;      40 .06962       ;MIX 20;        20 .93038;      41 .06962;\n"
	"#\n"
	"MIX 22;  1 .20886;      22 .79114       ;MIX 23;         2 .20886;      23 .79114;\n"
	"MIX 24;  3 .20886;      24 .79114       ;MIX 25;         4 .20886;      25 .79114;\n"
	"MIX 26;  5 .20886;      26 .79114       ;MIX 27;         6 .20886;      27 .79114;\n"
	"MIX 28;  7 .20886;      28 .79114       ;MIX 29;         8 .20886;      29 .79114;\n"
	"MIX 30;  9 .20886;      30 .79114       ;MIX 31;        10 .20886;      31 .79114;\n"
	"MIX 32; 11 .20886;      32 .79114       ;MIX 33;        12 .20886;      33 .79114;\n"
	"MIX 34; 13 .20886;      34 .79114       ;MIX 35;        14 .20886;      35 .79114;\n"
	"MIX 36; 15 .20886;      36 .79114       ;MIX 37;        16 .20886;      37 .79114;\n"
	"MIX 38; 17 .20886;      38 .79114       ;MIX 39;        18 .20886;      39 .79114;\n"
	"MIX 40; 19 .20886;      40 .79114       ;MIX 41;        20 .20886;      41 .79114;\n"
	"TRANSPORT\n"
	"   -cells  20\n"
	"   -shifts 5\n"
	"   -flow_direction  forward\n"
	"   -time_step       3600\n"
	"   -boundary_conditions   flux  flux\n"
	"   -diffusion_coefficient 0.0\n"
	"   -lengths         0.1\n"
	"   -dispersivities  0.015\n"
	"   -stagnant        1\n"
	"END\n"
	"SOLUTION 0  # Original solution reenters\n"
	"   units   mmol/l\n"
	"   pH       7.0\n"
	"   pe      13.0   O2(g)    -0.7\n"
	"   K        1.0\n"
	"   N(5)     1.0\n"
	"END\n"
	"SELECTED_OUTPUT\n"
	"   -file   ex13b.sel\n"
	"   -reset  false\n"
	"   -distance       true\n"
	"   -solution\n"
	"USER_PUNCH\n"
	"   -headings Cl_mmol Na_mmol\n"
	"  10 PUNCH TOT(\"Cl\")*1000, TOT(\"Na\")*1000\n"
	"TRANSPORT\n"
	"   -shifts  10\n"
	"   -punch_cells        1-20\n"
	"   -punch_frequency    10\n"
	"USER_GRAPH 1 Example 13B\n"
	"   -headings Distance Na Cl\n"
	"   -chart_title \"Dual Porosity, First-Order Exchange with Explicit Mixing Factors\"\n"
	"   -axis_titles \"Distance, in meters\" \"Millimoles per kilogram water\"\n"
	"   -axis_scale x_axis 0 2\n"
	"   -axis_scale y_axis 0 0.8\n"
	"   -plot_concentration_vs x\n"
	"   -start\n"
	"  10 GRAPH_X DIST\n"
	"  20 GRAPH_Y TOT(\"Na\")*1000 TOT(\"Cl\")*1000\n"
	"   -end\n"
	"END\n"
	,
	"Example 13—1D Transport in a Dual Porosity Column with Cation Exchange\n"
	"Stagnant zone with explicitly defined mixing factors\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ex13c(
	"TITLE Example 13C.--1 mmol/l NaCl/NO3 enters column with stagnant zones.\n"
	"                    5 layer stagnant zone with finite differences.\n"
	"SOLUTION 0    # 1 mmol/l NaCl\n"
	"   units   mmol/l\n"
	"   pH       7.0\n"
	"\n"
	"   pe      13.0    O2(g)   -0.7\n"
	"   Na       1.0    # Na has Retardation = 2\n"
	"   Cl       1.0    # Cl has Retardation = 1, stagnant exchange\n"
	"   N(5)     1.0    # NO3 is conservative\n"
	"#       charge imbalance is no problem ...\n"
	"END\n"
	"SOLUTION 1-121\n"
	"   units   mmol/l\n"
	"   pH       7.0\n"
	"   pe      13.0   O2(g)    -0.7\n"
	"   K        1.0\n"
	"   N(5)     1.0\n"
	"EXCHANGE_SPECIES # For linear exchange, make KX exch. coeff. equal to NaX\n"
	"   K+ + X- = KX\n"
	"   log_k   0.0\n"
	"   -gamma  3.5     0.015\n"
	"EXCHANGE 1-121\n"
	"   -equilibrate  1\n"
	"   X             1.e-3\n"
	"END\n"
	"PRINT\n"
	"   -reset false\n"
	"   -echo_input true\n"
	"   -status false\n"
	"MIX    1;    1  0.90712;   22  0.09288\n"
	"MIX   22;    1  0.57098;   22  0.21656;   42  0.21246\n"
	"MIX   42;   22  0.35027;   42  0.45270;   62  0.19703\n"
	"MIX   62;   42  0.38368;   62  0.44579;   82  0.17053\n"
	"MIX   82;   62  0.46286;   82  0.42143;  102  0.11571\n"
	"MIX  102;   82  0.81000;  102  0.19000\n"
	"MIX    2;    2  0.90712;   23  0.09288\n"
	"MIX   23;    2  0.57098;   23  0.21656;   43  0.21246\n"
	"MIX   43;   23  0.35027;   43  0.45270;   63  0.19703\n"
	"MIX   63;   43  0.38368;   63  0.44579;   83  0.17053\n"
	"MIX   83;   63  0.46286;   83  0.42143;  103  0.11571\n"
	"MIX  103;   83  0.81000;  103  0.19000\n"
	"MIX    3;    3  0.90712;   24  0.09288\n"
	"MIX   24;    3  0.57098;   24  0.21656;   44  0.21246\n"
	"MIX   44;   24  0.35027;   44  0.45270;   64  0.19703\n"
	"MIX   64;   44  0.38368;   64  0.44579;   84  0.17053\n"
	"MIX   84;   64  0.46286;   84  0.42143;  104  0.11571\n"
	"MIX  104;   84  0.81000;  104  0.19000\n"
	"MIX    4;    4  0.90712;   25  0.09288\n"
	"MIX   25;    4  0.57098;   25  0.21656;   45  0.21246\n"
	"MIX   45;   25  0.35027;   45  0.45270;   65  0.19703\n"
	"MIX   65;   45  0.38368;   65  0.44579;   85  0.17053\n"
	"MIX   85;   65  0.46286;   85  0.42143;  105  0.11571\n"
	"MIX  105;   85  0.81000;  105  0.19000\n"
	"MIX    5;    5  0.90712;   26  0.09288\n"
	"MIX   26;    5  0.57098;   26  0.21656;   46  0.21246\n"
	"MIX   46;   26  0.35027;   46  0.45270;   66  0.19703\n"
	"MIX   66;   46  0.38368;   66  0.44579;   86  0.17053\n"
	"MIX   86;   66  0.46286;   86  0.42143;  106  0.11571\n"
	"MIX  106;   86  0.81000;  106  0.19000\n"
	"MIX    6;    6  0.90712;   27  0.09288\n"
	"MIX   27;    6  0.57098;   27  0.21656;   47  0.21246\n"
	"MIX   47;   27  0.35027;   47  0.45270;   67  0.19703\n"
	"MIX   67;   47  0.38368;   67  0.44579;   87  0.17053\n"
	"MIX   87;   67  0.46286;   87  0.42143;  107  0.11571\n"
	"MIX  107;   87  0.81000;  107  0.19000\n"
	"MIX    7;    7  0.90712;   28  0.09288\n"
	"MIX   28;    7  0.57098;   28  0.21656;   48  0.21246\n"
	"MIX   48;   28  0.35027;   48  0.45270;   68  0.19703\n"
	"MIX   68;   48  0.38368;   68  0.44579;   88  0.17053\n"
	"MIX   88;   68  0.46286;   88  0.42143;  108  0.11571\n"
	"MIX  108;   88  0.81000;  108  0.19000\n"
	"MIX    8;    8  0.90712;   29  0.09288\n"
	"MIX   29;    8  0.57098;   29  0.21656;   49  0.21246\n"
	"MIX   49;   29  0.35027;   49  0.45270;   69  0.19703\n"
	"MIX   69;   49  0.38368;   69  0.44579;   89  0.17053\n"
	"MIX   89;   69  0.46286;   89  0.42143;  109  0.11571\n"
	"MIX  109;   89  0.81000;  109  0.19000\n"
	"MIX    9;    9  0.90712;   30  0.09288\n"
	"MIX   30;    9  0.57098;   30  0.21656;   50  0.21246\n"
	"MIX   50;   30  0.35027;   50  0.45270;   70  0.19703\n"
	"MIX   70;   50  0.38368;   70  0.44579;   90  0.17053\n"
	"MIX   90;   70  0.46286;   90  0.42143;  110  0.11571\n"
	"MIX  110;   90  0.81000;  110  0.19000\n"
	"MIX   10;   10  0.90712;   31  0.09288\n"
	"MIX   31;   10  0.57098;   31  0.21656;   51  0.21246\n"
	"MIX   51;   31  0.35027;   51  0.45270;   71  0.19703\n"
	"MIX   71;   51  0.38368;   71  0.44579;   91  0.17053\n"
	"MIX   91;   71  0.46286;   91  0.42143;  111  0.11571\n"
	"MIX  111;   91  0.81000;  111  0.19000\n"
	"MIX   11;   11  0.90712;   32  0.09288\n"
	"MIX   32;   11  0.57098;   32  0.21656;   52  0.21246\n"
	"MIX   52;   32  0.35027;   52  0.45270;   72  0.19703\n"
	"MIX   72;   52  0.38368;   72  0.44579;   92  0.17053\n"
	"MIX   92;   72  0.46286;   92  0.42143;  112  0.11571\n"
	"MIX  112;   92  0.81000;  112  0.19000\n"
	"MIX   12;   12  0.90712;   33  0.09288\n"
	"MIX   33;   12  0.57098;   33  0.21656;   53  0.21246\n"
	"MIX   53;   33  0.35027;   53  0.45270;   73  0.19703\n"
	"MIX   73;   53  0.38368;   73  0.44579;   93  0.17053\n"
	"MIX   93;   73  0.46286;   93  0.42143;  113  0.11571\n"
	"MIX  113;   93  0.81000;  113  0.19000\n"
	"MIX   13;   13  0.90712;   34  0.09288\n"
	"MIX   34;   13  0.57098;   34  0.21656;   54  0.21246\n"
	"MIX   54;   34  0.35027;   54  0.45270;   74  0.19703\n"
	"MIX   74;   54  0.38368;   74  0.44579;   94  0.17053\n"
	"MIX   94;   74  0.46286;   94  0.42143;  114  0.11571\n"
	"MIX  114;   94  0.81000;  114  0.19000\n"
	"MIX   14;   14  0.90712;   35  0.09288\n"
	"MIX   35;   14  0.57098;   35  0.21656;   55  0.21246\n"
	"MIX   55;   35  0.35027;   55  0.45270;   75  0.19703\n"
	"MIX   75;   55  0.38368;   75  0.44579;   95  0.17053\n"
	"MIX   95;   75  0.46286;   95  0.42143;  115  0.11571\n"
	"MIX  115;   95  0.81000;  115  0.19000\n"
	"MIX   15;   15  0.90712;   36  0.09288\n"
	"MIX   36;   15  0.57098;   36  0.21656;   56  0.21246\n"
	"MIX   56;   36  0.35027;   56  0.45270;   76  0.19703\n"
	"MIX   76;   56  0.38368;   76  0.44579;   96  0.17053\n"
	"MIX   96;   76  0.46286;   96  0.42143;  116  0.11571\n"
	"MIX  116;   96  0.81000;  116  0.19000\n"
	"MIX   16;   16  0.90712;   37  0.09288\n"
	"MIX   37;   16  0.57098;   37  0.21656;   57  0.21246\n"
	"MIX   57;   37  0.35027;   57  0.45270;   77  0.19703\n"
	"MIX   77;   57  0.38368;   77  0.44579;   97  0.17053\n"
	"MIX   97;   77  0.46286;   97  0.42143;  117  0.11571\n"
	"MIX  117;   97  0.81000;  117  0.19000\n"
	"MIX   17;   17  0.90712;   38  0.09288\n"
	"MIX   38;   17  0.57098;   38  0.21656;   58  0.21246\n"
	"MIX   58;   38  0.35027;   58  0.45270;   78  0.19703\n"
	"MIX   78;   58  0.38368;   78  0.44579;   98  0.17053\n"
	"MIX   98;   78  0.46286;   98  0.42143;  118  0.11571\n"
	"MIX  118;   98  0.81000;  118  0.19000\n"
	"MIX   18;   18  0.90712;   39  0.09288\n"
	"MIX   39;   18  0.57098;   39  0.21656;   59  0.21246\n"
	"MIX   59;   39  0.35027;   59  0.45270;   79  0.19703\n"
	"MIX   79;   59  0.38368;   79  0.44579;   99  0.17053\n"
	"MIX   99;   79  0.46286;   99  0.42143;  119  0.11571\n"
	"MIX  119;   99  0.81000;  119  0.19000\n"
	"MIX   19;   19  0.90712;   40  0.09288\n"
	"MIX   40;   19  0.57098;   40  0.21656;   60  0.21246\n"
	"MIX   60;   40  0.35027;   60  0.45270;   80  0.19703\n"
	"MIX   80;   60  0.38368;   80  0.44579;  100  0.17053\n"
	"MIX  100;   80  0.46286;  100  0.42143;  120  0.11571\n"
	"MIX  120;  100  0.81000;  120  0.19000\n"
	"MIX   20;   20  0.90712;   41  0.09288\n"
	"MIX   41;   20  0.57098;   41  0.21656;   61  0.21246\n"
	"MIX   61;   41  0.35027;   61  0.45270;   81  0.19703\n"
	"MIX   81;   61  0.38368;   81  0.44579;  101  0.17053\n"
	"MIX  101;   81  0.46286;  101  0.42143;  121  0.11571\n"
	"MIX  121;  101  0.81000;  121  0.19000\n"
	"TRANSPORT\n"
	"   -cells  20\n"
	"   -shifts 5\n"
	"   -flow_direction  forward\n"
	"   -time_step       3600\n"
	"   -boundary_conditions   flux  flux\n"
	"   -diffusion_coefficient 0.0\n"
	"   -lengths         0.1\n"
	"   -dispersivities  0.015\n"
	"   -stagnant        5\n"
	"END\n"
	"SOLUTION 0  # Original solution reenters\n"
	"   units   mmol/l\n"
	"   pH       7.0\n"
	"   pe      13.0   O2(g)    -0.7\n"
	"   K        1.0\n"
	"   N(5)     1.0\n"
	"END\n"
	"SELECTED_OUTPUT\n"
	"   -file   ex13c.sel\n"
	"   -reset  false\n"
	"   -distance       true\n"
	"   -solution\n"
	"USER_PUNCH\n"
	"   -headings Cl_mmol Na_mmol\n"
	"  10 PUNCH TOT(\"Cl\")*1000, TOT(\"Na\")*1000\n"
	"TRANSPORT\n"
	"   -shifts  10\n"
	"   -punch_cells        1-20\n"
	"   -punch_frequency    10\n"
	"USER_GRAPH 1 Example 13C\n"
	"        -headings Distance Na Cl\n"
	"   -chart_title \"Dual Porosity, Finite-Difference Approximation\"\n"
	"   -axis_titles \"Distance, in meters\" \"Millimoles per kilogram water\"\n"
	"   -axis_scale x_axis 0 2\n"
	"   -axis_scale y_axis 0 0.8\n"
	"   -plot_concentration_vs x\n"
	"   -start\n"
	"  10 GRAPH_X DIST\n"
	"  20 GRAPH_Y TOT(\"Na\")*1000 TOT(\"Cl\")*1000\n"
	"    -end\n"
	"END\n"
	,
	"Example 13—1D Transport in a Dual Porosity Column with Cation Exchange\n"
	"Stagnant zone with diffusion calculated by finite differences\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ex14(
	"TITLE Example 14.--Transport with equilibrium_phases, exchange, and surface reactions\n"
	"#\n"
	"# Use phreeqc.dat\n"
	"# Dzombak and Morel (1990) aqueous and surface complexation models for arsenic\n"
	"# are defined here\n"
	"#\n"
	"SURFACE_MASTER_SPECIES\n"
	"        Surf    SurfOH\n"
	"SURFACE_SPECIES\n"
	"        SurfOH = SurfOH\n"
	"                log_k   0.0\n"
	"        SurfOH  + H+ = SurfOH2+\n"
	"                log_k   7.29\n"
	"        SurfOH = SurfO- + H+\n"
	"                log_k   -8.93\n"
	"        SurfOH + AsO4-3 + 3H+ = SurfH2AsO4 + H2O\n"
	"                log_k   29.31\n"
	"        SurfOH + AsO4-3 + 2H+ = SurfHAsO4- + H2O\n"
	"                log_k   23.51\n"
	"        SurfOH + AsO4-3 = SurfOHAsO4-3\n"
	"                log_k   10.58\n"
	"SOLUTION_MASTER_SPECIES\n"
	"        As       H3AsO4        -1.0     74.9216    74.9216\n"
	"SOLUTION_SPECIES\n"
	"        H3AsO4 = H3AsO4\n"
	"                log_k           0.0\n"
	"        H3AsO4 = AsO4-3 + 3H+\n"
	"                log_k   -20.7\n"
	"        H+ + AsO4-3 = HAsO4-2\n"
	"                log_k   11.50\n"
	"        2H+ + AsO4-3 = H2AsO4-\n"
	"                log_k           18.46\n"
	"SOLUTION 1 Brine\n"
	"        pH      5.713\n"
	"        pe      4.0     O2(g)   -0.7\n"
	"        temp    25.\n"
	"        units   mol/kgw\n"
	"        Ca      .4655\n"
	"        Mg      .1609\n"
	"        Na      5.402\n"
	"        Cl      6.642           charge\n"
	"        C       .00396\n"
	"        S       .004725\n"
	"        As      .025 umol/kgw\n"
	"END\n"
	"USE solution 1\n"
	"EQUILIBRIUM_PHASES 1\n"
	"        Dolomite        0.0     1.6\n"
	"        Calcite         0.0     0.1\n"
	"SAVE solution 1\n"
	"# prints initial condition to the selected-output file\n"
	"SELECTED_OUTPUT\n"
	"        -file ex14.sel\n"
	"        -reset false\n"
	"        -step\n"
	"USER_PUNCH\n"
	"        -head  m_Ca m_Mg m_Na umol_As pH mmol_sorbedAs\n"
	"  10 PUNCH TOT(\"Ca\"), TOT(\"Mg\"), TOT(\"Na\"), TOT(\"As\")*1e6, -LA(\"H+\"), SURF(\"As\", \"Surf\")*1000\n"
	"END\n"
	"PRINT\n"
	"# skips print of initial exchange and initial surface to the selected-output file\n"
	"        -selected_out false\n"
	"EXCHANGE 1\n"
	"        -equil with solution 1\n"
	"        X       1.0\n"
	"SURFACE 1\n"
	"        -equil solution 1\n"
	"# assumes 1/10 of iron is HFO\n"
	"        SurfOH           0.07    600.    30.\n"
	"END\n"
	"SOLUTION 0 20 x precipitation\n"
	"        pH      4.6\n"
	"        pe      4.0     O2(g)   -0.7\n"
	"        temp    25.\n"
	"        units   mmol/kgw\n"
	"        Ca      .191625\n"
	"        Mg      .035797\n"
	"        Na      .122668\n"
	"        Cl      .133704\n"
	"        C       .01096\n"
	"        S       .235153         charge\n"
	"EQUILIBRIUM_PHASES 0\n"
	"        Dolomite        0.0     1.6\n"
	"        Calcite         0.0     0.1\n"
	"        CO2(g)          -1.5    10.\n"
	"SAVE solution 0\n"
	"END\n"
	"PRINT\n"
	"        -selected_out true\n"
	"        -status false\n"
	"ADVECTION\n"
	"        -cells 1\n"
	"        -shifts 200\n"
	"        -print_frequency 200\n"
	"USER_GRAPH 1 Example 14\n"
	"        -headings PV As(ppb) Ca(M) Mg(M) Na(M) pH\n"
	"        -chart_title \"Chemical Evolution of the Central Oklahoma Aquifer\"\n"
	"        -axis_titles \"Pore volumes or shift number\" \"Log(Concentration, in ppb or molal)\" \"pH\"\n"
	"        -axis_scale x_axis 0 200\n"
	"        -axis_scale y_axis 1e-6 100 auto auto Log\n"
	"  10 GRAPH_X STEP_NO\n"
	"  20 GRAPH_Y TOT(\"As\") * 74.92e6, TOT(\"Ca\"), TOT(\"Mg\"), TOT(\"Na\")\n"
	"  30 GRAPH_SY -LA(\"H+\")\n"
	"END\n"
	,
	"Example 14—Advective Transport, Cation Exchange, Surface Complexation, and Mineral Equilibria\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ex15(
	"TITLE Example 15.--1D Transport: Kinetic Biodegradation, Cell Growth, and Sorption\n"
	"***********\n"
	"PLEASE NOTE: This problem requires database file ex15.dat!!\n"
	"***********\n"
	"PRINT\n"
	"        -reset false\n"
	"        -echo_input true\n"
	"        -status false\n"
	"SOLUTION 0 Pulse solution with NTA and cobalt\n"
	"        units umol/L\n"
	"        pH      6\n"
	"        C       .49\n"
	"        O(0)    62.5 \n"
	"        Nta     5.23\n"
	"        Co      5.23\n"
	"        Na      1000\n"
	"        Cl      1000\n"
	"SOLUTION 1-10 Background solution initially filling column\n"
	"        units umol/L\n"
	"        pH      6\n"
	"        C       .49\n"
	"        O(0)    62.5 \n"
	"        Na      1000\n"
	"        Cl      1000\n"
	"COPY solution 0 100 # for use later on, and in\n"
	"COPY solution 1 101 # 20 cells model\n"
	"END\n"
	"RATES Rate expressions for the four kinetic reactions\n"
	"#\n"
	"        HNTA-2\n"
	"        -start\n"
	"10 Ks = 7.64e-7\n"
	"20 Ka = 6.25e-6\n"
	"30 qm = 1.407e-3/3600\n"
	"40 f1 = MOL(\"HNta-2\")/(Ks + MOL(\"HNta-2\"))\n"
	"50 f2 = MOL(\"O2\")/(Ka + MOL(\"O2\"))\n"
	"60 rate = -qm * KIN(\"Biomass\") * f1 * f2 \n"
	"70 moles = rate * TIME\n"
	"80 PUT(rate, 1)   # save the rate for use in Biomass rate calculation\n"
	"90 SAVE moles\n"
	"        -end\n"
	"#\n"
	"        Biomass\n"
	"        -start\n"
	"10 Y = 65.14\n"
	"20 b = 0.00208/3600\n"
	"30 rate = GET(1)  # uses rate calculated in HTNA-2 rate calculation\n"
	"40 rate = -Y*rate -b*M\n"
	"50 moles = -rate * TIME\n"
	"60 if (M + moles) < 0 then moles = -M\n"
	"70 SAVE moles\n"
	"        -end\n"
	"#\n"
	"        Co_sorption\n"
	"        -start\n"
	"10 km = 1/3600\n"
	"20 kd = 5.07e-3\n"
	"30 solids = 3.75e3\n"
	"40 rate = -km*(MOL(\"Co+2\") - (M/solids)/kd)\n"
	"50 moles = rate * TIME\n"
	"60 if (M - moles) < 0 then moles = M\n"
	"70 SAVE moles\n"
	"        -end\n"
	"#\n"
	"        CoNta_sorption\n"
	"        -start\n"
	"10 km = 1/3600\n"
	"20 kd = 5.33e-4\n"
	"30 solids = 3.75e3\n"
	"40 rate = -km*(MOL(\"CoNta-\") - (M/solids)/kd)\n"
	"50 moles = rate * TIME\n"
	"60 if (M - moles) < 0 then moles = M\n"
	"70 SAVE moles\n"
	"        -end\n"
	"KINETICS 1-10 Four kinetic reactions for all cells\n"
	"        HNTA-2\n"
	"                -formula C -3.12 H -1.968 O -4.848 N -0.424 Nta 1.\n"
	"        Biomass\n"
	"                -formula        H 0.0\n"
	"                -m              1.36e-4\n"
	"        Co_sorption\n"
	"                -formula CoCl2\n"
	"                -m      0.0\n"
	"                -tol 1e-11\n"
	"        CoNta_sorption\n"
	"                -formula NaCoNta\n"
	"                -m      0.0\n"
	"                -tol 1e-11\n"
	"COPY kinetics 1 101 # to use with 20 cells                \n"
	"END\n"
	"SELECTED_OUTPUT\n"
	"        -file   ex15.sel\n"
	"        -mol    Nta-3 CoNta- HNta-2 Co+2\n"
	"USER_PUNCH\n"
	"        -headings        hours   Co_sorb CoNta_sorb      Biomass\n"
	"        -start\n"
	"  10 punch TOTAL_TIME/3600 + 3600/2/3600\n"
	"  20 punch KIN(\"Co_sorption\")/3.75e3\n"
	"  30 punch KIN(\"CoNta_sorption\")/3.75e3\n"
	"  40 punch KIN(\"Biomass\")\n"
	"USER_GRAPH 1 Example 15\n"
	"        -headings 10_cells: Co+2 CoNTA- HNTA-2 pH\n"
	"        -chart_title \"Kinetic Biodegradation, Cell Growth, and Sorption: Dissolved Species\"\n"
	"        -axis_titles \"Time, in hours\" \"Micromoles per kilogram water\" \"pH\"\n"
	"        -axis_scale x_axis 0 75\n"
	"        -axis_scale y_axis 0 4\n"
	"        -axis_scale secondary_y_axis 5.799 6.8 0.2 0.1\n"
	"        -plot_concentration_vs t\n"
	"        -start\n"
	"  10 x = TOTAL_TIME/3600 + 3600/2/3600\n"
	"  20 PLOT_XY -1, -1, line_width = 0, symbol_size = 0\n"
	"  30 PLOT_XY x, MOL(\"Co+2\") * 1e6, color = Red, line_width = 0, symbol_size = 4\n"
	"  40 PLOT_XY x, MOL(\"CoNta-\") * 1e6, color = Green, line_width = 0, symbol_size = 4\n"
	"  50 PLOT_XY x, MOL(\"HNta-2\") * 1e6, color = Blue, line_width = 0, symbol_size = 4\n"
	"  60 PLOT_XY x, -LA(\"H+\"), y-axis = 2, color = Magenta, line_width = 0, symbol_size = 4\n"
	"        -end\n"
	"USER_GRAPH 2 Example 15\n"
	"        -headings 10_cells: Co+2 CoNTA- Biomass\n"
	"        -chart_title \"Kinetic Biodegradation, Cell Growth, and Sorption: Sorbed Species\"\n"
	"        -axis_titles \"Time, in hours\"  \"Nanomoles per kilogram water\" \\\n"
	"             \"Biomass, in milligrams per liter\"\n"
	"        -axis_scale x_axis 0 75\n"
	"        -axis_scale y_axis 0 2\n"
	"        -axis_scale secondary_y_axis 0 0.4\n"
	"        -plot_concentration_vs t\n"
	"        -start\n"
	"  10 x = TOTAL_TIME/3600 + 3600/2/3600\n"
	"  20 PLOT_XY -1, -1, line_width = 0, symbol_size = 0\n"
	"  30 PLOT_XY x, KIN(\"Co_sorption\") / 3.75e3 * 1e9, color = Red, line_width = 0, symbol_size = 4\n"
	"  40 PLOT_XY x, KIN(\"CoNta_sorption\") / 3.75e3 * 1e9, color = Green, line_width = 0, \\\n"
	"       symbol_size = 4\n"
	"  50 PLOT_XY x, KIN(\"Biomass\") * 1e3, y-axis = 2, color = Magenta, line_width = 0, \\\n"
	"       symbol_size = 4\n"
	"        -end          -end\n"
	"TRANSPORT First 20 hours have NTA and cobalt in infilling solution\n"
	"        -cells                10\n"
	"        -lengths              1\n"
	"        -shifts               20\n"
	"        -time_step            3600\n"
	"        -flow_direction       forward\n"
	"        -boundary_conditions  flux flux\n"
	"        -dispersivities       .05\n"
	"        -correct_disp         true\n"
	"        -diffusion_coefficient 0.0\n"
	"        -punch_cells          10\n"
	"        -punch_frequency      1\n"
	"        -print_cells          10\n"
	"        -print_frequency      5\n"
	"     \n"
	"COPY solution 101 0 # initial column solution becomes influent\n"
	"END\n"
	"TRANSPORT Last 55 hours with background infilling solution\n"
	"        -shifts               55\n"
	"COPY cell 100 0 # for the 20 cell model...\n"
	"COPY cell 101 1-20\n"
	"END\n"
	"USER_PUNCH\n"
	"        -start\n"
	"  10 punch TOTAL_TIME/3600 + 3600/4/3600\n"
	"  20 punch KIN(\"Co_sorption\")/3.75e3\n"
	"  30 punch KIN(\"CoNta_sorption\")/3.75e3\n"
	"  40 punch KIN(\"Biomass\")\n"
	"        -end\n"
	"USER_GRAPH 1\n"
	"        -headings 20_cells: Co+2 CoNTA- HNTA-2 pH\n"
	"        -start\n"
	"  10 x = TOTAL_TIME/3600 + 3600/4/3600\n"
	"  20 PLOT_XY -1, -1, line_width = 0, symbol_size = 0\n"
	"  30 PLOT_XY x, MOL(\"Co+2\") * 1e6, color = Red, symbol_size = 0\n"
	"  40 PLOT_XY x, MOL(\"CoNta-\") * 1e6, color = Green, symbol_size = 0\n"
	"  50 PLOT_XY x, MOL(\"HNta-2\") * 1e6, color = Blue, symbol_size = 0\n"
	"  60 PLOT_XY x, -LA(\"H+\"), y-axis = 2, color = Magenta, symbol_size = 0\n"
	"        -end\n"
	"USER_GRAPH 2\n"
	"        -headings 20_cells: Co+2 CoNTA- Biomass\n"
	"        -start\n"
	"  10 x = TOTAL_TIME/3600 + 3600/4/3600\n"
	"  20 PLOT_XY -1, -1, line_width = 0, symbol_size = 0\n"
	"  30 PLOT_XY x, KIN(\"Co_sorption\") / 3.75e3 * 1e9, color = Red, symbol_size = 0\n"
	"  40 PLOT_XY x, KIN(\"CoNta_sorption\") / 3.75e3 * 1e9, color = Green, symbol_size = 0\n"
	"  60 PLOT_XY x, KIN(\"Biomass\") * 1e3, y-axis = 2, color = Magenta, symbol_size = 0\n"
	"        -end        \n"
	"TRANSPORT First 20 hours have NTA and cobalt in infilling solution\n"
	"        -cells                20\n"
	"        -lengths              0.5\n"
	"        -shifts               40\n"
	"        -initial_time         0\n"
	"        -time_step            1800\n"
	"        -flow_direction       forward\n"
	"        -boundary_conditions  flux  flux\n"
	"        -dispersivities       .05\n"
	"        -correct_disp         true\n"
	"        -diffusion_coefficient 0.0\n"
	"        -punch_cells          20\n"
	"        -punch_frequency      2\n"
	"        -print_cells          20\n"
	"        -print_frequency      10\n"
	"COPY cell 101 0\n"
	"END\n"
	"TRANSPORT Last 55 hours with background infilling solution\n"
	"        -shifts               110\n"
	"END\n"
	,
	"Example 15—1D Transport: Kinetic Biodegradation, Cell Growth, and Sorption\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ex16(
	"TITLE Example 16.--Inverse modeling of Sierra springs\n"
	"SOLUTION_SPREAD\n"
	"        -units mmol/L\n"
	"Number	pH	Si	Ca	Mg	Na	K	Alkalinity	S(6)	Cl\n"
	"1	6.2	0.273	0.078	0.029	0.134	0.028	0.328      	0.01	0.014\n"
	"2	6.8	0.41	0.26	0.071	0.259	0.04	0.895    	0.025	0.03\n"
	"\n"
	"INVERSE_MODELING 1\n"
	"        -solutions 1 2\n"
	"        -uncertainty 0.025\n"
	"        -range\n"
	"        -phases\n"
	"                Halite\n"
	"                Gypsum\n"
	"                Kaolinite               precip\n"
	"                Ca-montmorillonite      precip\n"
	"                CO2(g)\n"
	"                Calcite\n"
	"                Chalcedony              precip\n"
	"                Biotite                 dissolve\n"
	"                Plagioclase             dissolve\n"
	"        -balances\n"
	"                Ca      0.05     0.025\n"
	"PHASES\n"
	"Biotite\n"
	"        KMg3AlSi3O10(OH)2 + 6H+ + 4H2O = K+ + 3Mg+2 + Al(OH)4- + 3H4SiO4 \n"
	"        log_k  0.0      # No log_k, Inverse modeling only\n"
	"Plagioclase\n"
	"        Na0.62Ca0.38Al1.38Si2.62O8 + 5.52 H+ + 2.48H2O = \\\n"
	"                0.62Na+ + 0.38Ca+2 + 1.38Al+3 + 2.62H4SiO4\n"
	"        log_k  0.0      # No log_k, inverse modeling only\n"
	"END\n"
	,
	"Example 16—Inverse Modeling of Sierra Spring Waters\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ex17(
	"#must use DATABASE pitzer.dat\n"
	"TITLE Example 17.--Inverse modeling of Black Sea water evaporation\n"
	"SOLUTION 1  Black Sea water\n"
	"        units   mg/L\n"
	"        density 1.014\n"
	"        pH      8.0     # estimated\n"
	"        Ca      233\n"
	"        Mg      679\n"
	"        Na      5820\n"
	"        K       193\n"
	"        S(6)    1460\n"
	"        Cl      10340\n"
	"        Br      35\n"
	"        C       1       CO2(g) -3.5\n"
	"SOLUTION 2  Composition during halite precipitation\n"
	"        units   mg/L\n"
	"        density 1.271\n"
	"        pH      5.0     # estimated\n"
	"        Ca      0.0 \n"
	"        Mg      50500\n"
	"        Na      55200\n"
	"        K       15800\n"
	"        S(6)    76200\n"
	"        Cl      187900\n"
	"        Br      2670\n"
	"        C       1       CO2(g) -3.5\n"
	"INVERSE_MODELING\n"
	"        -solution 1 2\n"
	"        -uncertainties .025\n"
	"        -range\n"
	"        -balances \n"
	"                Br\n"
	"                K\n"
	"                Mg\n"
	"        -phases\n"
	"                H2O(g)  pre\n"
	"                Calcite pre\n"
	"                CO2(g)  pre\n"
	"                Gypsum  pre\n"
	"                Halite  pre\n"
	"                Glauberite pre\n"
	"                Polyhalite pre\n"
	"END\n"
	,
	"Example 17—Inverse Modeling with Evaporation\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ex18(
	"TITLE Example 18.--Inverse modeling of Madison aquifer\n"
	"SOLUTION 1 Recharge number 3\n"
	"        units   mmol/kgw\n"
	"        temp    9.9\n"
	"        pe      0.\n"
	"        pH      7.55\n"
	"        Ca      1.2\n"
	"        Mg      1.01\n"
	"        Na      0.02\n"
	"        K       0.02\n"
	"        Fe(2)   0.001\n"
	"        Cl      0.02\n"
	"        S(6)    0.16\n"
	"        S(-2)   0\n"
	"        C(4)    4.30\n"
	"        -i      13C     -7.0    1.4    \n"
	"        -i      34S     9.7     0.9    \n"
	"SOLUTION 2 Mysse\n"
	"        units   mmol/kgw\n"
	"        temp    63.\n"
	"        pH      6.61\n"
	"        pe      0.      \n"
	"        redox   S(6)/S(-2)\n"
	"        Ca      11.28\n"
	"        Mg      4.54\n"
	"        Na      31.89\n"
	"        K       2.54\n"
	"        Fe(2)   0.0004\n"
	"        Cl      17.85\n"
	"        S(6)    19.86\n"
	"        S(-2)   0.26\n"
	"        C(4)    6.87\n"
	"        -i      13C     -2.3    0.2   \n"
	"        -i      34S(6)  16.3    1.5   \n"
	"        -i      34S(-2) -22.1   7     \n"
	"INVERSE_MODELING 1\n"
	"        -solutions 1 2 \n"
	"        -uncertainty 0.05\n"
	"        -range\n"
	"        -isotopes\n"
	"                13C\n"
	"                34S\n"
	"        -balances\n"
	"                Fe(2)   1.0\n"
	"                ph      0.1\n"
	"        -phases\n"
	"                Dolomite        dis     13C     3.0     2\n"
	"                Calcite         pre     13C     -1.5    1\n"
	"                Anhydrite       dis     34S     13.5    2\n"
	"                CH2O            dis     13C     -25.0   5\n"
	"                Goethite\n"
	"                Pyrite          pre     34S     -22.    2\n"
	"                CaX2            pre\n"
	"                Ca.75Mg.25X2    pre\n"
	"                MgX2            pre\n"
	"                NaX\n"
	"                Halite\n"
	"                Sylvite\n"
	"PHASES\n"
	"   Sylvite\n"
	"        KCl = K+ + Cl-\n"
	"        -log_k  0.0\n"
	"   CH2O\n"
	"        CH2O + H2O = CO2 + 4H+ + 4e-\n"
	"        -log_k  0.0\n"
	"EXCHANGE_SPECIES\n"
	"        0.75Ca+2 + 0.25Mg+2 + 2X- = Ca.75Mg.25X2\n"
	"        log_k   0.0\n"
	"END\n"
	,
	"Example 18—Inverse Modeling of the Madison Aquifer\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ex19(
	"TITLE Example 19.--Linear, Freundlich and Langmuir isotherms for\n"
	"      Cd sorption on loamy sand. Calculates Example 7.1\n"
	"      from Appelo and Postma, 2005. Data from Christensen, 1984.\n"
	"SURFACE_MASTER_SPECIES\n"
	"      Linear Linear\n"
	"      Freundlich Freundlich\n"
	"      Langmuir Langmuir\n"
	"SURFACE_SPECIES\n"
	"  Linear = Linear\n"
	"  Linear + Cd+2 = LinearCd+2\n"
	"      -log_k -100.7         # log10(0.2) - 100\n"
	"      -mole_balance LinearCdCl2\n"
	"  Freundlich = Freundlich\n"
	"  Freundlich + 0.722 Cd+2 = FreundlichCd+2\n"
	"      -log_k -102.61        # log10(0.421) + (0.722 - 1) * log10(112.4e6) - 100\n"
	"      -no_check\n"
	"      -mole_balance FreundlichCdCl2\n"
	"  Langmuir = Langmuir\n"
	"  Langmuir + Cd+2 = LangmuirCd+2\n"
	"      -log_k 6.56           # log10(112.4 / 30.9e-6)\n"
	"      -mole_balance LangmuirCdCl2\n"
	"SURFACE 1\n"
	"      Linear 1e100 1 1\n"
	"      Freundlich 1e100 1 1\n"
	"      Langmuir 8.45e-8 1 1  # 9.5 / 112.4e6\n"
	"      -no_edl\n"
	"SOLUTION 1\n"
	"      pH  6\n"
	"      Ca  1\n"
	"      Cl  2\n"
	"REACTION 1\n"
	"      CdCl2 1\n"
	"      0.7e-6 in 20\n"
	"USER_GRAPH Example 19\n"
	"      -headings Linear Freundlich Langmuir\n"
	"      -chart_title \"Sorption Isotherms\"\n"
	"      -axis_titles \"Dissolved Cd, in micrograms per kilogram water\" \\\n"
	"                    \"Sorbed Cd, in micrograms per gram soil\"\n"
	"      -plot_tsv_file ex19_meas.tsv\n"
	"      -axis_scale x_axis 0 40\n"
	"      -axis_scale y_axis 0 6\n"
	"      -initial_solutions true\n"
	"  -start\n"
	"10 x = act(\"Cd+2\") * 112.4e6\n"
	"20 PLOT_XY x, mol(\"LinearCd+2\")*112e6, color = Green, symbol = None, line_width = 2\n"
	"30 PLOT_XY x, mol(\"FreundlichCd+2\")*112e6, color = Blue, symbol = None, line_width = 2\n"
	"40 PLOT_XY x, mol(\"LangmuirCd+2\")*112e6, color = Orange, symbol = None, line_width = 2\n"
	"  -end\n"
	"PRINT\n"
	"     -reset false\n"
	"END\n"
	,
	"Example 19—Modeling Cd+2 Sorption with Linear, Freundlich, and Langmuir Isotherms, and with a Deterministic Distribution of Sorption Sites for Organic Matter, Clay Minerals, and Iron Oxyhydroxides\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ex20a(
	"TITLE Example 20A.--Calculate carbonate solid solution\n"
	"PRINT\n"
	"	-censor_species        1e-006\n"
	"SOLUTION 1 # water to find composition of marine carbonate\n"
	"	pH      8.2\n"
	"	Na	1       charge\n"
	"	Ca	10      Calcite        0\n"
	"	C	2	\n"
	"	[13C]	0       # permil\n"
	"	[14C]	0       # pmc\n"
	"	D	0       # permil\n"
	"	[18O]	0       # permil\n"
	"END\n"
	"SOLID_SOLUTION 1 No [14C]\n"
	"Calcite \n"
	"	-comp	Calcite                 0\n"
	"	-comp	CaCO2[18O](s)           0\n"
	"	-comp	CaCO[18O]2(s)           0\n"
	"	-comp	CaC[18O]3(s)            0\n"
	"	-comp 	Ca[13C]O3(s)            0\n"
	"	-comp	Ca[13C]O2[18O](s)       0\n"
	"	-comp	Ca[13C]O[18O]2(s)       0\n"
	"	-comp	Ca[13C][18O]3(s)        0\n"
	"END 	\n"
	"RUN_CELLS\n"
	"	-cells 1\n"
	"USER_PRINT\n"
	"-start\n"
	"	10 PRINT pad(\"Component\", 20), \"Mole fraction\"\n"
	"	20 t = LIST_S_S(\"Calcite\", count, name$, moles)\n"
	"	30 for i = 1 to count\n"
	"	40   PRINT pad(name$(i),20), moles(i)/t\n"
	"	50 next i\n"
	"-end\n"
	"END	\n"
	,
	"Example 20—Distribution of Isotopes between Water and Calcite\n"
	"calculating a calcite isotopic solid solution\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ex20b(
	"#must use DATABASE iso.dat\n"
	"TITLE Example 20B.--Isotope evolution.\n"
	"PRINT\n"
	"	-censor_species        1e-006\n"
	"KNOBS\n"
	"	-diagonal_scale\n"
	"	-step 10\n"
	"	-pe   5\n"
	"#\n"
	"# Open system calculation \n"
	"# 	\n"
	"SOLID_SOLUTION 1 With [14C]\n"
	"Calcite \n"
	"	-comp	Calcite                 0\n"
	"	-comp	CaCO2[18O](s)           0\n"
	"	-comp	CaCO[18O]2(s)           0\n"
	"	-comp	CaC[18O]3(s)            0\n"
	"	-comp 	Ca[13C]O3(s)            0\n"
	"	-comp	Ca[13C]O2[18O](s)       0\n"
	"	-comp	Ca[13C]O[18O]2(s)       0\n"
	"	-comp	Ca[13C][18O]3(s)        0\n"
	"	-comp 	Ca[14C]O3(s)            0\n"
	"	-comp	Ca[14C]O2[18O](s)       0\n"
	"	-comp	Ca[14C]O[18O]2(s)       0\n"
	"	-comp	Ca[14C][18O]3(s)        0\n"
	"END\n"
	"REACTION 1\n"
	"	Calcite               9.8283e-001 \n"
	"	Ca[13C]O3(s)          1.1011e-002 \n"
	"	CaCO2[18O](s)         6.0825e-003 \n"
	"	Ca[13C]O2[18O](s)     6.8147e-005 \n"
	"	CaCO[18O]2(s)         1.2548e-005 \n"
	"	Ca[13C]O[18O]2(s)     1.4058e-007 \n"
	"	CaC[18O]3(s)          8.6284e-009 \n"
	"	Ca[13C][18O]3(s)      9.6671e-011 \n"
	"	0.0005 mole\n"
	"END\n"
	"USER_PRINT\n"
	"10 PRINT \"Calcite added: \", GET(0) * RXN\n"
	"USER_GRAPH 1 Example 20\n"
	"        -headings Open--Dissolved Open--Calcite\n"
	"        -chart_title \"Oxygen-18\"\n"
	"        -axis_titles \"Marine calcite reacted, in moles\" \"Permil\"\n"
	"        -axis_scale x_axis 0 0.05 a a \n"
	"        -axis_scale y_axis -10 30 a a\n"
	"   -start\n"
	"10 PUT(GET(0) + 1, 0)   \n"
	"20 PLOT_XY RXN*GET(0),ISO(\"R(18O)\"), color=Red, line_w=2, symbol=None\n"
	"30 PLOT_XY RXN*GET(0),ISO(\"R(18O)_Calcite\"), color=Green, line_w=2, symbol=None\n"
	"   -end\n"
	"END\n"
	"USER_GRAPH 2 Example 20\n"
	"        -headings Open--Dissolved Open-Calcite\n"
	"        -chart_title \"Carbon-13\"\n"
	"        -axis_titles \"Marine calcite reacted, in moles\" \"Permil\"\n"
	"        -axis_scale x_axis 0 0.05 a a \n"
	"        -axis_scale y_axis -25 5.0 a a\n"
	"        -plot_tsv  ex20-c13.tsv\n"
	"   -start\n"
	"10 PLOT_XY RXN*GET(0),ISO(\"R(13C)\"), color=Red, line_w=2, symbol=None\n"
	"20 PLOT_XY RXN*GET(0),ISO(\"R(13C)_Calcite\"), color=Green, line_w=2, symbol=None \n"
	"   -end\n"
	"END\n"
	"USER_GRAPH 3 Example 20\n"
	"        -headings Open--Dissolved Open--Calcite\n"
	"        -chart_title \"Carbon-14\"\n"
	"        -axis_titles \"Marine calcite reacted, in moles\" \"Percent modern carbon\"\n"
	"        -axis_scale x_axis 0 0.05 a a \n"
	"        -axis_scale y_axis 0 100 a a\n"
	"        -plot_tsv  ex20-c14.tsv\n"
	"   -start\n"
	"10 PLOT_XY RXN*GET(0),ISO(\"R(14C)\"), color=Red, line_w=2, symbol=None\n"
	"20 PLOT_XY RXN*GET(0),ISO(\"R(14C)_Calcite\"), color=Green, line_w=2, symbol=None \n"
	"   -end                                                                            \n"
	"END\n"
	"SOLUTION 1\n"
	"	pH	5  	charge\n"
	"	pe      10\n"
	"	C	2	CO2(g)  -1.0	\n"
	"	[13C]	-25	# permil\n"
	"	[14C]	100	# pmc\n"
	"	[18O]	-5	# permil\n"
	"SELECTED_OUTPUT\n"
	"	-reset false\n"
	"	-file ex20_open\n"
	"USER_PUNCH\n"
	"-start\n"
	"10 FOR i = 1 to 100\n"
	"20 PUNCH EOL$ + \"USE solution 1\"      \n"
	"30 PUNCH EOL$ + \"USE solid_solution 1\"\n"
	"40 PUNCH EOL$ + \"USE reaction 1\"      \n"
	"50 PUNCH EOL$ + \"SAVE solution 1\"     \n"
	"60 PUNCH EOL$ + \"END\"                 \n"
	"70 NEXT i\n"
	"-end\n"
	"END\n"
	"PRINT\n"
	"	-selected_output false\n"
	"END\n"
	"INCLUDE$ ex20_open\n"
	"END\n"
	"#\n"
	"# Closed system calculation \n"
	"# \n"
	"USER_GRAPH 1 Oxygen-18\n"
	"        -headings Closed--Dissolved Closed--Calcite\n"
	"   -start\n"
	"10 PUT(GET(1) + 1, 1)   \n"
	"20 PLOT_XY RXN*GET(1),ISO(\"R(18O)\"), color=Blue, line_w=0, symbol=Circle\n"
	"30 PLOT_XY RXN*GET(1),ISO(\"R(18O)_Calcite\"), color=Black, line_w=0, symbol=Circle\n"
	"   -end\n"
	"END\n"
	"USER_GRAPH 2 Carbon-13\n"
	"        -headings Closed--Dissolved Closed--Calcite\n"
	"   -start\n"
	"10 PLOT_XY RXN*GET(1),ISO(\"R(13C)\"), color=Blue, line_w=2, symbol=None\n"
	"20 PLOT_XY RXN*GET(1),ISO(\"R(13C)_Calcite\"), color=Black, line_w=2, symbol=None \n"
	"   -end\n"
	"END\n"
	"USER_GRAPH 3 Carbon-14\n"
	"        -headings Closed--Dissolved Closed--Calcite\n"
	"   -start\n"
	"10 PLOT_XY RXN*GET(1),ISO(\"R(14C)\"), color=Blue, line_w=2, symbol=None\n"
	"20 PLOT_XY RXN*GET(1),ISO(\"R(14C)_Calcite\"), color=Black, line_w=2, symbol=None \n"
	"   -end\n"
	"END\n"
	"USER_PRINT\n"
	"10 PRINT \"Calcite added: \", GET(1), GET(1)*0.0005, RXN\n"
	"SOLUTION 1\n"
	"	pH	5  	charge\n"
	"	pe      10\n"
	"	C	2	CO2(g)  -1.0	\n"
	"	[13C]	-25	# permil\n"
	"	[14C]	100	# pmc\n"
	"	[18O]	-5	# permil\n"
	"END	\n"
	"INCREMENTAL_REACTIONS true\n"
	"# Alternative to redefinition of REACTION 1\n"
	"#REACTION_MODIFY 1\n"
	"#	-steps\n"
	"#		0.05\n"
	"#	-equal_increments	1\n"
	"#	-count_steps		100\n"
	"REACTION 1\n"
	"	Calcite               9.8283e-001 \n"
	"	Ca[13C]O3(s)          1.1011e-002 \n"
	"	CaCO2[18O](s)         6.0825e-003 \n"
	"	Ca[13C]O2[18O](s)     6.8147e-005 \n"
	"	CaCO[18O]2(s)         1.2548e-005 \n"
	"	Ca[13C]O[18O]2(s)     1.4058e-007 \n"
	"	CaC[18O]3(s)          8.6284e-009 \n"
	"	Ca[13C][18O]3(s)      9.6671e-011 \n"
	"	0.05 mole in 100 steps\n"
	"RUN_CELLS\n"
	"	-cells 1\n"
	"END	\n"
	"	\n"
	"\n"
	,
	"Example 20—Distribution of Isotopes between Water and Calcite\n"
	"open- and closed-system isotopic evolution of calcite and solution\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ex21(
	"TITLE Diffusion through Opalinus Clay in a radial diffusion cell, \n"
	"      Appelo and others, 2010, GCA, v. 74, p. 1201-1219.\n"
	"SOLUTION_MASTER_SPECIES\n"
	"# element   species   alk gfw_formula element_gfw\n"
	"  Hto       Hto       0.0   20        20\n"
	"  Na_tr     Na_tr+    0.0   22        22\n"
	"  Cl_tr     Cl_tr-    0.0   36        36\n"
	"  Cs        Cs+       0.0  132.905   132.905\n"
	"SOLUTION_SPECIES\n"
	"  Hto = Hto;        log_k 0; -gamma 1e6 0;     -dw 2.236e-9\n"
	"  Na_tr+ = Na_tr+;  log_k 0; -gamma 4.0 0.075; -dw 1.33e-9; -erm_ddl 1.23\n"
	"  Cl_tr- = Cl_tr-;  log_k 0; -gamma 3.5 0.015; -dw 1.31e-9 # dw = dw(water) / 1.55 = 2.03e-9 / 1.55\n"
	"  Cs+ = Cs+;        log_k 0; -gamma 3.5 0.015; -dw 2.07e-9; -erm_ddl 1.23\n"
	"SURFACE_MASTER_SPECIES\n"
	"  Su_fes Su_fes-   # Frayed Edge Sites\n"
	"  Su_ii Su_ii-     # Type II sites of intermediate strength\n"
	"  Su_ Su_-         # Double layer, planar sites are modeled with EXCHANGE\n"
	"SURFACE_SPECIES\n"
	"  Su_fes- = Su_fes-; log_k 0\n"
	"  Na+ + Su_fes- = NaSu_fes; log_k 10\n"
	"  Na_tr+ + Su_fes- = Na_trSu_fes; log_k 10\n"
	"  K+ + Su_fes- = KSu_fes; log_k 12.4\n"
	"  Cs+ + Su_fes- = CsSu_fes; log_k 17.14\n"
	"\n"
	"  Su_ii- = Su_ii-; log_k 0\n"
	"  Na+ + Su_ii- = NaSu_ii; log_k 10\n"
	"  Na_tr+ + Su_ii- = Na_trSu_ii; log_k 10\n"
	"  K+ + Su_ii- = KSu_ii; log_k 12.1\n"
	"  Cs+ + Su_ii- = CsSu_ii; log_k 14.6\n"
	"\n"
	"  Su_- = Su_-; log_k 0\n"
	"\n"
	"EXCHANGE_SPECIES\n"
	"  Na_tr+ + X- = Na_trX; log_k 0.0;  -gamma 4.0 0.075\n"
	"  Cs+ + X- = CsX;       log_k 2.04; -gamma 3.5 0.015\n"
	"\n"
	"SOLUTION 0-2 column with only cell 1, two boundary solutions 0 and 2.\n"
	"  Na 1; Cl 1\n"
	"END\n"
	"\n"
	"KNOBS; -diagonal_scale true # -tolerance 1e-20 # because of low concentrations\n"
	"\n"
	"SOLUTION 3 tracer solution\n"
	"  pH 7.6; pe 14 O2(g) -1.0; temp 23\n"
	"  Na 240; K 1.61; Mg 16.9; Ca 25.8; Sr 0.505\n"
	"  Cl 300; S(6) 14.1; Fe(2) 0.0; Alkalinity 0.476\n"
	"# uncomment tracer concentrations and kg water 1 by 1...\n"
	"  Hto 1.14e-6;   -water 0.2\n"
	"#  Cl_tr 2.505e-2; -water 0.502\n"
	"#  Cs 1; Na_tr 1.87e-7; -water 1.02\n"
	"SELECTED_OUTPUT\n"
	"  -file radial; -reset false\n"
	"USER_PUNCH\n"
	"     # Define symbols and pi...\n"
	"1    nl$ = EOL$                # newline\n"
	"2    x$  = CHR$(35)            # cross '#'\n"
	"3    sc$ = CHR$(59)            # semicolon ';'\n"
	"4    pi  = 2 * ARCTAN(1e10)    # 3.14159...\n"
	"\n"
	"     # Define experimental parameters...\n"
	"10   height = 0.052           # length of the clay cylinder / m\n"
	"20   r_int = 6.58e-3          # inner radius of clay cylinder / m\n"
	"30   r_ext = 25.4e-3          # outer radius\n"
	"40   thickn_filter1 = 1.8e-3  # tracer-in filter thickness / m\n"
	"50   thickn_filter2 = 1.6e-3  # tracer-out filter thickness / m\n"
	"60   por_filter1 = 0.418      # porosity\n"
	"70   por_filter2 = 0.367\n"
	"80   G_filter1 = 4.18         # geometrical factor. (for filters, G = por / 10)\n"
	"90   G_filter2 = 3.67\n"
	"100  V_end = 0.2              # volume of the tracer-out solution / L\n"
	"110  thickn_clay = r_ext - r_int # clay thickness / m\n"
	"120  por_clay = 0.159\n"
	"130  rho_b_eps = 2.7 * (1 - por_clay) / por_clay  # clay bulk density / porosity / (kg/L)\n"
	"140  CEC = 0.12 * rho_b_eps   # CEC / (eq/L porewater)\n"
	"150  A_por = 37e3 * rho_b_eps # pore surface area / (m2/L porewater)\n"
	"\n"
	"160  DIM tracer$(4), exp_time(4), scale_y1$(4), scale_y2$(4), profile_y1$(4), profile_y2$(4)\n"
	"170  DATA 'Hto', 'Cl_tr', 'Na_tr', 'Cs'\n"
	"180  READ tracer$(1), tracer$(2), tracer$(3), tracer$(4)\n"
	"     # experimental times (seconds) for HTO, 36Cl, 22Na and Cs, respectively,\n"
	"     # in order of increasing times...\n"
	"200  DATA 86400 * 20, 86400 * 40, 86400 * 45, 86400 * 1000\n"
	"210  READ exp_time(1), exp_time(2), exp_time(3), exp_time(4)\n"
	"     # scale y1-axis (flux) (not used)...\n"
	"230  DATA '1', '1', '1', '1'\n"
	"240  READ scale_y1$(1), scale_y1$(2), scale_y1$(3), scale_y1$(4)\n"
	"     # scale y2-axis (mass) (not used)...\n"
	"260  DATA '1', '1', '1', '1'\n"
	"270  READ scale_y2$(1), scale_y2$(2), scale_y2$(3), scale_y2$(4)\n"
	"     # scale max of the profile y axes...\n"
	"280  DATA '0 1.2e-9', '0 2.5e-5', '0 2e-10', '0 auto'\n"
	"290  READ profile_y1$(1), profile_y1$(2), profile_y1$(3), profile_y1$(4)\n"
	"300  DATA '0 1.2e-9', '0 2.5e-5', '0 6e-10', '0 auto'\n"
	"310  READ profile_y2$(1), profile_y2$(2), profile_y2$(3), profile_y2$(4)\n"
	"\n"
	"     # Define model parameters...\n"
	"350  Dw = 2.5e-9              # default tracer diffusion coefficient / (m2/s)\n"
	"360  nfilt1 = 1               # number of cells in filter 1\n"
	"370  nfilt2 = 1               # number of cells in filter 2\n"
	"380  nclay = 11               # number of clay cells\n"
	"390  f_free = 0.117           # fraction of free pore water (0.01 - 1)\n"
	"400  f_DL_charge = 0.45       # fraction of CEC charge in electrical double layer\n"
	"410  tort_n = -0.99           # exponent in Archie's law, -1.045 without filters\n"
	"420  G_clay = por_clay^tort_n # geometrical factor\n"
	"430  interlayer_D$ = 'false'  # 'true' or 'false' for interlayer diffusion\n"
	"440  G_IL = 700               # geometrical factor for clay interlayers\n"
	"450  punch_time = 60 * 60 * 6 # punch time / seconds\n"
	"460  profile$ = 'true'        # 'true' or 'false' for c/x profile visualization\n"
	"470  IF nfilt1 = 0 THEN thickn_filter1 = 0\n"
	"480  IF nfilt2 = 0 THEN thickn_filter2 = 0\n"
	"\n"
	"     # See which tracer is present...\n"
	"490  IF tot(\"Hto\") > 1e-10 THEN tracer = 1 ELSE \\\n"
	"     IF tot(\"Cl_tr\") > 1e-10 THEN tracer = 2 ELSE tracer = 3\n"
	"\n"
	"     # Define clay pore water composition...\n"
	"520  sol$ = nl$ + ' pH 7.6' + sc$ +' pe 14 O2(g) -1.0' + sc$ +' temp 23'\n"
	"530  sol$ = sol$ + nl$ + ' Na 240' + sc$ +' K 1.61' + sc$ +' Mg 16.9' + sc$ +' Ca 25.8' + sc$ +' Sr 0.505'\n"
	"540  sol$ = sol$ + nl$ + ' Cl 300' + sc$ +' S(6) 14.1' + sc$ +' Fe(2) 0.0' + sc$ +' Alkalinity 0.476'\n"
	"\n"
	"     # Define phases in which the tracers precipitate...\n"
	"550  tracer_phases$ = nl$ + 'PHASES '\n"
	"560  tracer_phases$ = tracer_phases$ + nl$ + ' A_Hto' + nl$ + ' Hto = Hto' + sc$ +' log_k -15'\n"
	"570  tracer_phases$ = tracer_phases$ + nl$ + ' A_Na_tr' + nl$ + ' Na_trCl = Na_tr+ + Cl-' + sc$ + ' log_k -14'\n"
	"580  tracer_phases$ = tracer_phases$ + nl$ + ' A_Cl_tr' + nl$ + ' NaCl_tr = Na+ + Cl_tr-' + sc$ +' log_k -14'\n"
	"590  tracer_phases$ = tracer_phases$ + nl$ + ' A_Cs' + nl$ + ' CsCl = Cs+ + Cl-' + sc$ + ' log_k -13'\n"
	"600  DIM tracer_equi$(4)\n"
	"610  FOR i = 1 TO 4\n"
	"620    tracer_equi$(i) = nl$ + 'A_' + tracer$(i) + ' 0 0'\n"
	"630  NEXT i\n"
	"\n"
	"     # Write solutions for the cells...\n"
	"650  punch nl$ + 'PRINT ' + sc$ + ' -reset false' + sc$ + ' -echo_input true' + sc$ + ' -user_print true'\n"
	"660  IF nfilt1 = 0 THEN GOTO 800\n"
	"670  punch nl$ + x$ + ' filter cells at tracer-in side...'\n"
	"680  r1 = r_int - thickn_filter1\n"
	"690  xf1 = thickn_filter1 / nfilt1\n"
	"700  FOR i = 1 TO nfilt1\n"
	"710    num$ = TRIM(STR$(i + 3)) + sc$\n"
	"720    V_water = 1e3 * height * por_filter1 * pi * (SQR(r1 + xf1) - SQR(r1))\n"
	"730    punch nl$ + 'SOLUTION ' + num$ + ' -water ' + STR$(V_water)\n"
	"740    punch sol$ + nl$\n"
	"750    r1 = r1 + xf1\n"
	"760  NEXT i\n"
	"\n"
	"800  punch nl$ + nl$ + x$ + ' cells in Opalinus Clay...'\n"
	"810  r1 = r_int\n"
	"820  x = thickn_clay / nclay\n"
	"830  FOR i = 1 TO nclay\n"
	"840    num$ = TRIM(STR$(i + 3 + nfilt1)) + sc$\n"
	"850    V_water = 1e3 * height * por_clay * pi * (SQR(r1 + x) - SQR(r1))\n"
	"860    punch nl$ + 'SOLUTION ' + num$ + ' -water ' + STR$(V_water * f_free)\n"
	"870    punch sol$\n"
	"880    IF f_free = 1 and tracer = 1  THEN GOTO 960\n"
	"890    punch nl$ + 'SURFACE ' + num$ + ' -equil ' + num$\n"
	"900    punch nl$ + ' Su_ ' + TRIM(STR$(f_DL_charge * CEC * V_water)) + STR$(A_por) + ' ' + STR$(V_water)\n"
	"910    punch nl$ + ' Su_ii ' + TRIM(STR$(7.88e-4 * rho_b_eps * V_water))\n"
	"920    punch nl$ + ' Su_fes ' + TRIM(STR$(7.4e-5 * rho_b_eps * V_water))\n"
	"930    IF f_free < 1 THEN punch nl$ + ' -Donnan ' + TRIM(STR$((1 - f_free) * 1e-3 / A_por))\n"
	"940    punch nl$ + 'EXCHANGE ' + num$ + ' -equil ' + num$\n"
	"950    punch nl$ + ' X ' + TRIM(STR$((1 - f_DL_charge) * CEC * V_water)) + nl$\n"
	"960    r1 = r1 + x\n"
	"970  NEXT i\n"
	"\n"
	"1000 IF nfilt2 = 0 THEN GOTO 1200\n"
	"1010 punch nl$ + nl$ + x$ + ' tracer-out filter cells...'\n"
	"1020 r1 = r_ext\n"
	"1030 xf2 = thickn_filter2 / nfilt2\n"
	"1040 FOR i = 1 TO nfilt2\n"
	"1050   num$ = TRIM(STR$(i + 3 + nfilt1 + nclay)) + sc$\n"
	"1060   V_water = 1e3 * height * por_filter2 * pi * (SQR(r1 + xf2) - SQR(r1))\n"
	"1070   punch nl$ + 'SOLUTION ' + num$ + ' -water ' + STR$(V_water)\n"
	"1080   punch sol$ + nl$\n"
	"1090   r1 = r1 + xf2\n"
	"1100 NEXT i\n"
	"\n"
	"1200 punch nl$ + x$ + ' outside solution...'\n"
	"1210 num$ = TRIM(STR$(4 + nfilt1 + nclay + nfilt2)) + sc$\n"
	"1220 punch nl$ + 'SOLUTION ' + num$ + ' -water ' + STR$(V_end)\n"
	"1230 punch sol$\n"
	"1240 punch nl$ + 'END'\n"
	"\n"
	"     # Write phases in which the tracers precipitate...\n"
	"1300 punch nl$ + tracer_phases$\n"
	"1310 punch nl$ + 'EQUILIBRIUM_PHASES ' + num$ + tracer_equi$(tracer)\n"
	"1312 If tracer = 3 THEN punch nl$ + tracer_equi$(tracer + 1)\n"
	"1320 punch nl$ + 'END'\n"
	"\n"
	"     # Define mixing factors for the diffusive flux between cells 1 and 2:\n"
	"     #    J_12 = -2 * Dw / (x_1 / g_1 + x_2 / g_2) * (c_2 - c_1)\n"
	"     # Multiply with dt * A / (V = 1e-3 m3).  (Actual volumes are given with SOLUTION; -water)\n"
	"     # Use harmonic mean: g_1 = por_1 / G_1, g_2 = por_2 / G_2, x_1 = Delta(x_1), etc.\n"
	"1400 IF nfilt1 > 0 THEN gf1 = por_filter1 / G_filter1\n"
	"1410 IF nfilt2 > 0 THEN gf2 = por_filter2 / G_filter2\n"
	"1420 g = por_clay / G_clay\n"
	"     # Find max time step = 0.5 * V_water * dx * G_factor / (Dw * por * A * fbc)\n"
	"     #            V_water = por * pi * height * ((r + dr)^2 - r^2)\n"
	"     #                  A = por * pi * height * r * 2\n"
	"     # At the inlet of the tracers, fbc = 2...\n"
	"1500 IF nfilt1 = 0 THEN GOTO 1530\n"
	"1510   r1 = r_int - thickn_filter1\n"
	"1520   ff = (SQR(r1 + xf1) - SQR(r1)) * xf1 * G_filter1 / (r1 * 2) / 2\n"
	"1530 ff1 = (SQR(r_int + x) - SQR(r_int)) * x * G_clay / (r_int * 2) / 2\n"
	"     # Perhaps the clay has very small cells...\n"
	"1540 IF nfilt1 = 0 THEN ff = ff1 ELSE IF ff1 * 2 < ff THEN ff = ff1 * 2\n"
	"     # Or at the filter1-clay transition, fbc = 1...\n"
	"1550 IF nfilt1 > 0 THEN ff1 = (SQR(r_int + x) - SQR(r_int)) * (xf1 / gf1 + x / g) / (2 * r_int * 2)\n"
	"1560 IF nfilt1 > 0 AND ff1 < ff THEN ff = ff1\n"
	"     # Perhaps filter2 has very small cells...\n"
	"1570 IF nfilt2 > 0 THEN ff1 = (SQR(r_ext + xf2) - SQR(r_ext)) * xf2 * G_filter2 / (r_ext * 2)\n"
	"1580 IF nfilt2 > 0 AND ff1 < ff THEN ff = ff1\n"
	"1590 dt_max = 0.5 * ff / Dw\n"
	"     # Check with punch times, set shifts...\n"
	"1610 IF punch_time < dt_max THEN dt = punch_time ELSE dt = dt_max\n"
	"1620 punch_fr = 1\n"
	"1630 IF dt < punch_time THEN punch_fr = ceil(punch_time / dt)\n"
	"1640 dt = punch_time / punch_fr\n"
	"1650 shifts = ceil(exp_time(tracer) / dt)\n"
	"     # Write mixing factors...\n"
	"1700 punch nl$ + nl$ + x$ + ' mixing factors...'\n"
	"1710 r1 = r_int\n"
	"1720 IF nfilt1 > 0 THEN r1 = r_int - thickn_filter1\n"
	"1730 A = height * 2 * pi\n"
	"1740 FOR i = 0 TO nfilt1 + nclay + nfilt2\n"
	"1750   IF i = 0 OR i = nfilt1 + nclay + nfilt2 THEN fbc = 2 ELSE fbc = 1\n"
	"1760   IF i > nfilt1 OR nfilt1 = 0 THEN GOTO 1810\n"
	"1770     IF i < nfilt1 THEN mixf = Dw * fbc / (xf1 / gf1) * dt * A * r1 / 1e-3\n"
	"1780     IF i = nfilt1 THEN mixf = 2 * Dw / (xf1 / gf1 + x / g) * dt * A * r1 / 1e-3\n"
	"1790     IF i < nfilt1 THEN r1 = r1 + xf1 ELSE r1 = r1 + x\n"
	"1800     GOTO 1880\n"
	"1810   IF i > nfilt1 + nclay THEN GOTO 1860\n"
	"1820     mixf = Dw * fbc / (x / g) * dt * A * r1 / 1e-3\n"
	"1830     IF i = nfilt1 + nclay AND nfilt2 > 0 THEN mixf = 2 * Dw / (xf2 / gf2 + x / g) * dt * A * r1 / 1e-3\n"
	"1840     IF i < nfilt1 + nclay THEN r1 = r1 + x ELSE r1 = r1 + xf2\n"
	"1850     GOTO 1880\n"
	"1860   mixf = Dw * fbc / (xf2 / gf2) * dt * A * r1 / 1e-3\n"
	"1870   r1 = r1 + xf2\n"
	"1880   punch nl$ + 'MIX ' + TRIM(STR$(i + 3)) + sc$ + STR$(i + 4) + STR$(mixf)\n"
	"1890 NEXT i\n"
	"1900 punch nl$ + 'END'\n"
	"\n"
	"     # Write TRANSPORT...\n"
	"2000 punch nl$ + 'TRANSPORT'\n"
	"2010 stag = 2 + nfilt1 + nclay + nfilt2\n"
	"2020 punch nl$ + ' -warnings true'\n"
	"2030 punch nl$ + ' -shifts ' + TRIM(STR$(shifts))\n"
	"2040 punch nl$ + ' -flow diff' + sc$ + ' -cells 1' + sc$ + ' -bcon 1 2' + sc$ + ' -stag ' + TRIM(STR$(stag))\n"
	"2050 punch nl$ + ' -time ' + STR$(dt)\n"
	"2060 punch nl$ + ' -multi_D true ' + STR$(Dw) + STR$(por_clay) + ' 0.0 ' + TRIM(STR$(-tort_n))\n"
	"2070 punch nl$ + ' -interlayer_D ' + interlayer_D$ + ' 0.001 0.0 ' + TRIM(STR$(G_IL))\n"
	"2080 punch nl$ + ' -punch_fr ' + TRIM(STR$(punch_fr)) + sc$ + ' -punch_c ' + TRIM(STR$(2 + stag))\n"
	"\n"
	"     # Write USER_GRAPH...\n"
	"2180 FOR i = 0 to 1\n"
	"2190   punch nl$ + 'USER_GRAPH ' + TRIM(STR$(tracer + i)) + ' Example 21' + nl$\n"
	"2200   punch nl$ + ' -chart_title \" ' + tracer$(tracer + i) + ' Diffusion to Outer Cell\"'\n"
	"2210   punch nl$ + ' -plot_tsv_file ex21_' + tracer$(tracer + i) + '_rad.tsv'\n"
	"2220   punch nl$ + ' -axis_scale x_axis 0 ' + TRIM(STR$(exp_time(tracer + i) / (3600 * 24)))\n"
	"2230   punch nl$ + ' -axis_titles \"Time, in days\" \"Flux, in moles per square meter per second\" \\\n"
	"            \"Accumulated mass, in moles\"'\n"
	"2240   punch nl$ + ' -plot_concentration_vs time'\n"
	"2250   punch nl$ + ' 10 days = total_time / (3600 * 24)'\n"
	"2260   punch nl$ + ' 20 a = equi(\"A_' + tracer$(tracer + i) + '\")'\n"
	"2270   punch nl$ + ' 30 IF get(1) = 0 AND total_time > 0 THEN put(total_time, 1)'\n"
	"2280   punch nl$ + ' 40 dt = get(1)'\n"
	"2290   A = 2 * pi * r_ext * height\n"
	"2300   i$ = TRIM(STR$(2 + i))\n"
	"2310   punch nl$ + ' 50 plot_xy days - dt / (2 * 3600 * 24), (a - get(' + i$ + ')) / dt /' + STR$(A) + \\\n"
	"            ', color = Green, symbol = None'\n"
	"2320   punch nl$ + ' 60 put(a, ' + i$ + ')'\n"
	"2330   punch nl$ + ' 70 plot_xy days, equi(\"A_' + tracer$(tracer + i) + \\\n"
	"            '\"), y_axis = 2, color = Red, symbol = None'\n"
	"2340   IF tracer < 3 THEN GOTO 2360\n"
	"2350 NEXT i\n"
	"2360 punch nl$ + 'END'\n"
	"\n"
	"2400 IF profile$ = 'true' THEN GOSUB 3000\n"
	"2410 IF tracer < 3 THEN END # finished for Hto and Cl\n"
	"\n"
	"     # Continue with Cs...\n"
	"2420 IF profile$ = 'false' THEN punch nl$ + 'USER_GRAPH ' + TRIM(STR$(tracer)) + sc$ + ' -detach' ELSE \\\n"
	"                                punch nl$ + 'USER_GRAPH ' + TRIM(STR$(tracer + 4)) + sc$ + ' -detach'\n"
	"2440 tracer = tracer + 1\n"
	"2450 punch nl$ + 'TRANSPORT'\n"
	"2460 shifts = ceil((exp_time(tracer) - exp_time(tracer - 1))/ dt)\n"
	"2480 punch nl$ + ' -shifts ' + TRIM(STR$(shifts))\n"
	"2490 punch nl$ + ' -punch_fr ' + TRIM(STR$(punch_fr)) + sc$ + ' -punch_c ' + TRIM(STR$(2 + stag))\n"
	"2500 punch nl$ + 'END'\n"
	"2510 IF profile$ = 'true' THEN GOSUB 3000\n"
	"2520 END # finished...\n"
	"\n"
	"     # Write TRANSPORT and USER_GRAPH for concentration profile...\n"
	"3000 punch nl$ + 'TRANSPORT'\n"
	"3010 punch nl$ + ' -shifts 0'\n"
	"3020 punch nl$ + ' -punch_fr 2' + sc$ + ' -punch_c 3-' + TRIM(STR$(2 + stag))\n"
	"     # Write USER_GRAPH...\n"
	"3030 punch nl$ + 'USER_GRAPH ' + TRIM(STR$(tracer)) + sc$ + ' -detach'\n"
	"3040 punch nl$ + 'USER_GRAPH ' + TRIM(STR$(tracer + 4)) + ' Example 21' + nl$\n"
	"3050 punch nl$ + ' -chart_title \"' + tracer$(tracer) + ' Concentration Profile: Filter1 | Clay | Filter2\"'\n"
	"3060 REM punch nl$ + ' -plot_tsv_file + tracer$(tracer) + '_prof.tsv'\n"
	"3070 punch nl$ + ' -axis_scale x_axis 0 ' + TRIM(STR$((thickn_filter1 + thickn_clay + thickn_filter2) * 1e3))\n"
	"3080 punch nl$ + ' -axis_scale y_axis ' + profile_y1$(tracer)\n"
	"3090 punch nl$ + ' -axis_scale sy_axis ' + profile_y2$(tracer)\n"
	"3100 punch nl$ + ' -axis_titles ' + '\"Distance, in millimeters\" \"Free pore-water molality\" \"Total molality\"'\n"
	"3110 punch nl$ + ' -headings ' + tracer$(tracer) + '_free ' + tracer$(tracer) + '_tot'\n"
	"3120 punch nl$ + ' -plot_concentration_vs x'\n"
	"3130 punch nl$ + ' -initial_solutions true'\n"
	"3140 punch nl$ + ' 10 IF cell_no = 3 THEN xval = 0 ELSE xval = get(14)'\n"
	"3150 punch nl$ + ' 20 IF (' + TRIM(STR$(nfilt1)) + ' = 0 OR cell_no > ' + TRIM(STR$(nfilt1 + 3)) + ') THEN GOTO 60'\n"
	"3160 punch nl$ + ' 30 IF (cell_no = 4) THEN xval = xval + 0.5 * ' + TRIM(STR$(xf1))\n"
	"3170 punch nl$ + ' 40 IF (cell_no > 4 AND cell_no < ' + TRIM(STR$(nfilt1 + 4)) + \\\n"
	"          ') THEN xval = xval + ' + TRIM(STR$(xf1))\n"
	"3180 punch nl$ + ' 50 GOTO 200'\n"
	"3190 punch nl$ + ' 60 IF (cell_no = ' + TRIM(STR$(4 + nfilt1)) + ') THEN xval = xval + 0.5 * ' + \\\n"
	"          TRIM(STR$(xf1)) + ' + 0.5 * ' + TRIM(STR$(x))\n"
	"3200 punch nl$ + ' 70 IF (cell_no > ' + TRIM(STR$(4 + nfilt1)) + ' AND cell_no < ' + \\\n"
	"          TRIM(STR$(4 + nfilt1 + nclay)) + ') THEN xval = xval + ' + TRIM(STR$(x)) + ' ELSE GOTO 90'\n"
	"3210 punch nl$ + ' 80 GOTO 200'\n"
	"3220 punch nl$ + ' 90 IF (cell_no = ' + TRIM(STR$(4 + nfilt1 + nclay)) + ') THEN xval = xval + 0.5 * ' + \\\n"
	"          TRIM(STR$(x)) + ' + 0.5 * ' + TRIM(STR$(xf2))\n"
	"3230 punch nl$ + ' 100 IF (cell_no > ' + TRIM(STR$(4 + nfilt1 + nclay)) + ' AND cell_no <= ' + \\\n"
	"          TRIM(STR$(3 + nfilt1 + nclay + nfilt2)) + ') THEN xval = xval + ' + TRIM(STR$(xf2))\n"
	"3240 punch nl$ + ' 110 IF (cell_no = ' + TRIM(STR$(4 + nfilt1 + nclay + nfilt2)) + \\\n"
	"          ') THEN xval = xval + 0.5 * ' + TRIM(STR$(xf2))\n"
	"3250 punch nl$ + ' 200 y1 = TOT(\"' + tracer$(tracer) + '\")'\n"
	"3260 punch nl$ + ' 210 plot_xy xval * 1e3, y1, color = Blue, symbol = Plus'\n"
	"3270 punch nl$ + ' 220 IF cell_no = 3 THEN put(y1, 15)'\n"
	"3280 punch nl$ + ' 230 IF (cell_no < ' + TRIM(STR$(4 + nfilt1)) + ' OR cell_no > ' + \\\n"
	"          TRIM(STR$(3 + nfilt1 + nclay)) + ') THEN GOTO 400'\n"
	"3290 punch nl$ + ' 240 y2 = SYS(\"' + tracer$(tracer) + '\") / (tot(\"water\") + edl(\"water\"))'\n"
	"     # Remove REM if total conc's per kg solid must be plotted (and adapt axis_titles)...\n"
	"3310 punch nl$ + ' 250 REM y2 = y2 / ' + TRIM(STR$(rho_b_eps)) + x$ + ' conc / kg solid'\n"
	"3320 punch nl$ + ' 260 plot_xy xval * 1e3, y2, symbol = Circle, y_axis = 2'\n"
	"3330 punch nl$ + ' 270 IF (cell_no > ' + TRIM(STR$(5 + nfilt1)) + ') THEN GOTO 400'\n"
	"3340 punch nl$ + ' 280 IF ' + TRIM(STR$(nfilt1)) + ' THEN plot_xy ' + TRIM(STR$(thickn_filter1 * 1e3)) + \\\n"
	"          ', get(15), color = Black, symbol = None'\n"
	"3350 punch nl$ + ' 290 IF ' + TRIM(STR$(nfilt2)) + ' THEN plot_xy ' + \\\n"
	"          TRIM(STR$((thickn_filter1 + thickn_clay) * 1e3)) + ', get(15), color = Black, symbol = None'\n"
	"3360 punch nl$ + ' 300 put(0, 15)'\n"
	"3370 punch nl$ + ' 400 put(xval, 14)'\n"
	"3380 punch nl$ + 'END'\n"
	"3390 RETURN\n"
	"END\n"
	"PRINT\n"
	" -selected_out false; -status false\n"
	"INCLUDE$ radial\n"
	"END\n"
	,
	"Example 21—Modeling Diffusion of HTO, 36Cl-, 22Na+ and Cs+ in a Radial Diffusion Cell\n"
	);

std::pair<CString, CString> CTreeCtrlPfw::ex22(
	"TITLE Example 22.--Compare experimental CO2 solubilities at high CO2 pressure \n"
	"   with Peng-Robinson calc'ns with fixed-volume gas_phase, 25, 50, 75, 100 oC.\n"
	"SOLUTION 1\n"
	"GAS_PHASE 1\n"
	"    -fixed_volume\n"
	"    CO2(g) 0\n"
	"    H2O(g) 0\n"
	"REACTION\n"
	" CO2 1; 0 27*1\n"
	"INCREMENTAL_REACTIONS true\n"
	"\n"
	"USER_GRAPH 1 Example 22, CO2 solubility at high pressures, 25 - 100C\n"
	" -plot_tsv_file co2.tsv\n"
	" -axis_titles \"Pressure, in atmospheres\" \\\n"
	"              \"CO2 concentration, in moles per kilogram water\"\n"
	" -axis_scale x_axis 0 500\n"
	" -axis_scale y_axis 0 2\n"
	" -connect_simulations false\n"
	" 10 graph_x PR_P(\"CO2(g)\")\n"
	" 20 graph_y TOT(\"C(4)\")\n"
	" -end\n"
	"USER_GRAPH 2 Example 22, P-Vm of CO2 gas, 25 - 100C\n"
	" -headings 25C\n"
	" -axis_titles \"Molar volume of CO2 gas, in liters per mole\" \\\n"
	"              \"CO2 pressure, in atmospheres\"\n"
	" -axis_scale x_axis 0 1\n"
	" -axis_scale y_axis 0 500\n"
	" -connect_simulations false\n"
	" 10 plot_xy gas_vm, gas_p symbol = None\n"
	" -end\n"
	"END\n"
	"\n"
	"USE solution 1\n"
	"USE gas_phase 1\n"
	"USE reaction 1\n"
	"REACTION_TEMPERATURE 2\n"
	" 50\n"
	"USER_GRAPH 2\n"
	" -headings 50C\n"
	"END\n"
	"\n"
	"USE solution 1\n"
	"USE gas_phase 1\n"
	"USE reaction 1\n"
	"REACTION_TEMPERATURE 3\n"
	" 75\n"
	"USER_GRAPH 2\n"
	" -headings 75C\n"
	"END\n"
	"\n"
	"USE solution 1\n"
	"USE gas_phase 1\n"
	"USE reaction 1\n"
	"REACTION_TEMPERATURE 4\n"
	" 100\n"
	"USER_GRAPH 2\n"
	" -headings 100C\n"
	"END\n"
	,
	"Example 22—Modeling Gas Solubilities: CO2 at High Pressures\n"
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

	// get cursor point 
	DWORD dw = ::GetMessagePos();
	POINT pt;
	pt.x = GET_X_LPARAM(dw);
	pt.y = GET_Y_LPARAM(dw);
	this->ScreenToClient(&pt);

	// only paste if dblclick on item (label or bitmap)
	TVHITTESTINFO tvHitTestInfo;
	tvHitTestInfo.pt = pt;
	CTreeCtrlNode item = this->HitTest(&tvHitTestInfo);
	if ((TVHT_ONITEM & tvHitTestInfo.flags) && (HTREEITEM)item && item.GetData())
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
	if (this->m_pWndDesc)
	{
		CTreeCtrlNode item(pNMTreeView->itemNew.hItem, this);
		if (((HTREEITEM)item ) && item.GetData())
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
		if (kd >= KD_PHREAK)
		{
			chem.AddTail(_T("KINETICS")).SetData((DWORD)&CTreeCtrlPfw::KINETICS);
				chem.GetLastChild().AddTail(_T("-cvode")).SetData((DWORD)&CTreeCtrlPfw::KINETICS_cvode);
				chem.GetLastChild().AddTail(_T("-formula")).SetData((DWORD)&CTreeCtrlPfw::KINETICS_formula);
				chem.GetLastChild().AddTail(_T("-m")).SetData((DWORD)&CTreeCtrlPfw::KINETICS_m);
				chem.GetLastChild().AddTail(_T("-m0")).SetData((DWORD)&CTreeCtrlPfw::KINETICS_m0);
				chem.GetLastChild().AddTail(_T("-parms")).SetData((DWORD)&CTreeCtrlPfw::KINETICS_parms);
				chem.GetLastChild().AddTail(_T("-tol")).SetData((DWORD)&CTreeCtrlPfw::KINETICS_tol);
				chem.GetLastChild().AddTail(_T("-steps")).SetData((DWORD)&CTreeCtrlPfw::KINETICS_steps);
				chem.GetLastChild().AddTail(_T("-step_divide")).SetData((DWORD)&CTreeCtrlPfw::KINETICS_step_divide);
				chem.GetLastChild().AddTail(_T("-runge_kutta")).SetData((DWORD)&CTreeCtrlPfw::KINETICS_runge_kutta);
				chem.GetLastChild().AddTail(_T("-bad_step_max")).SetData((DWORD)&CTreeCtrlPfw::KINETICS_bad_step_max);
				chem.GetLastChild().AddTail(_T("-cvode_steps")).SetData((DWORD)&CTreeCtrlPfw::KINETICS_cvode_steps);
				chem.GetLastChild().AddTail(_T("-cvode_order")).SetData((DWORD)&CTreeCtrlPfw::KINETICS_cvode_order);
		}
		chem.AddTail(_T("REACTION")).SetData((DWORD)&CTreeCtrlPfw::REACTION);
		chem.AddTail(_T("REACTION_TEMPERATURE")).SetData((DWORD)&CTreeCtrlPfw::REACTION_TEMPERATURE);
		if (kd >= KD_PHREAK)
		{
			chem.AddTail(_T("SOLID_SOLUTIONS")).SetData((DWORD)&CTreeCtrlPfw::SOLID_SOLUTIONS);
				chem.GetLastChild().AddTail(_T("-comp")).SetData((DWORD)&CTreeCtrlPfw::SOLID_SOLUTIONS_comp);
				chem.GetLastChild().AddTail(_T("-comp1")).SetData((DWORD)&CTreeCtrlPfw::SOLID_SOLUTIONS_comp1);
				chem.GetLastChild().AddTail(_T("-comp2")).SetData((DWORD)&CTreeCtrlPfw::SOLID_SOLUTIONS_comp2);
				chem.GetLastChild().AddTail(_T("-temp")).SetData((DWORD)&CTreeCtrlPfw::SOLID_SOLUTIONS_temp);
				chem.GetLastChild().AddTail(_T("-tempk")).SetData((DWORD)&CTreeCtrlPfw::SOLID_SOLUTIONS_tempk);
				chem.GetLastChild().AddTail(_T("-Gugg_nondim")).SetData((DWORD)&CTreeCtrlPfw::SOLID_SOLUTIONS_Gugg_nondim);
				chem.GetLastChild().AddTail(_T("-Gugg_kJ")).SetData((DWORD)&CTreeCtrlPfw::SOLID_SOLUTIONS_Gugg_kJ);
				chem.GetLastChild().AddTail(_T("-activity_coefficients")).SetData((DWORD)&CTreeCtrlPfw::SOLID_SOLUTIONS_activity_coefficients);
				chem.GetLastChild().AddTail(_T("-distribution_coefficients")).SetData((DWORD)&CTreeCtrlPfw::SOLID_SOLUTIONS_distribution_coefficients);
				chem.GetLastChild().AddTail(_T("-miscibility_gap")).SetData((DWORD)&CTreeCtrlPfw::SOLID_SOLUTIONS_miscibility_gap);
				chem.GetLastChild().AddTail(_T("-spinodal_gap")).SetData((DWORD)&CTreeCtrlPfw::SOLID_SOLUTIONS_spinodal_gap);
				chem.GetLastChild().AddTail(_T("-critical_point")).SetData((DWORD)&CTreeCtrlPfw::SOLID_SOLUTIONS_critical_point);
				chem.GetLastChild().AddTail(_T("-alyotropic_point")).SetData((DWORD)&CTreeCtrlPfw::SOLID_SOLUTIONS_alyotropic_point);
				chem.GetLastChild().AddTail(_T("-Thompson")).SetData((DWORD)&CTreeCtrlPfw::SOLID_SOLUTIONS_Thompson);
				chem.GetLastChild().AddTail(_T("-Margules")).SetData((DWORD)&CTreeCtrlPfw::SOLID_SOLUTIONS_Margules);
		}
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
		//{{NEW BASIC HERE}}
		CTreeCtrlNode pbasic = this->InsertItem(_T("PHREEQC BASIC statements"));
		pbasic.AddTail(_T("ACT(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::ACT);
		pbasic.AddTail(_T("ALK")).SetData((DWORD)&CTreeCtrlPfw::ALK);
		pbasic.AddTail(_T("APHI(\"...\")")).SetData((DWORD)&CTreeCtrlPfw::APHI);
		pbasic.AddTail(_T("CALC_VALUE(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::CALC_VALUE);
		pbasic.AddTail(_T("CELL_NO")).SetData((DWORD)&CTreeCtrlPfw::CELL_NO);
		pbasic.AddTail(_T("CHANGE_POR(.., ..)")).SetData((DWORD)&CTreeCtrlPfw::CHANGE_POR);
		pbasic.AddTail(_T("CHANGE_SURF(\"..\", .., \"..\", .., ..)")).SetData((DWORD)&CTreeCtrlPfw::CHANGE_SURF);
		pbasic.AddTail(_T("CHARGE_BALANCE")).SetData((DWORD)&CTreeCtrlPfw::CHARGE_BALANCE);
		pbasic.AddTail(_T("DESCRIPTION")).SetData((DWORD)&CTreeCtrlPfw::DESCRIPTION);
		pbasic.AddTail(_T("DH_A")).SetData((DWORD)&CTreeCtrlPfw::DH_A);
		pbasic.AddTail(_T("DH_Av")).SetData((DWORD)&CTreeCtrlPfw::DH_Av);
		pbasic.AddTail(_T("DH_B")).SetData((DWORD)&CTreeCtrlPfw::DH_B);
		pbasic.AddTail(_T("DIFF_C")).SetData((DWORD)&CTreeCtrlPfw::DIFF_C);
		pbasic.AddTail(_T("DIST")).SetData((DWORD)&CTreeCtrlPfw::DIST);
		pbasic.AddTail(_T("EDL(\"..\", \"..\")")).SetData((DWORD)&CTreeCtrlPfw::EDL);
		pbasic.AddTail(_T("EDL_SPECIES(\"..\", \"..\")")).SetData((DWORD)&CTreeCtrlPfw::EDL_SPECIES);
		pbasic.AddTail(_T("EPS_R")).SetData((DWORD)&CTreeCtrlPfw::EPS_R);
		pbasic.AddTail(_T("EQ_FRAC(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::EQ_FRAC);
		pbasic.AddTail(_T("EQUI(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::EQUI);
		pbasic.AddTail(_T("EQUI_DELTA(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::EQUI_DELTA);
		pbasic.AddTail(_T("EXISTS(..[, .., ..])")).SetData((DWORD)&CTreeCtrlPfw::EXISTS);
		pbasic.AddTail(_T("GAMMA(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::GAMMA);
		pbasic.AddTail(_T("GAS(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::GAS);
		pbasic.AddTail(_T("GAS_P")).SetData((DWORD)&CTreeCtrlPfw::GAS_P);
		pbasic.AddTail(_T("GAS_VM")).SetData((DWORD)&CTreeCtrlPfw::GAS_VM);
		pbasic.AddTail(_T("GET(..[, .., ..])")).SetData((DWORD)&CTreeCtrlPfw::GET);
		pbasic.AddTail(_T("GET_POR(..)")).SetData((DWORD)&CTreeCtrlPfw::GET_POR);
		pbasic.AddTail(_T("GFW")).SetData((DWORD)&CTreeCtrlPfw::GFW);
		pbasic.AddTail(_T("GRAPH_SY")).SetData((DWORD)&CTreeCtrlPfw::GRAPH_SY);
		pbasic.AddTail(_T("GRAPH_X")).SetData((DWORD)&CTreeCtrlPfw::GRAPH_X);
		pbasic.AddTail(_T("GRAPH_Y")).SetData((DWORD)&CTreeCtrlPfw::GRAPH_Y);
		pbasic.AddTail(_T("ISO(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::ISO);
		pbasic.AddTail(_T("ISO_UNITS(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::ISO_UNITS);
		pbasic.AddTail(_T("KAPPA")).SetData((DWORD)&CTreeCtrlPfw::KAPPA);
		pbasic.AddTail(_T("KIN(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::KIN);
		pbasic.AddTail(_T("KINETICS_FORMULA(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::KINETICS_FORMULA);
		pbasic.AddTail(_T("KIN_DELTA(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::KIN_DELTA);
		pbasic.AddTail(_T("KIN_TIME")).SetData((DWORD)&CTreeCtrlPfw::KIN_TIME);
		pbasic.AddTail(_T("LA(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::LA);
		pbasic.AddTail(_T("LG(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::LG);
		pbasic.AddTail(_T("LIST_S_S(\"..\", .., .., ..)")).SetData((DWORD)&CTreeCtrlPfw::LIST_S_S);
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
		pbasic.AddTail(_T("PHASE_FORMULA(\"...\")")).SetData((DWORD)&CTreeCtrlPfw::PHASE_FORMULA);
		pbasic.AddTail(_T("PHASE_VM(\"...\")")).SetData((DWORD)&CTreeCtrlPfw::PHASE_VM);
		pbasic.AddTail(_T("PLOT_XY ..., ...")).SetData((DWORD)&CTreeCtrlPfw::PLOT_XY);
		pbasic.AddTail(_T("PR_P(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::PR_P);
		pbasic.AddTail(_T("PR_PHI(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::PR_PHI);
		pbasic.AddTail(_T("PRESSURE")).SetData((DWORD)&CTreeCtrlPfw::PRESSURE_BASIC);
		pbasic.AddTail(_T("PRINT")).SetData((DWORD)&CTreeCtrlPfw::BASIC_PRINT);
		pbasic.AddTail(_T("PUNCH")).SetData((DWORD)&CTreeCtrlPfw::PUNCH);
		pbasic.AddTail(_T("PUT(x, ..[, .., ..])")).SetData((DWORD)&CTreeCtrlPfw::PUT);
		pbasic.AddTail(_T("QBRN")).SetData((DWORD)&CTreeCtrlPfw::QBRN);
#if defined PHREEQ98
		pbasic.AddTail(_T("RHO_m")).SetData((DWORD)&CTreeCtrlPfw::RHO_m);
		pbasic.AddTail(_T("RHO_v")).SetData((DWORD)&CTreeCtrlPfw::RHO_v);
#else
		pbasic.AddTail(_T("RHO")).SetData((DWORD)&CTreeCtrlPfw::RHO);
		pbasic.AddTail(_T("RHO_0")).SetData((DWORD)&CTreeCtrlPfw::RHO_0);
#endif
		pbasic.AddTail(_T("RXN")).SetData((DWORD)&CTreeCtrlPfw::RXN);
		pbasic.AddTail(_T("S_S(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::S_S);
		pbasic.AddTail(_T("SAVE ..")).SetData((DWORD)&CTreeCtrlPfw::BASIC_SAVE);
		pbasic.AddTail(_T("SC")).SetData((DWORD)&CTreeCtrlPfw::SC);
		pbasic.AddTail(_T("SI(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::SI);
		pbasic.AddTail(_T("SIM_NO")).SetData((DWORD)&CTreeCtrlPfw::SIM_NO);
		pbasic.AddTail(_T("SIM_TIME")).SetData((DWORD)&CTreeCtrlPfw::SIM_TIME);
		pbasic.AddTail(_T("SOLN_VOL")).SetData((DWORD)&CTreeCtrlPfw::SOLN_VOL);
		pbasic.AddTail(_T("SPECIES_FORMULA(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::SPECIES_FORMULA);
		pbasic.AddTail(_T("SR(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::SR);
		pbasic.AddTail(_T("STEP_NO")).SetData((DWORD)&CTreeCtrlPfw::STEP_NO);
		pbasic.AddTail(_T("STR_E$(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::STR_E);
		pbasic.AddTail(_T("STR_F$(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::STR_F);
		pbasic.AddTail(_T("SUM_GAS(\"{..}\",\"..\")")).SetData((DWORD)&CTreeCtrlPfw::SUM_GAS);
		pbasic.AddTail(_T("SUM_S_S(\"..\",\"..\")")).SetData((DWORD)&CTreeCtrlPfw::SUM_S_S);
		pbasic.AddTail(_T("SUM_SPECIES(\"{..}\",\"..\")")).SetData((DWORD)&CTreeCtrlPfw::SUM_SPECIES);
		pbasic.AddTail(_T("SURF(\"..\", \"..\") ")).SetData((DWORD)&CTreeCtrlPfw::SURF);
		pbasic.AddTail(_T("SYS(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::SYS);
		pbasic.AddTail(_T("TC")).SetData((DWORD)&CTreeCtrlPfw::TC);
		pbasic.AddTail(_T("TIME")).SetData((DWORD)&CTreeCtrlPfw::TIME);
		pbasic.AddTail(_T("TK")).SetData((DWORD)&CTreeCtrlPfw::TK);
		pbasic.AddTail(_T("TOT(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::TOT);
		pbasic.AddTail(_T("TOTAL_TIME")).SetData((DWORD)&CTreeCtrlPfw::TOTAL_TIME);
		pbasic.AddTail(_T("TOTMOLE(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::TOTMOLE);
		pbasic.AddTail(_T("VM(\"..\")")).SetData((DWORD)&CTreeCtrlPfw::VM);

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

	CTreeCtrlNode ie = this->InsertItem(_T("PHREEQC examples"));
	ie.AddTail(_T("ex1")).SetData((DWORD)&CTreeCtrlPfw::ex1);
	ie.AddTail(_T("ex2")).SetData((DWORD)&CTreeCtrlPfw::ex2);
	ie.AddTail(_T("ex2b")).SetData((DWORD)&CTreeCtrlPfw::ex2b);
	ie.AddTail(_T("ex3")).SetData((DWORD)&CTreeCtrlPfw::ex3);
	ie.AddTail(_T("ex4")).SetData((DWORD)&CTreeCtrlPfw::ex4);
	ie.AddTail(_T("ex5")).SetData((DWORD)&CTreeCtrlPfw::ex5);
	ie.AddTail(_T("ex6")).SetData((DWORD)&CTreeCtrlPfw::ex6);
	ie.AddTail(_T("ex7")).SetData((DWORD)&CTreeCtrlPfw::ex7);
	ie.AddTail(_T("ex8")).SetData((DWORD)&CTreeCtrlPfw::ex8);
	ie.AddTail(_T("ex9")).SetData((DWORD)&CTreeCtrlPfw::ex9);
	ie.AddTail(_T("ex10")).SetData((DWORD)&CTreeCtrlPfw::ex10);
	ie.AddTail(_T("ex11")).SetData((DWORD)&CTreeCtrlPfw::ex11);
	ie.AddTail(_T("ex12")).SetData((DWORD)&CTreeCtrlPfw::ex12);
	ie.AddTail(_T("ex13a")).SetData((DWORD)&CTreeCtrlPfw::ex13a);
	ie.AddTail(_T("ex13b")).SetData((DWORD)&CTreeCtrlPfw::ex13b);
	ie.AddTail(_T("ex13c")).SetData((DWORD)&CTreeCtrlPfw::ex13c);
	ie.AddTail(_T("ex14")).SetData((DWORD)&CTreeCtrlPfw::ex14);
	ie.AddTail(_T("ex15")).SetData((DWORD)&CTreeCtrlPfw::ex15);
	ie.AddTail(_T("ex16")).SetData((DWORD)&CTreeCtrlPfw::ex16);
	ie.AddTail(_T("ex17")).SetData((DWORD)&CTreeCtrlPfw::ex17);
	ie.AddTail(_T("ex18")).SetData((DWORD)&CTreeCtrlPfw::ex18);
	ie.AddTail(_T("ex19")).SetData((DWORD)&CTreeCtrlPfw::ex19);
	ie.AddTail(_T("ex20a")).SetData((DWORD)&CTreeCtrlPfw::ex20a);
	ie.AddTail(_T("ex20b")).SetData((DWORD)&CTreeCtrlPfw::ex20b);
	ie.AddTail(_T("ex21")).SetData((DWORD)&CTreeCtrlPfw::ex21);
	ie.AddTail(_T("ex22")).SetData((DWORD)&CTreeCtrlPfw::ex22);
}