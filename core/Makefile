.DEFAULT_GOAL := release

SRCDIR := src
OBJDIR := build
DEPDIR := .d
BINDIR := bin
INCDIR := inc

####### Dep Stuff
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td
POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@########

EXC := main

SRC := $(shell find $(SRCDIR) -name '*.cc' -not -path "*test/*" -not -path "*emulation/*" -not -path "*unixdomain/Serialize/*" -not -path "*test/")
DEP := $(shell find $(SRCDIR) -name '*.h')
vpath %.cc $(sort $(dir $(SRC)))
SRC := $(notdir $(SRC))
OBJ := $(addprefix $(OBJDIR)/, $(SRC:.cc=.o))

CXX ?= g++
CXXFLAGS ?= -std=c++14 $(DEPFLAGS) -I $(INCDIR) -I src/unixdomain/cc/

###################
.PHONY: test
test:
	@echo $(SRC)
	@echo
	@echo $(OBJ)
	@echo
	@echo $(dir $(DEP))

.PHONY: release
release: dirs $(BINDIR)/$(EXC)
	@echo "Making symlink: $(EXC) -> $<"
	@$(RM) $(EXC)
	@ln -s $(BINDIR)/$(EXC) $(EXC)
	@echo "Build complete"

.PHONY: dirs
dirs:
	@echo "Creating directories"
	@echo
	$(shell mkdir -p $(OBJDIR) >/dev/null)
	$(shell mkdir -p $(DEPDIR) >/dev/null)
	$(shell mkdir -p $(BINDIR) >/dev/null)
	$(shell mkdir -p $(INCDIR) >/dev/null)
	$(shell cp $(DEP) $(INCDIR))

.PHONY: clean
clean: 
	@echo "Deleting $(EXC) symlink"
	@$(RM) $(EXC)
	@echo "Deleting directories"
	@$(RM) -r $(OBJDIR)
	@$(RM) -r $(BINDIR)
	@$(RM) -r $(DEPDIR)
	@$(RM) -r $(INCDIR)

# Creation of the executable 
$(BINDIR)/$(EXC): $(OBJ)
	@echo "Linking: $@"
	
	$(CXX) $(OBJ) -o $@

# Source file rules
# After the first compilation they will be joined with the rules from the
# dependency files to provide header dependencies
$(OBJDIR)/%.o: %.cc
$(OBJDIR)/%.o: %.cc $(DEPDIR)/%.d
	@echo "Compiling: $< -> $@"
	$(CXX) $(CXXFLAGS) -c $< -o $@
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS))))
