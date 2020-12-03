main: main.o gestionefile.o
	gcc -o main main.o gestionefile.o

main.o: main.c gestionefile.h
	gcc -c main.c

gestionefile.o: gestionefile.c gestionefile.h
	gcc -c gestionefile.c

clean:
	rm *.o main