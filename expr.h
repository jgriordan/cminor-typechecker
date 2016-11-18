#ifndef EXPR_H
#define EXPR_H

#include "scope.h"
#include "param_list.h"

extern int resolve_failed;
extern int resolve_print;
extern int typecheck_failed;

typedef enum {
	EXPR_ASGN,
	EXPR_OR,
	EXPR_AND,
	EXPR_LE,
	EXPR_GE,
	EXPR_LT,
	EXPR_GT,
	EXPR_EQ,
	EXPR_NE,
	EXPR_ADD,
	EXPR_SUB,
	EXPR_MUL,
	EXPR_DIV,
	EXPR_MOD,
	EXPR_EXP,
	EXPR_NOT,
	EXPR_NEG,
	EXPR_INCREMENT,
	EXPR_DECREMENT,
	EXPR_ID,
	EXPR_CHARACTER_LITERAL,
	EXPR_STRING_LITERAL,
	EXPR_INTEGER_LITERAL,
	EXPR_BOOLEAN_LITERAL,
	EXPR_ARRAY,
	EXPR_INDEX,
	EXPR_CALL,
	EXPR_PAREN,
	EXPR_AR_INIT,
	EXPR_LIST
} expr_t;

struct expr {
	/* used by all kinds of exprs */
	expr_t kind;
	struct expr *left;
	struct expr *right;

	/* used by various leaf exprs */
	const char *name;
	struct symbol *symbol;
	int literal_value;
	const char * string_literal;
};

struct expr* expr_create( expr_t kind, struct expr* left, struct expr* right );

struct expr* expr_create_id( const char* n );
struct expr* expr_create_boolean_literal( int c );
struct expr* expr_create_integer_literal( int c );
struct expr* expr_create_character_literal( int c );
struct expr* expr_create_string_literal( const char* str );

void expr_print( struct expr* e );
int expr_list_equal_param_list( struct expr* e, struct param_list* p );
void expr_resolve( struct expr* e );
struct type* expr_typecheck( struct expr* e );

#endif
