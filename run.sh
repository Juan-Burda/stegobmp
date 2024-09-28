#!/bin/bash

docker rmi -f stego
docker build . -t stego 
docker run stego