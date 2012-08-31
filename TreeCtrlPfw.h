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

class CTreeCtrlPfw : public baseCTreeCtrlPfw
{
	DECLARE_DYNAMIC(CTreeCtrlPfw)

public:
	CTreeCtrlPfw();
	virtual ~CTreeCtrlPfw();

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
	static std::pair<CString, CString> CTreeCtrlPfw::REACTION;
	static std::pair<CString, CString> CTreeCtrlPfw::REACTION_TEMPERATURE;
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

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	void SetDescWindow(CWnd *pWnd) { m_pWndDesc = pWnd; };

protected:
	CWnd *m_pWndDesc;
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
};


