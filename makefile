SRCDIR=src
OBJDIR=obj
DEPDIR=dep
TRGDIR=bin
BUILTIN=builtin
TESTDIR=test
SRCS=$(wildcard $(SRCDIR)/*.cpp)
OBJS=$(addprefix $(OBJDIR)/,$(notdir $(SRCS:.cpp=.o)))
DEPS=$(addprefix $(DEPDIR)/,$(notdir $(SRCS:.cpp=.d)))
TARGET=$(TRGDIR)/rec.out

.PHONY: all
all: $(TARGET)

-include $(DEPS)

$(TARGET): $(OBJS)
	@mkdir -p $(TRGDIR)
	$(CXX) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	@mkdir -p $(DEPDIR)
	$(CXX) $< -c -o $@ -MMD -MP -MF $(DEPDIR)/$(*F).d -std=c++17

.PHONY: test
test: $(TARGET)
	@mkdir -p $(TESTDIR)
	$< test.c $(TESTDIR)/test.s
	as $(TESTDIR)/test.s $(BUILTIN)/*.s -o $(TESTDIR)/test.o
	ld $(TESTDIR)/test.o -o $(TESTDIR)/test.out
	./$(TESTDIR)/test.out

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(DEPDIR) $(TRGDIR) $(TESTDIR)
