// expr.c
// John Riordan

#include "expr.h"

struct expr* expr_create( expr_t kind, struct expr* left, struct expr* right ){
	struct expr* e = malloc( sizeof(*e) );
	e->kind = kind;
	e->left = left;
	e->right = right;
	e->name = 0;
	e->symbol = 0;
	e->literal_value = 0;
	e->string_literal = 0;
	return e;
}

struct expr* expr_create_id( const char* n ){
	struct expr* e = malloc( sizeof(*e) );
	e->kind = EXPR_ID;
	e->left = 0;
	e->right = 0;
	e->name = n;
	e->symbol = 0;
	e->literal_value = 0;
	e->string_literal = 0;
	return e;
}

struct expr* expr_create_boolean_literal( int c ){
	struct expr* e = malloc( sizeof(*e) );
	e->kind = EXPR_BOOLEAN_LITERAL;
	e->left = 0;
	e->right = 0;
	e->name = 0;
	e->symbol = 0;
	e->literal_value = c;
	e->string_literal = 0;
	return e;
}


struct expr* expr_create_integer_literal( int c ){
	struct expr* e = malloc( sizeof(*e) );
	e->kind = EXPR_INTEGER_LITERAL;
	e->left = 0;
	e->right = 0;
	e->name = 0;
	e->symbol = 0;
	e->literal_value = c;
	e->string_literal = 0;
	return e;
}

struct expr* expr_create_character_literal( int c ){
	struct expr* e = malloc( sizeof(*e) );
	e->kind = EXPR_CHARACTER_LITERAL;
	e->left = 0;
	e->right = 0;
	e->name = 0;
	e->symbol = 0;
	e->literal_value = c;
	e->string_literal = 0;
	return e;
}

struct expr* expr_create_string_literal( const char* str ){
	struct expr* e = malloc( sizeof(*e) );
	e->kind = EXPR_STRING_LITERAL;
	e->left = 0;
	e->right = 0;
	e->name = 0;
	e->symbol = 0;
	e->literal_value = 0;
	e->string_literal = str;
	return e;
}

void expr_print( struct expr* e ){
	if( !e ) return;
	if( e->kind == EXPR_INDEX ) printf( "[" );
	else if( e->kind == EXPR_AR_INIT ) printf( "{" );
	expr_print( e->left );
	switch( e->kind ){
		case EXPR_ASGN: printf( " = " ); break;
		case EXPR_OR: printf( " || " ); break;
		case EXPR_AND: printf( " && " ); break;
		case EXPR_LE: printf( " <= " ); break;
		case EXPR_GE: printf( " >= " ); break;
		case EXPR_LT: printf( " < " ); break;
		case EXPR_GT: printf( " > " ); break;
		case EXPR_EQ: printf( " == " ); break;
		case EXPR_NE: printf( " != " ); break;
		case EXPR_ADD: printf( " + " ); break;
		case EXPR_SUB: printf( " - " ); break;
		case EXPR_MUL: printf( " * " ); break;
		case EXPR_DIV: printf( " / " ); break;
		case EXPR_MOD: printf( " %% " ); break;
		case EXPR_EXP: printf( " ^ " ); break;
		case EXPR_NOT: printf( "!" ); break;
		case EXPR_NEG: printf( "-" ); break;
		case EXPR_INCREMENT: printf( "++" ); break;
		case EXPR_DECREMENT: printf( "--" ); break;
		case EXPR_ID: printf( "%s", e->name ); break;
		case EXPR_CHARACTER_LITERAL:
			if( (char)e->literal_value == '\0' )
				printf( "\'\\0\'" );
			else if( (char)e->literal_value == '\n' )
				printf( "\'\\n\'" );
			else
				printf( "\'%c\'", (char)e->literal_value );
			break;
		case EXPR_STRING_LITERAL: printf( "%s", e->string_literal ); break;
		case EXPR_INTEGER_LITERAL: printf( "%d", e->literal_value ); break;
		case EXPR_BOOLEAN_LITERAL: if( e->literal_value ) printf( "true" ); else printf( "false" ); break;
		case EXPR_ARRAY: break;
		case EXPR_INDEX: printf( "]" ); break;
		case EXPR_CALL: printf( "(" ); break;
		case EXPR_PAREN: printf( "(" ); break;
		case EXPR_AR_INIT: if( e->right ) printf( "}, " ); break;
		case EXPR_LIST: if( e->left ) printf( ", " ); break;
	}
	expr_print( e->right );
	if( e->kind == EXPR_CALL || e->kind == EXPR_PAREN ) printf( ")" );
	else if( e->kind == EXPR_AR_INIT && !e->right ) printf( "}" );
}

