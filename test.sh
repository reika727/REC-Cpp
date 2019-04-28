#!/bin/bash
try() {
    input=$1
    expected=$2
    ./mycc "$input" > tmp.s
    gcc -o tmp tmp.s
    ./tmp
    actual=$?
    if [ $actual = $expected ]; then
	echo "$input => $actual"
    else
	echo "expected $expected, but got $actual"
	exit 1
    fi
}
try '0;' 0
try '42;' 42
try '5+20-4;' 21
try ' 12 + 34 - 5 ;' 41
try '5+6*7;' 47
try '5*(9-6);' 15
try '(3+5)/2;' 4
try '(-3+2+2*(3-2+9)+(2+4+5)*(8/2*3)+(4+4*4)/(3+2))+(-(11-2));' 146
try '+4+-2;' 2
try '1<2;' 1
try '2<=2;' 1
try '3>1;' 1
try '3>=2;' 1
try 'a=2;' 2
try 'a=b=2;' 2
try 'a=2;b=3;b;' 3
try 'a=2;b=3;c=(a+a+a)/b*2;d=(c+a)*2;a=d*2;' 24
try 'a=2;b=4-a;' 2
try 'a=2;b=3;return (a+b)*3-2;return a;' 13
echo OK
