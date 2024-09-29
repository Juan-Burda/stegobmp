#!/bin/bash

# Remove the previous image
docker rmi -f stego

# Build the new image
docker build . -t stego

# Run the container with the provided arguments
docker run --rm -v "$PWD/resources:/usr/src/app/resources" stego "$@"
