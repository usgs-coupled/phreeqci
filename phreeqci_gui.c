// phreeqci_gui.c
//
// $Date: 2004/11/03 06:56:15 $
// $Revision: 1.6 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#define EXTERNAL extern
#include "phreeqc/src/global.h"
#include "phreeqc/src/output.h"	

#if !defined(PHREEQCI_GUI)
#error PHREEQCI_GUI must be defined
#endif

#include "phreeqc/src/phqalloc.h"

extern int clean_up(void);
extern int copy_token (char *token_ptr, char **ptr, int *l);
extern int element_compare(const void *ptr1, const void *ptr2);
extern exchange_compare(const void *ptr1,const void *ptr2);
extern gas_phase_compare(const void *ptr1,const void *ptr2);
extern int pp_assemblage_sort(void);
extern void initialize(void);
extern int main(int argc, char *argv[]);
extern void malloc_error(void);
extern master_compare(const void *ptr1,const void *ptr2);
extern mix_compare(const void *ptr1,const void *ptr2);
extern phase_compare(const void *ptr1,const void *ptr2);
extern s_compare(const void *ptr1,const void *ptr2);
extern struct master *master_bsearch(const char *ptr);
extern struct master *master_bsearch_primary(char *ptr);
extern surface_compare(const void *ptr1,const void *ptr2);

static GUICALLBACK s_gui = { 0, NULL, NULL, NULL };

void check_errors_gui (GUICALLBACK gui);
int copy_lfn_gui (char *token_ptr, char **ptr, int *length);
int error_msg (const char *err_str, const int stop);
void error_msg_gui (const char *err_str);
LONG ExceptionFilter(DWORD dwExceptionCode);
DWORD gui_main(int argc, char *argv[]);
void init_callback_gui(void);
void load_database(GUICALLBACK gui);
void load_database_local(GUICALLBACK gui, int last_simulation);
void load_keyword(GUICALLBACK gui);
int read_input(void);
void set_callback_gui(GUICALLBACK gui);
int tidy_model_gui(void);
int tidy_species_gui(void);
void warning_msg_gui (const char *err_str);

/* Used to return an exception code*/
static DWORD dwException;

// COMMENT: {10/21/2004 2:56:32 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 2:56:32 PM}DWORD gui_main(int argc, char *argv[])
// COMMENT: {10/21/2004 2:56:32 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 2:56:32 PM}{
// COMMENT: {10/21/2004 2:56:32 PM}	DWORD dwReturn;
// COMMENT: {10/21/2004 2:56:32 PM}	
// COMMENT: {10/21/2004 2:56:32 PM}	init_callback_gui();
// COMMENT: {10/21/2004 2:56:32 PM}
// COMMENT: {10/21/2004 2:56:32 PM}	__try
// COMMENT: {10/21/2004 2:56:32 PM}	{
// COMMENT: {10/21/2004 2:56:32 PM}#if defined(NEEDS_FIXING)
// COMMENT: {10/21/2004 2:56:32 PM}		// sanity check
// COMMENT: {10/21/2004 2:56:32 PM}		_ASSERTE( input      == NULL );
// COMMENT: {10/21/2004 2:56:32 PM}		_ASSERTE( output     == NULL );
// COMMENT: {10/21/2004 2:56:32 PM}		_ASSERTE( log_file   == NULL );
// COMMENT: {10/21/2004 2:56:32 PM}		_ASSERTE( punch_file == NULL );
// COMMENT: {10/21/2004 2:56:32 PM}		_ASSERTE( dump_file  == NULL );
// COMMENT: {10/21/2004 2:56:32 PM}		_ASSERTE( error_file == NULL );
// COMMENT: {10/21/2004 2:56:32 PM}#endif
// COMMENT: {10/21/2004 2:56:32 PM}
// COMMENT: {10/21/2004 2:56:32 PM}		// run program
// COMMENT: {10/21/2004 2:56:32 PM}		dwReturn = main( argc, argv );
// COMMENT: {10/21/2004 2:56:32 PM}	}
// COMMENT: {10/21/2004 2:56:32 PM}	__except ( ExceptionFilter(GetExceptionCode()) )
// COMMENT: {10/21/2004 2:56:32 PM}	{
// COMMENT: {10/21/2004 2:56:32 PM}		/* An error occurred or the user pressed cancel during run */
// COMMENT: {10/21/2004 2:56:32 PM}		dwReturn = dwException;
// COMMENT: {10/21/2004 2:56:32 PM}	}
// COMMENT: {10/21/2004 2:56:32 PM}
// COMMENT: {10/21/2004 2:56:32 PM}	return dwReturn;
// COMMENT: {10/21/2004 2:56:32 PM}}

// COMMENT: {10/21/2004 2:57:56 PM}LONG ExceptionFilter(DWORD dwExceptionCode)
// COMMENT: {10/21/2004 2:57:56 PM}{
// COMMENT: {10/21/2004 2:57:56 PM}	LONG lReturn;
// COMMENT: {10/21/2004 2:57:56 PM}	char buffer[200];
// COMMENT: {10/21/2004 2:57:56 PM}
// COMMENT: {10/21/2004 2:57:56 PM}	// save exception code
// COMMENT: {10/21/2004 2:57:56 PM}	dwException = dwExceptionCode;
// COMMENT: {10/21/2004 2:57:56 PM}
// COMMENT: {10/21/2004 2:57:56 PM}	switch (dwExceptionCode)
// COMMENT: {10/21/2004 2:57:56 PM}	{
// COMMENT: {10/21/2004 2:57:56 PM}	case USER_CANCELED_RUN :
// COMMENT: {10/21/2004 2:57:56 PM}		clean_up();
// COMMENT: {10/21/2004 2:57:56 PM}		lReturn = EXCEPTION_EXECUTE_HANDLER;
// COMMENT: {10/21/2004 2:57:56 PM}		break;
// COMMENT: {10/21/2004 2:57:56 PM}	case INPUT_CONTAINS_ERRORS :
// COMMENT: {10/21/2004 2:57:56 PM}		// clean_up() called by error_msg()
// COMMENT: {10/21/2004 2:57:56 PM}		lReturn = EXCEPTION_EXECUTE_HANDLER;
// COMMENT: {10/21/2004 2:57:56 PM}		break;
// COMMENT: {10/21/2004 2:57:56 PM}	default :
// COMMENT: {10/21/2004 2:57:56 PM}		// a hardware exception has occured
// COMMENT: {10/21/2004 2:57:56 PM}		switch (dwExceptionCode) 
// COMMENT: {10/21/2004 2:57:56 PM}		{
// COMMENT: {10/21/2004 2:57:56 PM}		case EXCEPTION_ACCESS_VIOLATION :
// COMMENT: {10/21/2004 2:57:56 PM}#ifdef _DEBUG
// COMMENT: {10/21/2004 2:57:56 PM}			return EXCEPTION_CONTINUE_SEARCH;
// COMMENT: {10/21/2004 2:57:56 PM}#endif
// COMMENT: {10/21/2004 2:57:56 PM}			// handle exception probably a NULL pointer error
// COMMENT: {10/21/2004 2:57:56 PM}			sprintf(buffer, "Handled exception (%u):EXCEPTION_ACCESS_VIOLATION.\nStopping\n", dwException);
// COMMENT: {10/21/2004 2:57:56 PM}			error_msg(buffer, CONTINUE);
// COMMENT: {10/21/2004 2:57:56 PM}			clean_up();
// COMMENT: {10/21/2004 2:57:56 PM}			lReturn = EXCEPTION_EXECUTE_HANDLER;
// COMMENT: {10/21/2004 2:57:56 PM}			break;
// COMMENT: {10/21/2004 2:57:56 PM}		default :
// COMMENT: {10/21/2004 2:57:56 PM}			sprintf(buffer, "Unhandled exception (%u).\nStopping\n", dwException);
// COMMENT: {10/21/2004 2:57:56 PM}			error_msg(buffer, CONTINUE);
// COMMENT: {10/21/2004 2:57:56 PM}			clean_up();
// COMMENT: {10/21/2004 2:57:56 PM}			// let operating system handle error
// COMMENT: {10/21/2004 2:57:56 PM}			lReturn = EXCEPTION_CONTINUE_SEARCH;
// COMMENT: {10/21/2004 2:57:56 PM}		}
// COMMENT: {10/21/2004 2:57:56 PM}	}
// COMMENT: {10/21/2004 2:57:56 PM}	return lReturn;
// COMMENT: {10/21/2004 2:57:56 PM}}

