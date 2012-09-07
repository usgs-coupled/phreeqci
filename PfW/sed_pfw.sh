#!/bin/sed -f
#
# fix double quotes
#
s/^\t\"/\t@/g
s/\"$/@/g
s/\"/\\"/g
s/@/\"/g

#
# fix functions with parens -- ie CHANGE_POR(.., ..) => CHANGE_POR
#
s/std::pair<CString, CString> CTreeCtrlPfw::\(.*\)(.*)(/std::pair<CString, CString> CTreeCtrlPfw::\1(/g

#
# PHREEQC PHREEQC Keywords/Start
#
s^CTreeCtrlPfw::PHREEQC Keywords\\Start\\^CTreeCtrlPfw::^g
s^CTreeCtrlPfw::SOLUTION\\^CTreeCtrlPfw::SOLUTION_^g
##s^CTreeCtrlPfw::SOLUTION_-^CTreeCtrlPfw::SOLUTION_^g
s^CTreeCtrlPfw::SOLUTION_SPREAD\\^CTreeCtrlPfw::SOLUTION_SPREAD_^g
##s^CTreeCtrlPfw::SOLUTION_SPREAD_-^CTreeCtrlPfw::SOLUTION_SPREAD_^g
# PHREEQC Keywords/Chemical reaction
s^CTreeCtrlPfw::PHREEQC Keywords\\Chemical reaction\\^CTreeCtrlPfw::^g
s^CTreeCtrlPfw::EQUILIBRIUM_PHASES\\-^CTreeCtrlPfw::EQUILIBRIUM_PHASES_^g
##s^CTreeCtrlPfw::EXCHANGE\\-^CTreeCtrlPfw::EXCHANGE_^g
##s^CTreeCtrlPfw::GAS_PHASE\\-^CTreeCtrlPfw::GAS_PHASE_^g
##s^CTreeCtrlPfw::INVERSE_MODELING\\-^CTreeCtrlPfw::INVERSE_MODELING_^g
##s^CTreeCtrlPfw::KINETICS\\-^CTreeCtrlPfw::KINETICS_^g
##s^CTreeCtrlPfw::SOLID_SOLUTIONS\\-^CTreeCtrlPfw::SOLID_SOLUTIONS_^g
##s^CTreeCtrlPfw::SURFACE\\-^CTreeCtrlPfw::SURFACE_^g
##{{{{{{
s^CTreeCtrlPfw::EXCHANGE\\^CTreeCtrlPfw::EXCHANGE_^g
s^CTreeCtrlPfw::GAS_PHASE\\^CTreeCtrlPfw::GAS_PHASE_^g
s^CTreeCtrlPfw::INCREMENTAL_REACTIONS\\^CTreeCtrlPfw::INCREMENTAL_REACTIONS_^g
s^CTreeCtrlPfw::INVERSE_MODELING\\^CTreeCtrlPfw::INVERSE_MODELING_^g
s^CTreeCtrlPfw::KINETICS\\^CTreeCtrlPfw::KINETICS_^g
s^CTreeCtrlPfw::REACTION\\^CTreeCtrlPfw::REACTION_^g
s^CTreeCtrlPfw::REACTION_TEMPERATURE\\^CTreeCtrlPfw::REACTION_TEMPERATURE_^g
s^CTreeCtrlPfw::SOLID_SOLUTIONS\\^CTreeCtrlPfw::SOLID_SOLUTIONS_^g
s^CTreeCtrlPfw::SURFACE\\^CTreeCtrlPfw::SURFACE_^g
## special cas
s^CTreeCtrlPfw::SURFACE_sites_units density^CTreeCtrlPfw::SURFACE_sites_units_density^g
##}}}}}}
# PHREEQC Keywords/Physical action
s^CTreeCtrlPfw::PHREEQC Keywords\\Physical action\\^CTreeCtrlPfw::^g
##s^CTreeCtrlPfw::ADVECTION\\-^CTreeCtrlPfw::ADVECTION_^g
#
# PHREEQC Keywords/Output
#
s^CTreeCtrlPfw::PHREEQC Keywords\\Output\\^CTreeCtrlPfw::^g
#
# PHREEQC Keywords/Database
#
s^CTreeCtrlPfw::PHREEQC Keywords\\Database\\^CTreeCtrlPfw::^g
s^CTreeCtrlPfw::EXCHANGE_SPECIES\\^CTreeCtrlPfw::EXCHANGE_SPECIES_^g
s^CTreeCtrlPfw::PHASES\\^CTreeCtrlPfw::PHASES_^g
s^CTreeCtrlPfw::SOLUTION_SPECIES\\^CTreeCtrlPfw::SOLUTION_SPECIES_^g
s^CTreeCtrlPfw::SURFACE_SPECIES\\^CTreeCtrlPfw::SURFACE_SPECIES_^g
#
# PHREEQC Keywords/Miscellaneous
#
s^CTreeCtrlPfw::PHREEQC Keywords\\Miscellaneous\\^CTreeCtrlPfw::^g
#
# PHREEQC BASIC statements
#
s^CTreeCtrlPfw::PHREEQC BASIC statements\\^CTreeCtrlPfw::^g
#
# GENERAL BASIC statements
#
s^CTreeCtrlPfw::GENERAL BASIC statements\\^CTreeCtrlPfw::^g


#
# last steps
#
s^CTreeCtrlPfw::\(.*\)_-^CTreeCtrlPfw::\1_^g
s^CTreeCtrlPfw::\(.*\)\\-^CTreeCtrlPfw::\1_^g








