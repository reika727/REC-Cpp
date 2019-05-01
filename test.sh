#!/bin/bash
try() {
    input="$1"
    echo "input : $input"
    echo -n 'result: '
    ./mycc "$input"
    if [ $? = 0 ]; then
	expected="$2"
	gcc -o tmp tmp.s
	./tmp
	actual=$?
	if [ "$actual" = "$expected" ]; then
	    echo "got $actual"
	else
	    echo "expected $expected, but got $actual"
	fi
    fi
    echo
}
#try 'a!;'
#try 'a?v;'
#try 'a'
#try 'a=+(1+2)*3);'
#try 'a+2=3;'
#try '12adada=13;'
try '((1+2)*3);' 9
try '+((1+2)*3);' 9
try '+(2);' 2
try '+(2+1);' 3
try '+(1*2);' 2
try '-(-3-2+-3);' 8
try 'a=4;a/(1+1);' 2
try 'a=53;a%(1+8);' 8
try 'a=2;' 2
try 'a=+2;' 2
try 'a=+(2);' 2
try 'a=+23;b=a*-2;b+(+47);' 1
try 'a=1;bc=1;a==bc;' 1
try '_A=2;b=2;_A!=b;' 0
try 'a=0;_bc=2;a>_bc;' 0
try 'a=23;Bc=33;a<Bc;' 1
try 'bc_sas232332e____as=21;return bc_sas232332e____as*2+3;' 45
try '_A=5;_Bc=5;_A*2>=_Bc*2;' 1
try 'A__=2;b_c=99;A__*300<=b_c;' 0
try 'a=2;a+=3;' 5
try 'a=2;b=3;c=2;a+=b+=c;' 7
try 'a=24;a-=13;' 11
try 'a=31;b=12;c=9;a-=b-=c;' 28
try 'a=12;b=9;a*=b;' 108
try 'a=12;a/=(2+4);' 2
try 'a=29;a%=9;' 2
try 'a=29;a+=3;b=5;b*=a;c=180;c-=b;d=100;d/=c;e=173;e%=d;' 3
try 'a=23;++a;++a;b=++a;b;' 26
try 'a=32;--a;--a;b=--a;b;' 29
try 'a=32;b=++a;++b;++a;c=(a+b);--c;' 67
