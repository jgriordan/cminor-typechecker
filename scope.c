// scope.c
// John Riordan

#include "scope.h"

void scope_init(){
	current_scope = malloc( sizeof(*current_scope) );
	current_scope->above = 0;
	current_scope->level = 0;
	current_scope->ht = hash_table_create( 0, 0 );
	current_scope->local_count = 0;
	current_scope->param_count = 0;
}

void scope_enter(){
	struct scope* s = malloc( sizeof(*s) );
	s->above = current_scope;
	s->level = current_scope->level + 1;
	s->ht = hash_table_create( 0, 0 );
	s->local_count = 0;
	s->param_count = 0;
	current_scope = s;
}

void scope_leave(){
	current_scope = current_scope->above;
}

int scope_level(){
	return current_scope->level;
}

void scope_bind( const char* name, struct symbol* s ){
	hash_table_insert( current_scope->ht, name, s );
	if( s->kind == SYMBOL_LOCAL ){
		current_scope->local_count++;
		s->which = current_scope->local_count;
	}
	if( s->kind == SYMBOL_PARAM ){
		current_scope->param_count++;
		s->which = current_scope->param_count;
	}
}

struct symbol* scope_lookup( const char* name ){
	struct symbol* sym;
	struct scope* scope = current_scope;
	while( scope ){
		sym = hash_table_lookup( scope->ht, name );
		if( sym ){
			return sym;
		}
		scope = scope->above;
	}
	return 0;
}
