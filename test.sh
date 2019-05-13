#!/bin/bash

./main > output.c
gcc -fopenmp output.c

