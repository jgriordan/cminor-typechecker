/* parser.bison */
/* John Riordan */

%{

#include <string.h>
#include <stdio.h>

#include "decl.h"
#include "param_list.h"
#include "stmt.h"
#include "expr.h"
#include "type.h"
#include "param_list.h"

struct decl* parser_result = 0;

extern char* yytext;
extern int yylex();
extern int yyerror( char* str );
extern FILE* yyin;

%}

%union{
	struct decl* decl;
	struct stmt* stmt;
	struct expr* expr;
	struct type* type;
	struct param_list* param_list;
	char* str;
}

%token TOKEN_COMMENT
%token TOKEN_ARRAY
%token TOKEN_BOOLEAN
%token TOKEN_CHAR
%token TOKEN_ELSE
%token TOKEN_FALSE
%token TOKEN_FOR
%token TOKEN_FUNCTION
%token TOKEN_IF
%token TOKEN_INTEGER
%token TOKEN_PRINT
%token TOKEN_RETURN
%token TOKEN_STRING
%token TOKEN_TRUE
%token TOKEN_VOID
%token TOKEN_WHILE
%token TOKEN_SEMI
%token TOKEN_COLON
%token TOKEN_COMMA
%token TOKEN_INCREMENT
%token TOKEN_DECREMENT
%token TOKEN_AND
%token TOKEN_OR
%token TOKEN_LE
%token TOKEN_GE
%token TOKEN_EQ
%token TOKEN_NE
%token TOKEN_EQUALS
%token TOKEN_LPAREN
%token TOKEN_RPAREN
%token TOKEN_LBRACKET
%token TOKEN_RBRACKET
%token TOKEN_LBRACE
%token TOKEN_RBRACE
%token TOKEN_NOT
%token TOKEN_LT
%token TOKEN_GT
%token TOKEN_ADD
%token TOKEN_SUB
%token TOKEN_MUL
%token TOKEN_DIV
%token TOKEN_MOD
%token TOKEN_EXP
%token TOKEN_CHARACTER
%token TOKEN_STRING_LITERAL
%token TOKEN_ID
%token TOKEN_INT
%token TOKEN_ERROR
%token TOKEN_EOF

%type <decl> program decl_list decl
%type <stmt> stmt_list stmt rstmt lstmt
%type <expr> expr_for expr_ar expr_ar_list expr_list expr expr1 expr2 expr3 expr4 expr5 expr6 expr7 atomic constant index
%type <type> type param_type
%type <param_list> param_list param
%type <str> id

%%

program	: decl_list { parser_result = $1; return 0; }
	;

decl_list
	: decl decl_list { $$ = $1; $1->next = $2; }
	| /* nothing */ { $$ = 0; }
	;

decl	: id TOKEN_COLON type TOKEN_SEMI { $$ = decl_create( $1, $3, 0, 0, 0 ); }
	| id TOKEN_COLON type TOKEN_EQUALS expr TOKEN_SEMI { $$ = decl_create( $1, $3, $5, 0, 0 ); }
	| id TOKEN_COLON type TOKEN_EQUALS expr_ar TOKEN_SEMI { $$ = decl_create( $1, $3, $5, 0, 0 ); }
	| id TOKEN_COLON type TOKEN_EQUALS TOKEN_LBRACE stmt_list TOKEN_RBRACE { $$ = decl_create( $1, $3, 0, $6, 0 ); }
	;

id	: TOKEN_ID { $$ = strdup( yytext ); }
	;

stmt_list
	: stmt stmt_list { $$ = $1; $1->next = $2; }
	| /* nothing */ { $$ = 0; }
	;

stmt	: lstmt { $$ = $1; }
	| rstmt { $$ = $1; }
	;

