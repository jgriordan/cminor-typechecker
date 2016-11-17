// scope.h
// John Riordan

#ifndef SCOPE_H
#define SCOPE_H

#include "hash_table.h"
#include "symbol.h"

struct scope{
	struct scope* above;
	int level;
	struct hash_table* ht;
	int local_count;
	int param_count;
};

struct scope* current_scope;

void scope_init();
void scope_enter();
void scope_leave();
int scope_level();
void scope_bind(const char* name, struct symbol* s);
struct symbol* scope_lookup(const char* name);

#endif
