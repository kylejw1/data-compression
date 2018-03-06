#!/bin/bash

docker run -u ${UID}:${UID} --rm -w '/app' -it -v $(pwd):/app gcc bash -c "mkdir -p bin && gcc -pedantic -o bin/a.out main.c && bin/a.out"