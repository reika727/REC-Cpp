PROG=mycc.out
SRCS=$(wildcard source/*.cpp)
OBJS=$(SRCS:%.cpp=%.o)
DEPS=$(SRCS:%.cpp=%.d)
CXXFLAGS= -MMD -MP -std=c++17

all: $(PROG)

-include $(DEPS)

$(PROG): $(OBJS)
	$(CXX) -o $@ $^

%(OBJS):

clean:
	rm -f *.out *~ tmp* .*.swp source/*.o source/*.d

test: $(PROG)
	./test.sh
