mycc: mycc.cpp

test: mycc
	./test.sh
clean:
	rm -f mycc mycc.out *.o *~ tmp*
