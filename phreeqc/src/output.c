#define EXTERNAL extern
#include <setjmp.h>
#include "global.h"
#include "output.h"
#include "phrqproto.h"
#include "phqalloc.h"
static char const rcsid[] = "$RCSfile: output.c,v $  $Revision: 1.2 $";

#define MAX_CALLBACKS 10
static struct output_callback output_callbacks[MAX_CALLBACKS];
static size_t count_output_callback = 0;
static int forward_output_to_log = 0;

/* ---------------------------------------------------------------------- */
int add_output_callback(PFN_OUTPUT_CALLBACK pfn, void *cookie)
/* ---------------------------------------------------------------------- */
{
	if (rcsid == NULL) fprintf(stderr," ");
	if (pfn) {
		if (count_output_callback >= MAX_CALLBACKS - 1) {
			sprintf(error_string, "Too many callbacks.\nSee %s\n", __FILE__);
			fprintf(stderr, "%s", error_string);
			error_msg(error_string, STOP);
			return ERROR;
		}
		output_callbacks[count_output_callback].callback = pfn;
		output_callbacks[count_output_callback].cookie = cookie;
		++count_output_callback;
	}
	return OK;
}

/* ---------------------------------------------------------------------- */
int output_message(const int type, const char *err_str, const int stop, const char *format, va_list args)
/* ---------------------------------------------------------------------- */
{
	extern jmp_buf mark;
	size_t i;

	for (i = 0; i < count_output_callback; ++i) {
		(output_callbacks[i].callback)(ACTION_OUTPUT, type, err_str, stop, output_callbacks[i].cookie, format, args);
	}

	if (stop == STOP) {
		longjmp(mark, input_error);
	}
	return OK;
}

/* ---------------------------------------------------------------------- */
int clean_up_output_callbacks(void)
/* ---------------------------------------------------------------------- */
{
	count_output_callback = 0;
	return OK;
}

/* ---------------------------------------------------------------------- */
int error_msg (const char *err_str, const int stop)
/* ---------------------------------------------------------------------- */
{
	va_list args = NULL;
	if (input_error <= 0) input_error = 1;

	return output_message(OUTPUT_ERROR, err_str, stop, "", args);
}

/* ---------------------------------------------------------------------- */
int warning_msg (const char *err_str)
/* ---------------------------------------------------------------------- */
{
	va_list args = NULL;
	int return_value;

	return_value = output_message(OUTPUT_WARNING, err_str, CONTINUE, "", args);
	count_warnings++;
	return (return_value);
}
/* ---------------------------------------------------------------------- */
int output_msg (const int type, const char* format, ...)
/* ---------------------------------------------------------------------- */
{
	int return_value;
	va_list args;

	va_start(args, format);
	return_value = output_message(type, NULL, CONTINUE, format, args);
	va_end(args);

	return(return_value);
}
/* ---------------------------------------------------------------------- */
void set_forward_output_to_log(int value)
/* ---------------------------------------------------------------------- */
{
	forward_output_to_log = value;
}
/* ---------------------------------------------------------------------- */
int get_forward_output_to_log(void)
/* ---------------------------------------------------------------------- */
{
	return forward_output_to_log;
}
/* ---------------------------------------------------------------------- */
int output_fflush(const int type)
/* ---------------------------------------------------------------------- */
{
	size_t i;
	int check;
	for (i = 0; i < count_output_callback; ++i) {
		check = (output_callbacks[i].callback)(ACTION_FLUSH, type, NULL, CONTINUE, output_callbacks[i].cookie, NULL, NULL);
		if (check != OK) return(ERROR);
	}
	return(OK);
}
/* ---------------------------------------------------------------------- */
int output_rewind(const int type)
/* ---------------------------------------------------------------------- */
{
	size_t i;
	int check;
	for (i = 0; i < count_output_callback; ++i) {
		check = (output_callbacks[i].callback)(ACTION_REWIND, type, NULL, CONTINUE, output_callbacks[i].cookie, NULL, NULL);
		if (check != OK) return(ERROR);
	}
	return(OK);
}
/* ---------------------------------------------------------------------- */
int output_close(const int type)
/* ---------------------------------------------------------------------- */
{
	size_t i;
	int check;
	for (i = 0; i < count_output_callback; ++i) {
		check = (output_callbacks[i].callback)(ACTION_CLOSE, type, NULL, CONTINUE, output_callbacks[i].cookie, NULL, NULL);
		if (check != OK) return(ERROR);
	}
	return(OK);
}
/* ---------------------------------------------------------------------- */
int output_open(const int type, const char *file_name)
/* ---------------------------------------------------------------------- */
{
	size_t i;
	int check;
	assert(file_name && strlen(file_name));
	for (i = 0; i < count_output_callback; ++i) {
		check = (output_callbacks[i].callback)(ACTION_OPEN, type, file_name, CONTINUE, output_callbacks[i].cookie, NULL, NULL);
		if (check != OK) return(ERROR);
	}
	return(OK);
}
