cli_exec:  com2 run
	rm -rf Compilador
    
run:
	./Compilador
    
com2:
	gcc -o Compilador program.c
