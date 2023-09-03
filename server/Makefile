all : server

server : server.c
	@echo "Compiling TCP Server code."
	@gcc server.c -o server
	@chmod +x server

clean :
	@echo "Cleaning everything but the source files."
	@rm server