# Use a base Alpine Linux image
FROM alpine:latest

# Install necessary dependencies and build tools
RUN apk update && \
    apk add --no-cache gcc g++ make libc-dev && \
    rm -rf /var/cache/apk/*

# Copy the client source code into the container
COPY client.c /app/client.c

# Compile the client inside the container
WORKDIR /app
RUN gcc -o client client.c

# Expose port 1717 to the world
EXPOSE 1717

# Default command to run the client
CMD ["./client"]
