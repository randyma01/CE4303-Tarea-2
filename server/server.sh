#!/bin/bash

# compile the server with Makefile
echo 'Executing the server Makefile...'
make clean
make server

# executing the server
echo 'Executing the server code...'
./server