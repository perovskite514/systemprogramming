tcpechoclient: tcpechoclient.c
		gcc -Wall -o tcpechoclient tcpechoclient.c

tcpechoserver1: tcpechoserver1.c
		gcc -Wall -pthread -o tcpechoserver1 tcpechoserver1.c

tcpechoserver2: tcpechoserver2.c
		gcc -Wall -o tcpechoserver2 tcpechoserver2.c

.PHONY : clean
clean:
	rm -f tcpechoclient tcpechoserver1 tcpechoserver2