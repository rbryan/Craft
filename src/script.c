#include "script.h"
#include "chibi-stub.h"

sexp chibi_context;

void init_chibi_context(){
	chibi_context = sexp_make_eval_context(NULL, NULL, NULL, 0, 0);
	sexp_load_standard_env(chibi_context, NULL, SEXP_SEVEN);
	sexp_load_standard_ports(chibi_context, NULL, stdin, stdout, stderr, 0);

	sexp env = sexp_context_env(chibi_context);

	sexp_init_library(chibi_context, NULL, 3, env, sexp_version, SEXP_ABI_IDENTIFIER);

	sexp_eval_string(chibi_context, "(add-message \"haha\")", -1, NULL);
}
