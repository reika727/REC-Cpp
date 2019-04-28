#!/bin/bash
try() {
    input=$1
    expected=$(($input))
    ./mycc "$input" > tmp.s
    gcc -o tmp tmp.s
    ./tmp
    actual=$?
    if [ $actual = $expected ]; then
	echo "$input = $actual"
    else
	echo "expected $expected, but got $actual"
	exit 1
    fi
}
try '0'
try '42'
try '5+20-4'
try ' 12 + 34 - 5 '
try '5+6*7'
try '5*(9-6)'
try '(3+5)/2'
try '(-3+2+2*(3-2+9)+(2+4+5)*(8/2*3)+(4+4*4)/(3+2))+(-(11-2))'
try '+4+-2'
try '1<2'
try '2<=2'
try '3>1'
try '3>=2'
echo OK
