// stmt.c
// John Riordan

#include "stmt.h"

struct stmt* stmt_create( stmt_kind_t kind, struct decl* d, struct expr* init_expr, struct expr* e, struct expr* next_expr, struct stmt* body, struct stmt* else_body, struct stmt* next ){
	struct stmt* s = malloc( sizeof(*s) );
	s->kind = kind;
	s->decl = d;
	s->init_expr = init_expr;
	s->expr = e;
	s->next_expr = next_expr;
	s->body = body;
	s->else_body = else_body;
	s->next = next;
	return s;
}

void stmt_indent( int indent ){
	int i;
	for( i=0; i<indent; i++ )
		printf( "\t" );
}

void stmt_print( struct stmt* s, int indent ){
	if( !s ) return;
	switch( s->kind ){
		case STMT_DECL:
			decl_print( s->decl, indent );
			break;
		case STMT_EXPR:
			stmt_indent( indent );
			expr_print( s->expr );
			printf( ";\n" );
			break;
		case STMT_IF_ELSE:
			stmt_indent( indent );
			printf( "if( " );
			expr_print( s->expr );
			printf( " )\n" );
			if( s->body ){
				if( s->body->kind == STMT_BLOCK )
					stmt_print( s->body, indent );
				else
					stmt_print( s->body, indent+1 );
			}
			if( s->else_body ){
				stmt_indent( indent );
				printf( "else\n" );
				if( s->else_body->kind == STMT_BLOCK )
					stmt_print( s->else_body, indent );
				else
					stmt_print( s->else_body, indent+1 );
			}
			break;
		case STMT_FOR:
			stmt_indent( indent );
			printf( "for( " );
			expr_print( s->init_expr );
			printf( "; " );
			expr_print( s->expr );
			printf( "; " );
			expr_print( s->next_expr );
			printf( " )\n" );
			if( s->body->kind == STMT_BLOCK )
				stmt_print( s->body, indent );
			else
				stmt_print( s->body, indent+1 );
			break;
		case STMT_PRINT:
			stmt_indent( indent );
			printf( "print" );
			if( s->expr ){
				printf( " " );
				expr_print( s->expr );
			}
			printf( ";\n" );
			break;
		case STMT_RETURN:
			stmt_indent( indent );
			printf( "return" );
			if( s->expr ){
				printf( " " );
				expr_print( s->expr );
			}
			printf( ";\n" );
			break;
		case STMT_BLOCK:
			stmt_indent( indent );
			printf( "{\n" );
			stmt_print( s->body, indent+1 );
			stmt_indent( indent );
			printf( "}\n" );
			break;
	}
	stmt_print( s->next, indent );
}

void stmt_resolve( struct stmt* s ){
	if( !s ) return;
	decl_resolve( s->decl );
	expr_resolve( s->init_expr );
	expr_resolve( s->expr );
	expr_resolve( s->next_expr );
	scope_enter();
	stmt_resolve( s->body );
	scope_leave();
	scope_enter();
	stmt_resolve( s->else_body );
	scope_leave();
	stmt_resolve( s->next );
}

void stmt_typecheck( struct stmt* s ){
	struct type* t;
	if( !s ) return;
	switch( s->kind ){
		case STMT_DECL:
			decl_typecheck( s->decl );
			break;
		case STMT_EXPR:
		case STMT_PRINT:
			expr_typecheck( s->expr );
			break;
		case STMT_IF_ELSE:
			t = expr_typecheck( s->expr );
			if( t->kind != TYPE_BOOLEAN ){
				printf( "type error: expression (" );
				expr_print( s->expr );
				printf( ") in if must be boolean, not " );
				type_print( t );
				printf( "\n" );
				typecheck_failed++;
			}
			stmt_typecheck( s->body );
			stmt_typecheck( s->else_body );
			break;
		case STMT_FOR:
			expr_typecheck( s->init_expr );
			expr_typecheck( s->expr );
			expr_typecheck( s->next_expr );
			stmt_typecheck( s->body );
			break;
		case STMT_RETURN:
			t = expr_typecheck( s->expr );
			if( !type_equal( t, return_type ) ){
				printf( "type error: return of " );
				type_print( t );
				printf( " (" );
				expr_print( s->expr );
				printf( ") does not match the function return type of " );
				type_print( return_type );
				printf( "\n" );
				typecheck_failed++;
			}
			break;
		case STMT_BLOCK:
			stmt_typecheck( s->body );
			break;
	}
	stmt_typecheck( s->next );
}