// COMMENT: {10/21/2004 3:03:34 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 3:03:34 PM}int tidy_model_gui(void)
// COMMENT: {10/21/2004 3:03:34 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 3:03:34 PM}{
// COMMENT: {10/21/2004 3:03:34 PM}/*
// COMMENT: {10/21/2004 3:03:34 PM} *   Sort arrays
// COMMENT: {10/21/2004 3:03:34 PM} */
// COMMENT: {10/21/2004 3:03:34 PM}
// COMMENT: {10/21/2004 3:03:34 PM}/* species */
// COMMENT: {10/21/2004 3:03:34 PM}	qsort (s,
// COMMENT: {10/21/2004 3:03:34 PM}	       (size_t) count_s,
// COMMENT: {10/21/2004 3:03:34 PM}	       (size_t) sizeof (struct species *),
// COMMENT: {10/21/2004 3:03:34 PM}	       s_compare);
// COMMENT: {10/21/2004 3:03:34 PM}
// COMMENT: {10/21/2004 3:03:34 PM}/* master species */
// COMMENT: {10/21/2004 3:03:34 PM}	qsort (master, 
// COMMENT: {10/21/2004 3:03:34 PM}	       (unsigned) count_master, 
// COMMENT: {10/21/2004 3:03:34 PM}	       sizeof(struct master *), 
// COMMENT: {10/21/2004 3:03:34 PM}	       master_compare);
// COMMENT: {10/21/2004 3:03:34 PM}
// COMMENT: {10/21/2004 3:03:34 PM}/* elements */
// COMMENT: {10/21/2004 3:03:34 PM}	qsort (elements,
// COMMENT: {10/21/2004 3:03:34 PM}	       (size_t) count_elements,
// COMMENT: {10/21/2004 3:03:34 PM}	       (size_t) sizeof (struct element *),
// COMMENT: {10/21/2004 3:03:34 PM}	       element_compare);
// COMMENT: {10/21/2004 3:03:34 PM}/* phases */
// COMMENT: {10/21/2004 3:03:34 PM}	qsort (phases,
// COMMENT: {10/21/2004 3:03:34 PM}	       (size_t) count_phases,
// COMMENT: {10/21/2004 3:03:34 PM}	       (size_t) sizeof (struct phase *),
// COMMENT: {10/21/2004 3:03:34 PM}	       phase_compare);
// COMMENT: {10/21/2004 3:03:34 PM}	
// COMMENT: {10/21/2004 3:03:34 PM}/* pure_phases */
// COMMENT: {10/21/2004 3:03:34 PM}	pp_assemblage_sort();
// COMMENT: {10/21/2004 3:03:34 PM}
// COMMENT: {10/21/2004 3:03:34 PM}/* exchangers */
// COMMENT: {10/21/2004 3:03:34 PM}	qsort (exchange,
// COMMENT: {10/21/2004 3:03:34 PM}	       (size_t) count_exchange,
// COMMENT: {10/21/2004 3:03:34 PM}	       (size_t) sizeof (struct exchange),
// COMMENT: {10/21/2004 3:03:34 PM}	       exchange_compare);
// COMMENT: {10/21/2004 3:03:34 PM}
// COMMENT: {10/21/2004 3:03:34 PM}/* surfaces */
// COMMENT: {10/21/2004 3:03:34 PM}	qsort (surface,
// COMMENT: {10/21/2004 3:03:34 PM}	       (size_t) count_surface,
// COMMENT: {10/21/2004 3:03:34 PM}	       (size_t) sizeof (struct surface),
// COMMENT: {10/21/2004 3:03:34 PM}	       surface_compare);
// COMMENT: {10/21/2004 3:03:34 PM}
// COMMENT: {10/21/2004 3:03:34 PM}/* mixtures */
// COMMENT: {10/21/2004 3:03:34 PM}	qsort (mix,
// COMMENT: {10/21/2004 3:03:34 PM}	       (size_t) count_mix,
// COMMENT: {10/21/2004 3:03:34 PM}	       (size_t) sizeof (struct mix),
// COMMENT: {10/21/2004 3:03:34 PM}	       mix_compare);
// COMMENT: {10/21/2004 3:03:34 PM}
// COMMENT: {10/21/2004 3:03:34 PM}/* gas_phase */
// COMMENT: {10/21/2004 3:03:34 PM}	qsort (gas_phase,
// COMMENT: {10/21/2004 3:03:34 PM}	       (size_t) count_gas_phase,
// COMMENT: {10/21/2004 3:03:34 PM}	       (size_t) sizeof (struct gas_phase),
// COMMENT: {10/21/2004 3:03:34 PM}	       gas_phase_compare);
// COMMENT: {10/21/2004 3:03:34 PM}/*
// COMMENT: {10/21/2004 3:03:34 PM} *   Check pointers, write reactions for species
// COMMENT: {10/21/2004 3:03:34 PM} */
// COMMENT: {10/21/2004 3:03:34 PM}	tidy_species_gui();
// COMMENT: {10/21/2004 3:03:34 PM}
// COMMENT: {10/21/2004 3:03:34 PM}#ifdef GUI_SKIP
// COMMENT: {10/21/2004 3:03:34 PM}
// COMMENT: {10/21/2004 3:03:34 PM}	tidy_phases();
// COMMENT: {10/21/2004 3:03:34 PM}/*
// COMMENT: {10/21/2004 3:03:34 PM} *   calculate gfw of water, kg/mole
// COMMENT: {10/21/2004 3:03:34 PM} */
// COMMENT: {10/21/2004 3:03:34 PM}	compute_gfw("H2O", &gfw_water);
// COMMENT: {10/21/2004 3:03:34 PM}	gfw_water *= 0.001;
// COMMENT: {10/21/2004 3:03:34 PM}/*
// COMMENT: {10/21/2004 3:03:34 PM} *   tidy surface data
// COMMENT: {10/21/2004 3:03:34 PM} */
// COMMENT: {10/21/2004 3:03:34 PM}	tidy_surface();
// COMMENT: {10/21/2004 3:03:34 PM}/*
// COMMENT: {10/21/2004 3:03:34 PM} *   tidy inverse data
// COMMENT: {10/21/2004 3:03:34 PM} */
// COMMENT: {10/21/2004 3:03:34 PM}	tidy_inverse();
// COMMENT: {10/21/2004 3:03:34 PM}/*
// COMMENT: {10/21/2004 3:03:34 PM} *   tidy gas phase data
// COMMENT: {10/21/2004 3:03:34 PM} */
// COMMENT: {10/21/2004 3:03:34 PM}	tidy_gas_phase();
// COMMENT: {10/21/2004 3:03:34 PM}/*
// COMMENT: {10/21/2004 3:03:34 PM} *   tidy pp_assemblage data
// COMMENT: {10/21/2004 3:03:34 PM} */
// COMMENT: {10/21/2004 3:03:34 PM}	tidy_pp_assemblage();
// COMMENT: {10/21/2004 3:03:34 PM}/*
// COMMENT: {10/21/2004 3:03:34 PM} *   Duplicate reaction
// COMMENT: {10/21/2004 3:03:34 PM} */
// COMMENT: {10/21/2004 3:03:34 PM}	for ( i = 0; i < count_irrev; ++i) {
// COMMENT: {10/21/2004 3:03:34 PM}		if ( irrev[i].n_user_end > irrev[i].n_user ) {
// COMMENT: {10/21/2004 3:03:34 PM}			n_user = irrev[i].n_user;
// COMMENT: {10/21/2004 3:03:34 PM}			last = irrev[i].n_user_end;
// COMMENT: {10/21/2004 3:03:34 PM}			irrev[i].n_user_end = irrev[i].n_user;
// COMMENT: {10/21/2004 3:03:34 PM}			for (j = n_user + 1; j <= last; ++j) {
// COMMENT: {10/21/2004 3:03:34 PM}				irrev_duplicate(n_user, j);
// COMMENT: {10/21/2004 3:03:34 PM}			}
// COMMENT: {10/21/2004 3:03:34 PM}		}
// COMMENT: {10/21/2004 3:03:34 PM}	}
// COMMENT: {10/21/2004 3:03:34 PM}/*
// COMMENT: {10/21/2004 3:03:34 PM} *   Duplicate temperature
// COMMENT: {10/21/2004 3:03:34 PM} */
// COMMENT: {10/21/2004 3:03:34 PM}	for ( i = 0; i < count_temperature; ++i) {
// COMMENT: {10/21/2004 3:03:34 PM}		if ( temperature[i].n_user_end > temperature[i].n_user ) {
// COMMENT: {10/21/2004 3:03:34 PM}			n_user = temperature[i].n_user;
// COMMENT: {10/21/2004 3:03:34 PM}			last = temperature[i].n_user_end;
// COMMENT: {10/21/2004 3:03:34 PM}			temperature[i].n_user_end = temperature[i].n_user;
// COMMENT: {10/21/2004 3:03:34 PM}			for (j = n_user + 1; j <= last; ++j) {
// COMMENT: {10/21/2004 3:03:34 PM}				temperature_duplicate(n_user, j);
// COMMENT: {10/21/2004 3:03:34 PM}			}
// COMMENT: {10/21/2004 3:03:34 PM}		}
// COMMENT: {10/21/2004 3:03:34 PM}	}
// COMMENT: {10/21/2004 3:03:34 PM}/*
// COMMENT: {10/21/2004 3:03:34 PM} *   Tidy punch information
// COMMENT: {10/21/2004 3:03:34 PM} */
// COMMENT: {10/21/2004 3:03:34 PM}	tidy_punch();
// COMMENT: {10/21/2004 3:03:34 PM}
// COMMENT: {10/21/2004 3:03:34 PM}#endif
// COMMENT: {10/21/2004 3:03:34 PM}/*
// COMMENT: {10/21/2004 3:03:34 PM} *   Error check, program termination
// COMMENT: {10/21/2004 3:03:34 PM} */
// COMMENT: {10/21/2004 3:03:34 PM}// COMMENT: {6/17/99 12:57:18 PM}	if (input_error > 0 || parse_error > 0 ) {
// COMMENT: {10/21/2004 3:03:34 PM}// COMMENT: {6/17/99 12:57:18 PM}		fprintf(log_file, "Calculations terminating due to input errors.\n");
// COMMENT: {10/21/2004 3:03:34 PM}// COMMENT: {6/17/99 12:57:18 PM}		error_msg("Calculations terminating due to input errors.", STOP);
// COMMENT: {10/21/2004 3:03:34 PM}// COMMENT: {6/17/99 12:57:18 PM}	}
// COMMENT: {10/21/2004 3:03:34 PM}
// COMMENT: {10/21/2004 3:03:34 PM}	return(OK);
// COMMENT: {10/21/2004 3:03:34 PM}}

