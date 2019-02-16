all: Compilador_Micro refresh

Compilador_Micro: micro.o parser.o scanner.o semantic.o fAuxs.o 
	gcc -o micro_compiler micro.o parser.o scanner.o semantic.o fAuxs.o
    
micro.o: micro.c headers/micro.h headers/parser.h
	gcc -c micro.c

parser.o: parser.c headers/parser.h headers/semantic.h headers/fAuxs.h headers/micro.h
	gcc -c parser.c

scanner.o: scanner.c headers/scanner.h headers/micro.h
	gcc -c scanner.c

semantic.o: semantic.c headers/semantic.h headers/fAuxs.h headers/micro.h
	gcc -c semantic.c

fAuxs.o: fAuxs.c headers/fAuxs.h headers/scanner.h headers/micro.h
	gcc -c fAuxs.c

refresh:
	-rm micro.o parser.o scanner.o semantic.o fAuxs.o

clean:
	-rm micro_compiler