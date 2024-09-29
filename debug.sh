#!/bin/bash

# Remove the previous image
docker rmi -f stego-debug

# Build the new image
docker build -f Dockerfile.debug . -t stego-debug

# Run the container with the provided arguments
docker run --rm -it stego-debug /bin/gdb ./bin/stegobmp
