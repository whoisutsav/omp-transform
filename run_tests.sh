#!/bin/bash

CC="gcc"
USE_TARGET=1 
TARGET_FLAGS=""
NUM_ITERATIONS=100

trap "exit" INT
mkdir -p test_output
for i in $(seq 1 $NUM_ITERATIONS); 
do
  if [ $USE_TARGET -eq 1 ]; then
	  ./testgen -target > test_output/test.c
	  $CC -fopenmp test_output/test.c -o test_output/test 
  else
    ./testgen > test_output/test.c
	  $CC -fopenmp test_output/test.c -o test_output/test $TARGET_FLAGS
  fi
          
	./test_output/test $(head -1 test_output/test.c | sed -e 's/\/\///g') 

	if [ $? -ne 0 ]
	then
		echo "Failure"
		cat test_output/test.c
	fi
done