// COMMENT: {10/21/2004 3:08:55 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 3:08:55 PM}int tidy_species_gui(void)
// COMMENT: {10/21/2004 3:08:55 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 3:08:55 PM}{
// COMMENT: {10/21/2004 3:08:55 PM}	int i;
// COMMENT: {10/21/2004 3:08:55 PM}#ifdef SKIP_GUI
// COMMENT: {10/21/2004 3:08:55 PM}	struct master *master_ptr;
// COMMENT: {10/21/2004 3:08:55 PM}#endif
// COMMENT: {10/21/2004 3:08:55 PM}/*
// COMMENT: {10/21/2004 3:08:55 PM} *   Make sure species pointers are ok
// COMMENT: {10/21/2004 3:08:55 PM} */
// COMMENT: {10/21/2004 3:08:55 PM}#ifdef SKIP_GUI
// COMMENT: {10/21/2004 3:08:55 PM}	if (check_species_input() == ERROR) {
// COMMENT: {10/21/2004 3:08:55 PM}		error_msg("Calculations terminating due to input errors.", STOP);
// COMMENT: {10/21/2004 3:08:55 PM}	}
// COMMENT: {10/21/2004 3:08:55 PM}#endif
// COMMENT: {10/21/2004 3:08:55 PM}/*
// COMMENT: {10/21/2004 3:08:55 PM} *   Set secondary and primary pointers in species structures
// COMMENT: {10/21/2004 3:08:55 PM} */
// COMMENT: {10/21/2004 3:08:55 PM}	for (i=0; i < count_s; ++i) {
// COMMENT: {10/21/2004 3:08:55 PM}		s[i]->primary=NULL;
// COMMENT: {10/21/2004 3:08:55 PM}		s[i]->secondary=NULL;
// COMMENT: {10/21/2004 3:08:55 PM}	}
// COMMENT: {10/21/2004 3:08:55 PM}	for (i=0; i < count_master; ++i) {
// COMMENT: {10/21/2004 3:08:55 PM}		if (master[i]->primary == TRUE) {
// COMMENT: {10/21/2004 3:08:55 PM}			master[i]->s->primary=master[i];
// COMMENT: {10/21/2004 3:08:55 PM}		} else {
// COMMENT: {10/21/2004 3:08:55 PM}			master[i]->s->secondary=master[i];
// COMMENT: {10/21/2004 3:08:55 PM}		}
// COMMENT: {10/21/2004 3:08:55 PM}		if (strcmp(master[i]->elt->name,"C") == 0) {
// COMMENT: {10/21/2004 3:08:55 PM}			s_co3=master[i]->s;
// COMMENT: {10/21/2004 3:08:55 PM}		}
// COMMENT: {10/21/2004 3:08:55 PM}		if (master[i]->gfw_formula != NULL) {
// COMMENT: {10/21/2004 3:08:55 PM}#ifdef SKIP_GUI
// COMMENT: {10/21/2004 3:08:55 PM}			if(compute_gfw(master[i]->gfw_formula, &master[i]->gfw) == ERROR){
// COMMENT: {10/21/2004 3:08:55 PM}				input_error++;
// COMMENT: {10/21/2004 3:08:55 PM}				sprintf(error_string, "Calculating gfw for master species, %s, formula %s.", 
// COMMENT: {10/21/2004 3:08:55 PM}					master[i]->elt->name, master[i]->gfw_formula);
// COMMENT: {10/21/2004 3:08:55 PM}				error_msg(error_string, CONTINUE);
// COMMENT: {10/21/2004 3:08:55 PM}			}
// COMMENT: {10/21/2004 3:08:55 PM}#endif
// COMMENT: {10/21/2004 3:08:55 PM}		}
// COMMENT: {10/21/2004 3:08:55 PM}	}
// COMMENT: {10/21/2004 3:08:55 PM}/*
// COMMENT: {10/21/2004 3:08:55 PM} *   Write equations for all master species in terms of primary
// COMMENT: {10/21/2004 3:08:55 PM} *   master species, set coefficient of element in master species
// COMMENT: {10/21/2004 3:08:55 PM} */
// COMMENT: {10/21/2004 3:08:55 PM}#ifdef SKIP_GUI
// COMMENT: {10/21/2004 3:08:55 PM}	for (i=0; i < count_master; ++i) {
// COMMENT: {10/21/2004 3:08:55 PM}		count_trxn=0;
// COMMENT: {10/21/2004 3:08:55 PM}		if ( master[i]->s->primary != NULL ) {
// COMMENT: {10/21/2004 3:08:55 PM}			trxn_add(master[i]->s->rxn, 1.0, FALSE);
// COMMENT: {10/21/2004 3:08:55 PM}			trxn_add(master[i]->s->rxn, -1.0, TRUE);
// COMMENT: {10/21/2004 3:08:55 PM}		} else {
// COMMENT: {10/21/2004 3:08:55 PM}			trxn_add(master[i]->s->rxn, 1.0, FALSE);
// COMMENT: {10/21/2004 3:08:55 PM}			rewrite_eqn_to_primary();
// COMMENT: {10/21/2004 3:08:55 PM}		}
// COMMENT: {10/21/2004 3:08:55 PM}		rxn_free(master[i]->rxn_primary);
// COMMENT: {10/21/2004 3:08:55 PM}		master[i]->rxn_primary = rxn_alloc(count_trxn+1);
// COMMENT: {10/21/2004 3:08:55 PM}		trxn_copy (master[i]->rxn_primary);
// COMMENT: {10/21/2004 3:08:55 PM}		master[i]->coef = coef_in_master(master[i]);
// COMMENT: {10/21/2004 3:08:55 PM}	}
// COMMENT: {10/21/2004 3:08:55 PM}/*
// COMMENT: {10/21/2004 3:08:55 PM} *   Rewrite all species to secondary species
// COMMENT: {10/21/2004 3:08:55 PM} */
// COMMENT: {10/21/2004 3:08:55 PM}	for (i=0; i < count_s; ++i) {
// COMMENT: {10/21/2004 3:08:55 PM}		count_trxn=0;
// COMMENT: {10/21/2004 3:08:55 PM}		if ( s[i]->primary != NULL || s[i]->secondary != NULL ) {
// COMMENT: {10/21/2004 3:08:55 PM}			trxn_add (s[i]->rxn, 1.0, FALSE);
// COMMENT: {10/21/2004 3:08:55 PM}			trxn_add (s[i]->rxn, -1.0, TRUE);
// COMMENT: {10/21/2004 3:08:55 PM}		} else {
// COMMENT: {10/21/2004 3:08:55 PM}			trxn_add (s[i]->rxn, 1.0, FALSE);
// COMMENT: {10/21/2004 3:08:55 PM}			rewrite_eqn_to_secondary();
// COMMENT: {10/21/2004 3:08:55 PM}		}
// COMMENT: {10/21/2004 3:08:55 PM}		rxn_free (s[i]->rxn_s);
// COMMENT: {10/21/2004 3:08:55 PM}		s[i]->rxn_s = rxn_alloc(count_trxn+1);
// COMMENT: {10/21/2004 3:08:55 PM}		trxn_copy (s[i]->rxn_s);
// COMMENT: {10/21/2004 3:08:55 PM}		/* calculate alkalinity */
// COMMENT: {10/21/2004 3:08:55 PM}		s[i]->alk = calc_alk(s[i]->rxn_s);
// COMMENT: {10/21/2004 3:08:55 PM}		/* set co2 coefficient */
// COMMENT: {10/21/2004 3:08:55 PM}		s[i]->co2 = 0.0;
// COMMENT: {10/21/2004 3:08:55 PM}		for (j=1; j < count_trxn; ++j) {
// COMMENT: {10/21/2004 3:08:55 PM}			if (trxn.token[j].s == s_co3) {
// COMMENT: {10/21/2004 3:08:55 PM}				s[i]->co2 = trxn.token[j].coef;
// COMMENT: {10/21/2004 3:08:55 PM}				break;
// COMMENT: {10/21/2004 3:08:55 PM}			}
// COMMENT: {10/21/2004 3:08:55 PM}		}
// COMMENT: {10/21/2004 3:08:55 PM}	}
// COMMENT: {10/21/2004 3:08:55 PM}#endif
// COMMENT: {10/21/2004 3:08:55 PM}/*
// COMMENT: {10/21/2004 3:08:55 PM} *   Set pointer in element to master species
// COMMENT: {10/21/2004 3:08:55 PM} */
// COMMENT: {10/21/2004 3:08:55 PM}	for (i=0; i < count_elements; ++i) {
// COMMENT: {10/21/2004 3:08:55 PM}		elements[i]->master  = master_bsearch(elements[i]->name);
// COMMENT: {10/21/2004 3:08:55 PM}		if (elements[i]->master == NULL) {
// COMMENT: {10/21/2004 3:08:55 PM}#ifdef SKIP_GUI
// COMMENT: {10/21/2004 3:08:55 PM}			input_error++;
// COMMENT: {10/21/2004 3:08:55 PM}#endif
// COMMENT: {10/21/2004 3:08:55 PM}			sprintf(error_string, "No master species for element %s.", elements[i]->name);
// COMMENT: {10/21/2004 3:08:55 PM}			error_msg(error_string, CONTINUE);
// COMMENT: {10/21/2004 3:08:55 PM}		}
// COMMENT: {10/21/2004 3:08:55 PM}		elements[i]->primary = master_bsearch_primary(elements[i]->name);
// COMMENT: {10/21/2004 3:08:55 PM}		if (elements[i]->primary == NULL) {
// COMMENT: {10/21/2004 3:08:55 PM}#ifdef SKIP_GUI
// COMMENT: {10/21/2004 3:08:55 PM}			input_error++;
// COMMENT: {10/21/2004 3:08:55 PM}#endif
// COMMENT: {10/21/2004 3:08:55 PM}			sprintf(error_string, "No master species for element %s.", elements[i]->name);
// COMMENT: {10/21/2004 3:08:55 PM}			error_msg(error_string, CONTINUE);
// COMMENT: {10/21/2004 3:08:55 PM}		}
// COMMENT: {10/21/2004 3:08:55 PM}	}
// COMMENT: {10/21/2004 3:08:55 PM}/*
// COMMENT: {10/21/2004 3:08:55 PM} *   Make sure all primary master species for redox elements
// COMMENT: {10/21/2004 3:08:55 PM} *   are also secondary master species
// COMMENT: {10/21/2004 3:08:55 PM} */
// COMMENT: {10/21/2004 3:08:55 PM}#ifdef SKIP_GUI
// COMMENT: {10/21/2004 3:08:55 PM}	for (i = 0; i < count_master; ++i) {
// COMMENT: {10/21/2004 3:08:55 PM}		if (master[i]->primary == FALSE) {
// COMMENT: {10/21/2004 3:08:55 PM}			master_ptr = master[i]->s->secondary->elt->primary;
// COMMENT: {10/21/2004 3:08:55 PM}			if (master_ptr->s->secondary == NULL) {
// COMMENT: {10/21/2004 3:08:55 PM}				input_error++;
// COMMENT: {10/21/2004 3:08:55 PM}				sprintf(error_string, "Every primary master species for a redox element\n"
// COMMENT: {10/21/2004 3:08:55 PM}					"\tmust also be a secondary master species.\n"
// COMMENT: {10/21/2004 3:08:55 PM}					"\t%s is the primary master species for element %s.\n"
// COMMENT: {10/21/2004 3:08:55 PM}					"\tAnother entry in SOLUTION_MASTER_SPECIES is needed.\n"
// COMMENT: {10/21/2004 3:08:55 PM}					"\tDefine species %s as a secondary master species for a valence state.\n"
// COMMENT: {10/21/2004 3:08:55 PM}					"\tFor example: \n"
// COMMENT: {10/21/2004 3:08:55 PM}					"\t%s(0)\t%s alk gfw",
// COMMENT: {10/21/2004 3:08:55 PM}					master_ptr->s->name, master_ptr->elt->name, 
// COMMENT: {10/21/2004 3:08:55 PM}					master_ptr->s->name, master_ptr->elt->name, 
// COMMENT: {10/21/2004 3:08:55 PM}					master_ptr->s->name);
// COMMENT: {10/21/2004 3:08:55 PM}				error_msg(error_string, CONTINUE);
// COMMENT: {10/21/2004 3:08:55 PM}			}
// COMMENT: {10/21/2004 3:08:55 PM}		}
// COMMENT: {10/21/2004 3:08:55 PM}	}
// COMMENT: {10/21/2004 3:08:55 PM}#endif
// COMMENT: {10/21/2004 3:08:55 PM}/*
// COMMENT: {10/21/2004 3:08:55 PM} *   Calculate H and O if alternate mass balance is given
// COMMENT: {10/21/2004 3:08:55 PM} */ 
// COMMENT: {10/21/2004 3:08:55 PM}#ifdef SKIP_GUI
// COMMENT: {10/21/2004 3:08:55 PM}	for (i = 0; i < count_s; ++i) {
// COMMENT: {10/21/2004 3:08:55 PM}		if (s[i]->next_secondary != NULL) {
// COMMENT: {10/21/2004 3:08:55 PM}			s[i]->h = 0.0;
// COMMENT: {10/21/2004 3:08:55 PM}			s[i]->o = 0.0;
// COMMENT: {10/21/2004 3:08:55 PM}			for (j = 0; s[i]->next_secondary[j].elt != NULL; ++j) {
// COMMENT: {10/21/2004 3:08:55 PM}				if (s[i]->next_secondary[j].elt->primary == NULL) continue;
// COMMENT: {10/21/2004 3:08:55 PM}				if (s[i]->next_secondary[j].elt->primary->s == s_hplus) {
// COMMENT: {10/21/2004 3:08:55 PM}					s[i]->h += s[i]->next_secondary[j].coef;
// COMMENT: {10/21/2004 3:08:55 PM}				} else if (s[i]->next_secondary[j].elt->primary->s == s_h2o) {
// COMMENT: {10/21/2004 3:08:55 PM}					s[i]->o += s[i]->next_secondary[j].coef;
// COMMENT: {10/21/2004 3:08:55 PM}				}
// COMMENT: {10/21/2004 3:08:55 PM}			}
// COMMENT: {10/21/2004 3:08:55 PM}			if (s[i]->type == EX) {
// COMMENT: {10/21/2004 3:08:55 PM}				for (j = 0; s[i]->next_secondary[j].elt != NULL; ++j) {
// COMMENT: {10/21/2004 3:08:55 PM}					if (s[i]->next_secondary[j].elt->primary->s->type == EX) {
// COMMENT: {10/21/2004 3:08:55 PM}						s[i]->equiv = s[i]->next_secondary[j].coef;
// COMMENT: {10/21/2004 3:08:55 PM}						break;
// COMMENT: {10/21/2004 3:08:55 PM}					}
// COMMENT: {10/21/2004 3:08:55 PM}				}
// COMMENT: {10/21/2004 3:08:55 PM}			}
// COMMENT: {10/21/2004 3:08:55 PM}		}
// COMMENT: {10/21/2004 3:08:55 PM}	}
// COMMENT: {10/21/2004 3:08:55 PM}#endif	
// COMMENT: {10/21/2004 3:08:55 PM}	return(OK);
// COMMENT: {10/21/2004 3:08:55 PM}}

