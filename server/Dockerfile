# Usa una imagen base de Ubuntu o la que prefieras
FROM ubuntu:latest

# Actualiza los paquetes e instala las dependencias necesarias
RUN apt-get update && \
    apt-get install -y gcc make && \
    apt-get clean

# Copia el código fuente del client al contenedor
COPY server.c /app/server.c

# Compila el server dentro del contenedor
WORKDIR /app
RUN gcc -o server server.c

# Expose the port 1717
EXPOSE  1717

# Comando por defecto para ejecutar el server
CMD ["./server"]
