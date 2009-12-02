#define EXTERNAL extern
#include <setjmp.h>
#include "phreeqc/src/global.h"
#include "phreeqc/src/phrqproto.h"
#include "phreeqc/src/input.h"
#include "phreeqci_files.h"

static char const rcsid[] = "$RCSfile: phreeqci_files.c,v $  $Revision: 1.2 $";

LONG ExceptionFilter(DWORD dwExceptionCode);
int tidy_model_gui(void);
int tidy_species_gui(void);

/* ---------------------------------------------------------------------- */
DWORD gui_main(int argc, char *argv[], void *cookie, PFN_OUTPUT_CALLBACK pfnWrite)
/* ---------------------------------------------------------------------- */
{
	DWORD dwReturn = 0;
	
	__try
	{
		int errors;
		void *db_cookie = NULL;
		void *input_cookie = NULL;

		if (rcsid == NULL) fprintf(stderr," ");

/*
 *   Add callbacks for error_msg and warning_msg
 */
		add_output_callback(phreeqc_handler, NULL);
		add_output_callback(pfnWrite, cookie);

	#ifdef DOS
		write_banner(); 
	#endif

/*
 *   Open input/output files
 */
		errors = process_file_names(argc, argv, &db_cookie, &input_cookie, TRUE);
		assert(errors == 0);

/*
 *   Initialize arrays
 */
		errors = do_initialize();
		assert(errors == 0);
		assert(pr.headings == TRUE);

/*
 *   Load database into memory
 */
		errors = read_database(getc_callback, db_cookie);
		assert(errors == 0);

/*
 *   Read input data for simulation
 */
		errors = run_simulations(getc_callback, input_cookie);
		assert(errors == 0);

/*
 *   Display successful status
 */
		errors = do_status();
		assert(errors == 0);

		clean_up();
		close_input_files();
		close_output_files();
	}
	__except ( ExceptionFilter(GetExceptionCode()) )
	{
		/* An error occurred or the user pressed cancel during run */
		dwReturn = GetExceptionCode();
	}

	return dwReturn;
}

/* ---------------------------------------------------------------------- */
LONG ExceptionFilter(DWORD dwExceptionCode)
/* ---------------------------------------------------------------------- */
{
	LONG lReturn;
	char buffer[200];

	switch (dwExceptionCode)
	{
	case USER_CANCELED_RUN :
		lReturn = EXCEPTION_EXECUTE_HANDLER;
		input_error = 0;
		break;

	case INPUT_CONTAINS_ERRORS :
		lReturn = EXCEPTION_EXECUTE_HANDLER;
		break;

	default :
		//
		// a hardware exception has occured
		//
		switch (dwExceptionCode) 
		{
		case EXCEPTION_ACCESS_VIOLATION :
#if defined(_DEBUG)
			//
			// let the just-in-time debugger handle
			//
			return EXCEPTION_CONTINUE_SEARCH;
#endif
			//
			// handle exception probably a NULL pointer error
			//
			sprintf(buffer, "Handled exception (%u):EXCEPTION_ACCESS_VIOLATION.\nStopping\n", dwExceptionCode);
			error_msg(buffer, CONTINUE);
			lReturn = EXCEPTION_EXECUTE_HANDLER;
			break;
		default :
			sprintf(buffer, "Unhandled exception (%u).\nStopping\n", dwExceptionCode);
			error_msg(buffer, CONTINUE);
			//
			// let operating system handle error
			//
			lReturn = EXCEPTION_CONTINUE_SEARCH;
		}
	}
	clean_up();
	close_input_files();
	close_output_files();
	return lReturn;
}


/* ---------------------------------------------------------------------- */
LONG load_database2_filter(DWORD dwExceptionCode)
/* ---------------------------------------------------------------------- */
{
	LONG lReturn = 0;
	switch (dwExceptionCode)
	{
	case INPUT_CONTAINS_ERRORS :
		lReturn = EXCEPTION_EXECUTE_HANDLER;
		break;
#if !defined(_DEBUG)
	case EXCEPTION_ACCESS_VIOLATION :
		lReturn = EXCEPTION_EXECUTE_HANDLER;
		break;
#endif
	}
	return lReturn;
}

