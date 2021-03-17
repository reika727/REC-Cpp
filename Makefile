SRCDIR  := src
OBJDIR  := obj
DEPDIR  := dep
TRGDIR  := bin
TESTDIR := test
SRCS    := $(wildcard $(SRCDIR)/*.cpp)
OBJS    := $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.cpp=.o)))
DEPS    := $(addprefix $(DEPDIR)/, $(notdir $(SRCS:.cpp=.d)))
TARGET  := $(TRGDIR)/rec

export TARGET

.PHONY: all
all: $(TARGET)

-include $(DEPS)

$(TARGET): $(OBJS)
	@mkdir -p $(TRGDIR)
	$(CXX) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	@mkdir -p $(DEPDIR)
	$(CXX) $< -c -Wall -Wextra -std=c++17 -o $@ -MMD -MP -MF $(DEPDIR)/$(*F).d

.PHONY: test
test: $(TARGET)
	$(MAKE) -C $(TESTDIR) $@

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(DEPDIR) $(TRGDIR)
	$(MAKE) -C $(TESTDIR) $@
