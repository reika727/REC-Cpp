mycc: mycc.cpp

test: mycc
	./test.sh
clean:
	rm -f mycc *.o *~ tmp*
