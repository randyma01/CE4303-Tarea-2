# Use a lightweight base image
FROM alpine:latest

# Set the working directory
WORKDIR /app

# Copy the client code into the container
COPY client-r.c /app

# Compile the client code
RUN apk update && apk add gcc libc-dev
RUN gcc client-r.c -o client-r

# Make port 1717 available to the world outside this container
EXPOSE 1717

# Set the entry point
ENTRYPOINT ["./client-r"]