rstmt	: expr TOKEN_SEMI { $$ = stmt_create( STMT_EXPR, 0, 0, $1, 0, 0, 0, 0 ); }
	| decl { $$ = stmt_create( STMT_DECL, $1, 0, 0, 0, 0, 0, 0 ); }
	| TOKEN_FOR TOKEN_LPAREN expr_for TOKEN_SEMI expr_for TOKEN_SEMI expr_for TOKEN_RPAREN rstmt { $$ = stmt_create( STMT_FOR, 0, $3, $5, $7, $9, 0, 0 ); }
	| TOKEN_LBRACE stmt_list TOKEN_RBRACE { $$ = stmt_create( STMT_BLOCK, 0, 0, 0, 0, $2, 0, 0 ); }
	| TOKEN_RETURN expr TOKEN_SEMI { $$ = stmt_create( STMT_RETURN, 0, 0, $2, 0, 0, 0, 0 ); }
	| TOKEN_RETURN TOKEN_SEMI { $$ = stmt_create( STMT_RETURN, 0, 0, 0, 0, 0, 0, 0 ); }
	| TOKEN_PRINT expr_list TOKEN_SEMI { $$ = stmt_create( STMT_PRINT, 0, 0, $2, 0, 0, 0, 0 ); }
	| TOKEN_PRINT TOKEN_SEMI { $$ = stmt_create( STMT_PRINT, 0, 0, 0, 0, 0, 0, 0 ); }
	| TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN rstmt TOKEN_ELSE rstmt { $$ = stmt_create( STMT_IF_ELSE, 0, 0, $3, 0, $5, $7, 0 ); }
	;

lstmt	: TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN stmt { $$ = stmt_create( STMT_IF_ELSE, 0, 0, $3, 0, $5, 0, 0 ); }
	;

expr_for: expr { $$ = $1; }
	| /* nothing */ { $$ = 0; }
	;

expr_ar	: TOKEN_LBRACE expr_ar_list TOKEN_RBRACE { $$ = expr_create( EXPR_AR_INIT, $2, 0 ); }
	;

expr_ar_list
	: expr_list { $$ = $1; }
	| expr_ar TOKEN_COMMA expr_ar_list { $1->right = $3; }
	| expr_ar  { $$ = $1; }
	;

expr_list
	: expr TOKEN_COMMA expr_list { $$ = expr_create( EXPR_LIST, $1, $3 ); }
	| expr { $$ = $1; }
	;

expr	: expr TOKEN_EQUALS expr1 { $$ = expr_create( EXPR_ASGN, $1, $3 ); }
	| expr1 { $$ = $1; }
	;

expr1	: expr1 TOKEN_OR expr2 { $$ = expr_create( EXPR_OR, $1, $3 ); }
	| expr1 TOKEN_AND expr2 { $$ = expr_create( EXPR_AND, $1, $3 ); }
	| expr2 { $$ = $1; }
	;

expr2	: expr2 TOKEN_LE expr3 { $$ = expr_create( EXPR_LE, $1, $3 ); }
	| expr2 TOKEN_GE expr3 { $$ = expr_create( EXPR_GE, $1, $3 ); }
	| expr2 TOKEN_LT expr3 { $$ = expr_create( EXPR_LT, $1, $3 ); }
	| expr2 TOKEN_GT expr3 { $$ = expr_create( EXPR_GT, $1, $3 ); }
	| expr2 TOKEN_EQ expr3 { $$ = expr_create( EXPR_EQ, $1, $3 ); }
	| expr2 TOKEN_NE expr3 { $$ = expr_create( EXPR_NE, $1, $3 ); }
	| expr3 { $$ = $1; }
	;

expr3	: expr3 TOKEN_ADD expr4 { $$ = expr_create( EXPR_ADD, $1, $3 ); }
	| expr3 TOKEN_SUB expr4 { $$ = expr_create( EXPR_SUB, $1, $3 ); }
	| expr4 { $$ = $1; }
	;

expr4	: expr4 TOKEN_MUL expr5 { $$ = expr_create( EXPR_MUL, $1, $3 ); }
	| expr4 TOKEN_DIV expr5 { $$ = expr_create( EXPR_DIV, $1, $3 ); }
	| expr4 TOKEN_MOD expr5 { $$ = expr_create( EXPR_MOD, $1, $3 ); }
	| expr5 { $$ = $1; }
	;

expr5	: expr5 TOKEN_EXP expr6 { $$ = expr_create( EXPR_EXP, $1, $3 ); }
	| expr6 { $$ = $1; }
	;

expr6	: TOKEN_NOT expr7 { $$ = expr_create( EXPR_NOT, 0, $2 ); }
	| TOKEN_SUB expr7 { $$ = expr_create( EXPR_NEG, 0, $2 ); }
	| expr7 { $$ = $1; }
	;

