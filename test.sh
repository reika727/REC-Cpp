#!/bin/bash
try() {
    input="$1"
    echo "input : $input"
    echo -e -n 'result: \e[31m';./mycc.out "$input";res="$?";echo -e -n '\e[m'
    if [ $res = 0 ]; then
	expected="$2"
	gcc -o tmp.out tmp.s
	./tmp.out
	actual=$?
	if [ "$actual" = "$expected" ]; then
	    echo "got $actual"
	else
	    echo -e "\e[31mexpected $expected, but got $actual\e[m"
	fi
    fi
    echo
}
try_link() {
    input="$1"
    echo "input : $input"
    echo -e -n 'result: \e[31m';./mycc.out "$input";res="$?";echo -e -n '\e[m'
    if [ $res = 0 ]; then
	gcc -o tmp.out tmp.s func.c
	./tmp.out
    fi
    echo
}
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
try '((1+2)*3);' 9
try '+((1+2)*3);' 9
try '+(2);' 2
try '+(2+1);' 3
try '+(1*2);' 2
try '-(-3-2+-3);' 8
try 'char a;a=4;a/(1+1);' 2
try 'char a;a=53;a%(1+8);' 8
try 'char a=2;a;' 2
try 'char a=+2;a;' 2
try 'char a=+(2);a;' 2
try 'char a=+23;char b=a*-2;b+(+47);' 1
try 'char a=1;char bc=1;a==bc;' 1
try 'char _A=2;char b=2;_A!=b;' 0
try 'char a=0;char _bc=2;a>_bc;' 0
try 'char a=23;char Bc=33;a<Bc;' 1
try 'char bc_sas232332e____as=21;bc_sas232332e____as*2+3;' 45
try 'char _A=5;char _Bc=5;_A*2>=_Bc*2;' 1
try 'char A__=2;char b_c=99;A__*300<=b_c;' 0
try 'char a=2;a+=3;' 5
try 'char a=2,b=3,c=2;a+=b+=c;' 7
try 'char a=24;a-=13;' 11
try 'char a=31,b=12,c=9;a-=b-=c;' 28
try 'char a=12,b=9;a*=b;' 108
try 'char a=12;a/=(2+4);' 2
try 'char a=29;a%=9;' 2
try 'char a=29;a+=3;char b=5;b*=a;char c=180;c-=b;char d=100;d/=c;char e=173;e%=d;' 3
try 'char a=23;++a;++a;char b=++a;b;' 26
try 'char a=32;--a;--a;char b=--a;b;' 29
try 'char a=32;char b=++a;++b;++a;char c=(a+b);--c;' 67
try 'char a=32;char b=2;{b=a;}b;' 32
try 'char a=32;char b=2;{char c=a;{b+=32;c+=b;}++c;a+=c;}a+b;' 133
try \
'char a=32;'\
'char b=46;'\
'++b;'\
'if(b>a){'\
'    char c=a+b;'\
'    c*=2;'\
'    if(c<100)'\
'        ++c;'\
'    else'\
'        c+=5;'\
'    c+=3;'\
'    b=c;'\
'}'\
'a=b;' 166
try \
'char a=10;'\
'char b=5;'\
'while(b>0){'\
'    ++a;'\
'    --b;'\
'}'\
'a;' 15
try \
'char a=2;'\
'char i;'\
'for(i=0;i<10;++i){'\
'    ++a;'\
'}'\
'a;' 12
try \
'char i=0;'\
'for(;i<10;++i);'\
'i;' 10
try \
'char a;'\
'for   (   a  =  0   ;   a  <  10    ;     )     ++a;'\
'a;' 10
try \
';char a=10;'\
'{'\
';'\
'}a;' 10
try_link 'char say_OK;say_OK();'
try_link 'char say_OK;char a=10,b=2,c=3;;say_OK();'
try_link 'char print_6num;char a=21,b=4,c=8,d=a+b*c,e=12,f=e;print_6num(a,b,c,d,e,f);'
try_link 'char print_12num;char a=21,b=4,c=8,d=a+b*c,e=12,f=e,g=32,h=21,i=g+h,j=21,k=a,l=0;print_12num(a,b,c,d,e,f,g,h,i,j,k,l);'
#__CO__
