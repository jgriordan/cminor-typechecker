/* scanner.flex */
/* John Riordan */

%{

#include "parser.tab.h"
extern int yylex();

%}

DIGIT	[0-9]
LETTER	[a-zA-Z]

%%

\/\*([^*]|\*[^\/])*\*\/ /*skip comments*/
\/\/.*\n /*skip comments*/
array { return TOKEN_ARRAY; }
boolean { return TOKEN_BOOLEAN; }
char { return TOKEN_CHAR; }
else { return TOKEN_ELSE; }
false { return TOKEN_FALSE; }
for { return TOKEN_FOR; }
function { return TOKEN_FUNCTION; }
if { return TOKEN_IF; }
integer { return TOKEN_INTEGER; }
print { return TOKEN_PRINT; }
return { return TOKEN_RETURN; }
string { return TOKEN_STRING; }
true { return TOKEN_TRUE; }
void { return TOKEN_VOID; }
while { return TOKEN_WHILE; }
; { return TOKEN_SEMI; }
: { return TOKEN_COLON; }
, { return TOKEN_COMMA; }
\+\+ { return TOKEN_INCREMENT; }
-- { return TOKEN_DECREMENT; }
&& { return TOKEN_AND; }
\|\| { return TOKEN_OR; }
\<= { return TOKEN_LE; }
>= { return TOKEN_GE; }
== { return TOKEN_EQ; }
!= { return TOKEN_NE; }
= { return TOKEN_EQUALS; }
\( { return TOKEN_LPAREN; }
\) { return TOKEN_RPAREN; }
\[ { return TOKEN_LBRACKET; }
\] { return TOKEN_RBRACKET; }
\{ { return TOKEN_LBRACE; }
\} { return TOKEN_RBRACE; }
! { return TOKEN_NOT; }
\< { return TOKEN_LT; }
> { return TOKEN_GT; }
\+ { return TOKEN_ADD; }
- { return TOKEN_SUB; }
\* { return TOKEN_MUL; }
\/ { return TOKEN_DIV; }
% { return TOKEN_MOD; }
\^ { return TOKEN_EXP; }
'(\\.|[^\n\\'])' { return TOKEN_CHARACTER; }
\"([^\\"\n]|\\.)*\" {
	int i = 0;
	int j;
	for( j=1; j<strlen(yytext)-1 && i<256; j++ ){
		// avoid double-counting escape characters
		if( yytext[j+1] == '\\' )
			j++;
		i++;
			
	}
	if( i == 256 && j < strlen(yytext)-1 ){
		printf( "scan error: too long of a string literal\n" );
		return TOKEN_ERROR;
	}
	return TOKEN_STRING_LITERAL;
}
({LETTER}|_)({LETTER}|{DIGIT}|_)* {
	if( strlen( yytext ) > 256 ){
		printf( "scan error: too long of an identifier\n" );
		return TOKEN_ERROR;
	}
	return TOKEN_ID;
}
{DIGIT}+ { return TOKEN_INT; }
(" "|\t|\n|\r)+ /* skip whitespace */
. {
	printf( "scan error: %s is not a valid character\n", yytext );
	return TOKEN_ERROR;
}

%%

int yywrap(){
	fclose(yyin);
	return TOKEN_EOF;
}
