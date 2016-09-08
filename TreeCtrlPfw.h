#pragma once


// CTreeCtrlPfw
#include "Tree.h"
#include "TreeCtrlKeyword.h"


#ifndef baseCTreeCtrlPfw
#define baseCTreeCtrlPfw CTreeCtrlEx
#endif

#undef REACTION
#undef SURFACE
#undef ADVECTION
#undef TRANSPORT
#undef ALK
#undef MU
#undef SURF

class CTreeCtrlPfw : public baseCTreeCtrlPfw
{
	DECLARE_DYNAMIC(CTreeCtrlPfw)

public:
	CTreeCtrlPfw();
	virtual ~CTreeCtrlPfw();

	enum KeywordDetail 
	{
		KD_NOVICE       = 0,
		KD_INTERMEDIATE = 1,
		KD_PHREAK       = 2
	};

	// Start
	static std::pair<CString, CString> CTreeCtrlPfw::COPY;
	static std::pair<CString, CString> CTreeCtrlPfw::DATABASE;
	static std::pair<CString, CString> CTreeCtrlPfw::END;
	static std::pair<CString, CString> CTreeCtrlPfw::SAVE;
	static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_temp;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_pH;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_pe;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_redox;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_units;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_density;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_isotope;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_water;
	static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_temp;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_pH;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_pe;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_redox;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_units;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_density;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_isotope;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_isotope_uncertainty;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_water;
 
	static std::pair<CString, CString> CTreeCtrlPfw::TITLE;
	static std::pair<CString, CString> CTreeCtrlPfw::USE;

	// Chemical reaction
	static std::pair<CString, CString> CTreeCtrlPfw::EQUILIBRIUM_PHASES;
		static std::pair<CString, CString> CTreeCtrlPfw::EQUILIBRIUM_PHASES_force_equality;
	static std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE;
		static std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_equilibrate;
		static std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_pitzer_exchange_gammas;
	static std::pair<CString, CString> CTreeCtrlPfw::GAS_PHASE;
		static std::pair<CString, CString> CTreeCtrlPfw::GAS_PHASE_fixed_pressure;
		static std::pair<CString, CString> CTreeCtrlPfw::GAS_PHASE_pressure;
		static std::pair<CString, CString> CTreeCtrlPfw::GAS_PHASE_volume;
		static std::pair<CString, CString> CTreeCtrlPfw::GAS_PHASE_temperature;
		static std::pair<CString, CString> CTreeCtrlPfw::GAS_PHASE_fixed_volume;
		static std::pair<CString, CString> CTreeCtrlPfw::GAS_PHASE_equilibrate;
	static std::pair<CString, CString> CTreeCtrlPfw::INCREMENTAL_REACTIONS;
	static std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING;
		static std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_solutions;
		static std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_uncertainty;
		static std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_phases;
		static std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_balances;
		static std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_isotopes;
		static std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_range;
		static std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_minimal;
		static std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_tolerance;
		static std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_force_solutions;
		static std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_uncertainty_water;
		static std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_mineral_water;
		static std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_multiple_precision;
		static std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_mp_tolerance;
		static std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_censor_mp;
		static std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_lon_netpath;
		static std::pair<CString, CString> CTreeCtrlPfw::INVERSE_MODELING_pat_netpath;
	static std::pair<CString, CString> CTreeCtrlPfw::KINETICS;
		static std::pair<CString, CString> CTreeCtrlPfw::KINETICS_cvode;
		static std::pair<CString, CString> CTreeCtrlPfw::KINETICS_formula;
		static std::pair<CString, CString> CTreeCtrlPfw::KINETICS_m;
		static std::pair<CString, CString> CTreeCtrlPfw::KINETICS_m0;
		static std::pair<CString, CString> CTreeCtrlPfw::KINETICS_parms;
		static std::pair<CString, CString> CTreeCtrlPfw::KINETICS_tol;
		static std::pair<CString, CString> CTreeCtrlPfw::KINETICS_steps;
		static std::pair<CString, CString> CTreeCtrlPfw::KINETICS_step_divide;
		static std::pair<CString, CString> CTreeCtrlPfw::KINETICS_runge_kutta;
		static std::pair<CString, CString> CTreeCtrlPfw::KINETICS_bad_step_max;
		static std::pair<CString, CString> CTreeCtrlPfw::KINETICS_cvode_steps;
		static std::pair<CString, CString> CTreeCtrlPfw::KINETICS_cvode_order;
 	static std::pair<CString, CString> CTreeCtrlPfw::REACTION;
	static std::pair<CString, CString> CTreeCtrlPfw::REACTION_TEMPERATURE;
	static std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_comp;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_comp1;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_comp2;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_temp;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_tempk;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_Gugg_nondim;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_Gugg_kJ;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_activity_coefficients;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_distribution_coefficients;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_miscibility_gap;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_spinodal_gap;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_critical_point;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_alyotropic_point;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_Thompson;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLID_SOLUTIONS_Margules;
	static std::pair<CString, CString> CTreeCtrlPfw::SURFACE;
		static std::pair<CString, CString> CTreeCtrlPfw::SURFACE_equilibrate;
		static std::pair<CString, CString> CTreeCtrlPfw::SURFACE_capacitance;
		static std::pair<CString, CString> CTreeCtrlPfw::SURFACE_cd_music;
		static std::pair<CString, CString> CTreeCtrlPfw::SURFACE_diffuse_layer;
		static std::pair<CString, CString> CTreeCtrlPfw::SURFACE_donnan;
		static std::pair<CString, CString> CTreeCtrlPfw::SURFACE_only_counter_ions;
		static std::pair<CString, CString> CTreeCtrlPfw::SURFACE_no_edl;
		static std::pair<CString, CString> CTreeCtrlPfw::SURFACE_sites_units;

