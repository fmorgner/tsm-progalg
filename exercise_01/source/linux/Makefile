CXXSTDV:=-std=c++11
EXENAME:=exercise01
CXXLIBS:=-lpthread -lfreeimageplus
SOURCES:=main.cpp summation.cpp imageprocessing.cpp

BUILDDIR:=build
DEPDIR:=$(BUILDDIR)/.dep
OBJDIR:=$(BUILDDIR)/.obj
BINDIR:=$(BUILDDIR)/bin

$(shell mkdir -p $(DEPDIR) >/dev/null)
$(shell mkdir -p $(OBJDIR) >/dev/null)
$(shell mkdir -p $(BINDIR) >/dev/null)

DEPFLAGS=-MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td
CXXFLAGS=$(CXXSTDV) -Wall -Wextra -pedantic -Iinc
CCOMMAND=$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
PCOMMAND=mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

OBJS:=$(patsubst %,$(OBJDIR)/%,$(SOURCES:.cpp=.o))

.PHONY: all clean

all: $(BINDIR)/$(EXENAME)

clean:
	@rm -f $(OBJDIR)/* bin/* $(DEPDIR)/*

$(OBJDIR)/%.o: src/%.cpp
$(OBJDIR)/%.o: src/%.cpp $(DEPDIR)/%.d
	@echo "CXX $<"
	@$(CCOMMAND) $(OUTPUT_OPTION) $<
	@$(PCOMMAND)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

$(BINDIR)/$(EXENAME): $(OBJS)
	@echo "LD $@"
	@$(CXX) -o $(BINDIR)/$(EXENAME) $(CXXSTDV) $(CXXLIBS) $^

-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SOURCES)))
