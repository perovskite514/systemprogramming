unsafe_btree: btree.o main.c
	gcc -Wall -pthread -o unsafe_btree btree.o main.c

btree.o: btree.c btree.h
	gcc -Wall -c btree.c 

.PHONY: clean
clean:
	rm -f *.o unsafe_btree
