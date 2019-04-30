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
try '_A=2;b=2;_A!=b;' 0
try 'a=0;_bc=2;a>_bc;' 0
try 'a=23;Bc=33;a<Bc;' 1
try 'bc_sas232332e____as=21;bc_sas232332e____as*2+3;' 45
try '_A=5;_Bc=5;_A*2>=_Bc*2;' 1
try 'A__=2;b_c=99;A__*300<=b_c;' 0
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
