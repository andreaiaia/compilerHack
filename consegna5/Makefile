main: main.o gestionefile.o operazioni.o
	gcc -o assembler main.o gestionefile.o operazioni.o

main.o: main.c gestionefile.h
	gcc -c main.c

gestionefile.o: gestionefile.c gestionefile.h
	gcc -c gestionefile.c

operazioni.o: operazioni.c operazioni.h
	gcc -c operazioni.c

clean:
	rm *.o main