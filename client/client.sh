#!/bin/bash

# compile the client with Makefile
echo 'Executing the client Makefile...'
make clean
make client

# executing the client
echo 'Executing the client code...'
./client