#!/bin/bash
try(){
    ./mycc.out "$1" 'tmp.s'
    if [ $? = 0 ]; then
	gcc -o 'tmp.out' 'tmp.s' 'func.c'
	./tmp.out
    fi
    echo
}
try "$1"
