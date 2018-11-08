#!/bin/bash

	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
	gcc -o tema1 tema1.c -L. -lhash
	./tema1 256 input.in
	
