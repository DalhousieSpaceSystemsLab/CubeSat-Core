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
UH_DIR = $(SRC_PATH)/../unixdomain/header/
INCLUDES = -I $(H_DIR)


### Build flags for all targets
CFLAGS    = -std=c++14
CXXFLAGS    = -std=c++14 -Wall -Iheader
CXXUNIXFLAGS = -std=c++14 -Wall -I$(UH_DIR) -Iheader -Iidentifiers

# Space separated pkg-config libraries used by this project
LIBS =

### Build tools
CC          = g++
#COMP        = $(CC) $(CFLAGS) -o $@ -c $<
#LINK        = $(CC) $(CFLAGS) -o $@ $^
#COMPLINK    = $(CC) $(CFLAGS) -o $@ $<

spencer: messagebuilderinterface.o messagebuilder.o keyvaluepaircontainerinterface.o keyvaluepaircontainer.o keyintpair.o keyfloatpair.o key.o message_test.o message.o
	$(CC) message_test.o messagebuilderinterface.o messagebuilder.o keyvaluepaircontainerinterface.o keyvaluepaircontainer.o keyintpair.o keyfloatpair.o key.o message.o -o mt

# unixsever.o: ../unixdomain/cc/UnixDomainStreamSocketServer.cc
# 	$(CC) $(CXXFLAGS) -c ../unixdomain/cc/UnixDomainStreamSocketServer.cc

unixsocket.o: $(SRC_PATH)/../unixdomain/cc/UnixDomainStreamSocket.cc
	$(CC) $(CXXUNIXFLAGS) -c ../unixdomain/cc/UnixDomainStreamSocket.cc -o unixsocket.o

unixclient.o: $(SRC_PATH)/../unixdomain/cc/UnixDomainStreamSocketClient.cc unixsocket.o
	$(CC) $(CXXUNIXFLAGS) -c ../unixdomain/cc/UnixDomainStreamSocketClient.cc unixsocket.o -o unixclient.o

messagingservice.o: cc/MessagingService.cc unixclient.o
	$(CC) $(CXXUNIXFLAGS) -c cc/MessagingService.cc unixclient.o -o messagingservice.o

messagesenderinterface.o: cc/MessageSenderInterface.cc messagingservice.o
	$(CC) $(CXXUNIXFLAGS) -c cc/MessageSenderInterface.cc messagingservice.o -o messagesenderinterface.o

messagebuilderinterface.o: cc/MessageBuilderInterface.cc
	$(CC) $(CXXFLAGS) -c cc/MessageBuilderInterface.cc

messagebuilder.o: cc/MessageBuilder.cc
	$(CC) $(CXXFLAGS) -c cc/MessageBuilder.cc

keyvaluepaircontainerinterface.o: cc/keyvaluepaircontainerinterface.cc
	$(CC) $(CXXFLAGS) -c cc/keyvaluepaircontainerinterface.cc


keyvaluepaircontainer.o: cc/KeyValuePairContainer.cc
	$(CC) $(CXXFLAGS) -c cc/KeyValuePairContainer.cc

keyintpair.o: cc/KeyIntPair.cc
	$(CC) $(CXXFLAGS) -c cc/KeyIntPair.cc

keyfloatpair.o: cc/keyFloatPair.cc
	$(CC) $(CXXFLAGS) -c cc/keyFloatPair.cc

key.o: cc/Key.cc
	$(CC) $(CXXFLAGS) -c cc/Key.cc

message_test.o: message_test.cc messagesenderinterface.o 
	$(CC) $(CXXUNIXFLAGS) -c message_test.cc messagesenderinterface.o -o message_test.o

message.o: cc/Message.cc
	$(CC) $(CXXFLAGS) -c cc/Message.cc

clean:
	rm -rf *o mt

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
