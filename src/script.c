#include "script.h"
#include "chibi-stub.h"
#include "main.h"

int chibi_input_fd[2];
FILE *chibi_input_fp[2];

int chibi_output_fd[2];
FILE *chibi_output_fp[2];

sexp chibi_context;

int get_blockreg_val(int reg, int index){
	Block *b;
	switch(reg){
		case 0:
			b = &g->block0;
			break;
		case 1:
			b = &g->block1;
			break;
		case 2:
			b = &g->copy0;
			break;
		case 3:
			b = &g->copy1;
			break;
		default:
			chibi_error("Bad reg value in get-blockreg-value.");
			return -1;
	}

	switch(index){
		case 0:
			return b->x;
		case 1:
			return b->y;
		case 2:
			return b->z;
		case 3:
			return b->w;
		default:
			chibi_error("Bad index value in get-blockreg-value.");
			return -1;
	}
}

void chibi_error(const char *error){
	char *errcmd;

	errcmd = malloc(strlen(error)+80);

	if(errcmd == NULL){
		fprintf(stderr, "The chibi_error function failed to error... awkward.\n");
		return;
	}

	sprintf(errcmd, "(error %s)", errcmd);

	sexp_eval_string(chibi_context, errcmd, -1, NULL);

	sprintf(errcmd, "(add-message %s)", errcmd);

	sexp_eval_string(chibi_context, errcmd, -1, NULL);

	free(errcmd);
	
}

void init_chibi_context(){

	if(pipe(chibi_input_fd))
		fprintf(stderr, "Failed to open chibi_input_fd pipe.\n");
	if(pipe(chibi_output_fd))
		fprintf(stderr, "Failed to open chibi_output_fd pipe.\n");
	if(!(chibi_input_fp[0] = fdopen(chibi_input_fd[0],"r")) ||
	   !(chibi_input_fp[1] = fdopen(chibi_input_fd[1],"w")) ||
	   !(chibi_output_fp[0] = fdopen(chibi_output_fd[0],"r")) ||
	   !(chibi_output_fp[1] = fdopen(chibi_output_fd[1],"w")))
	       fprintf(stderr, "Failed to open one of the chibi io fd.\n");	


	chibi_context = sexp_make_eval_context(NULL, NULL, NULL, 0, 0);
	sexp_load_standard_env(chibi_context, NULL, SEXP_SEVEN);
	sexp_load_standard_ports(chibi_context, NULL, chibi_input_fp[0], chibi_output_fp[1], chibi_output_fp[1], 1);

	sexp env = sexp_context_env(chibi_context);

	sexp_init_library(chibi_context, NULL, 3, env, sexp_version, SEXP_ABI_IDENTIFIER);

	sexp_gc_var1(config_path);
	sexp_gc_preserve1(chibi_context, config_path);

	config_path = sexp_c_string(chibi_context, "craftconf.scm", -1);

	sexp_load(chibi_context, config_path, NULL);

	sexp_gc_release1(chibi_context);

}

void destroy_chibi_context(){
	sexp_destroy_context(chibi_context);
	close(chibi_input_fd[0]);
	close(chibi_input_fd[1]);
	close(chibi_output_fd[0]);
	close(chibi_output_fd[1]);

}
