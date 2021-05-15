#File transfer using proxy server

#Client sends requests to proxy server
#proxy server checks in its directory for the requested file
#If file is present in proxy server's directory then it is transmitted to the client
#otherwise proxy server asks for the file from the main server
#main server transmits the file to proxy server
#proxy server maintains a copy of the file for further requests

#compile server
gcc server.c -o server.o -lpthread

#run server - hardcoded to run on port 5010
./server.o

#compile proxy server
gcc proxy.c -o proxy.o -lpthread

#run proxy server - hardcoded to run on port 5000
./proxy 127.0.0.1 5010 5000

#compile client
gcc client.c -o client.o -lpthread

#run client
./client.o