/* ---------------------------------------------------------------------- */
DWORD load_database2(void *cookie, PFN_READ_CALLBACK pfnRead, PFN_OUTPUT_CALLBACK pfnWrite)
/* ---------------------------------------------------------------------- */
{
	DWORD dw = 0;
	__try
	{
		assert(input_error == 0);
		add_output_callback(pfnWrite, cookie);
		phast = FALSE;
		do_initialize();
		read_database(pfnRead, cookie);
	}
	__except ( load_database2_filter(GetExceptionCode()) )
	{
		dw = GetExceptionCode();
	}
	return dw;
}

/* ---------------------------------------------------------------------- */
DWORD load_database_local2(void *cookie, PFN_READ_CALLBACK pfnRead, PFN_OUTPUT_CALLBACK pfnWrite, int last_simulation)
/* ---------------------------------------------------------------------- */
{
	DWORD dw = 0;
	__try
	{
		assert(input_error == 0);
		add_output_callback(pfnWrite, cookie);
		phast = FALSE;
		do_initialize();

		set_read_callback(pfnRead, cookie, FALSE);
		for (simulation = 1; simulation <= last_simulation; ++simulation) {
			if (read_input() == EOF) break;
			tidy_model_gui();
		}
		tidy_model_gui();
		input_error = 0;
	}
	__except ( load_database2_filter(GetExceptionCode()) )
	{
		dw = GetExceptionCode();
	}
	return dw;
}

void local_init(void)
{
	/* knobs */
	pr.logfile = -1;

	debug_diffuse_layer = -1;
	debug_inverse       = -1;
	debug_model         = -1;
	debug_prep          = -1;
	debug_set           = -1;
	delay_mass_water    = -1;
	diagonal_scale      = -1;

	/* print */
	pr.alkalinity         = -1;
	pr.gas_phase          = -1;
	pr.s_s_assemblage     = -1;
	pr.pp_assemblage      = -1;
	pr.surface            = -1;
	pr.exchange           = -1;
	pr.kinetics           = -1;
	pr.totals             = -1;
	pr.eh                 = -1;
	pr.species            = -1;
	pr.saturation_indices = -1;
	pr.irrev              = -1;
	pr.mix                = -1;
	pr.use                = -1;
	pr.punch              = -1;
	pr.status             = -1;
	pr.inverse            = -1;
	pr.dump               = -1;
	pr.user_print         = -1;
	pr.headings           = -1;
	pr.echo_input         = -1;

	/* punch */
	punch.alk            = -1;
	punch.charge_balance = -1;
	punch.dist           = -1;
	punch.high_precision = -1;
	punch.inverse        = -1;
	punch.mu             = -1;
	punch.pe             = -1;
	punch.percent_error  = -1;
	punch.ph             = -1;
	punch.rxn            = -1;
	punch.sim            = -1;
	punch.soln           = -1;
	punch.state          = -1;
	punch.step           = -1;
	punch.temp           = -1;
	punch.time           = -1;
	punch.user_punch     = -1;
	punch.water          = -1;
}

/* ---------------------------------------------------------------------- */
DWORD load_keyword2(void *cookie, PFN_READ_CALLBACK pfnRead, PFN_OUTPUT_CALLBACK pfnWrite)
/* ---------------------------------------------------------------------- */
{
	DWORD dw = 0;
	__try
	{
		assert(input_error == 0);
		add_output_callback(pfnWrite, cookie);
		phast = FALSE;
		do_initialize();
		local_init();
		set_read_callback(pfnRead, cookie, FALSE);
		read_input();
		input_error = 0;
	}
	__except ( load_database2_filter(GetExceptionCode()) )
	{
		dw = GetExceptionCode();
	}
	return dw;
}

