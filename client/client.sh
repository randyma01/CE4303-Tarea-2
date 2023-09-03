#!/bin/bash

# compile the client with Makefile
echo 'execute the client Makefile'
make clean
make client

# executing the client
echo 'execute the client itself'
./client