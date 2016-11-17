// symbol.h
// John Riordan

#ifndef SYMBOL_H
#define SYMBOL_H

typedef enum {
	SYMBOL_LOCAL,
	SYMBOL_PARAM,
	SYMBOL_GLOBAL
} symbol_t;

struct symbol {
	symbol_t kind;
	int which;
	struct type* type;
	char* name;
};

#include "type.h"

struct symbol* symbol_create( symbol_t kind, struct type* type, char* name );
void symbol_print( struct symbol* s );

#endif
