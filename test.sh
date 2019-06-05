#!/bin/bash

trap "exit" INT
for i in {1..100}
do
	./main > output.c
	gcc -fopenmp output.c -o emi_program
	./emi_program $(head -1 output.c | sed -e 's/\/\///g') 
	if [ $? -ne 0 ]
	then
		echo "Failure"
		cat output.c
	fi
done