	// Physical action
	static std::pair<CString, CString> CTreeCtrlPfw::ADVECTION;
		static std::pair<CString, CString> CTreeCtrlPfw::ADVECTION_cells;
		static std::pair<CString, CString> CTreeCtrlPfw::ADVECTION_shifts;
		static std::pair<CString, CString> CTreeCtrlPfw::ADVECTION_time_step;
		static std::pair<CString, CString> CTreeCtrlPfw::ADVECTION_initial_time;
		static std::pair<CString, CString> CTreeCtrlPfw::ADVECTION_print_cells;
		static std::pair<CString, CString> CTreeCtrlPfw::ADVECTION_print_frequency;
		static std::pair<CString, CString> CTreeCtrlPfw::ADVECTION_punch_cells;
		static std::pair<CString, CString> CTreeCtrlPfw::ADVECTION_punch_frequency;
		static std::pair<CString, CString> CTreeCtrlPfw::ADVECTION_warnings;
	static std::pair<CString, CString> CTreeCtrlPfw::MIX;
	static std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT;
		static std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_cells;
		static std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_shifts;
		static std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_time_step;
		static std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_flow_direction;
		static std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_boundary_conditions;
		static std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_lengths;
		static std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_dispersivities;
		static std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_correct_disp;
		static std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_diffusion_coefficient;
		static std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_stagnant;
		static std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_thermal_diffusion;
		static std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_initial_time;
		static std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_multi_d;
		static std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_interlayer_d;
		static std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_print_cells;
		static std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_print_frequency;
		static std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_punch_cells;
		static std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_punch_frequency;
		static std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_warnings;
		static std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_dump;
		static std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_dump_frequency;
		static std::pair<CString, CString> CTreeCtrlPfw::TRANSPORT_dump_restart;

