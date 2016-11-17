all: cminor cflat

cminor: main.o parser.tab.o scanner.o decl.o param_list.o stmt.o expr.o type.o symbol.o scope.o hash_table.o
	gcc -Wall main.o parser.tab.o scanner.o decl.o param_list.o stmt.o expr.o type.o symbol.o scope.o hash_table.o -o cminor

cflat: cminor
	cp cminor cflat

parser.tab.o: parser.tab.c
	gcc -Wall -c parser.tab.c

parser.tab.c parser.tab.h:
	yacc -d -bparser -v parser.bison

main.o: main.c
	gcc -Wall -c main.c

scanner.o: scanner.c
	gcc -Wall -c scanner.c -o scanner.o

scanner.c: lex.yy.c
	mv lex.yy.c scanner.c

lex.yy.c: scanner.flex
	flex scanner.flex

decl.o: decl.c
	gcc -Wall -c decl.c

param_list.o: param_list.c
	gcc -Wall -c param_list.c

stmt.o: stmt.c
	gcc -Wall -c stmt.c

expr.o: expr.c
	gcc -Wall -c expr.c

type.o: type.c
	gcc -Wall -c type.c

scope.o: scope.c
	gcc -Wall -c scope.c

symbol.o: symbol.c
	gcc -Wall -c symbol.c

hashtable.o: hashtable.c
	gcc -Wall -c hashtable.c

clean:
	rm -f parser.tab.* parser.output scanner.c *.o *.out cminor cflat
