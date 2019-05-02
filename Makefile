CFLAGS=-Wall
SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)

mycc: $(OBJS)
	g++ -o mycc $(OBJS) $(CFLAGS)
$(OBJS): mycc.hpp

test: mycc
	./test.sh
clean:
	rm -f mycc mycc.out *.o *~ tmp* .*.swp
