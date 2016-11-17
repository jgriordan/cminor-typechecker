// type.c
// John Riordan

#include "type.h"

struct type* type_create( type_kind_t kind, struct param_list* params, struct type* subtype, struct expr* size ){
	struct type* t = malloc( sizeof(*t) );
	t->kind = kind;
	t->params = params;
	t->subtype = subtype;
	t->size = size;
	return t;
}

void type_print( struct type* t ){	
	const char* typename[] = {
		"boolean",
		"char",
		"integer",
		"string",
		"array",
		"function ",
		"void"
	};
	if( !t ) return;
	printf( "%s", typename[t->kind] );
	if( t->kind == TYPE_ARRAY ){
		printf( " [" );
		expr_print( t->size );
		printf( "] " );
	}
	type_print( t->subtype );
	if( t->kind == TYPE_FUNCTION ){
		printf( " (" );
		param_list_print( t->params );
		printf( ")" );
	}
}

struct type* type_copy( struct type* t ){
	struct type* new_t = malloc( sizeof(*new_t) );
	new_t->kind = t->kind;
	new_t->params = t->params;
	new_t->subtype = t->subtype;
	new_t->size = t->size;
	return new_t;
}

int type_equal( struct type* a, struct type* b ){
	if( a->kind == TYPE_BOOLEAN || a->kind == TYPE_CHARACTER || a->kind == TYPE_INTEGER || a->kind == TYPE_STRING || a->kind == TYPE_VOID )
		return a->kind == b->kind;
	else if( a->kind == TYPE_ARRAY && b->kind == TYPE_ARRAY )
		return type_equal( a->subtype, b->subtype );
	else if( a->kind == TYPE_FUNCTION && b->kind == TYPE_FUNCTION )
		return type_equal( a->subtype, b->subtype ) && param_list_equal( a->params, b->params );
	else
		return 0;
}

void type_delete( struct type* t ){
	free( t );
	t = 0;
}

void type_resolve( struct type* t ){
	if( !t ) return;
	param_list_resolve( t->params );
}