// COMMENT: {10/21/2004 4:03:40 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 4:03:40 PM}void load_database(GUICALLBACK gui)
// COMMENT: {10/21/2004 4:03:40 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 4:03:40 PM}{
// COMMENT: {10/21/2004 4:03:40 PM}	assert(FALSE); // DEPRECATED
// COMMENT: {10/21/2004 4:03:40 PM}#if defined(NEEDS_FIXING)
// COMMENT: {10/21/2004 4:03:40 PM}	s_gui = gui;
// COMMENT: {10/21/2004 4:03:40 PM}
// COMMENT: {10/21/2004 4:03:40 PM}	initialize();
// COMMENT: {10/21/2004 4:03:40 PM}
// COMMENT: {10/21/2004 4:03:40 PM}	read_input();
// COMMENT: {10/21/2004 4:03:40 PM}
// COMMENT: {10/21/2004 4:03:40 PM}	tidy_model_gui();
// COMMENT: {10/21/2004 4:03:40 PM}	// TODO: What about errors??
// COMMENT: {10/21/2004 4:03:40 PM}#endif
// COMMENT: {10/21/2004 4:03:40 PM}}

// COMMENT: {10/21/2004 4:03:35 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 4:03:35 PM}void load_database_local(GUICALLBACK gui, int last_simulation)
// COMMENT: {10/21/2004 4:03:35 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 4:03:35 PM}{
// COMMENT: {10/21/2004 4:03:35 PM}	assert(FALSE); // DEPRECATED
// COMMENT: {10/21/2004 4:03:35 PM}
// COMMENT: {10/21/2004 4:03:35 PM}#if defined(NEEDS_FIXING)
// COMMENT: {10/21/2004 4:03:35 PM}	int simulation;
// COMMENT: {10/21/2004 4:03:35 PM}#endif
// COMMENT: {10/21/2004 4:03:35 PM}
// COMMENT: {10/21/2004 4:03:35 PM}	s_gui = gui;
// COMMENT: {10/21/2004 4:03:35 PM}
// COMMENT: {10/21/2004 4:03:35 PM}/*
// COMMENT: {10/21/2004 4:03:35 PM} *   Initialize arrays
// COMMENT: {10/21/2004 4:03:35 PM} */
// COMMENT: {10/21/2004 4:03:35 PM}	initialize();
// COMMENT: {10/21/2004 4:03:35 PM}
// COMMENT: {10/21/2004 4:03:35 PM}#if defined(NEEDS_FIXING)
// COMMENT: {10/21/2004 4:03:35 PM}	for (simulation = 1; simulation <= last_simulation; ++simulation) {
// COMMENT: {10/21/2004 4:03:35 PM}		if (read_input() == EOF) break;
// COMMENT: {10/21/2004 4:03:35 PM}	}
// COMMENT: {10/21/2004 4:03:35 PM}
// COMMENT: {10/21/2004 4:03:35 PM}	tidy_model_gui();
// COMMENT: {10/21/2004 4:03:35 PM}	// TODO: What about errors??
// COMMENT: {10/21/2004 4:03:35 PM}#endif
// COMMENT: {10/21/2004 4:03:35 PM}}