	// Output
	static std::pair<CString, CString> CTreeCtrlPfw::PRINT;
		static std::pair<CString, CString> CTreeCtrlPfw::PRINT_alkalinity;
		static std::pair<CString, CString> CTreeCtrlPfw::PRINT_echo_input;
		static std::pair<CString, CString> CTreeCtrlPfw::PRINT_Eh;
		static std::pair<CString, CString> CTreeCtrlPfw::PRINT_equilibrium_phases;
		static std::pair<CString, CString> CTreeCtrlPfw::PRINT_exchange;
		static std::pair<CString, CString> CTreeCtrlPfw::PRINT_gas_phase;
		static std::pair<CString, CString> CTreeCtrlPfw::PRINT_headings;
		static std::pair<CString, CString> CTreeCtrlPfw::PRINT_inverse_modeling;
		static std::pair<CString, CString> CTreeCtrlPfw::PRINT_kinetics;
		static std::pair<CString, CString> CTreeCtrlPfw::PRINT_other;
		static std::pair<CString, CString> CTreeCtrlPfw::PRINT_reset;
		static std::pair<CString, CString> CTreeCtrlPfw::PRINT_saturation_indices;
		static std::pair<CString, CString> CTreeCtrlPfw::PRINT_solid_solutions;
		static std::pair<CString, CString> CTreeCtrlPfw::PRINT_species;
		static std::pair<CString, CString> CTreeCtrlPfw::PRINT_surface;
		static std::pair<CString, CString> CTreeCtrlPfw::PRINT_totals;
		static std::pair<CString, CString> CTreeCtrlPfw::PRINT_user_print;
		static std::pair<CString, CString> CTreeCtrlPfw::PRINT_selected_output;
		static std::pair<CString, CString> CTreeCtrlPfw::PRINT_status;
		static std::pair<CString, CString> CTreeCtrlPfw::PRINT_warnings;
		static std::pair<CString, CString> CTreeCtrlPfw::PRINT_initial_isotopes;
		static std::pair<CString, CString> CTreeCtrlPfw::PRINT_isotope_ratios;
		static std::pair<CString, CString> CTreeCtrlPfw::PRINT_isotope_alphas;
		static std::pair<CString, CString> CTreeCtrlPfw::PRINT_censor_species;
	static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_file;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_selected_out;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_user_punch;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_high_precision;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_reset;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_simulation;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_state;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_solution;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_distance;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_time;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_step;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_pH;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_pe;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_reaction;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_temperature;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_alkalinity;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_ionic_strength;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_water;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_charge_balance;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_percent_error;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_totals;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_molalities;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_activities;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_equilibrium_phases;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_saturation_indices;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_gases;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_kinetic_reactants;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_solid_solutions;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_inverse_modeling;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_calculate_values;
		static std::pair<CString, CString> CTreeCtrlPfw::SELECTED_OUTPUT_isotopes;
	static std::pair<CString, CString> CTreeCtrlPfw::USER_GRAPH;
		static std::pair<CString, CString> CTreeCtrlPfw::USER_GRAPH_headings;
		static std::pair<CString, CString> CTreeCtrlPfw::USER_GRAPH_axis_scale;
		static std::pair<CString, CString> CTreeCtrlPfw::USER_GRAPH_axis_titles;
		static std::pair<CString, CString> CTreeCtrlPfw::USER_GRAPH_chart_title;
		static std::pair<CString, CString> CTreeCtrlPfw::USER_GRAPH_connect_simulations;
		static std::pair<CString, CString> CTreeCtrlPfw::USER_GRAPH_grid_offset;
		static std::pair<CString, CString> CTreeCtrlPfw::USER_GRAPH_initial_solutions;
		static std::pair<CString, CString> CTreeCtrlPfw::USER_GRAPH_plot_concentration_vs;
		static std::pair<CString, CString> CTreeCtrlPfw::USER_GRAPH_plot_csv_file;
		static std::pair<CString, CString> CTreeCtrlPfw::USER_GRAPH_start;
		static std::pair<CString, CString> CTreeCtrlPfw::USER_GRAPH_end;
	static std::pair<CString, CString> CTreeCtrlPfw::USER_PRINT;
		static std::pair<CString, CString> CTreeCtrlPfw::USER_PRINT_start;
		static std::pair<CString, CString> CTreeCtrlPfw::USER_PRINT_end;
	static std::pair<CString, CString> CTreeCtrlPfw::USER_PUNCH;
		static std::pair<CString, CString> CTreeCtrlPfw::USER_PUNCH_headings;
		static std::pair<CString, CString> CTreeCtrlPfw::USER_PUNCH_start;
		static std::pair<CString, CString> CTreeCtrlPfw::USER_PUNCH_end;
	