/* ---------------------------------------------------------------------- */
DWORD check_errors_gui2(void *cookie, PFN_READ_CALLBACK pfnRead, PFN_OUTPUT_CALLBACK pfnWrite)
/* ---------------------------------------------------------------------- */
{
	DWORD dw = 0;
	__try
	{
		assert(input_error == 0);
		add_output_callback(pfnWrite, cookie);
		phast = FALSE;
		do_initialize();
		set_read_callback(pfnRead, cookie, FALSE);
		for (simulation = 1; ; ++simulation) {
			if (read_input() == EOF) break;
		}
		input_error = 0;
	}
	__except ( load_database2_filter(GetExceptionCode()) )
	{
		dw = GetExceptionCode();
	}
	return dw;
}

/* ---------------------------------------------------------------------- */
int tidy_model_gui(void)
/* ---------------------------------------------------------------------- */
{
#ifdef GUI_SKIP
	int i, j;
	int n_user, last;
	if (rcsid == NULL) fprintf(stderr," ");
#endif
	/*
	 * Determine if any new elements, species, phases have been read
	 */
	new_model          = FALSE;
	new_pp_assemblage  = FALSE;
	new_surface        = FALSE;
	new_exchange       = FALSE;
	new_reaction       = FALSE;
	new_temperature    = FALSE;
	new_mix            = FALSE;
	new_solution       = FALSE;
	new_gas_phase      = FALSE;
	new_inverse        = FALSE;
	new_punch          = FALSE;
	new_surface        = FALSE;
	new_s_s_assemblage = FALSE;
	new_kinetics       = FALSE;
	if (keyword[2].keycount  > 0 ||   /*"species"*/
	    keyword[3].keycount  > 0 ||   /*"master"*/
	    keyword[5].keycount  > 0 ||   /*"phases"*/
	    keyword[11].keycount > 0 ||   /*"exchange_species"*/
	    keyword[12].keycount > 0 ||   /*"master_exchange_species"*/
	    keyword[14].keycount > 0 ||   /*"surface_species"*/
	    keyword[15].keycount > 0 ||   /*"master_surface_species"*/
	    keyword[36].keycount > 0 ||   /*"rates"*/
	    keyword[47].keycount > 0 ||   /*"llnl_aqueous_model_parameters"*/
	    keyword[49].keycount > 0 ||   /*"database"*/
	    keyword[51].keycount > 0 ||   /*"named_analytical_expressions"*/
	    keyword[54].keycount > 0 ||   /*"isotopes"*/
	    keyword[55].keycount > 0 ||   /*"calculate_values"*/
	    keyword[56].keycount > 0 ||   /*"isotopes_ratios",*/
	    keyword[57].keycount > 0 )    /*"isotopes_alphas"*/
	{
		new_model = TRUE;
	} 
	if (keyword[6].keycount  > 0) new_pp_assemblage = TRUE;  /*"pure_phases"*/
	if (keyword[16].keycount > 0) new_surface       = TRUE;  /*"surface"*/
	if (keyword[13].keycount > 0) new_exchange      = TRUE;  /*"exchange"*/
	if (keyword[7].keycount  > 0) new_reaction      = TRUE;  /*"reaction"*/
	if (keyword[17].keycount > 0) new_temperature   = TRUE;  /*"reacton_temperature"*/
	if (keyword[8].keycount  > 0) new_mix           = TRUE;  /*"mix"*/
	if (keyword[4].keycount  > 0 ||                          /*"solution"*/
	    keyword[43].keycount > 0 )                           /*"spread_solution"*/
	{
		new_solution = TRUE;
	}
	if (keyword[19].keycount > 0) new_gas_phase = TRUE;     /*"gas_phase"*/
	if (keyword[18].keycount > 0) new_inverse = TRUE;       /*"inverse_modeling"*/
	if (keyword[22].keycount > 0 ||                         /*"selected_output"*/
	    keyword[39].keycount > 0 ) {                        /*"user_punch"*/
		new_punch = TRUE;
	}
	if (keyword[40].keycount > 0) new_s_s_assemblage = TRUE;/*"solid_solutions"*/
	if (keyword[33].keycount > 0) new_kinetics = TRUE;      /*"kinetics"*/
	if (keyword[58].keycount > 0) new_copy = TRUE;          /*"copy"*/

	/*
	  0       "eof"
	  1       "end"
	  2       "species"
	  3       "master"
	  4       "solution"
	  5       "phases"
	  6       "pure_phases"
	  7       "reaction"
	  8       "mix"
	  9       "use"
	  10      "save"
	  11      "exchange_species"
	  12      "master_exchange_species"
	  13      "exchange"
	  14      "surface_species"
	  15      "master_surface_species"
	  16      "surface"
	  17      "reacton_temperature"
	  18      "inverse_modeling"
	  19      "gas_phase"
	  20      "transport"
	  21      "debug"
	  22      "selected_output"
	  23      "select_output"
	  24      "knobs"
	  25      "print"
	  26      "equilibrium_phases"
	  27      "equilibria"
	  28      "equilibrium"
	  29      "pure"
	  30      "title"
	  31      "comment"
	  32      "advection"
	  33      "kinetics"
	  34      "incremental_reactions"
	  35      "incremental"
	  36      "rates"
	  37      "solution_s"
	  38      "user_print"
	  39      "user_punch"
	  40      "solid_solutions"
	  41      "solid_solution"
	  42      "solution_spread"
	  43      "spread_solution"
	  44      "selected_out"
	  45      "select_out"
	  46      "user_graph"
	  47      "llnl_aqueous_model_parameters"
	  48      "llnl_aqueous_model"
	  49      "database"
	  50      "named_analytical_expression"
	  51      "named_analytical_expressions"
	  52      "named_expressions"
	  53      "named_log_k"
	  54      "isotopes"
	  55      "calculate_values"
	  56      "isotopes_ratios",
	  57      "isotopes_alphas"
	  58      "copy"
	*/

/*
 *   Sort arrays
 */

/* species */
	if (new_model == TRUE) {
		qsort (s,
		       (size_t) count_s,
		       (size_t) sizeof (struct species *),
		       s_compare);

/* master species */
		qsort (master, 
		       (unsigned) count_master, 
		       sizeof(struct master *), 
		       master_compare);

/* elements */
		qsort (elements,
		       (size_t) count_elements,
		       (size_t) sizeof (struct element *),
		       element_compare);
/* phases */
		qsort (phases,
		       (size_t) count_phases,
		       (size_t) sizeof (struct phase *),
		       phase_compare);
	
	}
/* pure_phases */
	if (new_pp_assemblage) pp_assemblage_sort();

/* solid solutions */
	if (new_s_s_assemblage) s_s_assemblage_sort();

/* exchangers */
	if (new_exchange) exchange_sort();
/* surfaces */
	if (new_surface) surface_sort();
#ifdef SKIP
/* mixtures */
	qsort (mix,
	       (size_t) count_mix,
	       (size_t) sizeof (struct mix),
	       mix_compare);
#endif

/* mixtures */
/* !!!!!
 * In transport mode, with stagnant cells, cell number is 'n_mix_user' for
 *  both mix with stagnant cells and for dispersive mix. 
 *  qsort(mix) then fails. Hence ...
 */  

	if ((state != TRANSPORT) || (simul_tr < 2) || (stag_data->count_stag == 0)) {
		mix_sort();
	}

/* gas_phase */
	if (new_gas_phase) gas_phase_sort();

/* kinetics */
	if (new_kinetics) kinetics_sort();
/*
 *   Check pointers, write reactions for species
 */
	if (new_model) {
		tidy_species_gui();

#ifdef GUI_SKIP
		tidy_phases();

		tidy_master_isotope();
/*
 *   calculate gfw of water, kg/mole
 */
		compute_gfw("H2O", &gfw_water);
		gfw_water *= 0.001;
#endif // GUI_SKIP
	}

#ifdef GUI_SKIP
/*
 *   tidy surface data
 */
	if (new_model || new_surface) tidy_surface();
/*
 *   tidy inverse data
 */
	if (new_inverse) tidy_inverse();
/*
 *   tidy gas phase data
 */
	if (new_gas_phase) tidy_gas_phase();
/*
 *   tidy pp_assemblage data
 */
	if (new_model || new_pp_assemblage) tidy_pp_assemblage();
/*
 *   tidy s_s_assemblage data
 */
	if (new_model || new_s_s_assemblage) tidy_s_s_assemblage();
/*
 *   tidy exchange data, after pp_assemblages
 */
	if (new_exchange) tidy_min_exchange();
	if (new_exchange) tidy_kin_exchange();
/*
 *   tidy surface data
 */
	if (new_surface) tidy_min_surface();
	if (new_surface) tidy_kin_surface();
/*
 *   tidy solution isotope data
 */
	if (new_solution) tidy_isotopes();
	if (new_model) tidy_isotope_ratios();
	if (new_model) tidy_isotope_alphas();
#endif // GUI_SKIP

/*
 *   Duplicate reaction
 */
#ifdef GUI_SKIP
	if (new_reaction) {
		for ( i = 0; i < count_irrev; i++) {
			if ( irrev[i].n_user_end > irrev[i].n_user ) {
				n_user = irrev[i].n_user;
				last = irrev[i].n_user_end;
				irrev[i].n_user_end = irrev[i].n_user;
				for (j = n_user + 1; j <= last; j++) {
					irrev_duplicate(n_user, j);
				}
			}
		}
	}
#endif // GUI_SKIP

/*
 *   Duplicate kinetics
 */
#ifdef GUI_SKIP
	if (new_kinetics) {
		for ( i = 0; i < count_kinetics; i++) {
			if ( kinetics[i].n_user_end > kinetics[i].n_user ) {
				n_user = kinetics[i].n_user;
				last = kinetics[i].n_user_end;
				kinetics[i].n_user_end = kinetics[i].n_user;
				for (j = n_user + 1; j <= last; j++) {
					kinetics_duplicate(n_user, j);
				}
			}
		}
	}
#endif // GUI_SKIP
/*
 *   Duplicate temperature
 */
#ifdef GUI_SKIP
	if (new_temperature) {
		temperature_sort();
		for ( i = 0; i < count_temperature; i++) {
			if ( temperature[i].n_user_end > temperature[i].n_user ) {
				n_user = temperature[i].n_user;
				last = temperature[i].n_user_end;
				temperature[i].n_user_end = temperature[i].n_user;
				for (j = n_user + 1; j <= last; j++) {
					temperature_duplicate(n_user, j);
				}
			}
		}
	}
/*
 *   Tidy punch information
 */
	if (input_error == 0 && (new_punch || new_model)) tidy_punch();
/*
 *   Tidy solution information
 */
	if (new_solution) tidy_solutions();

	/*	if (new_model || new_exchange || new_pp_assemblage || new_surface || new_gas_phase || new_kinetics) reset_last_model(); */
	if (new_model) reset_last_model();
#endif // GUI_SKIP

/*
 *   make sure essential species are defined
 */
#ifdef GUI_SKIP
	if (s_h2o == NULL) {
		input_error++;
		error_msg("H2O not defined.", STOP);
	}
	if (s_h2o->primary == NULL) {
		input_error++;
		error_msg("H2O, primary master species for O, not defined.", CONTINUE);
	}
	if (s_h2o->secondary == NULL) {
		input_error++;
		error_msg("H2O, secondary master species for O(-2), not defined.", CONTINUE);
	}
	if (s_hplus == NULL && s_h3oplus == NULL) {
		input_error++;
		error_msg("Neither H+ nor H3O+ are defined in solution_species.", STOP);
	} else if (s_hplus == NULL && s_h3oplus != NULL) {
		s_hplus = s_h3oplus;
		s_h3oplus = NULL;
	} else if (s_hplus != NULL && s_h3oplus == NULL) {
	} else if (s_hplus != NULL && s_h3oplus != NULL) {
		input_error++;
		error_msg("Can not define both H+ and H3O+ in solution_species.", STOP);
	}
	if (s_hplus->primary == NULL) {
		input_error++;
		error_msg("H3O+, primary master species for H, not defined.", CONTINUE);
	}
	if (s_hplus->secondary == NULL) {
		input_error++;
		error_msg("H3O+, secondary master species for H(1), not defined.", CONTINUE);
	}
	if (s_eminus == NULL) {
		input_error++;
		error_msg("e- not defined in solution_species.", CONTINUE);
	}
	if (s_eminus->primary == NULL) {
		input_error++;
		error_msg("e-, primary master species for E-, not defined.", CONTINUE);
	}
	if (s_h2 == NULL) {
		input_error++;
		error_msg("H2(aq) not defined in solution_species.", CONTINUE);
	}
	if (s_o2 == NULL) {
		input_error++;
		error_msg("O2(aq) not defined in solution_species.", CONTINUE);
	}
	element_h_one = element_store ("H(1)");
	if (element_h_one == NULL) {
		input_error++;
		error_msg("H(1) not defined in solution_master_species.", CONTINUE);
	}		
/*
 *   Error check, program termination
 */
	if (input_error > 0 || parse_error > 0 ) {
		error_msg("Calculations terminating due to input errors.", STOP);
	}
#endif // GUI_SKIP

	return(OK);
}

