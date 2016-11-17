// param_list.c
// John Riordan

#include "param_list.h"

struct param_list* param_list_create( char* name, struct type* type, struct param_list* next ){
	struct param_list* p = malloc( sizeof(*p) );
	p->name = name;
	p->type = type;
	p->symbol = 0;
	p->next = next;
	return p;
}

void param_list_print( struct param_list* p ){
	if( !p ) return;
	printf( "%s:", p->name );
	type_print( p->type );
	if( p->next ){
		printf( ", " );
		param_list_print( p->next );
	}
}

int param_list_equal( struct param_list* a, struct param_list* b ){
	if( a && b )
		return type_equal( a->type, b->type ) && param_list_equal( a->next, b->next );
	else if( !a && !b )
		return 1;
	else
		return 0;
}

void param_list_resolve( struct param_list* p ){
	struct symbol* s;
	if( !p ) return;
	s = symbol_create( SYMBOL_PARAM, p->type, p->name );
	scope_bind( p->name, s );
	param_list_resolve( p->next );
	p->symbol = s;
}
