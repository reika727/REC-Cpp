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
#<< "#__CO__"
try ');'
try 'a!;'
try 'a?v;'
try 'a'
try 'a=+(1+2)*3);'
try 'a+2=3;'
try '12adada=13;'
try 'a=2;a+=(2+2'
try 'a=2;a+return 3;'
try 'a=2;{2+2;'
#__CO__
#<<"#__CO__"
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
try 'bc_sas232332e____as=21;bc_sas232332e____as*2+3;' 45
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
try 'a=32;b=2;{b=a;}b;' 32
try 'a=32;b=2;{c=a;{b+=32;c+=b;}++c;a+=c;}a+b;' 133
try \
'a=32;'\
'b=46;'\
'++b;'\
'if(b>a){'\
'    c=a+b;'\
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
'a=10;'\
'b=5;'\
'while(b>0){'\
'++a;'\
'--b;'\
'}'\
'a;' 15
try \
'a=2;'\
'i;'\
'for(i=0;i<10;++i){'\
'    ++a;'\
'}'\
'a;' 12
try \
'i=0;'\
'for(;i<10;++i);'\
'i;' 10
try \
'a;'\
'for   (   a  =  0   ;   a  <  10    ;     )     ++a;'\
'a;' 10
try \
';a=10;'\
'{'\
';'\
'}' 10
try_link 'a=10;b=2;c=3;;say_OK();'
try_link 'a=21;b=4;c=8;d=a+b*c;e=12;f=e;print_6num(a,b,c,d,e,f);'
try_link 'a=21;b=4;c=8;d=a+b*c;e=12;f=e;g=32;h=21;i=g+h;j=21;k=a;l=0;print_12num(a,b,c,d,e,f,g,h,i,j,k,l);'
#__CO__
