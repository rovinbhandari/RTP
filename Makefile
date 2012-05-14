all:	server_udp.c.out client_udp.c.out

server_udp.c.out:	server_udp.c
	gcc server_udp.c -o server_udp.c.out

client_udp.c.out:	client_udp.c
	gcc client_udp.c -o client_udp.c.out

clean:
	rm server_udp.c.out
	rm client_udp.c.out
