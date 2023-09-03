#!/bin/bash

# compile the server with Makefile
echo 'execute the server Makefile'
make clean
make server

# executing the server
echo 'execute the server itself'
./server