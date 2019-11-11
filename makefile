SRCDIR=src
OBJDIR=obj
DEPDIR=dep
TRGDIR=bin
SRCS  =$(wildcard $(SRCDIR)/*.cpp)
OBJS  =$(addprefix $(OBJDIR)/,$(notdir $(SRCS:.cpp=.o)))
DEPS  =$(addprefix $(DEPDIR)/,$(notdir $(SRCS:.cpp=.d)))
TARGET=$(TRGDIR)/rec.out

all: $(TARGET)

-include $(DEPS)

$(TARGET): $(OBJS)
	@mkdir -p $(TRGDIR)
	$(CXX) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	@mkdir -p $(DEPDIR)
	$(CXX) $< -c -MMD -MP -o $@ -MF $(DEPDIR)/$(notdir $*).d

clean:
	rm -rf $(OBJDIR) $(DEPDIR) $(TRGDIR)

.PHONY: all clean
