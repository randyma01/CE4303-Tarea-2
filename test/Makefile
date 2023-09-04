all : client server

.PHONY : all

server : server.c
	@echo "Compiling TCP Server code."
	@gcc server.c -o server
	@chmod +x server


client : client.c
	@echo "Compiling TCP Client code."
	@gcc client.c -o client
	@chmod +x client


clean :
	@echo "Cleaning everything but the source files."
	@rm client server