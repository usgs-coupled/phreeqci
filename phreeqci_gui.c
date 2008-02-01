// phreeqci_gui.c
//
// $Id$
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
