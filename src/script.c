#include "script.h"

void init_chibi_context(){
	chibi_context = sexp_make_eval_context(NULL, NULL, NULL, 0, 0);
	sexp_load_standard_env(chibi_context, NULL, SEXP_SEVEN);
	sexp_load_standard_ports(chibi_context, NULL, stdin, stdout, stderr, 0);
}
