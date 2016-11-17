// decl.c
// John Riordan

#include "decl.h"

struct decl* decl_create( char* name, struct type* t, struct expr* v, struct stmt* c, struct decl* next ){
	struct decl* d = malloc( sizeof(*d) );
	d->name = name;
	d->type = t;
	d->value = v;
	d->code = c;
	d->symbol = 0;
	d->next = next;
	return d;
}

void decl_print( struct decl* d, int indent ){
	int i;
	if( !d ) return;
	for( i=0; i<indent; i++ )
		printf( "\t" );
	printf( "%s: ", d->name );
	type_print( d->type );
	if( d->code ){
		printf( " =\n{\n" );
		stmt_print( d->code, indent+1 );
		printf( "}\n\n" );
		decl_print( d->next, indent );
		return;
	}
	if( d->value ){
		printf( " = " );
		expr_print( d->value );
	}
	printf( ";\n" );
	decl_print( d->next, indent );
}

void decl_resolve( struct decl* d ){
	int level;
	struct symbol* s;
	if( !d ) return;
	level = scope_level();
	if( !level )
		s = symbol_create( SYMBOL_GLOBAL, d->type, d->name );
	else
		s = symbol_create( SYMBOL_LOCAL, d->type, d->name );
	scope_bind( d->name, s );
	expr_resolve( d->value );
	scope_enter();
	type_resolve( d->type );
	stmt_resolve( d->code );
	scope_leave();
	decl_resolve( d->next );
	d->symbol = s;
}

void decl_typecheck( struct decl* d ){
	struct type* t;
	if( !d ) return;
	if( d->value ){
		t = expr_typecheck( d->value );
		if( !type_equal( d->type, t ) ){
			printf( "type error: cannot assign " );
			type_print( t );
			printf( " (" );
			expr_print( d->value );
			printf( ") to " );
			type_print( d->type );
			printf( " (%s)\n", d->name );
			typecheck_failed = 1;
		}
	}
	if( d->code ){
		return_type = scope_lookup( d->name )->type->subtype;
		stmt_typecheck( d->code );
	}
	decl_typecheck( d->next );
}
