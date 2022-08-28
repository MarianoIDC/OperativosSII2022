$(CC)
server:
	gcc ./Servidor/DocServer.c -o ./Servidor/DocServer
	./Servidor/DocServer

client:
	gcc ./Cliente/DocCliente.c -o ./Cliente/DocCliente
	./Cliente/DocClient 192.168.122.118 8080
	
Clean:
	rm *.o server
	rm *.o client