// COMMENT: {10/21/2004 4:03:31 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 4:03:31 PM}void load_keyword(GUICALLBACK gui)
// COMMENT: {10/21/2004 4:03:31 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 4:03:31 PM}{
// COMMENT: {10/21/2004 4:03:31 PM}	s_gui = gui;
// COMMENT: {10/21/2004 4:03:31 PM}
// COMMENT: {10/21/2004 4:03:31 PM}	initialize();
// COMMENT: {10/21/2004 4:03:31 PM}
// COMMENT: {10/21/2004 4:03:31 PM}	/* knobs */
// COMMENT: {10/21/2004 4:03:31 PM}	pr.logfile = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	//{{
// COMMENT: {10/21/2004 4:03:31 PM}	debug_diffuse_layer = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	debug_inverse       = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	debug_model         = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	debug_prep          = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	debug_set           = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	delay_mass_water    = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	diagonal_scale      = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	//}}
// COMMENT: {10/21/2004 4:03:31 PM}
// COMMENT: {10/21/2004 4:03:31 PM}	/* print */
// COMMENT: {10/21/2004 4:03:31 PM}	pr.gas_phase          = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	pr.s_s_assemblage     = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	pr.pp_assemblage      = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	pr.surface            = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	pr.exchange           = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	pr.kinetics           = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	pr.totals             = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	pr.eh                 = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	pr.species            = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	pr.saturation_indices = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	pr.irrev              = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	pr.mix                = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	pr.use                = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	pr.punch              = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	pr.status             = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	pr.inverse            = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	pr.dump               = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	pr.user_print         = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	pr.headings           = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	pr.echo_input         = -1;
// COMMENT: {10/21/2004 4:03:31 PM}
// COMMENT: {10/21/2004 4:03:31 PM}	/* punch */
// COMMENT: {10/21/2004 4:03:31 PM}	punch.alk            = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	punch.charge_balance = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	punch.dist           = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	punch.high_precision = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	punch.inverse        = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	punch.mu             = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	punch.pe             = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	punch.percent_error  = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	punch.ph             = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	punch.rxn            = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	punch.sim            = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	punch.soln           = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	punch.state          = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	punch.step           = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	punch.temp           = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	punch.time           = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	punch.user_punch     = -1;
// COMMENT: {10/21/2004 4:03:31 PM}	punch.water          = -1;
// COMMENT: {10/21/2004 4:03:31 PM}
// COMMENT: {10/21/2004 4:03:31 PM}	read_input();
// COMMENT: {10/21/2004 4:03:31 PM}
// COMMENT: {10/21/2004 4:03:31 PM}	/*return 0;*/
// COMMENT: {10/21/2004 4:03:31 PM}}
// COMMENT: {10/21/2004 4:03:31 PM}
// COMMENT: {10/21/2004 4:03:24 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 4:03:24 PM}void check_errors_gui (GUICALLBACK gui)
// COMMENT: {10/21/2004 4:03:24 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 4:03:24 PM}{
// COMMENT: {10/21/2004 4:03:24 PM}	s_gui = gui;
// COMMENT: {10/21/2004 4:03:24 PM}
// COMMENT: {10/21/2004 4:03:24 PM}	initialize();
// COMMENT: {10/21/2004 4:03:24 PM}
// COMMENT: {10/21/2004 4:03:24 PM}#if defined(NEEDS_FIXING)
// COMMENT: {10/21/2004 4:03:24 PM}	for (simulation = 1; ; ++simulation) {
// COMMENT: {10/21/2004 4:03:24 PM}		if (read_input() == EOF) break;
// COMMENT: {10/21/2004 4:03:24 PM}	}
// COMMENT: {10/21/2004 4:03:24 PM}#endif
// COMMENT: {10/21/2004 4:03:24 PM}
// COMMENT: {10/21/2004 4:03:24 PM}	clean_up();
// COMMENT: {10/21/2004 4:03:24 PM}}
// COMMENT: {10/21/2004 4:03:20 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 4:03:20 PM}void init_callback_gui(void)
// COMMENT: {10/21/2004 4:03:20 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 4:03:20 PM}{
// COMMENT: {10/21/2004 4:03:20 PM}	GUICALLBACK null_gui = { 0, NULL, NULL, NULL };
// COMMENT: {10/21/2004 4:03:20 PM}	s_gui = null_gui;
// COMMENT: {10/21/2004 4:03:20 PM}}

// COMMENT: {10/21/2004 4:03:16 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 4:03:16 PM}void set_callback_gui(GUICALLBACK gui)
// COMMENT: {10/21/2004 4:03:16 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 4:03:16 PM}{
// COMMENT: {10/21/2004 4:03:16 PM}	s_gui = gui;
// COMMENT: {10/21/2004 4:03:16 PM}}


// COMMENT: {10/21/2004 4:03:09 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 4:03:09 PM}void warning_msg_gui(const char *err_str)
// COMMENT: {10/21/2004 4:03:09 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 4:03:09 PM}{
// COMMENT: {10/21/2004 4:03:09 PM}	if (s_gui.pfnWarn == NULL)
// COMMENT: {10/21/2004 4:03:09 PM}		return;
// COMMENT: {10/21/2004 4:03:09 PM}	s_gui.pfnWarn(s_gui.dwCookie, err_str);
// COMMENT: {10/21/2004 4:03:09 PM}}
// COMMENT: {10/21/2004 4:03:09 PM}
// COMMENT: {10/21/2004 4:03:09 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 4:03:09 PM}void error_msg_gui(const char *err_str)
// COMMENT: {10/21/2004 4:03:09 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 4:03:09 PM}{
// COMMENT: {10/21/2004 4:03:09 PM}	if (s_gui.pfnError == NULL)
// COMMENT: {10/21/2004 4:03:09 PM}		return;
// COMMENT: {10/21/2004 4:03:09 PM}	s_gui.pfnError(s_gui.dwCookie, err_str);
// COMMENT: {10/21/2004 4:03:09 PM}}

#define DEFAULT_BUFFER_SIZE 300

