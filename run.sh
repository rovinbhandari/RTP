#!/bin/bash

nclients=$1

echo "Starting up Server..."
( ./server_udp.c.out & )

sleep 2s

echo "Starting up $nclients Clients..."
for (( i = 1; i <= $nclients; i++ ))
do
	( ./client_udp.c.out & )
done

echo "Wait for processes to complete..."
sleep 20s && killall server_udp.c.out && killall client_udp.c.out
echo "Done."
