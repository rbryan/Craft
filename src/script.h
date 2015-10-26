#ifndef _script_h_
#define _script_h_

#include <chibi/eval.h>

extern sexp chibi_context;

extern int chibi_input_fd[2];
extern FILE *chibi_input_fp[2];

extern int chibi_output_fd[2];
extern FILE *chibi_output_fp[2];

extern void init_chibi_context();
extern void destroy_chibi_context();

extern int get_blockreg_val(int reg, int index);

extern void chibi_error(const char *error);


#endif