// COMMENT: {10/21/2004 4:03:04 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 4:03:04 PM}int get_logical_line_gui(FILE *fp, int *l)
// COMMENT: {10/21/2004 4:03:04 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 4:03:04 PM}{
// COMMENT: {10/21/2004 4:03:04 PM}/*
// COMMENT: {10/21/2004 4:03:04 PM} *   Reads file fp until end of line, ";", or eof
// COMMENT: {10/21/2004 4:03:04 PM} *   stores characters in line_save
// COMMENT: {10/21/2004 4:03:04 PM} *   reallocs line_save and line if more space is needed
// COMMENT: {10/21/2004 4:03:04 PM} *
// COMMENT: {10/21/2004 4:03:04 PM} *   returns:
// COMMENT: {10/21/2004 4:03:04 PM} *           EOF on empty line on end of file or
// COMMENT: {10/21/2004 4:03:04 PM} *           OK otherwise
// COMMENT: {10/21/2004 4:03:04 PM} *           *l returns length of line
// COMMENT: {10/21/2004 4:03:04 PM} */
// COMMENT: {10/21/2004 4:03:04 PM}	char buffer[DEFAULT_BUFFER_SIZE];
// COMMENT: {10/21/2004 4:03:04 PM}	long nSize;
// COMMENT: {10/21/2004 4:03:04 PM}	char* large_buffer;
// COMMENT: {10/21/2004 4:03:04 PM}
// COMMENT: {10/21/2004 4:03:04 PM}
// COMMENT: {10/21/2004 4:03:04 PM}	if (s_gui.pfnLine == NULL) {
// COMMENT: {10/21/2004 4:03:04 PM}		return 0;
// COMMENT: {10/21/2004 4:03:04 PM}	}
// COMMENT: {10/21/2004 4:03:04 PM}
// COMMENT: {10/21/2004 4:03:04 PM}	if (s_gui.pfnLine(s_gui.dwCookie, buffer, DEFAULT_BUFFER_SIZE, &nSize))
// COMMENT: {10/21/2004 4:03:04 PM}	{
// COMMENT: {10/21/2004 4:03:04 PM}		if (nSize == 0)
// COMMENT: {10/21/2004 4:03:04 PM}		{
// COMMENT: {10/21/2004 4:03:04 PM}			strcpy(line_save, "");
// COMMENT: {10/21/2004 4:03:04 PM}			*l = 0;
// COMMENT: {10/21/2004 4:03:04 PM}			return(EOF);
// COMMENT: {10/21/2004 4:03:04 PM}		}
// COMMENT: {10/21/2004 4:03:04 PM}		else
// COMMENT: {10/21/2004 4:03:04 PM}		{
// COMMENT: {10/21/2004 4:03:04 PM}			*l = nSize - 1;
// COMMENT: {10/21/2004 4:03:04 PM}			_ASSERTE( *l == (int)strlen(buffer) );
// COMMENT: {10/21/2004 4:03:04 PM}			line_save = (char *) PHRQ_realloc (line_save, (size_t) (*l + 20) * sizeof(char));
// COMMENT: {10/21/2004 4:03:04 PM}			if (line_save == NULL) malloc_error();
// COMMENT: {10/21/2004 4:03:04 PM}			strcpy(line_save, buffer);
// COMMENT: {10/21/2004 4:03:04 PM}			line = (char *) PHRQ_realloc (line, (size_t) (*l + 20) * sizeof(char));
// COMMENT: {10/21/2004 4:03:04 PM}			if (line == NULL) malloc_error();
// COMMENT: {10/21/2004 4:03:04 PM}			// line copied in get_line
// COMMENT: {10/21/2004 4:03:04 PM}			return(OK);
// COMMENT: {10/21/2004 4:03:04 PM}		}
// COMMENT: {10/21/2004 4:03:04 PM}
// COMMENT: {10/21/2004 4:03:04 PM}	}
// COMMENT: {10/21/2004 4:03:04 PM}	else
// COMMENT: {10/21/2004 4:03:04 PM}	{
// COMMENT: {10/21/2004 4:03:04 PM}		large_buffer = PHRQ_malloc((nSize + 1) * sizeof(char));
// COMMENT: {10/21/2004 4:03:04 PM}		if (large_buffer == NULL) malloc_error();
// COMMENT: {10/21/2004 4:03:04 PM}		if (s_gui.pfnLine(s_gui.dwCookie, large_buffer, nSize + 1, &nSize))
// COMMENT: {10/21/2004 4:03:04 PM}		{
// COMMENT: {10/21/2004 4:03:04 PM}			*l = nSize - 1;
// COMMENT: {10/21/2004 4:03:04 PM}			_ASSERTE( *l == (int)strlen(large_buffer) );
// COMMENT: {10/21/2004 4:03:04 PM}			line_save = (char *) PHRQ_realloc (line_save, (size_t) (*l + 20) * sizeof(char));
// COMMENT: {10/21/2004 4:03:04 PM}			if (line_save == NULL) malloc_error();
// COMMENT: {10/21/2004 4:03:04 PM}			strcpy(line_save, large_buffer);
// COMMENT: {10/21/2004 4:03:04 PM}			line = (char *) PHRQ_realloc (line, (size_t) (*l + 20) * sizeof(char));
// COMMENT: {10/21/2004 4:03:04 PM}			if (line == NULL) malloc_error();
// COMMENT: {10/21/2004 4:03:04 PM}			// line copied in get_line
// COMMENT: {10/21/2004 4:03:04 PM}			PHRQ_free(large_buffer);
// COMMENT: {10/21/2004 4:03:04 PM}			return(OK);
// COMMENT: {10/21/2004 4:03:04 PM}		}
// COMMENT: {10/21/2004 4:03:04 PM}		else
// COMMENT: {10/21/2004 4:03:04 PM}		{
// COMMENT: {10/21/2004 4:03:04 PM}			PHRQ_free(large_buffer);
// COMMENT: {10/21/2004 4:03:04 PM}			_ASSERTE(FALSE);
// COMMENT: {10/21/2004 4:03:04 PM}			 malloc_error();
// COMMENT: {10/21/2004 4:03:04 PM}			return(EOF);
// COMMENT: {10/21/2004 4:03:04 PM}		}
// COMMENT: {10/21/2004 4:03:04 PM}	}
// COMMENT: {10/21/2004 4:03:04 PM}}

// COMMENT: {10/21/2004 4:02:57 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 4:02:57 PM}int error_checking_gui(void)
// COMMENT: {10/21/2004 4:02:57 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 4:02:57 PM}{
// COMMENT: {10/21/2004 4:02:57 PM}/*
// COMMENT: {10/21/2004 4:02:57 PM} *   returns:
// COMMENT: {10/21/2004 4:02:57 PM} *           TRUE if error checking
// COMMENT: {10/21/2004 4:02:57 PM} *           FALSE otherwise
// COMMENT: {10/21/2004 4:02:57 PM} */
// COMMENT: {10/21/2004 4:02:57 PM}	return (s_gui.pfnLine == NULL) ? FALSE : TRUE;
// COMMENT: {10/21/2004 4:02:57 PM}}

// COMMENT: {10/21/2004 4:02:52 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 4:02:52 PM}int copy_lfn_gui (char *token_ptr, char **ptr, int *length)
// COMMENT: {10/21/2004 4:02:52 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 4:02:52 PM}{
// COMMENT: {10/21/2004 4:02:52 PM}/*
// COMMENT: {10/21/2004 4:02:52 PM} *   Copies from **ptr to *token_ptr until first space is encountered.
// COMMENT: {10/21/2004 4:02:52 PM} *
// COMMENT: {10/21/2004 4:02:52 PM} *   Arguments:
// COMMENT: {10/21/2004 4:02:52 PM} *      *token_ptr  output, place to store token
// COMMENT: {10/21/2004 4:02:52 PM} *
// COMMENT: {10/21/2004 4:02:52 PM} *     **ptr        input, character string to read token from
// COMMENT: {10/21/2004 4:02:52 PM} *                  output, next position after token
// COMMENT: {10/21/2004 4:02:52 PM} *
// COMMENT: {10/21/2004 4:02:52 PM} *       length     output, length of token
// COMMENT: {10/21/2004 4:02:52 PM} *
// COMMENT: {10/21/2004 4:02:52 PM} *   Returns:
// COMMENT: {10/21/2004 4:02:52 PM} *      UPPER,
// COMMENT: {10/21/2004 4:02:52 PM} *      LOWER,
// COMMENT: {10/21/2004 4:02:52 PM} *      DIGIT,
// COMMENT: {10/21/2004 4:02:52 PM} *      EMPTY,
// COMMENT: {10/21/2004 4:02:52 PM} *      UNKNOWN.
// COMMENT: {10/21/2004 4:02:52 PM} */
// COMMENT: {10/21/2004 4:02:52 PM}	int i, return_value;
// COMMENT: {10/21/2004 4:02:52 PM}	char c;
// COMMENT: {10/21/2004 4:02:52 PM}
// COMMENT: {10/21/2004 4:02:52 PM}/*
// COMMENT: {10/21/2004 4:02:52 PM} *   Read to end of whitespace
// COMMENT: {10/21/2004 4:02:52 PM} */
// COMMENT: {10/21/2004 4:02:52 PM}	while ( isspace((int) (c=**ptr)) ) (*ptr)++;
// COMMENT: {10/21/2004 4:02:52 PM}/*
// COMMENT: {10/21/2004 4:02:52 PM} *   Check what we have
// COMMENT: {10/21/2004 4:02:52 PM} */
// COMMENT: {10/21/2004 4:02:52 PM}	if ( isupper((int) c) ) {
// COMMENT: {10/21/2004 4:02:52 PM}		return_value=UPPER;
// COMMENT: {10/21/2004 4:02:52 PM}	} else if ( islower((int) c) ) {
// COMMENT: {10/21/2004 4:02:52 PM}		return_value=LOWER;
// COMMENT: {10/21/2004 4:02:52 PM}	} else if ( isdigit((int) c) || c=='.' || c=='-') {
// COMMENT: {10/21/2004 4:02:52 PM}		return_value=DIGIT;
// COMMENT: {10/21/2004 4:02:52 PM}	} else if ( c == '\0') {
// COMMENT: {10/21/2004 4:02:52 PM}		return_value=EMPTY;
// COMMENT: {10/21/2004 4:02:52 PM}	} else {
// COMMENT: {10/21/2004 4:02:52 PM}		return_value=UNKNOWN;
// COMMENT: {10/21/2004 4:02:52 PM}	}
// COMMENT: {10/21/2004 4:02:52 PM}/*
// COMMENT: {10/21/2004 4:02:52 PM} *   Begin copying to token
// COMMENT: {10/21/2004 4:02:52 PM} */
// COMMENT: {10/21/2004 4:02:52 PM}	i=0;
// COMMENT: {10/21/2004 4:02:52 PM}	while ( ( ! isspace ((int) (c=**ptr)) || c == ' ') &&
// COMMENT: {10/21/2004 4:02:52 PM}		c != ',' &&
// COMMENT: {10/21/2004 4:02:52 PM}		c != ';' &&
// COMMENT: {10/21/2004 4:02:52 PM}		c != '\0' ) {
// COMMENT: {10/21/2004 4:02:52 PM}		token_ptr[i]=c;
// COMMENT: {10/21/2004 4:02:52 PM}		(*ptr)++;
// COMMENT: {10/21/2004 4:02:52 PM}		i++;
// COMMENT: {10/21/2004 4:02:52 PM}	}
// COMMENT: {10/21/2004 4:02:52 PM}	token_ptr[i]='\0';
// COMMENT: {10/21/2004 4:02:52 PM}	*length=i;
// COMMENT: {10/21/2004 4:02:52 PM}	return(return_value);
// COMMENT: {10/21/2004 4:02:52 PM}}