	// Database
	static std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_MASTER_SPECIES;
	static std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_SPECIES;
		static std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_SPECIES_log_k;
		static std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_SPECIES_delta_h;
		static std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_SPECIES_analytical_expression;
		static std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_SPECIES_gamma;
		static std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_SPECIES_davies;
		static std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_SPECIES_no_check;
		static std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_SPECIES_mole_balance;
		static std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_SPECIES_add_constant;
		static std::pair<CString, CString> CTreeCtrlPfw::EXCHANGE_SPECIES_add_logk;
	static std::pair<CString, CString> CTreeCtrlPfw::PHASES;
		static std::pair<CString, CString> CTreeCtrlPfw::PHASES_log_k;
		static std::pair<CString, CString> CTreeCtrlPfw::PHASES_delta_h;
		static std::pair<CString, CString> CTreeCtrlPfw::PHASES_analytical_expression;
		static std::pair<CString, CString> CTreeCtrlPfw::PHASES_no_check;
		static std::pair<CString, CString> CTreeCtrlPfw::PHASES_add_constant;
		static std::pair<CString, CString> CTreeCtrlPfw::PHASES_add_logk;
	static std::pair<CString, CString> CTreeCtrlPfw::PITZER;
		static std::pair<CString, CString> CTreeCtrlPfw::PITZER_ALPHAS;
		static std::pair<CString, CString> CTreeCtrlPfw::PITZER_B0;
		static std::pair<CString, CString> CTreeCtrlPfw::PITZER_B1;
		static std::pair<CString, CString> CTreeCtrlPfw::PITZER_B2;
		static std::pair<CString, CString> CTreeCtrlPfw::PITZER_C0;
		static std::pair<CString, CString> CTreeCtrlPfw::PITZER_LAMDA;
		static std::pair<CString, CString> CTreeCtrlPfw::PITZER_MacInnes;
		static std::pair<CString, CString> CTreeCtrlPfw::PITZER_PSI;
		static std::pair<CString, CString> CTreeCtrlPfw::PITZER_redox;
		static std::pair<CString, CString> CTreeCtrlPfw::PITZER_THETA;
		static std::pair<CString, CString> CTreeCtrlPfw::PITZER_use_etheta;
		static std::pair<CString, CString> CTreeCtrlPfw::PITZER_ZETA;
	static std::pair<CString, CString> CTreeCtrlPfw::SIT;
	static std::pair<CString, CString> CTreeCtrlPfw::RATES;
		static std::pair<CString, CString> CTreeCtrlPfw::RATES_start;
		static std::pair<CString, CString> CTreeCtrlPfw::RATES_end;
	static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_MASTER_SPECIES;
	static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_log_k;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_delta_h;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_analytical_expression;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_gamma;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_no_check;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_mole_balance;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_llnl_gamma;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_co2_llnl_gamma;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_dw;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_erm_ddl;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_Millero;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_activity_water;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_add_constant;
		static std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPECIES_add_logk;
	static std::pair<CString, CString> CTreeCtrlPfw::SURFACE_MASTER_SPECIES;
	static std::pair<CString, CString> CTreeCtrlPfw::SURFACE_SPECIES;
		static std::pair<CString, CString> CTreeCtrlPfw::SURFACE_SPECIES_log_k;
		static std::pair<CString, CString> CTreeCtrlPfw::SURFACE_SPECIES_delta_h;
		static std::pair<CString, CString> CTreeCtrlPfw::SURFACE_SPECIES_analytical_expression;
		static std::pair<CString, CString> CTreeCtrlPfw::SURFACE_SPECIES_cd_music;
		static std::pair<CString, CString> CTreeCtrlPfw::SURFACE_SPECIES_no_check;
		static std::pair<CString, CString> CTreeCtrlPfw::SURFACE_SPECIES_mole_balance;
		static std::pair<CString, CString> CTreeCtrlPfw::SURFACE_SPECIES_add_constant;
		static std::pair<CString, CString> CTreeCtrlPfw::SURFACE_SPECIES_add_logk;