void expr_resolve( struct expr* e ){
	if( !e ) return;
	if( e->kind == EXPR_ID ){
		e->symbol = scope_lookup( e->name );
		if( !(e->symbol) ){
			printf( "resolve error: %s is not defined\n", e->name );
			resolve_failed = 1;
		} else if( resolve_print ) {
			switch( e->symbol->kind ){
				case SYMBOL_GLOBAL:
					printf( "%s resolves to global %s\n", e->name, e->symbol->name );
					break;
				case SYMBOL_LOCAL:
					printf( "%s resolves to local %d\n", e->name, e->symbol->which );
					break;
				case SYMBOL_PARAM:
					printf( "%s resolves to param %d\n", e->name, e->symbol->which );
					break;
			}
		}
	} else {
		expr_resolve( e->left );
		expr_resolve( e->right );
	}
}


struct type* expr_typecheck( struct expr* e ){
	struct type* lt;
	struct type* rt;
	if( !e ) return 0;
	lt = expr_typecheck( e->left );
	rt = expr_typecheck( e->right );
	switch( e->kind ){
		case EXPR_ASGN:
			
			return type_copy( lt );
		case EXPR_OR:
		case EXPR_AND:
			return type_create( TYPE_BOOLEAN, 0, 0, 0 );
		case EXPR_LE:
		case EXPR_GE:
		case EXPR_LT:
		case EXPR_GT:
			if( lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER ){
				printf( "type error: cannot compare " );
				type_print( lt );
				printf( " (" );
				expr_print( e->left );
				printf( ") to an " );
				type_print( rt );
				printf( " (" );
				expr_print( e->right );
				printf( ")\n" );
				typecheck_failed = 1;
			}
			return type_create( TYPE_BOOLEAN, 0, 0, 0 );
		case EXPR_EQ:
		case EXPR_NE:
			return type_create( TYPE_BOOLEAN, 0, 0, 0 );
		case EXPR_ADD:
		case EXPR_SUB:
		case EXPR_MUL:
		case EXPR_DIV:
		case EXPR_MOD:
		case EXPR_EXP:
			return type_create( TYPE_INTEGER, 0, 0, 0 );
		case EXPR_NOT:
			return type_create( TYPE_BOOLEAN, 0, 0, 0 );
		case EXPR_NEG:
			return type_create( TYPE_INTEGER, 0, 0, 0 );
		case EXPR_INCREMENT:
		case EXPR_DECREMENT:
			return type_create( TYPE_INTEGER, 0, 0, 0 );
		case EXPR_ID:
			return type_copy( e->symbol->type );
		case EXPR_CHARACTER_LITERAL:
			return type_create( TYPE_CHARACTER, 0, 0, 0 );
		case EXPR_STRING_LITERAL:
			return type_create( TYPE_STRING, 0, 0, 0 );
		case EXPR_INTEGER_LITERAL:
			return type_create( TYPE_INTEGER, 0, 0, 0 );
		case EXPR_BOOLEAN_LITERAL:
			return type_create( TYPE_BOOLEAN, 0, 0, 0 );
		case EXPR_ARRAY:
		case EXPR_INDEX:
		case EXPR_CALL:
		case EXPR_PAREN:
		case EXPR_AR_INIT:
		case EXPR_LIST:
			break;
	}
	return 0;
}
