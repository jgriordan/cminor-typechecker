// main.c
// John Riordan

#include <stdio.h>
#include <string.h>

#include "decl.h"

extern int yyparse();
extern struct decl* parser_result;
extern FILE* yyin;
int resolve_failed;
int resolve_print;
int typecheck_failed;
struct type* return_type;

int main( int argc, char* argv[] ){
	if( argc != 3 || ( strcmp( argv[1], "-print" ) && strcmp( argv[1], "-resolve" ) && strcmp(argv[1], "-typecheck" ) ) ){
		printf( "usage: %s -print <filename>\n", argv[0] );
		printf( "usage: %s -resolve <filename>\n", argv[0] );
		printf( "usage: %s -typecheck <filename>\n", argv[0] );
		return 1;
	}

	yyin = fopen( argv[2], "r" );
	if( !yyin ){
		printf( "error: invalid file %s\n", argv[2] );
		return 1;
	}

	// unsuccessful parse- just quit
	if( yyparse() ){
		return 1;
	}

	// successful parse- print
	if( !strcmp( argv[1], "-print" ) ){
		decl_print( parser_result, 0 );
	}

	// successful parse- name resolution
	if( !strcmp( argv[1], "-resolve" ) ){
		scope_init();
		resolve_failed = 0;
		resolve_print = 1;
		decl_resolve( parser_result );
		if( resolve_failed ) return 1;
	}

	if( !strcmp( argv[1], "-typecheck" ) ){
		scope_init();
		resolve_failed = 0;
		resolve_print = 0;
		typecheck_failed = 0;
		decl_resolve( parser_result );
		if( resolve_failed ){
			return 1;
		}
		decl_typecheck( parser_result );
		if( typecheck_failed ){
			return 1;
		}
	}

	return 0;
}