	// Miscellaneous
	static std::pair<CString, CString> CTreeCtrlPfw::CALCULATE_VALUES;
		static std::pair<CString, CString> CTreeCtrlPfw::CALCULATE_VALUES_start;
		static std::pair<CString, CString> CTreeCtrlPfw::CALCULATE_VALUES_end;
	static std::pair<CString, CString> CTreeCtrlPfw::ISOTOPES;
		static std::pair<CString, CString> CTreeCtrlPfw::ISOTOPES_isotope;
	static std::pair<CString, CString> CTreeCtrlPfw::ISOTOPE_ALPHAS;
	static std::pair<CString, CString> CTreeCtrlPfw::ISOTOPE_RATIOS;
	static std::pair<CString, CString> CTreeCtrlPfw::KNOBS;
		static std::pair<CString, CString> CTreeCtrlPfw::KNOBS_iterations;
		static std::pair<CString, CString> CTreeCtrlPfw::KNOBS_convergence_tolerance;
		static std::pair<CString, CString> CTreeCtrlPfw::KNOBS_tolerance;
		static std::pair<CString, CString> CTreeCtrlPfw::KNOBS_step_size;
		static std::pair<CString, CString> CTreeCtrlPfw::KNOBS_pe_step_size;
		static std::pair<CString, CString> CTreeCtrlPfw::KNOBS_diagonal_scale;
		static std::pair<CString, CString> CTreeCtrlPfw::KNOBS_debug_diffuse_layer;
		static std::pair<CString, CString> CTreeCtrlPfw::KNOBS_debug_inverse;
		static std::pair<CString, CString> CTreeCtrlPfw::KNOBS_debug_model;
		static std::pair<CString, CString> CTreeCtrlPfw::KNOBS_debug_prep;
		static std::pair<CString, CString> CTreeCtrlPfw::KNOBS_debug_set;
		static std::pair<CString, CString> CTreeCtrlPfw::KNOBS_logfile;
	static std::pair<CString, CString> CTreeCtrlPfw::LLNL_AQUEOUS_MODEL_PARAMETERS;
		static std::pair<CString, CString> CTreeCtrlPfw::LLNL_AQUEOUS_MODEL_PARAMETERS_temperatures;
		static std::pair<CString, CString> CTreeCtrlPfw::LLNL_AQUEOUS_MODEL_PARAMETERS_dh_a;
		static std::pair<CString, CString> CTreeCtrlPfw::LLNL_AQUEOUS_MODEL_PARAMETERS_dh_b;
		static std::pair<CString, CString> CTreeCtrlPfw::LLNL_AQUEOUS_MODEL_PARAMETERS_bdot;
		static std::pair<CString, CString> CTreeCtrlPfw::LLNL_AQUEOUS_MODEL_PARAMETERS_co2_coefs;
	static std::pair<CString, CString> CTreeCtrlPfw::NAMED_EXPRESSIONS;

