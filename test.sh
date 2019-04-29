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
try 'a=1;bc=1;a==bc;' 1
try 'a=0;bc=2;a>bc;' 0
try 'a=23;bc=33;a<bc;' 1
try 'a=21;bc=21;a!=b;' 0
try 'a=5;bc=5;a*2>=b*2;' 1
try 'a=2;bc=99;a*300<=bc;' 0
try \
'a=10;'\
'x=16;'\
'foo=5;'\
'bar=2;'\
'baz=(foo!=bar)+1;'\
'hoge=foo*(foo-bar)*4/baz;'\
'hoge=hoge+x-a+(x>a);'\
'hoge;' 37

echo OK
