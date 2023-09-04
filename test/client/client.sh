#!/bin/bash

# compile the client with Makefile
echo 'Executing the client Makefile...'
make clean
make client

# deleting existing Docker image
echo 'Deleting existing image for the client code...'
docker rmi client-app-msg:latest

# creating Docker image
echo 'Creating an image for the client code...'
docker build -t client-app-msg .

# creating and running Docker container
echo 'Creating and running container for the client code...'
docker run -it --network host client-app-msg

