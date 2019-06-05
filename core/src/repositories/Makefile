CXX ?= g++

### Path
SRC_PATH = .
BUILD_PATH = build
BIN_PATH = bin

### Executable
BIN_NAME = main

### Extensions
SRC_EXT = cc

### Code Lists
# Find all source files in the source directory, sorted by
# most recently modified
SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' -printf '%T@ %p\n' | sort -k 1nr | cut -d ' ' -f 2)
#SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' | sort -k 1nr | cut -f2-)
# Set the object file names, with the source directory stripped
# from the path, and the build path prepended in its place
OBJECTS = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)
# Set the dependency files that will be used to add header dependencies
DEPS = $(OBJECTS:.o=.d)


H_DIR = $(SRC_PATH)/header/
M_DIR = $(SRC_PATH)/../message/header/
I_DIR = $(SRC_PATH)/../message/identifiers/
UH_DIR = $(SRC_PATH)/../unixdomain/header/
INCLUDES = -I $(H_DIR)


### Build flags for all targets
CFLAGS    = -std=c++14
CXXFLAGS    = -std=c++14 -Wall -Iheader -I$(M_DIR) -Iheader -I$(I_DIR) -I$(UH_DIR)
CXXUNIXFLAGS = -std=c++14 -Wall -I$(M_DIR) -Iheader -I$(I_DIR) -I$(UH_DIR)

# Space separated pkg-config libraries used by this project
LIBS =

### Build tools
CC          = g++
#COMP        = $(CC) $(CFLAGS) -o $@ -c $<
#LINK        = $(CC) $(CFLAGS) -o $@ $^
#COMPLINK    = $(CC) $(CFLAGS) -o $@ $<

spencer: messagebuilderinterface.o messagebuilder.o keyvaluepaircontainerinterface.o keyvaluepaircontainer.o keyintpair.o keyfloatpair.o key.o repo_test.o message.o
	$(CC) repo_test.o messagebuilderinterface.o messagebuilder.o keyvaluepaircontainerinterface.o keyvaluepaircontainer.o keyintpair.o keyfloatpair.o key.o message.o -o rt

unixserver.o: $(SRC_PATH)/../unixdomain/cc/UnixDomainStreamSocketServer.cc unixsocket.o
	$(CC) $(CXXUNIXFLAGS) -c $(SRC_PATH)/../unixdomain/cc/UnixDomainStreamSocketServer.cc unixsocket.o -o unixserver.o

unixsocket.o: $(SRC_PATH)/../unixdomain/cc/UnixDomainStreamSocket.cc
	$(CC) $(CXXUNIXFLAGS) -c $(SRC_PATH)/../unixdomain/cc/UnixDomainStreamSocket.cc -o unixsocket.o

repository.o: cc/Repository.cc unixserver.o
	$(CC) $(CXXUNIXFLAGS) -c $(SRC_PATH)/cc/Repository.cc unixserver.o -o repository.o

powerrepository.o: cc/PowerRepository.cc repository.o
	$(CC) $(CXXUNIXFLAGS) -c $(SRC_PATH)/cc/PowerRepository.cc repository.o -o powerrepository.o

messagebuilderinterface.o: $(SRC_PATH)/../message/cc/MessageBuilderInterface.cc
	$(CC) $(CXXFLAGS) -c $(SRC_PATH)/../message/cc/MessageBuilderInterface.cc

messagebuilder.o: $(SRC_PATH)/../message/cc/MessageBuilder.cc
	$(CC) $(CXXFLAGS) -c $(SRC_PATH)/../message/cc/MessageBuilder.cc

keyvaluepaircontainerinterface.o: $(SRC_PATH)/../message/cc/keyvaluepaircontainerinterface.cc
	$(CC) $(CXXFLAGS) -c $(SRC_PATH)/../message/cc/keyvaluepaircontainerinterface.cc

keyvaluepaircontainer.o: $(SRC_PATH)/../message/cc/KeyValuePairContainer.cc
	$(CC) $(CXXFLAGS) -c $(SRC_PATH)/../message/cc/KeyValuePairContainer.cc

keyintpair.o: $(SRC_PATH)/../message/cc/KeyIntPair.cc
	$(CC) $(CXXFLAGS) -c $(SRC_PATH)/../message/cc/KeyIntPair.cc

keyfloatpair.o: $(SRC_PATH)/../message/cc/keyFloatPair.cc
	$(CC) $(CXXFLAGS) -c $(SRC_PATH)/../message/cc/keyFloatPair.cc

key.o: $(SRC_PATH)/../message/cc/Key.cc
	$(CC) $(CXXFLAGS) -c $(SRC_PATH)/../message/cc/Key.cc

repo_test.o: repo_test.cc powerrepository.o 
	$(CC) $(CXXUNIXFLAGS) -c repo_test.cc powerrepository.o -o repo_test.o

message.o: $(SRC_PATH)/../message/cc/Message.cc
	$(CC) $(CXXFLAGS) -c $(SRC_PATH)/../message/cc/Message.cc

clean:
	rm -rf *o rt

.PHONY: default_target
default_target: release

.PHONY: release
release: export CXXFLAGS := $(CXXFLAGS) $(COMPILE_FLAGS)
release: dirs
	@$(MAKE) all

.PHONY: dirs
dirs:
	@echo "Creating directories"
	@mkdir -p $(dir $(OBJECTS))
	@mkdir -p $(BIN_PATH)

# .PHONY: clean
# clean:
# 	@echo "Deleting $(BIN_NAME) symlink"
# 	@$(RM) $(BIN_NAME)
# 	@echo "Deleting directories"
# 	@$(RM) -r $(BUILD_PATH)
# 	@$(RM) -r $(BIN_PATH)


.PHONY: all
all: $(BIN_PATH)/$(BIN_NAME)
	@echo "Making symlink: $(BIN_NAME) -> $<"
	@$(RM) $(BIN_NAME)
	@ln -s $(BIN_PATH)/$(BIN_NAME) $(BIN_NAME)

# Creation of the executable
$(BIN_PATH)/$(BIN_NAME): $(OBJECTS)
	@echo "Linking: $@"
	$(CXX) $(OBJECTS) -o $@

# Add dependency files, if they exist
-include $(DEPS)

# Source file rules
# After the first compilation they will be joined with the rules from the
# dependency files to provide header dependencies
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@echo "Compiling: $< -> $@"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MP -MMD -c $< -o $@


### Standard parts
#program		:= output
#extra_clean	:=

### Include files
#all:

#include src/terminal/header/module.mk

### General rules
#%.o:	%.cpp
#	$(COMP)

### Rules
#.PHONY: all
#all:	$(program)

#.PHONY: clean
#clean:
#	rm $(program) $(extra_clean)

### Desired program output
#output: obj/terminal_test.o
#	$(LINK)
