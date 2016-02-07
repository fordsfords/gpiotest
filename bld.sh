#!/bin/sh
# bld.sh - script to build the CHIP "gpiotest" program.

gcc -Wall -O3 -std=c99 -o xiotest xiotest.c
gcc -Wall -O3 -std=c99 -o csitest csitest.c