/* ---------------------------------------------------------------------- */
int tidy_species_gui(void)
/* ---------------------------------------------------------------------- */
{
	int i;
#ifdef SKIP_GUI
	struct master *master_ptr;
#endif
/*
 *   Make sure species pointers are ok
 */
#ifdef SKIP_GUI
	if (check_species_input() == ERROR) {
		error_msg("Calculations terminating due to input errors.", STOP);
	}
#endif
/*
 *   Set secondary and primary pointers in species structures
 */
	for (i=0; i < count_s; ++i) {
		s[i]->primary=NULL;
		s[i]->secondary=NULL;
	}
	for (i=0; i < count_master; ++i) {
		if (master[i]->primary == TRUE) {
			master[i]->s->primary=master[i];
		} else {
			master[i]->s->secondary=master[i];
		}
		if (strcmp(master[i]->elt->name,"C") == 0) {
			s_co3=master[i]->s;
		}
		if (master[i]->gfw_formula != NULL) {
#ifdef SKIP_GUI
			if(compute_gfw(master[i]->gfw_formula, &master[i]->gfw) == ERROR){
				input_error++;
				sprintf(error_string, "Calculating gfw for master species, %s, formula %s.", 
					master[i]->elt->name, master[i]->gfw_formula);
				error_msg(error_string, CONTINUE);
			}
#endif
		}
	}
/*
 *   Write equations for all master species in terms of primary
 *   master species, set coefficient of element in master species
 */
#ifdef SKIP_GUI
	for (i=0; i < count_master; ++i) {
		count_trxn=0;
		if ( master[i]->s->primary != NULL ) {
			trxn_add(master[i]->s->rxn, 1.0, FALSE);
			trxn_add(master[i]->s->rxn, -1.0, TRUE);
		} else {
			trxn_add(master[i]->s->rxn, 1.0, FALSE);
			rewrite_eqn_to_primary();
		}
		rxn_free(master[i]->rxn_primary);
		master[i]->rxn_primary = rxn_alloc(count_trxn+1);
		trxn_copy (master[i]->rxn_primary);
		master[i]->coef = coef_in_master(master[i]);
	}
/*
 *   Rewrite all species to secondary species
 */
	for (i=0; i < count_s; ++i) {
		count_trxn=0;
		if ( s[i]->primary != NULL || s[i]->secondary != NULL ) {
			trxn_add (s[i]->rxn, 1.0, FALSE);
			trxn_add (s[i]->rxn, -1.0, TRUE);
		} else {
			trxn_add (s[i]->rxn, 1.0, FALSE);
			rewrite_eqn_to_secondary();
		}
		rxn_free (s[i]->rxn_s);
		s[i]->rxn_s = rxn_alloc(count_trxn+1);
		trxn_copy (s[i]->rxn_s);
		/* calculate alkalinity */
		s[i]->alk = calc_alk(s[i]->rxn_s);
		/* set co2 coefficient */
		s[i]->co2 = 0.0;
		for (j=1; j < count_trxn; ++j) {
			if (trxn.token[j].s == s_co3) {
				s[i]->co2 = trxn.token[j].coef;
				break;
			}
		}
	}
#endif
/*
 *   Set pointer in element to master species
 */
	for (i=0; i < count_elements; ++i) {
		elements[i]->master  = master_bsearch(elements[i]->name);
		if (elements[i]->master == NULL) {
#ifdef SKIP_GUI
			input_error++;
#endif
			sprintf(error_string, "No master species for element %s.", elements[i]->name);
			error_msg(error_string, CONTINUE);
		}
		elements[i]->primary = master_bsearch_primary(elements[i]->name);
		if (elements[i]->primary == NULL) {
#ifdef SKIP_GUI
			input_error++;
#endif
			sprintf(error_string, "No master species for element %s.", elements[i]->name);
			error_msg(error_string, CONTINUE);
		}
	}
/*
 *   Make sure all primary master species for redox elements
 *   are also secondary master species
 */
#ifdef SKIP_GUI
	for (i = 0; i < count_master; ++i) {
		if (master[i]->primary == FALSE) {
			master_ptr = master[i]->s->secondary->elt->primary;
			if (master_ptr->s->secondary == NULL) {
				input_error++;
				sprintf(error_string, "Every primary master species for a redox element\n"
					"\tmust also be a secondary master species.\n"
					"\t%s is the primary master species for element %s.\n"
					"\tAnother entry in SOLUTION_MASTER_SPECIES is needed.\n"
					"\tDefine species %s as a secondary master species for a valence state.\n"
					"\tFor example: \n"
					"\t%s(0)\t%s alk gfw",
					master_ptr->s->name, master_ptr->elt->name, 
					master_ptr->s->name, master_ptr->elt->name, 
					master_ptr->s->name);
				error_msg(error_string, CONTINUE);
			}
		}
	}
#endif
/*
 *   Calculate H and O if alternate mass balance is given
 */ 
#ifdef SKIP_GUI
	for (i = 0; i < count_s; ++i) {
		if (s[i]->next_secondary != NULL) {
			s[i]->h = 0.0;
			s[i]->o = 0.0;
			for (j = 0; s[i]->next_secondary[j].elt != NULL; ++j) {
				if (s[i]->next_secondary[j].elt->primary == NULL) continue;
				if (s[i]->next_secondary[j].elt->primary->s == s_hplus) {
					s[i]->h += s[i]->next_secondary[j].coef;
				} else if (s[i]->next_secondary[j].elt->primary->s == s_h2o) {
					s[i]->o += s[i]->next_secondary[j].coef;
				}
			}
			if (s[i]->type == EX) {
				for (j = 0; s[i]->next_secondary[j].elt != NULL; ++j) {
					if (s[i]->next_secondary[j].elt->primary->s->type == EX) {
						s[i]->equiv = s[i]->next_secondary[j].coef;
						break;
					}
				}
			}
		}
	}
#endif	
	return(OK);
}

