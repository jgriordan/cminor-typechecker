// type.h
// John Riordan

#ifndef TYPE_H
#define TYPE_H

#include <stdio.h>
#include <stdlib.h>
#include "expr.h"
#include "param_list.h"

typedef enum {
	TYPE_BOOLEAN,
	TYPE_CHARACTER,
	TYPE_INTEGER,
	TYPE_STRING,
	TYPE_ARRAY,
	TYPE_FUNCTION,
	TYPE_VOID
} type_kind_t;

struct type {
	type_kind_t kind;
	struct param_list* params;
	struct type* subtype;
	struct expr* size;
};

struct type* type_create( type_kind_t kind, struct param_list* params, struct type* subtype, struct expr* size );
void type_print( struct type* t );
struct type* type_copy( struct type* t );
int type_equal( struct type* a, struct type* b );
void type_delete( struct type* t );
void type_resolve( struct type* t );

#endif
