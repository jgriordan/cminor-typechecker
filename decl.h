// decl.h
// John Riordan

#ifndef DECL_H
#define DECL_H

#include "expr.h"
#include "scope.h"
#include "stmt.h"
#include "type.h"
#include <stdio.h>

struct decl {
	char* name;
	struct type* type;
	struct expr* value;
	struct stmt* code;
	struct symbol* symbol;
	struct decl* next;
};

struct decl* decl_create( char* name, struct type* t, struct expr* v, struct stmt* c, struct decl* next );
void decl_print( struct decl* d, int indent );
void decl_resolve( struct decl* d );
void decl_typecheck( struct decl* d );

#endif
