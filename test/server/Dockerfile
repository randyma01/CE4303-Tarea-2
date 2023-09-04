# Use a lightweight base image
FROM alpine:latest

# Install the required packages (e.g., gcc for compilation)
RUN apk add --no-cache gcc

# Copy the server source code into the container
COPY server.c /app/server.c

# Set the working directory
WORKDIR /app

# Compile the server code
RUN gcc server.c -o server

# Expose port 1717 to the world
EXPOSE 1717

# Specify the command to run when the container starts
CMD ["./server"]