// COMMENT: {10/21/2004 4:06:14 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 4:06:14 PM}int status (int count, const char *str)
// COMMENT: {10/21/2004 4:06:14 PM}/* ---------------------------------------------------------------------- */
// COMMENT: {10/21/2004 4:06:14 PM}{
// COMMENT: {10/21/2004 4:06:14 PM}	static int spinner;
// COMMENT: {10/21/2004 4:06:14 PM}	char sim_str[20];
// COMMENT: {10/21/2004 4:06:14 PM}	char state_str[45];
// COMMENT: {10/21/2004 4:06:14 PM}	char spin_str[2];
// COMMENT: {10/21/2004 4:06:14 PM}/*	char all[MAX_LENGTH]; */
// COMMENT: {10/21/2004 4:06:14 PM}#if defined(PHREEQCI_GUI)
// COMMENT: {10/21/2004 4:06:14 PM}	if (WaitForSingleObject(g_hKill /*g_eventKill*/, 0) == WAIT_OBJECT_0) {
// COMMENT: {10/21/2004 4:06:14 PM}		error_msg("Execution canceled by user.", CONTINUE);
// COMMENT: {10/21/2004 4:06:14 PM}		RaiseException(USER_CANCELED_RUN, 0, 0, NULL);
// COMMENT: {10/21/2004 4:06:14 PM}	}
// COMMENT: {10/21/2004 4:06:14 PM}#endif
// COMMENT: {10/21/2004 4:06:14 PM}#ifdef PHREEQ98
// COMMENT: {10/21/2004 4:06:14 PM}    if (ProcessMessages) ApplicationProcessMessages();
// COMMENT: {10/21/2004 4:06:14 PM}    if (stop_calculations == TRUE) error_msg("Execution canceled by user.", STOP);
// COMMENT: {10/21/2004 4:06:14 PM}#endif
// COMMENT: {10/21/2004 4:06:14 PM}
// COMMENT: {10/21/2004 4:06:14 PM}	if (pr.status == FALSE) return(OK);
// COMMENT: {10/21/2004 4:06:14 PM}	sprintf(sim_str,"Simulation %d.", simulation);
// COMMENT: {10/21/2004 4:06:14 PM}	sprintf(state_str," ");
// COMMENT: {10/21/2004 4:06:14 PM}	sprintf(spin_str," ");
// COMMENT: {10/21/2004 4:06:14 PM}
// COMMENT: {10/21/2004 4:06:14 PM}	if (state == INITIALIZE) {
// COMMENT: {10/21/2004 4:06:14 PM}#ifndef PHREEQCI_GUI
// COMMENT: {10/21/2004 4:06:14 PM}		fprintf(error_file,"\n%-80s","Initializing...");
// COMMENT: {10/21/2004 4:06:14 PM}#endif /* PHREEQCI_GUI */
// COMMENT: {10/21/2004 4:06:14 PM}#if defined(PHREEQCI_GUI)
// COMMENT: {10/21/2004 4:06:14 PM}		//{{
// COMMENT: {10/21/2004 4:06:14 PM}		////StatusError("Initializing...");
// COMMENT: {10/21/2004 4:06:14 PM}		//}}
// COMMENT: {10/21/2004 4:06:14 PM}		sprintf(g_szLineBuf, "\r\nInitializing...\r\n"); 
// COMMENT: {10/21/2004 4:06:14 PM}		PostMessage(g_status.hText, EM_REPLACESEL, (WPARAM)0, (LPARAM)g_szLineBuf);
// COMMENT: {10/21/2004 4:06:14 PM}#endif /* PHREEQCI_GUI */
// COMMENT: {10/21/2004 4:06:14 PM}#ifdef PHREEQ98
// COMMENT: {10/21/2004 4:06:14 PM}        if ((ShowProgress == TRUE) && (ShowProgressWindow == TRUE)) Status98("Initializing...","","");
// COMMENT: {10/21/2004 4:06:14 PM}        if ((ShowProgress == TRUE) && (ShowChart == TRUE)) Status98B("Initializing...","","");
// COMMENT: {10/21/2004 4:06:14 PM}#endif
// COMMENT: {10/21/2004 4:06:14 PM}
// COMMENT: {10/21/2004 4:06:14 PM}		status_on = TRUE;
// COMMENT: {10/21/2004 4:06:14 PM}		return(OK);
// COMMENT: {10/21/2004 4:06:14 PM}	}
// COMMENT: {10/21/2004 4:06:14 PM}/*
// COMMENT: {10/21/2004 4:06:14 PM} *   If str is defined print it
// COMMENT: {10/21/2004 4:06:14 PM} */
// COMMENT: {10/21/2004 4:06:14 PM}	if (str != NULL) {
// COMMENT: {10/21/2004 4:06:14 PM}		if (status_on == TRUE) {
// COMMENT: {10/21/2004 4:06:14 PM}#ifndef PHREEQCI_GUI
// COMMENT: {10/21/2004 4:06:14 PM}			backspace(error_file, 80);
// COMMENT: {10/21/2004 4:06:14 PM}#endif /* PHREEQCI_GUI */
// COMMENT: {10/21/2004 4:06:14 PM}		} else {
// COMMENT: {10/21/2004 4:06:14 PM}			status_on = TRUE;
// COMMENT: {10/21/2004 4:06:14 PM}		}
// COMMENT: {10/21/2004 4:06:14 PM}#ifndef PHREEQCI_GUI
// COMMENT: {10/21/2004 4:06:14 PM}#ifdef DOS
// COMMENT: {10/21/2004 4:06:14 PM}		backspace(error_file, 80);
// COMMENT: {10/21/2004 4:06:14 PM}		/* if (state == TRANSPORT ) backspace(error_file, 80); */
// COMMENT: {10/21/2004 4:06:14 PM}		fprintf(error_file, "%-79s", str);
// COMMENT: {10/21/2004 4:06:14 PM}#else
// COMMENT: {10/21/2004 4:06:14 PM}		fprintf(error_file, "%-80s", str);
// COMMENT: {10/21/2004 4:06:14 PM}#endif
// COMMENT: {10/21/2004 4:06:14 PM}#endif /* PHREEQCI_GUI */
// COMMENT: {10/21/2004 4:06:14 PM}#if defined(PHREEQCI_GUI)
// COMMENT: {10/21/2004 4:06:14 PM}		if (state != TRANSPORT) {
// COMMENT: {10/21/2004 4:06:14 PM}			//{{
// COMMENT: {10/21/2004 4:06:14 PM}			////StatusError(str);
// COMMENT: {10/21/2004 4:06:14 PM}			//}}
// COMMENT: {10/21/2004 4:06:14 PM}			sprintf(g_szLineBuf, "%-80s\r\n", str); 
// COMMENT: {10/21/2004 4:06:14 PM}			PostMessage(g_status.hText, EM_REPLACESEL, (WPARAM)0, (LPARAM)g_szLineBuf);
// COMMENT: {10/21/2004 4:06:14 PM}		} else {
// COMMENT: {10/21/2004 4:06:14 PM}			//{{
// COMMENT: {10/21/2004 4:06:14 PM}			////StatusTrans(str);
// COMMENT: {10/21/2004 4:06:14 PM}			//}}
// COMMENT: {10/21/2004 4:06:14 PM}			SetWindowText(g_status.hTrans, str);
// COMMENT: {10/21/2004 4:06:14 PM}// COMMENT: {5/3/2000 12:29:36 PM}			PostMessage(g_status.hTrans, WM_SETTEXT, 0, (LPARAM)str);
// COMMENT: {10/21/2004 4:06:14 PM}		}
// COMMENT: {10/21/2004 4:06:14 PM}#endif
// COMMENT: {10/21/2004 4:06:14 PM}#ifdef PHREEQ98
// COMMENT: {10/21/2004 4:06:14 PM}        if ((ShowProgress == TRUE) && (ShowProgressWindow == TRUE)) Status98T((char*)str,"","");
// COMMENT: {10/21/2004 4:06:14 PM}        if ((ShowProgress == TRUE) && (ShowChart == TRUE)) Status98B((char*)str,"","");
// COMMENT: {10/21/2004 4:06:14 PM}#endif
// COMMENT: {10/21/2004 4:06:14 PM}	} else if (state != TRANSPORT ) {
// COMMENT: {10/21/2004 4:06:14 PM}		if (state == INITIAL_SOLUTION) {
// COMMENT: {10/21/2004 4:06:14 PM}			sprintf(state_str, "Initial solution %d.", use.solution_ptr->n_user);
// COMMENT: {10/21/2004 4:06:14 PM}		} else if (state == INITIAL_EXCHANGE) {
// COMMENT: {10/21/2004 4:06:14 PM}			sprintf(state_str, "Initial exchange %d.", use.exchange_ptr->n_user);
// COMMENT: {10/21/2004 4:06:14 PM}		} else if (state == INITIAL_SURFACE) {
// COMMENT: {10/21/2004 4:06:14 PM}			sprintf(state_str, "Initial surface %d.", use.surface_ptr->n_user);
// COMMENT: {10/21/2004 4:06:14 PM}		} else if (state == INVERSE) {
// COMMENT: {10/21/2004 4:06:14 PM}			sprintf(state_str, "Inverse modeling %d.    Models = %d.", 
// COMMENT: {10/21/2004 4:06:14 PM}				use.inverse_ptr->n_user, count);
// COMMENT: {10/21/2004 4:06:14 PM}		} else if (state == REACTION) {
// COMMENT: {10/21/2004 4:06:14 PM}			if (use.kinetics_in == TRUE) {
// COMMENT: {10/21/2004 4:06:14 PM}				sprintf(state_str, "Kinetic step %d.", reaction_step);
// COMMENT: {10/21/2004 4:06:14 PM}			} else {
// COMMENT: {10/21/2004 4:06:14 PM}				sprintf(state_str, "Reaction step %d.", reaction_step);
// COMMENT: {10/21/2004 4:06:14 PM}			}
// COMMENT: {10/21/2004 4:06:14 PM}		} else if (state == ADVECTION || state == TRANSPORT) {
// COMMENT: {10/21/2004 4:06:14 PM}			if (state == ADVECTION) {
// COMMENT: {10/21/2004 4:06:14 PM}				sprintf(state_str, "Advection, shift %d.", advection_step);
// COMMENT: {10/21/2004 4:06:14 PM}			} else if (state == TRANSPORT) {
// COMMENT: {10/21/2004 4:06:14 PM}				sprintf(state_str, "Transport, shift %d.", transport_step);
// COMMENT: {10/21/2004 4:06:14 PM}			}
// COMMENT: {10/21/2004 4:06:14 PM}			spinner++;
// COMMENT: {10/21/2004 4:06:14 PM}			if (spinner == 1) {
// COMMENT: {10/21/2004 4:06:14 PM}				spin_str[0] = '/';
// COMMENT: {10/21/2004 4:06:14 PM}			} else if (spinner == 2) {
// COMMENT: {10/21/2004 4:06:14 PM}				spin_str[0] = '-';
// COMMENT: {10/21/2004 4:06:14 PM}			} else {
// COMMENT: {10/21/2004 4:06:14 PM}				spin_str[0] = '\\';
// COMMENT: {10/21/2004 4:06:14 PM}				spinner = 0;
// COMMENT: {10/21/2004 4:06:14 PM}			}
// COMMENT: {10/21/2004 4:06:14 PM}		}
// COMMENT: {10/21/2004 4:06:14 PM}		if (status_on == TRUE) {
// COMMENT: {10/21/2004 4:06:14 PM}#ifndef PHREEQCI_GUI
// COMMENT: {10/21/2004 4:06:14 PM}			backspace(error_file, 80);
// COMMENT: {10/21/2004 4:06:14 PM}#endif /* PHREEQCI_GUI */
// COMMENT: {10/21/2004 4:06:14 PM}		} else {
// COMMENT: {10/21/2004 4:06:14 PM}			status_on = TRUE;
// COMMENT: {10/21/2004 4:06:14 PM}		}
// COMMENT: {10/21/2004 4:06:14 PM}		if (use.kinetics_in == TRUE) {
// COMMENT: {10/21/2004 4:06:14 PM}#ifdef DOS
// COMMENT: {10/21/2004 4:06:14 PM}			backspace(error_file, 80);
// COMMENT: {10/21/2004 4:06:14 PM}			fprintf(error_file, "%-15s%-27s%37s", sim_str, state_str, " ");
// COMMENT: {10/21/2004 4:06:14 PM}#else
// COMMENT: {10/21/2004 4:06:14 PM}// COMMENT: {2/18/2003 3:12:32 PM}			fprintf(error_file, "%-15s%-27s%38s", sim_str, state_str, " ");
// COMMENT: {10/21/2004 4:06:14 PM}#endif
// COMMENT: {10/21/2004 4:06:14 PM}#if defined(PHREEQCI_GUI)
// COMMENT: {10/21/2004 4:06:14 PM}			//{{
// COMMENT: {10/21/2004 4:06:14 PM}			// prototype
// COMMENT: {10/21/2004 4:06:14 PM}			// void StatusSim(const char *, ...);
// COMMENT: {10/21/2004 4:06:14 PM}			////StatusSim("%-15s%-27s%38s", sim_str, state_str, " ");
// COMMENT: {10/21/2004 4:06:14 PM}			//}}
// COMMENT: {10/21/2004 4:06:14 PM}			sprintf(g_szLineBuf, "%-15s%-27s%38s", sim_str, state_str, " ");
// COMMENT: {10/21/2004 4:06:14 PM}			SetWindowText(g_status.hSim, g_szLineBuf);
// COMMENT: {10/21/2004 4:06:14 PM}// COMMENT: {5/3/2000 12:30:30 PM}			PostMessage(g_status.hSim, WM_SETTEXT, 0, (LPARAM)g_szLineBuf);
// COMMENT: {10/21/2004 4:06:14 PM}#endif /* PHREEQCI_GUI */
// COMMENT: {10/21/2004 4:06:14 PM}#ifdef PHREEQ98
// COMMENT: {10/21/2004 4:06:14 PM}            if ((ShowProgress == TRUE) && (ShowProgressWindow == TRUE)) Status98(sim_str,state_str,"");
// COMMENT: {10/21/2004 4:06:14 PM}            if ((ShowProgress == TRUE) && (ShowChart == TRUE)) Status98B(sim_str,state_str,"");
// COMMENT: {10/21/2004 4:06:14 PM}#endif
// COMMENT: {10/21/2004 4:06:14 PM}
// COMMENT: {10/21/2004 4:06:14 PM}		} else {
// COMMENT: {10/21/2004 4:06:14 PM}#ifndef PHREEQCI_GUI
// COMMENT: {10/21/2004 4:06:14 PM}#ifdef DOS
// COMMENT: {10/21/2004 4:06:14 PM}			backspace(error_file, 80);
// COMMENT: {10/21/2004 4:06:14 PM}			fprintf(error_file, "%-15s%-27s%1s%36s", sim_str, state_str, spin_str, " ");
// COMMENT: {10/21/2004 4:06:14 PM}#else
// COMMENT: {10/21/2004 4:06:14 PM}			fprintf(error_file, "%-15s%-27s%1s%37s", sim_str, state_str, spin_str, " ");
// COMMENT: {10/21/2004 4:06:14 PM}#endif
// COMMENT: {10/21/2004 4:06:14 PM}#endif /* PHREEQCI_GUI */
// COMMENT: {10/21/2004 4:06:14 PM}#if defined(PHREEQCI_GUI)
// COMMENT: {10/21/2004 4:06:14 PM}			//{{
// COMMENT: {10/21/2004 4:06:14 PM}			////StatusSim("%-15s%-27s%1s%37s", sim_str, state_str, spin_str, " ");
// COMMENT: {10/21/2004 4:06:14 PM}			//}}
// COMMENT: {10/21/2004 4:06:14 PM}			sprintf(g_szLineBuf, "%-15s%-27s%1s%37s", sim_str, state_str, spin_str, " ");
// COMMENT: {10/21/2004 4:06:14 PM}			SetWindowText(g_status.hSim, g_szLineBuf);
// COMMENT: {10/21/2004 4:06:14 PM}// COMMENT: {5/3/2000 12:29:20 PM}			PostMessage(g_status.hSim, WM_SETTEXT, 0, (LPARAM)g_szLineBuf);
// COMMENT: {10/21/2004 4:06:14 PM}#endif /* PHREEQCI_GUI */
// COMMENT: {10/21/2004 4:06:14 PM}#ifdef PHREEQ98
// COMMENT: {10/21/2004 4:06:14 PM}            if ((ShowProgress == TRUE) && (ShowProgressWindow == TRUE)) Status98(sim_str, state_str, spin_str);
// COMMENT: {10/21/2004 4:06:14 PM}            if ((ShowProgress == TRUE) && (ShowChart == TRUE)) Status98B(sim_str, state_str, spin_str);
// COMMENT: {10/21/2004 4:06:14 PM}#endif
// COMMENT: {10/21/2004 4:06:14 PM}		}
// COMMENT: {10/21/2004 4:06:14 PM}	}
// COMMENT: {10/21/2004 4:06:14 PM}	return(OK);
// COMMENT: {10/21/2004 4:06:14 PM}}
