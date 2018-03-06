#!/bin/bash

docker run --rm -w '/app' -it -v $(pwd):/app gcc bash -c "mkdir -p bin && gcc -o bin/out.a main.c && bin/out.a"