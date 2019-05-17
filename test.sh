#!/bin/bash
try(){
    ./mycc.out 'test.reka' 'tmp.s'
    if [ $? = 0 ]; then
	gcc -o 'tmp.out' 'tmp.s' 'func.c'
	./tmp.out
    fi
    echo
}
try