/* ---------------------------------------------------------------------- */
int status (int count, const char *str)
/* ---------------------------------------------------------------------- */
{
	static int spinner;
	char sim_str[20];
	char state_str[45];
	char spin_str[2];
/*	char all[MAX_LENGTH]; */
#if defined(PHREEQCI_GUI)
	if (WaitForSingleObject(g_hKill /*g_eventKill*/, 0) == WAIT_OBJECT_0) {
		error_msg("Execution canceled by user.", CONTINUE);
		RaiseException(USER_CANCELED_RUN, 0, 0, NULL);
	}
#endif

	if (pr.status == FALSE) return(OK);
	sprintf(sim_str,"Simulation %d.", simulation);
	sprintf(state_str," ");
	sprintf(spin_str," ");

	if (state == INITIALIZE) {
		output_msg(OUTPUT_SCREEN,"\n%-80s","Initializing...");
// COMMENT: {10/22/2004 9:35:31 PM}#if defined(PHREEQCI_GUI)
// COMMENT: {10/22/2004 9:35:31 PM}		sprintf(g_szLineBuf, "\r\nInitializing...\r\n"); 
// COMMENT: {10/22/2004 9:35:31 PM}		PostMessage(g_status.hText, EM_REPLACESEL, (WPARAM)0, (LPARAM)g_szLineBuf);
// COMMENT: {10/22/2004 9:35:31 PM}#endif /* PHREEQCI_GUI */
		status_on = TRUE;
		return(OK);
	}
/*
 *   If str is defined print it
 */
	if (str != NULL) {
		if (status_on != TRUE) {
			status_on = TRUE;
		}
		if (state != TRANSPORT) {
			sprintf(g_szLineBuf, "%-80s\r\n", str); 
			PostMessage(g_status.hText, EM_REPLACESEL, (WPARAM)0, (LPARAM)g_szLineBuf);
		} else {
			SetWindowText(g_status.hTrans, str);
		}
	} else if (state != TRANSPORT ) {
		if (state == INITIAL_SOLUTION) {
			sprintf(state_str, "Initial solution %d.", use.solution_ptr->n_user);
		} else if (state == INITIAL_EXCHANGE) {
			sprintf(state_str, "Initial exchange %d.", use.exchange_ptr->n_user);
		} else if (state == INITIAL_SURFACE) {
			sprintf(state_str, "Initial surface %d.", use.surface_ptr->n_user);
		} else if (state == INVERSE) {
			sprintf(state_str, "Inverse modeling %d.    Models = %d.", 
				use.inverse_ptr->n_user, count);
		} else if (state == REACTION) {
			if (use.kinetics_in == TRUE) {
				sprintf(state_str, "Kinetic step %d.", reaction_step);
			} else {
				sprintf(state_str, "Reaction step %d.", reaction_step);
			}
		} else if (state == ADVECTION || state == TRANSPORT) {
			if (state == ADVECTION) {
				sprintf(state_str, "Advection, shift %d.", advection_step);
			} else if (state == TRANSPORT) {
				sprintf(state_str, "Transport, shift %d.", transport_step);
			}
			spinner++;
			if (spinner == 1) {
				spin_str[0] = '/';
			} else if (spinner == 2) {
				spin_str[0] = '-';
			} else {
				spin_str[0] = '\\';
				spinner = 0;
			}
		}
		if (status_on != TRUE) {
			status_on = TRUE;
		}
		if (use.kinetics_in == TRUE) {
			sprintf(g_szLineBuf, "%-15s%-27s%38s", sim_str, state_str, " ");
			SetWindowText(g_status.hSim, g_szLineBuf);
		} else {
			sprintf(g_szLineBuf, "%-15s%-27s%1s%37s", sim_str, state_str, spin_str, " ");
			SetWindowText(g_status.hSim, g_szLineBuf);
		}
	}
	return(OK);
}
