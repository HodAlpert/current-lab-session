all: addresses.o
	gcc -g -Wall -o main addresses.o
addresses.o: addresses.c
	gcc -g -Wall -c -o addresses.o addresses.c
.PHONY: clean
clean:
	rm -f *.o main
