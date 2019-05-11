#!/bin/bash
try(){
    echo -e -n 'result: \e[31m';./mycc.out 'test.reka' 'tmp.s';res="$?";echo -e -n '\e[m'
    if [ $res = 0 ]; then
	gcc -o 'tmp.out' 'tmp.s' 'func.c'
	./tmp.out
    fi
    echo
}
try
<<"#__CO__"
try 'a?v;'
try '12adada=13;'
try 'a!;'
try 'a'
try ');'
try 'a=+(1+2)*3);'
try 'a=2;a+=(2+2'
try 'a=2;{2+2;'
try 'char a;a+2=3;'
try 'char a=2;{char b=23;}a=b;' 23
#__CO__
