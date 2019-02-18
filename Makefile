all: Compilador_Micro refresh

Compilador_Micro: micro.o Parser.o Scanner.o semantic.o fAuxs.o 
	gcc -o micro_compiler micro.o Parser.o Scanner.o semantic.o fAuxs.o
    
micro.o: micro.c headers/micro.h headers/parser.h
	gcc -c micro.c

parser.o: Parser.c headers/Parser.h headers/semantic.h headers/fAuxs.h headers/micro.h
	gcc -c Parser.c

scanner.o: Scanner.c headers/Scanner.h headers/micro.h
	gcc -c Scanner.c

semantic.o: semantic.c headers/semantic.h headers/fAuxs.h headers/micro.h
	gcc -c semantic.c

fAuxs.o: fAuxs.c headers/fAuxs.h headers/scanner.h headers/micro.h
	gcc -c fAuxs.c

refresh:
	-rm micro.o Parser.o Scanner.o semantic.o fAuxs.o

clean:
	-rm micro_compiler
