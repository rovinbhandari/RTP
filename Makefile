all:	server_udp.c.out client_udp.c.out

server_udp.c.out:	server_udp.o server_udp_functions.o commons_functions.o
	gcc server_udp.o server_udp_functions.o commons_functions.o -o server_udp.c.out

client_udp.c.out:	client_udp.o commons_functions.o
	gcc client_udp.o commons_functions.o -o client_udp.c.out

server_udp.o:	server_udp.c
	gcc -c server_udp.c -o server_udp.o

server_udp_functions.o:	server_udp_functions.c
	gcc -c server_udp_functions.c -o server_udp_functions.o

client_udp.o:	client_udp.c
	gcc -c client_udp.c -o client_udp.o

commons_functions.o:	commons_functions.c
	gcc -c commons_functions.c -o commons_functions.o

clean:
	rm -f server_udp.o
	rm -f server_udp_functions.o
	rm -f client_udp.o
	rm -f commons_functions.o
	rm -f server_udp.c.out
	rm -f client_udp.c.out