expr7	: atomic TOKEN_INCREMENT { $$ = expr_create( EXPR_INCREMENT, $1, 0 ); }
	| atomic TOKEN_DECREMENT { $$ = expr_create( EXPR_DECREMENT, $1, 0 ); }
	| atomic { $$ = $1; }
	;

atomic	: id { $$ = expr_create_id( $1 ); }
	| constant { $$ = $1; }
	| TOKEN_LPAREN expr TOKEN_RPAREN { $$ = expr_create(EXPR_PAREN, 0, $2 ); }
	| id index { $$ = expr_create( EXPR_ARRAY, expr_create_id( $1 ), $2 ); }
	| id TOKEN_LPAREN expr_list TOKEN_RPAREN { $$ = expr_create( EXPR_CALL, expr_create_id( $1 ), $3 ); }
	| id TOKEN_LPAREN TOKEN_RPAREN { $$ = expr_create( EXPR_CALL, expr_create_id( $1 ), 0 ); }
	;

index	: TOKEN_LBRACKET expr TOKEN_RBRACKET { $$ = expr_create( EXPR_INDEX, $2, 0 ); }
	| TOKEN_LBRACKET expr TOKEN_RBRACKET index { $$ = expr_create( EXPR_INDEX, $2, $4 ); }
	;

constant: TOKEN_CHARACTER {
		/* handle escape characters */
		if( yytext[1] == '\\' ){
			if( yytext[2] == '0' )
				$$ = expr_create_character_literal( '\0' );
			else if( yytext[2] == 'n' )
				$$ = expr_create_character_literal( '\n' );
			else
				$$ = expr_create_character_literal( yytext[1] );
		} else
			$$ = expr_create_character_literal( yytext[1] );
	}
	| TOKEN_STRING_LITERAL { $$ = expr_create_string_literal( strdup( yytext ) ); }
	| TOKEN_INT { $$ = expr_create_integer_literal( atoi( yytext ) ); }
	| TOKEN_TRUE { $$ = expr_create_boolean_literal( 1 ); }
	| TOKEN_FALSE { $$ = expr_create_boolean_literal( 0 ); }
	;

type	: TOKEN_BOOLEAN { $$ = type_create( TYPE_BOOLEAN, 0, 0, 0 ); }
	| TOKEN_INTEGER { $$ = type_create( TYPE_INTEGER, 0, 0, 0 ); }
	| TOKEN_CHAR { $$ = type_create( TYPE_CHARACTER, 0, 0, 0 ); }
	| TOKEN_STRING { $$ = type_create( TYPE_STRING, 0, 0, 0 ); }
	| TOKEN_VOID { $$ = type_create( TYPE_VOID, 0, 0, 0 ); }
	| TOKEN_ARRAY TOKEN_LBRACKET constant TOKEN_RBRACKET type { $$ = type_create( TYPE_ARRAY, 0, $5, $3 ); }
	| TOKEN_FUNCTION param_type TOKEN_LPAREN param_list TOKEN_RPAREN { $$ = type_create( TYPE_FUNCTION, $4, $2, 0 ); }
	| TOKEN_FUNCTION param_type TOKEN_LPAREN TOKEN_RPAREN { $$ = type_create( TYPE_FUNCTION, 0, $2, 0 ); }
	;

param_type
	: TOKEN_BOOLEAN { $$ = type_create( TYPE_BOOLEAN, 0, 0, 0 ); }
	| TOKEN_INTEGER { $$ = type_create( TYPE_INTEGER, 0, 0, 0 ); }
	| TOKEN_CHAR { $$ = type_create( TYPE_CHARACTER, 0, 0, 0 ); }
	| TOKEN_STRING { $$ = type_create( TYPE_STRING, 0, 0, 0 ); }
	| TOKEN_VOID { $$ = type_create( TYPE_VOID, 0, 0, 0 ); }
	| TOKEN_ARRAY TOKEN_LBRACKET TOKEN_RBRACKET param_type { $$ = type_create( TYPE_ARRAY, 0, $4, 0 ); }
	;
	

param_list
	: param TOKEN_COMMA param_list { $$ = $1; $1->next = $3; }
	| param { $$ = $1; }
	;

param	: id TOKEN_COLON param_type { $$ = param_list_create( $1, $3, 0 ); }
	;

%%

int yyerror( char* str ){
	printf( "parse error: %s\n", str );
	return 1;
}
