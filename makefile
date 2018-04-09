all: toy_printf.o main.o
	gcc -g -Wall -o main toy_printf.o main.o
toy_printf.o: toy_printf.c
	gcc -g -Wall -c -o toy_printf.o toy_printf.c
main.o: main.c toy_stdio.h
	gcc -g -Wall -c -o main.o main.c
.PHONY: clean
clean:
	rm -f *.o main