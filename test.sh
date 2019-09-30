#!/bin/bash

CC="gcc"
USE_TARGET=1 
TARGET_FLAGS=""
NUM_ITERATIONS=100

trap "exit" INT
for i in $(seq 1 $NUM_ITERATIONS); 
do
  if [ $USE_TARGET -eq 1 ]; then
	  ./testgen -target > output.c
	  $CC -fopenmp output.c -o output
  else
    ./testgen > output.c
	  $CC -fopenmp output.c -o output $TARGET_FLAGS
  fi
          
	./output $(head -1 output.c | sed -e 's/\/\///g') 

	if [ $? -ne 0 ]
	then
		echo "Failure"
		cat output.c
	fi
done



