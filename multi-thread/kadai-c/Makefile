bb: bb.o main.c
	gcc -Wall -pthread -o bb bb.o main.c

bb.o: bb.c bb.h
	gcc -Wall -c bb.c 

.PHONY: clean
clean:
	rm -f *.o bb