	// PHREEQC BASIC statements
	//{{NEW BASIC HERE}}
	static std::pair<CString, CString> CTreeCtrlPfw::ACT;
	static std::pair<CString, CString> CTreeCtrlPfw::ALK;
	static std::pair<CString, CString> CTreeCtrlPfw::CALC_VALUE;
	static std::pair<CString, CString> CTreeCtrlPfw::CELL_NO;
	static std::pair<CString, CString> CTreeCtrlPfw::CHANGE_POR;
	static std::pair<CString, CString> CTreeCtrlPfw::CHANGE_SURF;
	static std::pair<CString, CString> CTreeCtrlPfw::CHARGE_BALANCE;
	static std::pair<CString, CString> CTreeCtrlPfw::DESCRIPTION;
	static std::pair<CString, CString> CTreeCtrlPfw::DH_A;
	static std::pair<CString, CString> CTreeCtrlPfw::DH_Av;
	static std::pair<CString, CString> CTreeCtrlPfw::DH_B;
	static std::pair<CString, CString> CTreeCtrlPfw::DIFF_C;
	static std::pair<CString, CString> CTreeCtrlPfw::DIST;
	static std::pair<CString, CString> CTreeCtrlPfw::EDL;
	static std::pair<CString, CString> CTreeCtrlPfw::EDL_SPECIES;
	static std::pair<CString, CString> CTreeCtrlPfw::EPS_R;
	static std::pair<CString, CString> CTreeCtrlPfw::EQ_FRAC;
	static std::pair<CString, CString> CTreeCtrlPfw::EQUI;
	static std::pair<CString, CString> CTreeCtrlPfw::EQUI_DELTA;
	static std::pair<CString, CString> CTreeCtrlPfw::EXISTS;
	static std::pair<CString, CString> CTreeCtrlPfw::GAMMA;
	static std::pair<CString, CString> CTreeCtrlPfw::GAS;
	static std::pair<CString, CString> CTreeCtrlPfw::GAS_P;
	static std::pair<CString, CString> CTreeCtrlPfw::GAS_VM;
	static std::pair<CString, CString> CTreeCtrlPfw::GET;
	static std::pair<CString, CString> CTreeCtrlPfw::GET_POR;
	static std::pair<CString, CString> CTreeCtrlPfw::GFW;
	static std::pair<CString, CString> CTreeCtrlPfw::GRAPH_SY;
	static std::pair<CString, CString> CTreeCtrlPfw::GRAPH_X;
	static std::pair<CString, CString> CTreeCtrlPfw::GRAPH_Y;
	static std::pair<CString, CString> CTreeCtrlPfw::ISO;
	static std::pair<CString, CString> CTreeCtrlPfw::ISO_UNITS;
	static std::pair<CString, CString> CTreeCtrlPfw::KAPPA;
	static std::pair<CString, CString> CTreeCtrlPfw::KIN;
	static std::pair<CString, CString> CTreeCtrlPfw::KIN_DELTA;
	static std::pair<CString, CString> CTreeCtrlPfw::KIN_TIME;
	static std::pair<CString, CString> CTreeCtrlPfw::LA;
	static std::pair<CString, CString> CTreeCtrlPfw::LG;
	static std::pair<CString, CString> CTreeCtrlPfw::LIST_S_S;
	static std::pair<CString, CString> CTreeCtrlPfw::LK_NAMED;
	static std::pair<CString, CString> CTreeCtrlPfw::LK_PHASE;
	static std::pair<CString, CString> CTreeCtrlPfw::LK_SPECIES;
	static std::pair<CString, CString> CTreeCtrlPfw::LM;
	static std::pair<CString, CString> CTreeCtrlPfw::M;
	static std::pair<CString, CString> CTreeCtrlPfw::M0;
	static std::pair<CString, CString> CTreeCtrlPfw::MISC1;
	static std::pair<CString, CString> CTreeCtrlPfw::MISC2;
	static std::pair<CString, CString> CTreeCtrlPfw::MOL;
	static std::pair<CString, CString> CTreeCtrlPfw::MU;
	static std::pair<CString, CString> CTreeCtrlPfw::OSMOTIC;
	static std::pair<CString, CString> CTreeCtrlPfw::PARM;
	static std::pair<CString, CString> CTreeCtrlPfw::PERCENT_ERROR;
	static std::pair<CString, CString> CTreeCtrlPfw::PHASE_FORMULA;
	static std::pair<CString, CString> CTreeCtrlPfw::PLOT_XY;
	static std::pair<CString, CString> CTreeCtrlPfw::PR_P;
	static std::pair<CString, CString> CTreeCtrlPfw::PR_PHI;
	static std::pair<CString, CString> CTreeCtrlPfw::PRESSURE_BASIC;
	static std::pair<CString, CString> CTreeCtrlPfw::BASIC_PRINT;
	static std::pair<CString, CString> CTreeCtrlPfw::PUNCH;
	static std::pair<CString, CString> CTreeCtrlPfw::PUT;
	static std::pair<CString, CString> CTreeCtrlPfw::QBRN;
#if defined PHREEQ98
	static std::pair<CString, CString> CTreeCtrlPfw::RHO_m;
	static std::pair<CString, CString> CTreeCtrlPfw::RHO_v;
#else
	static std::pair<CString, CString> CTreeCtrlPfw::RHO;
#endif
	static std::pair<CString, CString> CTreeCtrlPfw::RXN;
	static std::pair<CString, CString> CTreeCtrlPfw::BASIC_SAVE;
	static std::pair<CString, CString> CTreeCtrlPfw::SC;
	static std::pair<CString, CString> CTreeCtrlPfw::SI;
	static std::pair<CString, CString> CTreeCtrlPfw::SIM_NO;
	static std::pair<CString, CString> CTreeCtrlPfw::SIM_TIME;
	static std::pair<CString, CString> CTreeCtrlPfw::SOLN_VOL;
	static std::pair<CString, CString> CTreeCtrlPfw::SPECIES_FORMULA;
	static std::pair<CString, CString> CTreeCtrlPfw::SR;
	static std::pair<CString, CString> CTreeCtrlPfw::STEP_NO;
	static std::pair<CString, CString> CTreeCtrlPfw::STR_F;
	static std::pair<CString, CString> CTreeCtrlPfw::STR_E;
	static std::pair<CString, CString> CTreeCtrlPfw::SUM_GAS;
	static std::pair<CString, CString> CTreeCtrlPfw::SUM_SPECIES;
	static std::pair<CString, CString> CTreeCtrlPfw::SUM_S_S;
	static std::pair<CString, CString> CTreeCtrlPfw::SURF;
	static std::pair<CString, CString> CTreeCtrlPfw::SYS;
	static std::pair<CString, CString> CTreeCtrlPfw::S_S;
	static std::pair<CString, CString> CTreeCtrlPfw::TC;
	static std::pair<CString, CString> CTreeCtrlPfw::TK;
	static std::pair<CString, CString> CTreeCtrlPfw::TIME;
	static std::pair<CString, CString> CTreeCtrlPfw::TOT;
	static std::pair<CString, CString> CTreeCtrlPfw::TOTMOLE;
	static std::pair<CString, CString> CTreeCtrlPfw::TOTAL_TIME;
	static std::pair<CString, CString> CTreeCtrlPfw::VM;

