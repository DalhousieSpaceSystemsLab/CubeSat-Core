### Build flags for all targets
CFLAGS    = -std=c++17

### Build tools
CC          = g++
COMP        = $(CC) $(CFLAGS) -o $@ -c $<
LINK        = $(CC) $(CFLAGS) -o $@ $^ 
COMPLINK    = $(CC) $(CFLAGS) -o $@ $< 

### Standard parts
program		:= output
extra_clean	:=

### Include files
all:

include src/terminal/header/module.mk

### General rules
%.o:	%.cpp
	$(COMP)

### Rules
.PHONY: all
all:	$(program)

.PHONY: clean
clean:
	rm $(extra_clean)

### Desired program output
output: obj/terminal_test.o
	$(LINK)