	// GENERAL BASIC statements
	static std::pair<CString, CString> CTreeCtrlPfw::AND;
	static std::pair<CString, CString> CTreeCtrlPfw::OR;
	static std::pair<CString, CString> CTreeCtrlPfw::NOT;
	static std::pair<CString, CString> CTreeCtrlPfw::ABS;
	static std::pair<CString, CString> CTreeCtrlPfw::ARCTAN;
	static std::pair<CString, CString> CTreeCtrlPfw::ASC;
	static std::pair<CString, CString> CTreeCtrlPfw::CEIL;
	static std::pair<CString, CString> CTreeCtrlPfw::CHR;
	static std::pair<CString, CString> CTreeCtrlPfw::COS;
	static std::pair<CString, CString> CTreeCtrlPfw::DIM;
	static std::pair<CString, CString> CTreeCtrlPfw::DATA;
	static std::pair<CString, CString> CTreeCtrlPfw::ELSE;
	static std::pair<CString, CString> CTreeCtrlPfw::EOL;
	static std::pair<CString, CString> CTreeCtrlPfw::EXP;
	static std::pair<CString, CString> CTreeCtrlPfw::FOR;
	static std::pair<CString, CString> CTreeCtrlPfw::NEXT;
	static std::pair<CString, CString> CTreeCtrlPfw::FLOOR;
	static std::pair<CString, CString> CTreeCtrlPfw::GOTO;
	static std::pair<CString, CString> CTreeCtrlPfw::GOSUB;
	static std::pair<CString, CString> CTreeCtrlPfw::RETURN;
	static std::pair<CString, CString> CTreeCtrlPfw::IF;
	static std::pair<CString, CString> CTreeCtrlPfw::INSTR;
	static std::pair<CString, CString> CTreeCtrlPfw::LEN;
	static std::pair<CString, CString> CTreeCtrlPfw::LOG;
	static std::pair<CString, CString> CTreeCtrlPfw::LOG10;
	static std::pair<CString, CString> CTreeCtrlPfw::LTRIM;
	static std::pair<CString, CString> CTreeCtrlPfw::MID;
	static std::pair<CString, CString> CTreeCtrlPfw::MOD;
	static std::pair<CString, CString> CTreeCtrlPfw::ON_GOTO;
	static std::pair<CString, CString> CTreeCtrlPfw::ON_GOSUB;
	static std::pair<CString, CString> CTreeCtrlPfw::PAD;
	static std::pair<CString, CString> CTreeCtrlPfw::READ;
	static std::pair<CString, CString> CTreeCtrlPfw::REM;
	static std::pair<CString, CString> CTreeCtrlPfw::RESTORE;
	static std::pair<CString, CString> CTreeCtrlPfw::RTRIM;
	static std::pair<CString, CString> CTreeCtrlPfw::SGN;
	static std::pair<CString, CString> CTreeCtrlPfw::SIN;
	static std::pair<CString, CString> CTreeCtrlPfw::SQR;
	static std::pair<CString, CString> CTreeCtrlPfw::SQRT;
	static std::pair<CString, CString> CTreeCtrlPfw::STR;
	static std::pair<CString, CString> CTreeCtrlPfw::TAN;
	static std::pair<CString, CString> CTreeCtrlPfw::TRIM;
	static std::pair<CString, CString> CTreeCtrlPfw::VAL;
	static std::pair<CString, CString> CTreeCtrlPfw::WHILE;
	static std::pair<CString, CString> CTreeCtrlPfw::WEND;

	// examples
	static std::pair<CString, CString> CTreeCtrlPfw::ex1;
	static std::pair<CString, CString> CTreeCtrlPfw::ex2;
	static std::pair<CString, CString> CTreeCtrlPfw::ex2b;
	static std::pair<CString, CString> CTreeCtrlPfw::ex3;
	static std::pair<CString, CString> CTreeCtrlPfw::ex4;
	static std::pair<CString, CString> CTreeCtrlPfw::ex5;
	static std::pair<CString, CString> CTreeCtrlPfw::ex6;
	static std::pair<CString, CString> CTreeCtrlPfw::ex7;
	static std::pair<CString, CString> CTreeCtrlPfw::ex8;
	static std::pair<CString, CString> CTreeCtrlPfw::ex9;
	static std::pair<CString, CString> CTreeCtrlPfw::ex10;
	static std::pair<CString, CString> CTreeCtrlPfw::ex11;
	static std::pair<CString, CString> CTreeCtrlPfw::ex12;
	static std::pair<CString, CString> CTreeCtrlPfw::ex13a;
	static std::pair<CString, CString> CTreeCtrlPfw::ex13b;
	static std::pair<CString, CString> CTreeCtrlPfw::ex13c;
	static std::pair<CString, CString> CTreeCtrlPfw::ex14;
	static std::pair<CString, CString> CTreeCtrlPfw::ex15;
	static std::pair<CString, CString> CTreeCtrlPfw::ex16;
	static std::pair<CString, CString> CTreeCtrlPfw::ex17;
	static std::pair<CString, CString> CTreeCtrlPfw::ex18;
	static std::pair<CString, CString> CTreeCtrlPfw::ex19;
	static std::pair<CString, CString> CTreeCtrlPfw::ex20a;
	static std::pair<CString, CString> CTreeCtrlPfw::ex20b;
	static std::pair<CString, CString> CTreeCtrlPfw::ex21;
	static std::pair<CString, CString> CTreeCtrlPfw::ex22;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);

	void SetDescWindow(CWnd *pWnd) { m_pWndDesc = pWnd; };

	void SetKeywordDetail(KeywordDetail kd);
	KeywordDetail GetKeywordDetail(void)const;
	void FillTree(KeywordDetail kd);

protected:
	CWnd *m_pWndDesc;
	enum KeywordDetail KeyDetail;
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
};


